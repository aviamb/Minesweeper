#ifndef SPRITES_H
#define SPRITES_H

const int sprite_0[12][12] = {
  {0,0,0,0,0,0,0,0,0,0,0,0},//1
  {0,0,0,0,0,0,0,0,0,0,0,0},//2
  {0,0,0,0,0,0,1,1,1,1,0,0},//3
  {0,0,0,0,0,1,1,0,0,1,1,0},//4
  {0,0,0,0,1,1,0,0,0,0,1,1},//5
  {0,0,0,0,1,1,0,0,0,0,1,1},//6
  {0,0,0,0,1,1,0,0,0,0,1,1},//7
  {0,0,0,0,1,1,0,0,0,0,1,1},//8
  {0,0,0,0,1,1,0,0,0,0,1,1},//9
  {0,0,0,0,1,1,0,0,0,0,1,1},//10
  {0,0,0,0,0,1,1,0,0,1,1,0},//11
  {0,0,0,0,0,0,1,1,1,1,0,0},//12
};

const int sprite_1[12][12] = {
  {0,0,0,0,0,0,0,0,0,0,0,0},//12
  {0,0,0,0,0,0,0,0,0,0,0,0},//1
  {0,0,0,0,0,0,0,1,0,0,0,0},//2
  {0,0,0,0,0,0,1,1,0,0,0,0},//3
  {0,0,0,0,0,1,1,1,0,0,0,0},//4
  {0,0,0,0,1,0,1,1,0,0,0,0},//5
  {0,0,0,0,0,0,1,1,0,0,0,0},//6
  {0,0,0,0,0,0,1,1,0,0,0,0},//7
  {0,0,0,0,0,0,1,1,0,0,0,0},//8
  {0,0,0,0,0,0,1,1,0,0,0,0},//9
  {0,0,0,0,0,0,1,1,0,0,0,0},//10
  {0,0,0,0,1,1,1,1,1,1,0,0},//11
};

const int sprite_2[12][12] = {
  {0,0,0,0,0,0,0,0,0,0,0,0},//1
  {0,0,0,0,0,0,0,0,0,0,0,0},//2
  {0,0,0,0,0,1,1,1,0,0,0,0},//3
  {0,0,0,0,1,1,1,1,1,0,0,0},//4
  {0,0,0,1,1,0,0,0,1,1,0,0},//5
  {0,0,0,0,0,0,0,0,1,1,0,0},//6
  {0,0,0,0,0,0,0,0,1,1,0,0},//7
  {0,0,0,0,0,0,0,1,1,0,0,0},//8
  {0,0,0,0,0,0,1,1,0,0,0,0},//9
  {0,0,0,0,0,1,1,0,0,0,0,0},//10
  {0,0,0,0,1,1,0,0,0,0,0,0},//11
  {0,0,0,1,1,1,1,1,1,1,0,0},//12
};

const int sprite_3[12][12] = {
  {0,0,0,0,0,0,0,0,0,0,0,0},//1
  {0,0,0,0,1,1,1,1,0,0,0,0},//2
  {0,0,0,1,1,1,1,1,1,0,0,0},//3
  {0,0,1,0,0,0,0,1,1,0,0,0},//4
  {0,0,0,0,0,0,0,1,1,0,0,0},//5
  {0,0,0,0,0,0,1,1,0,0,0,0},//6
  {0,0,0,0,1,1,1,0,0,0,0,0},//7
  {0,0,0,0,0,0,1,1,0,0,0,0},//8
  {0,0,0,0,0,0,0,1,1,0,0,0},//9
  {0,0,1,0,0,0,0,1,1,0,0,0},//10
  {0,0,0,1,1,1,1,1,1,0,0,0},//11
  {0,0,0,0,1,1,1,1,0,0,0,0},//12
};

const int sprite_bomb[12][12] = {
  {0,0,0,0,0,0,0,0,0,0,0,0},//1
  {0,0,0,0,0,0,0,0,0,0,0,0},//2
  {0,0,0,0,0,0,0,1,0,0,0,0},//3
  {0,0,0,0,0,0,1,1,1,0,0,0},//4
  {0,0,0,0,1,1,1,1,1,1,1,0},//5
  {0,0,0,0,1,1,1,1,1,1,1,0},//6
  {0,0,0,1,1,1,1,1,1,1,1,1},//7
  {0,0,0,1,1,1,1,1,1,1,1,1},//8
  {0,0,0,1,1,1,1,1,1,1,1,1},//9
  {0,0,0,0,1,1,1,1,1,1,1,0},//10
  {0,0,0,0,1,1,1,1,1,1,1,0},//11
  {0,0,0,0,0,0,1,1,1,0,0,0},//12
};

/*const int sprite_1[12][12] = {
  {0,0,0,0,0,0,0,0,0,0,0,0},//1
  {0,0,0,0,0,0,0,0,0,0,0,0},//2
  {0,0,0,0,0,0,0,0,0,0,0,0},//3
  {0,0,0,0,0,0,0,0,0,0,0,0},//4
  {0,0,0,0,0,0,0,0,0,0,0,0},//5
  {0,0,0,0,0,0,0,0,0,0,0,0},//6
  {0,0,0,0,0,0,0,0,0,0,0,0},//7
  {0,0,0,0,0,0,0,0,0,0,0,0},//8
  {0,0,0,0,0,0,0,0,0,0,0,0},//9
  {0,0,0,0,0,0,0,0,0,0,0,0},//10
  {0,0,0,0,0,0,0,0,0,0,0,0},//11
  {0,0,0,0,0,0,0,0,0,0,0,0},//12
}*/

#endif