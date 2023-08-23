////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  ImageDataASCII.cpp
//
//  Image Data for extended ASCII characters,
//  8x8 pixels
//
//  Copyright (c) Muckytaters 2023
//
//  This program is free software: you can
//  redistribute it and/or modify it under 
//  the terms of the GNU General Public License
//  as published by the Free Software Foundation,
//  either version 3 of the License, or (at your
//  option) any later version.
//
//  This program is distributed in the hope it
//  will be useful, but WITHOUT ANY WARRANTY;
//  without even the implied warranty of MERCHANTABILITY
//  or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU
//  General Public License along with this
//  program. If not, see http://www.gnu.org/license
//
//  IMPORTANT: Please see the statement in file 
//  'IMAGE_RIGHTS_DISCLAIMER.md'
//
////////////////////////////////////////////

#include "ImageDataASCII.h"

const std::vector<std::vector<uint8_t>> MCK::ImageDataASCII::image_data 
{  // Start of outer vector
{ // 0
0b00000001,
0b00000011,
0b00000111,
0b00001111,
0b00011111,
0b00111111,
0b01111111,
0b11111111
},
{ // 1
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00011000,
0b00111100,
0b01111110,
0b11111111
},
{ // 2
0b10000000,
0b11000000,
0b11100000,
0b11110000,
0b11111000,
0b11111100,
0b11111110,
0b11111111
},
{ // 3
0b11110000,
0b11110000,
0b11110000,
0b11110000,
0b11110000,
0b11110000,
0b11110000,
0b11110000
},
{ // 4
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b11110000,
0b11110000,
0b11110000,
0b11110000
},
{ // 5
0b11111111,
0b00000000,
0b11111111,
0b00000000,
0b11111111,
0b00000000,
0b11111111,
0b00000000
},
{ // 6
0b11111111,
0b11111111,
0b00000000,
0b00000000,
0b11111111,
0b11111111,
0b00000000,
0b00000000
},
{ // 7
0b01001001,
0b10010010,
0b00100100,
0b01001001,
0b10010010,
0b00100100,
0b01001001,
0b10010010
},
{ // 8
0b10011001,
0b00110011,
0b01100110,
0b11001100,
0b10011001,
0b00110011,
0b01100110,
0b11001100
},
{ // 9
0b00000001,
0b00000010,
0b00000100,
0b00001000,
0b00010000,
0b00100000,
0b01000000,
0b10000000
},
{ // 10
0b10000001,
0b01000010,
0b00100100,
0b00011000,
0b00011000,
0b00100100,
0b01000010,
0b10000001
},
{ // 11
0b10000001,
0b01000010,
0b00100100,
0b00011000,
0b00010000,
0b00100000,
0b01000000,
0b10000000
},
{ // 12
0b10000000,
0b01000000,
0b00100000,
0b00010000,
0b00010000,
0b00100000,
0b01000000,
0b10000000
},
{ // 13
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000
},
{ // 14
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b11111111
},
{ // 15
0b10000001,
0b10000001,
0b10000001,
0b10000001,
0b10000001,
0b10000001,
0b10000001,
0b11111111
},
{ // 16
0b11111111,
0b10000001,
0b10000001,
0b10000001,
0b10000001,
0b10000001,
0b10000001,
0b11111111
},
{ // 17
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b10000000
},
{ // 18
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b10000001
},
{ // 19
0b01010101,
0b00000000,
0b01010101,
0b00000000,
0b01010101,
0b00000000,
0b01010101,
0b00000000
},
{ // 20
0b01010101,
0b10101010,
0b01010101,
0b10101010,
0b01010101,
0b10101010,
0b01010101,
0b10101010
},
{ // 21
0b00011000,
0b00011000,
0b00011000,
0b00011000,
0b00011000,
0b00011000,
0b00011000,
0b00011000
},
{ // 22
0b00000000,
0b00000000,
0b00000000,
0b00011111,
0b00011111,
0b00011000,
0b00011000,
0b00011000
},
{ // 23
0b00011000,
0b00011000,
0b00011000,
0b00011111,
0b00011111,
0b00011000,
0b00011000,
0b00011000
},
{ // 24
0b00011000,
0b00011000,
0b00011000,
0b11111111,
0b11111111,
0b00011000,
0b00011000,
0b00011000
},
{ // 25
0b00000000,
0b00000000,
0b00000000,
0b00011000,
0b00011000,
0b00011000,
0b00011000,
0b00011000
},
{ // 26
0b01100110,
0b01100110,
0b01100110,
0b01100110,
0b01100110,
0b01100110,
0b01100110,
0b01100110
},
{ // 27
0b00000000,
0b01111111,
0b01111111,
0b01100000,
0b01100000,
0b01100111,
0b01100111,
0b01100110
},
{ // 28
0b01100110,
0b01100111,
0b01100111,
0b01100000,
0b01100000,
0b01100111,
0b01100111,
0b01100110
},
{ // 29
0b01100110,
0b11100111,
0b11100111,
0b00000000,
0b00000000,
0b11100111,
0b11100111,
0b01100110
},
{ // 30
0b01100110,
0b01100110,
0b01100110,
0b11100110,
0b11100110,
0b01100110,
0b01100110,
0b01100110
},
{ // 31
0b01100110,
0b01100110,
0b01100110,
0b11100111,
0b11100111,
0b01100110,
0b01100110,
0b01100110
},
{ // 32
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{ // 33
0b00011000,
0b00011000,
0b00011000,
0b00011000,
0b00011000,
0b00000000,
0b00011000,
0b00000000
},
{ // 34
0b00010100,
0b00010100,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{ // 35
0b00010100,
0b00010100,
0b01111111,
0b00010100,
0b01111111,
0b00010100,
0b00010100,
0b00000000
},
{ // 36
0b00001000,
0b00111111,
0b01001000,
0b00111110,
0b00001001,
0b01111110,
0b00001000,
0b00000000
},
{ // 37
0b01110001,
0b01010010,
0b01110100,
0b00001000,
0b00010111,
0b00100101,
0b01000111,
0b00000000
},
{ // 38
0b00111100,
0b01000010,
0b00110100,
0b01001000,
0b01000100,
0b01000010,
0b00111101,
0b00000000
},
{ // 39
0b00011000,
0b00001000,
0b00010000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{ // 40
0b00000100,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00000100,
0b00000000
},
{ // 41
0b00010000,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00010000,
0b00000000
},
{ // 42
0b00001000,
0b01001001,
0b00101010,
0b00011100,
0b00101010,
0b01001001,
0b00001000,
0b00000000
},
{ // 43
0b00000000,
0b00001000,
0b00001000,
0b00111110,
0b00001000,
0b00001000,
0b00000000,
0b00000000
},
{ // 44
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00011000,
0b00001000,
0b00010000
},
{ // 45
0b00000000,
0b00000000,
0b00000000,
0b00111110,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{ // 46
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00011000,
0b00011000,
0b00000000
},
{ // 47
0b00000001,
0b00000010,
0b00000100,
0b00001000,
0b00010000,
0b00100000,
0b01000000,
0b00000000
},
{ // 48
0b00111110,
0b01000011,
0b01000101,
0b01001001,
0b01010001,
0b01100001,
0b00111110,
0b00000000
},
{ // 49
0b00001000,
0b00011000,
0b00101000,
0b01001000,
0b00001000,
0b00001000,
0b01111111,
0b00000000
},
{ // 50
0b00111110,
0b01000001,
0b00000001,
0b00111110,
0b01000000,
0b01000000,
0b01111111,
0b00000000
},
{ // 51
0b00111110,
0b01000001,
0b00000001,
0b00011110,
0b00000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 52
0b00011100,
0b00100100,
0b01000100,
0b01000100,
0b01111111,
0b00000100,
0b00000100,
0b00000000
},
{ // 53
0b01111111,
0b01000000,
0b01000000,
0b01111110,
0b00000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 54
0b00111110,
0b01000000,
0b01000000,
0b01111110,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 55
0b01111111,
0b00000001,
0b00000010,
0b00000100,
0b00001000,
0b00001000,
0b00001000,
0b00000000
},
{ // 56
0b00111110,
0b01000001,
0b01000001,
0b00111110,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 57
0b00111110,
0b01000001,
0b01000001,
0b00111111,
0b00000001,
0b00000001,
0b00111110,
0b00000000
},
{ // 58
0b00000000,
0b00000000,
0b00011000,
0b00011000,
0b00000000,
0b00011000,
0b00011000,
0b00000000
},
{ // 59
0b00000000,
0b00000000,
0b00011000,
0b00011000,
0b00000000,
0b00011000,
0b00110000,
0b00000000
},
{ // 60
0b00000011,
0b00001100,
0b00110000,
0b01000000,
0b00110000,
0b00001100,
0b00000011,
0b00000000
},
{ // 61
0b00000000,
0b00000000,
0b00111110,
0b00000000,
0b00111110,
0b00000000,
0b00000000,
0b00000000
},
{ // 62
0b11000000,
0b00110000,
0b00001100,
0b00000010,
0b00001100,
0b00110000,
0b11000000,
0b00000000
},
{ // 63
0b00111110,
0b01000001,
0b00000001,
0b00011110,
0b00000000,
0b00011000,
0b00011000,
0b00000000
},
{ // 64
0b00111110,
0b01000001,
0b01011111,
0b01010001,
0b01011111,
0b01000000,
0b00111110,
0b00000000
},
{ // 65
0b00011100,
0b00100010,
0b01000001,
0b01111111,
0b01000001,
0b01000001,
0b01000001,
0b00000000
},
{ // 66
0b01111110,
0b01000001,
0b01000001,
0b01111110,
0b01000001,
0b01000001,
0b01111110,
0b00000000
},
{ // 67
0b00111110,
0b01000001,
0b01000000,
0b01000000,
0b01000000,
0b01000001,
0b00111110,
0b00000000
},
{ // 68
0b01111110,
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b01111110,
0b00000000
},
{ // 69
0b01111111,
0b01000000,
0b01000000,
0b01111100,
0b01000000,
0b01000000,
0b01111111,
0b00000000
},
{ // 70
0b01111111,
0b01000000,
0b01000000,
0b01111100,
0b01000000,
0b01000000,
0b01000000,
0b00000000
},
{ // 71
0b00111110,
0b01000001,
0b01000000,
0b01000111,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 72
0b01000001,
0b01000001,
0b01000001,
0b01111111,
0b01000001,
0b01000001,
0b01000001,
0b00000000
},
{ // 73
0b01111111,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b01111111,
0b00000000
},
{ // 74
0b01111111,
0b00000100,
0b00000100,
0b00000100,
0b00000100,
0b01000100,
0b00111000,
0b00000000
},
{ // 75
0b01000001,
0b01000010,
0b01000100,
0b01111000,
0b01000100,
0b01000010,
0b01000001,
0b00000000
},
{ // 76
0b01000000,
0b01000000,
0b01000000,
0b01000000,
0b01000000,
0b01000000,
0b01111111,
0b00000000
},
{ // 77
0b01000001,
0b01100011,
0b01010101,
0b01001001,
0b01000001,
0b01000001,
0b01000001,
0b00000000
},
{ // 78
0b01000001,
0b01100001,
0b01010001,
0b01001001,
0b01000101,
0b01000011,
0b01000001,
0b00000000
},
{ // 79
0b00111110,
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 80
0b01111110,
0b01000001,
0b01000001,
0b01111110,
0b01000000,
0b01000000,
0b01000000,
0b00000000
},
{ // 81
0b00111110,
0b01000001,
0b01000001,
0b01001001,
0b01000101,
0b01000010,
0b00111101,
0b00000000
},
{ // 82
0b01111110,
0b01000001,
0b01000001,
0b01111110,
0b01000001,
0b01000001,
0b01000001,
0b00000000
},
{ // 83
0b00111110,
0b01000001,
0b01000000,
0b00111110,
0b00000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 84
0b01111111,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00000000
},
{ // 85
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 86
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b00100010,
0b00010100,
0b00001000,
0b00000000
},
{ // 87
0b01000001,
0b01000001,
0b01000001,
0b01001001,
0b01010101,
0b01100011,
0b01000001,
0b00000000
},
{ // 88
0b01000001,
0b00100010,
0b00010100,
0b00001000,
0b00010100,
0b00100010,
0b01000001,
0b00000000
},
{ // 89
0b01000001,
0b01000001,
0b00100010,
0b00010100,
0b00001000,
0b00001000,
0b00001000,
0b00000000
},
{ // 90
0b01111111,
0b00000010,
0b00000100,
0b00001000,
0b00010000,
0b00100000,
0b01111111,
0b00000000
},
{ // 91
0b00011100,
0b00010000,
0b00010000,
0b00010000,
0b00010000,
0b00010000,
0b00011100,
0b00000000
},
{ // 92
0b01000000,
0b00100000,
0b00010000,
0b00001000,
0b00000100,
0b00000010,
0b00000001,
0b00000000
},
{ // 93
0b00011100,
0b00000100,
0b00000100,
0b00000100,
0b00000100,
0b00000100,
0b00011100,
0b00000000
},
{ // 94
0b00001000,
0b00010100,
0b00100010,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{ // 95
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b01111111,
0b00000000
},
{ // 96
0b00011000,
0b00010000,
0b00001000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{ // 97
0b00000000,
0b00000000,
0b00111110,
0b00000001,
0b00111111,
0b01000001,
0b00111111,
0b00000000
},
{ // 98
0b01000000,
0b01000000,
0b01111110,
0b01000001,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 99
0b00000000,
0b00000000,
0b00111110,
0b01000001,
0b01000000,
0b01000001,
0b00111110,
0b00000000
},
{ // 100
0b00000001,
0b00000001,
0b00111111,
0b01000001,
0b01000001,
0b01000001,
0b00111111,
0b00000000
},
{ // 101
0b00000000,
0b00000000,
0b00111110,
0b01000001,
0b01111111,
0b01000000,
0b00111110,
0b00000000
},
{ // 102
0b00001111,
0b00010000,
0b00010000,
0b01111111,
0b00010000,
0b00010000,
0b00010000,
0b00000000
},
{ // 103
0b00000000,
0b00000000,
0b00111111,
0b01000001,
0b00111111,
0b00000001,
0b00111110,
0b00000000
},
{ // 104
0b01000000,
0b01000000,
0b01111110,
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b00000000
},
{ // 105
0b00001000,
0b00000000,
0b00111000,
0b00001000,
0b00001000,
0b00001000,
0b00111110,
0b00000000
},
{ // 106
0b00000100,
0b00000000,
0b00011100,
0b00000100,
0b00000100,
0b00000100,
0b00111000,
0b00000000
},
{ // 107
0b01000000,
0b01000000,
0b01000001,
0b01000010,
0b01111100,
0b01000010,
0b01000001,
0b00000000
},
{ // 108
0b00001100,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00111110,
0b00000000
},
{ // 109
0b00000000,
0b00000000,
0b01110110,
0b01001001,
0b01001001,
0b01001001,
0b01001001,
0b00000000
},
{ // 110
0b00000000,
0b00000000,
0b01111110,
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b00000000
},
{ // 111
0b00000000,
0b00000000,
0b00111110,
0b01000001,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 112
0b00000000,
0b00000000,
0b01111110,
0b01000001,
0b01111110,
0b01000000,
0b01000000,
0b00000000
},
{ // 113
0b00000000,
0b00000000,
0b00111111,
0b01000001,
0b00111111,
0b00000001,
0b00000001,
0b00000000
},
{ // 114
0b00000000,
0b00000000,
0b01011110,
0b01100001,
0b01000000,
0b01000000,
0b01000000,
0b00000000
},
{ // 115
0b00000000,
0b00000000,
0b00111111,
0b01000000,
0b00111110,
0b00000001,
0b01111110,
0b00000000
},
{ // 116
0b00000000,
0b00100000,
0b01111110,
0b00100000,
0b00100000,
0b00100010,
0b00011100,
0b00000000
},
{ // 117
0b00000000,
0b00000000,
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b00111111,
0b00000000
},
{ // 118
0b00000000,
0b00000000,
0b01000001,
0b01000001,
0b00100010,
0b00010100,
0b00001000,
0b00000000
},
{ // 119
0b00000000,
0b00000000,
0b01000001,
0b01000001,
0b01001001,
0b01010101,
0b00100010,
0b00000000
},
{ // 120
0b00000000,
0b00000000,
0b01000001,
0b00110110,
0b00001000,
0b00110110,
0b01000001,
0b00000000
},
{ // 121
0b00000000,
0b00000000,
0b01000001,
0b01000001,
0b01111111,
0b00000001,
0b00111110,
0b00000000
},
{ // 122
0b00000000,
0b00000000,
0b01111111,
0b00000010,
0b00011100,
0b00100000,
0b01111111,
0b00000000
},
{ // 123
0b00000110,
0b00001000,
0b00001000,
0b00010000,
0b00001000,
0b00001000,
0b00000110,
0b00000000
},
{ // 124
0b00001000,
0b00001000,
0b00001000,
0b00000000,
0b00001000,
0b00001000,
0b00001000,
0b00000000
},
{ // 125
0b00110000,
0b00001000,
0b00001000,
0b00000100,
0b00001000,
0b00001000,
0b00110000,
0b00000000
},
{ // 126
0b00000000,
0b00000000,
0b00111001,
0b01001110,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{ // 127
0b00110011,
0b00110011,
0b11001100,
0b11001100,
0b00110011,
0b00110011,
0b11001100,
0b11001100
},
{ // 128
0b00111110,
0b01000001,
0b01000000,
0b01000000,
0b01000001,
0b00111110,
0b00000100,
0b00011100
},
{ // 129
0b00000000,
0b00100010,
0b00000000,
0b01000001,
0b01000001,
0b01000001,
0b00111111,
0b00000000
},
{ // 130
0b00000100,
0b00001000,
0b00111110,
0b01000001,
0b01111111,
0b01000000,
0b00111110,
0b00000000
},
{ // 131
0b00001000,
0b00010100,
0b00000000,
0b00111101,
0b01000011,
0b01000001,
0b00111111,
0b00000000
},
{ // 132
0b00000000,
0b00100010,
0b00000000,
0b00111101,
0b01000011,
0b01000001,
0b00111111,
0b00000000
},
{ // 133
0b00001000,
0b00000100,
0b00000000,
0b00111101,
0b01000011,
0b01000001,
0b00111111,
0b00000000
},
{ // 134
0b00011000,
0b00100100,
0b00011000,
0b00111101,
0b01000011,
0b01000001,
0b00111111,
0b00000000
},
{ // 135
0b00000000,
0b00000000,
0b00111111,
0b01000000,
0b01000000,
0b00111111,
0b00000100,
0b00011100
},
{ // 136
0b00001000,
0b00010100,
0b00111110,
0b01000001,
0b01111111,
0b01000000,
0b00111110,
0b00000000
},
{ // 137
0b00100010,
0b00000000,
0b00111110,
0b01000001,
0b01111111,
0b01000000,
0b00111110,
0b00100000
},
{ // 138
0b00010000,
0b00001000,
0b00111110,
0b01000001,
0b01111111,
0b01000000,
0b00111110,
0b00000000
},
{ // 139
0b00000000,
0b00100100,
0b00000000,
0b00011000,
0b00001000,
0b00001000,
0b00111110,
0b00000000
},
{ // 140
0b00001000,
0b00010100,
0b00000000,
0b00011000,
0b00001000,
0b00001000,
0b00111110,
0b00000000
},
{ // 141
0b00010000,
0b00001000,
0b00000000,
0b00011000,
0b00001000,
0b00001000,
0b00111110,
0b00000000
},
{ // 142
0b01000001,
0b00000000,
0b00111110,
0b01000001,
0b01111111,
0b01000001,
0b01000001,
0b00000000
},
{ // 143
0b00111100,
0b01000010,
0b00111100,
0b01000010,
0b01111110,
0b01000010,
0b01000010,
0b00000000
},
{ // 144
0b00001000,
0b00010000,
0b01111111,
0b01000000,
0b01111100,
0b01000000,
0b01111111,
0b00000000
},
{ // 145
0b00000000,
0b00000000,
0b00110110,
0b00001001,
0b01111111,
0b01001000,
0b00110111,
0b00000000
},
{ // 146
0b00000000,
0b00111111,
0b01001000,
0b01111110,
0b01001000,
0b01001000,
0b01001111,
0b00000000
},
{ // 147
0b00001000,
0b00010100,
0b00000000,
0b00111110,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 148
0b00000000,
0b00100010,
0b00000000,
0b00111110,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 149
0b00010000,
0b00001000,
0b00000000,
0b00111110,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 150
0b00001000,
0b00010100,
0b00000000,
0b01000001,
0b01000001,
0b01000001,
0b00111111,
0b00000000
},
{ // 151
0b00010000,
0b00001000,
0b00000000,
0b01000001,
0b01000001,
0b01000001,
0b00111111,
0b00000000
},
{ // 152
0b00000000,
0b00100010,
0b00000000,
0b01000001,
0b01111111,
0b00000001,
0b01111110,
0b00000000
},
{ // 153
0b01000001,
0b00000000,
0b00111110,
0b01000001,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 154
0b01000001,
0b00000000,
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 155
0b00000000,
0b00001000,
0b00111111,
0b01001000,
0b01001000,
0b00111111,
0b00001000,
0b00000000
},
{ // 156
0b00001100,
0b00010010,
0b00010000,
0b00111110,
0b00010000,
0b00010000,
0b01111111,
0b00000000
},
{ // 157
0b01000001,
0b00100010,
0b00010100,
0b00111110,
0b00001000,
0b00111110,
0b00001000,
0b00000000
},
{ // 158
0b00011110,
0b00100001,
0b01111000,
0b00100000,
0b01111000,
0b00100001,
0b00011110,
0b00000000
},
{ // 159
0b00011000,
0b00000000,
0b00011000,
0b00011000,
0b00011000,
0b00011000,
0b00011000,
0b00000000
},
{ // 160
0b00001000,
0b00010000,
0b00000000,
0b01111101,
0b10000011,
0b10000001,
0b01111111,
0b00000000
},
{ // 161
0b00000100,
0b00001000,
0b00000000,
0b00011000,
0b00001000,
0b00001000,
0b00111110,
0b00000000
},
{ // 162
0b00000100,
0b00001000,
0b00000000,
0b00111110,
0b01000001,
0b01000001,
0b00111110,
0b00000000
},
{ // 163
0b00000100,
0b00001000,
0b00000000,
0b01000001,
0b01000001,
0b01000001,
0b00111111,
0b00000000
},
{ // 164
0b00010100,
0b00101000,
0b00000000,
0b01111110,
0b01000001,
0b01000001,
0b01000001,
0b00000000
},
{ // 165
0b00001010,
0b00010100,
0b01000001,
0b01110001,
0b01001001,
0b01000111,
0b01000001,
0b00000000
},
{ // 166
0b00111000,
0b00100100,
0b00111100,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{ // 167
0b00011000,
0b00100100,
0b00011000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{ // 168
0b00011000,
0b00011000,
0b00000000,
0b01111000,
0b10000000,
0b10000010,
0b01111100,
0b00000000
},
{ // 169
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b01111111,
0b01000000,
0b01000000,
0b00000000
},
{ // 170
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b01111111,
0b00000001,
0b00000001,
0b00000000
},
{ // 171
0b00100010,
0b01100100,
0b00101011,
0b00101001,
0b00001011,
0b00010010,
0b00100011,
0b00000000
},
{ // 172
0b00100010,
0b01100100,
0b00101001,
0b00101011,
0b00010101,
0b00010111,
0b00100001,
0b00000000
},
{ // 173
0b01100010,
0b00100100,
0b01101001,
0b00101011,
0b01110101,
0b00010111,
0b00100001,
0b00000000
},
{ // 174
0b00001001,
0b00010010,
0b00100100,
0b01001000,
0b00100100,
0b00010010,
0b00001001,
0b00000000
},
{ // 175
0b01001000,
0b00100100,
0b00010010,
0b00001001,
0b00010010,
0b00100100,
0b01001000,
0b00000000
},
{ // 176
0b00000000,
0b00000000,
0b00110001,
0b01001010,
0b01000100,
0b01001010,
0b00110001,
0b00000000
},
{ // 177
0b00111110,
0b01000001,
0b01000001,
0b01001110,
0b01000001,
0b01010001,
0b01001110,
0b00000000
},
{ // 178
0b01111111,
0b00100001,
0b00100000,
0b00100000,
0b00100000,
0b00100000,
0b01110000,
0b00000000
},
{ // 179
0b00000000,
0b00000000,
0b00111111,
0b01010010,
0b00010010,
0b00010010,
0b00010010,
0b00000000
},
{ // 180
0b01111111,
0b00100001,
0b00010000,
0b00001000,
0b00010000,
0b00100001,
0b01111111,
0b00000000
},
{ // 181
0b00000000,
0b00000000,
0b00111111,
0b01000100,
0b01000010,
0b01000010,
0b00111100,
0b00000000
},
{ // 182
0b00000000,
0b00000000,
0b01100110,
0b00100010,
0b00100010,
0b00100010,
0b00111110,
0b01100000
},
{ // 183
0b00000000,
0b00000000,
0b00111110,
0b00001000,
0b00001000,
0b00001000,
0b00001110,
0b00000000
},
{ // 184
0b00011100,
0b00001000,
0b00111110,
0b01001001,
0b00111110,
0b00001000,
0b00011100,
0b00000000
},
{ // 185
0b00011100,
0b00100010,
0b01000001,
0b01111111,
0b01000001,
0b00100010,
0b00011100,
0b00000000
},
{ // 186
0b00111110,
0b01000001,
0b01000001,
0b01000001,
0b01100011,
0b00010100,
0b01110111,
0b00000000
},
{ // 187
0b00011100,
0b00010000,
0b00111000,
0b00100100,
0b00100010,
0b00100010,
0b00111110,
0b00000000
},
{ // 188
0b00000000,
0b00000000,
0b00110110,
0b01001001,
0b01001001,
0b00110110,
0b00000000,
0b00000000
},
{ // 189
0b00000000,
0b00100000,
0b01000110,
0b01001001,
0b01001001,
0b00111110,
0b00001000,
0b00001000
},
{ // 190
0b00111101,
0b01000011,
0b01000000,
0b00111000,
0b01000000,
0b01000001,
0b00111110,
0b00000000
},
{ // 191
0b00011100,
0b00100010,
0b00100010,
0b00100010,
0b00100010,
0b00100010,
0b00100010,
0b00000000
},
{ // 192
0b00000000,
0b00111111,
0b00000000,
0b00111111,
0b00000000,
0b00111111,
0b00000000,
0b00000000
},
{ // 193
0b00001000,
0b00001000,
0b00111110,
0b00001000,
0b00001000,
0b00000000,
0b00111110,
0b00000000
},
{ // 194
0b00110000,
0b00001100,
0b00000010,
0b00001100,
0b00110000,
0b00000000,
0b00111110,
0b00000000
},
{ // 195
0b00000110,
0b00011000,
0b00100000,
0b00011000,
0b00000110,
0b00000000,
0b00111110,
0b00000000
},
{ // 196
0b00000100,
0b00001010,
0b00001010,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00001000
},
{ // 197
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00101000,
0b00101000,
0b00010000,
0b00000000
},
{ // 198
0b00000000,
0b00001000,
0b00000000,
0b00111110,
0b00000000,
0b00001000,
0b00000000,
0b00000000
},
{ // 199
0b00000000,
0b00011001,
0b00100110,
0b00000000,
0b00011001,
0b00100110,
0b00000000,
0b00000000
},
{ // 200
0b00011100,
0b00010100,
0b00011100,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{ // 201
0b00000110,
0b00001001,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b01001000,
0b00110000
},
{ // 202
0b00000000,
0b00100010,
0b00010100,
0b00001000,
0b00010100,
0b00100010,
0b00000000,
0b00000000
},
{ // 203
0b00001111,
0b00001000,
0b00001000,
0b00010000,
0b00010000,
0b01010000,
0b00100000,
0b00000000
},
{ // 204
0b00011100,
0b00010100,
0b00010100,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{ // 205
0b00011000,
0b00001000,
0b00011000,
0b00010000,
0b00011000,
0b00000000,
0b00000000,
0b00000000
},
{ // 206
0b00000000,
0b00111110,
0b00111110,
0b00111110,
0b00111110,
0b00111110,
0b00000000,
0b00000000
},
{ // 207
0b01000001,
0b00100010,
0b00111110,
0b00010100,
0b00010100,
0b00001000,
0b00001000,
0b00000000
},
{ // 208
0b00110110,
0b01111111,
0b01111111,
0b01111111,
0b00111110,
0b00011100,
0b00001000,
0b00000000
},
{ // 209
0b00001000,
0b00011100,
0b00111110,
0b01111111,
0b00111110,
0b00011100,
0b00001000,
0b00000000
},
{ // 210
0b00001000,
0b00011100,
0b00111110,
0b01111111,
0b00111110,
0b00011100,
0b00111110,
0b00000000
},
{ // 211
0b00011100,
0b00011100,
0b01101011,
0b01111111,
0b01101011,
0b00001000,
0b00111110,
0b00000000
},
{ // 212
0b00011100,
0b00111110,
0b01111111,
0b01111111,
0b01111111,
0b00111110,
0b00011100,
0b00000000
},
{ // 213
0b00011100,
0b00100010,
0b01000001,
0b01000001,
0b01000001,
0b00100010,
0b00011100,
0b00000000
},
{ // 214
0b00001000,
0b00001000,
0b01111111,
0b00111110,
0b00011100,
0b00110110,
0b00100010,
0b00000000
},
{ // 215
0b00000000,
0b00000000,
0b00001000,
0b00011100,
0b00011100,
0b00001000,
0b00011100,
0b00000000
},
{ // 216
0b00000000,
0b00000000,
0b00101010,
0b00111110,
0b00011100,
0b00011100,
0b00111110,
0b00000000
},
{ // 217
0b00000000,
0b00000010,
0b00001110,
0b00111010,
0b00111110,
0b00000110,
0b00011111,
0b00000000
},
{ // 218
0b00001000,
0b00000100,
0b00010100,
0b00011100,
0b00001000,
0b00001000,
0b00011100,
0b00000000
},
{ // 219
0b00001100,
0b00011110,
0b00011110,
0b00000000,
0b00001100,
0b00001100,
0b00011110,
0b00000000
},
{ // 220
0b00010010,
0b00011110,
0b00011110,
0b00000000,
0b00001100,
0b00001100,
0b00011110,
0b00000000
},
{ // 221
0b00011100,
0b00011100,
0b00001000,
0b00111110,
0b00001000,
0b00010100,
0b00100010,
0b00000000
},
{ // 222
0b01011101,
0b01110111,
0b01010101,
0b00010100,
0b01011101,
0b01111111,
0b01011101,
0b00000000
},
{ // 223
0b00001000,
0b00001000,
0b00000000,
0b00000000,
0b00001000,
0b00001000,
0b00000000,
0b00000000
},
{ // 224
0b01111111,
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b01000001,
0b01111111,
0b00000000
},
{ // 225
0b01111111,
0b01000001,
0b01010101,
0b01001001,
0b01010101,
0b01000001,
0b01111111,
0b00000000
},
{ // 226
0b00000001,
0b00000011,
0b00000110,
0b00001100,
0b01011000,
0b01110000,
0b00100000,
0b00000000
},
{ // 227
0b01000001,
0b01100011,
0b00110110,
0b00011100,
0b00110110,
0b01100011,
0b01000001,
0b00000000
},
{ // 228
0b00000000,
0b00000000,
0b00000000,
0b00011000,
0b00011000,
0b00000000,
0b00000000,
0b00000000
},
{ // 229
0b00000000,
0b00000000,
0b00111100,
0b00111100,
0b00111100,
0b00111100,
0b00000000,
0b00000000
},
{ // 230
0b11111111,
0b11111111,
0b11111111,
0b11111111,
0b11111111,
0b11111111,
0b11111111,
0b11111111
},
{ // 231
0b00000000,
0b00000000,
0b00111100,
0b00100100,
0b00100100,
0b00111100,
0b00000000,
0b00000000
},
{ // 232
0b00000000,
0b01111110,
0b01000010,
0b01000010,
0b01000010,
0b01000010,
0b01111110,
0b00000000
},
{ // 233
0b00000000,
0b01111110,
0b01000010,
0b01011010,
0b01011010,
0b01000010,
0b01111110,
0b00000000
},
{ // 234
0b01111111,
0b01000001,
0b01011101,
0b01011101,
0b01011101,
0b01000001,
0b01111111,
0b00000000
},
{ // 235
0b01111111,
0b01000001,
0b01011101,
0b01010101,
0b01011101,
0b01000001,
0b01111111,
0b00000000
},
{ // 236
0b00000000,
0b00000000,
0b00001000,
0b00011100,
0b00001000,
0b00000000,
0b00000000,
0b00000000
},
{ // 237
0b00001000,
0b00001000,
0b00001000,
0b01111111,
0b00001000,
0b00001000,
0b00001000,
0b00000000
},
{ // 238
0b00011100,
0b00011100,
0b01111111,
0b01111111,
0b01111111,
0b00011100,
0b00011100,
0b00000000
},
{ // 239
0b00011100,
0b00010100,
0b01110111,
0b01000001,
0b01110111,
0b00010100,
0b00011100,
0b00000000
},
{ // 240
0b00111110,
0b01111111,
0b01001001,
0b01101011,
0b00111110,
0b00101010,
0b00000000,
0b00000000
},
{ // 241
0b00011100,
0b00111110,
0b01111111,
0b01111111,
0b01111111,
0b01111111,
0b01111111,
0b00000000
},
{ // 242
0b00011100,
0b00011100,
0b00011100,
0b00011100,
0b00011000,
0b00010100,
0b00001100,
0b00011100
},
{ // 243
0b00011000,
0b00010100,
0b00001100,
0b00011100,
0b00011000,
0b00010100,
0b00001100,
0b00011100
},
{ // 244
0b00011100,
0b00011100,
0b00011100,
0b00011100,
0b00011000,
0b00010000,
0b00000000,
0b00000000
},
{ // 245
0b01000010,
0b01111110,
0b01000010,
0b01000010,
0b01000010,
0b01111110,
0b01000010,
0b01000010
},
{ // 246
0b11011111,
0b11011111,
0b11011111,
0b00000000,
0b11111011,
0b11111011,
0b11111011,
0b00000000
},
{ // 247
0b10111011,
0b00000000,
0b11101110,
0b00000000,
0b10111011,
0b00000000,
0b11101110,
0b00000000
},
{ // 248
0b01010101,
0b00000000,
0b01000001,
0b00000000,
0b01000001,
0b00000000,
0b01010101,
0b00000000
},
{ // 249
0b00001000,
0b00001100,
0b00001110,
0b00001010,
0b00001000,
0b00111000,
0b00111000,
0b00000000
},
{ // 250
0b10001000,
0b01001100,
0b00101110,
0b00011010,
0b00001000,
0b00111100,
0b00111010,
0b00000001
},
{ // 251
0b00001000,
0b00001000,
0b00011100,
0b00011100,
0b00111110,
0b01111111,
0b00001000,
0b00010100
},
{ // 252
0b00001000,
0b00001000,
0b01111111,
0b00111110,
0b00001000,
0b00001000,
0b00011100,
0b00000000
},
{ // 253
0b01110111,
0b00100111,
0b00100101,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{ // 254
0b00111110,
0b01000001,
0b01011101,
0b01010101,
0b01011001,
0b01010101,
0b01000001,
0b00111110
},
{ // 255
0b00111110,
0b01000001,
0b01011101,
0b01010001,
0b01011101,
0b01000001,
0b00111110,
0b00000000
}
};  // End of outer vector
