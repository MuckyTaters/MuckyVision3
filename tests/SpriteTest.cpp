///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
//  SpriteTest.cpp
//
//  Shoot-em-up demo test, with audio.
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
#include "Point.h"
#include "BezierCurveCubic.h"
#include "LineSegment.h"
#include "GameEng.h"
#include "ImageMan.h"
#include "ImageText.h"
#include "GameEngAudio.h"
#include "SpriteAnimTime.h"
#include "SpriteMotionConstVel.h"
#include "SpriteCollisionBase.h"
#include "Sprite.h"
#include "QuadTree.h"
#include "CollisionNode.h"
#include "CollisionProcessing.h"
#include "Vect2D.h"
MCK::Vect2D<float> test_vect(0,1);

/////////////////////////////////////////////
// DEMO PARAMETERS
// (global for convenience, as only a short program)

// Define window size (these can be changed to any sensible value)
const int WINDOW_WIDTH_IN_PIXELS = 1280;  // 640;
const int WINDOW_HEIGHT_IN_PIXELS = 720;  // 360;

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
const float LINE_SEG_DISTANCE_STEP = 10.0f;
const int ALIEN_RAW_PIXEL_WIDTH = 8;
const int ALIEN_RAW_PIXEL_HEIGHT = 8;
const int ALIEN_SCALE = 2;
const float ALIEN_ASPECT_RATIO = 2.0f;
const int ALIEN_PIXEL_WIDTH
    = int(
        float( ALIEN_RAW_PIXEL_WIDTH * ALIEN_SCALE )
            * ALIEN_ASPECT_RATIO
    );
const int ALIEN_PIXEL_HEIGHT = ALIEN_RAW_PIXEL_HEIGHT * ALIEN_SCALE;
const int QUAD_TREE_LEVELS = 3;
const int CIRCLE_RAW_PIXEL_SIZE = 16;
const int CIRCLE_SCALE = 1;
const int CIRCLE_PIXEL_SIZE
    = int(
        float( CIRCLE_RAW_PIXEL_SIZE * CIRCLE_SCALE )
    );
const int CIRCLE_SPEED = 1;
const int NUM_CIRCLES = 512;
const int ALIEN_ROWS = 4;
const int ALIEN_COLS = 6;
const int ALIEN_V_SPACE = 4;
const int ALIEN_H_SPACE = 12;
const int ALIEN_BASE_Y = 100;
const float ALIEN_FORMATION_SPEED = 64.0f / 1000.0f;  // Pixels per tick
const int ALIEN_FORMATION_PIXEL_WIDTH 
            = ALIEN_COLS * ALIEN_PIXEL_WIDTH
                + ( ALIEN_COLS - 1 ) * ALIEN_H_SPACE;
const int ALIEN_FORMATION_X_SPAN
            = X_SPAN - ALIEN_FORMATION_PIXEL_WIDTH;
const int PATH_SCALE = WINDOW_WIDTH_IN_PIXELS / 40;

const uint32_t BEEP_INTERVAL = 800;

const int STAR_1_WIDTH = 4;
const int STAR_1_HEIGHT = 2;
const int STAR_2_WIDTH = 4;
const int STAR_2_HEIGHT = 2;
const size_t STARFIELD_1_SIZE
    = WINDOW_WIDTH_IN_PIXELS / STAR_1_WIDTH / 8
        * WINDOW_HEIGHT_IN_PIXELS / STAR_1_HEIGHT;
const size_t STARFIELD_2_SIZE
    = WINDOW_WIDTH_IN_PIXELS / STAR_2_WIDTH / 8
        * WINDOW_HEIGHT_IN_PIXELS / STAR_2_HEIGHT;
const int STARFIELD_1_PIXEL_WIDTH 
    = WINDOW_WIDTH_IN_PIXELS / STAR_1_WIDTH;
const int STARFIELD_1_PIXEL_HEIGHT 
    = WINDOW_HEIGHT_IN_PIXELS / STAR_1_HEIGHT;
