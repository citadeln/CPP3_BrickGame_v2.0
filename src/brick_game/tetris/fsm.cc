/**
 * @file
 * @brief Функции конечного автомата.
 */

#include "fsm.h"

/**
 * @brief Функция автомата конечных состояний.
 *
 * Функция хранит в себе таблицу автомата конечных состояний.
 * В зависимости от текущего состояния и ввода пользователя
 * принимает одно из возможных решений, и отдает функцию на выполнение.
 *
 * @param state  Строка в таблице. Текущее состояние системы.
 * @param action Столбец в таблице. Ввод пользователя или его отсутсвие
 *
 * @return function Возвращает функцию для выполнения.
 */
function fsm_table(GameState_t state, UserAction_t action) {
  if ((int)action == -1)
    action = UserAction_t::Action;  // было if ((int)action == -1) action = 8;

  static const function fsm_table[8][9] = {
      // START
      {spawn, NULL, exitstate, NULL, NULL, NULL, NULL, NULL, NULL},
      // SPAWN
      {spawn, spawn, spawn, spawn, spawn, spawn, spawn, spawn, spawn},
      // MOVING
      {NULL, pause, exitstate, move_left, move_right, NULL, fast_move_down,
       rotate, NULL},
      // SHIFTING
      {move_down, move_down, move_down, move_down, move_down, move_down,
       move_down, move_down, move_down},
      // COLLIDE
      {collide, collide, collide, collide, collide, collide, collide, collide,
       collide},
      // PAUSE
      {unpause, unpause, exitstate, unpause, unpause, unpause, unpause, unpause,
       NULL},
      // GAMEOVER
      {gameover, gameover, gameover, gameover, gameover, gameover, gameover,
       gameover, gameover},
      // EXIT_STATE
      {exitstate, exitstate, exitstate, exitstate, exitstate, exitstate,
       exitstate, exitstate, exitstate}};

  return fsm_table[state][action];
}

/**
 * @brief Функция появления блока на поле.
 *
 * Функция вводит в игру новую фигуру в начальном положении.
 * Начальное положение фигуры на 2 строки выше основного игрового поля.
 * Сразу после появления происходит проверка на движение вниз на 2 строки,
 * при столкновении происходит конец игры(gameover),
 * иначе вводит фигуру уже на основном игровом поле.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 */
void spawn(Params_t *prms) {
  prms->figureinfo->y = Y_BLOCK;
  prms->figureinfo->x = X_BLOCK;
  prms->figureinfo->figure_type = prms->figureinfo->next_figure_type;
  prms->figureinfo->figure_state = 0;
  prms->figureinfo->figure_color = prms->figureinfo->next_figure_color;
  prms->gameinfo->pause = 0;

  prms->figureinfo->next_figure_type = rand() % 7;
  if (COLOR)
    prms->figureinfo->next_figure_color =
        prms->figureinfo->next_figure_type + 1;
  else
    prms->figureinfo->next_figure_color = 1;

  for (int y = 0; y < MAX_FIGURE_SIZE; y++)
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) prms->gameinfo->next[y][x] = 0;

  for (int i = 0; i < BLOCKS; i++) {
    int y = 1 + get_block_coord(prms->figureinfo->next_figure_type, 0, i, 0);
    int x = 2 + get_block_coord(prms->figureinfo->next_figure_type, 0, i, 1);
    prms->gameinfo->next[y][x] = prms->figureinfo->next_figure_color;
  }

  if (collide_while_moving(prms, 1, 0))
    prms->state = GAMEOVER;
  else if (collide_while_moving(prms, 2, 0)) {
    prms->state = GAMEOVER;
    prms->figureinfo->y++;
  } else {
    prms->state = MOVING;
    prms->figureinfo->y += 2;
    timespec_get(&prms->start_time, TIME_UTC);
    prms->hold = false;
  }
  clear_or_draw_position(prms, 1);
}

/**
 * @brief Функция движения блока вниз на одну строку.
 *
 * Функция для движения вниз. Стирает из матрицы текущее положение фигуры,
 * проверяет возможно ли движение вниз без столкновения.
 * В случае столкновения заполняет матрицу по текущему положению и переходит к
 * стадии присоединения. Если столкновения нет, то заполняет матрицу на одну
 * строку ниже.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 */
void move_down(Params_t *prms) {
  clear_or_draw_position(prms, 0);

  if (collide_while_moving(prms, 1, 0)) {
    prms->state = COLLIDE;
  } else {
    prms->figureinfo->y++;
    prms->state = MOVING;
    timespec_get(&prms->start_time, TIME_UTC);
  }
  clear_or_draw_position(prms, 1);
}

