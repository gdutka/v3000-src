/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Protocol/MpService.h>
#include <Guid/EventGroup.h>
#include <Library/CcxRolesLib.h>
#include <Library/CcxBaseX86Lib.h>
#include <Library/AmdBaseLib.h>
#include <Library/CcxHaltLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/CcxResetTablesLib.h>
#include <Library/CcxSetMcaLib.h>
#include <Library/AmdIdsHookLib.h>
// @todo: The family specific includes need to be moved into SoC Zen3 services
#include <Addendum/Apcb/Inc/SSP/APOB.h>
#include <Addendum/Apcb/Inc/SSP/ApcbV3TokenUid.h>
#include <Library/AmdPspApobLib.h>
#include <Library/AmdCapsuleLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/FchBaseLib.h>
#include <Library/AmdHeapLib.h>
#include <Library/FabricWdtLib.h>
#include <Library/CcxSmmAccess2Lib.h>
#include <Library/DxeCcxBaseX86ServicesLib.h>
#include <Library/AmdTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>
#include <Protocol/AmdCoreTopologyV2Protocol.h>
#include <Protocol/FabricTopologyServices2.h>
#include <Protocol/AmdMpServicesPreReqProtocol.h>
#include <Protocol/AmdCcxProtocol.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/AmdNbioServicesProtocol.h>
#include <Protocol/AmdNbioSmuServicesProtocol.h>

#include <Protocol/VariableWrite.h>
#include <Protocol/AmdApcbProtocol.h>
#include <CcxRegistersZen3.h>
#include "AmdCcxZen3Dxe.h"
#include "CcxZen3MicrocodePatch.h"
#include "CcxZen3AcpiServicesDxe.h"
#include "CcxZen3SmbiosDxe.h"
#include "CcxZen3C6.h"
#include "CcxZen3Cpb.h"
#include "CcxZen3Prefetch.h"
#include <cpuRegisters.h>
#include <Filecode.h>

#define FILECODE CCX_ZEN3_DXE_AMDCCXZEN3DXE_FILECODE

extern EFI_GUID gAmdDownCoreStatusGuid;

/*----------------------------------------------------------------------------------------
 *                               D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
#define  AP_ALLOCATION_SIZE  0x100000 // This should match the value of FETCH_FROM_SPI_SIZE which is defined in ABL

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
EFI_EVENT            CcxZen3InitWithMpServicesEvent;
EFI_EVENT            CcxZen3OcCallbackEvent;
EFI_EVENT            CcxZen3OcCallbackOnMpServicesEvent;
AP_MSR_SYNC          mLateMsrSyncTable[] =
{
  { 0xC0010010, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0010030, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0010031, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0010032, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0010033, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0010034, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0010035, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0011020, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0011021, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0011022, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC001102B, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },
  { 0xC0011004, 0x0000000000000000, BIT53  },
  { 0xC0011029, 0x0000000000000000, BIT14  },               // PcdAmdRedirectForReturnDis
  { 0xC0011021, 0x0000000000000000, BIT5   },               // PcdAmdOpcacheCtrl
  { 0xC0011020, 0x0000000000000000, BIT28  }                // PcdAmdStreamingStoresCtrl
};

VOID                 *mRegistrationForCcxZen3InitWithMpServicesEvent;
VOID                 *mRegistrationForCcxZen3OcCallbackEvent;
VOID                 *mRegistrationForCcxZen3OcOnMpServicesCallbackEvent;
UINT64               mUcodePatchAddr = 0;
UINT64               BspPatchLevel;
UINT32               BspMsrLocation;
UINT32               ApSyncLocation;
UINT32               AllowToLaunchNextThreadLocation;
UINT8                SleepType;
VOID                 *ApStackBasePtr;
UINTN                ApTempBufferSize;
EFI_PHYSICAL_ADDRESS ApStartupVector;
CCX_GDT_DESCRIPTOR   ApGdtDescriptor;
VOID                 *MemoryContentCopy;
EFI_PHYSICAL_ADDRESS ApPageAllocation = 0;
SOC_LOGICAL_ID       mLogicalId = {0, 0};
CORE_LOGICAL_ID      mCoreLogicalId = {0, 0};
UINT32               mPackageId = 0;
BOOLEAN              mCpuWdtEn;
UINT16               mCpuWdtTimeOut;
UINT8                mCpuWdTmrCfgSeverity;
UINT8                mPcdAmdRedirectForReturnDis;
UINT8                mPcdAmdOpcacheCtrl;
UINT8                mPcdAmdStreamingStoresCtrl;
BOOLEAN              mPcdEnSpecStFill;
BOOLEAN              mPcdEnableFSRM;
BOOLEAN              mPcdEnableERMS;
BOOLEAN              mPcdEnableRMSS;
BOOLEAN              mPcdEnableSvmAVIC;
BOOLEAN              mPcdAmdIbsHardwareEn;
EFI_PHYSICAL_ADDRESS mRmpTableBase = 0;
EFI_PHYSICAL_ADDRESS mRmpTableLimit = 0;
DXE_AMD_CCX_INIT_COMPLETE_PROTOCOL    mCcxDxeInitCompleteProtocol;
DXE_AMD_CCX_OC_COMPLETE_PROTOCOL      mCcxOcCompleteProtocol;
AP_MTRR_SETTINGS     ApMsrSettingsList[] =
{
  { 0xC0010058,               0x0000000000000000  },
  { AMD_AP_MTRR_FIX64k_00000, 0x0000000000000000  },
  { AMD_AP_MTRR_FIX16k_80000, 0x0000000000000000  },
  { AMD_AP_MTRR_FIX16k_A0000, 0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_C0000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_C8000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_D0000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_D8000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_E0000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_E8000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_F0000,  0x0000000000000000  },
  { AMD_AP_MTRR_FIX4k_F8000,  0x0000000000000000  },
  { 0x00000200,               0x0000000000000000  },
  { 0x00000201,               0x0000000000000000  },
  { 0x00000202,               0x0000000000000000  },
  { 0x00000203,               0x0000000000000000  },
  { 0x00000204,               0x0000000000000000  },
  { 0x00000205,               0x0000000000000000  },
  { 0x00000206,               0x0000000000000000  },
  { 0x00000207,               0x0000000000000000  },
  { 0x00000208,               0x0000000000000000  },
  { 0x00000209,               0x0000000000000000  },
  { 0x0000020A,               0x0000000000000000  },
  { 0x0000020B,               0x0000000000000000  },
  { 0x0000020C,               0x0000000000000000  },
  { 0x0000020D,               0x0000000000000000  },
  { 0x0000020E,               0x0000000000000000  },
  { 0x0000020F,               0x0000000000000000  },
  { 0x000002FF,               0x0000000000000000  },
  { CPU_LIST_TERMINAL                             }
};

UINT64 GdtEntries[] =
{
 0x0000000000000000,  // [00h] Null descriptor
 0x00CF92000000FFFF,  // [08h] Linear data segment descriptor
 0x00CF9A000000FFFF,  // [10h] Linear code segment descriptor
 0x00CF92000000FFFF,  // [18h] System data segment descriptor
 0x00CF9A000000FFFF,  // [20h] System code segment descriptor
 0x0000000000000000,  // [28h] Spare segment descriptor
 0x00CF93000000FFFF,  // [30h] System data segment descriptor
 0x00AF9B000000FFFF,  // [38h] System code segment descriptor
 0x0000000000000000   // [40h] Spare segment descriptor
};

UINT8 AsmNearJump[] =
{ 0x90,         // nop
  0xE9,         // near jmp
  0xFC, 0xFB
};

UINT8 ApStartupCode[] =
{
//[000] [001] [002]
  0xBE, 0xF4, 0xFF,                     // mov si, 0FFF4h
//[003]
  0x66,
//[004] [005] [006] [007]
  0x2E, 0x0F, 0x01, 0x14,               // lgdt       fword ptr cs:[si]
//[008] [009] [010]
  0x0F, 0x20, 0xC0,                     // mov        eax, cr0                    ; Get control register 0
//[011] [012] [013] [014]
  0x66, 0x83, 0xC8, 0x03,               // or         eax, 000000003h             ; Set PE bit (bit #0)
//[015] [016] [017]
  0x0F, 0x22, 0xC0,                     // mov        cr0, eax
//[018] [019] [020]
  0x0F, 0x20, 0xE0,                     // mov        eax, cr4
//[021] [022] [023] [024] [025] [026]
  0x66, 0x0D, 0x00, 0x06, 0x00, 0x00,   // or         eax, 00000600h
//[027] [028] [029]
  0x0F, 0x22, 0xE0,                     // mov        cr4, eax
  // Protected Mode Start
//[030] [031] [032]
  0xB8, 0x18, 0x00,                     // mov        ax,  18h
//[033] [034]
  0x8E, 0xD8,                           // mov        ds,  ax
//[035] [036]
  0x8E, 0xC0,                           // mov        es,  ax
//[037] [038]
  0x8E, 0xE0,                           // mov        fs,  ax
//[039] [040]
  0x8E, 0xE8,                           // mov        gs,  ax
//[041] [042]
  0x8E, 0xD0,                           // mov        ss,  ax
//[043] [044] [045]
  0x66, 0x67, 0xEA,                     // Far jump
//[046] [047] [048] [049]
  0x24, 0xFC, 0x00, 0x00,               // Byte [48], [49] will be replaced with segment from BIOS Directory
//[050] [051]
  0x10, 0x00,

  // Long Mode setup
//[052] [053] [054]
  0x0F, 0x20, 0xE0,                     // mov eax, cr4
//[055] [056] [057] [058]
  0x0F, 0xBA, 0xE8, 0x05,               // bts eax, 5                             ; Set PAE (bit #5)
//[059] [060] [061]
  0x0F, 0x22, 0xE0,                     // mov cr4, eax
//[062] [063] [064] [065] [066]
  0xB9, 0x80, 0x00, 0x00, 0xC0,         // mov ecx, 0C0000080h                    ; Read EFER MSR
//[067] [068]
  0x0F, 0x32,                           // rdmsr
//[069] [070] [071] [072]
  0x0F, 0xBA, 0xE8, 0x08,               // bts eax, 8                             ; Set LME (bit #8)
//[073] [074]
  0x0F, 0x30,                           // wrmsr

//[075] [076] [077] [078] [079]
  0xBE, 0xE8, 0xFF, 0x00, 0x00,         // mov esi, 0000ffE8
                                        // Byte [78] [79] will be replaced with segment from BIOS Directory
//[080] [081]
  0x8B, 0x0E,                           // mov ecx, [esi]
//[082] [083] [084]
  0x0F, 0x22, 0xD9,                     // mov cr3, ecx                           ; Load CR3 with value from BSP

//[085] [086] [087]
  0x0F, 0x20, 0xC0,                     // mov eax, cr0
//[088] [089] [090] [091]
  0x0F, 0xBA, 0xE8, 0x1F,               // bts eax, 31                            ; Set PG bit (bit #31)
//[092] [093] [094]
  0x0F, 0x22, 0xC0,                     // mov cr0, eax

//[095] [096]
  0x67, 0xEA,                           // Far jump to long mode
//[097] [098] [099] [100]
  0x57, 0xFC, 0x00, 0x00,               // Bytes [99] [100] will be replaced with segment from BIOS Directory
//[101] [102]
  0x38, 0x00,
//[103] [104]
  0x48, 0xB8,                           // mov rax, offset ApEntryInCOffset
//[105] [106] [107] [108]               // Bytes [105] - [112] will be replaced with offset to APEntryInCOffset
  0xFF, 0xFF, 0xFF, 0xFF,
//[109] [110] [111] [112]
  0xFF, 0xFF, 0xFF, 0xFF,
//[113] [114]
  0xFF, 0xE0                            // jmp rax
};

#define  ZEN3_CAC_WEIGHT_NUM  23
BOOLEAN  mCacWeightsValid = FALSE;
UINT64   mCacWeights[ZEN3_CAC_WEIGHT_NUM];

typedef struct {
  UINT8 *AsmNearJumpAddress;
  UINTN ApStartupOffset;
  UINTN Size;
} AP_ADDRESS_MAP;

VOID
ApGetStartupCodeInfo (
     OUT   AP_ADDRESS_MAP *AddressMap
  );
/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
EFIAPI
CcxZen3InitWithMpServices (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

VOID
EFIAPI
CcxZen3OcCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

VOID
EFIAPI
CcxZen3OcCallbackOnMpServices (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

VOID ApAsmCode (
  VOID
  );

VOID
RegSettingBeforeLaunchingNextThread (
  VOID
  );

VOID
ApEntryPointInC (
  VOID
  );

VOID
SetupApStartupRegion (
  );

VOID
RestoreResetVector (
  IN       UINT16 TotalCoresLaunched
  );

VOID
CcxZen3SetMiscMsrs (
  IN       VOID  *Void
  );

VOID
CcxZen3SyncMiscMsrs (
  IN       VOID  *Void
  );

VOID
CcxZen3EnableSmee (
  );

VOID
CcxZen3EnableWdt (
   IN VOID *Void
  );

VOID
CcxZen3SetCacWeights (
  );

VOID
CcxZen3Prefetcher (
  );

VOID
EFIAPI
CcxReadyToBoot (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

VOID
CcxCheckDownCoreStatus (
  );

VOID
EFIAPI
CcxCheckDownCoreStatusCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );

VOID
CcxZen3SetSnpRmp (
  );

VOID
CcxZen3InitSnpRmp (
  IN    UINT8     SnpMemCover
  );

VOID
SetRom3RegionToMmio (
  IN MTRR_INFO *MtrrInfo
  );

/* -----------------------------------------------------------------------------*/
/**
 *
 *  AmdCcxZen3DxeInit
 *
 *  @param[in]  ImageHandle     EFI Image Handle for the DXE driver
 *  @param[in]  SystemTable     Pointer to the EFI system table
 *
 *  Description:
 *    Zen3 Driver Entry.  Initialize the core complex.
 *
 *  @retval EFI_STATUS
 *
 */