const int STARFIELD_2_PIXEL_WIDTH 
    = WINDOW_WIDTH_IN_PIXELS / STAR_2_WIDTH;
const int STARFIELD_2_PIXEL_HEIGHT 
    = WINDOW_HEIGHT_IN_PIXELS / STAR_2_HEIGHT;
const float STARFIELD_1_SPEED = 32.0f / 1000.0f;  // Pixels per tick
const float STARFIELD_2_SPEED = 24.0f / 1000.0f;  // Pixels per tick

// Waveform shortcuts
uint32_t SIN = uint32_t( MCK::VoiceSynth::SINE );
uint32_t SQU = uint32_t( MCK::VoiceSynth::SQUARE );
uint32_t TRI = uint32_t( MCK::VoiceSynth::TRIANGLE );
uint32_t SAW = uint32_t( MCK::VoiceSynth::SAWTOOTH );
uint32_t WHT = uint32_t( MCK::VoiceSynth::WHITENOISE );

////////////////////////////////////////////
// VOICE DATA
//  DUR,  WAVE, OCT, ATK,   DEC,   REL,  SUS_PROP, VOL
const std::vector<uint32_t> VOICE_DATA = {
    2205, TRI,  2,   550,   550,   8820,   192,    255,  // 0
    2205, TRI,  2,   550,   550,   8820,   192,    255,  // 1
    2205, TRI,  2,   550,   550,   8820,   192,    255,  // 2
    2205, SQU,  3,   550,   550,   8820,   192,     64,  // 3
    2205, SQU,  3,   550,   550,   8820,   192,     64,  // 4
    2205, SQU,  3,   550,   550,   8820,   192,     64,  // 5
    2205, TRI,  2,   550,   550,   8820,   192,     32,  // 6
    2205, TRI,  2,   550,   550,   8820,   192,     32   // 7
};
const int VOICE_DATA_COLS = 8;

/////////////////////////////////////////////
// ALIEN PATH DATA
//  X   Y  ( units = window width in pixels / 40 )
//  Note: p0 to p3 are control points for a cubic Bezier curve
const std::vector<std::vector<int16_t>> PATH_DATA = {
{
    // Entry from top-left
    0,  0,  // p0
    2,  4,  // p1
    5,  15, // p2
    20, 15, // p3/p0
    30, 15, // p1
    38, 15, // p2
    38, 10  // p3/p0
},
{
    // Entry from top-right
    40, 0,  // p0
    38, 4,  // p1
    35, 15, // p2
    20, 15, // p3/p0
    10, 15, // p1
    2,  15, // p2
    2,  10  // p3/p0
}
};

