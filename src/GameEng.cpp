////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  GameEng.cpp
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

#include "GameEng.h"

const std::vector<uint8_t> MCK::GameEng::CORE_PALETTE_REDS = {
    0x00,  // 0 - Black
    0x00,  // 1 - Black, semi-transparent
    0x3f,  // 2 - Dark Grey
    0x7f,  // 3 - Dark Red, semi-transparent
    0x00,  // 4 - Fully Transparent
    0xff,  // 5 - White
    0xbf,  // 6 - Light Grey
    0x7f,  // 7 - Mid Grey
    0x1f,  // 8 - V. Dark Grey
    0x7f,  // 9 - Dark Red
    0x00,  // 10 - Dark Green
    0x00,  // 11 - Dark Blue
    0xff,  // 12 - Pink
    0x7f,  // 13 - Pastel Green
    0x7f,  // 14 - Pastel Blue
    0xff,  // 15 - Pastel Yellow 
    0xff,  // 16 - Red
    0x00,  // 17 - Green
    0x00,  // 18 - Blue
    0xff,  // 19 - Yellow
    0x00,  // 20 - Cyan
    0xff,  // 21 - Magenta
    0xff,  // 22 - Orange
    0x7f,  // 23 - Lime
    0x7f,  // 24 - Purple
    0x00,  // 25 - Forest Green
    0x2f,  // 26 - Field Grey
    0x4f,  // 27 - Rotting Purple
    0xcf,  // 28 - Fleshtone
    0xbf,  // 29 - Sand
    0x7f,  // 30 - Mid Brown
    0x3f   // 31 - Dark Brown
};

const std::vector<uint8_t> MCK::GameEng::CORE_PALETTE_GREENS = {
    0x00,  // 0 - Black
    0x00,  // 1 - Black, semi-transparent
    0x3f,  // 2 - Dark Grey
    0x00,  // 3 - Dark Red, semi-transparent
    0x00,  // 4 - Fully Transparent
    0xff,  // 5 - White
    0xbf,  // 6 - Light Grey
    0x7f,  // 7 - Mid Grey
    0x1f,  // 8 - V. Dark Grey
    0x00,  // 9 - Dark Red
    0x7f,  // 10 - Dark Green
    0x00,  // 11 - Dark Blue
    0x7f,  // 12 - Pink
    0xff,  // 13 - Pastel Green
    0x7f,  // 14 - Pastel Blue
    0xff,  // 15 - Pastel Yellow 
    0x00,  // 16 - Red
    0xff,  // 17 - Green
    0x00,  // 18 - Blue
    0xff,  // 19 - Yellow
    0xff,  // 20 - Cyan
    0x00,  // 21 - Magenta
    0x7f,  // 22 - Orange
    0xff,  // 23 - Lime
    0x00,  // 24 - Purple
    0x3f,  // 25 - Forest Green
    0x4f,  // 26 - Field Grey
    0x3f,  // 27 - Rotting Purple
    0x7f,  // 28 - Fleshtone
    0x7f,  // 29 - Sand
    0x3f,  // 30 - Mid Brown
    0x1f   // 31 - Dark Brown
};

const std::vector<uint8_t> MCK::GameEng::CORE_PALETTE_BLUES = {
    0x00,  // 0 - Black
    0x00,  // 1 - Black, semi-transparent
    0x3f,  // 2 - Dark Grey
    0x00,  // 3 - Dark Red, semi-transparent
    0x00,  // 4 - Fully Transparent
    0xff,  // 5 - White
    0xbf,  // 6 - Light Grey
    0x7f,  // 7 - Mid Grey
    0x1f,  // 8 - V. Dark Grey
    0x00,  // 9 - Dark Red
    0x00,  // 10 - Dark Green
    0x7f,  // 11 - Dark Blue
    0x7f,  // 12 - Pink
    0x7f,  // 13 - Pastel Green
    0xff,  // 14 - Pastel Blue
    0x7f,  // 15 - Pastel Yellow 
    0x00,  // 16 - Red
    0x00,  // 17 - Green
    0xff,  // 18 - Blue
    0x00,  // 19 - Yellow
    0xff,  // 20 - Cyan
    0xff,  // 21 - Magenta
    0x00,  // 22 - Orange
    0x00,  // 23 - Lime
    0x7f,  // 24 - Purple
    0x00,  // 25 - Forest Green
    0x3f,  // 26 - Field Grey
    0x5f,  // 27 - Rotting Purple
    0x3f,  // 28 - Fleshtone
    0x00,  // 29 - Sand
    0x00,  // 30 - Mid Brown
    0x00   // 31 - Dark Brown
};

