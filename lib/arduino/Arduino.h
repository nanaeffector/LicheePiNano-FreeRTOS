/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2014 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Arduino_h
#define Arduino_h

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CORE_HAS_LIBB64

typedef bool boolean;
typedef uint8_t byte;
typedef uint16_t word;

// some libraries and sketches depend on this AVR stuff,
// assuming Arduino.h or WProgram.h automatically includes it...

#include "binary.h"
#include "itoa.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// Include Atmel headers
#include "arm32.h"

#define clockCyclesPerMicrosecond() ( SystemCoreClock / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (SystemCoreClock / 1000L) )
#define microsecondsToClockCycles(a) ( (a) * (SystemCoreClock / 1000000L) )

/* system functions */
int main( void );

/* sketch */
void setup( void ) ;
void loop( void ) ;

int __debug_buf(const char* head, char* buf, int len);

#include "WVariant.h"

#ifdef __cplusplus
} // extern "C"
#endif


#ifdef __cplusplus
  #include "WCharacter.h"
  #include "WString.h"
  #include "Tone.h"
  #include "WMath.h"
  #include "HardwareSerial.h"
  #include "pulse.h"
#endif


#include "wiring_digital.h"
#include "wiring_analog.h"
#include "wiring_shift.h"
#include "wiring_pwm.h"
#include "WInterrupts.h"


// The following headers are for C++ only compilation
#include "delay.h"

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif // abs
// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif // abs

#ifdef __cplusplus
  template<class T, class L> 
  auto min(const T& a, const L& b) -> decltype((b < a) ? b : a)
  {
    return (b < a) ? b : a;
  }

  template<class T, class L> 
  auto max(const T& a, const L& b) -> decltype((b < a) ? b : a)
  {
    return (a < b) ? b : a;
  }
#else
#ifndef min
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#endif
#ifndef max
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#endif
#endif

// Math.
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))


#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

enum BitOrder {
	LSBFIRST = 0,
	MSBFIRST = 1
};

// Bits and Bytes.
#define bit(b) (1UL << (b))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define lowByte(w) ((uint8_t) ((w) & 0xff))

// Interrupts.
#define interrupts()	arm32_interrupt_enable()
#define noInterrupts()	arm32_interrupt_disable()

#endif // Arduino_h
