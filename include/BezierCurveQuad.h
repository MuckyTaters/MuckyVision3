////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  BezierCurveQuad.h
//
//  Class template for quaratic Bezier curves
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

#ifndef MCK_BEZ_CURVE_QUAD_H
#define MCK_BEZ_CURVE_QUAD_H

#include "BezierCurveQuad.h"

namespace MCK
{

template<class T>
class BezierCurveQuad : public BezierCurveBase<T>
{
    public:

        //! Default constructor
        BezierCurveQuad( void ) : BezierCurveBase<T>()
        {
            // This is important, otherwise calculating a point
            // for an uninitialised instance could cause a
            // seg fault.
            this->control_points.resize( 3 );

            this->type = MCK::BezierCurveType::QUAD;
        }

        //! Destructor
        virtual ~BezierCurveQuad( void ) noexcept {}

        //! Initializer
        virtual void init( const std::vector<T> &_control_points )
        {
            // Check number of control points is acceptable
            if( _control_points.size() != 3 )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot init quadratic Bezier curve as " )
                    + std::string( "number of control points " )
                    + std::string( "supplied is not 3." )
#else
                    ""
#endif
                ) );
            }

            this->control_points = _control_points;

            this->initialized = true;
        }

        //! Alternate initializer
        virtual void init(
            T p0,
            T p1,
            T p2
        )
        {
            // Note: this is safe as 'control_points' is presized
            //       in the constructor.
            std::swap( p0, this->control_points[0] );
            std::swap( p1, this->control_points[1] );
            std::swap( p2, this->control_points[2] );
            this->initialized = true;
        }

        //! Get point on Bezier curve
        /*! @param t: Number on unit interval [0,1]
         *  IMPORTANT: No check or constraint is performed on t.
         */
        virtual T get_point( double t ) const noexcept
        {
            // Calculate 1 -t
            const double INV_T = 1 - t;

            // Note: this is safe even if not initialised,
            //       as 'control_points' is presized in the
            //       constructor.
            //       Of course the result would be garbage.
            return this->control_points[0] * ( INV_T * INV_T )
                   + this->control_points[1] * ( 2 * INV_T * t )
                   + this->control_points[2] * ( t * t );
        }
        
        //! Split curve at 't' and return part that includes first contol point
        virtual MCK::BezierCurveQuad<T> split_lo( double t ) const
        {
            // Split using De Casteljau's algorithm
            
            // Create point between P0 and P1
            const T P01 = this->control_points[0] * ( 1 - t )
                        + this->control_points[1] * t;

            // Create point between P1 and P2
            const T P12 = this->control_points[1] * ( 1 - t )
                        + this->control_points[2] * t;

            // Let calling method catch any (very unlikely) exception here
            BezierCurveQuad ans;
            ans.init(
                this->control_point[0],  // = P0
                P01,
                // Point between P01 and P12
                P01 * ( 1 - t ) + P12 * t
            );

            // I *think* the explicit 'move' is unneccesary here,
            // but including anyway
            return std::move( ans );
        }
        
        //! Split curve at 't' and return part that includes last contol point
        virtual MCK::BezierCurveQuad<T> split_hi( double t ) const
        {
            // Split using De Casteljau's algorithm
            
            // Create point between P0 and P1
            const T P01 = this->control_points[0] * ( 1 - t )
                        + this->control_points[1] * t;

            // Create point between P1 and P2
            const T P12 = this->control_points[1] * ( 1 - t )
                        + this->control_points[2] * t;

            // Let calling method catch any (very unlikely) exception here
            BezierCurveQuad ans;
            ans.init(
                // Point between P01 and P12
                P01 * ( 1 - t ) + P12 * t,
                P12,
                this->control_point[3]  // = P2
            );

            // I *think* the explicit 'move' is unneccesary here,
            // but including anyway
            return std::move( ans );
        }
};

}  // End of namespace MCK

#endif
