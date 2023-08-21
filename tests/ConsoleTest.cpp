///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
//  ImageManTest.cpp
//
//  Test/demo for ImageMan and associated classes
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
#include "ImageMan.h"
#include "ImageText.h"
#include "Console.h"

/////////////////////////////////////////////////////////
// TOP LEVEL ENTRY POINT OF THE TEST APPLICATION
int main( int argc, char** argv )
{   
    // Define window size (these can be changed to any sensible value)
    const int WINDOW_WIDTH_IN_PIXELS = 640;
    const int WINDOW_HEIGHT_IN_PIXELS = 360;
   

    //////////////////////////////////////////////
    // INITIALIZE SDL, CREATE WINDOW & RENDERER
    MCK::GameEng &game_eng = MCK::GameEng::get_singleton();
    try
    {
        game_eng.init(
            "ConsoleTest",  // Window name
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
            std::string( "Failed to create black white palette, error: ")
            + e.what() ) );
    }

    ///////////////////////////////////////////
    // CREATE DEMO PARAMETERS
    const uint8_t TILE_WIDTH = 16;
    const uint8_t TILE_HEIGHT = 16;
    const uint8_t MAIN_CONSOLE_LEFT = TILE_WIDTH * 1;
    const uint8_t MAIN_CONSOLE_TOP = TILE_HEIGHT * 1;
    const uint8_t MAIN_CONSOLE_WIDTH_IN_CHARS = 24;
    const uint8_t MAIN_CONSOLE_HEIGHT_IN_CHARS = 10;
    const uint16_t MAIN_CONSOLE_WIDTH_IN_PIXELS
        = TILE_WIDTH * MAIN_CONSOLE_WIDTH_IN_CHARS;
    const uint16_t MAIN_CONSOLE_HEIGHT_IN_PIXELS
        = TILE_HEIGHT * MAIN_CONSOLE_HEIGHT_IN_CHARS;
    const uint8_t BG_COL = MCK::COL_ROTTING_PURPLE;

    ////////////////////////////////////////////
    // Set clearing colo(u)r

    // Set render clearing colo(u)r for this demo
    try
    {
        game_eng.set_clearing_color( BG_COL );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to set clearing color, error: ")
            + e.what() ) );
    }

    /////////////////////////////////////////////
    // TEST CONSOLE
    std::shared_ptr<MCK::Console> console_test
        = std::make_shared<MCK::Console>();
    try
    {
        std::string marker_symbol( 1, uint8_t( 254 ) );
        std::string s;
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        s += "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
        console_test->init(
            game_eng,
            image_man,
            game_eng.get_prime_render_block(),
            black_white_palette_id,
            MAIN_CONSOLE_TOP,  // x_pos,
            MAIN_CONSOLE_LEFT,  // y_pos,
            MAIN_CONSOLE_WIDTH_IN_CHARS,
            MAIN_CONSOLE_HEIGHT_IN_CHARS,
            TILE_WIDTH,  // char_width_in_pixels,
            TILE_HEIGHT,  // char_height_in_pixels,
            s,  // initial_content,
            100,  // print_speed_in_ticks_per_char,
            10,  // scroll_speed_in_ticks_per_pixel,
            true,  // hoz_text_alignment
            MAIN_CONSOLE_HEIGHT_IN_CHARS - 1,  // start_line
            false // true  // add_to_front_of_parent_block = true
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create main console, error: ")
            + e.what() ) );
    }

    ///////////////////////////////////////////
    // CREATE RENDER BLOCK(S)
    std::shared_ptr<MCK::GameEngRenderBlock> border_overlay_block;
    std::shared_ptr<MCK::GameEngRenderBlock> underlay_block;
    try
    {
        border_overlay_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block(),
            true  // Display on top of other blocks
        );
        underlay_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block(),
            false  // Display behind other blocks
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create render block(s), error: ")
            + e.what() ) );
    }


    ///////////////////////////////////////////
    // CREATE RENDER INFO

    // Create border overlay render info, 
    // and associate with border overlay block
    try
    {
        // Main console write line underlay
        game_eng.create_blank_tex_render_info(
            MCK::COL_BLACK,
            underlay_block,
            MCK::GameEngRenderInfo::Rect( 
                MAIN_CONSOLE_LEFT,  // x pos
                MAIN_CONSOLE_TOP
                    + MAIN_CONSOLE_HEIGHT_IN_PIXELS
                        - TILE_HEIGHT,  // y pos
                MAIN_CONSOLE_WIDTH_IN_PIXELS,
                TILE_HEIGHT  // Height, in pixels
            )
        );

        // Top border
        game_eng.create_blank_tex_render_info(
            BG_COL,
            border_overlay_block,
            MCK::GameEngRenderInfo::Rect( 
                0,  // x pos
                0,  // y pos
                WINDOW_WIDTH_IN_PIXELS,  // full width of window 
                MAIN_CONSOLE_TOP  // Height, in pixels
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create borders, error: ")
            + e.what() ) );
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
    uint32_t end_ticks = current_ticks + 50000;
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

        // Upate console 
        try
        {
            console_test->update( current_ticks );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Console update failed, error: ")
                + e.what() ) );
        }

        // Clear, render and present
        {
            try
            {
                game_eng.clear();
                game_eng.render_all(
                    game_eng.get_prime_render_block(),
                    0,  // Hoz offset
                    0,  // Vert offset
                    true  // Integrity checks
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


}
