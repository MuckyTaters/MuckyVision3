////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  Defs.h
//
//  Common definitions file
//
//  Note: header only, there is no 'Defs.cpp' file
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

#ifndef MCK_DEFS_H
#define MCK_DEFS_H

#include<cstdint>  // For uint32_t et al.
#include "stddef.h" // For 'size_t' guaranteed in global namespace

// Defines data type used for object IDs
#define MCK_IMG_ID_TYPE uint32_t
#define MCK_PAL_ID_TYPE uint32_t
#define MCK_TEX_ID_TYPE uint64_t
#define MCK_PI 3.14127f
#define MCK_TWO_PI 6.28254f
#define MCK_ONE_OVER_ROOT_TWO 0.707106f

namespace MCK
{
    //! Ceiling/invalid value for image ids
    const MCK_IMG_ID_TYPE INVALID_IMG_ID = 0xFFFFFFFF;

    //! Ceiling/invalid value for local palette ids
    const MCK_IMG_ID_TYPE INVALID_PAL_ID = 0xFFFFFFFF;

    //! Ceiling/invalid value for texture ids
    const MCK_TEX_ID_TYPE INVALID_TEX_ID = 0xFFFFFFFFFFFFFFFF;

    //! Ceiling/invalid value for audio sample counts
    const uint64_t AUDIO_INVALID_SAMPLE_COUNT = 0xFFFFFFFFFFFFFFFF;

    //! Safety parameter - max window width
    const uint16_t MAX_WINDOW_WIDTH_IN_PIXELS = 4096;
    
    //! Safety parameter - max window height
    const uint16_t MAX_WINDOW_HEIGHT_IN_PIXELS = 2048;

    //! Expected max number of local colo(u)r palettes, as a power of 2
    /* Note: This is a 'soft' upper limit, exceeding it
     *       is permissible but may reduce efficiency
     *       during initialization.
     */
    const uint16_t MAX_LOCAL_COLOR_PALETTES = 1024; 

    //! Expected max number of images, as a power of 2
    /* Note: This is a 'soft' upper limit, exceeding it
     *       is permissible but may cause delays during
     *       imitialization.
     * Note: An 'image' here means just pixel data, so many
     *       textures can be created from one image.
     */
    const uint16_t MAX_IMAGES = 2048; 

    //! Colo(u)r codes for core palette
    /*! Note US spelling of GRAY used here
     *  These are separate values rather than an enumeration
     *  so that it is easier to use them as integers*/
    const uint8_t COL_BLACK = 0;
    const uint8_t COL_BLACK_SEMI_TRANS = 1;
    const uint8_t COL_DARK_GRAY = 2;
    const uint8_t COL_DARK_RED_SEMI_TRANS = 3;
    const uint8_t COL_TRANSPARENT = 4;
    const uint8_t COL_WHITE = 5;
    const uint8_t COL_LIGHT_GRAY = 6;
    const uint8_t COL_MID_GRAY = 7;
    const uint8_t COL_V_DARK_GRAY = 8;
    const uint8_t COL_DARK_RED = 9;
    const uint8_t COL_DARK_GREEN = 10;
    const uint8_t COL_DARK_BLUE = 11;
    const uint8_t COL_PINK = 12;
    const uint8_t COL_PASTEL_GREEN = 13;
    const uint8_t COL_PASTEL_BLUE = 14;
    const uint8_t COL_PASTEL_YELLOW = 15;
    const uint8_t COL_RED = 16;
    const uint8_t COL_GREEN = 17;
    const uint8_t COL_BLUE = 18;
    const uint8_t COL_YELLOW = 19;
    const uint8_t COL_CYAN = 20;
    const uint8_t COL_MAGENTA = 21;
    const uint8_t COL_ORANGE = 22;
    const uint8_t COL_LIME = 23;
    const uint8_t COL_PURPLE = 24;
    const uint8_t COL_FOREST_GREEN = 25;
    const uint8_t COL_FIELD_GRAY = 26;
    const uint8_t COL_ROTTING_PURPLE = 27;
    const uint8_t COL_FLESHTONE = 28;
    const uint8_t COL_SAND = 29;
    const uint8_t COL_MID_BROWN = 30;
    const uint8_t COL_DARK_BROWN = 31;
    const uint8_t TOTAL_CORE_COLORS = 32;
    
    const uint8_t COL_INVALID = 255;

    //! ASCII value of blank character in ImageText
    const uint8_t BLANK_CHAR = 32;

    //! Height and width (in pixels) of blank textures
    const uint8_t BLANK_TEX_SIZE = 8;

    //! Maximum character spacing, in pixels (max is 255)
    const uint8_t MAX_CHAR_SPACING = 32;

    //! Maximum line spacing, in pixels (max is 255)
    const uint8_t MAX_LINE_SPACING = 64;

    ////////////////////////////////
    // AUDIO
   
    // Virtual audio channels are software generated,
    // and are not related to system audio channels.
    // Each virtual channel can play one 'chunk' (sound)
    // at a time. You can have 1, 2, 4 or 8 virtual
    // channels in MuckyVision v3. The higher the number
    // the more sounds to be played simulatenously, but
    // individual sounds are quieter when more virtual
    // channels used. This is because speaker bandwith
    // is shared between all virtual channels, even
    // those not producing sound at that time.
#define MCK_NUM_VOICES 8

