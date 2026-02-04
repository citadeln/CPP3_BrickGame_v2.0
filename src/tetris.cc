/**
 * @file
 * @brief Основной файл, задающий стартовые настройки библиотеки ncurses и
 * работающий с вводом пользователя.
 */

#include "tetris.h"

using namespace s21;

/**
 * @brief Вход в программу.
 *
 * Основная функция, в которой инициализируются стартовые параметры
 * библиотеки ncurses и запускается цикл игры.
 *
 * - initscr() - инициализация библиотеки ncurses
 * - noecho() - отключение отображения введенных символов пользователем
 * - curs_set(0) - настройка видимости курсора: 0 - невидимый, 1 - видимый
 * - keypad(stdscr, TRUE) - включение режима чтения спец. клавиш (стрелки,
 * F1-F12 и т.д.)
 * - nodelay(stdscr, TRUE) - убирают задержку ожидания ввода пользователя в
 * getch()
 * - endwin() - завершение работы библиотеки ncurses
 */
int main(void) {
  srand(time(0));
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  color_pairs();

  game_loop();

  curs_set(1);
  endwin();

  return 0;
}

/**
 * @brief Функция цикла игры.
 *
 * Основной цикл игры.
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
 * @brief Функция получения ввода от пользователя.
 *
 * Функция получает ввод от пользователя и возвращает соответствующее значение
 * действия.
 *
 * @return UserAction_t Возвращает значение ввода или отсутствие ввода от
 * пользователя.
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
 * @brief Функция инициализации цветов.
 *
 * Функция инициализирует пары цветов для покраски фигур.
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
