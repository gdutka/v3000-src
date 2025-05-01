/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Filecode.h>
#include <Guid/EventGroup.h>
#include "AGESA.h"
#include "Addendum/Apcb/Inc/RMB/APOB.h"
#include <Library/DebugLib.h>
#include "Library/IdsLib.h"
#include <Library/UefiDriverEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/AmdPspApobLib.h>
#include <Library/RasRmbBaseLib.h>
#include <Protocol/FabricTopologyServices2.h>
#include <Protocol/AmdCoreTopologyProtocol.h>
#include <Protocol/AmdCoreTopologyV2Protocol.h>
#include "AmdRasRmbDxe.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE RAS_AMDRASRMBDXE_AMDRASRMBDXE_FILECODE


#define RAS_MAX_CORES (CCX_MAX_SOCKETS * CCX_MAX_DIES_PER_SOCKET * CCD_MAX_CCDS_PER_DIE * CCD_MAX_COMPLEXES_PER_CCD * CCX_MAX_CORES_PER_COMPLEX * CCX_MAX_THREADS_PER_CORE)

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

extern  EFI_BOOT_SERVICES       *gBS;
AMD_RAS_POLICY          *mAmdRasPolicy;
ADDR_DATA               *gAddrData;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
CollectDimmMap(VOID);

EFI_STATUS
CollectCpuMap(VOID);

EFI_STATUS
AmdRasPolicyInit(VOID);

//EFI_STATUS
//NbioErrThresInit(VOID);
VOID
NbioErrThresInit(
  IN EFI_EVENT  Event,
  IN VOID       *Context
);

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          T A B L E    D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

