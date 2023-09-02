////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  GameEngAudio.h
//
//  Wrapper for SDL2 Audio, also provides
//  a buffer between main program and
//  audio callback.
//
//  THIS CLASS IS 100% STATIC, RATHER THAN
//  BEING A SINGLETON CLASS (IT HAS NO
//  CONSTRUCTOR). THIS IS FOR EASE OF
//  INTEGRATION WITH THE AUDIO CALLBACK,
//  WHICH IS STATIC
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

#ifndef MCK_GAME_ENG_AUDIO_H
#define MCK_GAME_ENG_AUDIO_H

// This include must come first
#include "Version.h"

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

#if defined MCK_STD_OUT
#include <iostream> // For std::cout
#endif

#include <stdexcept>  // For exceptions
#include <memory>  // For shared_ptr
#include <vector>  // For vector

#include "Defs.h"
#include "VoiceBase.h"
#include "VoiceSynth.h"

namespace MCK
{

class GameEngAudio
{
    public:

        //! Initialize SDL2 audio
        static void init(
            uint8_t _master_volume,
            std::vector<std::shared_ptr<MCK::VoiceBase>> &_voices
        );

        //! Issue a single voice command at a given time point (or a current time)
        /*! IMPORTANT: This method should only be called by the main thread
         *  @param voice_id: Voice (0 to MCK_NUM_VOICES - 1 )
         *  @param command: Command bytes, relevant to that voice.
         *  @param ticks: Future time point at which to issue command. If 0 (default), current time is used.
            Note: Time points too far into the future will generate an exception
         */
        static void voice_command(
            uint8_t voice_id,
            uint8_t command,
            uint32_t ticks = 0
        );


        //! THIS METHOD IS ONLY USED BY SDL, DO NOT CALL DIRECTLY
        static void callback(
            void *user_data,
            uint8_t *raw_buffer,
            int bytes
        );


    private:

        // All constructors/destructor removed
        // to avoid istantiation.
        GameEngAudio( void ) = delete;
        ~GameEngAudio( void ) = delete;
        GameEngAudio( GameEngAudio const& ) = delete;
        void operator=( GameEngAudio const& )  = delete;

        static bool initialized;

        static uint64_t sample_counter;
        
        // These values are determined by SDL during initialisation
        static int samples_per_second;
        static SDL_AudioFormat format;
        static uint8_t num_channels;
        static uint8_t silence_byte_value;
        static uint16_t buffer_size_in_samples;
        static uint32_t buffer_size_in_bytes;
        static uint16_t bytes_per_sample;
        static uint8_t bytes_per_channel;

        static MCK::AudioDataType data_type;

        // Voices, stored by *base* pointer
        static std::vector<std::shared_ptr<MCK::VoiceBase>> voices;

        //! Ring buffer, for communication with main thread
        /*! This is composed of 'n' entries containing 8 bytes each
         *  where 'n' is set at compile time and defined in Defs.h.
         *  The 'i'th byte from left/lowest ( 0 <= i < n ) is
         *  a command to the 'i'th voice. The meaning of the
         *  command depends on the voice, see each specific
         *  voice class for details.
         *  Note there is some latency as these commands have
         *  to sit in the buffer for at least a tick or two.
         *  The purpose of this buffer is to avoid the need
         *  for C++11 parallelisation (e.g. mutex), as MinGW 
         *  support for that is patchy.
         *  CAUTION: This is a C-style array, chosen because
         *           speed of read/write is critical here.
         *           It is only safe if used with care. 
         *           Do not alter the buffering
         *           code unless you know what you are doing.
         */
        static MCK_AUDIO_RING_BUFFER_DATA_TYPE ring_buffer[];

        //! Remember previous position in ring buffer
        static size_t ring_buffer_prev_pos;

        static uint8_t master_volume;
        static float master_volume_on_unit_interval;
};

}  // End of namespace MCK

#endif
