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

namespace MCK
{

class GameEngAudio
{
    public:

        //! Initialize SDL2 audio
        static void init( uint8_t _master_volume );

        // //! Play up to eight chunks (one on each channel) at a particular point in time.
        // /*! @param chunk_ids: 8 x 8 bytes, each bytes containing (left to right) the ID of a chunk to play on channel 0-7 respectively.
        //  *  @param ticks: Time point at which to play these chunks. If 0 (default) current time is used.
        //  * Note: Any bytes corresponding to
        //  */
        // static void play_chunks(
        //     uint64_t chunk_ids,
        //     uint32_t ticks = 0
        // );
        
        //! Play a single chunk at a given time point (or a current time)
        /*! @param chunk_id: If of chunk to play
         *  @param channel_num: Number of channel (0-7, only rightmost 3 bits are used)
         *  @param ticks: Future time point at which to play these chunks. If 0 (default), current time is used.
         */
        static void play_single_chunk(
            uint8_t chunk_id,
            uint8_t channel_num,
            uint32_t ticks = 0
        );

        //! Set volume of audio channel 
        /*! @param volume: Channel volume, 0 = mute, 255 = max.
         *  @param channel_num: Number of channel (0-7, only rightmost 3 bits are used)
         */
        static void set_channel_volume(
            uint8_t volume,
            uint8_t channel_num
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
        GameEngAudio(GameEngAudio const&) = delete;
        void operator=(GameEngAudio const&)  = delete;

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

        //! Ring buffer, for communication with main thread
        /*! This is composed of 'n' entries containing 8 bytes each
         *  where 'n' is set at compile time and defined in Defs.h.
         *  Each byte represents a request to play an audio
         *  chunk (i.e. a sound) on one of eight channels at some
         *  particular time point (i.e. tick). There is some
         *  latency as these requests have to sit in the buffer
         *  for at least a tick or two.
         *  The purpose of this buffer is to avoid the need
         *  for C++11 parallelisation (e.g. mutex), as MinGW 
         *  support for that is patchy.
         *  CAUTION: This is a C-style array, chosen because
         *           speed of read/write is critical here.
         *           It is only safe if used with care. 
         *           Do not alter the buffering
         *           code unless you know what you are doing.
         */
        static uint64_t chunk_buffer[];

        static uint8_t master_volume;

        static std::vector<uint8_t> channel_volumes;

        // TODO: Store chunk type instances
};

}  // End of namespace MCK

#endif
