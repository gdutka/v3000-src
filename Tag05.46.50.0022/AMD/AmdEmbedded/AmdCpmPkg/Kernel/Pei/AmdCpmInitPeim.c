/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmPei.h>
#include <AmdCpmTable.h>
#include <AmdCpmBaseIo.h>
#include <AmdCpmCpu.h>
#include <AmdCpmFch.h>
#include <Ppi/FabricTopologyServices2Ppi.h>

typedef struct {
  IN         BOOLEAN      EarlyLinkStatus;
  IN         UINT8        PhysicalRootBridge;
  IN         UINT8        LogicalRootBridge;
  IN         UINT8        RootPortBus;
  IN         UINT8        RootPortDevice;
  IN         UINT8        RootPortFunction;
} EARLY_LINK_STATUS;

typedef struct _PEI_AMD_NBIO_BXB_EARLY_LINK_PPI PEI_AMD_NBIO_BXB_EARLY_LINK_PPI;

typedef
EFI_STATUS
(EFIAPI * AMD_NBIO_BXB_EARLY_LINK_CONFIG) (
  IN  PEI_AMD_NBIO_BXB_EARLY_LINK_PPI *This,
  OUT EARLY_LINK_STATUS               *EarlyLinkStatus
);

struct _PEI_AMD_NBIO_BXB_EARLY_LINK_PPI {
  AMD_NBIO_BXB_EARLY_LINK_CONFIG       GetEarlyLinkConfig;
};

typedef struct _BIXBY_LINK_DATA {
  PEI_AMD_NBIO_BXB_EARLY_LINK_PPI EarlyLinkPpi;
  EFI_PEI_PPI_DESCRIPTOR          ppiList;
  EARLY_LINK_STATUS               EarlyLink;
} BIXBY_LINK_DATA;

EFI_GUID  gEfiPeiPromotoryGpioPpiGuid     = { 0x31859c50, 0x2845, 0x42da, {0x9f, 0x37, 0xe7, 0x18, 0x67, 0xe3, 0xe0, 0x5e} };

VOID
EFIAPI
AmdCpmCoreTopologyInit (
  IN       AMD_CPM_TABLE_PPI           *CpmTablePpiPtr
  );

VOID
EFIAPI
AmdCpmLoadPreInitTable (
  IN       VOID                        *This,
  IN       UINT8                       Stage
  );

VOID
EFIAPI
AmdCpmPlatformIdInit (
  IN       AMD_CPM_TABLE_PPI           *CpmTablePpiPtr
  );

VOID
EFIAPI
AmdCpmPeiRegisterKernel (
  IN       VOID                        *This
  );

