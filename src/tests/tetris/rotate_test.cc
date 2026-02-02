#include "../../brick_game/tetris/backend.h"
#include "tetris_test.h"

static int array1[] = {1, 1, 1, 1};

START_TEST(test_rotate) {
  Params_t *prms = create_prms(NULL);
  prms->figureinfo->figure_type = 1;
  prms->figureinfo->figure_state = _i;
  prms->gameinfo->field[_i + 3][5] = array1[_i];
  prms->figureinfo->y = 2;
  prms->figureinfo->figure_state = _i;
  rotate(prms);
}

START_TEST(test_rotate_left_wall) {
  Params_t *prms = create_prms(NULL);
  prms->figureinfo->figure_type = 1;
  prms->figureinfo->figure_state = 2;
  prms->figureinfo->y = 4;
  prms->figureinfo->x = 0;
  rotate(prms);
}

START_TEST(test_rotate_right_wall) {
  Params_t *prms = create_prms(NULL);
  prms->figureinfo->figure_type = 1;
  prms->figureinfo->figure_state = 3;
  prms->figureinfo->y = 4;
  prms->figureinfo->x = 9;
  rotate(prms);
}
START_TEST(test_rotate_bottom) {
  Params_t *prms = create_prms(NULL);
  prms->figureinfo->figure_type = 0;
  prms->figureinfo->figure_state = 0;
  prms->figureinfo->y = 21;
  prms->figureinfo->x = 5;
  rotate(prms);
}
Suite *suite_rotate(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Rotate");
  tc_core = tcase_create("Core");

  tcase_add_loop_test(tc_core, test_rotate, 0, sizeof(array1) / sizeof(int));
  tcase_add_test(tc_core, test_rotate_left_wall);
  tcase_add_test(tc_core, test_rotate_right_wall);
  tcase_add_test(tc_core, test_rotate_bottom);
  suite_add_tcase(s, tc_core);

  return s;
}