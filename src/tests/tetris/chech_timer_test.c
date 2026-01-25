#include "../../brick_game/tetris/backend.h"
#include "tetris_test.h"

static int array1[] = {0, 1, 2};

START_TEST(test_chech_timer) {
  Params_t *prms = create_prms(NULL);
  prms->state = MOVING;
  prms->start_time.tv_sec -= array1[_i];
  chech_timer(prms);
}

Suite *suite_chech_timer(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Chech timer");
  tc_core = tcase_create("Core");

  tcase_add_loop_test(tc_core, test_chech_timer, 0,
                      sizeof(array1) / sizeof(int));

  suite_add_tcase(s, tc_core);

  return s;
}
