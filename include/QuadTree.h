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

#include <stdexcept>  // For exceptions
#include <memory>  // For shared pointer
#include <map>
#include <set>
//#include<vector>

#include "Defs.h"
#include "Point.h"
// #include "SpritePos.h"

#if defined MCK_STD_OUT
#include <string>
#endif

namespace MCK
{

// Forward declaration
class SpritePos;

// Common characteristics for both leaf and no-leaf nodes
template<class T>
class QuadTreeCommon
{
    public:

        //! Default constructor
        QuadTreeCommon( void )
        {
            this->collision_team_flags = 0;
        }

        //! Constructor
        QuadTreeCommon( 
           MCK::Point<T> _top_left,
           MCK::Point<T> _size
        ) : top_left( _top_left ), size( _size )
        {
            this->collision_team_flags = 0;
        }

        //! Get top left corner of cell
        MCK::Point<T>& get_top_left( void ) const noexcept
        {
            return this->top_left;
        }

        //! Get size of cell
        MCK::Point<T>& get_size( void ) const noexcept
        {
            return this->size;
        }

        //! Get collision team flags
        MCK_COLL_TEAM_DATA_TYPE get_collision_team_flags( void ) const noexcept
        {
            return this->collision_team_flags;
        }

        //! Get sprites by collision team
        const std::set<std::shared_ptr<MCK::SpritePos>>& get_sprites_by_team( uint8_t team_id ) const
        {
            std::map<
                uint8_t,
                std::set<
                    std::shared_ptr<MCK::SpritePos>
                >
            >::const_iterator it = this->sprites_by_team.find( team_id );
            if( it == this->sprites_by_team.end() )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Failed to get sprites for quad tree cell " )
                    + std::string( "as sprite team id " )
                    + std::to_string( team_id )
                    + std::string( " not recognized." ) 
#else
                    ""
#endif
                ) );
            }

            return it->second;
        }
        
        //! Add sprite
        void add_sprite(
            std::shared_ptr<MCK::SpritePos> sprite,
            uint8_t team_id
        )
        {
            // Let calling program handle any exception here
            std::set<std::shared_ptr<MCK::SpritePos>>& team
                = this->get_sprites_by_team_mutable( team_id );

            // If no sprites of that team are currently in this
            // cell, update flags to indicate a sprite of that
            // team is now present
            if( team.size() == 0 )
            {
                this->collision_team_flags
                    |= ( MCK_COLL_TEAM_DATA_TYPE( 1 ) << team_id );
            }

            // Add sprite to team (ignore any failure to add sprite
            // due to the sprites already being present)
            this->team.insert( sprite );
        }

        //! Remove sprite
        void remove_sprite(
            std::shared_ptr<MCK::SpritePos> sprite,
            uint8_t team_id
        )
        {
            // Let calling program handle any exception here
            std::set<std::shared_ptr<MCK::SpritePos>>& team
                = this->get_sprites_by_team( team_id );

            // Find the sprite and erase (if exists)
            auto it = team.find( sprite );
            if( it != team.end() )
            {
                team.erase( it );
            }

            // If no sprites of that team are currently in this
            // cell, remove corresponding team flag.
            if( team.size() == 0 )
            {
                this->collision_team_flags 
                    &= ~( MCK_COLL_TEAM_DATA_TYPE( 1 ) << team_id );
            }
        }

        //! Returns true if any sprite from any of the teams indicated is/are present in cell
        /* @param flags: Teams are indicated by setting 'n'th bit to 1, where n = team id.*/
        bool multiple_teams_present( MCK_COLL_TEAM_DATA_TYPE flags ) const noexcept
        {
            return bool( flags | this->collision_team_flags );
        }

        //! Returns true if any sprite from the team indicated is present in the cell
        bool team_present( uint8_t team_id ) const noexcept
        {
            return bool(
                ( MCK_COLL_TEAM_DATA_TYPE( 1 ) << team_id )
                    | this->collision_team_flags
            );
        }

        //! Clear all sprites from cell
        void clear_all_sprites( void ) noexcept
        {
            this->sprites_by_team.clear();
            this->collision_team_flags = 0;
        }


    protected:

        //! Get sprites by collision team (mutable)
        std::set<std::shared_ptr<MCK::SpritePos>>& get_sprites_by_team_mutable( uint8_t team_id )
        {
            std::map<
                uint8_t,
                std::set<
                    std::shared_ptr<MCK::SpritePos>
                >
            >::iterator it = this->sprites_by_team.find( team_id );
            if( it == this->sprites_by_team.end() )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Failed to get sprites for quad tree cell " )
                    + std::string( "(mutable) as sprite team id " )
                    + std::to_string( team_id )
                    + std::string( " not recognized." ) 
#else
                    ""
#endif
                ) );            
            }

            return it->second;
        }
        
        // Top left coord of cell
        MCK::Point<T> top_left;

        // Size of cell
        MCK::Point<T> size;

        // This integer contains one bit for each
        // sprite collision team. If a bit is set to 1,
        // the cell contains a sprite belonging to the
        // corresponding team.
        MCK_COLL_TEAM_DATA_TYPE collision_team_flags;

        // Sprites in cell, indexed by team
        std::map<
            uint8_t,
            std::set<
                std::shared_ptr<MCK::SpritePos>
            >
        > sprites_by_team;
};

// Non-leaf node ( i.e. when LEVEL > 0 )
template<class T, uint8_t LEVEL>
class QuadTree : public QuadTreeCommon<T>
{
    public:

        //! Default constructor
        QuadTree( void ) : QuadTreeCommon<T>()
        {
            parent_cell = NULL;
        }

