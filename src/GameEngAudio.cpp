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
//  BEING A SINGLETON CLASS. IT HAS NO
//  CONSTRUCTOR. THIS IS FOR EASE OF
//  INTEGRATION WITH THE AUDIO CALLBACK,
//  THAT HAS TO BE STATIC, OR A NON-MEMBER
//  FUNCTION.
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
size_t MCK::GameEngAudio::ring_buffer_prev_pos = 0;
bool MCK::GameEngAudio::mute = false;

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
uint8_t MCK::GameEngAudio::master_volume;

void MCK::GameEngAudio::init(
    uint8_t _master_volume,
    std::vector<std::shared_ptr<MCK::VoiceBase>> &_voices
)
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

    // Check audio ring buffer size isn't ridiculously small
    // and *is* a power of two
    if( MCK::AUDIO_RING_BUFFER_SIZE < 0x40
        || (
            MCK::AUDIO_RING_BUFFER_SIZE
            & ( MCK::AUDIO_RING_BUFFER_SIZE - 1 )
        ) != 0
    )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to initialized SDL audio " )
            + std::string( "as MCK::AUDIO_RING_BUFFER_SIZE " )
            + std::string( "is either too small (<64) or not " )
            + std::string( "a power of two." )
            
#else
            ""
#endif
        ) );
    }

    // Set master volume
    master_volume = _master_volume;

    // Clear ring buffer
    for( int i = 0; i < MCK::AUDIO_RING_BUFFER_SIZE; i++ )
    {
        MCK::GameEngAudio::ring_buffer[i] = 0;
    }

    // Check number of supplied voices is correct
    if( _voices.size() != MCK_NUM_VOICES )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to initialized SDL audio, " )
            + std::string( "as wrong number of voices supplied, " )
            + std::string( "size of 'voices' needs to match " )
            + std::string( "value of MCK_NUM_VOICES macro." )
#else
            ""
#endif
        ) );
    }
    
    // Check initialisation of supplied voices,
    // if okay store them
    voices.resize( MCK_NUM_VOICES, NULL );
    for( int i = 0; i < MCK_NUM_VOICES; i++ )
    {
        if( _voices[i].get() == NULL
            || !_voices[i]->is_initialized()
        )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Failed to initialized SDL audio, " )
                + std::string( "as supplied voice " )
                + std::to_string( i )
                + std::string( " is NULL or not initialized." )
#else
                ""
#endif
            ) );
        }

        // If initialised, store pointer
        voices[i] = _voices[i];
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
                + std::string( "is not yet supported by MuckyVision v3. " )
                + std::string( "Audio initialization failed." )
                + std::string( "\nFor information, your format is read as:" )
                + std::string( SIGNED ? "\n>> SIGNED" : "\n>> UNSIGNED" )
                + std::string( FLOAT ? "\n>> FLOAT" : "\n>> INTEGER" )
                + std::string( "\n>> NUMBER BITS: " )
                + std::to_string( NUM_BITS )
                + std::string( "\n>> BYTES PER CHANNEL: " )
                + std::to_string( MCK::GameEngAudio::bytes_per_channel )

#else
                ""
#endif
            ) );
        }
    }

    SDL_PauseAudio(0); // Start audio by default
    // SDL_PauseAudio(1); // Pause audio by default

    MCK::GameEngAudio::initialized = true;
}

void MCK::GameEngAudio::voice_command(
    uint8_t voice_id,
    uint8_t command,
    uint32_t ticks
)
{
    // IMPORTANT: This method should only be called by the main thread

    if( !MCK::GameEngAudio::initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to issue voice command as SDL audio " )
            + std::string( "not yet initialized!" )
#else
            ""
#endif
        ) );
    }
    
    // Check voice IDs within range
    if( voice_id >= MCK_NUM_VOICES )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to issue voice command as voice id " )
            + std::to_string( voice_id )
            + std::string( " is out of range." )
#else
            ""
