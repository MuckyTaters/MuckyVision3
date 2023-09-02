////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  VoiceBase.h
//
//  Abstract base class for voices.
//  Header only (there is no VoiceBase.cpp).
//
//  Instances of voice classes are passed
//  to the audio callback thread when
//  audio is initialised, but thereafter
//  these instances are not touched
//  by the main game thread. Any
//  communication from the main thread
//  to these instances goes through the
//  ring buffer in GameEngAudio. No
//  information can be safely passed
//  back to the main thread.
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

#ifndef MCK_VOICE_BASE_H
#define MCK_VOICE_BASE_H

// This include must come first
#include "Version.h"

#if defined MCK_STD_OUT
#include <iostream> // For std::cout
#endif

#include <stdexcept>  // For exceptions
#include <memory>  // For shared_ptr

#include "Defs.h"

namespace MCK
{

class VoiceBase
{
    public:

        VoiceBase( void )
        {
            this->type = MCK::VoiceType::BASE;
            this->initialized = false;
        }

        virtual ~VoiceBase( void ) {}

        bool is_initialized( void ) const noexcept
        {
            return initialized;
        }

        //! Issue a command to the voice
        /* Commands are defined in the child class */
        virtual void command( 
            uint8_t com,
            uint64_t sample_count
        ) = 0;

        //! Get sample at given time point
        /*! Return value must lie within range [-v,v]
         *  where 'v' = channel volume (0-255) / 255 / num of virtual channels
         */
        virtual float get_sample( uint64_t sample_count ) = 0;


    protected:

        //! Internal initialisation method, called by
        //  'init' method of child classes.
        void base_init(
            uint8_t initial_volume = 0xFF
        )
        {
            scale = float( initial_volume )
                      / 255.0f 
                        / float( MCK_NUM_VOICES );
            initialized = true;
        }

        MCK::VoiceType type;

        bool initialized;
            
        // This 'scale' variable takes account of voice volume
        // and number of voices, so that returned sample values are 
        // appropriately scaled. This means the returned voice
        // sample can simply be added to the total (all voice)
        // sample value without any further multiplication.
        float scale;

    private:

        // Copy and assignment constructor hidden to
        // prevent misuse.
        VoiceBase(VoiceBase const&) = delete;
        void operator=(VoiceBase const&)  = delete;
};

}  // End of namespace MCK

#endif
