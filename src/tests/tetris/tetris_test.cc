#include "tetris_test.h"

int main(void) {
  Suite *suite_list[] = {suite_userInput(),
                         suite_create_prms(),
                         suite_hi_score(),
                         suite_chech_timer(),

                         suite_states(),
                         suite_move_down(),
                         suite_rotate(),
                         suite_move_left(),
                         suite_move_right(),
                         suite_fast_move_down(),
                         suite_collide(),

                         NULL};

  for (int i = 0; suite_list[i]; i++) {
    fprintf(stderr,
            "------------------------------------------------------------------"
            "------------\n");
    SRunner *runner = srunner_create(suite_list[i]);
    srunner_run_all(runner, CK_NORMAL);
    srunner_free(runner);
  }

  return 0;
}
