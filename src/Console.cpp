////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  Console.cpp
//
//  Console for displaying scrolling text/symbols
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

#include "Console.h"

MCK::Console::Console( void )
{
    this->initialized = false;
    this->game_eng = NULL;
    this->image_man = NULL;
    this->width_in_chars = 0;
    this->height_in_chars = 0;
    this->char_width_in_pixels = 0,
    this->char_height_in_pixels = 0,
    this->print_speed_in_ticks_per_char = 0;
    this->scroll_speed_in_ticks_per_pixel = 0;
    this->hoz_text_alignment = true;
    this->ticks_at_last_update = 0;
}

void MCK::Console::init(
    GameEng &_game_eng,
    ImageMan &_image_man,
    std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
    MCK_PAL_ID_TYPE _local_palette_id,
    uint16_t x_pos,
    uint16_t y_pos,
    uint8_t _width_in_chars,
    uint8_t _height_in_chars,
    uint8_t _char_width_in_pixels,
    uint8_t _char_height_in_pixels,
    std::string &initial_content,
    uint32_t _print_speed_in_ticks_per_char,
    uint32_t _scroll_speed_in_ticks_per_pixel,
    bool _hoz_text_alignment,
    uint8_t start_line,
    bool add_to_front_of_parent_block
)
{
    if( this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot intialize Console instance, as already init."
#else
            ""
#endif
        ) );
    }

    // Check game engine
    if( !_game_eng.is_initialized() )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot initialize Console as GameEng not init."
#else
            ""
#endif
        ) );
    }
    this->game_eng = &_game_eng;

    // Check image manager
    if( !_image_man.is_initialized() )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot initialize Console as ImageMan not init."
#else
            ""
#endif
        ) );
    }
    this->image_man = &_image_man;

    // Check parent block pointer
    if( parent_block.get() == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot initialize Console as parent block ptr is NULL."
#else
            ""
#endif
        ) );
    }

    // Parameter sanity checks: size
    if( _width_in_chars == 0
        || _height_in_chars == 0
        || _char_width_in_pixels == 0
        || _char_height_in_pixels == 0
    )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot initialize Console as size " )
            + std::string( "and/or char size is zero." )
#else
            ""
#endif
        ) );
    }

    // Store parameter values
    this->width_in_chars = _width_in_chars;
    this->height_in_chars = _height_in_chars;
    this->char_width_in_pixels = _char_width_in_pixels;
    this->char_height_in_pixels = _char_height_in_pixels;
    this->local_palette_id = _local_palette_id;
    this->hoz_text_alignment = _hoz_text_alignment;

    // Create new render block
    try
    {
        this->block = this->game_eng->create_empty_render_block(
                    parent_block,
                    add_to_front_of_parent_block
                );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot initialize Console as failed " )
            + std::string( "to create block, error = " )
            + e.what()
#else
            ""
#endif
        ) );
    }

    /////////////////////////////////////////////////////
    // Calculate various values dependent on alignment...
    uint8_t num_lines, line_len;
    int dx, dy;
    MCK::ImageText::Just justification;
    if( this->hoz_text_alignment )
    {
        num_lines = this->height_in_chars;
        line_len = this->width_in_chars;
        dx = 0;
        dy = this->char_height_in_pixels;
        justification = MCK::ImageText::LEFT;
    }
    else
    {
        num_lines = this->width_in_chars;
        line_len = this->height_in_chars;
        dx = this->char_height_in_pixels;
        dy = 0;
        justification = MCK::ImageText::VERT_TOP;
    }
    
    // Ensure start line is less than num of lines
    start_line = std::min( int( start_line ), int( num_lines - 1 ) );

    // Create lines....

    // size_t max_content_pos = 0;
    for( int i = 0; i < num_lines; i++ )
    {
        // Work out position in initial_content of
        // content destined for this line
        // (or leave content_pos and content_len
        //  as zero, which produces no content)
        size_t content_pos = 0;
        size_t content_len = 0;
        if( i >= start_line )
        {
            const size_t POS = ( i - start_line )
                            * this->width_in_chars;
            if( POS < initial_content.size() )
            {
                content_pos = POS;
                // max_content_pos = POS;
                content_len = this->width_in_chars;
            }
        }
            
        // Construct new line
        std::shared_ptr<MCK::ImageText> new_line
            = std::make_shared<MCK::ImageText>();

        // Initialise new line
        try
        {
            new_line->init(
                *this->game_eng,
                *this->image_man,
                this->block,
                this->local_palette_id,
                x_pos + i * dx,
                y_pos + i * dy,
                line_len,
                this->char_width_in_pixels,
                this->char_height_in_pixels,
                initial_content.substr(
                    content_pos,
                    content_len
                ),
                justification
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Failed to create ImageText instance for line ")
                + std::to_string( i )
                + std::string( " of console, error: ")
                + e.what() ) );
        }

        lines.push_back( new_line );
    }

    // TODO... Enqueue any unused initial content in text buffer
    /*
       initial_content.substr(
            std::min(
                int( initial_content.size() - 1 ),
                line_len * ( num_lines - start_line )
            ),
            std::string::npos
        );
    */
}

