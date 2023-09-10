///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
//  GameEngZTest.cpp
//
//  Test/demo for 'z' values in
//  GameEng class, including
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
            "GameEngZTest",  // Window name
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

    // Transparent/red palette for 1bit images
    const MCK_PAL_ID_TYPE PALETTE_1BIT_TRANS_RED_ID = 1;
    const std::vector<uint8_t> PALETTE_1BIT_TRANS_RED =
    {
        MCK::COL_TRANSPARENT,
        MCK::COL_RED
    };

    // Transparent/green palette for 1bit images
    const MCK_PAL_ID_TYPE PALETTE_1BIT_TRANS_GREEN_ID = 2;
    const std::vector<uint8_t> PALETTE_1BIT_TRANS_GREEN =
    {
        MCK::COL_TRANSPARENT,
        MCK::COL_GREEN
    };

    // Transparent/blue palette for 1bit images
    const MCK_PAL_ID_TYPE PALETTE_1BIT_TRANS_BLUE_ID = 3;
    const std::vector<uint8_t> PALETTE_1BIT_TRANS_BLUE =
    {
        MCK::COL_TRANSPARENT,
        MCK::COL_BLUE
    };


    ///////////////////////////////////////////
    // CREATE IMAGE DATA

    // Circle of 16x16 pixels and 2 colo(u)rs
    // Note the two 8bit binary literals here represent 8x8
    // 1bit pixels (similar to above). In effect, each
    // 8bit number is split in half to give 4 bits per line.
    const MCK_IMG_ID_TYPE IMAGE_CIRCLE_1BIT_16x16_ID = 0;
    const std::vector<uint8_t> IMAGE_CIRCLE_1BIT_16x16 =
    {
        0b00000111, 0b11100000,
        0b00011111, 0b11111000,
        0b00111111, 0b11111100,
        0b01111111, 0b11111110,
        0b01111111, 0b11111110,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b01111111, 0b11111110,
        0b01111111, 0b11111110,
        0b00111111, 0b11111100,
        0b00011111, 0b11111000,
        0b00000111, 0b11100000
    };


    ///////////////////////////////////////////
    // CREATE TEXTURES

    // Red ball texture
    MCK_TEX_ID_TYPE red_circle_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                IMAGE_CIRCLE_1BIT_16x16_ID,
                PALETTE_1BIT_TRANS_RED_ID,
                1,  // bits_per_pixel,
                16,  // pitch_in_pixels,
                IMAGE_CIRCLE_1BIT_16x16,
                PALETTE_1BIT_TRANS_RED,
                red_circle_tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create red circle texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 16 )
        {
            throw( std::runtime_error(
                std::string( "Red circle texture has incorrect height")
            ) );
        }
    }

    // Green ball texture
    MCK_TEX_ID_TYPE green_circle_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                IMAGE_CIRCLE_1BIT_16x16_ID,
                PALETTE_1BIT_TRANS_GREEN_ID,
                1,  // bits_per_pixel,
                16,  // pitch_in_pixels,
                IMAGE_CIRCLE_1BIT_16x16,
                PALETTE_1BIT_TRANS_GREEN,
                green_circle_tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create green circle texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 16 )
        {
            throw( std::runtime_error(
                std::string( "Green circle texture has incorrect height")
            ) );
        }
    }

    // Blue ball texture
    MCK_TEX_ID_TYPE blue_circle_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                IMAGE_CIRCLE_1BIT_16x16_ID,
                PALETTE_1BIT_TRANS_BLUE_ID,
                1,  // bits_per_pixel,
                16,  // pitch_in_pixels,
                IMAGE_CIRCLE_1BIT_16x16,
                PALETTE_1BIT_TRANS_BLUE,
                blue_circle_tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create blue circle texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 16 )
        {
            throw( std::runtime_error(
                std::string( "Green circle texture has incorrect height")
            ) );
        }
    }


    ///////////////////////////////////////////
    // CREATE DEMO PARAMETERS
    const float PATH_CENTER_X = WINDOW_WIDTH_IN_PIXELS / 2;
    const float PATH_CENTER_Y = WINDOW_HEIGHT_IN_PIXELS / 2;
    const float PATH_RADIUS_X = 200;
    const float PATH_RADIUS_Y = 100;
    const float PATH_RADIUS_Z = 200;
    const float ANG_VEL_IN_REV_PER_SEC = 0.2f;
    const float ANG_DIFF_IN_REVS = 0.06f;  // 0.05f;

    ///////////////////////////////////////////
    // CREATE RENDER INFO

    std::shared_ptr<MCK::GameEngRenderInfo> red_circle;
    std::shared_ptr<MCK::GameEngRenderInfo> green_circle;
    std::shared_ptr<MCK::GameEngRenderInfo> blue_circle;
    try
    {
        red_circle = game_eng.create_render_info(
            red_circle_tex_id,
            game_eng.get_prime_render_block(),
            MCK::GameEngRenderInfo::Rect( 
                0,  // x pos - overwritten on first frame
                0,  // y pos - overwritten on first frame
                64,  // width 
                64  // height
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create red circle, error: ")
            + e.what() ) );
    }
    try
    {
        green_circle = game_eng.create_render_info(
            green_circle_tex_id,
            game_eng.get_prime_render_block(),
            MCK::GameEngRenderInfo::Rect( 
                0,  // x pos - overwritten on first frame
                0,  // y pos - overwritten on first frame
                64,  // width 
                64  // height
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create green circle, error: ")
            + e.what() ) );
    }
    try
    {
        blue_circle = game_eng.create_render_info(
            blue_circle_tex_id,
            game_eng.get_prime_render_block(),
            MCK::GameEngRenderInfo::Rect( 
                0,  // x pos - overwritten on first frame
                0,  // y pos - overwritten on first frame
                64,  // width 
                64  // height
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create blue circle, error: ")
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

        // TODO: Movement

        // Get current angle
        const float PRIME_ANGLE = float( current_ticks )
                                / 1000.0f
                                    * ANG_VEL_IN_REV_PER_SEC
                                        * MCK_TWO_PI;

        // Set red circle using prime angle
        {
            const int X = PATH_CENTER_X 
                            + int( sin( PRIME_ANGLE )
                                    * PATH_RADIUS_X + 0.5f );
            
            const int Y = PATH_CENTER_Y
                            + int( cos( PRIME_ANGLE )
                                    * PATH_RADIUS_Y + 0.5f );
            
            const int Z = MCK::DEFAULT_Z_VALUE
                            + int( cos( PRIME_ANGLE )
                                    * PATH_RADIUS_Z + 0.5f );

            red_circle->dest_rect.set_x( X );
            red_circle->dest_rect.set_y( Y );

            // Change 'z' value
            try
            {
                MCK::GameEng::change_z(
                    red_circle,
                    game_eng.get_prime_render_block(),
                    Z
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to change z, error: ")
                    + e.what() ) );
            }
        }

        // Set green circle using trailing angle
        {
            const float ANG = PRIME_ANGLE 
                                + ANG_DIFF_IN_REVS * MCK_TWO_PI;
            const int X = PATH_CENTER_X 
                            + int( sin( ANG )
                                    * PATH_RADIUS_X + 0.5f );
            
            const int Y = PATH_CENTER_Y
                            + int( cos( ANG )
                                    * PATH_RADIUS_Y + 0.5f );
            
            const int Z = MCK::DEFAULT_Z_VALUE
                            + int( cos( ANG )
                                    * PATH_RADIUS_Z + 0.5f );

            green_circle->dest_rect.set_x( X );
            green_circle->dest_rect.set_y( Y );

            // Change 'z' value
            try
            {
                MCK::GameEng::change_z(
                    green_circle,
                    game_eng.get_prime_render_block(),
                    Z
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to change z, error: ")
                    + e.what() ) );
            }
        }

        // Set blue circle using trailing angle
        {
            const float ANG = PRIME_ANGLE 
                                + 2.0f * ANG_DIFF_IN_REVS * MCK_TWO_PI;
            const int X = PATH_CENTER_X 
                            + int( sin( ANG )
                                    * PATH_RADIUS_X + 0.5f );
            
            const int Y = PATH_CENTER_Y
                            + int( cos( ANG )
                                    * PATH_RADIUS_Y + 0.5f );
            
            const int Z = MCK::DEFAULT_Z_VALUE
                            + int( cos( ANG )
                                    * PATH_RADIUS_Z + 0.5f );

            blue_circle->dest_rect.set_x( X );
            blue_circle->dest_rect.set_y( Y );

            // Change 'z' value
            try
            {
                MCK::GameEng::change_z(
                    blue_circle,
                    game_eng.get_prime_render_block(),
                    Z
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to change z, error: ")
                    + e.what() ) );
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


