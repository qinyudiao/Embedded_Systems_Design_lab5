
 //note periods = 80000000 / (freq * TABLE_SIZE)
 //TABLE_SIZE = 64
#define C2 19111   // 65.406 Hz
#define Db2 18039   // 69.296 Hz
#define D2 17026   // 73.416 Hz
#define Eb2 16071   // 77.782 Hz
#define E2 15169   // 82.407 Hz
#define F2 14317   // 87.307 Hz
#define Gb2 13514   // 92.499 Hz
#define G2 12755   // 97.999 Hz
#define Ab2 12039   // 103.826 Hz
#define A2 11364   // 110.000 Hz
#define Bb2 10726   // 116.541 Hz
#define B2 10124   // 123.471 Hz
#define C3 9556   // 130.813 Hz
#define Db3 9019   // 138.591 Hz
#define D3 8513   // 146.832 Hz
#define Eb3 8035   // 155.563 Hz
#define E3 7584   // 164.814 Hz
#define F3 7159   // 174.614 Hz
#define Gb3 6757   // 184.997 Hz
#define G3 6378   // 195.998 Hz
#define Ab3 6020   // 207.652 Hz
#define A3 5682   // 220.000 Hz
#define Bb3 5363   // 233.082 Hz
#define B3 5062   // 246.942 Hz
#define C4 4778   // 261.626 Hz
#define Db4 4510   // 277.183 Hz
#define D4 4257   // 293.665 Hz
#define Eb4 4018   // 311.127 Hz
#define E4 3792   // 329.628 Hz
#define F4 3579   // 349.228 Hz
#define Gb4 3378   // 369.994 Hz
#define G4 3189   // 391.995 Hz
#define Ab4 3010   // 415.305 Hz
#define A4 2841   // 440.000 Hz
#define Bb4 2681   // 466.164 Hz
#define B4 2531   // 493.883 Hz
#define C5 2389   // 523.251 Hz
#define Db5 2255   // 554.365 Hz
#define D5 2128   // 587.330 Hz
#define Eb5 2009   // 622.254 Hz
#define E5 1896   // 659.255 Hz
#define F5 1790   // 698.456 Hz
#define Gb5 1689   // 739.989 Hz
#define G5 1594   // 783.991 Hz
#define Ab5 1505   // 830.609 Hz
#define A5 1420   // 880.000 Hz
#define Bb5 1341   // 932.328 Hz
#define B5 1265   // 987.767 Hz
#define C6 1194   // 1046.502 Hz
#define Db6 1127   // 1108.731 Hz
#define D6 1064   // 1174.659 Hz
#define Eb6 1004   // 1244.508 Hz
#define E6 948   // 1318.510 Hz
#define F6 895   // 1396.913 Hz
#define Gb6 845   // 1479.978 Hz
#define G6 797   // 1567.982 Hz
#define Ab6 752   // 1661.219 Hz
#define A6 710   // 1760.000 Hz
#define Bb6 670   // 1864.655 Hz
#define B6 633   // 1975.533 Hz
#define C7 597   // 2093.005 Hz
#define R  5000  // sepecial case for rest

// note length
#define whole 16
#define half 8
#define quarter 4
#define eighth 2
#define sixteenth 1
#define triplet 1.333333
#define dottedeighth 3
#define dottedhalf 12


#define CLOCK_RATE 80000000

struct Note {
	unsigned short note;		// the note period value  defined
	double length;	// number of beats for the note
};

struct Song {
	unsigned short tempo;		//beats per minute
	
	struct Note *melody;
	const unsigned short *melody_instrument;
	unsigned short melody_note_num;
	
	struct Note *harmony;
	const unsigned short *harmony_instrument;
	unsigned short harmony_note_num;

};

extern int musicPlaying;
extern int songEnd;

void Music_Init(void);
void Music_Play(void);
void Music_Pause(void);
void Music_Rewind(void);
void Music_ChangeIntrument(void);
