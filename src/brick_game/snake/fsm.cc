/**
 * @file
 * @brief Реализация ViewModel игры Змейка (MVVM: ViewModel).
 *
 * Содержит реализацию конечного автомата (FSM).
 */

#include "fsm.h"

#include "backend.h" // Полное объявление SnakeModel для реализации методов

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
 * Реализует автоматические переходы: MOVING → SHIFTING →
 * MOVING/COLLIDE/GAMEOVER.
 */
void SnakeController::Tick() {
  if (state_ == SnakeState::MOVING && IsTimerExpired()) {
    DoShifting();
  }
}

int SnakeController::GetPauseStatus() const { return pause_status_; }
SnakeState SnakeController::GetState() const { return state_; }

// Прокси-методы к данным модели для View
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

void SnakeController::DoSpawn() {
  state_ = SnakeState::SPAWN;
  pause_status_ = 0;
  ResetTimer();
  // Переход SPAWN -> MOVING (яблоко уже создано в Reset/конструкторе)
  state_ = SnakeState::MOVING;
}

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

void SnakeController::DoCollide() {
  state_ = SnakeState::COLLIDE;

  if (model_.HasWon()) {
    DoGameOver();
  } else {
    // COLLIDE -> MOVING (новое яблоко уже сгенерировано)
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

  long long diff = (current_time.tv_sec * 1000000000LL + current_time.tv_nsec) -
                   (last_tick_.tv_sec * 1000000000LL + last_tick_.tv_nsec);

  return diff >= model_.GetSpeed();
}

void SnakeController::ResetTimer() { timespec_get(&last_tick_, TIME_UTC); }

} // namespace s21