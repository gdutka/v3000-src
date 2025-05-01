/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH BIOS Ram usage
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 10:28:05 -0800 (Tue, 09 Dec 2014) $
 *
 */
#ifndef _FCH_BIOS_RAM_USAGE_H_
#define _FCH_BIOS_RAM_USAGE_H_

#define BIOS_RAM_MMIO_BASE              0xFED10000ul
#define BIOS_RAM_EN                     0x01

#define BIOS_RAM_INDEX                  0xCD4
#define BIOS_RAM_DATA                   0xCD5

/*
* [0x00 ~ 0x0F] Reserved for FCH internal usage
*/
#define BIOS_RAM_REG00                  0x00
#define BIOS_RAM_REG01                  0x01
#define BIOS_RAM_REG02                  0x02
#define BIOS_RAM_REG03                  0x03
#define BIOS_RAM_REG04                  0x04
#define BIOS_RAM_REG05                  0x05
#define BIOS_RAM_REG06                  0x06
#define BIOS_RAM_REG07                  0x07
#define BIOS_RAM_REG08                  0x08
#define BIOS_RAM_REG09                  0x09
#define BIOS_RAM_REG0A                  0x0A
#define BIOS_RAM_REG0B                  0x0B
#define BIOS_RAM_REG0C                  0x0C
#define BIOS_RAM_REG0D                  0x0D
#define BIOS_RAM_REG0E                  0x0E
#define BIOS_RAM_REG0F                  0x0F

/*
* [0x10 ~ 0x1F] Reserved for FCH internal usage
*/
#define BIOS_RAM_REG10                  0x10
#define BIOS_RAM_REG11                  0x11
#define BIOS_RAM_REG12                  0x12
#define BIOS_RAM_REG13                  0x13
#define BIOS_RAM_REG14                  0x14
#define BIOS_RAM_REG15                  0x15
#define BIOS_RAM_REG16                  0x16
#define BIOS_RAM_REG17                  0x17
#define BIOS_RAM_REG18                  0x18
#define BIOS_RAM_REG19                  0x19
#define BIOS_RAM_REG1A                  0x1A
#define BIOS_RAM_REG1B                  0x1B
#define BIOS_RAM_REG1C                  0x1C
#define BIOS_RAM_REG1D                  0x1D
#define BIOS_RAM_REG1E                  0x1E
#define BIOS_RAM_REG1F                  0x1F

/*
* [0x20 ~ 0x2F]    SMIx00080-SMIx0008C[Smi Status 0/1/2/~]
*/
#define BIOS_RAM_REG20                  0x20
#define BIOS_RAM_REG21                  0x21
#define BIOS_RAM_REG22                  0x22
#define BIOS_RAM_REG23                  0x23
#define BIOS_RAM_REG24                  0x24
#define BIOS_RAM_REG25                  0x25
#define BIOS_RAM_REG26                  0x26
#define BIOS_RAM_REG27                  0x27
#define BIOS_RAM_REG28                  0x28
#define BIOS_RAM_REG29                  0x29
#define BIOS_RAM_REG2A                  0x2A
#define BIOS_RAM_REG2B                  0x2B
#define BIOS_RAM_REG2C                  0x2C
#define BIOS_RAM_REG2D                  0x2D
#define BIOS_RAM_REG2E                  0x2E
#define BIOS_RAM_REG2F                  0x2F

/*
* [0x30 ~ 0x33]   SMIx00090[Smi Status 4]
*/
#define BIOS_RAM_REG30                  0x30
#define BIOS_RAM_REG31                  0x31
#define BIOS_RAM_REG32                  0x32
#define BIOS_RAM_REG33                  0x33

/*
* [0x34 ~ 0x3F]   Reserved for FCH internal usage
*/
#define BIOS_RAM_REG34                  0x34
#define BIOS_RAM_REG35                  0x35
#define BIOS_RAM_REG36                  0x36
#define BIOS_RAM_REG37                  0x37
#define BIOS_RAM_REG38                  0x38
#define BIOS_RAM_REG39                  0x39
#define BIOS_RAM_REG3A                  0x3A
#define BIOS_RAM_REG3B                  0x3B
#define BIOS_RAM_REG3C                  0x3C
#define BIOS_RAM_REG3D                  0x3D
#define BIOS_RAM_REG3E                  0x3E
#define BIOS_RAM_REG3F                  0x3F

