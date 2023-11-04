////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  CollisionEvent.h
//
//  Struct to hold sprite collision data
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

#ifndef MCK_COLL_EVENT_H
#define MCK_COLL_EVENT_H

namespace MCK
{

struct CollisionEvent
{
    // Sprites involved in collision 
    // std::shared_ptr<MCK::CollisionPairing> pairing;
    std::shared_ptr<MCK::SpritePos> sprite_A;
    std::shared_ptr<MCK::SpritePos> sprite_B;

    // Angle of collision, clockwise from north,
    // with respect to sprite A, in radians
    float angle;

    // If > 0, 'angle' should be rounded to 2 * pi / angle_lock
    // For instance, if 'angle_lock' = 4, 'angle' is rounded
    // to nearest pi / 2 radians, i.e. 90 degrees.
    // If 'angle_lock' = 8, 45 degrees, and so on.
    int angle_lock;

    // Amount, in pixels, of overlap between the two sprites
    float overlap_dist;

    //! Default constructor
    CollisionEvent( void )
    {
        this->angle = 0.0f;
        this->angle_lock = 4; // North, South, East and West
        this->overlap_dist = 0.0f;
    }

    //! Constructor
    CollisionEvent
    (
        std::shared_ptr<MCK::SpritePos> _sprite_A,
        std::shared_ptr<MCK::SpritePos> _sprite_B,
        float _angle,
        int _angle_lock,
        float _overlap_dist
    ) : sprite_A( _sprite_A ),
        sprite_B( _sprite_B ),
        angle( _angle ),
        angle_lock( _angle_lock ),
        overlap_dist( _overlap_dist )
    {}
    
    //! Partial constructor
    CollisionEvent
    (
        std::shared_ptr<MCK::SpritePos> _sprite_A,
        std::shared_ptr<MCK::SpritePos> _sprite_B
    ) : sprite_A( _sprite_A ),
        sprite_B( _sprite_B )
    {
        this->angle = 0.0f;
        this->angle_lock = 4; // North, South, East and West
        this->overlap_dist = 0.0f;
    }
};

}  // End of namespace MCK

#endif
