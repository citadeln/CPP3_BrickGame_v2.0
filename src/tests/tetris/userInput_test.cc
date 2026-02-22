#include "../../brick_game/tetris/backend.h"
#include "tetris_test.h"

static UserAction_t array1[] = {
    UserAction_t::Start, UserAction_t::Pause,  UserAction_t::Terminate,
    UserAction_t::Left,  UserAction_t::Right,  UserAction_t::Up,
    UserAction_t::Down,  UserAction_t::Action, UserAction_t::ERRENUM};

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
