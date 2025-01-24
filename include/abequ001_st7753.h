#include "graph.h"
#include "timerISR.h"
#include "spiAVR.h"
#include "periph.h"
#include "helper.h"
#include "sprites.h"

#ifndef ST7753_H
#define ST7753_H

void Send_Command(int num) {
  PORTD = SetBit(PORTD, 6, 0);
  PORTD = SetBit(PORTD, 7, 0);
  SPI_SEND(num);
  PORTD = SetBit(PORTD, 7, 1);
  return;
}

void Send_Data(int num) {
  PORTD = SetBit(PORTD, 6, 1);
  PORTD = SetBit(PORTD, 7, 0); //cs pin to low
  SPI_SEND(num);
  PORTD = SetBit(PORTD, 7, 1);
  return;
}

void HardwareReset(){
    PORTB = SetBit(PORTB, 0, 0);
    _delay_ms(200);
    PORTB = SetBit(PORTB, 0, 1);
    _delay_ms(200);
}

void ST7735_init(){
  //CS: PORTB10 / 2
  //RESET: PORTB8 / 0
  //A0: PORTB9 / 1
  //SDA: PORTB11 / 3
  //SCK: PORTB13 / 5
  
  //PORTB = SetBit(PORTB, PIN_SS, 0);

  HardwareReset();
  
  Send_Command(0x01);
  _delay_ms(150);
  
  Send_Command(0x11);
  _delay_ms(200);
  
  Send_Command(0x3A);
  Send_Data(0x05); //16 bit color mode, opposed to 0x06 18 bit
  _delay_ms(10);

  Send_Command(0x29);
  _delay_ms(200);
}

void setBox(int x0, int y0, int x1, int y1) {
  Send_Command(0x2A);
      Send_Data(0);//x start
      Send_Data(x0);
      Send_Data(0);//x end
      Send_Data(x1);
      Send_Command(0x2B);
      Send_Data(0);//y start
      Send_Data(y0);
      Send_Data(0);//y end
      Send_Data(y1);

      Send_Command(0x2C);
  return;
}

void drawGridSpace(int x0, int y0, int x1, int y1, int c1, int c2, int fill, int c3, int c4) {
   //x0,y0 top left coords
   //x1,y1 bottom right coords

   //c1/c2 16 bit border color
   //fill bool
   //c3/c4 16 bit fill color
   
   //Top border
    for (int x = x0; x < x1; x++) {
        setBox(x, y0, x + 1, y0 + 1); //Draw single pixel
        Send_Data(c1);    //Black 
        Send_Data(c2);
    }

    //Bottom border
    for (int x = x0; x < x1; x++) {
        setBox(x, y1, x + 1, y1 + 1);
        Send_Data(c1);    //Black 
        Send_Data(c2);
    }

    //Left border
    for (int y = y0; y < y1; y++) {
        setBox(x0, y, x0 + 1, y + 1);
        Send_Data(c1);    //Black 
        Send_Data(c2);
    }

    //Right border
    for (int y = y0; y < y1; y++) {
        setBox(x1, y, x1 + 1, y + 1);
        Send_Data(c1);    //Black 
        Send_Data(c2);
    }

    if (fill) {
      for (int i = x0 + 1; i < x1 - 1; i++) {
        for (int j = y0 + 1; j < y1 - 1; j++) {
            setBox(i, j, i+1, j+1);  //Draw single pixel
            Send_Data(c3);
            Send_Data(c4);
        }
      }
    }

}

void offScreen() {
  setBox(0,0,128,128);
    for (int i = 0; i < 128; i++) {
      for (int j = 0; j < 128; j++) {
        Send_Data(255);
        Send_Data(255);
      }
    }
}

void createGrid() {
    int boxSize = 14;

    /*setBox(0,0,128,128);
    for (int i = 0; i < 128; i++) {
      for (int j = 0; j < 128; j++) {
        Send_Data(255);
        Send_Data(255);
      }
    }*/
    

    //9x9 grid
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            //coords per box
            int x0 = j * boxSize;       //x start
            int y0 = i * boxSize;       //y start
            int x1 = x0 + boxSize - 1;  //x end
            int y1 = y0 + boxSize - 1;  //y end

            //draw grid space for each slot in 9x9 board
            drawGridSpace(x0, y0, x1, y1, 0, 0, 0, 0, 0);
        }
    }
}

void gameOver() {
  for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            //coords per box
            int x0 = j * 14;       //x start
            int y0 = i * 14;       //y start
            int x1 = x0 + 14 - 1;  //x end
            int y1 = y0 + 14 - 1;  //y end

            //draw grid space for each slot in 9x9 board
            drawGridSpace(x0, y0, x1, y1, 0, 255, 1, 0, 255);
        }
    }
}

void gameComplete() {
  for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            //coords per box
            int x0 = j * 14;       //x start
            int y0 = i * 14;       //y start
            int x1 = x0 + 14 - 1;  //x end
            int y1 = y0 + 14 - 1;  //y end

            //draw grid space for each slot in 9x9 board
            drawGridSpace(x0, y0, x1, y1, 0x1F, 0, 1, 0x1F, 0);
        }
    }
}

