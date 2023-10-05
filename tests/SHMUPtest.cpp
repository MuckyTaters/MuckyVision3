///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
//  SHMUPtest.cpp
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
#include "Console.h"
#include "GameEngAudio.h"


/////////////////////////////////////////////
// DEMO PARAMETERS
// (global for convenience, as only a short program)

// Define window size (these can be changed to any sensible value)
const int WINDOW_WIDTH_IN_PIXELS = 640;
const int WINDOW_HEIGHT_IN_PIXELS = 360;

// Demo specific parameters
const int TEXT_SIZE = 16;
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
const int ALIEN_PIXEL_WIDTH = ALIEN_RAW_PIXEL_WIDTH * ALIEN_SCALE;
const int ALIEN_PIXEL_HEIGHT = ALIEN_RAW_PIXEL_HEIGHT * ALIEN_SCALE;
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
    2205, SAW,  2,   550,   550,   8820,   192,    128,  // 0
    2205, SAW,  2,   550,   550,   8820,   192,    128,  // 1
    2205, SAW,  2,   550,   550,   8820,   192,    128,  // 2
    2205, SAW,  2,   550,   550,   8820,   192,    128,  // 3
    2205, SAW,  2,   550,   550,   8820,   192,    128,  // 4
    2205, SAW,  2,   550,   550,   8820,   192,    128,  // 5
    2205, SAW,  2,   550,   550,   8820,   192,    128,  // 6
    2205, SAW,  2,   550,   550,   8820,   192,    128   // 7
};
const int VOICE_DATA_COLS = 8;

