#ifndef __BUTTON_H
#define __BUTTON_H
#include "lcd.h"
#include "block.h"
#include "game.h"
#include "grid.h"
#include "scene.h"
#include "parameter.h"
#include "ui.h"
#include "main.h"

#define BUTTON_NUM 3

extern uint32_t button_update_interval; // time gap to update button
extern uint32_t button_task_stamp;

void button_init();
void button_update();

#endif