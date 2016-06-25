/* This file contains the staff music editor programme
 *
 * For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "functions.h"
#include "graphics.h"


//how many notes are in the melody atm
int alpha_note_counter = 78;

/*
 * Local editor variables:
*/

//indicator for the current note edited
int current_note = 77;

//indicates which part of the note is being edited
//0-letter,1-sharp flat,2-duration
int current_edit = 0;

//returns an index for the note codes-frequencies-names arrays
int get_note_array_ind() {
    int k = 0;
    for (k = 0; k < NUMBER_OF_NOTES; k++) {
        if (melody_notes[current_note] == note_codes[k])
            return k;
    }
    return 0;
}

void display_note_name(int line) {
    display_string(line, note_names[get_note_array_ind()]);
}

void display_note_number(int line) {
    char a[16];
    itoa(a, current_note);
    display_string(line, a);
}

int get_tempo_duration(void) {
    switch (tempo_notes[current_note]) {
        case 1 :
            return whole_note;

        case 2 :
            return half_note;
        case 4 :
            return quarter_note;

        case 8 :
            return eighth_note;
        case 16 :
            return sixteenth_note;
        default :
            return 0;
    }
}

void display_tempo(int line) {
//    char a[16] = "tempo: 1/";
    char tempo_string[16];
    itoa(tempo_string, tempo_notes[current_note]);
//    strcat(a, tempo_string);
    display_string(line, tempo_string);
}

//show which attribute is being edited
void show_current_attribute(int line) {
    switch (current_edit) {
        case 1 :
            display_string(line, "< sharp / flat >");
            break;
        case 2 :
            display_string(line, "<    octave    >");
            break;
        case 3 :
            display_string(line, "< length (1/X) >");
            break;
        default :
            display_string(3, "<     note     >");
    }
}

void current_attribute_step(void) {
    if (current_edit == 3)
        current_edit = 0;
    else
        current_edit++;
    show_current_attribute(3);
    display_update();
    delay_ms(250);
}

//cycles through the values of an attribute upwards
void edit_up(void) {
    switch (current_edit) {
        case 1 : //sharp (not B,E), flat (not C,F)
            //ADG
            if (melody_notes[current_note] == 0)
                break;
            else if (melody_notes[current_note] / 100 == 1 || melody_notes[current_note] / 100 == 4 ||
                     melody_notes[current_note] / 100 == 7) {
                if ((melody_notes[current_note] / 10) % 10 == 2)
                    melody_notes[current_note] -= 20;
                else
                    melody_notes[current_note] += 10;
                display_note_name(0);
            }
                //BE
            else if (melody_notes[current_note] / 100 == 2 || melody_notes[current_note] / 100 == 5) {
                if ((melody_notes[current_note] / 10) % 10 == 1)
                    melody_notes[current_note] -= 10;
                else
                    melody_notes[current_note] += 10;
                display_note_name(0);
            }
                //CF
            else if (melody_notes[current_note] / 100 == 3 || melody_notes[current_note] / 100 == 6) {
                if ((melody_notes[current_note] / 10) % 10 == 2)
                    melody_notes[current_note] -= 20;
                else
                    melody_notes[current_note] += 20;
                display_note_name(0);
            }
            break;
        case 2 : //octave
            if (melody_notes[current_note] == 0)
                break;
            else if (melody_notes[current_note] % 10 == 8)
                melody_notes[current_note] -= 8;
            else
                melody_notes[current_note] += 1;
            display_note_name(0);
            break;
        case 3 : //tempo
            if (tempo_notes[current_note] == 16)
                tempo_notes[current_note] = 1;
            else
                tempo_notes[current_note] *= 2;
            char tempo_string[16];
            itoa(tempo_string, tempo_notes[current_note]);
            display_string(1, tempo_string);
            delay_ms(250);
            display_note_name(0);
            break;
        default : //note freq
            //all
            if ((melody_notes[current_note] / 10) % 10 == 0) {
                //G
                if (melody_notes[current_note] / 100 == 7)
                    melody_notes[current_note] = 0;
                    //ABCDEF
                else if (melody_notes[current_note] == 0)
                    melody_notes[current_note] = 100;
                else
                    melody_notes[current_note] += 100;
                display_note_name(0);
            }
                //flats
            else if ((melody_notes[current_note] / 10) % 10 == 1) {
                if (melody_notes[current_note] / 100 == 7) //G
                    melody_notes[current_note] -= 600;
                else if (melody_notes[current_note] / 100 == 1 || melody_notes[current_note] / 100 == 4) //A,D
                    melody_notes[current_note] += 100;
                else if (melody_notes[current_note] / 100 == 2 || melody_notes[current_note] / 100 == 5) //B,E
                    melody_notes[current_note] += 200;
                display_note_name(0);
            }
                //sharps
            else if ((melody_notes[current_note] / 10) % 10 == 2) {
                if (melody_notes[current_note] / 100 == 7) //G
                    melody_notes[current_note] -= 600;
                else if (melody_notes[current_note] / 100 == 3 || melody_notes[current_note] / 100 == 6) //C,F
                    melody_notes[current_note] += 100;
                else if (melody_notes[current_note] / 100 == 1 || melody_notes[current_note] / 100 == 4) //A,D
                    melody_notes[current_note] += 200;
                display_note_name(0);
            }
    }
    display_update();
    delay_ms(250);
}

//while sw4 on: btn2  <, btn1  >, btn4  n, btn3 y
void navigate(void) {
    display_4lines("Loading...", "", "while |4| on:", "<(4)     (1)>");
    delay_ms(250);
    while (1) {
        //move right
        if (get_button() == 1) {
            if (current_note < (MELODY_NOTES - 1)) {
                current_note++;
                if (alpha_note_counter == current_note) {
                    alpha_note_counter++;
                    melody_notes[current_note] = melody_notes[current_note - 1];
                    tempo_notes[current_note] = tempo_notes[current_note - 1];
                }
            }
            delay_ms(250);
        }
            //move left
        else if (get_button() == 4) {
            if (current_note > 0) {
                current_note--;
            }
            delay_ms(250);
        }
//        else if (get_button() == 3) {
//            play_note(note_frequencies[get_note_array_ind()], 500);
//            delay_ms(250);
//        }

        else if (get_switch() == 1) {
            display_4lines("navigate notes", "of the melody", "while |4| on:", "<(4)     (1)>");
            while (get_switch() == 1) {
                delay_ms(250);
            }
        }
        else if (get_switch() == 0)
            return;

        display_note_name(0);
        display_note_number(1);
        display_update();
    }
}

void music_editor(void) {
    //Initialize editor
    display_4lines("Loading...", "", "", "");
    delay_ms(250);
    while (1) {
        display_note_name(0);
        display_tempo(1);
        display_string(2, "");
        show_current_attribute(3);
        display_update();
        while (1) {
            if (get_button() == 1) {
                current_attribute_step();
                delay_ms(250);
            }
            else if (get_button() == 2) {
                edit_up();
                delay_ms(250);
            }
            else if (get_switch() == 4) {
                navigate();
                delay_ms(250);
                break;
            }
            else if (get_switch() == 1) {
                display_4lines("<Current note>", "<length>", "(4)exit  (3)play", "(2)+-    (1)>   ");
                while (get_switch() == 1) {
                    delay_ms(250);
                }
                break;
            }
            else if (get_button() == 3) {
                play_note(note_frequencies[get_note_array_ind()], get_tempo_duration());
                delay_ms(250);
            }
            else if (get_button() == 4)
                return;
        }
    }
}

