///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
//  ImageManTest.cpp
//
//  Test/demo for ImageMan and associated classes
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

#include "GameEng.h"
#include "ImageMan.h"

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


    //////////////////////////////////////////////
    // INITIALIZE IMAGE MANAGER
    MCK::ImageMan &image_man = MCK::ImageMan::get_singleton();
    try
    {
        image_man.init( game_eng );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to initialize Image Manager, error: ")
            + e.what() ) );
    }


    ///////////////////////////////////////////
    // CREATE DEMO PARAMETERS
    // TODO...

    ///////////////////////////////////////////
    // CREATE RENDER BLOCKS
    std::shared_ptr<MCK::GameEngRenderBlock> border_overlay_block;
    std::shared_ptr<MCK::GameEngRenderBlock> ascii_1_block;
    std::shared_ptr<MCK::GameEngRenderBlock> ascii_2_block;
    try
    {
        // Note: The order here is important as it
        //       defines the order in which blocks
        //       are rendered. Those placed at the
        //       front of the list will be rendered
        //       first, i.e. in the background.
        //       The border overlay is the *last* sub-block
        //       in prime render block, so that it
        //       is rendered after everything else
        border_overlay_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block(),  // 'prime render block' is the topmost block, created automatically by GameEng
            false  // Add to *back*, see note above
        );
        ascii_1_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block()
        );
        ascii_2_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block()
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create render blocks, error: ")
            + e.what() ) );
    }

    // Set scroll offset for ascii blocks
    ascii_1_block->hoz_offset = 0;
    ascii_2_block->hoz_offset = 0;

    
    ///////////////////////////////////////////
    // CREATE LOCAL PALETTES
    MCK_PAL_ID_TYPE black_white_palette_id;
    try
    {
         black_white_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_BLACK,
                    MCK::COL_WHITE
                }
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create black/white palette, error: ")
            + e.what() ) );
    }
    
    MCK_PAL_ID_TYPE purple_yellow_palette_id;
    try
    {
         purple_yellow_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_PURPLE,
                    MCK::COL_YELLOW
                }
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create transparent/yellow palette, error: ")
            + e.what() ) );
    }
    

    ///////////////////////////////////////////
    // CREATE RENDER INFO

    try
    {
        image_man.create_extended_ascii_image(
            'A',
            black_white_palette_id,
            50,
            30,
            4,
            4,
            ascii_1_block
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create 'A' image, error: ")
            + e.what() ) );
    }

    try
    {
        image_man.create_extended_ascii_image(
            'a',
            purple_yellow_palette_id,
            100,
            30,
            4,
            4,
            ascii_2_block
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create 'a' image, error: ")
            + e.what() ) );
    }

    /*
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
                32,
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
                WINDOW_WIDTH_IN_PIXELS - 32,  // x pos
                0,  // y pos
                32,
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
    */


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

