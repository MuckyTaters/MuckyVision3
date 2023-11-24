///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
//  SpriteTestWalk.cpp
//
//  Walking sprite demo.
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

#include <iostream>
#include "GameEng.h"
#include "ImageMan.h"
#include "ImageText.h"
#include "SpriteAnimWalk.h"
#include "SpriteMotionConstVel.h"
#include "Sprite.h"


/////////////////////////////////////////////
// DEMO PARAMETERS
// (global for convenience, as only a short program)

// Define window size (these can be changed to any sensible value)
const int WINDOW_WIDTH_IN_PIXELS = 640;  //  1280;
const int WINDOW_HEIGHT_IN_PIXELS = 360;  // 720;

// Demo specific parameters
const int CHAR_WIDTH = 16;
const int CHAR_HEIGHT = 16;
const int CHAR_SPACING = 2;
const int BORDER_X = 20;
const float X_MIN = float( BORDER_X );
const float X_MAX = float( WINDOW_WIDTH_IN_PIXELS - BORDER_X );
const int X_SPAN = int( X_MAX - X_MIN );
const int BORDER_Y = 20;
const float Y_MIN = float( BORDER_Y );
const float Y_MAX = float( WINDOW_HEIGHT_IN_PIXELS - BORDER_Y );
const int Y_SPAN = int( Y_MAX - Y_MIN );
const float Z_MIN = 0.0f;
const float Z_MAX = 100.0f;
const int Z_SPAN = int( Z_MAX - Z_MIN );
const float X_MID = X_MIN + X_SPAN / 2.0f;
const float Y_MID = Y_MIN + Y_SPAN / 2.0f;
const float XY_MIN_SCALE = 1.0f; 
const float XY_SCALE_PER_Z = ( 1.0f - XY_MIN_SCALE ) / float( Z_SPAN );
const int NUM_CHAR_LINES = WINDOW_HEIGHT_IN_PIXELS / CHAR_HEIGHT;
const int CHAR_LINE_LEN = WINDOW_WIDTH_IN_PIXELS / CHAR_WIDTH;

const int WALKING_SPRITE_SCALE = 3;
const int NUM_WALKING_SPRITES = 6;
const int FRAME_WIDTH = 8;
const int FRAME_HEIGHT = 16;
const float WALKING_SPEED = 0.08f;