EFI_STATUS
EFIAPI
AmdCpmInitBeforeS3LateRestore (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

EFI_STATUS
EFIAPI
AmdCpmInitAfterPeiMemoryDiscovered (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

EFI_STATUS
EFIAPI
AmdCpmInitPeimPreNotifyCallback (
  IN      EFI_PEI_SERVICES              **PeiServices,
  IN      EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDescriptor,
  IN      VOID                          *Ppi
  );

EFI_STATUS
EFIAPI
AmdCpmInitPeimNotifyCallback (
  IN      EFI_PEI_SERVICES              **PeiServices,
  IN      EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDescriptor,
  IN      VOID                          *Ppi
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mCpmInitPeimPreNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdCpmOemTablePpiGuid,
  AmdCpmInitPeimPreNotifyCallback
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mCpmInitPeimNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdFabricTopologyServices2PpiGuid,
  AmdCpmInitPeimNotifyCallback
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mCpmInitPeimBrNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdCpmOemTablePpiGuid,
  AmdCpmInitPeimNotifyCallback
};

EFI_STATUS
EFIAPI
AmdCpmPtInitPeim (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  );

EFI_STATUS
EFIAPI
AmdCpmBxbInitPeim (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mNotifyListS3Restore =
{
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gCpmPeiMemoryDiscoveredPpiGuid,
  AmdCpmInitBeforeS3LateRestore
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mNotifyAfterPeiMemoryDiscovered =
{
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gCpmPeiMemoryDiscoveredPpiGuid,
  AmdCpmInitAfterPeiMemoryDiscovered
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mCpmPtInitPeimNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiPromotoryGpioPpiGuid,
  AmdCpmPtInitPeim
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mCpmBxbInitPeimNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdNbioPcieTrainingDonePpiGuid,
  AmdCpmBxbInitPeim
};

AMD_CPM_CORE_TOPOLOGY_TABLE gCpmCoreTopologyTable = {
  {CPM_SIGNATURE_CORE_TOPOLOGY, sizeof (gCpmCoreTopologyTable) / sizeof (UINT8), 0, 0, 0, 0x0000000F},
};

/*----------------------------------------------------------------------------------------*/
/**
 * Init Promontory GPIO pins
 *
 * This function initializes Promontory GPIO Device
 * after AGESA FCH Promotory Gpio PPI is installed.
 *
 * @param[in]     PeiServices       Pointer to Pei Services
 * @param[in]     NotifyDescriptor  The descriptor for the notification event
 * @param[in]     Ppi               Pointer to the PPI in question.
 *
 * @retval        EFI_SUCCESS       Module initialized successfully
 * @retval        EFI_ERROR         Initialization failed (see error for more details)
 *
 */
EFI_STATUS
AmdCpmPtInitPeim (
  IN  EFI_PEI_SERVICES          **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN  VOID                      *Ppi
  )
{
  EFI_STATUS                    Status;
  AMD_CPM_TABLE_PPI             *CpmTablePtr;
  AMD_CPM_COMMON_FUNCTION       *CommonFunctionPtr;
  UINT32                        BridgeAddress;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-Start\n", __FUNCTION__));
  Status = (*PeiServices)->LocatePpi (
            (CPM_PEI_SERVICES**)PeiServices,
            &gAmdCpmTablePpiGuid,
            0,
            NULL,
            (VOID**)&CpmTablePtr
            );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-1-LocatePpi-gAmdCpmTablePpiGuid-FAIL\n", __FUNCTION__));
    return Status;
  }

  CommonFunctionPtr = &CpmTablePtr->CommonFunction;
  DEBUG((DEBUG_INFO, "KER-PEI-%a-GetPtBrDevAddr\n", __FUNCTION__));
  BridgeAddress = CommonFunctionPtr->GetPtBrDevAddr (CpmTablePtr);
  DEBUG((DEBUG_INFO, "KER-PEI-%a-PtBrMemIoBaseAddr = 0x%08X\n", __FUNCTION__, BridgeAddress));
  CpmTablePtr->MainTablePtr->PtBrMemIoBaseAddr = BridgeAddress;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-End-2-EFI_SUCCESS\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init Bixby GPIO pins
 *
 * This function initializes Bixby GPIO Device
 * after AGESA FCH Bixby Gpio PPI is installed.
 *
 * @param[in]     PeiServices       Pointer to Pei Services
 * @param[in]     NotifyDescriptor  The descriptor for the notification event
 * @param[in]     Ppi               Pointer to the PPI in question.
 *
 * @retval        EFI_SUCCESS       Module initialized successfully
 * @retval        EFI_ERROR         Initialization failed (see error for more details)
 *
 */
EFI_STATUS
AmdCpmBxbInitPeim (
  IN  EFI_PEI_SERVICES          **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN  VOID                      *Ppi
  )
{
  EFI_STATUS                    Status;
  AMD_CPM_TABLE_PPI             *CpmTablePtr;
  BIXBY_LINK_DATA               *BixbyLinkInstance;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-Start\n", __FUNCTION__));
  Status = (*PeiServices)->LocatePpi (
            (CPM_PEI_SERVICES**)PeiServices,
            &gAmdCpmTablePpiGuid,
            0,
            NULL,
            (VOID**)&CpmTablePtr
            );
  if (EFI_ERROR (Status)) {
    CpmTablePtr->MainTablePtr->BxbBusNumDetEn = 0;
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-1-LocatePpi-gAmdCpmTablePpiGuid-FAIL\n", __FUNCTION__));
    DEBUG((DEBUG_INFO, "KER-PEI-%a-Set MainTablePtr->BxbBusNumDetEn = 0x%02X\n", __FUNCTION__, CpmTablePtr->MainTablePtr->BxbBusNumDetEn));
    return Status;
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-LocatePpi-gAmdNbioBixbyEarlyLinkPpiGuid\n", __FUNCTION__));
  Status = (*PeiServices)->LocatePpi (
            PeiServices,
            &gAmdNbioBixbyEarlyLinkPpiGuid,
            0,
            NULL,
            (VOID**)&BixbyLinkInstance
            );

  if (EFI_ERROR (Status)) {
    CpmTablePtr->MainTablePtr->BxbBusNumDetEn = 0;
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-2-LocatePpi-gAmdCpmTablePpiGuid-FAIL\n", __FUNCTION__));
    DEBUG((DEBUG_INFO, "KER-PEI-%a-Set MainTablePtr->BxbBusNumDetEn = 0x%02X\n", __FUNCTION__, CpmTablePtr->MainTablePtr->BxbBusNumDetEn));
    return Status;
  }

  CpmTablePtr->MainTablePtr->BxbBusNumDetEn = 1;
  DEBUG((DEBUG_INFO, "KER-PEI-%a-Set MainTablePtr->BxbBusNumDetEn = 0x%02X\n", __FUNCTION__, CpmTablePtr->MainTablePtr->BxbBusNumDetEn));

  DEBUG((DEBUG_INFO, "KER-PEI-%a-End-3-EFI_SUCCESS\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * CPM Init Function Before S3 Late Restore
 *
 * This function updates CPM Main Table Pointer in AMD_CPM_TABLE_PPI Data Structure
 * after PEI MEMORY DISCOVERED PPI is installed.
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 * @param[in]     NotifyDesc     The descriptor for the notification event
 * @param[in]     InvokePpi      Pointer to the PPI in question.
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 *
 */
EFI_STATUS
EFIAPI
AmdCpmInitBeforeS3LateRestore (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  EFI_STATUS                  Status;
  AMD_CPM_TABLE_PPI           *CpmTablePtr;
  AMD_CPM_TABLE_HOB_PPI       *CpmTableHobPtr;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-Start\n", __FUNCTION__));
  DEBUG((DEBUG_INFO, "KER-PEI-%a-From-", __FUNCTION__));
  DEBUG((DEBUG_INFO, "AmdCpmInitPeimNotifyCallback-NotifyPpi-gCpmPeiMemoryDiscoveredPpiGuid\n"));
  RECORD_TIME_PEI (BeginAmdCpmInitBeforeS3LateRestore);

  Status = (*PeiServices)->LocatePpi (
                              (CPM_PEI_SERVICES**)PeiServices,
                              &gAmdCpmTablePpiGuid,
                              0,
                              NULL,
                              (VOID**)&CpmTablePtr
                              );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-1-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = (*PeiServices)->LocatePpi (
                              (CPM_PEI_SERVICES**)PeiServices,
                              &gAmdCpmTableHobPpiGuid,
                              0,
                              NULL,
                              (VOID**)&CpmTableHobPtr
                              );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-2-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  CpmTablePtr->CommonFunction.RelocateTable (CpmTableHobPtr);

  CpmTablePtr->MainTablePtr = (AMD_CPM_MAIN_TABLE*) ((AMD_CPM_HOB_HEADER*)CpmTableHobPtr)->MainTablePtr.Pointer;

  RECORD_TIME_PEI (EndAmdCpmInitBeforeS3LateRestore);

  DEBUG((DEBUG_INFO, "KER-PEI-%a-End-3-Status=%r\n", __FUNCTION__, Status));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * CPM Init Function After PeiMemoryDiscovered is installed
 *
 * This function updates CPM Main Table Pointer in AMD_CPM_TABLE_PPI Data Structure
 * after PEI MEMORY DISCOVERED PPI is installed.
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 * @param[in]     NotifyDesc     The descriptor for the notification event
 * @param[in]     InvokePpi      Pointer to the PPI in question.
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 *
 */
EFI_STATUS
EFIAPI
AmdCpmInitAfterPeiMemoryDiscovered (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  EFI_STATUS                  Status;
  AMD_CPM_TABLE_PPI           *CpmTablePtr;
  AMD_CPM_TABLE_HOB_PPI       *CpmTableHobPtr = NULL;
  VOID                        *FirstHob = NULL;
  VOID                        *Buffer = NULL;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-Start-From-", __FUNCTION__));
  DEBUG((DEBUG_INFO, "AmdCpmInitPeimNotifyCallback-NotifyPpi-gCpmPeiMemoryDiscoveredPpiGuid\n"));

  Status = (*PeiServices)->LocatePpi (
                              (CPM_PEI_SERVICES**)PeiServices,
                              &gAmdCpmTablePpiGuid,
                              0,
                              NULL,
                              (VOID**)&CpmTablePtr
                              );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-1-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-GetHobList-FirstHob\n", __FUNCTION__));
  Status = (*PeiServices)->GetHobList (
                              (CPM_PEI_SERVICES**)PeiServices,
                              &FirstHob
                              );
  if ((EFI_ERROR (Status)) || (!FirstHob)) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-2-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-GetNextGuidHob-gAmdCpmTableHobGuid\n", __FUNCTION__));
  Buffer = GetNextGuidHob (&gAmdCpmTableHobGuid, FirstHob);
  if (Buffer != NULL) {
    CpmTableHobPtr = GET_GUID_HOB_DATA (Buffer);
    DEBUG((DEBUG_INFO, "KER-PEI-%a-RelocateTable-CpmTableHobPtr %08X\n", __FUNCTION__, (UINT32)(UINTN)CpmTableHobPtr));
    CpmTablePtr->CommonFunction.RelocateTable (CpmTableHobPtr);

    CpmTablePtr->MainTablePtr = (AMD_CPM_MAIN_TABLE*) ((AMD_CPM_HOB_HEADER*)CpmTableHobPtr)->MainTablePtr.Pointer;
    DEBUG((DEBUG_INFO, "KER-PEI-%a-CpmTablePtr->MainTablePtr %08X\n", __FUNCTION__, (UINT32)(UINTN)CpmTablePtr->MainTablePtr));
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-End-3-Status=%r\n", __FUNCTION__, Status));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Init PEIM driver
 *
 * This function gets CPM definition tables from AmdCpmOemTablePpi, reorgnizes the tables
 * and installs AmdCpmTablePpi. It also initializes the registers by load Pre Init Table
 * and update Current Platform Id at AmdCpmInitPeim.
 *
 * @param[in]     FileHandle     Pointer to the firmware file system header
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmInitPeimEntryPoint (
  IN       CPM_PEI_FILE_HANDLE  FileHandle,
  IN       CPM_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                  Status;
  UINT32                      RegEax = 0;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-Start\n", __FUNCTION__));
  DEBUG((DEBUG_INFO, "KER-PEI-%a-NotifyPpi-gAmdCpmOemTablePpiGuid", __FUNCTION__));

  AsmCpuid (0x80000001, &RegEax, NULL, NULL, NULL);
  if ((RegEax & 0x0FFF0FF0) == 0x00660F50) {                     // CPM_CPU_REVISION_ID_BR
    DEBUG((DEBUG_INFO, "-For-BR-AmdCpmInitPeimNotifyCallback\n"));
    Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmInitPeimBrNotify);
  } else {
    DEBUG((DEBUG_INFO, "-For-AmdCpmInitPeimPreNotifyCallback\n"));
    Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmInitPeimPreNotify);
  }
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-1-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-End-2-Status=%r\n", __FUNCTION__, Status));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Pre-Callback Function of the AMD CPM Init PEIM driver
 *
 * @param[in]     PeiServices       The PEI core services table.
 * @param[in]     NotifyDescriptor  The descriptor for the notification event.
 * @param[in]     Ppi               Pointer to the PPI in question
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmInitPeimPreNotifyCallback (
  IN      EFI_PEI_SERVICES              **PeiServices,
  IN      EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDescriptor,
  IN      VOID                          *Ppi
  )
{
  EFI_STATUS                  Status;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-From-AmdCpmInitPeimEntryPoint-NotifyPpi-gAmdCpmOemTablePpiGuid\n"));

  DEBUG((DEBUG_INFO, "KER-PEI-%a-NotifyPpi-gAmdFabricTopologyServices2PpiGuid", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-For-AmdCpmInitPeimNotifyCallback\n"));
  Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmInitPeimNotify);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-1-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-End-2-Status=%r\n", __FUNCTION__, Status));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback Function of the AMD CPM Init PEIM driver
 *
 * @param[in]     PeiServices       The PEI core services table.
 * @param[in]     NotifyDescriptor  The descriptor for the notification event.
 * @param[in]     Ppi               Pointer to the PPI in question
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmInitPeimNotifyCallback (
  IN      EFI_PEI_SERVICES              **PeiServices,
  IN      EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDescriptor,
  IN      VOID                          *Ppi
  )
{
  EFI_STATUS                  Status;
  AMD_CPM_OEM_TABLE_PPI       *AmdCpmOemTablePpiPtr;
  AMD_CPM_TABLE_PPI           *AmdCpmTablePpiPtr;
  EFI_PEI_PPI_DESCRIPTOR      *PpiListCpmTablePtr;
  EFI_PEI_PPI_DESCRIPTOR      *PpiListCpmTableHobPtr;
  AMD_CPM_MAIN_TABLE          *MainTablePtr;
  AMD_CPM_SAVE_CONTEXT_TABLE  *SaveContextTablePtr;
  AMD_CPM_COMMON_FUNCTION     *CommonFunctionPtr;
  EFI_HOB_GUID_TYPE           *Hob;
  EFI_BOOT_MODE               BootMode;
  AMD_CPM_TABLE_LIST          HobList;
  UINT8                       *HobBuffer;
  UINT8                       CpmBootMode;
  BIXBY_LINK_DATA             *BixbyLinkInstance;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-From-AmdCpmInitPeimPreEntryPoint-NotifyPpi-gAmdFabricTopologyServices2PpiGuid\n"));
  RECORD_TIME_PEI (BeginAmdCpmInitPeimDriver);

  Status = (*PeiServices)->LocatePpi (
                              PeiServices,
                              &gAmdCpmOemTablePpiGuid,
                              0,
                              NULL,
                              (VOID**)&AmdCpmOemTablePpiPtr
                              );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-1-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-CpmGenerateTableList\n", __FUNCTION__));
  CpmGenerateTableList (AmdCpmOemTablePpiPtr->TableList, AmdCpmOemTablePpiPtr->PlatformId, &HobList);

  DEBUG((DEBUG_INFO, "KER-PEI-%a-GetBootMode\n", __FUNCTION__));
  Status = (*PeiServices)->GetBootMode (
                             PeiServices,
                             &BootMode
                             );
  DEBUG((DEBUG_INFO, "KER-PEI-%a-GetBootMode=0x%X\n", __FUNCTION__, BootMode));
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-2-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-CpmGetBootMode\n", __FUNCTION__));
  CpmBootMode = CpmGetBootMode ();
  DEBUG((DEBUG_INFO, "KER-PEI-%a-CpmGetBootMode=0x%X\n", __FUNCTION__, CpmBootMode));

  if (CpmBootMode != CPM_BOOT_MODE_S3) {
    If_CpmBuildGuidHob (gAmdCpmTableHobGuid, Hob, NULL, HobList.Size + AMD_TABLE_ADDITIONAL_BUFFER_SIZE, Status) {

      HobBuffer = (UINT8*) Hob;

      DEBUG((DEBUG_INFO, "KER-PEI-%a-NotifyPpi-gCpmPeiMemoryDiscoveredPpiGuid", __FUNCTION__));
      DEBUG((DEBUG_INFO, "-For-AmdCpmInitAfterPeiMemoryDiscovered\n"));
      Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyAfterPeiMemoryDiscovered);
    } else {
      DEBUG((DEBUG_INFO, "KER-PEI-%a-End-3-Status=%r\n", __FUNCTION__, Status));
      return EFI_NOT_FOUND;
    }
  } else {
    Status = (*PeiServices)->AllocatePool (
                                PeiServices,
                                HobList.Size + AMD_TABLE_ADDITIONAL_BUFFER_SIZE,
                                (VOID**)&HobBuffer
                                );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "KER-PEI-%a-End-4-Status=%r\n", __FUNCTION__, Status));
      return Status;
    }

    Status = (*PeiServices)->AllocatePool (
                                PeiServices,
                                sizeof (EFI_PEI_PPI_DESCRIPTOR),
                                (VOID**)&PpiListCpmTableHobPtr
                                );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "KER-PEI-%a-End-5-Status=%r\n", __FUNCTION__, Status));
      return Status;
    }

    PpiListCpmTableHobPtr->Flags  = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
    PpiListCpmTableHobPtr->Guid   = &gAmdCpmTableHobPpiGuid;
    PpiListCpmTableHobPtr->Ppi    = HobBuffer;

    DEBUG((DEBUG_INFO, "KER-PEI-%a-InstallPpi-gAmdCpmTableHobPpiGuid\n", __FUNCTION__));
    Status = (*PeiServices)->InstallPpi (
                                PeiServices,
                                PpiListCpmTableHobPtr
                                );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "KER-PEI-%a-End-6-Status=%r\n", __FUNCTION__, Status));
      return Status;
    }

    DEBUG((DEBUG_INFO, "KER-PEI-%a-NotifyPpi-gCpmPeiMemoryDiscoveredPpiGuid\n", __FUNCTION__));
    DEBUG((DEBUG_INFO, "KER-PEI-%a-For-AmdCpmInitBeforeS3LateRestore\n", __FUNCTION__));
    Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyListS3Restore);
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-CpmCopyTableListToMemory(HobList)\n", __FUNCTION__));
  MainTablePtr = CpmCopyTableListToMemory (&HobList, HobBuffer);

  Status = (*PeiServices)->AllocatePool (
                              PeiServices,
                              sizeof (AMD_CPM_TABLE_PPI),
                              (VOID**)&AmdCpmTablePpiPtr
                              );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-7-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  if (MainTablePtr->BxbBrMmioBaseAddr == 0) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-LocatePpi-gAmdNbioBixbyEarlyLinkPpiGuid\n", __FUNCTION__));
    Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gAmdNbioBixbyEarlyLinkPpiGuid,
                            0,
                            NULL,
                            (VOID**)&BixbyLinkInstance
                            );

    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "KER-PEI-%a-LocatePpi-gAmdNbioBixbyEarlyLinkPpiGuid-FAIL\n", __FUNCTION__));
    } else {
      DEBUG((DEBUG_INFO, "KER-PEI-%a-Call EarlyLinkPpi.GetEarlyLinkConfig\n", __FUNCTION__));
      BixbyLinkInstance->EarlyLinkPpi.GetEarlyLinkConfig((PEI_AMD_NBIO_BXB_EARLY_LINK_PPI *)BixbyLinkInstance, &(BixbyLinkInstance->EarlyLink));

      MainTablePtr->BxbBusNumDetEn = 0xFF;
      MainTablePtr->BxbBrMmioBaseAddr = ((UINT32)BixbyLinkInstance->EarlyLink.RootPortBus << 20) + ((UINT32)BixbyLinkInstance->EarlyLink.RootPortDevice << 15) + ((UINT32)BixbyLinkInstance->EarlyLink.RootPortFunction << 12);
      DEBUG((DEBUG_INFO, "KER-PEI-%a-Set MainTablePtr->BxbBusNumDetEn = 0x%02X\n", __FUNCTION__, MainTablePtr->BxbBusNumDetEn));
      DEBUG((DEBUG_INFO, "KER-PEI-%a-Set MainTablePtr->BxbBrMmioBaseAddr = 0x%08X\n", __FUNCTION__, MainTablePtr->BxbBrMmioBaseAddr));
    }
  }

  MainTablePtr->CurrentPlatformId = AmdCpmOemTablePpiPtr->PlatformId;
  MainTablePtr->Service.Pointer   = (VOID*)PeiServices;
  CommonFunctionPtr               = &(AmdCpmTablePpiPtr->CommonFunction);

  AmdCpmTablePpiPtr->Revision     = (AMD_CPM_KERNEL_VERSION << 16) + AmdCpmOemTablePpiPtr->Revision;
  AmdCpmTablePpiPtr->MainTablePtr = MainTablePtr;

  ((AMD_CPM_HOB_HEADER*)HobBuffer)->Revision = (UINT32)AmdCpmTablePpiPtr->Revision;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-AmdCpmPeiRegisterKernel\n", __FUNCTION__));
  AmdCpmPeiRegisterKernel (AmdCpmTablePpiPtr);
  DEBUG((DEBUG_INFO, "KER-PEI-%a-CpmRegisterBaseIo\n", __FUNCTION__));
  CpmRegisterBaseIo (AmdCpmTablePpiPtr);
  DEBUG((DEBUG_INFO, "KER-PEI-%a-CpmRegisterCpu\n", __FUNCTION__));
  CpmRegisterCpu (AmdCpmTablePpiPtr);
  DEBUG((DEBUG_INFO, "KER-PEI-%a-CpmRegisterFch\n", __FUNCTION__));
  CpmRegisterFch (AmdCpmTablePpiPtr);

  DEBUG((DEBUG_INFO, "KER-PEI-%a-AmdCpmCoreTopologyInit\n", __FUNCTION__));
  AmdCpmCoreTopologyInit (AmdCpmTablePpiPtr);

  DEBUG((DEBUG_INFO, "KER-PEI-%a-AmdCpmLoadPreInitTable\n", __FUNCTION__));
  AmdCpmLoadPreInitTable (AmdCpmTablePpiPtr, CPM_PRE_INIT_STAGE_0);
  DEBUG((DEBUG_INFO, "KER-PEI-%a-AmdCpmPlatformIdInit\n", __FUNCTION__));
  AmdCpmPlatformIdInit (AmdCpmTablePpiPtr);

  DEBUG((DEBUG_INFO, "KER-PEI-%a-GetTablePtr-CPM_SIGNATURE_SAVE_CONTEXT\n", __FUNCTION__));
  SaveContextTablePtr = CommonFunctionPtr->GetTablePtr (AmdCpmTablePpiPtr, CPM_SIGNATURE_SAVE_CONTEXT);
  if (SaveContextTablePtr) {
    if (SaveContextTablePtr->BufferSize < sizeof (AMD_CPM_PREDEFINED_SAVE_CONTEXT)) {
      DEBUG((DEBUG_INFO, "KER-PEI-%a-RemoveTable-SaveContextTablePtr\n", __FUNCTION__));
      CommonFunctionPtr->RemoveTable (AmdCpmTablePpiPtr, SaveContextTablePtr);
    } else {
      DEBUG((DEBUG_INFO, "KER-PEI-%a-SetSaveContext-BootMode=%02X\n", __FUNCTION__, CpmBootMode));
      CommonFunctionPtr->SetSaveContext (AmdCpmTablePpiPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, BootMode), CpmBootMode);
      if (CpmBootMode == CPM_BOOT_MODE_S0) {
        DEBUG((DEBUG_INFO, "KER-PEI-%a-SetSaveContext-DeviceStateOnResume = 0\n", __FUNCTION__));
        CommonFunctionPtr->SetSaveContext (AmdCpmTablePpiPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, DeviceStateOnResume), 0);
      }
    }
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-NotifyPpi-gEfiPeiPromotoryGpioPpiGuid\n", __FUNCTION__));
  Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmPtInitPeimNotify);

  DEBUG((DEBUG_INFO, "KER-PEI-%a-NotifyPpi-gAmdNbioPcieTrainingDonePpiGuid\n", __FUNCTION__));
  Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmBxbInitPeimNotify);

  Status = (*PeiServices)->AllocatePool (
                              PeiServices,
                              sizeof (EFI_PEI_PPI_DESCRIPTOR),
                              (VOID**)&PpiListCpmTablePtr
                              );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-8-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  PpiListCpmTablePtr->Flags   = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiListCpmTablePtr->Guid    = &gAmdCpmTablePpiGuid;
  PpiListCpmTablePtr->Ppi     = AmdCpmTablePpiPtr;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-InstallPpi-gAmdCpmTablePpiGuid %08X\n", __FUNCTION__, (UINT32)(UINTN)AmdCpmTablePpiPtr));
  Status = (*PeiServices)->InstallPpi (
                              PeiServices,
                              PpiListCpmTablePtr
                              );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-End-9-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }

  RECORD_TIME_PEI (EndAmdCpmInitPeimDriver);

  DEBUG((DEBUG_INFO, "KER-PEI-%a-End-A-Status=%r\n", __FUNCTION__, Status));
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize Core Topology Table
 *
 * @param[in]     CpmTablePpiPtr Point to Ppi
 *
 */