const std::vector<uint8_t> MCK::GameEng::CORE_PALETTE_ALPHAS = {
    0xff,  // 0 - Black
    0xbf,  // 0xe5,  // 1 - Black, semi-transparent
    0xff,  // 2 - Dark Grey
    0xbf,  // 0xe5,  // 3 - Dark Red, semi-transparent
    0x00,  // 4 - Fully Transparent
    0xff,  // 5 - White
    0xff,  // 6 - Light Grey
    0xff,  // 7 - Mid Grey
    0xff,  // 8 - V. Dark Grey
    0xff,  // 9 - Dark Red
    0xff,  // 10 - Dark Green
    0xff,  // 11 - Dark Blue
    0xff,  // 12 - Pink
    0xff,  // 13 - Pastel Green
    0xff,  // 14 - Pastel Blue
    0xff,  // 15 - Pastel Yellow 
    0xff,  // 16 - Red
    0xff,  // 17 - Green
    0xff,  // 18 - Blue
    0xff,  // 19 - Yellow
    0xff,  // 20 - Cyan
    0xff,  // 21 - Magenta
    0xff,  // 22 - Orange
    0xff,  // 23 - Lime
    0xff,  // 24 - Purple
    0xff,  // 25 - Forest Green
    0xff,  // 26 - Field Grey
    0xff,  // 27 - Rotting Purple
    0xff,  // 28 - Fleshtone
    0xff,  // 29 - Sand
    0xff,  // 30 - Mid Brown
    0xff   // 31 - Dark Brown
};

MCK::GameEng::GameEng( void )
{
    // Set all variables to invalid/default values
    this->renderer = NULL;
    this->window = NULL;
}

///////////////////////////////////////////////

MCK::GameEng::~GameEng( void )
{
#if defined MCK_STD_OUT && defined MCK_VERBOSE
    std::cout << "Destroying all SDL textures..."
              << std::endl;
    
    bool all_textures_destroyed = true;
#endif

    for( auto& it : this->textures )
    {
        SDL_Texture* const TEX = it.second;
        if( TEX != NULL )
        {
            try
            {
                SDL_DestroyTexture( TEX );
            }
            catch( const std::runtime_error& e )
            {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
                std::cout << "Failed to destroy SDL texture, "
                    << "SDL error message :"
                    << SDL_GetError()
                    << std::endl;
                
                all_textures_destroyed = false;
#endif
            }
        }
    }
    
#if defined MCK_STD_OUT && defined MCK_VERBOSE
    if( all_textures_destroyed )
    {
        std::cout << "All SDL textures successfully destroyed."
                  << std::endl;
    }
    else
    {
        std::cout << "Some SDL textures were NOT destroyed."
                  << std::endl;
    }

    std::cout << "Commencing game engine (SDL2) shutdown..."
              << std::endl;
#endif

    // Clean up and close down SDL
    if( this->renderer != NULL )
    {
        SDL_DestroyRenderer( this->renderer );
    }
    if( this->window != NULL )
    {
        SDL_DestroyWindow( this->window );
    }

    SDL_Quit();

    this->initialized = false;

#if defined MCK_STD_OUT
    std::cout << "Game engine (SDL2) shutdown complete."
              << std::endl;
#endif
}

///////////////////////////////////////////////

void MCK::GameEng::init(
    const char* window_title,
    int16_t window_initial_x_pos,
    int16_t window_initial_y_pos,
    uint16_t _window_width_in_pixels,
    uint16_t _window_height_in_pixels
)
{
    if( this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create game engine (SDL2) as SDL already init."
#else
            ""
#endif
        ) );
    }

    // Integrity check for core palette
    if( uint8_t( MCK::TOTAL_CORE_COLORS )
            != MCK::GameEng::CORE_PALETTE_REDS.size()
        || uint8_t( MCK::TOTAL_CORE_COLORS )
            != MCK::GameEng::CORE_PALETTE_GREENS.size()
        || uint8_t( MCK::TOTAL_CORE_COLORS )
            != MCK::GameEng::CORE_PALETTE_BLUES.size()
    )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Core palette size mismatch, internal error?"
#else
            ""
#endif
        ) );
    }

    // Check window size is within sane limits
    const long WIDTH = _window_width_in_pixels;
    const long HEIGHT = _window_height_in_pixels;
    if( WIDTH > MAX_WINDOW_WIDTH_IN_PIXELS )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Requested window width in pixels is too large."
#else
            ""
#endif
        ) );
    }
    if( HEIGHT > MAX_WINDOW_HEIGHT_IN_PIXELS )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Requested window height in pixels is too large."
#else
            ""
#endif
        ) );
    }
    
    // Record window dimensions
    this->window_height_in_pixels
        = _window_height_in_pixels;
    this->window_height_in_pixels
        = _window_height_in_pixels;

    // Try to initialize SDL2
    int rc = SDL_Init( SDL_INIT_VIDEO );
    if( rc < 0 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "SDL2 failed to initiate, error code: " )
            + std::to_string( rc )
#else
            ""
