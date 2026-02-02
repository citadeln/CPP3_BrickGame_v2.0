#include "../../brick_game/tetris/backend.h"
#include "tetris_test.h"

START_TEST(test_create_prms) {
  Params_t *prms = create_prms(NULL);
  prms = create_prms(prms);

  *prms->gameinfo = updateCurrentState();
}

Suite *suite_create_prms(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Create prms");
  tc_core = tcase_create("Core");

  tcase_add_loop_test(tc_core, test_create_prms, 0, 1);

  suite_add_tcase(s, tc_core);

  return s;
}