VOID
EFIAPI
AmdCpmCoreTopologyInit (
  IN       AMD_CPM_TABLE_PPI          *CpmTablePpiPtr
  )
{
  AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI          *FabricTopologyServicesPpi;
  AMD_CPM_CORE_TOPOLOGY_TABLE         *AmdCpmCoreTopologyPtr;
  EFI_STATUS                          Status;
  UINTN                               NumberOfProcessors;
  UINTN                               TotalNumberOfDie;
  UINTN                               TotalNumberOfRootBridges;
  UINTN                               SystemFabricID;
  UINTN                               BusNumberBase;
  UINTN                               BusNumberLimit;
  UINTN                               SocketIndex;
  UINTN                               DieIndex;
  UINTN                               RootBridgeIndex;
  UINTN                               Index;
  EFI_PEI_SERVICES                    **PeiServices;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-Start\n", __FUNCTION__));
  AmdCpmCoreTopologyPtr = NULL;
  if (CpmTablePpiPtr->ChipId.Sb >= CPM_FCH_REVISION_ID_ZP) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-AddTable-AMD_CPM_CORE_TOPOLOGY_TABLE\n", __FUNCTION__));
    AmdCpmCoreTopologyPtr = CpmTablePpiPtr->CommonFunction.AddTable (CpmTablePpiPtr, &gCpmCoreTopologyTable);
    AmdCpmCoreTopologyPtr->CoreList[0].Socket = CPM_END_OF_TABLE;
  }

  if (AmdCpmCoreTopologyPtr) {
    PeiServices = (EFI_PEI_SERVICES**) CpmTablePpiPtr->MainTablePtr->Service.Pointer;
    // Get number of sockets from CPU topology PPI
    // Locate CPU topology PPI
    Status = (**PeiServices).LocatePpi (
                               PeiServices,
                               &gAmdFabricTopologyServices2PpiGuid,
                               0,
                               NULL,
                               &FabricTopologyServicesPpi
                               );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "KER-PEI-%a-End-1-EFI_ERROR\n", __FUNCTION__));
      return;
    }

    DEBUG((DEBUG_INFO, "KER-PEI-%a-FabricTopologyServicesPpi->GetSystemInfo\n", __FUNCTION__));
    FabricTopologyServicesPpi->GetSystemInfo (&NumberOfProcessors, &TotalNumberOfDie, &TotalNumberOfRootBridges, NULL, NULL);
    DEBUG((DEBUG_INFO, "  NumberOfProcessors = 0x%X, TotalNumberOfDie = 0x%X, TotalNumberOfRootBridges = 0x%X\n", NumberOfProcessors, TotalNumberOfDie, TotalNumberOfRootBridges));

    DEBUG((DEBUG_INFO, "KER-PEI-%a-AMD_CPM_CORE_TOPOLOGY_TABLE-Start\n\n", __FUNCTION__));
    for (SocketIndex = 0, Index = 0; SocketIndex < NumberOfProcessors; SocketIndex++) {
      FabricTopologyServicesPpi->GetProcessorInfo (SocketIndex, &TotalNumberOfDie, &TotalNumberOfRootBridges);
      for (DieIndex = 0; DieIndex < TotalNumberOfDie; DieIndex++) {
        FabricTopologyServicesPpi->GetDieInfo (SocketIndex, DieIndex, &TotalNumberOfRootBridges, NULL, NULL);
        for (RootBridgeIndex = 0; RootBridgeIndex < TotalNumberOfRootBridges; RootBridgeIndex++) {
          FabricTopologyServicesPpi->GetRootBridgeInfo (SocketIndex, DieIndex, RootBridgeIndex, &SystemFabricID, &BusNumberBase, &BusNumberLimit, NULL, NULL, NULL);
          AmdCpmCoreTopologyPtr->CoreList[Index + RootBridgeIndex].Socket  = (UINT8) SocketIndex;
          AmdCpmCoreTopologyPtr->CoreList[Index + RootBridgeIndex].Die     = (UINT8) DieIndex;
          AmdCpmCoreTopologyPtr->CoreList[Index + RootBridgeIndex].Bridge  = (UINT8) RootBridgeIndex;
          AmdCpmCoreTopologyPtr->CoreList[Index + RootBridgeIndex].Bus     = (UINT8) BusNumberBase;
          DEBUG((DEBUG_INFO, "    CoreList[%X]: Socket = %X, Die = %X, Bridge = 0x%X, Bus = 0x%X\n", (Index + RootBridgeIndex), SocketIndex, DieIndex, RootBridgeIndex, BusNumberBase));
        }
        Index += TotalNumberOfRootBridges;
      }
    }
    AmdCpmCoreTopologyPtr->Header.Attribute |= (UINT32) ((NumberOfProcessors << 16) + (TotalNumberOfDie << 12) + (TotalNumberOfRootBridges << 8));
    AmdCpmCoreTopologyPtr->CoreList[Index].Socket = CPM_END_OF_TABLE;
    DEBUG((DEBUG_INFO, "    Header.Attribute     = 0x%X\n", AmdCpmCoreTopologyPtr->Header.Attribute));
    DEBUG((DEBUG_INFO, "    CoreList[%X].Socket  = CPM_END_OF_TABLE\n", Index));
    DEBUG((DEBUG_INFO, "\nKER-PEI-%a-AMD_CPM_CORE_TOPOLOGY_TABLE-End\n", __FUNCTION__));
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-End-2-OK\n", __FUNCTION__));
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load Pre Init Table.
 *
 * @param[in]     This           Pointer to AMD CPM TABLE Ppi
 * @param[in]     Stage          Stage number to initialize the registers
 *
 */
