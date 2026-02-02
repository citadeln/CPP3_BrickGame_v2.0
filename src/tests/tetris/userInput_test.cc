#include "../../brick_game/tetris/backend.h"
#include "tetris_test.h"

static UserAction_t array1[] = {Start, Pause, Terminate, Left, Right,
                                Up,    Down,  Action, ERRENUM};

static bool array2[] = {0, 1, 0, 1, 0, 1, 0, 1, 0};

START_TEST(test_userInput) { userInput(array1[_i], array2[_i]); }

Suite *suite_userInput(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("userInput");
  tc_core = tcase_create("Core");

  tcase_add_loop_test(tc_core, test_userInput, 0,
                      sizeof(array2) / sizeof(bool));

  suite_add_tcase(s, tc_core);

  return s;
}
