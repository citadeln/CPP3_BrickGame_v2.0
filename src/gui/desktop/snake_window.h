/**
 * @file
 * @brief Главное окно Qt для игры Змейка (MVVM: View / QMainWindow).
 *
 * SnakeWindow создаёт SnakeWidget и задаёт параметры окна.
 * Реализует метод startEventLoop(), аналогичный ncurses-версии,
 * для единообразия вызова из main_qt.cc.
 */

#pragma once

#include <QMainWindow>

#include "snake_widget.h"

namespace s21 {

/**
 * @brief Главное окно Qt-версии Змейки (MVVM: View / главное окно).
 */
class SnakeWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit SnakeWindow(SnakeController &controller, QWidget *parent = nullptr);
  ~SnakeWindow() override = default;

private:
  SnakeWidget *widget_; ///< Центральный виджет — игровое поле
};

} // namespace s21
