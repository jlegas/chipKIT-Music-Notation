/* Global function and extern vars declarations
 * for the miniproject
 *
 * For copyright and licensing, see file COPYING */

#define PRESCALE 64
#define PROCESSOR_FREQ 80000000
#define NUMBER_OF_NOTES 155
#define MELODY_NOTES 256 //max notes in the melody


//hardware fucntions
int getbtns(void);

int getsw(void);

int get_button(void);

int get_switch(void);

//project functions
void play_note(float note, int duration);

void music_player(int note_counter, int melody[], int tempo[]);

void player(void);

void settings(void);

void volume(void);

void tempo(void);

void help(void);

void menu(void);

void tempo_calculation(void);

extern int volume_level;

//editor functionality
void music_editor(void);

void get_notes(void);

//graphical editor
void staff_editor(void);


//delay in ms
void delay_ms(int duration);


/* Declare display-related functions from func.c */
void display_4lines(char *line0, char *line1, char *line2, char *line3);

void display_image(int x, const uint8_t *data);

void display_note(int x, const uint8_t *data);

void display_note_inv(int x, const uint8_t *data);

void display_fullscreen(const uint8_t *data);

void display_init(void);

void display_string(int line, char *s);

void display_update(void);

uint8_t spi_send_recv(uint8_t data);

/* Declare lab-related functions from func.c */
void quicksleep(int cyc);

/* Declare display_debug - a function to help debugging.
   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug(volatile int *const addr);

//convert number to string
char *itoa(char b[], int i);

//concatenate two strings
char *strcat(char *a, char *b);



//declare the data used in the program:
/* Declare bitmap array containing font */
extern const uint8_t const font[128 * 8];
/* Declare bitmap array containing icon */
extern const uint8_t const icon[128];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

//array of notes for the alpha-editor
extern int melody_notes[MELODY_NOTES];

//array of tempos, corresponding to the notes
//whole,half,fourth,eighth,sixteenth-1,2,4,8,16
extern int tempo_notes[MELODY_NOTES];


//array of notes for the staf-editor
extern int staff_notes[MELODY_NOTES];

//staff tempo notes
extern int staff_tempos[MELODY_NOTES];

//note codes A..G=1..8, -,b,s=0,1,2, 0..8=0..8
extern int note_codes[NUMBER_OF_NOTES];
//note names as an array
extern char *note_names[NUMBER_OF_NOTES];
//note frequencies
extern float note_frequencies[NUMBER_OF_NOTES];
//global position counter in the melody_note[]
extern int staff_note_counter;
extern int alpha_note_counter;

//indicates where in the program the execution currently is
extern int placeholder;
//music timing
extern int tempobpm; //beats per minute
extern int quarter_note; //duration of a quarter note in ms
extern int half_note; //duration of a half-note
extern int eighth_note; //duration of an eighth-note
extern int sixteenth_note; //duration of a sixteenth-note
extern int whole_note; //duration of a whole-note