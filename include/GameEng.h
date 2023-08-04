////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  GameEng.h
//
//  Wrapper for SDL2, also provides
//  texture management and
//  screen rendering
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

#ifndef MCK_GAME_ENG_H
#define MCK_GAME_ENG_H

// SDL includes (Linux/Windows specific)
#ifdef MCK_MINGW
#define SDL_MAIN_HANDLED  // Tells g++ to ignore SDL's own main
#include <SDL.h>
#endif
#ifndef MCK_MINGW
#include <SDL2/SDL.h>
#endif

#ifndef MCK_NO_STD_OUT
#include <iostream> // For std::cout
#endif

#include <limits>  // For numeric_limits, size of data type in bits
#include <list>  // For list
#include <map>  // For map
#include <memory>  // For shared_ptr

#include "Defs.h"
#include "GameEngRenderBlock.h"
#include "GameEngRenderInfo.h"
#include "KeyEvent.h"
#include "Version.h"

namespace MCK
{

class GameEng
{
    public:

        //! Constructs and initializes singleton instance of class
        static GameEng& get_singleton( void ) noexcept
        {
            static GameEng game_eng;
            return game_eng;
        }

        //! Initialize game engine
        /*! @param window_title: Title shown in window bar
         *  @param window_initial_x_pos: Hoz pos of window
         *  @param window_initial_y_pos: Vert pos of window
         *  @param _window_width_in_pixels: Window width
         *  @param _window_width_in_pixels: Window height
         */
        void init(
            const char* window_title,
            int16_t window_initial_x_pos,
            int16_t window_initial_y_pos,
            uint16_t _window_width_in_pixels,
            uint16_t _window_height_in_pixels
        );

        //! Returns true if game engine is initialized
        bool is_initialized( void ) const noexcept
        {
            return this->initialized;
        }

        //! Create texture
        /*! @param image_id: Arbitrary ID associated with image data
         *  @param local_palette_id: Arbitrary ID associated with palette
         *  data
         *  @param bits_per_pixel: Number of bits in palette ID
         *  @param pitch in pixels: Width of image in pixels 
         *  @param pixel_data: Pixel bits, packed sequentially into bytes
         *  @param local_palette: Vector of colo(u)r IDs
         *  @param tex_id: Resulting texture id
         *  @param tex_id: Resulting image height (for quality checks)
         */
        void create_texture(
            MCK_IMG_ID_TYPE image_id,
            MCK_PAL_ID_TYPE local_palette_id,
            uint8_t bits_per_pixel,
            uint16_t pitch_in_pixels,
            const std::vector<uint8_t> &pixel_data,
            const std::vector<uint8_t> &local_palette,
            MCK_TEX_ID_TYPE &tex_id,
            uint16_t &height_in_pixels
        );

        //! Create empty render block
        /*! @param parent_block: if empty pointer supplied, block has no parent and thus will not be rendered
         *  @param add_to_front: if True render before all sibling blocks, if false render after all sibling blocks
         */
        std::shared_ptr<MCK::GameEngRenderBlock> create_empty_render_block(
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
            bool add_to_front = true
        ) const;

        //! Create render info for specified texture id
        /*! Note: rectangles NOT passed by ref as generally
         *  supplied as r-values
         */
        std::shared_ptr<MCK::GameEngRenderInfo> create_render_info(
            MCK_TEX_ID_TYPE tex_id,
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
            MCK::GameEngRenderInfo::Rect dest_rect,
            bool clip = false,
            MCK::GameEngRenderInfo::Rect clip_rect
                = MCK::GameEngRenderInfo::Rect(),
            int rotation = 0,
            bool flip_x = false,
            bool flip_y = false
        ) const;

        //! Render specified render block (and all sub blocks)
        /*! @param render_block: Highest block to be rendered, e.g. prime_render_block
         *  @param hoz_offset: Horizonal offset, in pixels
         *  @param hoz_offset: Vertical offset, in pixels
         *  Note: nothing will be displayed onscreen
         *  until 'show()' is called.
         */
        void render_all(
            std::shared_ptr<MCK::GameEngRenderBlock> render_block, 
            int16_t hoz_offset = 0,
            int16_t vert_offset = 0
        ) const;

