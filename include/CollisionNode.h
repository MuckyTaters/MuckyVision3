////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  CollisionNode.h
//
//  Struct template for a quad tree node.
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

#ifndef MCK_COLL_NODE_H
#define MCK_COLL_NODE_H

#include<set>

namespace MCK
{

// Forward declaration
class SpriteCollisionRect;

struct CollisionNode
{
    //! Sprites present in node
    // Note: referenced here by sprite collision base
    std::set<std::shared_ptr<MCK::SpriteCollisionRect>> sprites;

    //! Count of sprites in all subnodes, used to prune tree searches
    uint16_t sub_node_sprite_count;

    CollisionNode( void )
    {
        this->sub_node_sprite_count = 0;
    }

    //! Add sprite to node
    /*! @returns true if sprite added ok, false if already present.*/
    bool add_sprite( std::shared_ptr<MCK::SpriteCollisionRect> sprite )
    {
        return this->sprites.insert( sprite ).second;
    }
};

}  // End of namespace MCK

#endif
