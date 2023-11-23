////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  ImageMan.cpp
//
//  Imager Manager singleton class,
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

#include "ImageMan.h"

MCK::ImageMan::ImageMan( void )
{
    // Set all variables to invalid/default values
    this->game_eng = NULL;
}

void MCK::ImageMan::init(
    GameEng &_game_eng
)
{
    if( this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot intialize Image Manager as already init."
#else
            ""
#endif
        ) );
    }

    if( !_game_eng.is_initialized() )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot initialize Image Manager as GameEng not init."
#else
            ""
#endif
        ) );
    }
    this->game_eng = &_game_eng;

    // Reserve space for local colo(u)r palette index
    this->palettes_by_id.reserve( MCK::MAX_LOCAL_COLOR_PALETTES );

    // Reserve space for image metadata 
    this->image_meta_data_by_id.reserve( MCK::MAX_IMAGES ); 
    
    // Create meta data for ascii image pixel data
    // As this pixel data is stored in the executable
    // we can safely use a conventional pointer (creating
    // a shared pointer would required copying this data
    // to heap memory, duplicating it).
    for( const std::vector<uint8_t> &data : MCK::ImageDataASCII::image_data )
    {
        this->image_meta_data_by_id.push_back(
            MCK::ImageMan::ImageMetaData(
                &data,
                MCK::ImageDataASCII::BITS_PER_PIXEL,
                MCK::ImageDataASCII::PITCH_IN_PIXELS,
                MCK::ImageDataASCII::HEIGHT_IN_PIXELS
            )
        );
    }

    // Create first item in 'ascii_mappings'
    // This the default mapping for the (extended) ascii image set
    this->ascii_mappings.clear();  // Not strictly necessary
    this->ascii_mappings.push_back(
        std::vector<MCK_IMG_ID_TYPE>( 256 )
    );

    // Get pointer to this
    std::vector<MCK_IMG_ID_TYPE>* const MAPPING
        = &ascii_mappings.back();

    // Fill mapping with default ASCII image IDs
    MCK_IMG_ID_TYPE ascii_image_id = 0;
    for( MCK_IMG_ID_TYPE &id : *MAPPING ) { id = ascii_image_id++; }

    this->initialized = true;
}

MCK_PAL_ID_TYPE MCK::ImageMan::create_local_palette(
    const std::shared_ptr<std::vector<uint8_t>> global_color_ids
)
{
    if( !this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create local palette as Image Manager not yet init."
#else
            ""
#endif
        ) );
    }

    const size_t SIZE = global_color_ids->size();

    if( SIZE == 0 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot create local palette as " )
            + std::string( "color id vector has zero size." )
#else
            ""
#endif
        ) );
    }

    // Loop over existing palettes and check for match
    const MCK_PAL_ID_TYPE NUM_PAL = this->palettes_by_id.size();
    for( MCK_PAL_ID_TYPE id = 0; id < NUM_PAL; id++ )
    {
        // Get pointer to palette
        const std::shared_ptr<std::vector<uint8_t>> PALETTE
            = this->palettes_by_id[id];

        // Ignore NULL pointers
        if( PALETTE.get() == NULL )
        {
            continue;
        }

        // If different length, no match
        if( PALETTE->size() != SIZE )
        {
            continue;
        }

        // If size matches, loop over elements
        size_t count = 0;
        bool match = true;
        for( const uint8_t COLOR_ID : *PALETTE )
        {
            if( global_color_ids->at(count) != COLOR_ID )
            {
                match = false;
                break;
            }
        }

        if( match )
        {
            // Return ID of existing palette
            return id;
        }
    }

    // Unlikey, but check if we've run out of palette IDs
    if( this->palettes_by_id.size() == MCK::INVALID_IMG_ID )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create local palette as run out of palette IDS."
#else
            ""
#endif
        ) );
    }

    // If we get to this point, assume this local
    // palette is new, and assign it a new ID
    this->palettes_by_id.push_back( global_color_ids );

    // Return index of new palette as ID
    return this->palettes_by_id.size() - 1;
}

