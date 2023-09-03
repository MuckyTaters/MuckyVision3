////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  Envelope.h
//
//  Provides ADSR envelope for audio.
//  ADSR stands for
//      Attack
//      Decay
//      Sustain
//      Release
//
//  There is no Envelope.cpp file
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

#include "Envelope.h"

MCK::Envelope::Envelope( void )
{
    // Default 'square' ADSR envelope
    attack_in_samples = 0;
    decay_in_samples = 0;
    release_in_samples = 0;
    sustain_level_as_proportion_of_peak = 1.0f;
}

MCK::Envelope::Envelope(
    uint32_t _attack_in_samples,
    uint32_t _decay_in_samples,
    uint32_t _release_in_samples,
    uint8_t _sustain_level_as_proportion_of_peak
)
{
    attack_in_samples = _attack_in_samples;
    decay_in_samples = _decay_in_samples;
    release_in_samples = _release_in_samples;
    set_sustain_level_as_proportion_of_peak(
            _sustain_level_as_proportion_of_peak
    );
}


MCK::Envelope::Envelope( Envelope const& other )
{
    attack_in_samples = other.attack_in_samples;
    decay_in_samples = other.decay_in_samples;
    release_in_samples = other.release_in_samples;
    sustain_level_as_proportion_of_peak
        = other.sustain_level_as_proportion_of_peak;
}

void MCK::Envelope::operator=( Envelope const& other )
{
    attack_in_samples = other.attack_in_samples;
    decay_in_samples = other.decay_in_samples;
    release_in_samples = other.release_in_samples;
    sustain_level_as_proportion_of_peak
        = other.sustain_level_as_proportion_of_peak;
}

float MCK::Envelope::get_value(
    uint64_t rel_sample_count,
    uint32_t sustain_in_samples
) const
{
    // Check if relative sample count falls 
    // within attack phase
    if( rel_sample_count < this->attack_in_samples )
    {
        // If yes, calculate envelope value in direct
        // proportion to position within attack phase
        // Note: denominator here cannot be zero due
        //       to the above check
        return float( rel_sample_count )
                / float( this->attack_in_samples );
    }

    // Move relative count to beginning of decay phase
    rel_sample_count -= this->attack_in_samples;

    // Check if relative sample count falls 
    // within decay phase
    if( rel_sample_count < this->decay_in_samples )
    {
        // If yes, calculate envelope value in
        // proportion to position within decay phase
        // Note: denominator here cannot be zero due
        //       to the above check
        return 1.0f 
            - float( rel_sample_count )
                * ( 1.0f - this->sustain_level_as_proportion_of_peak )
                    / float( this->decay_in_samples );
    }

    // Move relative count to beginning of sustain phase
    rel_sample_count -= this->decay_in_samples;

    // Check if relative sample count falls 
    // within sustain phase
    if( rel_sample_count < sustain_in_samples )
    {
        // If yes, return sustain proportion
        return this->sustain_level_as_proportion_of_peak;
    }

    // Move relative count to beginning of release phase
    rel_sample_count -= sustain_in_samples;

    // Check if relative sample count falls 
    // within release phase
    if( rel_sample_count < this->release_in_samples )
    {
        // If yes, return envelope value in proportion
        // proportion to position within release phase
        // Note: denominator here cannot be zero due
        //       to the above check
        return this->sustain_level_as_proportion_of_peak
                    * ( 1.0f - float( rel_sample_count )
                        / float( this->release_in_samples )
                    );
    }

    // If sample count is past end of envelope
    // return a negative value to indicate this.
    return -1.0f;
}
