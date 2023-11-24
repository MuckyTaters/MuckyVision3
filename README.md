------------------------------------
---MUCKY VISION 3 (BASIC ENGINE) ---
------------------------------------

README.md

Copyright (c) Muckytaters 2023

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

PLEASE ALSO SEE THE STATEMENT IN FILE 'IMAGE_RIGHTS_DISCLAIMER.md'.


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
several additional files (in the 'tests' folder) that build a series of small demos
programs. This will eventually become a library, potentially with bindings to other
languages.

MuckyVision's source code is organised in layers. The current commit 
comprises the lowest layer: "GameEng", the layer above "ImageMan",
a new sprite/collision layer, and the utility classes "ImageText",
"Console" and "VoiceSynth" (a synthesiser that works with the new 
audio manager). A number of geometric class templates have been added 
to the 'include' folder such as 'Point', 'LineSegment' and various 
Bezier curves (see below).

GameEng acts as a wrapper for SDL, so that higher layers have no need
to use SDL commands. This will make it easy to swap out SDL for another
library, should that be necessary in future.

GameEng now includes an audio manager, GameEngAudio, which acts as
a wrapper for SDL audio commands, and includes an audio callback
(a separate thread that ensures your computer's sound system is fed
with data in a timely manner). Given MinGW's limited support for C++11
parallelization techniques, GameEngAudio instead uses a ring buffer to
receive instructions from the main thread.

ImageMan is an image manager, it maintains pointers to binary image data
and colo(u)r palette information, and provides a less cumbersome way of
creating images. A in-built set of 256 ASCII characters is provided,
but users can also supply their own partial or complete ASCII character
sets. For partial character sets, any undefined characters default to their
in-built character image. Up to 255 custom ASCII character sets can be used
simultaneously, plus the in-built set.

Sprites are now provided by the 'Sprite.h' class template, which is
comprised of three sub-classes: motion, animation and collision.

Sprite motion options are currently:

    * SpriteMotionBase.h : No motion
    * SpriteMotionConstVel.h : Velocity can be set arbitrarily, but constant otherwise 

Sprite animation options are currently:

    * SpriteAnimBase.h : No animation
    * SpriteAnimTime.h : Each frame displayed for a set number of ticks
    * SpriteAnimDist.h : Each frame displayed for a set (non-integer) number of pixels
    * SpriteAnimWalk.h : As SpriteAnimDist but motion is frozen between frames
    
Sprite collision options are currently:

    * SpriteCollisionBase.h : No collision
    * SpriteCollisionRect.h : Rectangular collision boundary
    * SpriteCollisionCircle.h : Circular collision boundary

These will be extended to provide more motion, animation and collision detection options,
including Bezier curve motion, distance-based (i.e. walking) anmiation and per-pixel
collision detection. 

New supporting classes/structs are SpriteFrame.h (providing animation frames)
and SpritePos.h (providing basic position info inherited by motion, animation
and collision sub-classes).

The template class CollisionProcessing.h provides sprite collision processing,
supported by a quad tree (QuadTree.h) that contains sprite collision info,
currently provided by CollisionNode.h.

ImageText converts C++ strings into ASCII images, with option for text
spacing and left/right/center justification. Text can be updated
dynamically.

Console combines one or more ImageText objects to produce a smooth scrolling
text console, with text and line spacing. The developer can 'dump' text into the
console, as a C++ string, and this will be fed automatically, character by
character, into the console. Character printing speed and scroll speed can be
controlled, and text can be added either in rows or columns, scrolling up
or left respectively.

VoiceSynth is the first specialisation of the VoiceBase class.  A 'voice' in 
this context is a software audio channel, as distinct from a hardware audio channel,
e.g. left speaker and right speaker. Each voice can generate a single sound 
at any point in time, and up to eight voices can play at once. The maximum volume 
of each voice is constained so that the total volume never exceeds system capacity.
However, the mixing is quite basic: each voice is allocated an equal share of the 
volume 'bandwith', regardless of whether it is playing or not.

