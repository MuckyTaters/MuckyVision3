////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  Point.h
//
//  Template class providing 3D coordinate data
//
//  There is no associated cpp file.
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
#include<type_traits>  // For is_signed

/////////////////////////////////////////
// FLOAT DATA TYPE
// This can be integer or floating point, signed or unsigned.
// Signed types are more efficient for calculating inter-point
// distances
#define MCK_POINT_TYPE float

/////////////////////////////////////////
// FLOAT DATA TYPE EQUALITY TOLERANCE
// Maximum amount by which the x,y and z
// dimension of two points can differ
// for the two points to  be considered
// equal (only used for floating point types)
#define MCK_POINT_TYPE_EQU_TOL 1E-3;

namespace MCK
{

//! True if Point's data type is signed
const bool POINT_TYPE_SIGNED = std::is_signed<MCK_POINT_TYPE>::value;

//! True if Point's data type is floating point
const bool POINT_TYPE_FLOATING_POINT
    = std::is_floating_point<MCK_POINT_TYPE>::value;

class Point
{
    // These non-member functions require direct access to x,y,z
    friend inline constexpr MCK_POINT_TYPE dist_sq( Point const &lhs, Point const &rhs );
    friend inline constexpr int comp_x( Point const &lhs, Point const &rhs );
    friend inline constexpr int comp_y( Point const &lhs, Point const &rhs );
    friend inline constexpr int comp_z( Point const &lhs, Point const &rhs );


    public:

        //! Default constructor
        constexpr Point( void ) :
            x( MCK::DEFAULT_X_VALUE ),
            y( MCK::DEFAULT_Y_VALUE ),
            z( MCK::DEFAULT_Z_VALUE )
        {}

        //! Destructor (non-virtual for compatibility with constexpr)
        ~Point( void ) = default;

        //! 2D constructor
        constexpr Point( MCK_POINT_TYPE _x, MCK_POINT_TYPE _y ) :
            x( _x ),
            y( _y ),
            z( MCK::DEFAULT_Z_VALUE )
        {}

        //! 3D constructor
        constexpr Point( 
            MCK_POINT_TYPE _x,
            MCK_POINT_TYPE _y,
            MCK_POINT_TYPE _z
        ): 
            x( _x ),
            y( _y ),
            z( MCK::DEFAULT_Z_VALUE )
        {}

        //! Copy constructor
        constexpr Point( const Point &other ) = default;

        //! Assignment constructor
        constexpr Point& operator=( Point const &other ) = default;
        
        //! Addition with assignment
        constexpr Point& operator+=( Point const &other )
        {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
            return *this;
        }

        //! Subtraction with assignment
        constexpr Point& operator-=( Point const &other )
        {
            this->x -= other.x;
            this->y -= other.y;
            this->z -= other.z;
            return *this;
        }

        //! Scalar multiplication with assignment
        constexpr Point& operator*=( MCK_POINT_TYPE s )
        {
            this->x *= s;
            this->y *= s;
            this->z *= s;
            return *this;
        }

        //! Scalar division with assignment
        constexpr Point& operator/=( MCK_POINT_TYPE s )
        {
            this->x /= s;
            this->y /= s;
            this->z /= s;
            return *this;
        }


    protected:

        MCK_POINT_TYPE x;
        MCK_POINT_TYPE y;
        MCK_POINT_TYPE z;
};

//////////////////////////////////////////////
// NON-MEMBER METHODS RELATED TO 'Point'

//! Compares two points with respect to 'x' dimension
/*! @returns -1 if lhs.x < rhs.x, 1 if lhs.x > rhs.x, 0 otherwise
 */
constexpr inline int comp_x( Point const &lhs, Point const &rhs )
{
#if POINT_TYPE_FLOATING_POINT
    return ( lhs.x < rhs.x - MCK_POINT_TYPE_EQ_TOL ) ?
           -1 : ( lhs.x > rhs.x - MCK_POINT_TYPE_EQ_TOL ) ?
           1 : 0;
#else
    return ( lhs.x < rhs.x ) ?
           -1 : ( lhs.x > rhs.x ) ?
           1 : 0;
#endif
}

//! Compares two points with respect to 'y' dimension
/*! @returns -1 if lhs.y < rhs.y, 1 if lhs.y > rhs.y, 0 otherwise
 */
constexpr inline int comp_y( Point const &lhs, Point const &rhs )
{
#if POINT_TYPE_FLOATING_POINT
    return ( lhs.y < rhs.y - MCK_POINT_TYPE_EQ_TOL ) ?
           -1 : ( lhs.y > rhs.y - MCK_POINT_TYPE_EQ_TOL ) ?
           1 : 0;
#else
    return ( lhs.y < rhs.y ) ?
           -1 : ( lhs.y > rhs.y ) ?
           1 : 0;
#endif
}

//! Compares two points with respect to 'z' dimension
/*! @returns -1 if lhs.z < rhs.z, 1 if lhs.z > rhs.z, 0 otherwise
 */
constexpr inline int comp_z( Point const &lhs, Point const &rhs )
{
#if POINT_TYPE_FLOATING_POINT
    return ( lhs.z < rhs.z - MCK_POINT_TYPE_EQ_TOL ) ?
           -1 : ( lhs.z > rhs.z - MCK_POINT_TYPE_EQ_TOL ) ?
           1 : 0;
#else
    return ( lhs.z < rhs.z ) ?
           -1 : ( lhs.z > rhs.z ) ?
           1 : 0;
#endif
}

//! Compares two points with respect to all three dimensions
constexpr inline bool operator==( Point const &lhs, Point const &rhs )
{
    return comp_x( lhs, rhs ) == 0
           && comp_y( lhs, rhs ) == 0
           && comp_z( lhs, rhs ) == 0;
}

//! Addition operator
inline constexpr Point operator+( Point const &lhs, Point const &rhs )
{
    Point ans( lhs );
    ans += rhs;
    return ans;
}

//! Subtraction operator
inline constexpr Point operator-( Point const &lhs, Point const &rhs )
{
    Point ans( lhs );
    ans -= rhs;
    return ans;
}

//! Scalar multiplication operator (scalar on rhs)
inline constexpr Point operator*( Point const &lhs, float s )
{
    Point ans( lhs );
    ans *= s;
    return ans;
}

//! Scalar multiplication operator (scalar on lhs)
inline constexpr Point operator*( float s, Point const &rhs )
{
    Point ans( rhs );
    ans *= s;
    return ans;
}

//! Scalar division operator
inline constexpr Point operator/( Point const &lhs, float s )
{
    Point ans( lhs );
    ans /= s;
    return ans;
}

//! Square of distance between points
inline constexpr MCK_POINT_TYPE dist_sq( Point const &lhs, Point const &rhs )
{
#if POINT_TYPE_SIGNED
    MCK_POINT_TYPE ans_x = lhs.x - rhs.x;
    MCK_POINT_TYPE ans_y = lhs.y - rhs.y;
    MCK_POINT_TYPE ans_z = lhs.z - rhs.z;
#else
    MCK_POINT_TYPE ans_x
        = lhs.x > rhs.x ? lhs.x - rhs.x : rhs.x - lhs.x;
    MCK_POINT_TYPE ans_y
        = lhs.y > rhs.y ? lhs.y - rhs.y : rhs.y - lhs.y;
    MCK_POINT_TYPE ans_z
        = lhs.z > rhs.z ? lhs.z - rhs.z : rhs.z - lhs.z;
#endif
    return ans_x * ans_x + ans_y * ans_y + ans_z * ans_z;
}

}  // End of namespace MCK

#endif
