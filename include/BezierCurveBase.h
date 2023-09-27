////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  BezierCurveBase.h
//
//  Class template for a Bezier curve
//  base class. This is an abstract
//  class.
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

#ifndef MCK_BEZ_CURVE_BASE_H
#define MCK_BEZ_CURVE_BASE_H

#if defined MCK_STD_OUT
#include <string>
#endif

#include <vector>
#include <stdexcept>  // For exceptions

#include "Defs.h"

namespace MCK
{

template<class T>
class BezierCurveBase
{
    public:

        //! Minimum value of curve parameter
        static const int PARAM_MIN = 0;
        
        //! Maximum value of curve parameter
        static const int PARAM_MAX = 1;

        //! Default constructor
        BezierCurveBase( void ) noexcept
        {
            this->type = MCK::BezierCurveType::BASE;
            this->initialized = false;
        }

        //! Destructor
        virtual ~BezierCurveBase( void ) noexcept {}

        //! Initializer
        virtual void init( const std::vector<T> &_control_points ) = 0;

        //! Get point on Bezier curve
        /*! @param t: Number on unit interval [0,1]
         *  IMPORTANT: No check or constraint is performed on t.
         */
        virtual T get_point( double t ) const noexcept = 0;

        bool is_initialized( void ) const noexcept
        {
            return this->initialized;
        }

        MCK::BezierCurveType get_type( void ) const noexcept
        {
            return this->type;
        }

        //! Get read only version of control point
        /*! This will throw exception if 'n' is out-of-range */
        const T& get_control_point( size_t n ) const
        {
            if( n >= this->control_points.size() )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot get control point as n = " )
                    + std::to_string( n )
                    + std::string( " is out-of-range." )
#else
                    ""
#endif
                ) );
            }
            
            return this->control_points[n];
        }

        //! Get number of control points
        size_t get_num_of_control_points( void ) const noexcept
        {
            return control_points.size();
        }


    protected:

        MCK::BezierCurveType type; 

        bool initialized;

        std::vector<T> control_points;
};

}  // End of namespace MCK

#endif
