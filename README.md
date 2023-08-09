------------------------------------
---MUCKY VISION 3 (BASIC ENGINE) ---
------------------------------------

README.md

Copyright (c) Muckytators 2023

This program is free software: you can
redistribute it and/or modify it under 
the terms of the GNU General Public License
as published by the Free Software Foundation,
either version 3 of the License, or (at your
option) any later version.

This program, including this file, is distributed
in the hope it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU
General Public License along with this
program. If not, see http://www.gnu.org/license


Contents:

    1. Overview

    2. Current State of Development

    3. Future Plans

    4. Building the Supplied Demo


1. Overview

MuckyVision 3 is the third iteration of a retro game engine, the previous
iterations being closed-source. Key features of MuckyVision are pixelated
'character' graphics, a limited colour palette and a self-contained
executable (i.e. no need for external data files).

MuckyVision 3 is written in C++ and makes use of the SDL2 core library,
although no knowledge of SDL is required to use MuckyVision within a C++
application.


2. Current State of Development

At present, MuckyVision 3 is a collection of C++ header and source files, plus
several additional files (in the 'tests' folder) that build a small demo.
This will eventually become a library, potentially with bindings to other
languages.

MuckyVision's source code is organised in layers. The current commit 
comprises only the lowest layer: "GameEng". This acts as a
wrapper for SDL, so that higher layers have no need to use SDL
commands. This will make it easy to swap out SDL for another library,
should that be necessary in future.

The 'GameEng' layer provides a method of creating textures from
raw data that is especially useful for small two-colour images, of the
type associated with 8bit systems that support “character graphics”.
These images may be created within source code using binary
literals, such as the biplane texture used in tests/GameEngTest.cpp:

        0b00111110,  // ..#####.
        0b10001000,  // #...#...
        0b11111101,  // ######.#
        0b11111111,  // ########
        0b01111101,  // .#####.#
        0b00001000,  // ....#...
        0b00000110,  // .....##.
        0b00000110   // .....##.

I feel this method of creating graphics is close, in spirit, to the 8bit
era. Closer than the use of modern image file formats. It encourages developers to build
large images from small 'character' style elements, rather than importing a single
large image drawn in a modern graphics packages. Of course the importing of large images can
be done if really needed, using a script to insert data directly into the source file.

The 'GameEng' layer also provides a convenient way of managing large sets
of small images, by grouping them together in layers (known as 'blocks').
Hundreds of images can be moved, or made (in)visible, simply by making a small 
change to the block to which they belong. For example, the demo in tests/GameEngTest.cpp
uses two blocks, each comprised of many small identical images, to represent the 
foreground and background of a city scape, and implement a parallax scrolling effect
between the two.

Blocks are hierarchical, so changes to a given block automatically affect
all sub-blocks. This means entire game scenes can be switched on or off,
made to scroll or shake, or be rendered in a different order.

Classes/structs included in this commit:
    
    GameEng: Singleton class that interfaces with SDL.
    
    GameEngRenderInfo: Holds metadata for a single image (position, texture, etc.)
    
    GameEngRenderBlock: A ‘block’ container holding multiple images and other blocks.
    
    KeyEvent: Keyboard input events from user (not yet fully implemented).


3. Future Plans

Completing keyboard input is next on the TODO list, as is building
documentation using Doxgen.

Following that, the next layer added will probably be an "Image" layer providing
a set of predrawn character graphics, including basic ASCII characters and 
a set of primitive blocks/shapes for "PETSCI” style graphics.

Provision of C64 SID chip style audio (using dynamically generated waveforms)
is also a high priority.


4. Building the Supplied Demo

DISCLAIMER: Please do not install any software, or change your machine's
            settings, unless you are competent to do so and confident that
            you can safely reverse any changes you make, if needed. 
            These instructions are provided only as a guide, and are aimed 
            at experienced software engineers, so much important detail is
            omitted.
            The author will not be held responsible for any harm caused
            by following, or attempting to follow, these instructions,
            nor for the content of any third party websites (including
            those mentioned below).
            IF YOU ARE UNSURE WHAT YOU ARE DOING AT ANY POINT, PLEASE
            DO NOT PROCEED.

The 'MuckyVision3/tests' folder contains all the files required to 
build a live demo program, either on Linux or Windows (using MinGW).
Note the file MuckyVision3/include/Version.h provides control over certain
aspects of the build process, although the default settings should work
for both Linux and Windows (MinGW).

Suggestions for Linux:

    Ensure you have the developer version of SDL2 installed (on Debian
    based systems this is the libsdl2-dev package), together with  
    g++ and make (both in the build-essentials package).
    Navigate to MuckyVision3/tests, create an empty folder called 'build'
    (if it does not already exist) and from the 'tests' folder execute:

    make -f makefile_GameEngTest_linux

    You should see a file called GameEngTest, which when executed
    will create a new window showing a small white biplane moving
    over a city of scrolling purple blocks. The demo will run for
    about fourteen seconds, then close automatically. You made need to give
    this file run permission in order to execute it.

Suggestions for Windows (MinGW):

    Make sure you have MinGW (Minimalist GNU for Windows, currently at
    https://sourceforge.net/projects/mingw/) installed, with MinGW
    versions of 'make' and 'g++' included. These should be present
    as 'mingw32-make.exe' and 'mingw32-g++.exe' in your MinGW/bin
    folder. You may also need to add the location of MinGW/bin to
    your PATH environment variable.
    
    Also, you’ll need to download and unpack the MinGW version of SDL2
    to a sensible location on your computer. At the time of writing
    SDL2 is available from https://github.com/libsdl-org/SDL/releases.
    You are looking for a file called something like:
    SDL2-devel-2.??.?-mingw.zip, where ??.? is the current stable
    version of SDL2.
    
    Copy the file SDL2.dll from SDL-2.??.?/i686-w64-mingw32/bin to the
    MuckyVision3/tests folder. If performing the x86 build (see below),
    copy SDL2.dll from SDL-2/??.?/x86_64-w64-mingw32/bin instead.

    Navigate to MuckyVision3/tests and open makefile_GameEngTest_windows_MinGW
    for editing (best to make a backup copy first, just in case).
    On lines 72 and 73, replace the three question marks (???) with
    the actual paths to SDL2 and MinGW on your machine, respectively.
    Save the file and exit.

    Within the 'tests' folder, create an empty folder called 'builds',
    if it does not exist already.

    Then from the 'tests' folder, execute:
    
    mingw32-make.exe -f makefile_GameEngTest_windows_MinGW

    If this fails, check the paths carefully as this is the most
    likely source of error. Also, line 66 of the windows makefile
    provides an option for an x86_64 build, so feel free to try
    this if the default i686 build fails, or produces an executable
    that won't run.

    If successful, you should see a file called GameEngTest.exe, which 
    when executed will create a new window showing a small white biplane
    moving over a city of scrolling purple blocks. The demo will run for
    about fourteen seconds, then close automatically.

    For crisp pixelated graphics, you may need to disable "high-DPI 
    scaling" within the executable's compatibility properties. MuckyVision asks
    SDL to ask Windows to disable this on the user's behalf, but
    Windows does as it pleases!

END OF README.md