/**
 * @brief Функция поворота фигуры.
 *
 * Функция поворачивает фигуру против часовой стрелки относительно
 * блока-курсора. По умолчанию поворот происходит против часовой стрелки. Можно
 * изменить параметр ROTATION в defines.h для поворота по часовой.
 *
 * Функция берет из мессива фигур следующее состояние
 * и проверяет можно ли поместить такую фигуре в текущем положении в матрице.
 * В случае успеха фигуре отобразится в новом положении,
 * в случае отказа фигура останется в текущем виде.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 */
void rotate(Params_t *prms) {
  int new_state = 0;

  if (ROTATION) {
    if (prms->figureinfo->figure_state == FIGURE_STATES - 1)
      new_state = 0;
    else
      new_state = prms->figureinfo->figure_state + 1;
  } else {
    if (prms->figureinfo->figure_state == 0)
      new_state = FIGURE_STATES - 1;
    else
      new_state = prms->figureinfo->figure_state - 1;
  }

  clear_or_draw_position(prms, 0);

  if (collide_while_rotating(prms, new_state)) {
  } else {
    prms->figureinfo->figure_state = new_state;
  }
  clear_or_draw_position(prms, 1);
}

/**
 * @brief Функция движение влево.
 *
 * Функция проверяет можно ли сдвинуть фигуре влево на 1 столбец.
 * В случае успеха фигуре отобразится в новом положении,
 * в случае отказа фигура останется в текущем месте.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 */
void move_left(Params_t *prms) {
  clear_or_draw_position(prms, 0);

  if (!collide_while_moving(prms, 0, -1)) prms->figureinfo->x--;

  clear_or_draw_position(prms, 1);
}

/**
 * @brief Функция движение вправо.
 *
 * Функция проверяет можно ли сдвинуть фигуре вправо на 1 столбец.
 * В случае успеха фигуре отобразится в новом положении,
 * в случае отказа фигура останется в текущем месте.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 */
void move_right(Params_t *prms) {
  clear_or_draw_position(prms, 0);

  if (!collide_while_moving(prms, 0, 1)) prms->figureinfo->x++;

  clear_or_draw_position(prms, 1);
}

/**
 * @brief Функция ускоренного движения вниз.
 *
 * В зависимости от макроса DROP в fsm.h функция:
 *  - 1 - сдвигает фигуру максимально вниз и переводит состояние игры
 * в состояние присоединения.
 *  - 0 - сдвигает фигуру на одну строку вниз, если это возможно,
 * вне зависимости от таймера.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 */
void fast_move_down(Params_t *prms) {
  clear_or_draw_position(prms, 0);

  if (DROP) {
    int new_y = 1;

    while (!collide_while_moving(prms, new_y, 0)) {
      new_y++;
    }

    prms->figureinfo->y += new_y - 1;
    clear_or_draw_position(prms, 1);
    timespec_get(&prms->start_time, TIME_UTC);
    prms->state = COLLIDE;
  } else {
    if (collide_while_moving(prms, 1, 0)) {
      prms->state = COLLIDE;
    } else {
      prms->figureinfo->y++;
      prms->state = MOVING;
      timespec_get(&prms->start_time, TIME_UTC);
    }
    clear_or_draw_position(prms, 1);
  }
}

/**
 * @brief Функция проверки столкновения при движении фигуры.
 *
 * Функция проверяет столкновения фигуры при сдвиге блока-курсора на Y и X.
 * Если в новых координатах фигура выходит за границы поля,
 * либо сталкивается с другой фигурой на поле, то вернет 1.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 * @param y_shift Значения сдвига блока-курсора по шкале Y.
 * @param x_shift Значения сдвига блока-курсора по шкале X.
 *
 * @return int Возвращает:
 *  - 1, если произошло столкновение и такое движение не возможно,
 *  - 0, если столкновения не было и фигуру можно сдвинуть.
 */
int collide_while_moving(Params_t *prms, int y_shift, int x_shift) {
  int return_code = 0;

  for (int i = 0; i < BLOCKS && !return_code; i++) {
    int y = y_shift + prms->figureinfo->y +
            get_block_coord(prms->figureinfo->figure_type,
                            prms->figureinfo->figure_state, i, 0);
    int x = x_shift + prms->figureinfo->x +
            get_block_coord(prms->figureinfo->figure_type,
                            prms->figureinfo->figure_state, i, 1);
    if (y >= Y_FIELD + 2)
      return_code = 1;
    else if (x <= X_START - 1 || x >= X_FIELD)
      return_code = 1;
    else if (prms->gameinfo->field[y][x])
      return_code = 1;
  }
  return return_code;
}

