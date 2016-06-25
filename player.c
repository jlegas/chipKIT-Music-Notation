/* This file contains the music player programme
 *
 * For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "functions.h"
#include "graphics.h"


//plays the given note for a given time interval
void play_note(float note, int duration) {
    TMR2 = 0;
    int calculated_period = 0;
    if (note != 0) {
        calculated_period = PROCESSOR_FREQ / PRESCALE / note;
        PR2 = calculated_period; //initialize the PWM with the note's frequency
        OC1RS = (int) (calculated_period * volume_level / 100); //initialize the volume
        delay_ms(duration);
        OC1RS = 0;
        PR2 = 0;
    }
    else
        delay_ms(duration);
}

void music_player(int note_counter, int melody[], int tempo[]) {
    display_4lines("music playback", "", "", "");
    delay_ms(500);
    int i = 0;
    for (i = 0; i < note_counter; i++) {
        float note_to_play = 0;
        int duration_to_play = 0;
        if (get_button())
            break;
        int j = 0;
        if (melody[i] != 0) {
            for (j = 0; j < NUMBER_OF_NOTES; j++) {
                if (melody[i] == note_codes[j]) {
                    note_to_play = note_frequencies[j];
                    break;
                }
            }
        }
        switch (tempo[i]) {
            case 1 :
                duration_to_play = whole_note;
                break;
            case 2 :
                duration_to_play = half_note;
                break;
            case 4 :
                duration_to_play = quarter_note;
                break;
            case 8 :
                duration_to_play = eighth_note;
                break;
            case 16 :
                duration_to_play = sixteenth_note;
                break;
            default :
                duration_to_play = 0;
        }
        char a[16];
        char b[16];
        itoa(a, (int) note_to_play);
        itoa(b, duration_to_play);
        display_4lines("freq (Hz)", a, "duration (ms)", b);
        play_note(note_to_play, duration_to_play);
    }
    return;
}

void player(void) {
    while (1) {
        display_string(0, "(1)letter music");
        display_string(1, "(2)staff music");
        display_string(2, "");
        display_string(3, "(4)back to menu");
        display_update();
        delay_ms(250);

        while (1) {
            if (get_button() == 1) {
                music_player(alpha_note_counter, melody_notes, tempo_notes);
                delay_ms(250);
                break;
            }
            else if (get_button() == 2) {
                music_player(staff_note_counter, staff_notes, staff_tempos);
                delay_ms(250);
                break;
            }
            else if (get_button() == 4)
                return;
        }
    }
}