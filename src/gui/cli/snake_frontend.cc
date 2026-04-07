/**
 * @file
 * @brief Реализация консольного интерфейса Змейки (View) на ncurses.
 */

#include "snake_frontend.h"

#include <thread>
#include <chrono>

namespace s21 {

// ─────────────────────────────────────────────────────────────────────────────
// SnakeFrontend
// ─────────────────────────────────────────────────────────────────────────────

SnakeFrontend::SnakeFrontend(SnakeController& controller)
    : controller_(controller) {}

SnakeFrontend::~SnakeFrontend() {}

void SnakeFrontend::RenderGame() {
  clear();  // очищаем экран

  int pause_status = controller_.GetPauseStatus();
  SnakeState state = controller_.GetState();

  // Статусные сообщения
  if (state == SnakeState::START) {
    mvprintw(5, 5, "Press Enter to start");
  } else if (pause_status == 1) {
    mvprintw(5, 5, "PAUSE (press p to continue)");
  } else if (pause_status == 3) {
    mvprintw(5, 5, "GAME OVER (press Enter to restart)");
  }

  // Змейка
  for (const auto& pos : controller_.GetSnakePositions()) {
    mvaddch(pos.first + 1, pos.second + 1, '@');
  }

  // Яблоко
  auto apple = controller_.GetApplePosition();
  mvaddch(apple.first + 1, apple.second + 1, '*');

  // HUD
  mvprintw(0, 0, "Score: %d | Hi: %d | Level: %d",
           controller_.GetScore(),
           controller_.GetHighScore(),
           controller_.GetLevel());

  refresh();
}


// ─────────────────────────────────────────────────────────────────────────────
// SnakeView
// ─────────────────────────────────────────────────────────────────────────────

SnakeView::SnakeView(SnakeController& controller)
    : controller_(controller) {}

void SnakeView::startEventLoop() {
  SnakeFrontend frontend(controller_);

  frontend.RenderGame();

  while (controller_.GetPauseStatus() != -1) {  // -1 = EXIT_STATE
    int ch = getch();

    if (ch != ERR) {
      switch (ch) {
        case KEY_UP:
          controller_.ProcessUserInput('U');
          break;
        case KEY_DOWN:
          controller_.ProcessUserInput('D');
          break;
        case KEY_LEFT:
          controller_.ProcessUserInput('L');
          break;
        case KEY_RIGHT:
          controller_.ProcessUserInput('R');
          break;
        case 'p':
          controller_.ProcessUserInput('p');
          break;
        case 'q':
          controller_.ProcessUserInput('q');
          break;
        case '\n':
          controller_.ProcessUserInput('\n');
          break;
        default:
          break;
      }
    }

    controller_.Tick();
    frontend.RenderGame();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

}  // namespace s21