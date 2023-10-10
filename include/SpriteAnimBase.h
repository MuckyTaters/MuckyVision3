////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteAnimBase.h
//
//  Base animation class for sprite images. 
//
//  This is non-abstract, as it can be
//  used to respresent sprites that 
//  have a static image, hence the
//  'STATIC' type designation.
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

#ifndef MCK_SPRITE_ANIM_BASE_H
#define MCK_SPRITE_ANIM_BASE_H

#include "Defs.h"
#include "GameEngRenderBase.h"

namespace MCK
{

class SpriteAnimBase
{
    public:

        SpriteAnimBase( void )
        {
            this->type = MCK::SpriteAnimType::STATIC;
        }

        virtual ~SpriteAnimBase( void ) {}
    
        //! Set appearance of sprite (e.g texture, orientation)
        /*! Note: base version does nothing, i.e. leaves appearance as is.*/
        virtual void set_appearance( 
            MCK::GameEngRenderBase &render_instance
        ) const noexcept {}

        //! Set time for all instances, prior to setting appearance of individual instances
        static void set_current_ticks( uint32_t _ticks ) noexcept
        {
            MCK::SpriteAnimBase::current_ticks = _ticks;
        }

        //! Returns true if instance is initialized
        /*! Note: Base instance is initialized at construction.*/
        virtual bool is_initialized( void ) const noexcept
        {
            return true;
        }

    protected:

        static uint32_t current_ticks;

        MCK::SpriteAnimType type;
};

}  // End of namespace MCK

#endif
