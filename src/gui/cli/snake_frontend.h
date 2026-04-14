#pragma once

#include <ncurses.h>

#include "./../../brick_game/snake/fsm.h"
#include "frontend.h" // для print_rectangle и т.д.

namespace s21 {

/**
 * @brief Класс для отрисовки игры (View).
 * Работает исключительно через SnakeController (ViewModel).
 */
class SnakeFrontend {
public:
  explicit SnakeFrontend(SnakeController &controller);
  virtual ~SnakeFrontend();

  void RenderGame();
  void DrawOverlay(); // рамка + HUD

private:
  SnakeController &controller_;

  // Константы для змейки (аналогично тетрису)
  static constexpr int kFieldHeight = 20;
  static constexpr int kFieldWidth = 10;
  static constexpr int kHudWidth = 11;

  void DrawField();  // очистка поля
  void DrawSnake();  // змейка
  void DrawApple();  // яблоко
  void DrawStats();  // статистика
  void DrawStatus(); // START/PAUSE/GAMEOVER
};

/**
 * @brief Класс для управления циклом событий ncurses.
 */
class SnakeView {
public:
  explicit SnakeView(SnakeController &controller);
  void startEventLoop();

private:
  SnakeController &controller_;
};

} // namespace s21