std::shared_ptr<MCK::GameEngRenderInfo> MCK::ImageMan::create_texture_and_render_info(
    MCK_IMG_ID_TYPE image_id,
    MCK_PAL_ID_TYPE local_palette_id,
    uint8_t bits_per_pixel,
    uint16_t pitch_in_pixels,
    uint16_t expected_height_in_pixels,
    const std::vector<uint8_t> &pixel_data,
    MCK::GameEngRenderInfo::Rect dest_rect,
    std::shared_ptr<MCK::GameEngRenderBlock> parent_block
) const
{
    if( !this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Image Manager not yet init."
#else
            ""
#endif
        ) );
    }

    // Ensure that image ID is not invalid
    if( image_id == MCK::INVALID_IMG_ID )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot create image as image ID is invalid (" )
            + std::to_string( MCK::INVALID_IMG_ID )
            + std::string( ")" )
#else
            ""
#endif
        ) );
    }

    // Check palette id is valid
    if( local_palette_id >= this->palettes_by_id.size() )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Local palette ID " )
            + std::to_string( local_palette_id )
            + std::string( " is invalid, cannot create ASCII " )
            + std::string( "character." )
#else
            ""
#endif
        ) );
    }

    // Create texture
    uint16_t height_in_pixels = 0;
    MCK_TEX_ID_TYPE tex_id;
    try
    {
        this->game_eng->create_texture(
            image_id,
            local_palette_id,
            bits_per_pixel,
            pitch_in_pixels,
            pixel_data,
            *this->palettes_by_id[ local_palette_id ],
            tex_id,
            height_in_pixels
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot create image as texture " )
            + std::string( "creation failed, error = " )
            + e.what()
#else
            ""
#endif
        ) );
    }
    if( height_in_pixels != expected_height_in_pixels )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot create image as texture's " )
            + std::string( "height in pixels (" )
            + std::to_string( height_in_pixels )
            + std::string( ") not as expected (" )
            + std::to_string( expected_height_in_pixels )
            + std::string( ")." )
#else
            ""
#endif
        ) );
    }

    // Create image struct    
    std::shared_ptr<MCK::GameEngRenderInfo> ans;
    try
    {
        ans = this->game_eng->create_render_info(
            tex_id,
            parent_block,
            dest_rect,
            false  // No clip
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot create image as " )
            + std::string( "GameEng::create_render_info " )
            + std::string( "failed, error = " )
            + e.what()
#else
            ""
#endif
        ) );
    }

    return ans;
}

MCK_IMG_ID_TYPE MCK::ImageMan::create_custom_image(
    std::shared_ptr<const std::vector<uint8_t>> pixel_data,
    uint8_t bits_per_pixel,
    uint16_t width_in_pixels,
    uint16_t height_in_pixels
)
{
    // Initialisation check done by 
    // 'create_custom_image_quality_checks'
    // method

    ////////////////////////////////////////
    // Quality checks

    if( pixel_data.get() == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Can create custom image as pixel data pointer is NULL."
#else
            ""
#endif
        ) );
    }

    // If quality checks fail, this method will throw an exception.
    // No point catching it here.
    create_custom_image_quality_checks(
        bits_per_pixel,
        width_in_pixels,
        height_in_pixels,
        pixel_data->size()
    );

    try
    {
        this->image_meta_data_by_id.push_back(
            MCK::ImageMan::ImageMetaData(
                pixel_data,
                bits_per_pixel,
                width_in_pixels,
                height_in_pixels
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to create new image meta data entry, error = " )
            + e.what()
#else
            ""
#endif
        ) );
    }

    // Index of image data just added is image ID
    return this->image_meta_data_by_id.size() - 1;
}

MCK_IMG_ID_TYPE MCK::ImageMan::create_custom_image(
    const std::vector<uint8_t>* pixel_data,
    uint8_t bits_per_pixel,
    uint16_t width_in_pixels,
    uint16_t height_in_pixels
)
{
    // Initialisation check done by 
    // 'create_custom_image_quality_checks'
    // method

    ////////////////////////////////////////
    // Quality checks

    if( pixel_data == NULL )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Can create custom image as pixel data pointer is NULL."
#else
            ""
#endif
        ) );
    }

    // If quality checks fail, this method will throw an exception.
    // No point catching it here.
    create_custom_image_quality_checks(
        bits_per_pixel,
        width_in_pixels,
        height_in_pixels,
        pixel_data->size()
    );

    try
    {
        this->image_meta_data_by_id.push_back(
            MCK::ImageMan::ImageMetaData(
                pixel_data,
                bits_per_pixel,
                width_in_pixels,
                height_in_pixels
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to create new image meta data entry, error = " )
            + e.what()
#else
            ""
#endif
        ) );
    }

    // Index of image data just added is image ID
    return this->image_meta_data_by_id.size() - 1;
}