/*
* [0x40]   SW SMI value
*/
#define BIOS_RAM_REG40                  0x40

/*
* [0x41 ~ 0x4F]   Reserved for FCH internal usage
*/
#define BIOS_RAM_REG41                  0x41
#define BIOS_RAM_REG42                  0x42
#define BIOS_RAM_REG43                  0x43
#define BIOS_RAM_REG44                  0x44
#define BIOS_RAM_REG45                  0x45
#define BIOS_RAM_REG46                  0x46
#define BIOS_RAM_REG47                  0x47
#define BIOS_RAM_REG48                  0x48
#define BIOS_RAM_REG49                  0x49
#define BIOS_RAM_REG4A                  0x4A
#define BIOS_RAM_REG4B                  0x4B
#define BIOS_RAM_REG4C                  0x4C
#define BIOS_RAM_REG4D                  0x4D
#define BIOS_RAM_REG4E                  0x4E
#define BIOS_RAM_REG4F                  0x4F

/*
* [0x50 ~ 0x5F]   CPM Save Context Area
*/
#define BIOS_RAM_REG50                  0x50
#define BIOS_RAM_REG51                  0x51
#define BIOS_RAM_REG52                  0x52
#define BIOS_RAM_REG53                  0x53
#define BIOS_RAM_REG54                  0x54
#define BIOS_RAM_REG55                  0x55
#define BIOS_RAM_REG56                  0x56
#define BIOS_RAM_REG57                  0x57
#define BIOS_RAM_REG58                  0x58
#define BIOS_RAM_REG59                  0x59
#define BIOS_RAM_REG5A                  0x5A
#define BIOS_RAM_REG5B                  0x5B
#define BIOS_RAM_REG5C                  0x5C
#define BIOS_RAM_REG5D                  0x5D
#define BIOS_RAM_REG5E                  0x5E
#define BIOS_RAM_REG5F                  0x5F

/*
* [0x60 ~ 0x6F]   System Guard Level 3
*/
#define BIOS_RAM_REG60                  0x60
#define BIOS_RAM_REG61                  0x61
#define BIOS_RAM_REG62                  0x62
#define BIOS_RAM_REG63                  0x63
#define BIOS_RAM_REG64                  0x64
#define BIOS_RAM_REG65                  0x65
#define BIOS_RAM_REG66                  0x66
#define BIOS_RAM_REG67                  0x67
#define BIOS_RAM_REG68                  0x68
#define BIOS_RAM_REG69                  0x69
#define BIOS_RAM_REG6A                  0x6A
#define BIOS_RAM_REG6B                  0x6B
#define BIOS_RAM_REG6C                  0x6C
#define BIOS_RAM_REG6D                  0x6D
#define BIOS_RAM_REG6E                  0x6E
#define BIOS_RAM_REG6F                  0x6F

/*
* [0x70 ~ 0x77]   Wireless Manageability scratch register
*/
#define BIOS_RAM_MANOS_BASE                         0x70
#define BIOS_RAM_MANOS_LENGTH                       0x8
#define BIOS_RAM_MANOS_BOOT_TARGET                  (BIOS_RAM_MANOS_BASE + 0x00)
#define BIOS_RAM_MANOS_DB_PCDOVERRIDE               (BIOS_RAM_MANOS_BASE + 0x01)
#define BIOS_RAM_MANOS_BOOT_RETRY_COUNT             (BIOS_RAM_MANOS_BASE + 0x02)
#define BIOS_RAM_MANOS_JUMPED_TO_MANOS_FLAG         (BIOS_RAM_MANOS_BASE + 0x03)
#define BIOS_RAM_MANOS_SIGNATURE                    (BIOS_RAM_MANOS_BASE + 0x06)
#define BIOS_RAM_MANOS_CHECKSUM                     (BIOS_RAM_MANOS_BASE + 0x07)

#define BIOS_RAM_REG70                  0x70
#define BIOS_RAM_REG71                  0x71
#define BIOS_RAM_REG72                  0x72
#define BIOS_RAM_REG73                  0x73
#define BIOS_RAM_REG74                  0x74
#define BIOS_RAM_REG75                  0x75
#define BIOS_RAM_REG76                  0x76
#define BIOS_RAM_REG77                  0x77

