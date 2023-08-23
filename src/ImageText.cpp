////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  ImageText.cpp
//
//  Basic multi-character text object for Image layer
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
//
//  IMPORTANT: Please see the statement in file 
//  'IMAGE_RIGHTS_DISCLAIMER.md'
//
////////////////////////////////////////////

#include "ImageText.h"

MCK::ImageText::ImageText( void )
{
    this->initialized = false;
    this->game_eng = NULL;
    this->image_man = NULL;
    this->current_content = "";
    this->local_palette_id = MCK::INVALID_PAL_ID; 
    this->size_in_chars = 0;
    this->justification = MCK::ImageText::INVALID;
}

MCK::ImageText::~ImageText( void )
{
    if( this->game_eng != NULL
        && this->block.get() != NULL
    )
    {
        try
        {
            MCK::GameEng::remove_block(
                this->block,
                game_eng->get_prime_render_block()
            );
        }
        catch( std::exception &e )
        {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
            // Issue a warning, but no point throwing exception here
            std::cout << "Unable to remove block during ImageText destructor, error: " 
                      << e.what()
                      << std::endl;
#endif
        }
    }
}

void MCK::ImageText::init(
    GameEng &_game_eng,
    ImageMan &_image_man,
    std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
    MCK_PAL_ID_TYPE _local_palette_id,
    int x_pos,
    int y_pos,
    uint8_t _size_in_chars,
    uint8_t _char_width_in_pixels,
    uint8_t _char_height_in_pixels,
    std::string initial_content,
    MCK::ImageText::Just _justification,
    bool add_to_front_of_parent_block,
    uint8_t _char_spacing_in_pixels
)
{
    if( this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot intialize ImageText instance, as already init."
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
            "Cannot initialize ImageText as GameEng not init."
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
            "Cannot initialize ImageText as ImageMan not init."
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
            "Cannot initialize ImageText as parent block ptr is NULL."
#else
            ""
#endif
        ) );
    }

    // Parameter sanity checks: size
    if( _size_in_chars == 0
        || _char_width_in_pixels == 0
        || _char_height_in_pixels == 0
        || _char_spacing_in_pixels > MAX_CHAR_SPACING 
    )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot initialize ImageText as number " )
            + std::string( "of chars is zero, " )
            + std::string( "and/or char size is zero, " )
            + std::string( "and/or char spacing is too large." )
#else
            ""
#endif
        ) );
    }

    // Parameter sanity checks: justification
    if( _justification == MCK::ImageText::INVALID )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot initialize ImageText as invalid " )
            + std::string( "justifcation value selected." )
#else
            ""
#endif
        ) );
    }

    // Store parameter values
    this->size_in_chars = _size_in_chars;
    this->char_width_in_pixels = _char_width_in_pixels;
    this->char_height_in_pixels = _char_height_in_pixels;
    this->char_spacing_in_pixels = _char_spacing_in_pixels;
    this->local_palette_id = _local_palette_id;
    this->justification = _justification;

    // Check for content clipping
    if( initial_content.size() > this->size_in_chars )
    {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
        // Issue warning if content too large
        std::cout << "Warning: initial content of ImageText "
                  << "instance exceeds specified size of "
                  << int( this->size_in_chars )
                  << " characters, content will be clipped."
                  << std::endl;
#endif
    }

    // Assign initial content (possibly clipped)
    this->current_content.assign(
        initial_content,
        0,
        this->size_in_chars
    );

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
            std::string( "Cannot initialize ImageText as failed " )
            + std::string( "to create block, error = " )
            + e.what()
#else
            ""