EFI_STATUS
EFIAPI
AmdCcxZen3DxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  UINT16                                   CoreNumber;
  UINTN                                    Socket;
  UINTN                                    Die;
  UINTN                                    Ccd;
  UINTN                                    Ccx;
  UINTN                                    Core;
  UINTN                                    Thread;
  UINTN                                    NumberOfSockets;
  UINTN                                    NumberOfDies;
  UINTN                                    NumberOfCcds;
  UINTN                                    NumberOfComplexes;
  UINTN                                    NumberOfCores;
  UINTN                                    NumberOfThreads;
  UINTN                                    TotalCoresLaunched;
  EFI_STATUS                               Status;
  EFI_STATUS                               CalledStatus;
  AMD_CONFIG_PARAMS                        StdHeader;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL  *CoreTopology;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL  *FabricTopology;
  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL      *NbioSmuServices;
  EFI_HANDLE                               Handle;
  DXE_AMD_MP_SERVICES_PREREQ_PROTOCOL      AmdMpServicesPreReqProtocol;
  EFI_EVENT                                ReadyToBootEvent;
  ALLOCATE_HEAP_PARAMS                     AllocParams;
  PROFILE_FEATS                            PlatformProfile;
  PLATFORM_FEATS                           PlatformFeatures;
  EFI_EVENT                                EfiVariableWriteEvent;
  VOID                                    *RegistrationForEfiVariableWriteEvent;
  VOID                                    *IgnoredProtocol;
  UINT8                                    SnpMemCover;

  AGESA_TESTPOINT (TpCcxDxeEntry, NULL);

  IDS_HDT_CONSOLE (CPU_TRACE, "  AmdCcxZen3DxeInit Entry\n");

  Status = EFI_SUCCESS;

  if (CcxIsBsp (&StdHeader)) {

    // Check downcore status, and trigger a cold reset if needed
    CalledStatus = gBS->LocateProtocol (&gEfiVariableWriteArchProtocolGuid, NULL, &IgnoredProtocol);
    if (!EFI_ERROR (CalledStatus)) {
      CcxCheckDownCoreStatus ();
    } else {
      //
      // Set up call back after NV variable service are available.
      //
      CalledStatus = gBS->CreateEventEx (
                             EVT_NOTIFY_SIGNAL,
                             TPL_NOTIFY,
                             CcxCheckDownCoreStatusCallback,
                             NULL,
                             NULL,
                             &EfiVariableWriteEvent
                             );
      ASSERT (CalledStatus == EFI_SUCCESS);
      Status = (CalledStatus > Status) ? CalledStatus : Status;

      CalledStatus = gBS->RegisterProtocolNotify (
                            &gEfiVariableWriteArchProtocolGuid,
                            EfiVariableWriteEvent,
                            &(RegistrationForEfiVariableWriteEvent)
                            );
      ASSERT (CalledStatus == EFI_SUCCESS);
      Status = (CalledStatus > Status) ? CalledStatus : Status;
    }

    // Publish CCX services protocol
    CcxBaseServicesProtocolInstall (ImageHandle, SystemTable);

    // Get LogicalIds using the AmdTableLib functions so that the results are stored to
    // global variables for the APs to use
    GetSocLogicalIdOnCurrentCore (&StdHeader, &mLogicalId);
    GetCoreLogicalIdOnCurrentCore (&StdHeader, &mCoreLogicalId);

    // Get package type
    mPackageId = LibAmdGetPackageType (NULL);

    // Get PCD setting for CPU WDT
    mCpuWdtEn = PcdGetBool (PcdAmdCpuWdtEn);
    mCpuWdtTimeOut = PcdGet16 (PcdAmdCpuWdtTimeout);
    if (mCpuWdtTimeOut == 0xFFFF) {
      // If it's 'Auto', set timeout to 2.682s to make sure it's smaller than DF WDT
      mCpuWdtTimeOut = 0x100;
    }
    mCpuWdTmrCfgSeverity = PcdGet8 (PcdAmdCpuWdtSeverity);
    if ((mCpuWdTmrCfgSeverity != 0xFF) && (mCpuWdTmrCfgSeverity > 5)) {
      // 0 ~ 5 is valid
      mCpuWdTmrCfgSeverity = 0xFF;
    }

    // Load microcode on the BSP
    IDS_SKIP_HOOK (IDS_HOOK_CCX_SKIP_UCODE_PATCH, NULL, NULL) {
      CcxZen3MicrocodeInit (&mUcodePatchAddr, &StdHeader);
    }

    RegSettingBeforeLaunchingNextThread ();

    // L1, L2 HW Stream Prefetcher
    CcxZen3Prefetcher ();

    // if SNP is supported
    SnpMemCover = PcdGet8 (PcdAmdSnpMemCover);
    if (SnpMemCover > 0) {
      // Allocate & set RMP table
      CcxZen3InitSnpRmp (SnpMemCover);
      CcxZen3SetSnpRmp ();
    }

    if (gBS->LocateProtocol (&gAmdNbioSmuServicesProtocolGuid, NULL, &NbioSmuServices) == EFI_SUCCESS) {
      // Get CacWeights from SMU
      mCacWeightsValid = (BOOLEAN) (NbioSmuServices->AmdSmuReadCacWeights (NbioSmuServices, ZEN3_CAC_WEIGHT_NUM, mCacWeights) == EFI_SUCCESS);
    }

    CalledStatus = gBS->LocateProtocol (
                  &gAmdCoreTopologyServicesV2ProtocolGuid,
                  NULL,
                  &CoreTopology
                  );
    ASSERT (CalledStatus == EFI_SUCCESS);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    CalledStatus = gBS->LocateProtocol (
                  &gAmdFabricTopologyServices2ProtocolGuid,
                  NULL,
                  &FabricTopology
                  );
    ASSERT (CalledStatus == EFI_SUCCESS);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    if (AmdCapsuleGetStatus () == FALSE) {

      // Below 2 functions may be called by every threads in CcxProgramTablesAtReset
      // To avoid any UEFI services be called by APs, BSC calls them here
      // The result would be saved to a global variable
      GetPlatformFeatures (&StdHeader, &PlatformFeatures);
      GetPerformanceFeatures (&StdHeader, &PlatformProfile);

      // Setup reset vector with AP Start up code
      AGESA_TESTPOINT (TpCcxDxeStartLaunchAp, NULL);
      SetupApStartupRegion ();

      // Launch APs to get the APs to a known state before we publish
      // Ccx MP Services Protocol
      if (FabricTopology->GetSystemInfo (FabricTopology, &NumberOfSockets, NULL, NULL, NULL, NULL) != EFI_SUCCESS) {
        AGESA_TESTPOINT (TpCcxDxeLaunchApAbort, NULL);
        return EFI_DEVICE_ERROR;
      }
      TotalCoresLaunched = 0;
      CoreNumber = 0xFFFF;
      for (Socket = 0; Socket < NumberOfSockets; Socket++) {
        if (FabricTopology->GetProcessorInfo (FabricTopology, Socket, &NumberOfDies, NULL) != EFI_SUCCESS) {
          AGESA_TESTPOINT (TpCcxDxeLaunchApAbort, NULL);
          return EFI_DEVICE_ERROR;
        }
        for (Die = 0; Die < NumberOfDies; Die++) {
          if (CoreTopology->GetCoreTopologyOnDie (CoreTopology,
                                                  Socket,
                                                  Die,
                                                  &NumberOfCcds,
                                                  &NumberOfComplexes,
                                                  &NumberOfCores,
                                                  &NumberOfThreads) != EFI_SUCCESS) {
            AGESA_TESTPOINT (TpCcxDxeLaunchApAbort, NULL);
            return EFI_DEVICE_ERROR;
          }
          for (Ccd = 0; Ccd < NumberOfCcds; Ccd++) {
            for (Ccx = 0; Ccx < NumberOfComplexes; Ccx++) {
              for (Core = 0; Core < NumberOfCores; Core++) {
                for (Thread = 0; Thread < NumberOfThreads; Thread++) {
                  if (!((Socket == 0) && (Die == 0) && (Ccd == 0) && (Ccx == 0) && (Core == 0) && (Thread == 0))) {
                    IDS_HDT_CONSOLE (CPU_TRACE, "    Launch socket %X die %X ccd %X complex %X core %X thread %X\n",
                                                Socket,
                                                Die,
                                                Ccd,
                                                Ccx,
                                                Core,
                                                Thread);
                    CoreTopology->LaunchThread (CoreTopology, Socket, Die, Ccd, Ccx, Core, Thread);
                    TotalCoresLaunched++;
                    // Check whether the last core has completed necessory initialization before launching next thread
                    do {
                      gBS->CopyMem ((VOID *) &CoreNumber,
                                    (VOID *) (EFI_PHYSICAL_ADDRESS) (AllowToLaunchNextThreadLocation),
                                    sizeof (CoreNumber)
                                    );
                      IDS_HDT_CONSOLE (CPU_TRACE, "    Wait till core complete initialization, TotalCoresLaunched 0x%X, CoreNumber completed initialization 0x%X\n",
                                                  TotalCoresLaunched,
                                                  CoreNumber);
                    } while (CoreNumber != TotalCoresLaunched);
                    IDS_HDT_CONSOLE (CPU_TRACE, "    After core complete initialization\n");
                  }
                }
              }
            }
          }
        }
      }
      // Restore the data located at the reset vector
      RestoreResetVector ((UINT16) TotalCoresLaunched);
      AGESA_TESTPOINT (TpCcxDxeEndLaunchAp, NULL);
    }

    // Enable SMEE
    CcxZen3EnableSmee ();

    // Mca initialization
    CcxSetMca ();

    // Cac Weights initialization
    AGESA_TESTPOINT (TpCcxDxeCacWeights, NULL);
    CcxZen3SetCacWeights ();

    // Install gAmdMpServicesPreReqProtocolGuid protocol
    AmdMpServicesPreReqProtocol.Revision = AMD_MP_SERVICES_PREREQ_PROTOCOL_REVISION;
    Handle = NULL;
    CalledStatus = gBS->InstallProtocolInterface (
                            &Handle,
                            &gAmdMpServicesPreReqProtocolGuid,
                            EFI_NATIVE_INTERFACE,
                            &AmdMpServicesPreReqProtocol
                            );
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    //
    // Set up call back after MP services are available.
    //
    CalledStatus = gBS->CreateEventEx (
                             EVT_NOTIFY_SIGNAL,
                             TPL_NOTIFY,
                             CcxZen3InitWithMpServices,
                             NULL,
                             NULL,
                             &CcxZen3InitWithMpServicesEvent
                             );
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    CalledStatus = gBS->RegisterProtocolNotify (
                            &gEfiMpServiceProtocolGuid,
                            CcxZen3InitWithMpServicesEvent,
                            &(mRegistrationForCcxZen3InitWithMpServicesEvent)
                            );
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    //
    // Set up call back for OverClock.
    //
    CalledStatus = gBS->CreateEventEx (
                             EVT_NOTIFY_SIGNAL,
                             TPL_NOTIFY,
                             CcxZen3OcCallback,
                             NULL,
                             NULL,
                             &CcxZen3OcCallbackEvent
                             );
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    CalledStatus = gBS->RegisterProtocolNotify (
                            &gAmdNbioSmuInitCompleteProtocolGuid,
                            CcxZen3OcCallbackEvent,
                            &(mRegistrationForCcxZen3OcCallbackEvent)
                            );
    Status = (CalledStatus > Status) ? CalledStatus : Status;


    CalledStatus = gBS->CreateEventEx (
               EVT_NOTIFY_SIGNAL,
               TPL_NOTIFY,
               CcxReadyToBoot,
               NULL,
               &gEfiEventReadyToBootGuid,
               &ReadyToBootEvent
               );
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI CPU SSDT services protocol
    CalledStatus = CcxZen3AcpiCpuSsdtServicesProtocolInstall (ImageHandle, SystemTable);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI Ccx CRAT services protocol
    CalledStatus = CcxZen3CratServicesProtocolInstall (ImageHandle, SystemTable);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI Ccx SRAT services protocol
    CalledStatus = CcxZen3SratServicesProtocolInstall (ImageHandle, SystemTable);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI Ccx PCCT services protocol
     CalledStatus = CcxZen3PcctServicesProtocolInstall (ImageHandle, SystemTable);
     Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI Ccx CPPC services protocol
     CalledStatus = CcxZen3CppcServicesProtocolInstall (ImageHandle, SystemTable);
     Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI Ccx RAS services protocol
    CalledStatus = CcxZen3RasServicesProtocolInstall (ImageHandle, SystemTable);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish SMBIOS services protocol
    CalledStatus = CcxZen3SmbiosServicesProtocolInstall (ImageHandle, SystemTable);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Install gAmdCcxDxeInitCompleteProtocolGuid protocol
    mCcxDxeInitCompleteProtocol.Revision = AMD_CCX_PROTOCOL_REVISION;
    Handle = NULL;
    AGESA_TESTPOINT (TpCcxDxeBeforeInstallCcxDoneProtocol, NULL);
    CalledStatus = gBS->InstallProtocolInterface (
                            &Handle,
                            &gAmdCcxDxeInitCompleteProtocolGuid,
                            EFI_NATIVE_INTERFACE,
                            &mCcxDxeInitCompleteProtocol
                            );
    Status = (CalledStatus > Status) ? CalledStatus : Status;
    AGESA_TESTPOINT (TpCcxDxeAfterInstallCcxDoneProtocol, NULL);

    // Save PcdAmdCcxCfgPFEHEnable to heap so it could be gotten in SMI handler
    AllocParams.RequestedBufferSize = sizeof (BOOLEAN);
    AllocParams.BufferHandle = AMD_PFEH_HANDLE;
    AllocParams.Persist = HEAP_SYSTEM_MEM;

    if (HeapAllocateBuffer (&AllocParams, NULL) == AGESA_SUCCESS) {
      *((BOOLEAN *) AllocParams.BufferPtr) = PcdGetBool (PcdAmdCcxCfgPFEHEnable);
    }

    AGESA_TESTPOINT (TpCcxIdsAfterApLaunch, NULL);
    IDS_HOOK (IDS_HOOK_CCX_AFTER_AP_LAUNCH, NULL, NULL);
  }

  IDS_HDT_CONSOLE (CPU_TRACE, "  AmdCcxZen3DxeInit End\n");

  AGESA_TESTPOINT (TpCcxDxeExit, NULL);

  return (Status);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  SetupApStartupRegion
 *
 *
 *  Description:
 *    This routine sets up the necessary code and data to launch APs.
 *
 */
