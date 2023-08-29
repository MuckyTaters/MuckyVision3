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

MCK::VoiceSynth::VoiceSynth( void )
{
    this->type = MCK::VoiceType::SYNTH; 
    this->wave = MCK::VoiceSynth::Waveform::SINE;
    this->lowest_octave = 3;
    this->sliding_transition = false;
    this->duty_cycle = 0.5f;
    this->vibrato_mag = 0;
    this->vibrato_wavelen_in_samples = 0;
    this->starting_sample_count = MCK::AUDIO_INVALID_SAMPLE_COUNT;
}

void MCK::VoiceSynth::init(
    uint32_t _samples_per_second,
    MCK::VoiceSynth::Waveform _wave,
    uint8_t _lowest_octave,
    MCK::Envelope _envelope,
    uint8_t initial_volume,
    bool _sliding_transition,
    uint8_t _duty_cycle,
    uint8_t _vibrato_mag,
    uint16_t vibrato_freq
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
            "Lowest octave must be at least 6. Tuning A (440Hz) is in octave 4."
#else
            ""
#endif
        ) );
    }

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

    this->wave = _wave;

    this->lowest_octave = _lowest_octave;
    
    this->envelope = _envelope; 
    
    this->sliding_transition = _sliding_transition;
    
    this->duty_cycle = float( _duty_cycle ) / 255.0f;
    
    this->vibrato_mag
        = float( _vibrato_mag )
            / float( VOICE_SYNTH_MAX_VIB_PROPORTION )
                / 255.0f;
    
    this->vibrato_wavelen_in_samples
        = float( vibrato_freq ) / float( _samples_per_second );

    // Reserve space for note frequencies/wavelengths
    const uint8_t NUM_FREQ = pow( 2, MCK::VOICE_SYNTH_PITCH_BITS ); 
    this->freq_by_note_id.reserve( NUM_FREQ ); 
    this->wavelen_by_note_id.reserve( NUM_FREQ );
    
    // Calculate index of lowest note (C in lowest octave)
    const int LOWEST_C_INDEX
        = this->lowest_octave * 12;  // always 12 notes per octave

    // Calculate distance (in semitones) between lowest
    // note and tuning A (index 57)
    const int LO_DIFF = LOWEST_C_INDEX - 57;

    // Calculate total number of notes
    const int NUM_NOTES = pow( 2, MCK::VOICE_SYNTH_PITCH_BITS );

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

void MCK::VoiceSynth::command( uint8_t com )
{
    // TODO
}

float MCK::VoiceSynth::get_sample( uint64_t sample_count )
{
    // TEMPORARY CODE
    return 0.0f;

    // TODO
}
