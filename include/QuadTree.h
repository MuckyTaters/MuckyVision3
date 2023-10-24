////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  QuadTree.h
//
//  Struct template for a quad tree cell.
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

#ifndef MCK_QUAD_TREE_H
#define MCK_QUAD_TREE_H

#include<memory>  // For shared pointer
#include<map>
#include<set>
#include<vector>

#include "Defs.h"
#include "Point.h"
#include "SpritePos.h"

#if defined MCK_STD_OUT
#include <string>
#endif

namespace MCK
{

template<class T, uint8_t LEVEL = 0>
struct QuadTree
{
    //! Default constructor
    QuadTree( void )
    {
        this->collision_teams_present = 0;
    }

    //! Constructor
    QuadTree( MCK::Point<T> size )
    {
        this->collision_teams_present = 0;
        this->split_point = size * 0.5f;

        if( LEVEL > 0 )
        {
            /*
            // DEBUG
            std::cout << "LEVEL = " << int( LEVEL )
                      << ", split_point = "
                      << this->split_point.str()
                      << std::endl;
            */

            // This recursively calls this constructor
            this->sub_cells.reserve( 4 );
            this->sub_cells.push_back(
                MCK::QuadTree<T,LEVEL-1>(
                    MCK::Point<T>(  // Top left
                        size.get_x() * 0.25f,
                        size.get_y() * 0.25f
                    )    
                )
            );
            this->sub_cells.push_back(
                MCK::QuadTree<T,LEVEL-1>(
                    MCK::Point<T>(  // Top right
                        size.get_x() * 0.75f,
                        size.get_y() * 0.25f
                    )
                )
            );
            this->sub_cells.push_back(
                MCK::QuadTree<T,LEVEL-1>(
                    MCK::Point<T>(  // Bottom left
                        size.get_x() * 0.25f,
                        size.get_y() * 0.75f
                    )
                )
            );
            this->sub_cells.push_back(
                MCK::QuadTree<T,LEVEL-1>(
                    MCK::Point<T>(  // Bottom right
                        size.get_x() * 0.75f,
                        size.get_y() * 0.75f
                    )
                )
            );
        }
    }

    virtual ~QuadTree( void ) {}

    // Junction point of the four sub-cells
    MCK::Point<T> split_point;

    // Sub-cells (always either zero four)
    // Note: Zero indicates this is a leaf of the quad tree
    std::vector<MCK::QuadTree<T,LEVEL-1>> sub_cells;

    // This integer contains one bit for each
    // sprite collision team. If a bit is set to 1,
    // the cell contains a sprite belonging to the
    // corresponding team.
    MCK_COLL_TEAM_DATA_TYPE collision_teams_present;

    // Sprites in cell, indexed by team
    std::map<
        uint8_t,
        std::set<
            std::shared_ptr<MCK::SpritePos>
        >
    > sprites_by_team;
};

// Leaf node
template<class T>
class QuadTree<T,0>
{
    public:

        //! Default (leaf node) constructor
        QuadTree( void )
        {
            this->collision_teams_present = 0;
        }

        //! Constructor (for non-leaf nodes)
        QuadTree( MCK::Point<T> size )
        {
            this->collision_teams_present = 0;
        }

        virtual ~QuadTree( void ) {}


    protected:

        // This integer contains one bit for each
        // sprite collision team. If a bit is set to 1,
        // the cell contains a sprite belonging to the
        // corresponding team.
        MCK_COLL_TEAM_DATA_TYPE collision_teams_present;

        // Sprites in cell, indexed by team
        std::map<
            uint8_t,
            std::set<
                std::shared_ptr<MCK::SpritePos>
            >
        > sprites_by_team;
};

}  // End of namespace MCK

#endif

