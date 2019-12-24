#include "block.h"

block_config block;
block_config block_next1;
block_config block_next2;
shapes shape_list[SHAPE_NUM];

extern uint32_t uwTick;

uint8_t left_shift_check() {
	int8_t a, b;
	uint8_t i;
	shapes* it = &shape_list[block.shape];
	for(i = 0; i < 4; i++)
	{
	  a = it->x[block.rotation][i]+block.x - 1;
	  b = it->y[block.rotation][i]+block.y;
		if(a < 0 || !grid[b][a].isEmpty)
		{
			return 0;
		}
    }
  return 1;
}

uint8_t right_shift_check() {
	int8_t a, b;
	uint8_t i;
	shapes* it = &shape_list[block.shape];
	for(i = 0; i < 4; i++)
	{
	  a = it->x[block.rotation][i]+block.x + 1;
	  b = it->y[block.rotation][i]+block.y;
		if(a >= GRID_WIDTH || !grid[b][a].isEmpty) {return 0;}
    }
  return 1;
}

uint8_t down_shift_check() {
	int8_t a, b;
	uint8_t i;
	shapes* it = &shape_list[block.shape];
	for(i = 0; i < 4; i++)
	{
	  a = it->x[block.rotation][i]+block.x;
	  b = it->y[block.rotation][i]+block.y - 1;
		if(b < 0 || !grid[b][a].isEmpty ) {return 0;}
    }
  return 1;
}

uint8_t rotation_check() {
	int8_t a, b, rot;
	uint8_t i;
	shapes* it = &shape_list[block.shape];
	for(i=0;i<4;i++)
	{
    rot = (block.rotation+1)%4;
	  a = it->x[rot][i]+block.x;
	  b = it->y[rot][i]+block.y;
    if(a>=GRID_WIDTH || a<0 || b>=GRID_HEIGHT || b<0 || !grid[b][a].isEmpty) {return 0;}
    }
  return 1;
}

void flush_block(){
	uint8_t i,a,b;
	shapes* it = &shape_list[block.shape];
	for(i=0;i<4;i++)
	{
	  a = it->x[block.rotation][i]+block.x;
	  b = it->y[block.rotation][i]+block.y;
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
        .rotation = rand()%4
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
            .color = BLUE };
    shape_list[SHAPE_S] = (shapes){
        .x={{0,1,1,2},{1,1,2,2},{0,1,1,2},{1,1,2,2}},
        .y={{2,2,3,3},{3,2,2,1},{2,2,3,3},{3,2,2,1}},
            .color = GREEN };
    shape_list[SHAPE_T] = (shapes){
        .x={{0,1,1,2},{1,2,2,2},{0,1,1,2},{0,0,0,1}},
        .y={{3,3,2,3},{2,3,2,1},{2,3,2,2},{1,2,3,2}},
            .color = YELLOW };
    shape_list[SHAPE_Z] = (shapes){
        .x={{0,1,1,2},{0,0,1,1},{0,1,1,2},{0,0,1,1}},
        .y={{3,3,2,2},{1,2,2,3},{3,3,2,2},{1,2,2,3}},
            .color = BROWN };
    shape_list[SHAPE_I] = (shapes){
        .x={{0,1,2,3},{2,2,2,2},{0,1,2,3},{2,2,2,2}},
        .y={{3,3,3,3},{0,1,2,3},{3,3,3,3},{0,1,2,3}},
            .color = RED };
    shape_list[SHAPE_J] = (shapes){
        .x={{0,1,2,2},{0,1,1,1},{0,0,1,2},{1,1,1,2}},
        .y={{3,3,3,2},{1,1,2,3},{3,2,2,2},{1,2,3,3}},
            .color = BRRED };
    shape_list[SHAPE_L] = (shapes){
        .x={{0,0,1,2},{1,2,2,2},{0,1,2,2},{1,1,1,2}},
        .y={{2,3,3,3},{3,3,2,1},{2,2,2,3},{3,2,1,1}},
            .color = GRAY };

}
