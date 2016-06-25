/* This file contains the staff music editor programme
 *
 * For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "functions.h"
#include "graphics.h"


//how many notes is in the melody
int staff_note_counter = 4;

/*
 * Local editor variables:
*/

uint8_t note_image[64];

//selection of notes supported by staff editor
int staff_notes_set[] = {
        104,
        204,
        304,
        305,
        404,
        405,
        504,
        505,
        604,
        605,
        704,
        705,
};
int staff_notes_set_ordered[] = {
        304,
        404,
        504,
        604,
        704,
        104,
        204,
        305,
        405,
        505,
        605,
        705,
};
//selection of tempos supported by staff editor
int staff_tempos_set[] = {
        2,
        4,
        8,
        16,
};


//indicator for the current note edited
int staff_current_note = 3;

//indicates which part of the note is being edited
//0-note 1 length
int staff_current_edit = 0;

//returns an index for the note codes-frequencies-names arrays
int get_staff_note_array_ind() {
    int k = 0;
    for (k = 0; k < NUMBER_OF_NOTES; k++) {
        if (staff_notes[staff_current_note] == note_codes[k])
            return k;
    }
    return 0;
}


int get_staff_tempo_duration(void) {
    switch (staff_tempos[staff_current_note]) {
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

//draw notes (which note (1-5), where from the set of notes, where from the set of tempos)
void display_staff_note(int x, int note_set_index, int tempo_set_index) {
    //describe distance from X axis
    switch (x) {
        case 1:
            x = 22;
            break;
        case 2:
            x = 39;
            break;
        case 3:
            x = 56;
            break;
        case 4:
            x = 73;
            break;
        case 5:
            x = 90;
            break;
        default:
            x = 0;
    }

    if (note_set_index == 99 || tempo_set_index == 99) {
        if (x == 22)
            display_note_inv(x, empty_note);
        else
            display_note(x, empty_note);
        return;
    }

    int i;
    for (i = 0; i < 64; i++) {
        note_image[i] = note_graphics[i + ((tempo_set_index * 64 * 12) + (note_set_index * 64))];
    }

    if (x == 22)
        display_note_inv(x, note_image);
    else
        display_note(x, note_image);
}

int get_staff_note_set_index(int note_code) {
    int i;
    //magic number is the number of notes in staff_notes_set
    for (i = 0; i < 12; i++) {
        if (note_code == staff_notes_set[i])
            return i;
    }
    return 99;

}

int get_staff_note_set_ordered_index(int note_code) {
    int i;
    //magic number is the number of notes in staff_notes_set
    for (i = 0; i < 12; i++) {
        if (note_code == staff_notes_set_ordered[i])
            return i;
    }
    return 99;
}

int get_staff_tempo_set_index(int tempo_code) {
    int i;
    for (i = 0; i < 4; i++) { //magic number is the number of tempos in staff_tempos_set
        if (tempo_code == staff_tempos_set[i]) {
            return i;
        }
    }
    return 99;
}

void staff_show_current_notes(void) {
    display_staff_note(1, get_staff_note_set_index(staff_notes[staff_current_note]),
                       get_staff_tempo_set_index(staff_tempos[staff_current_note]));
    if (staff_current_note + 1 < MELODY_NOTES)
        display_staff_note(2, get_staff_note_set_index(staff_notes[staff_current_note + 1]),
                           get_staff_tempo_set_index(staff_tempos[staff_current_note + 1]));
    else display_staff_note(2, -1, -1);
    if (staff_current_note + 2 < MELODY_NOTES)
        display_staff_note(3, get_staff_note_set_index(staff_notes[staff_current_note + 2]),
                           get_staff_tempo_set_index(staff_tempos[staff_current_note + 2]));
    else display_staff_note(3, -1, -1);
    if (staff_current_note + 2 < MELODY_NOTES)
        display_staff_note(4, get_staff_note_set_index(staff_notes[staff_current_note + 3]),
                           get_staff_tempo_set_index(staff_tempos[staff_current_note + 3]));
    else display_staff_note(4, -1, -1);
    if (staff_current_note + 2 < MELODY_NOTES)
        display_staff_note(5, get_staff_note_set_index(staff_notes[staff_current_note + 4]),
                           get_staff_tempo_set_index(staff_tempos[staff_current_note + 4]));
    else display_staff_note(5, -1, -1);
}

void staff_show_current_attribute(void) {
    if (staff_current_edit == 1)
        display_note(110, edit_time);
    else
        display_note(110, edit_freq);
}

//0 - length, 1 - freq
void staff_current_attribute_step(void) {
    if (staff_current_edit == 1)
        staff_current_edit = 0;
    else
        staff_current_edit = 1;
    staff_show_current_attribute();
}


//cycles through the values of an attribute upwards
void staff_edit_up(void) {
    switch (staff_current_edit) {
        case 1 : //length 2
            if (staff_tempos[staff_current_note] == 16) {
                staff_tempos[staff_current_note] = staff_tempos_set[0];
            }
            else if (staff_tempos[staff_current_note] == 4) {
                staff_tempos[staff_current_note] = staff_tempos_set[2];
            }
            else if (staff_tempos[staff_current_note] == 8) {
                staff_tempos[staff_current_note] = staff_tempos_set[3];
            }
            else if (staff_tempos[staff_current_note] == 2) {
                staff_tempos[staff_current_note] = staff_tempos_set[1];
            }
            break;
        default : //note
            if (staff_notes[staff_current_note] == 705 || staff_notes[staff_current_note] == 0) {
                staff_notes[staff_current_note] = staff_notes_set_ordered[0];
            }
            else
                staff_notes[staff_current_note] = staff_notes_set_ordered[
                        get_staff_note_set_ordered_index(staff_notes[staff_current_note]) + 1];
    }
    display_staff_note(1, get_staff_note_set_index(staff_notes[staff_current_note]),
                       get_staff_tempo_set_index(staff_tempos[staff_current_note]));
    delay_ms(250);
}


//while sw4 on: btn2  <, btn1  >, btn4  n, btn3 y
void staff_navigate(void) {
    while (1) {
        char q[16];
        itoa(q, staff_current_note + 1);
        char r[16];
        itoa(r, staff_note_counter);
        display_4lines("current note", q, "out of", r);
        display_note(110, edit_leftright);
        //move right
        if (get_button() == 1) {
            if (staff_current_note < (MELODY_NOTES - 1)) {
                staff_current_note++;
                if (staff_note_counter == staff_current_note) {
                    staff_note_counter++;
                    staff_notes[staff_current_note] = staff_notes[staff_current_note - 1];
                    staff_tempos[staff_current_note] = staff_tempos[staff_current_note - 1];
                }
            }
        }
            //move left
        else if (get_button() == 2) {
            if (staff_current_note > 0) {
                staff_current_note--;
            }
        }
        else if (get_switch() == 0)
            return;
        else if (get_switch() == 1) {
            display_4lines("navigate notes", "of the melody", "while |4| on:", "<(4)     (1)>");
            while (get_switch() == 1) {
                delay_ms(250);
            }
        }
        delay_ms(250);
    }
}


void staff_editor(void) {
    //Initialize editor
    display_4lines("Loading...", "", "", "");
    delay_ms(250);
    while (1) {
        display_fullscreen(staff);
        staff_show_current_attribute();
        staff_show_current_notes();        //display current notes
        delay_ms(250);


        while (1) {
            if (get_button() == 1) {
                staff_current_attribute_step();
                delay_ms(250);
            }
            else if (get_button() == 2) {
                staff_edit_up();
                delay_ms(250);
            }
            else if (get_button() == 3) {
                play_note(note_frequencies[get_staff_note_array_ind()], get_staff_tempo_duration());
                delay_ms(250);
            }
            else if (get_switch() == 4) {
                staff_navigate();
                delay_ms(250);
                break;
            }
            else if (get_switch() == 1) {
                display_4lines("Choose a note", "and its length.", "(4)exit  (3)play", "(2)+-    (1)>   ");
                while (get_switch() == 1) {
                    delay_ms(250);
                }
                break;
            }
            else if (get_button() == 4)
                //exit_staff_editor
                return;
        }
    }

}//staff editor
