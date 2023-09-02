////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  VoiceSynth.cpp
//
//  Provides "chiptune" style waveforms  
//  for the audio callback
//
//  Instances of this class are passed
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

#include "VoiceSynth.h"

const std::vector<float> MCK::VoiceSynth::WHITENOISE_VALUES {
    0.23, 0.21, 0.58, 0.82, 0.83, 0.71, 0.60, 0.66,
    0.10, 0.59, 0.95, 0.31, 0.87, 0.74, 0.92, 0.42,
    0.91, 0.71, 0.01, 0.21, 0.31, 0.26, 0.65, 0.46,
    0.24, 0.95, 0.42, 0.37, 0.07, 0.34, 0.08, 0.58,
    0.01, 0.46, 0.61, 0.13, 0.54, 0.75, 0.35, 0.34,
    0.94, 0.51, 0.24, 0.05, 0.21, 0.83, 0.76, 0.03,
    0.87, 0.79, 0.53, 0.72, 0.85, 0.35, 0.22, 0.76,
    0.33, 0.45, 0.33, 0.22, 0.70, 0.78, 0.03, 0.93,
    0.42, 0.27, 0.81, 0.32, 0.39, 0.67, 0.16, 0.71,
    0.31, 0.83, 0.24, 0.45, 0.11, 0.27, 0.07, 0.38,
    0.81, 0.48, 0.67, 0.98, 0.63, 0.07, 0.65, 0.48,
    0.35, 0.89, 0.84, 0.73, 0.23, 0.02, 0.00, 0.98,
    0.96, 0.37, 0.93, 0.05, 0.92, 0.23, 0.64, 0.80,
    0.06, 0.76, 0.90, 0.06, 0.73, 0.96, 0.26, 0.68,
    0.12, 0.65, 0.31, 0.46, 0.47, 0.83, 0.19, 0.33,
    0.28, 0.94, 0.86, 0.63, 0.82, 0.90, 0.83, 0.09,
    0.02, 0.21, 0.74, 0.93, 0.44, 0.39, 0.06, 0.52,
    0.23, 0.78, 0.12, 0.31, 0.97, 0.32, 0.87, 0.68,
    0.92, 0.88, 0.50, 0.77, 0.84, 0.89, 0.65, 0.04,
    0.06, 0.16, 0.26, 0.39, 0.18, 0.08, 0.73, 0.74,
    0.44, 0.79, 0.92, 0.43, 0.27, 0.70, 0.79, 0.86,
    0.77, 0.87, 0.88, 0.77, 0.28, 0.40, 0.54, 0.78,
    0.47, 0.40, 0.10, 0.48, 0.62, 0.30, 0.77, 0.33,
    0.39, 0.52, 0.55, 0.40, 0.02, 0.44, 0.06, 0.73,
    0.04, 0.94, 0.49, 0.72, 0.04, 0.11, 0.95, 0.05,
    0.24, 0.60, 0.89, 0.71, 0.93, 0.25, 0.21, 0.50,
    0.39, 0.50, 0.56, 0.06, 0.69, 0.65, 0.55, 0.34,
    0.20, 0.76, 0.61, 0.40, 0.34, 0.67, 0.58, 0.73,
    0.19, 0.36, 0.72, 0.75, 0.35, 0.71, 0.54, 0.61,
    0.62, 0.95, 0.12, 0.90, 0.33, 0.11, 0.01, 0.50,
    0.74, 0.46, 0.23, 0.06, 0.41, 0.88, 0.90, 0.96,
    0.61, 0.95, 0.77, 0.08, 0.92, 0.72, 0.79, 0.54
};
const size_t MCK::VoiceSynth::WHITENOISE_VALUES_INDEX_MASK 
    = MCK::VoiceSynth::WHITENOISE_VALUES.size() - 1;

const double MCK::VoiceSynth::ONE_TWELTH = 1.0f / 12.0f;

MCK::VoiceSynth::VoiceSynth( void ) : VoiceBase()
{
    this->type = MCK::VoiceType::SYNTH; 
    this->wave = MCK::VoiceSynth::Waveform::SINE;
    this->lowest_octave = 3;
    this->sliding_transition = false;
    // this->duty_cycle = 0.5f;
    // this->vibrato_mag = 0;
    // this->vibrato_wavelen_in_samples = 0;
    this->starting_sample_count = MCK::AUDIO_INVALID_SAMPLE_COUNT;
    this->start_wavelen = double( VOICE_SYNTH_FREQ_A4 );  // Safer than zero
    this->sliding_transition_length_in_samples = MCK::AUDIO_INVALID_SAMPLE_COUNT;
    this->sustain = 0;
    this->envelope_value = 0.0f;
}

