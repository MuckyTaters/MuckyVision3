////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpritePos.h
//
//  Virtual base class for *all* sprites
//
//  Note: this class forms the lowest point
//        of a diamond shaped inheritance
//        structure, and is inherited
//        separately by motion, animation
//        and collision components.
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

#ifndef MCK_SPRITE_POS_H
#define MCK_SPRITE_POS_H

#include "Defs.h"
#include "Point.h"
#include "GameEngRenderBase.h"
#include "GameEng.h"
#include "ImageMan.h"

namespace MCK
{

class SpritePos
{
    public:

        //! Default constructor
        SpritePos( void )
        {
            this->update_render_instance = false;
            this->motion_type = MCK::SpriteMotionType::STATIONARY;
            this->anim_type = MCK::SpriteAnimType::STATIC;
            this->collision_type = MCK::SpriteCollisionType::NONE;
        }

        //! Constructor
        SpritePos(
            MCK::Point<float> _pos,
            std::shared_ptr<GameEngRenderBase> _render_instance
        ) : pos( _pos ),
            render_instance( _render_instance )
        {
            SpritePos();
        }

        virtual ~SpritePos( void ) {}

        //! Get type of sprite's motion component
        MCK::SpriteMotionType get_motion_type( void ) const noexcept
        {
            return this->motion_type;
        }

        //! Get type of sprite's animation component
        MCK::SpriteAnimType get_anim_type( void ) const noexcept
        {
            return this->anim_type;
        }

        //! Get type of sprite's collision component
        MCK::SpriteCollisionType get_collision_type( void ) const noexcept
        {
            return this->collision_type;
        }

        //! Sets two important static pointers
        /*! Throws if either supplied pointer is NULL.*/
        static void set_ptrs(
            const MCK::GameEng* _game_eng,
            const MCK::ImageMan* _image_man
        )
        {
            if( _game_eng == NULL )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    "GameEng pointer supplied to SpritePos is NULL."
#else
                    ""
#endif
                ) );
            }

            MCK::SpritePos::game_eng = _game_eng;
            
            if( _image_man == NULL )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    "ImageMan pointer supplied to SpritePos is NULL."
#else
                    ""
#endif
                ) );
            }

            MCK::SpritePos::image_man = _image_man;
        }

        //! Get position of sprite, relative to parent block
        const MCK::Point<float>& get_pos( void ) const noexcept
        {
            return this->pos;
        }

        //! Get render info or render block associated with sprite
        std::shared_ptr<GameEngRenderBase> get_render_instance( void ) const noexcept
        {
            return this->render_instance;
        }

        //! Set position of sprite, relative to parent block
        void set_pos( MCK::Point<float> _pos ) noexcept
        {
            this->pos = _pos;
        }

        //! Adjust position of sprite, relative to current position
        void adjust_pos( MCK::Point<float> diff ) noexcept
        {
            this->pos += diff;
        }

        //! Set render info or render block associated with sprite
        /* Note: Supplying a NULL pointer is acceptable, but means
         *       the sprite will not be rendered at present.
         */
        void set_render_instance(
            std::shared_ptr<GameEngRenderBase> _render_instance
        ) noexcept
        {
            this->render_instance = _render_instance;

            // Only update render instance if non-NULL
            this->update_render_instance
                = this->render_instance.get() != NULL;
        }

        //! Determines whether render instance is updated with sprite pos
        /*! Note: if 'render_instance' is NULL, 'val' = true will
         *        have no effect
         */
        void set_update_render_instance( bool val ) noexcept
        {
            // Only set 'update_render_instance' to true
            // if 'val' is true AND render_instance is non-NULL
            this->update_render_instance
                = val && this->render_instance != NULL;
        }

        //! Returns true if render instance is being updated with sprite pos
        bool get_update_render_instance( void ) const noexcept
        {
            return this->update_render_instance;
        }

        //! Set time for *all* sprite instances, in ticks
        static void set_ticks(
            uint32_t new_ticks,
            bool set_prev_ticks_same_as_current = false
        ) noexcept 
        {
            if( set_prev_ticks_same_as_current )
            {
                MCK::SpritePos::prev_ticks
                    = MCK::SpritePos::current_ticks
                        = new_ticks;
            }
            else
            {
                MCK::SpritePos::prev_ticks
                    = MCK::SpritePos::current_ticks;
                MCK::SpritePos::current_ticks = new_ticks;
            }

            MCK::SpritePos::ticks_elapsed
                = MCK::SpritePos::current_ticks 
                    - MCK::SpritePos::prev_ticks; 
        }

        uint32_t get_current_ticks( void ) const noexcept
        {
            return this->current_ticks;
        }


    protected:

        MCK::Point<float> pos;

        std::shared_ptr<GameEngRenderBase> render_instance;

        bool update_render_instance;

        // Time values for *all* sprite instances
        static uint32_t current_ticks;
        static uint32_t prev_ticks;
        static uint32_t ticks_elapsed;

        static const GameEng* game_eng;
        static const ImageMan* image_man;

        MCK::SpriteMotionType motion_type;
        MCK::SpriteAnimType anim_type;
        MCK::SpriteCollisionType collision_type;
};

}  // End of namespace MCK

#endif
