/* main.c 
** Lab 5
** Oct. 13th
*/
#include <string.h>
#include <stdint.h>
#include "Timer.h"
#include "Music.h"
#include "DAC.h"
#include "ST7735.h"

/***** public variable *****/
int musicPlaying;
int songEnd;
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
#define PF3       (*((volatile uint32_t *)0x40025020))


/** instruments **/
#define NUMBER_OF_INSTRUMENTS  5
#define TABLE_SIZE 64
const unsigned short Wave[64] = {  
  2048,2224,2399,2571,2737,2897,3048,3190,3321,3439,3545,
  3635,3711,3770,3813,3839,3848,3839,3813,3770,3711,3635,
  3545,3439,3321,3190,3048,2897,2737,2571,2399,2224,2048,
  1872,1697,1525,1359,1199,1048,906,775,657,551,461,385,326,283,
  257,248,257,265,300,385,461,551,657,775,906,1048,1199,1359,1525,1697,1872
};

const unsigned short Oboe[TABLE_SIZE] = {
	2126,2164,2238,2550,3356,3496,2550,1510,1322,1322,1406,1462,1538,1690,2078,2268,2418,2664,2930,
	3090,2854,3176,2740,2172,1416,1038,896,980,1132,1368,1604,1984,2126,2164,2238,2550,3356,3496,2550,
	1510,1322,1322,1406,1462,1538,1690,2078,2268,2418,2664,2930,3090,2854,3176,2740,2172,1416,1038,
	896,980,1132,1368,1604,1984
};

const unsigned short Trumpet[TABLE_SIZE] = {
	2014,2176,2312,2388,2134,1578,606,198,1578,3020,2952,2346,2134,2074,2168,2124,2022,2030,2090,2124,
	2022,2022,2116,2226,2168,2150,2158,2210,2176,2098,2030,2090,2014,2176,2312,2388,2134,1578,606,198,
	1578,3020,2952,2346,2134,2074,2168,2124,2022,2030,2090,2124,2022,2022,2116,2226,2168,2150,2158,
	2210,2176,2098,2030,2090
};

const unsigned short Flute[TABLE_SIZE] = {
	2014,2504,2748,3096,3396,3594,3650,3594,3350,3124,2766,2438,2184,2014,1826,1780,1666,1694,1620,1554,
	1488,1348,1196,1102,1018,952,990,1066,1178,1318,1516,1752,2014,2504,2748,3096,3396,3594,3650,3594,
	3350,3124,2766,2438,2184,2014,1826,1780,1666,1694,1620,1554,1488,1348,1196,1102,1018,952,990,1066,
	1178,1318,1516,1752
};

const unsigned short Guitar[TABLE_SIZE] = {  
  2048,2048,2052,2003,1869,1665,1459,1293,1238,1332,1563,
  1928,2343,2673,2922,3073,3116,3005,2792,2471,2080,1728,
  1510,1517,1754,2175,2716,3227,3579,3692,3544,3250,2907,
  2571,2347,2193,2103,2067,2054,2072,2157,2273,2367,2494,2527,2500,2363,
  2163,1948,1731,1602,1557,1621,1689,1717,1718,1701,1698,1750,1843,1954,2048,2048,2048
};


static uint16_t melody_out;
static uint16_t harmony_out;
static long melody_note_duration;     // made it global because of initial note
static long harmony_note_duration;

