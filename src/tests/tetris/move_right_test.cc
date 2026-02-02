#include "../../brick_game/tetris/backend.h"
#include "tetris_test.h"

static int array1[] = {0, 1};

START_TEST(test_move_right) {
  Params_t *prms = create_prms(NULL);
  prms->figureinfo->figure_type = 3;
  prms->figureinfo->y = 2;
  prms->figureinfo->x = 5;
  prms->gameinfo->field[2][6] = array1[_i];
  move_right(prms);
}

START_TEST(test_move_right_collide_with_right_wall) {
  Params_t *prms = create_prms(NULL);
  prms->figureinfo->figure_type = 0;
  prms->figureinfo->y = 5;
  prms->figureinfo->x = 9;
  move_right(prms);
}

Suite *suite_move_right(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Move right");
  tc_core = tcase_create("Core");

  tcase_add_loop_test(tc_core, test_move_right, 0,
                      sizeof(array1) / sizeof(int));
  tcase_add_test(tc_core, test_move_right_collide_with_right_wall);
  suite_add_tcase(s, tc_core);

  return s;
}
