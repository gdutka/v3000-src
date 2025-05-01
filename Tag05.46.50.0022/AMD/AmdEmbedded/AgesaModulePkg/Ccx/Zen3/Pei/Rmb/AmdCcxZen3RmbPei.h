/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CCX Zen3 RMB API, and related functions.
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Ccx
 * @e \$Revision$   @e \$Date$
 *
 */

#ifndef _AMD_CCX_ZEN3_RMB_PEI_H_
#define _AMD_CCX_ZEN3_RMB_PEI_H_

#pragma pack (push, 1)

/*---------------------------------------------------------------------------------------
 *          M I X E D   (Definitions And Macros / Typedefs, Structures, Enums)
 *---------------------------------------------------------------------------------------
 */


/*---------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *---------------------------------------------------------------------------------------
 */
#define  AP_ALLOCATION_SIZE  0x100000 // This should match the value of FETCH_FROM_SPI_SIZE which is defined in ABL
#define  RMB_MAX_NUMBER_OF_APS 15

// layout of ApStartupVector
// +---------------------------+
// | ApSyncLocation            |
// |   2 bytes (UINT16)        |
// +---------------------------+ +-> ApStartupVector + E
// | AllowToLaunchNextThreadLocation
// |   4 bytes (UINT32)        |
// +---------------------------+ +-> ApStartupVector + A
// | BSP GDTR                  |
// |   6 bytes (UINT16 + UINTN)|
// +-------------------+-------+ +-> ApStartupVector + 4
// | Near Jump to ApStartupCode|
// |   4 bytes                 |
// +---------------------------+ +-> ApStartupVector (got from BIOS directory)
// |                           |
// |  GDT                      |
// |    BSP_GDT_SIZE           |
// |    72 should be enough    |
// +---------------------------+ +-> ApStartupVector - BSP_GDT_OFFSET
// |                           |
// |  ApMtrrSyncList[]         |
// |    BSP_MSR_SIZE           |
// |    0x170 should be enough |
// |                           |
// |                           |
// +---------------------------+ +-> ApStartupVector - BSP_MSR_OFFSET
// |                           |
// |  Code for ApStartupCode   |
// |    AP_STARTUP_CODE_SIZE   |
// |    0x50 should be enough  |
// |                           |
// +---------------------------+ +-> ApStartupVector - AP_STARTUP_CODE_OFFSET

// IMPORTANT NOTE:
#define  AP_STARTUP_CODE_SIZE   0x50
#define  BSP_MSR_SIZE           0x170
#define  BSP_GDT_SIZE           0x50
#define  AP_TEMP_BUFFER_SIZE    (BSP_GDT_SIZE + BSP_MSR_SIZE + AP_STARTUP_CODE_SIZE + 0x10)

#define  AP_STARTUP_CODE_OFFSET (BSP_GDT_SIZE + BSP_MSR_SIZE + AP_STARTUP_CODE_SIZE)
#define  BSP_MSR_OFFSET         (BSP_GDT_SIZE + BSP_MSR_SIZE)
#define  BSP_GDT_OFFSET         (BSP_GDT_SIZE)

// If below size is changed, please update the same definition in ApAsm.nasm
#define  AP_STACK_SIZE          0x200
#define  MAX_APS_SUPPORTED       7


#define  ZEN3_CAC_WEIGHT_NUM  21

/*---------------------------------------------------------------------------------------
 *               T Y P E D E F S,   S T R U C T U R E S,    E N U M S
 *---------------------------------------------------------------------------------------
 */
/// GDT descriptor
typedef struct {
  UINT16  Size;        ///< Size
  UINTN   Pointer;     ///< Pointer
} CCX_GDT_DESCRIPTOR;

/// AP MSR sync up
typedef struct {
  IN  UINT32 MsrAddr;     ///< MSR address
  IN  UINT64 MsrData;     ///< MSR Settings
  IN  UINT64 MsrMask;     ///< MSR mask
} AP_MSR_SYNC;

/// Core Watchdog
typedef struct {
BOOLEAN              CpuWdtEn;
UINT16               CpuWdtTimeOut;
UINT8                CpuWdTmrCfgSeverity;
UINT8                PcdAmdRedirectForReturnDis;
} CORE_WATCHDOG;

/// GDT descriptor
typedef struct {
  UINT32                     BspMsrLocation;                   ///< Do NOT change the offset of this variable
  UINT32                     ApSyncLocation;                   ///< Do NOT change the offset of this variable
  UINT32                     AllowToLaunchNextThreadLocation;  ///< Do NOT change the offset of this variable
  VOID                       *ApStackBasePtr;                  ///< Do NOT change the offset of this variable
  CCX_GDT_DESCRIPTOR         ApGdtDescriptor;                  ///< Do NOT change the offset of this variable
  UINT8                      SleepType;
  UINT32                     SizeOfApMtrr;
  VOLATILE AP_MTRR_SETTINGS  *ApMtrrSyncList;
  VOLATILE AP_MSR_SYNC       *ApMsrSyncList;
  UINT64                     BspPatchLevel;
  UINT64                     UcodePatchAddr;
  ENTRY_CRITERIA             ResetTableCriteria;
  CORE_WATCHDOG              CpuWdt;
  UINT64                     CacWeights[ZEN3_CAC_WEIGHT_NUM];
} AMD_CCX_AP_LAUNCH_GLOBAL_DATA;


