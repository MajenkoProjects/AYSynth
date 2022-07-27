USBFS usbDevice;
USBManager USB(usbDevice, 0xf055, 0x8912, "Majenko Technologies", "AYSynth");
Audio_MIDI MIDI;
CDCACM uSerial;

// Change these to match your wiring
#define DA0	11
#define DA1 12
#define DA2 13
#define DA3 14
#define DA4 0
#define DA5 2
#define DA6 3
#define DA7 7
#define RST 18
#define BC1 9
#define BDIR 10
#define CLOCK 6

// AY-3-8912 register numbers
#define AY_FINE_A 			0
#define AY_COURSE_A			1
#define AY_FINE_B			2
#define AY_COURSE_B			3
#define AY_FINE_C			4
#define AY_COURSE_C			5
#define AY_VOL_A			8
#define AY_VOL_B			9
#define AY_VOL_C			10

// Storage and control for one channel
typedef struct {
	uint8_t id;			// Channel number 0-2
	uint32_t start;		// Time the channel started playing
	uint8_t volume;		// Current volume for the channel
	uint32_t freq;		// Base frequency for the channel
	uint32_t f_fine;	// Fine tune register
	uint32_t f_coarse;	// Coarse tune register
	uint32_t amp;		// Amplitude register
	uint8_t inst;		// Selected instrument
	uint32_t tick;		// Instrument tick counter
	uint32_t phase;		// Instrument envelope phase
	uint32_t angle;		// Instrument vibrato phase
} channel;

// Definitions for the three channels
channel channels[3] = {
	{ 0, 0, 0, 0, AY_FINE_A, AY_COURSE_A, AY_VOL_A, 0 },
	{ 1, 0, 0, 0, AY_FINE_B, AY_COURSE_B, AY_VOL_B, 1 },
	{ 2, 0, 0, 0, AY_FINE_C, AY_COURSE_C, AY_VOL_C, 2 },
};

// Frequency for each MIDI note. Ignore the decimals, the get
// truncated - they are just there from copying-and-pasting.
const uint32_t notes[] = {
	/*   0 */ 8.18,
	/*   1 */ 8.66,
	/*   2 */ 9.18,
	/*   3 */ 9.72,
	/*   4 */ 10.3,
	/*   5 */ 10.91,
	/*   6 */ 11.56,
	/*   7 */ 12.25,
	/*   8 */ 12.98,
	/*   9 */ 13.75,
	/*  10 */ 14.57,
	/*  11 */ 15.43,
	/*  12 */ 16.35,
	/*  13 */ 17.32,
	/*  14 */ 18.35,
	/*  15 */ 19.45,
	/*  16 */ 20.6,
	/*  17 */ 21.83,
	/*  18 */ 23.12,
	/*  19 */ 24.5,
	/*  20 */ 25.96,
	/*  21 */ 27.5,
	/*  22 */ 29.14,
	/*  23 */ 30.87,
	/*  24 */ 32.7,
	/*  25 */ 34.65,
	/*  26 */ 36.71,
	/*  27 */ 38.89,
	/*  28 */ 41.2,
	/*  29 */ 43.65,
	/*  30 */ 46.25,
	/*  31 */ 49,
	/*  32 */ 51.91,
	/*  33 */ 55,
	/*  34 */ 58.27,
	/*  35 */ 61.74,
	/*  36 */ 65.41,
	/*  37 */ 69.3,
	/*  38 */ 73.42,
	/*  39 */ 77.78,
	/*  40 */ 82.41,
	/*  41 */ 87.31,
	/*  42 */ 92.5,
	/*  43 */ 98,
	/*  44 */ 103.83,
	/*  45 */ 110,
	/*  46 */ 116.54,
	/*  47 */ 123.47,
	/*  48 */ 130.81,
	/*  49 */ 138.59,
	/*  50 */ 146.83,
	/*  51 */ 155.56,
	/*  52 */ 164.81,
	/*  53 */ 174.61,
	/*  54 */ 185,
	/*  55 */ 196,
	/*  56 */ 207.65,
	/*  57 */ 220,
	/*  58 */ 233.08,
	/*  59 */ 246.94,
	/*  60 */ 261.63,
	/*  61 */ 277.18,
	/*  62 */ 293.66,
	/*  63 */ 311.13,
	/*  64 */ 329.63,
	/*  65 */ 349.23,
	/*  66 */ 369.99,
	/*  67 */ 392,
	/*  68 */ 415.3,
	/*  69 */ 440,
	/*  70 */ 466.16,
	/*  71 */ 493.88,
	/*  72 */ 523.25,
	/*  73 */ 554.37,
	/*  74 */ 587.33,
	/*  75 */ 622.25,
	/*  76 */ 659.26,
	/*  77 */ 698.46,
	/*  78 */ 739.99,
	/*  79 */ 783.99,
	/*  80 */ 830.61,
	/*  81 */ 880,
	/*  82 */ 932.33,
	/*  83 */ 987.77,
	/*  84 */ 1046.5,
	/*  85 */ 1108.73,
	/*  86 */ 1174.66,
	/*  87 */ 1244.51,
	/*  88 */ 1318.51,
	/*  89 */ 1396.91,
	/*  90 */ 1479.98,
	/*  91 */ 1567.98,
	/*  92 */ 1661.22,
	/*  93 */ 1760,
	/*  94 */ 1864.66,
	/*  95 */ 1975.53,
	/*  96 */ 2093,
	/*  97 */ 2217.46,
	/*  98 */ 2349.32,
	/*  99 */ 2489.02,
	/* 100 */ 2637.02,
	/* 101 */ 2793.83,
	/* 102 */ 2959.96,
	/* 103 */ 3135.96,
	/* 104 */ 3322.44,
	/* 105 */ 3520,
	/* 106 */ 3729.31,
	/* 107 */ 3951.07,
	/* 108 */ 4186.01,
	/* 109 */ 4434.92,
	/* 110 */ 4698.64,
	/* 111 */ 4978.03,
	/* 112 */ 5274.04,
	/* 113 */ 5587.65,
	/* 114 */ 5919.91,
	/* 115 */ 6271.93,
	/* 116 */ 6644.88,
	/* 117 */ 7040,
	/* 118 */ 7458.62,
	/* 119 */ 7902.13,
	/* 120 */ 8372.02,
	/* 121 */ 8869.84,
	/* 122 */ 9397.27,
	/* 123 */ 9956.06,
	/* 124 */ 10548.08,
	/* 125 */ 11175.3,
	/* 126 */ 11839.82,
	/* 127 */ 12543.85,
};