void MCK::VoiceSynth::init(
    uint32_t _samples_per_second,
    uint32_t _sixteenth_duration_in_samples,
    MCK::VoiceSynth::Waveform _wave,
    uint8_t _lowest_octave,
    MCK::Envelope _envelope,
    uint8_t initial_volume,
    bool _sliding_transition
    // uint8_t _duty_cycle,
    // uint8_t _vibrato_mag,
    // uint16_t vibrato_freq
)
{
    if( this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot intialize VoiceSynth instance, as already init."
#else
            ""
#endif
        ) );
    }

    // Integrity check on WHITENOISE_VALUE
    {
        const size_t SIZE = WHITENOISE_VALUES.size();
        if( SIZE == 0 || SIZE & ( SIZE - 1 ) != 0 )
        {
            throw( std::runtime_error(
#if defined MCK_STD_OUT
                std::string( "Cannot intialize VoiceSynth instance, " )
                + std::string( "as WHITENOISE_VALUES array size is " )
                + std::string( "not a power of 2." )
#else
                ""
#endif
            ) );
        }
    }

    if( _lowest_octave > 6 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Lowest octave must be at least 6. Tuning A (440Hz) is in octave 4."
#else
            ""
#endif
        ) );
    }

/*
#if defined MCK_STD_OUT && defined MCK_VERBOSE
    // Check for clipping due to vibrato,
    // issue warning if so.
    if( int( initial_volume )
            + int( _vibrato_mag )
                / VOICE_SYNTH_MAX_VIB_PROPORTION 
                    > 255
    )
    {
        std::cout << "WARNING: Chosen vibrato magnitude may "
                  << "cause clipping, consider reducing channel "
                  << "volume and/or vibrato magnitude."
                  << std::endl;
    }
#endif
*/

    this->sixteenth_duration_in_samples 
            = _sixteenth_duration_in_samples; 

    this->wave = _wave;

    this->lowest_octave = _lowest_octave;
    
    this->envelope = _envelope; 
    
    // Note: sliding transition not used for whitenoise, as has no effect
    this->sliding_transition
        = this->wave != MCK::VoiceSynth::WHITENOISE 
          && _sliding_transition;
    
    /*
    this->duty_cycle = float( _duty_cycle ) / 255.0f;
    
    this->vibrato_mag
        = float( _vibrato_mag )
            / float( VOICE_SYNTH_MAX_VIB_PROPORTION )
                / 255.0f;
    
    this->vibrato_wavelen_in_samples
        = float( vibrato_freq ) / float( _samples_per_second );
    */

    // Get total number of notes by applying note bit shift
    // to note bit mask, then adding 1.
    const uint8_t NUM_NOTES
        = ( MCK::VOICE_SYNTH_PITCH_MASK  
                >> MCK::VOICE_SYNTH_PITCH_LSHIFT ) + 1; 
    
    // Reserve space for note frequencies/wavelengths
    this->freq_by_note_id.reserve( NUM_NOTES ); 
    this->wavelen_by_note_id.reserve( NUM_NOTES );

    // Calculate index of lowest note (C in lowest octave)
    const int LOWEST_C_INDEX
        = this->lowest_octave * 12;  // always 12 notes per octave

    // Calculate distance (in semitones) between lowest
    // note and tuning A (index 57)
    const int LO_DIFF = LOWEST_C_INDEX - 57;

    // Loop over notes
    for( int n = LO_DIFF; n < LO_DIFF + NUM_NOTES; n++ )
    {
        // Calculate frequency
        this->freq_by_note_id.push_back(
            double( VOICE_SYNTH_FREQ_A4 )
                * pow( 2.0f, ONE_TWELTH * n )
        );

        /*
        // DEBUG
        std::cout << "IDX = " << n
                  << ", FRQ = "
                  << this->freq_by_note_id.back()
                  << std::endl;
        */

        // Calculate wavelength (in samples)
        this->wavelen_by_note_id.push_back(
            uint32_t(
                double( MCK::AUDIO_WANT_SAMPLE_RATE ) 
                    / this->freq_by_note_id.back() + 0.5f
            )
        );
    }

    // Call base init
    this->base_init( _samples_per_second, initial_volume ); 
}

void MCK::VoiceSynth::command(
    uint8_t com,
    uint64_t sample_count
)
{
    if( !this->initialized )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            "Cannot accept VoiceSynth command, as not init."
#else
            ""
#endif
        ) );
    }

    // Get pitch ID
    const uint8_t PITCH_ID
        = ( com & MCK::VOICE_SYNTH_PITCH_MASK ) 
            >> MCK::VOICE_SYNTH_PITCH_LSHIFT;

    // Get duration ID
    const uint8_t DURATION_ID
       = ( com & MCK::VOICE_SYNTH_DURATION_MASK )
            >> MCK::VOICE_SYNTH_DURATION_LSHIFT;

    // DEBUG
    std::cout << "DURATION_ID = " << int( DURATION_ID ) << std::endl;

    // Get sustain duration, in samples
    this->sustain = this->sixteenth_duration_in_samples 
                        * pow( 2, DURATION_ID );

    // Note starting sample count
    this->starting_sample_count = sample_count;

    // If sliding transition, and note still in play,
    // use current 'note_wavelen' as 'start_wavelen'
    if( this->sliding_transition )
    {
        this->start_wavelen = this->note_wavelen;
        this->sliding_transition_length_in_samples
            = this->sustain / 2;  // Experimental
    }

    // Set note wavelength
    this->note_wavelen = this->wavelen_by_note_id.at( PITCH_ID ); 

    // Calculate initial envelope value
    try
    {
        this->envelope_value = this->envelope.get_value(
            0,  // Relative sample count = zero
            this->sustain
        );
    }
    catch( const std::exception &e )
    {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
        // Issue a warning, but no point throwing exception here
        std::cout << "Cannot get envelope value, error: '" 
                  << e.what()
                  << "', using 1.0f for value instead."
                  << std::endl;
#endif
        this->envelope_value = 1.0f;
    }
}

