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
    this->scroll_in_progress = false;
    this->next_char_pos = 0;
    this->ticks_at_last_update = 0;
    this->write_line_x_pos = 0;
    this->write_line_y_pos = 0;
    this->char_spacing_in_pixels = 0;
    this->line_spacing_in_pixels = 0;
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
    const std::string &initial_content,
    uint32_t _print_speed_in_ticks_per_char,
    uint32_t _scroll_speed_in_ticks_per_pixel,
    bool _hoz_text_alignment,
    uint8_t start_line,
    bool add_to_front_of_parent_block,
    uint8_t underlay_color_id,
    uint8_t _char_spacing_in_pixels,
    uint8_t _line_spacing_in_pixels
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
        || _char_spacing_in_pixels > MCK::MAX_CHAR_SPACING
        || _line_spacing_in_pixels > MCK::MAX_LINE_SPACING
    )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot initialize Console as size is zero, " )
            + std::string( "and/or char size is zero, " )
            + std::string( "and/or char spacing is too large, " )
            + std::string( "and/or line spacing is too large." )
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
    this->print_speed_in_ticks_per_char
            = _print_speed_in_ticks_per_char;
    this->scroll_speed_in_ticks_per_pixel
            = _scroll_speed_in_ticks_per_pixel;
    this->hoz_text_alignment = _hoz_text_alignment;
    this->char_spacing_in_pixels = _char_spacing_in_pixels;
    this->line_spacing_in_pixels = _line_spacing_in_pixels;

    // DEBUG
    std::cout << "@@@ " << int( this->char_spacing_in_pixels )
              << ", " << int( this->line_spacing_in_pixels )
              << std::endl;

    // Create new overlay render block
    // (the overlay block holds everything)
    try
    {
        this->overlay_block
            = this->game_eng->create_empty_render_block(
                parent_block,
                add_to_front_of_parent_block
            );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot initialize Console as failed " )
            + std::string( "to create overlay block, error = " )
            + e.what()
#else
            ""
#endif
        ) );
    }

    // Create new main render block
    // This is block to which ImageText lines at attached,
    // its offsets are used to scroll the console,
    try
    {
        this->block = this->game_eng->create_empty_render_block(
                    this->overlay_block,
                    true  // Display at front
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

    // Create new underlay render block
    // This is the block to which a blank self-colo(u)red
    // image is attached to hide the background that would
    // otherwise be visible behind the write line.
    try
    {
        this->underlay_block
            = this->game_eng->create_empty_render_block(
                this->overlay_block,
                false  // Display at rear
            );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot initialize Console as failed " )
            + std::string( "to create underlay block, error = " )
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
    uint16_t line_len_in_pixels;
    uint16_t console_width_in_pixels;
    uint16_t console_height_in_pixels;
    if( this->hoz_text_alignment )
    {
        num_lines = this->height_in_chars;
        line_len = this->width_in_chars;
        dx = 0;
        dy = this->char_height_in_pixels
                + this->line_spacing_in_pixels;
        justification = MCK::ImageText::LEFT;
        console_width_in_pixels
            = line_len * this->char_width_in_pixels
                + ( line_len - 1 ) * this->char_spacing_in_pixels;
        console_height_in_pixels
            = num_lines * this->char_height_in_pixels
                + ( num_lines - 1 ) * this->line_spacing_in_pixels;
    }
    else
    {
        num_lines = this->width_in_chars;
        line_len = this->height_in_chars;
        dx = this->char_width_in_pixels
                + this->line_spacing_in_pixels;
        dy = 0;
        justification = MCK::ImageText::VERT_TOP;
        console_width_in_pixels
            = num_lines * this->char_width_in_pixels
                + ( num_lines - 1 ) * this->char_spacing_in_pixels;
        console_height_in_pixels
            = line_len * this->char_height_in_pixels
                + ( line_len - 1 ) * this->line_spacing_in_pixels;
    }
    
    const int LAST_LINE_NUM = int( num_lines - 1 );

    // Ensure start line is less than num of lines
    start_line = std::min( int( start_line ), LAST_LINE_NUM );

    // Store postion of write line (i.e. last line)
    // Note: this is used, during scrolling, to initialise
    //       each new write line
    this->write_line_x_pos = x_pos + LAST_LINE_NUM * dx;
    this->write_line_y_pos = y_pos + LAST_LINE_NUM * dy;

    // Create underlay
    try
    {
        game_eng->create_blank_tex_render_info(
            underlay_color_id,
            underlay_block,
            MCK::GameEngRenderInfo::Rect(
                x_pos,
                y_pos,
                console_width_in_pixels,
                console_height_in_pixels
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to create underlay for write line")
            + std::string( " of console, error: ")
            + e.what()
#else
            ""
#endif
        ));
    }

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
                            * line_len;  // this->width_in_chars;
            if( POS < initial_content.size() )
            {
                content_pos = POS;
                content_len = line_len; // this->width_in_chars;
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
                justification, 
                true,  // Add to front
                this->char_spacing_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Failed to create ImageText instance for line ")
                + std::to_string( i )
                + std::string( " of console, error: ")
                + e.what()
#else
                ""
#endif
            ));
        }

        lines.push_back( new_line );
    }

    // Enqueue any unused initial content in text buffer
    {
        const size_t START_POS
            = std::min(
                int( initial_content.size() ),
                line_len * ( num_lines - start_line )
            );
        const size_t END_POS = initial_content.size();

        // If any unused content, start scrolling process
        // by setting appropriate scroll offset to 1.
        if( START_POS < END_POS )
        {
            this->scroll_in_progress = true;
        }

        // Enqueue unused content
        for( int i = START_POS; i < END_POS; i++ )
        {
            try
            {
                text_buffer.push( initial_content.at( i ) );
            }
            catch( std::exception &e )
            {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
                std::cout << "Failed to push char to text buffer, error: "
                          << e.what() << std::endl;
#endif
                // If fails, stop here but don't throw exception
                break;
            }
        }
    }

    // DEBUG
    std::cout << "@@ text_buffer.size() = "
              << text_buffer.size() << std::endl;

    initialized = true;
}

