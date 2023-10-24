////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  Point.h
//
//  Class template for 3D coordinate data
//
//  There is no associated cpp file.
//
//  This class is suitable for use in 
//  constant expressions
//
//  WARNING: This class uses template
//  metaprogramming techniques and has
//  only been tested with sensibly
//  selected template parameters.
//  It is possible that no-so-sensible
//  choices will succesfully compile
//  but produce unexpected behaviour at
//  runtime. I will endevour to produce
//  a list of safe choices for all
//  template parameters in this class
//  when time permits.
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

#ifndef MCK_POINT_H
#define MCK_POINT_H

#include<cstdint>  // For uint32_t et al., if needed
#include<memory>  // For shared pointer
#include<type_traits>  // For is_signed

#include "Defs.h"

#if defined MCK_STD_OUT
#include <string>
#endif

using std::is_signed;

namespace MCK
{

// Forward declaration
class GameEngRenderBlock;

template<class T>
class Point
{
    public:

        static const bool MCK_POINT_IS_SIGNED = std::is_signed<T>::value;
        static const bool MCK_POINT_IS_FLOATING_POINT = std::is_floating_point<T>::value;
        static const size_t MCK_POINT_BITSIZE = sizeof( T ) * 8;

        //! Default constructor
        constexpr Point( void ) noexcept :
            x( T( 0 ) ),
            y( T( 0 ) ),
            z( T( 0 ) )
        {}

        //! Destructor (non-virtual for compatibility with constexpr)
        // Note: constexpr not available for destructors in C++11
        ~Point( void ) = default;  // noexcept by default

        //! 2D constructor
        constexpr Point( T _x, T _y ) noexcept :
            x( _x ),
            y( _y ),
            z( 0 )
        {}

        //! 3D constructor
        constexpr Point( 
            T _x,
            T _y,
            T _z
        ) noexcept: 
            x( _x ),
            y( _y ),
            z( _z )
        {}

        //! 2D constructor (with block)
        constexpr Point(
            T _x,
            T _y,
            std::shared_ptr<MCK::GameEngRenderBlock> _block
        ) noexcept :
            x( _x ),
            y( _y ),
            z( 0 ),
            block( _block )
        {}

        //! 3D constructor (with block)
        constexpr Point( 
            T _x,
            T _y,
            T _z,
            std::shared_ptr<MCK::GameEngRenderBlock> _block
        ) noexcept: 
            x( _x ),
            y( _y ),
            z( _z ),
            block( _block )
        {}

        //! Copy constructor
        constexpr Point( const Point &other ) noexcept = default;

        //! Heterogeneous copy constructor
        /*! Made explicit to prevent unintended conversion */
        template <class U>
        explicit constexpr Point( const Point<U> &other ) noexcept
        {
            this->x = T( other.get_x() );
            this->y = T( other.get_y() );
            this->z = T( other.get_z() );
            this->block = other.get_block();
        }

        //! Assignment constructor
        constexpr Point& operator=( const Point &other ) noexcept = default;
       
        //! Move constructor
        constexpr Point( Point &&other ) noexcept = default;

        //! Get x coord
        constexpr T get_x( void ) const noexcept
        {
            return this->x;
        }

        //! Get y coord
        constexpr T get_y( void ) const noexcept
        {
            return this->y;
        }

        //! Get z coord
        constexpr T get_z( void ) const noexcept
        {
            return this->z;
        }

        //! Set x coord
        constexpr void set_x( T new_x ) noexcept
        {
            this->x = new_x;
        }

        //! Set y coord
        constexpr void set_y( T new_y ) noexcept

        {
            this->y = new_y;
        }

        //! Set z coord
        constexpr void set_z( T new_z ) noexcept

        {
            this->z = new_z;
        }

        //! Addition with assignment
        constexpr Point& operator+=( Point const &other ) noexcept

        {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
            return *this;
        }

        //! Subtraction with assignment
        constexpr Point& operator-=( Point const &other ) noexcept

        {
            this->x -= other.x;
            this->y -= other.y;
            this->z -= other.z;
            return *this;
        }

