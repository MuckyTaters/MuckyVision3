////////////////////////////////////////////
//  ------------------------------------
//  ---MUCKY VISION 3 (BASIC ENGINE) ---
//  ------------------------------------
//
//  SpritePos.cpp
//
//  Virtual base class for *all* sprites 
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

#include "SpritePos.h"

uint32_t MCK::SpritePos::current_ticks = 0;
uint32_t MCK::SpritePos::prev_ticks = 0;
uint32_t MCK::SpritePos::ticks_elapsed = 0;
const MCK::GameEng* MCK::SpritePos::game_eng = NULL;
const MCK::ImageMan* MCK::SpritePos::image_man = NULL;
