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

class GameEngRenderBlock : public GameEngRenderBase
{
    //! Friendship so GameEng can access protected/private members
    friend class GameEng;

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

        // //! Get number of render info structs associated with this block
        // /*! Note: excludes render info structs belonging to any sub-blocks
        //  */
        // size_t get_render_info_count( void ) const noexcept
        // {
        //     return this->render_info.size();
        // }

        // //! Get number of subservient blocks associated with this block
        // size_t get_sub_block_count( void ) const noexcept
        // {
        //     return this->sub_blocks.size();
        // }

        // std::shared_ptr<GameEngRenderBlock> get_sub_block( size_t index )
        // {
        //     if( index > this->sub_blocks.size() )
        //     {
        //         throw( std::runtime_error(
    // #if defined MCK_STD_OUT
        //             std::string( "Invalid sub block index: " )
        //             + std::to_string( index )
    // #else
        //             ""
    // #endif
        //         ) );
        //     }

        //     std::list<std::shared_ptr<GameEngRenderBlock>>::iterator it
        //         = this->sub_blocks.begin();
        //     std::advance<
        //         std::list<std::shared_ptr<GameEngRenderBlock>>::iterator,
        //         size_t
        //     >( it, index );
        //     return *it;
        // }

        // //! Reserve memory for pointers to future render instances
        // /*! Performs a 'reserve' operation on an internal vector.
        //  *  Set this to max number of images and sub-blocks your envisage
        //  *  attaching to this block. This is a 'soft' limit; you can
        //  *  exceed it, at the risk of some inefficiency.
        //  */
        // void reserve_space_for_render_instances( size_t num )
        // {
        //     if( num > this->render_instances.size() )
        //     {
        //         this->render_instances.reserve( num );
        //     }
        // }

        // //! Get info render instance at given index position
        // /*! @param index: Position of info render instance in internal vector
        //  * Note: Raises exception if index exceeds last element in vector
        //  */
        // std::shared_ptr<MCK::GameEngRenderInfo> get_render_info( size_t index )
        // {
        //     if( this->render_info.size() <= index )
        //     {
        //         throw( std::runtime_error(
    // #if defined MCK_STD_OUT
        //             std::string( "Invalid info render index: " )
        //             + std::to_string( index )
    // #else
        //             ""
    // #end if
        //         ) );
        //     }

        //     return this->render_info[index];
        // }

        // //! Remove last render info item from this block
        // void remove_last_render_info( void )
        // {
        //     if( this->render_info.size() > 0 )
        //     {
        //         this->render_info.pop_back();
        //     }
        // }


    protected :

        /*
        //! Vector of render information associated with this block
        std::vector<std::shared_ptr<MCK::GameEngRenderInfo>> render_info;
        */

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
