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
float MCK::GameEngAudio::master_volume_on_unit_interval
    = MCK::GameEngAudio::master_volume / float( 0xFF );

// These will be set by 'init' method
int MCK::GameEngAudio::samples_per_second;
SDL_AudioFormat MCK::GameEngAudio::format;
uint8_t MCK::GameEngAudio::num_channels;
uint8_t MCK::GameEngAudio::silence_byte_value;
uint16_t MCK::GameEngAudio::buffer_size_in_samples;
uint32_t MCK::GameEngAudio::buffer_size_in_bytes;
uint16_t MCK::GameEngAudio::bytes_per_sample;
uint8_t MCK::GameEngAudio::bytes_per_channel;
MCK_AUDIO_RING_BUFFER_DATA_TYPE MCK::GameEngAudio::ring_buffer[ MCK::AUDIO_RING_BUFFER_SIZE ];
MCK::AudioDataType MCK::GameEngAudio::data_type
    = MCK::AudioDataType::UNKNOWN;
std::vector<std::shared_ptr<MCK::VoiceBase>> MCK::GameEngAudio::voices;

void MCK::GameEngAudio::init( uint8_t _master_volume )
{
    // Forbid repeat initialization
    if( MCK::GameEngAudio::initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to initialized SDL audio " )
            + std::string( "as already initialized!" )
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
                + std::string( "error message = " )
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
    MCK::GameEngAudio::bytes_per_channel
        = ( have.format & SDL_AUDIO_MASK_BITSIZE ) / 8;  // here 8 = bits per byte
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
              << int( MCK::GameEngAudio::num_channels )
              << "\n>> Silence byte value = "
              << int( MCK::GameEngAudio::silence_byte_value )
              << "\n>> Buffer size (in samples)  = "
              << MCK::GameEngAudio::buffer_size_in_samples
              << "\n>> Buffer size (in bytes)  = "
              << MCK::GameEngAudio::buffer_size_in_bytes
              << "\n>> Bytes per sample = "
              << MCK::GameEngAudio::bytes_per_sample
              << "\n>> Bytes per channel = "
              << int( MCK::GameEngAudio::bytes_per_channel )
              << std::endl;
#endif

    // Set simplified indicator of data format
    {
        

        const bool FLOAT = SDL_AUDIO_ISFLOAT( MCK::GameEngAudio::format );
        const bool SIGNED = SDL_AUDIO_ISSIGNED( MCK::GameEngAudio::format );
        const uint8_t NUM_BITS = SDL_AUDIO_BITSIZE( MCK::GameEngAudio::format );

        // Check for signed 16bit integer
        if( !FLOAT
            && SIGNED
            && NUM_BITS == 16
            && MCK::GameEngAudio::bytes_per_channel == 2
        )
        {
            MCK::GameEngAudio::data_type
                = MCK::AudioDataType::SIGNED_16_BIT_INT;
        }
        // Check for float
        else if( FLOAT
                 && SIGNED
                 && NUM_BITS == 32
                 && MCK::GameEngAudio::bytes_per_channel == 4
        )
        {
            MCK::GameEngAudio::data_type
                = MCK::AudioDataType::SIGNED_32_BIT_FLOAT;
        }
        // Reject all other formats for now
        else
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Apologies, your audio data format " )
                + std::string( "is not supported by MuckyVision v3." )
                + std::string( " Audio initialization failed." )
#else
                ""
#endif
            ) );
        }
    }

    // Create voices (all VoiceSynth instances for now)
    MCK::GameEngAudio::voices.resize( MCK_NUM_VOICES, NULL ); 
    for( int i = 0; i < MCK_NUM_VOICES; i++ )
    {
        std::shared_ptr<MCK::VoiceSynth> synth
            = std::make_shared<MCK::VoiceSynth>();
        try
        {
            synth->init( 
                MCK::GameEngAudio::samples_per_second,
                2205,  // sixteenth_duration_in_samples, 
                // MCK::VoiceSynth::SAWTOOTH,
                MCK::VoiceSynth::WHITENOISE,
                // MCK::VoiceSynth::Waveform( i % 5 ),
                3,  // lowest octave
                MCK::Envelope(
                    550,  // Attack
                    550,  // Decay
                    2205,  // Sustain
                    192  // Sustain as proportion of peak (0-255)
                ),
                0xFF  // Initial volume
            );
        }
        catch( const std::exception &e )
        {
#if defined MCK_STD_OUT
            // Issue a warning, but no point throwing exception here
            std::cout << "Unable to init voice "
                      << std::to_string( i )
                      << ", error = " 
                      << e.what()
                      << std::endl;
#endif
            continue;
        }

        // Recast as base pointer, and store
        MCK::GameEngAudio::voices[i] = std::dynamic_pointer_cast<MCK::VoiceBase>( synth );
        
#if defined MCK_STD_OUT
        std::cout << "Successfully created voice " << i
                  << std::endl;
#endif 
    }

    // Set master volume
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

