/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */

#ifndef _AOD_H_
#define _AOD_H_

#include <Library/ApobCommonServiceLib.h>
#pragma pack(push, 1)
#define LINER_DATA          1
#define DISCREATE_DATA      0

#define AOD_MEMORY_BUFFER_VERSION   6
#define AOD_MAX_DIMMS_PER_CHANNEL    2
#define AOD_MAX_CHANNELS_PER_SOCKET  8

  #define COMMAND_BUFFER_START           0x00040001 //Command Buffer Start
  #define COMMAND_BUFFER_END             0x00040002 //Command Buffer End
  #define GET_OC_DISABLE                 0x00010003 //Get OC Disable
  #define GET_OC_VOLATAGE_MAX            0x00010004 //Get OC Voltage Max
  #define GET_OC_FREQUENCY_MAX           0x00010005 //Get OC Frequency Max
  #define GET_GFX_OC_VOLATAGE_MAX        0x00010006 //Get GFX OC Voltage Max
  #define GET_GFX_OC_FREQUENCY_MAX       0x00010007 //Get GFX OC Frequency Max
  #define GET_INTERLEVAVINGCAP           0x00010008 //Get InterLeaving Cap
  #define SET_SOFTWARE_DOWN_CORE         0x00020027 //Software Downcore Config
  #define SET_SMTEN                      0x00020026 //Set SMTEn
  #define SET_MEM_CLOCK                  0x00020001 //Set Mem Clock
  #define SET_TCL                        0x00020002 //Set Tcl
  #define SET_TRCDRD                     0x00020003 //Set Trcdrd
  #define SET_TRCDWR                     0x00020004 //Set Trcdwr
  #define SET_TRCD                       0x0002003B //Set Trcd
  #define SET_TRP                        0x00020006 //Set Trp
  #define SET_TRAS                       0x00020005 //Set Tras
  #define SET_TRCPAGE                    0x00020008 //Set Trcpage
  #define SET_TRC                        0x00020009 //Set Trc
  #define SET_TWR                        0x00020010 //Set Twr
  #define SET_TCKE                       0x00020013 //Set TCKE
  #define SET_TCWL                       0x00020014 //Set Tcwl
  #define SET_TRFC1                      0x0002003C //Set Trfc1
  #define SET_TRFC2                      0x0002000B //Set Trfc2
  #define SET_TRFCSB                     0x0002003D //Set TrfcSb
  #define SET_TRTP                       0x00020015 //Set Trtp
  #define SET_TRRDL                      0x0002000F //Set TrrdL
  #define SET_TRRDS                      0x0002000E //Set TrrdS
  #define SET_TFAW                       0x0002000D //Set Tfaw
  #define SET_TWTRL                      0x00020012 //Set TwtrL
  #define SET_TWTRS                      0x00020011 //Set TwtrS
  #define SET_TRDRDSCL                   0x00020017 //Set TrdrdScL
  #define SET_TRDRDSC                    0x00020016 //Set TrdrdSc
  #define SET_TRDRDSD                    0x00020018 //Set TrdrdSd
  #define SET_TRDRDDD                    0x00020019 //Set TrdrdDd
  #define SET_TWRWRSCL                   0x0002001B //Set TwrwrScL
  #define SET_TWRWRSC                    0x0002001A //Set TwrwrSc
  #define SET_TWRWRSD                    0x0002001C //Set TwrwrSd
  #define SET_TWRWRDD                    0x0002001D //Set TwrwrDd
  #define SET_TWRRD                      0x0002001F //Set Twrrd
  #define SET_TRDWR                      0x0002001E //Set Trdwr
  #define SET_TRFC                       0x0002000A //Set Trfc
  #define SET_ADDRCMDSETUP               0x00020029 //Set AddrCmdSetup
  #define SET_CSODTSETUP                 0x0002002A //Set CsOdtSetup
  #define SET_CADBUSCLKDRVSTREN          0x0002002C //Set CadBusClkDrvStren
  #define SET_CADBUSADDRCMDDRVSTREN      0x0002002D //Set CadBusAddrCmdDrvStren
  #define SET_CADBUSCSODTCMDDRVSTREN     0x0002002E //Set CadBusCsOdtCmdDrvStren
  #define SET_INTERLEAVEMODE             0x00020031 //Set Interleave Mode
  #define SET_INTERLEAVESIZE             0x00020032 //Set Interleave Size
  #define SET_RTTNOMWR                   0x0002003E //Set RttNomWr
  #define SET_RTTNOMRD                   0x0002003F //Set RttNomRd
  #define SET_RTTWR                      0x00020023 //Set RttWr
  #define SET_RTTPARK                    0x00020024 //Set RttPark
  #define SET_CLDO_VDDP                  0x00020030 //Set CLDO_VDDP
  #define SET_RTTPARKDQS                 0x00020040 //Set RttParkDqs
  #define SET_PROCODT                    0x00020007 //Set ProcODT
  #define SET_PROCDATADRIVESTRENGTH      0x00020041 //Set ProcDataDriveStrength
  #define SET_DRAMDATADRIVESTRENGTH      0x00020042 //Set DRAMDataDriveStrength
  #define SET_POWERDWONEN                0x00020025 //Set PowerDwonEn
  #define SET_VDDIO                      0x00030001 //Set DIMM VddIo
  #define SET_VTT                        0x00030002 //Set VTT
  #define SET_VPP                        0x00030003 //Set VPP
  #define SET_APU_VDDIO                  0x00030004 //Set APU VddIo
  #define SET_VDD_MEM                    0x00030005 //Set VDD_MEM
  #define SET_PPT_LIMIT                  0x00050001 //Set PPT Limit
  #define SET_TDC_LIMIT                  0x00050002 //Set TDC Limit
  #define SET_EDC_LIMIT                  0x00050003 //Set EDC Limit
  #define SET_SOC_TDC_LIMIT              0x0005000C //Set Soc TDC Limit
  #define SET_SOC_EDC_LIMIT              0x0005000D //Set Soc EDC Limit
  #define SET_SCALAR                     0x00050004 //Set Scalar
  #define SET_CURVE_OPT                  0x0005000A //Set Curve Optimizer
  #define SET_GFX_CURVE_OPT              0x00050010 //Set GFX Curve Optimizer
  #define SET_CCLK_FMAX                  0x00020036 //Set CCLK Fmax
  #define SET_GFXCCLK_FMAX               0x00020037 //Set GFXCCLK Fmax
  #define SET_FCLK_OC_MODE               0x00020033 //Set FCLK OC Mode
  #define SET_FCLK_FREQUENCY             0x00020035 //Set FCLK Frequency
  #define SET_MEMINTERLEAVINGMODE        0x00020031 //Set MemInterleaving Mode
  #define SET_MEMINTERLEAVINGSIZE        0x00020032 //Set MemInterleaving Size
  #define SET_SOCVID                     0x00020034 //Set SOC VID
  #define SET_DRAM_MAP_INVERSION         0x00050005 //Set DRAM Map Inversion
  #define SET_LCLK_FREQ                  0x00020043 //Set LCLK Freq
  #define SET_VDD_MISC                   0x00030006 //Set VDD_MISC
  #define SET_VDDG                       0x00020038 //Set CLDO VDDG
  #define SET_VDDG_IOD                   0x0005000B //Set VDDG IOD
  #define SET_CCD_FREQ_OPT               0x00050011 //Set CCD Freq Optimizer
  #define SET_UCLK_DIV1                  0x00020044 //Set UCLK DIV1
  #define SET_ECO_MODE                   0x0002003A //Set ECO Mode
  #define SET_RMP_MODE                   0x00020045 //Set RMP Mode
  #define SET_RMP_PROFILE                0x00020046 //Set RMP Profile
  #define SET_RMP_PSTATE                 0x00020047 //Set RMP PState
  #define SET_DIMM_VDDQ                  0x00030007 //Set DIMM Vdd

