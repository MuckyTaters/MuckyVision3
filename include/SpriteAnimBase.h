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
#include "SpritePos.h"

namespace MCK
{

class SpriteAnimBase : virtual public SpritePos
{
    public:

        SpriteAnimBase( void ) : SpritePos()
        {
            this->anim_type = MCK::SpriteAnimType::STATIC;
        }

        virtual ~SpriteAnimBase( void ) {}
    
        //! Set appearance of sprite (e.g texture, orientation)
        /*! Note: base version does nothing, i.e. leaves appearance as is.*/
        virtual void calc_frame( void ) {}

        /*
        MCK::SpriteAnimType get_type( void ) const noexcept
        {
            return this->MCK::SpriteAnimBase::type;
        }
        */

    protected:

        // MCK::SpriteAnimType type;
};

}  // End of namespace MCK

#endif