void MCK::GameEngAudio::callback(
    void *user_data,
    uint8_t *raw_buffer,
    int bytes
)
{
    // Get number of samples required
    const int LENGTH = bytes / MCK::GameEngAudio::bytes_per_sample;

    // Calculate end sample count
    const long END_SAMPLE_COUNT = MCK::GameEngAudio::sample_counter + LENGTH;
    
    // Keep track of samples filled, in case we need to pad with
    // silence bytes
    int num_of_samples_filled = 0;

    // Get signed 16bit integer pointer to actual buffer array
    int16_t *int16_buffer = (int16_t*)raw_buffer;

    // Get signed (32bit?) float pointer to actual buffer array
    float *float_buffer = (float*)raw_buffer;
        
    // TEST CODE
    if( MCK::GameEngAudio::sample_counter / 44100
            != ( 
                MCK::GameEngAudio::sample_counter
                    + buffer_size_in_samples
            ) / 44100 
    )
    {
        const int test_num = MCK::GameEngAudio::sample_counter / 44100; 
        const uint8_t PITCH_ID
            = ( test_num ) & 0x1F;
        const uint8_t DURATION_ID
            = 0x05;
            // = ( test_num ) & 0x07;
        const uint8_t COMMAND
            = (
                ( PITCH_ID << MCK::VOICE_SYNTH_PITCH_LSHIFT
                ) & MCK::VOICE_SYNTH_PITCH_MASK
            ) | (
                ( DURATION_ID << MCK::VOICE_SYNTH_DURATION_LSHIFT
                ) & MCK::VOICE_SYNTH_DURATION_MASK
            );

        // MCK::GameEngAudio::voices[0]->command(
        MCK::GameEngAudio::voices[test_num % MCK_NUM_VOICES]->command(
            COMMAND,
            MCK::GameEngAudio::sample_counter
        );
    }

    // Fill samples
    for( int sample_index = 0; sample_index < LENGTH; sample_index++ )
    {
        float val = 0.0f;

        const uint64_t SAMPLE_COUNT
            = MCK::GameEngAudio::sample_counter + sample_index;

        // Loop over voices
        for( auto vc : MCK::GameEngAudio::voices )
        {
            // Get sample value as float, in range [-1, 1]
            if( vc.get() != NULL )
            {
                val += vc->get_sample( SAMPLE_COUNT );
            }
        }

        // Short integer (16 bit signed)
        if( data_type == MCK::AudioDataType::SIGNED_16_BIT_INT )
        {
            // Calculate amplitude adjusted sample value
            const int16_t NORM_VAL = int16_t( val * 32767.0f + 0.5f ) ;
            
            // Duplicate sample across all channels
            for( int ch = 0;
                 ch < MCK::GameEngAudio::num_channels;
                 ch++
            )
            {
                int16_buffer[ sample_index * MCK::GameEngAudio::num_channels + ch ]
                    = NORM_VAL;
            }
        }
        else if( data_type == MCK::AudioDataType::SIGNED_32_BIT_FLOAT )
        {
            // Duplicate sample across all channels
            for( int ch = 0;
                 ch < MCK::GameEngAudio::num_channels;
                 ch++
            )
            {
                float_buffer[ sample_index * MCK::GameEngAudio::num_channels + ch ]
                    = val;  // No need to normalize
            }
        }
    
        num_of_samples_filled++;
    }

    // Fill any unused bytes with silence
    for( int i = num_of_samples_filled * MCK::GameEngAudio::bytes_per_sample; 
         i < bytes;
         i++
    )
    {
        // As setting all bytes to zero,
        // no need to determine data type
        raw_buffer[i] = MCK::GameEngAudio::silence_byte_value;
    }
    
    // Set new sample count
    MCK::GameEngAudio::sample_counter = END_SAMPLE_COUNT;
    
}
