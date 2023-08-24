////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  ImageDataASCII.h
//
//  Image Data for extended ASCII characters
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

#ifndef IMG_DATA_ASCII_H
#define IMG_DATA_ASCII_H

#include <cstdint>  // For uint8_t et al
#include <vector>

namespace MCK
{

struct ImageDataASCII
{
        //! Vector of character images, each stored in a flat (1d) vector
        static const std::vector<std::vector<uint8_t>> image_data;

        //! Bit depth shared by all images in 'image_data'
        static const uint8_t BITS_PER_PIXEL = 1;

        //! Width (in pixels), shared by all images in 'image_data'
        static const uint16_t PITCH_IN_PIXELS = 8;

        //! Height (in pixels), shared by all images in 'image_data'
        static const uint16_t HEIGHT_IN_PIXELS = 8;
};

}  // End of namespace MCK

#endif
