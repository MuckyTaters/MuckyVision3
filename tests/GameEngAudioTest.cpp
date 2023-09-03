///////////////////////////////////////////////
//  --------------------------------------
//  ---MUCKY VISION 3 ( BASIC ENGINE ) ---
//  --------------------------------------
//
 //  GameEngAudioTest.cpp
//
//  Test/demo for GameEngAudio class.
//  structs GameEngRenderInfo and GameEngRenderBlock
//
//  Copyright (c) Muckytaters 2023
//
//  For the contents of this file EXCEPT the
//  data in SONG_DATA:
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
//
//  ------------------------------------------------
//
//  The contents of SONG_DATA are adapted from
//  a midi file by 'Robert Lavigne' downloaded from:
//  https://imslp.org/wiki/Special:ReverseLookup/219533
//  
//  This midi file is licensed under:
//  Creative Commons Attribution Non-commercial Share Alike 3.0 
//  https://creativecommons.org/licenses/by-nc-sa/3.0/legalcode
//
////////////////////////////////////////////

#include "GameEng.h"
#include "ImageMan.h"
#include "GameEngAudio.h"
#include "VoiceSynth.h"
#include "Console.h"

const std::vector<uint32_t> SONG_DATA
{
    // TICKS,VOICE_ID,RAW_NOTE_ID,DURATION_ID,
    833,0,76,1,
    1042,0,75,1,
    1250,0,76,1,
    1458,0,75,1,
    1667,0,76,1,
    1875,0,71,1,
    2083,0,74,1,
    2292,0,72,1,
    2500,0,69,2,
    2500,1,45,1,
    2708,1,52,1,
    2917,1,57,1,
    3125,0,60,1,
    3333,0,64,1,
    3542,0,69,1,
    3750,0,71,2,
    3750,1,40,1,
    3958,1,52,1,
    4167,1,56,1,
    4375,0,64,1,
    4583,0,68,1,
    4792,0,71,1,
    5000,0,72,2,
    5000,1,45,1,
    5208,1,52,1,
    5417,1,57,1,
    5625,0,64,1,
    5833,0,76,1,
    6042,0,75,1,
    6250,0,76,1,
    6458,0,75,1,
    6667,0,76,1,
    6875,0,71,1,
    7083,0,74,1,
    7292,0,72,1,
    7500,0,69,2,
    7500,1,45,1,
    7708,1,52,1,
    7917,1,57,1,
    8125,0,60,1,
    8333,0,64,1,
    8542,0,69,1,
    8750,0,71,2,
    8750,1,40,1,
    8958,1,52,1,
    9167,1,56,1,
    9375,0,64,1,
    9583,0,72,1,
    9792,0,71,1,
    10000,0,69,3,
    10000,1,45,1,
    10208,1,52,1,
    10417,1,57,1,
    10833,0,76,1,
    11042,0,75,1,
    11250,0,76,1,
    11458,0,75,1,
    11667,0,76,1,
    11875,0,71,1,
    12083,0,74,1,
    12292,0,72,1,
    12500,0,69,2,
    12500,1,45,1,
    12708,1,52,1,
    12917,1,57,1,
    13125,0,60,1,
    13333,0,64,1,
    13542,0,69,1,
    13750,0,71,2,
    13750,1,40,1,
    13958,1,52,1,
    14167,1,56,1,
    14375,0,64,1,
    14583,0,68,1,
    14792,0,71,1,
    15000,0,72,2,
    15000,1,45,1,
    15208,1,52,1,
    15417,1,57,1,
    15625,0,64,1,
    15833,0,76,1,
    16042,0,75,1,
    16250,0,76,1,
    16458,0,75,1,
    16667,0,76,1,
    16875,0,71,1,
    17083,0,74,1,
    17292,0,72,1,
    17500,0,69,2,
    17500,1,45,1,
    17708,1,52,1,
    17917,1,57,1,
    18125,0,60,1,
    18333,0,64,1,
    18542,0,69,1,
    18750,0,71,2,
    18750,1,40,1,
    18958,1,52,1,
    19167,1,56,1,
    19375,0,64,1,
    19583,0,72,1,
    19792,0,71,1,
    20000,0,69,2,
    20000,1,45,1,
    20208,1,52,1,
    20417,1,57,1,
    20625,0,71,1,
    20833,0,72,1,
    21042,0,74,1,
    21250,0,76,2,
    21250,1,48,1,
    21458,1,55,1,
    21667,1,60,1,
    21875,0,67,1,
    22083,0,77,1,
    22292,0,76,1,
    22500,0,74,2,
    22500,1,43,1,
    22708,1,55,1,
    22917,1,59,1,
    23125,0,65,1,
    23333,0,76,1,
    23542,0,74,1,
    23750,0,72,2,
    23750,1,45,1,
    23958,1,52,1,
    24167,1,57,1,
    24375,0,64,1,
    24583,0,74,1,
    24792,0,72,1,
    25000,0,71,2,
    25000,1,40,1,
    25208,1,52,1,
    25417,1,64,1,
    25625,0,64,1,
    25833,0,76,1,
    26042,1,64,1,
    26250,1,76,1,
    26458,0,76,1,
    26667,0,88,1,
    26875,1,75,1,
    27083,1,76,1,
    27292,0,75,1,
    27500,0,76,1,
    27708,1,75,1,
    27917,1,76,1,
    28125,0,75,1,
    28333,0,76,1,
    28542,0,75,1,
    28750,0,76,1,
    28958,0,75,1,
    29167,0,76,1,
    29375,0,71,1,
    29583,0,74,1,
    29792,0,72,1,
    30000,0,69,2,
    30000,1,45,1,
    30208,1,52,1,
    30417,1,57,1,
    30625,0,60,1,
    30833,0,64,1,
    31042,0,69,1,
    31250,0,71,2,
    31250,1,40,1,
    31458,1,52,1,
    31667,1,56,1,
    31875,0,64,1,
    32083,0,68,1,
    32292,0,71,1,
    32500,0,72,2,
    32500,1,45,1,
    32708,1,52,1,
    32917,1,57,1,
    33125,0,64,1,
    33333,0,76,1,
    33542,0,75,1,
    33750,0,76,1,
    33958,0,75,1,
    34167,0,76,1,
    34375,0,71,1,
    34583,0,74,1,
    34792,0,72,1,
    35000,0,69,2,
    35000,1,45,1,
    35208,1,52,1,
    35417,1,57,1,
    35625,0,60,1,
    35833,0,64,1,
    36042,0,69,1,
    36250,0,71,2,
    36250,1,40,1,
    36458,1,52,1,
    36667,1,56,1,
    36875,0,64,1,
    37083,0,72,1,
    37292,0,71,1,
    37500,0,69,2,
    37500,1,45,1,
    37708,1,52,1,
    37917,1,57,1,
    38125,0,71,1,
    38333,0,72,1,
    38542,0,74,1,
    38750,0,76,3,
    38750,1,48,1,
    38958,1,55,1,
    39167,1,60,1,
    39375,0,67,1,
    39583,0,77,1,
    39792,0,76,1,
    40000,0,74,2,
    40000,1,43,1,
    40208,1,55,1,
    40417,1,59,1,
    40625,0,65,1,
    40833,0,76,1,
    41042,0,74,1,
    41250,0,72,2,
    41250,1,45,1,
    41458,1,52,1,
    41667,1,57,1,
    41875,0,64,1,
    42083,0,74,1,
    42292,0,72,1,
    42500,0,71,2,
    42500,1,40,1,
    42708,1,52,1,
    42917,1,64,1,
    43125,0,64,1,
    43333,0,76,1,
    43542,1,64,1,
    43750,1,76,1,
    43958,0,76,1,
    44167,0,88,1,
    44375,1,75,1,
    44583,1,76,1,
    44792,0,75,1,
    45000,0,76,1,
    45208,1,75,1,
    45417,1,76,1,
    45625,0,75,1,
    45833,0,76,1,
    46042,0,75,1,
    46250,0,76,1,
    46458,0,75,1,
    46667,0,76,1,
    46875,0,71,1,
    47083,0,74,1,
    47292,0,72,1,
    47500,0,69,2,
    47500,1,45,1,
    47708,1,52,1,
    47917,1,57,1,
    48125,0,60,1,
    48333,0,64,1,
    48542,0,69,1,
    48750,0,71,2,
    48750,1,40,1,
    48958,1,52,1,
    49167,1,56,1,
    49375,0,64,1,
    49583,0,68,1,
    49792,0,71,1,
    50000,0,72,2,
    50000,1,45,1,
    50208,1,52,1,
    50417,1,57,1,
    50625,0,64,1,
    50833,0,76,1,
    51042,0,75,1,
    51250,0,76,1,
    51458,0,75,1,
    51667,0,76,1,
    51875,0,71,1,
    52083,0,74,1,
    52292,0,72,1,
    52500,0,69,2,
    52500,1,45,1,
    52708,1,52,1,
    52917,1,57,1,
    53125,0,60,1,
    53333,0,64,1,
    53542,0,69,1,
    53750,0,71,2,
    53750,1,40,1,
    53958,1,52,1,
    54167,1,56,1,
    54375,0,64,1,
    54583,0,72,1,
    54792,0,71,1,
    55000,0,69,2,
    55000,1,45,1,
    55208,1,52,1,
    55417,1,57,1,
    55625,0,64,1,
    55625,0,72,1,
    55625,1,58,1,
    55625,1,60,1,
    55833,0,65,1,
    55833,0,72,1,
    55833,1,57,1,
    55833,1,60,1,
    56042,0,64,0,
    56042,0,67,0,
    56042,0,72,0,
    56042,1,55,1,
    56042,1,58,1,
    56042,1,60,1,
    56208,0,65,0,
    56208,0,69,0,
    56250,0,72,3,
    56250,1,53,1,
    56458,1,57,1,
    56667,1,60,1,
    56875,1,57,1,
    57083,0,77,2,
    57083,1,60,1,
    57292,1,57,1,
    57396,0,76,0,
    57500,0,76,2,
    57500,1,53,1,
    57708,1,58,1,
    57917,0,74,2,
    57917,1,62,1,
    58125,1,58,1,
    58333,0,82,2,
    58333,1,62,1,
    58542,1,58,1,
    58646,0,81,0,
    58750,0,81,1,
    58750,1,53,1,
    58958,0,79,1,
    58958,1,64,1,
    59167,0,77,1,
    59167,1,53,1,
    59167,1,55,1,
    59167,1,58,1,
    59375,0,76,1,
    59375,1,64,1,
    59583,0,74,1,
    59583,1,53,1,
    59583,1,55,1,
    59583,1,58,1,
    59792,0,72,1,
    59792,1,64,1,
    60000,0,70,2,
    60000,1,53,1,
    60208,1,57,1,
    60417,0,69,1,
    60417,1,60,1,
    60625,1,57,1,
    60771,0,70,0,
    60833,0,69,0,
    60833,1,60,1,
    60937,0,67,0,
    61042,0,69,0,
    61042,1,57,1,
    61146,0,70,0,
    61250,0,72,3,
    61250,1,53,1,
    61458,1,57,1,
    61667,1,60,1,
    61875,1,57,1,
    62083,0,74,1,
    62083,1,60,1,
    62292,0,75,1,
    62292,1,57,1,
    62500,0,76,2,
    62500,1,52,1,
    62708,1,57,1,
    62917,1,60,1,
    63125,0,76,1,
    63125,1,57,1,
    63333,0,77,1,
    63333,1,50,1,
    63333,1,62,1,
    63542,0,69,1,
    63542,1,53,1,
    63750,0,72,2,
    63750,1,55,1,
    63958,1,64,1,
    64167,1,55,1,
    64375,1,64,1,
    64391,0,74,0,
    64437,0,72,0,
    64479,0,71,0,
    64521,0,72,0,
    64583,0,74,2,
    64583,1,55,1,
    64792,1,65,1,
    64896,0,71,0,
    65000,0,72,0,
    65000,1,60,2,
    65000,1,64,2,
    65104,0,79,0,
    65208,0,67,0,
    65312,0,79,0,
    65417,0,69,0,
    65521,0,79,0,
    65625,0,71,0,
    65625,1,65,0,
    65625,1,67,0,
    65729,0,79,0,
    65833,0,72,0,
    65833,1,64,0,
    65833,1,67,0,
    65937,0,79,0,
    66042,0,74,0,
    66042,1,62,0,
    66042,1,65,0,
    66042,1,67,0,
    66146,0,79,0,
    66250,0,76,0,
    66250,1,60,2,
    66250,1,64,2,
    66250,1,67,2,
    66354,0,79,0,
    66458,0,84,0,
    66562,0,83,0,
    66667,0,81,0,
    66667,1,53,2,
    66667,1,57,2,
    66771,0,79,0,
    66875,0,77,0,
    66979,0,76,0,
    67083,0,74,0,
    67083,1,55,2,
    67083,1,59,2,
    67187,0,79,0,
    67292,0,77,0,
    67396,0,74,0,
    67500,0,72,0,
    67500,1,60,2,
    67604,0,79,0,
    67708,0,67,0,
    67812,0,79,0,
    67917,0,69,0,
    68021,0,79,0,
    68125,0,71,0,
    68125,1,65,0,
    68125,1,67,0,
    68229,0,79,0,
    68333,0,72,0,
    68333,1,64,0,
    68333,1,67,0,
    68437,0,79,0,
    68542,0,74,0,
    68542,1,62,0,
    68542,1,65,0,
    68542,1,67,0,
    68646,0,79,0,
    68750,0,76,0,
    68750,1,60,2,
    68750,1,64,2,
    68750,1,67,2,
    68854,0,79,0,
    68958,0,84,0,
    69062,0,83,0,
    69167,0,81,0,
    69167,1,53,2,
    69167,1,57,2,
    69271,0,79,0,
    69375,0,77,0,
    69479,0,76,0,
    69583,0,74,0,
    69583,1,55,2,
    69583,1,59,2,
    69687,0,79,0,
    69792,0,77,0,
    69896,0,74,0,
    70000,0,76,0,
    70000,1,56,2,
    70000,1,59,2,
    70104,0,77,0,
    70208,0,76,0,
    70312,0,75,0,
    70417,0,76,0,
    70521,0,71,0,
    70625,0,76,0,
    70729,0,75,0,
    70833,0,76,0,
    70937,0,71,0,
    71042,0,76,0,
    71146,0,75,0,
    71250,0,76,3,
    71875,0,71,1,
    72083,0,76,1,
    72292,0,75,1,
    72500,0,76,2,
    73125,0,71,1,
    73333,0,76,1,
    73542,1,75,1,
    73750,1,76,1,
    73958,0,75,1,
    74167,0,76,1,
    74375,1,75,1,
    74583,1,76,1,
    74792,0,75,1,
    75000,0,76,1,
    75208,0,75,1,
    75417,0,76,1,
    75625,0,71,1,
    75833,0,74,1,
    76042,0,72,1,
    76250,0,69,2,
    76250,1,45,1,
    76458,1,52,1,
    76667,1,57,1,
    76875,0,60,1,
    77083,0,64,1,
    77292,0,69,1,
    77500,0,71,2,
    77500,1,40,1,
    77708,1,52,1,
    77917,1,56,1,
    78125,0,64,1,
    78333,0,68,1,
    78542,0,71,1,
    78750,0,72,2,
    78750,1,45,1,
    78958,1,52,1,
    79167,1,57,1,
    79375,0,64,1,
    79583,0,76,1,
    79792,0,75,1,
    80000,0,76,1,
    80208,0,75,1,
    80417,0,76,1,
    80625,0,71,1,
    80833,0,74,1,
    81042,0,72,1,
    81250,0,69,2,
    81250,1,45,1,
    81458,1,52,1,
    81667,1,57,1,
    81875,0,60,1,
    82083,0,64,1,
    82292,0,69,1,
    82500,0,71,2,
    82500,1,40,1,
    82708,1,52,1,
    82917,1,56,1,
    83125,0,64,1,
    83333,0,72,1,
    83542,0,71,1,
    83750,0,69,2,
    83750,1,45,1,
    83958,1,52,1,
    84167,1,57,1,
    84375,0,71,1,
    84583,0,72,1,
    84792,0,74,1,
    85000,0,76,2,
    85000,1,48,1,
    85208,1,55,1,
    85417,1,60,1,
    85625,0,67,1,
    85833,0,77,1,
    86042,0,76,1,
    86250,0,74,2,
    86250,1,43,1,
    86458,1,55,1,
    86667,1,59,1,
    86875,0,65,1,
    87083,0,76,1,
    87292,0,74,1,
    87500,0,72,2,
    87500,1,45,1,
    87708,1,52,1,
    87917,1,57,1,
    88125,0,64,1,
    88333,0,74,1,
    88542,0,72,1,
    88750,0,71,2,
    88750,1,40,1,
    88958,1,52,1,
    89167,1,64,1,
    89375,0,64,1,
    89583,0,76,1,
    89792,1,64,1,
    90000,1,76,1,
    90208,0,76,1,
    90417,0,88,1,
    90625,1,75,1,
    90833,1,76,1,
    91042,0,75,1,
    91250,0,76,1,
    91458,1,75,1,
    91667,1,76,1,
    91875,0,75,1,
    92083,0,76,1,
    92292,0,75,1,
    92500,0,76,1,
    92708,0,75,1,
    92917,0,76,1,
    93125,0,71,1,
    93333,0,74,1,
    93542,0,72,1,
    93750,0,69,2,
    93750,1,45,1,
    93958,1,52,1,
    94167,1,57,1,
    94375,0,60,1,
    94583,0,64,1,
    94792,0,69,1,
    95000,0,71,2,
    95000,1,40,1,
    95208,1,52,1,
    95417,1,56,1,
    95625,0,64,1,
    95833,0,68,1,
    96042,0,71,1,
    96250,0,72,2,
    96250,1,45,1,
    96458,1,52,1,
    96667,1,57,1,
    96875,0,64,1,
    97083,0,76,1,
    97292,0,75,1,
    97500,0,76,1,
    97708,0,75,1,
    97917,0,76,1,
    98125,0,71,1,
    98333,0,74,1,
    98542,0,72,1,
    98750,0,69,2,
    98750,1,45,1,
    98958,1,52,1,
    99167,1,57,1,
    99375,0,60,1,
    99583,0,64,1,
    99792,0,69,1,
    100000,0,71,2,
    100000,1,40,1,
    100208,1,52,1,
    100417,1,56,1,
    100625,0,64,1,
    100833,0,72,1,
    101042,0,71,1,
    101250,0,69,2,
    101250,1,45,1,
    101458,1,45,1,
    101667,1,45,1,
    101875,1,45,1,
    102083,1,45,1,
    102292,1,45,1,
    102500,0,64,3,
    102500,0,67,3,
    102500,0,70,3,
    102500,0,73,3,
    102500,1,45,1,
    102708,1,45,1,
    102917,1,45,1,
    103125,1,45,1,
    103333,1,45,1,
    103542,1,45,1,
    103750,0,65,3,
    103750,0,69,3,
    103750,0,74,3,
    103750,1,45,1,
    103958,1,45,1,
    104167,1,45,1,
    104375,1,45,1,
    104583,0,73,1,
    104583,0,76,1,
    104583,1,45,1,
    104792,0,74,1,
    104792,0,77,1,
    104792,1,45,1,
    105000,0,68,3,
    105000,0,74,3,
    105000,0,77,3,
    105000,1,45,1,
    105208,1,45,1,
    105417,1,45,1,
    105625,1,45,1,
    105833,0,68,2,
    105833,0,74,2,
    105833,0,77,2,
    105833,1,45,1,
    106042,1,45,1,
    106250,0,69,3,
    106250,0,72,3,
    106250,0,76,3,
    106250,1,45,1,
    106458,1,45,1,
    106667,1,45,1,
    106875,1,45,1,
    107083,1,45,1,
    107292,1,45,1,
    107500,0,65,3,
    107500,0,74,3,
    107500,1,38,1,
    107500,1,45,1,
    107708,1,38,1,
    107708,1,45,1,
    107917,1,38,1,
    107917,1,45,1,
    108125,1,38,1,
    108125,1,45,1,
    108333,0,64,1,
    108333,0,72,1,
    108333,1,38,1,
    108333,1,45,1,
    108542,0,62,1,
    108542,0,71,1,
    108542,1,38,1,
    108542,1,45,1,
    108750,0,60,3,
    108750,0,66,3,
    108750,0,69,3,
    108750,1,39,1,
    108750,1,45,1,
    108958,1,39,1,
    108958,1,45,1,
    109167,1,39,1,
    109167,1,45,1,
    109375,1,39,1,
    109375,1,45,1,
    109583,0,60,2,
    109583,0,69,2,
    109583,1,39,1,
    109583,1,45,1,
    109792,1,39,1,
    109792,1,45,1,
    110000,0,60,2,
    110000,0,69,2,
    110000,1,40,1,
    110000,1,45,1,
    110208,1,40,1,
    110208,1,45,1,
    110417,0,64,2,
    110417,0,72,2,
    110417,1,40,1,
    110417,1,45,1,
    110625,1,40,1,
    110625,1,45,1,
    110833,0,62,2,
    110833,0,71,2,
    110833,1,40,1,
    110833,1,44,1,
    111042,1,40,1,
    111042,1,44,1,
    111250,0,60,3,
    111250,0,69,3,
    111250,1,33,1,
    111250,1,45,1,
    111458,1,45,1,
    111667,1,45,1,
    111875,1,45,1,
    112083,1,45,1,
    112292,1,45,1,
    112500,0,64,3,
    112500,0,67,3,
    112500,0,70,3,
    112500,0,73,3,
    112500,1,45,1,
    112708,1,45,1,
    112917,1,45,1,
    113125,1,45,1,
    113333,1,45,1,
    113542,1,45,1,
    113750,0,65,3,
    113750,0,69,3,
    113750,0,74,3,
    113750,1,45,1,
    113958,1,45,1,
    114167,1,45,1,
    114375,1,45,1,
    114583,0,73,1,
    114583,0,76,1,
    114583,1,45,1,
    114792,0,74,1,
    114792,0,77,1,
    114792,1,45,1,
    115000,0,74,3,
    115000,0,77,3,
    115000,1,45,1,
    115208,1,45,1,
    115417,1,45,1,
    115625,1,45,1,
    115833,0,74,2,
    115833,0,77,2,
    115833,1,45,1,
    116042,1,45,1,
    116250,0,74,3,
    116250,0,77,3,
    116250,1,46,1,
    116458,1,46,1,
    116667,1,46,1,
    116875,1,46,1,
    117083,1,46,1,
    117292,1,46,1,
    117500,0,67,3,
    117500,0,75,3,
    117500,1,46,1,
    117708,1,46,1,
    117917,1,46,1,
    118125,1,46,1,
    118333,0,65,1,
    118333,0,74,1,
    118333,1,46,1,
    118542,0,63,1,
    118542,0,72,1,
    118542,1,46,1,
    118750,0,62,3,
    118750,0,65,3,
    118750,0,70,3,
    118750,1,46,1,
    118958,1,46,1,
    119167,1,46,1,
    119375,1,46,1,
    119583,0,62,2,
    119583,0,65,2,
    119583,0,69,2,
    119583,1,46,1,
    119792,1,46,1,
    120000,0,62,3,
    120000,0,65,3,
    120000,0,68,3,
    120000,1,47,1,
    120208,1,47,1,
    120417,1,47,1,
    120625,1,47,1,
    120833,0,62,2,
    120833,0,65,2,
    120833,0,68,2,
    120833,1,47,1,
    121042,1,47,1,
    121250,0,60,3,
    121250,0,64,3,
    121250,0,69,3,
    121250,1,48,3,
    122500,0,64,2,
    122500,0,71,2,
    122500,1,52,2,
    122500,1,56,2,
    123750,0,57,0,
    123750,1,33,2,
    123887,0,60,0,
    124027,0,64,0,
    124167,0,69,0,
    124303,0,72,0,
    124443,0,76,0,
    124583,0,74,0,
    124583,1,57,2,
    124583,1,60,2,
    124583,1,64,2,
    124720,0,72,0,
    124860,0,71,0,
    125000,0,69,0,
    125000,1,57,2,
    125000,1,60,2,
    125000,1,64,2,
    125137,0,72,0,
    125277,0,76,0,
    125417,0,81,0,
    125553,0,84,0,
    125693,0,88,0,
    125833,0,86,0,
    125833,1,57,2,
    125833,1,60,2,
    125833,1,64,2,
    125970,0,84,0,
    126110,0,83,0,
    126250,0,81,0,
    126250,1,57,2,
    126250,1,60,2,
    126250,1,64,2,
    126387,0,84,0,
    126527,0,88,0,
    126667,0,93,0,
    126803,0,96,0,
    126943,0,100,0,
    127083,0,98,0,
    127083,1,57,2,
    127083,1,60,2,
    127083,1,64,2,
    127220,0,96,0,
    127360,0,95,0,
    127500,0,94,0,
    127500,1,57,2,
    127500,1,60,2,
    127500,1,64,2,
    127637,0,93,0,
    127777,0,92,0,
    127917,0,91,0,
    128053,0,90,0,
    128193,0,89,0,
    128333,0,88,0,
    128470,0,87,0,
    128610,0,86,0,
    128750,0,85,0,
    128887,0,84,0,
    129027,0,83,0,
    129167,0,82,0,
    129303,0,81,0,
    129443,0,80,0,
    129583,0,79,0,
    129720,0,78,0,
    129860,0,77,0,
    130000,0,76,1,
    130208,0,75,1,
    130417,0,76,1,
    130625,0,71,1,
    130833,0,74,1,
    131042,0,72,1,
    131250,0,69,2,
    131250,1,45,1,
    131458,1,52,1,
    131667,1,57,1,
    131875,0,60,1,
    132083,0,64,1,
    132292,0,69,1,
    132500,0,71,2,
    132500,1,40,1,
    132708,1,52,1,
    132917,1,56,1,
    133125,0,64,1,
    133333,0,68,1,
    133542,0,71,1,
    133750,0,72,2,
    133750,1,45,1,
    133958,1,52,1,
    134167,1,57,1,
    134375,0,64,1,
    134583,0,76,1,
    134792,0,75,1,
    135000,0,76,1,
    135208,0,75,1,
    135417,0,76,1,
    135625,0,71,1,
    135833,0,74,1,
    136042,0,72,1,
    136250,0,69,2,
    136250,1,45,1,
    136458,1,52,1,
    136667,1,57,1,
    136875,0,60,1,
    137083,0,64,1,
    137292,0,69,1,
    137500,0,71,2,
    137500,1,40,1,
    137708,1,52,1,
    137917,1,56,1,
    138125,0,64,1,
    138333,0,72,1,
    138542,0,71,1,
    138750,0,69,2,
    138750,1,45,1,
    138958,1,52,1,
    139167,1,57,1,
    139375,0,71,1,
    139583,0,72,1,
    139792,0,74,1,
    140000,0,76,2,
    140000,1,48,1,
    140208,1,55,1,
    140417,1,60,1,
    140625,0,67,1,
    140833,0,77,1,
    141042,0,76,1,
    141250,0,74,2,
    141250,1,43,1,
    141458,1,55,1,
    141667,1,59,1,
    141875,0,65,1,
    142083,0,76,1,
    142292,0,74,1,
    142500,0,72,2,
    142500,1,45,1,
    142708,1,52,1,
    142917,1,57,1,
    143125,0,64,1,
    143333,0,74,1,
    143542,0,72,1,
    143750,0,71,2,
    143750,1,40,1,
    143958,1,52,1,
    144167,1,64,1,
    144375,0,64,1,
    144583,0,76,1,
    144792,1,64,1,
    145000,1,76,1,
    145208,0,76,1,
    145417,0,88,1,
    145625,1,75,1,
    145833,1,76,1,
    146042,0,75,1,
    146250,0,76,1,
    146458,1,75,1,
    146667,1,76,1,
    146875,0,75,1,
    147083,0,76,1,
    147292,0,75,1,
    147500,0,76,1,
    147708,0,75,1,
    147917,0,76,1,
    148125,0,71,1,
    148333,0,74,1,
    148542,0,72,1,
    148750,0,69,2,
    148750,1,45,1,
    148958,1,52,1,
    149167,1,57,1,
    149375,0,60,1,
    149583,0,64,1,
    149792,0,69,1,
    150000,0,71,2,
    150000,1,40,1,
    150208,1,52,1,
    150417,1,56,1,
    150625,0,64,1,
    150833,0,68,1,
    151042,0,71,1,
    151250,0,72,2,
    151250,1,45,1,
    151458,1,52,1,
    151667,1,57,1,
    151875,0,64,1,
    152083,0,76,1,
    152292,0,75,1,
    152500,0,76,1,
    152708,0,75,1,
    152917,0,76,1,
    153125,0,71,1,
    153333,0,74,1,
    153542,0,72,1,
    153750,0,69,2,
    153750,1,45,1,
    153958,1,52,1,
    154167,1,57,1,
    154375,0,60,1,
    154583,0,64,1,
    154792,0,69,1,
    155000,0,71,2,
    155000,1,40,1,
    155250,1,52,1,
    155500,1,56,1,
    155750,0,64,1,
    156000,0,72,2,
    156333,0,71,2,
    156667,0,69,2,
    156667,1,33,3,
    156667,1,45,3
};

