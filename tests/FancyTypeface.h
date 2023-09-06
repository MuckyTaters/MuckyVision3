////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  FancyTypeface.h (DEMO FILE)
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

#ifndef IMG_FANCY
#define IMG_FANCY

#include <cstdint>  // For uint8_t
#include <vector>

namespace MCK
{

struct FancyTypeface
{
        //! Vector of miscellaneous images, each stored in a flat (1d) vector
        /*! ASCII 0-4 */
        static const std::vector<std::vector<uint8_t>> misc_pixel_data;
        
        //! Period (full stop), stored in a flat (1d ) vector
        /*! ASCII 46 */
        static const std::vector<uint8_t> period_pixel_data;

        //! Vector of numeral character images, each stored in a flat (1d) vector
        /*! ASCII 48-57 */
        static const std::vector<std::vector<uint8_t>> numeral_pixel_data;
        
        //! Vector of upper case character images, each stored in a flat (1d) vector
        /*! ASCII 65-90 */
        static const std::vector<std::vector<uint8_t>> upper_case_pixel_data;
        
        //! Copyright symbol, stored in a flat (1d ) vector
        /*! ASCII 255 (in MuckyVision) */
        static const std::vector<uint8_t> copyright_symbol_pixel_data;

        //! Bit depth shared by all images in this class
        static const uint8_t BITS_PER_PIXEL = 2;

        //! Width (in pixels), shared by all images
        static const uint16_t PITCH_IN_PIXELS = 12;

        //! Height (in pixels), shared by all images
        static const uint16_t HEIGHT_IN_PIXELS = 12;
};

}  // End of namespace MCK

#endif
