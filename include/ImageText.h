////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  ImageText.h
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
////////////////////////////////////////////

#ifndef MCK_IMG_TXT_H
#define MCK_IMG_TXT_H

#if defined MCK_STD_OUT
#include <iostream> // For std::cout
#endif

#include "ImageMan.h"

namespace MCK
{

class ImageText
{
    public:

        //! Enumeration for horizontal justification types
        /*! Note US spelling of 'center' */
        enum Just
        {
            INVALID,
            LEFT,
            RIGHT,
            CENTER,
            VERT_TOP,  // Veritcal text, justified to top
            VERT_BOTTOM,  // Veritcal text, justified to bottom
            VERT_CENTER  // Veritcal text, centered
        };

        ImageText( void );

        virtual ~ImageText( void );

        //! Initialization
        /*! @param _game_eng: GameEng instance
         *  @param _image_man: ImageMan instance
         *  @param parent_block: Block to which text box is assigned
         *  @param _local_palette_id: ID of palette used for text
         *  @param _x_pos: left of text box, in pixels, relative to parent block
         *  @param _y_pos: top of text box, in pixels, relative to parent block
         *  @param _size_in_chars: Size of text box, in characters
         *  @param _char_width_in_pixels: Pixel width of each character
         *  @param _char_height_in_pixels: Pixel height of each character
         *  @param initial_content: String containing characters to display
         *  @param _justification: Hoz/vert text direction and justification
         *  @param add_to_front_of_parent_block: If true, appears in front of any other blocks attached to parent block
         */
        void init(
            GameEng &_game_eng,
            ImageMan &_image_man,
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
            MCK_PAL_ID_TYPE _local_palette_id,
            int _x_pos,
            int _y_pos,
            uint8_t _size_in_chars,
            uint8_t _char_width_in_pixels,
            uint8_t _char_height_in_pixels,
            std::string initial_content = "",  // Not pass by ref as r-value typically supplied
            MCK::ImageText::Just _justification = MCK::ImageText::LEFT,
            bool add_to_front_of_parent_block = true
        );

        //! Returns true if initialized
        bool is_initialized( void ) const noexcept
        {
            return initialized;
        }

        //! Set new string content
        /*! @param new_content: String containing new content
         *  @param new_justification: New justification (omit to kee p current justifcation)
         * Note: string parameter not passed by reference as r-value
         *       typically supplied
         * Note: Hoz/vert alignment is preserved, even if justification specifies otherwise
         */
        void set_content(
            std::string new_content,
            MCK::ImageText::Just new_justification
                = MCK::ImageText::INVALID
        );

        //! Set individual character
        /*! @param ascii_value: Value of character
         *  @param char_pos: Position if character
         *  Note: No justification is applied, the position
         *        specified is the actual postion at which 
         *        the new ASCII value is written.
         */
        void set_char(
            uint8_t ascii_value,
            uint8_t char_pos
        );

        //! Get pixel position of top-left corner of text box
        /*! @param include_offset: Include offset, if any present
         */
        void get_top_left_pixel_pos(
            int &x_pos,
            int &y_pos,
            bool include_offset
        ) const;

        //! Move text box (by changing offset to reach specified position)
        void set_new_top_left_pixel_pos(
            int new_x_pos,
            int new_y_pos
        );

        void nudge_pixel_pos(
            int dx,
            int dy
        );

        //! Returns true if visible
        bool is_active( void ) const noexcept
        {
            return block.get() != NULL && block->active;
        }

        //! Make text visible
        void make_active( void ) noexcept
        {
            if( block.get() != NULL )
            {
                block->active = true;
            }
        }

        //! Make text invisible
        void make_inactive( void ) noexcept
        {
            if( block.get() != NULL )
            {
                block->active = false;
            }
        }

        //! Returns justification (left,right,center)
        MCK::ImageText::Just get_justification( void ) const noexcept
        {
            return justification;
        }

        //! Returns true if alignment is horizontal, false if vertical or invalid
        bool is_horizontal( void ) const noexcept
        {
            return justification == MCK::ImageText::LEFT
                   || justification == MCK::ImageText::RIGHT
                   || justification == MCK::ImageText::CENTER;
        }

        //! Get max number of available character slots
        /* Note: The actual content may not use all available slots
         */
        uint8_t get_max_size_in_chars( void ) const noexcept
        {
            return size_in_chars;
        }

        //! Get width of each character, in pixels
        uint8_t get_char_width_in_pixels( void ) const noexcept
        {
            return char_width_in_pixels;
        }

        //! Get height of each character, in pixels
        uint8_t get_char_height_in_pixels( void ) const noexcept
        {
            return char_height_in_pixels;
        }

        //! Get current content, as a string
        std::string get_current_content( void ) const noexcept
        {
            return current_content;
        }

        //! Get id of local palette used for characters
        MCK_PAL_ID_TYPE get_local_palette_id( void ) const noexcept
        {
            return local_palette_id;
        }

        //! Get pointer to text box's render block
        std::shared_ptr<MCK::GameEngRenderBlock> get_block( void )
        {
            return block;
        }


    protected:

        bool initialized;

        GameEng* game_eng;

        ImageMan* image_man;

        MCK::ImageText::Just justification;
        uint8_t size_in_chars;
        uint8_t char_width_in_pixels;
        uint8_t char_height_in_pixels;

        std::string current_content;

        MCK_PAL_ID_TYPE local_palette_id;

        std::shared_ptr<MCK::GameEngRenderBlock> block;
};

}  // End of namespace MCK

#endif
