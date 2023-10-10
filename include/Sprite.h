////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  Sprite.h
//
//  Class for sprites
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

template<class RENDER>
class Sprite : public RENDER
{
    public:

        Sprite(
            std::shared_ptr<MCK::SpriteAnimBase> _anim,
            std::shared_ptr<MCK::SpriteMotionBase> _motion,
            std::shared_ptr<MCK::SpriteCollisionBase> _collision,
            uint32_t z = MCK::DEFAULT_Z_VALUE )
            : anim( _anim ),
              motion( _motion ),
              collision( _collision ),
              RENDER( z )
        {}

        virtual ~Sprite( void ) {}

        //! Returns true if all sprite components that are present are initialized
        bool is_initialized( void )
        {
            return ( anim.get() == NULL
                        || anim->is_initialized()
                   ) && ( motion.get() == NULL
                        || motion->is_initialized()
                   ) && ( collision.get() == NULL
                        || collision->is_initialized()
                   );
        }

        //! Process sprite, at current ticks
        void process( void )
        {
            // Set sprite position
            if( this->motion.get() != NULL )
            {
                try
                {
                    this->motion->set_pos(
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
            }

            // Set sprite appearance
            if( this->anim.get() != NULL )
            {
                try
                {
                    this->anim->set_appearance(
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
        }


    protected:

        std::shared_ptr<MCK::SpriteAnimBase> anim;
        std::shared_ptr<MCK::SpriteMotionBase> motion;
        std::shared_ptr<MCK::SpriteCollisionBase> collision;
};

}  // End of namespace MCK

#endif
