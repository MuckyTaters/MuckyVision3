////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  KeyEvent.h
//
//  Struct to hold key event data
//
//  Note: header only, there is no 'KeyEvent.cpp' file
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

#ifndef MCK_KEY_EVENT_H
#define MCK_KEY_EVENT_H

#include "stdint.h"  // For uint32_t et al.

#ifndef MCK_NO_STD_OUT
#include <iostream> // For std::cout
#include <string>
#endif

#include "Version.h"
#include "Defs.h"

namespace MCK
{

// Key event struct
struct KeyEvent
{
    //! Key codes (independent of physical key assignment)
    /*! Note defined here without 'class' so 'Keys' not
     *  required when referencing these enumerations
     */
    enum Keys
    {
        KEY_INVALID,
        KEY_UP,
        KEY_RIGHT,
        KEY_DOWN,
        KEY_LEFT,
        KEY_SELECT,
        KEY_ESC,
        KEY_QUIT,
        KEY_TOGGLE_AUDIO,
        KEY_HELP,
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,  
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z,
        KEY_TOTAL
    };

    //! Key status at time of event
    enum Status { INVALID, PRESSED, AUTO_REPEAT, RELEASED };

    //! Code of key pressed
    MCK::KeyEvent::Keys key_code;

    //! Time (in ticks) of key press/release
    uint32_t timestamp;

    //! Status (pressed or released)
    MCK::KeyEvent::Status status;

    //! Reset KeyEvent to newly constructed state
    void reset( void ) noexcept
    {
        this->key_code = MCK::KeyEvent::KEY_INVALID;
        this->timestamp = 0;
        this->status = MCK::KeyEvent::INVALID;
    }

    //! Default constructor
    KeyEvent( void ) noexcept { this->reset(); }

    //! Constructor
    KeyEvent(
        MCK::KeyEvent::Keys _key_code,
        uint32_t _timestamp,
        MCK::KeyEvent::Status _status
    )
    {
        this->key_code  = _key_code;
        this->timestamp = _timestamp;
        this->status    = _status;
    }

#ifndef MCK_NO_STD_OUT
    std::string str( void )
    {
        return "KeyEvent{key_code="
               + std::to_string( this->key_code )
               + ",timestamp="
               + std::to_string( this->timestamp )
               + ",status="
               + std::to_string( this->status )
               + "}";
    }
#endif
};

}  // End of namespace MCK

#endif
