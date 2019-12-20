#ifndef __UI_H
#define __UI_H
#include "lcd.h"
#include "block.h"
#include "button.h"
#include "game.h"
#include "grid.h"
#include "scene.h"
#include "parameter.h"

#define screen_height 320
#define screen_width 240

#define main_origin_x 10
#define main_origin_y 10
#define main_height 300
#define main_width 150

#define sub_origin_x 170
#define sub_origin_y 10
#define sub_height 60
#define sub_width 60

#define font_size 16

#define cell_length 15

void draw_background();
void fill_main_background(uint16_t color);
void fill_sub_background(uint16_t color);
void fill_all_white();
void draw_block(uint8_t x, uint8_t y, uint16_t color, uint8_t mode);
void grid_render();
void sub_grid_render(uint8_t mode);
void draw_main_block(uint8_t clear);
void draw_next_block1(uint8_t clear);
void draw_next_block2(uint8_t clear);
void draw_name(char* name);
void update_score(uint16_t score);

#endif
