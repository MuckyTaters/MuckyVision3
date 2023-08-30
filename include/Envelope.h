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

#include<cstdint>  // For uint32_t et al.

namespace MCK
{

class Envelope
{
    public:

        //! Allow certain classes direct access to sustain level
        friend class VoiceSynth;

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
        Envelope( void );

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
        );

        virtual ~Envelope( void ) {}

        //! Copy constructor
        Envelope( Envelope const& other );
        
        //! Assignment constructor
        void operator=( Envelope const& other );

        uint32_t get_attack_in_samples( void ) const noexcept
        {
            return attack_in_samples;
        }

        uint32_t get_decay_in_samples( void ) const noexcept
        {
            return decay_in_samples;
        }

        uint32_t get_release_in_samples( void ) const noexcept
        {
            return release_in_samples;
        }
       
        //! Get sustain level
        // @returns: Sustain level: 0 (no sustain) to 255 (sustain level == peak level )
        uint8_t get_sustain_level_as_proportion_of_peak( void ) const noexcept
        {
            return uint8_t(
                sustain_level_as_proportion_of_peak * 255.0f + 0.5f
            );
        }
        
        void set_attack_in_samples( uint32_t val ) noexcept
        {
            attack_in_samples = val;
        }

        void set_decay_in_samples( uint32_t val ) noexcept
        {
            decay_in_samples = val;
        }

        void set_release_in_samples( uint32_t val ) noexcept
        {
            release_in_samples = val;
        }

        //! Set sustain level
        /*! @param val: Sustain level: 0 (no sustain) to 255 (sustain level == peak level )
         */
        void set_sustain_level_as_proportion_of_peak( uint8_t val ) noexcept
        {
            sustain_level_as_proportion_of_peak
                = float( val ) / 255.0f;
        }

        //! Get envelope value at given sample count
        /*! @param rel_sample_count: Sample count relative to *start* of envelope
         *  @param sustain_in_samples: Sustain time, in samples
         *  @returns: Envelope value in range [0,1], OR -1 if count is past end of envelope
         */
        float get_envelope_value(
            uint64_t rel_sample_count,
            uint32_t sustain_in_samples
        ) const;


    private:

        uint32_t attack_in_samples;
        uint32_t decay_in_samples;
        uint32_t release_in_samples;

        // This is must be kept on interval [0,1]
        float sustain_level_as_proportion_of_peak;
};

}  // End of namespace MCK

#endif
