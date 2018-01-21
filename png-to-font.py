#!/usr/bin/python

from PIL import Image
im = Image.open("typewriter-font.png")
pix = im.load()

def getLinePix(ch, h):
    y = (ch >> 6)*32 + h
    x = (ch & 63) * 8
    result = []
    for p in range(0,8):
        result.append(1 - pix[x+p, y][0]/255)
    return result

def Pix2Byte(p):
    result = 0
    for x in range(0,8):
        result <<= 1
        if p[x]:
            result |= 1
    return result

print """#include "aalib.h"
/* binary image of font8x32 follows */

__AA_CONST unsigned char typewriter_font_data[] = {"""


for ch in range(0,256):
    chdata = []
    for l in range(0,32):
        p =  getLinePix(ch, l)
        chdata.append(Pix2Byte(p))
    print '  ' + ', '.join('0x%02x'%i for i in chdata) + ','

print """ };

__AA_CONST struct aa_font typewriter_font =
{typewriter_font_data, 32, "my typewriter font", "twf"};
"""
