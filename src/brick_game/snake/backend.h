/**
 * @file
 * @brief Заголовочный файл модели игры Змейка (MVVM: Model).
 *
 * SnakeModel хранит всё состояние игры и реализует бизнес-логику.
 * Не зависит от View и ViewModel.
 */

#pragma once

#include <fstream>
#include <random>
#include <string>
#include <vector>

namespace s21 {

/**
 * @brief Модель игры Змейка (MVVM: Model).
 *
 * Отвечает за хранение и изменение игрового состояния:
 * позиции змейки, яблока, счёта, уровня и скорости.
 * Предоставляет методы-запросы (геттеры) и методы-команды.
 */
class SnakeModel {
 public:
  static constexpr int kFieldWidth = 20;
  static constexpr int kFieldHeight = 20;
  static constexpr int kWinLength = 200;
  static constexpr int kMaxLevel = 10;
  static constexpr long long kBaseSpeed = 500000000LL;  ///< 500 мс в нс
  static constexpr const char *kHiScoreFile = "hi-score-snake.txt";

  SnakeModel();
  ~SnakeModel();

  // ─── Команды (изменяют состояние) ─────────────────────────────────────────

  /**
   * @brief Сдвигает змейку на один шаг в текущем направлении.
   *
   * Обновляет направление из буфера, добавляет новую голову,
   * удаляет хвост (если не съедено яблоко), проверяет столкновения.
   * Устанавливает флаги game_over_ и apple_eaten_.
   */
  void MoveForward();

  /**
   * @brief Запрашивает смену направления движения.
   *
   * Изменение буферизируется в next_dir_ и применяется
   * при следующем вызове MoveForward(). Разворот на 180° игнорируется.
   *
   * @param direction Новое направление: 'U', 'D', 'L', 'R'.
   */
  void ChangeDirection(char direction);

  /**
   * @brief Сбрасывает модель в начальное состояние (новая игра).
   */
  void Reset();

  // ─── Запросы (только чтение) ──────────────────────────────────────────────

  bool IsGameOver() const;
  bool HasWon() const;

  /**
   * @brief Возвращает true, если за последний шаг было съедено яблоко.
   */
  bool IsAppleEaten() const;

  std::vector<std::pair<int, int>> GetSnakePositions() const;
  std::pair<int, int> GetApplePosition() const;
  int GetScore() const;
  int GetHighScore() const;
  int GetLevel() const;
  long long GetSpeed() const;
  char GetDirection() const;

 private:
  std::vector<std::pair<int, int>> snake_;  ///< {row, col}, голова — первый
  std::pair<int, int> apple_pos_{0, 0};
  int score_{0};
  int high_score_{0};
  int level_{1};
  long long speed_{kBaseSpeed};
  char dir_{'U'};  ///< Текущее направление движения
  char next_dir_{'U'};  ///< Буферизированное следующее направление
  bool game_over_{false};
  bool apple_eaten_{false};

  /**
   * @brief Проверяет, является ли направление противоположным текущему.
   */
  bool IsOppositeDirection(char dir) const;

  /**
   * @brief Генерирует позицию яблока, не занятую змейкой.
   */
  void SpawnApple();

  /**
   * @brief Обновляет score_, level_, speed_ после поедания яблока.
   * Сохраняет рекорд если побит.
   */
  void EatApple();

  /**
   * @brief Обновляет уровень и скорость по текущему счёту.
   */
  void UpdateLevelAndSpeed();

  /**
   * @brief Загружает рекорд из файла.
   */
  void LoadHighScore();

  /**
   * @brief Сохраняет рекорд в файл.
   */
  void SaveHighScore();
};

}  // namespace s21
