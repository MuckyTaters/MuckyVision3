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

#ifndef MCK_ENVELOPE_H
#define MCK_ENVELOPE_H

namespace MCK
{

struct Envelope
{
    //! Allow certain classes direct access to sustain level
    friend class VoiceSynth;

    uint32_t attack_in_samples;
    uint32_t decay_in_samples;
    uint32_t release_in_samples;
    
    uint8_t get_sustain_level_as_proportion_of_peak( void ) const noexcept
    {
        return uint8_t(
            sustain_level_as_proportion_of_peak * 255.0f + 0.5f
        );
    }
    
    void set_sustain_level_as_proportion_of_peak( uint8_t val ) noexcept
    {
        sustain_level_as_proportion_of_peak
            = float( val ) / 255.0f;
    }

    //! Create envelope
    /*! @param attack_in_samples: Attack time 
     *  @param decay_in_samples: Decay time
     *  @param release_in_samples: Release time
     *  @param sustain_level_as_proportion_of_peak: as per name
     *  Note1: Sustain time (in samples) is determined 
     *         by length of note, so it is not included in
     *         this data structure.
     *  Note2: All times are measured in sample counts.
     *         There are typically 44100 samples in one second,
     *         so e.g. 1/20th second = 2205 samples.
     */
    Envelope( void )
    {
        // Default 'square' ADSR envelope
        attack_in_samples = 0;
        decay_in_samples = 0;
        release_in_samples = 0;
        sustain_level_as_proportion_of_peak = 1.0f;
    }

    // Create default (square) envelope
    /*! Note: Sustain time (in samples) is determined 
     *  by length of note, so it is not included in
     * this data structure.
     */
    Envelope(
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

    virtual ~Envelope( void ) {}

    //! Copy constructor
    Envelope( Envelope const& other )
    {
        attack_in_samples = other.attack_in_samples;
        decay_in_samples = other.decay_in_samples;
        release_in_samples = other.release_in_samples;
        sustain_level_as_proportion_of_peak
            = other.sustain_level_as_proportion_of_peak;
    }
    
    //! Assignment constructor
    void operator=( Envelope const& other )
    {
        attack_in_samples = other.attack_in_samples;
        decay_in_samples = other.decay_in_samples;
        release_in_samples = other.release_in_samples;
        sustain_level_as_proportion_of_peak
            = other.sustain_level_as_proportion_of_peak;
    }
    
    private:

        // This is kept private to ensure it is on internal [0,1]
        float sustain_level_as_proportion_of_peak;
};

}  // End of namespace MCK

#endif