/**
 * @brief Функция проверки столкновения при повороте фигуры.
 *
 * Функция проверяет столкновения фигуры при повороте фигуры относительно
 * блока-курсора. Если в новом состоянии фигура выходит за нижнюю границу поля,
 * либо сталкивается с другой фигурой на поле, то вернет 1.
 *
 * Если в новом состоянии фигура выходит за боковую границу поля,
 * но не сталкивается с другой фигурой,
 * то блок-курсор сдвигается принудительно на 1 столбец от боковой границы
 * (для фигуры I сдвиг на 2 блока от левой границы из-за особенностей поворота).
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 * @param new_state Новое состояние фигуры
 *
 * @return int Возвращает:
 *  - 1, если произошло столкновение и такое движение не возможно,
 *  - 0, если столкновения не было и фигуру можно повернуть.
 */
int collide_while_rotating(Params_t *prms, int new_state) {
  int return_code = 0;

  for (int i = 0; i < BLOCKS && !return_code; i++) {
    int y = prms->figureinfo->y +
            get_block_coord(prms->figureinfo->figure_type, new_state, i, 0);
    ;
    int x = prms->figureinfo->x +
            get_block_coord(prms->figureinfo->figure_type, new_state, i, 1);
    ;
    if (x < X_START) {
      int shift_x = (prms->figureinfo->figure_type == 0) ? 2 : 1;
      if (collide_while_moving(prms, 0, shift_x))
        return_code = 1;
      else
        prms->figureinfo->x += shift_x;
    } else if (x >= X_FIELD) {
      if (collide_while_moving(prms, 0, -1))
        return_code = 1;
      else
        prms->figureinfo->x--;
    } else if (y >= Y_FIELD + 2 /*|| y <= Y_START + 1*/)
      return_code = 1;
    else if (prms->gameinfo->field[y][x])
      return_code = 1;
  }

  return return_code;
}

/**
 * @brief Функция присоединения фигуры при столкновении.
 *
 * Функция срабатывет при движении вниз и столкновении фигуры с нижней границей
 * поля или с другой фигурой.
 *
 * Функция проверяет игровое поле на заполненные линии, удаляет их,
 * после считает кол-во заработанных очков, в зависимости от кол-ва удаленных
 * линий.
 *
 * Функция просчитывает текущий уровень в зависимости от кол-ва набранных очков.
 * По умолчанию, новый уровень каждые 600 очков, максимум 10.
 *
 * Функция просчитывает скорость в зависимости от текущего уровня.
 *
 * После переводит состоянии игры в состояние появления новой фигуры на поле.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 */
void collide(Params_t *prms) {
  int lines = 0;

  for (int y = Y_FIELD + 1; y >= 0; y--) {
    int ones = 0;
    for (int x = 0; x < X_FIELD; x++)
      if (prms->gameinfo->field[y][x]) ones++;
    if (ones == X_FIELD) {
      lines++;
      remove_full_line(prms, y);
      y++;
    }
  }
  if (lines == 1)
    prms->gameinfo->score += 100;
  else if (lines == 2)
    prms->gameinfo->score += 300;
  else if (lines == 3)
    prms->gameinfo->score += 700;
  else if (lines == 4)
    prms->gameinfo->score += 1500;

  if (prms->gameinfo->level > 10)
    prms->gameinfo->level = 10;
  else
    prms->gameinfo->level = 1 + prms->gameinfo->score / 600;

  prms->gameinfo->speed = ((11 - prms->gameinfo->level) * 50000000);

  prms->state = SPAWN;
}

/**
 * @brief Функция удаления заполненной линии.
 *
 * Функция cдвигает все строки выше Y вниз на 1, затирая строку Y.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 * @param y Номер строки для удаления в матрице поля.
 */
void remove_full_line(Params_t *prms, int y) {
  for (; y >= 1; y--)
    for (int x = 0; x < X_FIELD; x++)
      prms->gameinfo->field[y][x] = prms->gameinfo->field[y - 1][x];

  for (int x = 0; x < X_FIELD; x++) prms->gameinfo->field[0][x] = 0;
}

/**
 * @brief Функция постановки игры на паузу.
 *
 * Функция ставит игру на паузу.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 */
void pause(Params_t *prms) {
  prms->state = PAUSE;
  prms->gameinfo->pause = 1;
}

/**
 * @brief Функция выводит игру из состояния паузы.
 *
 * Функция выводит игру из состояния паузы.
 * Засекает время выхода для последующего движения вниз.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 */
void unpause(Params_t *prms) {
  prms->gameinfo->pause = 0;
  timespec_get(&prms->start_time, TIME_UTC);
  prms->state = MOVING;
}

/**
 * @brief Функция окончания игры.
 *
 * Функция завершает игру при невозможности введения новой фигуры,
 * записывает новый рекорд и начинает сразу новую игру.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 */
void gameover(Params_t *prms) {
  hi_score(prms->gameinfo);
  prms->gameinfo->pause = 3;
  prms->state = START;
}

/**
 * @brief Функция выхода из игры.
 *
 * Функция записывает новый рекорд и завершает цикл игры.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 */
void exitstate(Params_t *prms) {
  hi_score(prms->gameinfo);
  prms->gameinfo->pause = -1;
}