void MCK::ImageMan::create_custom_image_quality_checks(
    uint8_t bits_per_pixel,
    uint16_t width_in_pixels,
    uint16_t height_in_pixels,
    size_t pixel_data_size
)
{
    if( !this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Can create custom image as Image Manager not yet init."
#else
            ""
#endif
        ) );
    }

    if( pixel_data_size == 0 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Can create custom image as pixel data is empty."
#else
            ""
#endif
        ) );
    }

    if( !( 
            bits_per_pixel == 1
            || bits_per_pixel == 2
            || bits_per_pixel == 4
        )
    )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot create custom image as bits per pixel (" )
            + std::to_string( bits_per_pixel )
            + std::string( "is invalid." )
#else
            ""
#endif
        ) );
    }

    if( width_in_pixels == 0 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create custom image as pitch is zero."
#else
            ""
#endif
        ) );
    }

    if( height_in_pixels == 0 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create custom image as height is zero."
#else
            ""
#endif
        ) );
    }

    const size_t BIT_COUNT = pixel_data_size * 8;
    const size_t MAX_PERMISSIBLE_BIT_COUNT
        = bits_per_pixel * width_in_pixels * height_in_pixels;
    const size_t MIN_PERMISSIBLE_BIT_COUNT
        = bits_per_pixel
            * width_in_pixels
                * ( height_in_pixels - 1 ) + 1;
    if( BIT_COUNT > MAX_PERMISSIBLE_BIT_COUNT
        || BIT_COUNT < MIN_PERMISSIBLE_BIT_COUNT
    )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot create custom image as pixel " )
            + std::string( "data size is not compatible with " )
            + std::string( "bits per pixels and/or pitch and/or height." )
#else

            ""
#endif
        ) );
    }
}

std::shared_ptr<MCK::GameEngRenderInfo> MCK::ImageMan::create_render_info(
    MCK_IMG_ID_TYPE image_id,
    MCK_PAL_ID_TYPE local_palette_id,
    int x_pos,
    int y_pos,
    uint16_t width_in_pixels,
    uint16_t height_in_pixels,
    std::shared_ptr<MCK::GameEngRenderBlock> parent_block
) const
{
    if( !this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create render info as Image Manager not yet init."
#else
            ""
#endif
        ) );
    }

    // Make sure image ID is valid
    if( image_id >= this->image_meta_data_by_id.size() )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot create render info as image ID " )
            + std::to_string( image_id ) 
            + std::string( " not recognized." )
#else
            ""
#endif
        ) );
    }

    // Get pointer to image meta data
    // (safe because of above check)
    const MCK::ImageMan::ImageMetaData* const META_DATA
        = &this->image_meta_data_by_id[ image_id ];

    // Make sure palette ID is valid
    if( local_palette_id >= this->palettes_by_id.size() )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot change render info texture as local palette ID " )
            + std::to_string( local_palette_id ) 
            + std::string( " not recognized." )
#else
            ""
#endif
        ) );
    }

    // Calculate texture ID
    const MCK_TEX_ID_TYPE TEX_ID
        = this->game_eng->calc_tex_id(
            image_id,
            local_palette_id
        );

    // Declare return value
    std::shared_ptr<MCK::GameEngRenderInfo> ans;

    // Create destination rectangle
    // Note: other render info (such as clip, flip
    // and rotate) can be set manually once the
    // render info struct has been obtained, however
    // it makes sense to require a destination rectangle
    // as texture cannot not be rendered without it.
    const MCK::GameEngRenderInfo::Rect DEST_RECT(
        x_pos,
        y_pos,
        width_in_pixels,
        height_in_pixels
    );

    // Determine (read-only) pointer to pixel data
    const std::vector<uint8_t>* pixel_data_ptr;
    if( META_DATA->pixel_data.get() != NULL )
    {
        // Extract pointer from shared pointer, if valid
        pixel_data_ptr = META_DATA->pixel_data.get();
    }
    else if( META_DATA->alt_pixel_data != NULL )
    {
        // Use non-shared pointer instead
        pixel_data_ptr = META_DATA->alt_pixel_data;
    }
    else
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to create texture as " )
            + std::string( "both pixel data pointer and alt " )
            + std::string( "pixel data pointer are NULL. " )
#else
            ""
#endif
        ) );
    }


    // If texture doesn't already exist, create it
    if( !this->game_eng->texture_exists( TEX_ID ) )
    {
        // Create texture (in case it doesn't exist already)
        try
        {
            ans = this->create_texture_and_render_info(
                image_id,
                local_palette_id,
                META_DATA->get_bits_per_pixel(),
                META_DATA->get_pitch_in_pixels(),
                META_DATA->get_height_in_pixels(),
                *pixel_data_ptr,
                DEST_RECT, 
                parent_block
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Attempt to create render info has failed, error = " )
                + e.what()
#else
                ""
#endif
            ) );
        }
    }
    else
    {
        // If texture exists, just create image
        try
        {
            ans = this->game_eng->create_render_info(
                TEX_ID,
                parent_block,
                DEST_RECT,
                false  // No clip
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Attempt to create render info has failed, error = " )
                + e.what()
#else
                ""
#endif
            ) );
        }
    }

    return ans;
}

