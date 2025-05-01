/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * CCX down core and SMT initialization
 *
 * This function reduces the number of threads in the system, if desired.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CCX
 * @e \$Revision$   @e \$Date$
 *
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

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Porting.h>
#include <AMD.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/CcxDownCoreLib.h>
#include <Library/ApobCommonServiceLib.h>
#include <Library/MemRestore.h>
#include <Library/AmdHeapLib.h>
#include <Library/CcxApicLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdTableLib.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Ppi/FabricTopologyServices2Ppi.h>
#include <Ppi/AmdCoreTopologyServicesV2Ppi.h>
#include <Ppi/AmdErrorLogPpi.h>
#include <Ppi/Reset2.h>
#include <Ppi/SocZen3ServicesPpi.h>
#include <Filecode.h>
#include "CcxZen3DownCoreInit.h"
#include <CcxRegistersZen3.h>
#include <Addendum/Apcb/Inc/SSP/APOB.h>
#include <Library/AmdPspApobLib.h>

#define FILECODE CCX_ZEN3_PEI_CCXZEN3DOWNCOREINIT_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
CcxZen3DownCoreAfterSmuServicesCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

UINT32
GetSetBitCount (
  IN       UINT32 Value
  );

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
EFI_PEI_NOTIFY_DESCRIPTOR   mAfterSmuServicesPpiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdNbioSmuServicesPpiGuid,
  CcxZen3DownCoreAfterSmuServicesCallback
};
extern EFI_GUID gEfiAmdAgesaSpecificWarmResetGuid;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
#define MAX_CCDS_PER_SKT 8
#define DOWN_CORE_RESET_DATA_SIZE  (sizeof(CHAR16) * 32 + sizeof(EFI_GUID))
/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3DownCoreInit
 *
 *  Description:
 *    This function initializes the software downcoring feature.
 *
 */
VOID
CcxZen3DownCoreInit (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  if (PcdGetBool (PcdAmdNewDownCoreSequence)) {
    IDS_HDT_CONSOLE (CPU_TRACE, "Support new down core sequence, skip AGESA down core function.\n");
  } else {
    (**PeiServices).NotifyPpi (PeiServices, &mAfterSmuServicesPpiCallback);
  }
}

#define ABL_CCD_BIST_FAILURE 0x4065

/* -----------------------------------------------------------------------------*/
/**
 *
 *  IsCcdBistFailure
 *
 *  Description:
 *    This function checks if there is a CCD BIST Failure and then
 *    skip down core related code
 *
 *
 */
