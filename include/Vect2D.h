////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  Vect2D.h
//
//  Class template for 2D vetor
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

#ifndef MCK_VECT_2D_H
#define MCK_VECT_2D_H

#include<type_traits>  // For is_signed

#if defined MCK_STD_OUT
#include <string>
#endif

using std::is_signed;

namespace MCK
{

template<class T>
class Vect2D
{
    public:

        //! Default constructor
        constexpr Vect2D( void ) noexcept :
            x( T( 0 ) ),
            y( T( 0 ) )
        {}

        //! Destructor (non-virtual for compatibility with constexpr)
        // Note: constexpr not available for destructors in C++11
        ~Vect2D( void ) = default;  // noexcept by default

        //! Constructor
        constexpr Vect2D( T _x, T _y ) noexcept :
            x( _x ),
            y( _y )
        {}

        //! Copy constructor
        constexpr Vect2D( const Vect2D &other ) noexcept = default;

        //! Heterogeneous copy constructor
        /*! Made explicit to prevent unintended conversion */
        template <class U>
        explicit constexpr Vect2D( const Vect2D<U> &other ) noexcept
        {
            this->x = T( other.get_x() );
            this->y = T( other.get_y() );
        }

        //! Assignment constructor
        constexpr Vect2D& operator=( const Vect2D &other ) noexcept = default;
       
        //! Move constructor
        constexpr Vect2D( Vect2D &&other ) noexcept = default;

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

        //! Get x,y coords
        constexpr void get_xy( T& _x, T& _y ) const noexcept
        {
            _x = this->x;
            _y = this->y;
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

        //! Addition with assignment
        constexpr Vect2D& operator+=( Vect2D const &other ) noexcept

        {
            this->x += other.x;
            this->y += other.y;
            return *this;
        }

        //! Subtraction with assignment
        constexpr Vect2D& operator-=( Vect2D const &other ) noexcept

        {
            this->x -= other.x;
            this->y -= other.y;
            return *this;
        }

        //! Scalar multiplication with assignment
        template <typename U>
        constexpr Vect2D& operator*=( U s ) noexcept
        {
            this->x *= s;
            this->y *= s;
            return *this;
        }

        //! Scalar division with assignment
        template <typename U>
        constexpr Vect2D& operator/=( U s ) noexcept
        {
            this->x /= s;
            this->y /= s;
            return *this;
        }

        //! Compares this point to another
        constexpr bool operator==( Vect2D const &rhs ) const noexcept
        {
            return comp_x( *this, rhs ) == 0
                   && comp_y( *this, rhs ) == 0;
        }

        //! Addition operator
        constexpr Vect2D operator+( Vect2D const &rhs ) const noexcept
        {
            Vect2D ans( *this );
            ans += rhs;
            return ans;
        }

        //! Subtraction operator
        constexpr Vect2D operator-( Vect2D const &rhs ) const noexcept
        {
            Vect2D ans( *this );
            ans -= rhs;
            return ans;
        }

        //! Scalar multiplication operator (scalar on rhs)
        template <typename U>
        constexpr Vect2D operator*( U s ) const noexcept
        {
            Vect2D ans( *this );
            ans *= s;
            return ans;
        }

        //! Scalar division operator
        template <typename U>
        constexpr Vect2D operator/( U s ) const noexcept
        {
            Vect2D ans( *this );
            ans /= s;
            return ans;
        }

        //! Non-aspect preserving scaling
        template <typename U>
        constexpr Vect2D scale( U hoz_scale, U vert_scale ) const noexcept
        {
            Vect2D ans( *this );
            ans.x *= hoz_scale;
            ans.y *= vert_scale;
            return ans;
        }

        //! Magnitude squared
        constexpr T mag_sq( void ) const noexcept
        {
            return this->x * this->x + this->y * this->y;
        }

        ///////////////////////////////////////
        // Static member functions

        //! Square of distance between two points (signed)
        template<typename U = T, typename std::enable_if<std::is_signed<U>::value, bool>::type = true>
        static constexpr U dist_sq( Vect2D const &lhs, Vect2D const &rhs ) noexcept
        {
            // Optimised for signed
            U ans_x = lhs.x - rhs.x;
            U ans_y = lhs.y - rhs.y;
            
            return ans_x * ans_x + ans_y * ans_y;
        }

        //! Square of distance between two points (unsigned)
        template<typename U = T, typename std::enable_if<std::is_unsigned<U>::value, bool>::type = true>
        static constexpr U dist_sq( Vect2D const &lhs, Vect2D const &rhs ) noexcept
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
        static constexpr int comp_x( Vect2D const &lhs, Vect2D const &rhs ) noexcept
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
        static constexpr int comp_x( Vect2D const &lhs, Vect2D const &rhs ) noexcept
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
        static constexpr int comp_y( Vect2D const &lhs, Vect2D const &rhs ) noexcept
        {
            return ( lhs.y < rhs.y - MCK_POINT_EQ_TOL ) ?
                   -1 : ( lhs.y > rhs.y + MCK_POINT_EQ_TOL ) ?
                   1 : 0;
        }

        //! Compares two points with respect to 'y' dimension (integer)
        /*! @returns -1 if lhs.y < rhs.y, 1 if lhs.y > rhs.y, 0 otherwise
         */
        template<typename U = T, typename std::enable_if<std::is_integral<U>::value, bool>::type = true>
        static constexpr int comp_y( Vect2D const &lhs, Vect2D const &rhs ) noexcept
        {
            return ( lhs.y < rhs.y ) ?
                   -1 : ( lhs.y > rhs.y ) ?
                   1 : 0;
        }

        //! Dot (inner) product
        static constexpr T dot_prod( Vect2D const &lhs, Vect2D const &rhs ) noexcept
        {
            return lhs.x * rhs.x + lhs.y * rhs.y;
        }

#if defined MCK_STD_OUT
        //! Print x,y coords
        constexpr std::string str( void ) const
        {
            return "(" + std::to_string( x ) + ","
                   + std::to_string( y ) + ")";
        }
#endif

    protected:

        T x;
        T y;
};

}  // End of namespace MCK

#endif


