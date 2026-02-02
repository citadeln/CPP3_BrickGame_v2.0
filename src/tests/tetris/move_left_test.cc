#include "../../brick_game/tetris/backend.h"
#include "tetris_test.h"

static int array1[] = {0, 1};

START_TEST(test_move_left) {
  Params_t *prms = create_prms(NULL);
  prms->figureinfo->figure_type = 3;
  prms->figureinfo->y = 2;
  prms->figureinfo->x = 5;
  prms->gameinfo->field[2][3] = array1[_i];
  move_left(prms);
}

START_TEST(test_move_left_collide_with_left_wall) {
  Params_t *prms = create_prms(NULL);
  prms->figureinfo->figure_type = 0;
  prms->figureinfo->y = 5;
  prms->figureinfo->x = 0;
  move_left(prms);
}

Suite *suite_move_left(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Move left");
  tc_core = tcase_create("Core");

  tcase_add_loop_test(tc_core, test_move_left, 0, sizeof(array1) / sizeof(int));
  tcase_add_test(tc_core, test_move_left_collide_with_left_wall);
  suite_add_tcase(s, tc_core);

  return s;
}
