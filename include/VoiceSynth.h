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

#ifndef MCK_VOICE_SYNTH_H
#define MCK_VOICE_SYNTH_H

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
        enum Waveform
        {
            SINE,
            SQUARE,
            TRIANGLE,
            SAWTOOTH,
            WHITENOISE
        };

        VoiceSynth( void );

        virtual ~VoiceSynth( void ) {}

        //! Initialize the voice
        /*  @param _sixteenth_duration_in_samples: duration of a 'sixteenth' note (sets tempo) 
         *  @param MCK::VoiceSynth::Waveform _wave: Waveform
         *  @param uint8_t _lowest_octave: Lowest pitch ID will be C in this octave (sets overall pitch)
         *  @param MCK::Envelope _envelope: ADSR volume envelope
         *  @param uint8_t volume: Volume (0 = silent, 255 = full)
         *  Note: _envelope is NOT passed by ref as it is typically
         *        supplied as an r-value
         */
        virtual void init(
            uint32_t _sixteenth_duration_in_samples, 
            MCK::VoiceSynth::Waveform _wave,
            uint8_t _lowest_octave,
            MCK::Envelope _envelope = MCK::Envelope(),
            uint8_t volume = 0xFF
        );

        //! Issue a command (i.e. play a note)
        /*! @param com: Command (see below)
         *  @param sample_count: Current sample count
         * Command:
         *  A single byte combination of pitch id (relative 
         *  to lowest octave) and duration id.
         *  The nummber and position of bits given to pitch 
         *  and duration are specified in Defs.h.
         *  Please use the 'construct_command' method to correctly
         *  form a command from pitch ID and duration ID.
         */
        virtual void command(
            uint8_t com,
            uint64_t sample_count
        );

        //! Get sample at given time point
        /*! @param sample_count: Current sample count
         *  @returns: value in range [-v,v] where 'v' = channel volume (0-255) ÷ 255 ÷ MCK_NUM_VOICES
         */
        virtual float get_sample( uint64_t sample_count );

        //! Construct command by packing pitch ID and duration ID into a single byte
        /*! IMPORANT: If pitch ID and/or duration ID are
         *  out-of-range, garbage will result (no checking is done)
         */
        static uint8_t construct_command(
            uint8_t pitch_id,
            uint8_t duration_id
        ) noexcept;
    

    protected:

        // Wavelengths, indexed by note id.
        std::vector<float> wavelen_by_note_id;

        // Frequencies, indexed by note id
        std::vector<float> freq_by_note_id;

        MCK::VoiceSynth::Waveform wave;
        uint8_t lowest_octave;
        MCK::Envelope envelope;
        bool sliding_transition;

        uint64_t starting_sample_count;

        // Used during sampling to remember envelope value
        float envelope_value;

        // Used during sampling to save looking wavelength up
        float note_wavelen;

        // Sustain value, in samples, of current note
        uint32_t sustain;

        //! Value 1/12, pre-calculated at compile time
        const static double ONE_TWELTH;

        //! Duration of a 'sixteenth' note, in whole milliseconds
        uint32_t sixteenth_duration_in_samples; 


    private:

        // Copy and assignment constructor hidden to
        // prevent misuse.
        VoiceSynth(VoiceSynth const&) = delete;
        void operator=(VoiceSynth const&)  = delete;
};

}  // End of namespace MCK

#endif
