#include "ui.h"

uint8_t pattern = 0;

void draw_background()
{
    POINT_COLOR = BLACK;
    //main window
    LCD_DrawRectangle(main_origin_x - 1, main_origin_y - 1, main_origin_x + main_width + 1, main_origin_y + main_height + 1);
    //sub window
    LCD_DrawRectangle(sub_origin_x - 1, sub_origin_y - 1, sub_origin_x + sub_width + 1, sub_origin_y + sub_height + 1);
    LCD_ShowString(sub_origin_x, sub_origin_y + sub_height + 10, screen_width, font_size, (uint8_t) font_size, (uint8_t *) "1st Next");
    LCD_DrawRectangle(sub_origin_x - 1, sub_origin_y + sub_height + font_size + 20 - 1, sub_origin_x + sub_width + 1, sub_origin_y + 2 * sub_height + font_size + 20 + 1);
    LCD_ShowString(sub_origin_x, sub_origin_y + 2 * sub_height + font_size + 30, screen_width, font_size, (uint8_t) font_size, (uint8_t *) "2nd Next");
    LCD_ShowString(sub_origin_x, sub_origin_y + 2 * sub_height + 80, screen_width, font_size, (uint8_t) font_size, (uint8_t *) "Level:");
    LCD_ShowString(sub_origin_x, sub_origin_y + 2 * sub_height + 100, screen_width, font_size, (uint8_t) font_size, (uint8_t *) "Easy1");
    LCD_ShowString(sub_origin_x, sub_origin_y + 2 * sub_height + 140, screen_width, font_size, (uint8_t) font_size, (uint8_t *) "Score:");
    LCD_ShowString(sub_origin_x, sub_origin_y + 2 * sub_height + 160, screen_width, font_size, (uint8_t) font_size, (uint8_t *) "     0");
    
    // LCD_ShowString(170, 150, 200, 16, 16, (uint8_t*) "Time:");
    // extern uint32_t uwTick;
    // char msg[30];
	// sprintf(msg, "%ds", (int) uwTick/1000);
    // LCD_ShowString(170, 170, 200, 16, 16, (uint8_t*) msg);

    // LCD_Fill(10,10,25,25,BLUE);
}

void fill_main_background(uint16_t color)
{
    LCD_Fill(main_origin_x, main_origin_y, main_origin_x + main_width, main_origin_y + main_height, color);
}

void fill_sub_background(uint16_t color)
{
    LCD_Fill(sub_origin_x, sub_origin_y, sub_origin_x + sub_width, sub_origin_y + sub_height, color);
    LCD_Fill(sub_origin_x, sub_origin_y + sub_height + font_size + 20, sub_origin_x + sub_width, sub_origin_y + 2 * sub_height + font_size + 20, color);
}

void fill_all_white()
{
    fill_main_background(WHITE);
    fill_sub_background(WHITE);
}

/*
* mode
* 0: main
* 1: next1
* 2: next2
*/
void draw_block(uint8_t x, uint8_t y, uint16_t color, uint8_t mode)
{
    uint16_t rx, ry;

    if (mode == 0)
    {
        rx = main_origin_x + x * cell_length;
        ry = main_origin_y + main_height - (y+1) * cell_length;
    }
    else if(mode == 1)
    {
        rx = sub_origin_x + x * cell_length;
        ry = sub_origin_y + sub_height - (y+1) * cell_length;
    }else if(mode == 2)
    {
        rx = sub_origin_x + x * cell_length;
        ry = sub_origin_y + 2 * sub_height + font_size + 20 - (y+1) * cell_length;
    }
    POINT_COLOR = color;
    LCD_DrawRectangle(rx, ry, rx + cell_length - 1, ry + cell_length - 1);
    LCD_Fill(rx + 2, ry + 2, rx + (cell_length - 2), ry + (cell_length - 2), color);
    if(color != WHITE){
        POINT_COLOR = BLACK;
    }
    if(pattern == 1){
        // bilibili
        LCD_DrawRectangle(rx + 11, ry + 12, rx + 4, ry + 5);
        LCD_DrawPoint(rx + 7, ry + 8);
        LCD_DrawPoint(rx + 9, ry + 8);
        LCD_DrawPoint(rx + 8, ry + 10);
        LCD_DrawPoint(rx + 7, ry + 4);
        LCD_DrawPoint(rx + 7, ry + 3);
        LCD_DrawPoint(rx + 9, ry + 4);
        LCD_DrawPoint(rx + 9, ry + 3);
    }
    if(pattern == 2){
        // shadow
        LCD_DrawPoint(rx + (cell_length - 2 - 1), ry + (cell_length - 2 - 1));
        LCD_DrawLine(rx + (cell_length - 2 - 1), ry + (cell_length - 2 - 5), rx + (cell_length - 2 - 5), ry + (cell_length - 2 - 1));
        LCD_DrawLine(rx + (cell_length - 2 - 1), ry + (cell_length - 2 - 7), rx + (cell_length - 2 - 7), ry + (cell_length - 2 - 1));
    }
}

