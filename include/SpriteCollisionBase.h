////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteCollisionBase.h
//
//  Base collision class for sprites
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

#ifndef MCK_SPRITE_COL_BASE_H
#define MCK_SPRITE_COL_BASE_H

#include "Defs.h"
#include "CollisionEvent.h"
#include "SpritePos.h"

namespace MCK
{

class SpriteCollisionBase : virtual public SpritePos
{
    public:

        SpriteCollisionBase( void ) : SpritePos()
        {
            this->collision_type = MCK::SpriteCollisionType::NONE;
        }
        
        virtual ~SpriteCollisionBase( void ) {}
   
        
        // //! Check for collision
        // /*! Base version of this method does nothing */
        // virtual void check_all_collisions( 
        //     std::vector<MCK::CollisionEvent> &collisions
        // ) const {}

        //! Get rectangular bounds
        /*! Base version of this method returns zeros */
        virtual void get_bounds(
            float& left,
            float& top,
            float& right,
            float& bottom
        ) const noexcept
        {
            left = top = right = bottom = 0.0f;
        }

        //! Updated bounds (in base class this does nothing)
        virtual void update_bounds( void ) const noexcept {}

        //! Get width
        /*! Base version of this method returns zero*/
        virtual float get_width( void ) const noexcept
        {
            return 0.0f;
        }

        //! Get height
        /*! Base version of this method returns zero*/
        virtual float get_height( void ) const noexcept
        {
            return 0.0f;
        }

        //! Get half-width
        /*! Base version of this method returns zero*/
        virtual float get_half_width( void ) const noexcept
        {
            return 0.0f;
        }

        //! Get half-height
        /*! Base version of this method returns zero*/
        virtual float get_half_height( void ) const noexcept
        {
            return 0.0f;
        }

        //! Get center x coord
        /*! Base version of this method returns zero */
        virtual float get_center_x( void ) const noexcept
        {
            return 0.0f;
        }

        //! Get center y coord
        /*! Base version of this method returns zero */
        virtual float get_center_y( void ) const noexcept
        {
            return 0.0f;
        }

        //! Get node in collision quad tree containing this sprite
        /*! In base version, this always returns NULL */
        virtual void* get_quad_tree_node( void ) const noexcept
        {
            return NULL;
        }
        
        //! Set void pointer to sprite's current node in collision tree
        /*! In base version, this does nothing */
        virtual void set_quad_tree_node( void* val ) noexcept {}
};

}  // End of namespace MCK

#endif
