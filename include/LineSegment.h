////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  LineSegment.h
//
//  Class template for a line segment
//  with fixed end points.
//  The template parameter
//  for each line segment is a curve,
//  e.g. a Bezier curve.
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

#ifndef MCK_LINE_SEG_H
#define MCK_LINE_SEG_H

#if defined MCK_STD_OUT
#include <string>
#include <iostream>
#endif

#include <cmath>  // For sqrt
#include <map>  // For map
#include <memory>  // For shared_ptr
#include <stdexcept>  // For exceptions
#include <utility>  // For swap

namespace MCK
{

template<template<class> class U, class T>
class LineSegment
{
    public:

        //! Constructor
        LineSegment( U<T> _curve )
        {
            std::swap( this->curve, _curve );
            this->initialized = false;
            id = MCK::DEFAULT_LINE_SEG_ID;
        }

        //! Destructor
        virtual ~LineSegment( void ) = default;
        
        //! Copy constructor
        LineSegment( const LineSegment &other ) = default;

        //! Assignment constructor
        LineSegment& operator=( LineSegment const &other ) = default;

        //! Returns id of segment, if set during initialization
        MCK_LINE_SEG_ID_TYPE get_id( void ) const noexcept
        {
            return id;
        }

        //! Returns true if segment initialized, false otherwise
        bool is_initialized( void ) const noexcept
        {
            return initialized;
        }

        //! Initialize segment by pre-calculating distance values
        /* @param distance_step: Distance between each pre-calculated point
         * @param xy_only: If true, arc distance is only measured in xy dimension
         * Note: Distances that fall between pre-calculated points
         * will be resolved by linear interpolation, so a smaller
         * 'distance_step' will give smoother results, but take more
         * time and memory to pre-calculate.
         */
        virtual void init(
            double distance_step,
            bool xy_only = false,
            MCK_LINE_SEG_ID_TYPE _id = MCK::DEFAULT_LINE_SEG_ID
        )
        {
            // Forbid re-initialisation
            if( this->initialized )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot initialize line segment " )
                    + std::string( "as already initialized." )
#else
                    ""
#endif
                ) );
            }

            // Check the supplied curve is initialised
            if( !this->curve.is_initialized() )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot initialize line segment " )
                    + std::string( "as supplied curve is not yet " )
                    + std::string( "initialized." )
#else
                    ""
#endif
                ) );
            }

            id = _id;

            // Pre-calculate square of distance step,
            // as this is value actually used
            this->distance_step_squared
                = distance_step * distance_step;

            // Get starting parameters, respesenting
            // the start and end of the curve
            const double START_PARAM = double( U<T>::PARAM_MIN );
            const double END_PARAM = double( U<T>::PARAM_MAX );
                    
            // Get start point
            const T START_POINT 
                = this->curve.get_point( START_PARAM );

            // Get end point
            const T END_POINT 
                = this->curve.get_point( END_PARAM );

            // Store most proximal point
            _store_point(
                0.0f,  // arc length always zero for this point
                START_POINT
            );

            // Process recursively, adding new points until
            // all points, including start and end 
            // point (at PARAM_MIN and PARAM_MAX, respectively)
            // are within 'distance_step' of each other,
            // in terms of straight line distance.
            // This method also estimates the total length
            // of the segment.
            try
            {
                this->length_of_segment = _bisection_search(
                    START_PARAM,
                    START_POINT,
                    END_PARAM,
                    END_POINT,
                    0.0f,  // arc length to start point, obviously zero
                    xy_only
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Failed to initialize line segment " )
                    + std::string( "as biscection search failed, error = " )
                    + e.what()
#else
                    ""
#endif
                ) );
            }

            // Store most distal point
            _store_point(
                this->length_of_segment,
                END_POINT
            );

            // If no exception thrown by search, set
            // initialization flag
            this->initialized = true;
        }

        //! Connect additional line segment(s) to the end of this one
        virtual void connect_segments(
            std::map<
                MCK_JUNC_CODE_TYPE,
                std::shared_ptr<const MCK::LineSegment<U,T>>
            > _connections,
            bool quality_check = true,
            bool prevent_overwrite = true
        )
        {
            if( prevent_overwrite && connections.size() > 0 )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot connect segments to " )
                    + std::string( "line segment as segment already " )
                    + std::string( "has connections and overwrite " )
                    + std::string( "prevention is selected." )
#else
                    ""
#endif
                ) );
            }

            if( quality_check )
            {
                for( const auto it : _connections )
                {
                    if( it.second.get() == NULL
                        || !it.second->is_initialized()
                    )
                    {
                        throw( std::runtime_error(
#if defined MCK_STD_OUT
                            std::string( "Cannot connect segments to " )
                            + std::string( "line segment as one of " )
                            + std::string( "segment is NULL or " )
                            + std::string( "not yet initialized." )
#else
                            ""
#endif
                        ) );
                    }
                }
            }

            swap( connections, _connections );
        }

        //! Connect single line segment(s) to the end of this one
        virtual void connect_single_segment(
            std::shared_ptr<const MCK::LineSegment<U,T>> next_seg,
            bool quality_check = true,
            bool prevent_overwrite = true
        )
        {
            if( prevent_overwrite && connections.size() > 0 )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot connect single segment to " )
                    + std::string( "line segment as segment already " )
                    + std::string( "has connections and overwrite " )
                    + std::string( "prevention is selected." )
#else
                    ""
#endif
                ) );
            }

            if( quality_check
                && ( next_seg.get() == NULL
                     || !next_seg->is_initialized()
                )
            )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot connect single segment to " )
                    + std::string( "line segment as " )
                    + std::string( "segment is NULL or " )
                    + std::string( "not yet initialized." )
