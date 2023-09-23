///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
//  BezierTest.cpp
//
//  Test/demo for Point, BezierCurve 
//  and LineSegment classes
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
#include "LineSegmentFixed.h"
#include "GameEng.h"
#include "ImageMan.h"
#include "ImageText.h"
#include "Console.h"

/////////////////////////////////////////////
// DEMO PARAMETERS
// (global for convenience, as only a short program)

// Define window size (these can be changed to any sensible value)
const int WINDOW_WIDTH_IN_PIXELS = 640;
const int WINDOW_HEIGHT_IN_PIXELS = 360;

// Demo specific parameters
const size_t NUM_WAYPOINTS = 500;
const int BORDER_X = 0;  // 30;
const float X_MIN = float( BORDER_X );
const float X_MAX = float( WINDOW_WIDTH_IN_PIXELS - BORDER_X );
const int X_SPAN = int( X_MAX - X_MIN );
const int BORDER_Y = 0;  // 20;
const float Y_MIN = float( BORDER_Y );
const float Y_MAX = float( WINDOW_HEIGHT_IN_PIXELS - BORDER_Y );
const int Y_SPAN = int( Y_MAX - Y_MIN );
const float OFFSET_MAG = 50;
const size_t START_NUM_BALLS = 10;
const size_t END_NUM_BALLS = 5000;
const float BALL_DIST_SEP = 8.0f;  // Ball separation in pixels
const float BALL_VEL = 200.0f; // Velocity in pixels per sec
const float Z_MIN = 0.0f;
const float Z_MAX = 100.0f;
const int Z_SPAN = int( Z_MAX - Z_MIN );
const float X_MID = X_MIN + X_SPAN / 2.0f;
const float Y_MID = Y_MIN + Y_SPAN / 2.0f;
const float XY_MIN_SCALE = 1.0f; // 0.25f;  // Scale at Z_MIN
const float XY_SCALE_PER_Z = ( 1.0f - XY_MIN_SCALE ) / float( Z_SPAN );
const int MAX_CIRCLE_SIZE = 16;  // In pixels


