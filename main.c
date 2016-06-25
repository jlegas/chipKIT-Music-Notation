/* This program is designed for ChipKit UNO32. It
 * uses the hardware PWM-capability to generate
 * signal that drives the piezzo-buzzer connected
 * to PIN3 on the board. The musical notes are
 * played by calculating PWM period for the required
 * sound frequency and an appropriate duty cycle to
 * get the wanted volume (0% - silence, 50% - max
 * vol).
 *
 * The code is built on the IS1200 course Lab3
 * assignment and modified by Justinas Legas.
 *
 * For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "functions.h"

/* tracks the execution of the program:
 * 0-main
 * 1-editor
 * 2-
 * 3-player
 * 4-settings
 */
int placeholder = 0;

/* Interrupt Service Routine */
void user_isr(void) {
    return;
}

//primary hardware initialization
void primary_init(void){
    /* Set up peripheral bus clock */
    /* OSCCONbits.PBDIV = 1; */
    OSCCONCLR = 0x100000; /* clear PBDIV bit 1 */
    OSCCONSET = 0x080000; /* set PBDIV bit 0 */

    /* Set up output pins */
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    TRISECLR = 0xFF;
    TRISDCLR = 0x1; //set port D0 as output
    PORTE = 0x0;

    /* Output pins for display signals */
    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;

    /* Set up input pins */
    //TRISDSET = (1 << 8);
    TRISDSET = 0xfe0;
    TRISFSET = (1 << 1);
    TRISESET = (0xff);

    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;
    /* SPI2STAT bit SPIROV = 0; */
    SPI2STATCLR = 0x40;
    /* SPI2CON bit CKP = 1; */
    SPI2CONSET = 0x40;
    /* SPI2CON bit MSTEN = 1; */
    SPI2CONSET = 0x20;
    /* SPI2CON bit ON = 1; */
    SPI2CONSET = 0x8000;

    /* Setup PWM */
    OC1CON = 0x0000; // Turn off the OC1 when performing the setup
    OC1R = 0; // Initialize primary Compare register
    OC1RS = 0; // Initialize secondary Compare register
    OC1CON = 0x0006; // Configure for PWM mode without Fault pin enabled
    PR2 = 0; // Set period (2840 for 440Hz A4 with 1:64 prescale: 80000000/64/2840)
    IFSCLR(0) = 0x00000100; // Clear the T2 interrupt flag
    IECSET(0) = 0x00000100; // Enable T2 interrupt
    IPCSET(2) = 0x0000001C; // Set T2 interrupt priority to 7
    OC1CON |= 0x8000; // Enable OC1
    TMR2 = 0; // reset the timer
    T2CON = 0x8060; // intialize the timer with 1:64 prescale and start it 0x8010

    /* Start timer3 */
    volatile int* t3con = (volatile int*) 0xBF800A00;
    volatile int* tmr3 = (volatile int*) 0xBF800A10;
    volatile int* pr3 = (volatile int*) 0xBF800A20;

    *t3con = 0; //stop the timer
    *tmr3 = 0; // reset the timer
    *pr3 = 80000000 / 256 / 1000; // set the period 1/1000 s
    *t3con = 0x8070; // intialize the timer with 1:256 prescale value

    /* Initialize the display */
    display_init();
    /* Enable interrupts */
    //enable_interrupt(); // call the global interrupt enable routine from Vector.S

    return;
}

int main(void) {
    primary_init();
    menu();







//    while (1) {
//    }
return 0;
}
