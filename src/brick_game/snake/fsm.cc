/**
 * @file
 * @brief Реализация ViewModel игры Змейка (MVVM: ViewModel).
 *
 * SnakeController реализует FSM согласно диаграмме:
 *
 * START → SPAWN → MOVING ──(timer)──► SHIFTING ──(no collide)──► MOVING
 *                                             └──(apple eaten)──► COLLIDE → SPAWN
 *                                             └──(wall/body)───► GAMEOVER
 * MOVING ──(p)──► PAUSE ──(p/Enter)──► MOVING
 * MOVING ──(q)──► EXIT_STATE
 * GAMEOVER ──(Enter)──► SPAWN
 * GAMEOVER ──(q)──────► EXIT_STATE
 */

#include "fsm.h"

namespace s21 {

// ─────────────────────────────────────────────────────────────────────────────
// Конструктор / деструктор
// ─────────────────────────────────────────────────────────────────────────────

SnakeController::SnakeController(SnakeModel &model)
    : model_(model), state_(SnakeState::START), pause_status_(2) {
  timespec_get(&last_tick_, TIME_UTC);
}

SnakeController::~SnakeController() {}

// ─────────────────────────────────────────────────────────────────────────────
// Публичный интерфейс (View вызывает эти методы)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Обрабатывает ввод пользователя согласно текущему состоянию FSM.
 *
 * View отправляет команду → ViewModel решает, как изменить Model / состояние.
 * Это реализует двустороннюю привязку: View → ViewModel → Model.
 */
void SnakeController::ProcessUserInput(char input) {
  switch (state_) {
    case SnakeState::START:
      if (input == '\n') {
        DoSpawn();
      } else if (input == 'q') {
        DoExit();
      }
      break;

    case SnakeState::MOVING:
      if (input == 'U' || input == 'D' || input == 'L' || input == 'R') {
        model_.ChangeDirection(input);
      } else if (input == 'p') {
        DoPause();
      } else if (input == 'q') {
        DoExit();
      }
      break;

    case SnakeState::PAUSE:
      if (input == 'p' || input == '\n') {
        DoUnpause();
      } else if (input == 'q') {
        DoExit();
      }
      break;

    case SnakeState::GAMEOVER:
      if (input == '\n') {
        model_.Reset();
        DoSpawn();
      } else if (input == 'q') {
        DoExit();
      }
      break;

    default:
      break;
  }
}

/**
 * @brief Обновляет состояние FSM.
 *
 * Вызывается из цикла событий View на каждой итерации.
 * Реализует автоматические переходы: MOVING → SHIFTING → MOVING/COLLIDE/GAMEOVER.
 */
void SnakeController::Tick() {
  if (state_ == SnakeState::MOVING && IsTimerExpired()) {
    DoShifting();
  }
}

int SnakeController::GetPauseStatus() const { return pause_status_; }

SnakeState SnakeController::GetState() const { return state_; }

std::vector<std::pair<int, int>> SnakeController::GetSnakePositions() const {
  return model_.GetSnakePositions();
}

std::pair<int, int> SnakeController::GetApplePosition() const {
  return model_.GetApplePosition();
}

int SnakeController::GetScore() const { return model_.GetScore(); }

int SnakeController::GetHighScore() const { return model_.GetHighScore(); }

int SnakeController::GetLevel() const { return model_.GetLevel(); }

long long SnakeController::GetSpeed() const { return model_.GetSpeed(); }

// ─────────────────────────────────────────────────────────────────────────────
// Переходы FSM (приватные)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief START/GAMEOVER → SPAWN → MOVING.
 *
 * Инициализирует новую игру и немедленно переходит в MOVING.
 */
void SnakeController::DoSpawn() {
  state_ = SnakeState::SPAWN;
  pause_status_ = 0;
  ResetTimer();
  // SPAWN → MOVING (яблоко уже создано в Reset/конструкторе)
  state_ = SnakeState::MOVING;
}

/**
 * @brief MOVING → SHIFTING: продвигает змейку на один шаг.
 *
 * После шага проверяет:
 *  - столкновение → GAMEOVER
 *  - поедание яблока + победа → GAMEOVER
 *  - поедание яблока → COLLIDE (обработка) → MOVING
 *  - нет событий → MOVING
 */
void SnakeController::DoShifting() {
  state_ = SnakeState::SHIFTING;
  model_.MoveForward();
  ResetTimer();

  if (model_.IsGameOver()) {
    DoGameOver();
    return;
  }

  if (model_.IsAppleEaten()) {
    DoCollide();
    return;
  }

  state_ = SnakeState::MOVING;
}

/**
 * @brief SHIFTING → COLLIDE: обработка поедания яблока.
 *
 * Яблоко уже съедено внутри MoveForward(), счёт/уровень/скорость обновлены,
 * новое яблоко сгенерировано. Проверяем условие победы.
 */
void SnakeController::DoCollide() {
  state_ = SnakeState::COLLIDE;

  if (model_.HasWon()) {
    DoGameOver();
  } else {
    // COLLIDE → SPAWN (новое яблоко уже сгенерировано) → MOVING
    state_ = SnakeState::MOVING;
  }
}

void SnakeController::DoPause() {
  state_ = SnakeState::PAUSE;
  pause_status_ = 1;
}

void SnakeController::DoUnpause() {
  pause_status_ = 0;
  ResetTimer();
  state_ = SnakeState::MOVING;
}

void SnakeController::DoGameOver() {
  state_ = SnakeState::GAMEOVER;
  pause_status_ = 3;
}

void SnakeController::DoExit() {
  state_ = SnakeState::EXIT_STATE;
  pause_status_ = -1;
}

// ─────────────────────────────────────────────────────────────────────────────
// Вспомогательные методы
// ─────────────────────────────────────────────────────────────────────────────

bool SnakeController::IsTimerExpired() const {
  struct timespec current_time;
  timespec_get(&current_time, TIME_UTC);

  long long diff =
      (current_time.tv_sec * 1000000000LL + current_time.tv_nsec) -
      (last_tick_.tv_sec * 1000000000LL + last_tick_.tv_nsec);

  return diff >= model_.GetSpeed();
}

void SnakeController::ResetTimer() {
  timespec_get(&last_tick_, TIME_UTC);
}

}  // namespace s21
