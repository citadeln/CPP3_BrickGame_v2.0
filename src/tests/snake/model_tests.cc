/**
 * @file
 * @brief Полные тесты SnakeModel + SnakeController FSM.
 */

#include <check.h>
#ifdef fail
#undef fail
#endif

#include "../../brick_game/snake/backend.h"
#include "../../brick_game/snake/fsm.h"

using namespace s21;

// ─────────────────────────────────────────────────────────────────────────────
// ТЕСТЫ SnakeModel
// ─────────────────────────────────────────────────────────────────────────────

// Проверяет начальную длину змейки = 4 сегмента
START_TEST(test_initial_length) {
  SnakeModel m;
  ck_assert_int_eq(static_cast<int>(m.GetSnakePositions().size()), 4);
}
END_TEST

// Проверяет начальный счёт = 0
START_TEST(test_initial_score) {
  SnakeModel m;
  ck_assert_int_eq(m.GetScore(), 0);
}
END_TEST

// Проверяет начальный уровень = 1
START_TEST(test_initial_level) {
  SnakeModel m;
  ck_assert_int_eq(m.GetLevel(), 1);
}
END_TEST

// Проверяет что игра не окончена в начале
START_TEST(test_initial_not_game_over) {
  SnakeModel m;
  ck_assert(!m.IsGameOver());
}
END_TEST

// Проверяет что яблоко спавнится внутри поля 20x20
START_TEST(test_apple_on_field) {
  SnakeModel m;
  auto [r, c] = m.GetApplePosition();
  ck_assert(r >= 0 && r < SnakeModel::kFieldHeight);
  ck_assert(c >= 0 && c < SnakeModel::kFieldWidth);
}
END_TEST

// Движение вверх: голова перемещается на строку выше
START_TEST(test_move_forward_up) {
  SnakeModel m;
  int old_row = m.GetSnakePositions().front().first;
  m.MoveForward();
  int new_row = m.GetSnakePositions().front().first;
  ck_assert_int_eq(new_row, old_row - 1);
}
END_TEST

// Движение вправо: голова перемещается на столбец правее
START_TEST(test_move_forward_right) {
  SnakeModel m;
  m.ChangeDirection('R');
  int old_col = m.GetSnakePositions().front().second;
  m.MoveForward();
  int new_col = m.GetSnakePositions().front().second;
  ck_assert_int_eq(new_col, old_col + 1);
}
END_TEST

// Комплексное движение: R→D→L, проверка поворота влево
START_TEST(test_move_forward_left) {
  SnakeModel m;
  m.ChangeDirection('R');
  m.MoveForward();
  m.ChangeDirection('D');
  m.MoveForward();
  m.ChangeDirection('L');
  int old_col = m.GetSnakePositions().front().second;
  m.MoveForward();
  int new_col = m.GetSnakePositions().front().second;
  ck_assert_int_eq(new_col, old_col - 1);
}
END_TEST

// Движение вниз: голова перемещается на строку ниже
START_TEST(test_move_forward_down) {
  SnakeModel m;
  m.ChangeDirection('R');
  m.MoveForward();
  m.ChangeDirection('D');
  int old_row = m.GetSnakePositions().front().first;
  m.MoveForward();
  int new_row = m.GetSnakePositions().front().first;
  ck_assert_int_eq(new_row, old_row + 1);
}
END_TEST

// Длина сохраняется без поедания яблока (хвост удаляется)
START_TEST(test_length_preserved_without_apple) {
  SnakeModel m;
  int old_len = static_cast<int>(m.GetSnakePositions().size());
  m.ChangeDirection('R');
  for (int i = 0; i < 3; ++i) {
    m.MoveForward();
    if (m.IsAppleEaten()) break;
  }
  if (!m.IsAppleEaten()) {
    int new_len = static_cast<int>(m.GetSnakePositions().size());
    ck_assert_int_eq(new_len, old_len);
  }
}
END_TEST

// Запрет разворота U→D (игнорируется, движется вверх)
START_TEST(test_no_reverse_u_d) {
  SnakeModel m;
  m.ChangeDirection('D');
  m.MoveForward();
  int head_row = m.GetSnakePositions().front().first;
  ck_assert_int_eq(head_row, 4);
}
END_TEST

// Запрет разворота R→L (игнорируется, движется вправо)
START_TEST(test_no_reverse_l_r) {
  SnakeModel m;
  m.ChangeDirection('R');
  m.MoveForward();
  m.ChangeDirection('L');
  int old_col = m.GetSnakePositions().front().second;
  m.MoveForward();
  int new_col = m.GetSnakePositions().front().second;
  ck_assert_int_eq(new_col, old_col + 1);
}
END_TEST

// Столкновение с верхней стеной (row < 0)
START_TEST(test_collision_with_top_wall) {
  SnakeModel m;
  for (int i = 0; i < 6; ++i) {
    m.MoveForward();
    if (m.IsGameOver()) break;
  }
  ck_assert(m.IsGameOver());
}
END_TEST

