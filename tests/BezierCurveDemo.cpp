///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
//  BezierCurveDemo.cpp
//
//  Demonstration/exploration of motion
//  governed by Bezier curves
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

///////////////////////////////////////////
// DEMO PARAMETERS
// Made global for convenience, 
// as this is only a short demo.
//
const int WINDOW_WIDTH_IN_PIXELS = 640;
const int WINDOW_HEIGHT_IN_PIXELS = 360;
const int UNSCALED_GRID_SIZE_IN_PIXELS = 16;
const int UNSCALED_CIRCLE_SIZE_IN_PIXELS = 16;
const int UNSCALED_SQUARE_SIZE_IN_PIXELS = 16;
const int GRID_SCALE = 2;
const int CIRCLE_SCALE = 1;
const int SQUARE_SCALE = 1;
const int SCALED_GRID_SIZE_IN_PIXELS =
            UNSCALED_GRID_SIZE_IN_PIXELS * GRID_SCALE;
const int SCALED_CIRCLE_SIZE_IN_PIXELS =
            UNSCALED_CIRCLE_SIZE_IN_PIXELS * CIRCLE_SCALE;
const int SCALED_SQUARE_SIZE_IN_PIXELS =
            UNSCALED_SQUARE_SIZE_IN_PIXELS * SQUARE_SCALE;
const int SQUARES_WIDE = WINDOW_WIDTH_IN_PIXELS
                            / SCALED_GRID_SIZE_IN_PIXELS;
const int SQUARES_HIGH = WINDOW_HEIGHT_IN_PIXELS
                            / SCALED_GRID_SIZE_IN_PIXELS;
const uint8_t BG_COL = MCK::COL_ROTTING_PURPLE;
const uint8_t GRID_COL = MCK::COL_MID_GRAY;
const uint8_t CHAR_WIDTH_IN_PIXELS = 8;
const uint8_t CHAR_HEIGHT_IN_PIXELS = 14;
const uint8_t CHAR_SPACING_IN_PIXELS = 0;
const uint8_t TEXT_CHARS = 16;

/////////////////////////////////////////////////////////
// STRUCT FOR WAYPOINTS
struct Waypoint
{
    int id;
    float x;
    float y;
    float z;
    float dx;
    float dy;
    float dz;
    uint32_t ticks;
    std::shared_ptr<MCK::GameEngRenderBlock> block;
    std::shared_ptr<MCK::GameEngRenderInfo> marker;
    std::vector<std::shared_ptr<MCK::ImageText>> text;

    Waypoint( void )
    {
        id = -1;
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        dx = 0.0f;
        dy = 0.0f;
        dz = 0.0f;
        ticks = 0;
    }

