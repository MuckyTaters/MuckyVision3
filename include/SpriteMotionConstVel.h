////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteMotionConstVel.h
//
//  Constant velocity motion class for sprites
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

#ifndef MCK_SPRITE_MTN_CST_VEL_H
#define MCK_SPRITE_MTN_CST_VEL_H

#include "Defs.h"
#include "SpriteMotionBase.h"

namespace MCK
{

class SpriteMotionConstVel : public SpriteMotionBase
{
    public:

        //! Default constructor
        SpriteMotionConstVel( void )
        {
            this->initialized = false;
            this->x = this->y = this->dx = this->dy = 0;
            this->type = MCK::SpriteMotionType::CONST_VEL;
        }

        //! Constructor
        /*! @param _x: Starting x coord (in pixels)
         *  @param _y: Starting y coord (in pixels)
         *  @param _dx: Velocity in x (in pixels per tick)
         *  @param _dy: Velocity in y (in pixels per tick)
         */
        SpriteMotionConstVel(
            float _x,
            float _y,
            float _dx,
            float _dy
        )
        : x( _x ), y( _y ), dx( _dx ), dy( _dy )
        {
            this->initialized = true;
            this->type = MCK::SpriteMotionType::CONST_VEL;
        }

        virtual ~SpriteMotionConstVel( void ) {}

        //! Set starting position and velocity of sprite
        virtual void initialize(
            float _x,
            float _y,
            float _dx,
            float _dy
        ) noexcept
        {
            this->x = _x;
            this->y = _y;
            this->dx = _dx;
            this->dy = _dy;
            this->initialized = true;
        }

        virtual bool is_initialized( void ) const noexcept
        {
            return this->initialized;
        }

        //! Set position of sprite
        virtual void set_pos(
            MCK::GameEngRenderBase &render_instance
        ) noexcept
        {
            this->x += this->dx * MCK::SpriteMotionBase::ticks_elapsed;
            this->y += this->dy * MCK::SpriteMotionBase::ticks_elapsed;
            render_instance.set_pos(
                int( this->x + 0.5f ),
                int( this->y + 0.5f )
            ); 
        }


    protected:

        bool initialized;

        float x, y, dx, dy;
};

}  // End of namespace MCK

#endif
