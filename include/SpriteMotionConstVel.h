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

// DEBUG
#include <cassert>

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
            this->motion_type = MCK::SpriteMotionType::CONST_VEL;
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
            this->prev_pos = this->pos;
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

        static void elastic_collision_circ(
            std::shared_ptr<MCK::SpriteMotionConstVel> sprite_A,
            std::shared_ptr<MCK::SpriteMotionConstVel> sprite_B,
            float mass_A = 1.0f,
            float mass_B = 1.0f,
            float radius_A = 0.0f,
            float radius_B = 0.0f,
            const MCK::Vect2D<float>* alt_center_A = NULL,
            const MCK::Vect2D<float>* alt_center_B = NULL
        )
        {
            // Ignore if either pointer NULL.
            if( sprite_A.get() == 0 || sprite_B.get() == 0 )
            {
                return;
            }

            const MCK::Vect2D<float> V1 = sprite_A->vel.as_Vect2D<float>();
            const MCK::Vect2D<float> V2 = sprite_B->get_vel().as_Vect2D<float>();
            const MCK::Vect2D<float> X1
                    = alt_center_A == NULL ?
                        sprite_A->pos.as_Vect2D<float>() :
                        *alt_center_A;

            const MCK::Vect2D<float> X2
                    = alt_center_B == NULL ?
                        sprite_B->pos.as_Vect2D<float>() :
                        *alt_center_B;
            
            // Floor MASS_SUM and POS_DIFF_SQ
            // at safe value for a denominator 
            const float MASS_SUM
                = std::max( 0.00000001f, mass_A + mass_B );
            const float POS_DIFF_SQ
                = std::max( 0.00000001f, ( X1 - X2 ).mag_sq() );
            const float RAD_TOTAL = radius_A + radius_B; 

            sprite_A->vel -= MCK::Point<float>( 
                            ( X1 - X2 )
                            * 2 * mass_B
                                / MASS_SUM
                            * MCK::Vect2D<float>::dot_prod( V1 - V2, X1 - X2 )
                            / POS_DIFF_SQ
                        );

            sprite_B->vel -= MCK::Point<float>( 
                            ( X2 - X1 )
                            * 2 * mass_A
                                / MASS_SUM
                            * MCK::Vect2D<float>::dot_prod( V2 - V1, X2 - X1 )
                            / POS_DIFF_SQ
                        );
            
            // Check for overlap
            if( POS_DIFF_SQ < RAD_TOTAL * RAD_TOTAL )
            {
                MCK::Point<float> diff( ( X1 - X2 ) );
                diff *= ( RAD_TOTAL - sqrt( POS_DIFF_SQ ) )
                            / sqrt( POS_DIFF_SQ ) / 2.0f;
                sprite_A->adjust_pos( diff );
                sprite_B->adjust_pos( diff * -1.0f );
            }
        }

        static void elastic_collision_rect(
            std::shared_ptr<MCK::SpriteMotionConstVel> sprite_A,
            std::shared_ptr<MCK::SpriteMotionConstVel> sprite_B,
            float mass_A = 1.0f,
            float mass_B = 1.0f,
            float width_A = 0.0f,
            float height_A = 0.0f,
            float width_B = 0.0f,
            float height_B = 0.0f
        )
        {
            // Ignore if either pointer NULL.
            if( sprite_A.get() == NULL || sprite_B.get() == NULL )
            {
                return;
            }

            // Get centre offset
            const MCK::Vect2D<float> CENTER_OFFSET_A(
                width_A / 2.0f,
                height_A / 2.0f
            );
            const MCK::Vect2D<float> CENTER_OFFSET_B(
                width_B / 2.0f,
                height_B / 2.0f
            );

            // Get current centre of rectangles
            const MCK::Vect2D<float> CENTER_A
                = sprite_A->get_pos().as_Vect2D<float>()
                    + CENTER_OFFSET_A; 
            const MCK::Vect2D<float> CENTER_B
                = sprite_B->get_pos().as_Vect2D<float>()
                    + CENTER_OFFSET_B; 

            // Get previous centre of rectangles
            const MCK::Vect2D<float> PREV_CENTER_A
                = sprite_A->get_prev_pos().as_Vect2D<float>() 
                    + CENTER_OFFSET_A; 
            const MCK::Vect2D<float> PREV_CENTER_B
                = sprite_B->get_prev_pos().as_Vect2D<float>() 
                    + CENTER_OFFSET_B; 

            // Calculate max centre-to-centre non-overlapping dist
            const float MAX_OVERLAP_X = ( width_A + width_B ) / 2.0f;;
            const float MAX_OVERLAP_Y = ( height_A + height_B ) / 2.0f;

            // Calculate previous overlap (if any)
            bool PREV_OVERLAP_X =  
                fabs( PREV_CENTER_A.get_x() - PREV_CENTER_B.get_x() )
                    - MAX_OVERLAP_X
                        < 0.0f;
            bool PREV_OVERLAP_Y = 
                fabs( PREV_CENTER_A.get_y() - PREV_CENTER_B.get_y() )
                    - MAX_OVERLAP_Y
                        < 0.0f;

            // Calculate current overlap (if any)
            bool OVERLAP_X = 
                fabs( CENTER_A.get_x() - CENTER_B.get_x() )
                    - MAX_OVERLAP_X
                        < 0.0f;
            bool OVERLAP_Y = 
                fabs( CENTER_A.get_y() - CENTER_B.get_y() )
                    - MAX_OVERLAP_Y
                        < 0.0f;

            // If new X overlap, change x velocities
            if( OVERLAP_X && !PREV_OVERLAP_X )
            {
                const float OLD_VEL_A = sprite_A->vel.get_x(); 
                const float OLD_VEL_B = sprite_B->vel.get_x();

                sprite_A->vel.set_x(
                    (
                        ( mass_A - mass_B ) * OLD_VEL_A
                        + 2 * mass_B * OLD_VEL_B
                    ) / ( mass_A + mass_B )
                );

                sprite_B->vel.set_x(
                    (
                        ( mass_B - mass_A ) * OLD_VEL_B
                        + 2 * mass_A * OLD_VEL_A
                    ) / ( mass_A + mass_B )
                );
            }

            // If new Y overlap, change y velocities
            if( OVERLAP_Y && !PREV_OVERLAP_Y )
            {
                const float OLD_VEL_A = sprite_A->vel.get_y(); 
                const float OLD_VEL_B = sprite_B->vel.get_y();

                sprite_A->vel.set_y(
                    (
                        ( mass_A - mass_B ) * OLD_VEL_A
                        + 2 * mass_B * OLD_VEL_B
                    ) / ( mass_A + mass_B )
                );

                sprite_B->vel.set_y(
                    (
                        ( mass_B - mass_A ) * OLD_VEL_B
                        + 2 * mass_A * OLD_VEL_A
                    ) / ( mass_A + mass_B )
                );
            }
        }
         
        const Point<float>& get_prev_pos( void ) const noexcept
        {
            return this->prev_pos;
        }


    protected:

        MCK::Point<float> vel;

        MCK::Point<float> prev_pos;
};

}  // End of namespace MCK

#endif
