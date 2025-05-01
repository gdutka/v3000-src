/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <PiPei.h>
#include <Ppi/MemoryDiscovered.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdCapsuleLib.h>
#include <Library/FchBaseLib.h>
#include <Library/FabricResourceManagerLib.h>
#include <Library/PciLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/PeiServicesTablePointerLib.h>

#include <AmdUsb4.h>
#include <AmdUsb4/RingDescriptor.h>
#include <Library/AmdUsb4Lib.h>

#define FILECODE USB4_AMDUSB4PEI_AMDUSB4PEI_FILECODE

#ifndef EXTERN
#define EXTERN extern
#endif

EXTERN UINT32  Usb4HostNumber;
EXTERN AMD_USB4_HOST_PCI_ADDRESS  Usb4HostList[];

EXTERN
EFI_STATUS
AmdUsb4AllocateRingMemory (
  IN     AMD_USB4_CM             *pCM,
  IN     UINT32                  RingNumber
  );

EXTERN VOID
AmdUsb4DeAllocateRingMemory (
  IN     AMD_USB4_CM             *pCM,
  IN     UINT32                  RingNumber
  );

EXTERN
EFI_STATUS
AmdUsb4HostConnectionInfo (
  IN     AMD_USB4_CM         *pCM,
  IN     UINT32              Usb4HostIndex
  );

EXTERN
VOID
AmdDisableUsb4Ring (
  IN  AMD_USB4_CM     *pCM,
  IN  UINT32          RingNumber
  );

EXTERN VOID
AmdUsb4UpperBridgeMMIOSaveRestore (
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  *pAmdUsb4HostPciAddress,
  IN     UINT32                     HostAmount,
  IN     BOOLEAN                    Action
  );

EFI_STATUS
AmdUsb4AllocateBuffer (
  IN      UINTN                 Size,
      OUT EFI_PHYSICAL_ADDRESS  *Buffer
  )
{
  EFI_STATUS          Status;
  EFI_PEI_SERVICES    **PeiServices;

  Status      = EFI_SUCCESS;
  PeiServices = (EFI_PEI_SERVICES**)GetPeiServicesTablePointer ();

  IDS_HDT_CONSOLE_USB4_ASSERT (NULL!=PeiServices);

  Status = (*PeiServices)->AllocatePages (
                             PeiServices,
                             EfiBootServicesData,
                             EFI_SIZE_TO_PAGES (Size),
                             Buffer
                             );
  return Status;
}


VOID
AmdUsb4FreeBuffer (
  IN      EFI_PHYSICAL_ADDRESS       Buffer,
  IN      UINTN                      Size
  )
{
  EFI_STATUS          Status;
  EFI_PEI_SERVICES    **PeiServices;

  Status      = EFI_SUCCESS;
  PeiServices = (EFI_PEI_SERVICES**)GetPeiServicesTablePointer ();

  IDS_HDT_CONSOLE_USB4_ASSERT (NULL!=PeiServices);

  (*PeiServices)->FreePages (
                    PeiServices,
                    Buffer,
                    EFI_SIZE_TO_PAGES (Size)
                    );
}


