#include "../../brick_game/tetris/backend.h"
#include "tetris_test.h"

static int array1[] = {0, 200, 500, 6000, 10000};

static int array2[] = {0, 6000, 500, 200, 0};

START_TEST(test_hi_score) {
  Params_t *prms = create_prms(NULL);
  prms->gameinfo->high_score = array1[_i];
  prms->gameinfo->score = array2[_i];
  hi_score(prms->gameinfo);
}

Suite *suite_hi_score(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("High score");
  tc_core = tcase_create("Core");

  tcase_add_loop_test(tc_core, test_hi_score, 0, sizeof(array1) / sizeof(int));

  suite_add_tcase(s, tc_core);

  return s;
}