const std::vector<std::string> NOTES { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

// Create a fixed string, by clipping or padding the input string
// Note string is NOT passed as ref to permit use of r-values
std::string fixed_width( std::string in, size_t len, char pad_char = ' ' )
{
    // Check if clipping required
    if( in.size() > len )
    {
        return in.substr( 0, len );
    }
    
    // Otherwise, pad
    return in + std::string( len - in.size(), pad_char ); 
}

// Integer wrapper for above function
std::string fixed_width( int in, size_t len, char pad_char = ' ' )
{
    return fixed_width( std::to_string( in ), len, pad_char );
}

/////////////////////////////////////////////////////////
// TOP LEVEL ENTRY POINT OF THE TEST APPLICATION
int main( int argc, char** argv )
{   
    // Define window size (these can be changed to any sensible value)
    const int WINDOW_WIDTH_IN_PIXELS = 640;
    const int WINDOW_HEIGHT_IN_PIXELS = 360;
  

    //////////////////////////////////////////////
    // INITIALIZE SDL, CREATE WINDOW & RENDERER
    MCK::GameEng &game_eng = MCK::GameEng::get_singleton();
    try
    {
        game_eng.init(
            "GameEngAudioTest",  // Window name
            0,  // Window x pos
            0,  // Window y pos
            WINDOW_WIDTH_IN_PIXELS,
            WINDOW_HEIGHT_IN_PIXELS
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to initialize SDL, error: ")
            + e.what() ) );
    }

    //////////////////////////////////////////////
    // INITIALIZE IMAGE MANAGER
    MCK::ImageMan &image_man = MCK::ImageMan::get_singleton();
    try
    {
        image_man.init( game_eng );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to initialize Image Manager, error: ")
            + e.what() ) );
    }


    //////////////////////////////////////////////
    // DEFINE AUDIO VOICES
    // IMPORTANT: Once these voices are handed over
    // to GameEngAudio, they should NOT be accessed
    // again by the main thread (i.e. this program),
    // as they will be accessed regularly by the
    // audio callback thread and they are NOT thread safe.
    const int LOWEST_OCTAVE_LO_VOICES = 1;
    const int LOWEST_OCTAVE_HI_VOICES = 3;
    std::vector<std::shared_ptr<MCK::VoiceBase>> new_voices( MCK_NUM_VOICES, NULL ); 
    for( int i = 0; i < MCK_NUM_VOICES; i++ )
    {
        std::shared_ptr<MCK::VoiceSynth> synth
            = std::make_shared<MCK::VoiceSynth>();
        try
        {
            synth->init( 
                2205,  // sixteenth_duration_in_samples, 
                MCK::VoiceSynth::Waveform::SINE,
                i < MCK_NUM_VOICES / 2 ?
                    LOWEST_OCTAVE_LO_VOICES :
                    LOWEST_OCTAVE_HI_VOICES,
                MCK::Envelope(
                    550,  // Attack
                    550,  // Decay
                    8820,  // Release
                    192  // Sustain as proportion of peak (0-255)
                ),
                0xFF  // Initial volume
            );
        }
        catch( const std::exception &e )
        {
#if defined MCK_STD_OUT
            // Issue a warning, but no point throwing exception here
            std::cout << "Unable to init voice "
                      << std::to_string( i )
                      << ", error = " 
                      << e.what()
                      << std::endl;
#endif
            continue;
        }

        // Recast as base pointer, and store
        new_voices[i] = std::dynamic_pointer_cast<MCK::VoiceBase>( synth );
        
        std::cout << "Successfully created voice " << i
                  << std::endl;
    }


    //////////////////////////////////////////////
    // INITIALIZE SDL AUDIO
    try
    {
        MCK::GameEngAudio::init(
            0xFF,  // Master volume
            new_voices
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to initialize SDL Audio, error: ")
            + e.what() ) );
    }


    ///////////////////////////////////////////
    // CREATE LOCAL PALETTE(S)
    MCK_PAL_ID_TYPE title_palette_id;
    MCK_PAL_ID_TYPE console_1_palette_id;
    MCK_PAL_ID_TYPE console_2_palette_id;
    const uint8_t CONSOLE2_BG_COL = MCK::COL_FOREST_GREEN;
    try
    {
        title_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_BLACK,
                    MCK::COL_WHITE,
                }
            )
        );
        console_1_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    MCK::COL_BLACK,
                    MCK::COL_RED,
                }
            )
        );
        console_2_palette_id = image_man.create_local_palette(
            std::make_shared<std::vector<uint8_t>>(
                std::vector<uint8_t>{
                    CONSOLE2_BG_COL,
                    MCK::COL_GREEN
                }
            )
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create palettes, error: ")
            + e.what() ) );
    }

    ///////////////////////////////////////////
    // CREATE MAIN CONSOLE PARAMETERS
    const uint8_t BG_COL = MCK::COL_ROTTING_PURPLE;
   
    // General
    const uint8_t TILE_WIDTH = 16;
    const uint8_t TILE_HEIGHT = 16;
    
    // Console 1
    const uint8_t CON1_CHAR_WIDTH = 8;
    const uint8_t CON1_CHAR_HEIGHT = 16;
    const uint16_t CON1_CONSOLE_LEFT = TILE_WIDTH * 1;
    const uint16_t CON1_CONSOLE_TOP = TILE_HEIGHT * 3;
    const uint8_t CON1_CONSOLE_WIDTH_IN_CHARS = 37;
    const uint8_t CON1_CONSOLE_HEIGHT_IN_CHARS = 8;
    const uint8_t CON1_CONSOLE_CHAR_SPACING = 2;
    const uint8_t CON1_CONSOLE_LINE_SPACING = 4;

    // Console 2
    const uint8_t CON2_CHAR_WIDTH = 8;
    const uint8_t CON2_CHAR_HEIGHT = 8;
    const uint16_t CON2_CONSOLE_LEFT = TILE_WIDTH * 29;
    const uint16_t CON2_CONSOLE_TOP = TILE_HEIGHT * 3;
    const uint8_t CON2_CONSOLE_WIDTH_IN_CHARS = 12;
    const uint8_t CON2_CONSOLE_HEIGHT_IN_CHARS = 6;
    const uint8_t CON2_CONSOLE_CHAR_SPACING = 1;
    const uint8_t CON2_CONSOLE_LINE_SPACING = 0;

    ////////////////////////////////////////////
    // SET CLEARING COLO(U)R

    // Set render clearing colo(u)r for this demo
    try
    {
        game_eng.set_clearing_color( BG_COL );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to set clearing color, error: ")
            + e.what() ) );
    }

    //////////////////////////////////////////////
    // CREATE TITLE TEXT BOX
    std::shared_ptr<MCK::Console> title_text
        = std::make_shared<MCK::Console>();
    try
    {
        std::string CR( 1, uint8_t( 255 ) );
        std::string s = "AUDIO DEMO  " + CR + " MuckyTaters 2023";
        title_text->init(
            game_eng,
            image_man,
            game_eng.get_prime_render_block(),
            title_palette_id,
            CON1_CONSOLE_LEFT + TILE_WIDTH,  // x_pos,
            TILE_HEIGHT,  // y_pos,
            32,  // width in chars
            1,  // height in chars
            TILE_WIDTH,  // char_width_in_pixels,
            TILE_HEIGHT,  // char_height_in_pixels,
            s,
            0,  // print_speed_in_ticks_per_char,
            0,  // scroll_speed_in_ticks_per_pixel,
            true,  // hoz_text_alignment
            0, // start_line
            true,  // add_to_front_of_parent_block = true
            MCK::COL_BLACK,  // underlay colo(u)r
            2,  // char spacing in pixels
            0  // line spacing in pixels
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create title text, error: ")
            + e.what() ) );
    }

    //////////////////////////////////////////////
    // CREATE CONSOLE 1
    std::shared_ptr<MCK::Console> console_1
        = std::make_shared<MCK::Console>();
    try
    {
        console_1->init(
            game_eng,
            image_man,
            game_eng.get_prime_render_block(), // mini_console_block,
            console_1_palette_id,
            CON1_CONSOLE_LEFT,  // x_pos,
            CON1_CONSOLE_TOP,  // y_pos,
            CON1_CONSOLE_WIDTH_IN_CHARS,
            CON1_CONSOLE_HEIGHT_IN_CHARS,
            CON1_CHAR_WIDTH,  // char_width_in_pixels,
            CON1_CHAR_HEIGHT,  // char_height_in_pixels,
            "------ -- -",  // initial_content,
            0,  // print_speed_in_ticks_per_char,
            0,  // scroll_speed_in_ticks_per_pixel,
            true,  // hoz_text_alignment
            CON1_CONSOLE_HEIGHT_IN_CHARS - 1,  // start_line
            false, // true  // add_to_front_of_parent_block = true
            MCK::COL_BLACK,  // underlay colo(u)r
            CON1_CONSOLE_CHAR_SPACING,
            CON1_CONSOLE_LINE_SPACING
        );
    }
    catch( std::exception &e )
    {
        throw( std::runtime_error(
            std::string( "Failed to create console 1, error: ")
            + e.what() ) );
    }






    /////////////////////////////////////////////
    // MAIN LOOP STARTS HERE
    /////////////////////////////////////////////
    const float TARGET_FPS = 100;
    const uint32_t TICKS_PER_FRAME
        = uint32_t( 1000.0f / TARGET_FPS + 0.5f );
    uint32_t frame_num = 0;
    uint32_t current_ticks = game_eng.get_ticks();
    const uint32_t START_TICKS = current_ticks;
    uint32_t next_frame_ticks = current_ticks + TICKS_PER_FRAME; 
    const uint32_t END_TICKS = current_ticks + 160000;
    const float SCROLL_RATE = 0.1f;  // Pixels per tick
    size_t song_data_index = 0;
    const size_t SONG_DATA_SIZE = SONG_DATA.size();
    uint8_t id_of_next_avail_lo_voice = 0;
    uint8_t id_of_next_avail_hi_voice = MCK_NUM_VOICES / 2;
    bool first_note = true;
    do
    {
        ////////////////////////////////////////
        // Time management

        // Give time back to operating system
        game_eng.delay( TICKS_PER_FRAME / 4 );

        // Get current time
        current_ticks = game_eng.get_ticks();

        // If next frame not due, ignore
        if( current_ticks < next_frame_ticks )
        {
            continue;
        }
        else
        {
            // Calculate time of *next* frame
            next_frame_ticks = current_ticks + TICKS_PER_FRAME;
            frame_num++;
        }

        ////////////////////////////////////////
        // Handle user input
        {
            // Get pending keyboard (or quit) events
            std::vector< MCK::KeyEvent > events;
            try
            {
                game_eng.get_pending_keyboard_actions( events );
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Failed to get use input, error: ")
                    + e.what() ) );
            }

            for( auto e : events )
            {
                // For now, only check for quit
                if( e.key_code == MCK::KeyEvent::KEY_QUIT
                    && e.status == MCK::KeyEvent::PRESSED
                )
                {
                    // Just exit: GameEng instance will be
                    // destroyed automatically, and SDL 
                    // shut down safely
                    exit( 0 );
                }

                // TODO: Other keyboard input
            }
        }

        // Play notes in song
        {
            const size_t SONG_TICKS = current_ticks - START_TICKS;
            while( song_data_index < SONG_DATA.size() - 4
                   &&  SONG_DATA.at( song_data_index ) < SONG_TICKS
            )
            {
                // Get voice (in this demo this is LO or HI)
                const bool LO
                    = SONG_DATA.at( ++song_data_index );

                // Get lowest octave in note range,
                // depending on voice
                const uint8_t LOWEST_OCTAVE
                    = LO ? 2 : 4;

                // Get raw note ID (independent of LO/HI voice pitch IDs)
                const uint8_t RAW_NOTE_ID
                    = SONG_DATA.at( ++song_data_index );

                // Get pitch ID, specific to LO or HI voice
                const uint8_t PITCH_ID
                    = RAW_NOTE_ID - 12 * LOWEST_OCTAVE;

                // Get duration ID, same for both voices
                const uint8_t DURATION_ID
                    = SONG_DATA.at( ++song_data_index );
                song_data_index++; 

                // Issue command to next available voice
                uint8_t voice_id;
                if( LO )
                {
                    // LO voices
                    voice_id = id_of_next_avail_lo_voice++;
                    if( id_of_next_avail_lo_voice == MCK_NUM_VOICES / 2 )
                    {
                        id_of_next_avail_lo_voice = 0;
                    }
                }
                else
                {
                    // HI voices
                    voice_id = id_of_next_avail_hi_voice++;
                    if( id_of_next_avail_hi_voice == MCK_NUM_VOICES )
                    {
                        id_of_next_avail_hi_voice = MCK_NUM_VOICES / 2;
                    }
                }

                const uint8_t COMMAND
                    = MCK::VoiceSynth::construct_command(
                        PITCH_ID,
                        DURATION_ID
                    );

                try
                {
                    MCK::GameEngAudio::voice_command(
                        voice_id,
                        COMMAND
                    );
                }
                catch( std::exception &e )
                {
                    // Log error but carry on
                    std::cout << "Failed to issue command to "
                              << "voice with id " << voice_id
                              << ", error = " << e.what()
                              << std::endl;
                }

                // Add note info to console 1 (best effort)
                {
                    // Add ticks
                    std::string msg = " ";

                    if( !first_note )
                    {
                        msg += " ";
                    }

                    msg += fixed_width( SONG_TICKS, 7 );

                    // Add LO/HI and voice ID
                    msg += LO ? "LO " : "HI ";

                    msg += std::to_string( LO ? voice_id : voice_id - 4 );
                    msg += " ";

                    // Add note, positioned by octave of raw note id
                    const int OCTAVE_NUM = RAW_NOTE_ID / 12;
                    const int WITHIN_OCTAVE_NOTE_ID
                        = RAW_NOTE_ID % 12;
                    for( int i = 2; i < OCTAVE_NUM; i++ )
                    {
                        msg += ".. ";
                    }
                    msg += fixed_width(
                        NOTES[ WITHIN_OCTAVE_NOTE_ID ],
                        3
                    );
                    for( int i = 0; i < 8 - OCTAVE_NUM; i++ )
                    {
                        msg += ".. ";
                    }

                    // Add duration ID
                    msg += fixed_width( pow( 2, DURATION_ID ), 1 );

                    // Add padding to make full length line
                    const int PAD_SIZE
                        = CON1_CONSOLE_WIDTH_IN_CHARS
                                - msg.size() - 1;
                    if( PAD_SIZE > 0 )
                    {
                        msg += std::string( PAD_SIZE, ' ' );
                    }

                    if( !first_note )
                    {
                        msg += " ";
                    }
                    else
                    {
                        first_note = false;
                    }

                    // Add all this to console 1
                    console_1->add_content( msg );
                        first_note = false;
                    
                }

                std::cout << "current_ticks = "
                          << current_ticks
                          << ", voice_id = "
                          << int( voice_id )
                          << ", PITCH_ID = "
                          << int( PITCH_ID )
                          << ", DURATION_ID = "
                          << int( DURATION_ID )
                          << std::endl;
            }
        }

        // Update console(s) 
        try
        {
            console_1->update( current_ticks );
            // console_2->update( current_ticks );
        }
        catch( std::exception &e )
        {
            throw( std::runtime_error(
                std::string( "Console update(s) failed, error: ")
                + e.what() ) );
        }

        // Clear, render and present
        {
            try
            {
                game_eng.clear();
                game_eng.render_all(
                    game_eng.get_prime_render_block(),
                    0,  // Hoz offset
                    0  // Vert offset
                );
                game_eng.show();
            }
            catch( std::exception &e )
            {
                throw( std::runtime_error(
                    std::string( "Render sequence failed, error: ")
                    + e.what() ) );
            }
        }
    }
    while( current_ticks < END_TICKS );

    // Note: SDL is closed down when 'game_eng'
    // goes out of scope
}
