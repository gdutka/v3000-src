/** @file
  Header file for H2O Console Redirection Configuration

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _CR_CONFIG_H_
#define _CR_CONFIG_H_

#include <Guid/CrConfigHii.h>
#include <Guid/SolSetupConfigHii.h>

#define CR_CONFIGURATION_VARSTORE_ID        0x7777

#define CR_DEVICE_LABEL_START               0x1000
#define CR_DEVICE_LABEL_END                 0x1001

#define CONSOLE_REDIRECTION_ROOT_FORM_ID    0x2001
#define CR_DEVICE_FORM_ID                   0x2002

#define CR_DEVICE_FORM_0_ID                 0x2002
#define CR_DEVICE_FORM_1_ID                 0x2003
#define CR_DEVICE_FORM_2_ID                 0x2004
#define CR_DEVICE_FORM_3_ID                 0x2005
#define CR_DEVICE_FORM_4_ID                 0x2006
#define CR_DEVICE_FORM_5_ID                 0x2007
#define CR_DEVICE_FORM_6_ID                 0x2008
#define CR_DEVICE_FORM_7_ID                 0x2009
#define CR_DEVICE_FORM_8_ID                 0x200A
#define CR_DEVICE_FORM_9_ID                 0x200B
#define CR_DEVICE_FORM_10_ID                0x200C
#define CR_DEVICE_FORM_11_ID                0x200D
#define CR_DEVICE_FORM_12_ID                0x200E
#define CR_DEVICE_FORM_13_ID                0x200F
#define CR_DEVICE_FORM_14_ID                0x2010
#define CR_DEVICE_FORM_15_ID                0x2011
#define CR_DEVICE_FORM_16_ID                0x2012
#define CR_DEVICE_FORM_17_ID                0x2013
#define CR_DEVICE_FORM_18_ID                0x2014
#define CR_DEVICE_FORM_19_ID                0x2015
#define CR_DEVICE_FORM_20_ID                0x2016
#define CR_DEVICE_FORM_21_ID                0x2017
#define CR_DEVICE_FORM_22_ID                0x2018
#define CR_DEVICE_FORM_23_ID                0x2019
#define CR_DEVICE_FORM_24_ID                0x201A
#define CR_DEVICE_FORM_25_ID                0x201B
#define CR_DEVICE_FORM_26_ID                0x201C
#define CR_DEVICE_FORM_27_ID                0x201D
#define CR_DEVICE_FORM_28_ID                0x201E
#define CR_DEVICE_FORM_29_ID                0x201F
#define CR_DEVICE_FORM_30_ID                0x2020
#define CR_DEVICE_FORM_31_ID                0x2021

#define CR_DEVICE_QUESTION_ID_BASE          0xC000

#define KEY_GLOBAL_TERMINAL_TYPE            0x0D00
#define KEY_GLOBAL_BAUD_RATE                0x0D01
#define KEY_GLOBAL_DATA_BITS                0x0D02
#define KEY_GLOBAL_PARITY                   0x0D03
#define KEY_GLOBAL_STOP_BITS                0x0D04
#define KEY_GLOBAL_FLOW_CONTROL             0x0D05

//
// Preserve question id from 0x0E00 to 0x0EFF for CR devices use
//
#define CR_DEVICE_QUESTION_ID_MASK          0x0E00

#define KEY_PORT_ENABLE0                    0x0E00
#define KEY_PORT_ENABLE1                    0x0E01
#define KEY_PORT_ENABLE2                    0x0E02
#define KEY_PORT_ENABLE3                    0x0E03
#define KEY_PORT_ENABLE4                    0x0E04
#define KEY_PORT_ENABLE5                    0x0E05
#define KEY_PORT_ENABLE6                    0x0E06
#define KEY_PORT_ENABLE7                    0x0E07
#define KEY_PORT_ENABLE8                    0x0E08
#define KEY_PORT_ENABLE9                    0x0E09
#define KEY_PORT_ENABLE10                   0x0E0A
#define KEY_PORT_ENABLE11                   0x0E0B
#define KEY_PORT_ENABLE12                   0x0E0C
#define KEY_PORT_ENABLE13                   0x0E0D
#define KEY_PORT_ENABLE14                   0x0E0E
#define KEY_PORT_ENABLE15                   0x0E0F
#define KEY_PORT_ENABLE16                   0x0E10
#define KEY_PORT_ENABLE17                   0x0E11
#define KEY_PORT_ENABLE18                   0x0E12
#define KEY_PORT_ENABLE19                   0x0E13
#define KEY_PORT_ENABLE20                   0x0E14
#define KEY_PORT_ENABLE21                   0x0E15
#define KEY_PORT_ENABLE22                   0x0E16
#define KEY_PORT_ENABLE23                   0x0E17
#define KEY_PORT_ENABLE24                   0x0E18
#define KEY_PORT_ENABLE25                   0x0E19
#define KEY_PORT_ENABLE26                   0x0E1A
#define KEY_PORT_ENABLE27                   0x0E1B
#define KEY_PORT_ENABLE28                   0x0E1C
#define KEY_PORT_ENABLE29                   0x0E1D
#define KEY_PORT_ENABLE30                   0x0E1E
#define KEY_PORT_ENABLE31                   0x0E1F

#define KEY_USE_GLOBAL_SETTING0             0x0E20
#define KEY_USE_GLOBAL_SETTING1             0x0E21
#define KEY_USE_GLOBAL_SETTING2             0x0E22
#define KEY_USE_GLOBAL_SETTING3             0x0E23
#define KEY_USE_GLOBAL_SETTING4             0x0E24
#define KEY_USE_GLOBAL_SETTING5             0x0E25
#define KEY_USE_GLOBAL_SETTING6             0x0E26
#define KEY_USE_GLOBAL_SETTING7             0x0E27
#define KEY_USE_GLOBAL_SETTING8             0x0E28
#define KEY_USE_GLOBAL_SETTING9             0x0E29
#define KEY_USE_GLOBAL_SETTING10            0x0E2A
#define KEY_USE_GLOBAL_SETTING11            0x0E2B
#define KEY_USE_GLOBAL_SETTING12            0x0E2C
#define KEY_USE_GLOBAL_SETTING13            0x0E2D
#define KEY_USE_GLOBAL_SETTING14            0x0E2E
#define KEY_USE_GLOBAL_SETTING15            0x0E2F
#define KEY_USE_GLOBAL_SETTING16            0x0E30
#define KEY_USE_GLOBAL_SETTING17            0x0E31
#define KEY_USE_GLOBAL_SETTING18            0x0E32
#define KEY_USE_GLOBAL_SETTING19            0x0E33
#define KEY_USE_GLOBAL_SETTING20            0x0E34
#define KEY_USE_GLOBAL_SETTING21            0x0E35
#define KEY_USE_GLOBAL_SETTING22            0x0E36
#define KEY_USE_GLOBAL_SETTING23            0x0E37
#define KEY_USE_GLOBAL_SETTING24            0x0E38
#define KEY_USE_GLOBAL_SETTING25            0x0E39
#define KEY_USE_GLOBAL_SETTING26            0x0E3A
#define KEY_USE_GLOBAL_SETTING27            0x0E3B
#define KEY_USE_GLOBAL_SETTING28            0x0E3C
#define KEY_USE_GLOBAL_SETTING29            0x0E3D
#define KEY_USE_GLOBAL_SETTING30            0x0E3E
#define KEY_USE_GLOBAL_SETTING31            0x0E3F

#define KEY_TERMINAL_TYPE0                  0x0E40
#define KEY_TERMINAL_TYPE1                  0x0E41
#define KEY_TERMINAL_TYPE2                  0x0E42
#define KEY_TERMINAL_TYPE3                  0x0E43
#define KEY_TERMINAL_TYPE4                  0x0E44
#define KEY_TERMINAL_TYPE5                  0x0E45
#define KEY_TERMINAL_TYPE6                  0x0E46
#define KEY_TERMINAL_TYPE7                  0x0E47
#define KEY_TERMINAL_TYPE8                  0x0E48
#define KEY_TERMINAL_TYPE9                  0x0E49
#define KEY_TERMINAL_TYPE10                 0x0E4A
#define KEY_TERMINAL_TYPE11                 0x0E4B
#define KEY_TERMINAL_TYPE12                 0x0E4C
#define KEY_TERMINAL_TYPE13                 0x0E4D
#define KEY_TERMINAL_TYPE14                 0x0E4E
#define KEY_TERMINAL_TYPE15                 0x0E4F
#define KEY_TERMINAL_TYPE16                 0x0E50
#define KEY_TERMINAL_TYPE17                 0x0E51
#define KEY_TERMINAL_TYPE18                 0x0E52
#define KEY_TERMINAL_TYPE19                 0x0E53
#define KEY_TERMINAL_TYPE20                 0x0E54
#define KEY_TERMINAL_TYPE21                 0x0E55
#define KEY_TERMINAL_TYPE22                 0x0E56
#define KEY_TERMINAL_TYPE23                 0x0E57
#define KEY_TERMINAL_TYPE24                 0x0E58
#define KEY_TERMINAL_TYPE25                 0x0E59
#define KEY_TERMINAL_TYPE26                 0x0E5A
#define KEY_TERMINAL_TYPE27                 0x0E5B
#define KEY_TERMINAL_TYPE28                 0x0E5C
#define KEY_TERMINAL_TYPE29                 0x0E5D
#define KEY_TERMINAL_TYPE30                 0x0E5E
#define KEY_TERMINAL_TYPE31                 0x0E5F

#define KEY_BAUD_RATE0                      0x0E60
#define KEY_BAUD_RATE1                      0x0E61
#define KEY_BAUD_RATE2                      0x0E62
#define KEY_BAUD_RATE3                      0x0E63
#define KEY_BAUD_RATE4                      0x0E64
#define KEY_BAUD_RATE5                      0x0E65
#define KEY_BAUD_RATE6                      0x0E66
#define KEY_BAUD_RATE7                      0x0E67
#define KEY_BAUD_RATE8                      0x0E68
#define KEY_BAUD_RATE9                      0x0E69
#define KEY_BAUD_RATE10                     0x0E6A
#define KEY_BAUD_RATE11                     0x0E6B
#define KEY_BAUD_RATE12                     0x0E6C
#define KEY_BAUD_RATE13                     0x0E6D
#define KEY_BAUD_RATE14                     0x0E6E
#define KEY_BAUD_RATE15                     0x0E6F
#define KEY_BAUD_RATE16                     0x0E70
#define KEY_BAUD_RATE17                     0x0E71
#define KEY_BAUD_RATE18                     0x0E72
#define KEY_BAUD_RATE19                     0x0E73
#define KEY_BAUD_RATE20                     0x0E74
#define KEY_BAUD_RATE21                     0x0E75
#define KEY_BAUD_RATE22                     0x0E76
#define KEY_BAUD_RATE23                     0x0E77
#define KEY_BAUD_RATE24                     0x0E78
#define KEY_BAUD_RATE25                     0x0E79
#define KEY_BAUD_RATE26                     0x0E7A
#define KEY_BAUD_RATE27                     0x0E7B
#define KEY_BAUD_RATE28                     0x0E7C
#define KEY_BAUD_RATE29                     0x0E7D
#define KEY_BAUD_RATE30                     0x0E7E
#define KEY_BAUD_RATE31                     0x0E7F

#define KEY_DATA_BITS0                      0x0E80
#define KEY_DATA_BITS1                      0x0E81
#define KEY_DATA_BITS2                      0x0E82
#define KEY_DATA_BITS3                      0x0E83
#define KEY_DATA_BITS4                      0x0E84
#define KEY_DATA_BITS5                      0x0E85
#define KEY_DATA_BITS6                      0x0E86
#define KEY_DATA_BITS7                      0x0E87
#define KEY_DATA_BITS8                      0x0E88
#define KEY_DATA_BITS9                      0x0E89
#define KEY_DATA_BITS10                     0x0E8A
#define KEY_DATA_BITS11                     0x0E8B
#define KEY_DATA_BITS12                     0x0E8C
#define KEY_DATA_BITS13                     0x0E8D
#define KEY_DATA_BITS14                     0x0E8E
#define KEY_DATA_BITS15                     0x0E8F
#define KEY_DATA_BITS16                     0x0E90
#define KEY_DATA_BITS17                     0x0E91
#define KEY_DATA_BITS18                     0x0E92
#define KEY_DATA_BITS19                     0x0E93
#define KEY_DATA_BITS20                     0x0E94
#define KEY_DATA_BITS21                     0x0E95
#define KEY_DATA_BITS22                     0x0E96
#define KEY_DATA_BITS23                     0x0E97
#define KEY_DATA_BITS24                     0x0E98
#define KEY_DATA_BITS25                     0x0E99
#define KEY_DATA_BITS26                     0x0E9A
#define KEY_DATA_BITS27                     0x0E9B
#define KEY_DATA_BITS28                     0x0E9C
#define KEY_DATA_BITS29                     0x0E9D
#define KEY_DATA_BITS30                     0x0E9E
#define KEY_DATA_BITS31                     0x0E9F

#define KEY_PARITY0                         0x0EA0
#define KEY_PARITY1                         0x0EA1
#define KEY_PARITY2                         0x0EA2
#define KEY_PARITY3                         0x0EA3
#define KEY_PARITY4                         0x0EA4
#define KEY_PARITY5                         0x0EA5
#define KEY_PARITY6                         0x0EA6
#define KEY_PARITY7                         0x0EA7
#define KEY_PARITY8                         0x0EA8
#define KEY_PARITY9                         0x0EA9
#define KEY_PARITY10                        0x0EAA
#define KEY_PARITY11                        0x0EAB
#define KEY_PARITY12                        0x0EAC
#define KEY_PARITY13                        0x0EAD
#define KEY_PARITY14                        0x0EAE
#define KEY_PARITY15                        0x0EAF
#define KEY_PARITY16                        0x0EB0
#define KEY_PARITY17                        0x0EB1
#define KEY_PARITY18                        0x0EB2
#define KEY_PARITY19                        0x0EB3
#define KEY_PARITY20                        0x0EB4
#define KEY_PARITY21                        0x0EB5
#define KEY_PARITY22                        0x0EB6
#define KEY_PARITY23                        0x0EB7
#define KEY_PARITY24                        0x0EB8
#define KEY_PARITY25                        0x0EB9
#define KEY_PARITY26                        0x0EBA
#define KEY_PARITY27                        0x0EBB
#define KEY_PARITY28                        0x0EBC
#define KEY_PARITY29                        0x0EBD
#define KEY_PARITY30                        0x0EBE
#define KEY_PARITY31                        0x0EBF

#define KEY_STOP_BITS0                      0x0EC0
#define KEY_STOP_BITS1                      0x0EC1
#define KEY_STOP_BITS2                      0x0EC2
#define KEY_STOP_BITS3                      0x0EC3
#define KEY_STOP_BITS4                      0x0EC4
#define KEY_STOP_BITS5                      0x0EC5
#define KEY_STOP_BITS6                      0x0EC6
#define KEY_STOP_BITS7                      0x0EC7
#define KEY_STOP_BITS8                      0x0EC8
#define KEY_STOP_BITS9                      0x0EC9
#define KEY_STOP_BITS10                     0x0ECA
#define KEY_STOP_BITS11                     0x0ECB
#define KEY_STOP_BITS12                     0x0ECC
#define KEY_STOP_BITS13                     0x0ECD
#define KEY_STOP_BITS14                     0x0ECE
#define KEY_STOP_BITS15                     0x0ECF
#define KEY_STOP_BITS16                     0x0ED0
#define KEY_STOP_BITS17                     0x0ED1
#define KEY_STOP_BITS18                     0x0ED2
#define KEY_STOP_BITS19                     0x0ED3
#define KEY_STOP_BITS20                     0x0ED4
#define KEY_STOP_BITS21                     0x0ED5
#define KEY_STOP_BITS22                     0x0ED6
#define KEY_STOP_BITS23                     0x0ED7
#define KEY_STOP_BITS24                     0x0ED8
#define KEY_STOP_BITS25                     0x0ED9
#define KEY_STOP_BITS26                     0x0EDA
#define KEY_STOP_BITS27                     0x0EDB
#define KEY_STOP_BITS28                     0x0EDC
#define KEY_STOP_BITS29                     0x0EDD
#define KEY_STOP_BITS30                     0x0EDE
#define KEY_STOP_BITS31                     0x0EDF

#define KEY_FLOW_CONTROL0                   0x0EE0
#define KEY_FLOW_CONTROL1                   0x0EE1
#define KEY_FLOW_CONTROL2                   0x0EE2
#define KEY_FLOW_CONTROL3                   0x0EE3
#define KEY_FLOW_CONTROL4                   0x0EE4
#define KEY_FLOW_CONTROL5                   0x0EE5
#define KEY_FLOW_CONTROL6                   0x0EE6
#define KEY_FLOW_CONTROL7                   0x0EE7
#define KEY_FLOW_CONTROL8                   0x0EE8
#define KEY_FLOW_CONTROL9                   0x0EE9
#define KEY_FLOW_CONTROL10                  0x0EEA
#define KEY_FLOW_CONTROL11                  0x0EEB
#define KEY_FLOW_CONTROL12                  0x0EEC
#define KEY_FLOW_CONTROL13                  0x0EED
#define KEY_FLOW_CONTROL14                  0x0EEE
#define KEY_FLOW_CONTROL15                  0x0EEF
#define KEY_FLOW_CONTROL16                  0x0EF0
#define KEY_FLOW_CONTROL17                  0x0EF1
#define KEY_FLOW_CONTROL18                  0x0EF2
#define KEY_FLOW_CONTROL19                  0x0EF3
#define KEY_FLOW_CONTROL20                  0x0EF4
#define KEY_FLOW_CONTROL21                  0x0EF5
#define KEY_FLOW_CONTROL22                  0x0EF6
#define KEY_FLOW_CONTROL23                  0x0EF7
#define KEY_FLOW_CONTROL24                  0x0EF8
#define KEY_FLOW_CONTROL25                  0x0EF9
#define KEY_FLOW_CONTROL26                  0x0EFA
#define KEY_FLOW_CONTROL27                  0x0EFB
#define KEY_FLOW_CONTROL28                  0x0EFC
#define KEY_FLOW_CONTROL29                  0x0EFD
#define KEY_FLOW_CONTROL30                  0x0EFE
#define KEY_FLOW_CONTROL31                  0x0EFF

#define KEY_SCAN_F9                         0xF0D2

//
// Console Redirection Text Mode Resolution (CRTextModeResolution)
//
#define CR_TEXT_MODE_AUTO                   0
#define CR_TEXT_MODE_80X25                  1
#define CR_TEXT_MODE_80X24DF                2
#define CR_TEXT_MODE_80X24DL                3
#define CR_TEXT_MODE_128X40                 4

#endif