void MCK::ImageMan::change_render_info_tex(
    std::shared_ptr<MCK::GameEngRenderInfo> info,
    MCK_IMG_ID_TYPE image_id,
    MCK_PAL_ID_TYPE local_palette_id,
    bool keep_orig_dest_rect_width,
    bool keep_orig_dest_rect_height
) const
{
    if( !this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot change render info texture as Image Manager not yet init."
#else
            ""
#endif
        ) );
    }

    // Make sure image ID is valid
    if( image_id >= this->image_meta_data_by_id.size() )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot change render info texture as image ID " )
            + std::to_string( image_id ) 
            + std::string( " not recognized." )
#else
            ""
#endif
        ) );
    }

    // Get pointer to image meta data
    // (safe because of above check)
    const MCK::ImageMan::ImageMetaData* const META_DATA
        = &this->image_meta_data_by_id[ image_id ];

    // Determine (read-only) pointer to pixel data
    const std::vector<uint8_t>* pixel_data_ptr;
    if( META_DATA->pixel_data.get() != NULL )
    {
        // Extract pointer from shared pointer, if valid
        pixel_data_ptr = META_DATA->pixel_data.get();
    }
    else if( META_DATA->alt_pixel_data != NULL )
    {
        // Use non-shared pointer instead
        pixel_data_ptr = META_DATA->alt_pixel_data;
    }
    else
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "(2)Failed to create texture as " )
            + std::string( "both pixel data pointer and alt " )
            + std::string( "pixel data pointer are NULL. " )
#else
            ""
#endif
        ) );
    }

    // Make sure palette ID is valid
    if( local_palette_id >= this->palettes_by_id.size() )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot change render info texture as local palette ID " )
            + std::to_string( local_palette_id ) 
            + std::string( " not recognized." )
#else
            ""
#endif
        ) );
    }

    // Calculate texture ID
    const MCK_TEX_ID_TYPE TEX_ID
        = this->game_eng->calc_tex_id(
            image_id,
            local_palette_id
        );

    // If texture doesn't already exist, create it
    if( !this->game_eng->texture_exists( TEX_ID ) )
    {
        // Create texture
        uint16_t height_in_pixels = 0;
        MCK_TEX_ID_TYPE tex_id;
        try
        {
            this->game_eng->create_texture(
                image_id,
                local_palette_id,
                META_DATA->get_bits_per_pixel(),
                META_DATA->get_pitch_in_pixels(),
                *pixel_data_ptr,
                *palettes_by_id[ local_palette_id ],
                tex_id,
                height_in_pixels
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Cannot change image texture as texture " )
                + std::string( "creation failed, error = " )
                + e.what()
#else
                ""
#endif
            ) );
        }
        if( height_in_pixels != META_DATA->get_height_in_pixels() )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Cannot change image texture as texture's " )
                + std::string( "height in pixels (" )
                + std::to_string( height_in_pixels )
                + std::string( ") not as expected (" )
                + std::to_string( META_DATA->get_height_in_pixels() )
                + std::string( ")." )
#else
                ""
#endif
            ) );
        }
    }

    // Change render info texture, at GameEng level
    // (because only GameEng has access to the actual
    //  texture instance)
    if( info.get() != NULL )
    {
        try
        {
            this->game_eng->change_render_info_tex(
                info,
                TEX_ID
            );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Cannot change image texture as GameEng " )
                + std::string( "returned error: " )
                + e.what()
#else
                ""
#endif
            ) );
        }
    }

    // If necessary, update size of destination rect
    if( info.get() != NULL )
    {
        // If keeping neither width or height, just use
        // source image size
        if( !keep_orig_dest_rect_width
            && !keep_orig_dest_rect_height
        )
        {
            info->dest_rect.set_w( META_DATA->get_pitch_in_pixels() );
            info->dest_rect.set_h( META_DATA->get_height_in_pixels() );

        }
        // If keeping width, set height of new image using existing
        // horizontal scale
        else if( keep_orig_dest_rect_width )
        {
            // Adjust height, in proportion current hoz scale (sic)
            const int RAW_WIDTH = META_DATA->get_pitch_in_pixels();
            if( RAW_WIDTH > 0 )
            {
                const float HOZ_SCALE
                    = info->dest_rect.get_w() / RAW_WIDTH;
                info->dest_rect.set_h(
                    META_DATA->get_height_in_pixels() * HOZ_SCALE
                );
            }
            else
            {
                info->dest_rect.set_h(
                    META_DATA->get_height_in_pixels()
                );
            }
        }
        // If keeping height, set width of new image using existing
        // vertical scale
        else if( keep_orig_dest_rect_height )
        {
            // Adjust width, in proportion current vert scale (sic)
            const int RAW_HEIGHT = META_DATA->get_height_in_pixels();
            if( RAW_HEIGHT > 0 )
            {
                const float VERT_SCALE
                    = info->dest_rect.get_h() / RAW_HEIGHT;
                info->dest_rect.set_w(
                    META_DATA->get_pitch_in_pixels() * VERT_SCALE
                );
            }
            else
            {
                info->dest_rect.set_w(
                    META_DATA->get_pitch_in_pixels()
                );
            }
        }
    }
}

