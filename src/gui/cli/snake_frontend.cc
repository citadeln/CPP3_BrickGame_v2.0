/**
 * @file
 * @brief Реализация консольного интерфейса Змейки (View) на ncurses.
 */

#include "snake_frontend.h"

#include <chrono>
#include <thread>

namespace s21 {

// ─────────────────────────────────────────────────────────────────────────────
// SnakeFrontend
// ─────────────────────────────────────────────────────────────────────────────

SnakeFrontend::SnakeFrontend(SnakeController& controller)
    : controller_(controller) {}

SnakeFrontend::~SnakeFrontend() {}

void SnakeFrontend::RenderGame() {
  // Рисуем статичную рамку + HUD один раз (если нужно)
  // DrawOverlay();  // вызывается один раз в startEventLoop

  DrawField();   // очищаем игровое поле
  DrawSnake();   // змейка
  DrawApple();   // яблоко
  DrawStats();   // статистика
  DrawStatus();  // статусное сообщение
  refresh();
}

void SnakeFrontend::DrawOverlay() {
  // Рамка игрового поля (аналогично тетрису)
  print_rectangle(0, kFieldHeight + 1, 0, kFieldWidth * 2 + 1);

  // Рамка HUD справа
  print_rectangle(0, kFieldHeight + 1, kFieldWidth * 2 + 2,
                  kFieldWidth * 2 + kHudWidth + 3);

  // Заголовки HUD
  print_rectangle(1, 3, kFieldWidth * 2 + 3, kFieldWidth * 2 + kHudWidth + 2);
  mvprintw(1, kFieldWidth * 2 + kHudWidth / 2, " HiSCORE ");

  print_rectangle(4, 6, kFieldWidth * 2 + 3, kFieldWidth * 2 + kHudWidth + 2);
  mvprintw(4, kFieldWidth * 2 + kHudWidth / 2 + 1, " SCORE ");

  print_rectangle(7, 9, kFieldWidth * 2 + 3, kFieldWidth * 2 + kHudWidth + 2);
  mvprintw(7, kFieldWidth * 2 + kHudWidth / 2 + 1, " LEVEL ");

  print_rectangle(10, 12, kFieldWidth * 2 + 3, kFieldWidth * 2 + kHudWidth + 2);
  mvprintw(10, kFieldWidth * 2 + kHudWidth / 2 + 1, " SPEED ");

  // Подсказки
  mvprintw(kFieldHeight - 5, kFieldWidth * 2 + 4, "p - pause");
  mvprintw(kFieldHeight - 3, kFieldWidth * 2 + 4, "q - quit");
}

void SnakeFrontend::DrawField() {
  // Очищаем игровое поле (не трогаем HUD и рамку)
  for (int y = 1; y <= kFieldHeight; y++) {
    for (int x = 1; x <= kFieldWidth; x++) {
      mvaddch(y, x, ' ');
    }
  }
}

void SnakeFrontend::DrawSnake() {
  for (const auto& pos : controller_.GetSnakePositions()) {
    int row = pos.first + 1;  // отступ от рамки
    int col = pos.second + 1;
    mvaddch(row, col, '@');
  }
}

void SnakeFrontend::DrawApple() {
  auto apple = controller_.GetApplePosition();
  int row = apple.first + 1;
  int col = apple.second + 1;
  mvaddch(row, col, '*');
}

void SnakeFrontend::DrawStats() {
  // HiScore (показываем максимум из текущего и рекорда)
  int hi = std::max(controller_.GetHighScore(), controller_.GetScore());
  mvprintw(2, kFieldWidth * 2 + kHudWidth / 2 + 1, "%04d", hi);

  // Score
  mvprintw(5, kFieldWidth * 2 + kHudWidth / 2 + 1, "%04d", controller_.GetScore());

  // Level
  mvprintw(8, kFieldWidth * 2 + kHudWidth / 2 + 3, "%d", controller_.GetLevel());

  // Speed (примерно, как у тетриса)
  mvprintw(11, kFieldWidth * 2 + kHudWidth / 2 + 1, "0.%lld",
           controller_.GetSpeed() / 100000000LL);
}

void SnakeFrontend::DrawStatus() {
  int pause_status = controller_.GetPauseStatus();
  SnakeState state = controller_.GetState();

  if (state == SnakeState::START) {
    mvprintw(kFieldHeight / 2, kFieldWidth / 2, "Press ENTER");
    mvprintw(kFieldHeight / 2 + 1, kFieldWidth / 2, " to start! ");
  } else if (pause_status == 1) {
    // Рамка паузы
    print_rectangle(kFieldHeight / 2 - 1, kFieldHeight / 2 + 1,
                    kFieldWidth - 6, kFieldWidth + 4);
    mvprintw(kFieldHeight / 2, kFieldWidth - 3, " PAUSE ");
  } else if (pause_status == 3) {
    // Рамка GAME OVER
    print_rectangle(kFieldHeight / 2 - 1, kFieldHeight / 2 + 2,
                    kFieldWidth - 8, kFieldWidth + 6);
    mvprintw(kFieldHeight / 2, kFieldWidth - 6, " GAME OVER ");
    mvprintw(kFieldHeight / 2 + 1, kFieldWidth - 6, "Press ENTER");
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// SnakeView
// ─────────────────────────────────────────────────────────────────────────────

SnakeView::SnakeView(SnakeController& controller) : controller_(controller) {}

void SnakeView::startEventLoop() {
  SnakeFrontend frontend(controller_);

  // Инициализация: рисуем рамку и HUD один раз
  clear();
  frontend.DrawOverlay();
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
    frontend.RenderGame();  // перерисовываем только изменяющееся

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

}  // namespace s21
