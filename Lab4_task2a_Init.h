/* University of Washington
 * ECE/CSE 474,  [11/3/2022]
 *
 *   [Robyn Jung 1976925]
 *   [Margot Enright-Down 1776361]
 *      
 *   [Lab 4][Task 1]
 *   Function Content
 */

#ifndef _LAB4_TASK2A_INIT_H_
#define _LAB4_TASK2A_INIT_H_

// Preset frequency for 120 MHZ, 60 MHZ, and 12 MHZ clock
// Pass in the preset to Timer1_Init and PLL_Init to configure
// the system clock to specified frequency
enum frequency {PRESET1 = 120, PRESET2 = 60, PRESET3 = 12};

// Initializes the PLL module and generates a system clock frequency
// that equal to the frequency preset.
// Returns 1 if configured successfully, -1 if you select a non-exist preset.
int PLL_Init(enum frequency freq);

void trafficDisp();
void trafficTimer_init();
void Timer1A_Handler();
void trafficFSM();

void GREEN_on();
void GREEN_off();
void RED_on();
void RED_off();
void YELLOW_on();
void YELLOW_off();

#endif