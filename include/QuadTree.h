////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  QuadTree.h
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

#ifndef MCK_QUAD_TREE_H
#define MCK_QUAD_TREE_H

#include <stdexcept>  // For exceptions
#include <memory>  // For shared pointer

#include "Defs.h"
#include "Point.h"

#if defined MCK_STD_OUT
#include <string>
#endif

namespace MCK
{

// Forward declaration
template<class T, class CONTENT>
class QuadTree;

// Common characteristics for both leaf and no-leaf nodes
template<class T, class CONTENT>
class QuadTreeLeaf
{
    public:

        //! Default constructor
        QuadTreeLeaf( void )
        {
            this->parent_node = NULL;
            this->non_leaf = false;
        }

        //! Constructor
        QuadTreeLeaf(
            MCK::QuadTree<T,CONTENT>* _parent_node,
            MCK::Point<T> _top_left,
            MCK::Point<T> _size
        ) : parent_node( _parent_node ),
            top_left( _top_left ),
            size( _size )
        {
            QuadTreeLeaf<T,CONTENT>();
        }

        // Destructor
        virtual ~QuadTreeLeaf( void ) {}
        
        //! Get parent (non-leaf) node
        MCK::QuadTree<T,CONTENT>* get_parent_node( void ) const noexcept
        {
            return this->parent_node;
        }

        //! Get top left corner of node
        const MCK::Point<T>& get_top_left( void ) const noexcept
        {
            return this->top_left;
        }

        //! Get size of node
        const MCK::Point<T>& get_size( void ) const noexcept
        {
            return this->size;
        }

        //! Is top node
        bool is_top_node( void ) const noexcept
        {
            return this->parent_node == NULL;
        }

        //! Get read-only pointer to content
        const CONTENT* get_content( void ) const noexcept
        {
            return &content;
        }

        //! Get pointer to user content
        CONTENT* get_content_mutable( void ) const noexcept
        {
            return &content;
        }

        //! Returns true if this is actually the base of a non-leaf node
        bool is_non_leaf( void ) const noexcept
        {
            return non_leaf;
        }

        // These are accessors for QuadTree, defined here
        // so they may be called for a QuadTree instance
        // referenced only by a QuadTreeLeaf pointer.
        virtual const MCK::QuadTreeLeaf<T,CONTENT>* get_top_left_sub_node( void ) const noexcept
        {
            return NULL;
        }
        
        virtual const MCK::QuadTreeLeaf<T,CONTENT>* get_top_right_sub_node( void ) const noexcept
        {
            return NULL;
        }

        virtual const MCK::QuadTreeLeaf<T,CONTENT>* get_bottom_left_sub_node( void ) const noexcept
        {
            return NULL;
        }
        
        virtual const MCK::QuadTreeLeaf<T,CONTENT>* get_bottom_right_sub_node( void ) const noexcept
        {
            return NULL;
        }

    protected:

        // Parent node
        MCK::QuadTree<T,CONTENT>* parent_node;
        
        // Top left coord of node
        MCK::Point<T> top_left;

        // Size of node
        MCK::Point<T> size;

        // User content
        CONTENT content;

        // Set to true only for child class QuadTree
        // (i.e. provides reflection).
        bool non_leaf;
};

// Template for non-leaf nodes
template<class T, class CONTENT>
class QuadTree : public QuadTreeLeaf<T,CONTENT>
{
    public:

        //! Default constructor
        QuadTree( void ) : QuadTreeLeaf<T,CONTENT>()
        {
            this->non_leaf = true;
            this->top_left_sub_node = NULL;
            this->top_right_sub_node = NULL;
            this->bottom_left_sub_node = NULL;
            this->bottom_right_sub_node = NULL;
        }

