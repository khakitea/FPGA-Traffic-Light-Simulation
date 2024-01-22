#include "SSD2119_Display.h"
#include "SSD2119_Touch.h"
#include "tm4c1294ncpdt.h"
#include "Lab4_Task2a_Init.h"
#include <stdint.h>
#include <stdio.h>

enum frequency freq;
//static unsigned long delay;

int main(void) {
  
  freq = PRESET2;
  PLL_Init(freq);
  LCD_Init();
  Touch_Init();
  trafficDisp();
  
  //Ignore
  //Testing LCD touch interrupt
  //PortA Handler Init  PA2
//  SYSCTL_RCGCGPIO_R |= 0x1;
//  delay++;
//  delay++;
//  GPIO_PORTA_AMSEL_R &= ~0x4;
//  GPIO_PORTA_AFSEL_R &= ~0x4;
//  GPIO_PORTA_DIR_R &= ~0x4;
//  GPIO_PORTA_DEN_R |= 0x4;
//  GPIO_PORTA_IS_R &= ~0x4;
//  GPIO_PORTA_IBE_R &= ~0x4;
//  GPIO_PORTA_ICR_R |= 0x4;
//  
//  NVIC_EN0_R |= 0x10;
//  GPIO_PORTA_IM_R |= 0x4;
  
  while(1) {
    
    printf("%d\t", Touch_ReadX());
    printf("%d\n", Touch_ReadY());
    
    trafficFSM();
  }
  
  return 0;
}
//Ignore
//Testing LCD touch interrupt
//void PortA_Handler( void ){
//  if (GPIO_PORTA_MIS_R == 0x4) {
//    flag = 1;
//  } else {
//    flag = 0;
//  }
//  GPIO_PORTA_ICR_R |= 0x4;
//}