/////////////////////////////////////////////
// ALIEN PATH DATA
//  X   Y  ( units = window width in pixels / 40 )
const std::vector<int16_t> PATH_DATA = {
    0,  0,  // p0
    2,  4,  // p1
    5,  15, // p2
    20, 15, // p3/p0
    30, 15, // p1
    38, 15, // p2
    38, 10  // p3/p0
};

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
            "SHMUPtest",  // Window name
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
    
    ////////////////////////////////////////////
    // CREATE IMAGES
    
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
    std::shared_ptr<MCK::GameEngRenderBlock> alien_formation_block;
    try
    {
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
    // Create animation info

    // Create entry path for aliens (in sprite_block)
    std::shared_ptr<
        MCK::LineSegment<
            MCK::BezierCurveCubic,
            MCK::Point<float>
        >
    > start_seg;
    std::shared_ptr<
        MCK::LineSegment<
            MCK::BezierCurveCubic,
            MCK::Point<float>
        >
    > prev_seg;
    {
        const size_t NUM_SEGS = ( PATH_DATA.size() - 2  ) / 6;

        // DEBUG
        std::cout << "NUM_SEGS = " << NUM_SEGS << std::endl;

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
                        PATH_DATA[ BASE + 0 ] * PATH_SCALE,
                        PATH_DATA[ BASE + 1 ] * PATH_SCALE,
                        sprite_block
                    ),
                    MCK::Point<float>(  // p1
                        PATH_DATA[ BASE + 2 ] * PATH_SCALE,
                        PATH_DATA[ BASE + 3 ] * PATH_SCALE,
                        sprite_block
                    ),
                    MCK::Point<float>(  // p2
                        PATH_DATA[ BASE + 4 ] * PATH_SCALE,
                        PATH_DATA[ BASE + 5 ] * PATH_SCALE,
                        sprite_block
                    ),
                    MCK::Point<float>(  // p3
                        PATH_DATA[ BASE + 6 ] * PATH_SCALE,
                        PATH_DATA[ BASE + 7 ] * PATH_SCALE,
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

            // DEBUG
            std::cout << ">> Line seg " << i << ": " << std::endl;
            new_seg->str();

            // Store pointer to first segment only
            if( i == 0 )
            {
                start_seg = new_seg;
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

    // POD struct to hold sprite info
    struct BasicSprite
    {
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

    // Alien sprites
    struct AlienSprite : public BasicSprite
    {
        // Coordinates of alien within alien_formation_block
        MCK::Point<float> formation_pos;

        bool in_formation;

        int id;

        // Used only when docking with formation
        MCK::Point<float> temp_control_point;

        // Default constructor
        AlienSprite( void )
        {
            in_formation = true;
            id = -1;
        }
    };

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

            // DEBUG
            ALIEN->in_formation = !(
                ( i == ALIEN_COLS - 1 )
                && ( j == ALIEN_ROWS - 1 )
            );
            
            // Set intial path location
            ALIEN->current_seg = start_seg;
            ALIEN->dist = 0.0f;

            // Set speed
            ALIEN->current_speed = 640.0f / 1000.0f;
            ALIEN->target_speed = 32.0f / 1000.0f;
            ALIEN->acc = -0.25f / 1000.0f;

            // Set (evantual) position within formation
            ALIEN->formation_pos.set_x(
                i * ( ALIEN_PIXEL_WIDTH + ALIEN_H_SPACE )
            );
            ALIEN->formation_pos.set_y(
                j * ( ALIEN_PIXEL_HEIGHT + ALIEN_V_SPACE )
            );

            // Create render info, and set x,y coords
            try
            {
                if( ALIEN->in_formation )
                {
                    ALIEN->render_info = image_man.create_render_info(
                        image_id,
                        palette_id,
                        ALIEN->formation_pos.get_x(),
                        ALIEN->formation_pos.get_y(),
                        ALIEN_PIXEL_WIDTH,
                        ALIEN_PIXEL_HEIGHT,
                        alien_formation_block 
                    );
                }
                else
                {
                    ALIEN->render_info = image_man.create_render_info(
                        image_id,
                        palette_id,
                        -1 * ALIEN_PIXEL_WIDTH,
                        -1 * ALIEN_PIXEL_HEIGHT,
                        ALIEN_PIXEL_WIDTH,
                        ALIEN_PIXEL_HEIGHT,
                        sprite_block
                    );
                }
            }
            catch( std::exception &e )
            {
                std::cout << "Failed to create render info, error: "
                  << e.what() << std::endl;
            }

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
    const uint32_t START_TICKS = current_ticks;
    uint32_t start_ticks = current_ticks;
    uint32_t next_frame_ticks = current_ticks + TICKS_PER_FRAME; 
    uint32_t end_ticks = current_ticks + 250000;
    uint32_t ticks_at_last_animation = current_ticks;
    uint32_t ticks_at_last_fps_update = current_ticks;
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

        /*
        ////////////////////////////////////////
        // Update FPS
        if( frame_num % 50 == 0 )
        {
            const float TICKS_PASSED_F
                = current_ticks - ticks_at_last_fps_update;
            ticks_at_last_fps_update
                = current_ticks;

            try/
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
        */

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

        // Animate formation
        const uint32_t TICKS_SINCE_LAST_ANIM
            = current_ticks - ticks_at_last_animation;
        ticks_at_last_animation = current_ticks;
        if( TICKS_SINCE_LAST_ANIM > 0 )
        {
            // Get total pixels moved by formation since
            // start.
            const uint32_t RAW_X_POS 
                = uint32_t( 
                    float( current_ticks - START_TICKS )
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

        // Animate aliens not in formation
        for( AlienSprite &aln : aliens )
        {
            // DEBUG
            // std::cout << "aln.id = " << aln.id << std::endl;

            // Ignore those in formation
            if( aln.in_formation )
            {
                continue;
            }

            // NULL pointer check
            if( aln.render_info.get() == NULL )
            {
                std::cout << "Alien " << aln.id
                          << " render_info pointer is NULL"
                          << std::endl;
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
                        - MCK::Point<float>(
                            alien_formation_block->hoz_offset,  
                            alien_formation_block->vert_offset
                        );
                
                // Set third control point, P2, a fixed hoz distance
                // from P3, in direction of P0.
                const MCK::Point<float> P2(
                    P3.get_x() + MCK::Point<float>::comp_x( P0, P3 ) * 100,
                    P3.get_y()
                );
                
                // Second control point, P1, is simply the 
                // alien's 'temp_control_point'
                const MCK::Point<float> &P1 = aln.temp_control_point;

                // Create Bezier
                const MCK::BezierCurveCubic<MCK::Point<float>> bez(
                    P0,
                    P1,
                    P2,
                    P3
                );

                // Estimate 't' value on Bezier Curve by assuming
                // that, for small 't' values, direction is in
                // a straight line from P0 towards P1, with 't'
                // being the dis proportion of distance P0->P1.
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

                // If we've (tolerably) reached the formation point,
                // move the alien to the formation block and declare
                // it to be in formation
                if( MCK::Point<float>::dist_sq( NEW_POS, P3 )
                        < 0.001f
                )
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
                    // DEBUG
                    std::cout << "end of path" << std::endl;

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

            // DEBUG
            // std::cout << "aln.dist = " << aln.dist << std::endl;

            // Calculate position of alien, within sprite block
            const MCK::Point<float> POS
                = aln.current_seg->get_point_by_arc_len( aln.dist );

            aln.render_info->dest_rect.set_x( POS.get_x() );
            aln.render_info->dest_rect.set_y( POS.get_y() );
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
