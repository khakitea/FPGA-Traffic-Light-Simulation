# Check out project list and summary on my [Notion](https://right-thrush-091.notion.site/Robyn-Jung-4c3bc2947eaa4279a296059c6bc376eb?pvs=4) page!

## FPGA Traffic Light Simulation on LCD Touch Screen

Part of a project for "Intro to Embedded Systems" course at University of Washington, Seattle.  
Design and implement traffic light simulation on breadboard (already done and not included in this project) and LCD touch screen.  

## Hardware:  
* TIVA Board TM4C1294NCPDT  
* LCD touch screen  
## Software:  
* IAR Embedded Workbench IDE  

## Approach  
  For FSM design schematic, please check my Notion page.  This project was further developed from physical design (LEDs, switches, and buttons on breadboard) onto a touch screen interface.  
  * Timers were used validate user input and LED swtich timing.
  * The user interface design was drawn using the provided function.
    - Initializing the ports and GPIO pins were done by me.
  * To manage and control the touch inputs from the userend, I used PLL.
    -'Lab4_task2a_init.c'
  * Interrupts were used to catch input signals.
    - 'SSD2119_Touch'
