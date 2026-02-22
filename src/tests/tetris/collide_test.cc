#include "../../brick_game/tetris/backend.h"
#include "tetris_test.h"

static int array[] = {-2, -1, 0, 1, 2};

START_TEST(test_collide) {
  Params_t *prms = create_prms(NULL);

  for (int y = Y_FIELD - array[_i]; y < Y_FIELD + 2; y++)
    for (int x = X_START; x < X_FIELD; x++) prms->gameinfo->field[y][x] = 1;

  prms->state = GameState_t::COLLIDE;
  function func = fsm_table(prms->state, UserAction_t::Left);
  func(prms);
}

Suite *suite_collide(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Collide");
  tc_core = tcase_create("Core");

  tcase_add_loop_test(tc_core, test_collide, 0, sizeof(array) / sizeof(int));
  suite_add_tcase(s, tc_core);

  return s;
}
