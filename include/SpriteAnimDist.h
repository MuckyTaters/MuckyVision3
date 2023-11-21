////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteAnimDist.h
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

#ifndef MCK_SPRITE_ANIM_DIST_H
#define MCK_SPRITE_ANIM_DIST_H

#include "Defs.h"
#include "SpriteAnimTime.h"

namespace MCK
{

class SpriteAnimDist : public SpriteAnimTime
{
    public:

        SpriteAnimDist( void ) : SpriteAnimTime()
        {
            this->anim_type = MCK::SpriteAnimType::DISTANCE;
            this->count_hoz_pixels = true;
            this->count_vert_pixels = true;
            this->prev_x = this->MCK::SpritePos::pos.get_x();
            this->prev_y = this->MCK::SpritePos::pos.get_y();
        }

        //! Frame changes only caused by vertical component of movement
        void count_vert_pixels_only( void ) noexcept
        {
            this->count_hoz_pixels = false;
            this->count_vert_pixels = true;
        }

        //! Frame changes only caused by horizontal component of movement
        void count_hoz_pixels_only( void ) noexcept
        {
            this->count_hoz_pixels = true;
            this->count_vert_pixels = false;
        }

        //! Frame changes caused by movement in any direction
        void count_all_pixels( void ) noexcept
        {
            this->count_hoz_pixels = true;
            this->count_vert_pixels = true;
        }

        //! If true, only horizontal component of movement causes frame changes
        //
        bool hoz_pixels_counted( void ) const noexcept
        {
            return this->count_hoz_pixels;
        }

        //! If true, only vertical component of movement causes frame changes
        bool vert_pixels_counted( void ) const noexcept
        {
            return this->count_vert_pixels;
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
            // Get pixels moved
            float pixels;
            if( count_hoz_pixels && !count_vert_pixels )
            {
                pixels = fabs( this->MCK::SpritePos::pos.get_x()
                            - this->prev_x );
            }
            else if( !count_hoz_pixels && count_vert_pixels )
            {
                pixels = fabs( this->MCK::SpritePos::pos.get_y()
                            - this->prev_y );
            }
            else
            {
                pixels = sqrt( 
                            MCK::pow2( this->MCK::SpritePos::pos.get_x() - this->prev_x )
                            + MCK::pow2( this->MCK::SpritePos::pos.get_y() - this->prev_y )
                        );
            }
            
            // Update previous position
            this->prev_x = this->MCK::SpritePos::pos.get_x();
            this->prev_y = this->MCK::SpritePos::pos.get_y();

            // Change frame until pixels used up
            while( pixels > this->dist_to_next_frame )
            {
                pixels -= this->dist_to_next_frame;

                std::cout << "Select frame " << this->frame_num + 1 
                          << std::endl;

                this->select_frame( this->frame_num + 1 );
                
                // Safety check, to prevent infinite loop
                if( this->dist_to_next_frame == 0 ) { break; }
            }
            this->dist_to_next_frame -= pixels;
        }

        virtual void select_frame( size_t _frame_num )
        {
            MCK::SpriteAnimTime::select_frame( _frame_num );
            
            // Set ticks/distance/etc. for next frame
            this->dist_to_next_frame
                = this->frames[ this->frame_num ].duration; 
        }


    protected:

        bool count_vert_pixels;
        bool count_hoz_pixels;

        float prev_x;
        float prev_y;
        
        float dist_to_next_frame;
};

}  // End of namespace MCK

#endif