#else
                    ""
#endif
                ) );
            }

            // Create connection map entry for next segment,
            // using the default key value.
            connections.clear();
            connections.insert(
                std::pair<
                    MCK_JUNC_CODE_TYPE,
                    std::shared_ptr<const MCK::LineSegment<U,T>>
                >(
                    MCK::DEFAULT_LINE_SEG_KEY_VALUE,
                    next_seg
                )
            );
        }

        //! Get single connecting segment
        /*! Note: if none or > 1 connecting segments, exception is thrown */
        std::shared_ptr<const MCK::LineSegment<U,T>> get_single_connection( void ) const
        {
            if( !has_single_connection() )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot get single connection as " )
                    + std::string( "line segment with id " )
                    + ( id != MCK::DEFAULT_LINE_SEG_ID ?
                            std::to_string( id ) : "NONE"
                      )
                    + std::string( " does not have " )
                    + std::string( "a single connection, it has " )
                    + std::to_string( connections.size() )
                    + std::string( " connections." )
#else
                    ""
#endif
                ) );
            }
            
            return connections.begin()->second;
        }

        // Get read-only pointer to connecting segments
        // Note: this pointer is valid as long as the segment
        //       instance exists, however the connections
        //       themsevles *might* be overwritten, so do not
        //       retain iterators or size info between frames.
        virtual const std::map<
            MCK_JUNC_CODE_TYPE,
            std::shared_ptr<const MCK::LineSegment<U,T>>
        >* get_connections( void ) const noexcept
        {
            return &connections;
        }

        bool has_connections( void ) const noexcept
        {
            return connections.size() > 0;
        }

        bool has_single_connection( void ) const noexcept
        {
            return connections.size() == 1;
        }

        bool get_num_connections( void ) const noexcept
        {
            return connections.size();
        }

        //! Get position on line by distance measured from the starting end.
        /*! @param arc_len: Arc length, i.e. distance along the line from the starting end
            Note: Will throw an exception if arc_len is negative
                  or exceeds the length of the segment
         */
        virtual T get_point_by_arc_len( double arc_len ) const
        {
            if( !initialized )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot get point by arc length " )
                    + std::string( "for line segment as it is " )
                    + std::string( "not yet initialized." )
#else
                    ""
#endif
                ) );
            }

            // Get the two stored points/arc length pairs
            // whose arc length is closest to the desired arc length
            T p1, p2;
            double arc_len_1, arc_len_2;
            
            // Get iterator for p2 using 'lower_bound' method
            auto it = this->points_by_arc_len.lower_bound( arc_len );
            if( it != this->points_by_arc_len.end() )
            {
                arc_len_2 = it->first;
                p2 = it->second;
            }
            else
            {
                // If result invalid (i.e. arc_len > length of line)
                // just return the last point on the line
                auto it2 = this->points_by_arc_len.rbegin();
                if( it2 != this->points_by_arc_len.rend() )
                {
                    return it2->second;
                }
                else
                {
                    // If even last point invalid, throw
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Cannot get point on " )
                        + std::string( "line segment at arc length " )
                        + std::to_string( arc_len )
                        
#else
                        ""
#endif
                    ) );
                }
            }
            
            // Try to get preceding point by
            // decrementing iterator
            if( it != this->points_by_arc_len.end()
                && it != points_by_arc_len.begin()
            )
            {
                // Get iterator for p1 using 'upper_bound' method
                it--;
                arc_len_1 = it->first;
                p1 = it->second;
            }
            else
            {
                // If p2 is first point on curve, 
                // just return p2
                return p2;
            }

            // If arc_len_1 and arc_len_2 are tolerably identical,
            // just return either point, say, p1
            // This prevents a division by zero error
            const double DENOM = fabs( arc_len_1 - arc_len_2 );
            if( DENOM < MCK_POINT_EQ_TOL )
            {
                return p1;
            }

            // Interpolate between p1 and p2,
            // in proportion to arc length,
            // and return result
            const double RATIO 
                = fabs( arc_len_1 - arc_len ) / DENOM;
            return p1 * ( 1.0f - RATIO ) + p2 * RATIO;
        }

        //! Get maximum (straight line) distance between sample points, set at initialization
        double get_distance_step( void ) const noexcept
        {
            return sqrt( this->distance_step_squared );
        }

        //! Get (estimated) total length of line segment
        double get_length( void ) const noexcept
        {
            return this->length_of_segment;
        }

        //! Get read-only version of curve on which line segment is based.
        virtual const U<T>& get_curve( void ) const noexcept
        {
            // Don't pass by ref as this curve instance
            // must NOT be changed once the the line segment
            // is initialized.
            return this->curve;
        }

