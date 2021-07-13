/*
 * Copyright (c) 2019 Brian T. Park
 *
 * Parts derived from the Arduino SDK
 * Copyright (c) 2005-2013 Arduino Team
 *
 * Parts inspired by [Entering raw
 * mode](https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html).
 *
 * Parts inspired by [ESP8266 Host
 * Emulation](https://github.com/esp8266/Arduino/tree/master/tests/host).
 *
 */

#include <inttypes.h>
#include <unistd.h> // usleep()
#include <time.h> // clock_gettime()
#include "Arduino.h"

// -----------------------------------------------------------------------
// Arduino methods emulated in Unix
// -----------------------------------------------------------------------

void yield() {
  char c = '\0';
  if (read(STDIN_FILENO, &c, 1) == 1) {
    Serial.insertChar(c);
  }

  usleep(1000); // prevents program from consuming 100% CPU
}

void pinMode(uint8_t /*pin*/, uint8_t /*mode*/) {}

void digitalWrite(uint8_t /*pin*/, uint8_t /*val*/) {}

int digitalRead(uint8_t /*pin*/) { return 0; }

int analogRead(uint8_t /*pin*/) { return 0; }

void analogWrite(uint8_t /*pin*/, int /*val*/) {}

unsigned long millis() {
  struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  unsigned long ms = spec.tv_sec * 1000U + spec.tv_nsec / 1000000UL;
  return ms;
}

unsigned long micros() {
  struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  unsigned long us = spec.tv_sec * 1000000UL + spec.tv_nsec / 1000U;
  return us;
}

void delay(unsigned long ms) {
  usleep(ms * 1000);
}

void delayMicroseconds(unsigned int us) {
  usleep(us);
}

unsigned long pulseIn(
    uint8_t /*pin*/, uint8_t /*state*/,unsigned long /*timeout*/) {
  return 0;
}

unsigned long pulseInLong(
    uint8_t /*pin*/, uint8_t /*state*/, unsigned long /*timeout*/) {
  return 0;
}

void shiftOut(
    uint8_t /*dataPin*/, uint8_t /*clockPin*/, uint8_t /*bitOrder*/,
    uint8_t /*val*/) {}

uint8_t shiftIn(
    uint8_t /*dataPin*/, uint8_t /*clockPin*/, uint8_t /*bitOrder*/) {
  return 0;
}