BOOLEAN
EFIAPI
IsCcdBistFailure (
    IN  EFI_PEI_SERVICES                **PeiServices
)
{
  EFI_STATUS                              Status;
  UINTN                                   SystemDieCount, DieCount, SocketCount, CcdCount, ComplexCount, CoreCount, ThreadCount;
  UINT32                                  Socket, Die, Ccd, Core, SystemDieLoop, BistData, CcdBistMap, CoreDisFuse, CcdDownFuse, CcdPresentFuse;
  BOOLEAN                                 SmtEnabledByFuse, CcdBistFailure = FALSE;
  APOB_APCB_BOOT_INFO_STRUCT              *ApobEntry = NULL;
  PEI_AMD_NBIO_SMU_SERVICES_PPI           *NbioSmuServices;
  AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI   *FabricTopologyServices;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PPI       *CoreTopologyServices;
  AMD_PEI_SOC_ZEN3_SERVICES_PPI           *SocZen3Services;
  PEI_AMD_ERROR_LOG_PPI                   *AmdErrorLog;

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_APCB_BOOT_INFO_TYPE, 0, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if(EFI_ERROR(Status)) return FALSE;

  IDS_HDT_CONSOLE (CPU_TRACE, "APCB_TOKEN_UID_ACTION_ON_BIST_FAILURE = %x\n", ApobEntry->ActionOnBistFailure);

  if(ApobEntry->ActionOnBistFailure == 0) return FALSE;

  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioSmuServicesPpiGuid, 0, NULL, &NbioSmuServices);
  if(EFI_ERROR(Status)) return FALSE;

  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdFabricTopologyServices2PpiGuid, 0, NULL, &FabricTopologyServices);
  if(EFI_ERROR(Status)) return FALSE;

  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdCoreTopologyServicesV2PpiGuid, 0, NULL, &CoreTopologyServices);
  if(EFI_ERROR(Status)) return FALSE;

  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdSocZen3ServicesPpiGuid, 0, NULL, &SocZen3Services);
  if(EFI_ERROR(Status)) return FALSE;

  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdErrorLogPpiGuid, 0, NULL, &AmdErrorLog);
  if(EFI_ERROR(Status)) return FALSE;

  FabricTopologyServices->GetSystemInfo (&SocketCount, &SystemDieCount, NULL, NULL, NULL);

  SystemDieLoop = 0;
  for (Socket = 0; Socket < SocketCount; Socket++) {
    FabricTopologyServices->GetProcessorInfo (Socket, &DieCount, NULL);
    for (Die = 0; Die < DieCount; Die++) {
      CoreTopologyServices->GetCoreTopologyOnDie (Socket, Die, &CcdCount, &ComplexCount, &CoreCount, &ThreadCount);
      IDS_HDT_CONSOLE (CPU_TRACE, "Socket = %d, Die = %d, CcdCount = %d, ComplexCount = %d, CoreCount = %d, ThreadCount = %d\n", Socket, Die, CcdCount, ComplexCount, CoreCount, ThreadCount);
      SocZen3Services->GetOpnCorePresence (PeiServices, SystemDieLoop, &CcdPresentFuse, &CcdDownFuse, &CoreDisFuse, &SmtEnabledByFuse);
      IDS_HDT_CONSOLE (CPU_TRACE, "Socket = %d, Die = %d, CcdPresentFuse = %x, CcdDownFuse = %x, CoreDisFuse = %x, SmtEnabledByFuse = %x\n", Socket, Die, CcdPresentFuse, CcdDownFuse, CoreDisFuse, SmtEnabledByFuse);
      CcdBistMap = 0;
      for (Ccd = 0; Ccd < CCD_MAX_CCDS_PER_DIE; Ccd++) {
        if(CcdPresentFuse & (1 << Ccd)){
          Status = NbioSmuServices->SmuReadBistInfo ((UINT8)SystemDieLoop, (UINT8)Ccd, &BistData);
          IDS_HDT_CONSOLE (CPU_TRACE, "Socket = %d, Die = %d, CCD %d BIST DATA = %08x\n", Socket, Die, Ccd, BistData);  //Upper 16 bits: Core Bist Result, Lower 16 bits: L3 Bist Result
          for (Core = 0; Core < CoreCount; Core++){
            if(((CoreDisFuse & (1 << Core)) == 0) && (((BistData >> 16) & (1 << Core)) == 0)){
              IDS_HDT_CONSOLE (CPU_TRACE, "Socket = %d, Die = %d, CCD %d, Core %d BIST failure!\n", Socket, Die, Ccd, Core);
              CcdBistFailure = TRUE;
              CcdBistMap |= (1 << Ccd);
            }
          }
          if((BistData & 0x01) == 0){
            IDS_HDT_CONSOLE (CPU_TRACE, "Socket = %d, Die = %d, CCD %d, L3 BIST failure!\n", Socket, Die, Ccd);
            CcdBistFailure = TRUE;
            CcdBistMap |= (1 << Ccd);
          }
        }
      }
      if(CcdBistMap){
        IDS_HDT_CONSOLE (CPU_TRACE, "Socket = %d, Die = %d, CcdBistMap = %x\n", Socket, Die, CcdBistMap);
        AmdErrorLog->AmdErrorLogPei (AmdErrorLog,
                                      AGESA_ALERT,
                                      ABL_CCD_BIST_FAILURE,
                                      (Socket << 16) + (Die << 8) + CcdBistMap,
                                      0x00000000,
                                      0x00000000,
                                      0x00000000
                                      );
      }
      SystemDieLoop++;
    }
  }
  return CcdBistFailure;
}

