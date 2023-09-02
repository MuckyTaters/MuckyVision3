///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
 //  GameEngAudioTest.cpp
//
//  Test/demo for GameEngAudio class.
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
#include "GameEngAudio.h"
#include "VoiceSynth.h"

/////////////////////////////////////////////////////////
// TOP LEVEL ENTRY POINT OF THE TEST APPLICATION
int main( int argc, char** argv )
{   
    // Define window size (these can be changed to any sensible value)
    const int WINDOW_WIDTH_IN_PIXELS = 320;
    const int WINDOW_HEIGHT_IN_PIXELS = 240;
  

    //////////////////////////////////////////////
    // INITIALIZE SDL, CREATE WINDOW & RENDERER
    MCK::GameEng &game_eng = MCK::GameEng::get_singleton();
    try
    {
        game_eng.init(
            "GameEngAudioTest",  // Window name
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
    // INITIALIZE SDL AUDIO
    try
    {
        MCK::GameEngAudio::init(
            0xFF  // Master volume
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to initialize SDL Audio, error: ")
            + e.what() ) );
    }


    ///////////////////////////////////////////
    // CREATE LOCAL COLO(U)R PALETTES
    // (subsets of the core 32 colo(u)r palette)
    
    // Black/white palette for 1bit images
    const MCK_PAL_ID_TYPE PALETTE_ID = 0;
    const std::vector<uint8_t> PALETTE =
    {
        MCK::COL_BLACK,
        MCK::COL_WHITE
    };


    ///////////////////////////////////////////
    // CREATE IMAGE DATA

    // Square made of 8x8 pixels and 2 colo(u)rs
    // Note the two 8bit binary literals here represent 8x8
    // 1bit pixels (similar to above). In effect, each
    // 8bit number is split in half to give 4 bits per line.
    const MCK_IMG_ID_TYPE IMAGE_ID = 1;
    const std::vector<uint8_t> IMAGE =
    {
        0b00000000, 0b00000000,
        0b00000000, 0b01000000,
        0b00000000, 0b11000001,
        0b00000001, 0b01000001,
        0b00000010, 0b01000101,
        0b01111100, 0b01000101,
        0b01000000, 0b01010101,
        0b01000000, 0b01010101,
        0b01000000, 0b01010101,
        0b01000000, 0b01010101,
        0b01000000, 0b01010101,
        0b01111100, 0b01000101,
        0b00000010, 0b01000101,
        0b00000001, 0b01000001,
        0b00000000, 0b11000001,
        0b00000000, 0b01000000
    };


    ///////////////////////////////////////////
    // CREATE TEXTURES

    // Square (cyan/purple)
    MCK_TEX_ID_TYPE tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                IMAGE_ID,
                PALETTE_ID,
                1,  // bits_per_pixel,
                16,  // pitch_in_pixels,
                IMAGE,
                PALETTE,
                tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 16 )
        {
            throw( std::runtime_error(
                std::string( "Texture has incorrect height")
            ) );
        }
    }


    ///////////////////////////////////////////
    // CREATE RENDER INFO
    try
    {
        game_eng.create_render_info(
            tex_id,
            game_eng.get_prime_render_block(),
            MCK::GameEngRenderInfo::Rect( 
                20,  // x pos
                20,  // y pos
                128, // width
                128  // height 
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create render info, error: ")
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