bool validPosition(const int pos[4], const int dir) {
  int valid = 1;

  if (dir == 0) { //up
    if (pos[1] < 14) {
      valid = 0;
    }
  }
  else if (dir == 1) {//down
    if (pos[3] > 8 * 14) {
      valid = 0;
    }
  }
  else if (dir == 2) { //left
    if (pos[0] < 14) {
      valid = 0;
    }
  }
  else if (dir == 3) {//right
    if (pos[2] > 8 * 14) {
      valid = 0;
    }
  }
  else if (dir == 4) {//diagonal up left
    if (pos[0] < 14 || pos[1] < 14) {
      valid = 0;
    }
  }
  else if (dir == 5) {//diagonal up right
    if (pos[1] < 14 || pos[2] > 8) {
      valid = 0;
    }
  }
  else if (dir == 6) {//diagonal down left
    if (pos[3] > 8 * 14 || pos[0] < 14) {
      valid = 0;
    }
  }
  else if (dir == 7) {//diagonal down right
    if (pos[3] > 8 * 14|| pos[2] > 8 * 14) {
      valid = 0;
    }
  }

  return valid; 
}

void sprite(const int spriteCode[12][12], const int pos[4]) {
  int x0 = pos[0] - 1;
  int y0 = pos[1] - 1;
  int x1 = pos[2] - 1;
  int y1 = pos[3] - 1;  

  for (int i = x0; i < x1 - 1; i++) {
    for (int j = y0; j < y1 - 1; j++) {
      if (spriteCode[j-y0][i-x0]) {
        setBox(i, j, i+1, j+1);
        Send_Data(0);
        Send_Data(0);
      }
    }
  }
   
   return;
}

void reveal(const int board[9][9], const int position[4], int gridTiles[9][9]) {
  if (gridTiles[position[2]/14 - 1][position[3]/14 - 1] != 0) {return;}
  
  if ((board[position[3]/14 - 1][position[2]/14 - 1]) == -1) {
    sprite(sprite_bomb,position);
    gridTiles[position[2]/14 - 1][position[3]/14 - 1] = -1;
  }
  else if ((board[position[3]/14 - 1][position[2]/14 - 1]) == 0) {
    sprite(sprite_0, position);
    gridTiles[position[2]/14 - 1][position[3]/14 - 1] = 1;
  }
  else if ((board[position[3]/14 - 1][position[2]/14 - 1]) == 1) {
    sprite(sprite_1, position);
    gridTiles[position[2]/14 - 1][position[3]/14 - 1] = 1;
  }
  else if ((board[position[3]/14 - 1][position[2]/14 - 1]) == 2) {
    {sprite(sprite_2, position);}
    gridTiles[position[2]/14 - 1][position[3]/14 - 1] = 1;
  }
  else if ((board[position[3]/14 - 1][position[2]/14 - 1]) == 3) {
    {sprite(sprite_3, position);}
    gridTiles[position[2]/14 - 1][position[3]/14 - 1] = 1;
  }
}

void move(int dir, int position[4]) {
  if (dir == 0) {//u
    position[1] -= 14;
    position[3] -= 14;
  }
  else if (dir == 1) {//d
    position[1] += 14;
    position[3] += 14;
  }
  else if (dir == 2) {//l
    position[0] -= 14;
    position[2] -= 14;
  }
  else if (dir == 3) {//r
    position[0] += 14;
    position[2] += 14;
  }
  else if (dir == 4) {//ul
    position[1] -= 14;
    position[3] -= 14;
    position[0] -= 14;
    position[2] -= 14;
  }
  else if (dir == 5) {//ur
    position[1] -= 14;
    position[3] -= 14;
    position[0] += 14;
    position[2] += 14;
  }
  else if (dir == 6) { //dl
    position[1] += 14;
    position[3] += 14;
    position[0] -= 14;
    position[2] -= 14;
  }
  else if (dir == 7) {//dr
    position[1] += 14;
    position[3] += 14;
    position[0] += 14;
    position[2] += 14;
  }
  return;
}

void startExplode(const int board[9][9], int position[4], int gridTiles[9][9], int result[2]) {
  //reveal target number
  int countValidNeighbors = 0;
  int countInvalidNeighbors = 0;
  reveal(board, position, gridTiles);
  
  if ((board[position[3]/14 - 1][position[2]/14 - 1]) != -1) {countValidNeighbors = 1;}
  else {countInvalidNeighbors = 1;}
  
  for (int i = 0; i < 8; i++) {
    if(validPosition(position,i)) {
      int initialPos[4] = {position[0],position[1],position[2],position[3]};
      move(i, initialPos);
      reveal(board, initialPos, gridTiles);
      if ((board[initialPos[3]/14 - 1][initialPos[2]/14 - 1]) != -1) {countValidNeighbors++;}
      else {countInvalidNeighbors++;}
    }  
  }

  result[0] = countValidNeighbors;
  result[1] = countInvalidNeighbors;

  // position[0] = initialPos[0];
  // position[1] = initialPos[1];
  // position[2] = initialPos[2];
  // position[3] = initialPos[3];

  return;//return number of revealed safe tiles
}

#endif