/**
 * @file
 * @brief Точка входа Qt-версии BrickGame (Змейка с GUI на Qt).
 *
 * Model и ViewModel не зависят от GUI-фреймворка —
 * это демонстрирует преимущество паттерна MVVM:
 * замена слоя View (ncurses → Qt) не затрагивает Model и ViewModel.
 *
 * Сборка:  make build-qt
 * Запуск:  ./brickgame-qt
 */

#include <QApplication>

#include "brick_game/snake/backend.h"
#include "brick_game/snake/fsm.h"
#include "gui/desktop/snake_window.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setApplicationName("BrickGame — Snake");
  app.setApplicationVersion("2.0");

  // ── MVVM: создаём независимые слои ──────────────────────────────────────
  s21::SnakeModel model; // Model:     бизнес-логика, данные
  s21::SnakeController controller(model); // ViewModel: FSM, привязка данных
  s21::SnakeWindow window(controller); // View:      Qt-окно

  window.show();
  return app.exec();
}
