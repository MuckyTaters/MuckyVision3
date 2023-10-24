////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteCollisionCircle.h
//
//  Circular collision class for sprites
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

#ifndef MCK_SPRITE_COL_CIRCLE_H
#define MCK_SPRITE_COL_CIRCLE_H

#include "SpriteCollisionBase.h"

namespace MCK
{

class SpriteCollisionCircle : public SpriteCollisionBase
{
    public:

        //! Default constructor
        SpriteCollisionCircle( void )
        {
            this->type = MCK::SpriteCollisionType::CIRCLE;
            this->radius = 0.0f;
            this->center_x_offset = 0.0f;
            this->center_y_offset = 0.0f;
        }
        
        //! Setting radius and offsets
        virtual void set_vals(
            float _radius,
            float _center_x_offset,
            float _center_y_offset
        ) noexcept
        {
            this->radius = std::max( 0.0f, radius );
            this->center_x_offset = _center_x_offset;
            this->center_y_offset = _center_y_offset;
        }

        virtual ~SpriteCollisionCircle( void ) {}
  
        //! Check for collision
        virtual void check_all_collisions(
            std::vector<MCK::CollisionEvent> &collisions
        ) const
        {
            //TODO
        }


    protected:

        // Make this class non-abstract
        virtual void dummy( void ) {};

        float center_x_offset;
        float center_y_offset;

        float radius;
};

}  // End of namespace MCK

#endif