// Столкновение с правой стеной (col >= 20)
START_TEST(test_collision_with_right_wall) {
  SnakeModel m;
  m.ChangeDirection('R');
  for (int i = 0; i < 11; ++i) {
    m.MoveForward();
    if (m.IsGameOver()) break;
  }
  ck_assert(m.IsGameOver());
}
END_TEST

// Столкновение с нижней стеной (row >= 20)
START_TEST(test_collision_with_bottom_wall) {
  SnakeModel m;
  m.ChangeDirection('R');
  m.MoveForward();
  m.ChangeDirection('D');
  for (int i = 0; i < 15; ++i) {
    m.MoveForward();
    if (m.IsGameOver()) break;
  }
  ck_assert(m.IsGameOver());
}
END_TEST

// Столкновение с левой стеной (col < 0)
START_TEST(test_collision_with_left_wall) {
  SnakeModel m;
  m.ChangeDirection('L');
  for (int i = 0; i < 11; ++i) {
    m.MoveForward();
    if (m.IsGameOver()) break;
  }
  ck_assert(m.IsGameOver());
}
END_TEST

// Базовая скорость на уровне 1 = 500мс (500000000нс)
START_TEST(test_speed_level1) {
  SnakeModel m;
  ck_assert_int_eq(static_cast<int>(m.GetSpeed()), 500000000);
}
END_TEST

// Reset возвращает начальное состояние
START_TEST(test_reset) {
  SnakeModel m;
  m.ChangeDirection('R');
  m.MoveForward();
  m.Reset();
  ck_assert_int_eq(static_cast<int>(m.GetSnakePositions().size()), 4);
  ck_assert_int_eq(m.GetScore(), 0);
  ck_assert(!m.IsGameOver());
}
END_TEST

// Победа требует 200 сегментов (начальная длина < 200)
START_TEST(test_has_won_false) {
  SnakeModel m;
  ck_assert(!m.HasWon());
}
END_TEST

// ─────────────────────────────────────────────────────────────────────────────
// ТЕСТЫ SnakeController FSM
// ─────────────────────────────────────────────────────────────────────────────

// Начальное состояние START, pause_status=2
START_TEST(test_fsm_start_enter) {
  SnakeModel model;
  SnakeController ctrl(model);
  ck_assert_int_eq(ctrl.GetPauseStatus(), 2);
  ck_assert(ctrl.GetState() == SnakeState::START);
}
END_TEST

// Enter: START → SPAWN → MOVING, pause_status=0
START_TEST(test_fsm_spawn_to_moving) {
  SnakeModel model;
  SnakeController ctrl(model);
  ctrl.ProcessUserInput('\n');
  ck_assert_int_eq(ctrl.GetPauseStatus(), 0);
  ck_assert(ctrl.GetState() == SnakeState::MOVING);
}
END_TEST

// 'p': MOVING ↔ PAUSE (pause_status 0↔1)
START_TEST(test_fsm_pause_unpause) {
  SnakeModel model;
  SnakeController ctrl(model);
  ctrl.ProcessUserInput('\n');
  ctrl.ProcessUserInput('p');
  ck_assert_int_eq(ctrl.GetPauseStatus(), 1);
  ck_assert(ctrl.GetState() == SnakeState::PAUSE);

  ctrl.ProcessUserInput('p');
  ck_assert_int_eq(ctrl.GetPauseStatus(), 0);
  ck_assert(ctrl.GetState() == SnakeState::MOVING);
}
END_TEST

// Прокси-геттеры возвращают данные Model
START_TEST(test_proxy_methods) {
  SnakeModel model;
  SnakeController ctrl(model);
  ck_assert_int_eq(ctrl.GetScore(), model.GetScore());
  ck_assert_int_eq(ctrl.GetHighScore(), model.GetHighScore());
  ck_assert_int_eq(ctrl.GetLevel(), model.GetLevel());
  ck_assert(!ctrl.GetSnakePositions().empty());
  auto apple = ctrl.GetApplePosition();
  ck_assert(apple.first >= 0 && apple.second >= 0);
}
END_TEST

// 'q' из START → EXIT_STATE (pause_status=-1)
START_TEST(test_fsm_exit_q_start) {
  SnakeModel model;
  SnakeController ctrl(model);
  ctrl.ProcessUserInput('q');
  ck_assert(ctrl.GetState() == SnakeState::EXIT_STATE);
  ck_assert_int_eq(ctrl.GetPauseStatus(), -1);
}
END_TEST

