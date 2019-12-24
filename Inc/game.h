#ifndef __GAME_H
#define __GAME_H
#include "lcd.h"
#include "block.h"
#include "button.h"
#include "grid.h"
#include "ui.h"

#define INIT_SPEED 500
#define STEP_SPEED 20 

extern uint32_t fall_down_interval; // time gap to fall down
extern uint32_t fall_task_stamp;

void grid_move_left();
void grid_move_right();
void reset_grid_fall_speed();
void accelerate_fall_speed();
void grid_rotation();
uint8_t game_fail_check();
void grid_quick_fall_down();
void grid_fall_down();
void game_reset();
void game_update();
extern uint32_t uwTick;

#endif
