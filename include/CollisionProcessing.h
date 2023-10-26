////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  CollisionProcessing.h
//
//  Collision processing for sprites.
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

#ifndef MCK_COLL_PROC_H
#define MCK_COLL_PROC_H

#include<CollisionEvent.h>
#include<CollisionNode.h>
#include<QuadTree.h>

namespace MCK
{

template<class T, class CONTENT>
class CollisionProcessing
{
    public:

        CollisionProcessing( void ) {}

        void init(
            uint8_t _levels,
            MCK::Point<T> top_left_pixel_coords,
            MCK::Point<T> size_in_pixels
        )
        {
            if( _levels > MCK::MAX_QUAD_TREE_LEVELS )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Quad tree creation failed, " )
                    + std::string( "requested levels " )
                    + std::to_string( int( _levels ) )
                    + std::string( " exceeds max of " )
                    + std::to_string( int( MCK::MAX_QUAD_TREE_LEVELS ) )
#else
                    ""
#endif
                ) );
            }

            this->levels = _levels;

            try
            {
                // Build quad tree, with (default) empty content 
                MCK::QuadTree<T,CONTENT>* parent_node = NULL;
                this->quad_tree = std::make_shared<MCK::QuadTree<T,CONTENT>>(
                    this->levels,
                    parent_node,
                    top_left_pixel_coords,
                    size_in_pixels
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Failed to build quad tree, error = " )
                    + e.what()
#else
                    ""
#endif
                ) );
            }

#if defined MCK_STD_OUT && defined MCK_VERBOSE
            std::cout << "Created collision tree with "
                      << int( this->levels ) << " levels."
                      << std::endl;
#endif
        }

        void process(
            std::vector<MCK::CollisionEvent> &collisions
        ) const
        {
            // Throw if not yet init
            if( this->quad_tree.get() == NULL )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot process collisions " )
                    + std::string( "as collision processing " )
                    + std::string( "not yet init." )
#else
                    ""
#endif
                ) );
            }

            // Start recursion from top of quad tree
            std::set<MCK::SpriteCollisionBase> sprites_to_be_tested;
            try
            {
                process_recursion(
                    collisions,
                    static_cast<const MCK::QuadTreeLeaf<T,CONTENT>*>(
                        &*( this->quad_tree )
                    ),
                    sprites_to_be_tested
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Failed to build quad tree, error = " )
                    + e.what()
#else
                    ""
#endif
                ) );
            }
        }

        // Returns true if collision tree has been built
        bool is_initialized( void ) const noexcept
        {
            return this->quad_tree.get() != NULL;
        }

        //! Number of levels in collision tree
        uint8_t get_levels( void ) const noexcept
        {
            return this->levels;
        }

    protected:

        // Process recursion for non-team sprites
        template<typename U = CONTENT, typename std::enable_if<std::is_same<U,MCK::CollisionNode>::value, bool>::type = true>
        void process_recursion(
            std::vector<MCK::CollisionEvent> &collisions,
            const MCK::QuadTreeLeaf<T,MCK::CollisionNode>* node,
            std::set<MCK::SpriteCollisionBase> &sprites_to_be_tested
        ) const
        {
            if( node == NULL )
            {
                return;
            }

            // DEBUG
            {
                MCK::CollisionEvent temp;
                collisions.push_back( temp );
            }

            // TODO check collisions and update 'sprites_to_be_tested'

            // If not leaf node, process sub-nodes
            if( node->is_non_leaf() )
            {
                // Top left sub-node
                try
                {
                    process_recursion(
                        collisions,
                        node->get_top_left_sub_node(),
                        sprites_to_be_tested    
                    );
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Failed to process top left " )
                        + std::string( " sub node, error = " )
                        + e.what()
#else
                        ""
#endif
                    ) );
                }

                // Top right sub-node
                try
                {
                    process_recursion(
                        collisions,
                        node->get_top_right_sub_node(),
                        sprites_to_be_tested    
                    );
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Failed to process top right " )
                        + std::string( " sub node, error = " )
                        + e.what()
#else
                        ""
#endif
                    ) );
                }

                // Bottom left sub-node
                try
                {
                    process_recursion(
                        collisions,
                        node->get_bottom_left_sub_node(),
                        sprites_to_be_tested    
                    );
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Failed to process bottom left " )
                        + std::string( " sub node, error = " )
                        + e.what()
#else
                        ""
#endif
                    ) );
                }

                // Bottom right sub-node
                try
                {
                    process_recursion(
                        collisions,
                        node->get_bottom_right_sub_node(),
                        sprites_to_be_tested    
                    );
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Failed to process bottom right " )
                        + std::string( " sub node, error = " )
                        + e.what()
#else
                        ""
#endif
                    ) );
                }

                // TODO: Process other subnodes
            }
        }

        // TODO Sprite team version of process_recursion 

        uint8_t levels;

        std::shared_ptr<MCK::QuadTree<T,CONTENT>> quad_tree;
};

}  // End of namespace MCK
        
#endif
