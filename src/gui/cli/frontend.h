/**
 * @file
 * @brief Заголовочныый файл библиотеки отрисовки интерфейса.
 */
#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>

#include "./../../brick_game/tetris/backend.h"
#include "./../../brick_game/tetris/defines.h"

#define HUD_WIDTH 11  ///< Ширина панели со статистикой

void print_game(GameInfo_t gameinfo);
void print_stats(GameInfo_t gameinfo);
void print_overlay(void);
void print_start(void);
void print_pause();
void print_gameover();
void print_exit();
void print_rectangle(int y1, int y2, int x1, int x2);

#endif  // FRONTEND_H