VOID
EFIAPI
AmdCpmLoadPreInitTable (
  IN       VOID                   *This,
  IN       UINT8                  Stage
  )
{
  AMD_CPM_PRE_INIT_TABLE          *PreInitTablePtr;
  AMD_CPM_GPIO_INIT_TABLE         *GpioPreInitTablePtr;
  AMD_CPM_PRE_SETTING_ITEM        *TablePtr;
  AMD_CPM_TABLE_PPI               *CpmTablePpiPtr;
  AMD_CPM_GPIO_ITEM               *GpioTablePtr;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-Start\n", __FUNCTION__));
  CpmTablePpiPtr  = (AMD_CPM_TABLE_PPI*) This;
  PreInitTablePtr = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_PRE_INIT);
  GpioPreInitTablePtr = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_GPIO_PRE_INIT);

  if (PreInitTablePtr) {
    TablePtr = &PreInitTablePtr->Item[0];
    while (TablePtr->Type != CPM_END_OF_TABLE ) {
      if (TablePtr->Stage == Stage) {
        if ( TablePtr->Purpose != INACTIVE_ITEMS &&
             ((TablePtr->Purpose == NORMAL_PURPOSE) || (TablePtr->Purpose == CpmTablePpiPtr->MainTablePtr->ModernStandbyEn)) ) {
          switch (TablePtr->Type) {
          case 0:
            CpmTablePpiPtr->CommonFunction.MmioAndThenOr8 ( CpmTablePpiPtr->MainTablePtr->AcpiMemIoBaseAddr + (TablePtr->Select << 0x08) + TablePtr->Offset,
                                                            TablePtr->AndMask,
                                                            TablePtr->OrMask
                                                          );
            break;
          case 1:
          case 2:
            CpmTablePpiPtr->CommonFunction.PciAndThenOr8 ( CpmTablePpiPtr,
                                                           0x00,
                                                           (TablePtr->Select & 0xF8) >> 3,
                                                            TablePtr->Select & 7,
                                                            TablePtr->Offset + (TablePtr->Type == 1? 0 : 0x100),
                                                            TablePtr->AndMask,
                                                            TablePtr->OrMask
                                                          );
            break;
          }
        }
      }
      TablePtr ++;
    }
  }

  // Only need to be set once, otherwise it will override other's modification.
  if (GpioPreInitTablePtr && (Stage == CPM_PRE_INIT_STAGE_0)) {
    GpioTablePtr = &GpioPreInitTablePtr->GpioList[0];
    while (GpioTablePtr->Pin != CPM_END_OF_TABLE ) {
      CpmTablePpiPtr->CommonFunction.SetGpio ( \
        CpmTablePpiPtr, \
        (UINT32)((((UINT8)GpioTablePtr->Misc) << 24) | (UINT16)GpioTablePtr->Pin | (((UINT8)(GpioTablePtr->Misc >> 8)) << 16)), \
        GpioTablePtr->Setting.Raw \
        );
      GpioTablePtr++;
    }
  }
  DEBUG((DEBUG_INFO, "KER-PEI-%a-End\n", __FUNCTION__));
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize Current Platform Id.
 *
 * @param[in]     CpmTablePpiPtr Point to Ppi
 *
 */
