#ifndef __GRID_H
#define __GRID_H
#include "lcd.h"
#include "block.h"
#include "button.h"
#include "game.h"
#include "scene.h"
#include "parameter.h"
#include "ui.h"

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

typedef struct
{
	uint8_t isEmpty;
	uint16_t color;
} cell;

extern cell grid[GRID_HEIGHT][GRID_WIDTH];
extern cell sub_grid[4][4];

uint8_t delete_full_lines();
void clear_row(uint8_t line);
void grid_init();
void sub_grid_init();

#endif