#endif
        ) );
    }
    else
    {
#if defined MCK_STD_OUT
        std::cout << "SDL2 successfully initialized." << std::endl;
#endif
    }

    // Try to set scaling quality to nearest pixel sampling
    // This is absolutely necessary for a 'pixelated' appearance
    if( !SDL_SetHintWithPriority(
        SDL_HINT_RENDER_SCALE_QUALITY,
        "0",
        SDL_HINT_OVERRIDE )
    )
    {
#if defined MCK_STD_OUT
        std::cout << "Warning: SDL2 nearest pixel samping "
                  << "not enabled! SDL error:"
                  << SDL_GetError()
                  << std::endl;
#endif
    }
    else
    {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
        std::cout << "SDL2 nearest pixel sampling"
                  << " hint set with high priority."
                  << std::endl;
#endif
    }

    // Try to disable high DPI scaling
    // This is helpful for a 'pixelated' appearance
    if( !SDL_SetHintWithPriority(
        SDL_HINT_VIDEO_HIGHDPI_DISABLED,  // Valid for SDL2.0.1+
        "1",  // Do not allow high-DPI windows 
        SDL_HINT_OVERRIDE )
    )
    {
#if defined MCK_STD_OUT
        std::cout << "Warning: SDL2 set disable high DPI video hint "
                  << "failed! SDL error:"
                  << SDL_GetError()
                  << std::endl;
#endif
    }
    else
    {
#if defined MCK_STD_OUT &&  defined MCK_VERBOSE
        std::cout << "SDL2 disable high DPI video "
                  << " hint set with high priority"
                  << std::endl;
#endif
    }

    //Create SDL window
    this->window = SDL_CreateWindow(
        window_title,
        window_initial_x_pos,
        window_initial_y_pos,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );  // No resize allowed
    if( this->window == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create window with SDL2, SDL error:"
            + *SDL_GetError()
#else
            ""
#endif
        ) );
    }
    else
    {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
        std::cout << "Window created with SDL2" << std::endl;
#endif
    }

    // Create renderer
    this->renderer = SDL_CreateRenderer(
        this->window,
        -1,
        SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC );
    if( this->renderer == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
                "Cannot create SDL2 renderer, "
                "SDL Error: "
                + *SDL_GetError()
#else
                ""
#endif
        ) );
    }
    else
    {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
        std::cout<< "SDL2 renderer created for window" << std::endl;
#endif
    }

    // Clear window (best effort, issues warning if fails)
    {
        // Set renderer colour
        // (black, for clearing window)
        int rc = SDL_SetRenderDrawColor(
            this->renderer,
            0x00,
            0x00,
            0x00,
            0xFF
        );
        // -ve return code indicates failure
        if ( rc < 0 )
        {
#if defined MCK_STD_OUT
            // Issue warning but do not throw execption
            std::cout << "Failed to set draw color, "
                      << "SDL2 error message = "
                      << SDL_GetError()
                      << std::endl;
#endif
        }
        else
        {
            // Clear window
            int rc = SDL_RenderClear( this->renderer );
            // -ve return code indicates failure
            if ( rc < 0 )
            {
#if defined MCK_STD_OUT
                // Issue warning but do not throw execption
                std::cout << "Failed to clear SDL window, "
                          << "SDL2 error message = "
                          << SDL_GetError()
                          << std::endl;
#endif
            }
            else
            {
                try
                {
                    SDL_RenderPresent( this->renderer );
                }
                catch( std::exception &e )
                {
#if defined MCK_STD_OUT
                    // Issue warning but do not throw execption
                    std::cout << "First presentation of renderer "
                              << "failed, SDL2 error message = "
                              << SDL_GetError()
                              << std::endl;
#endif
                }
            }
        }

    }  // End of clear window block

    // Set keyboard state variables
    try
    {
        this->keyboard_state
            = SDL_GetKeyboardState(
                &this->keyboard_state_array_size
        );
    }
    catch( std::exception &e )
    {
#if defined MCK_STD_OUT
        std::cout << "Failed to get keyboard state pointers, error = "
                  << SDL_GetError() << std::endl;
#endif
        this->keyboard_state = NULL;
        this->keyboard_state_array_size = 0;
    }

    if( this->keyboard_state == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Keyboard state pointer is NULL, SDL error: "
            + *SDL_GetError()
#else
            ""
#endif
        ) );
    }

    if( this->keyboard_state_array_size == 0 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Keyboard state array size is zero, SDL error:"
            + *SDL_GetError()
#else
            ""
#endif
        ) );
    }

    // Link SDL scancodes to MuckyVision key code, using
    // default settings (i.e. A maps to A, B maps to B, and so on )
    this->init_key( SDL_SCANCODE_UP, MCK::KeyEvent::KEY_UP );
    this->init_key( SDL_SCANCODE_RIGHT, MCK::KeyEvent::KEY_RIGHT );
    this->init_key( SDL_SCANCODE_DOWN, MCK::KeyEvent::KEY_DOWN );
    this->init_key( SDL_SCANCODE_LEFT, MCK::KeyEvent::KEY_LEFT );
    this->init_key( SDL_SCANCODE_RETURN, MCK::KeyEvent::KEY_SELECT );
    this->init_key( SDL_SCANCODE_ESCAPE, MCK::KeyEvent::KEY_ESC );
    this->init_key( SDL_SCANCODE_F9, MCK::KeyEvent::KEY_QUIT );
    this->init_key( SDL_SCANCODE_F1, MCK::KeyEvent::KEY_HELP );
    this->init_key( SDL_SCANCODE_A, MCK::KeyEvent::KEY_A );
    this->init_key( SDL_SCANCODE_B, MCK::KeyEvent::KEY_B );
    this->init_key( SDL_SCANCODE_C, MCK::KeyEvent::KEY_C );
    this->init_key( SDL_SCANCODE_D, MCK::KeyEvent::KEY_D );
    this->init_key( SDL_SCANCODE_E, MCK::KeyEvent::KEY_E );
    this->init_key( SDL_SCANCODE_F, MCK::KeyEvent::KEY_F );
    this->init_key( SDL_SCANCODE_G, MCK::KeyEvent::KEY_G );
    this->init_key( SDL_SCANCODE_H, MCK::KeyEvent::KEY_H );
    this->init_key( SDL_SCANCODE_I, MCK::KeyEvent::KEY_I );
    this->init_key( SDL_SCANCODE_J, MCK::KeyEvent::KEY_J );
    this->init_key( SDL_SCANCODE_K, MCK::KeyEvent::KEY_K );
    this->init_key( SDL_SCANCODE_L, MCK::KeyEvent::KEY_L );
    this->init_key( SDL_SCANCODE_M, MCK::KeyEvent::KEY_M );
    this->init_key( SDL_SCANCODE_N, MCK::KeyEvent::KEY_N );
    this->init_key( SDL_SCANCODE_O, MCK::KeyEvent::KEY_O );
    this->init_key( SDL_SCANCODE_P, MCK::KeyEvent::KEY_P );
    this->init_key( SDL_SCANCODE_Q, MCK::KeyEvent::KEY_Q );
    this->init_key( SDL_SCANCODE_R, MCK::KeyEvent::KEY_R );
    this->init_key( SDL_SCANCODE_S, MCK::KeyEvent::KEY_S );
    this->init_key( SDL_SCANCODE_T, MCK::KeyEvent::KEY_T );
    this->init_key( SDL_SCANCODE_U, MCK::KeyEvent::KEY_U );
    this->init_key( SDL_SCANCODE_V, MCK::KeyEvent::KEY_V );
    this->init_key( SDL_SCANCODE_W, MCK::KeyEvent::KEY_W );
    this->init_key( SDL_SCANCODE_X, MCK::KeyEvent::KEY_X );
    this->init_key( SDL_SCANCODE_Y, MCK::KeyEvent::KEY_Y );
    this->init_key( SDL_SCANCODE_Z, MCK::KeyEvent::KEY_Z );

    // Create (empty) prime render block
    this->prime_render_block
        = std::make_shared<MCK::GameEngRenderBlock>();
    
    // Set initialization flag
    this->initialized = true;

