
#include <stdlib.h>
#include <string.h>
#include <cv.h>
#include <cvu.h>

typedef unsigned char byte;
typedef unsigned short word;

//
// MUSIC ROUTINES
//

void music_start(const byte* music);
void music_update();

// Namespace(bias=0, freq=1789772.5, length=64, maxbits=15.0, upper=49)
// 437.0 459.515928524 49
const int note_table[64] = {
17356, 32764, 30926, 29190, 27551, 26005, 24546, 23168, 21868, 20640, 19482, 18388, 17356, 32765, 30926, 29190, 27552, 26005, 24546, 23168, 21868, 20640, 19482, 18389, 17357, 16382, 15463, 14595, 13776, 13003, 12273, 11584, 10934, 10320, 9741, 9194, 8678, 8191, 7731, 7298, 6888, 6501, 6136, 5792, 5467, 5160, 4871, 4597, 4339, 4096, 3866, 3649, 3444, 3251, 3068, 2896, 2733, 2580, 2435, 2299, 2170, 2048, 1933, 1824, };

struct {
  byte volume;
} voice[3];

byte music_index = 0;
byte cur_duration = 0;
byte num_voices = 3;

static const byte* music_ptr = NULL;

inline byte next_music_byte() {
  return *music_ptr++;
}

void music_update() {
  byte ch;
  byte freech = 0;
  for (ch=0; ch<num_voices; ch++) {
    if (voice[ch].volume) {
      cv_set_attenuation(CV_SOUNDCHANNEL_0+ch*2, 32-voice[ch].volume--);
    } else {
      freech = ch;
    }
  }
  if (music_ptr) {
    ch = freech;
    while (cur_duration == 0) {
      byte note = next_music_byte();
      if ((note & 0x80) == 0) {
        int period = note_table[note & 63];
        cv_set_attenuation(CV_SOUNDCHANNEL_0+ch*2, 32);
        cv_set_frequency(CV_SOUNDCHANNEL_0+ch*2, period);
        voice[ch].volume = 31;
        ch = ch ? ch-1 : num_voices-1;
      } else {
        if (note == 0xff)
          music_ptr = NULL;
        cur_duration = note & 63;
      }
    }
    cur_duration--;
  }
}

void music_start(const byte* music) __critical {
  music_ptr = music;
  cur_duration == 0;
}

///
/// MUSIC TEST
///

#ifdef __MAIN__

