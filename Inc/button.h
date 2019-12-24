#ifndef __BUTTON_H
#define __BUTTON_H
#include "lcd.h"
#include "block.h"
#include "game.h"
#include "grid.h"
#include "ui.h"
#include "main.h"

#define BUTTON_NUM 3
#define BUTTON_BUFFER_SIZE 4
extern uint32_t button_update_interval; // time gap to update button
extern uint32_t button_task_stamp;

void button_init();
void button_update();
void button_action();
extern uint32_t uwTick;

#endif
