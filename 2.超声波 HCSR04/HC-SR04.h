#ifndef __HC_SR04_H
#define __HC_SR04_H
#include "stm32f10x.h"    
void HC_SR04_Init(void);
uint32_t HC_SR04_Compute(void);

extern uint32_t Distance;


#endif
