////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  ImageMan.h
//
//  Image Manager singleton class,
//  sits on top of GameEng instance.
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

#ifndef MCK_IMG_MAN_H
#define MCK_IMG_MAN_H

#if defined MCK_STD_OUT
#include <iostream> // For std::cout
#endif

#include <map>  // For map
#include <memory>  // For shared_ptr
#include <string>  // For string uses other than std output
#include <vector>  // For vector

#include "Defs.h"
#include "GameEng.h"
#include "GameEngRenderBlock.h"
#include "GameEngRenderInfo.h"
#include "ImageDataASCII.h"
#include "Version.h"

namespace MCK
{

class ImageMan
{
    public:

        //! Constructs and initializes singleton instance of class
        static ImageMan& get_singleton( void ) noexcept
        {
            static ImageMan image_man;
            return image_man;
        }

        //! Initialize image manager
        /*! @param _game_eng: Initialized singleton GameEng instance
         */
        void init(
            GameEng &_game_eng
        );

        //! Returns true if initialized, false otherwise
        bool is_initialized( void ) const noexcept
        {
            return initialized;
        }

        //! Create local colo(u)r palette
        /*! @params global_color_ids: Pointer to a vector of length 2, 4 or 16, each element being a global colo(u)r ID.
         *  @returns ID of new (or existing) local palette.
         *  A local palette is a subset of the global colo(u)r
         *  palette containing 2, 4, or 16 colo(u)rs, each 
         *  referenced by their ID in the global palette.
         *  Local palettes are used to assign colo(u)rs to
         *  pixels within an image, and the palette's size should
         *  always correspond to the number of bits per pixel
         *  (1, 2 or 4 respectively). 
         *  Local palettes are order sensitive, so (for example)
         *  {0,1} and {1,0} will produce mutally inverted images.
         *  If a local palette already exists, ID of existing
         *  palette will be returned.
         */
        MCK_PAL_ID_TYPE create_local_palette(
            const std::shared_ptr<std::vector<uint8_t>> global_color_ids
        );

        //! Create a custom image (using shared pointer), and return its Image ID
        /*! @param pixel_data: Shared pointer to byte vector holding pixel data.
         *  @param pitch_per_pixel: Image width in pixels.
         *  @param pitch_per_pixel: Image height in pixels.
         *  Note: no checking is done for duplicate images, if you are
         *        not careful you could have the same image registered
         *        under different image IDs. It is your responsibility
         *        to avoid this happening.
         */
        MCK_IMG_ID_TYPE create_custom_image(
            std::shared_ptr<const std::vector<uint8_t>> pixel_data,
            uint8_t bits_per_pixel,
            uint16_t width_in_pixels,
            uint16_t height_in_pixels
        );

        //! Create a custom image (using non-shared pointer), and return its Image ID
        /*! @param pixel_data: Pointer to byte vector holding pixel data.
         *  @param pitch_per_pixel: Image width in pixels.
         *  @param pitch_per_pixel: Image height in pixels.
         *  IMPORTANT: 'pixel_data' must continue to point to 
         *             the actual pixel data until the END OF THE
         *             PROGRAM, otherwise a seg fault may occur. 
         *  Note: no checking is done for duplicate images, if you are
         *        not careful you could have the same image registered
         *        under different image IDs. It is your responsibility
         *        to avoid this happening.
         */
        MCK_IMG_ID_TYPE create_custom_image(
            const std::vector<uint8_t>* pixel_data,
            uint8_t bits_per_pixel,
            uint16_t width_in_pixels,
            uint16_t height_in_pixels
        );

        //! Create render info for an extended ascii character
        // @param ascii_value: ASCII code (0-255) of desired char
        // @param local_palette_id: ID of existing local colo(u)r palette
        // @param x_pos: Hoz screen pos of image (excluding any block offsets) 
        // @param x_pos: Vert screen pos of image (excluding any block offsets) 
        // @param width_in_pixels: image width, in window pixels
        // @param height_in_pixels: image height, in window pixels
        // @param owning_block: Pointer to block to which image is assigned (if points to NULL, image will be an *orphan*)
        // @param ascii_set: ASCII set (zero is default)
        // Notes: Orphan images will not be visible until they are assigned to an active block.
        //        First local palette colo(u)r is background
        //        Second local palette colo(u)r is foreground
        //        Any other local palette colo(u)rs are ignored
        std::shared_ptr<MCK::GameEngRenderInfo> create_extended_ascii_render_info(
            uint8_t ascii_value,
            MCK_PAL_ID_TYPE local_palette_id,
            int x_pos,
            int y_pos,
            uint16_t width_in_pixels,
            uint16_t height_in_pixels,
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
            uint8_t ascii_set = 0
        ) const
        {
            // Let calling program handle any exception here,
            // including any exception from 'get_ascii_image_id'
            return this->create_render_info(
                get_ascii_image_id( ascii_value, ascii_set ),
                local_palette_id,
                x_pos,
                y_pos,
                width_in_pixels,
                height_in_pixels,
                parent_block
            );
        }

