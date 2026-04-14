/**
 * @file
 * @brief Заголовочный файл ViewModel игры Змейка (MVVM: ViewModel).
 *
 * SnakeController — модель представления (ViewModel).
 * Содержит конечный автомат (FSM), связывает Model и View.
 * View подписывается на состояние через GetPauseStatus() / GetState(),
 * отправляет команды через ProcessUserInput().
 */

#pragma once

#include <ctime>
#include <utility>
#include <vector>

namespace s21 {

// Forward declaration, чтобы разорвать циклическую зависимость с backend.h
class SnakeModel;

/**
 * @brief Состояния конечного автомата (FSM) Змейки.
 *
 * Соответствуют диаграмме FSM:
 * START → SPAWN → MOVING → SHIFTING → COLLIDE → SPAWN | GAMEOVER
 */
enum class SnakeState : int {
  START = 0, ///< Начальный экран (ожидание Enter)
  SPAWN,     ///< Инициализация / рестарт игры
  MOVING, ///< Движение: ожидание таймера и ввода пользователя
  SHIFTING, ///< Принудительный шаг вперёд (таймер сработал)
  COLLIDE, ///< Обработка столкновения (съедено яблоко)
  PAUSE,     ///< Пауза
  GAMEOVER,  ///< Конец игры
  EXIT_STATE ///< Выход из программы
};

/**
 * @brief ViewModel игры Змейка (MVVM: ViewModel).
 *
 * Реализует FSM (диаграмма состояний).
 * Служит абстракцией представления и обёрткой над данными Model.
 *
 * View вызывает:
 *  - ProcessUserInput() — отправка команды;
 *  - Tick()             — обновление состояния (вызов в цикле событий);
 *  - GetPauseStatus()   — получение статуса для отрисовки;
 *  - геттеры данных     — получение данных для рендеринга.
 */
class SnakeController {
public:
  explicit SnakeController(SnakeModel &model);
  ~SnakeController();

  // ─── Команды от View (двусторонняя привязка) ──────────────────────────────

  /**
   * @brief Обрабатывает ввод пользователя и изменяет состояние FSM / Model.
   *
   * @param input Символ команды: Enter, 'p', 'q', 'U', 'D', 'L', 'R'.
   */
  void ProcessUserInput(char input);

  /**
   * @brief Обновляет состояние FSM (проверка таймера, переходы).
   *
   * Вызывается из цикла событий View на каждой итерации.
   */
  void Tick();

  // ─── Запросы состояния для View ───────────────────────────────────────────

  /**
   * @brief Возвращает статус паузы для View (аналог GameInfo_t::pause).
   *
   * -1 — выход; 0 — игра идёт; 1 — пауза; 2 — старт; 3 — gameover.
   */
  int GetPauseStatus() const;

  SnakeState GetState() const;

  // Прокси к данным модели для View
  std::vector<std::pair<int, int>> GetSnakePositions() const;
  std::pair<int, int> GetApplePosition() const;
  int GetScore() const;
  int GetHighScore() const;
  int GetLevel() const;
  long long GetSpeed() const;

private:
  SnakeModel &model_;
  SnakeState state_{SnakeState::START};
  int pause_status_{2}; ///< -1/0/1/2/3 — статус для View
  struct timespec last_tick_ {};

  // ─── Переходы FSM ─────────────────────────────────────────────────────────

  void DoSpawn();
  void DoShifting();
  void DoCollide();
  void DoPause();
  void DoUnpause();
  void DoGameOver();
  void DoExit();

  /**
   * @brief Проверяет, истёк ли таймер автоматического шага.
   * @return true, если прошло >= speed_ наносекунд с последнего шага.
   */
  bool IsTimerExpired() const;

  /**
   * @brief Сбрасывает таймер автоматического шага.
   */
  void ResetTimer();
};

} // namespace s21