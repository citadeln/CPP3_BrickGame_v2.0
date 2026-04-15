/**
 * @file
 * @brief Заголовочный файл функций конечного автомата.
 */

#ifndef FSM_H
#define FSM_H

#include "backend.h"
#include "types.h"

/**
 * @brief псевдоним для указателя на функцию
 */
typedef void (*function)(Params_t *prms);

function fsm_table(GameState_t state, UserAction_t action);

void spawn(Params_t *prms);
void move_down(Params_t *prms);
void rotate(Params_t *prms);
void move_left(Params_t *prms);
void move_right(Params_t *prms);
void fast_move_down(Params_t *prms);

int collide_while_moving(Params_t *prms, int y_shift, int x_shift);
int collide_while_rotating(Params_t *prms, int new_state);

void collide(Params_t *prms);
void remove_full_line(Params_t *prms, int y);

void pause(Params_t *prms);
void unpause(Params_t *prms);
void gameover(Params_t *prms);
void exitstate(Params_t *prms);

#endif  // FSM_H