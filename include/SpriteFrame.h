////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteFrame.h
//
//  Base animation class for sprite images. 
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

#ifndef MCK_SPRITE_FRAME_H
#define MCK_SPRITE_FRAME_H

#include "Defs.h"

namespace MCK
{

struct SpriteFrame
{
    uint32_t duration_in_ticks;
    MCK_IMG_ID_TYPE image_id;
    MCK_PAL_ID_TYPE palette_id;
    bool keep_orig_dest_rect_size;

    // TODO: offsets?
    
    // Default constructor
    SpriteFrame( void )
    {
        duration_in_ticks = 0;
        image_id = MCK::INVALID_IMG_ID;
        palette_id = MCK::INVALID_PAL_ID;
        keep_orig_dest_rect_size = true;
    }

    // Constructor
    SpriteFrame(
        uint32_t _duration_in_ticks,
        MCK_IMG_ID_TYPE _image_id,
        MCK_PAL_ID_TYPE _palette_id,
        bool _keep_orig_dest_rect_size = true
    ) : 
        duration_in_ticks( _duration_in_ticks ),
        image_id( _image_id ),
        palette_id ( _palette_id ),
        keep_orig_dest_rect_size( _keep_orig_dest_rect_size )
    {}
};

}  // End of namespace MCK

#endif
