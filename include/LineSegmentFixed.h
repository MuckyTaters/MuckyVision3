////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  LineSegmentFixed.h
//
//  Class template for a line segment
//  with fixed end points.
//  The template parameter
//  for each line segment is a curve,
//  e.g. a Bezier curve.
//  This is an abstract class.
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

#ifndef MCK_LINE_SEG_FIXED_H
#define MCK_LINE_SEG_FIXED_H

#include <stdexcept>  // For exceptions
#include <map>  // For map
#include <utility>  // For swap

#include "LineSegmentBase.h"

namespace MCK
{

template<template<class> class U, class T>
class LineSegmentFixed : public LineSegmentBase<U,T>
{
    public:

        //! Constructor
        LineSegmentFixed( U<T> _curve ) : LineSegmentBase<U,T>( _curve )
        {
            this->type = MCK::LineSegmentType::FIXED;
            this->initialized = false;
        }

        //! Destructor
        ~LineSegmentFixed( void ) = default;
        
        //! Copy constructor
        LineSegmentFixed( const LineSegmentFixed &other ) = default;

        //! Assignment constructor
        LineSegmentFixed& operator=( LineSegmentFixed const &other ) = default;

        //! Returns true if segment initialized, false otherwise
        bool is_initialized( void ) const noexcept
        {
            return initialized;
        }

        //! Initialize segment by pre-calculating distance values
        /* @param distance_step: Distance between each pre-calculated point
         * Note: Distances that fall between pre-calculated points
         * will be resolved by linear interpolation, so a smaller
         * 'distance_step' will give smoother results, but take more
         * time and memory to pre-calculate.
         */
        virtual void init( double distance_step )
        {
            // TODO....

            this->initialized = true;
        }

        //! Get position on line by distance measured from the starting end.
        /*! @param arc_len: Arc length, i.e. distance along the line from the starting end
         */
        virtual T get_point_by_arc_len( double arc_len ) const
        {
            if( !initialized )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot get point by arc length " )
                    + std::string( "for fixed line segment as it is " )
                    + std::string( "not yet initialized." )
#else
                    ""
#endif
                ) );
            }

            // TODO...

            // Temporary code
            T ans;
            return ans; 
        }
        

    protected:

        bool initialized;

        std::map<double,T> points_by_arc_len;
};

}  // End of namespace MCK

#endif