VoiceSynth provides simple SID chip style waveforms in a ADSR volume envelope.
It can play notes of varying duration at standard pitchs (using 440Hz or 432Hz tuning)
across a range of octaves. VoiceSynth characteristics (including waveform and octave range)
are fixed when the voice is initialised, but you are free to vary settings between
VoiceSynth instances. For example, you could have a high-pitched sine wave instance,
a low-pitched square wave instance and a white noise instance all playing simultaneously.

In preparation for Bezier sprite motion, the following class templates have been added
to provide a suite of geometry tools. These will eventually be used to control sprite paths.
    
    * Vect2D.h simplified 2D verion of Point.h
    * Point.h describes a point in 3D space, although can be used solely in 2D space.
    * BezierCurveBase.h provides a common base for Bezier curve classes.
    * BezierCurveLinear.h describes a straight line between two Point instances.
    * BezierCurveQuad.h describes a curved line between two Points, with one additional control Point.
    * BezierCurveCubic.h describes a straight line between two Points, with two additional control Points.
    * LineSegment.h provides estimation of arc length for any curve instance with fixed end/control Points.

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
    
    GameEngAudio: All-static class that interfaces with SDL audio.

    GameEngRenderInfo: Holds metadata for a single image (position, texture, etc.)
    
    GameEngRenderBlock: A ‘block’ container holding multiple images and other blocks.
    
    KeyEvent: Keyboard input events from user (not yet fully implemented).

    ImageMan: Singleton class that provides image data and palette management.

    ImageDataASCII: An extended ASCII set of 256 8x8 pixel images.

    ImageText: A 'text box' that visualizes C++ strings.

    Console: A 'window' of smooth scrolling text, printed one character at a time.

    Envelope: ADSR (Attack, Decay, Sustain, Release) volume envelope for audio.

    VoiceBase: Abstract base class for all 'voice' classes.

    VoiceSynth: Simple SID chip style sound synthesiser.

    Also see sprite, collision and  geometry class templates described above (these have no associated .cpp file)


3. Future Plans

Next on the TODO list for graphics is to continue work on sprites
and collision detection. The development of geometry classes has
been a step towards this.

The next step for audio is to implement a self-playing VoiceSynth class.
At present, the main program holds the song data and sends commands to the
VoiceSynth instances to play notes. This is fine for interactive audio 
applications (e.g. a tracker), but for games it is more convenient for the
Voice class itself to manage the playing of songs independently.


4. Building the Supplied Demos

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
build three live demo programs, either on Linux or Windows (using MinGW).
Note the file MuckyVision3/include/Version.h provides control over certain
aspects of the build process, although the default settings should work
for both Linux and Windows (MinGW).

For the other demos (ImageManTest, ConsoleTest, FancyConsoleTest and 
GameAudioTest, etc.) replace all references to 'GameEng' below
with the respective demo name, including where it is part of a larger word.
Hence 'makefile_GameEngTest_linux' becomes 'makefile_ImageManTest_linux'
or 'makefile_FancyConsole_linux' or whatever. Note that lines numbers in 
the other makefiles may differ slightly.


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
    a while, then close automatically. You made need to give
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
    likely source of error. Also, line 67 of the windows makefile
    provides an option for an x86_64 build, so feel free to try
    this if the default i686 build fails, or produces an executable
    that won't run.

    If successful, you should see a file called GameEngTest.exe, which 
    when executed will create a new window showing a small white biplane
    moving over a city of scrolling purple blocks. The demo will run for
    a while, then close automatically.

    For crisp pixelated graphics, you may need to disable "high-DPI 
    scaling" within the executable's compatibility properties. MuckyVision asks
    SDL to ask Windows to disable this on the user's behalf, but
    Windows does as it pleases!

END OF README.md
