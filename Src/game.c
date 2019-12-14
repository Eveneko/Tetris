#include "game.h"

uint16_t score;
uint32_t fall_down_interval; // time gap to fall down
uint32_t fall_task_stamp = 0;

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
		block.rotation = (block.rotation + 1)%4;
		draw_main_block(0);
	}
}

uint8_t game_fail_check() {
	int8_t a, b;
	uint8_t i;
	shapes* it = &shape_list[block.shape];
	for(i = 0; i < 4; i++) {
		a = it->x[block.rotation][i]+block.x;
		b = it->y[block.rotation][i]+block.y;
		if(!grid[b][a].isEmpty) {return 1;}
	}
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
	// if the grid can fall down
	if(down_shift_check()){
		draw_main_block(1);
		block.y -= 1;
		draw_main_block(0);
	} else {
		flush_block();
		uint8_t row_num = delete_full_lines();
		if(row_num){
			score += row_num;
			update_score(score);
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
			// loading
			LCD_Clear(WHITE);
			POINT_COLOR = RED;
			LCD_ShowString(60, 60, 200, 24, 24, (uint8_t*) "Game Over!");
			char msg[30];
			sprintf(msg, "Your score is %4d", score);
			LCD_ShowString(40, 100, 200, 16, 16, (uint8_t*) msg);
			POINT_COLOR = BLACK;
			LCD_ShowString(20, 160, 200, 16, 16, (uint8_t*) "Press any key to continue.");
			while(1){
				HAL_Delay(21);
				if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) != GPIO_PIN_SET) {break;}
				if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) != GPIO_PIN_SET) {break;}
				if (HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin) != GPIO_PIN_RESET) {break;}
			}
			LCD_Clear(WHITE);
			game_reset();
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