////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  Version.h
//
//  This file provides build control
//
//  Comment these definitions in or out
//  depending on the version you wish to build
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

#ifndef MCK_VERSION_H
#define MCK_VERSION_H

// Version number in format A.B.C where
//  A = 2 (to distinguish from the original MuckyVision engine)
//  B = 0 or 1 (0 is pre-production, 1 is production)
//  C = iteration (starts at 1)
#define MCK_VERSION 2.0.3

// Keep this commented OUT unless you are
// cross compiling for Windows *on Linux*
// using MINGW
// #define MCK_MINGW

// Keep this commented IN unless you
// want NO console output
#define MCK_STD_OUT

// Keep this commented IN if you
// want verbose console output
#define MCK_VERBOSE

// Keep this commented IN if you
// want integrity checks
// during render (slower but safer)
#define MCK_RENDER_INTEGRITY_CHK

#endif