VOID
EFIAPI
AmdCpmPlatformIdInit (
  IN       AMD_CPM_TABLE_PPI           *CpmTablePpiPtr
  )
{
  AMD_CPM_PLATFORM_ID_TABLE           *PlatformIdTablePtr;
  AMD_CPM_PLATFORM_ID_TABLE2          *PlatformIdTable2Ptr;
  AMD_CPM_PLATFORM_ID_TABLE3          *PlatformIdTable3Ptr;
  AMD_CPM_PLATFORM_ID_CONVERT_TABLE   *PlatformIdConvertTablePtr;
  UINT16                              PlatformId;
  UINT8                               Index;
  UINT8                               CpuRevisionId;
  EFI_STATUS                          Status;
  UINT8                               Data;
  UINT16                              PlatformIdOverride;
  UINT8                               SmbusSelect;
  UINT8                               SmbusBit;
  UINT8                               SmbusBitStart;
  UINT8                               RegisterOffset;
  UINT8                               StartBit;
  UINT8                               BitLength;
  UINT8                               PlatformIdBitOffset;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-Start\n", __FUNCTION__));
  PlatformId                  = CpmTablePpiPtr->MainTablePtr->CurrentPlatformId;
  PlatformIdTablePtr          = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_GET_PLATFORM_ID);
  PlatformIdTable2Ptr         = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_GET_PLATFORM_ID2);
  PlatformIdTable3Ptr         = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_GET_PLATFORM_ID3);
  PlatformIdConvertTablePtr   = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_GET_PLATFORM_ID_CONVERT);
  CpuRevisionId               = CpmTablePpiPtr->CommonFunction.GetCpuRevisionId (CpmTablePpiPtr);

  PlatformIdOverride          = CFG_CPM_PLATFORM_ID_OVERRIDE;
  DEBUG((DEBUG_INFO, "KER-PEI-%a-CpuRevisionId      = %02X\n", __FUNCTION__, CpuRevisionId));
  DEBUG((DEBUG_INFO, "KER-PEI-%a-CurrentPlatformId  = %04X\n", __FUNCTION__, PlatformId));
  DEBUG((DEBUG_INFO, "KER-PEI-%a-PlatformIdOverride = %04X\n", __FUNCTION__, PlatformIdOverride));
  if (PlatformIdOverride != 0xFFFF) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-PlatformIdOverride != 0xFFFF\n", __FUNCTION__));
    PlatformId = PlatformIdOverride;
  } else if (PlatformIdTablePtr) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-PlatformIdTablePtr exist\n", __FUNCTION__));
    PlatformId = 0;
    for (Index = 0; Index < AMD_PLATFORM_ID_TABLE_SIZE; Index++) {
      if (PlatformIdTablePtr->GpioPin[Index] == CPM_END_OF_TABLE) {
        break;
      }
      PlatformId = PlatformId + (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, (UINT32) PlatformIdTablePtr->GpioPin[Index]) << Index);
    }
  } else if (PlatformIdTable2Ptr) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-PlatformIdTable2Ptr exist\n", __FUNCTION__));
    PlatformId = 0;
    SmbusBitStart = 0;
    for (Index = 0; Index < AMD_PLATFORM_ID_TABLE_SIZE; Index++) {
      if (PlatformIdTable2Ptr->GpioPin[Index].SmbusSelect == CPM_END_OF_TABLE) {
        break;
      }
      Data = 0;
      // Config I2C MUX
      if (PlatformIdTable2Ptr->GpioPin[Index].SmbusSelect & 0x80) {
        SmbusBitStart = Index + 1;
        SmbusSelect = PlatformIdTable2Ptr->GpioPin[Index].SmbusSelect & 0x7F;
        Status = CpmTablePpiPtr->CommonFunction.ReadSmbus (CpmTablePpiPtr, SmbusSelect, (PlatformIdTable2Ptr->GpioPin[Index].SmbusAddress >> 1), PlatformIdTable2Ptr->GpioPin[Index].SmbusOffset, 1, &Data);
        if (!EFI_ERROR (Status)) {
          // Set this Bit
          if (PlatformIdTable2Ptr->GpioPin[Index].SmbusBit & 0x80) {
            SmbusBit = PlatformIdTable2Ptr->GpioPin[Index].SmbusBit & 0x7F;
            Data |= (UINT8)(1 << SmbusBit);
          }
          // Clear this Bit
          else {
            SmbusBit = PlatformIdTable2Ptr->GpioPin[Index].SmbusBit & 0x7F;
            Data &= (UINT8)(~(1 << SmbusBit));
          }
          Status = CpmTablePpiPtr->CommonFunction.WriteSmbus (CpmTablePpiPtr, SmbusSelect, (PlatformIdTable2Ptr->GpioPin[Index].SmbusAddress >> 1), PlatformIdTable2Ptr->GpioPin[Index].SmbusOffset, 1, &Data);
          if (EFI_ERROR (Status)) {
            continue;
          }
        }
      } else {
        // Read Platform Id
        Status = CpmTablePpiPtr->CommonFunction.ReadSmbus (CpmTablePpiPtr, PlatformIdTable2Ptr->GpioPin[Index].SmbusSelect, (PlatformIdTable2Ptr->GpioPin[Index].SmbusAddress >> 1), PlatformIdTable2Ptr->GpioPin[Index].SmbusOffset, 1, &Data);
        if (!EFI_ERROR (Status)) {
          Data = (Data >> PlatformIdTable2Ptr->GpioPin[Index].SmbusBit) & 0x01;
          PlatformId = PlatformId + (Data << (Index - SmbusBitStart));
        }
      }
    }
  } else if (PlatformIdTable3Ptr) {
    DEBUG((DEBUG_INFO, "KER-PEI-%a-PlatformIdTable3Ptr exist\n", __FUNCTION__));
    PlatformId = 0;
    for (Index = 0; Index < AMD_PLATFORM_ID_TABLE_SIZE; Index++) {
      RegisterOffset      = PlatformIdTable3Ptr->GpioPin[Index].RegisterOffset;
      StartBit            = PlatformIdTable3Ptr->GpioPin[Index].StartBit;
      BitLength           = PlatformIdTable3Ptr->GpioPin[Index].BitLength;
      PlatformIdBitOffset = PlatformIdTable3Ptr->GpioPin[Index].PlatformIdBitOffset;
      if ((RegisterOffset == CPM_END_OF_TABLE) && (StartBit == 0xFF) && (BitLength == 0xFF) && (PlatformIdBitOffset == 0xFF)) {
        break;
      }
      if ((StartBit + BitLength) > 8) {
        break;
      }
      Status = CpmTablePpiPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &RegisterOffset, &Data);
      if (!EFI_ERROR (Status)) {
        Data = (Data >> StartBit) & (0xFF >> (8 - BitLength));
        PlatformId |= ((UINT16)Data) << PlatformIdBitOffset;
      }
    }
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-CPM_SET_ORIGINAL_PLATFORM_ID (PlatformId = %04X);\n", __FUNCTION__, PlatformId));
  CPM_SET_ORIGINAL_PLATFORM_ID (PlatformId);

  if (PlatformIdConvertTablePtr) {
    for (Index = 0; Index < AMD_PLATFORM_ID_CONVERT_TABLE_SIZE; Index++) {
      if (PlatformIdConvertTablePtr->Item[Index].CpuRevisionId == CPM_END_OF_TABLE) {
        PlatformId = 0;
        break;
      }
      if ((PlatformId & PlatformIdConvertTablePtr->Item[Index].OriginalIdMask) == PlatformIdConvertTablePtr->Item[Index].OriginalId &&
          CpuRevisionId == PlatformIdConvertTablePtr->Item[Index].CpuRevisionId) {
        PlatformId = PlatformIdConvertTablePtr->Item[Index].ConvertedId;
        break;
      }
    }
    DEBUG((DEBUG_INFO, "KER-PEI-%a-PlatformId         = %04X\n", __FUNCTION__, PlatformId));
  }
  if (PlatformId >= 32) {
    PlatformId = 0;
    DEBUG((DEBUG_INFO, "KER-PEI-%a-PlatformId         = %04X\n", __FUNCTION__, PlatformId));
  }

  DEBUG((DEBUG_INFO, "KER-PEI-%a-CPM_SET_CURRENT_PLATFORM_ID (PlatformId = %04X);\n", __FUNCTION__, PlatformId));
  CPM_SET_CURRENT_PLATFORM_ID (PlatformId);

  CpmTablePpiPtr->MainTablePtr->CurrentPlatformId = PlatformId;
  DEBUG((DEBUG_INFO, "KER-PEI-%a-PlatformId         = %04X\n", __FUNCTION__, PlatformId));
  DEBUG((DEBUG_INFO, "KER-PEI-%a-End\n", __FUNCTION__));
}