/*
* [0x78 ~ 0x7F]   A/B recovery reason code logged by PSP
*/
#define BIOS_RAM_AB_RECOVERY_REASON_BASE            0x78
#define BIOS_RAM_AB_RECOVERY_REASON_LENGTH          4

#define BIOS_RAM_REG78                  0x78
#define BIOS_RAM_REG79                  0x79
#define BIOS_RAM_REG7A                  0x7A
#define BIOS_RAM_REG7B                  0x7B
#define BIOS_RAM_REG7C                  0x7C
#define BIOS_RAM_REG7D                  0x7D
#define BIOS_RAM_REG7E                  0x7E
#define BIOS_RAM_REG7F                  0x7F

/*
* [0x80 ~ 0x83]   Error log
*/
#define BIOS_RAM_REG80                  0x80
#define BIOS_RAM_REG81                  0x81
#define BIOS_RAM_REG82                  0x82
#define BIOS_RAM_REG83                  0x83

/*
* [0x84 ~ 0x87]   Sycall Raw data pointer
*/
#define BIOS_RAM_SMM_ISOLATION_BASE                 0x84
#define BIOS_RAM_SMM_ISOLATION_LENGTH               0x8

#define BIOS_RAM_REG84                  0x84
#define BIOS_RAM_REG85                  0x85
#define BIOS_RAM_REG86                  0x86
#define BIOS_RAM_REG87                  0x87

/*
* [0x88 ~ 0x8F]   Unused
*/
#define BIOS_RAM_REG88                  0x88
#define BIOS_RAM_REG89                  0x89
#define BIOS_RAM_REG8A                  0x8A
#define BIOS_RAM_REG8B                  0x8B
#define BIOS_RAM_REG8C                  0x8C
#define BIOS_RAM_REG8D                  0x8D
#define BIOS_RAM_REG8E                  0x8E
#define BIOS_RAM_REG8F                  0x8F

/*
* [0x90 ~ 0x93]   S0i3 Unlock Flag
* [0x94 ~ 0x97]   Non Smm Capsule MSR Save Restore Buffer 
* [0x98 ~ 0x9F]   AGESA MemTestEx
*/
#define BIOS_RAM_REG90                  0x90
#define BIOS_RAM_REG91                  0x91
#define BIOS_RAM_REG92                  0x92
#define BIOS_RAM_REG93                  0x93
#define BIOS_RAM_REG94                  0x94
#define BIOS_RAM_REG95                  0x95
#define BIOS_RAM_REG96                  0x96
#define BIOS_RAM_REG97                  0x97
#define BIOS_RAM_REG98                  0x98
#define BIOS_RAM_REG99                  0x99
#define BIOS_RAM_REG9A                  0x9A
#define BIOS_RAM_REG9B                  0x9B
#define BIOS_RAM_REG9C                  0x9C
#define BIOS_RAM_REG9D                  0x9D
#define BIOS_RAM_REG9E                  0x9E
#define BIOS_RAM_REG9F                  0x9F

/*
* [0xA0 ~ 0xAB]   AGESA MemTestEx
* [0xA0 ~ 0xAF]   Unused
*/
#define BIOS_RAM_REGA0                  0xA0
#define BIOS_RAM_REGA1                  0xA1
#define BIOS_RAM_REGA2                  0xA2
#define BIOS_RAM_REGA3                  0xA3
#define BIOS_RAM_REGA4                  0xA4
#define BIOS_RAM_REGA5                  0xA5
#define BIOS_RAM_REGA6                  0xA6
#define BIOS_RAM_REGA7                  0xA7
#define BIOS_RAM_REGA8                  0xA8
#define BIOS_RAM_REGA9                  0xA9
#define BIOS_RAM_REGAA                  0xAA
#define BIOS_RAM_REGAB                  0xAB
#define BIOS_RAM_REGAC                  0xAC
#define BIOS_RAM_REGAD                  0xAD
#define BIOS_RAM_REGAE                  0xAE
#define BIOS_RAM_REGAF                  0xAF

