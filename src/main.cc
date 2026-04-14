/**
 * @file
 * @brief Основной файл: выбор игры, инициализация ncurses, цикл игры.
 */

#include "main.h"

#include <iostream>

using namespace s21;

/**
 * @brief Выбор игры: Тетрис или Змейка.
 */
char select_game() {
  std::cout << "\nSelect the game:\n";
  std::cout << "1. Tetris\n";
  std::cout << "2. Snake\n";
  std::cout << "Your choice: ";
  char choice;
  std::cin >> choice;
  return choice;
}

/**
 * @brief Вход в программу.
 *
 * Инициализирует ncurses и запускает выбранную игру.
 *
 * - initscr()           — инициализация ncurses
 * - noecho()            — скрыть ввод пользователя
 * - curs_set(0)         — скрыть курсор
 * - keypad(stdscr,TRUE) — поддержка спецклавиш (стрелки и т.д.)
 * - nodelay(stdscr,TRUE)— неблокирующий getch()
 * - endwin()            — завершение ncurses
 */
int main(void) {
  srand(static_cast<unsigned>(time(nullptr)));

  char game_choice = select_game();

  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  color_pairs();

  if (game_choice == '1') {
    // ── Тетрис ──────────────────────────────────────────────────────────────
    game_loop();

  } else if (game_choice == '2') {
    // ── Змейка (MVVM) ────────────────────────────────────────────────────────
    // Model:     SnakeModel     — бизнес-логика, данные
    // ViewModel: SnakeController — FSM, привязка данных
    // View:      SnakeView      — ncurses-цикл событий + SnakeFrontend-рендер
    SnakeModel snake_model;
    SnakeController snake_controller(snake_model);
    SnakeView snake_view(snake_controller);
    snake_view.startEventLoop();

  } else {
    endwin();
    std::cerr << "Invalid choice.\n";
    return 1;
  }

  curs_set(1);
  endwin();

  return 0;
}

/**
 * @brief Цикл игры Тетрис.
 */
void game_loop() {
  GameInfo_t gameinfo;
  gameinfo.pause = 2;

  while (gameinfo.pause != -1) {
    userInput(get_signal(), 0);
    gameinfo = updateCurrentState();
    print_game(gameinfo);
  }
}

/**
 * @brief Получение ввода пользователя для Тетриса.
 *
 * @return UserAction_t Действие пользователя или ERRENUM при отсутствии ввода.
 */
UserAction_t get_signal() {
  UserAction_t action = UserAction_t::ERRENUM;

  switch (getch()) {
  case '\n':
    action = UserAction_t::Start;
    break;
  case 'p':
    action = UserAction_t::Pause;
    break;
  case 'q':
    action = UserAction_t::Terminate;
    break;
  case KEY_LEFT:
    action = UserAction_t::Left;
    break;
  case KEY_RIGHT:
    action = UserAction_t::Right;
    break;
  case KEY_UP:
    action = UserAction_t::Up;
    break;
  case KEY_DOWN:
    action = UserAction_t::Down;
    break;
  case ' ':
    action = UserAction_t::Action;
    break;
  default:
    action = UserAction_t::ERRENUM;
    break;
  }

  return action;
}

/**
 * @brief Инициализация цветовых пар ncurses.
 */
void color_pairs() {
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(5, COLOR_RED, COLOR_BLACK);
  init_pair(6, COLOR_BLUE, COLOR_BLACK);
  init_pair(7, COLOR_YELLOW, COLOR_BLACK);
}
