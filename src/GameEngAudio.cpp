////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  GameEngAudio.cpp
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

#include "GameEngAudio.h"

// Set initial values
bool MCK::GameEngAudio::initialized = false;
uint64_t MCK::GameEngAudio::sample_counter = 0;
uint8_t MCK::GameEngAudio::master_volume = 0xFF;
std::vector<uint8_t> MCK::GameEngAudio::channel_volumes(
    8,  // Always 8 channels
    AUDIO_DEFAULT_CHANNEL_VOLUME
);
// These will be set by 'init' method
int MCK::GameEngAudio::samples_per_second;
SDL_AudioFormat MCK::GameEngAudio::format;
uint8_t MCK::GameEngAudio::num_channels;
uint8_t MCK::GameEngAudio::silence_byte_value;
uint16_t MCK::GameEngAudio::buffer_size_in_samples;
uint32_t MCK::GameEngAudio::buffer_size_in_bytes;
uint16_t MCK::GameEngAudio::bytes_per_sample;
uint8_t MCK::GameEngAudio::bytes_per_channel;
uint64_t MCK::GameEngAudio::chunk_buffer[ MCK::AUDIO_CHUNK_BUFFER_SIZE ];

void MCK::GameEngAudio::init( uint8_t _master_volume )
{
    // Forbid repeat initialization
    if( MCK::GameEngAudio::initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to initialized SDL audio " )
            + std::string( "as already initialized!" );
#else
            ""
#endif
        ) );
    }

    // Initialise SDL audio
    {
        int rc = SDL_Init( SDL_INIT_AUDIO );
        if( rc != 0 )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Failed to initialized SDL audio, " )
                + std::string( "error message = " );
                + SDL_GetError()
#else
                ""
#endif
            ) );
        }
    }

    // Create SDL object containing desired audio choices
    SDL_AudioSpec want;
    want.freq = MCK::AUDIO_WANT_SAMPLE_RATE;
    want.format = MCK::AUDIO_WANT_FORMAT;
    want.channels = MCK::AUDIO_WANT_CHANNELS;
    want.samples = MCK::AUDIO_WANT_BUFFER_SIZE;
    want.callback = MCK::GameEngAudio::callback;
    want.userdata = &MCK::GameEngAudio::sample_counter;

#if defined MCK_STD_OUT && defined MCK_VERBOSE
    // Report desired audio choices
    std::cout << "Initializing SDL audio:"
              << "\n>> Wanted sample rate = "
              << want.freq
              << "\n>> Wanted format = "
              << want.format
              << "\n>> Wanted number of channels = "
              << int( want.channels )
              << "\n>> Wanted buffer size (in samples) = "
              << want.samples
              << std::endl;
#endif
        
    // Open SDL audio channels
    SDL_AudioSpec have;
    {
        int rc = SDL_OpenAudio(&want, &have);
        if( rc != 0 )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Failed to open SDL audio, " )
                + std::string( "error message = " )
                + SDL_GetError()
#else
                ""
#endif
            ) );
        }
    }

    // Check callback is correct
    if( have.callback != MCK::GameEngAudio::callback )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Audio callback funtion not as requested, " )
            + std::string( "SDL2 error?" )
#else
            ""
#endif
        ) );
    }

    // Store audio details
    MCK::GameEngAudio::samples_per_second = have.freq;
    MCK::GameEngAudio::format = have.format;
    MCK::GameEngAudio::num_channels = have.channels;
    MCK::GameEngAudio::silence_byte_value = have.silence;
    MCK::GameEngAudio::buffer_size_in_samples = have.samples;
    MCK::GameEngAudio::buffer_size_in_bytes = have.size;
    MCK::GameEngAudio::bytes_per_channel = ( have.format & SDL_AUDIO_MASK_BITSIZE ) / 8;
    MCK::GameEngAudio::bytes_per_sample = have.size / have.samples;

    // Check 'bytes_per_channel' matches 'bytes_per_samples'
    if( MCK::GameEngAudio::bytes_per_channel * MCK::GameEngAudio::num_channels
            != MCK::GameEngAudio::bytes_per_sample )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Mismatch between SDL_AudioSpec.size and other audio info"
#else
            ""
#endif
        ) );
    }

#if defined MCK_STD_OUT && defined MCK_VERBOSE
    // Report obtained audio choices
    std::cout << "Successfully initialized SDL audio:"
              << "\n>> Samples per second = "
              << MCK::GameEngAudio::samples_per_second
              << "\n>> Format = "
              << MCK::GameEngAudio::format
              << "\n>> Number of channels = "
              << MCK::GameEngAudio::num_channels
              << "\n>> Silence byte value = "
              << int( MCK::GameEngAudio::silence_byte_value )
              << "\n>> Buffer size (in samples)  = "
              << MCK::GameEngAudio::buffer_size_in_samples
              << "\n>> Buffer size (in bytes)  = "
              << MCK::GameEngAudio::buffer_size_in_bytes
              << "\n>> Bytes per sample = "
              << MCK::GameEngAudio::bytes_per_sample
              << "\n>> Bytes per channel = "
              << MCK::GameEngAudio::bytes_per_channel
              << std::endl;
#endif

    MCK::GameEngAudio::master_volume = _master_volume;

    SDL_PauseAudio(0); // Start audio by default
    // SDL_PauseAudio(1); // Pause audio by default

    MCK::GameEngAudio::initialized = true;
}

void MCK::GameEngAudio::play_single_chunk(
    uint8_t chunk_id,
    uint8_t channel_num,
    uint32_t ticks
)
{
    // TODO
}

void MCK::GameEngAudio::set_channel_volume(
    uint8_t volume,
    uint8_t channel_num
)
{
    // Only use first 3 bits of channel number
    const uint8_t CHAN_NUM = channel_num & 0x03;

    // Set volume
    channel_volumes.at( channel_num ) = volume;
}

void MCK::GameEngAudio::callback(
    void *user_data,
    uint8_t *raw_buffer,
    int bytes
)
{
    const int LENGTH = bytes / MCK::GameEngAudio::bytes_per_sample;

    {
        // If no game engine instance, set buffer to zero
        for( int i = 0; i < bytes; i++ )
        {
            // As seeting all bytes to zero,
            // no need to determine data type
            raw_buffer[i] = MCK::GameEngAudio::silence_byte_value;
        }
        // Return here as no more to do
        return;
    }
}
