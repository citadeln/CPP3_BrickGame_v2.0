/**
 * @file
 * @brief Основной заголовочный файл.
 */

/**
 * @mainpage Тетрис
 *
 * Реализация Тетриса на языке Си.
 *
 * @author kristieh
 */

#ifndef TETRIS_H
#define TETRIS_H

#include "./brick_game/tetris/backend.h"
#include "./gui/cli/frontend.h"

void game_loop();
UserAction_t get_signal();
void color_pairs();

#endif  // TETRIS_H