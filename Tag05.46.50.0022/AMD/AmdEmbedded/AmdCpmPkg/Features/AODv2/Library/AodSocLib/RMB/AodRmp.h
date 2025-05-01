/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/

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
#ifndef _AOD_Rmp_H_
#define _AOD_Rmp_H_
#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/MdeModuleHii.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/AmdApcbProtocol.h>
#include <RMB/ApcbV3TokenUid.h>
#include <RMB/ApcbV3Priority.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/AmdPspApobLib.h>
#include <Features/AODv2/Include/Aod.h>
#include <Features/AODv2/Include/Library/AodBaseLib.h>
#include "AodSocLib.h"
#pragma pack(push,1)
#ifndef MAX_RMP_PROFILES
  #define MAX_RMP_PROFILES   2
#endif

#define EXPO_SIGNATURE  SIGNATURE_32 ('E', 'X', 'P', 'O')
///
/// EXPO Header struct definition
///
typedef struct {
  UINT32      Signature;                                   // "EXPO"
  struct {
    //  Minor Version Encoding (e.g.,02h indicates minor revision x.2)
    UINT8     MinorVersion                      : 4;
    //  Major Version Encoding (e.g.,01h indicates major revision 1.y)
    UINT8     MajorVersion                      : 4;
  } Version;
  struct {
    //  Profile - Enable, (1 = enabled, 0 = disabled)
    UINT8     Profile0Enable                    : 1;
    //
    //  Profile 0 DIMMs/channel
    //    supported (1-4, if bit 4 = 0, this field should be 000b)
    //    000b = Profile not supported
    //    001b = 1 DPC,
    //    010b = 2 DPC
    //    011b = 3 DPC
    //    100b = 4 DPC
    //    101b - 111b = Reserved
    //
    UINT8     Profile0SupportedState            : 3;
    //Profile 1 Enable, (1 = enabled, 0 = disabled)
    UINT8     Profile1Enable                    : 1;
    //
    //  Profile 1 DIMMs/channel
    //    supported (1-4, if bit 4 = 0, this field should be 000b)
    //    000b = Profile not supported
    //    001b = 1 DPC,
    //    010b = 2 DPC
    //    011b = 3 DPC
    //    100b = 4 DPC
    //    101b - 111b = Reserved
    //
    UINT8     Profile1SupportedState            : 3;
  } RmpProfileConguration;
  struct {
    //  Profile 0 Block 1 Enable, (1 = enabled, 0 = disabled)
    UINT8     Profile0Block1Support : 1;
    UINT8     Reserved : 3;
    //  Profile 1 Block 1 Enable, (1 = enabled, 0 = disabled)
    UINT8     Profile1Block1Support : 1;
    UINT8     Reserved1 : 3;
  } RmpOptionalBlock;
  UINT8       Rsrved[3];
} RMP_HEADER;
///
/// RMP Voltage struct definition
///
typedef union {
  struct {
    //   BIT0       :  0.05 per step (0 = 0V, 1 = 0.05V)
    UINT8 VDD_0_DOT_05 : 1;
    //   BIT[4:1]   :  0.10V per step (valid values are 0 to 9)
    UINT8 VDD_0_DOT_1  : 4;
    //   BIT[6:5]   :  1V per step (valid values 0 to 2)
    UINT8 VDD_1        : 2;
    UINT8 Reserved_7   : 1;
  } Bits;
  UINT8 Raw;
} RMP_VOLTAGE;
#define AOD_VOLTAGE(a) (((a>>5)&0x3)* 1000 + ((a>>1)&0xf) * 100 + (a&0x1) * 50)
///
/// RMP TIMING struct definition
///
typedef union {
  struct {
    UINT8 Lsb : 8;
    UINT8 Msb : 8;
  } Bits;
  UINT16 Raw;
} RMP_TIMING;

