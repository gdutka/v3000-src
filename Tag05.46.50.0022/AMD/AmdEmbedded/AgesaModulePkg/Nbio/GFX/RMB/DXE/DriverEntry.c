/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AmdNbioGfxRMBDxe Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  AmdNbioGfxRMBDxe
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */
#include <PiDxe.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Guid/GnbPcieInfoHob.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/HobList.h>
#include <Guid/GnbIntegratedSysInfoV4Hob.h>
#include <Addendum/Apcb/Inc/RMB/ApcbGnbGroup.h>
#include <Addendum/Apcb/Inc/RMB/ApcbV3TokenUid.h>
#include <Library/ApcbLibV3.h>
#include <Addendum/Apcb/Inc/RMB/ApcbV3Priority.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/GnbUmaCarveOutDefaultVariable.h>
#include <Library/ApobCommonServiceLib.h>

#define FILECODE NBIO_GFX_RMB_DXE_DRIVERENTRY_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern EFI_GUID gPspFlashAccSmmCommReadyProtocolGuid;


/*----------------------------------------------------------------------------------------
 *           T A B L E S
 *----------------------------------------------------------------------------------------
 */

#define APCB_GROUP_GNB                0x1705
#define IOMAP_REGCF9                  0xCF9
#define MAX_IP_DISCOVERY_SIZE         (64 * 1024) //64KB is max IP discovery size
#define NumberOfDdiPort               5
#define APCB_TYPE_PURPOSE_NORMAL      5
#define ResetTypeCold                 0
#define MAX_DMCUB_SIZE                (256 * 1024) //256KB is max DMCUB size

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
SyncDpPhyStaicTable (
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Assign DP Static table display path settings
 *
 *
 *
 * @param[in] DisplayPath            Display path
 */
VOID
DisplayPathDpStaticTableAssigmnet (
  IN OUT      APCB_STATIC_INFO_V1_0_CONNECT_ENTRY   **DisplayPath,
  IN          NBIO_Frame_Buffer_DATA_V4_HOB         *FrambufferHob
  )
{
  APCB_STATIC_INFO_V1_0_CONNECT_ENTRY        *LocalDisplayPath;
  UINT8                                      Index;
//  UINT8                                      LocalChannelMapping;

  Index = 0;
//  LocalChannelMapping = 0;
  LocalDisplayPath = *DisplayPath;

  for (Index = 0; Index < NumberOfDdiPort; Index++) {
//    LocalChannelMapping = FrambufferHob->AtomFusionSystemInfoV4.sIntegratedSysInfo.extdispconninfo.sPath[Index].ucChannelMapping;
    LocalDisplayPath->extdispconninfo.path[Index].device_tag = FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.extdispconninfo.sPath[Index].usDeviceTag;
    LocalDisplayPath->extdispconninfo.path[Index].device_acpi_enum = FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.extdispconninfo.sPath[Index].usDeviceACPIEnum;
    LocalDisplayPath->extdispconninfo.path[Index].connectorobjid =FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.extdispconninfo.sPath[Index].usDeviceConnector;
    LocalDisplayPath->extdispconninfo.path[Index].auxddclut_index = FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.extdispconninfo.sPath[Index].ucExtAUXDDCLutIndex;
    LocalDisplayPath->extdispconninfo.path[Index].hpdlut_index = FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.extdispconninfo.sPath[Index].ucExtHPDPINLutIndex;
    LocalDisplayPath->extdispconninfo.path[Index].ext_encoder_objid = FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.extdispconninfo.sPath[Index].usExtEncoderObjId;
    LocalDisplayPath->extdispconninfo.path[Index].channelmapping = FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.extdispconninfo.sPath[Index].ucChannelMapping;
    LocalDisplayPath->extdispconninfo.path[Index].chpninvert = FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.extdispconninfo.sPath[Index].ucChPNInvert;
    LocalDisplayPath->extdispconninfo.path[Index].caps = FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.extdispconninfo.sPath[Index].usCaps;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Dump Dp Phy Static display path settings
 *
 *
 *
 * @param[in] DisplayPath            Display path
 */
VOID
LocalDpPhyStaticDumpDisplayPath (
  IN       APCB_STATIC_INFO_V1_0_CONNECT_ENTRY            *DisplayPath
  )
{
  UINT8                                 Index;

  for (Index = 0; Index < NumberOfDdiPort; Index++) {
    IDS_HDT_CONSOLE (GNB_TRACE, "     usDeviceTag = 0x%x\n",
     DisplayPath->extdispconninfo.path[Index].device_tag
     );
    IDS_HDT_CONSOLE (GNB_TRACE, "     usDeviceACPIEnum = 0x%x\n",
     DisplayPath->extdispconninfo.path[Index].device_acpi_enum
     );
    IDS_HDT_CONSOLE (GNB_TRACE, "     usDeviceConnector = 0x%x\n",
     DisplayPath->extdispconninfo.path[Index].connectorobjid
     );
    IDS_HDT_CONSOLE (GNB_TRACE, "     ucExtAUXDDCLutIndex = 0x%x\n",
     DisplayPath->extdispconninfo.path[Index].auxddclut_index
     );
    IDS_HDT_CONSOLE (GNB_TRACE, "     ucExtHPDPINLutIndex = 0x%x\n",
     DisplayPath->extdispconninfo.path[Index].hpdlut_index
     );
    IDS_HDT_CONSOLE (GNB_TRACE, "     usExtEncoderObjId = 0x%x\n",
     DisplayPath->extdispconninfo.path[Index].ext_encoder_objid
     );
    IDS_HDT_CONSOLE (GNB_TRACE, "     ucChannelMapping = 0x%x\n",
     DisplayPath->extdispconninfo.path[Index].channelmapping
     );
    IDS_HDT_CONSOLE (GNB_TRACE, "     ucChPNInvert = 0x%x\n",
     DisplayPath->extdispconninfo.path[Index].chpninvert
     );
    IDS_HDT_CONSOLE (GNB_TRACE, "     usCaps = 0x%x\n",
     DisplayPath->extdispconninfo.path[Index].caps
     );
  }
}

EFI_STATUS
CompareBetweenApcbAndUMA (
  IN   NBIO_Frame_Buffer_DATA_V4_HOB        *FrambufferHob
  )
{
  EFI_STATUS                           Status;
  APCB_STATIC_INFO_V1_0_CONNECT_ENTRY  *DisplayConnect;
  UINT32                               TypeDataSize;
  AMD_APCB_SERVICE_PROTOCOL            *mApcbDxeServiceProtocol;
  UINT8                                Purpose;
  UINT8                                Index;
  BOOLEAN                              ApcbUpdate;
  EFI_HANDLE                           Handle;

  IDS_HDT_CONSOLE (MAIN_FLOW, "CompareBetweenApcbAndUMA Entry\n");
  Status = EFI_SUCCESS;
  Handle = NULL;
  Index = 0;
  Purpose = APCB_TYPE_PURPOSE_NORMAL;
  ApcbUpdate = TRUE;

  // //Environment: 0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO, 4 - GC Lite
  if (PcdGet32 (PcdAmdEnvironmentFlag) != 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "SoC emulation\n");
    return EFI_SUCCESS;
  }

  //
  // Get APCB data
  //
  Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &mApcbDxeServiceProtocol);
  if (Status != EFI_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "Locate fail\n");
    ApcbUpdate = 0;
  }

  Status = mApcbDxeServiceProtocol->ApcbGetType (mApcbDxeServiceProtocol, &Purpose, APCB_GROUP_GNB,
    (UINT16)APCB_GNB_TYPE_DP_PHY_STATIC_CONNECT, 0, (UINT8 **)&DisplayConnect, &TypeDataSize);
  if (Status != EFI_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "get fail\n");
    ApcbUpdate = 0;
  }

  if (ApcbUpdate == TRUE) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "Start to compare \n");
    LocalDpPhyStaticDumpDisplayPath (DisplayConnect);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Start to compare phase 2\n");

    // magic number
    for (Index = 0; Index < NumberOfDdiPort; Index++) {
      // To compare the display connector type between hob and APCB
      IDS_HDT_CONSOLE (MAIN_FLOW, "FrambufferHob sPath[%x].usDeviceConnector: %lx\n",Index, FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.extdispconninfo.sPath[Index].usDeviceConnector);
      IDS_HDT_CONSOLE (MAIN_FLOW, "ACPB DisplayConnect[%x].connectorobjid:  %lx\n",Index, DisplayConnect->extdispconninfo.path[Index].connectorobjid);
      if (FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.extdispconninfo.sPath[Index].usDeviceConnector != DisplayConnect->extdispconninfo.path[Index].connectorobjid ||
        FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.extdispconninfo.sPath[Index].usCaps != DisplayConnect->extdispconninfo.path[Index].caps) {
        DisplayPathDpStaticTableAssigmnet (&DisplayConnect, FrambufferHob);

        //
        // Set APCB data
        //
        Status = mApcbDxeServiceProtocol->ApcbSetType (mApcbDxeServiceProtocol, APCB_TYPE_PURPOSE_NORMAL, APCB_GROUP_GNB,
          (UINT16)APCB_GNB_TYPE_DP_PHY_STATIC_CONNECT, 0, (UINT8 *)DisplayConnect, TypeDataSize);
          IDS_HDT_CONSOLE (MAIN_FLOW, "Set APCB successful! Status %d \n", Status);
        Status = mApcbDxeServiceProtocol->ApcbFlushData (mApcbDxeServiceProtocol);
          IDS_HDT_CONSOLE (MAIN_FLOW, "Set APCB flash successful! Status %d \n", Status);
        //
        // Get APCB data
        //
        Status = mApcbDxeServiceProtocol->ApcbGetType (mApcbDxeServiceProtocol, &Purpose, APCB_GROUP_GNB,
          (UINT16)APCB_GNB_TYPE_DP_PHY_STATIC_CONNECT, 0, (UINT8 **)&DisplayConnect, &TypeDataSize);

        LocalDpPhyStaticDumpDisplayPath (DisplayConnect);
        //
        //issue cf9 reset
        //
        IDS_HDT_CONSOLE (MAIN_FLOW, "Issue cf9 reset\n");
        gRT->ResetSystem (ResetTypeCold, 0, 0, NULL);
//        IoWrite8 (IOMAP_REGCF9, 0x0E);
       break;
      }

      IDS_HDT_CONSOLE (MAIN_FLOW, "No need to reset for Display APCB update\n");
    }
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "CompareBetweenApcbAndUMA Exit\n");
  return Status;
}