/*++

Routine Description:

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
EFIAPI
AmdUsb4PreOsCmPei (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  EFI_STATUS                Status;
  UINT32                    HostIndex;
  AMD_USB4_CM               *pCM;
  AMD_USB4_CM_ROUTER        *pRouter;
  AMD_USB4_HOST_DATA        *pAmdUsb4HostData;
  AMD_USB4_HOST_PCI_ADDRESS *pAmdUsb4HostPciAddress;

  AGESA_TESTPOINT (TpUsb4PeiMemoryDicoverPpiCallbackEntry, NULL);
  Status                  = EFI_SUCCESS;
  HostIndex               = 0;
  pCM                     = NULL;
  pRouter                 = NULL;
  pAmdUsb4HostData        = NULL;
  pAmdUsb4HostPciAddress  = NULL;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Enter.\n", __FUNCTION__);

#if AMD_USB4_DEBUG_PRE_OS_CM_DISABLE
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Skip USB4 Pre-OS CM.\n");
  return (EFI_SUCCESS);
#endif

  //
  // Allocate memory for the private data
  //
  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (AMD_USB4_HOST_DATA),
                             &pAmdUsb4HostData
                             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Cannot allocate memory for USB4 Host Data\n");
    return Status;
  } else {
    ZeroMem (pAmdUsb4HostData, sizeof (AMD_USB4_HOST_DATA));
    pAmdUsb4HostData->AmdUsb4Version = AMD_USB4_CM_VERSION;
  }

  //
  // Get USB4 Host List
  //
  Status = AmdUsb4GetUsb4Hosts (
             pAmdUsb4HostData,
             &Usb4HostNumber,
             Usb4HostList
             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Cannot get USB4 Host!\n");
    return Status;
  }

  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (AMD_USB4_HOST_PCI_ADDRESS) * Usb4HostNumber,
                             &pAmdUsb4HostPciAddress
                             );
  IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));
  CopyMem (pAmdUsb4HostPciAddress, Usb4HostList, sizeof (AMD_USB4_HOST_PCI_ADDRESS) * Usb4HostNumber);

  AmdUsb4UpperBridgeMMIOSaveRestore (
    pAmdUsb4HostPciAddress,
    Usb4HostNumber,
    TRUE
    );

  //
  // Enable USB4 Host MMIO
  //
  Status = AmdUsb4InitHostMmio (
             pAmdUsb4HostData,
             &Usb4HostNumber,
             Usb4HostList
             );
  if (Status != EFI_SUCCESS) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Cannot Enable USB4 Host MMIO!\n");
    return Status;
  }

#if AMD_USB4_DEBUG_BP_BEFORE_CM
  IDS_HDT_CONSOLE_USB4_DEBUG ("[AmdUsb4DxeEntry] Check CMOS[0x88] Value.\n");
  IoWrite8 (0x72, 0x88);
  if ( IoRead8 (0x73) == 0x5A ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "[AmdUsb4DxeEntry] CMOS[0x88] Value is value. go into infinite loop until the it changes to other value\n"
      );
    while (1) {
      if ( IoRead8 (0x73) == 0x5A ) {
        gBS->Stall (500000);  // Stall 500ms
      } else {
        break;
      }
    }
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG ("[AmdUsb4DxeEntry] CMOS[0x88] Value is not 0x5A. Keep going.\n");
  }
#endif

  //
  // Loop for all USB4 Host
  //
  for (HostIndex = 0; HostIndex < Usb4HostNumber; HostIndex++) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Start Host Index: %d\n", HostIndex);
    //
    // Init CM data structure for Host
    //
    Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               sizeof (AMD_USB4_CM),
                               &(pAmdUsb4HostData->pCM[HostIndex])
                               );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Cannot allocate memory for USB4 CM!\n");
      return EFI_OUT_OF_RESOURCES;
    }

    ZeroMem (pAmdUsb4HostData->pCM[HostIndex], sizeof (AMD_USB4_CM));
    pCM = pAmdUsb4HostData->pCM[HostIndex];
    pCM->HIMmioBase = pAmdUsb4HostData->Usb4HostMmioBase[HostIndex];
    IDS_HDT_CONSOLE_USB4_DEBUG ("pCM address=0x%x, pCM->HIMmioBase=0x%lx\n", pCM, pCM->HIMmioBase);
    CopyMem (&(pCM->HostRouterPciAddress), &(Usb4HostList[HostIndex]), sizeof (AMD_USB4_HOST_PCI_ADDRESS));

    //
    //   Allocate Memory for Ring0 Interface
    //
    Status = AmdUsb4AllocateRingMemory (
               pCM,
               0
               );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Cannot allocate memory for Ring0!\n");
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Allocated 64 double words buffer for Tx and Rx buffer
    //
    Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               sizeof (UINT32) * 64,
                               &(pCM->PtrControlPacketTxBuffer)
                               );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Cannot allocate memory for PtrControlPacketTxBuffer!\n");
      return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem (pCM->PtrControlPacketTxBuffer, sizeof (UINT32) * 64);

    Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               sizeof (UINT32) * 64,
                               &(pCM->PtrControlPacketRxBuffer)
                               );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Cannot allocate memory for PtrControlPacketRxBuffer!\n");
      return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem (pCM->PtrControlPacketRxBuffer, sizeof (UINT32) * 64);

    Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               sizeof (UINT32) * 64,
                               &(pCM->PtrControlPacketClearBuffer)
                               );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Cannot allocate memory for PtrControlPacketClearBuffer!\n");
      return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem (pCM->PtrControlPacketClearBuffer, sizeof (UINT32) * 64);

    //
    // Allocate Memory for DROM buffer
    //
    Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               sizeof (UINT8) * MAX_CM_DROM_BUFFER_SIZE,
                               &(pCM->PtrDROMBuffer)
                               );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Cannot allocate memory for PtrDROMBuffer!\n");
      return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem (pCM->PtrDROMBuffer, sizeof (UINT8) * MAX_CM_DROM_BUFFER_SIZE);

    //
    // Allocate memory for CM data pCMData
    //
    Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               sizeof (AMD_USB4_CM_CELL) * MAX_CM_DATA_ARRAY_AMOUNT,
                               &(pCM->PtrCmData)
                               );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Cannot allocate memory for CM Data!\n");
      return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem (pCM->PtrCmData, sizeof (AMD_USB4_CM_CELL) * MAX_CM_DATA_ARRAY_AMOUNT);
    pCM->CmDataIndex = 0;

    //
    // Init H/I for Ring0
    //
    Status = AmdUsb4HostInterfaceRing0Init (
               pCM,
               TX_RING0_SIZE,
               RX_RING0_SIZE,
               MAX_CONTROL_PACKET_SIZE,
               MAX_CONTROL_PACKET_SIZE
               );

    //
    // Update USB3/PCIe/DP info that connect to USB4 Host Router
    //
    Status = AmdUsb4HostConnectionInfo (pCM, HostIndex);

    //
    // Initialize DisplayPort Plugged Adapter list
    //
    InitializeListHead (&pCM->DpInPluggedList);
    InitializeListHead (&pCM->DpOutPluggedList);

    //
    // Set TMU mode
    //
    pCM->CmTmuMode = TMU_MODE_HIFI_BI; // set to HIFI_BI for Pre-OS CM

    //
    // Initialize RS-FEC Gen3 and Gen2
    //
    pCM->RequestRsFecGen3 = ((PcdGet8 (PcdUsb4Gen3RsFec) >> HostIndex) & ((UINT8) BIT0)) ? 1 : 0;
    pCM->RequestRsFecGen2 = ((PcdGet8 (PcdUsb4Gen2RsFec) >> HostIndex) & ((UINT8) BIT0)) ? 1 : 0;
    pCM->LinkSpeed  = ((PcdGet8 (PcdUsb4Gen3Support) >> HostIndex) & ((UINT8) BIT0)) ? 1 : 0;

    pCM->HPDWaitTimeout.OldTimerValue = 0;
    pCM->HPDWaitTimeout.EndValue      = 0;

    //
    // Init Host Router
    //
    //    Enumerate Host Router
    //
    pRouter = (AMD_USB4_CM_ROUTER*)&(pCM->PtrCmData[pCM->CmDataIndex]);
    pCM->CmDataIndex++;

    AGESA_TESTPOINT (TP_HOST_ROUTER_INIT, NULL);

    Status = HostRouterInitialization (pCM, pRouter);
    if ( Status != EFI_SUCCESS ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Host Router Init fail!\n");
      return EFI_DEVICE_ERROR;
    }

    AGESA_TESTPOINT (TP_DEVICE_ROUTER_INIT, NULL);

    //
    // Detect and Init Device Router
    //
    Status = DeviceRouterInitialization (pCM, pRouter);
    if ( Status == EFI_DEVICE_ERROR ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Device Router Init fail!\n");
      return EFI_DEVICE_ERROR;
    }

    AGESA_TESTPOINT (TP_WAIT_DPOUT, NULL);

    //
    // Check Timer requirement for DP OUT
    //
    do {
      AmdUsb4ClearReceiveRing (pCM);
    }  while ( ( HasItTimedOut (&(pCM->HPDWaitTimeout)) != EFI_TIMEOUT ) );

    AGESA_TESTPOINT (TP_WAIT_DPOUT | 1, NULL);
    PreEnableDpTunneling (pCM);

    //
    // Setup Path
    //
    AGESA_TESTPOINT (TP_DP_TUNNELING, NULL);
    EnableDpTunneling (pCM);
    AGESA_TESTPOINT (TP_USB_TUNNELING, NULL);
    EnableUsb3Tunneling (pCM);
    AGESA_TESTPOINT (TP_PCI_TUNNELING, NULL);
    EnablePcieTunneling (pCM);

    //
    // Set Timer event
    //

    //
    // Check if there is Child device router connected.
    // Set topology id valid to 0 if no device router.
    //
    AnyDeviceRouterConnected (pCM, pRouter);

    //
    // Disable USB4 Ring
    //
    AmdDisableUsb4Ring (pCM, 0);
    // Free Ring and Ring buffer
    AmdUsb4DeAllocateRingMemory (pCM, 0);
  }

  AmdUsb4UpperBridgeMMIOSaveRestore (
    pAmdUsb4HostPciAddress,
    Usb4HostNumber,
    FALSE
    );

  IDS_HDT_CONSOLE_USB4_INFO ("%a Exit.\n", __FUNCTION__);
  AGESA_TESTPOINT (TpUsb4PeiMemoryDicoverPpiCallbackExit, NULL);

  return Status;
}


EFI_PEI_NOTIFY_DESCRIPTOR   mUsb4MemoryDicoverPpiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  AmdUsb4PreOsCmPei
};


/*++

Routine Description:

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
AmdUsb4PeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS            Status;

  AGESA_TESTPOINT (TpUsb4PeiEntry, NULL);
  Status    = EFI_SUCCESS;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Enter\n", __FUNCTION__);

  if ( PcdGetBool (PcdAmdUsb4CmEnable) == 0 ) {
    IDS_HDT_CONSOLE_USB4_INFO (" Usb4 Pre-OS CM do not support\n");
    return (EFI_SUCCESS);
  }

  if ( AMD_USB4_PRE_OS_CM_PEI_SUPPORT == 0 ) {
    IDS_HDT_CONSOLE_USB4_INFO (" Usb4 Pre-OS CM PEI do not support\n");
    return (EFI_SUCCESS);
  }

  //
  // Check if Boot mode is boot then hook for MemoryDiscovery protocol
  //
  if ((FchReadSleepType () != 3) && (AmdCapsuleGetStatus () == FALSE)) {
    Status = (**PeiServices).NotifyPpi (PeiServices, &mUsb4MemoryDicoverPpiCallback);
  }

  IDS_HDT_CONSOLE_USB4_INFO ("%a Exit\n", __FUNCTION__);
  AGESA_TESTPOINT (TpUsb4PeiExit, NULL);

  return Status;
}
