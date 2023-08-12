////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  ImageData_EXT_ASCII.h
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
////////////////////////////////////////////

#ifndef IMG_DATA_EXT_ASCII
#define IMG_DATA_EXT_ASCII

#include <cstdint>  // For uint8_t
#include <vector>

namespace MCK
{

class ImageData_EXT_ASCII
{
    public:

        static const std::vector<std::vector<uint8_t>> image_data;

};

}  // End of namespace MCK

#endif
