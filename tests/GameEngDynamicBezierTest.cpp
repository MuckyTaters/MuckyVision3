///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
//  GameEngZTest.cpp
//
//  Test/demo for dynamic Bezier curves
//  (i.e. a Bezier curve with movable
//   a end point )
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
#include "Point.h"
#include "BezierCurveCubic.h"

///////////////////////////////////////////
// DEMO PARAMETERS
// Made global for convenience, 
// as this is only a short demo.
//
const int WINDOW_WIDTH_IN_PIXELS = 640;
const int WINDOW_HEIGHT_IN_PIXELS = 480;
const int NUM_CIRCLES = 4;
const int MAX_CIRCLE_SCALE = 3;
const float MAX_CIRCLE_RADIUS = MAX_CIRCLE_SCALE * 8.0f;
const float CENTER_X = WINDOW_WIDTH_IN_PIXELS / 2;
const float CENTER_Y = WINDOW_HEIGHT_IN_PIXELS / 2;
const float CENTER_Z = 0;
const int LOGO_SCALE = 4;
const int LOGO_X = WINDOW_WIDTH_IN_PIXELS - 80 * LOGO_SCALE;
const int LOGO_Y = WINDOW_HEIGHT_IN_PIXELS - 8 * LOGO_SCALE;
const float CONTROL_POINT_H_OFFSET = 0.0f;
const float CONTROL_POINT_V_OFFSET = 150.0f;
const std::vector<MCK::Point<float>> START_POINTS {
    MCK::Point<float>(
        0.0f,
        WINDOW_HEIGHT_IN_PIXELS / 3.0f * 1.0f,
        0.0f
    ),
    MCK::Point<float>(
        0.0f,
        WINDOW_HEIGHT_IN_PIXELS / 3.0f * 2.0f,
        0.0f
    ),
    MCK::Point<float>(
        WINDOW_WIDTH_IN_PIXELS - 2.0f * MAX_CIRCLE_RADIUS,
        WINDOW_HEIGHT_IN_PIXELS / 3.0f * 1.0f,
        0.0f
    ),
    MCK::Point<float>(
        WINDOW_WIDTH_IN_PIXELS - 2.0f * MAX_CIRCLE_RADIUS,
        WINDOW_HEIGHT_IN_PIXELS / 3.0f * 2.0f,
        0.0f
    )
};

