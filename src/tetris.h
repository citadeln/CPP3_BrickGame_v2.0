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

// Определение пространства имён s21
namespace s21 {};

#include "./brick_game/tetris/backend.h"
#include "./gui/cli/frontend.h"

void game_loop();
UserAction_t get_signal();
void color_pairs();

#endif  // TETRIS_H