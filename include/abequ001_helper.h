#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <util/delay.h>

#ifndef HELPER_H
#define HELPER_H

//Functionality - finds the greatest common divisor of two values
//Parameter: Two long int's to find their GCD
//Returns: GCD else 0
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a % b;
		if( c == 0 ) { return b; }
		a = b;
		b = c;
	}
	return 0;
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
   return (b ?  (x | (0x01 << k))  :  (x & ~(0x01 << k)) );
              //   Set bit to 1           Set bit to 0
}

unsigned char GetBit(unsigned char x, unsigned char k) {
   return ((x & (0x01 << k)) != 0);
}

 
int nums[16] = {0b1111110, 0b0110000, 0b1101101, 0b1111001, 0b0110011, 0b1011011, 0b1011111, 0b1110000, 0b1111111, 0b1111011, 0b1110111, 0b0011111, 0b1001110, 0b0111101, 0b1001111, 0b1000111}; 
// a  b  c  d  e  f  g

void outNum(int num) {
    if (nums[num] & 0x01) {//g
        PORTB = SetBit(PORTB, 4, 1);
    } else {
        PORTB = SetBit(PORTB, 4, 0);
    }

    if (nums[num] & 0x02) {//f
        PORTB = SetBit(PORTB, 2, 1);
    } else {
        PORTB = SetBit(PORTB, 2, 0);
    }

    if (nums[num] & 0x40) {//a
        PORTC = SetBit(PORTC, 5, 1);
    } else {
        PORTC = SetBit(PORTC, 5, 0);
    }

    if (nums[num] & 0x20) {//b
        PORTD = SetBit(PORTD, 5, 1);
    } else {
        PORTD = SetBit(PORTD, 5, 0);
    }

    if (nums[num] & 0x04) {//e
        PORTD = SetBit(PORTD, 4, 1);
    } else {
        PORTD = SetBit(PORTD, 4, 0);
    }

    if (nums[num] & 0x08) {//d
        PORTD = SetBit(PORTD, 3, 1);
    } else {
        PORTD = SetBit(PORTD, 3, 0);
    }

    if (nums[num] & 0x10) {//c
        PORTD = SetBit(PORTD, 2, 1);
    } else {
        PORTD = SetBit(PORTD, 2, 0);
    }
	return;
}



long map(long x, long in_min, long in_max, long out_min, long out_max) {
return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif /* HEPLER_H */