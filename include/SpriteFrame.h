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
    uint32_t duration;
    MCK_IMG_ID_TYPE image_id;
    MCK_PAL_ID_TYPE palette_id;
    bool keep_orig_dest_rect_width;
    bool keep_orig_dest_rect_height;
    int offset_x;
    int offset_y;
    uint8_t flags;

    //! Default constructor
    SpriteFrame( void )
    {
        this->duration = 0;
        this->image_id = MCK::INVALID_IMG_ID;
        this->palette_id = MCK::INVALID_PAL_ID;
        this->keep_orig_dest_rect_width = true;
        this->keep_orig_dest_rect_height = true;
        this->offset_x = 0;
        this->offset_y = 0;
        this->flags = 0;
    }

    //! Constructor
    /*! @param _duration: Duration of frame (in ticks or pixels)
     */
    SpriteFrame(
        uint32_t _duration,
        MCK_IMG_ID_TYPE _image_id,
        MCK_PAL_ID_TYPE _palette_id,
        bool _keep_orig_dest_rect_width = true,
        bool _keep_orig_dest_rect_height = true,
        int _offset_x = 0,
        int _offset_y = 0,
        uint8_t _flags = 0
    ) : 
        duration( _duration ),
        image_id( _image_id ),
        palette_id ( _palette_id ),
        keep_orig_dest_rect_width( _keep_orig_dest_rect_width ),
        keep_orig_dest_rect_height( _keep_orig_dest_rect_height ),
        offset_x( _offset_x ),
        offset_y( _offset_y ),
        flags( _flags )
    {}
};

}  // End of namespace MCK

#endif
