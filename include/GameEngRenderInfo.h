////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  GameEngRenderInfo.h
//
//  Information needed to render 
//  a single texture using the
//  SDL_RenderCopy or
//  SDL_RenderCopyEx command
//
//  This file has no corresponding .cpp file
//
//  Copyright (c) Muckytators 2023
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

#ifndef MCK_GAME_ENG_RI_H
#define MCK_GAME_ENG_RI_H

// SDL includes (Linux/Windows specific)
#ifdef MCK_MINGW
#include <SDL.h>
#endif
#ifndef MCK_MINGW
#include <SDL2/SDL.h>
#endif

namespace MCK
{

struct GameEngRenderInfo
{
    // GameEng instance can access protected/private members
    friend class GameEng;

    //!Non-SDL wrapper for SDL_Rect
    struct Rect
    {
        // GameEng instance can access protected/private members
        friend class GameEng;
        
        int get_x( void ) const noexcept { return r.x; }
        int get_y( void ) const noexcept { return r.y; }
        int get_w( void ) const noexcept { return r.w; }
        int get_h( void ) const noexcept { return r.h; }

        Rect( void )
        {
            r.x = 0;
            r.y = 0;
            r.w = 0;
            r.h = 0;
        }

        Rect( int _x, int _y, int _w, int _h )
        {
            r.x = _x;
            r.y = _y;
            r.w = _w;
            r.h = _h;
        }
        
        private:

            SDL_Rect r;
    };

    //! Texture ID
    /* Added for convenience, as 'tex' is private.
     * Should be the correct ID of 'tex',
     * but this is not enforced */
    MCK_TEX_ID_TYPE tex_id; 

    //! Destination rectangle of texture in game window
    /*! This is measured in *screen* pixels,
     *  relative to top-left of game window.
     */
    MCK::GameEngRenderInfo::Rect dest_rect;

    //! Flag to indicate whether clipping rectangle used
    bool clip;

    //! Clipping rectangle for source texture
    /*! This is measured in source texture's 
     * own pixel coords.
     */
    MCK::GameEngRenderInfo::Rect clip_rect;

    //! Get rotation, as multiple of 90 degrees
    int get_rotation( void ) const noexcept
    {
        return ( flags & MCK::GameEngRenderInfo::ROTATION_MASK ) 
                    >> ROTATION_RSHIFT;
    }

    //! Get flip x
    bool get_flip_x( void ) const noexcept
    {
        return ( flags & MCK::GameEngRenderInfo::FLIP_X_MASK ) > 0;
    }

    //! Get flip y
    bool get_flip_y( void ) const noexcept
    {
        return ( flags & MCK::GameEngRenderInfo::FLIP_Y_MASK ) > 0;
    }

    //! Default constructor
    GameEngRenderInfo( void )
    {
        this->clip = false;
        this->flags = 0x00;
        this->tex_id = 0;
        this->tex = NULL;
    }

    private:

        const static uint8_t ROTATION_MASK = 0x03;
        const static uint8_t ROTATION_RSHIFT = 0;
        const static uint8_t FLIP_X_MASK = 0x04;
        const static uint8_t FLIP_Y_MASK = 0x08;

        //! Pointer to source texture
        /*! Game engine accesses this through friend status*/
        SDL_Texture* tex;

        //! Flags to indicate rotation and flipping
        /*! See flag masks */
        uint8_t flags;

        static uint8_t calc_flags(
            int rotation,
            bool flip_x,
            bool flip_y
        )
        {
            return ( ( rotation % 4 ) << ROTATION_RSHIFT )
                   || flip_x ? FLIP_X_MASK : 0
                   || flip_y ? FLIP_Y_MASK : 0;
        }
};

}  // End of namespace MCK

#endif
