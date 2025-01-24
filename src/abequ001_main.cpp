#include "graph.h"
#include "timerISR.h"
#include "spiAVR.h"
#include "periph.h"
#include "helper.h"
#include "st7753.h"
#include "sprites.h"
#include <time.h>
#include "notes.h"
//#include "irAVR.h"

void TimerISR(void);
int safe = 0;
int bomb = 0;
int joyD = 0;

int fail = -1;
int numSafeFlags = -1;
int numBombFlags = 0;
int sysState = 1;

int board[9][9] = {0};
int position[4] = {1,1,14,14};
int gridTiles[9][9] = {0};

int result[2] = {0};

int GetDirection(int y, int x) {
  if (x > 0 && y > 900) {
    return 0; //up
  }
  else if (x > 0 && y < 500) {
    return 1; //down
  }
  else if (x < 500 && y > 0) {
    return 2; //left
  }
  else if (x > 900 && y > 0) {
    return 3; //right
  }
  else {
    return 4; //neutral
  }
}

#define NUM_TASKS 9 //TODO: Change to the number of tasks being used

//Task struct for concurrent synchSMs implmentations
typedef struct _task{
	signed 	 char state; 		//Task's current state
	unsigned long period; 		//Task period
	unsigned long elapsedTime; 	//Time elapsed since last task tick
	int (*TickFct)(int); 		//Task tick function
} task;

//TODO: Define Periods for each task
// e.g. const unsined long TASK1_PERIOD = <PERIOD>
const unsigned long GCD_PERIOD = 25;//TODO:Set the GCD Period
const unsigned long LB_PERIOD = 25;
const unsigned long RB_PERIOD = 25;
const unsigned long JOY_PERIOD = 25;
const unsigned long LCD_PERIOD = 200;
const unsigned long BFLAG_PERIOD = 200;
const unsigned long SFLAG_PERIOD = 200;
const unsigned long JB_PERIOD = 25;
const unsigned long BUZZER_PERIOD = 400;
const unsigned long DISPLAY_PERIOD = 100;

task tasks[NUM_TASKS]; // declared task array with 5 tasks

//TODO: Declare your tasks' function and their states here
enum LB_States{LB_init, LB_off, LB_on} LB_State;
int LB_TickFct(int state){
  switch(state){
    case LB_init: state = LB_off; break;
    case LB_off: 
      if (GetBit(PINC, 0)) {state = LB_on;}
      else {state = LB_off;}
      break;
    case LB_on: 
      if (GetBit(PINC, 0)) {state = LB_on;}
      else {state = LB_off;}
      break;
    default: state = LB_init; break;
  }
  switch(state){
    case LB_init: break;
    case LB_off: safe = 0; break;
    case LB_on: safe = 1; break;
    default: break;
  }
  return state;
}

enum RB_States{RB_init, RB_off, RB_on} RB_State;
int RB_TickFct(int state){
  switch(state){
    case RB_init: state = RB_off; break;
    case RB_off: 
      if (GetBit(PINC, 1)) {state = RB_on;}
      else {state = RB_off;}
      break;
    case RB_on: 
      if (GetBit(PINC, 1)) {state = RB_on;}
      else {state = RB_off;}
      break;
    default: state = RB_init; break;
  }
  switch(state){
    case RB_init: break;
    case RB_off: bomb = 0; break;
    case RB_on: bomb = 1; break;
    default: break;
  }
  return state;
}

enum Joy_States{Joy_init, Joy_read} Joy_State;
int Joy_TickFct(int state){
  switch(state){
    case Joy_init: state = Joy_read; break;
    case Joy_read: state = Joy_read; break;
    default: state = Joy_init; break;
  }
  switch(state){
    case Joy_init: break;
    case Joy_read: 
      joyD = GetDirection(ADC_read(2),ADC_read(3));
      break;
    default: break;
  }
  return state;
}

enum LCD_States{LCD_init, LCD_on} LCD_State;
int LCD_TickFct(int state){
  static int cnt = 0;
  switch(state) {
    case LCD_init:
      if (joyD != 4) state = LCD_on;
      else state = LCD_init;
      break;
    case LCD_on: 
      if (joyD != 4 || cnt < 2) state = LCD_on;
      else state = LCD_init;
      break;
    default: state = LCD_init; break;
  }
  switch(state) {
    case LCD_init: 
      if (sysState && gridTiles[position[2]/14 - 1][position[3]/14 - 1] == 0) drawGridSpace(position[0],position[1],position[2],position[3],0,255,1,255,255);
      else drawGridSpace(position[0],position[1],position[2],position[3],0,255,0,0,0);
      cnt = 0;
      break;
    case LCD_on: 
      if (joyD == 0) {//up
        if (validPosition(position, 0)) {
          if (sysState && gridTiles[position[2]/14 - 1][position[3]/14 - 1] == 0) drawGridSpace(position[0],position[1],position[2],position[3],0,0,1,255,255);
          else drawGridSpace(position[0],position[1],position[2],position[3],0,0,0,255,255);
          move(0, position);
        }
      }
      else if (joyD == 1) { //down
        if (validPosition(position, 1)) {
          if (sysState && gridTiles[position[2]/14 - 1][position[3]/14 - 1] == 0) drawGridSpace(position[0],position[1],position[2],position[3],0,0,1,255,255);
          else drawGridSpace(position[0],position[1],position[2],position[3],0,0,0,255,255);
          move(1, position);
        }
      }
      else if (joyD == 2) { //left
        if (validPosition(position, 2)) {
          if (sysState && gridTiles[position[2]/14 - 1][position[3]/14 - 1] == 0) drawGridSpace(position[0],position[1],position[2],position[3],0,0,1,255,255);
          else drawGridSpace(position[0],position[1],position[2],position[3],0,0,0,255,255);
          move(2, position);
        }
      }
      else if (joyD == 3) { //right
        if (validPosition(position, 3)) {
          if (sysState && gridTiles[position[2]/14 - 1][position[3]/14 - 1] == 0) drawGridSpace(position[0],position[1],position[2],position[3],0,0,1,255,255);
          else drawGridSpace(position[0],position[1],position[2],position[3],0,0,0,255,255);          
          move(3, position);
        }
      }
      cnt++;
      break;
    default: break;
  }
  return state;
}