// 'q' из PAUSE → EXIT_STATE (pause_status=-1)
START_TEST(test_fsm_exit_q_pause) {
  SnakeModel model;
  SnakeController ctrl(model);
  ctrl.ProcessUserInput('\n');  // MOVING
  ctrl.ProcessUserInput('p');   // PAUSE
  ctrl.ProcessUserInput('q');   // PAUSE → EXIT
  ck_assert(ctrl.GetState() == SnakeState::EXIT_STATE);
  ck_assert_int_eq(ctrl.GetPauseStatus(), -1);
}
END_TEST

// Неизвестная клавиша 'x' → default: (состояние не меняется)
START_TEST(test_fsm_default_case) {
  SnakeModel model;
  SnakeController ctrl(model);
  ctrl.ProcessUserInput('x');
  ck_assert_int_eq(ctrl.GetPauseStatus(), 2);
  ck_assert(ctrl.GetState() == SnakeState::START);
}
END_TEST

// Направление 'R' в MOVING не меняет состояние FSM
START_TEST(test_fsm_direction_change) {
  SnakeModel model;
  SnakeController ctrl(model);
  ctrl.ProcessUserInput('\n');  // MOVING
  ctrl.ProcessUserInput('R');   // Направление
  ck_assert_int_eq(ctrl.GetPauseStatus(), 0);
  ck_assert(ctrl.GetState() == SnakeState::MOVING);
}
END_TEST

// ─────────────────────────────────────────────────────────────────────────────
// Сьюты
// ─────────────────────────────────────────────────────────────────────────────

Suite* snake_model_suite(void) {
  Suite* s = suite_create("SnakeModel");

  TCase* tc_init = tcase_create("Initialization");
  tcase_add_test(tc_init, test_initial_length);
  tcase_add_test(tc_init, test_initial_score);
  tcase_add_test(tc_init, test_initial_level);
  tcase_add_test(tc_init, test_initial_not_game_over);
  tcase_add_test(tc_init, test_apple_on_field);
  suite_add_tcase(s, tc_init);

  TCase* tc_move = tcase_create("Movement");
  tcase_add_test(tc_move, test_move_forward_up);
  tcase_add_test(tc_move, test_move_forward_right);
  tcase_add_test(tc_move, test_move_forward_left);
  tcase_add_test(tc_move, test_move_forward_down);
  tcase_add_test(tc_move, test_length_preserved_without_apple);
  suite_add_tcase(s, tc_move);

  TCase* tc_dir = tcase_create("Direction");
  tcase_add_test(tc_dir, test_no_reverse_u_d);
  tcase_add_test(tc_dir, test_no_reverse_l_r);
  suite_add_tcase(s, tc_dir);

  TCase* tc_coll = tcase_create("Collision");
  tcase_add_test(tc_coll, test_collision_with_top_wall);
  tcase_add_test(tc_coll, test_collision_with_right_wall);
  tcase_add_test(tc_coll, test_collision_with_bottom_wall);
  tcase_add_test(tc_coll, test_collision_with_left_wall);
  suite_add_tcase(s, tc_coll);

  TCase* tc_stats = tcase_create("Stats");
  tcase_add_test(tc_stats, test_speed_level1);
  suite_add_tcase(s, tc_stats);

  TCase* tc_misc = tcase_create("Misc");
  tcase_add_test(tc_misc, test_reset);
  tcase_add_test(tc_misc, test_has_won_false);
  suite_add_tcase(s, tc_misc);

  return s;
}

Suite* fsm_suite(void) {
  Suite* s = suite_create("SnakeController FSM");

  TCase* tc_basic = tcase_create("Basic States");
  tcase_add_test(tc_basic, test_fsm_start_enter);
  tcase_add_test(tc_basic, test_fsm_spawn_to_moving);
  tcase_add_test(tc_basic, test_fsm_pause_unpause);
  tcase_add_test(tc_basic, test_proxy_methods);
  suite_add_tcase(s, tc_basic);

  TCase* tc_exit = tcase_create("Exit Paths");
  tcase_add_test(tc_exit, test_fsm_exit_q_start);
  tcase_add_test(tc_exit, test_fsm_exit_q_pause);
  suite_add_tcase(s, tc_exit);

  TCase* tc_input = tcase_create("Input Handling");
  tcase_add_test(tc_input, test_fsm_default_case);
  tcase_add_test(tc_input, test_fsm_direction_change);
  suite_add_tcase(s, tc_input);

  return s;
}

int main(void) {
  int failed = 0;

  Suite* model_suite = snake_model_suite();
  SRunner* model_runner = srunner_create(model_suite);
  srunner_run_all(model_runner, CK_NORMAL);
  failed += srunner_ntests_failed(model_runner);
  srunner_free(model_runner);

  Suite* fsm_s = fsm_suite();
  SRunner* fsm_runner = srunner_create(fsm_s);
  srunner_run_all(fsm_runner, CK_NORMAL);
  failed += srunner_ntests_failed(fsm_runner);
  srunner_free(fsm_runner);

  return (failed == 0) ? 0 : 1;
}
