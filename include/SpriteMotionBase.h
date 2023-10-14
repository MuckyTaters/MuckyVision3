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
#include "GameEngRenderBase.h"

namespace MCK
{

class SpriteMotionBase
{
    public:

        SpriteMotionBase( void )
        {
            this->type = MCK::SpriteMotionType::STATIONARY;
        }

        virtual ~SpriteMotionBase( void ) {}

        //! Set position of sprite
        /*! Note: base version does nothing, i.e. leaves x and y unchanged.*/
        virtual void set_pos(
            MCK::GameEngRenderBase &render_instance
        ) const noexcept {}

        //! Set time for all instances, prior to setting position of individual instances
        static void set_ticks(
            uint32_t new_ticks,
            bool set_prev_ticks_same_as_current = false
        ) noexcept 
        {
            if( set_prev_ticks_same_as_current )
            {
                MCK::SpriteMotionBase::prev_ticks
                    = MCK::SpriteMotionBase::current_ticks
                        = new_ticks;
            }
            else
            {
                MCK::SpriteMotionBase::prev_ticks
                    = MCK::SpriteMotionBase::current_ticks;
                MCK::SpriteMotionBase::current_ticks = new_ticks;
            }

            MCK::SpriteMotionBase::ticks_elapsed
                = MCK::SpriteMotionBase::current_ticks 
                    - MCK::SpriteMotionBase::prev_ticks; 
        }

        //! Returns true if instance is initialized
        /*! Note: Base instance is initialized at construction.*/
        virtual bool is_initialized( void ) const noexcept
        {
            return true;
        }

        

    protected:

        static uint32_t current_ticks;
        static uint32_t prev_ticks;
        static uint32_t ticks_elapsed;

        MCK::SpriteMotionType type;
};

}  // End of namespace MCK

#endif