enum BFlag_States{BFlag_init, BFlag_wait, BFlag_flag} BFlag_State;
int BFlag_TickFct(int state){
  switch(state){
    case BFlag_init: state = BFlag_wait; break;
    case BFlag_wait: 
      state = bomb ? BFlag_flag : BFlag_wait;
      break;
    case BFlag_flag: 
      state = BFlag_wait;
      break;
    default: state = BFlag_init; break;
  }
  switch(state){
    case BFlag_init: break;
    case BFlag_wait: 
      break;
    case BFlag_flag: 
      if (gridTiles[position[2]/14 - 1][position[3]/14 - 1] != -1) {
        gridTiles[position[2]/14 - 1][position[3]/14 - 1] = -1; //1 for bomb flag //? is -1 correct? 
        sprite(sprite_bomb, position);
        numBombFlags++;
      }
      else {gridTiles[position[2]/14 - 1][position[3]/14 - 1] = 0; numBombFlags--;}
      _delay_ms(500);
      break;
    default: break;
  }
  return state;
}

enum SFlag_States{SFlag_init, SFlag_wait, SFlag_start, SFlag_check, SFlag_fail, SFlag_success} SFlag_State;
int SFlag_TickFct(int state){
  switch(state){
    case SFlag_init: state = SFlag_wait; break;
    case SFlag_wait: 
      if (!safe) state = SFlag_wait;
      else if (safe && (numSafeFlags == -1)) state = SFlag_start;
      else state = SFlag_check;
      break;
    case SFlag_start: 
      state = SFlag_wait;
      break;
    case SFlag_check: 
      if (fail == -1) state = SFlag_check;
      else if (fail == 0) state = SFlag_success;
      else if (fail == 1) state = SFlag_fail;
      break;
    case SFlag_fail: state = SFlag_wait; break;
    case SFlag_success: state = SFlag_wait; break;
    default: state = SFlag_init; break;
  }
  switch(state){
    case SFlag_init: break;
    case SFlag_wait: break;
    case SFlag_start: 
      _delay_ms(500);
      startExplode(board,position,gridTiles, result);//change to however many open upon start
      numSafeFlags = result[0];
      numBombFlags = result[1];
      break;
    case SFlag_check: 
      if (board[position[3]/14 - 1][position[2]/14 - 1] == -1) {fail = 1;}
      else fail = 0;
      break;
    case SFlag_fail: 
      gameOver();
      sysState = 0;
      break;
    case SFlag_success: 
      reveal(board, position, gridTiles);
      numSafeFlags++;
      if (numSafeFlags == 73) {gameComplete(); sysState = 0;}
      break;
    default: break;
  }
  return state;
}

enum JB_States{JB_init, JB_off, JB_on} JB_State;
int JB_TickFct(int state){
  switch(state){
    case JB_init: state = JB_off; break;
    case JB_off: 
      state = !GetBit(PINC, 4) ? JB_on : JB_off;
      break;
    case JB_on: 
      state = !GetBit(PINC, 4) ? JB_on : JB_off;
      break;
    default: state = JB_init; break;
      }
  switch(state){
    case JB_init: break;
    case JB_off: break;
    case JB_on: 
      safe = 0;
      bomb = 0;
      joyD = 0;
      fail = -1;
      numSafeFlags = -1;
      numBombFlags = 0;

      for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = 0;
            gridTiles[i][j] = 0;
        }
      }

      position[0] = 1;
      position[1] = 1;
      position[2] = 14;
      position[3] = 14;      
      
      sysState = 1;

      Graph_init(board);

      //white board
        setBox(0,0,128,128);
        for (int i = 0; i < 128; i++) {
          for (int j = 0; j < 128; j++) {
            Send_Data(255);
            Send_Data(255);
          }
        }
    
    createGrid();

      break;
    default: break;
  }
  return state;
}

