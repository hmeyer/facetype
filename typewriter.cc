#include "typewriter.h"
#include <wiringPi.h>

const int kOutPort[] = {
  0, 1, 2, 3, 4, 5, 6, 7};

const int kSelectPort[] = {
  11, 8, 9};

const int kEnablePin = 10;

Typewriter::Typewriter() {
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

void Typewriter::press_key(int key) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(kOutPort[i], key & (1<<i));
  }
  int select = (key >> 3) & 7;
  for (int i = 0; i < 3; i++) {
    digitalWrite(kSelectPort[i], select & (1<<i));
  }
  digitalWrite(kEnablePin, LOW);
  delay(100);
  digitalWrite(kEnablePin, HIGH);
}
