#include "button.h"

uint8_t button_state_buffer[BUTTON_NUM][BUTTON_BUFFER_SIZE];
uint32_t button_update_interval; // time gap to update button
uint32_t button_task_stamp;

void button_init() {
	for(uint8_t i=0; i<BUTTON_NUM; i++){
		for(uint8_t j=BUTTON_BUFFER_SIZE-1; j>=0; j--) {
			button_state_buffer[i][j]=0;
		}
	}
	button_update_interval = 40;
	button_task_stamp = 0;
}

void button_update() {
	if(uwTick - button_task_stamp >= button_update_interval){
		button_task_stamp = uwTick;
		read_button();
	}
}


void read_button() {
	for(uint8_t i=0; i<BUTTON_NUM; i++){
		for(uint8_t j=BUTTON_BUFFER_SIZE-1; j>=1; j--) {
			button_state_buffer[i][j]=button_state_buffer[i][j-1];
		}
	}
	// left
	if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_SET) {
		button_state_buffer[0][0]=1;
	} else {
		button_state_buffer[0][0]=0;
	}
	// right
	if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_SET) {
		button_state_buffer[1][0]=1;
	} else {
		button_state_buffer[1][0]=0;
	}
	if (HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin) == GPIO_PIN_SET) {
		button_state_buffer[2][0]=1;
	} else {
		button_state_buffer[2][0]=0;
	}

	button_action();
}

void button_action() {
	if (button_state_buffer[0][0]==0 && button_state_buffer[0][1]==1 && button_state_buffer[0][2]==1 && button_state_buffer[0][3]==1) {
		if (button_state_buffer[2][0]==0 && button_state_buffer[2][1]==0 && button_state_buffer[2][2]==0 && button_state_buffer[2][3]==0) {
			// left shift
			grid_move_left();
		} else {
			// rotate;
			grid_rotation();
		}
	}
	if (button_state_buffer[1][0]==0 && button_state_buffer[1][1]==1 && button_state_buffer[1][2]==1 && button_state_buffer[1][3]==1) {
		if (button_state_buffer[2][0]==0 && button_state_buffer[2][1]==0 && button_state_buffer[2][2]==0 && button_state_buffer[2][3]==0) {
			// right shift
			grid_move_right();
		} else {
			// goto floor instantly;
			grid_quick_fall_down();
		}
	}
	// if wk_up is pressed, red light will on
	if (button_state_buffer[2][0]==0 && button_state_buffer[2][1]==0 && button_state_buffer[2][2]==0 && button_state_buffer[2][3]==0) {
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
	}
}