enum Buzzer_States{Buzzer_init, Buzzer_off, Buzzer_on} Buzzer_State;
int Buzzer_TickFct(int state){
  static int songIndex = 0;
  static int note = 1; 
  switch(state){
    case Buzzer_init: 
      state = Buzzer_off;
      break;
    case Buzzer_off: 
      state = (sysState && numSafeFlags > 1) ? Buzzer_on : Buzzer_off;
      break;
    case Buzzer_on: 
      state = (sysState && numSafeFlags > 1) ? Buzzer_on : Buzzer_off;
      break;
    default: state = Buzzer_init; break;
  }
  switch(state){
    case Buzzer_init: 
      songIndex = 0;
      break;
    case Buzzer_off: 
      ICR1 = REST;
      OCR1A = ICR1;
      songIndex = 0;
      break;
    case Buzzer_on: 
      note = pgm_read_word(&(song[songIndex / 8][songIndex % 8])); // Read the note frequency from PROGMEM
      ICR1 = note;     // Set the frequency (ICR1 determines the tone)
      OCR1A = ICR1 / 2; 
      songIndex++;
      if (songIndex > 72) songIndex = 0;
      break;
    default: break;
  }
  return state; 
}

enum Display_State{Display_init, Display_on} Display_State;
int Display_TickFct(int state){
  switch(state){
    case Display_init: state = Display_on; break;
    case Display_on: state = sysState ? Display_on : Display_init; break;
    default: state = Display_init; break;
  }
  switch(state){
    case Display_init: outNum(0); break;
    case Display_on: if (numBombFlags < 10) outNum(numBombFlags); break;
    default: break;
  }
  return state;
}

void TimerISR() {
    {for ( unsigned int i = 0; i < NUM_TASKS; i++ ) {                   // Iterate through each task in the task array
      if ( tasks[i].elapsedTime == tasks[i].period ) {           // Check if the task is ready to tick
        tasks[i].state = tasks[i].TickFct(tasks[i].state); // Tick and set the next state for this task
        tasks[i].elapsedTime = 0;                          // Reset the elapsed time for the next tick
      }
      tasks[i].elapsedTime += GCD_PERIOD;                        // Increment the elapsed time by GCD_PERIOD
    }}}

//TODO: Create your tick functions for each task

int main(void) {
    //TODO: initialize all your inputs and ouputs
    DDRB = 0xFF;
    PORTB = 0x00;

    DDRC = 0x20;
    PORTC = 0xDF;

    DDRD = 0xFF;
    PORTD = 0x00;

    srand(time(NULL));
    ADC_init();   // initializes ADC
    SPI_INIT();
    ST7735_init();
    Graph_init(board);

   //white board
    setBox(0,0,128,128);
    for (int i = 0; i < 128; i++) {
      for (int j = 0; j < 128; j++) {
        Send_Data(255);
        Send_Data(255);
      }
    }
    
    createGrid();

    TCCR1A |= (1 << WGM11) | (1 << COM1A1); //COM1A1 sets it to channel A
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); //CS11 sets the prescaler to be 8
    //WGM11, WGM12, WGM13 set timer to fast pwm mode
    ICR1 = A4; //20ms pwm period
    OCR1A =  ICR1/* set your value here */ ;

    //TODO: Initialize tasks here
    tasks[0].period = LB_PERIOD;
    tasks[0].state = LB_init;
    tasks[0].elapsedTime = tasks[0].period;
    tasks[0].TickFct = &LB_TickFct;

    tasks[1].period = RB_PERIOD;
    tasks[1].state = RB_init;
    tasks[1].elapsedTime = tasks[1].period;
    tasks[1].TickFct = &RB_TickFct;

    tasks[2].period = JOY_PERIOD;
    tasks[2].state = Joy_init;
    tasks[2].elapsedTime = tasks[2].period;
    tasks[2].TickFct = &Joy_TickFct;

    tasks[3].period = LCD_PERIOD;
    tasks[3].state = LCD_init;
    tasks[3].elapsedTime = tasks[3].period;
    tasks[3].TickFct = &LCD_TickFct;

    tasks[4].period = BFLAG_PERIOD;
    tasks[4].state = BFlag_init;
    tasks[4].elapsedTime = tasks[4].period;
    tasks[4].TickFct = &BFlag_TickFct;

    tasks[5].period = SFLAG_PERIOD;
    tasks[5].state = SFlag_init;
    tasks[5].elapsedTime = tasks[5].period;
    tasks[5].TickFct = &SFlag_TickFct;

    tasks[6].period = JB_PERIOD;
    tasks[6].state = JB_init;
    tasks[6].elapsedTime = tasks[6].period;
    tasks[6].TickFct = &JB_TickFct;

    tasks[7].period = BUZZER_PERIOD;
    tasks[7].state = Buzzer_init;
    tasks[7].elapsedTime = tasks[7].period;
    tasks[7].TickFct = &Buzzer_TickFct;
    
    tasks[8].period = DISPLAY_PERIOD;
    tasks[8].state = Display_init;
    tasks[8].elapsedTime = tasks[8].period;
    tasks[8].TickFct = &Display_TickFct;
    

    TimerSet(GCD_PERIOD);
    TimerOn();

    while (1) { }
    return 0;
}