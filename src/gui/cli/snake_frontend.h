// snake_frontend.h
#pragma once

#include <ncurses.h>

#include "./../../brick_game/snake/backend.h"

namespace s21 {

class SnakeFrontend {
 public:
  SnakeFrontend(SnakeModel&);
  virtual ~SnakeFrontend();

  void RenderGame();
  void HandleInput(int ch);

 private:
  SnakeModel& model_;
};

}  // namespace s21