#if defined MCK_STD_OUT
    std::cout << "Game engine (SDL2) initialization complete."
              << std::endl;
#endif
}

///////////////////////////////////////////////

void MCK::GameEng::render_all( 
    std::shared_ptr<MCK::GameEngRenderBlock> render_block,
    int16_t hoz_offset,
    int16_t vert_offset
) const
{
    if( !this->initialized || this->renderer == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot render all as SDL not yet init."
#else
            ""
#endif
        ) );
    }

    // Check if supplied render block is prime render block
    const bool IS_PRIME
        = render_block.get() == this->prime_render_block.get();

    // If supplied block NULL, ignore
    // Also if block is inactive (except prime render block), ignore
    if( render_block.get() == NULL
        || ( !IS_PRIME && !render_block->active )
    )
    {
        return;
    }

    // Calculate cumulative offset for this render block
    const int16_t HOZ_OFFSET = hoz_offset + render_block->hoz_offset;
    const int16_t VERT_OFFSET = vert_offset + render_block->vert_offset;

    // Iterate recursively over subservient blocks
    for( auto sub_block : render_block->sub_blocks )
    {
        try
        {
            this->render_all( sub_block, HOZ_OFFSET, VERT_OFFSET );
        }
        catch( std::exception &e )
        {
            // Issue warning but do not throw,
            // as we can try rendering other sub-blocks
#if defined MCK_STD_OUT
            std::cout << "Failed to render sub-block, error = "
                      << e.what() << std::endl;
#endif
        }
    }

    // Check for offset
    if( HOZ_OFFSET == 0 && VERT_OFFSET == 0 )
    {
        ///////////////////////////////////////
        // If no offset, use a faster loop

        // Iterate over all textures in block
        for( std::shared_ptr<MCK::GameEngRenderInfo> info 
                : render_block->render_info
        )
        {
            // Ignore NULL textures
            if( info->tex == NULL )
            {
                continue;
            }

            // If no flags, use simpler command
            if( info->flags == 0 )
            {
                try
                {
                    SDL_RenderCopy(
                        this->renderer,
                        info->tex,
                        info->clip ? &info->clip_rect.r : NULL,
                        &info->dest_rect.r
                    );
                }
                catch( std::exception &e )
                {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
                    std::cout << "SDL_RenderCopy failed, error = "
                              << SDL_GetError() << std::endl;
#endif
                }
            }
            else
            {
                // Get rotation angle (in degrees)
                const double ANGLE
                    = ( 
                        ( info->flags & MCK::GameEngRenderInfo::ROTATION_MASK )
                            >> MCK::GameEngRenderInfo::ROTATION_RSHIFT
                    ) * 90.0f;
            
                // Get flip
                SDL_RendererFlip flip = SDL_FLIP_NONE;
                if( info->get_flip_x() )
                {
                    flip = SDL_RendererFlip( flip | SDL_FLIP_HORIZONTAL );
                }
                if( info->get_flip_y() )
                {
                    flip = SDL_RendererFlip( flip | SDL_FLIP_VERTICAL );
                }
                
                try
                {
                    SDL_RenderCopyEx(
                        this->renderer,
                        &*info->tex,  // Convert from shared_ptr
                        info->clip ? &info->clip_rect.r : NULL,
                        &info->dest_rect.r,
                        ANGLE,
                        NULL,  // Rotate about centre
                        flip
                    );
                }
                catch( std::exception &e )
                {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
                    std::cout << "SDL_RenderCopyEx failed, error = "
                              << SDL_GetError() << std::endl;
#endif
                }
            }

        }
    }
    else
    {
        ///////////////////////////////////////
        // If offset used, calculate each
        // destination rectangle separately

        // Iterate over all textures in block
        for( std::shared_ptr<MCK::GameEngRenderInfo> info
                : render_block->render_info
        )
        {
            // Ignore NULL pointers and NULL textures
            if( info.get() == NULL || info->tex == NULL )
            {
                continue;
            }

            // Get offset destination rectangle
            SDL_Rect dest = info->dest_rect.r;
            dest.x += HOZ_OFFSET;
            dest.y += VERT_OFFSET;

            // If no flags, use simpler command
            if( info->flags == 0 )
            {
                try
                {
                    SDL_RenderCopy(
                        this->renderer,
                        info->tex,
                        info->clip ? &info->clip_rect.r : NULL,
                        &dest
                    );
                }
                catch( std::exception &e )
                {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
                    std::cout << "Offset SDL_RenderCopy failed, error = "
                              << SDL_GetError() << std::endl;
#endif
                }
            }
            else
            {
                // Get rotation angle (in degrees)
                const double ANGLE
                    = ( 
                        ( info->flags & MCK::GameEngRenderInfo::ROTATION_MASK )
                            >> MCK::GameEngRenderInfo::ROTATION_RSHIFT
                    ) * 90.0f;
            
                // Get flip
                SDL_RendererFlip flip = SDL_FLIP_NONE;
                if( info->get_flip_x() )
                {
                    flip = SDL_RendererFlip( flip | SDL_FLIP_HORIZONTAL );
                }
                if( info->get_flip_y() )
                {
                    flip = SDL_RendererFlip( flip | SDL_FLIP_VERTICAL );
                }
                
                try
                {
                    SDL_RenderCopyEx(
                        this->renderer,
                        info->tex,
                        info->clip ? &info->clip_rect.r : NULL,
                        &dest,
                        ANGLE,
                        NULL,  // Rotate about centre
                        flip
                    );
                }
                catch( std::exception &e )
                {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
                    std::cout << "Offset SDL_RenderCopyEx failed, error = "
                              << SDL_GetError() << std::endl;
#endif
                }
            }
        }
    }
}