        //! Constructor
        QuadTree(
            uint8_t level,
            MCK::QuadTree<T,CONTENT>* _parent_node,
            MCK::Point<T> _top_left,
            MCK::Point<T> _size,
            T split_ratio_hoz = 0.5f,
            T split_ratio_vert = 0.5f
        ) : QuadTreeLeaf<T,CONTENT>( _parent_node, _top_left, _size )
        {
            this->non_leaf = true;
            
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

            // Calculate size of top-left sub-node
            const MCK::Point<T> TOP_LEFT_SUB_CELL_SIZE
                = this->size.scale(
                    split_ratio_hoz,
                    split_ratio_vert
                );

            // Calculate point at which the sub-nodes meet
            this->split_point = this->top_left + TOP_LEFT_SUB_CELL_SIZE;

            // For levels above 1, create four non-leaf nodes
            if( level > 1 )
            {
                const uint8_t NEW_LEVEL = level - 1;

                // Create top left sub node
                this->top_left_sub_node =
                    static_cast<MCK::QuadTreeLeaf<T,CONTENT>*>(
                        new MCK::QuadTree<T,CONTENT>(
                            NEW_LEVEL,
                            this,
                            this->top_left,
                            TOP_LEFT_SUB_CELL_SIZE,
                            split_ratio_hoz,
                            split_ratio_vert
                        )
                    );

                // Create top right sub node
                this->top_right_sub_node =
                    static_cast<MCK::QuadTreeLeaf<T,CONTENT>*>(
                        new MCK::QuadTree<T,CONTENT>(
                            NEW_LEVEL,
                            this,
                            MCK::Point<T>(
                                this->split_point.get_x(),
                                this->top_left.get_y()
                            ),
                            this->size.scale(
                                1.0f - split_ratio_hoz,
                                split_ratio_vert
                            ),
                            split_ratio_hoz,
                            split_ratio_vert
                        )
                    );

                // Create bottom left sub node
                this->bottom_left_sub_node =
                    static_cast<MCK::QuadTreeLeaf<T,CONTENT>*>(
                        new MCK::QuadTree<T,CONTENT>(
                            NEW_LEVEL,
                            this,
                            MCK::Point<T>(
                                this->top_left.get_x(),
                                this->split_point.get_y()
                            ),
                            this->size.scale(
                                split_ratio_hoz,
                                1.0f - split_ratio_vert
                            ),
                            split_ratio_hoz,
                            split_ratio_vert
                        )
                    );

                // Create bottom right sub node
                this->bottom_right_sub_node =
                    static_cast<MCK::QuadTreeLeaf<T,CONTENT>*>(
                        new MCK::QuadTree<T,CONTENT>(
                            NEW_LEVEL,
                            this,
                            this->split_point,
                            this->size.scale(
                                1.0f - split_ratio_hoz,
                                1.0f - split_ratio_vert
                            ),
                            split_ratio_hoz,
                            split_ratio_vert
                        )
                    );    
            }
            // For level 1, create four leaf nodes
            else
            {
                // Create top left sub node
                this->top_left_sub_node =
                    new MCK::QuadTreeLeaf<T,CONTENT>(
                        this,
                        this->top_left,
                        TOP_LEFT_SUB_CELL_SIZE
                    );

                // Create top right sub node
                this->top_right_sub_node =
                    new MCK::QuadTreeLeaf<T,CONTENT>(
                        this,
                        MCK::Point<T>(
                            this->split_point.get_x(),
                            this->top_left.get_y()
                        ),
                        this->size.scale(
                            1.0f - split_ratio_hoz,
                            split_ratio_vert
                        )
                    );

                // Create bottom left sub node
                this->bottom_left_sub_node =
                    new MCK::QuadTreeLeaf<T,CONTENT>(
                        this,
                        MCK::Point<T>(
                            this->top_left.get_x(),
                            this->split_point.get_y()
                        ),
                        this->size.scale(
                            split_ratio_hoz,
                            1.0f - split_ratio_vert
                        )
                    );

                // Create bottom right sub node
                this->bottom_right_sub_node =
                    new MCK::QuadTreeLeaf<T,CONTENT>(
                        this,
                        this->split_point,
                        this->size.scale(
                            1.0f - split_ratio_hoz,
                            1.0f - split_ratio_vert
                        )
                    );
            }
        }

        // Destructor
        virtual ~QuadTree( void )
        {
            // Delete all sub nodes...

            if( this->top_left_sub_node != NULL )
            {
                delete this->top_left_sub_node;
            }
            
            if( this->top_right_sub_node != NULL )
            {
                delete this->top_right_sub_node;
            }
            
            if( this->bottom_left_sub_node != NULL )
            {
                delete this->bottom_left_sub_node;
            }
            
            if( this->bottom_right_sub_node != NULL )
            {
                delete this->bottom_right_sub_node;
            }
        }
           
        virtual const MCK::QuadTreeLeaf<T,CONTENT>* get_top_left_sub_node( void ) const noexcept
        {
            return top_left_sub_node;
        }

        virtual const MCK::QuadTreeLeaf<T,CONTENT>* get_top_right_sub_node( void ) const noexcept
        {
            return top_right_sub_node;
        }

        virtual const MCK::QuadTreeLeaf<T,CONTENT>* get_bottom_left_sub_node( void ) const noexcept
        {
            return bottom_left_sub_node;
        }

        virtual const MCK::QuadTreeLeaf<T,CONTENT>* get_bottom_right_sub_node( void ) const noexcept
        {
            return bottom_right_sub_node;
        }


    protected:

        Point<T> split_point;

        // All sub nodes stored using base class (leaf) pointer
        QuadTreeLeaf<T,CONTENT>* top_left_sub_node;
        QuadTreeLeaf<T,CONTENT>* top_right_sub_node;
        QuadTreeLeaf<T,CONTENT>* bottom_left_sub_node;
        QuadTreeLeaf<T,CONTENT>* bottom_right_sub_node;
};

}  // End of namespace MCK

#endif
