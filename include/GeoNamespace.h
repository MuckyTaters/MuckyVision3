////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  GeoNamespace.h
//
//  Classes and functions for geometric shapes.
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

#ifndef MCK_GEO_H
#define MCK_GEO_H

#include "Vect2D.h"

namespace MCK
{

namespace GEO
{

enum class Type
{
    BASE,
    CIRCLE,
    RECT
};

template<typename T>
class Base2D
{
    public:

        Base2D( void )
        {
            this->type = MCK::GEO::Type::BASE;
        }

        virtual ~Base2D( void ) {};

        MCK::GEO::Type get_type( void ) const noexcept
        {
            return this->type;
        }

        virtual const Vect2D<T>& get_center( void ) const noexcept
        {
            return this->center;
        }

        virtual const Vect2D<T>& get_center_offset( void ) const noexcept
        {
            return this->center_offset;
        }

        virtual void set_center_offset(
            const Vect2D<T>& pos,
            const Vect2D<T>& _center_offset
        ) noexcept
        {
            this->center_offset = _center_offset;
            this->change_pos( pos );
        }

        virtual const T get_left_bound( void ) const noexcept
        {
            return this->left_bound;
        }

        virtual const T get_top_bound( void ) const noexcept
        {
            return this->top_bound;
        }

        virtual const T get_right_bound( void ) const noexcept
        {
            return this->right_bound;
        }

        virtual const T get_bottom_bound( void ) const noexcept
        {
            return this->bottom_bound;
        }

        //! Returns true if shape contains specified point
        virtual bool contains( const MCK::Vect2D<T> &point ) const noexcept = 0;

        //! Get sqauare of distance of point from center
        virtual T dist_sq_from_center( const MCK::Vect2D<T> &point ) const noexcept
        {
            return MCK::Vect2D<T>::dist_sq( point, this->center ); 
        }

        //! Change position
        virtual void change_pos( Vect2D<T> pos ) noexcept = 0;

        //! Get area of shape
        virtual T get_area( void ) const noexcept = 0;


    protected:

        T left_bound;
        T top_bound;
        T right_bound;
        T bottom_bound;
        Vect2D<T> center;
        Vect2D<T> center_offset;

        Type type;

};  // End of MCK::GEO::Base2D class

// Forward declaration
template<typename T>
class Rectangle;

template<typename T>
class Circle : public Base2D<T>
{
    public:

        // Friendship granted for efficiency
        friend MCK::GEO::Rectangle<T>;

        Circle( void ) : Base2D<T>()
        {
            this->type = MCK::GEO::Type::CIRCLE;
            this->radius = 1.0f;
        }

        Circle(
            T _radius,
            MCK::Vect2D<T> _center_offset,
            MCK::Vect2D<T> pos = MCK::Vect2D<T>()
        ) : Base2D<T>()
        {
            this->type = MCK::GEO::Type::CIRCLE;
            this->center_offset = _center_offset;
            this->radius = _radius;
            this->radius_sq = this->radius * this->radius;
            this->change_pos( pos );
        }

        virtual ~Circle( void ) {}

        virtual T get_radius( void ) const noexcept
        {
            return this->radius;
        }

        virtual void set_radius( T _radius ) noexcept
        {
            this->radius = _radius;
            this->radius_sq = _radius * radius;
        }

        virtual T get_radius_sq( void ) const noexcept
        {
            return this->radius_sq;
        }

        //! Change position
        virtual void change_pos( Vect2D<T> pos ) noexcept
        {
            this->center = pos + this->center_offset;
            this->left_bound = this->center.get_x() - this->radius;
            this->top_bound = this->center.get_y() - this->radius;
            this->right_bound = this->center.get_x() + this->radius;
            this->bottom_bound = this->center.get_y() + this->radius;
        }

        //! Get area of shape
        virtual T get_area( void ) const noexcept
        {
            return this->radius_sq * 3.1415927f;
        }
        
        //! Returns true if shape contains specified point
        virtual bool contains( const MCK::Vect2D<T> &point ) const noexcept
        {
            return this->dist_sq_from_center( point ) <= this->radius_sq;
        }

