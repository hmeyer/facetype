#include "typewriter.h"
#include <iostream>
#include <cctype>
#include <unordered_map>
#include <array>
#include <wiringPi.h>

const int kOutPort[] = {
        0, 2, 3, 12, 13, 14, 21, 22
};

const int kSelectPort[] = {
        24, 23, 25
};

const int kEnablePin = 7;
const int kLShiftPin = 29;
const int kCodePin = 28;
const int kModPin = 27;

// This value almost matches the typewriter speed, it's a bit faster actually.
const int kEnableWaitMs = 10;
const int kKeyPressDurationMs = 80;
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
        {0xfc /* ü */, 25},
        {0xe4 /* ä */, 27},
        {0xdf /* ß */, 29},
        {0xb4 /* ´ */, 30},
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
        {0xf6 /* ö */, 59},
        {'.', 60},
        {'9', 61},
        {'0', 62},
        {'-', 63}
};

const std::unordered_map<char, char> kShiftChars = {
        {'!', '1'},
        {'"', '2'},
        {0xa7 /* § */, '3'},
        {'$', '4'},
        {'%', '5'},
        {'&', '6'},
        {'/', '7'},
        {'(', '8'},
        {')', '9'},
        {'=', '0'},
        {'?', 0xdf /* ß */},
        {'`', 0xb4 /* ´ */},
        {'*', '+'},
        {';', ','},
        {':', '.'},
        {'_', '-'},
        {0xc4 /* Ä */, 0xe4 /* ä */},
        {0xd6 /* Ö */, 0xf6 /* ö */},
        {0xdc /* Ü */, 0xfc /* ü */}
};

const std::unordered_map<char, char> kModChars = {
        {0xb5 /* µ */, '1'},
        {0xb2 /* ² */, '2'},
        {0xb3 /* ³ */, '3'},
        {0xa3 /* £ */, '4'},
        {0xb0 /* ° */, '5'},
        {'\'', '6'},
        {'#', '7'},
        {'<', '8'},
        {'>', '9'},
        {'^', 0xb4 /* ´ */},
};

const std::unordered_map<char, std::array<char,2> > kAccentChars = {
        // Sadly those look ugly.
        // {0xc0 /* À */, std::array<char, 2>{'`', 'A'}},
        // {0xc1 /* Á */, std::array<char, 2>{0xb4 /* ´ */, 'A'}},
        // {0xc2 /* Â */, std::array<char, 2>{'^', 'A'}},
        // {0xc8 /* È */, std::array<char, 2>{'`', 'E'}},
        // {0xc9 /* É */, std::array<char, 2>{0xb4 /* ´ */, 'E'}},
        // {0xca /* Ê */, std::array<char, 2>{'^', 'E'}},
        // {0xcc /* Ì */, std::array<char, 2>{'`', 'I'}},
        // {0xcd /* Í */, std::array<char, 2>{0xb4 /* ´ */, 'I'}},
        // {0xce /* Î */, std::array<char, 2>{'^', 'I'}},
        // {0xd2 /* Ò */, std::array<char, 2>{'`', 'O'}},
        // {0xd3 /* Ó */, std::array<char, 2>{0xb4 /* ´ */, 'O'}},
        // {0xd4 /* Ô */, std::array<char, 2>{'^', 'O'}},
        // {0xd9 /* Ù */, std::array<char, 2>{'`', 'U'}},
        // {0xda /* Ú */, std::array<char, 2>{0xb4 /* ´ */, 'U'}},
        // {0xdb /* Û */, std::array<char, 2>{'^', 'U'}},
        // {0xdd /* Ý */, std::array<char, 2>{0xb4 /* ´ */, 'Y'}},

        {0xe0 /* à */, std::array<char, 2>{'`', 'a'}},
        {0xe1 /* á */, std::array<char, 2>{0xb4 /* ´ */, 'a'}},
        {0xe2 /* â */, std::array<char, 2>{'^', 'a'}},
        {0xe8 /* è */, std::array<char, 2>{'`', 'e'}},
        {0xe9 /* é */, std::array<char, 2>{0xb4 /* ´ */, 'e'}},
        {0xea /* ê */, std::array<char, 2>{'^', 'e'}},
        {0xec /* ì */, std::array<char, 2>{'`', 'i'}},
        {0xed /* í */, std::array<char, 2>{0xb4 /* ´ */, 'i'}},
        {0xee /* î */, std::array<char, 2>{'^', 'i'}},
        {0xf2 /* ò */, std::array<char, 2>{'`', 'o'}},
        {0xf3 /* ó */, std::array<char, 2>{0xb4 /* ´ */, 'o'}},
        {0xf4 /* ô */, std::array<char, 2>{'^', 'o'}},
        {0xf9 /* ù */, std::array<char, 2>{'`', 'u'}},
        {0xfa /* ú */, std::array<char, 2>{0xb4 /* ´ */, 'u'}},
        {0xfb /* û */, std::array<char, 2>{'^', 'u'}},
        {0xfd /* ý */, std::array<char, 2>{0xb4 /* ´ */, 'y'}}
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
        delay(kEnableWaitMs);
}

void Typewriter::print_char(char c, Boldness b) {
        print_char_base(c);
}

void Typewriter::print_char_base(char c, bool undead) {
        bool dead_key = (c == '^' || c == '`' || c == 0xb4 /* ´ */);

        auto accent_char_it = kAccentChars.find(c);
        if (accent_char_it != kAccentChars.end()) {
                print_char_base(accent_char_it->second[0], true /* undead */);
                print_char_base(accent_char_it->second[1]);
                return;
        }
        if (std::isupper(c)) {
                enable_shift(true);
                delay(kEnableWaitMs);
                c = std::tolower(c);
        }
        auto shift_char_it = kShiftChars.find(c);
        if (shift_char_it != kShiftChars.end()) {
                enable_shift(true);
                delay(kEnableWaitMs);
                c = shift_char_it->second;
        }
        auto mod_char_it = kModChars.find(c);
        if (mod_char_it != kModChars.end()) {
                enable_mod(true);
                delay(kEnableWaitMs);
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
        if (ascii_2_key_it == kAscii2Key.end() ||
            (!undead && dead_key)) { print_char(' '); }
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