#if defined MCK_STD_OUT
        void str( void )
        {
            std::cout << "MCK::LineSegment points_by_arc_len:"
                      << std::endl;
            if( points_by_arc_len.size() == 0 )
            {
                std::cout << ">> NO CONTENT" << std::endl;
            }
            for( auto it : points_by_arc_len )
            {
                std::cout << ">> " << it.first << "," 
                          << it.second.str()
                          << std::endl;
            }
        }
#endif
        

    protected:

        //! Internal method used by 'init'
        double _bisection_search(
            double curve_parameter_at_point_A,
            const T &point_A,
            double curve_parameter_at_point_B,
            const T &point_B,
            double arc_length_to_point_A,
            bool xy_only = false
        )
        {
            // If points A and B are sufficiently close,
            // end the recursion here.
            // Note: return distance so that total
            //       segment length may be calculated
            if( xy_only )
            {
                // Calculate square of xy distance
                const double DIST_SQ_XY
                    = T::dist_sq_xy( point_A, point_B );

                if( DIST_SQ_XY <= this->distance_step_squared ) 
                {
                    return arc_length_to_point_A + sqrt( DIST_SQ_XY );
                }
            }
            else
            {
                // Calculate square of distance
                const double DIST_SQ
                    = T::dist_sq( point_A, point_B );

                if( DIST_SQ <= this->distance_step_squared ) 
                {
                    // Return distance so that total
                    // segment length may be calculated
                    return arc_length_to_point_A + sqrt( DIST_SQ );
                }
            }

            // Otherwise, bisect the curve parameter
            // and find the point defined by this
            const double NEW_PARAM = (
                curve_parameter_at_point_A
                + curve_parameter_at_point_B
            ) / 2.0f;
            const T NEW_POINT = this->curve.get_point( NEW_PARAM );
        
            // Recursively search both halves of the
            // bisected segment. 
            // Note: It is important to search A-to-NEW_POINT
            //       first, so that sum 'arc_length_to_point_A
            //       + DIST_A_TO_NEW' is accurate when the search
            //       moves to NEW_POINT-to-B half.
            //       If any exception throw here, let it unwind the
            //       recursion and be caught by 'init'. No point
            //       carrying on as distances may be inaccurate.
            const double ARC_LENGTH_TO_NEW
                = this->_bisection_search(
                    curve_parameter_at_point_A,
                    point_A,
                    NEW_PARAM,
                    NEW_POINT,
                    arc_length_to_point_A,
                    xy_only
                );

            const double ARC_LENGTH_TO_B
                = this->_bisection_search(
                    NEW_PARAM,
                    NEW_POINT,
                    curve_parameter_at_point_B,
                    point_B,
                    ARC_LENGTH_TO_NEW,
                    xy_only
                );

            // Store new point, indexed by distance
            // Note: If any exception throw here, let it unwind the
            //       recursion and be caught by 'init'. No point
            //       carrying on as distances may be inaccurate.
            this->_store_point(
                ARC_LENGTH_TO_NEW,
                NEW_POINT
            );

            // .. and return arc length to most distal point
            // for use by calling method
            return ARC_LENGTH_TO_B;
        }

        //! Internal method used by '_bisection_search'
        void _store_point(
            double arc_length,
            const T &point
        )
        {
            auto rc = points_by_arc_len.insert(
                    std::pair<double,T>(
                        arc_length,
                        point
                    )
                );
            if( !rc.second )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot initialize line segment " )
                    + std::string( "as failed to store point " )
                    + point.str()
                    + std::string( " at arc length " )
                    + std::to_string( arc_length )
                    + std::string( ". Perhaps duplication of arc length?" )
#else
                    ""
#endif
                ) );
            }
        }

        bool initialized;

        MCK_LINE_SEG_ID_TYPE id;

        double distance_step_squared;

        double length_of_segment;
        
        U<T> curve;

        std::map<double,T> points_by_arc_len;

        std::map<
            MCK_JUNC_CODE_TYPE,
            std::shared_ptr<const MCK::LineSegment<U,T>>
        > connections;
};

}  // End of namespace MCK

#endif
