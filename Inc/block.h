#ifndef __BLOCK_H
#define __BLOCK_H
#include "lcd.h"
#include "button.h"
#include "game.h"
#include "grid.h"
#include "ui.h"
#include<time.h>

typedef struct{
    uint8_t x[4][4];
    uint8_t y[4][4];
    uint16_t color;
} shapes;

typedef struct{
	int8_t x;
	int8_t y;
	uint8_t shape;
	uint8_t rotation;
} block_config;


#define SHAPE_I 0
#define SHAPE_J 1
#define SHAPE_L 2
#define SHAPE_O 3
#define SHAPE_S 4
#define SHAPE_T 5
#define SHAPE_Z 6
#define SHAPE_NUM 7

extern block_config block;
extern block_config block_next1;
extern block_config block_next2;
extern shapes shape_list[SHAPE_NUM];

uint8_t left_shift_check(void);
uint8_t right_shift_check(void);
uint8_t down_shift_check(void);
uint8_t rotation_check(void);
void flush_block(void);
void block_reset(void);
void block_init(void);


#endif
