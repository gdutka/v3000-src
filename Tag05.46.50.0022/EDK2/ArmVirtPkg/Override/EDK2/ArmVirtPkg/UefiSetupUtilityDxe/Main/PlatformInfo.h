/** @file

;******************************************************************************
;* Copyright (c) 2014 - 2016, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _SETUP_PLATFORM_INFO_H_
#define _SETUP_PLATFORM_INFO_H_

#include <Uefi.h>
#include <SetupUtility.h>
//#include <SaAccess.h>
//#include <CpuRegs.h>
//#include <CpuDataStruct.h>
//#include <OemPlatformInforDisplayOptionDef.h>
//#include <OemDataHubSubClassProcessor.h>
#include <Protocol/DataHub.h>

//#include <Protocol/MeVersion.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>
//#include <Protocol/GopComponentName2.h>
#include <IndustryStandard/Pci30.h>
//#include <Register/Cpuid.h>

#define DISPLAY_ENABLE       1
#define DISPLAY_INFO_END     2
//
// Platform Information Display Option Table
//
#define PLATFORM_INFO_DISPLAY_OPTION_TABLE_LIST     {DISPLAY_INFO_END, NULL}

#define NOT_SUPPORTED                  0
#define SUPPORTED                      1

#define LEGACY_BIOS_INT_10             0x10

#define VBE_OEM_EXTENSION_SERVICES     0x4F14
#define GET_VBIOS_VERSION_SUBFUNCTION  0x0087

#define GET_VIDEO_BIOS_INFORMATION     0x5F01

#define MAXIMUN_EFFICIENCY_RATIO       100
#define CRB_BOARD_ID_1                 0x01
#define CRB_BOARD_ID_FAB_ID_FAB2       0x01
#define CRB_BOARD_ID_FAB_ID_FAB3       0x02
#define CRB_BOARD_ID_FAB_ID_FAB4       0x03
#define FAB2_ID                        0x02
#define FAB3_ID                        0x03
#define FAB4_ID                        0x04
//
// LAN PHY Revision definitions
//
#define PHY_CONFIG_REG    0x00000020
#define PHY_SEMAPHORE_REG 0x00000F00
#define PHY_PAGE769_SET_REG 0x43f6020
#define PHY_SLOW_MDIO_MODE_REG 0x4302580
#define PHY_READ_PHY_OFFSET3_REG 0x8430000
#define PHY_FAST_MDIO_MODE_REG 0x4302180
#define LAN_PHY_REV_TC 0xA0
#define LAN_PHY_REV_A0 0xA1
#define LAN_PHY_REV_A1 0xA2
#define LAN_PHY_REV_A2 0xA3
#define LAN_PHY_REV_A3 0xA4
#define LAN_PHY_REV_B1 0xA5
#define LAN_PHY_REV_B2 0xA6
#define GBE_MAX_LOOP_TIME 4000
EFI_STATUS
typedef
( EFIAPI *DISPLAY_PLATFORM_INFO_FUNCTION ) (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

EFI_STATUS
typedef
( EFIAPI *DISPLAY_CPU_INFO_FUNCTION ) (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    SMBIOS_TABLE_TYPE4        *Record
  );

EFI_STATUS
typedef
( EFIAPI *DISPLAY_CPU_CAPABILITY_FUNCTION ) (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    VOID*                     CpuIdRegisters
  );

EFI_STATUS
typedef
( EFIAPI *DISPLAY_SA_INFO_FUNCTION ) (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

EFI_STATUS
typedef
( EFIAPI *DISPLAY_EC_INFO_FUNCTION ) (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

typedef struct {
  UINTN                            Option;
  DISPLAY_PLATFORM_INFO_FUNCTION   DisplayPlatformInfoFunction;
} PLATFORM_INFO_DISPLAY_TABLE;

typedef struct {
  UINTN                            Option;
  DISPLAY_CPU_INFO_FUNCTION        DisplayCpuInfoFunction;
} CPU_INFO_DISPLAY_TABLE;


typedef struct {
  UINTN                            Option;
  DISPLAY_EC_INFO_FUNCTION         DisplayEcFunction;
} EC_INFO_DISPLAY_TABLE;

typedef struct {
  UINTN                            Level;
  UINTN                            Type;
  UINTN                            CacheSize;
} CACHE_INFO_SIZE_TABLE;

typedef struct {
  UINTN                            Option;
  DISPLAY_CPU_CAPABILITY_FUNCTION  DisplayCpuCapabilityFunction;
} CPU_CAPABILITY_DISPLAY_TABLE;

typedef struct {
  UINT8                            ReversionValue;
  CHAR16                           *SteppingString;
} STEPPING_STRING_DEFINITION;

typedef struct {
  UINT16                           ReversionValue;
  CHAR16                           *SteppingString;
} SKU_STRING_DEFINITION;

typedef struct {
  UINT16                           SubInstance;
  EFI_STRING_ID                    TokenToUpdate1;
  EFI_STRING_ID                    TokenToUpdate2;
} UPDATE_INFO_STRUCT;

//
// Function Definition
//
EFI_STATUS
PlatformInfoInit (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle

  );

EFI_STATUS
GetVbiosVersion (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

EFI_STATUS
GetGopVersion (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

EFI_STATUS
GetEcVersion (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

EFI_STATUS
GetPchReversionId (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

EFI_STATUS
GetSaReversionId (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

EFI_STATUS
GetEcVerIdFunc (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

EFI_STATUS
GetBoardFunc (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

EFI_STATUS
GetFabFunc (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );


EFI_STATUS
GetMeVersion (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

EFI_STATUS
GetCpuMiscInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

EFI_STATUS
GetCpuSpeedFunc (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    SMBIOS_TABLE_TYPE4        *Record
  );

EFI_STATUS
GetCacheInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    SMBIOS_TABLE_TYPE4        *Record
  );

EFI_STATUS
GetL1DataCache (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    SMBIOS_TABLE_TYPE4        *Record
  );

EFI_STATUS
GetL1Insruction (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    SMBIOS_TABLE_TYPE4        *Record
  );

EFI_STATUS
GetL2Cache (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    SMBIOS_TABLE_TYPE4        *Record
  );

EFI_STATUS
GetL3Cache (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    SMBIOS_TABLE_TYPE4        *Record
  );

EFI_STATUS
GetCpuIdFunc (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    SMBIOS_TABLE_TYPE4        *Record
  );
  
EFI_STATUS
GetCpuSteppingFunc (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    SMBIOS_TABLE_TYPE4        *Record
  );

EFI_STATUS
GetCoreThreadNumFunc (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    SMBIOS_TABLE_TYPE4        *Record
  );

EFI_STATUS
GetMicrocodeVersion (
  IN     VOID                       *OpCodeHandle,
  IN     EFI_HII_HANDLE             MainHiiHandle,
  IN     EFI_HII_HANDLE             AdvanceHiiHandle,
  IN     CHAR16                     *StringBuffer,
  IN    SMBIOS_TABLE_TYPE4        *Record
  );

EFI_STATUS
GetTxtCapability (
  IN     VOID                       *OpCodeHandle,
  IN     EFI_HII_HANDLE             MainHiiHandle,
  IN     EFI_HII_HANDLE             AdvanceHiiHandle,
  IN     CHAR16                     *StringBuffer,
  IN     VOID*                      CpuIdRegisters
  );

EFI_STATUS
GetVtdCapability (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    VOID*                     CpuIdRegisters
  );

EFI_STATUS
GetVtxCapability (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    VOID*                     CpuIdRegisters
  );

EFI_STATUS
NewStringToHandle (
  IN  EFI_HII_HANDLE           SrcHiiHandle,
  IN  EFI_STRING_ID            SrcStringId,
  IN  EFI_HII_HANDLE           DstHiiHandle,
  OUT EFI_STRING_ID            *DstStringId
  );
EFI_STATUS
GetGTInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer,
  IN    SMBIOS_TABLE_TYPE4        *Record
  );
EFI_STATUS
GetLanPhyReversion (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_HII_HANDLE            MainHiiHandle,
  IN    EFI_HII_HANDLE            AdvanceHiiHandle,
  IN    CHAR16                    *StringBuffer
  );

EFI_STATUS
AcquireGbeSemaphore(
  IN  UINT32      GbEBar
  );

VOID
ReleaseGbeSemaphore(
  IN  UINT32      GbEBar
  );

EFI_STATUS
GetPhyVersion(
  UINT32 GbEBar,
  UINT8  *LanPhyRev
  );
#endif
