////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  Sprite.h
//
//  Class for sprites
//
//  Note: all sprites must be constructed
//        via the GameEngSpriteFactory instance.
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

#ifndef MCK_SPRITE_H
#define MCK_SPRITE_H

#include <memory>  // For shared pointer
#include "SpriteAnimBase.h"
#include "SpriteMotionBase.h"
#include "SpriteCollisionBase.h"

namespace MCK
{

// Forward declaration for friendship
class GameEng;
template<class MOTION, class ANIM, class COLL, class RENDER>
class GameEngSpriteFactory;

// Class inherents from:
// MOTION: sprite motion class
// ANIM: sprite (appearance) animation class
// COLL: sprite collision class
// RENDER: GameEngRenderInfo or GameEngRenderBlock
template<class MOTION, class ANIM, class COLL, class RENDER>
class Sprite : public MOTION, public ANIM, public COLL, public RENDER
{
    public:
    
        // Only GameEngSpriteFactory instance can set protected/private members
        friend class MCK::GameEngSpriteFactory<MOTION,ANIM,COLL,RENDER>;

        //! (Default) constructor
        Sprite( uint32_t z = MCK::DEFAULT_Z_VALUE )
        : RENDER( z )
        {}

        //! Destructor
        virtual ~Sprite( void ) {}

        //! Returns true if all sprite components that are present are initialized
        bool is_initialized( void )
        {
            return ( this->ANIM::is_initialized()
                     && this->MOTION::is_initialized()
                     && this->COLL::is_initialized()
                   );
        }

        //! Process sprite, at current ticks
        void process( void )
        {
            // Set sprite position
            try
            {
                this->MOTION::set_pos(
                    // Downcast by reference to avoid
                    // a temporary copy being made
                    static_cast<GameEngRenderBase&>( *this )
                );
            }
            catch( std::exception &e )
            {
                std::cout << "Failed to set sprite position, error: "
                          << e.what() << std::endl;
            }

            // Set sprite appearance
            try
            {
                this->ANIM::set_appearance(
                    // Downcast by reference to avoid
                    // a temporary copy being made
                    static_cast<GameEngRenderBase&>( *this )
                );
            }
            catch( std::exception &e )
            {
                std::cout << "Failed to set sprite appearance, error: "
                          << e.what() << std::endl;
            }
        }


    protected:

};

}  // End of namespace MCK

#endif
