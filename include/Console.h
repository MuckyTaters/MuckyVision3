////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  Console.h
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

#ifndef MCK_CONSOLE_H
#define MCK_CONSOLE_H

#if defined MCK_STD_OUT
#include <iostream> // For std::cout
#endif

#include <queue>

#include "ImageText.h"

namespace MCK
{

class Console
{
    public:

        Console( void );
        virtual ~Console( void ) {}

        void init(
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
            uint32_t _print_speed_in_ticks_per_char = 0,
            uint32_t _scroll_speed_in_ticks_per_pixel = 0,
            bool _hoz_text_alignment = true,
            uint8_t start_line = 0,
            bool add_to_front_of_parent_block = true
        );

        bool is_initialized( void ) const noexcept
        {
            return initialized;
        }

        //! Process any temporal changes
        void update( void );

        //! Add content to console input buffer
        void add_content( std::string in );

        //! This method animates console and ideally should be called every frame
        /*! @param current_ticks: If ommited, ticks are obtained from GameEng instance
         */
        void update( uint32_t current_ticks = 0 );

    protected:

        bool initialized;

        GameEng* game_eng;

        ImageMan* image_man;

        std::list<std::shared_ptr<MCK::ImageText>> lines;

        std::queue<uint8_t> text_buffer;

        uint8_t width_in_chars;
        uint8_t height_in_chars;
        uint8_t char_width_in_pixels;
        uint8_t char_height_in_pixels;
        uint32_t print_speed_in_ticks_per_char;
        uint32_t scroll_speed_in_ticks_per_pixel;
        bool hoz_text_alignment;

        int16_t write_line_x_pos;
        int16_t write_line_y_pos;

        // uint8_t scroll_offset;
        uint8_t next_char_pos;

        uint32_t ticks_at_last_update;
        
        MCK_PAL_ID_TYPE local_palette_id;

        std::shared_ptr<MCK::GameEngRenderBlock> block;
};

}  // End of namespace MCK

#endif