void MCK::GameEng::create_texture(
    MCK_IMG_ID_TYPE image_id,
    MCK_PAL_ID_TYPE local_palette_id,
    uint8_t bits_per_pixel,
    uint16_t pitch_in_pixels,
    const std::vector<uint8_t> &pixel_data,
    const std::vector<uint8_t> &local_palette,
    MCK_TEX_ID_TYPE &tex_id,
    uint16_t &height_in_pixels
)
{
    // Set answers to default
    tex_id = 0;
    height_in_pixels = 0;

    if( !this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create texture as SDL not yet init."
#else
            ""
#endif
        ) );
    }

    if( bits_per_pixel == 0 || bits_per_pixel > 8 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create texture as bits per pixel invalid."
#else
            ""
#endif
        ) );
    }

    if( pixel_data.size() == 0 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create texture as no pixel data."
#else
            ""
#endif
        ) );
    }

    if( local_palette.size() != pow( 2, bits_per_pixel ) )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create texture as palette wrong size."
#else
            ""
#endif
        ) );
    }

    // Calculate texture id
    tex_id = MCK::GameEng::calc_tex_id( image_id, local_palette_id );

    // If this texture already exists, do nothing
    {
        std::map<MCK_TEX_ID_TYPE,SDL_Texture*>::iterator it 
            = textures.find( tex_id );
        if( it != textures.end() )
        {
            return;
        }
    }

    // Calculate image height
    height_in_pixels
        = uint16_t(
            std::ceil ( 
                float( pixel_data.size() * 8 ) 
                    / float( bits_per_pixel )
                        / float( pitch_in_pixels )
            )
        );

    // Create new (empty) SDL surface
    SDL_Surface* surface = NULL;
    try
    {
        surface = SDL_CreateRGBSurface(
            0,  // No flags used (as per SDL instructions)
            pitch_in_pixels,
            height_in_pixels,
            32,  // depth in bits
            0x00FF0000,  // Red mask
            0x0000FF00,  // Green mask
            0x000000FF,  // Blue mask
            0xFF000000   // Alpha mask
        );
    }
    catch( const std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Failed to create new SDL surface, SDL error = "
            + *SDL_GetError()
#else
            ""
#endif
        ) );
    }
    if( surface == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "No throw, but new SDL surface NULL, SDL error = "
            + *SDL_GetError()
#else
            ""
#endif
        ) );
    }

    // Check surface width/height
    if( surface->w != pitch_in_pixels
        || surface->h != height_in_pixels )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "New SDL Surface width and/or height does "
            "not match width/height used to create it."
