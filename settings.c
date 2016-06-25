/* This file contains the settings programme
 *
 * For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "functions.h"
#include "graphics.h"


//music volume
int volume_level = 10; //defines the volume level - duty cycle % for PWM. min 1, max 50

//music timing
int tempobpm = 150; //beats per minute
int quarter_note; //duration of a quarter note in ms
int half_note; //duration of a half-note
int eighth_note; //duration of an eighth-note
int sixteenth_note; //duration of a sixteenth-note
int whole_note; //duration of a whole-note

void volume(void) {
    while (1) {
        char volume_level_string[16];
        itoa(volume_level_string, volume_level);
        display_string(0, "volume   (4)exit");
        display_string(1, "(2)++    (3)--");
        display_string(2, "current vol:");
        display_string(3, volume_level_string);
        display_update();
        delay_ms(250);

        while (1) {
            if (get_button() == 2 && volume_level < 10) {
                volume_level++;
                break;
            }
            else if (get_button() == 3 && volume_level > 0) {
                volume_level--;
                break;
            }
            else if (get_button() == 4)
                return;
        }

    }
}

void tempo_calculation(void) {
    quarter_note = (60000 / tempobpm); //duration of a quarter note in ms
    half_note = 2 * quarter_note; //duration of a half-note
    eighth_note = quarter_note / 2; //duration of an eighth-note
    sixteenth_note = quarter_note / 4; //duration of a sixteenth-note
    whole_note = 4 * quarter_note; //duration of a whole-note
}

void tempo(void) {
    while (1) {
        char tempo_level_string[16];
        itoa(tempo_level_string, tempobpm);
        display_string(0, "tempo    (4)exit");
        display_string(1, "(2)++    (3)--");
        display_string(2, "current bpm:");
        display_string(3, tempo_level_string);
        display_update();
        delay_ms(250);

        while (1) {

            if (get_button() == 2) {
                tempobpm++;
                tempo_calculation();
                break;
            }
            else if (get_button() == 3 && tempobpm > 30) {
                tempobpm--;
                tempo_calculation();
                break;
            }
            else if (get_button() == 4)
                return;
        }
    }
}

void help(void) {
    return;
}

void settings(void) {
    while (1) {
        display_string(0, "(1)volume");
        display_string(1, "(2)tempo");
        display_string(2, "");
        display_string(3, "(4)back to menu");
        display_update();

        delay_ms(250);

        while (1) {
            if (get_button() == 1) {
                volume();
                break;
            }
            else if (get_button() == 2) {
                tempo();
                break;
            }
//            else if (get_button() == 3) {
//                help();
//                break;
//            }
            else if (get_button() == 4)
                return;
        }
    }


}
