/**
 * @file
 * @brief Реализация главного окна Qt для игры Змейка (MVVM: View).
 */

#include "snake_window.h"

namespace s21 {

SnakeWindow::SnakeWindow(SnakeController &controller, QWidget *parent)
    : QMainWindow(parent) {
  setWindowTitle("BrickGame v2.0 — Snake");

  widget_ = new SnakeWidget(controller, this);
  setCentralWidget(widget_);

  // Окно фиксированного размера (по размеру виджета)
  setFixedSize(widget_->sizeHint());

  widget_->setFocus();
}

}  // namespace s21