// Method to reverse sprite motion when touch edge of window
void window_edge_bounce( std::shared_ptr<MCK::SpriteMotionConstVel> sprite )
{
    const MCK::Point<float> &POS = sprite->get_pos();
   
    // Vertical bounce
    if( POS.get_y() < 0.0f
            || sprite->get_render_instance()->get_bottom()
                > WINDOW_HEIGHT_IN_PIXELS
    )
    {
        const MCK::Point<float> &VEL = sprite->get_vel();
        const MCK::Point<float> NEW_VEL(
            VEL.get_x(),
            VEL.get_y() * -1.0f
        );
        sprite->set_vel( NEW_VEL );
    }
    
    // Horizontal bounce
    if( POS.get_x() < 0.0f
        || sprite->get_render_instance()->get_right()
                > WINDOW_WIDTH_IN_PIXELS
    )
    {
        const MCK::Point<float> &VEL = sprite->get_vel();
        const MCK::Point<float> NEW_VEL(
            VEL.get_x() * -1.0f,
            VEL.get_y()
        );
        sprite->set_vel( NEW_VEL );
    }
}


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
            "SpriteTest",  // Window name
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
    // DEFINE AUDIO VOICES
    // IMPORTANT: Once these voices are handed over
    // to GameEngAudio, they should NOT be accessed
    // again by the main thread (i.e. this program),
    // as they will be accessed regularly by the
    // audio callback thread and they are NOT thread safe.
    std::vector<std::shared_ptr<MCK::VoiceBase>> new_voices( MCK_NUM_VOICES, NULL ); 
    for( int i = 0; i < MCK_NUM_VOICES; i++ )
    {
        const int ROW_BASE = VOICE_DATA_COLS * i; 
        const uint32_t SIXTEENTH_DURATION = VOICE_DATA[ ROW_BASE ];
        const MCK::VoiceSynth::Waveform WAVE 
            = MCK::VoiceSynth::Waveform( VOICE_DATA[ ROW_BASE + 1 ] );
        const uint8_t LOWEST_OCTAVE = VOICE_DATA[ ROW_BASE + 2 ];
        const uint32_t ATTACK = VOICE_DATA[ ROW_BASE + 3 ];
        const uint32_t DECAY = VOICE_DATA[ ROW_BASE + 4 ];
        const uint32_t RELEASE = VOICE_DATA[ ROW_BASE + 5 ];
        const uint8_t SUST_PROP = VOICE_DATA[ ROW_BASE + 6 ];
        const uint8_t VOL = VOICE_DATA[ ROW_BASE + 7 ];

        std::shared_ptr<MCK::VoiceSynth> synth
            = std::make_shared<MCK::VoiceSynth>();
        try
        {
            synth->init(
                SIXTEENTH_DURATION,
                WAVE,
                LOWEST_OCTAVE,
                MCK::Envelope(
                    ATTACK,
                    DECAY,
                    RELEASE,
                    SUST_PROP
                ),
                VOL
            );
        }
        catch( const std::exception &e )
        {
#if defined MCK_STD_OUT
            // Issue a warning, but no point throwing exception here
            std::cout << "Unable to init voice "
                      << std::to_string( i )
                      << ", error = " 
                      << e.what()
                      << std::endl;
#endif
            continue;
        }

        // Recast as base pointer, and store
        new_voices[i] = std::dynamic_pointer_cast<MCK::VoiceBase>( synth );
        
        std::cout << "Successfully created voice " << i
                  << std::endl;
    }

    // Group similar voices together.
    // This allows similar FX to be played 
    // concurrently using different voices,
    // avoiding the 'popping' sound that results
    // from FX being cut-off prematurely when new
    // FX are played
    const std::vector<uint8_t> FX_0_VOICES = { 0, 1, 2 };
    const std::vector<uint8_t> FX_1_VOICES = { 3, 4, 5 };
    const std::vector<uint8_t> FX_2_VOICES = { 6, 7 };

    // These indices point to elements of the above vectors,
    // and MUST never exceed the length of thier associated
    // vector. They are not const as they are incremented
    // each time an FX of that type is played.
    uint8_t fx_0_voice_index = 0;
    uint8_t fx_1_voice_index = 0;
    uint8_t fx_2_voice_index = 0;


    //////////////////////////////////////////////
    // INITIALIZE SDL AUDIO
    try
    {
        MCK::GameEngAudio::init(
            0xFF,  // Master volume
            new_voices
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to initialize SDL Audio, error: ")
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
    MCK_PAL_ID_TYPE starfield_1_palette_id;
    MCK_PAL_ID_TYPE starfield_2_palette_id;
    MCK_PAL_ID_TYPE alien_1_palette_id;
    MCK_PAL_ID_TYPE alien_2_palette_id;
    MCK_PAL_ID_TYPE alien_3_palette_id;
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
        starfield_1_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_MID_GRAY,
                }
            )
        );
        starfield_2_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_DARK_GRAY,
                }
            )
        );
        alien_1_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_GREEN
                }
            )
        );
        alien_2_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_PASTEL_BLUE,
                }
            )
        );
        alien_3_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_PURPLE,
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
   
    // Starfield 1
    std::vector<uint8_t> starfield_1(
        STARFIELD_1_SIZE,
        0x00000000
    );
    for( int i = 0; i < 40; i++ )
    {
        // Set random pixel to '1'
        starfield_1.at( rand() % STARFIELD_1_SIZE )
            |= ( 1 << rand() % 8 ); 
    }

    // Starfield 2
    std::vector<uint8_t> starfield_2(
        STARFIELD_2_SIZE,
        0x00000000
    );
    for( int i = 0; i < 40; i++ )
    {
        // Set random pixel to '1'
        starfield_2.at( rand() % STARFIELD_2_SIZE )
            |= ( 1 << rand() % 8 ); 
    }

    // Circle
    const MCK_IMG_ID_TYPE CIRCLE_IMAGE_ID = 2;
    const std::vector<uint8_t> IMAGE_CIRCLE =
    {
        0b00000111, 0b11100000,
        0b00011111, 0b11111000, 
        0b00111100, 0b00111100, 
        0b01110000, 0b00001110, 
        0b11100000, 0b00000111, 
        0b11100000, 0b00000111, 
        0b11000000, 0b00000011,
        0b11000000, 0b00000011,
        0b11000000, 0b00000011,
        0b11000000, 0b00000011, 
        0b11100000, 0b00000011, 
        0b11100000, 0b00000011, 
        0b01110000, 0b00001110, 
        0b00111100, 0b00111100, 
        0b00011111, 0b11111000,
        0b00000111, 0b11100000
    };

    ////////////////////////////////////////////
    // CREATE IMAGES
    
    MCK_IMG_ID_TYPE starfield_1_image_id;
    try
    {
        starfield_1_image_id = image_man.create_custom_image(
            std::make_shared<const std::vector<uint8_t>>(
                starfield_1
            ),
            1,  // bits_per_pixel,
            STARFIELD_1_PIXEL_WIDTH,
            STARFIELD_1_PIXEL_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create starfield 1 image, error: ")
            + e.what() ) );
    }
    
    MCK_IMG_ID_TYPE starfield_2_image_id;
    try
    {
        starfield_2_image_id = image_man.create_custom_image(
            std::make_shared<const std::vector<uint8_t>>(
                starfield_2
            ),
            1,  // bits_per_pixel,
            STARFIELD_2_PIXEL_WIDTH,
            STARFIELD_2_PIXEL_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create starfield 2 image, error: ")
            + e.what() ) );
    }

    MCK_IMG_ID_TYPE circle_image_id;
    try
    {
        circle_image_id = image_man.create_custom_image(
            std::make_shared<const std::vector<uint8_t>>(
                IMAGE_CIRCLE
            ),
            1,  // bits_per_pixel,
            CIRCLE_RAW_PIXEL_SIZE,
            CIRCLE_RAW_PIXEL_SIZE
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create circle image, error: ")
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
    std::shared_ptr<MCK::GameEngRenderBlock> starfield_1_block;
    std::shared_ptr<MCK::GameEngRenderBlock> starfield_2_block;
    std::shared_ptr<MCK::GameEngRenderBlock> sprite_block;
    try
    {
        starfield_1_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block(),
            MCK::DEFAULT_Z_VALUE - 1
        );
        starfield_2_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block(),
            MCK::DEFAULT_Z_VALUE - 2
        );
        sprite_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block(),
            MCK::DEFAULT_Z_VALUE
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create render block(s), error: ")
            + e.what() ) );
    }


    /////////////////////////////////////////////
    // CREATE COLLISION PROCESSING
    
    MCK::CollisionProcessing<float,MCK::CollisionNode> coll_proc;
    coll_proc.init(
        QUAD_TREE_LEVELS,
        MCK::Point<float>( 0, 0 ),
        MCK::Point<float>(
            WINDOW_WIDTH_IN_PIXELS,
            WINDOW_HEIGHT_IN_PIXELS
        )
    );

    // Declare collision vector
    std::vector<MCK::CollisionEvent> collisions;
    collisions.reserve( NUM_CIRCLES * NUM_CIRCLES );

    
    /////////////////////////////////////////////
    // CREATE STARFIELDS
    {
        // Starfield 1
        try
        {
            image_man.create_render_info(
                starfield_1_image_id,
                starfield_1_palette_id,
                0,
                -1 * WINDOW_HEIGHT_IN_PIXELS,
                WINDOW_WIDTH_IN_PIXELS,
                WINDOW_HEIGHT_IN_PIXELS,
                starfield_1_block
            );
            image_man.create_render_info(
                starfield_1_image_id,
                starfield_1_palette_id,
                0,
                0,
                WINDOW_WIDTH_IN_PIXELS,
                WINDOW_HEIGHT_IN_PIXELS,
                starfield_1_block
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create starfield 1 " )
                + std::string( "render info, error: " )
                + e.what() ) );
        }
        
        // Starfield 2
        try
        {
            image_man.create_render_info(
                starfield_2_image_id,
                starfield_2_palette_id,
                0,
                -1 * WINDOW_HEIGHT_IN_PIXELS,
                WINDOW_WIDTH_IN_PIXELS,
                WINDOW_HEIGHT_IN_PIXELS,
                starfield_2_block
            );
            image_man.create_render_info(
                starfield_2_image_id,
                starfield_2_palette_id,
                0,
                0,
                WINDOW_WIDTH_IN_PIXELS,
                WINDOW_HEIGHT_IN_PIXELS,
                starfield_2_block
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create starfield 2 " )
                + std::string( "render info, error: " )
                + e.what() ) );
        }
    }

    /////////////////////////////////////////////
    // ADD TITLE
    MCK::ImageText title_text;
    {
        std::string copyright( 1, uint8_t( 255  ));
        std::string s = "MUCKYVISION " + copyright + " 2023";

        try
        {
            title_text.init(
                game_eng,
                image_man,
                game_eng.get_prime_render_block(),
                title_palette_id,
                ( WINDOW_WIDTH_IN_PIXELS
                    - s.length() * CHAR_WIDTH
                        - ( s.length() - 1 ) * CHAR_SPACING
                ) / 2,
                ( WINDOW_HEIGHT_IN_PIXELS - CHAR_HEIGHT ) / 2,
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
                std::string( "Failed to create title text, error :" )
                + e.what() ) );
        }
    }
    
    /////////////////////////////////////////////
    // ADD INFO TEXT
    MCK::ImageText info_text_1;
    {
        std::string s = "Sprite motion demo for the MuckyVision3 game engine";
        try
        {
            info_text_1.init(
                game_eng,
                image_man,
                game_eng.get_prime_render_block(),
                title_palette_id,
                ( WINDOW_WIDTH_IN_PIXELS
                    - s.length() * CHAR_WIDTH / 2
                        - ( s.length() - 1 ) * CHAR_SPACING
                ) / 2,
                ( WINDOW_HEIGHT_IN_PIXELS - CHAR_HEIGHT ) / 2
                    + CHAR_HEIGHT * 3,
                s.length(),
                CHAR_WIDTH / 2,
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
                std::string( "Failed to create info text, error :" )
                + e.what() ) );
        }

        info_text_1.make_inactive();
    }

    MCK::ImageText info_text_2;
    {
        std::string s = "Code here: github.com/MuckyTaters/MuckyVision3";
        try
        {
            info_text_2.init(
                game_eng,
                image_man,
                game_eng.get_prime_render_block(),
                title_palette_id,
                ( WINDOW_WIDTH_IN_PIXELS
                    - s.length() * CHAR_WIDTH / 2
                        - ( s.length() - 1 ) * CHAR_SPACING
                ) / 2,
                ( WINDOW_HEIGHT_IN_PIXELS - CHAR_HEIGHT ) / 2
                    + CHAR_HEIGHT * 5,
                s.length(),
                CHAR_WIDTH / 2,
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
                std::string( "Failed to create info text, error :" )
                + e.what() ) );
        }

        info_text_2.make_inactive();
    }


    /////////////////////////////////////////////
    // CREATE CIRCLES WITH COLLISION DETECTION

    // Store game engine and image manager pointers
    // statically in MCK::SpritePos
    MCK::SpritePos::set_ptrs( &game_eng, &image_man );

    std::vector<
        std::shared_ptr<
            MCK::Sprite<
                MCK::SpriteMotionConstVel,
                MCK::SpriteAnimTime,
                MCK::SpriteCollisionCircle
            >
        >
    > test_sprites;
    test_sprites.reserve( NUM_CIRCLES );

    for( int i = 0; i < NUM_CIRCLES; i++ )
    {
        const int DIAM = CIRCLE_PIXEL_SIZE * ( ( rand() % 2 ) + 1 ); 

        test_sprites.push_back(
            std::make_shared<
                MCK::Sprite<
                    MCK::SpriteMotionConstVel,
                    MCK::SpriteAnimTime,
                    MCK::SpriteCollisionCircle
                >
            >(
            )
        );
        try
        {
            test_sprites.back()->init(
                image_man,
                sprite_block,
                circle_image_id,
                alien_1_palette_id,
                DIAM * 2.5f  // x coord
                    + rand() % ( WINDOW_WIDTH_IN_PIXELS - 5 * DIAM ),
                DIAM * 2.5f  // y coord
                    + rand() % ( WINDOW_HEIGHT_IN_PIXELS - 5 * DIAM ),
                MCK::MAX_Z_VALUE,
                DIAM,  // width_in_pixels,
                DIAM  // height_in_pixels,
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create circle sprite, error :" )
                + e.what() ) );
        }

        // Initialize motion for const velocity test sprite 
        test_sprites.back()->MCK::SpriteMotionConstVel::set_vel(
            MCK::Point<float>(
                ( ( rand() % 10 ) - 5 ) * 0.01f * CIRCLE_SPEED,
                ( ( rand() % 10 ) - 5 ) * 0.01f * CIRCLE_SPEED
            )
        );

        // Initialize circular collision by setting radius 
        test_sprites.back()->MCK::SpriteCollisionCircle::set_vals(
            DIAM / 2.0f,  // Radius
            DIAM / 2.0f,  // x-offset
            DIAM / 2.0f  // y-offset
        );

        // Add test_sprite to collision processing
        try
        {
            MCK::Point<float> pos 
                = test_sprites.back()->MCK::SpritePos::get_pos();
            bool rc = coll_proc.add_sprite(
                std::dynamic_pointer_cast<MCK::SpriteCollisionRect>( test_sprites.back() )
            );
            std::cout << "rc = " << rc << std::endl;
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to add test_sprite to coll_proc, error :" )
                + e.what() ) );
        }

        // Initialize animation for test sprite
        std::dynamic_pointer_cast<MCK::SpriteAnimTime>( test_sprites.back() )
            ->set_frames(
            {
                MCK::SpriteFrame(
                    10000,
                    circle_image_id,
                    alien_2_palette_id
                ),
                MCK::SpriteFrame(
                    10000,
                    circle_image_id,
                    alien_3_palette_id
                )
            }
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
    uint32_t next_beep_ticks = START_TICKS + BEEP_INTERVAL;
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
        // Process sprites
        
        for( auto &sprite : test_sprites )
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

            // For demo purposes, bounce sprite off edge
            // of window
            window_edge_bounce(
                std::dynamic_pointer_cast<
                    MCK::SpriteMotionConstVel
                >( sprite )
            );

            // Update sprite's position in collision detection
            // system
            try
            {
                coll_proc.update_sprite_pos( sprite );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to update sprite pos, error :" )
                    + e.what() ) );
            }
        }
        

        ////////////////////////////////////////
        // Detect collisions
        
        collisions.clear();
        uint32_t t1 = game_eng.get_ticks();
        coll_proc.process( collisions );
        uint32_t t2 = game_eng.get_ticks();
        std::cout << "Collision processing: " << t2 - t1 
                  << " ticks." << std::endl;


        ////////////////////////////////////////
        // Action collisions
        {
            int count = 0;
            for( MCK::CollisionEvent coll : collisions )
            {
                // Ignore NULL pointers
                if( coll.sprite_A.get() == NULL
                    || coll.sprite_B.get() == NULL
                )
                {
                    continue;
                }

                // Get sprite positions
                const MCK::Point<float> &POS_A
                    = coll.sprite_A->get_pos();
                const MCK::Point<float> &POS_B
                    = coll.sprite_B->get_pos();

                // Get sprite collision types
                const MCK::SpriteCollisionType COLL_TYPE_A
                    = coll.sprite_A->get_collision_type(); 
                const MCK::SpriteCollisionType COLL_TYPE_B
                    = coll.sprite_B->get_collision_type(); 

                // Check if A and B are circles
                if( COLL_TYPE_A == MCK::SpriteCollisionType::CIRCLE
                    && COLL_TYPE_B == MCK::SpriteCollisionType::CIRCLE
                )
                {
                    std::shared_ptr<MCK::SpriteCollisionCircle> COLL_CIRC_A
                        = std::dynamic_pointer_cast<MCK::SpriteCollisionCircle>( coll.sprite_A );

                    std::shared_ptr<MCK::SpriteCollisionCircle> COLL_CIRC_B
                        = std::dynamic_pointer_cast<MCK::SpriteCollisionCircle>( coll.sprite_B );

                    const MCK::Vect2D<float> CENTER_A(
                            COLL_CIRC_A->get_center_x(),
                            COLL_CIRC_A->get_center_y()
                        );

                    const MCK::Vect2D<float> CENTER_B(
                            COLL_CIRC_B->get_center_x(),
                            COLL_CIRC_B->get_center_y()
                        );

                    MCK::SpriteMotionConstVel::elastic_collision(
                        std::dynamic_pointer_cast<MCK::SpriteMotionConstVel>( coll.sprite_A ),
                        std::dynamic_pointer_cast<MCK::SpriteMotionConstVel>( coll.sprite_B ),
                        COLL_CIRC_A->get_radius() * 1.0f,  // mass A
                        COLL_CIRC_B->get_radius() * 1.0f,  // mass B
                        COLL_CIRC_A->get_radius(),
                        COLL_CIRC_B->get_radius(),
                        &CENTER_A,
                        &CENTER_B
                    );
                }
            }
        }

        ////////////////////////////////////////
        // BEEP
        if( CURRENT_TICKS > next_beep_ticks )
        {
            next_beep_ticks += BEEP_INTERVAL;

            // Play sound effect on appearance
            try
            {
                MCK::GameEngAudio::voice_command(
                    FX_2_VOICES.at( fx_2_voice_index ),
                    MCK::VoiceSynth::construct_command(
                        0x1F + fx_2_voice_index * 0x02, // Pitch ID
                        0x02  // Duration ID
                    )
                );
            }
            catch( std::exception &e )
            {
                std::cout << "(3)Failed to issue voice "
                          << "command, error = "
                          << e.what();
            }

            // Update FX 2 index
            fx_2_voice_index =
                ( fx_2_voice_index + 1 )
                    % FX_2_VOICES.size(); 
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

        //////////////////////////////////////////////
        // ANIMATION

        const uint32_t TICKS_SINCE_LAST_ANIM
            = CURRENT_TICKS - ticks_at_last_animation;
        ticks_at_last_animation = CURRENT_TICKS;
        if( TICKS_SINCE_LAST_ANIM > 0 )
        {
            /////////////////////////
            // Move starfields
            { 
                // Get total pixels moved by starfield 1 since
                // start
                const uint32_t RAW_Y_POS 
                    = uint32_t( 
                        float( CURRENT_TICKS - START_TICKS )
                            * STARFIELD_1_SPEED
                    );

                // Apply this to starfield block using modulo
                // to create wrap around effect.
                starfield_1_block->vert_offset
                    = RAW_Y_POS % WINDOW_HEIGHT_IN_PIXELS; 
            }
            { 
                // Get total pixels moved by starfield 2 since
                // start
                const uint32_t RAW_Y_POS 
                    = uint32_t( 
                        float( CURRENT_TICKS - START_TICKS )
                            * STARFIELD_2_SPEED
                    );

                // Apply this to starfield block using modulo
                // to create wrap around effect.
                starfield_2_block->vert_offset
                    = RAW_Y_POS % WINDOW_HEIGHT_IN_PIXELS; 
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
