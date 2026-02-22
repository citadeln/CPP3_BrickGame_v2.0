// snake_frontend.h
#pragma once

#include <ncurses.h>

// #include "./../../brick_game/snake/backend.h"
#include "./../../brick_game/snake/fsm.h"

namespace s21 {

class SnakeFrontend {
 public:
  SnakeFrontend(SnakeModel& model);
  virtual ~SnakeFrontend();

  void RenderGame();
  void HandleInput(int ch);

 private:
  SnakeModel& model_;
};

class SnakeView {
 public:
  explicit SnakeView(SnakeController& controller);
  void startEventLoop();

 private:
  SnakeController& controller_;
};

}  // namespace s21