/*---------------------------------------------------------------------------------------
 *                        F U N C T I O N    P R O T O T Y P E
 *---------------------------------------------------------------------------------------
 */
VOID ApAsmCode (
  VOID
 );

VOID
RegSettingBeforeLaunchingNextThread (
  );

VOID
ApEntryPointInC (
  IN       VOLATILE AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData
  );

VOID
SetupApStartupRegion (
  IN       VOLATILE AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData,
  IN       EFI_PHYSICAL_ADDRESS *ApStartupVector,
  IN       VOID                **MemoryContentCopy
  );

VOID
RestoreResetVector (
  IN       VOLATILE AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData,
  IN       UINT16 TotalCoresLaunched,
  IN       EFI_PHYSICAL_ADDRESS *ApStartupVector,
  IN       VOID                **MemoryContentCopy
  );

VOID
CcxZen3RmbMicrocodeInit (
  IN OUT   UINT64             *UcodePatchAddr,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

VOID
CcxZen3RmbEnableWdt (
  IN       CORE_WATCHDOG  *CpuWdt
  );

VOID
CcxZen3RmbEnableSmee (
  );

VOID
CcxZen3RmbPrefetcher (
  );

VOID
CcxZen3RmbSetMiscMsrs (
  );

VOID
CcxZen3RmbSyncMiscMsrs (
  IN       VOLATILE AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData
  );

VOID
CcxZen3RmbGetCacWeights (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI       *NbioSmuServices,
  IN       UINT64                              *CacWeights
  );

VOID
CcxZen3RmbSetCacWeights (
  IN       UINT64                              *CacWeights
  );

AGESA_STATUS
CcxZen3RmbInitializeC6 (
  IN       AMD_CONFIG_PARAMS         *StdHeader
  );

AGESA_STATUS
CcxZen3RmbInitializeCpb (
  IN       AMD_CONFIG_PARAMS         *StdHeader
  );

///  The possible hardware prefetch mode settings.
typedef enum  {
  HARDWARE_PREFETCHER_AUTO,                     ///< Use the recommended setting for the processor. In most cases, the recommended setting is enabled.
  DISABLE_HW_PREFETCHER_TRAINING_ON_SOFTWARE_PREFETCHES,  ///< Use the recommended setting for the hardware prefetcher, but disable training on software prefetches.
  DISABLE_L1_PREFETCHER,                        ///< Use the recommended settings for the hardware prefetcher, but disable L1 prefetching and above.
  DISABLE_L2_STRIDE_PREFETCHER,                 ///< Use the recommended settings for the hardware prefetcher, but disable the L2 stride prefetcher and above
  DISABLE_HARDWARE_PREFETCH,                    ///< Disable hardware prefetching.
  MAX_HARDWARE_PREFETCH_MODE                    ///< Not a hardware prefetch mode, use for limit checking.
} HARDWARE_PREFETCH_MODE;

///  The possible software prefetch mode settings.
typedef enum  {
  SOFTWARE_PREFETCHES_AUTO,                     ///< Use the recommended setting for the processor. In most cases, the recommended setting is enabled.
  DISABLE_SOFTWARE_PREFETCHES,                  ///< Disable software prefetches (convert software prefetch instructions to NOP).
  MAX_SOFTWARE_PREFETCH_MODE                    ///< Not a software prefetch mode, use for limit checking.
} SOFTWARE_PREFETCH_MODE;

/// Advanced performance tunings, prefetchers.
/// These settings provide for performance tuning to optimize for specific workloads.
typedef struct {
  IN HARDWARE_PREFETCH_MODE  HardwarePrefetchMode; ///< This value provides for advanced performance tuning by controlling the hardware prefetcher setting.
  IN SOFTWARE_PREFETCH_MODE  SoftwarePrefetchMode; ///< This value provides for advanced performance tuning by controlling the software prefetch instructions.
} CCX_PREFETCH_MODE;

AGESA_STATUS
CcxZen3RmbInitializePrefetchMode (
  IN       AMD_CONFIG_PARAMS                *StdHeader
  );

VOID
CcxZen3RmbSyncMiscMsrs (
  IN       VOLATILE AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData
  );

#pragma pack (pop)

#endif  // _AMD_CCX_ZEN3_RMB_PEI_H_