        //! Returns true if this circle overlaps specified circle
        virtual bool overlaps( const MCK::GEO::Circle<T> &other ) const
        {
            const T DIST = this->radius + other.radius;
            return this->dist_sq_from_center( other.center ) 
                        <= DIST * DIST;
        }


    protected:

        T radius;
        T radius_sq;

};  // End of MCK::GEO::Circle class


template<typename T>
class Rectangle : public Base2D<T>
{
    public:

        Rectangle( void ) : Base2D<T>()
        {
            this->type = MCK::GEO::Type::RECT;
        }

        Rectangle(
            T _width,
            T _height,
            MCK::Vect2D<T> _center_offset,
            MCK::Vect2D<T> pos = MCK::Vect2D<T>()
        ) : Base2D<T>()
        {
            this->width = _width;
            this->height = _height;
            this->type = MCK::GEO::Type::RECT;
            this->center_offset = _center_offset;
            this->change_pos( pos );
        }

        virtual ~Rectangle( void ) {}

        //! Change position
        virtual void change_pos( Vect2D<T> pos ) noexcept
        {
            this->center = pos + this->center_offset;
            this->left_bound = this->center.get_x() - this->width / 2.0f;
            this->top_bound = this->center.get_y() - this->height / 2.0f;
            this->right_bound = this->center.get_x() + this->width / 2.0f;
            this->bottom_bound = this->center.get_y() + this->height / 2.0f;
        }

        //! Returns true if shape contains specified point
        virtual bool contains( const MCK::Vect2D<T> &point ) const noexcept
        {
            return point.get_x() >= this->left_bound
                    && point.get_x() <= this->right_bound
                    && point.get_y() <= this->top_bound
                    && point.get_y() <= this->bottom_bound;
        }

        //! Returns true if this rectangle overlaps specified rectangle
        virtual bool overlaps( const MCK::GEO::Rectangle<T> &other ) const
        {
            return !( this->right_bound < other.left_bound
                      || this->left_bound > other.right_bound
                      || this->bottom_bound < other.top_bound
                      || this->top_bound > other.bottom_bound
                    );
        }

        //! Returns true if this rectangle overlaps specified rectangle in X dimension
        virtual bool overlaps_hoz( const MCK::GEO::Rectangle<T> &other ) const
        {
            return !( this->right_bound < other.left_bound
                      || this->left_bound > other.right_bound
                    );
        }

        //! Returns true if this rectangle overlaps specified rectangle in Y dimension
        virtual bool overlaps_vert( const MCK::GEO::Rectangle<T> &other ) const
        {
            return !( this->bottom_bound < other.top_bound
                      || this->top_bound > other.bottom_bound
                    );
        }

