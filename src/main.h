/**
 * @file
 * @brief Основной заголовочный файл.
 */

/**
 * @mainpage Brick Game
 *
 * Реализация игр Тетрис и Змейка на C++.
 * Архитектура Змейки построена по паттерну MVVM.
 *
 * @author s21
 */

#ifndef MAIN_H
#define MAIN_H

// Определение пространства имён s21
namespace s21 {}

#include "./brick_game/snake/backend.h"
#include "./brick_game/snake/fsm.h"
#include "./brick_game/tetris/backend.h"
#include "./gui/cli/frontend.h"
#include "./gui/cli/snake_frontend.h"

void game_loop();
UserAction_t get_signal();
void color_pairs();

#endif  // MAIN_H
