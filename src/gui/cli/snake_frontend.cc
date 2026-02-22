// snake_frontend.cc
#include "snake_frontend.h"

namespace s21 {

SnakeFrontend::SnakeFrontend(SnakeModel& model) : model_(model) {}

SnakeFrontend::~SnakeFrontend() {}

void SnakeFrontend::RenderGame() {
  // Рендерим игровое поле и змейку
  // Например, используем стандартные примитивы curses
  for (const auto& pos : model_.GetSnakePositions()) {
    mvaddch(pos.first, pos.second, '@');
  }
  mvaddch(model_.GetApplePosition().first, model_.GetApplePosition().second,
          '*');
  refresh();
}

void SnakeFrontend::HandleInput(int ch) {
  // Обрабатываем пользовательский ввод
  if (ch == KEY_UP) {
    model_.ChangeDirection('U');
  } else if (ch == KEY_DOWN) {
    model_.ChangeDirection('D');
  } else if (ch == KEY_LEFT) {
    model_.ChangeDirection('L');
  } else if (ch == KEY_RIGHT) {
    model_.ChangeDirection('R');
  }
}

}  // namespace s21
