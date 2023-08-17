////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  Defs.h
//
//  Common definitions file
//
//  Note: header only, there is no 'Defs.cpp' file
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
////////////////////////////////////////////

#ifndef MCK_DEFS_H
#define MCK_DEFS_H

// Defines data type used for object IDs
#define MCK_IMG_ID_TYPE uint32_t
#define MCK_PAL_ID_TYPE uint32_t
#define MCK_TEX_ID_TYPE uint64_t

namespace MCK
{
    //! Ceiling/invalid value to image ids
    const MCK_IMG_ID_TYPE INVALID_IMG_ID = 0xFFFFFFFF;

    //! Safety parameter - max window width
    const uint16_t MAX_WINDOW_WIDTH_IN_PIXELS = 4096;
    
    //! Safety parameter - max window height
    const uint16_t MAX_WINDOW_HEIGHT_IN_PIXELS = 2048;

    //! Expected max number of local colo(u)r palettes, as a power of 2
    /* Note: This is a 'soft' upper limit, exceeding it
     *       is permissible but may reduce efficiency
     *       during initialization.
     */
    const uint16_t MAX_LOCAL_COLOR_PALETTES = 1024; 

    //! Expected max number of images, as a power of 2
    /* Note: This is a 'soft' upper limit, exceeding it
     *       is permissible but may cause delays during
     *       imitialization.
     * Note: An 'image' here means just pixel data, so many
     *       textures can be created from one image.
     */
    const uint16_t MAX_IMAGES = 2048; 

    //! Colo(u)r codes for core palette
    /*! Note US spelling of GRAY used here
     *  These are separate values rather than an enumeration
     *  so that it is easier to use them as integers*/
    static const uint8_t COL_BLACK = 0;
    static const uint8_t COL_BLACK_SEMI_TRANS = 1;
    static const uint8_t COL_DARK_GRAY = 2;
    static const uint8_t COL_DARK_RED_SEMI_TRANS = 3;
    static const uint8_t COL_TRANSPARENT = 4;
    static const uint8_t COL_WHITE = 5;
    static const uint8_t COL_LIGHT_GRAY = 6;
    static const uint8_t COL_MID_GRAY = 7;
    static const uint8_t COL_V_DARK_GRAY = 8;
    static const uint8_t COL_DARK_RED = 9;
    static const uint8_t COL_DARK_GREEN = 10;
    static const uint8_t COL_DARK_BLUE = 11;
    static const uint8_t COL_PINK = 12;
    static const uint8_t COL_PASTEL_GREEN = 13;
    static const uint8_t COL_PASTEL_BLUE = 14;
    static const uint8_t COL_PASTEL_YELLOW = 15;
    static const uint8_t COL_RED = 16;
    static const uint8_t COL_GREEN = 17;
    static const uint8_t COL_BLUE = 18;
    static const uint8_t COL_YELLOW = 19;
    static const uint8_t COL_CYAN = 20;
    static const uint8_t COL_MAGENTA = 21;
    static const uint8_t COL_ORANGE = 22;
    static const uint8_t COL_LIME = 23;
    static const uint8_t COL_PURPLE = 24;
    static const uint8_t COL_FOREST_GREEN = 25;
    static const uint8_t COL_FIELD_GRAY = 26;
    static const uint8_t COL_ROTTING_PURPLE = 27;
    static const uint8_t COL_FLESHTONE = 28;
    static const uint8_t COL_SAND = 29;
    static const uint8_t COL_MID_BROWN = 30;
    static const uint8_t COL_DARK_BROWN = 31;
    static const uint8_t TOTAL_CORE_COLORS = 32;
    
    static const uint8_t COL_INVALID = 255;

}  // End of namespace MCK

#endif