EFI_STATUS
EFIAPI
CcxZen3DownCoreAfterSmuServicesCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  UINTN                                  Socket;
  UINTN                                  Die;
  UINTN                                  CcdCount;
  UINTN                                  ComplexCount;
  UINTN                                  CoreCount;
  UINTN                                  FusedCoreCount;
  UINTN                                  ThreadCount;
  UINTN                                  DieCount;
  UINTN                                  SystemDieCount;
  UINTN                                  SocketCount;
  UINTN                                  DesiredCcdCount;
  UINTN                                  DesiredCoreCount;
  UINT32                                 DownCoreBitMap[MAX_CCDS_PER_SKT];
  UINT32                                 CoreDisByBitMap;
  UINT32                                 SystemDieLoop;
  UINT32                                 CoreDisByFuseCount;
  UINT32                                 CcdPresentByFuseCount;
  UINT32                                 CcdDownByFuseCount;
  UINT32                                 OpnCoreCount;
  UINT32                                 CoreDisMask;
  UINT32                                 CcdDisMask;
  UINT32                                 CcdLoop;
  UINT32                                 DisMask;
  UINT32                                 ApobInstanceId;
  UINT32                                 SmuArg[6];
  UINT32                                 ThrdPerCoreFuse;
  UINT32                                 CoreDisFuse;
  UINT32                                 CcdDownFuse;
  UINT32                                 CcdPresentFuse;
  UINT32                                 Ccd0SwDowncoreAddr;
  EFI_STATUS                             Status;
  EFI_STATUS                             CalledStatus;
  PEI_AMD_NBIO_SMU_SERVICES_PPI         *NbioSmuServices;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PPI     *CoreTopologyServices;
  AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI *FabricTopologyServices;
  PEI_AMD_ERROR_LOG_PPI                 *AmdErrorLog;
  EFI_PEI_RESET2_PPI                    *Reset2;
  AMD_PEI_SOC_ZEN3_SERVICES_PPI         *SocZen3Services;
  UINT8                                  CcdMode;
  UINT8                                  DownCoreMode;
  UINT8                                  SmtMode;
  UINT8                                  ApicMode;
  UINT8                                  PhysCcdNumber[MAX_SOCKETS_SUPPORTED][MAX_DIES_PER_SOCKET][MAX_CCDS_PER_SKT];
  BOOLEAN                                CcdAttainable;
  BOOLEAN                                CoreAttainable;
  BOOLEAN                                IssueReset;
  BOOLEAN                                AlreadyDownCored;
  BOOLEAN                                Met;
  BOOLEAN                                SmtEnabledByFuse;
  BOOLEAN                                GameMode;
  BOOLEAN                                IsVmr;
  ALLOCATE_HEAP_PARAMS                   AllocParams;
  UINTN                                  Size;
  UINT8                                  DownCoreResetData[DOWN_CORE_RESET_DATA_SIZE];
  AGESA_STATUS                           AgesaStatus;
  SOC_LOGICAL_ID                         SocLogicalId;

  AGESA_TESTPOINT (TpCcxPeiDownCoreCallbackEntry, NULL);

  Status = EFI_SUCCESS;
  IssueReset = FALSE;
  CcdDisMask = 0;
  CoreDisMask = 0;
  DesiredCcdCount = 0xFFFFFFFF;
  DesiredCoreCount = 0xFFFFFFFF;
  ThrdPerCoreFuse = 2;
  ThreadCount = ThrdPerCoreFuse;
  AlreadyDownCored = FALSE;
  Met = TRUE;
  IsVmr = FALSE;
  LibAmdMemFill (DownCoreBitMap, 0x00, sizeof (DownCoreBitMap), NULL);
  LibAmdMemFill (PhysCcdNumber, 0xFF, sizeof (PhysCcdNumber), NULL);
  LibAmdMemFill (SmuArg, 0x00, sizeof (SmuArg), NULL);

  GetSocLogicalIdOnCurrentCore (NULL, &SocLogicalId);
  if ((SocLogicalId.Family & AMD_FAMILY_19_VMR) != 0) {
    IsVmr = TRUE;
  }

  CcdMode = PcdGet8 (PcdAmdCcdMode);
  DownCoreMode = PcdGet8 (PcdAmdDownCoreMode);
  SmtMode = PcdGet8 (PcdAmdSmtMode);
  ApicMode = PcdGet8 (PcdAmdApicMode);
  ASSERT (CcdMode < CCD_MODE_MAX);
  ASSERT (DownCoreMode < CCX_DOWN_CORE_MAX);
  ASSERT (SmtMode <= 1);

  CalledStatus = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioSmuServicesPpiGuid, 0, NULL, &NbioSmuServices);
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  CalledStatus = (*PeiServices)->LocatePpi (PeiServices, &gAmdFabricTopologyServices2PpiGuid, 0, NULL, &FabricTopologyServices);
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  CalledStatus = (*PeiServices)->LocatePpi (PeiServices, &gAmdCoreTopologyServicesV2PpiGuid, 0, NULL, &CoreTopologyServices);
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  CalledStatus = (*PeiServices)->LocatePpi (PeiServices, &gAmdErrorLogPpiGuid, 0, NULL, &AmdErrorLog);
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  CalledStatus = (*PeiServices)->LocatePpi (PeiServices, &gAmdSocZen3ServicesPpiGuid, 0, NULL, &SocZen3Services);
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  ASSERT (Status == EFI_SUCCESS);

  if(IsCcdBistFailure(PeiServices)){
    return Status;
  }

  if ((CcdMode != CCD_MODE_AUTO) && !SocZen3Services->SocDoesNotHavePhysicalCcds) {
    switch (CcdMode) {
    case CCD_MODE_6_CCDS:
      DesiredCcdCount = 6;
      CcdDisMask = 0x0028;
      break;
    case CCD_MODE_4_CCDS:
      DesiredCcdCount = 4;
      CcdDisMask = 0x00AA;
      break;
    case CCD_MODE_3_CCDS:
      DesiredCcdCount = 3;
      CcdDisMask = 0x00AB;
      break;
    case CCD_MODE_2_CCDS:
      DesiredCcdCount = 2;
      CcdDisMask = 0x00EB;
      break;
    case CCD_MODE_1_CCD:
      if (SocZen3Services->IsOneCcdModeSupported (PeiServices)) {
        DesiredCcdCount = 1;
        CcdDisMask = 0x00FE;
      }
      break;
    default:
      break;
    }

    if (DesiredCcdCount == 0xFFFFFFFF) {
      AmdErrorLog->AmdErrorLogPei (AmdErrorLog,
                                    AGESA_ALERT,
                                    CPU_EVENT_DOWN_CORE_FAILURE,
                                    CCD_INVALID_SELECTION_FAILURE,
                                    0x00000000,
                                    0x00000000,
                                    0x00000000
                                    );
    }
  }


  GameMode = FALSE;
  if ((!SocZen3Services->SocDoesNotHavePhysicalCcds) && (SocZen3Services->IsGameModeSupported (PeiServices)) && (PcdGetBool (PcdAmdGameMode))) {
    DesiredCcdCount = 1;
    CcdDisMask = 0x00FB;
    GameMode = TRUE;
  }

  if (DownCoreMode != CCX_DOWN_CORE_AUTO) {
    switch (DownCoreMode) {
    case CCX_DOWN_CORE_1_0:
      DesiredCoreCount = 1;
      CoreDisMask = 0x000000FE;
      break;
    case CCX_DOWN_CORE_2_0:
      DesiredCoreCount = 2;
      CoreDisMask = 0x000000FC;
      break;
    case CCX_DOWN_CORE_3_0:
      DesiredCoreCount = 3;
      CoreDisMask = 0x000000F8;
      break;
    case CCX_DOWN_CORE_4_0:
      DesiredCoreCount = 4;
      CoreDisMask = 0x000000F0;
      break;
    case CCX_DOWN_CORE_5_0:
      DesiredCoreCount = 5;
      CoreDisMask = 0x000000E0;
      break;
    case CCX_DOWN_CORE_6_0:
      DesiredCoreCount = 6;
      CoreDisMask = 0x000000C0;
      break;
    case CCX_DOWN_CORE_7_0:
      DesiredCoreCount = 7;
      CoreDisMask = 0x00000080;
      break;
    default:
      break;
    }

    if (DesiredCoreCount == 0xFFFFFFFF) {
      AmdErrorLog->AmdErrorLogPei (AmdErrorLog,
                                    AGESA_ALERT,
                                    CPU_EVENT_DOWN_CORE_FAILURE,
                                    CORE_INVALID_SELECTION_FAILURE,
                                    0x00000000,
                                    0x00000000,
                                    0x00000000
                                    );
    }
  }

  CcdAttainable = TRUE;
  CoreAttainable = TRUE;
  SystemDieLoop = 0;
  AllocParams.BufferPtr = NULL;

  FabricTopologyServices->GetSystemInfo (&SocketCount, &SystemDieCount, NULL, NULL, NULL);

  for (Socket = 0; Socket < SocketCount; Socket++) {
    FabricTopologyServices->GetProcessorInfo (Socket, &DieCount, NULL);
    for (Die = 0; Die < DieCount; Die++) {
      CcdDownByFuseCount = 0;
      CcdPresentByFuseCount = 0;
      CoreDisByFuseCount = 0;

      // Get actual topology
      CoreTopologyServices->GetCoreTopologyOnDie (Socket, Die, &CcdCount, &ComplexCount, &CoreCount, &ThreadCount);

      // Get PhysCcdNumber
      ApobInstanceId = MAKE_APOB_INSTANCE_ID (Socket, Die);
      for (CcdLoop = 0; CcdLoop < CcdCount; CcdLoop++) {
        ApobGetPhysCcdNumber (ApobInstanceId, CcdLoop, &PhysCcdNumber[Socket][Die][CcdLoop]);
      }

      // Get CCD_Down, CCD_Present, and a valid core_disable fuse
      SocZen3Services->GetOpnCorePresence (PeiServices, SystemDieLoop, &CcdPresentFuse, &CcdDownFuse, &CoreDisFuse, &SmtEnabledByFuse);

      CcdDownByFuseCount = GetSetBitCount (CcdDownFuse & CcdPresentFuse);
      CcdPresentByFuseCount = GetSetBitCount (CcdPresentFuse);
      CoreDisByFuseCount = GetSetBitCount (CoreDisFuse);
      FusedCoreCount = 8 - CoreDisByFuseCount;
      ThrdPerCoreFuse = SmtEnabledByFuse ? 2 : 1;

      IDS_HDT_CONSOLE (CPU_TRACE, "  CcdDisMask %X CoreDisMask %X\n", CcdDisMask, CoreDisMask);
      IDS_HDT_CONSOLE (CPU_TRACE, "  CoreDisByFuseCount %X CcdDownByFuseCount %X CcdPresentByFuseCount %X\n", CoreDisByFuseCount, CcdDownByFuseCount, CcdPresentByFuseCount);
      IDS_HDT_CONSOLE (CPU_TRACE, "  CcdCount %X ComplexCount %X CoreCount %X\n", CcdCount, ComplexCount, CoreCount);

      if ((DownCoreMode == CCX_DOWN_CORE_1_0) && IsVmr && (CcdPresentByFuseCount == 2)) {
        CoreDisMask = 0x000000FD;
      }
      if (DesiredCcdCount == 0xFFFFFFFF) {
        DesiredCcdCount = CcdCount;
      }

      if (DesiredCoreCount == 0xFFFFFFFF) {
        DesiredCoreCount = CoreCount;
      }

      if (((CcdCount + CcdDownByFuseCount) < CcdPresentByFuseCount) ||
          (((CoreCount * ComplexCount) + CoreDisByFuseCount) < 8)) {
        AlreadyDownCored = TRUE;
        CcdDisMask = 0;
        CoreDisMask = 0;

        if (AllocParams.BufferPtr == NULL) {
          // allocate a heap to indicate it's already down cored
          AllocParams.RequestedBufferSize = sizeof (BOOLEAN);
          AllocParams.BufferHandle = AMD_DOWN_CORE_STATUS_HANDLE;
          AllocParams.Persist = HEAP_SYSTEM_MEM;

          AgesaStatus = HeapAllocateBuffer (&AllocParams, NULL);
          ASSERT (AgesaStatus == AGESA_SUCCESS);
        }
      }

      // Get down core bitmap
      // BitMap downcore mode is only supported on 1 socket system.
      if (SocketCount == 1) {
        DownCoreBitMap[0] = PcdGet16 (PcdAmdCoreDisCcd0);
        DownCoreBitMap[1] = PcdGet16 (PcdAmdCoreDisCcd1);
        DownCoreBitMap[2] = PcdGet16 (PcdAmdCoreDisCcd2);
        DownCoreBitMap[3] = PcdGet16 (PcdAmdCoreDisCcd3);
        DownCoreBitMap[4] = PcdGet16 (PcdAmdCoreDisCcd4);
        DownCoreBitMap[5] = PcdGet16 (PcdAmdCoreDisCcd5);
        DownCoreBitMap[6] = PcdGet16 (PcdAmdCoreDisCcd6);
        DownCoreBitMap[7] = PcdGet16 (PcdAmdCoreDisCcd7);

        // Get DesiredCoreCount from down core bitmap
        if (DownCoreBitMap[PhysCcdNumber[0][0][0]] != 0) {
          DesiredCoreCount = 0;
          CoreDisByBitMap = GetSetBitCount (DownCoreBitMap[PhysCcdNumber[0][0][0]] & 0xFF);
          if (CoreDisByBitMap < FusedCoreCount) {
            DesiredCoreCount = FusedCoreCount - CoreDisByBitMap;
          }
          ASSERT (DesiredCoreCount != 0);
        }
      }

      IDS_HDT_CONSOLE (CPU_TRACE, "  DesiredCcdCount %X DesiredCoreCount %X\n", DesiredCcdCount, DesiredCoreCount);

      if ((CcdDisMask != 0) || (CoreDisMask != 0) || (DownCoreBitMap[0] != 0)) {
        // CCDs/Cores can be removed, if:
        // - it has not been previously down cored
        // - number of CCDs/Cores requested is less than current enabled CCDs/Cores
        if (DesiredCcdCount > CcdCount) {
          IDS_HDT_CONSOLE (CPU_TRACE, "  Invalid CCD config requested\n");
          CcdAttainable = FALSE;
          CcdDisMask = 0;
          AmdErrorLog->AmdErrorLogPei (AmdErrorLog,
                                        AGESA_ALERT,
                                        CPU_EVENT_DOWN_CORE_FAILURE,
                                        CCD_INVALID_CONFIG_FAILURE,
                                        (((UINT32) LShiftU64 (DesiredCcdCount, 16)) | CcdCount),
                                        0x00000000,
                                        0x00000000
                                        );
        }

        if (DesiredCcdCount == CcdCount) {
          IDS_HDT_CONSOLE (CPU_TRACE, "  CCD config met\n");
          CcdDisMask = 0;
          AmdErrorLog->AmdErrorLogPei (AmdErrorLog,
                                        AGESA_ALERT,
                                        CPU_EVENT_DOWN_CORE_FAILURE,
                                        CCD_DOWN_CORE_CONFIG_MET,
                                        (((UINT32) LShiftU64 (DesiredCcdCount, 16)) | CcdCount),
                                        0x00000000,
                                        0x00000000
                                        );
        } else {
          Met = FALSE;
        }

        if (DesiredCoreCount > CoreCount) {
          IDS_HDT_CONSOLE (CPU_TRACE, "  Invalid core config requested\n");
          CoreAttainable = FALSE;
          CoreDisMask = 0;
          AmdErrorLog->AmdErrorLogPei (AmdErrorLog,
                                        AGESA_ALERT,
                                        CPU_EVENT_DOWN_CORE_FAILURE,
                                        CORE_INVALID_CONFIG_FAILURE,
                                        (0x00010000 | ComplexCount),
                                        (((UINT32) LShiftU64 (DesiredCoreCount, 16)) | CoreCount),
                                        0x00000000
                                        );
        }

        if (DesiredCoreCount == CoreCount) {
          IDS_HDT_CONSOLE (CPU_TRACE, "  Core config met\n");
          CoreDisMask = 0;
          AmdErrorLog->AmdErrorLogPei (AmdErrorLog,
                                        AGESA_ALERT,
                                        CPU_EVENT_DOWN_CORE_FAILURE,
                                        CORE_DOWN_CORE_CONFIG_MET,
                                        (0x00010000 | ComplexCount),
                                        (((UINT32) LShiftU64 (DesiredCoreCount, 16)) | CoreCount),
                                        0x00000000
                                        );
        } else {
          Met = FALSE;
        }
      }

      if (!AlreadyDownCored && !Met && CoreAttainable && CcdAttainable &&
          ((CcdDisMask != 0) || (CoreDisMask != 0) || (DownCoreBitMap[0] != 0))) {
        Ccd0SwDowncoreAddr = SocZen3Services->GetCcd0SoftDowncoreAddress (PeiServices);
        for (CcdLoop = 0; CcdLoop < CcdCount; CcdLoop ++) {
          if (((CcdDisMask & (1 << PhysCcdNumber[Socket][Die][CcdLoop])) == 0)) {
            if (DownCoreBitMap[PhysCcdNumber[Socket][Die][CcdLoop]] != 0) {
              DisMask = DownCoreBitMap[PhysCcdNumber[Socket][Die][CcdLoop]] & 0xFF;
            } else {
              DisMask = CoreDisMask;
            }
          } else {
            DisMask = 0xFF;
          }

          if (DisMask != 0) {
            IDS_HDT_CONSOLE (CPU_TRACE, "  Set CCD %x# down core register %x\n", CcdLoop, DisMask);
            NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (Ccd0SwDowncoreAddr + ((UINT32) PhysCcdNumber[Socket][Die][CcdLoop] << 25)), &DisMask);
            SmuArg[0] = 0xAA55AA55;
            NbioSmuServices->SmuServiceRequest (NbioSmuServices, SystemDieLoop, 1, SmuArg, SmuArg);
            IssueReset = TRUE;
          }
        }
      }

      // xApic mode, total thread count get from OPN is >= 255, already downcore core count to 7 and now DownCoreMode is auto
      // Need to change DownCoreMode to CCX_DOWN_CORE_7_0 to stop from cold reset, this is for Server case
      OpnCoreCount = SocketCount * (CcdPresentByFuseCount - CcdDownByFuseCount) * (8 - CoreDisByFuseCount) * ThrdPerCoreFuse;
      if ((OpnCoreCount >= XAPIC_ID_MAX) && (DownCoreMode == CCX_DOWN_CORE_AUTO) && (ApicMode == xApicMode) && (CoreCount == 7))
      {
        // set DownCoreMode to something other than CCX_DOWN_CORE_AUTO to prevent infinite reset loop
        DownCoreMode = CCX_DOWN_CORE_7_0;
        IDS_HDT_CONSOLE (CPU_TRACE, "  Down core occurred as a result of xApicMode selection\n");
        AmdErrorLog->AmdErrorLogPei (AmdErrorLog,
                                      AGESA_ALERT,
                                      CPU_EVENT_DOWN_CORE_FAILURE,
                                      CORE_DOWN_CORE_FROM_XAPIC,
                                      0x00000000,
                                      0x00000000,
                                      0x00000000
                                      );
      }

      SystemDieLoop++;
    }
  }

  if (SmtMode == 0) {
    if (ThreadCount > 1) {
      IDS_HDT_CONSOLE (CPU_TRACE, "  Disabling SMT\n");
      NbioSmuServices->SmuDisableSmt (NbioSmuServices);
      IssueReset = TRUE;
    }
  }

  if (AlreadyDownCored || (ThreadCount != ThrdPerCoreFuse) || !SocZen3Services->IsS3Supported (PeiServices)) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  Setting PcdAmdAcpiS3Support to FALSE\n");
    PcdSetBoolS (PcdAmdAcpiS3Support, FALSE);
  }

  Reset2 = NULL;
  (*PeiServices)->LocatePpi (PeiServices, &gEfiPeiReset2PpiGuid, 0, NULL, &Reset2);
  // if DownCoreMode = Auto and it's already downcored
  // or if AlreadyDownCored, and actual core/ccx count meet neither DownCoreMode nor Fuse
  // or SmtMode = Auto, and actual ThreadsPerCore doesn't meet fuse
  // issue code reset
  if ((!Met && AlreadyDownCored) ||
      (!GameMode && (CcdMode == CCD_MODE_AUTO) && (DownCoreMode == CCX_DOWN_CORE_AUTO) && AlreadyDownCored && (DownCoreBitMap[PhysCcdNumber[0][0][0]] == 0)) ||
      ((SmtMode == 1) && (ThreadCount != ThrdPerCoreFuse))) {
    AmdMemRestoreDiscardCurrentMemContext();
    if (Reset2 != NULL) {
      IDS_HDT_CONSOLE (CPU_TRACE, "  Issuing cold reset\n");
      Reset2->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
    }
  }

  if (IssueReset) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  Issuing warm reset\n");
    AmdMemRestoreDiscardCurrentMemContext();
    if (Reset2 != NULL) {
      Status = StrCpyS (
                    (CHAR16 *)DownCoreResetData,
                    DOWN_CORE_RESET_DATA_SIZE,
                    L"Specific Warm Reset"
                    );
      ASSERT (Status == EFI_SUCCESS);

      Size = StrnSizeS ((CHAR16 *)DownCoreResetData, (DOWN_CORE_RESET_DATA_SIZE / sizeof(CHAR16)));
      ASSERT ((Size + sizeof(EFI_GUID)) <= DOWN_CORE_RESET_DATA_SIZE);

      CopyGuid (
            (EFI_GUID *)(DownCoreResetData + Size),
            &gEfiAmdAgesaSpecificWarmResetGuid
            );
      Size += sizeof(EFI_GUID);
      Reset2->ResetSystem (
            EfiResetPlatformSpecific,
            EFI_SUCCESS,
            Size,
            DownCoreResetData
            );
    } else {
      (**PeiServices).ResetSystem (PeiServices);
    }
  }

  AGESA_TESTPOINT (TpCcxPeiDownCoreCallbackExit, NULL);

  return Status;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  GetSetBitCount
 *
 *  Description:
 *    This helper function returns the number of bits that are set in the
 *    input value.
 *
 */
UINT32
GetSetBitCount (
  IN       UINT32 Value
  )
{
  UINT32  Count;
  UINT32  Temp;

  Count = 0;
  Temp = Value;
  while (Temp != 0) {
    if ((Temp & 1) != 0) {
      Count++;
    }
    Temp >>= 1;
  }

  return Count;
}

