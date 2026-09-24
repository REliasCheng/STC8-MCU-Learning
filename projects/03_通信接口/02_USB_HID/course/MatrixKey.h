#ifndef MATRIXKEY_H
#define MATRIXKEY_H

#include "GPIO.h"

#define COL1 P03
#define COL2 P06
#define COL3 P07
#define COL4 P17

#define ROW1 P34
#define ROW2 P35
#define ROW3 P40
#define ROW4 P41


// æÿ’Ûº¸≈Ã≥ı ºªØ
void MK_Init();

// æÿ’Ûº¸≈Ã…®√Ë
void MK_Scan02( void(*down)(u8 , u8 ) , void (*up)(u8 , u8 ));

#endif