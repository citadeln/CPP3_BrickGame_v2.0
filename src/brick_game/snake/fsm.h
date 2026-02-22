/**
 * @file
 * @brief Заголовочный файл функций конечного автомата.
 */

#pragma once

#include "backend.h"

namespace s21 {

class SnakeController {
 public:
  explicit SnakeController(SnakeModel& model);
  ~SnakeController();

  void ProcessUserInput(char input);
  void UpdateGameState();

 private:
  SnakeModel& model_;
};

}  // namespace s21
