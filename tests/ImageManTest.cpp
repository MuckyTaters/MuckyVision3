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
#include "FancyTypeface.h"

// Calculate vertical pos of text
int calc_vert_offset(
    uint32_t ticks,
    float hoz_pos,  // 0.0 to 1.0
    int vert_range
)
{
    const float PERIOD
        = float( ticks ) / 1000.0f + hoz_pos;
    return int(
               float( 
                   sin( PERIOD * 3.14127f * 2.0f ) * vert_range
               ) + 0.5f
           ); 
}


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
            "ImageManTest",  // Window name
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
    // CREATE DEMO PARAMETERS
    const int NUM_ASCII_RENDER_BLOCKS = 16;
    const int VERT_RANGE = 5;
    const int NUM_ROWS = 8;
    const int NUM_COLS = 256 / NUM_ROWS;
    const int TEXT_H_SCALE = 2;
    const int TEXT_V_SCALE = 3;

    ///////////////////////////////////////////
    // CREATE RENDER BLOCKS

    std::shared_ptr<MCK::GameEngRenderBlock> logo_block;
    try
    {
        logo_block =
            game_eng.create_empty_render_block(
                game_eng.get_prime_render_block()
            );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create logo block, error: ")
            + e.what() ) );
    }

    std::vector<std::shared_ptr<MCK::GameEngRenderBlock>> ascii_blocks;
    for( int i = 0; i < NUM_ASCII_RENDER_BLOCKS; i++ )
    {
        try
        {
            ascii_blocks.push_back(
                game_eng.create_empty_render_block(
                    game_eng.get_prime_render_block()
                )
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create render block, error: ")
                + e.what() ) );
        }

        // Set block's vertical position
        ascii_blocks[i]->vert_offset
            = calc_vert_offset(
                0, 
                float( i ) / float( NUM_ASCII_RENDER_BLOCKS ),
                VERT_RANGE
            );
    }

    
    ///////////////////////////////////////////
    // CREATE LOCAL PALETTES
    MCK_PAL_ID_TYPE black_yellow_palette_id;
    try
    {
         black_yellow_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_BLACK,
                    MCK::COL_YELLOW
                }
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create black yellow palette, error: ")
            + e.what() ) );
    }
    
    MCK_PAL_ID_TYPE black_red_palette_id;
    try
    {
         black_red_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_BLACK,
                    MCK::COL_RED
                }
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create black red palette, error: ")
            + e.what() ) );
    }
    
    MCK_PAL_ID_TYPE black_green_palette_id;
    try
    {
         black_green_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_BLACK,
                    MCK::COL_GREEN
                }
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create black green palette, error: ")
            + e.what() ) );
    }
    
    MCK_PAL_ID_TYPE logo_palette_id;
    try
    {
         logo_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_DARK_RED
                }
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create logo palette, error: ")
            + e.what() ) );
    }
    
    MCK_PAL_ID_TYPE fancy_palette_id;
    try
    {
         fancy_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_BLACK,
                    MCK::COL_CYAN,
                    MCK::COL_SAND
                }
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create black green palette, error: ")
            + e.what() ) );
    }
    

    /////////////////////////////////////////////
    // Create custom image(s)
    MCK_IMG_ID_TYPE fancy_A_image_id;
    {
        try
        {
            fancy_A_image_id = image_man.create_custom_image(
                std::make_shared<const std::vector<uint8_t>>(
                    MCK::FancyTypeface::image_data[0]
                ),
                2,  // bits_per_pixel,
                12,  // pitch_in_pixels,
                12  // height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create fancy 'A', error: ")
                + e.what() ) );
        }
    }

    ///////////////////////////////////////////
    // CREATE RENDER INFO

    // Create 256 extended ASCII chars
    int block_count = 0;
    for( int row = 0; row < NUM_ROWS; row++ )
    {
        for( int c = 0; c < NUM_COLS; c++ )
        {
            const uint8_t CHAR_ID = row * 32 + c;

            // Determine which block to assign this
            // image to
            const int BLOCK_ID =
                ( block_count++ 
                    + ( row % 2 ) 
                        * NUM_ASCII_RENDER_BLOCKS / 2
                ) % NUM_ASCII_RENDER_BLOCKS;

            // Calculate position within block
            const int X_POS = 36 + 18 * c;
            const int Y_POS = 20 + row * VERT_RANGE * 8;

            MCK_PAL_ID_TYPE palette_id;
            switch( row % 4 )
            {
                case 0:
                    palette_id = black_yellow_palette_id;
                    break;

                case 1:
                    palette_id = black_red_palette_id;
                    break;

                case 2:
                    palette_id = black_green_palette_id;
                    break;

                case 3:
                    palette_id = black_red_palette_id;
                    break;
            }

            // Create image
            try
            {
                image_man.create_extended_ascii_render_info(
                    CHAR_ID,
                    palette_id,
                    X_POS,
                    Y_POS,
                    TEXT_H_SCALE,
                    TEXT_V_SCALE,
                    ascii_blocks[ BLOCK_ID ]
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to create ascii image, error: ")
                    + e.what() ) );
            }
        }
    }

    // Create logo as string
    {
        const int H_SCALE = 2;
        const int V_SCALE = 2;
        const uint8_t COPYRIGHT_SYMBOL = 0xFF;
        std::string logo_str;
        logo_str += COPYRIGHT_SYMBOL;
        logo_str += " MUCKYTATERS 2023";
        const int LOGO_SIZE = logo_str.size();

        // Loop over chars in logo string, creating an ASCII image
        // for each char and assigning it to the logo render block
        int x_pos = WINDOW_WIDTH_IN_PIXELS
                        - logo_str.size() * 8 * H_SCALE;
        for( uint8_t c : logo_str )
        {
            try
            {
                image_man.create_extended_ascii_render_info(
                    c,
                    logo_palette_id,
                    x_pos,
                    WINDOW_HEIGHT_IN_PIXELS - 8 * V_SCALE,
                    H_SCALE,
                    V_SCALE,
                    logo_block
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to create logo ascii image, error: ")
                    + e.what() ) );
            }

            x_pos += 8 * H_SCALE;
        }
    }

    // Overwrite 'A', i.e. first render info item in 3rd ASCII block
    try
    {
        image_man.change_render_info_tex(
            ascii_blocks[1]->render_info[4], 
            fancy_A_image_id,
            fancy_palette_id,
            true  // keep_orig_dest_rect_size
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to change 'A', error: ")
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

        for( int i = 0; i < NUM_ASCII_RENDER_BLOCKS; i++ )
        {
            ascii_blocks[i]->vert_offset
                = calc_vert_offset(
                    current_ticks, 
                    float( i ) / float( NUM_ASCII_RENDER_BLOCKS ),
                    VERT_RANGE
                );
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

