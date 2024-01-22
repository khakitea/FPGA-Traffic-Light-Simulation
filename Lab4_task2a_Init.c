/* University of Washington
 * ECE/CSE 474,  [11/9/2022]
 *
 *   [Robyn Jung 1976925]
 *   [Margot Enright-Down 1776361]
 *      
 *   [Lab 4][Task 1]
 *   [Task 1 Inits]
 */

#include "PLL_Header.h"
#include "Lab4_Task2a_Init.h"
#include "SSD2119_Display.h"
#include "tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>
   
#define OFF     0
#define STOP    1
#define GO      2
#define WARN    3

typedef enum lightStates {reset, stop, warn, go}lightState;
lightState currState = reset;           // 0: off  1: stop  2: go  3: warn
unsigned char now = 0;
unsigned long x, y;
unsigned long delay;

int PLL_Init(enum frequency freq) {
    // Do NOT modify this function.
    MOSCCTL &= ~(0x4);                      // Power up MOSC
    MOSCCTL &= ~(0x8);                      // Enable MOSC
    while ((RIS & 0x100) == 0) {};          // Wait for MOSC to be ready
    RSCLKCFG |= (0x3 << 20);                // Select MOSC as system clock source
    RSCLKCFG |= (0x3 << 24);                // Select MOSC as PLL clock source

    PLLFREQ0 |= 0x60;                       // Set MINT field
    PLLFREQ1 |= 0x4;                        // Set N field

    MEMTIM0 &= ~((0xF << 22) | (0xF << 6));     // Reset FBCHT and EBCHT
    MEMTIM0 &= ~((0xF << 16) | (0xF << 0));     // Reset EWS and FWS
    MEMTIM0 &= ~((0x1 << 21) | (0x1 << 5));     // Reset FBCE and EBCE

    RSCLKCFG &= ~(0x1 << 28);                   // Temporarilly bypass PLL

    switch (freq) {
        case 120:
            MEMTIM0 |= (0x6 << 22) | (0x6 << 6);    // Set FBCHT and EBCHT
            MEMTIM0 |= (0x5 << 16) | (0x5 << 0);    // Set EWS and FWS
            RSCLKCFG |= 0x3;                        // Set PSYSDIV to use 120 MHZ clock
            RSCLKCFG &= ~0x3FC;                     // Update PSYSDIV field
            break;
        case 60:
            MEMTIM0 |= (0x3 << 22) | (0x3 << 6);    // Set FBCHT and EBCHT
            MEMTIM0 |= (0x2 << 16) | (0x2 << 0);    // Set EWS and FWS
            RSCLKCFG |= 0x7;                        // Set PSYSDIV to use 60 MHZ clock
            RSCLKCFG &= ~0x3F8;                     // Update PSYSDIV field
            break;
        case 12:
            MEMTIM0 |= (0x1 << 21) | (0x1 << 5);    // Set FBCE and EBCE
            RSCLKCFG |= 0x27;                       // Set PSYSDIV to use 12 MHZ clock
            RSCLKCFG &= ~0x3D8;                     // Update PSYSDIV field
            break;
        default:
            return -1;
    }

    RSCLKCFG |= (0x1 << 30);                // Enable new PLL settings
    PLLFREQ0 |= (0x1 << 23);                // Power up PLL
    while ((PLLSTAT & 0x1) == 0) {};        // Wait for PLL to lock and stabilize

    RSCLKCFG |= (0x1u << 31) | (0x1 << 28);  // Use PLL and update Memory Timing Register
    return 1;
}

void trafficDisp() {
  // Draw Buttons
  // x, y, width, height, color(cyan 3)
  LCD_DrawFilledRect(64, 170, 64, 40, Color4[3]);
  LCD_DrawFilledRect(192, 170, 64, 40, Color4[3]);
  // Draw lights
  //center x, y, r, color
  LCD_DrawFilledCircle(80, 90, 30, Color4[4]);  //dull red
  LCD_DrawFilledCircle(160, 90, 30, Color4[6]); //brown
  LCD_DrawFilledCircle(240, 90, 30, Color4[2]); //dull green
  
  //Write Button names
  LCD_SetCursor(74,172);
  LCD_Printf("PED");
  LCD_SetCursor(202, 172);
  LCD_Printf("SYS");
}

void trafficTimer_init() {
  SYSCTL_RCGCTIMER_R = 0x2;              //Timer 1

  //Timer1 A
  TIMER1_CTL_R = 0x0;           //timer 1A disable
  TIMER1_CFG_R = 0x0;             //32-bit mode
  TIMER1_TAMR_R |= 0x2;            //periodic timer mode
  TIMER1_TAMR_R &= 0xFFEF;         //setting to TACDIR to countdown
  TIMER1_TAILR_R = 0x1312D000;       //80,000,000   5sec
  TIMER1_ICR_R |= 0x1;             //clear flag
  
//  //enable timer interrupt
//  NVIC_EN0_R |= 1<<21u;   //only using interrupt with timer1A
//  TIMER1_IMR_R |= 0x01;
  
  //start timer
  TIMER1_CTL_R |= 0x1;             //timer 1A enable
}


