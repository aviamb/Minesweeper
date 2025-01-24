#ifndef NOTES_H
#define NOTES_H

#include <avr/pgmspace.h>

#define D2 27396
#define A2 18180
#define D3 13604
#define B3 8096
#define C4 7633
#define D4 6802
#define E4 6060
#define F4 5730
#define GF4 5404
#define A4 4544
#define B4 4048
#define G4 2550
#define GS4 2406
#define CS5 3823
#define D5 3406
#define E5 3034
#define F5 2864
#define A5 2272
#define GS5 2409
#define C5 3823
#define FS5 2702



#define REST 1

PROGMEM const int song[9][8] = {
    {A4, A4, A4, A4, A5, A5, A5, A5},//somehwere
    {GS5, GS5, E5, FS5, E5, E5, A5, A5},//o ver the  rain bow
    {A4, A4, A4, A4, FS5, FS5, FS5, FS5},//way up
    {E5, E5, E5, E5, E5, E5, E5, E5},//high
    {GF4, GF4, GF4, GF4, D5, D5, D5, D5},//theres a
    {CS5, CS5, A4, B4, CS5, CS5, D5, D5},//land that i heard of
    {B4, B4, GS4, A4, B4, B4, CS5, CS5},//once in a lull a 
    {A4, A4, A4, A4, A4, A4, A4, A4}, //bye
    {REST, REST, REST, REST, REST, REST, REST, REST}
};

#endif