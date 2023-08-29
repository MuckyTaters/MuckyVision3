////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  VoiceSynth.h
//
//  Provides "Synth" style waveforms  
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

#ifndef MCK_VOICE_CHIPTUNE_H
#define MCK_VOICE_CHIPTUNE_H

#include <vector>  // For vector
#include <math.h>  // For pow

#include "VoiceBase.h"
#include "Envelope.h"

namespace MCK
{

class VoiceSynth : public VoiceBase
{
    public:

        //! Different waveforms offered by Synth voice.
        enum class Waveform
        {
            SINE,
            SQUARE,
            TRIANGLE,
            SAWTOOTH,
            WHITENOISE
        };

        VoiceSynth( void );

        ~VoiceSynth( void ) {}

        //! Initialize the voice
        /*! Note: _envelope is not passed by ref as it is typically
         *        supplied as an r-value
         */
        virtual void init(
            uint32_t _samples_per_second,
            MCK::VoiceSynth::Waveform _wave,
            uint8_t _lowest_octave,
            MCK::Envelope _envelope = MCK::Envelope(),
            uint8_t initial_volume = 0xFF,
            bool _sliding_transition = false,
            uint8_t _duty_cycle = 0x80,
            uint8_t _vibrato_mag = 0x00,
            uint16_t vibrato_freq = 0x00
        );

        //! Issue a command (i.e. play a note)
        /* Bits 0-4 note number (determines pitch). 
         * Bits 5-7 note length (as power of 2, 2^0 being an eight)
         */
        virtual void command( uint8_t com );

        //! Get sample at given time point
        /*! Return value must lie within range [-v,v]
         *  where 'v' = channel volume (0-255) / 255 / num of virtual channels
         */
        virtual float get_sample( uint64_t sample_count );


    protected:

        // Wavelengths, indexed by note id.
        std::vector<uint32_t> wavelen_by_note_id;

        // Frequencies, indexed by note id
        std::vector<double> freq_by_note_id;

        MCK::VoiceSynth::Waveform wave;
        uint8_t lowest_octave;
        MCK::Envelope envelope;
        bool sliding_transition;
        float duty_cycle;
        float vibrato_mag;
        float vibrato_wavelen_in_samples;

        uint64_t starting_sample_count;
    
        // Value 1/12, pre-calculated at compile time
        const static double ONE_TWELTH;

    private:

        // Copy and assignment constructor hidden to
        // prevent misuse.
        VoiceSynth(VoiceSynth const&) = delete;
        void operator=(VoiceSynth const&)  = delete;
};

}  // End of namespace MCK

#endif