struct Note moonlight_melody[] = {
		{Ab4, triplet},{Db5, triplet},{E5, triplet},{Ab4, triplet},{Db5, triplet},{E5, triplet},{Ab4, triplet},{Db5, triplet},{E5, triplet},{Ab4, triplet},{Db5, triplet},{E5, triplet},
		{Ab4, triplet},{Db5, triplet},{E5, triplet},{Ab4, triplet},{Db5, triplet},{E5, triplet},{Ab4, triplet},{Db5, triplet},{E5, triplet},{Ab4, triplet},{Db5, triplet},{E5, triplet},
		{Bb4, triplet},{Db5, triplet},{E5, triplet},{Bb4, triplet},{Db5, triplet},{E5, triplet},{Bb4, triplet},{D5, triplet},{Gb5, triplet},{Bb4, triplet},{D5, triplet},{Gb5, triplet},
		{Ab4, triplet},{B5, triplet},{Gb5, triplet},{Ab4, triplet},{Db5, triplet},{E5, triplet},{Ab4, triplet},{Db5, triplet},{Eb5, triplet},{F4, triplet},{B5, triplet},{Eb5, triplet},
		{E4, triplet},{Ab4, triplet},{C5, triplet},{Ab4, triplet},{Db5, triplet},{E5, triplet},{Ab4, triplet},{Db5, triplet},{E5, triplet},{Ab4, triplet},{Db5, triplet},{E5, triplet},
		{Ab4, triplet},{Eb5, triplet},{Gb5, triplet},{Ab4, triplet},{Eb5, triplet},{Gb5, triplet},{Ab4, triplet},{Eb5, triplet},{Gb5, triplet},{Ab4, triplet},{Eb5, triplet},{Gb5, triplet},
		{Ab4, triplet},{Db5, triplet},{E5, triplet},{Ab4, triplet},{Db5, triplet},{E5, triplet},{Bb4, triplet},{Db5, triplet},{Gb5, triplet},{Bb4, triplet},{Db5, triplet},{Gb5, triplet},
		{Ab4, triplet},{B4, triplet},{E5, triplet},{Ab4, triplet},{B4, triplet},{E5, triplet},{Bb4, triplet},{B4, triplet},{Eb5, triplet},{Bb4, triplet},{B4, triplet},{Eb5, triplet},
		{Ab4, triplet},{B4, triplet},{E5, triplet},{Ab4, triplet},{B4, triplet},{E5, triplet},{Ab4, triplet},{B4, triplet},{E5, triplet},{Ab4, triplet},{B4, triplet},{E5, triplet},
		{G4, triplet},{B4, triplet},{E5, triplet},{G4, triplet},{B4, triplet},{E5, triplet},{G4, triplet},{B4, triplet},{E5, triplet},{G4, triplet},{B4, triplet},{E5, triplet},
		{G4, triplet},{B4, triplet},{F5, triplet},{G4, triplet},{B4, triplet},{F5, triplet},{G4, triplet},{B4, triplet},{F5, triplet},{G4, triplet},{B4, triplet},{F5, triplet},
		{G4, triplet},{C5, triplet},{E5, triplet},{G4, triplet},{B4, triplet},{E5, triplet},{G4, triplet},{Db5, triplet},{E5, triplet},{Gb4, triplet},{Db5, triplet},{E5, triplet}
};

struct Note moonlight_melody_mp[] = {
		{R, whole},
		{R, whole},
		{R, whole},
		{R, dottedhalf},{Ab5, dottedeighth},{Ab5, sixteenth},
		{Ab5, dottedhalf},{Ab5, dottedeighth},{Ab5, sixteenth},
		{Ab5, half},{A5, half},
		{Ab5, half},{Gb5, quarter},{B5, quarter},
		{E5, quarter},{R, dottedhalf},
		{R, dottedhalf},{G5, dottedeighth},{G5, sixteenth},
		{G5, dottedhalf},{G5, dottedeighth},{G5, sixteenth},
		{G5, dottedhalf},{Gb5, quarter}
};

struct Note moonlight_melody_BG1[] = {
		{Db4, whole},
		{B3, whole},
		{A3, half},{Gb3, half},
		{Ab3, half},{Ab3, half},
		{Db4, whole},
		{C4, whole},
		{Db4, half},{Gb3, half},
		{B3, half},{B3, half},
		{E4, whole},
		{E4, whole},
		{D4, whole},
		{C4, quarter},{B3, quarter},{Bb3, half}
};

struct Note moonlight_melody_BG2[] = {
		{C3, whole},
		{C3, whole},
		{C3, whole},
		{C3, whole},
		{C3, whole},
		{C3, whole},
		{C3, whole},
		{C3, whole},
		{C3, whole},
		{C3, whole},
		{C3, whole},
		{C3, whole},
};

struct Song song = {
	196, moonlight_melody, Guitar, 132, moonlight_melody_mp, Guitar, 24
	
};

static const unsigned short *instruments[NUMBER_OF_INSTRUMENTS] = {Wave, Trumpet, Flute, Guitar, Oboe};
static char *instrumentText[NUMBER_OF_INSTRUMENTS] = {"Wave", "Trumpet", "Flute", "Guitar", "Oboe"};
static unsigned curInstrument;  // not init in Music_Init, so that when rewind the instrument does not get reset
static void melody_handler(void);
static void harmony_handler(void);	
static unsigned long getNoteDuration(struct Note note, uint16_t tempo);
static unsigned long getBeatDuration(uint16_t tempo);
	
void Music_Init(void) {
	melody_note_duration = getNoteDuration(*song.melody, song.tempo);
	harmony_note_duration = getNoteDuration(*song.harmony, song.tempo);
	curInstrument = 0;
	musicPlaying = 1;
	melody_out = 0;
	harmony_out = 0;
	songEnd = 0;
	Timer2A_Init(&melody_handler, (*song.melody).note);
	Timer3A_Init(&harmony_handler, (*song.harmony).note);
}


  static uint16_t melodyInstrumentIndex = 0;
	static uint16_t melodyNoteIndex = 0;
	static long melodyLasts = 0;