typedef struct _DownBitmapStruct {
  UINT16     DownCoreBitmap    : 8; // 0x00 means all cores enabled, 0xff means all core disable
  UINT16     DieNum            : 7; // Die or CCD index to be perform bitmap downcore
  UINT16     IsBitmap          : 1; // 1: Bitmap downcore
} DownCoreBitmapStruct;

typedef struct _DownCcdCoreStruct {
  UINT16     DownCoreIndex     : 8; // 0 means Auto and all Cores are enabled
  UINT16     DownCcdIndex      : 7; // 0 means Auto and all CCDs are enabled
  UINT16     IsBitmap          : 1; // 0: Using DownCoreIndex and DownCcxIndex, not bitmap downcore
} DownCcdCoreStruct;


typedef union _DowncoreUnion {
  DownCoreBitmapStruct DownCoreConfig;
  DownCcdCoreStruct    DownCcdCoreIndexConfig;
  UINT16               ActiveCoreNum;
} DowncoreUnion;

//
//Command ID: 0x00020043
//  Command Name: "Set LCLK Freq"
//  Command Support Value: Linear, range 150~2500,
//  A unsigned int dword is needed when RM pass the value of WMI Method "RunCommand" to BIOS ACPI.
//  Bit[15:0]: Frequency
//  Bit[30]: 1 Set Max LCLK limit, 0 Set Mmin LCLK limit
//  Bit[31]: 1 Enable LCLK Frequency Adjust, 0 Auto by system default
//
#define LCLK_MANUEL                             1
#define LCLK_AUTO                               0
typedef union _AOD_LCLK_FREQ {
  struct {
    UINT32  Frequency       : 16; // Bit[15:0]: Frequency
    UINT32  Rsved           : 14;
    UINT32  MaxOrMin        : 1;  // Bit[30]: 1 Set Max LCLK limit, 0 Set Mmin LCLK limit
    UINT32  State           : 1;  // Bit[31]: 1 Enable LCLK Frequency Adjust, 0 Auto by system default
  } Bits;
  UINT32  Raw;
} AOD_LCLK_FREQ;