#endif
        ) );
    }

    const uint8_t CONTENT_SIZE = this->current_content.size();

    // Reserve memory for render info structs
    //this->block->render_info.reserve( this->size_in_chars );
    this->block->reserve_space_for_info_render( this->size_in_chars );

    // Getting starting character position for this content,
    // and also x,y pos of character image
    // both dependant on justification
    uint8_t start_char;
    int dx, dy;
    //this->get_start_char_and_xy( i, start_char, x, y );
    
    switch( this->justification )
    {
        case MCK::ImageText::LEFT:
            start_char = 0;
            dx = this->char_width_in_pixels + char_spacing_in_pixels;
            dy = 0;
            break;
        
        case MCK::ImageText::RIGHT:
            start_char = this->size_in_chars - CONTENT_SIZE; 
            dx = this->char_width_in_pixels + char_spacing_in_pixels;
            dy = 0;
            break;
        
        case MCK::ImageText::CENTER:
            start_char = ( this->size_in_chars - CONTENT_SIZE ) / 2;
            dx = this->char_width_in_pixels + char_spacing_in_pixels;
            dy = 0;
            break;
    
        case MCK::ImageText::VERT_TOP:
            start_char = 0;
            dx = 0;
            dy = this->char_height_in_pixels + char_spacing_in_pixels;
            break;
        
        case MCK::ImageText::VERT_BOTTOM:
            start_char = this->size_in_chars - CONTENT_SIZE; 
            dx = 0;
            dy = this->char_height_in_pixels + char_spacing_in_pixels;
            break;
        
        case MCK::ImageText::VERT_CENTER:
            start_char = ( this->size_in_chars - CONTENT_SIZE ) / 2;
            dx = 0;
            dy = this->char_height_in_pixels + char_spacing_in_pixels;
            break;
    
        default:
            // Treat invalid value as left/top justified
            start_char = 0;
    }

    // Create render info structs, and assign to block
    for( int i = 0; i < this->size_in_chars; i++ )
    {
        // Get ASCII value of this char 
        // (or set as blank if out of chars)
        char c;
        if( i < start_char )
        {
            c = MCK::BLANK_CHAR;
        }
        else if( i < start_char + this->current_content.size() )
        {
            c = this->current_content[i - start_char];
        }
        else
        {
            c = MCK::BLANK_CHAR;
        }

        try
        {
            // this->block->render_info.push_back(
            this->image_man->create_extended_ascii_render_info(
                c,
                this->local_palette_id,
                x_pos + i * dx,
                y_pos + i * dy,
                this->char_width_in_pixels,
                this->char_height_in_pixels,
                block
            );
            //);
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Cannot initialize ImageText as failed " )
                + std::string( "to create block, error = " )
                + e.what()
#else
                ""
#endif
            ) );
        }
    }

    // Internal quality check
    if( this->block->get_render_info_count() != this->size_in_chars )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot initialize ImageText as block's " )
            + std::string( "render info list has size " )
            + std::to_string( this->block->get_render_info_count() )
            + std::string( "where as size_in_chars is " )
            + std::to_string( this->size_in_chars )
            + std::string( ". These should match! Internal error?." )
#else
            ""
#endif
        ) );
    }

    this->initialized = true;
}

void MCK::ImageText::set_content(
    std::string new_content,
    MCK::ImageText::Just new_justification
)
{
    if( !this->initialized || this->block.get() == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot set content as ImageText instance not yet init."
#else
            ""
#endif
        ) );
    }

#if defined MCK_STD_OUT && defined MCK_VERBOSE
    // Issue warning if new content too large
    if( new_content.size() > this->size_in_chars )
    {
        std::cout << "Warning: new content of ImageText instance "
                  << "exceeds specified size of "
                  << int( this->size_in_chars )
                  << ", content will be clipped."
                  << std::endl;
    }