///
/// EXPO Block0 struct definition (9.0)
///
typedef struct {
  RMP_VOLTAGE              VDD;            // SDRAM VDD
  RMP_VOLTAGE              VDDQ;           // SDRAM VDDQ
  RMP_VOLTAGE              VPP;            // SDRAM VPP
  UINT8                    RFU;
  RMP_TIMING               tCKAVGmin;      // Minimum Cycle Time (ps)
  RMP_TIMING               tAAmin;         // Minimum CAS Latency Time (ps)
  RMP_TIMING               tRCDmin;        // Minimum RAS to CAS Delay Time (ps)
  RMP_TIMING               tRPmin;         // Row Precharge Delay Time (ps)
  RMP_TIMING               tRASmin;        // Minimum Active to Precharge Delay Time (ps)
  RMP_TIMING               tRCmin;         // Minimum Active to Active/Refresh Delay Time (ps)
  RMP_TIMING               tWRmin;         // Minimum Write Recovery Time Time (ps)
  RMP_TIMING               tRFC1min;       // Minimum Refresh Recovery Delay Time, normal refresh mode (ns)
  RMP_TIMING               tRFC2min;       // Minimum Refresh Recovery Delay Time, fine granularity refresh mode (ns)
  RMP_TIMING               tRFCsbmin;      // Minimum Refresh Recovery Delay Time, same bank refresh mode (ns)
} RMP_PROFILE_BLOCK_0;
///
/// EXPO Block1 struct definition (9.0)
///
typedef struct {
  RMP_TIMING               tRRD_Lmin;      // Minimum Active to Active Delay Time within same bank (ps)
  RMP_TIMING               tCCD_Lmin;      // Minimum Read to Read Command Delay within same bank group (ps)
  RMP_TIMING               tCCD_L_WRmin;   // Minimum Write to Write Command Delay within same bank group (ps)
  RMP_TIMING               tCCD_L_WR2min;  // Minimum Write to Write Command Delay Second Write Not RMW within same bank group (ps)
  RMP_TIMING               tFAWmin;        // Minimum Four Activate Window (ps)
  RMP_TIMING               tWTR_Lmin;      // Minimum Write to Read Command Delay Second within same bank group (ps)
  RMP_TIMING               tWTR_Smin;      // Minimum Write to Read Command Delay Second within different bank group (ps)
  RMP_TIMING               tRTPmin;        // Minimum Read to Precharge Delay (ps)
} RMP_PROFILE_BLOCK_1;

///
/// RMP Block struct definition
///
typedef struct {
  RMP_PROFILE_BLOCK_0        Block0;
  RMP_PROFILE_BLOCK_1        Block1;
} RMP_PROFILE_BLOCK;
///
/// RMP Profile struct definition
///
typedef struct {
  RMP_HEADER                    Header;
  RMP_PROFILE_BLOCK             RmpProfileBlock[MAX_RMP_PROFILES];
} RMP_PROFILE;

#pragma pack(pop)
#define AOD_VOLTAGE(a) (((a>>5)&0x3)* 1000 + ((a>>1)&0xf) * 100 + (a&0x1) * 50)
#define AOD_MEM_ROUND_DOWN_BY_STEP(a, b) (a -= a%b)
//
// Per DDR5 Jedec Spec:
// Clock periods such as tCKAVGmin are rounded down (RD) to 1 ps of accuracy;
// for example, 0.454545... ns is defined as 454 ps and 0.357142... ns is
//  defined as 357 ps.
//
#define AOD_RD_TIME(Memclk) ((UINT32)(1000000 / Memclk))

// Use JEDEC Rounding Rule to round down with a 0.3% guardband
//
// Integer math is used in the industry to calculate nCK values by expressing
// timing values in ps based integers, scaling the specified parameter value up
// by 1000, dividing by the application clock period (rounded down (RD) to 1ps
// range), adding an inverse correction factor of 99.7% or (997), dividing the
// result by 1000, then truncating (same as integer math) down to the next lower
// integer value
#define AOD_PS_TO_CLKs(Memclk, ps) (((ps * 997)/AOD_RD_TIME(Memclk) + 1000) / 1000)


/**
  This function used to generate Rmp PROFILE Options

  @param[in]  HiiHandle                - the hii handle

  @retval EFI_SUCCESS
          others by callee
**/
EFI_STATUS
EFIAPI
AodGenerateRmpOptions (
  IN  EFI_HII_HANDLE              HiiHandle
  );

/**
  This function used to set VDDIO Phy voltage, when select a Rmp profile.

  @param[in]  HiiHandle          the hii handle
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[in]  HiiHandle          the ifr varstore data
  @param[out] Req                On return, points to the action requested by the
                                 callback function.
  @retval EFI_UNSUPPORTED if not action changing or key value not matched
          EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodExpoProfileSelectCallback (
  IN EFI_HII_HANDLE                       Handle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  );

/**
  This function used to set VDDIO Phy voltage, when select a Rmp mode.

  @param[in]  HiiHandle          the hii handle
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[in]  HiiHandle          the ifr varstore data
  @param[out] Req                On return, points to the action requested by the
                                 callback function.
  @retval EFI_UNSUPPORTED if not action changing or key value not matched
          EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodExpoCallback (
  IN EFI_HII_HANDLE                       Handle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  );

/**
  Update WMI Class RMPPackage

  @param[in]  Nvs     the AOD global NVS

  @retval EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodRMPUpdateNvs (
  IN  VOID            *Nvs
  );

/**
  This function used to Get Rmp data from APOB

  @param[OUT]  RmpData                - the Rmp data from APOB

  @retval EFI_SUCCESS       read Rmp data successfully
          EFI_UNSUPPORTED   Rmp data is invalid
**/
EFI_STATUS
EFIAPI
AodGetRmpData (
  OUT  VOID              **RmpData
  );
#endif