        //! Clears screen to black
        void clear( void ) const;

        //! Present all rendering undertaken since last 'show'
        void show( void ) const;

        //! Get milliseconds since initiation
        uint32_t get_ticks( void ) const { return SDL_GetTicks(); }
    
        //! Delay, to give time back to the operating system
        void delay( int ticks ) const
        {
            SDL_Delay( ticks );
        }

        std::shared_ptr<MCK::GameEngRenderBlock> get_prime_render_block( void ) const noexcept
        {
            return prime_render_block;
        }

        void get_pending_keyboard_actions(
            std::vector< KeyEvent > &key_events
        );
     

    private:

        //! Calculate texture id from image id and palette id
        static MCK_TEX_ID_TYPE calc_tex_id(
            MCK_IMG_ID_TYPE image_id,
            MCK_PAL_ID_TYPE local_palette_id
        )
        {
            return image_id 
                   | ( MCK_TEX_ID_TYPE( local_palette_id )
                           << std::numeric_limits<MCK_IMG_ID_TYPE>::digits );
        }

        //! Get RGBA values for a given colo(u)r ID
        void get_RGBA(
            uint8_t col_id,
            uint8_t &r,
            uint8_t &g,
            uint8_t &b,
            uint8_t &a
        ) const
        {
            // TODO: Add provision for extended palette
            //       with user-defined colo(u)rs
            
            // Check colo(u)r ID is within range
            if( col_id >= MCK::TOTAL_CORE_COLORS )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Invalid colo(u)r ID: " )
                    + std::to_string( col_id )
#else
                    ""
#endif
                ) );
            }

            r = MCK::GameEng::CORE_PALETTE_REDS[col_id];
            g = MCK::GameEng::CORE_PALETTE_GREENS[col_id];
            b = MCK::GameEng::CORE_PALETTE_BLUES[col_id];
            a = MCK::GameEng::CORE_PALETTE_ALPHAS[col_id];
        }

        //! Flag to indicate game engine initialized
        bool initialized;

        // Individual 8bit colo(u)r components 
        // of core palette
        static const std::vector<uint8_t> CORE_PALETTE_REDS;
        static const std::vector<uint8_t> CORE_PALETTE_GREENS;
        static const std::vector<uint8_t> CORE_PALETTE_BLUES;
        static const std::vector<uint8_t> CORE_PALETTE_ALPHAS;
        
        //! Map linking texture ids to texture pointers
        std::map<MCK_TEX_ID_TYPE,SDL_Texture*> textures;

        /*
        //! List of all render blocks to be processed
        std::list<std::shared_ptr<MCK::GameEngRenderBlock>> render_list;
        */
        
        //! Top level render block
        std::shared_ptr<MCK::GameEngRenderBlock> prime_render_block;

        //! SDL renderer instance
        SDL_Renderer* renderer;

        //! SDL window instance
        SDL_Window* window;
            
        //! Game window width
        /*! Width of game window in pixels.*/
        unsigned int window_width_in_pixels;

        //! Game window height
        /*! Height of game window in pixels.*/
        unsigned int window_height_in_pixels;
        
        //! Pointer to SDL's keyboard state array
        const Uint8* keyboard_state;

        //! Current size of SDL's keyboard state array.
        /*! Warning: pointer set during init,
         *  but may vary if keyboard unplugged/changed
         */
        int keyboard_state_array_size;

        // Scan codes, used to link key strokes to actions
        std::vector<SDL_Scancode> scancodes;

        // Constructor/destructor/copy/assignment/
        // initializer made private to avoid misuse.
        // Use public 'get_singleton' method instead
        
        GameEng( void );
        ~GameEng( void );

        GameEng(GameEng const&) = delete;
        void operator=(GameEng const&)  = delete;
};

}  // End of namespace MCK

#endif