/**************************************************************
*                        TASK 2A
**************************************************************/
void trafficFSM() {
  
/********************State Functionality********************/
  switch(currState) {
      case reset:
        //get touch input
        x = Touch_ReadX();
        y = Touch_ReadY();
        //if SYS_BUTTON pressed, go to stop
        if ((y > 710) && (y < 980)) {           //touch in the y zone
          if ((x > 1780) && (x < 2020)) {       //x zone
            TIMER1_ICR_R |= 0x1;   //clear flag timer1A
            currState = stop;
            break;
          } else {
            break;
          }
        } else {
          currState = reset;
        }
        break;
    
      case stop:
        //get touch input
        x = Touch_ReadX();
        y = Touch_ReadY();
        
        //after 5 sec, go to go
        if ((TIMER1_RIS_R & 0x1) == 1){
          TIMER1_ICR_R |= 0x1;   //clear flag timer1A
          TIMER1_ICR_R |= 0x1;   //clear flag timer1A
          delay++;
          currState = go;
          break;
        }
        //if SYS_BUTTON pressed, go to reset
        else if ((y > 710) && (y < 980)) {           //touch in the y zone
          if ((x > 1780) && (x < 2020)) {            // x zone
            TIMER1_ICR_R |= 0x1;
            TIMER1_ICR_R |= 0x1;
            currState = reset;
            break;
         //if PED pressed, stay
          }else if ((x >1100) && (x < 1500)) {
            TIMER1_ICR_R |= 0x1;
            TIMER1_ICR_R |= 0x1;
            currState = stop;
            break;
          }
        }
        //When idle, clear flag and do nothing
        else {
          currState = stop;
        }
        break;
    
      case go:
        //get touch input
        x = Touch_ReadX();
        y = Touch_ReadY();
        
        //after 5 sec, go to stop
        if ((TIMER1_RIS_R & 0x1) == 1){
          TIMER1_ICR_R |= 0x1;   //clear flag timer1A
          TIMER1_ICR_R |= 0x1;   //clear flag timer1A
          currState = stop;
          break;
        }
        //if SYS_BUTTON pressed, go to reset
        else if ((y > 710) && (y < 980)) {           //touch in the y zone
          if ((x > 1780) && (x < 2020)) {       //x zone
            TIMER1_ICR_R |= 0x1;
            TIMER1_ICR_R |= 0x1;   //clear flag timer1A
            currState = reset;
            break;
        //if PED pressed, go to warn
          } else if ((x >1100) && (x < 1500)) {
            TIMER1_ICR_R |= 0x1;
            TIMER1_ICR_R |= 0x1;   //clear flag timer1A
            currState = warn;
            break;
          }
        } else {
          currState = go;
        }
        break;
    
      case warn:
        //get touch input
        x = Touch_ReadX();
        y = Touch_ReadY();
        
        //if SYS_BUTTON pressed, go to reset
        if ((y > 710) && (y < 980)) {           //touch in the y zone
          if ((x > 1780) && (x < 2020)) {       //x zone
            TIMER1_ICR_R |= 0x1;
            TIMER1_ICR_R |= 0x1;   //clear flag timer1A
            currState = reset;
            break;
        //if PED pressed, go 
          } else if ((x >1100) && (x < 1500)) {
            
            break;
          }
        }
        //after 5 sec, go to stop
        else if((TIMER1_RIS_R & 0x1) == 1){
          TIMER1_ICR_R |= 0x1;   //clear flag timer1A
          TIMER1_ICR_R |= 0x1;   //clear flag timer1A
          currState = stop;
          break;
        } else {
          currState = warn;
        }
        break;
    
      default:
        currState = reset;
        break;
  }
  
  
  
  
/********************State Behaviors********************/
  switch(currState) {
  case reset:
    RED_off();
    YELLOW_off();
    GREEN_off();
    break;
    
  case stop:
    RED_on();
    YELLOW_off();
    GREEN_off();
    break;
    
  case go:
    GREEN_on();
    RED_off();
    YELLOW_off();
    break;
    
  case warn:
    YELLOW_on();
    RED_off();
    GREEN_off();
    break;
    
  default:
    break;
  }
}

void GREEN_on() {
  LCD_DrawFilledCircle(240, 90, 30, Color4[10]);
}

void RED_on() {
  LCD_DrawFilledCircle(80, 90, 30, Color4[12]);
}

void YELLOW_on() {
  LCD_DrawFilledCircle(160, 90, 30, Color4[14]);
}

void GREEN_off() {
  LCD_DrawFilledCircle(240, 90, 30, Color4[2]);
}

void RED_off() {
  LCD_DrawFilledCircle(80, 90, 30, Color4[4]); 
}

void YELLOW_off() {
  LCD_DrawFilledCircle(160, 90, 30, Color4[6]);
}
