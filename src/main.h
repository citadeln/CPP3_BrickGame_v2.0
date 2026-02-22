/**
 * @file
 * @brief Основной заголовочный файл.
 */

/**
 * @mainpage Тетрис
 *
 * Реализация Тетриса на языке Си.
 *
 * @author monroebu (легаси - kristieh)
 */

#ifndef TETRIS_H
#define TETRIS_H

// Определение пространства имён s21
namespace s21 {};

#include "./brick_game/snake/backend.h"
#include "./brick_game/tetris/backend.h"
#include "./gui/cli/frontend.h"
#include "./gui/cli/snake_frontend.h"

void game_loop();
UserAction_t get_signal();
void color_pairs();

#endif  // TETRIS_H