VOID
SetupApStartupRegion (
  VOID
  )
{
  UINT8             i;
  EFI_STATUS        Status;
  IA32_DESCRIPTOR   BspGdtr;
  TYPE_ATTRIB       TypeAttrib;
  UINT64            EntryAddress;
  UINT32            EntrySize;
  UINT64            EntryDest;
  UINT32            Segment;
  UINT32            C3Value;
  UINT8             ApcbPurpose;
  UINT32            Apcb32;
  UINT64            ApEntryInCOffset;
  AMD_APCB_SERVICE_PROTOCOL    *ApcbDxeServiceProtocol;

  C3Value = (UINT32) AsmReadCr3 ();

  if (BIOSEntryInfo (BIOS_FIRMWARE, INSTANCE_IGNORED, &TypeAttrib, &EntryAddress, &EntrySize, &EntryDest) == FALSE) {
   IDS_HDT_CONSOLE (CPU_TRACE, "Fail to get the correct Entry of BIOS firmware\n");
  }

  if (TypeAttrib.Copy == 0) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  First fetch is pointing to SPI\n");
    // Locate APCB service
    ApcbDxeServiceProtocol = NULL;
    Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &ApcbDxeServiceProtocol);

    // Get AP base address from APCB
    ApPageAllocation = 0x10000000;  // This is default value if there's no APCB APCB_TOKEN_UID_CPU_FETCH_FROM_SPI_AP_BASE
                                    // This value MUST match value of FETCH_FROM_SPI_AP_BASE which is defined in ABL

    if (ApcbDxeServiceProtocol != NULL) {
      if (ApcbDxeServiceProtocol->ApcbGetToken32 (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_CPU_FETCH_FROM_SPI_AP_BASE, &Apcb32) == EFI_SUCCESS) {
        ApPageAllocation = Apcb32 & 0xFFFF0000;
      }
    }
    Status = gBS->AllocatePages (AllocateAddress,
                                 EfiBootServicesData,
                                 EFI_SIZE_TO_PAGES (AP_ALLOCATION_SIZE),
                                 &ApPageAllocation
                                 );

    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE (CPU_TRACE, "  [ERROR] Try to allocate pages at 0x%x but failed. Please update APCB_TOKEN_UID_CPU_FETCH_FROM_SPI_AP_BASE\n", ApPageAllocation);
    }
    ASSERT (Status == EFI_SUCCESS); // If code hits this assert, you have to change APCB_TOKEN_UID_CPU_FETCH_FROM_SPI_AP_BASE
    EntryDest = ApPageAllocation;
    EntrySize = 0x100000;

    IDS_HDT_CONSOLE (CPU_TRACE, "  AP CS Base = 0x%x\n", ((UINT32) EntryDest + EntrySize - 0x10000));
  }

  Segment = ((UINT32) EntryDest + EntrySize - 0x10000);
  ApStartupVector = (EFI_PHYSICAL_ADDRESS) (((UINT32) EntryDest + EntrySize - 0x10000) + 0xFFF0);

  ApStartupCode[48] = (UINT8) ((Segment >> 16) & 0xFF);
  ApStartupCode[78] = (UINT8) ((Segment >> 16) & 0xFF);
  ApStartupCode[99] = (UINT8) ((Segment >> 16) & 0xFF);

  ApStartupCode[49] = (UINT8) ((Segment >> 24) & 0xFF);
  ApStartupCode[79] = (UINT8) ((Segment >> 24) & 0xFF);
  ApStartupCode[100] = (UINT8) ((Segment >> 24) & 0xFF);

  ApEntryInCOffset = (UINT64) &ApAsmCode;

  ApStartupCode[105] = (UINT8) (ApEntryInCOffset & 0xFF);
  ApStartupCode[106] = (UINT8) ((ApEntryInCOffset >> 8) & 0xFF);
  ApStartupCode[107] = (UINT8) ((ApEntryInCOffset >> 16) & 0xFF);
  ApStartupCode[108] = (UINT8) ((ApEntryInCOffset >> 24) & 0xFF);
  ApStartupCode[109] = (UINT8) ((ApEntryInCOffset >> 32) & 0xFF);
  ApStartupCode[110] = (UINT8) ((ApEntryInCOffset >> 40) & 0xFF);
  ApStartupCode[111] = (UINT8) ((ApEntryInCOffset >> 48) & 0xFF);
  ApStartupCode[112] = (UINT8) ((ApEntryInCOffset >> 56) & 0xFF);

  // Allocate some space for APs to use as stack space
  Status = gBS->AllocatePool (EfiBootServicesData,
                              (ZEN3_MAX_NUMBER_OF_APS * 0x400), // Allocate 1K for all possible cores
                              &ApStackBasePtr);

  // Allocate space to store data at reset vector
  ApTempBufferSize = 0x410;
  Status = gBS->AllocatePool (EfiBootServicesData,
                              ApTempBufferSize,
                              &MemoryContentCopy
                              );

  gBS->SetMem (MemoryContentCopy,
               ApTempBufferSize,
               0
               );

  // Copy data at reset vector to temporary buffer so we
  // can temporarily replace it with AP start up code.
  gBS->CopyMem (MemoryContentCopy,
                (VOID *) (ApStartupVector - 0x400),
                ApTempBufferSize
                );

  gBS->SetMem ((VOID *) (ApStartupVector - 0x400),
               ApTempBufferSize,
               0
               );


  // Copy AP start up code to Segment + 0xFBF0
  gBS->CopyMem ((VOID *) (ApStartupVector - 0x400),
               (VOID *) &ApStartupCode,
               sizeof (ApStartupCode)


               );

  // Save BSP's patch level so that AP can use it to determine whether microcode patch
  // loading should be skipped
  BspPatchLevel = AsmReadMsr64 (MSR_PATCH_LEVEL);

  // Save sleep type so that AP needn't to get it by running FchReadSleepType (),
  // otherwise, it would lead into a race condition.
  SleepType = FchReadSleepType ();

  // Sync Fixed-MTRRs with BSP
  AsmMsrOr64 (0xC0010010, BIT19);

  for (i = 0; ApMsrSettingsList[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
    ApMsrSettingsList[i].MsrData = AsmReadMsr64 (ApMsrSettingsList[i].MsrAddr);
  }

  // Some Fixed-MTRRs should be set according to PCDs
  UpdateApMtrrSettings (ApMsrSettingsList);

  AsmMsrAnd64 (0xC0010010, ~((UINT64) BIT19));

  // Copy BSP MSR values to Segment + 0xFF00
  ApSyncLocation = (UINT32) (ApStartupVector + 0xE);
  AllowToLaunchNextThreadLocation = (UINT32) (ApStartupVector + 0xC);
  BspMsrLocation = (UINT32) (ApStartupVector - 0x1C0);

  ASSERT (sizeof (ApMsrSettingsList) <= 0x1C0);
  gBS->CopyMem ((VOID *) ((ApStartupVector - 0x1C0)),
                (VOID *) &ApMsrSettingsList,
                sizeof (ApMsrSettingsList)
                );


  // Copy GDT Entries to Segment + 0xFFA0
  gBS->CopyMem ((VOID *) (ApStartupVector - 0x50),
                (VOID *) &GdtEntries,
                sizeof (GdtEntries)
                );

  BspGdtr.Limit = sizeof (GdtEntries) - 1;
  BspGdtr.Base = (UINTN) ApStartupVector - 0x50;

  // Copy pointer to GDT entries to Segment + 0xFFF4
  gBS->CopyMem ((VOID *) (ApStartupVector + sizeof (AsmNearJump)),
                (VOID *) &BspGdtr,
                sizeof (BspGdtr)
                );

  // Copy the near jump to AP startup code to reset vector. The near jump
  // forces execution to start from CS:FBF0
  gBS->CopyMem ((VOID *) ApStartupVector,
                (VOID *) AsmNearJump,
                sizeof (AsmNearJump)
                );

  // Copy the value of C3 to Segment + 0xFFE8
  gBS->CopyMem ((VOID *) (ApStartupVector - 0x08),
                (VOID *) &C3Value,
                sizeof (C3Value)
                );

  ApGdtDescriptor.Size = (sizeof (GdtEntries)) - 1;
  ApGdtDescriptor.Pointer = (UINT64) &GdtEntries[0];

  AsmWbinvd ();
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  RestoreResetVector
 *
 *  @param[in] TotalCoresLaunched      The number of cores that were launched by the BSC
 *
 *  Description:
 *    This routine restores the code in the AP reset vector once all the APs that
 *    were launched are done running AP code
 *
 */
VOID
RestoreResetVector (
  IN       UINT16 TotalCoresLaunched
  )
{
  UINT16  CoreNumber;

  CoreNumber = 0xFFFF;

  // Check whether the last core has completed running the AP Startup code
  do {
    gBS->CopyMem ((VOID *) &CoreNumber,
                  (VOID *) (EFI_PHYSICAL_ADDRESS) (ApSyncLocation),
                  sizeof (CoreNumber)
                  );
  } while (CoreNumber != TotalCoresLaunched);

  gBS->CopyMem ((VOID *) (ApStartupVector - 0x400),
                MemoryContentCopy,
                ApTempBufferSize
                );

  // Clean up memory allocations
  gBS->FreePool (MemoryContentCopy);

  if (ApPageAllocation != 0) {
    gBS->FreePages (ApPageAllocation, EFI_SIZE_TO_PAGES (AP_ALLOCATION_SIZE));
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  RegSettingBeforeLaunchingNextThread
 *
 *  Description:
 *    Necessary register setting before launching next thread
 *
 */
VOID
RegSettingBeforeLaunchingNextThread (
  VOID
  )
{
  AMD_CONFIG_PARAMS    StdHeader;
  CPUID_DATA           CpuId;
  BOOLEAN              IsSmtDisabled;

  AsmCpuidEx (
      0x8000001D,
      0,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );

  IsSmtDisabled = (BOOLEAN) (((CpuId.EAX_Reg >> 14) & 0xFFF) == 0);

  // Thread 0s are launched before thread 1s.  We want the last thread of a core to set the bit.
  // When SMT is disabled, thread 0 should set the bit.
  if (IsSmtDisabled || !CcxIsComputeUnitPrimary (&StdHeader)) {
    // MSRC001_1023[49, TwCfgCombineCr0Cd] = 1
    AsmMsrOr64 (MSR_TW_CFG, BIT49);
  }

  // Zen3 A0 only
  if ((((mCoreLogicalId.CoreFamily & AMD_CORE_FAMILY_19_ZEN3) != 0) &&
      ((mCoreLogicalId.CoreRevision & AMD_CORE_F19_ZEN3_A0) != 0))) {
    if (!CcxIsComputeUnitPrimary (&StdHeader)) {
      // MSR0000_0048[2] = 1
      AsmMsrOr64 (0x48, BIT2);
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  ApEntryPointInC
 *
 *  Description:
 *    This routine is the C entry point for APs and is called from ApAsmCode
 *
 */
VOID
ApEntryPointInC (
  VOID
  )
{
  AMD_CONFIG_PARAMS    StdHeader;

  ASSERT (mLogicalId.Family != 0);
  ASSERT (mLogicalId.Revision != 0);

  CcxProgramTablesAtReset (SleepType, &StdHeader);

  // Skip loading microcode patch on AP if BSP's patch level is 0.
  if (BspPatchLevel != 0) {
    // Using the address saved by BSP previously
    if (mUcodePatchAddr != 0) {
      if (CcxIsComputeUnitPrimary (&StdHeader)) {
        AsmWriteMsr64 (0xC0010020, mUcodePatchAddr);
      }
    }
  }

  // RMP table initialization
  if (mRmpTableLimit != 0) {
    CcxZen3SetSnpRmp ();
  }

  // Mca initialization
  CcxSetMca ();

  // Cac Weights initialization
  CcxZen3SetCacWeights ();

}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3InitWithMpServices
 *
 *  @param[in] Event        The event that invoked this routine
 *  @param[in] Context      Unused
 *
 *  Description:
 *    This routine runs necessary routines across all APs
 *
 */
VOID
EFIAPI
CcxZen3InitWithMpServices (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  UINTN                     i;
  EFI_STATUS                Status;
  AMD_CONFIG_PARAMS         StdHeader;
  EFI_MP_SERVICES_PROTOCOL  *MpServices;
  MTRR_INFO                 MtrrInfo;
  UINT64                    TOM2;

  AGESA_TESTPOINT (TpCcxDxeMpCallbackEntry, NULL);

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3InitWithMpServices Entry\n");

  CcxZen3InitializeC6 (&StdHeader);

  CcxZen3InitializePrefetchMode (&StdHeader);

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);
  ASSERT (!EFI_ERROR (Status));
  if (EFI_ERROR (Status)) {
    AGESA_TESTPOINT (TpCcxDxeMpCallbackAbort, NULL);
    return;
  }

  // Get PCD here for CcxZen3SetMiscMsrs
  // APs should avoid getting PCD
  mPcdAmdRedirectForReturnDis = PcdGet8 (PcdAmdRedirectForReturnDis);
  mPcdAmdOpcacheCtrl = PcdGet8 (PcdAmdOpcacheCtrl);
  mPcdAmdStreamingStoresCtrl = PcdGet8 (PcdAmdStreamingStoresCtrl);
  mPcdEnSpecStFill = PcdGetBool (PcdAmdEnSpecStFill);
  mPcdEnableFSRM = PcdGetBool (PcdAmdEnableFSRM);
  mPcdEnableERMS = PcdGetBool (PcdAmdEnableERMS);
  mPcdEnableRMSS = PcdGetBool (PcdAmdEnableRMSS);
  mPcdEnableSvmAVIC = PcdGetBool (PcdAmdEnableSvmAVIC);
  mPcdAmdIbsHardwareEn = PcdGetBool (PcdAmdIbsHardwareEn);

  MpServices->StartupAllAPs (
      MpServices,
      CcxZen3SetMiscMsrs,
      FALSE,
      NULL,
      0,
      NULL,
      NULL
  );
  CcxZen3SetMiscMsrs (NULL);

  // Enable watchdog timer
  MpServices->StartupAllAPs (
      MpServices,
      CcxZen3EnableWdt,
      FALSE,
      NULL,
      0,
      NULL,
      NULL
  );
  CcxZen3EnableWdt (NULL);

  for (i = 0; i < (sizeof (mLateMsrSyncTable) / sizeof (mLateMsrSyncTable[0])); i++) {
    mLateMsrSyncTable[i].MsrData = AsmReadMsr64 (mLateMsrSyncTable[i].MsrAddr);
  }
  MpServices->StartupAllAPs (
      MpServices,
      CcxZen3SyncMiscMsrs,
      FALSE,
      NULL,
      0,
      NULL,
      NULL
  );

  TOM2 = AsmReadMsr64 (0xC001001D);
  if (TOM2 >= 0xFD00000000) {
    // Decode FD_0000_0000 ~ FD_03FF_FFFF(64MB) as MMIO for ROM3
    MtrrInfo.MtrrIndex = 0;
    MtrrInfo.MtrrBase = 0x00FD00000000;
    MtrrInfo.MtrrMask = 0x00FFFC000800;
    MpServices->StartupAllAPs (
        MpServices,
        SetRom3RegionToMmio,
        FALSE,
        NULL,
        0,
        (VOID *) &MtrrInfo,
        NULL
    );
    SetRom3RegionToMmio (&MtrrInfo);
  }

  Status = CcxSmmAccess2ProtocolInstall ();
  if (Status != EFI_SUCCESS) {
    IDS_HDT_CONSOLE (CPU_TRACE, "   WARNING! CcxSmmAccess2ProtocolInstall was not installed\n");
  }

  IDS_HOOK (IDS_HOOK_CCX_AFTER_PWR_MNG, NULL, NULL);

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3InitWithMpServices Exit\n");

  AGESA_TESTPOINT (TpCcxDxeMpCallbackExit, NULL);
}

VOID
EFIAPI
CcxZen3OcCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  gBS->CreateEventEx (
                       EVT_NOTIFY_SIGNAL,
                       TPL_NOTIFY,
                       CcxZen3OcCallbackOnMpServices,
                       NULL,
                       NULL,
                       &CcxZen3OcCallbackOnMpServicesEvent
                       );

  gBS->RegisterProtocolNotify (
                      &gEfiMpServiceProtocolGuid,
                      CcxZen3OcCallbackOnMpServicesEvent,
                      &(mRegistrationForCcxZen3OcOnMpServicesCallbackEvent)
                      );

}

VOID
EFIAPI
CcxZen3OcCallbackOnMpServices (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_HANDLE Handle;

  AGESA_TESTPOINT (TpCcxDxeOcCallbackEntry, NULL);
  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3OcCallback Entry\n");

  CcxZen3InitializeCpb (NULL);

  IDS_HOOK (IDS_HOOK_CCX_CUSTOM_PSTATES, NULL, NULL);

  // Install gAmdCcxOcCompleteProtocolGuid protocol after custom Pstate feature
  mCcxOcCompleteProtocol.Revision = 0;
  Handle = NULL;
  gBS->InstallProtocolInterface (
           &Handle,
           &gAmdCcxOcCompleteProtocolGuid,
           EFI_NATIVE_INTERFACE,
           &mCcxOcCompleteProtocol
           );

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3OcCallback Exit\n");
  AGESA_TESTPOINT (TpCcxDxeOcCallbackExit, NULL);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3SetMiscMsrs
 *
 *  Description:
 *    This routine sets miscellaneous MSRs:
 *    - Forces TSC recalculation
 *    - MSRC001_1023[49, TwCfgCombineCr0Cd]
 *    - Applies miscellaneous settings controlled by PCDs
 *
 */
VOID
CcxZen3SetMiscMsrs (
  IN       VOID  *Void
  )
{
  UINT64  LocalMsrRegister;

  // Force recalc of TSC on all threads after loading patch
  LocalMsrRegister = AsmReadMsr64 (0xC0010064);
  AsmWriteMsr64 (0xC0010064, LocalMsrRegister);

  if (CcxIsComputeUnitPrimary (NULL)) {
    // MSRC001_1023[49, TwCfgCombineCr0Cd] = 1
    AsmMsrOr64 (0xC0011023, BIT49);
  }

  // MSR_C001_1029[14]
  if (mPcdAmdRedirectForReturnDis != 0xFF) {
    AsmMsrAndThenOr64 (0xC0011029, ~(UINT64) BIT14, LShiftU64 ((mPcdAmdRedirectForReturnDis & 1), 14));
  }

  // MSR_C001_1021[5]
  if (mPcdAmdOpcacheCtrl != 0xFF) {
    AsmMsrAndThenOr64 (0xC0011021, ~(UINT64) BIT5, LShiftU64 ((mPcdAmdOpcacheCtrl & 1), 5));
  }

  // MSR_C001_1020[28]
  if (mPcdAmdStreamingStoresCtrl != 0xFF) {
    AsmMsrAndThenOr64 (0xC0011020, ~(UINT64) BIT28, LShiftU64 ((mPcdAmdStreamingStoresCtrl & 1), 28));
  }

  // GN ONLY
  if (((mLogicalId.Family & AMD_FAMILY_GN) != 0) && ((mLogicalId.Revision & AMD_REV_F19_GN_ALL) != 0)) {
    // Reserved MSR_C001_10E5[26]
    AsmMsrAndThenOr64 (0xC00110E5, ~(UINT64) BIT26, ((mPcdEnSpecStFill) ? (UINT64) BIT26 : 0));

    // MSR_C001_10DD[13]
    if (mPcdEnableSvmAVIC) {
      AsmMsrOr64 (0xC00110DD, (UINT64) BIT13);
    } else {
      AsmMsrAnd64 (0xC00110DD, ~((UINT64) BIT13));
    }
  }

  // Zen3 Bx only
  if ((((mCoreLogicalId.CoreFamily & AMD_CORE_FAMILY_19_ZEN3) != 0) &&
      ((mCoreLogicalId.CoreRevision & AMD_CORE_F19_ZEN3_Bx) != 0))) {
    // MSR_C001_10DF[36]
    if (mPcdEnableFSRM) {
      AsmMsrOr64 (0xC00110DF, (UINT64) BIT36);
    } else {
      AsmMsrAnd64 (0xC00110DF, ~((UINT64) BIT36));
    }

    // MSR_C001_1002[9] : ERMS
    if (mPcdEnableERMS) {
      AsmMsrOr64 (0xC0011002, (UINT64) BIT9);
    } else {
      AsmMsrAnd64 (0xC0011002, ~((UINT64) BIT9));
    }

    // MSR_C001_1000[15]
    if (mPcdEnableRMSS) {
      AsmMsrOr64 (0xC0011000, (UINT64) BIT15);
    } else {
      AsmMsrAnd64 (0xC0011000, ~((UINT64) BIT15));
    }

    // MSR_C001_1020[54]
    if (mPcdAmdIbsHardwareEn) {
      AsmMsrOr64 (0xC0011020, (UINT64) BIT54);
    }
  }

}


/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3SyncMiscMsrs
 *
 *  Description:
 *    This routine synchronizes the MSRs in mLateMsrSyncTable across all APs
 *
 */
VOID
CcxZen3SyncMiscMsrs (
  IN       VOID  *Void
  )
{
  UINTN  i;

  for (i = 0; i < (sizeof (mLateMsrSyncTable) / sizeof (mLateMsrSyncTable[0])); i++) {
    AsmMsrAndThenOr64 (
        mLateMsrSyncTable[i].MsrAddr,
        ~(mLateMsrSyncTable[i].MsrMask),
        (mLateMsrSyncTable[i].MsrData & mLateMsrSyncTable[i].MsrMask)
        );
  }
}

UINT32 CpuWdtCountSelDecode [] =
{
  4095,
  2047,
  1023,
  511,
  255,
  127,
  63,
  31,
  8191,
  16383
};
#define NumberOfCpuWdtCountSel (sizeof (CpuWdtCountSelDecode) / sizeof (CpuWdtCountSelDecode[0]))
#define MinCpuWdtCountSel 7
#define MaxCpuWdtCountSel 9

UINT8 CpuWdtCountSelBumpUp [] =
{
  7,
  6,
  5,
  4,
  3,
  2,
  1,
  0,
  8,
  9
};

UINT64 CpuWdtTimeBaseDecode [] =
{
  1310000,
  1280
};
#define NumberOfCpuWdtTimeBase (sizeof (CpuWdtTimeBaseDecode) / sizeof (CpuWdtTimeBaseDecode[0]))
#define MinCpuWdtTimeBase 1
#define MaxCpuWdtTimeBase 0

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3EnableWdt
 *
 *  Description:
 *    This routine enables watchdog
 *
 */
VOID
CcxZen3EnableWdt (
  IN  VOID *Void
  )
{
  UINT8           BumpUpIndex;
  UINT16          CpuWdtTimeBase;
  UINT16          CpuWdtCountSel;
  UINT64          CpuWdtTime;
  UINT64          DfCcmWdtTime;
  CPU_WDT_CFG_MSR CpuWdtCfg;


  if ((CcxIsBsp (NULL)) && (FabricGetCcmWdtInfo (&DfCcmWdtTime)) && (mCpuWdtEn)) {
    CpuWdtTimeBase = mCpuWdtTimeOut & 0xFF;
    CpuWdtCountSel = (mCpuWdtTimeOut & 0xFF00) >> 8;
    ASSERT (CpuWdtTimeBase < NumberOfCpuWdtTimeBase);
    ASSERT (CpuWdtCountSel < NumberOfCpuWdtCountSel);

    // RESTRICTION: When both CPU WDT & DF WDT are enable, the DF CCM WDT timeout must be greater than the CPU WDT timeout limit
    CpuWdtTime = (UINT64) (CpuWdtTimeBaseDecode[CpuWdtTimeBase] * CpuWdtCountSelDecode[CpuWdtCountSel]);
    if (DfCcmWdtTime <= CpuWdtTime) {
      IDS_HDT_CONSOLE (CPU_TRACE, "  WARNING: CPU WDT (%ld.%ld S) is greater than or equal to the DF CCM WDT (%ld.%ld S)\n", (CpuWdtTime / 1000000000), (CpuWdtTime % 1000000000), (DfCcmWdtTime / 1000000000), (DfCcmWdtTime % 1000000000));
      IDS_HDT_CONSOLE (CPU_TRACE, "  Current CPU WDT setting: CpuWdtCountSel %X, CpuWdtTimeBase %X\n", CpuWdtCountSel, CpuWdtTimeBase);
      // slow down the CPU WDT to at least the value of the DF CCM WDT
      IDS_HDT_CONSOLE (CPU_TRACE, "  Try to slow down CPU WDT\n");
      mCpuWdtEn = FALSE; // Disable CPU WDT
      while ((CpuWdtCountSel != MinCpuWdtCountSel) || (CpuWdtTimeBase != MinCpuWdtTimeBase)) {
        if ((CpuWdtCountSel == MinCpuWdtCountSel) && (CpuWdtTimeBase != MinCpuWdtTimeBase)) {
          // Try to change TimeBase
          CpuWdtCountSel = MaxCpuWdtCountSel;
          CpuWdtTimeBase = MinCpuWdtTimeBase;
        } else {
          // Try to change CountSel
          for (BumpUpIndex = (NumberOfCpuWdtCountSel - 1); BumpUpIndex > 0; BumpUpIndex--) {
            if (CpuWdtCountSelBumpUp[BumpUpIndex] == CpuWdtCountSel) {
              CpuWdtCountSel = CpuWdtCountSelBumpUp[BumpUpIndex - 1];
              break;
            }
          }
        }
        CpuWdtTime = (UINT64) (CpuWdtTimeBaseDecode[CpuWdtTimeBase] * CpuWdtCountSelDecode[CpuWdtCountSel]);

        if (DfCcmWdtTime > CpuWdtTime) {
          // Slow down succeed, enable CPU WDT
          mCpuWdtEn = TRUE;
          mCpuWdtTimeOut = CpuWdtTimeBase | (CpuWdtCountSel << 8);
          IDS_HDT_CONSOLE (CPU_TRACE, "  Succeed! New CPU WDT %ld.%ld S\n", (CpuWdtTime / 1000000000), (CpuWdtTime % 1000000000));
          IDS_HDT_CONSOLE (CPU_TRACE, "  New CPU WDT setting: CpuWdtCountSel %X, CpuWdtTimeBase %X\n", CpuWdtCountSel, CpuWdtTimeBase);
          break;
        }
      }
      if (!mCpuWdtEn) {
        IDS_HDT_CONSOLE (CPU_TRACE, "  Failed, we would not enable CPU WDT\n");
      }
    }
  }

  CpuWdtCfg.Value = AsmReadMsr64 (0xC0010074);
  if (mCpuWdtEn) {
    CpuWdtCfg.Field.CpuWdTmrCfgEn = 1;
    CpuWdtCfg.Field.CpuWdTmrTimebaseSel = mCpuWdtTimeOut & 0xFF;
    CpuWdtCfg.Field.CpuWdTmrCfgCount = (mCpuWdtTimeOut & 0xFF00) >> 8;
    if (mCpuWdTmrCfgSeverity != 0xFF) {
      CpuWdtCfg.Field.CpuWdTmrCfgSeverity = mCpuWdTmrCfgSeverity;
    }
  } else {
    CpuWdtCfg.Field.CpuWdTmrCfgEn = 0;
    CpuWdtCfg.Field.CpuWdTmrTimebaseSel = 0;
    CpuWdtCfg.Field.CpuWdTmrCfgCount = 0;
  }
  AsmWriteMsr64 (0xC0010074, CpuWdtCfg.Value);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3EnableSmee
 *
 *  Description:
 *    This routine enables secure memory encryption
 *
 */
VOID
CcxZen3EnableSmee (
  )
{
  if (PcdGetBool (PcdAmdSmee)) {
    AsmMsrOr64 (0xC0010010, BIT23);
  }
}


VOID
CcxZen3Prefetcher (
  )
{
  // L1 Stream HW Prefetcher
  if (!PcdGetBool (PcdAmdL1StreamPrefetcher)) {
    // MSR C001_1022[16] = 1
    AsmMsrOr64 (0xC0011022, BIT16);
  }

  // L1 Stride Prefetcher
  if (!PcdGetBool (PcdAmdL1StridePrefetcher)) {
    // MSR C001_1022[17] = 1
    AsmMsrOr64 (0xC0011022, BIT17);
  }

  // L1 Region Prefetcher
  if (!PcdGetBool (PcdAmdL1RegionPrefetcher)) {
    // MSR C001_1022[18] = 1
    AsmMsrOr64 (0xC0011022, BIT18);
  }

  // L2 Stream Prefetcher
  if (!PcdGetBool (PcdAmdL2StreamPrefetcher)) {
    // MSR C001_102B[0] = 0
    AsmMsrAnd64 (0xC001102B, ~((UINT64)BIT0));
  }

  // L2 Up/Down Prefetcher
  if (!PcdGetBool (PcdAmdL2UpDownPrefetcher)) {
    // MSR C001_102B[2] = 0
    AsmMsrAnd64 (0xC001102B, ~((UINT64)BIT2));
  }

}

/*---------------------------------------------------------------------------------------*/
/**
 * CcxReadyToBoot
 *
 * Calls CcxReadyToBoot
 *
 *  Parameters:
 *    @param[in]     Event
 *    @param[in]     *Context
 *
 *    @retval        VOID
 */
VOID
EFIAPI
CcxReadyToBoot (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS                   Status;
  EFI_SMM_CONTROL2_PROTOCOL    *SmmControl;
  UINT8                        SmiDataValue;

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxReadyToBoot");
  AGESA_TESTPOINT (TpCcxDxeRtbCallBackEntry, NULL);

  Status = gBS->LocateProtocol (
                  &gEfiSmmControl2ProtocolGuid,
                  NULL,
                  (VOID **)&SmmControl
                  );

  if (EFI_ERROR (Status)) {
    return;
  }

  SmiDataValue = PcdGet8 (PcdAmdCcxS3SaveSmi);

  SmmControl->Trigger (
                    SmmControl,
                    &SmiDataValue,
                    NULL,
                    0,
                    0
                    );

  gBS->CloseEvent (Event);

  IDS_HOOK (IDS_HOOK_CCX_READY_TO_BOOT, NULL, NULL);

  AGESA_TESTPOINT (TpCcxDxeRtbCallBackExit, NULL);
}

VOID
EFIAPI
CcxCheckDownCoreStatusCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  CcxCheckDownCoreStatus ();
  gBS->CloseEvent (Event);

}

VOID
CcxCheckDownCoreStatus (
  )
{
  BOOLEAN          AgesaDownCored;
  BOOLEAN          NvDownCored;
  UINTN            VariableSize;
  EFI_STATUS       Status;
  LOCATE_HEAP_PTR  LocateHeapParams;

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxCheckDownCoreStatus Entry\n");

  LocateHeapParams.BufferHandle = AMD_DOWN_CORE_STATUS_HANDLE;
  AgesaDownCored = (BOOLEAN) (HeapLocateBuffer (&LocateHeapParams, NULL) == AGESA_SUCCESS);

  NvDownCored = FALSE;
  VariableSize = sizeof (BOOLEAN);
  Status = gRT->GetVariable (
                  L"DownCoreStatus",
                  &gAmdDownCoreStatusGuid,
                  NULL,
                  &VariableSize,
                  (VOID *) &NvDownCored
                  );
  if ((AgesaDownCored != NvDownCored) || (Status != EFI_SUCCESS)) {
    VariableSize = sizeof (BOOLEAN);
    Status = gRT->SetVariable (
                    L"DownCoreStatus",
                    &gAmdDownCoreStatusGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    VariableSize,
                    (VOID *) &AgesaDownCored
                    );
    if (NvDownCored && !AgesaDownCored) {
      IDS_HDT_CONSOLE (CPU_TRACE, "  downcored -> auto, issuing cold reset\n");
      gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
    }
  }

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxCheckDownCoreStatus Exit\n");
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3SetCacWeights
 *
 *  Description:
 *    This routine sets all CAC weights
 *
 */
VOID
CcxZen3SetCacWeights (
  )
{
  UINT8  WeightIndex;
  UINT64 LocalMsr;

  if (CcxIsComputeUnitPrimary (NULL)) {
    LocalMsr = AsmReadMsr64 (0xC0011074);
    AsmWriteMsr64 (0xC0011074, 0);

    if (mCacWeightsValid) {
      for (WeightIndex = 0; WeightIndex < ZEN3_CAC_WEIGHT_NUM; WeightIndex++) {
        AsmWriteMsr64 (0xC0011076, WeightIndex);
        AsmWriteMsr64 (0xC0011077, mCacWeights[WeightIndex]);
      }
    }

    AsmWriteMsr64 (0xC0011074, (LocalMsr | BIT63));
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3SetSnpRmp
 *
 *  Description:
 *    This routine sets Reverse Map Table (RMP) base and limit
 *
 */
VOID
CcxZen3SetSnpRmp (
  )
{
  UINT64    MsrData;

  MsrData = AsmReadMsr64 (0xC0010132) & 0xFFF0000000001FFFull;
  MsrData |= mRmpTableBase;
  AsmWriteMsr64 (0xC0010132, MsrData);

  MsrData = AsmReadMsr64 (0xC0010133) & 0xFFF0000000001FFFull;
  MsrData |= mRmpTableLimit;
  AsmWriteMsr64 (0xC0010133, MsrData);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3InitSnpRmp
 *
 *  Description:
 *    This routine allocates the Secure Nested Paging (SNP)
 *    Reverse Map Table (RMP)
 *
 */
VOID
CcxZen3InitSnpRmp (
  UINT8   SnpMemCover
  )
{
  EFI_STATUS              Status;
  UINT8                   BytesPer4kPage;
  UINT64                  SnpMemSizeToCover;
  UINT64                  RmpTableSize;
  EFI_PHYSICAL_ADDRESS    RmpTableBase;

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3InitSnpRmp Entry\n");

  // init variables
  Status = EFI_SUCCESS;
  BytesPer4kPage = 8;
  SnpMemSizeToCover = 0;
  RmpTableSize = 0;
  RmpTableBase = 0;

  // if VMPL enabled
  if (PcdGetBool (PcdAmdVmplEnable)) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  VMPL Enabled\n");
    AsmMsrOr64 (0xC0010010, BIT25);
    BytesPer4kPage = 16;
  }

  // use TOM2 for total memory to cover
  SnpMemSizeToCover = AsmReadMsr64 (0xC001001D);

  // if custom memory size, use specified size
  if (SnpMemCover == 2) {
    // PcdAmdSnpMemSize is in MB, so convert to bytes
    SnpMemSizeToCover = ((UINT64) PcdGet32 (PcdAmdSnpMemSize)) << 20;
  }

  // 16KB + ((Bytes/4K-page) * (# of pages [SnpMemSizeToCover/4K]))
  RmpTableSize = SIZE_16KB + ((UINT64) BytesPer4kPage * (SnpMemSizeToCover >> 12));

  // round RMP table size up to nearest 1MB
  RmpTableSize = (RmpTableSize + SIZE_1MB - 1) & ~(SIZE_1MB - 1);

  // find enough memory for RMP table to fit on MB boundary
  Status = gBS->AllocatePages (AllocateAnyPages,
                               EfiReservedMemoryType,
                               EFI_SIZE_TO_PAGES (RmpTableSize + SIZE_1MB),
                               &RmpTableBase
                               );
  ASSERT (!EFI_ERROR (Status));
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  [ERROR] Failed to allocate RMP Table.\n");
    return;
  }

  // align on MB boundary
  mRmpTableBase = (RmpTableBase + SIZE_1MB - 1) & ~(SIZE_1MB - 1);

  // free pages before reallocating on MB boundary
  gBS->FreePages (RmpTableBase, EFI_SIZE_TO_PAGES (RmpTableSize + SIZE_1MB));

  // reserve memory for RMP Table on MB boundary
  Status = gBS->AllocatePages (AllocateAddress,
                               EfiReservedMemoryType,
                               EFI_SIZE_TO_PAGES (RmpTableSize),
                               &mRmpTableBase
                               );
  ASSERT (!EFI_ERROR (Status));
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  [ERROR] Try to allocate RMP Table at 0x%x but failed.\n", mRmpTableBase);
    return;
  }

  // zero out RMP table
  gBS->SetMem ((VOID *) mRmpTableBase,
               RmpTableSize,
               0
               );

  // save RMP limit
  mRmpTableLimit = mRmpTableBase + RmpTableSize - 1;

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3InitSnpRmp Exit\n");
}

VOID
SetRom3RegionToMmio (
  IN MTRR_INFO *MtrrInfo
  )
{
  AsmWriteMsr64 (((MtrrInfo->MtrrIndex << 1) + 0xC0010016), MtrrInfo->MtrrBase);
  AsmWriteMsr64 (((MtrrInfo->MtrrIndex << 1) + 0xC0010017), MtrrInfo->MtrrMask);
  AsmWbinvd ();
}
