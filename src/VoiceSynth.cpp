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

const double MCK::VoiceSynth::ONE_TWELTH = 1.0f / 12.0f;

MCK::VoiceSynth::VoiceSynth( void ) : VoiceBase()
{
    this->type = MCK::VoiceType::SYNTH; 
    this->wave = MCK::VoiceSynth::Waveform::SINE;
    this->lowest_octave = 3;
    this->starting_sample_count = MCK::AUDIO_INVALID_SAMPLE_COUNT;
    this->sustain = 0;
    this->envelope_value = 0.0f;
    this->note_wavelen = 1.0f;
}

void MCK::VoiceSynth::init(
    uint32_t _sixteenth_duration_in_samples,
    MCK::VoiceSynth::Waveform _wave,
    uint8_t _lowest_octave,
    MCK::Envelope _envelope,
    uint8_t volume
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

    if( _lowest_octave > 6 )
    {
        throw( std::runtime_error(
#if defined MCK_STD_OUT
            std::string( "Cannot intialize VoiceSynth instance " )
            + std::string( "with lowest octave set to " )
            + std::to_string( _lowest_octave )
            + std::string( ". Lowest octave must be not exceed 6. " )
            + std::string( "For info: 'tuning A' is in octave 4." )
#else
            ""
#endif
        ) );
    }

    this->sixteenth_duration_in_samples 
            = _sixteenth_duration_in_samples; 

    this->wave = _wave;

    this->lowest_octave = _lowest_octave;
    
    this->envelope = _envelope; 

    // Calculate total number of notes via number of bits allocated
    // to pitch IDs in the command byte. This is done by applying
    // the pitch ID bit shift (in reverse, i.e. right) to the 
    // pitch ID bit mask, then adding 1.
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
    // note and 'tuning A' (which always has raw note ID of 57)
    const int LO_DIFF = LOWEST_C_INDEX - 57;

    // Loop over notes
    for( int n = LO_DIFF; n < LO_DIFF + NUM_NOTES; n++ )
    {
        // Calculate frequency
        this->freq_by_note_id.push_back(
            float( VOICE_SYNTH_FREQ_A4 ) // Freq of 'tuning A'
                * pow( 2.0f, MCK::VoiceSynth::ONE_TWELTH * n )
        );

        // Calculate wavelength (in samples) from frequency
        this->wavelen_by_note_id.push_back(
                float( MCK::AUDIO_WANT_SAMPLE_RATE ) 
                    / this->freq_by_note_id.back()
        );
    }

    // Call base init
    this->base_init( volume ); 
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

    // Get sustain duration, in samples
    this->sustain = this->sixteenth_duration_in_samples 
                        * pow( 2, DURATION_ID );

    // Note starting sample count
    this->starting_sample_count = sample_count;

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
    if( this->wave == MCK::VoiceSynth::WHITENOISE )
    {
        return this->scale * ( float( rand() & 255 ) / 128.0f - 1.0f );
    }

    // Calculate period within waveform (on unit interval)
    const float PERIOD
        = float( REL_SAMPLE_COUNT ) / this->note_wavelen
            - float( 
                uint32_t(
                    float( REL_SAMPLE_COUNT ) 
                        / this->note_wavelen
                )  
            );

    // Calculate waveform value, scaled by envelope value
    switch( this->wave )
    {
        case MCK::VoiceSynth::SINE:
            return this->scale * this->envelope_value * sin( PERIOD * MCK_TWO_PI );
            break;

        case MCK::VoiceSynth::SQUARE:
            // Note: this is scaled to match RMS value of SINE
            //       using a constant approx. equal to the
            //       reciprocal of the square root of two.
            return this->scale * this->envelope_value * MCK_ONE_OVER_ROOT_TWO 
                    * ( ( PERIOD > 0.5f ) * 2.0f - 1.0f );

        case MCK::VoiceSynth::TRIANGLE:
            if( PERIOD < 0.5f )
            {
                return this->scale * this->envelope_value * ( PERIOD * 4.0f - 1.0f );
            }
            else
            {
                return this->scale * this->envelope_value * ( 3.0f - PERIOD * 4.0f ); 
            }

        case MCK::VoiceSynth::SAWTOOTH:
            return this->scale * this->envelope_value * PERIOD * 4.0f - 2.0f;

        // Note: white noise waveform handled earlier.
    }

    // This shouldn't occur, but just to be safe
    return 0.0f;
}

uint8_t MCK::VoiceSynth::construct_command(
    uint8_t pitch_id,
    uint8_t duration_id
) noexcept
{
    return ( 
        // Left shift pitch ID and then mask off
        ( pitch_id << MCK::VOICE_SYNTH_PITCH_LSHIFT )
            & MCK::VOICE_SYNTH_PITCH_MASK
        ) | (
        // Left shift duration ID and then mask off
        ( duration_id << MCK::VOICE_SYNTH_DURATION_LSHIFT )
            & MCK::VOICE_SYNTH_DURATION_MASK
    );
}
