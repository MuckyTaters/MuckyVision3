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
#include<SpriteCollisionRect.h>
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

        //! Include sprite in collision processing
        /*! @returns: True is sprite added okay, false is already present.*/
        bool add_sprite(
            std::shared_ptr<MCK::SpriteCollisionRect> sprite
        )
        {
            if( sprite.get() == NULL )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot add sprite as sprite " )
                    + std::string( "pointer is NULL." )
#else
                    ""
#endif
                ) );
            }

            // Get sprite bounds
            float left, top, right, bottom;
            sprite->get_bounds( left, top, right, bottom );
            
            // Start the recursion to find correct place
            // in collision tree.
            // Let calling program catch any exception here.
            return this->add_sprite_recursive(
                sprite,
                left,
                top,
                right,
                bottom
            );
        }

        //! Update sprite position
        /*! IMPORTANT: The specified sprite MUST be associated
         *             with this collision tree or undefined
         *             behavio(u)r will occur, possibly a seg fault
         */
        void update_sprite_pos(
            std::shared_ptr<MCK::SpriteCollisionRect> sprite
        )
        {
            // If sprite pointer is NULL, ignore
            if( sprite.get() == NULL )
            {
                return;
            }

            // Get collision tree node holding sprite
            QuadTreeLeaf<T,CONTENT>* node = 
                ( QuadTreeLeaf<T,CONTENT>* )
                    ( sprite->get_quad_tree_node() );
        
            // If node is NULL, abort
            if( sprite->get_quad_tree_node() == NULL )
            {
                return;
            }

            // Get sprite bounds
            T sprite_left, sprite_top, sprite_right, sprite_bottom;
            sprite->get_bounds(
                sprite_left,
                sprite_top,
                sprite_right,
                sprite_bottom
            );

            // Declare pointer to new node
            QuadTreeLeaf<T,CONTENT>* sprites_new_node = NULL;
            
            // Call recursion to check if sprite needs moving
            // to another node.
            try
            {
                check_if_sprite_needs_moving_to_another_node( 
                    node, 
                    sprite,
                    sprite_left,
                    sprite_top,
                    sprite_right,
                    sprite_bottom,
                    sprites_new_node
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Error in move sprite check: " )
                    + e.what()
#else
                    ""
#endif
                ) );
            }
        
            // Check if sprite needs moving to another node
            if( sprites_new_node != NULL
                && node != sprites_new_node )
            {
                try
                {
                    if( sprites_new_node->get_content_mutable()
                            ->add_sprite( sprite ) )
                    {
                        node->get_content_mutable()
                            ->remove_sprite( sprite );
                    }
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Error in add/remove sprite: " )
                        + e.what()
#else
                        ""
#endif
                    ) );
                }

                // Change node pointer in sprite
                sprite->set_quad_tree_node( sprites_new_node );

            }
        }

        //! Check for sprite collisions
        void process(
            std::vector<MCK::CollisionEvent> &collisions
        )
        {
            collisions.clear();

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

    protected:

        //! Process recursion for 'add_sprite'
        bool add_sprite_recursive(
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
                // Add sprite
                bool rc = node->get_content_mutable()
                    ->add_sprite( sprite );
                
                // If added okay, store pointer to node in sprite
                if( rc )
                {
                    sprite->set_quad_tree_node( 
                        ( void* )( node )
                    );
                }
                
                // Return true if added ok,
                // false if already present
                return rc;
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
                // Add sprite
                bool rc = node->get_content_mutable()
                    ->add_sprite( sprite );
                
                // If added okay, store pointer to node in sprite
                if( rc )
                {
                    sprite->set_quad_tree_node( 
                        ( void* )( node )
                    );
                }
                
                // Return true if added ok,
                // false if already present
                return rc;
            }

            // Determine which sub-node to add sprite to
            MCK::QuadTreeLeaf<T,CONTENT>* sub_node;
            if( left_side )
            {
                if( top_side )
                {
                    sub_node = non_leaf_node->get_top_left_sub_node();
                }
                else
                {
                    sub_node = non_leaf_node->get_bottom_left_sub_node();
                }
            }
            else
            {
                if( top_side )
                {
                    sub_node = non_leaf_node->get_top_right_sub_node();
                }
                else
                {
                    sub_node = non_leaf_node->get_bottom_right_sub_node();
                }
            }

            bool rc;
            try
            {
                rc = this->add_sprite_recursive(
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

        // Process recursion for non-team sprites
        template<typename U = CONTENT, typename std::enable_if<std::is_same<U,MCK::CollisionNode>::value, bool>::type = true>
        void process_recursion(
            std::vector<MCK::CollisionEvent> &collisions,
            const MCK::QuadTreeLeaf<T,MCK::CollisionNode>* node
        )
        {
            if( node == NULL )
            {
                // DEBUG
                std::cout << "NULL node." << std::endl;
            
                return;
            }

            // Get pointer to node content (this cannot be NULL)
            const MCK::CollisionNode* const COLL_NODE = node->get_content();

            // Get pointer to the node's sprites
            const std::set<std::shared_ptr<MCK::SpriteCollisionRect>>* const SPRITES
                = &COLL_NODE->sprites;

            // Test for collisions between the sprites in this node,
            // themselves, and 'sprites_to_be_tested'
            std::set<std::shared_ptr<MCK::SpriteCollisionRect>>::const_iterator it1;

            // for( auto sprite_1 : *SPRITES )
            for( it1 = SPRITES->begin();
                 it1 != SPRITES->end();
                 it1++
            )
            {
                // Test against other sprites in this node
                std::set<std::shared_ptr<MCK::SpriteCollisionRect>>::const_iterator it2 = it1; 
                it2++;
                for( it2;
                     it2 != SPRITES->end();
                     it2++
                )
                {
                    try
                    {
                        // Regular version
                        MCK::CollisionProcessing<T,CONTENT>::check_collision(
                            collisions,
                            *it1,
                            *it2
                        );
                    }
                    catch( std::exception &e )
                    {
                        throw( std::runtime_error(
#if defined MCK_STD_OUT
                            std::string( "Failed to check collision, error = " )
                            + e.what()
#else
                            ""
#endif
                        ) );
                    }
                }

                // Test against sprites from higher nodes
                for( auto sprite_2 : sprites_to_be_tested )
                {
                    try
                    {

                        // Regular version
                        MCK::CollisionProcessing<T,CONTENT>::check_collision(
                            collisions,
                            *it1,
                            sprite_2
                        );
                    }
                    catch( std::exception &e )
                    {
                        throw( std::runtime_error(
#if defined MCK_STD_OUT
                            std::string( "(2)Failed to check collision, error = " )
                            + e.what()
#else
                            ""
#endif
                        ) );
                    }
                }
            }

            // If node is non-leaf and there are sprites present
            // in sub-nodes, process sub-nodes
            if( node->is_non_leaf() 
                // && node->get_content()->sub_node_sprite_count > 0
            )
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

        //! Recursive method for moving sprite to another node
        void check_if_sprite_needs_moving_to_another_node( 
            QuadTreeLeaf<T,CONTENT>* node, 
            std::shared_ptr<MCK::SpriteCollisionRect> sprite,
            T sprite_left,
            T sprite_top,
            T sprite_right,
            T sprite_bottom,
            QuadTreeLeaf<T,CONTENT>* &sprites_new_node, 
            bool called_from_parent_node = false
        )
        {
            // Get node bounds
            T node_left, node_top,
              node_right, node_bottom;
            
            node->get_top_left().get_xy(
                node_left,
                node_top
            );
            
            node->get_bottom_right().get_xy(
                node_right,
                node_bottom
            );

            bool sprite_belongs_in_this_node = true;

            // If method was NOT called from parent node
            // then check if sprite bounds exceeds node bounds.
            if( !called_from_parent_node
                && ( sprite_left < node_left
                  || sprite_top < node_top
                  || sprite_right > node_right
                  || sprite_bottom > node_bottom
                )
            )
            {
                // IMPORTANT: Do NOT combine the top node check
                //            here with the outer 'if' statement,
                //            as the child node checks in the
                //            'else' block below will then be 
                //            logically flawed.
                
                // If this is NOT top_node, check if it belongs
                // to parent node
                if( !node->is_top_node() )
                {
                    sprite_belongs_in_this_node = false;

                    // Let calling method deal with any exception here
                    this->check_if_sprite_needs_moving_to_another_node( 
                        node->get_parent_node(),
                        sprite,
                        sprite_left,
                        sprite_top,
                        sprite_right,
                        sprite_bottom,
                        sprites_new_node 
                    );
                }

                // If the node IS the top node, the sprite
                // belongs to this node so no action required here.
            }
            // Otherwise, check if node is non-leaf
            // and if so chech if sprite now fits
            // within a sub-node
            // Note: do this even if calling method
            //       was a sub-node, as sprite may now fit
            //       within a different sub-node.
            else if( node->is_non_leaf() )
            {
                // Recast 'node' as non-leaf node pointer
                MCK::QuadTree<T,CONTENT>* non_leaf_node
                    = static_cast<MCK::QuadTree<T,CONTENT>*>( node );
                
                // Get split point coords
                T split_x, split_y;
                non_leaf_node->get_split_point().get_xy( split_x, split_y );

                // Check if sprite fits within a sub-node
                if( sprite_right <= split_x )
                {
                    if( sprite_bottom <= split_y )
                    {
                        // Move to top left sub-node
                        sprite_belongs_in_this_node = false;
                    
                        // Let calling method deal with any exception here
                        this->check_if_sprite_needs_moving_to_another_node( 
                            non_leaf_node->get_top_left_sub_node(),
                            sprite,
                            sprite_left,
                            sprite_top,
                            sprite_right,
                            sprite_bottom,
                            sprites_new_node,
                            true  // Calling from parent node
                        );

                    }
                    else if( sprite_top >= split_y )
                    {
                        // Move to bottom left sub-node
                        sprite_belongs_in_this_node = false;
                    
                        // Let calling method deal with any exception here
                        this->check_if_sprite_needs_moving_to_another_node( 
                            non_leaf_node->get_bottom_left_sub_node(),
                            sprite,
                            sprite_left,
                            sprite_top,
                            sprite_right,
                            sprite_bottom,
                            sprites_new_node,
                            true  // Calling from parent node
                        );
                    }
                }
                else if( sprite_left >= split_x )
                {
                    if( sprite_bottom <= split_y )
                    {
                        // Move to top right sub-node
                        sprite_belongs_in_this_node = false;
                    
                        // Let calling method deal with any exception here
                        this->check_if_sprite_needs_moving_to_another_node( 
                            non_leaf_node->get_top_right_sub_node(),
                            sprite,
                            sprite_left,
                            sprite_top,
                            sprite_right,
                            sprite_bottom,
                            sprites_new_node,
                            true  // Calling from parent node
                        );
                    }
                    else if( sprite_top >= split_y )
                    {
                        // Move to bottom right sub-node
                        sprite_belongs_in_this_node = false;
                    
                        // Let calling method deal with any exception here
                        this->check_if_sprite_needs_moving_to_another_node( 
                            non_leaf_node->get_bottom_right_sub_node(),
                            sprite,
                            sprite_left,
                            sprite_top,
                            sprite_right,
                            sprite_bottom,
                            sprites_new_node,
                            true  // Calling from parent node
                        );
                    }
                }
            }

            // If it belong in this node, add sprite to this node
            if( sprite_belongs_in_this_node )
            {
                sprites_new_node = node;
            }
        }

        //! Determine
        static bool check_collision(
            std::vector<MCK::CollisionEvent> &collisions,
            std::shared_ptr<MCK::SpriteCollisionRect> sprite_1,
            std::shared_ptr<MCK::SpriteCollisionRect> sprite_2
        )
        {
            switch( sprite_1->get_collision_type() )
            {
                case MCK::SpriteCollisionType::RECT:
                    switch( sprite_2->get_collision_type() )
                    {
                        case MCK::SpriteCollisionType::RECT:
                            return false;
                            break;

                        case MCK::SpriteCollisionType::CIRCLE:
                            return false;
                            break;
                    }
                    break;

                case MCK::SpriteCollisionType::CIRCLE:

                    std::shared_ptr<MCK::SpriteCollisionCircle> circle_1
                        = std::dynamic_pointer_cast<MCK::SpriteCollisionCircle>( sprite_1 ); 

                    switch( sprite_2->get_collision_type() )
                    {
                        case MCK::SpriteCollisionType::RECT:
                            return false;
                            break;

                        case MCK::SpriteCollisionType::CIRCLE:
                    
                            std::shared_ptr<MCK::SpriteCollisionCircle> circle_2
                                = std::dynamic_pointer_cast<MCK::SpriteCollisionCircle>( sprite_2 ); 

                            const float TOTAL_RAD 
                                = circle_1->get_radius()
                                    + circle_2->get_radius();

                            const float DIST_SQ
                                = circle_1->get_dist_sq_from_center(
                                    circle_2->get_center_x(),
                                    circle_2->get_center_y()
                                );

                            if( DIST_SQ < TOTAL_RAD * TOTAL_RAD )
                            {
                                collisions.push_back(
                                    CollisionEvent(
                                        std::dynamic_pointer_cast<MCK::SpritePos>( sprite_1 ),
                                        std::dynamic_pointer_cast<MCK::SpritePos>( sprite_2 )
                                    )  
                                );

                                return true;
                            }
                            else
                            {
                                return false;
                            }

                            /*
                            // DEBUG - check bounds
                            const bool RECT_CHECK = 
                                circle_1->get_left_bound()
                                    < circle_2->get_right_bound()
                                && circle_1->get_left_bound()
                                    > circle_2->get_left_bound()
                                && circle_1->get_top_bound()
                                    < circle_2->get_bottom_bound()
                                && circle_1->get_top_bound() 
                                    > circle_2->get_top_bound();
                            
                            if( RECT_CHECK )
                            {
                                collisions.push_back(
                                    CollisionEvent(
                                        std::dynamic_pointer_cast<MCK::SpritePos>( sprite_1 ),
                                        std::dynamic_pointer_cast<MCK::SpritePos>( sprite_2 )
                                    )
                                );
                                collisions.back().circle_collision = false;
                                collisions.back().rect_collision = true;
                                
                                return true;
                            }
                            else
                            {
                                return false;
                            }
                            */

                            break;
                    }
                    break;
            }
                
            return false;
        }

        uint8_t levels;

        std::vector<std::shared_ptr<MCK::SpriteCollisionRect>> sprites_to_be_tested;

        std::shared_ptr<MCK::QuadTree<T,CONTENT>> quad_tree;
};


}  // End of namespace MCK
        
#endif