        //! Scalar multiplication with assignment
        template <typename U>
        constexpr Point& operator*=( U s ) noexcept
        {
            this->x *= s;
            this->y *= s;
            this->z *= s;
            return *this;
        }

        //! Scalar division with assignment
        template <typename U>
        constexpr Point& operator/=( U s ) noexcept
        {
            this->x /= s;
            this->y /= s;
            this->z /= s;
            return *this;
        }

        //! Compares this point to another with respect to all three dimensions
        constexpr bool operator==( Point const &rhs ) const noexcept
        {
            return comp_x( *this, rhs ) == 0
                   && comp_y( *this, rhs ) == 0
                   && comp_z( *this, rhs ) == 0;
        }

        //! Addition operator
        constexpr Point operator+( Point const &rhs ) const noexcept
        {
            Point ans( *this );
            ans += rhs;
            return ans;
        }

        //! Subtraction operator
        constexpr Point operator-( Point const &rhs ) const noexcept
        {
            Point ans( *this );
            ans -= rhs;
            return ans;
        }

        //! Scalar multiplication operator (scalar on rhs)
        template <typename U>
        constexpr Point operator*( U s ) const noexcept
        {
            Point ans( *this );
            ans *= s;
            return ans;
        }

        //! Scalar division operator
        template <typename U>
        constexpr Point operator/( U s ) const noexcept
        {
            Point ans( *this );
            ans /= s;
            return ans;
        }

        //! Non-aspect preserving scaling
        template <typename U>
        constexpr Point scale( U hoz_scale, U vert_scale ) const noexcept
        {
            Point ans( *this );
            ans.x *= hoz_scale;
            ans.y *= vert_scale;
            return ans;
        }

        ///////////////////////////////////////
        // Static member functions

        //! Square of distance between two points in xyz space (signed)
        template<typename U = T, typename std::enable_if<std::is_signed<U>::value, bool>::type = true>
        static constexpr U dist_sq( Point const &lhs, Point const &rhs ) noexcept
        {
            // Optimised for signed
            U ans_x = lhs.x - rhs.x;
            U ans_y = lhs.y - rhs.y;
            U ans_z = lhs.z - rhs.z;
            
            return ans_x * ans_x + ans_y * ans_y + ans_z * ans_z;
        }

        //! Square of distance between two points in xyz space (unsigned)
        template<typename U = T, typename std::enable_if<std::is_unsigned<U>::value, bool>::type = true>
        static constexpr U dist_sq( Point const &lhs, Point const &rhs ) noexcept
        {
            // Safe for unsigned
            U ans_x
                = lhs.x > rhs.x ? lhs.x - rhs.x : rhs.x - lhs.x;
            U ans_y
                = lhs.y > rhs.y ? lhs.y - rhs.y : rhs.y - lhs.y;
            U ans_z
                = lhs.z > rhs.z ? lhs.z - rhs.z : rhs.z - lhs.z;

            return ans_x * ans_x + ans_y * ans_y + ans_z * ans_z;
        }

        //! Square of distance between two points in xy space (signed)
        template<typename U = T, typename std::enable_if<std::is_signed<U>::value, bool>::type = true>
        static constexpr U dist_sq_xy( Point const &lhs, Point const &rhs ) noexcept
        {
            // Optimised for signed
            U ans_x = lhs.x - rhs.x;
            U ans_y = lhs.y - rhs.y;
            
            return ans_x * ans_x + ans_y * ans_y;
        }
        
        //! Square of distance between two points in xy space (unsigned)
        template<typename U = T, typename std::enable_if<std::is_unsigned<U>::value, bool>::type = true>
        static constexpr U dist_sq_xy( Point const &lhs, Point const &rhs ) noexcept
        {
            // Safe for unsigned
            U ans_x
                = lhs.x > rhs.x ? lhs.x - rhs.x : rhs.x - lhs.x;
            U ans_y
                = lhs.y > rhs.y ? lhs.y - rhs.y : rhs.y - lhs.y;

            return ans_x * ans_x + ans_y * ans_y;
        }