static void melody_handler(void) {
	PF3 ^= 0x08;
	PF3 ^= 0x08;
	if (melodyNoteIndex >= song.melody_note_num) {  // done playing the song
			melody_out = 0;
	}
	else if (song.melody[melodyNoteIndex].note == R) {	 // a Rest
			// keeping the previous melody out value;
				melodyLasts++;
	}
	else {
			melody_out = song.melody_instrument[melodyInstrumentIndex];
	melodyInstrumentIndex = (melodyInstrumentIndex + 1) % TABLE_SIZE;
	melodyLasts++;
	}
			
	uint16_t output = (melody_out + harmony_out) / 4;
	DAC_Out(output);
	
	if (melodyLasts >= melody_note_duration) {  // this note has been played long enough
		// move on to the next note
//		noteIndex = noteIndex + 1;
		melodyNoteIndex = melodyNoteIndex + 1;
		melodyInstrumentIndex = 0;
		melodyLasts = 0;
		melody_note_duration = getNoteDuration(song.melody[melodyNoteIndex], song.tempo); 		// update the note duration
		if (melodyNoteIndex >  song.melody_note_num) {
			songEnd = 1; // Not entirely valid since harmony might still be playing, but should end soon if they have same number of bars
			Timer2A_Disarm();
		}			
		else Timer2A_Period(song.melody[melodyNoteIndex].note);  // change the int period to the new note
	}
	
		PF3 ^= 0x08;
}

  static uint16_t harmonyInstrumentIndex = 0;
	static uint16_t harmonyNoteIndex = 0;
	static long harmonyLasts = 0;

static void harmony_handler(void) {

	if (harmonyNoteIndex >= song.harmony_note_num) {  // done playing the song
			harmony_out = 0;
	}
		else if (song.harmony[harmonyNoteIndex].note == R) {	 // a Rest
			// keeping the previous melody out value;
				harmonyLasts++;
	}
	else {
	harmony_out = song.harmony_instrument[harmonyInstrumentIndex];
	harmonyInstrumentIndex = (harmonyInstrumentIndex + 1) % TABLE_SIZE;
	harmonyLasts++;
	}
	
	uint16_t output = (melody_out + harmony_out) / 4;
	DAC_Out(output);
	
	if (harmonyLasts >= harmony_note_duration) {  // this note has been played long enough
		// move on to the next note
		harmonyNoteIndex = harmonyNoteIndex + 1;
		harmonyInstrumentIndex = 0;
		harmonyLasts = 0;
		harmony_note_duration = getNoteDuration(song.harmony[harmonyNoteIndex], song.tempo); 		// update the note duration
		if (harmonyNoteIndex >  song.harmony_note_num) {
			songEnd = 1; // Not entirely valid since melody might still be playing, but should end soon if they have same number of bars
			Timer3A_Disarm();
		}			
		Timer3A_Period(song.harmony[harmonyNoteIndex].note);  // change the int period to the new note
	}
}

/*
  Start the music
*/
void Music_Play(void){
	Timer2A_Arm();
	Timer3A_Arm();
	musicPlaying = 1;
	ST7735_OutUDec(88);
}

void Music_Pause(void){
	Timer2A_Disarm();
	Timer3A_Disarm();
	musicPlaying = 0;
	ST7735_OutUDec(44);
}

void Music_Rewind(void) {
	Music_Pause();
 ST7735_OutUDec(123321);
	melody_note_duration = getNoteDuration(*song.melody, song.tempo);
	harmony_note_duration = getNoteDuration(*song.harmony, song.tempo);
	 melodyInstrumentIndex = 0;
	melodyNoteIndex = 0;
	melodyLasts = 0;
	 harmonyInstrumentIndex = 0;
	 harmonyNoteIndex = 0;
	harmonyLasts = 0;
			Timer2A_Period((*song.melody).note);  // change the int period to the new note

			Timer3A_Period((*song.harmony).note);  // change the int period to the new note

		melody_out = 0;
	harmony_out = 0;
	songEnd = 0;
	Music_Play();

}

void Music_ChangeIntrument(void) {
	curInstrument = (curInstrument + 1) % NUMBER_OF_INSTRUMENTS;
	song.melody_instrument = instruments[curInstrument];
  song.harmony_instrument = instruments[curInstrument];
	Output_Clear();
	ST7735_DrawString(0,0,instrumentText[curInstrument],ST7735_YELLOW);
}

/* 
assume time signature n/4: a beat is quarter-note 
returns the number of periods for one beat given TEMPO of the song
*/
static unsigned long getBeatDuration(uint16_t tempo) {
	// 	TEMPO beats per (minute = 60 * CLOCK_RATE periods) 
	unsigned long periods_per_beat = CLOCK_RATE / tempo * 60;
	return periods_per_beat;
}

/*
	return number of interrupts for the given note length and note freq (period)
*/
static unsigned long getNoteDuration(struct Note note, uint16_t tempo) {
	unsigned long periods_per_beat = getBeatDuration(tempo);
	/*
	whole = 16 = 4 beats
	so LENGTH/4 = number of beats for the note
	so periods of the note = LENGTH/4 * periods_per_beat
	*/
	unsigned long periods_for_note = note.length * periods_per_beat /4;
	return periods_for_note / note.note;   // note.note is the period of interrupt (taken into consideration the TABLE_SIZE)
}


