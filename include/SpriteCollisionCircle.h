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

#ifndef MCK_SPRITE_COL_ALT_CIRCLE_H
#define MCK_SPRITE_COL_ALT_CIRCLE_H

#include "GeoNamespace.h"
#include "SpriteCollisionBase.h"

namespace MCK
{

class SpriteCollisionCircle : public SpriteCollisionBase, public GEO::Circle<float>
{
    public:

        //! Default constructor
        SpriteCollisionCircle( void ) : SpriteCollisionBase(), GEO::Circle<float>()
        {
            this->collision_type = MCK::SpriteCollisionType::CIRCLE;
        }
        
        //! Setting radius and offsets
        virtual void set_vals(
            float _radius,
            float _center_x_offset,
            float _center_y_offset
        ) noexcept
        {
            this->radius = std::max( 0.0f, _radius );
            this->set_center_offset(
                this->SpritePos::pos.as_Vect2D(),
                MCK::Vect2D<float>(
                    _center_x_offset,
                    _center_y_offset
                )
            );
            this->update_bounds();
        }

        virtual ~SpriteCollisionCircle( void ) {}

        virtual void update_bounds( void ) noexcept
        {
            this->change_pos(
                this->SpritePos::pos.as_Vect2D()
            );
        }

        float get_dist_sq_from_center( float x, float y ) const noexcept
        {
            return MCK::pow2<float>(
                        this->center.get_x() - x 
                   ) + MCK::pow2<float>(
                        this->center.get_y() - y
                   );  
        }

        //! Returns true if specified point lies within circle
        bool contains( float x, float y ) const noexcept
        {
            return this->dist_sq_from_center(
                            MCK::Vect2D<float>( x, y )
                         ) < this->radius_sq;
        }

        //! Returns true if specified point lies within circle
        bool contains( MCK::Vect2D<float> point ) const noexcept
        {
            return this->dist_sq_from_center( point )
                           < this->radius_sq;
        }

        float get_center_x_offset( void ) const noexcept
        {
            return this->center_offset.get_x();
        }

        float get_center_y_offset( void ) const noexcept
        {
            return this->center_offset.get_y();
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
        virtual float get_center_x( void ) const noexcept
        {
            return this->center.get_x();
        }

        //! Get center y coord
        virtual float get_center_y( void ) const noexcept
        {
            return this->center.get_y();
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

        // Pointer to node in quad-tree that currently
        // contains this sprite
        void* quad_tree_node;
};

}  // End of namespace MCK

#endif
