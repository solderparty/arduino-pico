/*
    Arduino header for the Raspberry Pi Pico RP2040

    Copyright (c) 2021 Earle F. Philhower, III <earlephilhower@yahoo.com>

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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// Wacky deprecated AVR compatibility functions
#include "stdlib_noniso.h"

#include "api/ArduinoAPI.h"
#include <pins_arduino.h>


// Required for the port*Register macros
#include "hardware/gpio.h"

#include "debug_internal.h"

// Try and make the best of the old Arduino abs() macro.  When in C++, use
// the sane std::abs() call, but for C code use their macro since stdlib abs()
// is int but their macro "works" for everything (with potential side effects)
#ifdef abs
#undef abs
#endif // abs
#ifdef __cplusplus
using std::abs;
#else
#define abs(x) ((x)>0?(x):-(x))
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// For compatibility to many platforms and libraries
#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

// Disable/re-enable all interrupts.  Safely handles nested disables
void interrupts();
void noInterrupts();

// GPIO change/value interrupts
void attachInterrupt(pin_size_t pin, voidFuncPtr callback, PinStatus mode);
void detachInterrupt(pin_size_t pin);

// AVR compatibility macros...naughty and accesses the HW directly
#define digitalPinToPort(pin)       (0)
#define digitalPinToBitMask(pin)    (1UL << (pin))
#define digitalPinToTimer(pin)      (0)
#define digitalPinToInterrupt(pin)  (pin)
#define NOT_AN_INTERRUPT            (-1)
#define portOutputRegister(port)    ((volatile uint32_t*) sio_hw->gpio_out)
#define portInputRegister(port)     ((volatile uint32_t*) sio_hw->gpio_in)
#define portModeRegister(port)      ((volatile uint32_t*) sio_hw->gpio_oe)

// IO config
void pinMode(pin_size_t pinNumber, PinMode pinMode);

// SIO (GPIO)
void digitalWrite(pin_size_t pinNumber, PinStatus status);
PinStatus digitalRead(pin_size_t pinNumber);

// ADC
int analogRead(pin_size_t pinNumber);
float analogReadTemp();  // Returns core temp in Centigrade

// PWM
void analogWrite(pin_size_t pinNumber, int value);
void analogWriteFreq(uint32_t freq);
void analogWriteRange(uint32_t range);
void analogWriteResolution(int res);

// Timing
void delay(unsigned long);
void delayMicroseconds(unsigned int us);
unsigned long millis();

#ifdef __cplusplus
} // extern "C"
#endif

// Ancient AVR defines
#define HAVE_HWSERIAL0
#define HAVE_HWSERIAL1
#define HAVE_HWSERIAL2

#ifdef __cplusplus

#ifdef USE_TINYUSB
// Needed for declaring Serial
#include "Adafruit_USBD_CDC.h"
#else
#include "SerialUSB.h"
#endif

#include "SerialUART.h"
#include "RP2040.h"
#include "Bootsel.h"

// Template which will evaluate at *compile time* to a single 32b number
// with the specified bits set.
template <size_t N>
constexpr uint32_t __bitset(const int (&a)[N], size_t i = 0U) {
    return i < N ? (1L << a[i]) | __bitset(a, i + 1) : 0;
}
#endif


// ARM toolchain doesn't provide itoa etc, provide them
#include "api/itoa.h"

#endif // Arduino_h
