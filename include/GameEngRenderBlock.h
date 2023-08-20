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

#include "GameEngRenderInfo.h"

namespace MCK
{

struct GameEngRenderBlock
{
    //! Friendship so GameEng can access protected/private members
    friend class GameEng;

    //! Flag to indicate if block should be rendered
    /*! True = render, false = don't render */
    bool active;

    //! Optional horizontal offset (in screen pixels)
    int16_t hoz_offset;

    //! Optional vertical offset (in screen pixels)
    int16_t vert_offset;

    //! Default constructor
    GameEngRenderBlock( void )
    {
        this->active = true;
        this->hoz_offset = 0;
        this->vert_offset = 0;
    }

    //! Get number of render info structs associated with this block
    /*! Note: excludes render info structs belonging to any sub-blocks
     */
    size_t get_render_info_count( void ) const noexcept
    {
        return render_info.size();
    }

    //! Get number of subservient blocks associated with this block
    size_t get_sub_block_count( void ) const noexcept
    {
        return render_info.size();
    }

    //! Reserve memory for pointers to future info render objects
    /*! Performs a 'reserve' operation on an internal vector.
     *  Set this to max number of info render objects your envisage
     *  attaching to this block. This is a 'soft' limit; you can
     *  exceed it, at the risk of some inefficiency.
     */
    void reserve_space_for_info_render( size_t num )
    {
        if( num > render_info.size() )
        {
            render_info.reserve( num );
        }
    }

    //! Get info render instance at given index position
    /*! @param index: Position of info render instance in internal vector
     * Note: Raises exception if index exceeds last element in vector
     */
    std::shared_ptr<MCK::GameEngRenderInfo> get_render_info( size_t index )
    {
        if( render_info.size() <= index )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Invalid info render index: " )
                + std::to_string( index )
#else
                ""
#endif
            ) );
        }

        return render_info[index];
    }

    //! Remove last render info item from this block
    void remove_last_render_info( void )
    {
        if( render_info.size() > 0 )
        {
            render_info.pop_back();
        }
    }


    protected :

        // Pointer to parent block, only accessible through
        // friend access (GameEng). This pointer should only
        // be used to ensure parent is correct, not to access
        // the parent (for safety it is best not to actually
        // dereference this pointer)
        const MCK::GameEngRenderBlock* parent_block;

        //! Vector of render information associated with this block
        std::vector<std::shared_ptr<MCK::GameEngRenderInfo>> render_info;

        //! List of blocks subserviant to this block
        std::list<std::shared_ptr<MCK::GameEngRenderBlock>> sub_blocks;

};

}  // End of namespace MCK

#endif
