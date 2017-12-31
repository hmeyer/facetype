#include "typewriter.h"
#include <iostream>
#include <wiringPi.h>

const int kOutPort[] = {
//  0, 2, 3, 12, 13, 14, 21, 22
  0, 1, 2, 3, 4, 5, 6, 7};

const int kSelectPort[] = {
//  23, 24, 25};
  11, 8, 9};

// const int kEnablePin = 7;
const int kEnablePin = 10;
const int kLShiftPin = 29;
const int kModPin = 28;
const int kCodePin = 27;

// Keyboard Matrix duty cycle is 8ms, so 10ms should be safe.
const int kKeyPressDurationMs = 10;

Typewriter::Typewriter() {
  wiringPiSetup();
  for (auto pin : kOutPort) {
    digitalWrite(pin, HIGH);
    pinMode(pin, OUTPUT);
  }
  for (auto pin : kSelectPort) {
    digitalWrite(pin, HIGH);
    pinMode(pin, OUTPUT);
  }
  digitalWrite(kEnablePin, HIGH);
  pinMode(kEnablePin, OUTPUT);
}

void loggingWrite(int pin, bool value) {
  std::cout << "P" << pin << ":" << (value?"-":"#") << " ";
  digitalWrite(pin, value);
}

void Typewriter::press_key(int key) {
  int key_mod_8 = key & 7;
  for (int i = 0; i < 8; i++) {
    loggingWrite(kOutPort[i], i != key_mod_8);
  }
  std::cout << std::endl;
  int select = (key >> 3) & 7;
  for (int i = 0; i < 3; i++) {
    loggingWrite(kSelectPort[i], select & (1<<i));
  }
  std::cout << std::endl;
  loggingWrite(kEnablePin, false);
  std::cout << std::endl;
  delay(kKeyPressDurationMs);
  loggingWrite(kEnablePin, true);
  std::cout << std::endl;
}
