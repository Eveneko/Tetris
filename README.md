# Tetris

## Environment

- STM32F103RC
- STM32CubeMX
- PlatformIO

## Structure

- Src  
├── block.c  
├── bmp_background.c  
├── button.c  
├── game.c  
├── grid.c  
├── lcd.c  
├── main.c  
├── parameter.c  
├── scene.c  
├── stm32f1xx_hal_msp.c  
├── stm32f1xx_it.c  
├── syscalls.c  
├── sysmem.c  
├── system_stm32f1xx.c  
└── ui.c  

## Feature

- Draw at least 7 types of blocks with different colors;
- Rotate and move the blocks left and right by keys;
- A line desappears when the whole lines are all filled;
- Show the scores and at least 2 blocks will soon drop;
- At least 3 rounds, for the blocks drops faster when level up;
- Effects of interface and operating;
- Key combinations control Tetris behavior;
- Nice backgroud;
- block pattern choose.

## Run

- `KEY1`: move left
- `KEY0`: move right
- `WAKE_UP + KEY1`: rotate
- `WAKE_UP + KEY0`: fall down fast
- Press any key to start

## Test

- Game start  
  ![gamestart](./images/gamestart.jpg)  
- Bloack pattern choose
  ![pattern](./images/pattern.jpg)  
- Gaming  
  ![gaming](./images/gaming.jpg)  
- Game over  
  ![gameover](./images/gameover.jpg)  
