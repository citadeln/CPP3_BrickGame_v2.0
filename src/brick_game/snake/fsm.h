/**
 * @file
 * @brief Заголовочный файл функций конечного автомата.
 */

#include "backend.h"

namespace s21 {

SnakeController::SnakeController(SnakeModel& model) : model_(model) {}

SnakeController::~SnakeController() {}

void SnakeController::ProcessUserInput(char input) {
    // Обработчик действий пользователя
    model_.ChangeDirection(input);
}

void SnakeController::UpdateGameState() {
    // Обновление состояния игры
    model_.MoveForward();
}

}  // namespace s21