        //! Returns true if this rectangle overlaps specified circle
        virtual bool overlaps( const MCK::GEO::Circle<T> &other ) const
        {
            // Check for bound overlap
            if( !( this->right_bound < other.left_bound
                      || this->left_bound > other.right_bound
                      || this->bottom_bound < other.top_bound
                      || this->top_bound > other.bottom_bound
                )
            )
            {
                // Exclude corner cases
                const T X = other.get_center().get_x();
                const T Y = other.get_center().get_y();
                if( X < this->left_bound  // Top left corner
                    && Y < this->top_bound
                    && !other.contains(
                        Vect2D<float>(
                            this->left_bound,
                            this->top_bound
                        )
                    ) || (
                    X > this->right_bound  // Top right corner
                    && Y < this->top_bound
                    && !other.contains(
                            Vect2D<float>(
                                this->right_bound,
                                this->top_bound
                            )
                        )
                    ) || (
                    X < this->left_bound  // Bottom left corner
                    && Y > this->bottom_bound
                    && !other.contains(
                            Vect2D<float>(
                                this->left_bound,
                                this->bottom_bound
                            )
                        )
                    ) || (
                    X > this->right_bound  // Bottom right corner
                    && Y > this->bottom_bound
                    && !other.contains(
                            Vect2D<float>(
                                this->right_bound,
                                this->bottom_bound
                            )
                        )
                    )
                )
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            
            return false;
        }

        //! Get area of shape
        virtual T get_area( void ) const noexcept
        {
            return this->width * this->height;
        }

        virtual T get_width( void ) const noexcept
        {
            return this->width;
        }

        virtual T get_height( void ) const noexcept
        {
            return this->height;
        }


    protected:

        T width;
        T height;

};  // End of MCK::GEO::Rectangle class

/////////////////////////////////////////////
// Methods relating to multiple instances

//! Returns true is shapes overlap
/*! Will throw if 'a' or 'b' do not point to a valid instance
 * Note: Returns false if no overlap comparison available for 
 *       given types
 */
template<typename T>
bool overlap(
    std::shared_ptr<MCK::GEO::Base2D<T>> a,
    std::shared_ptr<MCK::GEO::Base2D<T>> b
) noexcept
{
    switch( a->get_type() )
    {
        case MCK::GEO::Type::CIRCLE:

            switch( b->get_type() )
            {
                case MCK::GEO::Type::CIRCLE:
                    
                    return std::static_pointer_cast<MCK::GEO::Circle<T>>( b )
                        ->overlaps(
                            *std::static_pointer_cast<MCK::GEO::Circle<T>>( a )
                        );
                    break;
                
                case MCK::GEO::Type::RECT:
                    
                    return std::static_pointer_cast<MCK::GEO::Rectangle<T>>( b )
                        ->overlaps(
                            *std::static_pointer_cast<MCK::GEO::Circle<T>>( a )
                        );
                    break;
            }

        case MCK::GEO::Type::RECT:

            switch( b->get_type() )
            {
                case MCK::GEO::Type::RECT:
                    
                    return std::static_pointer_cast<MCK::GEO::Rectangle<T>>( a )
                        ->overlaps(
                            *std::static_pointer_cast<MCK::GEO::Rectangle<T>>( b )
                        );
                    break;
                
                case MCK::GEO::Type::CIRCLE:
                    
                    return std::static_pointer_cast<MCK::GEO::Rectangle<T>>( a )
                        ->overlaps(
                            *std::static_pointer_cast<MCK::GEO::Circle<T>>( b )
                        );
                    break;
            }

        // TODO: Other types
    }


    // DEBUG
    std::cout << "@?@ Overlap not supported for geometric type(s)"
              << std::endl;

    return false;
}

//! Returns true is shapes overlap
template<typename T>
bool overlap(
    const MCK::GEO::Base2D<T>* a,
    const MCK::GEO::Base2D<T>* b
) noexcept
{
    switch( a->get_type() )
    {
        case MCK::GEO::Type::CIRCLE:

            switch( b->get_type() )
            {
                case MCK::GEO::Type::CIRCLE:
                    
                    return static_cast<MCK::GEO::Circle<T>*>( b )->overlaps(
                            static_cast<MCK::GEO::Circle<T>*>( a )
                        );
                    break;
                
                case MCK::GEO::Type::RECT:
                    
                    return static_cast<MCK::GEO::Rectangle<T>*>( b )->overlaps(
                            *static_cast<MCK::GEO::Circle<T>*>( a )
                        );
                    break;
            }

        case MCK::GEO::Type::RECT:

            switch( b->get_type() )
            {
                case MCK::GEO::Type::RECT:
                    
                    return static_cast<MCK::GEO::Rectangle<T>*>( a )
                        ->overlaps(
                            *static_cast<MCK::GEO::Rectangle<T>*>( b )
                        );
                    break;
                
                case MCK::GEO::Type::CIRCLE:
                    
                    return static_cast<MCK::GEO::Rectangle<T>*>( a )
                        ->overlaps(
                            *static_cast<MCK::GEO::Circle<T>*>( b )
                        );
                    break;
            }

        // TODO: Other types
    }

    // DEBUG
    std::cout << "@?@ Overlap not supported for geometric type(s)"
              << std::endl;

    return false;
}

}  // End of namespace GEO

}  // End of namespace MCK

#endif
