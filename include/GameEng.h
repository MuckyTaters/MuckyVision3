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

#ifndef MCK_GAME_ENG_H
#define MCK_GAME_ENG_H

// SDL includes
#ifdef MCK_MINGW
// This is required if cross-compiling for Windows *on Linux*
#define SDL_MAIN_HANDLED  // Tells g++ to ignore SDL's own main
#include <SDL.h>
#endif
#ifndef MCK_MINGW
// This is required if compiling on Linux or Windows (MinGW)
#include <SDL2/SDL.h>
#endif

#if defined MCK_STD_OUT
#include <iostream> // For std::cout
#endif

#include <cmath>  // For ceil
#include <stdexcept>  // For exceptions
#include <limits>  // For numeric_limits, size of data type in bits
#include <list>  // For list
#include <map>  // For map
#include <memory>  // For shared_ptr

#include "Defs.h"
#include "GameEngRenderBlock.h"
#include "GameEngRenderInfo.h"
#include "KeyEvent.h"
#include "Sprite.h"
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

        // //! Create texture
        // /*! @param image_id: Arbitrary ID associated with image data
        //  *  @param local_palette_id: Arbitrary ID associated with palette
        //  *  data
        //  *  @param bits_per_pixel: Number of bits in palette ID
        //  *  @param pitch in pixels: Width of image in pixels 
        //  *  @param pixel_data: Pixel bits, packed sequentially into bytes
        //  *  @param local_palette: Vector of colo(u)r IDs
        //  *  @param tex_id: Resulting texture id
        //  *  @param height_in_pixels: Resulting image height (for quality checks)
        //  */
        // void create_blank_texture(
        //     MCK_IMG_ID_TYPE image_id,
        //     MCK_PAL_ID_TYPE local_palette_id,
        //     uint8_t bits_per_pixel,
        //     uint16_t pitch_in_pixels,
        //     const std::vector<uint8_t> &pixel_data,
        //     const std::vector<uint8_t> &local_palette,
        //     MCK_TEX_ID_TYPE &tex_id,
        //     uint16_t &height_in_pixels
        // );

        //! Create texture
        /*! @param image_id: Arbitrary ID associated with image data
         *  @param local_palette_id: Arbitrary ID associated with palette
         *  data
         *  @param bits_per_pixel: Number of bits in palette ID
         *  @param pitch in pixels: Width of image in pixels 
         *  @param pixel_data: Pixel bits, packed sequentially into bytes
         *  @param local_palette: Vector of colo(u)r IDs
         *  @param tex_id: Resulting texture id
         *  @param height_in_pixels: Resulting image height (for quality checks)
         */
        void create_texture(
            MCK_IMG_ID_TYPE image_id,
            MCK_PAL_ID_TYPE local_palette_id,
            uint8_t bits_per_pixel,
            uint16_t pitch_in_pixels,
            const std::vector<uint8_t> &pixel_data,
            const std::vector<uint8_t> &local_palette,
            MCK_TEX_ID_TYPE &tex_id,
            uint16_t &height_in_pixels,
            bool throw_if_exists = false
        );

        //! Returns true if texture with this ID already exists
        bool texture_exists( MCK_TEX_ID_TYPE tex_id ) const
        {
            return textures.find( tex_id ) != textures.end();
        }
        
        //! Create empty render block
        /*! @param parent_block: if empty pointer supplied, block has no parent and thus will not be rendered
         *  @param add_to_front: if True render before all sibling blocks, if false render after all sibling blocks
         */
        std::shared_ptr<MCK::GameEngRenderBlock> create_empty_render_block(
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
            uint32_t z = MCK::DEFAULT_Z_VALUE
        ) const;

        //! Create render info for specified texture id
        /*! @param tex_id: Texture ID
         *  @param parent_block: Block to which image is to be attached
         *  @param dest_rect: Destination of image, in game window pixels
         *  @param clip: If True, only render part of texture
         *  @param clip_rect: If clip == True, defines part of texture to render
         *  @param rotation: Rotated of image, in multiples of 90 degrees
         *  @param flip_x: Horizonatal flip
         *  @param flip_y: Vertical flip
         *  Note: rectangles are intentionally NOT passed by reference as
         *  they are generally supplied as r-values
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
            bool flip_y = false,
            uint32_t z = MCK::DEFAULT_Z_VALUE
        ) const;

        //! Create blank textured render info
        /*! @param col_id: Core colo(u)r ID (i.e. MCK::COL_....)
         *  @param parent_block: Block to attach this texture to
         *  @param dest_rect: Window pixel coordinates
         */ 
        std::shared_ptr<MCK::GameEngRenderInfo> create_blank_tex_render_info(
            uint8_t col_id,
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
            MCK::GameEngRenderInfo::Rect dest_rect,
            uint32_t z = MCK::DEFAULT_Z_VALUE
        ) const;

        //! Change texture of render info object
        /*! @param info: Pointer to render info object
         *  @param new_tex_id: ID of the new texture
         */
        void change_render_info_tex(
            std::shared_ptr<MCK::GameEngRenderInfo> info,
            MCK_TEX_ID_TYPE new_tex_id
        ) const;

        //! Render specified render block (and all sub blocks)
        /*! @param render_block: Highest block to be rendered, e.g. prime_render_block
         *  @param hoz_offset: Horizonal offset, in pixels
         *  @param hoz_offset: Vertical offset, in pixels
         *  This is typcially called once per frame.
         *  When called with render_block set to GameEng's
         *  prime render block, *everything* active is rendered.
         *  However, nothing will be displayed onscreen
         *  until 'show()' is called.
         */
        void render_all(
            std::shared_ptr<MCK::GameEngRenderBlock> render_block, 
            int16_t hoz_offset = 0,
            int16_t vert_offset = 0
        ) const;

        //! Set render clearing colo(u)r
        /*! @param global_col_id: Core colo(u)r ID (i.e. MCK::COL_....)
         */
        void set_clearing_color( uint8_t global_color_id ) const;

        //! Clears screen
        void clear( void ) const;

        //! Present all rendering undertaken since last 'show'
        void show( void ) const;

        //! Get milliseconds since SDL initialization
        uint32_t get_ticks( void ) const { return SDL_GetTicks(); }
    
        //! Delay, to give time back to the operating system
        /* @param ticks: Nominal delay in milliseconds
         * Note: the exact duration of the delay is not guaranteed
         */
        void delay( int ticks ) const
        {
            SDL_Delay( ticks );
        }

        //! Get the render block to which all other render blocks are subservient
        std::shared_ptr<MCK::GameEngRenderBlock> get_prime_render_block( void ) const noexcept
        {
            return prime_render_block;
        }

        //! Get vector of new keyboard events, in the order they occurred.
        /*! @param key_events: Vector that will hold the resulting events.
         */
        void get_pending_keyboard_actions(
            std::vector< KeyEvent > &key_events
        );
     
        //! Calculate texture id from image id and palette id
        /*! @param image_id: Image ID (ImageMan manages these IDs)
         *  @param local_palette_id: Local palette ID (ImageMan manages these IDs)
         */
        static MCK_TEX_ID_TYPE calc_tex_id(
            MCK_IMG_ID_TYPE image_id,
            MCK_PAL_ID_TYPE local_palette_id
        )
        {
            return image_id 
                   | ( MCK_TEX_ID_TYPE( local_palette_id )
                           << std::numeric_limits<MCK_IMG_ID_TYPE>::digits );
        }

        //! Remove GameEngRenderBlock instance from render tree
        /*! @param block_to_remove: Render block to be removed
         *  @param block_to_start_search: Render block at which start searching for 'block_to_remove'
         *  If unsure, to 'block_to_start_search' to prime render
         *  block, then all blocks will be searched.
         *  Note: this method is static as it only operates on
         *  the blocks themselves, not GameEng
         */
        static void remove_block(
            std::shared_ptr<MCK::GameEngRenderBlock> block_to_remove, 
            std::shared_ptr<MCK::GameEngRenderBlock> block_to_start_search
        )
        {
            if( block_to_remove.get() == NULL 
                || block_to_remove->get_type() 
                    != MCK::RenderInstanceType::BLOCK
            )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    std::string( "Cannot remove block as render " )
                    + std::string( "instance supplied is not a block." )
#else
                    ""
#endif
                ) );
            }

            // Convert block pointer to base pointer, as this is
            // what we will actually searching for
            std::shared_ptr<MCK::GameEngRenderBase> instance_to_remove
                = std::dynamic_pointer_cast<MCK::GameEngRenderBase>(
                    block_to_remove
                );

            MCK::GameEng::remove_render_instance(
                instance_to_remove,
                block_to_start_search
            );
        }
        
        //! Remove render instance from render tree
        static void remove_render_instance(
            std::shared_ptr<MCK::GameEngRenderBase> instance_to_remove, 
            std::shared_ptr<MCK::GameEngRenderBlock> block_to_start_search
        );

        //! Move render instance to different block
        static void move_render_instance(
            std::shared_ptr<MCK::GameEngRenderBase> instance_to_move, 
            std::shared_ptr<MCK::GameEngRenderBlock> old_block,
            std::shared_ptr<MCK::GameEngRenderBlock> new_block,
            uint32_t new_z = MCK::DEFAULT_Z_VALUE
        );

        //! Change the 'z' value of a render instance
        /*! @param render_instance: Instance being changed
         *  @param parent_block: Parent block
         *  @param new_z: New value of 'z'
         *  @param use_current_z_as_hint: Set to true if 'z' not changing my much
         */
        static void change_z(
            std::shared_ptr<MCK::GameEngRenderBase> render_instance,
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
            uint32_t new_z,
            bool use_current_z_as_hint = true
        );

        //! Change the 'z' value of a render instance
        /*! @param render_instance: Instance being changed
         *  @param parent_block: Parent block
         *  @param new_z: New value of 'z' RELATIVE to DEFAULT_Z_VALUE
         *  @param use_current_z_as_hint: Set to true if 'z' not changing my much
         */
        static void change_z_rel_to_default(
            std::shared_ptr<MCK::GameEngRenderBase> render_instance,
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
            int32_t new_z,
            bool use_current_z_as_hint = true
        )
        {
            return MCK::GameEng::change_z(
                render_instance,
                parent_block,
                new_z + MCK::DEFAULT_Z_VALUE,
                use_current_z_as_hint = true
            );
        }

        //! Create sprite based on render info 
        /*! @param tex_id: Initial texture ID
         *  @param parent_block: Block to which sprite is to be attached
         *  @param dest_rect: Intial destination of sprite, in game window pixels
         *  @param rotation: Initial rotated of sprite, in multiples of 90 degrees
         *  @param flip_x: Intial horizonatal flip
         *  @param flip_y: Initial vertical flip
         *  Note: rectangles are intentionally NOT passed by reference as
         *  they are generally supplied as r-values
         */
        std::shared_ptr<MCK::Sprite<MCK::GameEngRenderInfo>> create_sprite(
            std::shared_ptr<MCK::SpriteAnimBase> _anim,
            std::shared_ptr<MCK::SpriteMotionBase> _motion,
            std::shared_ptr<MCK::SpriteCollisionBase> _collision,
            MCK_TEX_ID_TYPE tex_id,
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
            MCK::GameEngRenderInfo::Rect dest_rect,
            int rotation = 0,
            bool flip_x = false,
            bool flip_y = false,
            uint32_t z = MCK::DEFAULT_Z_VALUE
        ) const;


    private:

        //! Assign MuckyVision key code to SDL scancode, assuming scancode has no current assignment
        /*! Note: if a scancode has already been assigned to the key,
         *  this call will be ignored.
         *  This method is only used during initialisation,
         *  hence it being private.
         */
        void init_key(
            SDL_Scancode sc,
            MCK::KeyEvent::Keys k
        )
        {
            this->scancodes.insert(
                std::pair<SDL_Scancode,MCK::KeyEvent::Keys>(
                    sc,
                    k
                )
            );
        }

        //! Abstraction of image creation process, only used internally
        void basic_create_texture(
            uint8_t bits_per_pixel,
            uint16_t pitch_in_pixels,
            uint16_t height_in_pixels,
            const std::vector<uint8_t> &pixel_data,
            const std::vector<uint8_t> &local_palette,
            SDL_Texture* &texture
        );

        //! Get RGBA values for a given colo(u)r ID, only used internally
        static void get_RGBA(
            uint8_t col_id,
            uint8_t &r,
            uint8_t &g,
            uint8_t &b,
            uint8_t &a
        )
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

        //! Vector of blank textures (one for each colo(u)r)
        std::vector<SDL_Texture*> blank_textures;

        //! Top level render block
        std::shared_ptr<MCK::GameEngRenderBlock> prime_render_block;

        //! SDL2 renderer instance
        SDL_Renderer* renderer;

        //! SDL2 window instance
        SDL_Window* window;
            
        //! Game window width
        /*! Width of game window in pixels.*/
        unsigned int window_width_in_pixels;

        //! Game window height
        /*! Height of game window in pixels.*/
        unsigned int window_height_in_pixels;
        
        //! Pointer to SDL2's keyboard state array
        const Uint8* keyboard_state;

        //! Current size of SDL's keyboard state array.
        /*! Warning: pointer set during init,
         *  but may vary if keyboard unplugged/changed
         */
        int keyboard_state_array_size;

        // Scan codes, used to link key strokes to actions
        std::map<SDL_Scancode,MCK::KeyEvent::Keys> scancodes;

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
