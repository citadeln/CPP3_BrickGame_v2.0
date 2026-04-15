#ifndef TETRIS_TEST_H
#define TETRIS_TEST_H

#include <check.h>
#include <stdio.h>

#include "../../brick_game/tetris/backend.h"

Suite *suite_userInput(void);
Suite *suite_create_prms(void);
Suite *suite_hi_score(void);
Suite *suite_chech_timer(void);

Suite *suite_states(void);
Suite *suite_move_down(void);
Suite *suite_rotate(void);
Suite *suite_move_left(void);
Suite *suite_move_right(void);
Suite *suite_fast_move_down(void);
Suite *suite_collide(void);

#endif  // TETRIS_TEST_H