#endif
        ) );
    }

    // Get current time point
    const uint32_t CURRENT_TICKS = SDL_GetTicks();

    // If time point is in past (or default), just use current time
    ticks = std::max( ticks, CURRENT_TICKS );

    // Check if request time point is too far in future
    // such that it would 'lap' the current time in the
    // ring buffer, and actually play sooner than expected
    if( ( ticks - CURRENT_TICKS ) 
            > ( MCK::AUDIO_RING_BUFFER_SIZE 
                   - MCK::AUDIO_RING_BUFFER_LAG_IN_TICKS )
    )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Failed to issue voice command as ticks " )
            + std::string( " specified is too far in advance and " )
            + std::string( " with 'lap' the audio ring buffer." )
#else
            ""
#endif
        ) );

    }

    // Construct 'AND' value that will clear any existing command
    // for this voice at this time-point (ticks).
    const MCK_AUDIO_RING_BUFFER_DATA_TYPE AND_VALUE
        = ~( 
            MCK_AUDIO_RING_BUFFER_DATA_TYPE( 0xFF )
                << ( voice_id * 8 )
        );

    // Construct 'OR' value that will add new command
    // for this voice at this time-point (ticks).
    const MCK_AUDIO_RING_BUFFER_DATA_TYPE OR_VALUE
        = MCK_AUDIO_RING_BUFFER_DATA_TYPE( command )
            << ( voice_id * 8 );

    // Get ring buffer position
    const uint32_t POS = ticks & MCK::AUDIO_RING_BUFFER_SIZE_MASK;

    // Write command to ring buffer
    MCK::GameEngAudio::ring_buffer[POS] &= AND_VALUE;
    MCK::GameEngAudio::ring_buffer[POS] |= OR_VALUE;
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

    ////////////////////////////////////////////
    // CHECK RING BUFFER FOR VOICE COMMAND(S)
    // (ignoring all but most recent command
    //  for each voice)
    std::vector<uint8_t> commands( MCK_NUM_VOICES, 0 );
    bool no_commands = true;
    {
        // Get current position in buffer, slightly
        // behind actual game time
        const uint32_t RAW_TICKS = SDL_GetTicks();

        const uint32_t END_POS
            = ( RAW_TICKS - MCK::AUDIO_RING_BUFFER_LAG_IN_TICKS )
                & MCK::AUDIO_RING_BUFFER_SIZE_MASK;

        uint32_t current_pos
            = MCK::GameEngAudio::ring_buffer_prev_pos 
                & MCK::AUDIO_RING_BUFFER_SIZE_MASK;

        while( current_pos != END_POS )
        {
            // Increment (and constrain) ticks
            current_pos = ++current_pos & MCK::AUDIO_RING_BUFFER_SIZE_MASK;

            // Read from ring buffer (this is safe only as long
            // as 'current_pos' has been bitwise 'anded'
            // with MCK::AUDIO_RING_BUFFER_SIZE_MASK )
            const MCK_AUDIO_RING_BUFFER_DATA_TYPE NEW_COMMANDS
                = ring_buffer[current_pos];

            // Clear ring buffer entry no it has been read
            ring_buffer[current_pos] = 0;

            // Check for new voice commands, and overwrite any
            // existing voice commands
            for( int i = 0; i < MCK_NUM_VOICES; i++ )
            {
                const uint8_t COM
                    = ( NEW_COMMANDS >> ( i * 8 ) ) & 0xFF;

                if( COM != 0 )
                {
                    // Store command
                    commands[i] = COM;
                    no_commands = false;
                }
            }
        }

        // Remember final position as start point for next
        // audio callback
        MCK::GameEngAudio::ring_buffer_prev_pos = current_pos;
    }

    // Process commands (if any)
    if( !no_commands )
    {
        for( int i = 0; i < MCK_NUM_VOICES; i++ )
        {
            if( commands[i] != 0
                && i < MCK::GameEngAudio::voices.size()
                && MCK::GameEngAudio::voices[i].get() != NULL
            )
            {
                MCK::GameEngAudio::voices[i]->command(
                    commands[i],
                    MCK::GameEngAudio::sample_counter
                );
            }       
        }
    }

    // Fill samples (unless mute is on)
    if( !mute )
    {
        float master_volume_on_unit_interval
            = float( master_volume ) / 255.0f;

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

            // Apply master volume
            val *= master_volume_on_unit_interval;

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