    Waypoint(
        int _id,
        float _x,
        float _y,
        float _z,
        float _dx,
        float _dy,
        float _dz,
        uint32_t _ticks
    )
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
        dx = _dx;
        dy = _dy;
        dz = _dz;
        ticks = _ticks;
    }
};

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
            "BezierCurveTest",  // Window name
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
    // SET BACKGROUND COLO(U)R (BEST EFFORT)
    game_eng.set_clearing_color( BG_COL );


    ///////////////////////////////////////////
    // CREATE LOCAL COLO(U)R PALETTES
    // (subsets of the core 32 colo(u)r palette)
   
    // Note: When ImageMan is used, palettes should
    //       be registered with ImageMan

    // Palette for text
    std::shared_ptr<std::vector<uint8_t>> TEXT_PALETTE =
        std::make_shared<std::vector<uint8_t>>(
            std::vector<uint8_t>{
                MCK::COL_BLACK,
                MCK::COL_WHITE
            }
        );
    const MCK_PAL_ID_TYPE TEXT_PALETTE_ID 
        = image_man.create_local_palette( TEXT_PALETTE );

    // Palette for grid
    std::shared_ptr<std::vector<uint8_t>> GRID_PALETTE =
        std::make_shared<std::vector<uint8_t>>(
            std::vector<uint8_t>{
                MCK::COL_TRANSPARENT,
                GRID_COL
            }
        );
    const MCK_PAL_ID_TYPE GRID_PALETTE_ID
        = image_man.create_local_palette( GRID_PALETTE );

    // Palette for red objects
    std::shared_ptr<std::vector<uint8_t>> RED_PALETTE =
        std::make_shared<std::vector<uint8_t>>(
            std::vector<uint8_t>{
                MCK::COL_TRANSPARENT,
                MCK::COL_RED
            }
        );
    const MCK_PAL_ID_TYPE RED_PALETTE_ID
        = image_man.create_local_palette( RED_PALETTE );

    // Palette for blue objects
    std::shared_ptr<std::vector<uint8_t>> BLUE_PALETTE =
        std::make_shared<std::vector<uint8_t>>(
            std::vector<uint8_t>{
                MCK::COL_TRANSPARENT,
                MCK::COL_BLUE
            }
        );
    const MCK_PAL_ID_TYPE BLUE_PALETTE_ID
        = image_man.create_local_palette( BLUE_PALETTE );


    ///////////////////////////////////////////
    // CREATE IMAGE DATA

    // Grid square, must be
    // GRID_SIZE_IN_PIXELS high and wide
    const MCK_IMG_ID_TYPE GRID_IMAGE_ID = 0;
    const std::vector<uint8_t> GRID_IMAGE =
    {
        0b00000000, 0b00000001,
        0b00000000, 0b00000001, 
        0b00000000, 0b00000001, 
        0b00000000, 0b00000001, 
        0b00000000, 0b00000001, 
        0b00000000, 0b00000001, 
        0b00000000, 0b00000001,
        0b00000000, 0b00000001,
        0b00000000, 0b00000001,
        0b00000000, 0b00000001, 
        0b00000000, 0b00000001, 
        0b00000000, 0b00000001, 
        0b00000000, 0b00000001, 
        0b00000000, 0b00000001, 
        0b00000000, 0b00000001,
        0b11111111, 0b11111111
    };

    // Square
    const MCK_IMG_ID_TYPE SQUARE_IMAGE_ID = 1;
    const std::vector<uint8_t> SQUARE_IMAGE =
    {
        0b11111111, 0b11111111,
        0b11111111, 0b11111111, 
        0b11111111, 0b11111111, 
        0b11100000, 0b00000111, 
        0b11100000, 0b00000111, 
        0b11100000, 0b00000111, 
        0b11100000, 0b00000111,
        0b11100000, 0b00000111,
        0b11100000, 0b00000111,
        0b11100000, 0b00000111, 
        0b11100000, 0b00000111, 
        0b11100000, 0b00000111, 
        0b11100000, 0b00000111, 
        0b11111111, 0b11111111, 
        0b11111111, 0b11111111,
        0b11111111, 0b11111111
    };

    // Circle
    const MCK_IMG_ID_TYPE CIRCLE_IMAGE_ID = 2;
    const std::vector<uint8_t> CIRCLE_IMAGE =
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


    ///////////////////////////////////////////
    // CREATE TEXTURES

    // Grid texture
    MCK_TEX_ID_TYPE grid_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                GRID_IMAGE_ID,
                GRID_PALETTE_ID,
                1,  // bits_per_pixel,
                UNSCALED_GRID_SIZE_IN_PIXELS,  // pitch_in_pixels,
                GRID_IMAGE,
                *GRID_PALETTE,
                grid_tex_id,
                height_in_pixels,
                true  // throw if exists
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create grid texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != UNSCALED_GRID_SIZE_IN_PIXELS )
        {
            throw( std::runtime_error(
                std::string( "Grid texture has incorrect height")
            ) );
        }
    }

    // Red circle texture
    MCK_TEX_ID_TYPE red_circle_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                CIRCLE_IMAGE_ID,
                RED_PALETTE_ID,
                1,  // bits_per_pixel,
                UNSCALED_GRID_SIZE_IN_PIXELS,  // pitch_in_pixels,
                CIRCLE_IMAGE,
                *RED_PALETTE,
                red_circle_tex_id,
                height_in_pixels,
                true  // throw if exists
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create red circle texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != UNSCALED_GRID_SIZE_IN_PIXELS )
        {
            throw( std::runtime_error(
                std::string( "Red circle texture has incorrect height")
            ) );
        }
    }

    // Blue circle texture
    MCK_TEX_ID_TYPE blue_circle_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                CIRCLE_IMAGE_ID,
                BLUE_PALETTE_ID,
                1,  // bits_per_pixel,
                UNSCALED_GRID_SIZE_IN_PIXELS,  // pitch_in_pixels,
                CIRCLE_IMAGE,
                *BLUE_PALETTE,
                blue_circle_tex_id,
                height_in_pixels,
                true  // throw if exists
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create blue circle texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != UNSCALED_GRID_SIZE_IN_PIXELS )
        {
            throw( std::runtime_error(
                std::string( "Blue circle texture has incorrect height")
            ) );
        }
    }

    // Red square texture
    MCK_TEX_ID_TYPE red_square_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                SQUARE_IMAGE_ID,
                RED_PALETTE_ID,
                1,  // bits_per_pixel,
                UNSCALED_GRID_SIZE_IN_PIXELS,  // pitch_in_pixels,
                SQUARE_IMAGE,
                *RED_PALETTE,
                red_square_tex_id,
                height_in_pixels,
                true  // throw if exists
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create red square texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != UNSCALED_GRID_SIZE_IN_PIXELS )
        {
            throw( std::runtime_error(
                std::string( "Red square texture has incorrect height")
            ) );
        }
    }

    // Blue square texture
    MCK_TEX_ID_TYPE blue_square_tex_id;
    {
        uint16_t height_in_pixels;
        try
        {
            game_eng.create_texture(
                SQUARE_IMAGE_ID,
                BLUE_PALETTE_ID,
                1,  // bits_per_pixel,
                UNSCALED_GRID_SIZE_IN_PIXELS,  // pitch_in_pixels,
                SQUARE_IMAGE,
                *BLUE_PALETTE,
                blue_square_tex_id,
                height_in_pixels,
                true  // throw if exists
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create blue square texture, error: ")
                + e.what() ) );
        }

        if( height_in_pixels != UNSCALED_GRID_SIZE_IN_PIXELS )
        {
            throw( std::runtime_error(
                std::string( "Blue square texture has incorrect height")
            ) );
        }
    }


    //////////////////////////////////////////////////////
    // CREATE WAYPOINTS (ID, POSITION AND TICKS ONLY)
    std::vector<Waypoint> waypoints;
    waypoints.reserve( 4 );
    waypoints.push_back( Waypoint(
            0,  // ID
            SCALED_GRID_SIZE_IN_PIXELS * 2,
            SCALED_GRID_SIZE_IN_PIXELS * 6,
            0,  // No z dimenion for now
            0,  // No hoz velocity
            -8,  // Vert velocity 8 pixels per sec upwards
            0,  // No z dimenion for now
            0   // ticks
        ) 
    );
    waypoints.push_back( Waypoint(
            1,  // ID
            SCALED_GRID_SIZE_IN_PIXELS * 8,
            SCALED_GRID_SIZE_IN_PIXELS * 1,
            0,  // No z dimenion for now
            8,  // Hoz velocity 8 pixels per sec right
            0,  // No vert velocity
            0,  // No z dimenion for now
            0   // ticks
        ) 
    );
    waypoints.push_back( Waypoint(
            2,  // ID
            SCALED_GRID_SIZE_IN_PIXELS * 14,
            SCALED_GRID_SIZE_IN_PIXELS * 3,
            0,  // No z dimenion for now
            -8,  // Hoz velocity 8 pixels per sec left
            0,  // No vert velocity
            0,  // No z dimenion for now
            0   // ticks
        ) 
    );
    waypoints.push_back( Waypoint(
            3,  // ID
            SCALED_GRID_SIZE_IN_PIXELS * 12,
            SCALED_GRID_SIZE_IN_PIXELS * 8,
            0,  // No z dimenion for now
            8,  // Hoz velocity 8 pixels per sec right
            0,  // No vert velocity
            0,  // No z dimenion for now
            0   // ticks
        ) 
    );


    ///////////////////////////////////////////
    // CREATE RENDER BLOCKS
    std::shared_ptr<MCK::GameEngRenderBlock> grid_block;
    std::shared_ptr<MCK::GameEngRenderBlock> waypoint_block;
    try
    {
        grid_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block(),
            MCK::MIN_Z_VALUE  // Render behind everything else
        );
        waypoint_block = game_eng.create_empty_render_block(
            game_eng.get_prime_render_block(),
            MCK::DEFAULT_Z_VALUE - 1
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create render blocks, error: ")
            + e.what() ) );
    }
    

    ///////////////////////////////////////////
    // CREATE RENDER INFO
    
    // Create grid
    for( int i = 0; i < SQUARES_WIDE; i++ )
    {
        for( int j = 0; j < SQUARES_HIGH; j++ )
        {
            try
            {
                game_eng.create_render_info(
                    grid_tex_id,
                    grid_block,
                    MCK::GameEngRenderInfo::Rect(
                        i * SCALED_GRID_SIZE_IN_PIXELS,
                        j * SCALED_GRID_SIZE_IN_PIXELS,
                        SCALED_GRID_SIZE_IN_PIXELS,
                        SCALED_GRID_SIZE_IN_PIXELS
                    ),
                    false,  // No clip
                    MCK::GameEngRenderInfo::Rect(),
                    0,  // No rotation
                    false,  // No flip x
                    false,  // No flip y
                    MCK::DEFAULT_Z_VALUE
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to create grid (" )
                    + std::to_string( i )
                    + std::string( ",")
                    + std::to_string( j )
                    + std::string( ") error: ")
                    + e.what() ) );
            }
        }
    }

    // Create render block, render info and text for waypoints
    for( Waypoint &wp : waypoints )
    {
        // Create block (best effort, not likely to fail)
        wp.block = game_eng.create_empty_render_block(
                waypoint_block,
                MCK::DEFAULT_Z_VALUE
            );

        // Create render info
        try
        {
            wp.marker = game_eng.create_render_info(
                red_circle_tex_id,
                wp.block,
                MCK::GameEngRenderInfo::Rect(
                    wp.x - SCALED_CIRCLE_SIZE_IN_PIXELS / 2,
                    wp.y - SCALED_CIRCLE_SIZE_IN_PIXELS / 2,
                    SCALED_CIRCLE_SIZE_IN_PIXELS,
                    SCALED_CIRCLE_SIZE_IN_PIXELS
                ),
                false,  // No clip
                MCK::GameEngRenderInfo::Rect(),
                0,  // No rotation
                false,  // No flip x
                false,  // No flip y
                MCK::DEFAULT_Z_VALUE
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create waypoint render info" )
                + std::to_string( wp.id )
                + std::string( ", error: ")
                + e.what() ) );
        }
    
        // Create text
        for( int i = 0; i < 5; i++ )
        {
            std::string s;
            switch( i )
            {
                case 0:
                    s = "x=" + std::to_string( wp.x );
                    break;

                case 1:
                    s = "y=" + std::to_string( wp.y );
                    break;

                case 2:
                    s = "dx=" + std::to_string( wp.dx );
                    break;

                case 3:
                    s = "dy=" + std::to_string( wp.dy );
                    break;

                case 4:
                    s = "ticks=" + std::to_string( wp.ticks );
                    break;
            }
            
            wp.text.push_back( std::make_shared<MCK::ImageText>() );

            try
            {
                wp.text.back()->init(
                    game_eng,
                    image_man,
                    wp.block,
                    TEXT_PALETTE_ID,
                    wp.x + SCALED_CIRCLE_SIZE_IN_PIXELS,
                    wp.y + float( i - 0.5f )
                                * SCALED_CIRCLE_SIZE_IN_PIXELS,
                    TEXT_CHARS,  // Size in chars
                    CHAR_WIDTH_IN_PIXELS,
                    CHAR_HEIGHT_IN_PIXELS,
                    s,
                    MCK::ImageText::LEFT,  // Justification
                    CHAR_SPACING_IN_PIXELS,
                    0,  // Default ascii set
                    MCK::DEFAULT_Z_VALUE - 1
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to create waypoint text" )
                    + std::string( ", error: ")
                    + e.what() ) );
            }
        }
    }

    // Test Square
    try
    {
        game_eng.create_render_info(
            blue_square_tex_id,
            game_eng.get_prime_render_block(),
            MCK::GameEngRenderInfo::Rect(
                150,
                100,
                SCALED_SQUARE_SIZE_IN_PIXELS,
                SCALED_SQUARE_SIZE_IN_PIXELS
            ),
            false,  // No clip
            MCK::GameEngRenderInfo::Rect(),
            0,  // No rotation
            false,  // No flip x
            false,  // No flip y
            MCK::DEFAULT_Z_VALUE
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create square, error = " )
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