//
//Command ID: 0x00050010
//  Command Name: "Set GFX Curve Optimizer"
//  Command Support Value: Linear, range 0~30,
//  A unsigned char byte is needed when RM pass the value of WMI Method "RunCommand" to BIOS ACPI.
//  Bit[7:6]: 0 means disable, 1 means Positive, 2 means Negative
//  Bit[4:0]: range 0 ~30
//
#define GFX_CURVE_OPTIMIZER_DISABLE             0
#define GFX_CURVE_OPTIMIZER_POSITIVE            1
#define GFX_CURVE_OPTIMIZER_NEGATIVE            2
#define GFX_CURVE_OPTIMIZER_MAGNITUDE_MAX       30
typedef union _AOD_GFX_PSM {
  struct {
    UINT8  Magnitude       : 5; //Bit[4:0]: range 0 ~30
    UINT8  Rsved           : 1;
    UINT8  State           : 2; //Bit[7:6]: 0 means disable, 1 means Positive, 2 means Negative
  } GfxPsmData;
  UINT8  Raw;
} AOD_GFX_PSM;

//
//Command ID: 0x00020038
//  Command Name: "Set CLDO VDDG"
//  Command Support Value: Linear, range 600~1500,
//  A unsigned word is needed when RM pass the value of WMI Method "RunCommand" to BIOS ACPI.
//  Bit[10:0]: voltage
//  Bit[13:11]: ccd number
//  Bit[14]: 0 means Global Manual Control,1 means Per-CCD Control
//
typedef union _AOD_VDDG {
  struct {
    UINT16   Voltage     : 11; // VddG voltage
    UINT16   Ccd         : 3;  // ccd number
    UINT16   PerCcd      : 1;  // 0 means Global Manual Control,1 means Per-CCD Control
    UINT16   Rsv         : 1;
  } Bits;
  UINT16   Raw;
} AOD_VDDG, AOD_VDDG_IOD;