#endif

    // Assign current content (possibly clipped)
    this->current_content.assign( new_content, 0, this->size_in_chars );

    // Set new justifcation (if specified)
    if( new_justification != MCK::ImageText::INVALID )
    {
        // Automatically convert LEFT to TOP (and visa versa)
        // and RIGHT to BOTTOM, etc...
        if( this->is_horizontal() )
        {
            // Convert vert justifications to hoz ones
            switch( new_justification )
            {
                case MCK::ImageText::VERT_TOP:
                    this->justification = MCK::ImageText::LEFT;
                    break;

                case MCK::ImageText::VERT_BOTTOM:
                    this->justification = MCK::ImageText::RIGHT;
                    break;

                case MCK::ImageText::VERT_CENTER:
                    this->justification = MCK::ImageText::CENTER;
                    break;

                default:
                    this->justification = new_justification;
            }
        }
        else
        {
            // Convert hoz justifications to vert ones
            switch( new_justification )
            {
                case MCK::ImageText::LEFT:
                    this->justification = MCK::ImageText::VERT_TOP;
                    break;

                case MCK::ImageText::RIGHT:
                    this->justification = MCK::ImageText::VERT_BOTTOM;
                    break;

                case MCK::ImageText::CENTER:
                    this->justification = MCK::ImageText::VERT_CENTER;
                    break;

                default:
                    this->justification = new_justification;
            }
        }
    }

    const uint8_t CONTENT_SIZE = this->current_content.size();

    // Getting starting character position for this content,
    // dependant on justification
    uint8_t start_char;
    switch( this->justification )
    {
        case MCK::ImageText::LEFT:
            start_char = 0;
            break;
        
        case MCK::ImageText::RIGHT:
            start_char = this->size_in_chars - CONTENT_SIZE; 
            break;
        
        case MCK::ImageText::CENTER:
            start_char = ( this->size_in_chars - CONTENT_SIZE ) / 2;
            break;

        case MCK::ImageText::VERT_TOP:
            start_char = 0;
            break;
        
        case MCK::ImageText::VERT_BOTTOM:
            start_char = this->size_in_chars - CONTENT_SIZE; 
            break;
        
        case MCK::ImageText::VERT_CENTER:
            start_char = ( this->size_in_chars - CONTENT_SIZE ) / 2;
            break;
            
        default:
            // Treat invalid as left justified
            start_char = 0;
            break;
    }

    // Update render info structs
    for( int i = 0; i < this->size_in_chars; i++ )
    {
        // Get ASCII value of this char 
        // (or set as blank if out of chars)
        char c;
        if( i < start_char )
        {
            c = MCK::BLANK_CHAR;
        }
        else if( i < start_char + this->current_content.size() )
        {
            c = this->current_content[i - start_char];
        }
        else
        {
            c = MCK::BLANK_CHAR;
        }

        try
        {
            this->image_man->change_render_info_ascii_value(
                this->block->get_render_info( i ),
                c,
                this->local_palette_id
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Failed to assign new ascii texture, error = " )
                + e.what()
#else
                ""
#endif
            ) );
        }
    }
}

void MCK::ImageText::get_top_left_pixel_pos(
    int &x_pos,
    int &y_pos,
    bool include_offset
) const
{
    if( !this->initialized || this->block.get() == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot get top-left pixel pos as ImageText instance not yet init."
#else
            ""
#endif
        ) );
    }
   
    if( this->block->get_render_info_count() == 0
        || this->block->get_render_info( 0 ).get() == NULL
    )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to get top-left pixel position " )
            + std::string( "as render info is absent." )
#else
            ""
#endif
        ) );
    }

    // Get x/y pos from first character
    x_pos = this->block->get_render_info( 0 )->dest_rect.get_x()
                + this->block->hoz_offset;
    y_pos = this->block->get_render_info( 0 )->dest_rect.get_y()
                + this->block->vert_offset;
    
    // Add offset, if needed
    if( include_offset )
    {
        x_pos += this->block->hoz_offset;
        y_pos += this->block->vert_offset;
    }
}

void MCK::ImageText::set_new_top_left_pixel_pos(
    int new_x_pos,
    int new_y_pos
)
{
    if( !this->initialized || this->block.get() == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot move ImageText instance as not yet init."
#else
            ""
#endif
        ) );
    }
   
    if( this->block->get_render_info_count() == 0
        || this->block->get_render_info( 0 ).get() == NULL
    )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to move ImageText instance " )
            + std::string( "as render info is absent." )
#else
            ""
#endif
        ) );
    }

    // Get top-left of first char image
    const int FIRST_CHAR_X = this->block->get_render_info( 0 )->dest_rect.get_x();
    const int FIRST_CHAR_Y = this->block->get_render_info( 0 )->dest_rect.get_y();

    // Calculate and set required offset
    this->block->hoz_offset = new_x_pos - FIRST_CHAR_X;
    this->block->vert_offset = new_y_pos - FIRST_CHAR_Y;
}

void MCK::ImageText::nudge_pixel_pos(
    int dx,
    int dy
)
{
    if( !this->initialized || this->block.get() == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot nudge ImageText instance as not yet init."
#else
            ""
#endif
        ) );
    }
   
    this->block->hoz_offset += dx;
    this->block->vert_offset += dy;
}

void MCK::ImageText::set_char(
    uint8_t ascii_value,
    uint8_t char_pos
)
{
    if( !this->initialized || this->block.get() == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot set char as ImageText instance not yet init."
#else
            ""
#endif
        ) );
    }

    if( char_pos >= this->block->get_render_info_count() )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot set char as char_pos is out-of-range."
#else
            ""
#endif
        ) );
    }

    // Update render info struct for this character position
    try
    {
        this->image_man->change_render_info_ascii_value(
            this->block->get_render_info( char_pos ),
            ascii_value,
            this->local_palette_id
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to assign set new char texture, error = " )
            + e.what()
#else
            ""
#endif
        ) );
    }
}
