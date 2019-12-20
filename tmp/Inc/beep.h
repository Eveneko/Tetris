#ifndef __BEEP__H
#define __BEEP__H

#include "lcd.h"
#include "button.h"
#include "game.h"
#include "grid.h"
#include "scene.h"
#include "parameter.h"
#include "ui.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

//定义GPIOB的位地址变量宏，位输入宏，输出宏
// #define PBeep PBout(5)

void BEEP_Init(void);
void Sound(uint16_t frq);
void Sound2(uint16_t time);
void play_music(void);
void play_successful(void);
void play_failed(void);

#endif