        //! Create render info for specified image and colo(u)r palette 
        // @param image_id: ID of existing image
        // @param local_palette_id: ID of existing local colo(u)r palette
        // @param x_pos: Hoz screen pos of image (excluding any block offsets) 
        // @param x_pos: Vert screen pos of image (excluding any block offsets) 
        // @param width_in_pixels: image width, in window pixels
        // @param height_in_pixels: image height, in window pixels
        // @param owning_block: Pointer to block to which image is assigned (if points to NULL, image will be an *orphan*)
        // Notes: Orphan images will not be visible until they are assigned to an active block.
        std::shared_ptr<MCK::GameEngRenderInfo> create_render_info(
            MCK_IMG_ID_TYPE image_id,
            MCK_PAL_ID_TYPE local_palette_id,
            int x_pos,
            int y_pos,
            uint16_t width_in_pixels,
            uint16_t height_in_pixels,
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block
        ) const;

        //! Change texture of render info object
        /*! @param info: Pointer to render info object
         *  @param image_id: ID of (an existing) image that will be used for the new texture
         *  @param local_palette_id: ID of existing local colo(u)r palette, used for new texture
         *  @param keep_orignal_dest_rect_size: If true, size of destination rectangle is preserved, even if new texture is a different size
         */
        void change_render_info_tex(
            std::shared_ptr<MCK::GameEngRenderInfo> info,
            MCK_IMG_ID_TYPE image_id,
            MCK_PAL_ID_TYPE local_palette_id,
            bool keep_orig_dest_rect_size = false
        ) const;

        //! Assign new ASCII texture to render info object, keeping original dest_rect size
        /*! @param info: Pointer to render info object
         *  @param ascii_value: ASCII code (0-255) of desired char
         *  @param local_palette_id: ID of existing local colo(u)r palette, used for new texture
         */
        void change_render_info_ascii_value(
            std::shared_ptr<MCK::GameEngRenderInfo> info,
            uint8_t ascii_value,
            MCK_PAL_ID_TYPE local_palette_id,
            uint8_t ascii_set = 0
        ) const
        {
            // Let calling program handle any exception here,
            // including any exception from 'get_ascii_image_id'
            this->change_render_info_tex(
                info,
                get_ascii_image_id( ascii_value, ascii_set ),
                local_palette_id,
                true // keep original dest_rect size
            );
        }


        //! Create an alternatve ASCII code to image ID mapping
        /*! @param image_ids: Vector of character image IDs, indexed by ASCII code
         *  @returns: ID of this alternate ASCII set (1-255).
         *  Note 1: 'image_ids' must be on length 128 or 256.
         *  Note 2: Only 254 alternative mappings (each of 128
         *          or 256 elements) are permitted, hence the
         *          8bit ID code.
         *  Note 3: For safety, the supplied vector will be
         *          copied and stored, so you are free to delete it.
         *  Note 4: All image IDs in vector must correspond to
         *          images that currently exist, so create
         *          all your alternative ascii characters *before*
         *          you call this method.
         *  Where an image ID is given as INVALID_IMG_ID, the
         *  alternative mapping will be ignored and the default
         *  extended ASCII image used. Similarly, if ASCII codes
         *  128-255 are used with a mapping that is only 128
         *  items in length, the default extended ASCII image
         *  will be used.
         */
        uint8_t create_alt_ascii_set(
            const std::vector<MCK_IMG_ID_TYPE> &ascii_to_image_id_mapping
        );

        //! Check if ASCII set ID is valid
        bool ascii_set_valid( uint8_t ascii_set ) const noexcept
        {
             return ascii_set < this->ascii_mappings.size();
        }


    private:

        //! Private struct used to store metadata for a sequence of images
        struct ImageMetaData
        {
            //! Shared pointer to pixel data
            std::shared_ptr<const std::vector<uint8_t>> pixel_data;

            //! Alternative pointer to pixel data when shared pointer not appropriate
            const std::vector<uint8_t>* alt_pixel_data;

