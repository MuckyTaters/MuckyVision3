////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  GameEngSpriteFactory.h
//
//  Class template (with direct access to 
//  SDL2 and MCK::GameEng) for creating
//  sprites.
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

#ifndef MCK_GAME_ENG_SPR_FCT_H
#define MCK_GAME_ENG_SPR_FCT_H

// SDL includes
#ifdef MCK_MINGW
// This is required if cross-compiling for Windows *on Linux*
#define SDL_MAIN_HANDLED  // Tells g++ to ignore SDL's own main
#include <SDL.h>
#endif
#ifndef MCK_MINGW
// This is required if compiling on Linux or Windows (MinGW)
#include <SDL2/SDL.h>
#endif

#include<GameEng.h>

#if defined MCK_STD_OUT
#include <iostream> // For std::cout
#endif

namespace MCK
{

template<class MOTION, class ANIM, class COLL, class RENDER>
class GameEngSpriteFactory
{
    public:
    
        static std::shared_ptr<Sprite<MOTION,ANIM,COLL,RENDER>> create_sprite(
            GameEng &game_eng,
            MCK_TEX_ID_TYPE initial_tex_id,
            std::shared_ptr<MCK::GameEngRenderBlock> parent_block,
            int initial_pos_x,
            int initial_pos_y,
            uint16_t width_in_pixels,
            uint16_t height_in_pixels,
            uint32_t z = MCK::DEFAULT_Z_VALUE
        )
        {
            if( !game_eng.initialized )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    "Cannot create sprite as GameEng not yet init."
#else
                    ""
#endif
                ) );
            }

            std::shared_ptr<MCK::Sprite<MOTION,ANIM,COLL,RENDER>> new_sprite
                = std::make_shared<MCK::Sprite<MOTION,ANIM,COLL,RENDER>>( z );

            // Let calling method catch any exception raised here
            GameEngSpriteFactory::_init_render_instance(
                new_sprite,
                game_eng,
                initial_tex_id,
                initial_pos_x,
                initial_pos_y,
                width_in_pixels,
                height_in_pixels
            );

            // Associate info with render block, if render block
            // supplied (note, always added to end of render block)
            if( parent_block.get() != NULL )
            {
                new_sprite->RENDER::parent_block
                    = parent_block.get();

                try
                {
                    parent_block->render_instances.insert(
                        std::pair<
                            uint64_t,
                            std::shared_ptr<MCK::GameEngRenderBase>
                        >( 
                            new_sprite->RENDER::render_order, 
                            std::dynamic_pointer_cast<MCK::GameEngRenderBase>(
                                new_sprite
                            )
                        )    
                    );
                }
                catch( const std::exception &e )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Failed to insert new sprite " )
                        + std::string( "into parent block, error = " )
                        + e.what()
#else
                        ""
#endif
                    ) );
                }
            }

            return new_sprite;
        }


    private:

        // Initialise components inherited from GameEngRenderInfo 
        template<typename U = RENDER, typename std::enable_if<std::is_same<U,MCK::GameEngRenderInfo>::value, bool>::type = true>
        static void _init_render_instance(
            std::shared_ptr<MCK::Sprite<MOTION,ANIM,COLL,U>> new_sprite,
            MCK::GameEng &game_eng,
            MCK_TEX_ID_TYPE initial_tex_id,
            int initial_pos_x,
            int initial_pos_y,
            uint16_t width_in_pixels,
            uint16_t height_in_pixels
        )
        {
            // Get intial texture
            SDL_Texture* initial_tex = NULL;
            {
                std::map<MCK_TEX_ID_TYPE,SDL_Texture*>::const_iterator it 
                    = game_eng.textures.find( initial_tex_id );
                if( it == game_eng.textures.end() )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Cannot create sprite as texture with id " )
                        + std::to_string( initial_tex_id )
                        + std::string( "does not exist." )
#else
                        ""
#endif
                    ) );
                }
                initial_tex = it->second;
                if( initial_tex == NULL )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Cannot create sprite as texture with id " )
                        + std::to_string( initial_tex_id )
                        + std::string( "is NULL." )
#else
                        ""
#endif
                    ) );
                }
            }

            new_sprite->RENDER::tex = initial_tex;
            new_sprite->RENDER::dest_rect.set_all(
                initial_pos_x,
                initial_pos_y,
                width_in_pixels,
                height_in_pixels
            );
        }
        
        // Initialise components inherited from GameEngRenderBlock
        template<typename U = RENDER, typename std::enable_if<std::is_same<U,MCK::GameEngRenderBlock>::value, bool>::type = true>
        static void _init_render_instance(
            std::shared_ptr<MCK::Sprite<MOTION,ANIM,COLL,U>> new_sprite,
            GameEng &game_eng,
            MCK_TEX_ID_TYPE initial_tex_id,
            int initial_pos_x,
            int initial_pos_y,
            uint16_t width_in_pixels,
            uint16_t height_in_pixels
        )
        {
            // TODO
        }
};

}  // End of namespace MCK

#endif
