////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  GameEngRenderBlock.h
//
//  A vector of GameEngRenderInfo structs,
//  a list of subservient blocks,
//  and additional meta information
//
//  This file has no corresponding .cpp file
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

#ifndef MCK_GAME_ENG_RB_H
#define MCK_GAME_ENG_RB_H

#include <vector>

#include "GameEngRenderInfo.h"

namespace MCK
{

struct GameEngRenderBlock
{
    //! Flag to indicate if block should be rendered
    /*! True = render, false = don't render */
    bool active;

    //! Optional horizontal offset (in screen pixels)
    int16_t hoz_offset;

    //! Optional vertical offset (in screen pixels)
    int16_t vert_offset;

    //! Vector of render information associated with this block

    std::vector<std::shared_ptr<MCK::GameEngRenderInfo>> render_info;

    //! List of blocks subserviant to this block
    std::list<std::shared_ptr<MCK::GameEngRenderBlock>> sub_blocks;

    //! Default constructor
    GameEngRenderBlock( void )
    {
        this->active = true;
        this->hoz_offset = 0;
        this->vert_offset = 0;
    }

};

}  // End of namespace MCK

#endif
