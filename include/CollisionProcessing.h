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
            
            // Reserving space for the working sprite vector
            // is important for efficiency.
            sprites_to_be_tested.clear();
            sprites_to_be_tested.reserve( MCK::MAX_SPRITES );

            // Build quad tree, with (default) empty content 
            try
            {
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


        //! Check for sprite collisions
        void process(
            std::vector<MCK::CollisionEvent> &collisions
        )
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

            // Clear any sprite pointers remaining from last process
            sprites_to_be_tested.clear();

            // Start recursion from top of quad tree
            try
            {
                process_recursion(
                    collisions,
                    static_cast<const MCK::QuadTreeLeaf<T,CONTENT>*>(
                        &*( this->quad_tree )
                    )
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

        //! Add sprite to collision tree
        /*! @param node: For internal use only, user should keep as default (NULL)
         *  @returns: True if added okay, false if already present
         * Note: This method is called recursively, using 'node'.
         *       Calling methods should set 'node' to NULL (default)         */
        bool add_sprite(
            std::shared_ptr<MCK::SpriteCollisionRect> sprite,
            T left,
            T top,
            T right,
            T bottom,
            MCK::QuadTreeLeaf<T,CONTENT>* node = NULL
        )
        {
            // If 'node' is NULL, use top node of quad tree
            if( node == NULL )
            {
                // Quality checks
                if( left > right || top > bottom )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Failed to add sprite as bounding box " )
                        + std::string( "inverted hoz and/or vert (left=" )
                        + std::to_string( left )
                        + std::string( ",top=" )
                        + std::to_string( top )
                        + std::string( ",right=" )
                        + std::to_string( left )
                        + std::string( ",bottom=" )
                        + std::to_string( left )
#else
                        ""
#endif
                    ) );
                }

                if( this->quad_tree.get() == NULL )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Failed to add sprite as collision " )
                        + std::string( "processing not yet init." )
#else
                        ""
#endif
                    ) );
                }

                node = static_cast<
                            MCK::QuadTreeLeaf<T,CONTENT>*
                       >(
                            &*(this->quad_tree)
                       );
            }
            
            // If leaf, add sprite and return
            if( !node->is_non_leaf() )
            {
                // DEBUG
                std::cout << "Adding sprite to leaf node "
                          << node->get_top_left().str()
                          << "," << node->get_size().str()
                          << std::endl;

                // Add sprite, return true if added ok,
                // false if already present
                return node->get_content_mutable()
                    ->add_sprite( sprite );
            }

            // If non-leaf node,
            // check if entire bounding box lies on one side
            // of split point, hoz and/or vert
            MCK::QuadTree<T,CONTENT>* non_leaf_node
                = static_cast<MCK::QuadTree<T,CONTENT>*>( node );
            bool left_side, right_side, top_side, bottom_side;
            uint8_t side_count = 0;

            const T MID_X = non_leaf_node->get_split_point().get_x();
            side_count += left_side = right <= MID_X;
            side_count += right_side = left >= MID_X; 
            
            const T MID_Y = non_leaf_node->get_split_point().get_y();
            side_count += top_side = bottom <= MID_Y;
            side_count += bottom_side = top >= MID_Y; 

            // If bounding box includes split point,
            // ignore sub-nodes and
            // assign sprite to this node, then return
            if( side_count < 2 )
            {
                // DEBUG
                std::cout << "Adding sprite to non-leaf node "
                          << node->get_top_left().str()
                          << "," << node->get_size().str()
                          << std::endl;

                // Add sprite, return true if added ok,
                // false if already present
                return node->get_content_mutable()
                    ->add_sprite( sprite );
            }

            // Determine which sub-node to add sprite to
            MCK::QuadTreeLeaf<T,CONTENT>* sub_node;
            if( left_side )
            {
                if( top_side )
                {
                    // DEBUG
                    std::cout << "Adding sprite to top-left sub-node"
                              << std::endl;
                    sub_node = non_leaf_node->get_top_left_sub_node();
                }
                else
                {
                    // DEBUG
                    std::cout << "Adding sprite to bottom-left sub-node"
                              << std::endl;
                    sub_node = non_leaf_node->get_bottom_left_sub_node();
                }
            }
            else
            {
                if( top_side )
                {
                    // DEBUG
                    std::cout << "Adding sprite to top-right sub-node"
                              << std::endl;
                    sub_node = non_leaf_node->get_top_right_sub_node();
                }
                else
                {
                    // DEBUG
                    std::cout << "Adding sprite to bottom-right sub-node"
                              << std::endl;
                    sub_node = non_leaf_node->get_bottom_right_sub_node();
                }
            }

            bool rc;
            try
            {
                rc = this->add_sprite(
                    sprite,
                    left,
                    top,
                    right,
                    bottom,
                    sub_node
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Failed to add sprite to sub node, rror = " )
                    + e.what()
#else
                    ""
#endif
                ) );
            }

            // If successful, increase sub-node sprite count
            if( rc )
            {
                node->get_content_mutable()
                    ->sub_node_sprite_count++;

                return true;
            }
        
            // If reached this point, sprite has not been added
            // so return false
            return false;
        }

    protected:

        // Process recursion for non-team sprites
        template<typename U = CONTENT, typename std::enable_if<std::is_same<U,MCK::CollisionNode>::value, bool>::type = true>
        void process_recursion(
            std::vector<MCK::CollisionEvent> &collisions,
            const MCK::QuadTreeLeaf<T,MCK::CollisionNode>* node
        )
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

            // Get pointer to node content (this cannot be NULL)
            const MCK::CollisionNode* const COLL_NODE = node->get_content();

            // Get pointer to the node's sprites
            const std::set<std::shared_ptr<MCK::SpriteCollisionRect>>* const SPRITES
                = &COLL_NODE->sprites;

            // Test for collisions between the sprites in this node,
            // themselves, and 'sprites_to_be_tested'
            for( auto sprite_1 : *SPRITES )
            {
                // Test against other sprites in this node
                for( auto sprite_2 : *SPRITES )
                {
                    if( sprite_1 != sprite_2 )
                    {
                        // TODO
                    }
                }

                // Test against sprites from higher nodes
                for( auto sprite_2 : sprites_to_be_tested )
                {
                    // TODO
                }
            }

            // If node is non-leaf and there are sprites present
            // in sub-nodes, process sub-nodes
            if( node->is_non_leaf() )
            {
                // Add node's sprites to 'sprites_to_be_tested'
                for( auto sprite : *SPRITES )
                {
                    sprites_to_be_tested.push_back( sprite );
                }

                // Top left sub-node
                try
                {
                    process_recursion(
                        collisions,
                        node->get_top_left_sub_node()
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
                        node->get_top_right_sub_node()
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
                        node->get_bottom_left_sub_node()
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
                        node->get_bottom_right_sub_node()
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
                
                // Remove node's sprites from 'sprites_to_be_tested'
                // before unwinding the recursion further
                const size_t NUM_SPRITES = SPRITES->size();
                if( NUM_SPRITES >= sprites_to_be_tested.size() )
                {
                    sprites_to_be_tested.clear();
                }
                else
                {
                    for( int i = 0; i < NUM_SPRITES; i++ )
                    {
                        sprites_to_be_tested.pop_back();
                    }
                }
            }
        }

        uint8_t levels;

        std::vector<std::shared_ptr<MCK::SpriteCollisionRect>> sprites_to_be_tested;

        std::shared_ptr<MCK::QuadTree<T,CONTENT>> quad_tree;
};

}  // End of namespace MCK
        
#endif
