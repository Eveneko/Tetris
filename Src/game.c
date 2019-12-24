#include "game.h"

uint16_t score;
uint32_t fall_down_interval; // time gap to fall down

uint32_t fall_task_stamp = 0;

void game_update() {
	if(uwTick - fall_task_stamp >= fall_down_interval){
		fall_task_stamp = uwTick;
		grid_fall_down();
	}
}


void grid_move_left(){
	if(left_shift_check()){
		draw_main_block(1);
		block.x -= 1;
		draw_main_block(0);
	}
}

void grid_move_right(){
	if(right_shift_check()){
		draw_main_block(1);
		block.x += 1;
		draw_main_block(0);
	}
}

void reset_grid_fall_speed(){
	fall_down_interval = INIT_SPEED;
}

void accelerate_fall_speed() {
    if(fall_down_interval > 200){
	    fall_down_interval -= STEP_SPEED;
    }
}

void grid_rotation(){
	if(rotation_check()){
		draw_main_block(1);
		block.direction = (block.direction + 1)%4;
		draw_main_block(0);
	}
}

uint8_t game_fail_check() {
	if (intersection_check()){return 1;}
	return 0;
}

void grid_quick_fall_down() {
	while (down_shift_check()) {
		draw_main_block(1);
		block.y -= 1;
		draw_main_block(0);
		// let fall down visible
		HAL_Delay(50);
	}
}

void grid_fall_down() {
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	// if the grid can fall down
	if(down_shift_check()){
		draw_main_block(1);
		block.y -= 1;
		draw_main_block(0);
	} else {
		set_block_static();
		uint8_t row_num = delete_full_lines();
		if(row_num){
			score += row_num * row_num;
			update_score(score);
			// green light will on
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			for (uint8_t i = 0; i < row_num; i++) {
				accelerate_fall_speed();
			}
			update_level();
			grid_render();
		}
		// place new block
		draw_next_block1(1);
		draw_next_block2(1);

		block_reset();

		if(!game_fail_check()){
			draw_main_block(0);
			draw_next_block1(0);
			draw_next_block2(0);
		} else{
			// game over
			game_over(score);
		}
	}
}

void game_reset() {

	grid_init();
	sub_grid_init();
	block_init();

	score=0;
	reset_grid_fall_speed();

	// reset all display
	draw_background();
	update_score(score);
	draw_main_block(0);
	draw_next_block1(0);
	draw_next_block2(0);
}