void grid_render()
{
    int8_t x, y;
    cell _cell;
    for (y = GRID_HEIGHT-1; y >= 0; y--)
    {
        for (x = 0; x < GRID_WIDTH; x++)
        {
            _cell = grid[y][x];
            if (_cell.isEmpty){
              draw_block(x, y, WHITE, 0);
            } else {
              draw_block(x, y, _cell.color, 0);
            }
        }
    }
}

/*
* mode
* 1: next1
* 2: next2
*/
void sub_grid_render(uint8_t mode)
{
    uint8_t x, y;
    cell _cell;
    for (y = 0; y < 4; y++)
    {
        for (x = 0; x < 4; x++)
        {
            _cell = sub_grid[y][x];
            if (_cell.isEmpty)
                continue;
            draw_block(x, y, _cell.color, mode);
        }
    }
}

void draw_main_block(uint8_t clear)
{
    uint8_t i;
    shapes* it = &shape_list[block.shape];
    uint16_t color = it->color;
    if(clear){color = WHITE;}
    for(i = 0; i < 4; i++)
    {
        draw_block(it->x[block.direction][i] +  block.x, it->y[block.direction][i] + block.y, color, 0);
    }
    // draw_main_grid();
}

void draw_next_block1(uint8_t clear)
{
	uint8_t i;
	shapes* it = &shape_list[block_next1.shape];
    uint16_t color = it->color;
    if(clear){color = WHITE;}
    for(i = 0; i < 4; i++)
	{
	    draw_block(it->x[block_next1.direction][i], it->y[block_next1.direction][i], color, 1);
	}
}

void draw_next_block2(uint8_t clear)
{
	uint8_t i;
	shapes* it = &shape_list[block_next2.shape];
    uint16_t color = it->color;
    if(clear){color = WHITE;}
    for(i = 0; i < 4; i++)
	{
	    draw_block(it->x[block_next2.direction][i], it->y[block_next2.direction][i], color, 2);
	}
}

void draw_name(char* name)
{
  LCD_ShowString(170, 210, 200, 16, 16, (uint8_t *)name);
}

void draw_main_grid()
{
    POINT_COLOR = LGRAY;
    uint8_t i;
    for(i = 1; i < 10; i++){
        LCD_DrawLine(main_origin_x + i * cell_length, main_origin_y , main_origin_x + i * cell_length, main_origin_y + main_height);
    }
    for(i = 1; i < 20; i++){
        LCD_DrawLine(main_origin_x, main_origin_y + i * cell_length, main_origin_x + main_width, main_origin_y + i * cell_length);
    }
}

void update_score(uint16_t score)
{
    char str_score[5];
    if (score > 9999)
        score = 9999;
    sprintf(str_score, "%6d", score);
    printf("score:%d str:%s\n", score, str_score);
    POINT_COLOR = BLACK;
    LCD_Fill(sub_origin_x, sub_origin_y + 2 * sub_height + 160, sub_origin_x + screen_width, sub_origin_y + 2 * sub_height + 160 + font_size, WHITE);
    LCD_ShowString(sub_origin_x, sub_origin_y + 2 * sub_height + 160, screen_width, font_size, font_size, (uint8_t *)str_score);

}

void update_level()
{
    uint8_t level = (INIT_SPEED -  fall_down_interval) / STEP_SPEED;
    char level_msg[32];
    if(level / 5 == 0){
        sprintf(level_msg, "Easy%d", level%5+1);
        LCD_Fill(sub_origin_x, sub_origin_y + 2 * sub_height + 100, sub_origin_x + screen_width, sub_origin_y + 2 * sub_height + 100 + font_size, WHITE);
        LCD_ShowString(sub_origin_x, sub_origin_y + 2 * sub_height + 100, screen_width, font_size, font_size, (uint8_t *)level_msg);
    }
    else if(level / 5 == 1){
        sprintf(level_msg, "Medium%d", level%5+1);
        LCD_Fill(sub_origin_x, sub_origin_y + 2 * sub_height + 100, sub_origin_x + screen_width, sub_origin_y + 2 * sub_height + 100 + font_size, WHITE);
        LCD_ShowString(sub_origin_x, sub_origin_y + 2 * sub_height + 100, screen_width, font_size, font_size, (uint8_t *)level_msg);
    }
    else if(level / 5 == 2){
        sprintf(level_msg, "Hard%d", level%5+1);
        LCD_Fill(sub_origin_x, sub_origin_y + 2 * sub_height + 100, sub_origin_x + screen_width, sub_origin_y + 2 * sub_height + 100 + font_size, WHITE);
        LCD_ShowString(sub_origin_x, sub_origin_y + 2 * sub_height + 100, screen_width, font_size, font_size, (uint8_t *)level_msg);
    }
    else{
        sprintf(level_msg, "NB");
        LCD_Fill(sub_origin_x, sub_origin_y + 2 * sub_height + 100, sub_origin_x + screen_width, sub_origin_y + 2 * sub_height + 100 + font_size, WHITE);
        LCD_ShowString(sub_origin_x, sub_origin_y + 2 * sub_height + 100, screen_width, font_size, font_size, (uint8_t *)level_msg);
    }
}

