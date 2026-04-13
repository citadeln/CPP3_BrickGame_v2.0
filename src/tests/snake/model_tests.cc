/**
 * @file
 * @brief Юнит-тесты для SnakeModel (MVVM: Model).
 */

#include <check.h>

#include "../../brick_game/snake/backend.h"
#ifdef fail
#undef fail
#endif

using namespace s21;

// ─────────────────────────────────────────────────────────────────────────────
// Тесты начального состояния (4 сегмента!)
// ─────────────────────────────────────────────────────────────────────────────

START_TEST(test_initial_length) {
  SnakeModel m;
  ck_assert_int_eq(static_cast<int>(m.GetSnakePositions().size()), 4);
}
END_TEST

START_TEST(test_initial_score) {
  SnakeModel m;
  ck_assert_int_eq(m.GetScore(), 0);
}
END_TEST

START_TEST(test_initial_level) {
  SnakeModel m;
  ck_assert_int_eq(m.GetLevel(), 1);
}
END_TEST

START_TEST(test_initial_not_game_over) {
  SnakeModel m;
  ck_assert(!m.IsGameOver());
}
END_TEST

START_TEST(test_apple_on_field) {
  SnakeModel m;
  auto [r, c] = m.GetApplePosition();
  ck_assert(r >= 0 && r < SnakeModel::kFieldHeight);
  ck_assert(c >= 0 && c < SnakeModel::kFieldWidth);
}
END_TEST

// ─────────────────────────────────────────────────────────────────────────────
// Тесты движения
// ─────────────────────────────────────────────────────────────────────────────

START_TEST(test_move_forward_up) {
  SnakeModel m;
  int old_row = m.GetSnakePositions().front().first;  // строка 5
  m.MoveForward();
  int new_row = m.GetSnakePositions().front().first;
  ck_assert_int_eq(new_row, old_row - 1);  // 5 → 4
}
END_TEST

START_TEST(test_move_forward_right) {
  SnakeModel m;
  m.ChangeDirection('R');
  int old_col = m.GetSnakePositions().front().second;  // столбец 10
  m.MoveForward();
  int new_col = m.GetSnakePositions().front().second;
  ck_assert_int_eq(new_col, old_col + 1);  // 10 → 11
}
END_TEST

START_TEST(test_move_forward_left) {
  SnakeModel m;
  m.ChangeDirection('R');
  m.MoveForward();  // 10→11
  m.ChangeDirection('D');
  m.MoveForward();
  m.ChangeDirection('L');
  int old_col = m.GetSnakePositions().front().second;
  m.MoveForward();
  int new_col = m.GetSnakePositions().front().second;
  ck_assert_int_eq(new_col, old_col - 1);
}
END_TEST

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

// ─────────────────────────────────────────────────────────────────────────────
// Тесты запрета разворота
// ─────────────────────────────────────────────────────────────────────────────

START_TEST(test_no_reverse_u_d) {
  SnakeModel m;
  m.ChangeDirection('D');  // Разворот 'U'→'D' — игнорируется
  m.MoveForward();
  int head_row = m.GetSnakePositions().front().first;
  ck_assert_int_eq(head_row, 4);  // 5-1=4, не вниз!
}
END_TEST

START_TEST(test_no_reverse_l_r) {
  SnakeModel m;
  m.ChangeDirection('R');
  m.MoveForward();
  m.ChangeDirection('L');  // Разворот → игнорируется
  int old_col = m.GetSnakePositions().front().second;
  m.MoveForward();
  int new_col = m.GetSnakePositions().front().second;
  ck_assert_int_eq(new_col, old_col + 1);  // Продолжает вправо!
}
END_TEST

// ─────────────────────────────────────────────────────────────────────────────
// Тесты столкновений (kFieldWidth=20)
// ─────────────────────────────────────────────────────────────────────────────

START_TEST(test_collision_with_top_wall) {
  SnakeModel m;
  // Из строки 5 вверх 6 шагов → row=-1
  for (int i = 0; i < 6; ++i) {
    m.MoveForward();
    if (m.IsGameOver()) break;
  }
  ck_assert(m.IsGameOver());
}
END_TEST

START_TEST(test_collision_with_right_wall) {
  SnakeModel m;
  m.ChangeDirection('R');
  // Из столбца 10 вправо 11 шагов → col=21 >=20
  for (int i = 0; i < 11; ++i) {
    m.MoveForward();
    if (m.IsGameOver()) break;
  }
  ck_assert(m.IsGameOver());
}
END_TEST

START_TEST(test_collision_with_bottom_wall) {
  SnakeModel m;
  m.ChangeDirection('R');
  m.MoveForward();
  m.ChangeDirection('D');
  // Из строки ~6 вниз 15 шагов → row=21 >=20
  for (int i = 0; i < 15; ++i) {
    m.MoveForward();
    if (m.IsGameOver()) break;
  }
  ck_assert(m.IsGameOver());
}
END_TEST

START_TEST(test_collision_with_left_wall) {
  SnakeModel m;
  m.ChangeDirection('L');
  // Из столбца 10 влево 11 шагов → col=-1
  for (int i = 0; i < 11; ++i) {
    m.MoveForward();
    if (m.IsGameOver()) break;
  }
  ck_assert(m.IsGameOver());
}
END_TEST

// ─────────────────────────────────────────────────────────────────────────────
// Остальные тесты
// ─────────────────────────────────────────────────────────────────────────────

START_TEST(test_speed_level1) {
  SnakeModel m;
  ck_assert_int_eq(static_cast<int>(m.GetSpeed()), 500000000);
}
END_TEST

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

START_TEST(test_has_won_false) {
  SnakeModel m;
  ck_assert(!m.HasWon());
}
END_TEST

// ─────────────────────────────────────────────────────────────────────────────
// Сьют
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

int main(void) {
  int failed = 0;
  Suite* s = snake_model_suite();
  SRunner* sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failed == 0) ? 0 : 1;
}