/////////////////////////////////////////////////////////
// Utility function to set circle position and size
void set_circle_pos(
    MCK::GameEng &game_eng,
    std::shared_ptr<MCK::GameEngRenderInfo> circle,
    MCK::Point<float> pos,
    float scale = 1.0f
)
{
    const float Z = pos.get_z();

    const float SCALE 
        = std::min(
            1.0f,
            Z / ( Z - CENTER_Z )
        );

    const float CIRCLE_DIAMETER
        = SCALE * MAX_CIRCLE_RADIUS * 2.0f;
    
    // Offset to adjust X and Y to account for actual circle diameter
    const float OFFSET
        = ( 1.0f - SCALE ) *  MAX_CIRCLE_RADIUS;

    // Set circle location/size
    circle->dest_rect.set_x( int( pos.get_x() + OFFSET + 0.5f ) );
    circle->dest_rect.set_y( int( pos.get_y() + OFFSET + 0.5f ) );
    circle->dest_rect.set_w( int( CIRCLE_DIAMETER + 0.5f ) );
    circle->dest_rect.set_h( int( CIRCLE_DIAMETER + 0.5f ) );

    // Change 'z' value
    try
    {
        MCK::GameEng::change_z_rel_to_default(
            circle,
            game_eng.get_prime_render_block(),
            int32_t( Z + 0.5f )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to change z, error: ")
            + e.what() ) );
    }
}


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
            "GameEngDynamicBezierTest",  // Window name
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
    // SET BACKGROUND COLO(U)R (BEST EFFORT)
    game_eng.set_clearing_color( MCK::COL_ROTTING_PURPLE );


    ///////////////////////////////////////////
    // CREATE LOCAL COLO(U)R PALETTES
    // (subsets of the core 32 colo(u)r palette)
    
    // Black/white palette for 1bit images
    const MCK_PAL_ID_TYPE LOGO_PALETTE_ID = 0;
    const std::vector<uint8_t> LOGO_PALETTE =
    {
        MCK::COL_BLACK,
        MCK::COL_YELLOW
    };

    // Palette A for 2bit images
    const MCK_PAL_ID_TYPE PALETTE_A_ID = 1;
    const std::vector<uint8_t> PALETTE_A =
    {
        MCK::COL_TRANSPARENT,
        MCK::COL_WHITE,
        MCK::COL_BLACK,
        MCK::COL_RED
    };

    // Palette B for 2bit images
    const MCK_PAL_ID_TYPE PALETTE_B_ID = 2;
    const std::vector<uint8_t> PALETTE_B =
    {
        MCK::COL_TRANSPARENT,
        MCK::COL_WHITE,
        MCK::COL_BLACK,
        MCK::COL_GREEN
    };

    // Palette C for 2bit images
    const MCK_PAL_ID_TYPE PALETTE_C_ID = 3;
    const std::vector<uint8_t> PALETTE_C =
    {
        MCK::COL_TRANSPARENT,
        MCK::COL_WHITE,
        MCK::COL_BLACK,
        MCK::COL_BLUE
    };


    ///////////////////////////////////////////
    // CREATE IMAGE DATA

    // Circle of 16x16 pixels and 4 colo(u)rs
    // Each 8bit binary literal here represents 4 pixels,
    // with 00 = local palette colo(u)r 0
    //      01 = local palette colo(u)r 1
    //      ... and so on
    const MCK_IMG_ID_TYPE IMAGE_CIRCLE_2BIT_16x16_ID = 0;
    const std::vector<uint8_t> IMAGE_CIRCLE_2BIT_16x16 =
    {
        0b00000000, 0b00101010, 0b10101000, 0b00000000,
        0b00000010, 0b10111111, 0b11111110, 0b10000000,
        0b00001011, 0b11011111, 0b11111111, 0b11100000,
        0b00101111, 0b01111111, 0b11111111, 0b11111000,
        0b00101111, 0b01111111, 0b11111111, 0b11111000,
        0b10111101, 0b11111111, 0b11111111, 0b11111110,
        0b10111111, 0b11111111, 0b11111111, 0b11111110,
        0b10111111, 0b11111111, 0b11111111, 0b11111110,
        0b10111111, 0b11111111, 0b11111111, 0b11111110,
        0b10111111, 0b11111111, 0b11111111, 0b11111110,
        0b10111111, 0b11111111, 0b11111111, 0b11111110,
        0b00101111, 0b11111111, 0b11111111, 0b11111000,
        0b00101111, 0b11111111, 0b11111111, 0b11111000,
        0b00001011, 0b11111111, 0b11111111, 0b11100000,
        0b00000010, 0b10111111, 0b11111110, 0b10000000,
        0b00000000, 0b00101010, 0b10101000, 0b00000000

    };

    // Left hand side of logo
    // Each bit here represents 1 pixel,
    // with 40 pixels in each row
    const MCK_IMG_ID_TYPE LOGO_LEFT_ID = 1;
    const std::vector<uint8_t> LOGO_LEFT =
    {
        0b00111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
        0b01000001, 0b01000101, 0b00100110, 0b01001010, 0b00101111, 
        0b01011101, 0b01101101, 0b00101001, 0b01010001, 0b01000010, 
        0b01010001, 0b01010101, 0b00101000, 0b01100000, 0b10000010, 
        0b01011101, 0b01000101, 0b00101001, 0b01010000, 0b10000010, 
        0b01000001, 0b01000100, 0b11000110, 0b01001000, 0b10000010, 
        0b00111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000
    };

    // Right hand side of logo
    // Each bit here represents 1 pixel,
    // with 40 pixels in each row
    const MCK_IMG_ID_TYPE LOGO_RIGHT_ID = 2;
    const std::vector<uint8_t> LOGO_RIGHT =
    {
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
        0b10011001, 0b11110111, 0b10111001, 0b11101011, 0b11011110, 
        0b00100100, 0b01000100, 0b00100101, 0b00001000, 0b01000010, 
        0b00111100, 0b01000111, 0b00111001, 0b11100011, 0b11011110, 
        0b00100100, 0b01000100, 0b00100100, 0b00100010, 0b00000010, 
        0b00100100, 0b01000111, 0b10100101, 0b11100011, 0b11011110, 
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000
    };


    ///////////////////////////////////////////
    // CREATE TEXTURES

    // Type A circle texture
    MCK_TEX_ID_TYPE circle_A_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                IMAGE_CIRCLE_2BIT_16x16_ID,
                PALETTE_A_ID,
                2,  // bits_per_pixel,
                16,  // pitch_in_pixels,
                IMAGE_CIRCLE_2BIT_16x16,
                PALETTE_A,
                circle_A_tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create circle A texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 16 )
        {
            throw( std::runtime_error(
                std::string( "Circle A texture has incorrect height")
            ) );
        }
    }

    // Type B circle texture
    MCK_TEX_ID_TYPE circle_B_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                IMAGE_CIRCLE_2BIT_16x16_ID,
                PALETTE_B_ID,
                2,  // bits_per_pixel,
                16,  // pitch_in_pixels,
                IMAGE_CIRCLE_2BIT_16x16,
                PALETTE_B,
                circle_B_tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create circle B texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 16 )
        {
            throw( std::runtime_error(
                std::string( "Circle B texture has incorrect height")
            ) );
        }
    }

    // Type C circle texture
    MCK_TEX_ID_TYPE circle_C_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                IMAGE_CIRCLE_2BIT_16x16_ID,
                PALETTE_C_ID,
                2,  // bits_per_pixel,
                16,  // pitch_in_pixels,
                IMAGE_CIRCLE_2BIT_16x16,
                PALETTE_C,
                circle_C_tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create circle C texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 16 )
        {
            throw( std::runtime_error(
                std::string( "Circle C texture has incorrect height")
            ) );
        }
    }

    // Logo left texture
    MCK_TEX_ID_TYPE logo_left_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                LOGO_LEFT_ID,
                LOGO_PALETTE_ID,
                1,  // bits_per_pixel,
                40,  // pitch_in_pixels,
                LOGO_LEFT,
                LOGO_PALETTE,
                logo_left_tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create logo left texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 8 )
        {
            throw( std::runtime_error(
                std::string( "Logo left texture has incorrect height")
            ) );
        }
    }

    // Logo right texture
    MCK_TEX_ID_TYPE logo_right_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                LOGO_RIGHT_ID,
                LOGO_PALETTE_ID,
                1,  // bits_per_pixel,
                40,  // pitch_in_pixels,
                LOGO_RIGHT,
                LOGO_PALETTE,
                logo_right_tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create logo right texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != 8 )
        {
            throw( std::runtime_error(
                std::string( "Logo right texture has incorrect height")
            ) );
        }
    }


    ///////////////////////////////////////////
    // CREATE RENDER INFO
    
    // Create circles
    MCK::Point<float> target(
        CENTER_X - MAX_CIRCLE_RADIUS,
        CENTER_Y - MAX_CIRCLE_RADIUS
    );
    struct circ
    {
        MCK::BezierCurveCubic<MCK::Point<float>> curve;
        std::shared_ptr<MCK::GameEngRenderInfo> info;
        double speed_in_pixels_per_sec;
        uint32_t ticks_at_last_processing;
            
        circ( void ) 
        {
            speed_in_pixels_per_sec = 0.0f;
            ticks_at_last_processing = 0;
        }

        circ( 
            MCK::BezierCurveCubic<MCK::Point<float>> _curve,
            std::shared_ptr<MCK::GameEngRenderInfo> _info,
            double _speed_in_pixels_per_sec,
            uint32_t _ticks_at_last_processing
        ) : info( _info ), 
            speed_in_pixels_per_sec ( _speed_in_pixels_per_sec ),
            ticks_at_last_processing ( _ticks_at_last_processing )
        {
            std::swap( curve, _curve );
        }
    };

    std::vector<circ> circles;
    circles.reserve( NUM_CIRCLES );
    for( int i = 0; i < NUM_CIRCLES; i++ )
    {
        // Cycle through circle textures
        MCK_TEX_ID_TYPE tex_id;
        switch( i % 3 )
        {
            case 0:
                tex_id = circle_A_tex_id;
                break;

            case 1:
                tex_id = circle_B_tex_id;
                break;

            case 2:
                tex_id = circle_C_tex_id;
                break;
        }

        const MCK::Point<float> P0 = START_POINTS[ i % 4 ];

        try
        {
            circles.push_back( 
                circ(
                    MCK::BezierCurveCubic<MCK::Point<float>>(
                        P0,
                        P0 + MCK::Point<float>(
                            CONTROL_POINT_H_OFFSET,
                            - CONTROL_POINT_V_OFFSET
                            ),
                        target + MCK::Point<float>(
                            - CONTROL_POINT_H_OFFSET,
                            - CONTROL_POINT_V_OFFSET
                            ),
                        target
                    ),
                    game_eng.create_render_info(
                        tex_id,
                        game_eng.get_prime_render_block(),
                        // This rect is overwritten on first frame
                        MCK::GameEngRenderInfo::Rect(
                            P0.get_x(),
                            P0.get_y(),
                            MAX_CIRCLE_RADIUS * 2.0f,
                            MAX_CIRCLE_RADIUS * 2.0f
                        )
                    ),
                    32.0f + i * 16.0f,  // Speed
                    game_eng.get_ticks()  // Ticks at last processing
                )
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create circle, error: ")
                + e.what() ) );
        }
    }

    // Logo
    try
    {
        // Logo left
        game_eng.create_render_info(
            logo_left_tex_id,
            game_eng.get_prime_render_block(),
            MCK::GameEngRenderInfo::Rect(
                LOGO_X,
                LOGO_Y,
                40 * LOGO_SCALE,
                8 * LOGO_SCALE
            ),
            false,  // No clip
            MCK::GameEngRenderInfo::Rect(),
            0,  // No rotation
            false,  // No flip x
            false,  // No flip y
            MCK::MAX_Z_VALUE  // Always on top
        );

        // Logo right
        game_eng.create_render_info(
            logo_right_tex_id,
            game_eng.get_prime_render_block(),
            MCK::GameEngRenderInfo::Rect(
                LOGO_X + 40 * LOGO_SCALE,
                LOGO_Y,
                40 * LOGO_SCALE,
                8 * LOGO_SCALE
            ),
            false,  // No clip
            MCK::GameEngRenderInfo::Rect(),
            0,  // No rotation
            false,  // No flip x
            false,  // No flip y
            MCK::MAX_Z_VALUE  // Always on top
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create logo, error: ")
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
    uint32_t end_ticks = current_ticks + 60000;
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

        //////////////////////////////////////////
        // MOVEMENT

        // Loop over circles
        for( int i = 0; i < NUM_CIRCLES; i++ )
        {
            // Get (mutable) reference to this circle
            circ &c = circles[i];

            // Get references to first and second
            // control points
            const MCK::Point<float> &P0
                = c.curve.get_control_point( 0 );
            const MCK::Point<float> &P1
                = c.curve.get_control_point( 1 );

            // Estaimte number of pixels moved
            const double EST_PIXELS_MOVED
                = c.speed_in_pixels_per_sec / 1000.f
                    * ( current_ticks - c.ticks_at_last_processing );
            c.ticks_at_last_processing = current_ticks;

            // Estimate new 't' value on Bezier Curve
            double est_t
                = std::min(
                    double( 1.0f ),
                    EST_PIXELS_MOVED / sqrt( 
                        MCK::Point<float>::dist_sq( P0, P1 ) 
                    )
                  );

            // Replace Bezier curve with sub-set of current curve,
            // starting at new value of 't'
            c.curve = c.curve.split_hi( est_t );

            // Calculate new position
            const MCK::Point<float> P0_new
                = c.curve.get_point( est_t );

            // Set new position
            c.info->dest_rect.set_x( P0_new.get_x() );
            c.info->dest_rect.set_y( P0_new.get_y() );

            // If we've (tolerably) reached target point,
            // return circle to original position and
            // start again.
            if( MCK::Point<float>::dist_sq( P0_new, target )
                    < 0.001f
            )
            {
                const MCK::Point<float> P0 = START_POINTS[ i % 4 ];
                c.curve = MCK::BezierCurveCubic<MCK::Point<float>>(
                    P0,
                    P0 + MCK::Point<float>(
                        CONTROL_POINT_H_OFFSET,
                        - CONTROL_POINT_V_OFFSET
                        ),
                    target + MCK::Point<float>(
                        - CONTROL_POINT_H_OFFSET,
                        - CONTROL_POINT_V_OFFSET
                        ),
                    target
                );
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