VOID
SyncDpPhyStaicTable (
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  )
{
  EFI_STATUS                           Status;
  VOID                                 *Interface;
  NBIO_Frame_Buffer_DATA_V4_HOB        *FrambufferHob;

  IDS_HDT_CONSOLE (MAIN_FLOW, "SyncDpPhyStaicTable Entry\n");

  Status = gBS->LocateProtocol (
                &gPspFlashAccSmmCommReadyProtocolGuid,
                NULL,
                &Interface
                );
  if (EFI_ERROR (Status)) {
    return;
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "Locate gPspFlashAccSmmCommReadyProtocolGuid  success\n");

  gBS->CloseEvent(Event);

  FrambufferHob = Context;
  Status = CompareBetweenApcbAndUMA (FrambufferHob);
  IDS_HDT_CONSOLE (MAIN_FLOW, "Status of CompareBetweenApcbAndUMA %d\n", Status);

  IDS_HDT_CONSOLE (MAIN_FLOW, "SyncDpPhyStaicTable Exit\n");

}

VOID
EFIAPI
SaveCarveOutDefaultVar (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  UINT8    *ApcbUmaModeDefault;

  ApcbUmaModeDefault = (UINT8*)Context;
  IDS_HDT_CONSOLE (MAIN_FLOW, "ApcbUmaModeDefault %x\n", *ApcbUmaModeDefault);

  IDS_HDT_CONSOLE (MAIN_FLOW, "Save to EFI variable\n");
  gRT->SetVariable (
                    AMD_UMA_CARVE_OUT_DEFAULT_VARIABLE,
                    &gAmdUmaCarveOutDefaultVarGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    1,
                    ApcbUmaModeDefault
                    );
  gBS->CloseEvent (Event);

}

