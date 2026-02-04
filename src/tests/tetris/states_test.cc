#include "../../brick_game/tetris/backend.h"
#include "tetris_test.h"

START_TEST(test_gameover) {
  Params_t *prms = create_prms(NULL);
  prms->state = GameState_t::GAMEOVER;
  UserAction_t action = UserAction_t::Left;
  function func = fsm_table(prms->state, action);
  func(prms);
}

START_TEST(test_exitstate) {
  Params_t *prms = create_prms(NULL);
  prms->state = GameState_t::EXIT_STATE;
  UserAction_t action = UserAction_t::Left;
  function func = fsm_table(prms->state, action);
  func(prms);
}

START_TEST(test_pause) {
  Params_t *prms = create_prms(NULL);
  prms->state = GameState_t::MOVING;
  UserAction_t action = UserAction_t::Pause;
  function func = fsm_table(prms->state, action);
  func(prms);
}

START_TEST(test_uppause) {
  Params_t *prms = create_prms(NULL);
  prms->state = GameState_t::PAUSE;
  UserAction_t action = UserAction_t::Left;
  function func = fsm_table(prms->state, action);
  func(prms);
}

Suite *suite_states(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("States");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_gameover);
  tcase_add_test(tc_core, test_exitstate);
  tcase_add_test(tc_core, test_pause);
  tcase_add_test(tc_core, test_uppause);
  suite_add_tcase(s, tc_core);

  return s;
}
