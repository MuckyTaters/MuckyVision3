////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteCollisionCircle.h
//
//  Circular collision class for sprites
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

#ifndef MCK_SPRITE_COL_CIRCLE_H
#define MCK_SPRITE_COL_CIRCLE_H

#include "SpriteCollisionBase.h"

namespace MCK
{

class SpriteCollisionCircle : public SpriteCollisionBase
{
    public:

        //! Default constructor
        SpriteCollisionCircle( void )
        {
            this->collision_type = MCK::SpriteCollisionType::CIRCLE;
            this->radius = 0.0f;
            this->center_x_offset = 0.0f;
            this->center_y_offset = 0.0f;
            this->center_x_offset = 0.0f;
            this->center_y_offset = 0.0f;
        }
        
        //! Setting radius and offsets
        virtual void set_vals(
            float _radius,
            float _center_x_offset,
            float _center_y_offset
        ) noexcept
        {
            this->center_x_offset = _center_x_offset;
            this->center_y_offset = _center_y_offset;
            this->radius = std::max( 0.0f, _radius );
            this->update_bounds();
            /*
            this->left_bound = this->SpritePos::pos.get_x() - this->center_x_offset;
            this->top_bound = this->SpritePos::pos.get_y() - this->center_y_offset;
            this->width = this->height = _radius * 2.0f;
            this->right_bound = this->left_bound + this->width;
            this->bottom_bound = this->top_bound + this->height;
            */
        }

        virtual ~SpriteCollisionCircle( void ) {}
  
        /*
        //! For circle this method does nothing
        virtual void set_width_and_height(
            float _width,
            float _height
        )
        {}
        */

        virtual void update_bounds( void ) noexcept
        {
            /*
            this->left_bound = this->SpritePos::pos.get_x();
            this->top_bound = this->SpritePos::pos.get_y();
            this->right_bound = this->left_bound + this->width;
            this->bottom_bound = this->top_bound + this->height;
            */
            this->center_x = this->SpritePos::pos.get_x() 
                                + this->center_x_offset;
            this->center_y = this->SpritePos::pos.get_y()
                                + this->center_y_offset;
        }

        float get_dist_sq_from_center( float x, float y ) const noexcept
        {
            return 
                ( this->center_x - x ) * ( this->center_x - x )
                + ( this->center_y - y ) * ( this->center_y - y );
        }

        float get_radius( void ) const noexcept
        {
            return this->radius;
        }

        float get_center_x_offset( void ) const noexcept
        {
            return center_x_offset;
        }

        float get_center_y_offset( void ) const noexcept
        {
            return center_y_offset;
        }

        //! Get rectangular bounds
        virtual void get_bounds(
            float& left,
            float& top,
            float& right,
            float& bottom
        ) const noexcept
        {
            left = this->center_x - this->center_x_offset;
            top = this->center_y - this->center_y_offset;
            right = this->center_x + this->center_x_offset;
            bottom = this->center_y + this->center_y_offset;
        }
        
        //! Get center x coord
        /*! Base version of this method returns zeros */
        virtual float get_center_x( void ) const noexcept
        {
            return this->center_x;
        }

        //! Get center y coord
        /*! Base version of this method returns zeros */
        virtual float get_center_y( void ) const noexcept
        {
            return this->center_y;
        }

        //! Get void pointer to sprite's current node in collision tree
        virtual void* get_quad_tree_node( void ) const noexcept
        {
            return this->quad_tree_node;
        }

        //! Set void pointer to sprite's current node in collision tree
        virtual void set_quad_tree_node( void* val ) noexcept
        {
            this->quad_tree_node = val;
        }

        //! Get width
        virtual float get_width( void ) const noexcept
        {
            return this->radius * 2.0f;
        }

        //! Get height
        virtual float get_height( void ) const noexcept
        {
            return this->radius * 2.0f;
        }

        //! Get half width
        virtual float get_half_width( void ) const noexcept
        {
            return this->radius;
        }

        //! Get half height
        virtual float get_half_height( void ) const noexcept
        {
            return this->radius;
        }


    protected:

        float center_x_offset;
        float center_y_offset;

        float center_x;
        float center_y;

        float radius;
        
        // Pointer to node in quad-tree that currently
        // contains this sprite
        void* quad_tree_node;
};

}  // End of namespace MCK

#endif