#else
            ""
#endif
        ) );
    }

    // Check surface bit depth
    const SDL_PixelFormat* const FORMAT = surface->format;
    if( FORMAT == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "SDL Surface's format is NULL"
#else
            ""
#endif
        ) );
    }
    if( FORMAT->BitsPerPixel != 32 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "SDL Surface bit depth is not 32 as specified."
#else
            ""
#endif
        ) );
    }

    // Check surface has pixels (if width or height is zero,
    // this will be NULL)
    if( surface->pixels == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "SDL Surface pixel pointer is NULL, perhaps width/height zero?"
#else
            ""
#endif
        ) );
    }

    // Check surface pitch (based on image width and 32 bits per pixel)
    const int EXPECTED_PITCH = pitch_in_pixels * sizeof( uint32_t );
    if( EXPECTED_PITCH != surface->pitch )
    {
#if defined MCK_STD_OUT
        std::cout << "SDL Surface pitch is "
            << surface->pitch
            << ", bytes not "
            << EXPECTED_PITCH
            << " bytes as expected. ** This could be a bug in SDL. ** "
            << "Will override SDL surface pitch value to the "
            << "correct value, but be mindful of this if further "
            << "problems occur, particularly "
            << "the 'shearing' (diagonalization) of images."
            << std::endl;
#endif
        
        // Try to correct pitch
        surface->pitch = EXPECTED_PITCH;
    }

    // Get bit shifts for RGB and alpha
    const uint8_t RSHIFT = FORMAT->Rshift;
    const uint8_t GSHIFT = FORMAT->Gshift;
    const uint8_t BSHIFT = FORMAT->Bshift;
    const uint8_t ASHIFT = FORMAT->Ashift;

    // Get pitch of pixel array when read as double words (4 bytes) 
    const std::size_t WORD_PITCH = surface->pitch / 4;

    // Get end of pixel array in memory, for safety checks
    const uint32_t* const WORD_MAX = (uint32_t*) surface->pixels
                             + surface->h * WORD_PITCH;

    // Lock surface
    try
    {
        SDL_LockSurface( surface );
    }
    catch( const std::exception &e )
    {
        throw( std::runtime_error( 
#if defined MCK_STD_OUT
            "Unable to lock SDL surface."
#else
            ""
#endif
        ) );
    }

    // Loop over pixel data (ignoring any odds bits at the end)
    uint32_t* surface_word_pos = (uint32_t*) surface->pixels; 
    uint8_t bit_pos_within_byte = 0;
    size_t data_pos_in_bytes = 0;
    uint8_t current_byte = pixel_data[0];  // Checked size > 0
    const size_t NUM_PIXELS = pixel_data.size() * 8 / bits_per_pixel;
    for( size_t pixel_pos = 0; pixel_pos < NUM_PIXELS; pixel_pos++ )
    {
        // Construct colo(u)r ID from next set of bits in pixel data
        uint8_t palette_col_id = 0x00;
        for( uint8_t i = 0; i < bits_per_pixel; i++ )
        {
            // Calculate mask used to isolate bit from pixel
            // data, then increment bit position
            const uint8_t BIT_MASK = 0x01 << ( 7 - bit_pos_within_byte++ );

            // If current bit set to 1,
            // set i'th bit of 'palette_col_id'
            if( current_byte & BIT_MASK )
            {
                palette_col_id |= 0x01 << i;
            }

            // If this is end of byte, 
            // get next byte (unless this is the last byte)
            if( bit_pos_within_byte > 7
                && pixel_pos < NUM_PIXELS - 1
            )
            {
                // Set bit pos back to zero
                bit_pos_within_byte = 0;

                // Increment byte pos, and check does
                // not exceed data vector
                if( ++data_pos_in_bytes >= pixel_data.size() )
                {
                    throw( std::runtime_error( 
#if defined MCK_STD_OUT
                        "Data pos exceeds pixel data, internal error?"
#else
                        ""
#endif
                    ) );
                }

                // Get new pixel data byte
                current_byte = pixel_data[ data_pos_in_bytes ];
            }
        }
        
        // Check palette_col_id is valid
        if( palette_col_id >= local_palette.size() )
        {
            throw( std::runtime_error( 
#if defined MCK_STD_OUT
                "palette_col_id exceeds local_palette, internal error?"
#else
                ""
#endif
            ) );
        }

        // Get colo(u)r ID from local palette
        const uint8_t COL_ID = local_palette[ palette_col_id ];

        // Get RGBA values for this colo(u)r
        uint8_t r, g, b, a;
        try
        {
            MCK::GameEng::get_RGBA( COL_ID, r, g, b, a );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error( 
#if defined MCK_STD_OUT
                std::string( "Failed to decode colo(u)r ID " )
                + std::to_string( COL_ID )
                + std::string( ", error = " )
                + e.what()
#else
                ""
#endif
            ) );
        }

        // Calculate 32bit colo(u)r
        const uint32_t COL_32
            = ( r << RSHIFT )
              | ( g << GSHIFT )
              | ( b << BSHIFT )
              | ( a << ASHIFT );

        // Set surface pixel, and increment surface word position
        if( surface_word_pos < WORD_MAX )
        {
            *surface_word_pos++ = COL_32;
        }
        else
        {
            throw( std::runtime_error( 
#if defined MCK_STD_OUT
                "surface_word_pos exceeds WORD_MAX, internal error?"
#else
                ""
#endif
            ) );
        }
    }
    
    // Unlock surface
    try
    {
        SDL_UnlockSurface( surface );
    }
    catch( const std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Unable to unlock SDL surface."
#else
            ""
#endif
        ) );
    }

    //////////////////////////////////////////////////
    // Create texture from surface
    //
    
    // Warning, this command will 
    // produce a segfault if 'renderer' pointer is
    // set to a junk value
    SDL_Texture* texture = NULL;
    try
    {
        texture = SDL_CreateTextureFromSurface( this->renderer, surface );
    }
    catch( const std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Unable to create SDL texture, error: " 
            + *SDL_GetError()
#else
            ""
#endif
        ) );
    }
    catch( ... )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Create SDL texture has thrown non-exception, error: " 
            + *SDL_GetError()