            //! Default constructor
            ImageMetaData( void )
            {
                this->info = 0;
                this->alt_pixel_data = NULL;
            }

            //! Constructor (shared pointer version)
            /*! Note: Only valid values for _bits_per_pixel are 1, 2 and 4
             *  Note: Only lowest 12 bits of pitch and height are used
             */
            ImageMetaData(
                std::shared_ptr<const std::vector<uint8_t>> _pixel_data,
                uint8_t _bits_per_pixel,
                uint16_t _pitch_in_pixels,
                uint16_t _height_in_pixels
            )
            {
                this->pixel_data = _pixel_data;
                this->alt_pixel_data = NULL;
                this->info = 0 
                       | _bits_per_pixel
                       | ( uint32_t( ( _pitch_in_pixels % 0x0FFF ) ) << 8 )
                       | ( uint32_t( ( _height_in_pixels % 0x0FFF ) ) << 20 );
                this->alt_pixel_data = NULL;
            }

            //! Constructor (non-shared pointer version)
            /*! Note: Only valid values for _bits_per_pixel are 1, 2 and 4
             *  Note: Only lowest 12 bits of pitch and height are used
             */
            ImageMetaData(
                const std::vector<uint8_t>* _alt_pixel_data,
                uint8_t _bits_per_pixel,
                uint16_t _pitch_in_pixels,
                uint16_t _height_in_pixels
            )
            {
                this->alt_pixel_data = _alt_pixel_data;
                this->info = 0 
                       | _bits_per_pixel
                       | ( uint32_t( ( _pitch_in_pixels % 0x0FFF ) ) << 8 )
                       | ( uint32_t( ( _height_in_pixels % 0x0FFF ) ) << 20 );
            }

            //! Get bits per pixel
            /* Note:only values 1, 2 and 4 are valid,
             * all other values should be considered invalid
             */
            uint8_t get_bits_per_pixel( void ) const noexcept
            {
                return this->info & 0xFF;
            }

            //! Get pitch in pixels
            /* Note: only first 12 bits are used */
            uint16_t get_pitch_in_pixels( void ) const noexcept
            {
                return ( this->info >> 8 ) & 0x0FFF;
            }

            //! Get height in pixels
            /* Note: only first 12 bits are used */
            uint16_t get_height_in_pixels( void ) const noexcept
            {
                return ( this->info >> 20 ) & 0x0FFF;
            }

            private:

                // bits 0-7: Bit depth (only values 1, 2 and 4 valid)
                // bits 8-19: Image width
                // bits 20-31: Image height
                uint32_t info;
        };

        //! Extended ASCII images are numbered sequentially from this value
        // MCK_IMG_ID_TYPE ascii_image_id_base;

        //! Private method for creating texture and render info
        std::shared_ptr<MCK::GameEngRenderInfo> create_texture_and_render_info(
            MCK_IMG_ID_TYPE image_id,
            MCK_PAL_ID_TYPE local_palette_id,
            uint8_t bits_per_pixel,
            uint16_t pitch_in_pixels,
            uint16_t expected_height_in_pixels,
            const std::vector<uint8_t> &pixel_data,
            MCK::GameEngRenderInfo::Rect dest_rect,
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block
        ) const;

        //! Private method for checks shared by both versions of create_custom_image
        void create_custom_image_quality_checks(
            uint8_t bits_per_pixel,
            uint16_t width_in_pixels,
            uint16_t height_in_pixels,
            size_t pixel_data_size
        );

        //! Private method to obtain image ID for given ASCII code and ASCII set
        MCK_IMG_ID_TYPE get_ascii_image_id(
            uint8_t ascii_val,
            uint8_t ascii_set
        ) const;


        bool initialized;

        GameEng* game_eng;

        //! Store pointers to local palettes, indexed by their id 
        std::vector<std::shared_ptr<std::vector<uint8_t>>> palettes_by_id;

        //! Store pointers to image data, indexed by image ID
        std::vector<MCK::ImageMan::ImageMetaData> image_meta_data_by_id;

        //! Store pointer to vector(s) that link ascii codes to image data
        std::vector<std::vector<MCK_IMG_ID_TYPE>> ascii_mappings;

        // Constructor/destructor/copy/assignment/
        // initializer made private to avoid misuse.
        // Use public 'get_singleton' method instead
        
        ImageMan( void );
        ~ImageMan( void ) {};

        ImageMan(ImageMan const&) = delete;
        void operator=(ImageMan const&)  = delete;
};

}  // End of namespace MCK

#endif