// Function to calculate offset and size of image
// depending on 'z' value
void set_image_pos_and_size(
    std::shared_ptr<MCK::GameEngRenderInfo> &render_info,
    MCK::Point<float> p,
    std::shared_ptr<MCK::GameEngRenderBlock> &parent_render_block
)
{
    const float XY_SCALE = XY_MIN_SCALE + XY_SCALE_PER_Z * p.get_z();
    
    const int SIZE = int( MAX_CIRCLE_SIZE * XY_SCALE  + 0.5f );
    const int OFFSET = MAX_CIRCLE_SIZE - SIZE;
    render_info->dest_rect.set_all(
        int( X_MID + ( ( p.get_x() - X_MID ) ) * XY_SCALE + 0.5f )
            + OFFSET,
        int( Y_MID + ( ( p.get_y() - Y_MID ) ) * XY_SCALE + 0.5f )
            + OFFSET,
        SIZE,
        SIZE
    );
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
            "BezierTest",  // Window name
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
    MCK_PAL_ID_TYPE palette_A_id;
    MCK_PAL_ID_TYPE palette_B_id;
    MCK_PAL_ID_TYPE palette_C_id;
    try
    {
        title_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_BLACK,
                    MCK::COL_WHITE,
                }
            )
        );
        palette_A_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_WHITE,
                    MCK::COL_BLACK,
                    MCK::COL_RED
                }
            )
        );
        palette_B_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_WHITE,
                    MCK::COL_BLACK,
                    MCK::COL_DARK_RED
                }
            )
        );
        palette_C_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_WHITE,
                    MCK::COL_BLACK,
                    MCK::COL_DARK_RED_SEMI_TRANS
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

    // Circle of 16x16 pixels and 4 colo(u)rs
    // Each 8bit binary literal here represents 4 pixels,
    // with 00 = local palette colo(u)r 0
    //      01 = local palette colo(u)r 1
    //      ... and so on
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


    ////////////////////////////////////////////
    // CREATE BALL IMAGE
    MCK_IMG_ID_TYPE circle_image_id;
    try
    {
        circle_image_id = image_man.create_custom_image(
            std::make_shared<const std::vector<uint8_t>>(
                IMAGE_CIRCLE_2BIT_16x16
            ),
            2,  // bits_per_pixel,
            16, // width_in_pixels,
            16  // height_in_pixels
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create circle A, error: ")
            + e.what() ) );
    }


    ////////////////////////////////////////////
    // SET CLEARING COLO(U)R

    // Set render clearing colo(u)r for this demo
    try
    {
        game_eng.set_clearing_color( MCK::COL_ROTTING_PURPLE );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to set clearing color, error: ")
            + e.what() ) );
    }

    //////////////////////////////////////////////
    // CREATE TITLE TEXT BOXES
    std::vector<std::shared_ptr<MCK::ImageText>> title_line_text;
    {
        std::string CR( 1, uint8_t( 255 ) );

        std::vector<std::string> title = {
            CR + " MuckyTaters 2023",
            "Bezier curve sprite path stress test"
            // "github.com/MuckyTaters/MuckyVision3"
        };

        int line_count = 0;
        for( auto &s : title )
        {
            std::shared_ptr<MCK::ImageText> title_line
                = std::make_shared<MCK::ImageText>();
            try
            {
                title_line->init(
                    game_eng,
                    image_man,
                    game_eng.get_prime_render_block(),
                    title_palette_id,
                    ( WINDOW_WIDTH_IN_PIXELS - s.size() * 16 ) / 2,  // x_pos,
                    16 + 32 * line_count++,  // y_pos,
                    s.size(),  // width in chars
                    16,  // char_width_in_pixels,
                    16,  // char_height_in_pixels,
                    s,
                    MCK::ImageText::Just::CENTER,
                    0,  // char spacing
                    0,  // Default ASCII set
                    MCK::MAX_Z_VALUE  // Render on top
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to create title text, error: ")
                    + e.what() ) );
            }

            title_line_text.push_back( title_line );
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
            32,  // x_pos,
            WINDOW_HEIGHT_IN_PIXELS - 32,  // y_pos,
            8,  // width in chars
            16,  // char_width_in_pixels,
            16,  // char_height_in_pixels,
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
    
    ///////////////////////////////////////////////////
    // CREATE BALL COUNT READ OUT
    std::shared_ptr<MCK::ImageText> count_text =
        std::make_shared<MCK::ImageText>();
    try
    {
        count_text->init(
            game_eng,
            image_man,
            game_eng.get_prime_render_block(),
            title_palette_id,
            WINDOW_WIDTH_IN_PIXELS - 250,  // x_pos,
            WINDOW_HEIGHT_IN_PIXELS - 32,  // y_pos,
            14,  // width in chars
            16,  // char_width_in_pixels,
            16,  // char_height_in_pixels,
            "sprites: " + std::to_string( START_NUM_BALLS ),
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
    

    /////////////////////////////////////////////
    // CREATE LINE SEGMENTS

    // Declare (empty) vector of line fixed segments
    std::vector<
        MCK::LineSegmentFixed<
            MCK::BezierCurveCubic,
            MCK::Point<float>
        >
    > line_segs;
    line_segs.reserve( NUM_WAYPOINTS );

    // Define start point
    MCK::Point<float> current_point(
        X_MIN + float( rand() % X_SPAN ),
        Y_MIN + float( rand() % Y_SPAN ),
        0 
    );

    // Define first control point
    MCK::Point<float> current_control_offset;
    {
        float angle = float( ( rand() & 256 ) / 256 * MCK_TWO_PI );
        current_control_offset.set_x(
            float( sin( angle ) * OFFSET_MAG )
        );
        current_control_offset.set_y(
            float( cos( angle ) * OFFSET_MAG )
        );
    }

    // Construct the line segments using cubic Bezier curves
    for( int i = 0; i < NUM_WAYPOINTS; i++ )
    {
        // Declare new end point and final control point offset
        MCK::Point<float> new_point;
        MCK::Point<float> new_control_offset;

        // In order to loop the line segments:
        // If this is last line segment, make end point
        // (and final control point offset) match the start point
        // (and first control point offset) of the first section.
        if( NUM_WAYPOINTS > 1 && i == NUM_WAYPOINTS - 1 )
        {
            // Get first bezier curve
            const MCK::BezierCurveCubic<MCK::Point<float>> &first_bez
                = line_segs[0].get_curve();

            // Copy start point
            new_point = first_bez.get_control_point( 0 );

            // Work out first control point offset of first curve
            new_control_offset
                = first_bez.get_control_point( 1 ) - new_point;
        }
        else
        {
            // Get new end point and associated control point
            new_point.set_x(
                X_MIN + float( rand() % X_SPAN )
            );
            new_point.set_y(
                Y_MIN + float( rand() % Y_SPAN )
            );

            float new_angle = float( ( rand() & 256 ) / 256 * MCK_TWO_PI );
            current_control_offset.set_x(
                float( sin( new_angle ) * OFFSET_MAG )
            );
            current_control_offset.set_y(
                float( cos( new_angle ) * OFFSET_MAG )
            );
            new_control_offset.set_x(
                float( sin( new_angle ) * OFFSET_MAG )
            );
            new_control_offset.set_y(
                float( cos( new_angle ) * OFFSET_MAG )
            );
        }

        // Create new bezier curve
        MCK::BezierCurveCubic<MCK::Point<float>> bez;
        try
        {
            bez.init(
                current_point,
                current_point + current_control_offset,
                new_point - new_control_offset,
                new_point
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create cubic Bezier, error: ")
                + e.what() ) );
        }
        std::cout << "Successfully initialized bezier curve "
                  << i << std::endl;

        // Create new line segment
        line_segs.push_back(
            MCK::LineSegmentFixed<
                MCK::BezierCurveCubic,
                MCK::Point<float>
            >( bez )
        );
        
        // Initialize line segment
        try
        {
            line_segs.back().init(
                1.0f  // Distance step 1 pixel
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to init line segment, error: ")
                + e.what() ) );
        }
        std::cout << "Successfully initialized line segment "
                  << i << std::endl;

        current_point = new_point;
        current_control_offset = new_control_offset;
    }

    const MCK::Point<float> START_POINT
        = line_segs.front().get_curve().get_point( 0.0f );


    /////////////////////////////////////////////
    // Create animation info
    std::shared_ptr<MCK::GameEngRenderBlock> prime_render_block
        = game_eng.get_prime_render_block();
    struct Ball
    {
        std::shared_ptr<MCK::GameEngRenderInfo> render_info;
        size_t seg_index;
        const MCK::LineSegmentFixed<
            MCK::BezierCurveCubic,
            MCK::Point<float>
        >* seg;
        float dist;  // Distance along line segment
    };
    std::vector<Ball> balls;
    balls.reserve( END_NUM_BALLS );
    for( int i = 0; i < START_NUM_BALLS; i++ )
    {
        balls.push_back( Ball() );
        Ball &b = balls.back();

        // Calculate starting segment
        // and distance within segment
        b.dist = i * BALL_DIST_SEP;
        b.seg_index = 0;
        b.seg = &line_segs[0];
        while( b.dist >= b.seg->get_length() )
        {
            b.dist -= b.seg->get_length();
            if( ++b.seg_index >= line_segs.size() )
            {
                b.seg_index = 0; 
            }
            b.seg = &line_segs[b.seg_index];
        }
        
        // Get starting coords
        MCK::Point<float> p;
        try
        { 
            p = b.seg->get_point_by_arc_len( b.dist );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to get point on line seg, error: ")
                + std::to_string( b.seg_index )
                + std::string( " error: ")
                + e.what() ) );
        }

        MCK_PAL_ID_TYPE palette_id;
        switch( i % 3 )
        {
            case 0:
                palette_id = palette_A_id;
                break;
            
            case 1:
                palette_id = palette_B_id;
                break;
            
            default:
                palette_id = palette_C_id;
        }

        // Create render info, and set x,y coords
        try
        {
            b.render_info = image_man.create_render_info(
                circle_image_id,
                palette_id,
                0,
                0,
                MAX_CIRCLE_SIZE,  //  width_in_pixels,
                MAX_CIRCLE_SIZE,  //  height_in_pixels,
                game_eng.get_prime_render_block()
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create render info, error: ")
                + e.what() ) );
        }
        
        // Set ball's position and size
        try
        {
            set_image_pos_and_size(
                b.render_info,
                p,
                prime_render_block
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to set image pos and size, error: ")
                + e.what() ) );
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

        ////////////////////////////////////////
        // Update FPS
        if( frame_num % 50 == 0 )
        {
            const float TICKS_PASSED_F
                = current_ticks - ticks_at_last_fps_update;
            ticks_at_last_fps_update
                = current_ticks;

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
        // Add new balls, intermitently
        if( ( current_ticks & 0x0600 ) != 0 )
        {
            const MCK::Point<float> LAST_BALL( 
                balls.back().render_info->dest_rect.get_x(),
                balls.back().render_info->dest_rect.get_y()
            );

            if( balls.size() < END_NUM_BALLS 
                && MCK::Point<float>::dist_sq( START_POINT, LAST_BALL )
                    > BALL_DIST_SEP * BALL_DIST_SEP
            )
            {
                balls.push_back( Ball() );
                Ball &b = balls.back();

                b.seg_index = 0;
                b.seg = &line_segs[0];

                MCK_PAL_ID_TYPE palette_id;
                switch( balls.size() % 3 )
                {
                    case 0:
                        palette_id = palette_A_id;
                        break;
                    
                    case 1:
                        palette_id = palette_B_id;
                        break;
                    
                    default:
                        palette_id = palette_C_id;
                }

                // Create render info, and set x,y coords
                bool error = false;
                try
                {
                    b.render_info = image_man.create_render_info(
                        circle_image_id,
                        palette_id,
                        START_POINT.get_x(),
                        START_POINT.get_y(),
                        MAX_CIRCLE_SIZE,  //  width_in_pixels,
                        MAX_CIRCLE_SIZE,  //  height_in_pixels,
                        game_eng.get_prime_render_block()
                    );
                }
                catch( std::exception &e )
                {
                    std::cout << "Failed to create render info, error: "
                              << e.what() << std::endl;
                    error = true;
                }

                if( !error )
                {
                    try
                    {
                        count_text->set_content(
                            "sprites: " + std::to_string( balls.size() )
                        );
                    }
                    catch( std::exception &e )
                    {
                        std::cout << "Failed to update count box, error: "
                                  << e.what() << std::endl;
                    }
                }
            }
        }

        ////////////////////////////////////////
        // Animate balls
        const uint32_t TICKS_SINCE_LAST_ANIM
            = current_ticks - ticks_at_last_animation;
        ticks_at_last_animation = current_ticks;
        if( TICKS_SINCE_LAST_ANIM > 0 )
        {
            const float DIST_INC
                = BALL_VEL 
                    * float( TICKS_SINCE_LAST_ANIM ) / 1000.0f;
            for( Ball &b : balls )
            {
                // Increment distance of ball, in proportion 
                // to time.
                b.dist += DIST_INC;

                // Calculate starting segment
                // and distance within segment
                while( b.dist >= b.seg->get_length() )
                {
                    b.dist -= b.seg->get_length();
                    if( ++b.seg_index >= line_segs.size() )
                    {
                        b.seg_index = 0; 
                    }
                    b.seg = &line_segs[b.seg_index];
                }
                
                // Get current coords
                MCK::Point<float> p;
                try
                { 
                    p = b.seg->get_point_by_arc_len( b.dist );
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
                        std::string( "Failed to get point on line seg, error: ")
                        + std::to_string( b.seg_index )
                        + std::string( " error: ")
                        + e.what() ) );
                }

                // Set ball's position and size
                try
                {
                    set_image_pos_and_size(
                        b.render_info,
                        p,
                        prime_render_block
                    );
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
                        std::string( "Failed to set image pos and size, error: ")
                        + e.what() ) );
                }
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
