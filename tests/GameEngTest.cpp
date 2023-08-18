///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
//  GameEngTest.cpp
//
//  Test/demo for GameEng class, including
//  structs GameEngRenderInfo and GameEngRenderBlock
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

/////////////////////////////////////////////////////////
// TOP LEVEL ENTRY POINT OF THE TEST APPLICATION
int main( int argc, char** argv )
{   
    // Define window size (these can be changed to any sensible value)
    const int WINDOW_WIDTH_IN_PIXELS = 640;
    const int WINDOW_HEIGHT_IN_PIXELS = 480;
   

    //////////////////////////////////////////////
    // INITIALIZE SDL, CREATE WINDOW & RENDERER
    MCK::GameEng &game_eng = MCK::GameEng::get_singleton();
    try
    {
        game_eng.init(
            "GameEngTest",  // Window name
            0,  // Window x pos
            0,  // Window y pos
            WINDOW_WIDTH_IN_PIXELS,
            WINDOW_HEIGHT_IN_PIXELS
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to initialize SDL, error: ")
            + e.what() ) );
    }


    ///////////////////////////////////////////
    // CREATE LOCAL COLO(U)R PALETTES
    // (subsets of the core 32 colo(u)r palette)
    
    // Black/white palette for 1bit images
    const MCK_PAL_ID_TYPE PALETTE_1BIT_BLACK_WHITE_ID = 0;
    const std::vector<uint8_t> PALETTE_1BIT_BLACK_WHITE =
    {
        MCK::COL_BLACK,
        MCK::COL_WHITE
    };

    // Transparent/white palette for 1bit images
    const MCK_PAL_ID_TYPE PALETTE_1BIT_TRANS_WHITE_ID = 1;
    const std::vector<uint8_t> PALETTE_1BIT_TRANS_WHITE =
    {
        MCK::COL_TRANSPARENT,
        MCK::COL_WHITE
    };

    // Cyan/purple palette for 1bit images
    const MCK_PAL_ID_TYPE PALETTE_1BIT_CYAN_PURP_ID = 2;
    std::vector<uint8_t> PALETTE_1BIT_CYAN_PURP =
    {
        MCK::COL_CYAN,
        MCK::COL_PURPLE
    };

    // Dark/V.Dark Grey palette for 1bit images
    const MCK_PAL_ID_TYPE PALETTE_1BIT_GRAY_ID = 3;
    std::vector<uint8_t> PALETTE_1BIT_GRAY =
    {
        MCK::COL_DARK_GRAY,
        MCK::COL_V_DARK_GRAY
    };


    ///////////////////////////////////////////
    // CREATE IMAGE DATA

    // 4x4 black pixels
    // Note the { 0 , 0 } here represents 16 binary digits,
    // i.e. 1 digit per pixel for 4 rows of 4 pixels,
    // all set to zero.
    const MCK_IMG_ID_TYPE IMAGE_BLANK_1BIT_4x4_ID = 0;
    const std::vector<uint8_t> IMAGE_BLANK_1BIT_4x4 = { 0, 0 };

    // Square made of 4x4 pixels and 2 colo(u)rs
    // Note the two 8bit binary literals here represent 4x4
    // 1bit pixels (similar to above). In effect, each
    // 8bit number is split in half to give 4 bits per line.
    const MCK_IMG_ID_TYPE IMAGE_SQUARE_1BIT_4x4_ID = 1;
    const std::vector<uint8_t> IMAGE_SQUARE_1BIT_4x4 =
    {
        0b11111001,
        0b10011111
    };

    // Biplane made of 8x8 pixels and 2 colo(u)rs
    // Note if you have a text editor capable of
    // highlighting search matches, then searching for 
    // '0' or '1' will highlight the image below in
    // a way that makes the pixels much easier to see.
    const MCK_IMG_ID_TYPE IMAGE_BIPLANE_1BIT_8x8_ID = 2;
    const std::vector<uint8_t> IMAGE_BIPLANE_1BIT_8x8 =
    {
        0b00111110,
        0b10001000,
        0b11111101,
        0b11111111,
        0b01111101,
        0b00001000,
        0b00000110,
        0b00000110
    };


    ///////////////////////////////////////////
    // CREATE TEXTURES

    // Plain black texture
    MCK_TEX_ID_TYPE black_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                IMAGE_BLANK_1BIT_4x4_ID,
                PALETTE_1BIT_BLACK_WHITE_ID,
                1,  // bits_per_pixel,
                4,  // pitch_in_pixels,
                IMAGE_BLANK_1BIT_4x4,
                PALETTE_1BIT_BLACK_WHITE,
                black_tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create biplane texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 4 )
        {
            throw( std::runtime_error(
                std::string( "Black texture has incorrect height")
            ) );
        }
    }


    // Biplane
    MCK_TEX_ID_TYPE biplane_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                IMAGE_BIPLANE_1BIT_8x8_ID,
                PALETTE_1BIT_TRANS_WHITE_ID,
                1,  // bits_per_pixel,
                8,  // pitch_in_pixels,
                IMAGE_BIPLANE_1BIT_8x8,
                PALETTE_1BIT_TRANS_WHITE,
                biplane_tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create biplane texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 8 )
        {
            throw( std::runtime_error(
                std::string( "Biplane texture has incorrect height")
            ) );
        }
    }

    // Square (cyan/purple)
    MCK_TEX_ID_TYPE square_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                IMAGE_SQUARE_1BIT_4x4_ID,
                PALETTE_1BIT_CYAN_PURP_ID,
                1,  // bits_per_pixel,
                4,  // pitch_in_pixels,
                IMAGE_SQUARE_1BIT_4x4,
                PALETTE_1BIT_CYAN_PURP,
                square_tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create square texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 4 )
        {
            throw( std::runtime_error(
                std::string( "Square texture has incorrect height")
            ) );
        }
    }

    // Square (two-tone grey)
    MCK_TEX_ID_TYPE gray_square_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                IMAGE_SQUARE_1BIT_4x4_ID,
                PALETTE_1BIT_GRAY_ID,
                1,  // bits_per_pixel,
                4,  // pitch_in_pixels,
                IMAGE_SQUARE_1BIT_4x4,
                PALETTE_1BIT_GRAY,
                gray_square_tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create square texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 4 )
        {
            throw( std::runtime_error(
                std::string( "Square texture has incorrect height")
            ) );
        }
    }


    ///////////////////////////////////////////
    // CREATE DEMO PARAMETERS
    const int BIPLANE_SIZE = 8;  // Raw size, in pixels
    const int BIPLANE_SCALE = 4;
    int biplane_x = 30;
    int biplane_y = 50;
    const int SQUARE_SIZE = 4;  // Raw size, in pixels
    const int SQUARE_SCALE = 8;
    const int SQUARE_TOTAL_SIZE = SQUARE_SIZE * SQUARE_SCALE;
    int scroll_offset = SQUARE_TOTAL_SIZE - 1;
    const int NUM_TOWERS = WINDOW_WIDTH_IN_PIXELS / SQUARE_TOTAL_SIZE - 1;
    const int MAX_TOWER_HEIGHT_IN_SQUARES = 8;
    const int TOWER_FLOOR_IN_PIXELS_FROM_TOP_OF_WINDOW
        = WINDOW_HEIGHT_IN_PIXELS - SQUARE_TOTAL_SIZE;


    ///////////////////////////////////////////
    // CREATE RENDER BLOCKS
    std::shared_ptr<MCK::GameEngRenderBlock> border_overlay_block;
    std::shared_ptr<MCK::GameEngRenderBlock> biplane_block;
    std::shared_ptr<MCK::GameEngRenderBlock> city_block;
    std::shared_ptr<MCK::GameEngRenderBlock> background_block;
    try
    {
        // Note: The order here is important as it
        //       defines the order in which blocks
        //       are rendered. Those placed at the
        //       front of the block will be rendered
        //       in front of the rest of the block.
        //       The border overlay is the front-most sub-block
        //       in prime render block, so that it
        //       is rendered after everything else
        biplane_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block()
        );
        city_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block()
        );
        background_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block(),
            false  // Add to *back* of block
        );
        border_overlay_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block(),  // 'prime render block' is the topmost block, created automatically by GameEng
            true  // Add to *front* of block, see note above
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create render blocks, error: ")
            + e.what() ) );
    }

    // Set scroll offset for city block
    city_block->hoz_offset = scroll_offset;

    // Set scroll offset for background
    background_block->hoz_offset = scroll_offset;

    // Create mutliple blocks, all subservient to 'city_block'
    // Each block will hold a 'tower' of square images of random height
    std::list<std::shared_ptr<MCK::GameEngRenderBlock>> tower_blocks;
    for( int i = 0; i < NUM_TOWERS; i++ )
    {
        std::shared_ptr<MCK::GameEngRenderBlock> block;
        try
        {
            // Create a new (empty) render block, subservient to 'city_block'
            block = game_eng.create_empty_render_block( city_block );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create render block, error: ")
                + e.what() ) );
        }
        
        // Set horizontal offset, as an exact multiple of tower width
        // This is necessary, otherwise the towers would be superimposed
        // on top of each other.
        block->hoz_offset = i * SQUARE_TOTAL_SIZE;

        // Store tower block pointer, for future reference
        // (although it can also be retrieved from 'city_block')
        tower_blocks.push_back( block );
    }

    // Create mutliple tower blocks at half scale,
    // all subservient to 'background'
    // Each block will hold a 'tower' of half scale square images of random height
    std::list<std::shared_ptr<MCK::GameEngRenderBlock>> background_tower_blocks;
    for( int i = 0; i < NUM_TOWERS * 2; i++ )
    {
        std::shared_ptr<MCK::GameEngRenderBlock> block;
        try
        {
            block = game_eng.create_empty_render_block( background_block );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create render block, error: ")
                + e.what() ) );
        }
        
        // Set horizontal offset
        block->hoz_offset = i * SQUARE_TOTAL_SIZE / 2;

        // Store tower block pointer, for future reference
        // (although it can also be retrieved from 'background_block')
        background_tower_blocks.push_back( block );
    }

    ///////////////////////////////////////////
    // CREATE RENDER INFO

    // Create border overlay render info, 
    // and associate with border overlay block
    try
    {
        // Lefthand border
        game_eng.create_render_info(
            black_tex_id,
            border_overlay_block,
            MCK::GameEngRenderInfo::Rect( 
                0,  // x pos
                0,  // y pos
                SQUARE_TOTAL_SIZE,  // width equal to one tower
                WINDOW_HEIGHT_IN_PIXELS  // full height of window 
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create left border, error: ")
            + e.what() ) );
    }
    try
    {
        // Righthand border
        game_eng.create_render_info(
            black_tex_id,
            border_overlay_block,
            MCK::GameEngRenderInfo::Rect( 
                WINDOW_WIDTH_IN_PIXELS - SQUARE_TOTAL_SIZE,  // x pos
                0,  // y pos
                SQUARE_TOTAL_SIZE,  // width equal to one tower
                WINDOW_HEIGHT_IN_PIXELS  // full height of window
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create left border, error: ")
            + e.what() ) );
    }

    // Create biplane render info, and associate with biplane block
    std::shared_ptr<MCK::GameEngRenderInfo> biplane_info;
    {
        try
        {
            biplane_info = game_eng.create_render_info(
                biplane_tex_id,
                biplane_block,
                MCK::GameEngRenderInfo::Rect( 
                    biplane_x,
                    biplane_y,
                    8 * BIPLANE_SCALE,
                    8 * BIPLANE_SCALE
                )
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create biplane render info, error: ")
                + e.what() ) );
        }
    }

    // Create squares render info, and associate with tower blocks
    {
        int x = 0;  // Enumerates tower blocks

        // Loop over tower blocks
        for( auto block : tower_blocks )
        {
            // Generate tower height, in squares
            const int TOWER_HEIGHT
                = rand() % ( MAX_TOWER_HEIGHT_IN_SQUARES - 1 ) + 1;

            // Loop over each square
            for( int y = 0; y < TOWER_HEIGHT; y++ )
            {
                try
                {
                    // Create square and add to tower
                    game_eng.create_render_info(
                        square_tex_id,  // texture ID
                        block,  // parent (tower) block
                        MCK::GameEngRenderInfo::Rect( 
                            0,  // x pos
                            TOWER_FLOOR_IN_PIXELS_FROM_TOP_OF_WINDOW - ( y + 1 ) * SQUARE_TOTAL_SIZE,  // y pos
                            SQUARE_TOTAL_SIZE,  // square width
                            SQUARE_TOTAL_SIZE  // square height
                        )
                    );
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
                        std::string( "Failed to create square render info, error: ")
                        + e.what() ) );
                }
            }
        
            x++;
        }
    }

    // Create half-size squares render info,
    // and associate with background tower blocks
    {
        int x = 0;  // Enumerates tower blocks

        // Loop over background tower blocks
        for( auto block : background_tower_blocks )
        {
            // Generate tower height, in squares
            const int TOWER_HEIGHT = rand() % 12;

            // Loop over each square
            for( int y = 0; y < TOWER_HEIGHT; y++ )
            {
                try
                {
                    // Create square and add to tower
                    game_eng.create_render_info(
                        gray_square_tex_id,  // texture ID
                        block,  // parent (tower) block
                        MCK::GameEngRenderInfo::Rect( 
                            0,  // x pos
                            TOWER_FLOOR_IN_PIXELS_FROM_TOP_OF_WINDOW - ( y + 1 ) * SQUARE_TOTAL_SIZE / 2,  // y pos
                            SQUARE_TOTAL_SIZE / 2,  // square width
                            SQUARE_TOTAL_SIZE / 2  // square height
                        )
                    );
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
                        std::string( "Failed to create half square render info, error: ")
                        + e.what() ) );
                }
            }
        
            x++;
        }
    }

    /////////////////////////////////////////////
    // MAIN LOOP STARTS HERE
    /////////////////////////////////////////////
    const float TARGET_FPS = 100;
    const uint32_t TICKS_PER_FRAME
        = uint32_t( 1000.0f / TARGET_FPS + 0.5f );
    uint32_t frame_num = 0;
    uint32_t current_ticks = game_eng.get_ticks();
    uint32_t start_ticks = current_ticks;
    uint32_t next_frame_ticks = current_ticks + TICKS_PER_FRAME; 
    uint32_t end_ticks = current_ticks + 13500;
    const float SCROLL_RATE = 0.1f;  // Pixels per tick
    do
    {
        ////////////////////////////////////////
        // Time management

        // Give time back to operating system
        game_eng.delay( TICKS_PER_FRAME / 4 );

        // Get current time
        current_ticks = game_eng.get_ticks();

        // If next frame not due, ignore
        if( current_ticks < next_frame_ticks )
        {
            continue;
        }
        else
        {
            // Calculate time of *next* frame
            next_frame_ticks = current_ticks + TICKS_PER_FRAME;
            frame_num++;
        }

        ////////////////////////////////////////
        // Handle user input
        {
            // Get pending keyboard (or quit) events
            std::vector< MCK::KeyEvent > events;
            try
            {
                game_eng.get_pending_keyboard_actions( events );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to get use input, error: ")
                    + e.what() ) );
            }

            for( auto e : events )
            {
                // For now, only check for quit
                if( e.key_code == MCK::KeyEvent::KEY_QUIT
                    && e.status == MCK::KeyEvent::PRESSED
                )
                {
                    // Just exit: GameEng instance will be
                    // destroyed automatically, and SDL 
                    // shut down safely
                    exit( 0 );
                }

                // TODO: Other keyboard input
            }
        }

        ////////////////////////////////////////
        // Handle scrolling of foreground
        {
            // Calculate current scroll position, in pixels
            const int PREV_OFFSET = city_block->hoz_offset;
            city_block->hoz_offset =
                SQUARE_TOTAL_SIZE 
                    - uint32_t( 
                        float( current_ticks - start_ticks )
                            * SCROLL_RATE
                    )  % SQUARE_TOTAL_SIZE;

            // If offset has reached minimum,
            // shift all the towers one square left
            // and place the leftmost tower on the right
            if( PREV_OFFSET < city_block->hoz_offset )
            {
                for( auto block : city_block->sub_blocks )
                {
                    block->hoz_offset -= SQUARE_TOTAL_SIZE;
                    if( block->hoz_offset < 0 )
                    {
                        block->hoz_offset
                            += SQUARE_TOTAL_SIZE * NUM_TOWERS; 
                    }
                }
            }
        }

        ////////////////////////////////////////
        // Handle scrolling of background
        {
            // Calculate current scroll position, in pixels
            const int PREV_OFFSET = background_block->hoz_offset;
            background_block->hoz_offset =
                SQUARE_TOTAL_SIZE / 2
                    - uint32_t( 
                        float( current_ticks - start_ticks )
                            * SCROLL_RATE / 2
                    )  % ( SQUARE_TOTAL_SIZE / 2 );

            // If offset has reached minimum,
            // shift all the towers one square left
            // and place the leftmost tower on the right
            if( PREV_OFFSET < background_block->hoz_offset )
            {
                for( auto block : background_block->sub_blocks )
                {
                    block->hoz_offset -= SQUARE_TOTAL_SIZE / 2;
                    if( block->hoz_offset < 0 )
                    {
                        // Note here * 2 and / 2 cancel out
                        block->hoz_offset
                            += SQUARE_TOTAL_SIZE * NUM_TOWERS; 
                    }
                }
            }
        }

        ////////////////////////////////////////
        // Move biplane
        if( biplane_block->hoz_offset < WINDOW_WIDTH_IN_PIXELS - 50 )
        {
            biplane_block->hoz_offset
                = frame_num % WINDOW_WIDTH_IN_PIXELS;
        }

        // Clear, render and present
        {
            try
            {
                game_eng.clear();
                game_eng.render_all(
                    game_eng.get_prime_render_block(),
                    0,  // Hoz offset
                    0  // Vert offset
                );
                game_eng.show();
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Render sequence failed, error: ")
                    + e.what() ) );
            }
        }
    }
    while( current_ticks < end_ticks );

    // Note: SDL is closed down when 'game_eng'
    // goes out of scope
}

