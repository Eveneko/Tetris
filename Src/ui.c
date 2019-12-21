#include "ui.h"

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
    LCD_DrawRectangle(rx, ry, rx + cell_length, ry + cell_length);
    LCD_Fill(rx + 2, ry + 2, rx + (cell_length - 2), ry + (cell_length - 2), color);
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
        draw_block(it->x[block.rotation][i] +  block.x, it->y[block.rotation][i] + block.y, color, 0);
    }
}

void draw_next_block1(uint8_t clear)
{
	uint8_t i;
	shapes* it = &shape_list[block_next1.shape];
    uint16_t color = it->color;
    if(clear){color = WHITE;}
    for(i = 0; i < 4; i++)
	{
	    draw_block(it->x[block_next1.rotation][i], it->y[block_next1.rotation][i], color, 1);
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
	    draw_block(it->x[block_next2.rotation][i], it->y[block_next2.rotation][i], color, 2);
	}
}

void draw_name(char* name)
{
  LCD_ShowString(170, 210, 200, 16, 16, (uint8_t *)name);
}

void update_score(uint16_t score)
{
    char str_score[5];
    if (score > 9999)
        score = 9999;
    sprintf(str_score, "%6d", score);
    printf("score:%d str:%s\n", score, str_score);
    POINT_COLOR = BLACK;
    LCD_ShowString(sub_origin_x, sub_origin_y + 2 * sub_height + 160, screen_width, font_size, font_size, (uint8_t *)str_score);
}

void game_start()
{
    LCD_Clear(WHITE);
    POINT_COLOR = RED;
    LCD_ShowString(90, 60, 200, 24, 24, (uint8_t*) "Tetris");
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

void game_over(uint16_t score)
{
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