        //! Constructor
        QuadTree( 
            MCK::Point<T> _top_left,
            MCK::Point<T> _size,
            T split_ratio_hoz = 0.5f,
            T split_ratio_vert = 0.5f,
            QuadTree<T,LEVEL+1>* _parent_cell = NULL
        ) : QuadTreeCommon<T>( _top_left, _size ),
            parent_cell( _parent_cell )
        {
            // If horizontal split ratio out-of-range, default to 0.5f
            if( split_ratio_hoz < 0.0f || split_ratio_hoz > 1.0f )
            {
                split_ratio_hoz = 0.5f;
            }

            // If vertical split ratio out-of-range, default to 0.5f
            if( split_ratio_vert < 0.0f || split_ratio_vert > 1.0f )
            {
                split_ratio_vert = 0.5f;
            }

            // Calculate size of top-left sub-cell
            const MCK::Point<T> TOP_LEFT_SUB_CELL_SIZE
                = this->size.scale(
                    split_ratio_hoz,
                    split_ratio_vert
                );

            // Calculate point at which the sub-cells meet
            this->split_point = this->top_left + TOP_LEFT_SUB_CELL_SIZE;

            /*
            // DEBUG
            std::cout << "QuadTree cell: LEVEL = " << int( LEVEL )
                      << ", top_left = "
                      << this->top_left.str()
                      << ", size = "
                      << this->size.str()
                      << ", split_point = "
                      << this->split_point.str()
                      << ", parent_cell = "
                      << this->parent_cell
                      << std::endl;
            */

            // Create sub-cells recursively......
            this->sub_cells.reserve( 4 );
            
            // Create top left sub cell
            // Note decrement of 'LEVEL' template argument
            this->sub_cells.push_back(
                MCK::QuadTree<T,LEVEL-1>(
                    this->top_left,
                    TOP_LEFT_SUB_CELL_SIZE,
                    split_ratio_hoz,
                    split_ratio_vert,
                    this
                )
            );

            // Create top right sub cell
            this->sub_cells.push_back(
                MCK::QuadTree<T,LEVEL-1>(
                    MCK::Point<T>(
                        this->split_point.get_x(),
                        this->top_left.get_y()
                    ),
                    this->size.scale(
                        1.0f - split_ratio_hoz,
                        split_ratio_vert
                    ),
                    split_ratio_hoz,
                    split_ratio_vert,
                    this
                )
            );

            // Create bottom left sub cell
            this->sub_cells.push_back(
                MCK::QuadTree<T,LEVEL-1>(
                    MCK::Point<T>(
                        this->top_left.get_x(),
                        this->split_point.get_y()
                    ),
                    this->size.scale(
                        split_ratio_hoz,
                        1.0f - split_ratio_vert
                    ),
                    split_ratio_hoz,
                    split_ratio_vert,
                    this
                )
            );

            // Create bottom right sub cell
            this->sub_cells.push_back(
                MCK::QuadTree<T,LEVEL-1>(
                    this->split_point,
                    this->size.scale(
                        1.0f - split_ratio_hoz,
                        1.0f - split_ratio_vert
                    ),
                    split_ratio_hoz,
                    split_ratio_vert,
                    this
                )
            );
        }

        virtual ~QuadTree( void ) {}

        //! Get parent cell
        MCK::QuadTree<T,LEVEL+1>* get_parent_cell( void ) const noexcept
        {
            return parent_cell;
        }

        //! Is top node
        bool is_top_node( void ) const noexcept
        {
            return parent_cell == NULL;
        }


    protected:

        // Parent cell
        MCK::QuadTree<T,LEVEL+1>* parent_cell;

        // Junction point of the four sub-cells
        MCK::Point<T> split_point;

        // Sub-cells (always either zero four)
        // Note: Zero indicates this is a leaf of the quad tree
        std::vector<MCK::QuadTree<T,LEVEL-1>> sub_cells;
};

// Leaf node (i.e. LEVEL == 0)
template<class T>
class QuadTree<T,0> : public QuadTreeCommon<T>
{
    public:

        //! Default constructor
        QuadTree( void ) : QuadTreeCommon<T>()
        {
            parent_cell = NULL;
        }

        //! Constructor
        QuadTree(
            MCK::Point<T> _top_left,
            MCK::Point<T> _size,
            T dummy_1 = 0.5f,  // 'split_ratio_hoz' not required
            T dummy_2 = 0.5f, // 'split_ratio_vert' not required
            QuadTree<T,1>* _parent_cell = NULL
        ) : QuadTreeCommon<T>( _top_left, _size ),
            parent_cell( _parent_cell )
        {
            /*
            // DEBUG
            std::cout << "QuadTree cell: LEVEL = 0"
                      << ", top_left = "
                      << this->top_left.str()
                      << ", size = "
                      << this->size.str()
                      << ", parent_cell = "
                      << this->parent_cell
                      << std::endl;
            */
        }

        virtual ~QuadTree( void ) {}
    
        //! Get parent cell
        MCK::QuadTree<T,1>* get_parent_cell( void ) const noexcept
        {
            return parent_cell;
        }

        //! Is top node
        bool is_top_node( void ) const noexcept
        {
            return parent_cell == NULL;
        }


    protected:

        // Parent cell
        MCK::QuadTree<T,1>* parent_cell;
};

// Absolute top node (i.e. LEVEL == MCK::MAX_QUAD_TREE_LEVEL+1)
// This class is empty but is necessary to prevent infinite
// recursion from the 'QuadTree<T,LEVEL+1> parent_cell' member
// in the generic-level QuadTree template (above)
template<class T>
class QuadTree<T,MCK::MAX_QUAD_TREE_LEVEL+1> : public QuadTreeCommon<T>{};

}  // End of namespace MCK

#endif
