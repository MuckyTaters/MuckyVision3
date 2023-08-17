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

        //!Enumeration for horizontal justification types
        /*! Note US spelling of 'center' */
        enum HozJust
        {
            LEFT,
            RIGHT,
            CENTER
        };

        ImageText( void );

        virtual ~ImageText( void ) {}

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
            MCK::ImageText::HozJust _justify = MCK::ImageText::LEFT,
            bool add_to_front_of_parent_block = true
        );

        bool is_initialized( void ) const noexcept
        {
            return initialized;
        }

        //! Set new string content
        /*! @param new_content: String containing new content
         * Note: parameter not passed by reference as r-value
         *       typically supplied
         */
        void set_content( std::string new_content );

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


    protected:

        bool initialized;

        GameEng* game_eng;

        ImageMan* image_man;

        MCK::ImageText::HozJust justify;
        int x_pos;
        int y_pos;
        uint8_t size_in_chars;
        uint8_t char_width_in_pixels;
        uint8_t char_height_in_pixels;

        std::string current_content;

        MCK_PAL_ID_TYPE local_palette_id;

        std::shared_ptr<MCK::GameEngRenderBlock> block;
};

}  // End of namespace MCK

#endif
