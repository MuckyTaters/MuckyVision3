////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteMotionConstVel.h
//
//  Constant velocity motion class for sprites
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
#include "Point.h"
#include "SpriteMotionBase.h"

namespace MCK
{

class SpriteMotionConstVel : public SpriteMotionBase
{
    public:

        //! Default constructor
        SpriteMotionConstVel( void ) : SpriteMotionBase()
        {
            this->type = MCK::SpriteMotionType::CONST_VEL;
        }

        //! Partial constructor
        SpriteMotionConstVel(
            MCK::Point<float> _pos,
            std::shared_ptr<GameEngRenderBase> _render_instance
        ) : SpriteMotionBase( _pos, _render_instance )
        {
            SpriteMotionConstVel();
        }

        virtual ~SpriteMotionConstVel( void ) {}

        //! Get velocity of sprite
        const MCK::Point<float>& get_vel( void ) const noexcept
        {
            return this->vel;
        }

        //! Set velocity of sprite
        void set_vel( MCK::Point<float> _vel ) noexcept
        {
            this->vel = _vel;
        }

        // Calculate position, based on time 
        virtual void calc_pos( void ) noexcept
        {
            this->pos += this->vel * float( MCK::SpritePos::ticks_elapsed );
           
            // Update render instance
            if( this->SpritePos::update_render_instance )
            {
                this->SpritePos::render_instance->set_x(
                    int( this->pos.get_x() + 0.5f )
                );
                this->SpritePos::render_instance->set_y(
                    int( this->pos.get_y() + 0.5f )
                );
            }
        }


    protected:

        MCK::Point<float> vel;
};

}  // End of namespace MCK

#endif