#else
            ""
#endif
        ) );
    }

    if( texture == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "New SDL texture is NULL."
#else
            ""
#endif
        ) );
    }
    else
    {
        // Check texture size matches nominal size
        // of RAM image surface
        Uint32 format;
        int access, w, h;
        int rc = SDL_QueryTexture(
            texture,
            &format,
            &access,
            &w,
            &h );
        
        if( rc != 0 )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                "Unable to query new texture, error code = "
                + std::to_string( rc ) 
#else
                ""
#endif
            ) );
        }
        else
        {
            if( w != surface->w
                || h != surface->h )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    "Texture size not same as "
                    "nominal size of source surface."
#else
                    ""
#endif
                ) );
            }
        }
    }
   
    // Destroy surface
    try
    {
        SDL_FreeSurface( surface );
    }
    catch( const std::exception &e )
    {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
        // Issue a warning, but no point throwing exception here
        std::cout << "Unable to destroy SDL surface, error: " 
                  << SDL_GetError()
                  << std::endl;
#endif
    }

    // Store pointer to texture
    // This should not fail due to previous
    // check the texutre did not currently exist
    {
        this->textures.insert(
            std::pair<MCK_TEX_ID_TYPE,SDL_Texture*>(
                tex_id,
                texture
            )
        );
    }

#if defined MCK_STD_OUT && defined MCK_VERBOSE
    std::cout << "Created texture with id " << tex_id << std::endl;
#endif
}
        
std::shared_ptr<MCK::GameEngRenderBlock> MCK::GameEng::create_empty_render_block(
    std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
    bool add_to_front
) const
{
    if( !this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create empty render block as SDL not yet init."
#else
            ""
#endif
        ) );
    }

    // Let calling method catch any (unlikely) exceptions hereafter

    // Create new block
    std::shared_ptr<MCK::GameEngRenderBlock> new_block
        = std::make_shared<MCK::GameEngRenderBlock>();
            
    // Add to parent block, if exists
    if( parent_block.get() != NULL )
    {
        if( add_to_front )
        {
            parent_block->sub_blocks.push_front( new_block );
        }
        else
        {
            parent_block->sub_blocks.push_back( new_block );
        }
    }

    return new_block;
}

void MCK::GameEng::show( void ) const
{
    try
    {
        SDL_RenderPresent( this->renderer );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Presentation of renderer " )
            + std::string( "failed, SDL2 error message = " )
            + SDL_GetError()
#else
            ""
#endif
        ) );
    }
}
        