/*
* [0xB0 ~ 0xBF]   Unused
*/
#define BIOS_RAM_REGB0                  0xB0
#define BIOS_RAM_REGB1                  0xB1
#define BIOS_RAM_REGB2                  0xB2
#define BIOS_RAM_REGB3                  0xB3
#define BIOS_RAM_REGB4                  0xB4
#define BIOS_RAM_REGB5                  0xB5
#define BIOS_RAM_REGB6                  0xB6
#define BIOS_RAM_REGB7                  0xB7
#define BIOS_RAM_REGB8                  0xB8
#define BIOS_RAM_REGB9                  0xB9
#define BIOS_RAM_REGBA                  0xBA
#define BIOS_RAM_REGBB                  0xBB
#define BIOS_RAM_REGBC                  0xBC
#define BIOS_RAM_REGBD                  0xBD
#define BIOS_RAM_REGBE                  0xBE
#define BIOS_RAM_REGBF                  0xBF

/*
* [0xC0 ~ 0xCF]   Unused
*/
#define BIOS_RAM_REGC0                  0xC0
#define BIOS_RAM_REGC1                  0xC1
#define BIOS_RAM_REGC2                  0xC2
#define BIOS_RAM_REGC3                  0xC3
#define BIOS_RAM_REGC4                  0xC4
#define BIOS_RAM_REGC5                  0xC5
#define BIOS_RAM_REGC6                  0xC6
#define BIOS_RAM_REGC7                  0xC7
#define BIOS_RAM_REGC8                  0xC8
#define BIOS_RAM_REGC9                  0xC9
#define BIOS_RAM_REGCA                  0xCA
#define BIOS_RAM_REGCB                  0xCB
#define BIOS_RAM_REGCC                  0xCC
#define BIOS_RAM_REGCD                  0xCD
#define BIOS_RAM_REGCE                  0xCE
#define BIOS_RAM_REGCF                  0xCF

/*
* [0xD0 ~ 0xDF]   Unused
*/
#define BIOS_RAM_REGD0                  0xD0
#define BIOS_RAM_REGD1                  0xD1
#define BIOS_RAM_REGD2                  0xD2
#define BIOS_RAM_REGD3                  0xD3
#define BIOS_RAM_REGD4                  0xD4
#define BIOS_RAM_REGD5                  0xD5
#define BIOS_RAM_REGD6                  0xD6
#define BIOS_RAM_REGD7                  0xD7
#define BIOS_RAM_REGD8                  0xD8
#define BIOS_RAM_REGD9                  0xD9
#define BIOS_RAM_REGDA                  0xDA
#define BIOS_RAM_REGDB                  0xDB
#define BIOS_RAM_REGDC                  0xDC
#define BIOS_RAM_REGDD                  0xDD
#define BIOS_RAM_REGDE                  0xDE
#define BIOS_RAM_REGDF                  0xDF

/*
* [0xE0 ~ 0xEF]   Unused
*/
#define BIOS_RAM_REGE0                  0xE0
#define BIOS_RAM_REGE1                  0xE1
#define BIOS_RAM_REGE2                  0xE2
#define BIOS_RAM_REGE3                  0xE3
#define BIOS_RAM_REGE4                  0xE4
#define BIOS_RAM_REGE5                  0xE5
#define BIOS_RAM_REGE6                  0xE6
#define BIOS_RAM_REGE7                  0xE7
#define BIOS_RAM_REGE8                  0xE8
#define BIOS_RAM_REGE9                  0xE9
#define BIOS_RAM_REGEA                  0xEA
#define BIOS_RAM_REGEB                  0xEB
#define BIOS_RAM_REGEC                  0xEC
#define BIOS_RAM_REGED                  0xED
#define BIOS_RAM_REGEE                  0xEE
#define BIOS_RAM_REGEF                  0xEF

/*
* [0xF0 ~ 0xFF]   Unused
*/
#define BIOS_RAM_REGF0                  0xF0
#define BIOS_RAM_REGF1                  0xF1
#define BIOS_RAM_REGF2                  0xF2
#define BIOS_RAM_REGF3                  0xF3
#define BIOS_RAM_REGF4                  0xF4
#define BIOS_RAM_REGF5                  0xF5
#define BIOS_RAM_REGF6                  0xF6
#define BIOS_RAM_REGF7                  0xF7
#define BIOS_RAM_REGF8                  0xF8
#define BIOS_RAM_REGF9                  0xF9
#define BIOS_RAM_REGFA                  0xFA
#define BIOS_RAM_REGFB                  0xFB
#define BIOS_RAM_REGFC                  0xFC
#define BIOS_RAM_REGFD                  0xFD
#define BIOS_RAM_REGFE                  0xFE
#define BIOS_RAM_REGFF                  0xFF

#endif



