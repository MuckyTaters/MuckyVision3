////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteCollisionRect.h
//
//  Rectangular collision class for sprites,
//  and base class for other types of sprite
//  collision class.
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

#ifndef MCK_SPRITE_COL_RECT_H
#define MCK_SPRITE_COL_RECT_H

#include "SpriteCollisionBase.h"

namespace MCK
{

class SpriteCollisionRect : public SpriteCollisionBase
{
    public:

        //! Default constructor
        SpriteCollisionRect( void )
        {
            this->collision_type = MCK::SpriteCollisionType::RECT;
            this->left_bound = 0;
            this->top_bound = 0;
            this->right_bound = 0;
            this->bottom_bound = 0;
            this->width = 0;
            this->height = 0;
            this->quad_tree_node = NULL;
        }

        virtual void update_bounds( void ) noexcept
        {
            this->left_bound = this->SpritePos::pos.get_x();
            this->top_bound = this->SpritePos::pos.get_y();
            this->right_bound = this->left_bound + this->width;
            this->bottom_bound = this->top_bound + this->height;
        }
        
        //! Setting width and height
        virtual void set_width_and_height(
            float _width,
            float _height
        )
        {
            this->width = _width;
            this->height = _height;
        }

        //! Get width
        virtual float get_width( void ) const noexcept
        {
            return this->width;
        }

        //! Get height
        virtual float get_height( void ) const noexcept
        {
            return this->height;
        }

        //! Get half width
        virtual float get_half_width( void ) const noexcept
        {
            return this->width / 2.0f;
        }

        //! Get half height
        virtual float get_half_height( void ) const noexcept
        {
            return this->height / 2.0f;
        }

        //! Get rectangular bounds
        virtual void get_bounds(
            float& left,
            float& top,
            float& right,
            float& bottom
        ) const noexcept
        {
            left = this->left_bound;
            top = this->top_bound;
            right = this->right_bound;
            bottom = this->bottom_bound;
        }

        //! Get center x coord
        /*! Base version of this method returns zeros */
        virtual float get_center_x( void ) const noexcept
        {
            return this->left_bound + this->width / 2.0f;
        }

        //! Get center y coord
        /*! Base version of this method returns zeros */
        virtual float get_center_y( void ) const noexcept
        {
            return this->top_bound + this->height / 2.0f;
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

        /*
        //! Get left bound
        float get_left_bound( void ) const noexcept
        {
            return left_bound;
        }

        //! Get top bound
        float get_top_bound( void ) const noexcept
        {
            return top_bound;
        }

        //! Get right bound
        float get_right_bound( void ) const noexcept
        {
            return right_bound;
        }

        //! Get bottom bound
        float get_bottom_bound( void ) const noexcept
        {
            return bottom_bound;
        }
        */


    protected:

        // Make this class non-abstract
        virtual void dummy( void ) {};

        float width;
        float height;

        float left_bound;
        float top_bound;
        float right_bound;
        float bottom_bound;

        // Pointer to node in quad-tree that currently
        // contains this sprite
        void* quad_tree_node;
};

}  // End of namespace MCK

#endif
