/**
 * @file
 * @brief Заголовочный файл основных функций игры.
 */

#pragma once

#include <random>
#include <vector>

namespace s21 {

class SnakeModel {
 public:
  SnakeModel();
  ~SnakeModel();

  /**
   * @brief Методы управления состоянием игры
   */
  void MoveForward();
  void ChangeDirection(char direction);
  bool IsGameOver() const;
  bool HasWon() const;
  void EatApple();
  void UpdateLevelAndSpeed();

  // Доступ к состоянию игры
  std::vector<std::pair<int, int>> GetSnakePositions() const;
  std::pair<int, int> GetApplePosition() const;
  int GetScore() const;
  int GetHighScore() const;
  int GetLevel() const;
  int GetSpeed() const;

 private:
  std::vector<std::pair<int, int>> snake_;
  std::pair<int, int> apple_pos_;
  int score_{0};
  int high_score_{0};
  int level_{1};
  int speed_{100};
  char dir_{'U'};  // Направление движения (Up, Down, Left, Right)

  void SpawnApple();
  void CheckCollision();
  void SaveHighScore();
};

}  // namespace s21