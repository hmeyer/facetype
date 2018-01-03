#include "typewriter.h"
#include <iostream>
#include <unordered_map>
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

// This value almost matches the typewriter speed, it's a bit faster actually.
const int kKeyPressDurationMs = 50;

const std::unordered_map<char, int> kAscii2Key = {
{'m', 0},
{',', 1},
{'v', 2},
{'c', 3},
{'x', 4},
{'n', 5},
{'b', 6},
{'y', 7},
{'u', 8},
{'i', 9},
{'r', 10},
{'e', 11},
{'w', 12},
{'z', 13},
{'t', 14},
{'q', 15},
{'+', 24},
{'ü', 25},
{'ä', 27},
{'ß', 29},
{'\'', 30},
{'j', 32},
{'k', 33},
{'f', 34},
{'d', 35},
{'s', 36},
{'h', 37},
{'g', 38},
{'a', 39},
{'7', 40},
{'8', 41},
{'4', 42},
{'3', 43},
{'2', 44},
{'6', 45},
{'5', 46},
{'1', 47},
// 48 DELETE
// 49 BACK
{'\r', 51},
{'o', 56},
{'l', 57},
{'p', 58},
{'ö', 59},
{'.', 60},
{'9', 61},
{'0', 62},
{'-', 63}
};

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

void Typewriter::press_key(int key) {
  int key_mod_8 = key & 7;
  for (int i = 0; i < 8; i++) {
    digitalWrite(kOutPort[i], i != key_mod_8);
  }
  std::cout << std::endl;
  int select = (key >> 3) & 7;
  for (int i = 0; i < 3; i++) {
    digitalWrite(kSelectPort[i], select & (1<<i));
  }
  std::cout << std::endl;
  digitalWrite(kEnablePin, false);
  delay(kKeyPressDurationMs);
  digitalWrite(kEnablePin, true);
}

void Typewriter::print_char(char c, Boldness b) {
  auto it = kAscii2Key.find(c);
  if (it != kAscii2Key.end()) {
    press_key(it->second);
  }
}

void Typewriter::print_string(const std::string& s, Boldness b) {
  for (char c : s) {
    print_char(c, b);
  }
}

