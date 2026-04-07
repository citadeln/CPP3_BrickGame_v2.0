#pragma once

#include <ncurses.h>
#include "./../../brick_game/snake/fsm.h"  // используем SnakeController

namespace s21 {

/**
 * @brief Класс для отрисовки игры (View).
 * Работает исключительно через SnakeController (ViewModel).
 */
class SnakeFrontend {
 public:
  explicit SnakeFrontend(SnakeController& controller);
  virtual ~SnakeFrontend();

  void RenderGame();

 private:
  SnakeController& controller_;  ///< Ссылка на ViewModel
};


/**
 * @brief Класс для управления циклом событий ncurses.
 */
class SnakeView {
 public:
  explicit SnakeView(SnakeController& controller);
  void startEventLoop();

 private:
  SnakeController& controller_;
};

}  // namespace s21