/*---------------------------------------------------------------------------------------*/
/**
 * Register Common Kenerl functions at the AmdCpmInitPeim entry point.
 *
 * This function registers CPM common kernel functions in AmdCpmTablePpi
 * at AmdCpmInitPeim.
 *
 *  @param[in]   This                Pointer to Ppi.
 *
 */
VOID
EFIAPI
AmdCpmPeiRegisterKernel (
  IN       VOID                        *This
  )
{
  AMD_CPM_COMMON_FUNCTION             *CommonFunctionPtr;
  CommonFunctionPtr                   = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-Start\n", __FUNCTION__));
  CommonFunctionPtr->GetTablePtr      = CpmGetTablePtr;
  CommonFunctionPtr->GetTablePtr2     = CpmGetTablePtr2;
  CommonFunctionPtr->AddTable         = CpmAddTable;
  CommonFunctionPtr->RemoveTable      = CpmRemoveTable;
  CommonFunctionPtr->RelocateTable    = CpmRelocateTableList;
  CommonFunctionPtr->CopyMem          = CpmCopyMem;
  CommonFunctionPtr->LoadPreInitTable = AmdCpmLoadPreInitTable;

  DEBUG((DEBUG_INFO, "KER-PEI-%a-End\n", __FUNCTION__));
  return;
}
