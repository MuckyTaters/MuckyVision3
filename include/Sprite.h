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
#include "SpriteAnimTime.h"
#include "SpriteMotionConstVel.h"
#include "SpriteCollisionCircle.h"
#include "ImageMan.h"

namespace MCK
{

// This class inherents from:
// MOTION: sprite motion class
// ANIM: sprite (appearance) animation class
// COLL: sprite collision class
template<class MOTION, class ANIM, class COLL>
class Sprite : public MOTION, public ANIM, public COLL
{
    public:

        //! (Default) constructor
        Sprite( void )
        {
            this->initialized = false;
        }

        //! Destructor
        virtual ~Sprite( void ) {}

        void init(
            // ImageMan &image_man,
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
            MCK_IMG_ID_TYPE initial_image_id,
            MCK_PAL_ID_TYPE initial_local_palette_id,
            int initial_pos_x,
            int initial_pos_y,
            uint32_t initial_z, 
            uint16_t width_in_pixels,
            uint16_t height_in_pixels,
            bool make_as_block = false
        )
        {
            if( this->initialized )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    "Cannot init sprite as already init."
#else
                    ""
#endif
                ) );
            }

            if( this->MCK::SpritePos::image_man == NULL
                || !this->MCK::SpritePos::image_man->is_initialized()
            )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    "Cannot init sprite as ImageMan not yet init."
#else
                    ""
#endif
                ) );
            }

            this->SpritePos::pos = MCK::Point<float>(
                initial_pos_x,
                initial_pos_y
            );

            if( !make_as_block )
            {
                try
                {
                    this->SpritePos::set_render_instance(
                        this->MCK::SpritePos::image_man->create_render_info(
                            initial_image_id,
                            initial_local_palette_id,
                            initial_pos_x,
                            initial_pos_y,
                            width_in_pixels,
                            height_in_pixels,
                            parent_block        
                        )
                    );
                }
                catch( const std::runtime_error& e )
                {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
                    std::cout << "Sprite init failed, error = "
                              << e.what() << std::endl;
#endif
                }
            }
            else
            {
                //TODO
            }

            try
            {
                MCK::GameEng::change_z(
                    this->SpritePos::render_instance,
                    parent_block,
                    initial_z,
                    false  // Don't use current 'z' as hint
                );
            }
            catch( const std::runtime_error& e )
            {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
                std::cout << "Sprite init set 'z' failed, error = "
                          << e.what() << std::endl;
#endif
            }

            this->initialized = true;
        }

        //! Process sprite, at current ticks
        void process( void )
        {
            // Set sprite position
            try
            {
                this->MOTION::calc_pos();
            }
            catch( std::exception &e )
            {
                std::cout << "Failed to set sprite position, error: "
                          << e.what() << std::endl;
            }

            // Set sprite appearance
            try
            {
                this->ANIM::calc_frame();
            }
            catch( std::exception &e )
            {
                std::cout << "Failed to set sprite appearance, error: "
                          << e.what() << std::endl;
            }

            //    // TODO Process collisions
            if( this->get_collision_type() 
                    != MCK::SpriteCollisionType::NONE
            )
            {
                this->COLL::update_bounds(); 
            }
        }

        bool is_initialized( void ) const noexcept
        {
            return initialized;
        }

    protected:
 
        bool initialized;
};

}  // End of namespace MCK

#endif