    // This is an internal data type used by GameEngAudio,
    // and it is dependent on the number of virtual channels
#if MCK_NUM_VOICES == 8
#define MCK_AUDIO_RING_BUFFER_DATA_TYPE uint64_t
#elif MCK_NUM_VOICES == 4
#define MCK_AUDIO_RING_BUFFER_DATA_TYPE uint32_t
#elif MCK_NUM_VOICES == 2
#define MCK_AUDIO_RING_BUFFER_DATA_TYPE uint16_t
#elif MCK_NUM_VOICES == 1
#define MCK_AUDIO_RING_BUFFER_DATA_TYPE uint8_t
#endif

    //! This is calculated at compile time, do not alter
    const size_t AUDIO_NUM_VOICES_MASK
        = ( uint8_t( 1 ) << MCK_NUM_VOICES ) - 1;
   
    //! First choice of audio format
    const int AUDIO_WANT_FORMAT = 32784;  // == AUDIO_S16SYS ( SDL macro for signed short integer)

    //! First choice of audio sample rate
    const int AUDIO_WANT_SAMPLE_RATE = 44100;

    //! First choice for number of system audio channels
    /* This should be set to 1 (mono), as MuckyVision v3
     * does not currently support stereo. */
    const int AUDIO_WANT_CHANNELS = 1;

    //! First choice of audio buffer size (choice is usually linked to audio format)
    const int AUDIO_WANT_BUFFER_SIZE = 2048;

    //! Size (in game ticks) of ring buffer in GameEngAudio
    /* This buffer enables GameEngAudio to take instructions
     * from the main program. It is synchronised with game time,
     * and each element represents one 'tick' (millisecond).
     * The size (which must be a power of 2) should be set 
     * to be comfortably larger than the maximum expected gap
     * (in milliseconds) between audio callbacks.
     * ** This number must be power of 2. **/
    const size_t AUDIO_RING_BUFFER_SIZE = 0X200;

    //! This is calculated at compile time, do not alter
    const size_t AUDIO_RING_BUFFER_SIZE_MASK
        = AUDIO_RING_BUFFER_SIZE - 1;
  
    //! Minimum time lag (in ticks, i.e. milliseconds) between ring buffer input and output
    const size_t AUDIO_RING_BUFFER_LAG_IN_TICKS = 2;


    //! Default volume for individual channels (0x00 = mute, 0xFF = max)
    const uint8_t AUDIO_DEFAULT_CHANNEL_VOLUME = 0x80;

    enum class AudioDataType
    {
        UNKNOWN,
        SIGNED_16_BIT_INT,
        SIGNED_32_BIT_FLOAT
    };

    //! Provides reflection to voice classes
    /*! Do not alter this enumeration unless you are
     *  creating/removing a voice class and know
     *  what you are doing.
     */
    enum class VoiceType
    {
        BASE,
        SYNTH
    };

    //! Provides reflection to render instance classes
    /*! Do not alter this enumeration unless you are
     *  creating/removing a render instance class and know
     *  what you are doing.
     */
    enum class RenderInstanceType
    {
        BASE,
        INFO,
        BLOCK
    };

    //! Frequency, in integer Hertz, of tuning note A4 (440Hz or 432Hz)
    const int VOICE_SYNTH_FREQ_A4 = 440;

    //! Max vibrato magnitude equals max voice magnitude divided by this number
    /* E.g. a value of 4 means max vibrato magnitude is 25%
     * of max voice magnitude. So, if max vibrato magnitude is
     * selected for a particular voice, that voice's volume
     * should be set to no more than 75%, otherwise clipping 
     * may occur.
     */
    // const int VOICE_SYNTH_MAX_VIB_PROPORTION = 4;

    //! These values define the bit content of the commands send to VoiceSynth
    const uint8_t VOICE_SYNTH_PITCH_MASK = 0x3F;
    const uint8_t VOICE_SYNTH_PITCH_LSHIFT = 0;
    const uint8_t VOICE_SYNTH_DURATION_MASK = 0xC0;
    const uint8_t VOICE_SYNTH_DURATION_LSHIFT = 6;

    //! Calculate envelope value every 'n' samples, where 'n' is this value + 1 (and 'n' must be power of 2)
    const uint64_t VOICE_SYNTH_ENVELOPE_INTERVAL_MASK = 0x0F;

    //! Default x coord value
    const uint32_t DEFAULT_X_VALUE = 0;
    
    //! Default y coord value
    const uint32_t DEFAULT_Y_VALUE = 0;
    
    //! Minimum z value (determines within-block render order)
    const uint32_t MIN_Z_VALUE = 0x0;

    //! Default z value (determines within-block render order)
    const uint32_t DEFAULT_Z_VALUE = 0x80000000;
    
    //! Maximum z value (determines within-block render order)
    const uint32_t MAX_Z_VALUE = 0xFFFFFFFF;

}  // End of namespace MCK

#endif