std::shared_ptr<MCK::GameEngRenderInfo> MCK::GameEng::create_render_info(
    MCK_TEX_ID_TYPE tex_id,
    std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
    MCK::GameEngRenderInfo::Rect dest_rect,
    bool clip,
    MCK::GameEngRenderInfo::Rect clip_rect,
    int rotation,
    bool flip_x,
    bool flip_y
) const
{
    if( !this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create render info as SDL not yet init."
#else
            ""
#endif
        ) );
    }

    // Get texture
    SDL_Texture* tex = NULL;
    {
        std::map<MCK_TEX_ID_TYPE,SDL_Texture*>::const_iterator it 
            = textures.find( tex_id );
        
        // If texture does not exist, throw exception
        if( it == textures.end() )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Cannot create render info as texture with id " )
                + std::to_string( tex_id )
                + std::string( " does not exist." )
#else
                ""
#endif
            ) );
        }

        tex = it->second;
    }

    if( tex == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot create render info as texture with id " )
            + std::to_string( tex_id )
            + std::string( " is NULL." )
#else
            ""
#endif
        ) );
    }

    // Create new info instance
    std::shared_ptr<MCK::GameEngRenderInfo> new_info
        = std::make_shared<MCK::GameEngRenderInfo>();
    new_info->tex = tex;
    new_info->tex_id = tex_id;
    new_info->dest_rect = dest_rect;
    new_info->clip = clip;
    new_info->clip_rect = clip_rect;
    new_info->flags = MCK::GameEngRenderInfo::calc_flags(
        rotation,
        flip_x,
        flip_y
    );

    // Associate info with render block, if render block
    // supplied (note, always added to end of render block)
    if( parent_block.get() != NULL )
    {
        parent_block->render_info.push_back( new_info );
    }

    return new_info;
}

void MCK::GameEng::clear( void ) const
{
    if( !this->initialized || this->renderer == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot clear screen as game engine not initialized."
#else
            ""
#endif
        ) );
    }

    // Clear window
    int rc = SDL_RenderClear( this->renderer );
    // -ve return code indicates failure
    if ( rc < 0 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to clear SDL renderer, error = " )
            + SDL_GetError()
#else
            ""
#endif
        ) );
    }
}

void MCK::GameEng::get_pending_keyboard_actions(
    std::vector< MCK::KeyEvent > &key_events
)
{
    // Clear answer
    key_events.clear();

    if( !this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot get keyboard actions as SDL2 not initialized."
#else
            ""
#endif
        ) );
    }
   
    // Tell SDL to retrieve any pending events
    SDL_PumpEvents();

    // Handle all pending events
    SDL_Event e;
    while ( SDL_PollEvent( &e ) )
    {
        // Declare KeyEvent instance, set to default values
        MCK::KeyEvent key_ev;

        // Set timestamp
        key_ev.timestamp = e.key.timestamp;

        // If quit signal received, treat same as pressing 
        // of quit key
        if( e.type == SDL_QUIT )
        {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
            std::cout << "SDL close window signal detected" << std::endl;
#endif

            key_ev.key_code = MCK::KeyEvent::Keys::KEY_QUIT;
            key_ev.status = MCK::KeyEvent::PRESSED;
            key_events.push_back( key_ev );
            continue;
        }

        // Otherwise, check for a key up or key down event
        const bool UP = e.type == SDL_KEYUP;
        const bool DOWN = e.type == SDL_KEYDOWN;
        if( !UP && !DOWN )
        {
            // For now, igore all other non-key input
            continue;
        }

        // For key presses, get status
        if( DOWN )
        {
            if( e.key.repeat )
            {
                key_ev.status = MCK::KeyEvent::Status::AUTO_REPEAT;
            }
            else
            {
                key_ev.status = MCK::KeyEvent::Status::PRESSED;
            }
        }
        else
        {
            key_ev.status = MCK::KeyEvent::Status::RELEASED;
        }

        // Determine key code (leave as invalid if not recognised)
        std::map<SDL_Scancode,MCK::KeyEvent::Keys>::const_iterator it 
            = scancodes.find( e.key.keysym.scancode );
        if( it != scancodes.end() )
        {
            key_ev.key_code = it->second;
        }

#if defined MCK_STD_OUT && defined MCK_VERBOSE
        if( key_ev.key_code == MCK::KeyEvent::KEY_QUIT )
        {
            std::cout << "Quit key event (status="
                      << key_ev.status << ")"
                      << std::endl;
        }
#endif

        // Store event
        key_events.push_back( key_ev );
    }
}

void MCK::GameEng::set_clearing_color( uint8_t global_color_id ) const
{
    if( global_color_id >= MCK::TOTAL_CORE_COLORS )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Cannot set clearing color as " )
                + std::string( "global color ID " )
                + std::to_string( global_color_id )
                + std::string( " is invalid." )
#else
                ""
#endif
        ) );
    }

    // Set renderer colour
    // (black, for clearing window)
    int rc = SDL_SetRenderDrawColor(
        this->renderer,
        CORE_PALETTE_REDS[ global_color_id ],
        CORE_PALETTE_GREENS[ global_color_id ],
        CORE_PALETTE_BLUES[ global_color_id ],
        CORE_PALETTE_ALPHAS[ global_color_id ]
    );
    // -ve return code indicates failure
    if ( rc < 0 )
    {
#if defined MCK_STD_OUT
        // Issue warning but do not throw execption
        std::cout << "Failed to set render clearing color, "
                  << "SDL2 error message = "
                  << SDL_GetError()
                  << std::endl;
#endif
    }
}
