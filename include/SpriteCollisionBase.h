////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteCollisionBase.h
//
//  Base collision class for sprites
//
//  This is non-abstract, as it can be
//  used to respresent sprites that 
//  do not require collision detection
//  Hence the 'NONE' type designation.
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

#ifndef MCK_SPRITE_COL_BASE_H
#define MCK_SPRITE_COL_BASE_H

#include "Defs.h"
#include "CollisionEvent.h"
#include "SpritePos.h"

namespace MCK
{

class SpriteCollisionBase : virtual public SpritePos
{
    public:

        SpriteCollisionBase( void ) : SpritePos()
        {
            this->type = MCK::SpriteCollisionType::NONE;
        }
        
        virtual ~SpriteCollisionBase( void ) {}
    
        //! Check for collision
        /*! Base version of this method does nothing */
        virtual void check_all_collisions( 
            std::vector<MCK::CollisionEvent> &collisions
        ) const {}

        virtual MCK::SpriteCollisionType get_type( void ) const noexcept
        {
            return this->type;
        }

    protected:

        MCK::SpriteCollisionType type;
};

}  // End of namespace MCK

#endif
