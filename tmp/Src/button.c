#include "button.h"

uint8_t button_state[BUTTON_NUM];
uint8_t button_prev_state[BUTTON_NUM];
uint32_t button_update_interval; // time gap to update button
uint32_t button_task_stamp;

void button_init() {
	for(uint8_t i=0; i<BUTTON_NUM; i++){
		button_state[i]=0;
		button_prev_state[i]=0;
	}
	button_update_interval = 20;
	button_task_stamp = 0;
}


void button_update() {
	for(uint8_t i=0; i<BUTTON_NUM; i++){
		button_prev_state[i]=button_state[i];
	}
	// left
	if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_SET) {
		button_state[0]=1;
	} else {
		button_state[0]=0;
	}
	// right
	if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_SET) {
		button_state[1]=1;
	} else {
		button_state[1]=0;
	}
	if (HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin) == GPIO_PIN_SET) {
		button_state[2]=1;
	} else {
		button_state[2]=0;
	}

	button_action();
}

void button_action() {
	if (button_state[0]==1 && button_prev_state[0]==0) {
		if (button_prev_state[2]==0) {
			// left shift
			grid_move_left();
		} else {
			// rotate;
			grid_rotation();
		}
	}
	if (button_state[1]==1 && button_prev_state[1]==0) {
		if (button_prev_state[2]==0) {
			// right shift
			grid_move_right();
		} else {
			// goto floor instantly;
			grid_quick_fall_down();
		}
	}
}