////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  LineSegmentBase.h
//
//  Class template for a line segment
//  base class. The template parameter
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

#ifndef MCK_LINE_SEG_BASE_H
#define MCK_LINE_SEG_BASE_H

#include<utility>  // For swap

namespace MCK
{

template<template<class> class U, class T>
class LineSegmentBase
{
    public:

        //! Constructor
        LineSegmentBase( U<T> _curve )
        {
            std::swap( this->curve, _curve );
            this->type = MCK::LineSegmentType::BASE;
        }

        //! Destructor
        ~LineSegmentBase( void ) = default;
        
        //! Copy constructor
        LineSegmentBase( const LineSegmentBase &other ) = default;

        //! Assignment constructor
        LineSegmentBase& operator=( LineSegmentBase const &other ) = default;

        //! Get type
        MCK::LineSegmentType get_type( void ) const noexcept
        {
            return this->type;
        }

        //! Get position on line by distance measured from the starting end.
        /*! @param arc_len: Arc length, i.e. distance along the line from the starting end
         */
        virtual T get_point_by_arc_len( double arc_len ) const = 0;
        

    protected:

        MCK::LineSegmentType type;

        U<T> curve;
};

}  // End of namespace MCK

#endif