uint8_t MCK::ImageMan::create_alt_ascii_set(
    const std::vector<MCK_IMG_ID_TYPE> &ascii_to_image_id_mapping
)
{
    if( !this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create alt ascii set as Image Manager not yet init."
#else
            ""
#endif
        ) );
    }

    if( this->ascii_mappings.size() >= 255 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot change create alt ascii set there are already 255 ascii sets (including the default set)."
#else
            ""
#endif
        ) );
    }

    // Get size of supplied mapping
    const size_t SIZE = ascii_to_image_id_mapping.size();

    if( !( SIZE == 128 || SIZE == 256 ) )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot create alt ascii set as supplied vector not of size 128 or 256."
#else
            ""
#endif
        ) );
    }

    // Declare new mapping, of appropriate size
    this->ascii_mappings.push_back( std::vector<MCK_IMG_ID_TYPE>( SIZE )  );

    // Get pointer to new mapping
    std::vector<MCK_IMG_ID_TYPE>* const NEW_MAPPING
        = &this->ascii_mappings.back();
    
    // Iterate over supplied vector, checking each image ID
    // is valid and placing ID in 'MAPPING'
    // Note: we can safely ignore image IDs with value
    // MCK::INVALID_IMG_ID as this is the instruction to
    // use the image ID stored in default ASCII set instead.
    uint8_t ascii_val = 0;
    for( const MCK_IMG_ID_TYPE IMAGE_ID : ascii_to_image_id_mapping )
    {
        if( IMAGE_ID >= this->image_meta_data_by_id.size()
            && IMAGE_ID != MCK::INVALID_IMG_ID
        )
        {
#if defined MCK_STD_OUT
            // Report error and skip to next image ID
            std::cout << "Error: failed to create a mapping from "
                      << "ASCII code " << int( ascii_val )
                      << " to image ID " << IMAGE_ID
                      << " as that image ID is invalid."
                      << std::endl;
#endif

            // Use default ASCII character instead
            // (indicated by using the special value MCK::INVALID_IMG_ID)
            NEW_MAPPING->at( ascii_val++ ) = MCK::INVALID_IMG_ID;
        }
        else
        {
            // Store image id, indexed by ASCII value
            NEW_MAPPING->at( ascii_val++ ) = IMAGE_ID;
        }
    }

    // Return mapping id
    return this->ascii_mappings.size() - 1;
}

MCK_IMG_ID_TYPE MCK::ImageMan::get_ascii_image_id(
    uint8_t ascii_val,
    uint8_t ascii_set
) const
{
    // Set to default answer value
    MCK_IMG_ID_TYPE image_id = MCK::INVALID_IMG_ID;
    
    if( ascii_set >= this->ascii_mappings.size() )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "ASCII set " )
            + std::to_string( ascii_set ) 
            + std::string( " does not exist." )
#else
            ""
#endif
        ) );
    }

    // Get pointer to mapping
    const std::vector<MCK_IMG_ID_TYPE>* const MAPPING
        = &this->ascii_mappings[ ascii_set ];

    // If mapping does not extend to ascii_value, 
    // or mapped image id is invalid, use default ascii
    // character image instead.
    if( ascii_val < MAPPING->size()
        && MAPPING->at( ascii_val ) < this->image_meta_data_by_id.size()
    )
    {
        // Get ascii image
        image_id = MAPPING->at( ascii_val );
    }
    else
    {
        // Get default ascii image
        // (these vector look ups should be safe)
        image_id = this->ascii_mappings.at( 0 ).at( ascii_val );
    }

    return image_id;
}