//
// Command ID:  0x50011
// Command Name: "Set CCD Freq Optimizer"
//  A unsigned dword is needed when RM pass the value of WMI Method "RunCommand" to BIOS ACPI.
//  Bit[16:0]: Freq in MHz
//  Bit[19:16] Phycical CCD index
//  Bit[31] 0 means disabled, 1 means CCD Frequency Adjust
//
typedef union _AOD_CCD_FREQ_OPT {
  struct {
    UINT32   Freq        : 16; // [15:0] Freq in MHz
    UINT32   Ccd         : 4;  // [19:16] Phycical CCD index
    UINT32   Rsv         : 11; // [30:20] Reserved
    UINT32   Enable      : 1;  // [31] 0 means disabled, 1 means CCD Frequency Adjust
  } Bits;
  UINT32   Raw;
} AOD_CCD_FREQ_OPT;
typedef struct _AodMemDimmSpdDataStruct {
  UINT8 MaxDimmsPerChannel;
  UINT8 MaxChannelsPerSocket;
  UINT8 Reserved[2];
  APOB_SPD_STRUCT   DimmSmbusInfo[AOD_MAX_DIMMS_PER_CHANNEL * AOD_MAX_CHANNELS_PER_SOCKET];
} AodMemDimmSpdDataStruct;
///
/// definition of RMP data to Ryzen Master
///
/*
class RMPPackage
{
        [WmiDataId(1),
        read, write,
        Description("Is RMP available")
        ] uint8 IsRmpAvailable;
        [WmiDataId(2),
        read, write,
        Description("Number of RMP Profiles")
        ] uint8 NumberOfProfiles;
        [WmiDataId(3),
        read, write,
        Description("Basic Info : Profile Enable State in RMP SPD ")
        ] uint8 IsProfileEnable[2];
        [WmiDataId(4),
        read, write,
        Description("Basic Info : Profile Memory Clock units MHz")
        ] uint16 MemoryClock[2];
        [WmiDataId(5),
        read, write,
        Description("Basic Info : CAS Latency in CLK")
        ] uint16 CASLatency[2];
        [WmiDataId(6),
         read, write,
         Description("RMP SPD Data")
        ] uint8 Data[128];
}
*/
#define AOD_MAX_RMP_PROFILE 2
typedef struct _AOD_RMP_PACKAGE {
  UINT8    IsRmpAvailable;                               // Is RMP available
  UINT8    NumberOfProfiles;                             // Number of RMP Profiles
  UINT8    IsProfileEnable[AOD_MAX_RMP_PROFILE];         // Is Profile Enabled in SPD
  UINT16   MemoryClock[AOD_MAX_RMP_PROFILE];             // Memory Clock of each RMP Profile in MHz
  UINT16   CASLatency[AOD_MAX_RMP_PROFILE];              // CAS Latency in CLK
  UINT8    RMPSpdData[128];
} AOD_RMP_PACKAGE;
typedef struct _AodNvAreaStruct {   // Size: 200 + 8 + 532 * 16 + 140 = 8512
  UINT16                    AodSmiCmd;
  UINT32                    AodSmiStatus;
  UINT8                     Reserved[194];
  UINT32                    CmdId;
  UINT32                    CmdData;
  AodMemDimmSpdDataStruct   AodMemDimmSpdData;
  UINT64                    AodSmmCommBufAddr;
  UINT32                    AodSmmCommBufSize;
  AOD_RMP_PACKAGE           AodRmpPackData;
  UINT32                    AodWMIRuntimeCurrent[128];
} AodNvAreaStruct;

typedef struct _AOD_NV_AREA_UPDATED_PROTOCOL {
  UINT32                   Version;
  AodNvAreaStruct          *AodNvAreaPtr;
} AOD_NV_AREA_UPDATED_PROTOCOL;





#pragma pack(pop)

  /*Cmdid                  ,   Name           ,     Property, Data Count, Data*/
#define AOD_COMMON_WMI_LIST\
  {COMMAND_BUFFER_START,       "Command Buffer Start",          1,     0,          {0,}},\
  {COMMAND_BUFFER_END,         "Command Buffer End",            1,     0,          {0,}},\
  {GET_OC_DISABLE,             "Get OC Disable",                1,     3,          {0,0xffffffff,1}},\
  {GET_OC_VOLATAGE_MAX,        "Get OC Voltage Max",            1,     3,          {0,0xffffffff,1}},\
  {GET_OC_FREQUENCY_MAX,       "Get OC Frequency Max",          1,     3,          {0,0xffffffff,1}},\
  {GET_GFX_OC_VOLATAGE_MAX,    "Get GFX OC Voltage Max",        1,     3,          {0,0xffffffff,1}},\
  {GET_GFX_OC_FREQUENCY_MAX,   "Get GFX OC Frequency Max",      1,     3,          {0,0xffffffff,1}}


#endif