// A container for phase information
typedef struct {
	uint32_t time;	// How many ticks between updates
	int step;		// Volume change per update
	int target;		// Target volume
} timestep;

// Definition of an instrument
typedef struct {
	timestep phases[4];	// Four phases: attack, hold, decay, sustain
	int v_depth; 		// Vibrato depth
	int v_rate;			// Vibrato speed
} instrument;

// The defined instruments
instrument instruments[] = {
	// 0 - Piano
	{ 
		{ { 1, 4, 15 }, // Fast attack
		{ 50, -1, 2 }, // Slight decay on hold
		{ 4, -1, 2 }, //Slower decay
		{ 20, -1, 0 } }, //Slight ring at the end
		0, 0 // No vibrato
	},
	// 1 - organ
	{ 
		{ { 5, 1, 15 }, // Fast attack
		{ 10, 0, 15 }, // No change on hold
		{ 10, -1, 5 }, //Slower decay
		{ 100, -1, 0 } }, //Slight ring at the end
		5, 2 // Small vibrato
	},
	// 2 - Bass
	{ 
		{ { 1, 7, 15 }, // Fast attack
		{ 40, -1, 5 }, // Decay on hold
		{ 10, -1, 5 }, // Same on decay
		{ 100, -1, 0 } }, //Slight ring at the end
		0, 0 // No vibrato
	},
};


// Set the data bus to a value
void db(uint8_t b) {
	digitalWrite(DA0, b & 0x01);
	digitalWrite(DA1, b & 0x02);
	digitalWrite(DA2, b & 0x04);
	digitalWrite(DA3, b & 0x08);
	digitalWrite(DA4, b & 0x10);
	digitalWrite(DA5, b & 0x20);
	digitalWrite(DA6, b & 0x40);
	digitalWrite(DA7, b & 0x80);
}

// Set the bus to INACTIVE mode
void inactive() {
	LATACLR = 0b11;
}

// Set the bus to READ PSG mode
void readPSG() {
	LATACLR = 0b11;
	LATASET = 0b01;
}

// Set the bus to WRITE PSG mode
void writePSG() {
	LATACLR = 0b11;
	LATASET = 0b10;
}

// Set the bus to LATCH ADDRESS mode
void latchAddress() {
	LATACLR = 0b11;
	LATASET = 0b11;
}

// Write a value to a register
void writeReg(uint8_t reg, uint8_t val) {
	inactive();
	db(reg);
	latchAddress();
	delayMicroseconds(1);
	inactive();
	delayMicroseconds(1);
	db(val);
	writePSG();
	delayMicroseconds(1);
	inactive();
}

// Reset the AY-3-8912
void reset() {
	delay(100);
	digitalWrite(RST, LOW);
	delay(100);
	digitalWrite(RST, HIGH);
	delay(100);
}

// Configure the IO pins
void initPins() {
	pinMode(DA0, OUTPUT); digitalWrite(DA0, LOW);
	pinMode(DA1, OUTPUT); digitalWrite(DA1, LOW);
	pinMode(DA2, OUTPUT); digitalWrite(DA2, LOW);
	pinMode(DA3, OUTPUT); digitalWrite(DA3, LOW);
	pinMode(DA4, OUTPUT); digitalWrite(DA4, LOW);
	pinMode(DA5, OUTPUT); digitalWrite(DA5, LOW);
	pinMode(DA6, OUTPUT); digitalWrite(DA6, LOW);
	pinMode(DA7, OUTPUT); digitalWrite(DA7, LOW);
	pinMode(BC1, OUTPUT); digitalWrite(BC1, LOW);
	pinMode(BDIR, OUTPUT); digitalWrite(BDIR, LOW);
	pinMode(RST, OUTPUT); digitalWrite(RST, HIGH);
	pinMode(CLOCK, OUTPUT); digitalWrite(CLOCK, LOW);
}