void MCK::Console::update( uint32_t current_ticks )
{
    // Initialisation and safety checks
    if( !this->initialized
        || this->game_eng == NULL
        || this->lines.size() == 0
        || this->block.get() == NULL
    )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot update Console instance, as not yet init."
#else
            ""
#endif
        ) );
    }

    // If no time supplied, get time via GameEng
    if( current_ticks == 0 )
    {
        current_ticks = this->game_eng->get_ticks();
    }

    // Get ticks since last udpate
    // Note: if current_ticks < ticks_at_last_update,
    //       TICKS will be *very* large, resulting
    //       in all text being pulled from the buffer
    //       which is the correct outcome, so no need
    //       to check if current_ticks < ticks_at_last_update
    uint32_t ticks = current_ticks - this->ticks_at_last_update;

    // Get shortcut to block offset used for scrolling
    int16_t* const SCROLL_OFFSET
        = this->hoz_text_alignment ?
            &this->block->vert_offset :
            &this->block->hoz_offset;

    // Get max scroll offset
    const int16_t MAX_SCROLL_OFFSET
        = this->hoz_text_alignment ?
            int16_t( this->char_height_in_pixels )
                + int16_t( this->line_spacing_in_pixels ) :
            int16_t( this->char_width_in_pixels )
                + int16_t( this->line_spacing_in_pixels );

    // Get number of chars on line
    const uint8_t LINE_LEN
        = this->hoz_text_alignment ?
            this->width_in_chars :
            this->height_in_chars;

    // DEBUG
    //std::cout << "@@ *SCROLL_OFFSET = "
    //          << *SCROLL_OFFSET << std::endl; 

    // Draw characters from text buffers. and/or scroll console,
    // until 'ticks' are used up or text buffer is empty
    // Explainer: The 'while' logic here asks:
    //  a) Is there text to print, and sufficient time to print
    //     at least one char?
    //  OR
    //  b) Is scrolling in progress, and sufficient time to scroll
    //     at least one pixel?
    //  If this evaluates to False, 'ticks' is subtracted from
    //  'ticks_at_last_update' when next calculated
    while( ( ticks >= this->print_speed_in_ticks_per_char
             && this->text_buffer.size() > 0
             && !this->scroll_in_progress
             // && *SCROLL_OFFSET == 0 
           ) ||
           ( ticks >= this->scroll_speed_in_ticks_per_pixel
             // && *SCROLL_OFFSET != 0
             && this->scroll_in_progress
           )
    )
    {

        /////////////////////////////////////////////
        // SCROLLING

        // If scrolling has started, scroll one pixel
        if( ticks >= this->scroll_speed_in_ticks_per_pixel
            // && *SCROLL_OFFSET != 0
            && this->scroll_in_progress
        )
        {
            // Check for end of scroll
            if( *SCROLL_OFFSET == -1 * MAX_SCROLL_OFFSET + 1 )
            {
                // Reset scroll offset
                *SCROLL_OFFSET = 0;

                // Destroy first remaining line
                // TODO: Keep a history
                this->lines.pop_front();

                // Change position of all remaining lines
                const int DX = -1 * ( this->char_width_in_pixels
                                        + this->line_spacing_in_pixels 
                                    ) * !this->hoz_text_alignment;
                const int DY = -1 * ( this->char_height_in_pixels
                                        + this->line_spacing_in_pixels
                                    ) * this->hoz_text_alignment;

                for( std::shared_ptr<MCK::ImageText> ln : this->lines )
                {
                    try
                    {
                        ln->nudge_pixel_pos( DX, DY );
                    }
                    catch( std::exception &e )
                    {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
                        std::cout << "Failed to nudge line, error: "
                                  << e.what() << std::endl;
#endif
                        // If fails, skip line but don't throw exception
                        continue;
                    }

                }

                // Create new line for writing
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
                        this->write_line_x_pos,
                        this->write_line_y_pos,
                        this->hoz_text_alignment ?
                            this->width_in_chars :
                            this->height_in_chars,
                        this->char_width_in_pixels,
                        this->char_height_in_pixels,
                        "",
                        this->hoz_text_alignment ?
                            MCK::ImageText::LEFT :
                            MCK::ImageText::VERT_TOP,
                        true,  // Render on top
                        this->char_spacing_in_pixels
                    );
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Failed to create ImageText instance ")
                        + std::string( "for new write line, error: ")
                        + e.what()
#else
                        ""
#endif
                    ));
                }

                // Store new write line
                lines.push_back( new_line );

                // End scrolling here
                this->scroll_in_progress = false;
            }
            else
            {
                (*SCROLL_OFFSET)--;
                ticks -= this->scroll_speed_in_ticks_per_pixel;
                continue;
            }

            /*
            if( (*SCROLL_OFFSET)-- > 0 )
            {
                continue;
            }
            else
            {
                // Move ALL lines, delete first line
            }
            */
        }

        ///////////////////////////////////////////////////
        // CHARACTER PRINTING

        if ( ticks >= this->print_speed_in_ticks_per_char
              && this->text_buffer.size() > 0
              // && *SCROLL_OFFSET == 0 
              && !this->scroll_in_progress
        )
        {
            // Get next char
            const uint8_t C = this->text_buffer.front();
            this->text_buffer.pop();

            // Print char
            try
            {
                this->lines.back()->set_char( C, this->next_char_pos++ );
            }
            catch( std::exception &e )
            {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
                std::cout << "Failed to set char, error: "
                          << e.what() << std::endl;
#endif
                // If fails, just skip char
                continue;
            }

            // If ok, decrement ticks and move position for next char
            ticks -= this->print_speed_in_ticks_per_char;
            if( this->next_char_pos >= LINE_LEN )
            {
                this->next_char_pos = 0;

                this->scroll_in_progress = true;

                // Setting scroll offset to < 0 triggers scrolling
                // procedure
                // *SCROLL_OFFSET = -1;
                
                // End update here
                break;
            }
        }
    }

    // Save current ticks for next update, adding any 'unused'
    // ticks from this update
    this->ticks_at_last_update = current_ticks - ticks;
}

void MCK::Console::add_content( const std::string &in )
{
    // Initialisation and safety checks
    if( !this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot update Console instance, as not yet init."
#else
            ""
#endif
        ) );
    }

    // Let calling method catch any exception here
    for( char c : in )
    {
        text_buffer.push( c );
    }
}
