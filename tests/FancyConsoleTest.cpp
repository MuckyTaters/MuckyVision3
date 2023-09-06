///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
//  FancyConsoleTest.cpp
//
//  Test/demo emo for Console and associated classes,
//  with fancy typeface.
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

#include "FancyTypeface.h"
#include "GameEng.h"
#include "ImageMan.h"
#include "ImageText.h"
#include "Console.h"


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
            "ConsoleTest",  // Window name
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
    MCK_PAL_ID_TYPE text_palette_id;
    MCK_PAL_ID_TYPE lscape_console_palette_id;
    const uint8_t MINI_CONSOLE_BG_COL = MCK::COL_FOREST_GREEN;
    try
    {
        text_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_SAND,
                    MCK::COL_BLACK,
                    MCK::COL_CYAN
                }
            )
        );
        lscape_console_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_TRANSPARENT,
                    MCK::COL_DARK_GRAY,
                    MCK::COL_DARK_BLUE,
                    MCK::COL_PINK
                }
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create black white palette, error: ")
            + e.what() ) );
    }

    ///////////////////////////////////////////////////
    // CREATE FANCY TYPEFACE - UPPER CASE CHARACTERS
    uint8_t fancy_ascii_set;
    std::vector<MCK_IMG_ID_TYPE> fancy_ascii_to_image_id_mapping(
        256,
        MCK::INVALID_IMG_ID
    );
    {
        // This is for upper case letters only, so we
        // start at ASCII value 65 ( 'A' )
        int ascii_code = 65;

        // Loop over image data, creating images and adding
        // image IDs to mapping
        for( auto &pixel_data : MCK::FancyTypeface::upper_case_pixel_data )
        {
            // Safety check
            if( ascii_code >= fancy_ascii_to_image_id_mapping.size() )
            {
                break;
            }

            // Create image
            MCK_IMG_ID_TYPE char_image_id;
            try
            {
                char_image_id = image_man.create_custom_image(
                    &pixel_data,
                    MCK::FancyTypeface::BITS_PER_PIXEL,
                    MCK::FancyTypeface::PITCH_IN_PIXELS,
                    MCK::FancyTypeface::HEIGHT_IN_PIXELS
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to create fancy ASCII character " )
                    + std::to_string( ascii_code )
                    + std::string( ", error = " )
                    + e.what() ) );
            }

            // Add image id to mapping
            fancy_ascii_to_image_id_mapping[ ascii_code++ ] = char_image_id;
        }
    }
    
    ///////////////////////////////////////////////////
    // CREATE FANCY TYPEFACE - NUMERAL CHARACTERS
    {
        // This is for numerals only, so we
        // start at ASCII value 48 ( '0' )
        int ascii_code = 48;

        // Loop over image data, creating images and adding
        // image IDs to mapping
        for( auto &pixel_data : MCK::FancyTypeface::numeral_pixel_data )
        {
            // Safety check
            if( ascii_code >= fancy_ascii_to_image_id_mapping.size() )
            {
                break;
            }

            // Create image
            MCK_IMG_ID_TYPE char_image_id;
            try
            {
                char_image_id = image_man.create_custom_image(
                    &pixel_data,
                    MCK::FancyTypeface::BITS_PER_PIXEL,
                    MCK::FancyTypeface::PITCH_IN_PIXELS,
                    MCK::FancyTypeface::HEIGHT_IN_PIXELS
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to create fancy ASCII character " )
                    + std::to_string( ascii_code )
                    + std::string( ", error = " )
                    + e.what() ) );
            }

            // Add image id to mapping
            fancy_ascii_to_image_id_mapping[ ascii_code++ ] = char_image_id;
        }
    }
    
    ///////////////////////////////////////////
    // CREATE ADDIITONAL FANCY CHARACTERS
    {
        // MISC GRAPHICS (ASCII 0 - 4)
        {
            int ascii_code = 0;

            // Loop over image data, creating images and adding
            // image IDs to mapping
            for( auto &pixel_data : MCK::FancyTypeface::misc_pixel_data )
            {
                // Safety check
                if( ascii_code >= fancy_ascii_to_image_id_mapping.size() )
                {
                    break;
                }

                // Create image
                MCK_IMG_ID_TYPE char_image_id;
                try
                {
                    char_image_id = image_man.create_custom_image(
                        &pixel_data,
                        MCK::FancyTypeface::BITS_PER_PIXEL,
                        MCK::FancyTypeface::PITCH_IN_PIXELS,
                        MCK::FancyTypeface::HEIGHT_IN_PIXELS
                    );
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
                        std::string( "Failed to create fancy ASCII character " )
                        + std::to_string( ascii_code )
                        + std::string( ", error = " )
                        + e.what() ) );
                }

                // Add image id to mapping
                fancy_ascii_to_image_id_mapping[ ascii_code++ ] = char_image_id;
            }
        }

        // PERIOD / FULL STOP (ASCII 46)
        {
            // Create period (full stop) image
            MCK_IMG_ID_TYPE period_image_id;
            try
            {
                period_image_id = image_man.create_custom_image(
                    &MCK::FancyTypeface::period_pixel_data,
                    MCK::FancyTypeface::BITS_PER_PIXEL,
                    MCK::FancyTypeface::PITCH_IN_PIXELS,
                    MCK::FancyTypeface::HEIGHT_IN_PIXELS
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to create fancy period " )
                    + std::string( "image, error = " )
                    + e.what() ) );
            }

            // Add image id to mapping
            // Note: ascii 46 is period (full stop)
            fancy_ascii_to_image_id_mapping[ 46 ] = period_image_id;
        }

        // COPYRIGHT SYMBOL (ASCII 255 IN MUCKYVISION)
        {
            // Create copyright symbol image
            MCK_IMG_ID_TYPE cr_image_id;
            try
            {
                cr_image_id = image_man.create_custom_image(
                    &MCK::FancyTypeface::copyright_symbol_pixel_data,
                    MCK::FancyTypeface::BITS_PER_PIXEL,
                    MCK::FancyTypeface::PITCH_IN_PIXELS,
                    MCK::FancyTypeface::HEIGHT_IN_PIXELS
                );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to create fancy copyright " )
                    + std::string( "symbol image, error = " )
                    + e.what() ) );
            }

            // Add image id to mapping
            // Note: ascii 255 is copyright symbol in MuckyVision
            fancy_ascii_to_image_id_mapping[ 255 ] = cr_image_id;
        }
    }

    ////////////////////////////////////////////////////////
    // CREATE CUSTOM ASCII MAPPING FOR FANCY CHARACTERS
    {
        // Create alternate ASCII set
        try
        {
            fancy_ascii_set = image_man.create_alt_ascii_set(
                fancy_ascii_to_image_id_mapping
            );
                
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create alternate " )
                + std::string( "ascii mapping, error: " )
                + e.what() ) );
        }
    }

    ///////////////////////////////////////////
    // CREATE MAIN CONSOLE PARAMETERS
    const uint8_t BG_COL = MCK::COL_PASTEL_BLUE;
   
    // General
    const uint8_t TILE_WIDTH = 16;
    const uint8_t TILE_HEIGHT = 16;
    
    // Main console
    const uint8_t MAIN_CHAR_WIDTH = 12;
    const uint8_t MAIN_CHAR_HEIGHT = 24;
    const uint16_t MAIN_CONSOLE_LEFT = TILE_WIDTH * 1;
    const uint16_t MAIN_CONSOLE_TOP = TILE_HEIGHT * 3.5;
    const uint8_t MAIN_CONSOLE_WIDTH_IN_CHARS = 30;
    const uint8_t MAIN_CONSOLE_HEIGHT_IN_CHARS = 6;
    const uint8_t MAIN_CONSOLE_CHAR_SPACING = 2;
    const uint8_t MAIN_CONSOLE_LINE_SPACING = 4;

    // Mini console
    const uint8_t MINI_CHAR_WIDTH = 12;
    const uint8_t MINI_CHAR_HEIGHT = 12;
    const uint16_t MINI_CONSOLE_LEFT = TILE_WIDTH * 28;
    const uint16_t MINI_CONSOLE_TOP = TILE_HEIGHT * 3;
    const uint8_t MINI_CONSOLE_WIDTH_IN_CHARS = 10;
    const uint8_t MINI_CONSOLE_HEIGHT_IN_CHARS = 6;
    const uint8_t MINI_CONSOLE_CHAR_SPACING = 1;
    const uint8_t MINI_CONSOLE_LINE_SPACING = 0;

    ////////////////////////////////////////////
    // SET CLEARING COLO(U)R

    // Set render clearing colo(u)r for this demo
    try
    {
        game_eng.set_clearing_color( BG_COL );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to set clearing color, error: ")
            + e.what() ) );
    }

    //////////////////////////////////////////////
    // CREATE TITLE TEXT BOX
    std::shared_ptr<MCK::Console> title_text
        = std::make_shared<MCK::Console>();
    try
    {
        std::string CR( 1, uint8_t( 255 ) );
        std::string s = "CONSOLE DEMO 2 " + CR + "MUCKYTATERS 2023";
        title_text->init(
            game_eng,
            image_man,
            game_eng.get_prime_render_block(),
            text_palette_id,
            MAIN_CONSOLE_LEFT + TILE_WIDTH,  // x_pos,
            TILE_HEIGHT,  // y_pos,
            32,  // width in chars
            1,  // height in chars
            16,  // char_width_in_pixels,
            24,  // char_height_in_pixels,
            s,
            0,  // print_speed_in_ticks_per_char,
            0,  // scroll_speed_in_ticks_per_pixel,
            true,  // hoz_text_alignment
            0, // start_line
            true,  // add_to_front_of_parent_block = true
            BG_COL,  // underlay colo(u)r
            2,  // char spacing in pixels
            0,  // line spacing in pixels
            fancy_ascii_set
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create title text, error: ")
            + e.what() ) );
    }

    //////////////////////////////////////////////
    // CREATE MESSAGE TEXTS FOR CONSOLES
    const std::string MSG_1
        = "THE QUICK BROWN FOX JUMPED OVER THE LAZY SLEEPING DOG. ";
          // "the quick brown fox jumped over the lazy sleeping dog. ";
       
    std::string msg_2;
    { 
        // Empty ASCII character
        std::string E( 1, uint8_t( 32 ) );

        // Box ASCII character
        std::string B( 1, uint8_t( 11 ) );

        /*
        // Left slope ASCII character
        std::string L( 1, uint8_t( 5 ) );

        // Peak ASCII character
        std::string P( 1, uint8_t( 6 ) );

        // Right slope ASCII character
        std::string R( 1, uint8_t( 7 ) );
        
        // Dip ASCII character
        std::string D( 1, uint8_t( 8 ) );
        
        // Full ASCII character
        std::string F( 1, uint8_t( 9 ) );
        
        // Tower ASCII character
        std::string T( 1, uint8_t( 10 ) );
        */

        msg_2 += E +"T"+ E + E + E + E + E;
        msg_2 += E +"H"+ E + E + E + E + E;
        msg_2 += E +"E"+ E + E + E + E + E;
        msg_2 += E + E + E + E + E + E + E;
        msg_2 += E + E +"Q"+ E + E + E + E;
        msg_2 += E + E +"U"+ E + E + E + E;
        msg_2 += E + E +"I"+ E + E + E + E;
        msg_2 += E + E +"C"+ E + E + E + E;
        msg_2 += E + E +"K"+ E + E + E + E;
        msg_2 += E + E + E + E + E + E + E;
        msg_2 += E + E + E + E + E +"B"+ E;
        msg_2 += E + E + E + E + E +"R"+ E;
        msg_2 += E + E + E + E + E +"O"+ E;
        msg_2 += E + E + E + E + E +"W"+ E;
        msg_2 += E + E + E + E + E +"N"+ E;
        msg_2 += E + E + E + E + E + E + E;
        msg_2 += E + E +"F"+ E + E + E + E;
        msg_2 += E + E +"O"+ E + E + E + E;
        msg_2 += E + E +"X"+ E + E + E + E;
        msg_2 += E + E + E + E + E + E + E;
        msg_2 += E + E + E +"J"+ E + E + E;
        msg_2 += E + E + E +"U"+ E + E + E;
        msg_2 += E + E + E +"M"+ E + E + E;
        msg_2 += E + E + E +"P"+ E + E + E;
        msg_2 += E + E + E +"E"+ E + E + E;
        msg_2 += E + E + E +"D"+ E + E + E;
        msg_2 += E + E + E + E + E + E + E;
        msg_2 += E +"O"+ E + E + E + E + E;
        msg_2 += E +"V"+ E + E + E + E + E;
        msg_2 += E +"E"+ E + E + E + E + E;
        msg_2 += E +"R"+ E + E + E +"T"+ E;
        msg_2 += E + E + E + E + E +"H"+ E;
        msg_2 += E + E + E + E + E +"E"+ E;
        msg_2 += E + E + E +"L"+ E + E + E;
        msg_2 += E + E + E +"A"+ E + E + E;
        msg_2 += E + E + E +"Z"+ E + E + E;
        msg_2 += E + E + E +"Y"+ E + E + E;
        msg_2 += E + E + E + E + E + E + E;
        msg_2 += E + E +"S"+ E + E + E + E;
        msg_2 += E + E +"L"+ E + E + E + E;
        msg_2 += E + E +"E"+ E + E + E + E;
        msg_2 += E + E +"E"+ E + E + E + E;
        msg_2 += E + E +"P"+ E + E + E + E;
        msg_2 += E + E +"I"+ E + E + E + E;
        msg_2 += E + E +"N"+ E + E + E + E;
        msg_2 += E + E +"G"+ E + E + E + E;
        msg_2 += E + E + E + E + E + E + E;
        msg_2 += E + E + E + E +"D"+ E + E;
        msg_2 += E + E + E + E +"O"+ E + E;
        msg_2 += E + E + E + E +"G"+ E + E;
        msg_2 += E + E + E + E + E + E + E;
        /*
        msg_2 += E + E + E + E + E + E + E;
        msg_2 += E + E + E + E + E + E + E;
        msg_2 += E + E + E + E + E + E + E;
        msg_2 += E + E + E + E + E + E + E;
        */
    }
    
    std::string msg_3;
    {
        // Left slope ASCII character
        std::string L( 1, uint8_t( 0 ) );

        // Peak ASCII character
        std::string P( 1, uint8_t( 1 ) );

        // Right slope ASCII character
        std::string R( 1, uint8_t( 2 ) );
        
        // Dip ASCII character
        std::string D( 1, uint8_t( 3 ) );
        
        // Full ASCII character
        std::string F( 1, uint8_t( 4 ) );
        
        // Empty ASCII character
        std::string E( 1, uint8_t( 32 ) );
        
        msg_3 += E + E + E + E + R + F + F;
        msg_3 += E + E + E + E + L + F + F;
        msg_3 += E + E + E + L + F + F + F;
        msg_3 += E + E + P + F + F + F + F;
        msg_3 += E + E + E + R + F + F + F;
        msg_3 += E + E + E + E + D + F + F;
        msg_3 += E + E + E + L + F + F + F;
        msg_3 += E + E + L + F + F + F + F;
        msg_3 += E + P + F + F + F + F + F;
        msg_3 += E + E + R + F + F + F + F;
        msg_3 += E + E + E + R + F + F + F;
        msg_3 += E + E + E + E + R + F + F;
        msg_3 += E + E + E + E + L + F + F;
        msg_3 += E + E + E + E + R + F + F;
        msg_3 += E + E + E + E + L + F + F;
        msg_3 += E + E + E + L + F + F + F;
        msg_3 += E + E + P + F + F + F + F;
        msg_3 += E + E + E + R + F + F + F;
        msg_3 += E + E + E + E + D + F + F;
        msg_3 += E + E + E + L + F + F + F;
        msg_3 += E + E + L + F + F + F + F;
        msg_3 += E + P + F + F + F + F + F;
        msg_3 += E + E + R + F + F + F + F;
        msg_3 += E + E + E + R + F + F + F;
        msg_3 += E + E + E + E + R + F + F;
        msg_3 += E + E + E + E + L + F + F;
        msg_3 += E + E + E + P + F + F + F;
        msg_3 += E + E + E + E + R + F + F;
        msg_3 += E + E + E + E + L + F + F;
        msg_3 += E + E + E + L + F + F + F;
        msg_3 += E + E + P + F + F + F + F;
        msg_3 += E + E + E + R + F + F + F;
        msg_3 += E + E + E + E + D + F + F;
        msg_3 += E + E + E + L + F + F + F;
        msg_3 += E + E + L + F + F + F + F;
        msg_3 += E + P + F + F + F + F + F;
        msg_3 += E + E + R + F + F + F + F;
        msg_3 += E + E + E + R + F + F + F;
        msg_3 += E + E + E + E + R + F + F;
        msg_3 += E + E + E + E + L + F + F;
        msg_3 += E + E + E + E + R + F + F;
        msg_3 += E + E + E + E + L + F + F;
        msg_3 += E + E + E + L + F + F + F;
        msg_3 += E + E + P + F + F + F + F;
        msg_3 += E + E + E + R + F + F + F;
        msg_3 += E + E + E + E + D + F + F;
        msg_3 += E + E + E + L + F + F + F;
        msg_3 += E + E + L + F + F + F + F;
        msg_3 += E + P + F + F + F + F + F;
        msg_3 += E + E + R + F + F + F + F;
        msg_3 += E + E + E + R + F + F + F;
        msg_3 += E + E + E + E + R + F + F;
        msg_3 += E + E + E + E + E + D + F;
        msg_3 += E + E + E + E + L + F + F;
        msg_3 += E + E + E + P + F + F + F;
    }                                      


    //////////////////////////////////////////////
    // CREATE MINI CONSOLE 
    std::shared_ptr<MCK::Console> mini_console
        = std::make_shared<MCK::Console>();
    try
    {
        mini_console->init(
            game_eng,
            image_man,
            game_eng.get_prime_render_block(), // mini_console_block,
            text_palette_id,
            MINI_CONSOLE_LEFT,  // x_pos,
            MINI_CONSOLE_TOP,  // y_pos,
            MINI_CONSOLE_WIDTH_IN_CHARS,
            MINI_CONSOLE_HEIGHT_IN_CHARS,
            MINI_CHAR_WIDTH,  // char_width_in_pixels,
            MINI_CHAR_HEIGHT,  // char_height_in_pixels,
            MSG_1,  // initial_content,
            100,  // print_speed_in_ticks_per_char,
            10,  // scroll_speed_in_ticks_per_pixel,
            true,  // hoz_text_alignment
            MINI_CONSOLE_HEIGHT_IN_CHARS - 1,  // start_line
            false, // true  // add_to_front_of_parent_block = true
            MINI_CONSOLE_BG_COL,  // underlay colo(u)r
            MINI_CONSOLE_CHAR_SPACING,
            MINI_CONSOLE_LINE_SPACING,
            fancy_ascii_set
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create mini console, error: ")
            + e.what() ) );
    }

    /////////////////////////////////////////////
    // CREATE MAIN CONSOLE
    std::shared_ptr<MCK::Console> main_console
        = std::make_shared<MCK::Console>();
    try
    {
        main_console->init(
            game_eng,
            image_man,
            game_eng.get_prime_render_block(),
            text_palette_id,
            MAIN_CONSOLE_LEFT,  // y_pos,
            MAIN_CONSOLE_TOP,  // x_pos,
            MAIN_CONSOLE_WIDTH_IN_CHARS,
            MAIN_CONSOLE_HEIGHT_IN_CHARS,
            MAIN_CHAR_WIDTH,  // char_width_in_pixels,
            MAIN_CHAR_HEIGHT,  // char_height_in_pixels,
            MSG_1,  // initial_content,
            20,  // print_speed_in_ticks_per_char,
            10,  // scroll_speed_in_ticks_per_pixel,
            true,  // hoz_text_alignment
            MAIN_CONSOLE_HEIGHT_IN_CHARS - 1,  // start_line
            false, // true  // add_to_front_of_parent_block = true
            MCK::COL_DARK_GRAY,  // Underlay colo(u)r
            MAIN_CONSOLE_CHAR_SPACING,
            MAIN_CONSOLE_LINE_SPACING,
            fancy_ascii_set
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create main console, error: ")
            + e.what() ) );
    }

    //////////////////////////////////////////////
    // MORE DEMO PARAMETERS

    // Lanscape console
    const uint8_t LSCAPE_CHAR_WIDTH = 32;
    const uint8_t LSCAPE_CHAR_HEIGHT = 16;
    const uint16_t LSCAPE_CONSOLE_LEFT = MAIN_CONSOLE_LEFT;
    const uint16_t LSCAPE_CONSOLE_TOP
        = MAIN_CONSOLE_TOP
            + main_console->get_height_in_pixels() + TILE_HEIGHT;
    const uint8_t LSCAPE_CONSOLE_WIDTH_IN_CHARS = 20;
    const uint8_t LSCAPE_CONSOLE_HEIGHT_IN_CHARS = 7;

    //////////////////////////////////////////////
    // CREATE BACKGROUND LANDSCAPE CONSOLE  
    std::shared_ptr<MCK::Console> bg_lscape_console
        = std::make_shared<MCK::Console>();
    try
    {
        bg_lscape_console->init(
            game_eng,
            image_man,
            game_eng.get_prime_render_block(),
            lscape_console_palette_id,
            LSCAPE_CONSOLE_LEFT,  // x_pos,
            LSCAPE_CONSOLE_TOP,  // y_pos,
            LSCAPE_CONSOLE_WIDTH_IN_CHARS,
            LSCAPE_CONSOLE_HEIGHT_IN_CHARS,
            LSCAPE_CHAR_WIDTH,  // char_width_in_pixels,
            LSCAPE_CHAR_HEIGHT,  // char_height_in_pixels,
            msg_3,  // initial_content,
            0,  // print_speed_in_ticks_per_char,
            10,  // scroll_speed_in_ticks_per_pixel,
            false,  // vert_text_alignment
            0,  // start_line
            true,  // add_to_front_of_parent_block = true
            MCK::COL_BLACK,
            0,  // char spacing
            0,  // line spacing
            fancy_ascii_set
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create background landscape console, error: ")
            + e.what() ) );
    }

    //////////////////////////////////////////////
    // CREATE LANDSCAPE CONSOLE  
    std::shared_ptr<MCK::Console> lscape_console
        = std::make_shared<MCK::Console>();
    try
    {
        lscape_console->init(
            game_eng,
            image_man,
            game_eng.get_prime_render_block(),
            lscape_console_palette_id,
            LSCAPE_CONSOLE_LEFT,  // x_pos,
            LSCAPE_CONSOLE_TOP,  // y_pos,
            LSCAPE_CONSOLE_WIDTH_IN_CHARS,
            LSCAPE_CONSOLE_HEIGHT_IN_CHARS,
            LSCAPE_CHAR_WIDTH,  // char_width_in_pixels,
            LSCAPE_CHAR_HEIGHT,  // char_height_in_pixels,
            msg_2,  // initial_content,
            0,  // print_speed_in_ticks_per_char,
            5,  // scroll_speed_in_ticks_per_pixel,
            false,  // vert_text_alignment
            10,  // start_line
            true, // add_to_front_of_parent_block = true
            MCK::COL_TRANSPARENT,
            0,  // char spacing
            0,  // line spacing
            fancy_ascii_set
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create landscape console, error: ")
            + e.what() ) );
    }


    ///////////////////////////////////////////
    // CREATE RENDER INFO
    try
    {
        // Top border for Main console
        game_eng.create_blank_tex_render_info(
            BG_COL,
            main_console->get_overlay_block(),
            MCK::GameEngRenderInfo::Rect( 
                MAIN_CONSOLE_LEFT,  // x pos
                MAIN_CONSOLE_TOP
                    - main_console->get_max_vert_scroll_offset(),  // y pos
                main_console->get_width_in_pixels(),
                main_console->get_max_vert_scroll_offset()  // Height, in pixels
            )
        );
        
        // Top border for Mini console,
        // to cover offset during scrolling
        game_eng.create_blank_tex_render_info(
            BG_COL,
            mini_console->get_overlay_block(),
            MCK::GameEngRenderInfo::Rect( 
                MINI_CONSOLE_LEFT,  // x pos
                MINI_CONSOLE_TOP
                    - mini_console->get_max_vert_scroll_offset(), // y pos
                mini_console->get_width_in_pixels(),  // Width
                MINI_CHAR_HEIGHT + MINI_CONSOLE_LINE_SPACING  // Height, in pixels
            )
        );
        
        // left border for lanscape console
        game_eng.create_blank_tex_render_info(
            BG_COL,
            lscape_console->get_overlay_block(),
            MCK::GameEngRenderInfo::Rect( 
                0,  // x pos
                LSCAPE_CONSOLE_TOP,  // y pos
                LSCAPE_CONSOLE_LEFT, 
                lscape_console->get_height_in_pixels()
            )
        );
        
        // right border for lanscape console
        game_eng.create_blank_tex_render_info(
            BG_COL,
            lscape_console->get_overlay_block(),
            MCK::GameEngRenderInfo::Rect( 
                LSCAPE_CONSOLE_LEFT
                    + lscape_console->get_width_in_pixels()
                        - LSCAPE_CHAR_WIDTH,  // x pos
                LSCAPE_CONSOLE_TOP,  // y pos
                LSCAPE_CHAR_WIDTH, 
                lscape_console->get_height_in_pixels()
            )
        );
        
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create borders, error: ")
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

        // Move mini-console on a figure of eight path
        {
            mini_console->get_overlay_block()->vert_offset
                = 
                    int( 
                        cos(
                            float(
                                ( current_ticks - start_ticks ) % 3000
                            ) / 1500.0f * 3.14127f
                        ) * 50.0f + 0.5f
                    ) + 50;
            
            mini_console->get_overlay_block()->hoz_offset
                = 
                    int( 
                        sin(
                            float(
                                ( current_ticks - start_ticks ) % 3000
                            ) / 750.0f * 3.14127f
                        ) * 20.0f + 0.5f
                    ) + 20;
        }

        // Update console(s) 
        try
        {
            main_console->update( current_ticks );
            mini_console->update( current_ticks );
            bg_lscape_console->update( current_ticks );
            lscape_console->update( current_ticks );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Console update(s) failed, error: ")
                + e.what() ) );
        }

        // Add more content to main console, if required
        if( main_console->get_text_buffer_size()
                < MAIN_CONSOLE_WIDTH_IN_CHARS
        )
        {
            try
            {
                main_console->add_content( MSG_1 );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to add content to main console, error: ")
                    + e.what() ) );
            }
        }

        // Add more content to mini console, if required
        if( mini_console->get_text_buffer_size()
                < MINI_CONSOLE_WIDTH_IN_CHARS
        )
        {
            try
            {
                mini_console->add_content( MSG_1 );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to add content to mini console, error: ")
                    + e.what() ) );
            }
        }

        // Add more content to background landscape console, if required
        if( bg_lscape_console->get_text_buffer_size()
                < LSCAPE_CONSOLE_HEIGHT_IN_CHARS
        )
        {
            try
            {
                bg_lscape_console->add_content( msg_3 );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to add content to lscape console, error: ")
                    + e.what() ) );
            }
        }

        // Add more content to landscape console, if required
        if( lscape_console->get_text_buffer_size()
                < LSCAPE_CONSOLE_HEIGHT_IN_CHARS
        )
        {
            try
            {
                lscape_console->add_content( msg_2 );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to add content to lscape console, error: ")
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