const byte music1[] = {
0x35,0x41,0x8a,0x37,0x43,0x8a,0x33,0x3f,0x8a,0x30,0x3c,0x94,0x3e,0x32,0x8a,0x3a,0x2e,0x94,0x35,0x29,0x8a,0x37,0x2b,0x8a,0x33,0x27,0x8a,0x30,0x24,0x94,0x32,0x26,0x8a,0x2e,0x22,0x94,0x29,0x1d,0x8a,0x2b,0x1f,0x8a,0x27,0x1b,0x8a,0x24,0x18,0x94,0x1a,0x26,0x8a,0x18,0x24,0x8a,0x17,0x23,0x8a,0x16,0x22,0xa8,0x3a,0x35,0x32,0x94,0x29,0x26,0x22,0x8a,0x2a,0x8a,0x2b,0x1b,0x8a,0x33,0x8a,0x22,0x1f,0x27,0x8a,0x2b,0x8a,0x33,0x22,0x16,0x94,0x2b,0x27,0x25,0x8a,0x33,0x8a,0x20,0x14,0x94,0x27,0x24,0x94,0x1f,0x13,0x8a,0x3f,0x37,0x33,0x8a,0x38,0x35,0x41,0x8a,0x42,0x39,0x36,0x8a,0x43,0x3a,0x37,0x8a,0x3f,0x37,0x33,0x8a,0x35,0x41,0x38,0x8a,0x3a,0x37,0x43,0x8a,0x16,0x8a,0x3e,0x35,0x32,0x8a,0x41,0x38,0x35,0x94,0x3f,0x33,0x37,0x94,0x1f,0x22,0x27,0x94,0x27,0x22,0x1f,0x94,0x29,0x26,0x22,0x8a,0x2a,0x8a,0x2b,0x1b,0x8a,0x33,0x8a,0x1f,0x22,0x27,0x8a,0x2b,0x8a,0x33,0x22,0x16,0x94,0x2b,0x27,0x25,0x8a,0x33,0x8a,0x20,0x14,0x94,0x24,0x27,0x94,0x1f,0x13,0x94,0x3c,0x33,0x30,0x8a,0x3a,0x33,0x2e,0x8a,0x39,0x33,0x2d,0x8a,0x3c,0x30,0x8a,0x3f,0x33,0x1d,0x8a,0x43,0x37,0x8a,0x1d,0x8a,0x41,0x35,0x8a,0x3f,0x33,0x27,0x8a,0x3c,0x30,0x8a,0x41,0x35,0x38,0x94,0x22,0x16,0x94,0x24,0x18,0x94,0x29,0x26,0x1a,0x8a,0x2a,0x8a,0x2b,0x1b,0x8a,0x33,0x8a,0x27,0x22,0x1f,0x8a,0x2b,0x8a,0x33,0x16,0x22,0x94,0x2b,0x27,0x25,0x8a,0x33,0x8a,0x20,0x14,0x94,0x27,0x24,0x94,0x13,0x1f,0x8a,0x3f,0x33,0x37,0x8a,0x41,0x38,0x35,0x8a,0x42,0x39,0x36,0x8a,0x37,0x3a,0x43,0x8a,0x3f,0x37,0x33,0x8a,0x41,0x38,0x35,0x8a,0x43,0x3a,0x37,0x8a,0x16,0x8a,0x3e,0x35,0x32,0x8a,0x41,0x38,0x35,0x94,0x3f,0x37,0x33,0x94,0x27,0x22,0x1f,0x94,0x2b,0x27,0x22,0x94,0x3f,0x33,0x8a,0x41,0x35,0x8a,0x43,0x37,0x27,0x8a,0x3f,0x33,0x8a,0x41,0x35,0x2b,0x8a,0x43,0x37,0x8a,0x25,0x19,0x8a,0x3f,0x33,0x8a,0x41,0x35,0x2b,0x8a,0x3f,0x33,0x8a,0x43,0x37,0x24,0x8a,0x3f,0x33,0x8a,0x41,0x35,0x2c,0x8a,0x43,0x37,0x8a,0x23,0x17,0x8a,0x3f,0x33,0x8a,0x41,0x35,0x2c,0x8a,0x3f,0x33,0x8a,0x3a,0x37,0x43,0x8a,0x3f,0x37,0x33,0x8a,0x41,0x38,0x35,0x8a,0x43,0x3a,0x37,0x8a,0x16,0x8a,0x3e,0x32,0x35,0x8a,0x41,0x35,0x38,0x94,0x3f,0x33,0x37,0x94,0x22,0x16,0x94,0x24,0x18,0x94,0x29,0x26,0x1a,0x8a,0x2a,0x8a,0x2b,0x1b,0x8a,0x33,0x8a,0x27,0x22,0x1f,0x8a,0x2b,0x8a,0x33,0x22,0x16,0x94,0x2b,0x25,0x22,0x8a,0x33,0x8a,0x20,0x14,0x94,0x27,0x24,0x94,0x1f,0x13,0x8a,0x3f,0x37,0x33,0x8a,0x35,0x41,0x38,0x8a,0x42,0x39,0x36,0x8a,0x43,0x3a,0x37,0x8a,0x3f,0x37,0x33,0x8a,0x38,0x35,0x41,0x8a,0x43,0x3a,0x37,0x8a,0x16,0x8a,0x3e,0x35,0x32,0x8a,0x35,0x41,0x38,0x94,0x37,0x33,0x3f,0x94,0x27,0x22,0x1f,0x94,0x27,0x22,0x1f,0x94,0x29,0x26,0x22,0x8a,0x2a,0x8a,0x2b,0x1b,0x8a,0x33,0x8a,0x22,0x1f,0x27,0x8a,0x2b,0x8a,0x33,0x22,0x16,0x94,0x2b,0x27,0x25,0x8a,0x33,0x8a,0x20,0x14,0x94,0x27,0x24,0x94,0x1f,0x13,0x94,0x3c,0x33,0x30,0x8a,0x3a,0x33,0x2e,0x8a,0x2d,0x33,0x39,0x8a,0x3c,0x30,0x8a,0x3f,0x33,0x27,0x8a,0x43,0x37,0x8a,0x1d,0x8a,0x41,0x35,0x8a,0x33,0x3f,0x27,0x8a,0x3c,0x30,0x8a,0x41,0x38,0x35,0x94,0x22,0x16,0x94,0x24,0x18,0x94,0x29,0x26,0x1a,0x8a,0x2a,0x8a,0x2b,0x1b,0x8a,0x33,0x8a,0x22,0x27,0x1f,0x8a,0x2b,0x8a,0x33,0x22,0x16,0x94,0x2b,0x27,0x25,0x8a,0x33,0x8a,0x20,0x14,0x94,0x24,0x27,0x94,0x1f,0x13,0x8a,0x3f,0x37,0x33,0x8a,0x41,0x38,0x35,0x8a,0x36,0x39,0x42,0x8a,0x43,0x3a,0x37,0x8a,0x3f,0x37,0x33,0x8a,0x41,0x38,0x35,0x8a,0x37,0x43,0x3a,0x8a,0x16,0x8a,0x3e,0x35,0x32,0x8a,0x41,0x38,0x35,0x94,0x33,0x37,0x3f,0x94,0x27,0x22,0x1f,0x94,0x22,0x27,0x2b,0x94,0x3f,0x33,0x8a,0x41,0x35,0x8a,0x43,0x37,0x27,0x8a,0x3f,0x33,0x8a,0x41,0x35,0x2b,0x8a,0x43,0x37,0x8a,0x25,0x19,0x8a,0x3f,0x33,0x8a,0x41,0x35,0x2b,0x8a,0x3f,0x33,0x8a,0x43,0x37,0x24,0x8a,0x33,0x3f,0x8a,0x41,0x35,0x2c,0x8a,0x43,0x37,0x8a,0x23,0x17,0x8a,0x3f,0x33,0x8a,0x41,0x35,0x2c,0x8a,0x3f,0x33,0x8a,0x43,0x3a,0x37,0x8a,0x3f,0x37,0x33,0x8a,0x41,0x38,0x35,0x8a,0x37,0x3a,0x43,0x8a,0x16,0x8a,0x3e,0x35,0x32,0x8a,0x41,0x38,0x35,0x94,0x33,0x37,0x3f,0x94,0x22,0x16,0x94,0x1b,0x0f,0x8a,0x37,0x2b,0x33,0x8a,0x38,0x2c,0x35,0x8a,0x39,0x36,0x2d,0x8a,0x3a,0x37,0x2e,0x94,0x3c,0x37,0x30,0x8a,0x3a,0x2e,0x37,0x8a,0x16,0x8a,0x37,0x33,0x2b,0x8a,0x2c,0x38,0x35,0x8a,0x39,0x36,0x2d,0x8a,0x3a,0x37,0x2e,0x94,0x3c,0x37,0x30,0x8a,0x2e,0x37,0x3a,0x8a,0x16,0x8a,0x37,0x8a,0x33,0x27,0x2b,0x8a,0x2e,0x8a,0x30,0x14,0x8a,0x32,0x8a,0x33,0x2c,0x27,0x8a,0x35,0x8a,0x37,0x20,0x8a,0x35,0x8a,0x33,0x2c,0x27,0x8a,0x35,0x8a,0x2e,0x1f,0x8a,0x37,0x8a,0x38,0x2b,0x27,0x8a,0x3a,0x8a,0x3c,0x16,0x8a,0x3a,0x8a,0x37,0x22,0x2b,0x8a,0x38,0x8a,0x3a,0x37,0x2e,0x94,0x3c,0x30,0x37,0x8a,0x3a,0x37,0x2e,0x8a,0x16,0x8a,0x37,0x33,0x2b,0x8a,0x38,0x2c,0x35,0x8a,0x39,0x36,0x2d,0x8a,0x3a,0x37,0x2e,0x94,0x3c,0x37,0x30,0x8a,0x2e,0x37,0x3a,0x8a,0x1f,0x8a,0x3a,0x8a,0x3c,0x1e,0x8a,0x3d,0x8a,0x3e,0x3a,0x35,0x8a,0x3e,0x35,0x3a,0x8a,0x22,0x29,0x26,0x8a,0x3e,0x33,0x39,0x8a,0x1d,0x8a,0x3c,0x8a,0x39,0x33,0x29,0x8a,0x35,0x8a,0x3a,0x32,0x29,0x94,0x20,0x14,0x94,0x1f,0x13,0x8a,0x37,0x2b,0x33,0x8a,0x38,0x35,0x2c,0x8a,0x39,0x36,0x2d,0x8a,0x2e,0x3a,0x37,0x94,0x3c,0x37,0x30,0x8a,0x3a,0x37,0x2e,0x8a,0x16,0x8a,0x33,0x2b,0x37,0x8a,0x38,0x35,0x2c,0x8a,0x39,0x36,0x2d,0x8a,0x3a,0x37,0x2e,0x94,0x30,0x37,0x3c,0x8a,0x3a,0x2e,0x37,0x8a,0x16,0x8a,0x37,0x8a,0x33,0x2b,0x27,0x8a,0x2e,0x8a,0x30,0x14,0x8a,0x32,0x8a,0x33,0x2c,0x27,0x8a,0x35,0x8a,0x37,0x20,0x8a,0x35,0x8a,0x33,0x23,0x2c,0x8a,0x35,0x8a,0x33,0x1f,0x94,0x2b,0x22,0x27,0x94,0x1b,0x8a,0x2e,0x8a,0x2d,0x2b,0x27,0x8a,0x2e,0x8a,0x33,0x2c,0x27,0x94,0x30,0x2c,0x27,0x8a,0x33,0x8a,0x21,0x24,0x2a,0x8a,0x30,0x8a,0x33,0x2a,0x27,0x8a,0x30,0x8a,0x2e,0x2b,0x27,0x8a,0x33,0x8a,0x37,0x2b,0x22,0x8a,0x3a,0x8a,0x2b,0x27,0x22,0x8a,0x37,0x8a,0x33,0x2b,0x27,0x8a,0x2e,0x8a,0x30,0x2d,0x1d,0x94,0x33,0x2d,0x24,0x94,0x37,0x2c,0x26,0x8a,0x35,0x2c,0x8a,0x26,0x22,0x8a,0x33,0x2b,0x8a,0x27,0x1b,0x94,0x22,0x16,0x94,0x1f,0x13,0x8a,0x43,0x3f,0x37,0x8a,0x44,0x41,0x38,0x8a,0x45,0x42,0x39,0x8a,0x3a,0x37,0x2e,0x94,0x3c,0x30,0x37,0x8a,0x3a,0x37,0x2e,0x8a,0x16,0x8a,0x37,0x33,0x2b,0x8a,0x38,0x2c,0x35,0x8a,0x39,0x36,0x2d,0x8a,0x3a,0x37,0x2e,0x94,0x3c,0x37,0x30,0x8a,0x2e,0x37,0x3a,0x8a,0x16,0x8a,0x37,0x8a,0x33,0x2b,0x27,0x8a,0x2e,0x8a,0x30,0x14,0x8a,0x32,0x8a,0x33,0x2c,0x27,0x8a,0x35,0x8a,0x37,0x20,0x8a,0x35,0x8a,0x33,0x23,0x27,0x8a,0x35,0x8a,0x2e,0x1f,0x8a,0x37,0x8a,0x38,0x27,0x2b,0x8a,0x3a,0x8a,0x3c,0x16,0x8a,0x3a,0x8a,0x37,0x2b,0x27,0x8a,0x38,0x8a,0x3a,0x37,0x2e,0x94,0x3c,0x30,0x37,0x8a,0x3a,0x37,0x2e,0x8a,0x16,0x8a,0x37,0x33,0x2b,0x8a,0x38,0x2c,0x35,0x8a,0x39,0x36,0x2d,0x8a,0x3a,0x37,0x2e,0x94,0x3c,0x37,0x30,0x8a,0x2e,0x37,0x3a,0x8a,0x1f,0x8a,0x3a,0x8a,0x3c,0x1e,0x8a,0x3d,0x8a,0x3e,0x3a,0x35,0x8a,0x3e,0x35,0x3a,0x8a,0x29,0x26,0x22,0x8a,0x3e,0x33,0x39,0x8a,0x1d,0x8a,0x3c,0x8a,0x39,0x33,0x27,0x8a,0x35,0x8a,0x3a,0x32,0x29,0x94,0x20,0x14,0x94,0x1f,0x13,0x8a,0x37,0x2b,0x33,0x8a,0x38,0x35,0x2c,0x8a,0x39,0x36,0x2d,0x8a,0x2e,0x3a,0x37,0x94,0x3c,0x37,0x30,0x8a,0x3a,0x37,0x2e,0x8a,0x16,0x8a,0x33,0x2b,0x37,0x8a,0x38,0x35,0x2c,0x8a,0x39,0x36,0x2d,0x8a,0x3a,0x37,0x2e,0x94,0x30,0x3c,0x37,0x8a,0x37,0x2e,0x3a,0x8a,0x16,0x8a,0x37,0x8a,0x33,0x2b,0x27,0x8a,0x2e,0x8a,0x30,0x14,0x8a,0x32,0x8a,0x33,0x27,0x24,0x8a,0x35,0x8a,0x37,0x20,0x8a,0x35,0x8a,0x33,0x2c,0x27,0x8a,0x35,0x8a,0x33,0x1f,0x94,0x2b,0x27,0x22,0x94,0x1b,0x8a,0x2e,0x8a,0x2d,0x2b,0x27,0x8a,0x2e,0x8a,0x33,0x2c,0x27,0x94,0x30,0x2c,0x20,0x8a,0x33,0x8a,0x2a,0x27,0x24,0x8a,0x30,0x8a,0x33,0x2a,0x27,0x8a,0x30,0x8a,0x2e,0x2b,0x22,0x8a,0x33,0x8a,0x37,0x2b,0x27,0x8a,0x3a,0x8a,0x2b,0x27,0x22,0x8a,0x37,0x8a,0x33,0x2b,0x22,0x8a,0x2e,0x8a,0x30,0x2d,0x27,0x94,0x33,0x2d,0x24,0x94,0x37,0x2c,0x26,0x8a,0x35,0x2c,0x8a,0x26,0x22,0x8a,0x33,0x2b,0x8a,0x27,0x1b,0x94,0x22,0x16,0x94,0x1b,0x0f,0x94,0x29,0x8a,0x2a,0x8a,0x2b,0x1b,0x8a,0x33,0x8a,0x27,0x1f,0x22,0x8a,0x2b,0x8a,0x33,0x22,0x16,0x94,0x2b,0x27,0x25,0x8a,0x33,0x8a,0x20,0x14,0x94,0x27,0x24,0x94,0x1f,0x13,0x8a,0x3f,0x37,0x33,0x8a,0x41,0x38,0x35,0x8a,0x42,0x39,0x36,0x8a,0x3a,0x37,0x43,0x8a,0x3f,0x37,0x33,0x8a,0x41,0x38,0x35,0x8a,0x43,0x3a,0x37,0x8a,0x16,0x8a,0x3e,0x35,0x32,0x8a,0x41,0x38,0x35,0x94,0x3f,0x37,0x33,0x94,0x27,0x22,0x1f,0x94,0x22,0x1f,0x27,0x94,0x29,0x26,0x22,0x8a,0x2a,0x8a,0x2b,0x1b,0x8a,0x33,0x8a,0x27,0x22,0x1f,0x8a,0x2b,0x8a,0x33,0x22,0x16,0x94,0x2b,0x27,0x22,0x8a,0x33,0x8a,0x20,0x14,0x94,0x27,0x24,0x94,0x1f,0x13,0x94,0x33,0x3c,0x30,0x8a,0x3a,0x33,0x2e,0x8a,0x39,0x33,0x2d,0x8a,0x3c,0x30,0x8a,0x3f,0x33,0x27,0x8a,0x43,0x37,0x8a,0x1d,0x8a,0x41,0x35,0x8a,0x3f,0x33,0x27,0x8a,0x3c,0x30,0x8a,0x35,0x41,0x38,0x94,0x22,0x16,0x94,0x24,0x18,0x94,0x29,0x26,0x1a,0x8a,0x2a,0x8a,0x2b,0x1b,0x8a,0x33,0x8a,0x1f,0x22,0x27,0x8a,0x2b,0x8a,0x33,0x22,0x16,0x94,0x2b,0x27,0x25,0x8a,0x33,0x8a,0x20,0x14,0x94,0x24,0x27,0x94,0x1f,0x13,0x8a,0x3f,0x37,0x33,0x8a,0x41,0x38,0x35,0x8a,0x42,0x39,0x36,0x8a,0x37,0x3a,0x43,0x8a,0x3f,0x33,0x37,0x8a,0x41,0x35,0x38,0x8a,0x43,0x3a,0x37,0x8a,0x16,0x8a,0x3e,0x35,0x32,0x8a,0x38,0x35,0x41,0x94,0x3f,0x37,0x33,0x94,0x27,0x22,0x1f,0x94,0x22,0x2b,0x27,0x94,0x3f,0x33,0x8a,0x41,0x35,0x8a,0x43,0x37,0x27,0x8a,0x3f,0x33,0x8a,0x41,0x35,0x2b,0x8a,0x43,0x37,0x8a,0x25,0x19,0x8a,0x33,0x3f,0x8a,0x41,0x35,0x27,0x8a,0x3f,0x33,0x8a,0x43,0x37,0x24,0x8a,0x3f,0x33,0x8a,0x41,0x35,0x2c,0x8a,0x43,0x37,0x8a,0x23,0x17,0x8a,0x3f,0x33,0x8a,0x41,0x35,0x23,0x8a,0x3f,0x33,0x8a,0x43,0x3a,0x37,0x8a,0x33,0x37,0x3f,0x8a,0x41,0x38,0x35,0x8a,0x43,0x3a,0x37,0x8a,0x16,0x8a,0x32,0x35,0x3e,0x8a,0x41,0x38,0x35,0x94,0x3f,0x37,0x33,0x94,0x22,0x16,0x94,0x3f,0x37,0x33,0xa8,0x38,0x3c,0x14,0x8a,0x3b,0x8a,0x3c,0x38,0x24,0x94,0x1b,0x94,0x3f,0x3c,0x38,0x94,0x38,0x3d,0x41,0x94,0x31,0x2c,0x29,0x8a,0x30,0x8a,0x31,0x20,0x8a,0x33,0x8a,0x35,0x2c,0x25,0x94,0x38,0x35,0x11,0x8a,0x37,0x8a,0x38,0x35,0x2c,0x94,0x18,0x94,0x3c,0x38,0x35,0x94,0x35,0x3a,0x3d,0x94,0x2e,0x25,0x29,0x8a,0x2d,0x8a,0x2e,0x1d,0x8a,0x30,0x8a,0x31,0x29,0x25,0x8a,0x3a,0x8a,0x35,0x25,0x19,0x94,0x3a,0x25,0x29,0x8a,0x35,0x8a,0x22,0x16,0x8a,0x3a,0x8a,0x35,0x23,0x17,0x94,0x33,0x24,0x18,0x94,0x24,0x27,0x2c,0x94,0x38,0x1d,0x94,0x2c,0x29,0x24,0x94,0x37,0x1f,0x8a,0x3b,0x8a,0x3e,0x26,0x29,0x8a,0x43,0x8a,0x23,0x8a,0x41,0x8a,0x3e,0x2b,0x29,0x8a,0x3f,0x8a,0x3c,0x2b,0x27,0xa8,0x3d,0x22,0x27,0x94,0x1b,0x94,0x3c,0x38,0x14,0x8a,0x3b,0x8a,0x38,0x3c,0x2c,0x94,0x1b,0x94,0x3f,0x3c,0x38,0x94,0x41,0x3d,0x38,0x94,0x31,0x2c,0x25,0x8a,0x30,0x8a,0x31,0x20,0x8a,0x33,0x8a,0x35,0x2c,0x29,0x94,0x38,0x35,0x11,0x8a,0x37,0x8a,0x38,0x35,0x2c,0x94,0x18,0x94,0x35,0x3c,0x38,0x94,0x3d,0x3a,0x35,0x94,0x2e,0x29,0x25,0x8a,0x2d,0x8a,0x2e,0x1d,0x8a,0x30,0x8a,0x31,0x29,0x25,0x8a,0x3a,0x8a,0x35,0x25,0x19,0x94,0x3a,0x29,0x25,0x8a,0x35,0x8a,0x16,0x22,0x8a,0x3a,0x8a,0x35,0x23,0x17,0x94,0x33,0x24,0x18,0x8a,0x20,0x14,0x8a,0x1f,0x13,0x8a,0x11,0x1d,0x8a,0x38,0x32,0x2f,0x9e,0x38,0x8a,0x3c,0x33,0x30,0x8a,0x3f,0x33,0x8a,0x2c,0x27,0x24,0x8a,0x3a,0x31,0x8a,0x27,0x1b,0x8a,0x33,0x8a,0x35,0x31,0x1b,0x8a,0x37,0x8a,0x38,0x30,0x20,0x94,0x32,0x8a,0x33,0x8a,0x35,0x8a,0x37,0x8a,0x38,0x8a,0x3a,0x8a,0x3c,0x38,0x14,0x8a,0x3b,0x8a,0x38,0x3c,0x2c,0x94,0x1b,0x94,0x3f,0x3c,0x38,0x94,0x41,0x3d,0x38,0x94,0x31,0x2c,0x29,0x8a,0x30,0x8a,0x31,0x20,0x8a,0x33,0x8a,0x35,0x29,0x2c,0x94,0x38,0x35,0x11,0x8a,0x37,0x8a,0x38,0x35,0x2c,0x94,0x18,0x94,0x3c,0x35,0x38,0x94,0x35,0x3d,0x3a,0x94,0x2e,0x29,0x25,0x8a,0x2d,0x8a,0x2e,0x1d,0x8a,0x30,0x8a,0x31,0x29,0x25,0x8a,0x3a,0x8a,0x35,0x25,0x19,0x94,0x3a,0x29,0x25,0x8a,0x35,0x8a,0x22,0x16,0x8a,0x3a,0x8a,0x35,0x23,0x17,0x94,0x33,0x24,0x18,0x94,0x2c,0x27,0x24,0x94,0x38,0x1d,0x94,0x2c,0x29,0x24,0x94,0x37,0x1f,0x8a,0x3b,0x8a,0x3e,0x26,0x2b,0x8a,0x43,0x8a,0x23,0x8a,0x41,0x8a,0x3e,0x2b,0x29,0x8a,0x3f,0x8a,0x3c,0x2b,0x27,0xa8,0x3d,0x2b,0x27,0x94,0x1b,0x94,0x3c,0x38,0x14,0x8a,0x3b,0x8a,0x38,0x3c,0x24,0x94,0x1b,0x94,0x3f,0x3c,0x38,0x94,0x41,0x3d,0x38,0x94,0x31,0x2c,0x29,0x8a,0x30,0x8a,0x31,0x20,0x8a,0x33,0x8a,0x35,0x2c,0x29,0x94,0x35,0x38,0x11,0x8a,0x37,0x8a,0x38,0x35,0x2c,0x94,0x18,0x94,0x3c,0x38,0x35,0x94,0x3d,0x3a,0x35,0x94,0x2e,0x25,0x29,0x8a,0x2d,0x8a,0x2e,0x1d,0x8a,0x30,0x8a,0x31,0x29,0x25,0x8a,0x3a,0x8a,0x35,0x25,0x19,0x94,0x3a,0x29,0x25,0x8a,0x35,0x8a,0x22,0x16,0x8a,0x3a,0x8a,0x35,0x23,0x17,0x94,0x33,0x24,0x18,0x8a,0x20,0x14,0x8a,0x1f,0x13,0x8a,0x1d,0x11,0x8a,0x38,0x32,0x2f,0x9e,0x38,0x8a,0x30,0x33,0x3c,0x8a,0x3f,0x33,0x8a,0x2c,0x27,0x24,0x8a,0x3a,0x31,0x8a,0x27,0x1b,0x8a,0x33,0x8a,0x35,0x31,0x1b,0x8a,0x37,0x8a,0x30,0x38,0x20,0xa8,0x3f,0x3c,0x38,0xa8,0x33,0x20,0x27,0x94,0x30,0x2c,0x27,0x8a,0x33,0x8a,0x2a,0x27,0x24,0x8a,0x30,0x8a,0x33,0x2a,0x21,0x8a,0x30,0x8a,0x2e,0x2b,0x27,0x8a,0x33,0x8a,0x37,0x22,0x27,0x8a,0x3a,0x8a,0x2b,0x27,0x22,0x8a,0x37,0x8a,0x33,0x2b,0x27,0x8a,0x2e,0x8a,0x30,0x2d,0x27,0x94,0x33,0x2d,0x24,0x94,0x37,0x2c,0x26,0x8a,0x35,0x2c,0x8a,0x26,0x22,0x8a,0x33,0x2b,0x8a,0x27,0x1b,0xa8,0x33,0x37,0x3a,0xa8,0x2c,0x29,0x14,0x94,0x2b,0x28,0x20,0x8a,0x2c,0x29,0x8a,0x18,0x8a,0x2b,0x28,0x8a,0x29,0x2c,0x24,0x94,0x14,0x8a,0x30,0x8a,0x2c,0x35,0x24,0x8a,0x30,0x8a,0x33,0x18,0x8a,0x35,0x8a,0x33,0x24,0x20,0x8a,0x30,0x8a,0x2e,0x2b,0x1b,0x94,0x2d,0x2a,0x27,0x8a,0x2e,0x2b,0x8a,0x16,0x8a,0x2d,0x2a,0x8a,0x2e,0x2b,0x1f,0x94,0x1b,0x8a,0x33,0x8a,0x37,0x2e,0x27,0x8a,0x33,0x8a,0x35,0x16,0x8a,0x37,0x8a,0x35,0x1f,0x22,0x8a,0x33,0x8a,0x35,0x32,0x16,0x94,0x34,0x31,0x26,0x8a,0x35,0x32,0x8a,0x1a,0x8a,0x34,0x31,0x8a,0x32,0x35,0x26,0x94,0x16,0x8a,0x38,0x8a,0x3c,0x32,0x22,0x8a,0x38,0x8a,0x3a,0x1d,0x8a,0x3c,0x8a,0x3a,0x26,0x22,0x8a,0x38,0x8a,0x3f,0x33,0x27,0x8a,0x3f,0x33,0x8a,0x3f,0x33,0x27,0xa8,0x3c,0x33,0x27,0x94,0x3a,0x33,0x1f,0x94,0x2e,0x2b,0x8a,0x2e,0x2b,0x8a,0x2e,0x2b,0x94,0x2b,0x2e,0x94,0x2c,0x29,0x14,0x94,0x2b,0x28,0x24,0x8a,0x2c,0x29,0x8a,0x18,0x8a,0x2b,0x28,0x8a,0x2c,0x29,0x24,0x94,0x14,0x8a,0x30,0x8a,0x35,0x2c,0x20,0x8a,0x30,0x8a,0x33,0x18,0x8a,0x35,0x8a,0x33,0x24,0x20,0x8a,0x30,0x8a,0x2e,0x2b,0x1b,0x94,0x2a,0x2d,0x27,0x8a,0x2e,0x2b,0x8a,0x16,0x8a,0x2d,0x2a,0x8a,0x2e,0x2b,0x1f,0x94,0x1b,0x8a,0x33,0x8a,0x37,0x2e,0x27,0x8a,0x33,0x8a,0x35,0x16,0x8a,0x37,0x8a,0x35,0x27,0x22,0x8a,0x33,0x8a,0x30,0x20,0x14,0x8a,0x2f,0x8a,0x30,0x1d,0x11,0x8a,0x3a,0x30,0x8a,0x1f,0x13,0x8a,0x38,0x30,0x8a,0x20,0x14,0x8a,0x33,0x30,0x8a,0x37,0x2e,0x22,0x8a,0x36,0x8a,0x37,0x22,0x2b,0x8a,0x3c,0x8a,0x2a,0x27,0x21,0x8a,0x3f,0x8a,0x3a,0x2b,0x27,0x8a,0x37,0x8a,0x33,0x2d,0x18,0x94,0x33,0x2d,0x1d,0x94,0x37,0x2c,0x32,0x8a,0x35,0x32,0x2c,0x8a,0x26,0x1a,0x8a,0x33,0x2e,0x2b,0x8a,0x27,0x1b,0x94,0x2b,0x2e,0x8a,0x2e,0x2b,0x8a,0x2e,0x2b,0x94,0x2e,0x2b,0x94,0x2c,0x29,0x14,0x94,0x2b,0x28,0x20,0x8a,0x29,0x2c,0x8a,0x18,0x8a,0x2b,0x28,0x8a,0x2c,0x29,0x24,0x94,0x14,0x8a,0x30,0x8a,0x35,0x2c,0x24,0x8a,0x30,0x8a,0x33,0x18,0x8a,0x35,0x8a,0x33,0x24,0x20,0x8a,0x30,0x8a,0x2e,0x2b,0x1b,0x94,0x2d,0x2a,0x27,0x8a,0x2e,0x2b,0x8a,0x16,0x8a,0x2d,0x2a,0x8a,0x2e,0x2b,0x1f,0x94,0x1b,0x8a,0x33,0x8a,0x37,0x2e,0x27,0x8a,0x33,0x8a,0x35,0x16,0x8a,0x37,0x8a,0x35,0x27,0x22,0x8a,0x33,0x8a,0x35,0x32,0x16,0x94,0x31,0x34,0x26,0x8a,0x35,0x32,0x8a,0x1a,0x8a,0x34,0x31,0x8a,0x35,0x32,0x26,0x94,0x16,0x8a,0x38,0x8a,0x3c,0x32,0x20,0x8a,0x38,0x8a,0x3a,0x1d,0x8a,0x3c,0x8a,0x3a,0x26,0x22,0x8a,0x38,0x8a,0x33,0x3f,0x27,0x8a,0x3f,0x33,0x8a,0x3f,0x33,0x1e,0xa8,0x3c,0x33,0x27,0x94,0x3a,0x33,0x27,0x94,0x2b,0x2e,0x8a,0x2e,0x2b,0x8a,0x2e,0x2b,0x94,0x2e,0x2b,0x94,0x2c,0x29,0x14,0x94,0x2b,0x28,0x20,0x8a,0x2c,0x29,0x8a,0x18,0x8a,0x2b,0x28,0x8a,0x2c,0x29,0x24,0x94,0x14,0x8a,0x30,0x8a,0x35,0x2c,0x24,0x8a,0x30,0x8a,0x33,0x18,0x8a,0x35,0x8a,0x33,0x24,0x20,0x8a,0x30,0x8a,0x2e,0x2b,0x1b,0x94,0x2d,0x2a,0x27,0x8a,0x2e,0x2b,0x8a,0x16,0x8a,0x2a,0x2d,0x8a,0x2e,0x2b,0x27,0x94,0x1b,0x8a,0x33,0x8a,0x37,0x2e,0x27,0x8a,0x33,0x8a,0x35,0x16,0x8a,0x37,0x8a,0x35,0x22,0x1f,0x8a,0x33,0x8a,0x30,0x20,0x14,0x8a,0x2f,0x8a,0x30,0x1d,0x11,0x8a,0x3a,0x30,0x8a,0x1f,0x13,0x8a,0x38,0x30,0x8a,0x20,0x14,0x8a,0x33,0x30,0x8a,0x37,0x2e,0x22,0x8a,0x36,0x8a,0x37,0x2b,0x27,0x8a,0x3c,0x8a,0x21,0x27,0x2a,0x8a,0x3f,0x8a,0x3a,0x2b,0x27,0x8a,0x37,0x8a,0x33,0x2d,0x24,0x94,0x33,0x2d,0x1d,0x94,0x37,0x32,0x2c,0x8a,0x35,0x2c,0x32,0x8a,0x26,0x1a,0x8a,0x33,0x2e,0x2b,0x8a,0x27,0x1b,0x94,0x22,0x16,0x94,0x3f,0x3a,0x37,0xff
};

void vint_handler(void) {
  cv_set_colors(CV_COLOR_BLACK, CV_COLOR_BLUE);
  music_update();
  cv_set_colors(CV_COLOR_BLACK, CV_COLOR_BLACK);
}

void main(void) {
  cv_set_vint_handler(vint_handler);
  cv_set_screen_active(true);
  while (1) {
    if (!music_ptr) music_start(music1);
  }
}

#endif