// Configure the 1.6MHz REFCLK
void initClock() {
	mapPps(CLOCK, PPS_OUT_REFCLKO);

	REFOCONbits.ON = 0;
	REFOCONbits.ACTIVE = 0;
	REFOCONbits.OE = 1;
	REFOCONbits.ROSEL = 0b0010; // Posc 8MHz
	REFOCONbits.RODIV = 2;
	REFOTRIMbits.ROTRIM = 256;
	REFOCONbits.ACTIVE = 1;
	REFOCONbits.ON = 1;
}

// Stop a sound on a channel
void stop(int channel) {
	if (channels[channel].start == 0) return;
	channels[channel].phase = 2;
}

// Play a new sound on a channel
void play(int channel, uint32_t freq) {
	uint32_t div = 100000 / freq;

	channels[channel].freq = freq;
	channels[channel].phase = 0;
	channels[channel].start = millis();
	channels[channel].angle = 0;
	channels[channel].volume = 0;

	writeReg(channels[channel].f_fine, div & 0xFF);
	writeReg(channels[channel].f_coarse, div >> 8);
	writeReg(channels[channel].amp, 0);
}

// Handle incoming MIDI messages
void callback(uint8_t status, uint8_t d0, uint8_t d1) {
	uSerial.printf("%02x %d %d\r\n", status, d0, d1);

	uint8_t command = status & 0xF0;
	uint8_t channel = status & 0x0F;

	if (channel > 2) return;

	if (command == 0x80) {
		stop(channel);
	}

	if (command == 0x90) {
		if (d1 > 0) {
			play(channel, notes[d0]);
		} else {
			stop(channel);
		}
	}
}

void setup() {
	USB.addDevice(MIDI);
	USB.addDevice(uSerial);
	USB.begin();

	MIDI.onMidiMessage(callback);

	initPins();
	initClock();
	reset();

	// Mixers
	writeReg(7, 0b00111000);

	// Amplitudes
	writeReg(8, 0b00000000);
	writeReg(9, 0b00000000);
	writeReg(10, 0b00000000);
}

// This is where the envelope magic happens....
void loop() {
	static uint32_t ts = millis();

	// Once every millisecond.....
	if (millis() - ts > 0) {
		ts = millis();
		for (int i = 0; i < 3; i++) {
			// If there is a note playing
			if (channels[i].start > 0) {	
				instrument *ins = &instruments[channels[i].inst];

				channels[i].tick++;

				// Envelope handling

				// If enough ticks have passed
				if (channels[i].tick >= ins->phases[channels[i].phase].time) {
					channels[i].tick = 0;
					// Change the volume
					channels[i].volume += ins->phases[channels[i].phase].step;

					// Check if the target is reached either up or down
					if (ins->phases[channels[i].phase].step > 0) { // increase
						if (channels[i].volume >= ins->phases[channels[i].phase].target) {
							channels[i].volume = ins->phases[channels[i].phase].target;
							channels[i].phase++;
						}
					} else if (ins->phases[channels[i].phase].step < 0) { // decrease
						if (channels[i].volume <= ins->phases[channels[i].phase].target) {
							channels[i].volume = ins->phases[channels[i].phase].target;
							channels[i].phase++;
						}
					}

					// If it's the end of the envelope then stop the note
					if (channels[i].phase >= 4) {
						channels[i].start = 0;
						channels[i].volume = 0;
						channels[i].phase = 0;
					}

					// Update the volume
					writeReg(channels[i].amp, channels[i].volume);
				}

				// Vibrato handling

				// If vibrato is enabled
				if (ins->v_depth > 0) { // Some vibrato
					// Increase the angle by the rate
					channels[i].angle += ins->v_rate;
					// Wrap around 360 degrees
					if (channels[i].angle >= 360) channels[i].angle -= 360;
					// Convert to radians
					float a = channels[i].angle * 0.0174533;
					// get the SINE
					float v = sin(a);
					// Multiply by the depth
					float add = v * ins->v_depth;
					// Add the current frequency multiplied by the depth divided by 1000 to
					// the current frequency. This ensures that different pitches get the
					// same proportional change in the frequency.
					int32_t nf = channels[i].freq + (channels[i].freq * add / 1000);
					// Clamp the valies
					if (nf < 1) nf = 1;
					int32_t div = 100000 / nf;
					if (div < 1) div = 1;
					if (div > 0xfff) div = 0xfff;
					// update the frequency
					writeReg(channels[i].f_fine, div & 0xFF);
					writeReg(channels[i].f_coarse, div >> 8);
				}
			}
		}
	}
}
