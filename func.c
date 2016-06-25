/* This file contains main functions for hw initialization
 *
 * For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "functions.h"
#include "graphics.h"


#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
    int i;
    for (i = cyc; i > 0; i--);
}

// delay of n miliseconds by using timeout flag IFS and timer 3
void delay_ms(int duration) {
    // introduce a timertest counter which checks for timeout for n times to get a n miliseconds timeout
    int timertest = 0; //used for delay_ms()
    IFSCLR(0) = (1 << 12); // clear timeout flag IFS T3IF
    while (timertest < duration) {
        if ((IFS(0) & 0x1000)) {
            timertest++;
            IFSCLR(0) = (1 << 12); // clear timeout flag IFS
        }
    }
}

/* display_debug
   A function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug(volatile int *const addr) {
    display_string(1, "Addr");
    display_string(2, "Data");
    //num32asc(&textbuffer[1][6], (int) addr); //removed num32asc
    //num32asc(&textbuffer[2][6], *addr); //removed num32asc
    display_update();
}

uint8_t spi_send_recv(uint8_t data) {
    while (!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while (!(SPI2STAT & 1));
    return SPI2BUF;
}

void display_init(void) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    quicksleep(10);
    DISPLAY_ACTIVATE_VDD;
    quicksleep(1000000);

    spi_send_recv(0xAE);
    DISPLAY_ACTIVATE_RESET;
    quicksleep(10);
    DISPLAY_DO_NOT_RESET;
    quicksleep(10);

    spi_send_recv(0x8D);
    spi_send_recv(0x14);

    spi_send_recv(0xD9);
    spi_send_recv(0xF1);

    DISPLAY_ACTIVATE_VBAT;
    quicksleep(10000000);

    spi_send_recv(0xA1);
    spi_send_recv(0xC8);

    spi_send_recv(0xDA);
    spi_send_recv(0x20);

    spi_send_recv(0xAF);
}

void display_4lines(char *line0, char *line1, char *line2, char *line3) {
    display_string(0, line0);
    display_string(1, line1);
    display_string(2, line2);
    display_string(3, line3);
    display_update();
}

void display_string(int line, char *s) {
    int i;
    if (line < 0 || line >= 4)
        return;
    if (!s)
        return;

    for (i = 0; i < 16; i++)
        if (*s) {
            textbuffer[line][i] = *s;
            s++;
        } else
            textbuffer[line][i] = ' ';
}

void display_image(int x, const uint8_t *data) {
    int i, j;

    for (i = 0; i < 4; i++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;

        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(x & 0xF);
        spi_send_recv(0x10 | ((x >> 4) & 0xF));

        DISPLAY_CHANGE_TO_DATA_MODE;

        for (j = 0; j < 32; j++)
            spi_send_recv(~data[i * 32 + j]);
    }
}

//dislays selected note in inverse
void display_note_inv(int x, const uint8_t *data) {
    int i, j;

    for (i = 0; i < 4; i++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;

        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(x & 0xF);
        spi_send_recv(0x10 | ((x >> 4) & 0xF));

        DISPLAY_CHANGE_TO_DATA_MODE;

        for (j = 0; j < 16; j++)
            spi_send_recv(data[i * 16 + j]);
    }
}

//dislays note
void display_note(int x, const uint8_t *data) {
    int i, j;

    for (i = 0; i < 4; i++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;

        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(x & 0xF);
        spi_send_recv(0x10 | ((x >> 4) & 0xF));

        DISPLAY_CHANGE_TO_DATA_MODE;

        for (j = 0; j < 16; j++)
            spi_send_recv(~data[i * 16 + j]);
    }
}

//displays fulscreen graphics
void display_fullscreen(const uint8_t *data) {
    int i, j;

    for (i = 0; i < 4; i++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;

        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(0 & 0xF);
        spi_send_recv(0x10 | ((0 >> 4) & 0xF));

        DISPLAY_CHANGE_TO_DATA_MODE;

        for (j = 0; j < 128; j++)
            spi_send_recv(~data[i * 128 + j]);
    }
}


void display_update(void) {
    int i, j, k;
    int c;
    for (i = 0; i < 4; i++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(0x0);
        spi_send_recv(0x10);

        DISPLAY_CHANGE_TO_DATA_MODE;

        for (j = 0; j < 16; j++) {
            c = textbuffer[i][j];
            if (c & 0x80)
                continue;

            for (k = 0; k < 8; k++)
                spi_send_recv(font[c * 8 + k]);
        }
    }
}

//returns the bits representing the values of the toggle switches on the I/O board
int getsw(void) {
    return ((PORTD >> 8) & 0xf);
}

//returns the bits representing the values of the buttons on the I/O board
int getbtns(void) {
    //buttons 2,3,4 mapped to PORTD, button 1 mapped to PORTF
    return ((((PORTD >> 5) & 0x7) << 1) | ((PORTF >> 1) & 0x1));
}

//returns the number of the button pressed (1..4)
int get_button(void) {
    if (getbtns() & 0b1)
        return 1;
    else if (getbtns() & 0b10)
        return 2;
    else if (getbtns() & 0b100)
        return 3;
    else if (getbtns() & 0b1000)
        return 4;
    else
        return 0;
}

int get_switch(void) {
    if (getsw() & 0b1)
        return 1;
    else if (getsw() & 0b10)
        return 2;
    else if (getsw() & 0b100)
        return 3;
    else if (getsw() & 0b1000)
        return 4;
    else
        return 0;
}


/*
//checks if two strings are different (0-same, 1-different)
int string_comp(char *str1, char *str2) {
    while (*str1 == *str2) {
        if (*str1 == '\0')
            return 0;
        str1++;
        str2++;
    }
    return 1;
}
*/

//converts integer to string
char *itoa(char b[], int i) {
    char const digit[] = "0123456789";
    char *p = b;
    if (i < 0) {
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do { //Move to where representation ends
        ++p;
        shifter = shifter / 10;
    } while (shifter);
    *p = '\0';
    do { //Move back, inserting digits
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}

//displays the initial menu screen
void menu(void) {
    tempo_calculation();
    char a[16];
    char b[16];
    itoa(a, tempobpm);
    itoa(b, whole_note);
    display_string(0, "tempo (1/X)");
    display_string(1, a);
    display_string(2, "whole note (ms)");
    display_string(3, b);
    display_update();
    delay_ms(1000);

    while (1) {
        display_string(0, "(1)music editor");
        display_string(1, "(2)staff editor");
        display_string(2, "(3)music player");
        display_string(3, "(4)settings");
        display_update();

        if (get_button() == 1) {
            music_editor();
            delay_ms(250);
        }
        else if (get_button() == 2) {
            staff_editor();
            delay_ms(250);
        }
        else if (get_button() == 3) {
            player();
            delay_ms(250);
        }
        else if (get_button() == 4) {
            settings();
            delay_ms(250);
        }
        else if (get_switch() == 1) {
            char a[16];
            char b[16];
            itoa(a, tempobpm);
            itoa(b, whole_note);
            display_string(0, "tempo (1/X)");
            display_string(1, a);
            display_string(2, "whole note (ms)");
            display_string(3, b);
            display_update();
            while (get_switch() == 1) {
                delay_ms(250);
            }
        }
    }
}

