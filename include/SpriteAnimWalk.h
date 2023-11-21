////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteAnimWalk.h
//
//  Distance animation class for sprite images. 
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

#ifndef MCK_SPRITE_ANIM_WALK_H
#define MCK_SPRITE_ANIM_WALK_H

#include "Defs.h"
#include "SpriteAnimDist.h"

namespace MCK
{

class SpriteAnimWalk : public SpriteAnimDist
{
    public:

        SpriteAnimWalk( void ) : SpriteAnimDist()
        {
            this->anim_type = MCK::SpriteAnimType::DISTANCE;
            this->count_hoz_pixels = true;
            this->count_vert_pixels = true;
            this->prev_x = this->MCK::SpritePos::get_pos().get_x();
            this->prev_y = this->MCK::SpritePos::get_pos().get_y();
        }

        void set_frames(
            std::vector<MCK::SpriteFrame> _frames,
            size_t starting_frame_num = 0,
            bool _use_offsets = false
        )
        {
            MCK::SpriteAnimTime::set_frames(
                _frames,
                starting_frame_num,
                _use_offsets
            );

            this->current_x = int( MCK::SpritePos::pos.get_x() + 0.5f );
            this->current_y = int( MCK::SpritePos::pos.get_y() + 0.5f );
        }

        //! Calculate next frame, baseed on current pixels moved
        /*! Note: Frames with 0 duration will be displayed
         *        for one frame only before proceeding to the
         *        next. This is to prevent an infinite loop
         *        occuring if all frame durations are set
         *        to zero.
         */
        virtual void calc_frame( void )
        {
            this->MCK::SpriteAnimDist::calc_frame();
        
            this->SpritePos::render_instance->set_pos(
                this->current_x,
                this->current_y
            );
        }

        virtual void select_frame( size_t _frame_num )
        {
            MCK::SpriteAnimDist::select_frame( _frame_num );
       
            this->current_x = 
                this->SpritePos::render_instance->get_x();

            this->current_y = 
                this->SpritePos::render_instance->get_y();
        }


    protected:

        int current_x;
        int current_y;
};

}  // End of namespace MCK

#endif

