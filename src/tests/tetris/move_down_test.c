#include "../../brick_game/tetris/backend.h"
#include "tetris_test.h"

static int array1[] = {1, 1, 1, 1};

START_TEST(test_move_down) {
  Params_t *prms = create_prms(NULL);
  prms->figureinfo->figure_type = 3;
  prms->gameinfo->field[_i][5] = array1[_i];
  spawn(prms);
  move_down(prms);
}

START_TEST(test_move_down_collide_with_bottom) {
  Params_t *prms = create_prms(NULL);
  prms->figureinfo->figure_type = 3;
  prms->figureinfo->y = 20;
  move_down(prms);
}

Suite *suite_move_down(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Move down");
  tc_core = tcase_create("Core");

  tcase_add_loop_test(tc_core, test_move_down, 0, sizeof(array1) / sizeof(int));
  tcase_add_test(tc_core, test_move_down_collide_with_bottom);
  suite_add_tcase(s, tc_core);

  return s;
}