        //! Compares two points with respect to 'x' dimension (floating point)
        /*! @returns -1 if lhs.x < rhs.x, 1 if lhs.x > rhs.x, 0 otherwise
         */
        template<typename U = T, typename std::enable_if<std::is_floating_point<U>::value, bool>::type = true>
        static constexpr int comp_x( Point const &lhs, Point const &rhs ) noexcept
        {
            // Safe for floating point
            return ( lhs.x < rhs.x - MCK_POINT_EQ_TOL ) ?
                   -1 : ( lhs.x > rhs.x + MCK_POINT_EQ_TOL ) ?
                   1 : 0;
        }

        //! Compares two points with respect to 'x' dimension (integer)
        /*! @returns -1 if lhs.x < rhs.x, 1 if lhs.x > rhs.x, 0 otherwise
         */
        template<typename U = T, typename std::enable_if<std::is_integral<U>::value, bool>::type = true>
        static constexpr int comp_x( Point const &lhs, Point const &rhs ) noexcept
        {
            // Optimised for integer
            return ( lhs.x < rhs.x ) ?
                   -1 : ( lhs.x > rhs.x ) ?
                   1 : 0;
        }

        //! Compares two points with respect to 'y' dimension (floating point)
        /*! @returns -1 if lhs.y < rhs.y, 1 if lhs.y > rhs.y, 0 otherwise
         */
        template<typename U = T, typename std::enable_if<std::is_floating_point<U>::value, bool>::type = true>
        static constexpr int comp_y( Point const &lhs, Point const &rhs ) noexcept
        {
            return ( lhs.y < rhs.y - MCK_POINT_EQ_TOL ) ?
                   -1 : ( lhs.y > rhs.y + MCK_POINT_EQ_TOL ) ?
                   1 : 0;
        }

        //! Compares two points with respect to 'y' dimension (integer)
        /*! @returns -1 if lhs.y < rhs.y, 1 if lhs.y > rhs.y, 0 otherwise
         */
        template<typename U = T, typename std::enable_if<std::is_integral<U>::value, bool>::type = true>
        static constexpr int comp_y( Point const &lhs, Point const &rhs ) noexcept
        {
            return ( lhs.y < rhs.y ) ?
                   -1 : ( lhs.y > rhs.y ) ?
                   1 : 0;
        }

        //! Compares two points with respect to 'z' dimension (floating point)
        /*! @returns -1 if lhs.z < rhs.z, 1 if lhs.z > rhs.z, 0 otherwise
         */
        template<typename U = T, typename std::enable_if<std::is_floating_point<U>::value, bool>::type = true>
        static constexpr int comp_z( Point const &lhs, Point const &rhs ) noexcept
        {
            return ( lhs.y < rhs.y - MCK_POINT_EQ_TOL ) ?
                   -1 : ( lhs.y > rhs.y + MCK_POINT_EQ_TOL ) ?
                   1 : 0;
        }

        //! Compares two points with respect to 'z' dimension (integer)
        /*! @returns -1 if lhs.z < rhs.z, 1 if lhs.z > rhs.z, 0 otherwise
         */
        template<typename U = T, typename std::enable_if<std::is_integral<U>::value, bool>::type = true>
        static constexpr int comp_z( Point const &lhs, Point const &rhs ) noexcept
        {
            return ( lhs.z < rhs.z ) ?
                   -1 : ( lhs.z > rhs.z ) ?
                   1 : 0;
        }

        constexpr void set_block( 
            std::shared_ptr<MCK::GameEngRenderBlock> _block
        ) noexcept
        {
            block = _block;
        }

        constexpr std::shared_ptr<MCK::GameEngRenderBlock> get_block ( void ) const noexcept
        {
            return block;
        }

#if defined MCK_STD_OUT
        //! Print x,y,z coords
        constexpr std::string str( void ) const
        {
            return "(" + std::to_string( x ) + ","
                   + std::to_string( y ) + ","
                   + std::to_string( z ) + ")";
        }
        
        //! Print x,y coords only
        constexpr std::string str_xy( void ) const
        {
            return "(" + std::to_string( x ) + ","
                   + std::to_string( y ) + ")";
        }
#endif

    protected:

        T x;
        T y;
        T z;

        std::shared_ptr<MCK::GameEngRenderBlock> block;
};

}  // End of namespace MCK

#endif
