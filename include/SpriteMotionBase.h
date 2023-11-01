////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteMotionBase.h
//
//  Base motion class for sprites
//
//  This is non-abstract, as it can be
//  used to respresent sprites that 
//  maitain a static position relative
//  to their block. Hence the
//  'STATIIONARY' type designation.
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

#ifndef MCK_SPRITE_MOTION_BASE_H
#define MCK_SPRITE_MOTION_BASE_H

#include "Defs.h"
#include "SpritePos.h"

namespace MCK
{

class SpriteMotionBase : virtual public SpritePos
{
    public:

        // Default constructor
        SpriteMotionBase( void ) : SpritePos()
        {
            this->motion_type = MCK::SpriteMotionType::STATIONARY;
        }

        //! Constructor
        SpriteMotionBase(
            MCK::Point<float> _pos,
            std::shared_ptr<GameEngRenderBase> _render_instance
        ) : SpritePos( _pos, _render_instance )
        {}

        virtual ~SpriteMotionBase( void ) {}
       
        // Calculate position, based on time 
        virtual void calc_pos( void ) const noexcept {}

        /*
        MCK::SpriteMotionType get_type ( void ) const noexcept
        {
            return this->type;
        }
        */
    
    protected:

        // MCK::SpriteMotionType type;
};

}  // End of namespace MCK

#endif