/*********************************************************************************
 * Name: AmdRasRmbDxeInit
 *
 * Description
 *   Entry point of the AMD RAS RMB DXE driver
 *   Perform the configuration init, resource reservation, early post init
 *   and install all the supported protocol
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
AmdRasRmbDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS          Status = EFI_SUCCESS;
  EFI_HANDLE          Handle = NULL;
  EFI_EVENT           ExitBootServicesEvent;

  DEBUG((EFI_D_ERROR, "[RAS] RMB AGESA RAS DXE driver entry\n"));

  AmdRasPolicyInit();

  CollectDimmMap();

  CollectCpuMap();

  Handle = ImageHandle;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdRasInitDataProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  mAmdRasPolicy
                  );
  if (EFI_ERROR (Status)) {
    return (Status);
  }

  //
  // Register the event handling function
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  NbioErrThresInit,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ExitBootServicesEvent
                  );


  return (Status);
}

VOID
NbioErrThresInit(
  IN EFI_EVENT  Event,
  IN VOID       *Context
)
{
#if 0
  UINT8                 Die;
  UINT8                 DieBusNum;
  UINT16                VendorID;
  UINTN                 PciAddress;
  PARITY_CONTROL_0_REG  ParityControl0;

  //Init reset value
  ParityControl0.Fields.ParityUCPThreshold = 1;
  ParityControl0.Fields.ParityCorrThreshold = 1;

  //Read pcd setting
  if (mAmdRasPolicy->NbioDeferredErrThreshEn) {
    ParityControl0.Fields.ParityUCPThreshold = mAmdRasPolicy->NbioDeferredErrThreshCount;
    DEBUG ((EFI_D_ERROR, "[RAS]NBIO Deferred Error Threshold : %d\n", ParityControl0.Fields.ParityUCPThreshold));
  }
  if (mAmdRasPolicy->NbioCorrectedErrThreshEn) {
    ParityControl0.Fields.ParityCorrThreshold = mAmdRasPolicy->NbioCorrectedErrThreshCount;
    DEBUG ((EFI_D_ERROR, "[RAS]NBIO Corrected Error Threshold : %d\n", ParityControl0.Fields.ParityCorrThreshold));
  }

  //Register Error handler per Die.
  for (Die = 0; Die < MAX_NBIO_SUPPORT; Die++) {
    PciAddress = ((Die + 24) << 15) + 0x000;
    VendorID = PciRead16 (PciAddress);
    if (VendorID != AMD_VENDOR_ID) {
      continue;
    }

    PciAddress = ((Die + 24) << 15) + (DF_CFGADDRESSCNTL_FUNC << 12) + DF_CFGADDRESSCNTL_OFFSET;
    DieBusNum = PciRead8 (PciAddress);

    DEBUG ((EFI_D_ERROR, "[RAS]Bus: 0x%0x, Parity Control 0 : 0x%08x\n", DieBusNum, ParityControl0.Value));
    RasSmnWrite(DieBusNum, RAS_PARITY_CONTROL_0, &ParityControl0.Value);
  }
#endif
}

EFI_STATUS
AmdRasPolicyInit(VOID)
{
  EFI_STATUS          Status = EFI_SUCCESS;

  //
  //  Allocate memory and Initialize for Data block
  //
  Status = gBS->AllocatePool (
                  EfiReservedMemoryType,
                  sizeof (AMD_RAS_POLICY),
                  (VOID **)&mAmdRasPolicy
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (mAmdRasPolicy, sizeof (AMD_RAS_POLICY));

  mAmdRasPolicy->PFEHEnable = PcdGetBool(PcdAmdCcxCfgPFEHEnable);
  mAmdRasPolicy->MceSwSmiData = PcdGet8(PcdMceSwSmiData);
  mAmdRasPolicy->McaErrThreshEn = PcdGetBool(PcdMcaErrThreshEn);

  if (PcdGet16 (PcdMcaErrThreshCount) == 0xFFFF)  {
    //Use default setting
    if (PcdGetBool(PcdAmdCcxCfgPFEHEnable)) {
      PcdSet16S (PcdMcaErrThreshCount, 0x0FFE);
    } else {
      PcdSet16S (PcdMcaErrThreshCount, 0x0FF5);
    }
  }
  mAmdRasPolicy->McaErrThreshCount = PcdGet16(PcdMcaErrThreshCount);

  mAmdRasPolicy->NbioCorrectedErrThreshEn = PcdGetBool(PcdNbioCorrectedErrThreshEn);
  mAmdRasPolicy->NbioCorrectedErrThreshCount = PcdGet16(PcdNbioCorrectedErrThreshCount);
  mAmdRasPolicy->NbioDeferredErrThreshEn = PcdGetBool(PcdNbioDeferredErrThreshEn);
  mAmdRasPolicy->NbioDeferredErrThreshCount = PcdGet16(PcdNbioDeferredErrThreshCount);

  //FCH software SMI command port
  mAmdRasPolicy->SwSmiCmdPortAddr = MmioRead16((ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6A));

  return Status;
}

EFI_STATUS
CollectDimmMap(VOID)
{
  EFI_STATUS    Status = EFI_SUCCESS;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL *FabricTopology;
  UINTN     NumberOfInstalledProcessors;
  UINTN     NumberOfRootBridgesOnDie;
  UINTN     TotalNumberOfDie;
  UINTN     BusNumberBase;
  UINTN     socketId;
  UINTN     dieId;
  UINTN     channelId;
  UINTN     channelIdInSystem;

  Status = gBS->AllocatePool (
      EfiReservedMemoryType,  // IN EFI_MEMORY_TYPE PoolType
      sizeof (ADDR_DATA),     // IN UINTN Size
      (VOID **)&gAddrData     // OUT VOID **Buffer
      );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  ZeroMem (gAddrData, sizeof (ADDR_DATA));

  NumberOfInstalledProcessors = 0;
  TotalNumberOfDie = 0;
  NumberOfRootBridgesOnDie = 0;
  BusNumberBase = 0;

  Status = gBS->LocateProtocol(&gAmdFabricTopologyServices2ProtocolGuid, NULL, (VOID **) &FabricTopology);
  FabricTopology->GetSystemInfo (FabricTopology, &NumberOfInstalledProcessors, NULL, NULL, NULL, NULL);

  for (socketId = 0; socketId < NumberOfInstalledProcessors; socketId++) {
    FabricTopology->GetProcessorInfo (FabricTopology, socketId, &TotalNumberOfDie, NULL);
    for (dieId = 0; dieId < TotalNumberOfDie; dieId++) {
      FabricTopology->GetRootBridgeInfo (FabricTopology, socketId, dieId, NumberOfRootBridgesOnDie, NULL, &BusNumberBase, NULL, NULL, NULL, NULL);

      for (channelId = 0; channelId < UMC_PER_DIE; channelId++) {
        DEBUG((EFI_D_ERROR, "\nsocket %x die %x channel %x pci bus = %x\n", socketId, dieId, channelId, BusNumberBase));
        retrieve_regs (socketId, dieId, channelId, 0, BusNumberBase);
        channelIdInSystem = convert_to_addr_trans_index (socketId, dieId, channelId, 0);
        DEBUG((EFI_D_ERROR, "\n channelIdInSystem = %x\n", channelIdInSystem));
      }
    }
  }

  //Save Memory Address Data pointer to Amd Ras Policy
  mAmdRasPolicy->AddrData = gAddrData;

  return Status;
}

EFI_STATUS
CollectCpuMap(VOID)
{
  AMD_CORE_TOPOLOGY_SERVICES_PROTOCOL           *CoreTopologyServices;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL        *CoreTopology2Services;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL        *FabricTopologyServices;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT   ApobCcdLogToPhysMap;
  APOB_CCX_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT   ApobCcxLogToPhysMap;
  APOB_TYPE_HEADER                              *ApobEntry;
  CPU_INFO      *RasCpuMap;
  UINT32        ApobInstanceId;
  UINT32        Index;
  UINTN         SocketLoop;
  UINTN         DieLoop;
  UINTN         CcdLoop;
  UINTN         ComplexLoop;
  UINTN         CoreLoop;
  UINTN         ThreadLoop;
  UINTN         NumberOfSockets;
  UINTN         NumberOfSystemDies;
  UINTN         NumberOfDies;
  UINTN         NumberOfCcds;
  UINTN         NumberOfComplexes;
  UINTN         NumberOfCores;
  UINTN         NumberOfThreads;
  UINTN         SystemDieCount;
  EFI_STATUS    Status = EFI_SUCCESS;

  CoreTopologyServices = NULL;
  CoreTopology2Services = NULL;
  NumberOfComplexes = 0;
  NumberOfCores = 0;
  NumberOfThreads = 0;

  Status = gBS->LocateProtocol (&gAmdFabricTopologyServices2ProtocolGuid, NULL, &FabricTopologyServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (gBS->LocateProtocol (&gAmdCoreTopologyServicesV2ProtocolGuid, NULL, &CoreTopology2Services) != EFI_SUCCESS) {
    Status = gBS->LocateProtocol (&gAmdCoreTopologyServicesProtocolGuid, NULL, &CoreTopologyServices);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //
  //  Allocate memory and Initialize for Data block
  //
  Status = gBS->AllocatePool (
                  EfiReservedMemoryType,
                  sizeof (CPU_INFO) * RAS_MAX_CORES,
                  (VOID **)&RasCpuMap
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (RasCpuMap, sizeof (CPU_INFO) * RAS_MAX_CORES);

  Status = FabricTopologyServices->GetSystemInfo (FabricTopologyServices, &NumberOfSockets, &NumberOfSystemDies, NULL, NULL, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SystemDieCount = 0;
  Index = 0;
  NumberOfCcds = 0;
  for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
    Status = FabricTopologyServices->GetProcessorInfo (FabricTopologyServices, SocketLoop, &NumberOfDies, NULL);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
      SystemDieCount++;
      if (CoreTopology2Services != NULL) {
        Status = CoreTopology2Services->GetCoreTopologyOnDie (CoreTopology2Services, SocketLoop, DieLoop, &NumberOfCcds, &NumberOfComplexes, &NumberOfCores, &NumberOfThreads);
        if (EFI_ERROR (Status)) {
          return Status;
        }
      } else if (CoreTopologyServices != NULL) {
        Status = CoreTopologyServices->GetCoreTopologyOnDie (CoreTopologyServices, SocketLoop, DieLoop, &NumberOfComplexes, &NumberOfCores, &NumberOfThreads);
        if (EFI_ERROR (Status)) {
          return Status;
        }
      }

      ApobInstanceId = ((UINT32) SocketLoop << 8) | (UINT32) DieLoop;
      if (CoreTopology2Services != NULL) {
        Status = AmdPspGetApobEntryInstance (APOB_CCX, APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE, ApobInstanceId, FALSE, &ApobEntry);
        if (EFI_ERROR (Status)) {
          return Status;
        }
        CopyMem (&ApobCcdLogToPhysMap, ApobEntry, sizeof (APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT));
      } else {
        Status = AmdPspGetApobEntryInstance (APOB_CCX, APOB_CCX_LOGICAL_TO_PHYSICAL_MAP_TYPE, ApobInstanceId, FALSE, &ApobEntry);
        if (EFI_ERROR (Status)) {
          return Status;
        }
        CopyMem (&ApobCcxLogToPhysMap, ApobEntry, sizeof (APOB_CCX_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT));

        NumberOfCcds = 1;
        ApobCcdLogToPhysMap.CcdMap[0].PhysCcdNumber = 0;
        ApobCcdLogToPhysMap.CcdMap[1].PhysCcdNumber = CCX_NOT_PRESENT;
        for (ComplexLoop = 0; ComplexLoop < CCX_MAX_COMPLEXES_PER_DIE; ComplexLoop++) {
          ApobCcdLogToPhysMap.CcdMap[0].ComplexMap[ComplexLoop].PhysComplexNumber = ApobCcxLogToPhysMap.ComplexMap[ComplexLoop].PhysComplexNumber;
          for (CoreLoop = 0; CoreLoop < CCX_MAX_CORES_PER_COMPLEX; CoreLoop++) {
            ApobCcdLogToPhysMap.CcdMap[0].ComplexMap[ComplexLoop].CoreInfo[CoreLoop].PhysCoreNumber =
              ApobCcxLogToPhysMap.ComplexMap[ComplexLoop].CoreInfo[CoreLoop].PhysCoreNumber;
            for (ThreadLoop = 0; ThreadLoop < CCX_MAX_THREADS_PER_CORE; ThreadLoop++) {
              ApobCcdLogToPhysMap.CcdMap[0].ComplexMap[ComplexLoop].CoreInfo[CoreLoop].IsThreadEnabled[ThreadLoop] =
                ApobCcxLogToPhysMap.ComplexMap[ComplexLoop].CoreInfo[CoreLoop].IsThreadEnabled[ThreadLoop];
            }
          }
        }
      }

      for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
        if (ApobCcdLogToPhysMap.CcdMap[CcdLoop].PhysCcdNumber == CCX_NOT_PRESENT) {
          return EFI_NOT_FOUND;
        }
        for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
          if (ApobCcdLogToPhysMap.CcdMap[CcdLoop].ComplexMap[ComplexLoop].PhysComplexNumber == CCX_NOT_PRESENT) {
            return EFI_NOT_FOUND;
          }
          for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
            if (ApobCcdLogToPhysMap.CcdMap[CcdLoop].ComplexMap[ComplexLoop].CoreInfo[CoreLoop].PhysCoreNumber == CCX_NOT_PRESENT) {
              return EFI_NOT_FOUND;
            }
            if (Index == RAS_MAX_CORES) {
              return EFI_BUFFER_TOO_SMALL;                //Should not be here
            }
            RasCpuMap[Index].ProcessorNumber = Index;    //CPU Logic Number
            RasCpuMap[Index].SocketId = (UINT8)(SocketLoop & 0xFF);
            RasCpuMap[Index].DieId = (CoreTopology2Services != NULL) ? (UINT8)(CcdLoop & 0xFF) : (UINT8)(DieLoop & 0xFF);
            RasCpuMap[Index].CcxId = (UINT8)(ComplexLoop & 0xFF);
            RasCpuMap[Index].CoreId = (UINT8)(CoreLoop & 0xFF);
            RasCpuMap[Index].ThreadID = 0;
            Index++;
            if (NumberOfThreads > 1) {
              if (Index == RAS_MAX_CORES) {
                return EFI_BUFFER_TOO_SMALL;
              }
              RasCpuMap[Index].ProcessorNumber = Index; //CPU Logic Number
              RasCpuMap[Index].SocketId = (UINT8)(SocketLoop & 0xFF);
              RasCpuMap[Index].DieId = (CoreTopology2Services != NULL) ? (UINT8)(CcdLoop & 0xFF) : (UINT8)(DieLoop & 0xFF);
              RasCpuMap[Index].CcxId = (UINT8)(ComplexLoop & 0xFF);
              RasCpuMap[Index].CoreId = (UINT8)(CoreLoop & 0xFF);
              RasCpuMap[Index].ThreadID = 1;
              Index++;
            }
          }
        }
      }
    }
  }

  mAmdRasPolicy->TotalNumberOfProcessors = Index;

  //Update Ras CPU map pointer to AMD RAS Policy buffer.
  mAmdRasPolicy->RasCpuMap = RasCpuMap;

  return (Status);
}

