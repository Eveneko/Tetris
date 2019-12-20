#include "grid.h"

cell grid[GRID_HEIGHT][GRID_WIDTH];
cell sub_grid[4][4];

// find the line that is full and delete, return the number of lines deleted.
uint8_t delete_full_lines() {
	int8_t x, y;
	uint8_t row_num = 0;

	// from up to down, for every row, delete the full row.
	for (y = GRID_HEIGHT ; y >=0 ; y--) {
		for (x = 0; x < GRID_WIDTH; x++){
			if(grid[y][x].isEmpty) {
				break;
			}
			if(x == GRID_WIDTH-1){
				clear_row(y);
				row_num ++;
			}
		}
	}

	return row_num;
}

void clear_row(uint8_t line) {
	uint8_t x, y;
    //shift from upper to downer
    for (y = line; y < GRID_HEIGHT-1; y++) {
        for (x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = grid[y+1][x];
        }
    }
    for (x = 0; x < GRID_WIDTH; x++) {
    	grid[GRID_HEIGHT-1][x].isEmpty = 1;
	}
}

void grid_init(){
	uint8_t x, y;
    for (y = 0; y < GRID_HEIGHT; y++) {
        for (x = 0; x < GRID_WIDTH; x++) {
            grid[y][x].isEmpty = 1;
		}
    }
}

void sub_grid_init(){
	uint8_t x, y;
    for (y = 0; y < 4; y++) {
        for (x = 0; x < 4; x++) {
            sub_grid[y][x].isEmpty=1;
        }
    }
}