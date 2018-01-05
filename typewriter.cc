#include "typewriter.h"
#include <iostream>
#include <cctype>
#include <unordered_map>
#include <wiringPi.h>

const int kOutPort[] = {
  0, 2, 3, 12, 13, 14, 21, 22};

const int kSelectPort[] = {
  24, 23, 25};

const int kEnablePin = 7;
const int kLShiftPin = 29;
const int kCodePin = 28;
const int kModPin = 27;

// This value almost matches the typewriter speed, it's a bit faster actually.
const int kKeyPressDurationMs = 50;
const int kMaxLineChars = 70;

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
{'´', 30},
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
{' ', 52},
{'o', 56},
{'l', 57},
{'p', 58},
{'ö', 59},
{'.', 60},
{'9', 61},
{'0', 62},
{'-', 63}
};

const std::unordered_map<char, char> kShiftChars = {
{'!', '1'},
{'"', '2'},
{'§', '3'},
{'$', '4'},
{'%', '5'},
{'&', '6'},
{'/', '7'},
{'(', '8'},
{')', '9'},
{'=', '0'},
{'?', 'ß'},
{'`', '´'},
{'*', '+'},
{';', ','},
{':', '.'},
{'_', '-'}
};

const std::unordered_map<char, char> kModChars = {
{'µ', '1'},
{'²', '2'},
{'³', '3'},
{'£', '4'},
{'°', '5'},
{'\'', '6'},
{'#', '7'},
{'<', '8'},
{'>', '9'},
{'^', '´'},
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
  for (auto pin : {kEnablePin, kLShiftPin, kModPin, kCodePin}) {
    digitalWrite(pin, HIGH);
    pinMode(pin, OUTPUT);
  }
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
  bool dead_key = (c == '^' || c == '`' || c == '´');

  if (std::isupper(c)) {
    enable_shift(true);
    c = std::tolower(c);
  }
  auto shift_char_it = kShiftChars.find(c);
  if (shift_char_it != kShiftChars.end()) {
    enable_shift(true);
    delay(kKeyPressDurationMs);
    c = shift_char_it->second;
  }
  auto mod_char_it = kModChars.find(c);
  if (mod_char_it != kModChars.end()) {
    enable_mod(true);
    delay(kKeyPressDurationMs);
    c = mod_char_it->second;
  }
  auto ascii_2_key_it = kAscii2Key.find(c);
  if (ascii_2_key_it != kAscii2Key.end()) {
    press_key(ascii_2_key_it->second);
    line_chars_++;
    if (ascii_2_key_it->second == 51) line_chars_ = 0;
    if (line_chars_ > kMaxLineChars) {
      line_chars_ = 0;
      press_key(51);
    }
  }
  enable_shift(false);
  enable_mod(false);
  if (dead_key) { print_char(' '); }
}

void Typewriter::print_string(const std::string& s, Boldness b) {
  for (char c : s) {
    print_char(c, b);
  }
}

void Typewriter::enable_shift(bool enable) {
  digitalWrite(kLShiftPin, !enable);
}

void Typewriter::enable_mod(bool enable) {
  digitalWrite(kModPin, !enable);
}

void Typewriter::enable_code(bool enable) {
  digitalWrite(kCodePin, !enable);
}
