#include "block.h"

block_config block;
block_config block_next1;
block_config block_next2;
shapes shape_list[SHAPE_NUM];

extern uint32_t uwTick;

uint8_t left_shift_check() {
	int8_t x, y, i;
	shapes* it = &shape_list[block.shape];
	int8_t dir = block.direction;
	int8_t cell_num =it->cell_num;
	for(i=0;i<cell_num;i++)
	{
		x = it->x[dir][i]+block.x;
		y = it->y[dir][i]+block.y;
		x--;
		if (cell_occupation_check(x,y)== 1) {
			return 0;
		}
	}
	return 1;
}

uint8_t right_shift_check() {
	int8_t x, y, i;
	shapes* it = &shape_list[block.shape];
	int8_t dir = block.direction;
	int8_t cell_num =it->cell_num;
	for(i=0;i<cell_num;i++)
	{
		x = it->x[dir][i]+block.x;
		y = it->y[dir][i]+block.y;
		x++;
		if (cell_occupation_check(x,y)== 1) {
			return 0;
		}
	}
	return 1;
}

uint8_t down_shift_check() {
	int8_t x, y, i;
	shapes* it = &shape_list[block.shape];
	int8_t dir = block.direction;
	int8_t cell_num =it->cell_num;
	for(i=0;i<cell_num;i++)
	{
		x = it->x[dir][i]+block.x;
		y = it->y[dir][i]+block.y;
		y--;
		if (cell_occupation_check(x,y)== 1) {
			return 0;
		}
	}
	return 1;
}

uint8_t rotation_check() {
	int8_t x, y, i;
	shapes* it = &shape_list[block.shape];
	int8_t dir = (block.direction+1)%4;
	int8_t cell_num =it->cell_num;
	for(i=0;i<cell_num;i++)
	{
		x = it->x[dir][i]+block.x;
		y = it->y[dir][i]+block.y;
		if (cell_occupation_check(x,y)== 1) {
			return 0;
		}
    }
	return 1;
}

// intersect with other : 1
uint8_t intersection_check() {
	int8_t x, y, i;
	shapes* it = &shape_list[block.shape];
	int8_t dir = block.direction;
	int8_t cell_num =it->cell_num;
	for(i=0;i<cell_num;i++)
	{
		x = it->x[dir][i]+block.x;
		y = it->y[dir][i]+block.y;
		if (cell_occupation_check(x,y) == 1) {
			return 1;
		}
    }
	return 0;
}

// occupied 1
// not occupied 0
uint8_t cell_occupation_check(int8_t x, int8_t y){
	if(x>=GRID_WIDTH || x<0 || y>=GRID_HEIGHT || y<0 || !grid[y][x].isEmpty) {return 1;}
	return 0;
}

void set_block_static(){
	uint8_t i,a,b;
	shapes* it = &shape_list[block.shape];
	for(i=0;i<4;i++)
	{
	  a = it->x[block.direction][i]+block.x;
	  b = it->y[block.direction][i]+block.y;
		grid[b][a].isEmpty = 0;
		grid[b][a].color = it->color;
	}
}



void block_reset() {
    block = block_next1;
    block_next1 = block_next2;
    srand(uwTick*rand());
    block_next2 = (block_config){
        .x = 3, .y = GRID_HEIGHT-4,
        .shape = rand()%SHAPE_NUM,
        .direction = rand()%4
    };
}

void block_init() {
    srand(uwTick*rand());
    uint8_t i;
    for(i = 0; i < 3; i++){
      block_reset();
    }

    shape_list[SHAPE_O] = (shapes){
        .x={{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1}},
        .y={{3,3,2,2},{3,3,2,2},{3,3,2,2},{3,3,2,2}},
        .color = BLUE,
    	.cell_num = 4};
    shape_list[SHAPE_S] = (shapes){
        .x={{0,1,1,2},{1,1,2,2},{0,1,1,2},{1,1,2,2}},
        .y={{2,2,3,3},{3,2,2,1},{2,2,3,3},{3,2,2,1}},
		.color = GREEN,
		.cell_num = 4};
    shape_list[SHAPE_T] = (shapes){
        .x={{0,1,1,2},{1,2,2,2},{0,1,1,2},{0,0,0,1}},
        .y={{3,3,2,3},{2,3,2,1},{2,3,2,2},{1,2,3,2}},
		.color = YELLOW,
		.cell_num = 4};
    shape_list[SHAPE_Z] = (shapes){
        .x={{0,1,1,2},{0,0,1,1},{0,1,1,2},{0,0,1,1}},
        .y={{3,3,2,2},{1,2,2,3},{3,3,2,2},{1,2,2,3}},
		.color = BROWN,
		.cell_num = 4};
    shape_list[SHAPE_I] = (shapes){
        .x={{0,1,2,3},{2,2,2,2},{0,1,2,3},{2,2,2,2}},
        .y={{3,3,3,3},{0,1,2,3},{3,3,3,3},{0,1,2,3}},
		.color = RED,
		.cell_num = 4};
    shape_list[SHAPE_J] = (shapes){
        .x={{0,1,2,2},{0,1,1,1},{0,0,1,2},{1,1,1,2}},
        .y={{3,3,3,2},{1,1,2,3},{3,2,2,2},{1,2,3,3}},
		.color = BRRED,
		.cell_num = 4};
    shape_list[SHAPE_L] = (shapes){
        .x={{0,0,1,2},{1,2,2,2},{0,1,2,2},{1,1,1,2}},
        .y={{2,3,3,3},{3,3,2,1},{2,2,2,3},{3,2,1,1}},
		.color = GRAY,
		.cell_num = 4};

}
