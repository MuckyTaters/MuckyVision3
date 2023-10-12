////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  GameEngRenderBase.h
//
//  Abstract base class for GameEngRenderInfo
//  and GameEngRenderBlock
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

#ifndef MCK_GAME_ENG_BASE_H
#define MCK_GAME_ENG_BASE_H

#include<cstdint>  // For uint32_t et al.

// SDL includes (Linux/Windows specific)
// This is required if cross-compiling for Windows *on Linux*
#ifdef MCK_MINGW
#include <SDL.h>
#endif
#ifndef MCK_MINGW
// This is required if compiling on Linux or Windows (MinGW)
#include <SDL2/SDL.h>
#endif

#include "Defs.h"

namespace MCK
{

// Forward declaration
class GameEngRenderBlock;

class GameEngRenderBase
{
    friend class GameEng;

    public:

        GameEngRenderBase( uint32_t z = MCK::DEFAULT_Z_VALUE )
        {
            // Set render order from combination of 
            // 'z' value and auto-generated id.
            render_order = ( uint64_t( z ) << 32 )
                           | MCK::GameEngRenderBase::next_id++;

            // Check if ids have 'lapped' (unlikely!)
            if( next_id == 0 )
            {
                MCK::GameEngRenderBase::duplicate_ids_exist = true;
            }

            this->parent_block = NULL;
            
            this->type = MCK::RenderInstanceType::BASE;
        }

        //! Get 'z' value of render instance
        /*! The 'z' value is the alignment of the instance
         *  with regard to the axis perpendicular to x and y.
         *  That is, an axis which runs towards or away from
         *  the surface of the monitor, in an imaginary sense.
            It is used, together with 'id', to determine which
            images are rendered in front of others.
            Instances with higher 'z' values should be rendered
            in front of those with lower 'z' values. When
            instances have equal 'z' values, the instance with
            the highest 'id' should be rendered in front.
         */
        uint32_t get_z( void ) const noexcept
        {
            // 'z' is upper 32 bits of 'render_order';
            return uint32_t( this->render_order >> 32 );
        }

        //! Set x position
        virtual void set_x( int x ) noexcept = 0;

        //! Set y position
        virtual void set_y( int y ) noexcept = 0;

        //! Get ID of render instance
        /*! This ID is generated at construction and does not change.
         *  It's purpose is to determine render order amongst
         *  instances with identical 'z' values.
         *  If you create more than 2^32 render instances in one
         *  program run, then some instances may have duplicate IDs.
         *  This may cause flicker if these instance share the same
         *  'z' value. However, no other ill effects should result.
         */
        uint32_t get_id( void ) const noexcept
        {
            // ID is lower 32 bits of 'render_order'
            return this->render_order & 0xFFFFFFFF;
        }

        //! Get type of instance (provides reflection)
        MCK::RenderInstanceType get_type( void ) const noexcept
        {
            return this->type;
        }

        //!* Value that combines z-value and ID
        /*! This value may be useful as it is the key used
         *  to store a pointer to the instance in the parent
         *  block.
         */
        uint64_t get_render_order( void ) const noexcept
        {
            return this->render_order;
        }
       
        virtual void render( SDL_Renderer* renderer ) const = 0;

    protected:

        MCK::RenderInstanceType type;

        /*! This number suggests the order in which the instance
         *  is rendered. An instance with a low render_order
         *  should be rendered before (and therefore appear
         *  behind) an instance with a higher value.
         *  Changes of render order value must only be
         *  performed by certain (friend) classes, as the actual
         *  order of render is determined by the position of the
         *  render instance in the render list. This number is
         *  used to maintaining that list, and should NEVER be
         *  altered without (potentially) reordering the render
         *  list.
         */
        uint64_t render_order;

        //! This variable keeps record of next available ID
        static uint32_t next_id;

        //! This variable indicates that more than 2^32 IDs exist
        static bool duplicate_ids_exist;

        // Pointer to parent block, only accessible through
        // friend access (GameEng). This pointer should only
        // be used to ensure parent is correct, not to access
        // the parent (for safety it is best not to actually
        // dereference this pointer)
        const MCK::GameEngRenderBlock* parent_block;

        // Dummy method to make this class abstract
        virtual void dummy( void ) = 0;

};

}  // End of namespace MCK

#endif
