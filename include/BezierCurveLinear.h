////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  BezierCurveLinear.h
//
//  Class template for linear Bezier curves
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

#ifndef MCK_BEZ_CURVE_LIN_H
#define MCK_BEZ_CURVE_LIN_H

#include <utility>  // For swap

#include "BezierCurveBase.h"

namespace MCK
{

template<class T>
class BezierCurveLinear : public BezierCurveBase<T>
{
    public:

        //! Default constructor
        BezierCurveLinear( void ) : BezierCurveBase<T>()
        {
            // This is important, otherwise calculating a point
            // for an uninitialised instance could cause a
            // seg fault.
            this->control_points.resize( 2 );

            this->type = MCK::BezierCurveType::LINEAR;
        }

        //! Destructor
        virtual ~BezierCurveLinear( void ) noexcept {}

        //! Copy constructor
        BezierCurveLinear( const BezierCurveLinear<T> &other <) noexcept = default;

        //! Assignment constructor
        BezierCurveLinear& operator=( const BezierCurveLinear<T> &other ) noexcept = default;
       
        //! Move constructor
        BezierCurveLinear( const BezierCurveLinear<T> &&other ) noexcept = default;

        //! Initializer
        virtual void init( const std::vector<T> _control_points )
        {
            // Check number of control points is acceptable
            if( _control_points.size() != 2 )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot init linear Bezier curve as " )
                    + std::string( "number of control points " )
                    + std::string( "supplied is not 2." )
#else
                    ""
#endif
                ) );
            }

            std::swap( this->control_points, _control_points );

            this->initialized = true;
        }

        //! Alternate initializer
        virtual void init(
            T p0,
            T p1
        )
        {
            // Note: this is safe as 'control_points' is presized
            //       in the constructor.
            std::swap( p0, this->control_points[0] );
            std::swap( p1, this->control_points[1] );
            this->initialized = true;
        }

        //! Get point on Bezier curve
        /*! @param t: Floating point number on unit interval [0,1]
         *  IMPORTANT: No check or constraint is performed on t.
         */
        virtual T get_point( double t ) const noexcept
        {
            // Note: this is safe even if not initialised,
            //       as 'control_points' is presized in the
            //       constructor.
            //       Of course the result would be garbage.
            return this->control_points[0] * ( 1 - t )
                   + this->control_points[1] * t;
        }
        
        //! Split curve at 't' and return part that includes first contol point
        virtual MCK::BezierCurveLinear<T> split_lo( double t ) const
        {
            // Split using De Casteljau's algorithm
            // Let calling method catch any (very unlikely) exception here
            MCK::BezierCurveLinear<T> ans; 
            ans.init(
                this->control_point[0],
                this->control_points[0] * ( 1 - t )
                    + this->control_points[1] * t
            );

            // I *think* the explicit 'move' is unneccesary here,
            // but including anyway
            return std::move( ans );
        }

        //! Split curve at 't' and return part that includes last contol point
        virtual MCK::BezierCurveLinear<T> split_hi( double t ) const
        {
            // Split using De Casteljau's algorithm
            // Let calling method catch any (very unlikely) exception here
            MCK::BezierCurveLinear<T> ans; 
            ans.init(
                this->control_points[0] * ( 1 - t )
                    + this->control_points[1] * t,
                this->control_point[1]
            );

            // I *think* the explicit 'move' is unneccesary here,
            // but including anyway
            return std::move( ans );
        }
};        

}  // End of namespace MCK

#endif
