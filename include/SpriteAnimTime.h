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
            this->anim_type = MCK::SpriteAnimType::TEMPORAL;
            this->use_offsets = false;
        }

        bool has_frames( void ) const noexcept
        {
            return this->frames.size() > 0;
        }

        //! Create new frame set
        virtual void set_frames(
            std::vector<MCK::SpriteFrame> _frames,
            size_t starting_frame_num = 0,
            bool _use_offsets = false
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

            this->use_offsets = _use_offsets;

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

        virtual void select_frame( size_t _frame_num )
        {
            // If there are no frames, no nothing
            if( this->frames.size() == 0 )
            {
                return;
            }

            // Get pointer to existing frame
            const MCK::SpriteFrame* PREV_FRM
                = this->frame_num < frames.size() ?
                    &this->frames.at( this->frame_num ) : NULL;

            // Set new frame number (wrap around if necessary)
            this->frame_num = _frame_num % this->frames.size();

            // Get shortcut to new frame
            const SpriteFrame* const FRM
                = &this->frames[ this->frame_num ];
       
            // Update render instance
            if( this->SpritePos::update_render_instance )
            {
                // Update texture
                if(
                    PREV_FRM == NULL
                    || PREV_FRM->image_id != FRM->image_id
                    || PREV_FRM->palette_id != FRM->palette_id
                )
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
                                FRM->keep_orig_dest_rect_width,
                                FRM->keep_orig_dest_rect_height
                            );

                            // DEBUG
                            MCK::GameEngRenderInfo::Rect dr
                                = std::dynamic_pointer_cast<
                                    MCK::GameEngRenderInfo
                                >( this->SpritePos::render_instance )->dest_rect;
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

                // Adjust offsets
                if( this->use_offsets 
                    && this->SpritePos::render_instance.get() != NULL
                )
                {
                    this->SpritePos::render_instance->set_pos(
                        int( this->SpritePos::pos.get_x() + 0.5f )
                            + FRM->offset_x,
                        int( this->SpritePos::pos.get_y() + 0.5f )
                            + FRM->offset_y
                    );
                }
                
                // Set ticks/distance/etc. for next frame
                this->steps_to_next_frame
                    = this->frames[ this->frame_num ].duration; 
            
                // Update flags
                if( this->MCK::SpritePos::render_instance != NULL
                    && this->MCK::SpritePos::render_instance->get_type()
                        == MCK::RenderInstanceType::INFO
                )
                {
                    std::static_pointer_cast<MCK::GameEngRenderInfo>(
                        this->MCK::SpritePos::render_instance
                    )->set_flags( FRM->flags );
                }
            }
        }

        //! Calculate next frame, baseed on current ticks
        /*! Note: Frames with 0 duration will be displayed
         *        for one frame only before proceeding to the
         *        next. This is to prevent an infinite loop
         *        occuring if all frame durations are set
         *        to zero.
         */
        virtual void calc_frame( void )
        {
            // Change frame until ticks used up
            uint32_t ticks = MCK::SpritePos::ticks_elapsed;

            while( ticks > this->steps_to_next_frame )
            {
                ticks -= this->steps_to_next_frame;
                
                this->select_frame( this->frame_num + 1 );
                
                // Safety check, to prevent infinite loop
                if( this->steps_to_next_frame == 0 ) { break; }
            }
            this->steps_to_next_frame -= ticks;
        }

        //! Returns true if sprite uses offsets
        bool uses_offsets( void ) const noexcept
        {
            return this->use_offsets;
        }


    protected:

        std::vector<MCK::SpriteFrame> frames;

        size_t frame_num;

        uint32_t steps_to_next_frame;

        bool use_offsets;
};

}  // End of namespace MCK

#endif