const int CORRIDOR_HEIGHT_IN_CHARS = 5;
const int CORRIDOR_HEIGHT = CHAR_HEIGHT * CORRIDOR_HEIGHT_IN_CHARS;
const int Y_START = CORRIDOR_HEIGHT - WALKING_SPRITE_SCALE * FRAME_HEIGHT;


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//   MM MM    AA    III   NN  N
//   M M M   A  A    I    N N N  
//   M   M   AAAA    I    N NNN
//   M   M   A  A   III   N   N
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
// TOP LEVEL ENTRY POINT OF THE TEST APPLICATION
int main( int argc, char** argv )
{
    //////////////////////////////////////////////
    // INITIALIZE SDL, CREATE WINDOW & RENDERER
    MCK::GameEng &game_eng = MCK::GameEng::get_singleton();
    try
    {
        game_eng.init(
            "Sprite Running Test",  // Window name
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
    // CREATE LOCAL PALETTE(S)
    MCK_PAL_ID_TYPE title_palette_id;
    MCK_PAL_ID_TYPE purple_palette_id;
    MCK_PAL_ID_TYPE cyan_palette_id;
    MCK_PAL_ID_TYPE green_palette_id;
    MCK_PAL_ID_TYPE blue_palette_id;
    MCK_PAL_ID_TYPE orange_palette_id;
    MCK_PAL_ID_TYPE red_palette_id;
    MCK_PAL_ID_TYPE brick_palette_id;
    MCK_PAL_ID_TYPE bg_palette_id;
    try
    {
        title_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_BLACK,
                    MCK::COL_YELLOW,
                }
            )
        );
        purple_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_PURPLE
                }
            )
        );
        cyan_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_CYAN
                }
            )
        );
        green_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_GREEN
                }
            )
        );
        blue_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_BLUE
                }
            )
        );
        orange_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_ORANGE
                }
            )
        );
        red_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_RED
                }
            )
        );
        brick_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_DARK_RED
                }
            )
        );
        bg_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_MID_GRAY
                }
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create palette, error: ")
            + e.what() ) );
    }

    
    ///////////////////////////////////////////
    // CREATE IMAGE DATA

    // Frame 0
    const std::vector<uint8_t> FRAME_0_IMAGE =
    {
        0b00001100,  // 0
        0b00001100,  // 1
        0b00011000,  // 2
        0b00011000,  // 3
        0b00111000,  // 4
        0b00011110,  // 5
        0b00011000,  // 6
        0b00011000,  // 7
        0b11011100,  // 8
        0b10111100,  // 9
        0b00011100,  // 10
        0b00000100,  // 11
        0b00000100,  // 12
        0b00001100,  // 13
        0b00001000,  // 14
        0b00001100   // 15
    };

    // Frame 1
    const std::vector<uint8_t> FRAME_1_IMAGE =
    {
        0b00001100,  // 0
        0b00001100,  // 1
        0b00011000,  // 2
        0b00011000,  // 3
        0b00011000,  // 4
        0b00011110,  // 5
        0b00011000,  // 6
        0b00011000,  // 7
        0b00011000,  // 8
        0b11111000,  // 9
        0b10011000,  // 10
        0b00001000,  // 10
        0b00011000,  // 12
        0b00010000,  // 13
        0b00010000,  // 14
        0b00011000   // 15
    };
    
    // Frame 2
    const std::vector<uint8_t> FRAME_2_IMAGE =
    {
        0b00011000,  // 0
        0b00011000,  // 1
        0b00110000,  // 2
        0b00110000,  // 3
        0b00110000,  // 4
        0b00111000,  // 5
        0b00110000,  // 6
        0b00111000,  // 7
        0b00111000,  // 8
        0b00111000,  // 9
        0b11111100,  // 10
        0b10100000,  // 10
        0b01000000,  // 12
        0b01000000,  // 13
        0b10000000,  // 14
        0b11000000   // 15
    };
    
    // Frame 3
    const std::vector<uint8_t> FRAME_3_IMAGE =
    {
        0b00011000,  // 0
        0b00011000,  // 1
        0b00110000,  // 2
        0b01110010,  // 3
        0b01111100,  // 4
        0b00110000,  // 5
        0b00110000,  // 6
        0b00111000,  // 7
        0b00111100,  // 8
        0b00101110,  // 9
        0b00111000,  // 10
        0b01100000,  // 10
        0b01100000,  // 12
        0b10000000,  // 13
        0b10000000,  // 14
        0b11000000   // 15
    };
    
    // Frame 4
    const std::vector<uint8_t> FRAME_4_IMAGE =
    {
        0b00001100,  // 0
        0b00001100,  // 1
        0b00001001,  // 2
        0b00011101,  // 3
        0b00111110,  // 4
        0b00111000,  // 5
        0b00011000,  // 6
        0b00011100,  // 7
        0b00011111,  // 8
        0b00011011,  // 9
        0b00110010,  // 10
        0b00110100,  // 11
        0b00101100,  // 12
        0b01000000,  // 13
        0b10000000,  // 14
        0b11000000,  // 15
    };
    
    // Frame 5
    const std::vector<uint8_t> FRAME_5_IMAGE =
    {
        0b00000110, 0b00000000,  // 0
        0b00000110, 0b00000000,  // 1
        0b00000100, 0b10000000,  // 2
        0b00011110, 0b10000000,  // 3
        0b00011101, 0b00000000,  // 4
        0b00011100, 0b00000000,  // 5
        0b00011100, 0b00000000,  // 6
        0b00001110, 0b00000000,  // 7
        0b00001111, 0b10000000,  // 8
        0b00001100, 0b10000000,  // 9
        0b00011000, 0b10000000,  // 10
        0b00011000, 0b10000000,  // 10
        0b00110000, 0b10000000,  // 12
        0b01000000, 0b11000000,  // 13
        0b10000000, 0b00000000,  // 14
        0b11000000, 0b00000000   // 15
    };
    
    // Frame 6
    const std::vector<uint8_t> FRAME_6_IMAGE =
    {
        0b00000011, 0b00000000,  // 0
        0b00000011, 0b00000000,  // 1
        0b00000010, 0b01000000,  // 2
        0b00001111, 0b01000000,  // 3
        0b00010110, 0b10000000,  // 4
        0b00010110, 0b00000000,  // 5
        0b00010110, 0b00000000,  // 6
        0b00010110, 0b00000000,  // 7
        0b00000111, 0b10000000,  // 8
        0b00001111, 0b11000000,  // 9
        0b00001100, 0b01100000,  // 10
        0b00011000, 0b00100000,  // 10
        0b11100000, 0b00100000,  // 12
        0b10000000, 0b00110000,  // 13
        0b10000000, 0b00000000,  // 14
        0b00000000, 0b00000000   // 15
    };
    
    // Frame 7
    const std::vector<uint8_t> FRAME_7_IMAGE =
    {
        0b00000001, 0b10000000,  // 0
        0b00000001, 0b10000000,  // 1
        0b00000001, 0b00000000,  // 2
        0b00000011, 0b10100000,  // 3
        0b00000111, 0b11000000,  // 4
        0b00000111, 0b00000000,  // 5
        0b00000111, 0b00000000,  // 6
        0b00000111, 0b00000000,  // 7
        0b00000011, 0b10000000,  // 8
        0b00000111, 0b11000000,  // 9
        0b00000110, 0b01100000,  // 10
        0b11111100, 0b00110000,  // 10
        0b10000000, 0b00010000,  // 12
        0b00000000, 0b00010000,  // 13
        0b00000000, 0b00011000,  // 14
        0b00000000, 0b00000000   // 15
    };
    
    // Frame 8
    const std::vector<uint8_t> FRAME_8_IMAGE =
    {
        0b00000001, 0b10000000,  // 0
        0b00000001, 0b10000000,  // 1
        0b00000011, 0b00000000,  // 2
        0b00000111, 0b10100000,  // 3
        0b00000111, 0b11000000,  // 4
        0b00000011, 0b00000000,  // 5
        0b00000011, 0b00000000,  // 6
        0b00000011, 0b00000000,  // 7
        0b00000111, 0b10000000,  // 8
        0b11110111, 0b10000000,  // 9
        0b10011100, 0b11000000,  // 10
        0b00000000, 0b01000000,  // 10
        0b00000000, 0b01000000,  // 12
        0b00000000, 0b01000000,  // 13
        0b00000000, 0b01100000,  // 14
        0b00000000, 0b00000000   // 15
    };
    
    // Frame 9
    const std::vector<uint8_t> FRAME_9_IMAGE =
    {
        0b00000011, 0b00000000,  // 0
        0b00000011, 0b00000000,  // 1
        0b00000110, 0b00000000,  // 2
        0b00000110, 0b00000000,  // 3
        0b00001111, 0b01000000,  // 4
        0b00000110, 0b10000000,  // 5
        0b00000110, 0b00000000,  // 6
        0b00000110, 0b00000000,  // 7
        0b11100111, 0b00000000,  // 8
        0b10111111, 0b00000000,  // 9
        0b00001101, 0b00000000,  // 10
        0b00000001, 0b10000000,  // 10
        0b00000000, 0b10000000,  // 12
        0b00000000, 0b10000000,  // 13
        0b00000000, 0b10000000,  // 14
        0b00000000, 0b11000000   // 15
    };


    ////////////////////////////////////////////
    // CREATE IMAGES

    MCK_IMG_ID_TYPE frame_0_image_id;
    try
    {
        frame_0_image_id = image_man.create_custom_image(
            &FRAME_0_IMAGE,
            1,  // bits_per_pixel,
            FRAME_WIDTH,
            FRAME_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create frame 0 image, error: ")
            + e.what() ) );
    }

    MCK_IMG_ID_TYPE frame_1_image_id;
    try
    {
        frame_1_image_id = image_man.create_custom_image(
            &FRAME_1_IMAGE,
            1,  // bits_per_pixel,
            FRAME_WIDTH,
            FRAME_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create frame 1 image, error: ")
            + e.what() ) );
    }

    MCK_IMG_ID_TYPE frame_2_image_id;
    try
    {
        frame_2_image_id = image_man.create_custom_image(
            &FRAME_2_IMAGE,
            1,  // bits_per_pixel,
            FRAME_WIDTH,
            FRAME_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create frame 2 image, error: ")
            + e.what() ) );
    }

    MCK_IMG_ID_TYPE frame_3_image_id;
    try
    {
        frame_3_image_id = image_man.create_custom_image(
            &FRAME_3_IMAGE,
            1,  // bits_per_pixel,
            FRAME_WIDTH,
            FRAME_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create frame 3 image, error: ")
            + e.what() ) );
    }

    MCK_IMG_ID_TYPE frame_4_image_id;
    try
    {
        frame_4_image_id = image_man.create_custom_image(
            &FRAME_4_IMAGE,
            1,  // bits_per_pixel,
            FRAME_WIDTH,
            FRAME_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create frame 4 image, error: ")
            + e.what() ) );
    }

    MCK_IMG_ID_TYPE frame_5_image_id;
    try
    {
        frame_5_image_id = image_man.create_custom_image(
            &FRAME_5_IMAGE,
            1,  // bits_per_pixel,
            FRAME_WIDTH * 2,
            FRAME_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create frame 5 image, error: ")
            + e.what() ) );
    }

    MCK_IMG_ID_TYPE frame_6_image_id;
    try
    {
        frame_6_image_id = image_man.create_custom_image(
            &FRAME_6_IMAGE,
            1,  // bits_per_pixel,
            FRAME_WIDTH * 2,
            FRAME_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create frame 6 image, error: ")
            + e.what() ) );
    }

    MCK_IMG_ID_TYPE frame_7_image_id;
    try
    {
        frame_7_image_id = image_man.create_custom_image(
            &FRAME_7_IMAGE,
            1,  // bits_per_pixel,
            FRAME_WIDTH * 2,
            FRAME_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create frame 7 image, error: ")
            + e.what() ) );
    }

    MCK_IMG_ID_TYPE frame_8_image_id;
    try
    {
        frame_8_image_id = image_man.create_custom_image(
            &FRAME_8_IMAGE,
            1,  // bits_per_pixel,
            FRAME_WIDTH * 2,
            FRAME_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create frame 8 image, error: ")
            + e.what() ) );
    }

    MCK_IMG_ID_TYPE frame_9_image_id;
    try
    {
        frame_9_image_id = image_man.create_custom_image(
            &FRAME_9_IMAGE,
            1,  // bits_per_pixel,
            FRAME_WIDTH * 2,
            FRAME_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create frame 9 image, error: ")
            + e.what() ) );
    }


    ////////////////////////////////////////////
    // SET CLEARING COLO(U)R

    // Set render clearing colo(u)r for this demo
    try
    {
        game_eng.set_clearing_color( MCK::COL_BLACK );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to set clearing color, error: ")
            + e.what() ) );
    }

    ///////////////////////////////////////////
    // CREATE RENDER BLOCKS
    std::shared_ptr<MCK::GameEngRenderBlock> sprite_block;
    std::shared_ptr<MCK::GameEngRenderBlock> bg_block;
    try
    {
        sprite_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block(),
            MCK::DEFAULT_Z_VALUE
        );
        bg_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block(),
            MCK::DEFAULT_Z_VALUE + 1
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create render block(s), error: ")
            + e.what() ) );
    }


    /////////////////////////////////////////////
    // ADD BACKGROUND
    std::vector<MCK::ImageText> bg1_lines( NUM_CHAR_LINES );
    std::vector<MCK::ImageText> bg2_lines( NUM_CHAR_LINES );
    {
        std::string brick( 1, uint8_t( 246 ) );
        std::string rev_blank( 1, uint8_t( 230 ) );
        std::string bg1_wall_line;
        std::string bg1_corridor_line;
        std::string bg2_wall_line;
        std::string bg2_corridor_line;
        for( int j = 0; j < CHAR_LINE_LEN; j++ )
        {
            bg1_wall_line += rev_blank;
            bg2_wall_line += brick;
            bg1_corridor_line += ( j == 0 || j == CHAR_LINE_LEN - 1 )?
                             rev_blank : " ";
            bg2_corridor_line += ( j == 0 || j == CHAR_LINE_LEN - 1 )?
                             brick : " ";
        }
        for( int i = 0; i < NUM_CHAR_LINES; i++ )
        {
            try
            {
                bg1_lines[i].init(
                    game_eng,
                    image_man,
                    bg_block,
                    bg_palette_id,
                    0,
                    i * CHAR_HEIGHT,
                    CHAR_LINE_LEN,
                    CHAR_WIDTH,
                    CHAR_HEIGHT,
                    i % CORRIDOR_HEIGHT_IN_CHARS == 0 ?
                        bg1_wall_line : bg1_corridor_line
                );
                bg2_lines[i].init(
                    game_eng,
                    image_man,
                    bg_block,
                    brick_palette_id,
                    0,
                    i * CHAR_HEIGHT,
                    CHAR_LINE_LEN,
                    CHAR_WIDTH,
                    CHAR_HEIGHT,
                    i % CORRIDOR_HEIGHT_IN_CHARS == 0 ?
                        bg2_wall_line : bg2_corridor_line
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to create bg line, error :" )
                    + e.what() ) );
            }
        }
    }

    /////////////////////////////////////////////
    // ADD TITLE
    MCK::ImageText title_text;
    {
        std::string s = "SPRITE RUNNING DEMO";

        try
        {
            title_text.init(
                game_eng,
                image_man,
                game_eng.get_prime_render_block(),
                title_palette_id,
                ( WINDOW_WIDTH_IN_PIXELS
                    - s.length() * CHAR_WIDTH // * 1.25f
                        - ( s.length() - 1 ) * CHAR_SPACING
                ) / 2,
                CHAR_HEIGHT,
                s.length(),
                CHAR_WIDTH, // * 1.25f,
                CHAR_HEIGHT, //  * 1.5f,
                s,
                MCK::ImageText::CENTER,
                CHAR_SPACING,
                0,  // basic ascii set
                MCK::MAX_Z_VALUE
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create title text, error :" )
                + e.what() ) );
        }
    }

    MCK::ImageText copyright_text;
    {
        std::string copyright( 1, uint8_t( 255  ));
        std::string s = "MUCKYVISION " + copyright + " 2023";

        try
        {
            copyright_text.init(
                game_eng,
                image_man,
                game_eng.get_prime_render_block(),
                title_palette_id,
                ( WINDOW_WIDTH_IN_PIXELS
                    - s.length() * CHAR_WIDTH
                        - ( s.length() - 1 ) * CHAR_SPACING
                ) - CHAR_WIDTH * 2,  // x_pos,
                WINDOW_HEIGHT_IN_PIXELS - CHAR_HEIGHT * 1,  // y_pos,
                s.length(),
                CHAR_WIDTH,
                CHAR_HEIGHT,
                s,
                MCK::ImageText::CENTER,
                CHAR_SPACING,
                0,  // basic ascii set
                MCK::MAX_Z_VALUE
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create copyright text, error :" )
                + e.what() ) );
        }
    }

    ///////////////////////////////////////////////////
    // CREATE FPS READ OUT
    std::shared_ptr<MCK::ImageText> fps_text =
        std::make_shared<MCK::ImageText>();
    try
    {
        fps_text->init(
            game_eng,
            image_man,
            game_eng.get_prime_render_block(),
            title_palette_id,
            CHAR_WIDTH * 2,  // x_pos,
            WINDOW_HEIGHT_IN_PIXELS - CHAR_HEIGHT * 1,  // y_pos,
            8,  // width in chars
            CHAR_WIDTH,
            CHAR_HEIGHT,
            "0.00",
            MCK::ImageText::Just::LEFT,
            0,  // char spacing
            0,  // Default ASCII set
            MCK::MAX_Z_VALUE  // Render on top
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create FPS text, error: ")
            + e.what() ) );
    }


    /////////////////////////////////////////////////////
    // PREPARE FOR SPRITES
    
    // Store game engine and image manager pointers
    // statically in MCK::SpritePos
    MCK::SpritePos::set_ptrs( &game_eng, &image_man );


    /////////////////////////////////////////////////////
    // CREATE SPRITES
    std::vector<
        std::shared_ptr<
            MCK::Sprite<
                MCK::SpriteMotionConstVel,
                MCK::SpriteAnimWalk,
                MCK::SpriteCollisionBase
            >
        >
    > walking_sprites;
    walking_sprites.reserve( NUM_WALKING_SPRITES );

    for( int i = 0; i < NUM_WALKING_SPRITES; i++ )
    {
        const int WIDTH = FRAME_WIDTH * WALKING_SPRITE_SCALE;
        const int HEIGHT = FRAME_HEIGHT * WALKING_SPRITE_SCALE;
        walking_sprites.push_back(
            std::make_shared<
                MCK::Sprite<
                    MCK::SpriteMotionConstVel,
                    MCK::SpriteAnimWalk,
                    MCK::SpriteCollisionBase
                >
            >(
            )
        );

        MCK_PAL_ID_TYPE pal_id;
        switch( i % 6 )
        {
            case 0:
                pal_id = purple_palette_id;
                break;

            case 1:
                pal_id = cyan_palette_id;
                break;

            case 2:
                pal_id = green_palette_id;
                break;
            
            case 3:
                pal_id = blue_palette_id;
                break;

            case 4:
                pal_id = orange_palette_id;
                break;

            case 5:
                pal_id = red_palette_id;
                break;
        }

        try
        {
            walking_sprites.back()->init(
                sprite_block,
                frame_0_image_id,
                pal_id,
                WIDTH * i * 2 + 50,  // x coord
                Y_START,  // y coord
                MCK::MAX_Z_VALUE,
                WIDTH,  // width_in_pixels,
                HEIGHT  // height_in_pixels,
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create walking sprite, error :" )
                + e.what() ) );
        }

        // Initialize motion for const velocity test sprite 
        walking_sprites.back()->MCK::SpriteMotionConstVel::set_vel(
            MCK::Point<float>(
                WALKING_SPEED * ( 1.0f + i * 0.3f ),
                0
            )
        );

        // Initialize animation for test sprite
        // const int FRAME_DURATION = 4 * MCK::SPRITE_DIST_GRANULARITY;
        const uint8_t FLAGS = MCK::GameEngRenderInfo::calc_flags(
            0,
            false,
            false
        );

        const int FRAME_DURATION
            = int( float( WALKING_SPRITE_SCALE ) * 1.5f + 0.5f );

        std::dynamic_pointer_cast<MCK::SpriteAnimWalk>( walking_sprites.back() )
            ->set_frames(
            {
                MCK::SpriteFrame(
                    FRAME_DURATION,
                    frame_0_image_id,
                    pal_id,
                    false,
                    true,
                    0,
                    0,
                    FLAGS
                ),
                MCK::SpriteFrame(
                    FRAME_DURATION,
                    frame_1_image_id,
                    pal_id,
                    false,
                    true,
                    0,
                    0,
                    FLAGS
                ),
                MCK::SpriteFrame(
                    FRAME_DURATION,
                    frame_2_image_id,
                    pal_id,
                    false,
                    true,
                    int( 1.5f * float( WALKING_SPRITE_SCALE ) ),
                    0,
                    FLAGS
                ),
                MCK::SpriteFrame(
                    FRAME_DURATION,
                    frame_3_image_id,
                    pal_id,
                    false,
                    true,
                    0, // int( 1.5f * float( WALKING_SPRITE_SCALE ) + 0.5f ),
                    0,
                    FLAGS
                ),
                MCK::SpriteFrame(
                    FRAME_DURATION,
                    frame_4_image_id,
                    pal_id,
                    false,
                    true,
                    int( -1.5f * float( WALKING_SPRITE_SCALE ) + 0.5f ),
                    0,
                    FLAGS
                ),
                MCK::SpriteFrame(
                    FRAME_DURATION,
                    frame_5_image_id,
                    pal_id,
                    false,
                    true,
                    int( -3.0f * float( WALKING_SPRITE_SCALE ) + 0.5f ),
                    0,
                    FLAGS
                ),
                MCK::SpriteFrame(
                    FRAME_DURATION,
                    frame_6_image_id,
                    pal_id,
                    false,
                    true,
                    int( -3.0f * float( WALKING_SPRITE_SCALE ) + 0.5f ),
                    0,
                    FLAGS
                ),
                MCK::SpriteFrame(
                    FRAME_DURATION,
                    frame_7_image_id,
                    pal_id,
                    false,
                    true,
                    int( -3.0f * float( WALKING_SPRITE_SCALE ) + 0.5f ),
                    0,
                    FLAGS
                ),
                MCK::SpriteFrame(
                    FRAME_DURATION,
                    frame_8_image_id,
                    pal_id,
                    false,
                    true,
                    int( -1.5f * float( WALKING_SPRITE_SCALE ) + 0.5f ),
                    0,
                    FLAGS
                ),
                MCK::SpriteFrame(
                    FRAME_DURATION,
                    frame_9_image_id,
                    pal_id,
                    false,
                    true,
                    int( -1.5f * float( WALKING_SPRITE_SCALE ) ),
                    0,
                    FLAGS
                )
            },
            0,  // Starting frame
            true  // Use offsets
        );
    }


    /////////////////////////////////////////////
    // MAIN LOOP STARTS HERE
    /////////////////////////////////////////////
    const float TARGET_FPS = 100;
    const uint32_t TICKS_PER_FRAME
        = uint32_t( 1000.0f / TARGET_FPS + 0.5f );
    uint32_t frame_num = 0;
    const uint32_t START_TICKS = game_eng.get_ticks();
    uint32_t next_frame_ticks = START_TICKS + TICKS_PER_FRAME; 
    uint32_t end_ticks = START_TICKS + 250000;
    uint32_t ticks_at_last_animation = START_TICKS;
    uint32_t ticks_at_last_fps_update = START_TICKS;
    do
    {
        ////////////////////////////////////////
        // Time management

        // Give time back to operating system
        game_eng.delay( TICKS_PER_FRAME / 4 );

        // Get current time
        const uint32_t CURRENT_TICKS = game_eng.get_ticks();

        // If next frame not due, ignore
        if( CURRENT_TICKS < next_frame_ticks )
        {
            continue;
        }
        else
        {
            // Calculate time of *next* frame
            next_frame_ticks = CURRENT_TICKS + TICKS_PER_FRAME;
            frame_num++;
        }
        
        // Set timing for sprites
        MCK::SpritePos::set_ticks( game_eng.get_ticks() );
   

        ////////////////////////////////////////
        // Update FPS
        if( frame_num % 50 == 0 )
        {
            const float TICKS_PASSED_F
                = CURRENT_TICKS - ticks_at_last_fps_update;
            ticks_at_last_fps_update
                = CURRENT_TICKS;

            try
            {
                fps_text->set_content(
                    std::string( "FPS " )
                    + std::to_string(
                        50.0f / TICKS_PASSED_F * 1000.0f
                    ).substr( 0, 4 )
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to set fps, error: ")
                    + e.what() ) );
            }
        }


        ////////////////////////////////////////
        // Process sprites
        for( auto &sprite : walking_sprites )
        {
            // Process sprite (move, animate etc.)
            try
            {
                sprite->process();
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to process sprite, error :" )
                    + e.what() ) );
            }

            // Reposition sprites when they reach edge of window
            if( sprite->::MCK::SpritePos::get_pos().get_x() > X_MAX )
            {
                float POS_Y = sprite->::MCK::SpritePos::get_pos().get_y();
                sprite->::MCK::SpritePos::set_pos(
                    MCK::Point<float>(
                        0,
                        POS_Y < Y_MAX - CORRIDOR_HEIGHT ? 
                            POS_Y + CORRIDOR_HEIGHT : Y_START
                    )
                );
            }
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
    while( game_eng.get_ticks() < end_ticks );
}
