////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  GameEngRenderBlock.h
//
//  A vector of GameEngRenderInfo structs,
//  a list of subservient blocks,
//  and additional meta information
//
//  This file has no corresponding .cpp file
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

#ifndef MCK_GAME_ENG_RB_H
#define MCK_GAME_ENG_RB_H

#include <vector>
#include <stdexcept>

#include "GameEngRenderBase.h"
#include "GameEngRenderInfo.h"

namespace MCK
{

/*
// Forward declaration for friendship
template<class MOTION, class ANIM, class COLL, class RENDER>
class GameEngSpriteFactory;
*/

class GameEngRenderBlock : public GameEngRenderBase
{
    //! Friendship so GameEng can access protected/private members
    friend class GameEng;

    /*
    template<class MOTION, class ANIM, class COLL, class RENDER>
    friend class MCK::GameEngSpriteFactory;
    */

    public:

        //! Flag to indicate if block should be rendered
        /*! True = render, false = don't render */
        bool active;

        //! Optional horizontal offset (in screen pixels)
        int16_t hoz_offset;

        //! Optional vertical offset (in screen pixels)
        int16_t vert_offset;

        //! Default constructor
        GameEngRenderBlock( uint32_t z = MCK::DEFAULT_Z_VALUE )
            : GameEngRenderBase( z )
        {
            this->active = true;
            this->hoz_offset = 0;
            this->vert_offset = 0;

            this->type = MCK::RenderInstanceType::BLOCK;

        }

        //! Returns true if block contains no render instances, i.e. images or other blocks
        bool is_empty( void ) const noexcept
        {
            return render_instances.size() == 0;
        }

        //! Set x position
        virtual void set_x( int x ) noexcept
        {
            hoz_offset = x;
        }

        //! Set y position
        virtual void set_y( int y ) noexcept
        {
            vert_offset = y;
        }

        //! Get x position
        virtual int get_x( void ) const noexcept
        {
            return hoz_offset;
        }

        //! Get y position
        virtual int get_y( void ) const noexcept
        {
            return vert_offset;
        }

        //! Get width
        virtual int get_width( void ) const noexcept
        {
            // TODO: NOT YET IMPLEMENTED
            return 0;
        }

        //! Get height position
        virtual int get_height( void ) const noexcept
        {
            // TODO: NOT YET IMPLEMENTED
            return 0;
        }

        //! Get position of bottom edge
        virtual int get_bottom( void ) const noexcept
        {
            // TODO: NOT YET IMPLEMENTED
            return 0;
        };
    
        //! Get position of right-hand edge
        virtual int get_right( void ) const noexcept
        {
            // TODO: NOT YET IMPLEMENTED
            return 0;
        };
        
        //! Set position
        virtual void set_pos( int x, int y ) noexcept
        {
            hoz_offset = x;
            vert_offset = y;
        }

        //! Adjust position
        virtual void adj_pos( int dx, int dy ) noexcept
        {
            hoz_offset += dx;
            vert_offset += dy;
        }

        virtual void render( SDL_Renderer* renderer ) const
        {
            // TODO
        }


    protected :

        //! Map of render instances subserviant to this block
        /*! The key is the 'render_order' of the associated
         *  render instance, comprised of 'z' value (32 most
         *  significant bits) and render instance ID (32 least
         *  significant bits).
         *  This is a 'multimap' to account for the possibility
         *  of duplicate IDs if more than 2^32 instances are 
         *  created.
         */
        std::multimap<uint64_t,std::shared_ptr<MCK::GameEngRenderBase>> render_instances;

        // Dummy method to this class non-abstract
        void dummy( void ) {}
};

}  // End of namespace MCK

#endif