float MCK::VoiceSynth::get_sample( uint64_t sample_count )
{
    // This is called alot, so for speed we forgo the
    // initialization check

    // If note not due to play yet, ignore
    if( sample_count < this->starting_sample_count )
    {
        return 0.0f;
    }

    // Get relative, i.e. 'within note', sample count
    const uint64_t REL_SAMPLE_COUNT
        = sample_count - this->starting_sample_count;

    // Intermitently recalculate envelope value
    if( ( REL_SAMPLE_COUNT & ( MCK::VOICE_SYNTH_ENVELOPE_INTERVAL_MASK ) ) 
            ==  MCK::VOICE_SYNTH_ENVELOPE_INTERVAL_MASK
    )
    {
        try
        {
            this->envelope_value = this->envelope.get_value(
                REL_SAMPLE_COUNT,
                this->sustain
            );
        }
        catch( const std::exception &e )
        {
#if defined MCK_STD_OUT && defined MCK_VERBOSE
            // Issue a warning, but no point throwing exception here
            std::cout << "Cannot get envelope value, error: '" 
                      << e.what()
                      << "', using 1.0f for value instead."
                      << std::endl;
#endif
            this->envelope_value = 1.0f;
        }

        /*
        // DEBUG
        std::cout << "REL_SAMPLE_COUNT = "
                  << REL_SAMPLE_COUNT
                  << ", envelope_value = "
                  << this->envelope_value
                  << ", sample count = "
                  << sample_count 
                  << std::endl;
        */

        // If envelope value is negative, we've reached the
        // end of the envelope and the note has expired.
        if( this->envelope_value < -0.001f )
        {
            // For safety, this should not be left negative
            this->envelope_value = 0.0f;

            // Setting this to invalid terminates the note
            this->starting_sample_count
                = MCK::AUDIO_INVALID_SAMPLE_COUNT;

            // Return here as no more to do
            return 0.0f;
        }
    }

    // For whitenoise, no wavelength needed
    if( wave == MCK::VoiceSynth::WHITENOISE )
    {
        // Note1: for added randomness, use absolute sample count
        // to look up random number (otherwise each whitenoise
        // note will use the same sequence of random values)
        // Note2: The '&' only works when WHITENOISE_VALUES
        // size is a power of 2, which was checked during init.
        return scale 
            * ( WHITENOISE_VALUES[
                    sample_count & WHITENOISE_VALUES_INDEX_MASK
                ] * 2.0f - 1.0f 
            );
    }

    // Calculate current wavelength (every 32 samples)
    uint32_t current_wavelen;
    // TODO: This is NOT working!
    /*
    if( this->sliding_transition
        && this->sliding_transition_length_in_samples > REL_SAMPLE_COUNT 
    )
    {
        current_wavelen = uint32_t(
            start_wavelen + 
            float( this->sliding_transition_length_in_samples 
                    - REL_SAMPLE_COUNT 
            ) / float( this->sliding_transition_length_in_samples )
            * float( int64_t( this->note_wavelen ) - int64_t( this->start_wavelen ) )
            + 0.5f
        );
    }
    else
    */
    {
        current_wavelen = note_wavelen;
    } 

    // Calculate period within waveform (on unit interval)
    const float PERIOD
        = float( REL_SAMPLE_COUNT ) / float( current_wavelen )
            - float( 
                uint32_t(
                    float( REL_SAMPLE_COUNT ) 
                        / float( current_wavelen )
                )  
            );

    // Calculate waveform value, scaled by envelope value
    switch( wave )
    {
        case MCK::VoiceSynth::SINE:
            return scale * envelope_value * sin( PERIOD * MCK_TWO_PI );
            break;

        case MCK::VoiceSynth::SQUARE:
            // Note: this is scaled to match RMS value of SINE
            //       using a constant approx. equal to the
            //       reciprocal of the square root of two.
            return scale * envelope_value * MCK_ONE_OVER_ROOT_TWO 
                    * ( ( PERIOD > 0.5f ) * 2.0f - 1.0f );

        case MCK::VoiceSynth::TRIANGLE:
            if( PERIOD < 0.5f )
            {
                return scale * envelope_value * ( PERIOD * 4.0f - 1.0f );
            }
            else
            {
                return scale * envelope_value * ( 3.0f - PERIOD * 4.0f ); 
            }

        case MCK::VoiceSynth::SAWTOOTH:
            return scale * envelope_value * PERIOD * 4.0f - 2.0f;

        // Note: white noise waveform handled earlier.
    }

    // This shouldn't occur, but just to be safe
    return 0.0f;
}
