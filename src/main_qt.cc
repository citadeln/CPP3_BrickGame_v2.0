/**
 * @file
 * @brief Точка входа Qt-версии BrickGame v2.0 (Tetris + Snake с Qt GUI).
 *
 * Порядок запуска:
 *   1. Показывается лаунчер — экран выбора игры.
 *   2. Пользователь выбирает Tetris или Snake — открывается игровое окно.
 *   3. По нажатию Q игровое окно закрывается и лаунчер появляется снова.
 *   4. «✕ / Выход» в лаунчере завершает приложение.
 *
 * Сборка:  make build-qt
 * Запуск:  ./brickgame-qt
 */

#include <QApplication>
#include <QDialog>
#include <QEventLoop>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

#include "brick_game/snake/backend.h"
#include "brick_game/snake/fsm.h"
#include "gui/desktop/snake_widget.h"
#include "gui/desktop/tetris_widget.h"

// ─────────────────────────────────────────────────────────────────────────────
// Лаунчер — экран выбора игры
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Показывает диалог выбора игры.
 * @return 1 — Snake, 2 — Tetris, 0 — выход / закрыто.
 */
static int showLauncher() {
  QDialog dlg;
  dlg.setWindowTitle("BrickGame v2.0");
  dlg.setFixedSize(300, 240);
  dlg.setStyleSheet(
      "QDialog  { background-color: #12121c; }"
      "QLabel   { color: #50e070; font-family: Monospace; font-size: 22px;"
      "           font-weight: bold; qproperty-alignment: AlignCenter; }"
      "QPushButton {"
      "    background-color: #20203a; color: #dde; border: 1px solid #404070;"
      "    font-family: Monospace; font-size: 14px; padding: 10px;"
      "    border-radius: 4px; }"
      "QPushButton:hover  { background-color: #2a2a50; border-color: #6060b0; }"
      "QPushButton:pressed{ background-color: #151530; }");

  auto *layout = new QVBoxLayout(&dlg);
  layout->setSpacing(12);
  layout->setContentsMargins(30, 25, 30, 25);

  auto *title = new QLabel("BRICK GAME\n  v2.0", &dlg);
  layout->addWidget(title);
  layout->addSpacing(8);

  int choice = 0;

  auto *btnSnake = new QPushButton("  SNAKE  (Змейка)", &dlg);
  auto *btnTetris = new QPushButton("  TETRIS (Тетрис)", &dlg);

  QObject::connect(btnSnake, &QPushButton::clicked, [&]() {
    choice = 1;
    dlg.accept();
  });
  QObject::connect(btnTetris, &QPushButton::clicked, [&]() {
    choice = 2;
    dlg.accept();
  });

  layout->addWidget(btnSnake);
  layout->addWidget(btnTetris);

  // Подсказка
  auto *hint = new QLabel("Enter — start  /  Q — quit", &dlg);
  hint->setStyleSheet("color: #505080; font-size: 10px;");
  layout->addWidget(hint);

  if (dlg.exec() != QDialog::Accepted) return 0;
  return choice;
}

// ─────────────────────────────────────────────────────────────────────────────
// Вспомогательная функция: запускает игровой виджет в окне и ждёт gameEnded()
// ─────────────────────────────────────────────────────────────────────────────

template <typename Widget>
static void runGame(Widget *widget, const QString &title) {
  QMainWindow win;
  win.setWindowTitle(title);
  win.setCentralWidget(widget);
  win.setFixedSize(widget->size());

  QEventLoop loop;
  QObject::connect(widget, &Widget::gameEnded, &loop, &QEventLoop::quit);

  widget->startGame();
  win.show();
  loop.exec();
  win.hide();

  // Отсоединяем виджет от окна, чтобы не удалять его вместе с win
  win.takeCentralWidget();
}

// ─────────────────────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────────────────────

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setApplicationName("BrickGame");
  app.setApplicationVersion("2.0");

  while (true) {
    int choice = showLauncher();
    if (choice == 0) break;

    if (choice == 1) {
      // ── Snake (MVVM) ────────────────────────────────────────────────────
      // Новая модель на каждый запуск игры — счёт и позиция сбрасываются,
      // рекорд сохраняется в файл через деструктор SnakeModel.
      s21::SnakeModel model;
      s21::SnakeController controller(model);
      s21::SnakeWidget widget(controller);

      runGame(&widget, "BrickGame v2.0 \xe2\x80\x94 Snake");

    } else {
      // ── Tetris (C API из BrickGame v1.0) ────────────────────────────────
      // Глобальное состояние тетриса управляется C API — reset при Enter.
      s21::TetrisWidget widget;

      runGame(&widget, "BrickGame v2.0 \xe2\x80\x94 Tetris");
    }
  }

  return 0;
}
