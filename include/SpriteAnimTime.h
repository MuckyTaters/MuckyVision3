////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpriteAnimTime.h
//
//  Temporal animation class for sprite images. 
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

#ifndef MCK_SPRITE_ANIM_TIME_H
#define MCK_SPRITE_ANIM_TIME_H

#include "Defs.h"
#include "SpriteFrame.h"
#include "SpriteAnimBase.h"

namespace MCK
{

class SpriteAnimTime : public SpriteAnimBase
{
    public:

        SpriteAnimTime( void ) : SpriteAnimBase()
        {
            this->type = MCK::SpriteAnimType::TEMPORAL;
        }

        bool has_frames( void ) const noexcept
        {
            return this->frames.size() > 0;
        }

        //! Create new frame set
        void set_frames(
            std::vector<MCK::SpriteFrame> _frames,
            size_t starting_frame_num = 0
        )
        {
            if( _frames.size() == 0 )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    "Cannot set frames as frame vector is empty."
#else
                    ""
#endif
                ) );
            }

            if( MCK::SpritePos::image_man == NULL )
            {
                throw( std::runtime_error(
#if defined MCK_STD_OUT
                    "Cannot set frames as SpritePos::image_man is NULL."
#else
                    ""
#endif
                ) );
            }

            // Dummy pointer, pointing to NULL by default
            // Explainer: using a NULL render info pointer
            //            in the method call below instructs
            //            the method to check/create texture only,
            //            without affecting any actual render
            //            info instance.
            std::shared_ptr<MCK::GameEngRenderInfo> null_info_ptr;

            // Create frame textures if necessary
            for( auto &frm : _frames )
            {
                try
                {
                    // Check/create texture (see explainer above)
                    MCK::SpritePos::image_man->change_render_info_tex(
                        null_info_ptr,
                        frm.image_id,
                        frm.palette_id
                    );
                }
                catch( std::exception &e )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        std::string( "Cannot set frame as texture " )
                        + std::string( "check/creation failed, error = " )
                        + e.what()
#else
                        ""
#endif
                    ) );
                }
            }

            std::swap( this->frames, _frames );
            this->frame_num = starting_frame_num % this->frames.size();
            this->select_frame( this->frame_num );
        }   

        void next_frame( void )
        {
            this->select_frame( this->frame_num + 1 );
        }

        void select_frame( size_t _frame_num )
        {
            // If there are no frames, no nothing
            if( this->frames.size() == 0 )
            {
                return;
            }

            // Set new frame number (wrap around if necessary)
            this->frame_num = _frame_num % this->frames.size();

            // Get shortcut to new frame
            const SpriteFrame* const FRM
                = &this->frames[ this->frame_num ];
                
            // Set time
            this->ticks_to_next_frame
                = this->frames[ this->frame_num ].duration_in_ticks; 
       
            // Set texture
            if( this->SpritePos::update_render_instance )
            {
                if( MCK::SpritePos::image_man == NULL )
                {
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        "Cannot set new render texture as 'SpritePos::image_man' NULL."
#else
                        ""
#endif
                    ) );
                }

                if( this->SpritePos::render_instance.get() != NULL
                    && this->SpritePos::render_instance->get_type()
                        == MCK::RenderInstanceType::INFO
                )
                {
                    try
                    {
                        // GameEngRenderInfo
                        image_man->change_render_info_tex(
                            std::dynamic_pointer_cast<
                                MCK::GameEngRenderInfo
                            >( this->SpritePos::render_instance ),
                            FRM->image_id,
                            FRM->palette_id,
                            FRM->keep_orig_dest_rect_size
                        );
                    }
                    catch( std::exception &e )
                    {
                        throw( std::runtime_error(
#if defined MCK_STD_OUT
                            std::string( "Cannot update render info " )
                            + std::string( "for select frame, error =  " )
                            + e.what()
#else
                            ""
#endif
                        ) );
                    }
                }
                else
                {
                    // TODO: GameEngRenderBlock
                    throw( std::runtime_error(
#if defined MCK_STD_OUT
                        "Cannot set new texture."
#else
                        ""
#endif
                    ) );
                }
            }
        }

        virtual void calc_frame( void )
        {
            uint32_t ticks = MCK::SpritePos::ticks_elapsed;
            while( ticks > this->ticks_to_next_frame )
            {
                ticks -= this->ticks_to_next_frame;
                this->select_frame( this->frame_num + 1 );
            }
            this->ticks_to_next_frame -= ticks;
        }

    protected:

        std::vector<MCK::SpriteFrame> frames;

        size_t frame_num;

        uint32_t ticks_to_next_frame;
};

}  // End of namespace MCK

#endif
