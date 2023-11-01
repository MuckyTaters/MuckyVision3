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
const int QUAD_TREE_LEVELS = 9;
const int CIRCLE_RAW_PIXEL_SIZE = 16;
const int CIRCLE_SCALE = 1;
const int CIRCLE_PIXEL_SIZE
    = int(
        float( CIRCLE_RAW_PIXEL_SIZE * CIRCLE_SCALE )
    );
const int NUM_CIRCLES = 480;
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

    // Alien 1
    const std::vector<uint8_t> IMAGE_ALIEN_1 =
    {
        0b10000010,
        0b11000110,
        0b11010110,
        0b11010110,
        0b11010110,
        0b01111100,
        0b00111000,
        0b00010000
    };

    // Alien 2
    const std::vector<uint8_t> IMAGE_ALIEN_2 =
    {
        0b01000100,
        0b11010110,
        0b11111110,
        0b11111110,
        0b11010110,
        0b11010110,
        0b11010110,
        0b01000100
    };

    // Alien 3
    const std::vector<uint8_t> IMAGE_ALIEN_3 =
    {
        0b01111100,
        0b11111110,
        0b10111010,
        0b11010110,
        0b01111100,
        0b01010100,
        0b01010100,
        0b01010100
    };
   
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
    
    MCK_IMG_ID_TYPE alien_1_image_id;
    try
    {
        alien_1_image_id = image_man.create_custom_image(
            std::make_shared<const std::vector<uint8_t>>(
                IMAGE_ALIEN_1
            ),
            1,  // bits_per_pixel,
            ALIEN_RAW_PIXEL_WIDTH,
            ALIEN_RAW_PIXEL_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create alien 1 image, error: ")
            + e.what() ) );
    }
    
    MCK_IMG_ID_TYPE alien_2_image_id;
    try
    {
        alien_2_image_id = image_man.create_custom_image(
            std::make_shared<const std::vector<uint8_t>>(
                IMAGE_ALIEN_2
            ),
            1,  // bits_per_pixel,
            ALIEN_RAW_PIXEL_WIDTH,
            ALIEN_RAW_PIXEL_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create alien 2 image, error: ")
            + e.what() ) );
    }

    MCK_IMG_ID_TYPE alien_3_image_id;
    try
    {
        alien_3_image_id = image_man.create_custom_image(
            std::make_shared<const std::vector<uint8_t>>(
                IMAGE_ALIEN_3
            ),
            1,  // bits_per_pixel,
            ALIEN_RAW_PIXEL_WIDTH,
            ALIEN_RAW_PIXEL_HEIGHT
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create alien 3 image, error: ")
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
    std::shared_ptr<MCK::GameEngRenderBlock> alien_formation_block;
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
        alien_formation_block = game_eng.create_empty_render_block(
            sprite_block,
            MCK::DEFAULT_Z_VALUE - 1
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create render block(s), error: ")
            + e.what() ) );
    }
    // Set starting offset for alien formation
    alien_formation_block->hoz_offset = BORDER_X; 
    alien_formation_block->vert_offset = BORDER_Y; 


    /////////////////////////////////////////////
    // CREATE SPRITE PATHS

    // Declare vector of entry paths for aliens
    // Note: each entry is a "starting segment"
    //       that connects to further segments in turn
    const size_t NUM_PATHS = PATH_DATA.size();
    std::vector<
        std::shared_ptr<
            MCK::LineSegment<
                MCK::BezierCurveCubic,
                MCK::Point<float>
            >
        >
    > start_segs( NUM_PATHS );

    // Construct each path, segment by segment
    for( size_t n = 0; n < NUM_PATHS; n++ )
    {
        std::shared_ptr<
            MCK::LineSegment<
                MCK::BezierCurveCubic,
                MCK::Point<float>
            >
        > prev_seg;
        {
            const size_t NUM_SEGS = ( PATH_DATA[n].size() - 2  ) / 6;

            for( size_t i = 0; i < NUM_SEGS; i++ )
            {
                // Calculate starting position of the control point
                // coords within PATH_DATA
                const size_t BASE = i * 6;

                // Construct a shared pointer to a line segment,
                // based on a cubic Bezier curve using the
                // control point coords stored in PATH_DATA
                std::shared_ptr<
                    MCK::LineSegment<
                        MCK::BezierCurveCubic,
                        MCK::Point<float>
                    >
                > new_seg = std::make_shared<
                    MCK::LineSegment<
                        MCK::BezierCurveCubic,
                        MCK::Point<float>
                    >
                >( MCK::BezierCurveCubic<MCK::Point<float>>(
                        MCK::Point<float>(  // p0
                            PATH_DATA[n][ BASE + 0 ] * PATH_SCALE,
                            PATH_DATA[n][ BASE + 1 ] * PATH_SCALE,
                            sprite_block
                        ),
                        MCK::Point<float>(  // p1
                            PATH_DATA[n][ BASE + 2 ] * PATH_SCALE,
                            PATH_DATA[n][ BASE + 3 ] * PATH_SCALE,
                            sprite_block
                        ),
                        MCK::Point<float>(  // p2
                            PATH_DATA[n][ BASE + 4 ] * PATH_SCALE,
                            PATH_DATA[n][ BASE + 5 ] * PATH_SCALE,
                            sprite_block
                        ),
                        MCK::Point<float>(  // p3
                            PATH_DATA[n][ BASE + 6 ] * PATH_SCALE,
                            PATH_DATA[n][ BASE + 7 ] * PATH_SCALE,
                            sprite_block
                        )
                    )
                );

                // Initialise this line segment
                try
                {
                    new_seg->init(
                        LINE_SEG_DISTANCE_STEP,
                        true,  // xy only
                        i  // Use 'i' as segment id
                    );
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
                        std::string( "Failed to init line segment " )
                        + std::to_string( i )
                        + std::string( ", error: " )
                        + e.what() ) );
                }

                // Store pointer to first segment only
                if( i == 0 )
                {
                    start_segs[n] = new_seg;
                }
                // Otherwise, connect segment to previous segment
                else {
                    try
                    {
                        prev_seg->connect_single_segment( new_seg );
                    }
                    catch( std::exception &e )
                    {
                        throw( std::runtime_error(
                            std::string( "Failed to connect line segment " )
                            + std::to_string( i )
                            + std::string( ", error: " )
                            + e.what() ) );
                    }
                }

                // Store current segment as previous segment
                prev_seg = new_seg;            
            }
        }
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
        // MCK::Point<float>( 640, 480 )
    );

    // Declare collision vector
    std::vector<MCK::CollisionEvent> collisions;
    collisions.reserve( NUM_CIRCLES * NUM_CIRCLES );


    /////////////////////////////////////////////
    // CREATE SPRITES

    // Ad-hoc POD struct to hold sprite info
    struct BasicSprite
    {
        MCK_IMG_ID_TYPE image_id;
        MCK_PAL_ID_TYPE palette_id;

        std::shared_ptr<MCK::GameEngRenderInfo> render_info;
       
        // Line segment currently being traversed
        std::shared_ptr<
            const MCK::LineSegment<
                MCK::BezierCurveCubic,
                MCK::Point<float>
            >
        > current_seg;

        float dist;  // Distance along current line segment
        float current_speed;  // Current speed, pixels per tick
        float target_speed;  // Target speed, pixels per tick
        float acc;  // Acceleration, pixels per tick per tick
    };

    // Specialised sprite struct to hold alien sprite info
    struct AlienSprite : public BasicSprite
    {
        // Coordinates of alien within alien_formation_block
        MCK::Point<float> formation_pos;

        bool in_formation;

        int id;

        // Used only when docking with formation
        MCK::Point<float> temp_control_point;

        // Sprite remains hidden until this time
        uint32_t appearance_ticks;

        // Default constructor
        AlienSprite( void )
        {
            in_formation = true;
            id = -1;
            appearance_ticks = 0;
        }
    };

    // Create alien sprites
    std::vector<AlienSprite> aliens;
    aliens.resize( ALIEN_ROWS * ALIEN_COLS );
    for( int j = 0; j < ALIEN_ROWS; j++ )
    {
        MCK_PAL_ID_TYPE palette_id;
        MCK_IMG_ID_TYPE image_id;
        if( j > 1 )
        {
            palette_id = alien_1_palette_id; 
            image_id = alien_1_image_id;
        }
        else if( j > 0 )
        {
            palette_id = alien_2_palette_id; 
            image_id = alien_2_image_id;
        }
        else
        {
            palette_id = alien_3_palette_id; 
            image_id = alien_3_image_id;
        }

        for( int i = 0; i < ALIEN_COLS; i++ )
        {
            const int COUNT = j * ALIEN_COLS + i;

            // Get pointer to sprite
            AlienSprite* const ALIEN
                = &aliens[ COUNT ];

            ALIEN->id = COUNT;

            ALIEN->image_id = image_id;
            ALIEN->palette_id = palette_id;

            ALIEN->in_formation = false;

            // Set intial path location
            switch( j )
            {
                case ALIEN_ROWS - 1:
                    ALIEN->current_seg = start_segs[0];
                    ALIEN->appearance_ticks = 1000 + i * 200;
                    break;

                case ALIEN_ROWS - 2:
                    ALIEN->current_seg = start_segs[1];
                    ALIEN->appearance_ticks
                        = 3000 + ( ALIEN_COLS - i ) * 200;
                    break;
                
                case ALIEN_ROWS - 3:
                    ALIEN->current_seg = start_segs[0];
                    ALIEN->appearance_ticks = 5000 + i * 200;
                    break;

                case ALIEN_ROWS - 4:
                    ALIEN->current_seg = start_segs[1];
                    ALIEN->appearance_ticks
                        = 7000 + ( ALIEN_COLS - i ) * 200;
                    break;

                default:
                    ALIEN->current_seg.reset();
                    ALIEN->appearance_ticks = 0;
            }


            // Set speed and acceleration
            ALIEN->current_speed = 640.0f / 1000.0f;
            ALIEN->target_speed = 128.0f / 1000.0f;
            ALIEN->acc = -0.25f / 1000.0f;

            // Set distance along starting segment
            ALIEN->dist = 0.0f;
            
            // Set (evantual) position within formation
            ALIEN->formation_pos.set_x(
                i * ( ALIEN_PIXEL_WIDTH + ALIEN_H_SPACE )
            );
            ALIEN->formation_pos.set_y(
                j * ( ALIEN_PIXEL_HEIGHT + ALIEN_V_SPACE )
            );
        }
    }

    
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
        std::string s = "MUCKYVADERS " + copyright + " 2023";

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
                game_eng.get_prime_render_block(),
                circle_image_id,
                alien_1_palette_id,
                CIRCLE_PIXEL_SIZE * 2.5f  // x coord
                    + rand() % ( WINDOW_WIDTH_IN_PIXELS - 5 * CIRCLE_PIXEL_SIZE ),
                CIRCLE_PIXEL_SIZE * 2.5f  // y coord
                    + rand() % ( WINDOW_HEIGHT_IN_PIXELS - 5 * CIRCLE_PIXEL_SIZE ),
                MCK::MAX_Z_VALUE,
                CIRCLE_PIXEL_SIZE,  // width_in_pixels,
                CIRCLE_PIXEL_SIZE  // height_in_pixels,
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
                ( ( rand() % 10 ) - 5 ) * 0.03f,
                ( ( rand() % 10 ) - 5 ) * 0.03f
            )
        );

        // Initialize circular collision by setting radius 
        test_sprites.back()->MCK::SpriteCollisionCircle::set_vals(
            CIRCLE_PIXEL_SIZE / 2.0f,  // Radius
            CIRCLE_PIXEL_SIZE / 2.0f,  // x-offset
            CIRCLE_PIXEL_SIZE / 2.0f  // y-offset
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

    /*
    // Test sprite 1
    auto test_sprite = std::make_shared<
        MCK::Sprite<
            MCK::SpriteMotionConstVel,
            MCK::SpriteAnimBase,
            MCK::SpriteCollisionCircle
        >
    >();

    try
    {
        test_sprite->init(
            image_man,
            game_eng.get_prime_render_block(),
            circle_image_id,
            alien_1_palette_id,
            100,  // x
            200,  // y
            MCK::MAX_Z_VALUE,
            CIRCLE_PIXEL_SIZE,  // width_in_pixels,
            CIRCLE_PIXEL_SIZE  // height_in_pixels,
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create simple sprite, error :" )
            + e.what() ) );
    }

    // Initialize motion for const velocity test sprite 
    test_sprite->MCK::SpriteMotionConstVel::set_vel(
        MCK::Point<float>( 0.01f * 3, -0.02f * 3 )
    );

    // Initialize circular collision by setting radius 
    test_sprite->MCK::SpriteCollisionCircle::set_vals(
        CIRCLE_PIXEL_SIZE / 2.0f,  // Radius
        CIRCLE_PIXEL_SIZE / 2.0f,  // x-offset
        CIRCLE_PIXEL_SIZE / 2.0f  // y-offset
        // 0.0f,  // x-offset
        // 0.0f   // y-offset
    );

    // TEST CODE
    // Add test_sprite to collision processing
    try
    {
        MCK::Point<float> pos 
            = test_sprite->MCK::SpritePos::get_pos();
        bool rc = coll_proc.add_sprite(
            std::dynamic_pointer_cast<MCK::SpriteCollisionRect>( test_sprite )
        );
        std::cout << "rc = " << rc << std::endl;
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to add test_sprite to coll_proc, error :" )
            + e.what() ) );
    }

    // END OF TEST CODE

    // Test sprite 2
    auto test_sprite2 = std::make_shared<
        MCK::Sprite<
            MCK::SpriteMotionConstVel,
            MCK::SpriteAnimTime,
            MCK::SpriteCollisionCircle
        >
    >();
    try
    {
        test_sprite2->init(
            image_man,
            game_eng.get_prime_render_block(),
            circle_image_id,
            alien_2_palette_id,
            100,  // x
            200,  // y
            MCK::MAX_Z_VALUE,
            CIRCLE_PIXEL_SIZE,  // width_in_pixels,
            CIRCLE_PIXEL_SIZE  // height_in_pixels,
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create simple sprite2, error :" )
            + e.what() ) );
    }

    // Initialize circular collision by setting radius 
    test_sprite2->MCK::SpriteCollisionCircle::set_vals(
        CIRCLE_PIXEL_SIZE / 2.0f,  // Radius
        CIRCLE_PIXEL_SIZE / 2.0f,  // x-offset
        CIRCLE_PIXEL_SIZE / 2.0f  // y-offset
        // 0.0f,  // x-offset
        // 0.0f   // y-offset
    );
    // Initialize animation for test sprite 2 
    test_sprite2->MCK::SpriteAnimTime::set_frames(
        {
            MCK::SpriteFrame(
                5000,
                circle_image_id,
                alien_2_palette_id
            ),
            MCK::SpriteFrame(
                5000,
                circle_image_id,
                alien_3_palette_id
            )
        }
    );

    // Initialize motion for const velocity test sprite 2 
    test_sprite2->MCK::SpriteMotionConstVel::set_vel(
        MCK::Point<float>( 0.02f * 4, -0.01f * 4 )
    );

    // TEST CODE
    // Add test_sprite2 to collision processing
    try
    {
        MCK::Point<float> pos 
            = test_sprite2->MCK::SpritePos::get_pos();
        bool rc = coll_proc.add_sprite(
            std::dynamic_pointer_cast<MCK::SpriteCollisionRect>( test_sprite2 )
        );
        std::cout << "rc = " << rc << std::endl;
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to add test_sprite_2 to coll_proc, error :" )
            + e.what() ) );
    }
    // END OF TEST CODE
    */


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
            for( MCK::CollisionEvent coll :  collisions )
            {
                if( ( frame_num + count++ ) % 4 != 0 )
                {
                    continue;
                }

                // Sprite A
                if( coll.sprite_A.get() != 0 
                    && std::dynamic_pointer_cast<MCK::SpriteAnimBase>( coll.sprite_A )->get_type() 
                        == MCK::SpriteAnimType::TEMPORAL
                )
                {
                    std::dynamic_pointer_cast<MCK::SpriteAnimTime>( coll.sprite_A )
                        ->next_frame();
                }
                
                // Sprite B
                if( coll.sprite_B.get() != 0 
                    && std::dynamic_pointer_cast<MCK::SpriteAnimBase>( coll.sprite_B )->get_type() 
                        == MCK::SpriteAnimType::TEMPORAL
                )
                {
                    std::dynamic_pointer_cast<MCK::SpriteAnimTime>( coll.sprite_B )
                        ->next_frame();
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

            /////////////////////////
            // Move alien formation
            {
                // Get total pixels moved by formation since
                // start.
                const uint32_t RAW_X_POS 
                    = uint32_t( 
                        float( CURRENT_TICKS - START_TICKS )
                            * ALIEN_FORMATION_SPEED
                    );

                // Determine if formation is now moving left (true)
                // or right (false)
                const bool LEFT
                    = ( RAW_X_POS / ALIEN_FORMATION_X_SPAN ) % 2;

                // Calculate pixel displacement (from left or right)
                const uint32_t X_DISP 
                    = RAW_X_POS % ALIEN_FORMATION_X_SPAN;

                // Calculate actual screen position
                alien_formation_block->hoz_offset
                    = LEFT ?
                        X_DISP + BORDER_X
                        : ALIEN_FORMATION_X_SPAN - X_DISP + BORDER_X;
            }
        }
        

        // Process all alien sprites
        bool all_in_formation = true;
        for( AlienSprite &aln : aliens )
        {
            // For purpose of making info text active
            if( !aln.in_formation )
            {
                all_in_formation = false;
            }

            // Create render info for newly appearing aliens
            if( aln.render_info.get() == NULL 
                && aln.appearance_ticks < CURRENT_TICKS
            )
            {
                // Play sound effect on appearance
                try
                {
                    MCK::GameEngAudio::voice_command(
                        FX_1_VOICES.at( fx_1_voice_index ),
                        MCK::VoiceSynth::construct_command(
                            0x10, // Pitch ID
                            2  // Duration ID
                        )
                    );
                }
                catch( std::exception &e )
                {
                    std::cout << "(2)Failed to issue voice "
                              << "command, error = "
                              << e.what();
                }

                // Update FX 1 index
                fx_1_voice_index =
                    ( fx_1_voice_index + 1 )
                        % FX_1_VOICES.size(); 

                // If alien is in formation, use formation pos,
                // otherwise if starting line segment is defined,
                // place alien 'dist' pixels along line segment.
                const MCK::Point<float> POS
                    = aln.in_formation ?
                        aln.formation_pos :
                        aln.current_seg.get() == NULL ?
                            MCK::Point<float>( 0.0f, 0.0f ) :
                            aln.current_seg->get_point_by_arc_len(
                                aln.dist
                            );

                try
                {
                    aln.render_info = image_man.create_render_info(
                        aln.image_id,
                        aln.palette_id,
                        POS.get_x(),
                        POS.get_y(),
                        ALIEN_PIXEL_WIDTH,
                        ALIEN_PIXEL_HEIGHT,
                        aln.in_formation ?
                            alien_formation_block :
                            sprite_block
                    );
                }
                catch( std::exception &e )
                {
                    std::cout << "Failed to create render info, error: "
                      << e.what() << std::endl;
                }
            }

            // Continue to process only aliens NOT in formation
            // with render info present
            if( aln.in_formation
                || aln.render_info.get() == NULL
            )
            {
                continue;
            }

            // Update alien's speed
            if( aln.acc > 0 )
            {
                aln.current_speed = std::min(
                    aln.target_speed,
                    aln.current_speed 
                        + aln.acc * TICKS_SINCE_LAST_ANIM
                );
            }
            else
            {
                aln.current_speed = std::max(
                    aln.target_speed,
                    aln.current_speed 
                        + aln.acc * TICKS_SINCE_LAST_ANIM
                );
            }

            // If no line segment assigned, assume
            // alien is docking with formation
            if( aln.current_seg.get() == NULL )
            {
                // Estimate distance travelled (in pixels)
                // since last animation update
                const float DIST
                    = aln.current_speed * TICKS_SINCE_LAST_ANIM;

                // Create a cubic Bezier curve plotting a route
                // from aliens current position to it's (current)
                // formation position.........

                // Set first control point, P0, as current position
                const MCK::Point<float> P0(
                    aln.render_info->dest_rect.get_x(),
                    aln.render_info->dest_rect.get_y()
                );

                // Set last control point, P3, as alien's formation
                // position (within formation block),
                // tranformed into sprite block's coord
                // system.
                const MCK::Point<float> P3
                    = aln.formation_pos
                        + MCK::Point<float>(
                            alien_formation_block->hoz_offset,  
                            alien_formation_block->vert_offset
                        );
                
                // Set third control point, P2, a fixed hoz distance
                // from P3, in direction of P0.
                const MCK::Point<float> P2(
                    P3.get_x() 
                        - MCK::Point<float>::comp_x( P0, P3 )
                            * std::min(
                                fabs( 
                                    P0.get_x() - P3.get_x()
                                ),
                                100.0f
                            ),
                    P3.get_y()
                );
                
                // Second control point, P1, is simply the 
                // alien's 'temp_control_point'
                const MCK::Point<float> &P1 = aln.temp_control_point;

                // Create cubic Bezier curve
                const MCK::BezierCurveCubic<MCK::Point<float>> bez(
                    P0,
                    P1,
                    P2,
                    P3
                );

                // Estimate 't' value on Bezier curve by assuming
                // that, for small 't' values, direction is in
                // a straight line from P0 towards P1, with 't'
                // being the proportion of distance P0->P1.
                // For safety, the 't' value is capped at 1.0f.
                const double EST_T
                    = std::min(
                        1.0f,
                        DIST / sqrt( 
                            MCK::Point<float>::dist_sq( P0, P1 ) 
                        )
                    );

                // Calculate new position for alien, using
                // the Bezier curve and the estimated 't' value
                const MCK::Point<float> NEW_POS
                    = bez.get_point( EST_T );

                // Calculate (square of) distance from 
                // the alien's formation point
                const float DIST_SQ = MCK::Point<float>::dist_sq( NEW_POS, P3 );

                // If we've (tolerably) reached the formation point,
                // move the alien to the formation block and declare
                // it to be in formation
                if( DIST_SQ < 1.0f )  // 1.0f = to nearest pixel
                {
                    // Move alien to formation block
                    try
                    {
                        MCK::GameEng::move_render_instance(
                            aln.render_info,
                            sprite_block,
                            alien_formation_block
                        );
                    }
                    catch( std::exception &e )
                    {
                        throw( std::runtime_error(
                            std::string( "Failed to move alien to formation block, error: ")
                            + e.what() ) );
                    }
                    
                    // Set formation position
                    aln.render_info->dest_rect.set_x(
                        aln.formation_pos.get_x()
                    ); 
                    aln.render_info->dest_rect.set_y(
                        aln.formation_pos.get_y()
                    ); 
                   
                    aln.in_formation = true;

                    // Play FX when joins formation
                    try
                    {
                        MCK::GameEngAudio::voice_command(
                            FX_0_VOICES.at( fx_0_voice_index ),
                            MCK::VoiceSynth::construct_command(
                                0x1F, // Pitch ID
                                1  // Duration ID
                            )
                        );
                    }
                    catch( std::exception &e )
                    {
                        std::cout << "Failed to issue voice "
                                  << "command, error = "
                                  << e.what();
                    }

                    // Update FX 0 index
                    fx_0_voice_index =
                        ( fx_0_voice_index + 1 )
                            % FX_0_VOICES.size(); 
                }
                else
                {
                    // If not yet in formation,
                    // set alien position
                    aln.render_info->dest_rect.set_x(
                        NEW_POS.get_x()
                    ); 
                    aln.render_info->dest_rect.set_y(
                        NEW_POS.get_y()
                    ); 
                   
                    // We need to calculate a new temporary control
                    // point, for use in the next animation step.
                    // To do this we split the Bezier curve at 'EST_T',
                    // and take the second control point of the resulting
                    // sub-curve.
                    aln.temp_control_point
                        = bez.split_hi( EST_T ).get_control_point( 1 );
                }

                // Skip to next alien
                continue;
            }

            aln.dist += aln.current_speed * TICKS_SINCE_LAST_ANIM;
            bool skip = false;
            while( aln.dist > aln.current_seg->get_length() )
            {
                aln.dist -= aln.current_seg->get_length();
                
                if( aln.current_seg->has_single_connection() )
                {
                    // Move alien to next segment on path
                    try
                    {
                        aln.current_seg = aln.current_seg->get_single_connection();
                    }
                    catch( std::exception &e )
                    {
                        throw( std::runtime_error(
                            std::string( "Failed to get next segment, error: ")
                            + e.what() ) );
                    }
                }
                else
                {
                    // Get third and fourth contol points for
                    // existing segment
                    const MCK::BezierCurveCubic<MCK::Point<float>> CURVE
                        = aln.current_seg->get_curve();
                    const MCK::Point<float> P2 = CURVE.get_control_point( 2 );
                    const MCK::Point<float> P3 = CURVE.get_control_point( 3 );
                    
                    // Set temporary control point by adding
                    // vector P2->P3 to P3
                    aln.temp_control_point = P3 * 2 - P2;

                    // Set current location of alien as P3
                    // plus a vector of magnitude 'dist' in 
                    // direction P2->P3
                    const MCK::Point<float> NEW_POS
                        = P3 + ( P3 - P2 )
                            / sqrt(
                                MCK::Point<float>::dist_sq( P2, P3 ) 
                            ) * aln.dist;
                    aln.render_info->dest_rect.set_x( NEW_POS.get_x() );
                    aln.render_info->dest_rect.set_y( NEW_POS.get_y() );

                    // Clear current segment but do NOT set
                    // 'in_formation' flag; this indicates
                    // alien is docking
                    aln.current_seg.reset();

                    skip = true;
                    break;
                }
            }

            if( skip )
            {
                continue;
            }

            // Calculate position of alien, within sprite block
            const MCK::Point<float> POS
                = aln.current_seg->get_point_by_arc_len( aln.dist );

            aln.render_info->dest_rect.set_x( POS.get_x() );
            aln.render_info->dest_rect.set_y( POS.get_y() );
        }

        if( all_in_formation
            && ( !info_text_1.is_active()
                 || !info_text_2.is_active()
            )
        )
        {
            info_text_1.make_active();
            info_text_2.make_active();
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