void game_start()
{
    LCD_Clear(WHITE);
    LCD_ShowImage();
    POINT_COLOR = LGRAYBLUE;
    LCD_Fill(30, 230, 210, 246, BLACK);
    LCD_ShowString(30, 230, 210, 16, 16, (uint8_t*) "Press any key to start.");
    while(1){
        HAL_Delay(21);
        if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) != GPIO_PIN_SET) {break;}
        if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) != GPIO_PIN_SET) {break;}
        if (HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin) != GPIO_PIN_RESET) {break;}
    }
    // LCD_Clear(WHITE);
    choose_pattern();
    game_reset();
}

void choose_pattern()
{
    LCD_Clear(BLACK);
    // LCD_Fill(0, 0, screen_width, screen_height, BLACK);
    POINT_COLOR = LGRAYBLUE;
    LCD_ShowString(20, 40, 200, 16, 16, (uint8_t*) "Choose the block pattern:");
    POINT_COLOR = LGRAY;
    LCD_ShowString(80, 100, 200, 24, 24, (uint8_t*) "Normal");
    LCD_ShowString(70, 150, 200, 24, 24, (uint8_t*) "Bilibili");
    LCD_ShowString(80, 200, 200, 24, 24, (uint8_t*) "Shadow");
    POINT_COLOR = LGRAYBLUE;
    LCD_ShowString(10, 270, 240, 12, 12, (uint8_t*) "WK_UP: OK  |  KEY1: <-  |  KEY0: ->");
    POINT_COLOR = YELLOW;
    LCD_DrawRectangle(70, 100, 160, 124);
    // LCD_DrawRectangle(60, 150, 170, 174);
    // LCD_DrawRectangle(70, 200, 160, 224);
    HAL_Delay(1000);
    while(1){
        HAL_Delay(200);
        if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) != GPIO_PIN_SET) {
            if(pattern == 1){
                POINT_COLOR = BLACK;
                LCD_DrawRectangle(60, 150, 170, 174);
                POINT_COLOR = YELLOW;
                LCD_DrawRectangle(70, 100, 160, 124);
                pattern = 0;
            }
            else if(pattern == 2){
                POINT_COLOR = BLACK;
                LCD_DrawRectangle(70, 200, 160, 224);
                POINT_COLOR = YELLOW;
                LCD_DrawRectangle(60, 150, 170, 174);
                pattern = 1;
            }
            else if(pattern == 0)
            {
                POINT_COLOR = BLACK;
                LCD_DrawRectangle(70, 100, 160, 124);
                POINT_COLOR = YELLOW;
                LCD_DrawRectangle(70, 200, 160, 224);
                pattern = 2;
            }            
            continue;
        }
        if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) != GPIO_PIN_SET) {
            if(pattern == 0){
                POINT_COLOR = BLACK;
                LCD_DrawRectangle(70, 100, 160, 124);
                POINT_COLOR = YELLOW;
                LCD_DrawRectangle(60, 150, 170, 174);
                pattern = 1;
            }
            else if(pattern == 1){
                POINT_COLOR = BLACK;
                LCD_DrawRectangle(60, 150, 170, 174);
                POINT_COLOR = YELLOW;
                LCD_DrawRectangle(70, 200, 160, 224);
                pattern = 2;
            }
            else if(pattern == 2){
                POINT_COLOR = BLACK;
                LCD_DrawRectangle(70, 200, 160, 224);
                POINT_COLOR = YELLOW;
                LCD_DrawRectangle(70, 100, 160, 124);
                pattern = 0;
            }
            continue;
        }
        if (HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin) != GPIO_PIN_RESET) {break;}
    }
    LCD_Clear(WHITE);
}

void game_over(uint16_t score)
{
    POINT_COLOR = BRRED;
    LCD_ShowString(10, 100, 200, 24, 24, (uint8_t*) "Stackoverflow!");
    uint8_t i;
    for(i = 0; i < 10; i++){
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        HAL_Delay(100);
    }
    LCD_Clear(BLACK);
    // LCD_ShowImage();
    POINT_COLOR = RED;
    LCD_ShowString(60, 60, 200, 24, 24, (uint8_t*) "Game Over!");
    char msg[30];
    sprintf(msg, "Your score is %4d", score);
    LCD_ShowString(40, 100, 200, 16, 16, (uint8_t*) msg);
    POINT_COLOR = LGRAY;
    LCD_ShowString(20, 160, 200, 16, 16, (uint8_t*) "Press any key to continue.");
    while(1){
        HAL_Delay(21);
        if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) != GPIO_PIN_SET) {break;}
        if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) != GPIO_PIN_SET) {break;}
        if (HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin) != GPIO_PIN_RESET) {break;}
        // bling
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        HAL_Delay(100);
    }
    LCD_Clear(WHITE);
    game_reset();
}