EFI_STATUS
EFIAPI
AmdNbioGfxRMBDxeEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                           Status;
  NBIO_Frame_Buffer_DATA_V4_HOB        *FrambufferHob;
  UINT64                               DestinationAddress;
  VOID                                 *Registration;
  ATOM_COMMON_TABLE_HEADER             *TableHeaderPtr;
  ATOM_N6_DISPLAY_PHY_TUNING_SET       *DispPhyTuningDataPtr;
  BOOLEAN                              IsN6TableReady;
  AMD_APCB_SERVICE_PROTOCOL            *ApcbDxeServiceProtocol;
  UINT8                                ApcbPurpose;
  UINT8                                ApcbUmaMode;
  UINTN                                VariableSize;
  UINT8                                ApcbUmaModeDefault;
  EFI_EVENT                            ReadyToBootEvt;
  BOOLEAN                              ApcbRecoveryFlag;

  AGESA_TESTPOINT (TpNbioGfxDxeEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "AmdNbioGfxRMBDxeEntry\n");

  Status = EFI_SUCCESS;
  IsN6TableReady = FALSE;
  ApcbRecoveryFlag = FALSE;
  //
  // Get information data from HOB.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &FrambufferHob);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  FrambufferHob = GetNextGuidHob (&gGnbIntegratedSysInfoV4HobGuid, FrambufferHob);
  if (FrambufferHob == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Get APCB data
  //
  FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.UMACarveoutIndexMax = 1;
  FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.UMACarveoutID[0] = 1;
  FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.UMACarveoutID[1] = 2;
  Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &ApcbDxeServiceProtocol);
  if (Status == EFI_SUCCESS) {
    Status = ApobGetApcbRecoveryFlag (&ApcbRecoveryFlag);
    if (ApcbRecoveryFlag != TRUE) { // Non APCB Recovery Flag
      //
      // UMA Carve Out Control
      //
      Status = ApcbDxeServiceProtocol->ApcbGetToken8 (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_UMAMODE, &ApcbUmaMode);
      IDS_HDT_CONSOLE (MAIN_FLOW, "Get APCB_TOKEN_UID_UMAMODE: 0x%x Purpose:0x%x\n", ApcbUmaMode, ApcbPurpose);
      if (!EFI_ERROR(Status)) {
        FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.UMACarveoutIndex = (ApcbUmaMode == 3) ? 2 : 1;

        if (APCB_PRIORITY_LEVEL_DEFAULT == ApcbPurpose) {
          IDS_HDT_CONSOLE (MAIN_FLOW, "Getting from default APCB.\n");
          FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.UMACarveoutIndexDefault = (ApcbUmaMode == 3) ? 2 : 1;
          VariableSize = sizeof (UINT8);
          Status = gRT->GetVariable (
                    AMD_UMA_CARVE_OUT_DEFAULT_VARIABLE,
                    &gAmdUmaCarveOutDefaultVarGuid,
                    NULL,
                    &VariableSize,
                    (VOID *) &ApcbUmaModeDefault
                    );
          if (EFI_ERROR(Status) || ((Status == EFI_SUCCESS) && (ApcbUmaModeDefault != ApcbUmaMode)) ) {
            IDS_HDT_CONSOLE (MAIN_FLOW, "Create ReadyToBoot for saving variable\n");
            Status = EfiCreateEventReadyToBootEx (
               TPL_CALLBACK,
               SaveCarveOutDefaultVar,
               (UINT8*)&ApcbUmaMode,
               &ReadyToBootEvt
               );
          }
        } else {
          IDS_HDT_CONSOLE (MAIN_FLOW, "Get from updated ACPB, need to get the default from EFI variable\n");
          VariableSize = sizeof (UINT8);
          Status = gRT->GetVariable (
                    AMD_UMA_CARVE_OUT_DEFAULT_VARIABLE,
                    &gAmdUmaCarveOutDefaultVarGuid,
                    NULL,
                    &VariableSize,
                    (VOID *) &ApcbUmaModeDefault
                    );
           if (!EFI_ERROR (Status)) {
              IDS_HDT_CONSOLE (MAIN_FLOW, "Default UMA Carve Out Mode from EFI variable: 0x%x\n", ApcbUmaModeDefault);
              FrambufferHob->AtomFusionSystemInfoV6.sIntegratedSysInfo.UMACarveoutIndexDefault = (ApcbUmaModeDefault == 3) ? 2 : 1;
           }
        }
      }
    }
  }

  // Support IP Discovery and DMCUB
  DestinationAddress = (UINT64)(((FrambufferHob->UMAInfo.Base + FrambufferHob->UMAInfo.Size) - sizeof(ATOM_FUSION_SYSTEM_INFO_V6)) - MAX_IP_DISCOVERY_SIZE -MAX_DMCUB_SIZE);
  IDS_HDT_CONSOLE (MAIN_FLOW, "DXE FrambufferHob->UMAInfo.Base: %lx\n", FrambufferHob->UMAInfo.Base);
  IDS_HDT_CONSOLE (MAIN_FLOW, "DXE FrambufferHob->UMAInfo.Size: %lx\n", FrambufferHob->UMAInfo.Size);

  if (PcdGet32 (PcdAmdDisplayPhyTuningSettingTableHeader) != 0x00) {
    TableHeaderPtr = (ATOM_COMMON_TABLE_HEADER*)(UINTN) PcdGet32 (PcdAmdDisplayPhyTuningSettingTableHeader);
    DispPhyTuningDataPtr = (ATOM_N6_DISPLAY_PHY_TUNING_SET*)(UINTN) PcdGet32 (PcdAmdDisplayPhyTuningSettingTableContent);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Pcd of TableHeaderPtr: %lx\n", TableHeaderPtr);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Pcd of DispPhyTuningDataPtr: %lx\n", DispPhyTuningDataPtr);
    IDS_HDT_CONSOLE (MAIN_FLOW, "TableHeaderPtr->structuresize: %x\n", TableHeaderPtr->structuresize);
    IsN6TableReady = TRUE;
  }
  CopyMem ((VOID*)DestinationAddress,
    &(FrambufferHob->AtomFusionSystemInfoV6),
    sizeof(ATOM_FUSION_SYSTEM_INFO_V6));
  IDS_HDT_CONSOLE (MAIN_FLOW, "UMA Top - 64K - 256K - (Integrated_Table) Address: %lx\n", DestinationAddress);

  if (IsN6TableReady == TRUE) {
    DestinationAddress =  DestinationAddress - sizeof(ATOM_COMMON_TABLE_HEADER);
    CopyMem ((VOID*)DestinationAddress,
      (VOID *)TableHeaderPtr,
      sizeof(ATOM_COMMON_TABLE_HEADER));
    IDS_HDT_CONSOLE (MAIN_FLOW, "UMA Top - 64K - 256K - (Integrated_Table) - Header Address: %lx\n", DestinationAddress);

    DestinationAddress =  (DestinationAddress + sizeof(ATOM_COMMON_TABLE_HEADER)) - (TableHeaderPtr->structuresize);
    CopyMem ((VOID*)DestinationAddress,
      (VOID *)DispPhyTuningDataPtr,
      ((TableHeaderPtr->structuresize) - sizeof(ATOM_COMMON_TABLE_HEADER)));
    IDS_HDT_CONSOLE (MAIN_FLOW, "Support IP Discovery - Integrated Table - Header - Data DestinationAddress: %lx\n", DestinationAddress);
  }

//  IDS_HDT_CONSOLE (MAIN_FLOW, "Real Integrated Table: %lx\n", (UINT64)((FrambufferHob->UMAInfo.Base + FrambufferHob->UMAInfo.Size) - sizeof(ATOM_FUSION_SYSTEM_INFO_V6)));
  IDS_HDT_CONSOLE (MAIN_FLOW, "Final address: Support IP Discovery - DMCUB - Integrated Table - Whole D6 DestinationAddress: %lx\n", DestinationAddress);

  // Create event
  EfiCreateProtocolNotifyEvent (&gPspFlashAccSmmCommReadyProtocolGuid, TPL_NOTIFY, SyncDpPhyStaicTable, FrambufferHob, &Registration);

  IDS_HDT_CONSOLE (MAIN_FLOW, "AmdNbioGfxRMBDxeExit\n");
  AGESA_TESTPOINT (TpNbioGfxDxeExit, NULL);
  return EFI_SUCCESS;
}

