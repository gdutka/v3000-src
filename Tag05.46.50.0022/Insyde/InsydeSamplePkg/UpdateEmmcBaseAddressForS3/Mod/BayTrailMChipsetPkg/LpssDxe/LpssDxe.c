/** @file

;******************************************************************************
;* Copyright (c) 2015, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
/*++

Copyright (c) 2012 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  LpssDxe.c

Abstract:

  LPSS Platform Driver

 
--*/


#include <LpssDxe.h>
LPSS_DEVICE_INFO  mSccDeviceListA0Stepping[] = {
  {0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SCC_SDIO_0, PCI_FUNCTION_NUMBER_PCH_SCC_SDIO,\
  0, 0, 0, GLOBAL_NVS_OFFSET(eMMCAddr),  GLOBAL_NVS_OFFSET(eMMCLen), \
  1, 0, 0, GLOBAL_NVS_OFFSET(eMMC1Addr), GLOBAL_NVS_OFFSET(eMMC1Len), \
  R_PCH_SCC_EP_PCICFGCTR1, (B_PCH_SCC_EP_PCICFGCTR1_ACPI_INT_EN1 | B_PCH_SCC_EP_PCICFGCTR1_PCI_CFG_DIS1),\
  NULL, 0xFFFF, FALSE}, 

  {0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SCC_SDIO_2, PCI_FUNCTION_NUMBER_PCH_SCC_SDIO,\
  0, 0, 0, GLOBAL_NVS_OFFSET(SDCardAddr),  GLOBAL_NVS_OFFSET(SDCardLen), \
  1, 0, 0, GLOBAL_NVS_OFFSET(SDCard1Addr), GLOBAL_NVS_OFFSET(SDCard1Len), \
  R_PCH_SCC_EP_PCICFGCTR2, (B_PCH_SCC_EP_PCICFGCTR2_ACPI_INT_EN1 | B_PCH_SCC_EP_PCICFGCTR2_PCI_CFG_DIS1),
  NULL, 0xFFFF, FALSE}  
};

LPSS_DEVICE_INFO  mSccDeviceListB0Stepping[] = {
  {0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SCC_SDIO_3, PCI_FUNCTION_NUMBER_PCH_SCC_SDIO,\
  0, 0, 0, GLOBAL_NVS_OFFSET(eMMCAddr),  GLOBAL_NVS_OFFSET(eMMCLen), \
  1, 0, 0, GLOBAL_NVS_OFFSET(eMMC1Addr), GLOBAL_NVS_OFFSET(eMMC1Len), \
  R_PCH_SCC_EP_PCICFGCTR4, (B_PCH_SCC_EP_PCICFGCTR1_ACPI_INT_EN1 | B_PCH_SCC_EP_PCICFGCTR1_PCI_CFG_DIS1),\
  NULL, 0xFFFF, FALSE}, 

  {0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SCC_SDIO_2, PCI_FUNCTION_NUMBER_PCH_SCC_SDIO,\
  0, 0, 0, GLOBAL_NVS_OFFSET(SDCardAddr),  GLOBAL_NVS_OFFSET(SDCardLen), \
  1, 0, 0, GLOBAL_NVS_OFFSET(SDCard1Addr), GLOBAL_NVS_OFFSET(SDCard1Len), \
  R_PCH_SCC_EP_PCICFGCTR2, (B_PCH_SCC_EP_PCICFGCTR2_ACPI_INT_EN1 | B_PCH_SCC_EP_PCICFGCTR2_PCI_CFG_DIS1),
  NULL, 0xFFFF, FALSE}
};

//
// Default set to A0 device table
//
LPSS_DEVICE_INFO *mSccDeviceList = mSccDeviceListA0Stepping;

//
// A0 and B0 have the same table size
//
#define SCC_DEVICE_NUMBER  sizeof(mSccDeviceListA0Stepping)/sizeof(LPSS_DEVICE_INFO)

EFI_HANDLE         mImageHandle;

extern EFI_GUID gEfiEventExitBootServicesGuid;

extern PCH_STEPPING EFIAPI PchStepping (VOID);


VOID
UpdateSdhcBaseAddressTable (
  IN  UINTN                   Bus,
  IN  UINTN                   Dev,
  IN  UINTN                   Fun,
  IN  PHYSICAL_ADDRESS        Bar0,
  IN  PHYSICAL_ADDRESS        Bar1
  )
{
  PHYSICAL_ADDRESS            Bar;
  UINTN                       Index;
  UINTN                       SoltNum;
  UINT64                      *ControllerTable;
  UINTN                       SdhcList[4] = {
                                PCI_DEVICE_NUMBER_PCH_SCC_SDIO_0,
                                PCI_DEVICE_NUMBER_PCH_SCC_SDIO_1,
                                PCI_DEVICE_NUMBER_PCH_SCC_SDIO_2,
                                PCI_DEVICE_NUMBER_PCH_SCC_SDIO_3,
                              };
  
  Index = 0;
  while (SdhcList[Index++] != Dev) if (Index > 4) return;

  Index   = 0;
  SoltNum = 0;
  Bar     = Bar0;
  ControllerTable = (UINT64 *)PcdGetPtr (PcdH2OSdhcBaseAddressTable);
  while (ControllerTable[Index] != 0xFFFFFFFFFFFFFFFFULL) {
    if (ControllerTable[Index] == 0) {
      ControllerTable[Index] = (UINT64)(LShiftU64(Bar, 32) + LShiftU64(Bus, 24) + LShiftU64(Dev, 16) + LShiftU64(Fun, 8) + SoltNum);
      if ((Bar1 == 0) || (Bar1 == Bar)) break;
      Bar = Bar1;
      SoltNum++;
    }
	Index++;
  }
}

//
// Description:
//
// After PmAuthProtocol being installed, we can't save S3 required settings into SMRAM because of SmmLock.
// So we should save them in advance. 
// Also, we can ""NOT"" transfer all bootable media into ACPI mode before ExitBootService 
// because ACPI mode can't access PCI device, it will further make storage disappeared under setup menu. 
// 
EFI_STATUS
EFIAPI
SaveMsgBusForS3 (
  LPSS_DEVICE_INFO   *Device
  )
{
  UINT32             MCR, MCRX, MDR;

  //
  // Read the current MDR from message bus
  //
  Mmio32(EC_BASE, MC_MCRX) = (Device->AcpiModeRegOffset & MSGBUS_MASKHI);
  Mmio32(EC_BASE, MC_MCR) = (UINT32)((PCH_SCC_EP_PRIVATE_READ_OPCODE << 24) | (PCH_SCC_EP_PORT_ID << 16) | \
                            ((Device->AcpiModeRegOffset & MSGBUS_MASKLO) << 8) | MESSAGE_DWORD_EN);
  MDR = Mmio32(EC_BASE, MC_MDR);

  //
  // Process data for S3
  //
  MCRX = Device->AcpiModeRegOffset & MSGBUS_MASKHI;
  MCR = (UINT32)((PCH_SCC_EP_PRIVATE_WRITE_OPCODE << 24) | (PCH_SCC_EP_PORT_ID << 16) | \
        ((Device->AcpiModeRegOffset & MSGBUS_MASKLO) << 8) | MESSAGE_DWORD_EN);
  MDR = ((MDR & 0xFFFFFFFF) | Device->AcpiModeRegValue);

  //
  // In S3 execute, we should follow the MSG BUS access procedure to restore the Pre-saving data.
  // 
  // Indirect IO access: (According to BayTrail-M EDS chapter 3.6)
  //
  // 1. Write Index port into MSG BUS_MCRX first.
  // 2. Write content to data_register which is called MSG BUS_MDR.
  // 3. Send "message bus control" to complete the procedure.
  //
  S3BootScriptSaveMemWrite(
    EfiBootScriptWidthUint32, 
    (UINTN) (EC_BASE + MC_MCRX),
    1,
    (VOID *) &MCRX
    );
  
  S3BootScriptSaveMemWrite(
    EfiBootScriptWidthUint32, 
    EC_BASE + MC_MDR,
    1,
    (VOID *) &MDR
    );
  
  S3BootScriptSaveMemWrite(
    EfiBootScriptWidthUint32, 
    EC_BASE + MC_MCR,
    1,
    (VOID *) &MCR
    );

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
SaveGpioAdjustmentForS3 (
  LPSS_DEVICE_INFO   *Device
  )
{
  /*
     WARNING: this function takes effect on PmAuthProtocol installed
  
     Please add your requirement(s) at this function.
     Note that this function only saves content, NOT for adjust.
  */

  UINT8    Buffer8 = 0;
  //
  // Known device: SD card HOST
  //
  if (Device->DeviceNum == PCI_DEVICE_NUMBER_PCH_SCC_SDIO_2) {
    //
    // Bit 0~2 is the GPIO function indication, use F0 under ACPI mode for SD card
    // In PCI mode needn't to save this because PlatformEarlyInit will be dispatched during S3 resume
    //
    Buffer8 = 0x80;
    S3BootScriptSaveMemWrite(
      EfiBootScriptWidthUint8, 
      IO_BASE_ADDRESS + 0x03A0,
      1,
      (VOID *) &Buffer8
      );
  }
  
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ConfigMsgBusForBoot (
  LPSS_DEVICE_INFO   *Device
  )
{
   UINT32      Buffer32;
   DEBUG((DEBUG_ERROR, "Start to write configuration \n")); 
   DEBUG((DEBUG_ERROR, " Port id: 0x%x\n", PCH_SCC_EP_PORT_ID));
   DEBUG((DEBUG_ERROR, " reg offset: 0x%x\n", Device->AcpiModeRegOffset));
   DEBUG((DEBUG_ERROR, " Data write: 0x%x\n", Device->AcpiModeRegValue));

   SccMsgBus32AndThenOr(
     Device->AcpiModeRegOffset,
     Buffer32,
     0xFFFFFFFF,
     Device->AcpiModeRegValue
   );
   DEBUG((DEBUG_ERROR, "Start to read configuration back\n")); 
   SccMsgBusRead32(
     Device->AcpiModeRegOffset,
     Buffer32
   );

   DEBUG((DEBUG_ERROR, " Port id: 0x%x\n", PCH_SCC_EP_PORT_ID));
   DEBUG((DEBUG_ERROR, " reg offset: 0x%x\n", Device->AcpiModeRegOffset));
   DEBUG((DEBUG_ERROR, " Data read out: 0x%x\n", Buffer32));
   return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
ConfigGpioAdjustmentForBoot (
  LPSS_DEVICE_INFO   *Device
  )
{
  /*
     WARNING: this function takes effect on ExiteBootService
  
     Please add your requirement(s) at this function.
     Note that this function only sets GPIO, NOT for save S3 context.
  */

  //
  // Known device: SD card HOST
  //
  if (Device->DeviceNum == PCI_DEVICE_NUMBER_PCH_SCC_SDIO_2) {
    //
    // Switch GPIO to F0 for SD card 
    //
    MmioAnd32 (IO_BASE_ADDRESS + 0x03A0, 0xFFFFFFFE);
  }

  return EFI_SUCCESS;
}


VOID
SavePciConfigurationToStructure (
  LPSS_DEVICE_INFO        *DeviceInfo,
  EFI_PHYSICAL_ADDRESS    BaseAddress,
  UINT8                   BarIndex
  )
{
  UINT8                   NewCommand;
  UINT32                  NewBar;
  EFI_STATUS              Status;
  UINTN                   SccPciMmBase;
  UINT16                  Command;
  UINTN                   Seg, Bus, Dev, Func;

  Status = DeviceInfo->PciIo->GetLocation (
             DeviceInfo->PciIo,
             &Seg,
             &Bus,
             &Dev,
             &Func
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  SccPciMmBase = MmPciAddress (0, Bus, Dev, Func, 0);  
  Status = DeviceInfo->PciIo->Pci.Read (
             DeviceInfo->PciIo,
             EfiPciIoWidthUint16,
             PCI_COMMAND_OFFSET,
             1,
             &Command
             );
  //
  // Save the command data for filling MMIO space while system enters ExitBootService
  //
  DeviceInfo->PciCommand = Command;
  
                           
  NewCommand = 0;
  //Status = PciIo->Pci.Write(DeviceInfo->PciIo,
  //                          EfiPciIoWidthUint8,
  //                          PCI_COMMAND_OFFSET,
  //                          1,
  //                          &NewCommand
  //                          );
  
  S3BootScriptSaveMemWrite(
    EfiBootScriptWidthUint8,
    (UINTN) (SccPciMmBase + PCI_COMMAND_OFFSET),
    1,
    (VOID *) &NewCommand
    );
  
  NewBar = (UINT32)BaseAddress;
  //Status = PciIo->Pci.Write(DeviceInfo->PciIo,
  //                          EfiPciIoWidthUint32,
  //                          PCI_BASE_ADDRESSREG_OFFSET + BarIndex * sizeof(UINT32),
  //                          1,
  //                          &NewBar
  //                          );
  
  S3BootScriptSaveMemWrite(
    EfiBootScriptWidthUint32,
    (UINTN) (SccPciMmBase + PCI_BASE_ADDRESSREG_OFFSET + BarIndex * sizeof(UINT32)),
    1,
    (VOID *) &NewBar
    );
  

  //Status = PciIo->Pci.Write(DeviceInfo->PciIo,
  //                          EfiPciIoWidthUint16,
  //                          PCI_COMMAND_OFFSET,
  //                          1,
  //                          &Command
  //                          );
  
  S3BootScriptSaveMemWrite(
    EfiBootScriptWidthUint16,
    (UINTN) (SccPciMmBase + PCI_COMMAND_OFFSET),
    1,
    (VOID *) &Command
    );
}

  
VOID
UpdateLpssSccDeviceBar (
  LPSS_DEVICE_INFO        *DeviceInfo
                  )
{
  VOID         *Resources;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *ptr;
  EFI_STATUS                        Status;
  UINT8                             BarIndex;
  EFI_PHYSICAL_ADDRESS              BaseAddress;
  for (BarIndex = 0; BarIndex < PCI_MAX_BAR; BarIndex ++) {
    Status = DeviceInfo->PciIo->GetBarAttributes(//scan pci device's bar
                      DeviceInfo->PciIo,
                      BarIndex,
                      NULL,
                      &Resources
                      );
    if (EFI_ERROR(Status)) {
      continue;
    }
    ptr = Resources;
    if ((ptr->Desc != ACPI_ADDRESS_SPACE_DESCRIPTOR) || (ptr->ResType != ACPI_ADDRESS_SPACE_TYPE_MEM)) {
      gBS->FreePool(Resources);
      continue;
    }
    DEBUG((DEBUG_ERROR, "Devices  %x %x %x %x %x %lx %lx %lx %lx %lx\n", 
      ptr->Desc, ptr->Len, ptr->ResType, ptr->GenFlag, ptr->SpecificFlag,
      ptr->AddrSpaceGranularity, ptr->AddrRangeMin, ptr->AddrRangeMax,
      ptr->AddrTranslationOffset, ptr->AddrLen
      ));
    BaseAddress = 0xFFFFFFFF;
#if 1    
    Status = gDS->AllocateMemorySpace (
                    EfiGcdAllocateMaxAddressSearchBottomUp,
                    EfiGcdMemoryTypeMemoryMappedIo,
                    HighBitSet64(ptr->AddrSpaceGranularity),
                    ptr->AddrLen,
                    &BaseAddress,
                    mImageHandle,
                    NULL
                    );
//    ASSERT_EFI_ERROR (Status);
#else
    RegBase = MmPciAddress (
                  0,
                   DeviceInfo->BusNum,
                   DeviceInfo->DeviceNum,
                   DeviceInfo->FunctionNum,
                   0
                   );
    BaseAddress = (EFI_PHYSICAL_ADDRESS)(MmioRead32(RegBase + R_PCH_LPSS_SDIO_BAR + 0x04 * BarIndex) & B_PCH_LPSS_SDIO_BAR_BA);
#endif                   
    DEBUG((DEBUG_ERROR, "New Resource is %x %lx\n", BarIndex, BaseAddress));
    SavePciConfigurationToStructure (DeviceInfo, BaseAddress, BarIndex);
    if (BarIndex == DeviceInfo->ReportBarIndex) {
       DeviceInfo->ReportBar = BaseAddress;
       DeviceInfo->ReportBarLen = ptr->AddrLen;
    }
    if (BarIndex == DeviceInfo->ReportBarIndex1) {
       DeviceInfo->ReportBar1 = BaseAddress;
       DeviceInfo->ReportBarLen1 = ptr->AddrLen;
    }
    gBS->FreePool(Resources); 
  }
}

EFI_STATUS
UpdateLpssSccDeviceList()
{
  EFI_STATUS              Status;
  UINTN                   HandleCount;
  EFI_HANDLE              *Handles;
  EFI_PCI_IO_PROTOCOL     *PciIo;
  UINTN                   Index;
  UINTN                   Index2;
  UINTN                   Segment;
  UINTN                   BusNum;
  UINTN                   DeviceNum;
  UINTN                   FunctionNum;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  for(Index = 0; Index < HandleCount; Index ++) {
    
    Status = gBS->HandleProtocol (
                    Handles[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                    );
    if (EFI_ERROR(Status)) {
      continue;
    }
    Status = PciIo->GetLocation(
                      PciIo,
                      &Segment,
                      &BusNum,
                      &DeviceNum,
                      &FunctionNum
                      );

    for(Index2 = 0; Index2 < SCC_DEVICE_NUMBER; Index2 ++) {
      if (mSccDeviceList[Index2].Segment     == Segment &&
          mSccDeviceList[Index2].BusNum      == BusNum &&
          mSccDeviceList[Index2].DeviceNum   == DeviceNum &&
          mSccDeviceList[Index2].FunctionNum == FunctionNum) {
          DEBUG((EFI_D_ERROR, "Update Device %x %x %x\n", BusNum, DeviceNum, FunctionNum));
            mSccDeviceList[Index2].PciIo = PciIo;
            UpdateLpssSccDeviceBar(&mSccDeviceList[Index2]);
            UpdateSdhcBaseAddressTable (
              mSccDeviceList[Index2].BusNum, 
              mSccDeviceList[Index2].DeviceNum,
              mSccDeviceList[Index2].FunctionNum,
              mSccDeviceList[Index2].ReportBar,
              mSccDeviceList[Index2].ReportBar1
              );
      }
    }
    
  }
  gBS->FreePool(Handles);
  return EFI_SUCCESS;
}

EFI_STATUS 
GetPCIAddrRange(
       EFI_PHYSICAL_ADDRESS *RangeMin, 
       EFI_PHYSICAL_ADDRESS *RangeMax
       )
{
  EFI_STATUS                    Status;

  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *PciResource;
  EFI_HANDLE                                       RootBridgeHandle;
  UINT64                                           Attributes;
  VOID                                            *Configuration;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR               *ptr;
  BOOLEAN                                         Found;
  BOOLEAN                                         NextBridge;
  *RangeMin = (EFI_PHYSICAL_ADDRESS)(-1);
  *RangeMax = 0;
  Status = gBS->LocateProtocol (
                  &gEfiPciHostBridgeResourceAllocationProtocolGuid,
                  NULL,
                  (VOID **)&PciResource
                  );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Can't locate PciResource Protocol\n"));
    return EFI_NOT_FOUND;

  }
  RootBridgeHandle = NULL;
  Found = FALSE;
  NextBridge = TRUE;
  while(NextBridge) {
    Status = PciResource->GetNextRootBridge(
                            PciResource,
                            &RootBridgeHandle
                            );
    if (EFI_ERROR(Status)) {
      NextBridge = FALSE;
	  continue;
    }
    Status = PciResource->GetAllocAttributes (
                          PciResource,
                          RootBridgeHandle,
                          &Attributes
                          );

    DEBUG((DEBUG_ERROR, "RootBridgeHandle %x %lx\n", RootBridgeHandle, Attributes));
    Status = PciResource->GetProposedResources (
                          PciResource,
                          RootBridgeHandle,
                          &Configuration
                          );
    if (EFI_ERROR(Status)) {
      continue;
    }
    ptr = Configuration;
    while(ptr->Desc == ACPI_ADDRESS_SPACE_DESCRIPTOR) {
      DEBUG((DEBUG_ERROR, "  %x %x %x %x %x %lx %lx %lx %lx %lx\n", 
        ptr->Desc, ptr->Len, ptr->ResType, ptr->GenFlag, ptr->SpecificFlag,
        ptr->AddrSpaceGranularity, ptr->AddrRangeMin, ptr->AddrRangeMax,
        ptr->AddrTranslationOffset, ptr->AddrLen
        ));
      if (ptr->ResType == ACPI_ADDRESS_SPACE_TYPE_MEM) {
        if (*RangeMin > ptr->AddrRangeMin)  *RangeMin = ptr->AddrRangeMin;
        if (*RangeMax < (ptr->AddrRangeMin + ptr->AddrLen))  *RangeMax = ptr->AddrRangeMin + ptr->AddrLen;
        Found = TRUE;
      }
      ptr ++;
    }
    gBS->FreePool(Configuration);
  }
  if (!Found) {
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}


VOID
TurnOnAcpiEnableDevice(
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  UINTN                    SccDevice;
  UINT8                    CommandZero = 0;
  EFI_STATUS               Status;
  UINT8                    BarIndex;
  UINT32                   Bar;
  LPSS_DEVICE_INFO         *Device;
  BOOLEAN                  UefiBoot;

  Device = (LPSS_DEVICE_INFO *) Context;

  for (SccDevice = 0; SccDevice < SCC_DEVICE_NUMBER; SccDevice++) {
    if (Device[SccDevice].IsAcpiEnable) {
      //
      // Trun off the PCI device
      //
      Status = Device[SccDevice].PciIo->Pci.Write (
                 Device[SccDevice].PciIo,
                 EfiPciIoWidthUint8,
                 PCI_COMMAND_OFFSET,
                 1,
                 &CommandZero
                 );
      //
      // Select the resource
      //
      BarIndex = (Device[SccDevice].ReportBar != 0) ? 0 : 1;
      Bar      = (Device[SccDevice].ReportBar != 0) ? (UINT32) Device[SccDevice].ReportBar : \
                                                      (UINT32) Device[SccDevice].ReportBar1;
      //
      // Re-assign resource
      //
      Status = Device[SccDevice].PciIo->Pci.Write (
                 Device[SccDevice].PciIo,
                 EfiPciIoWidthUint32,
                 PCI_BASE_ADDRESSREG_OFFSET + BarIndex * sizeof(UINT32),
                 1,
                 &Bar
                 );
      //
      // Turn on the device
      //
      Status = Device[SccDevice].PciIo->Pci.Write (
                 Device[SccDevice].PciIo,
                 EfiPciIoWidthUint16,
                 PCI_COMMAND_OFFSET,
                 1,
                 &(Device[SccDevice].PciCommand)
                 );

      //
      // Configure Message Bus for enable ACPI mode
      //
      ConfigMsgBusForBoot (&Device[SccDevice]);

      //
      // Set GPIO for enable ACPI mode (Optional)
      //
      ConfigGpioAdjustmentForBoot (&Device[SccDevice]);
    }
  }

  UefiBoot = TRUE;
  gRT->SetVariable (
          L"UefiBoot",
          &gSystemConfigurationGuid,
          EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
          sizeof (BOOLEAN),
          &UefiBoot
          );

}

VOID
UpdateLpssSccDeviceInfo(
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsArea, 
  UINT32  						PciMode
                     )
{
  UINTN         Index;

  for(Index = 0; Index < SCC_DEVICE_NUMBER; Index ++) {
    if (mSccDeviceList[Index].ReportBar != 0) {
      *(UINT32*)((CHAR8*)GlobalNvsArea->Area + mSccDeviceList[Index].AddrOffset) = (UINT32)mSccDeviceList[Index].ReportBar;
      *(UINT32*)((CHAR8*)GlobalNvsArea->Area + mSccDeviceList[Index].LenOffset) =  (UINT32)mSccDeviceList[Index].ReportBarLen;
      *(UINT32*)((CHAR8*)GlobalNvsArea->Area + mSccDeviceList[Index].AddrOffset1) = (UINT32)mSccDeviceList[Index].ReportBar1;
      *(UINT32*)((CHAR8*)GlobalNvsArea->Area + mSccDeviceList[Index].LenOffset1) =  (UINT32)mSccDeviceList[Index].ReportBarLen1;
      if (PciMode == 1) {
        DEBUG((DEBUG_ERROR, " Bypass ACPI mode, work in PCI mode\n"));
      } else {
        DEBUG((DEBUG_ERROR, " SccAcpiModeEnable started, work in ACPI mode\n"));
        mSccDeviceList[Index].IsAcpiEnable = TRUE;
      }
    }
  }
}


VOID
EFIAPI
OnReadyToBoot4LPSS(
IN EFI_EVENT  Event,
IN VOID       *Context
  )
{
  EFI_PHYSICAL_ADDRESS RangeMin; 
  EFI_PHYSICAL_ADDRESS RangeMax;
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsArea;
  EFI_STATUS           Status;
  EFI_EVENT            EventForAcpi;
  

  static UINT8 FirstTime=1;
  if(1==FirstTime)
      FirstTime=0;
  else
      return;

  Status = GetPCIAddrRange(&RangeMin, &RangeMax);
  if (EFI_ERROR(Status)) {
    return;
  }
  Status = gBS->LocateProtocol (
                  &gEfiGlobalNvsAreaProtocolGuid,
                  NULL,
                  (VOID **)&GlobalNvsArea
                  );
  if (EFI_ERROR(Status)) {
    return;
  }
  GlobalNvsArea->Area->PCIBottomAddress = (UINT32)RangeMin;
  GlobalNvsArea->Area->PCITopAddress = (UINT32)RangeMax - 1;

  UpdateLpssSccDeviceList();
  // EFI boot stays in ACPI mode
  UpdateLpssSccDeviceInfo(GlobalNvsArea, 0);
  //SignalAllDriversLpssDone();  

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  TurnOnAcpiEnableDevice,
                  (VOID *) mSccDeviceList,
                  &gEfiEventExitBootServicesGuid,
                  &EventForAcpi
                  );
  
  return;
}

VOID
EFIAPI
OnReadyToBoot4LPSSLegacy(
IN EFI_EVENT  Event,
IN VOID       *Context
  )
{
  EFI_PHYSICAL_ADDRESS RangeMin; 
  EFI_PHYSICAL_ADDRESS RangeMax;
  EFI_GLOBAL_NVS_AREA_PROTOCOL  *GlobalNvsArea;
  EFI_STATUS           Status;

  static UINT8 FirstTime=1;
  if(1==FirstTime)
    FirstTime=0;
  else
    return;

  
  Status = GetPCIAddrRange(&RangeMin, &RangeMax);
  if (EFI_ERROR(Status)) {
    return;
  }
  Status = gBS->LocateProtocol (
                  &gEfiGlobalNvsAreaProtocolGuid,
                  NULL,
                  (VOID **)&GlobalNvsArea
                  );
  if (EFI_ERROR(Status)) {
    return;
  }
  GlobalNvsArea->Area->PCIBottomAddress = (UINT32)RangeMin;
  GlobalNvsArea->Area->PCITopAddress = (UINT32)RangeMax - 1;

  UpdateLpssSccDeviceList();
  // Legacy boot stays in PCI mode
  UpdateLpssSccDeviceInfo(GlobalNvsArea, 1);
  //SignalAllDriversLpssDone();

  //
  // PCI Mode
  //
  GlobalNvsArea->Area->LpssSccMode = 1;  
  
  return;
}

UINT8
EmmcBootMode (
  IN CHIPSET_CONFIGURATION      SystemConfiguration
  )
{
  if (SystemConfiguration.eMMCBootMode == 1) {
    if (PchStepping() == PchA0 || PchStepping() == PchA1) {
      //
      // 4.41
      //
      return 2;
    } else {
      //
      // 4.5
      //
      return 3;
    }
  } else if (SystemConfiguration.eMMCBootMode == 2) {
    return 2;
  } else if (SystemConfiguration.eMMCBootMode == 3) {
    return 3;
  } else {
    return 0;
  }
}
EFI_STATUS
EFIAPI
LpssDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
/*++

Routine Description:

  Entry point for Acpi platform driver.

Arguments:

  ImageHandle  -  A handle for the image that is initializing this driver.
  SystemTable  -  A pointer to the EFI system table.

Returns:

  EFI_SUCCESS           -  Driver initialized successfully.
  EFI_LOAD_ERROR        -  Failed to Initialize or has been loaded.
  EFI_OUT_OF_RESOURCES  -  Could not allocate needed resources.

--*/
{
  EFI_STATUS                    Status = 0;
  EFI_STATUS                    GetPolicyStatus = 0;
  EFI_EVENT                     Event;
  DXE_PCH_PLATFORM_POLICY_PROTOCOL *DxePlatformPchPolicy;
  UINTN                            VarSize;
  CHIPSET_CONFIGURATION            SystemConfiguration;
  VOID                             *FullSystemConfig;
  UINT8                            MmcBootMode = 0;
  VOID                             *RegistrationExitPmAuth;
  BOOLEAN                       UefiBoot;

  mImageHandle = ImageHandle;
  mSccDeviceList = mSccDeviceListA0Stepping;
  
  VarSize = PcdGet32 (PcdSetupConfigSize);
  FullSystemConfig = (CHIPSET_CONFIGURATION *)AllocatePool (VarSize);
  ASSERT (FullSystemConfig != NULL);
  
  Status = gRT->GetVariable(
                  L"Setup",
                  &gSystemConfigurationGuid,
                  NULL,
                  &VarSize,
                  FullSystemConfig
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Variable service not found\n"));
    ZeroMem (&SystemConfiguration, sizeof(CHIPSET_CONFIGURATION));
  } else {
    ASSERT (sizeof(CHIPSET_CONFIGURATION) <= VarSize);
    CopyMem (&SystemConfiguration, FullSystemConfig, sizeof(CHIPSET_CONFIGURATION));  
  }
  FreePool (FullSystemConfig);
  

  GetPolicyStatus  = gBS->LocateProtocol (&gDxePchPlatformPolicyProtocolGuid, NULL, (VOID **)&DxePlatformPchPolicy); 
  if (EFI_ERROR (GetPolicyStatus)) {
    //
    // Get platform policy fail, we have another chance to get eMMC settings depending on Variable
    //
    if (!EFI_ERROR (Status)) {
      MmcBootMode = EmmcBootMode (SystemConfiguration);
      if (MmcBootMode == 2) {
        DEBUG ((EFI_D_ERROR, "A0/A1 SCC eMMC 4.41 table\n"));
        mSccDeviceList = mSccDeviceListA0Stepping;
      } else if (MmcBootMode == 3) {
        DEBUG ((EFI_D_ERROR, "B0 SCC eMMC 4.5 table\n"));
        mSccDeviceList = mSccDeviceListB0Stepping;
      } else {
        DEBUG ((EFI_D_ERROR, "eMMC device is not enabled!!!\n"));
      }
    }
  } else {
    if (DxePlatformPchPolicy->SccConfig->eMMCEnabled) {
      DEBUG ((EFI_D_ERROR, "A0/A1 SCC eMMC 4.41 table\n"));
      mSccDeviceList = mSccDeviceListA0Stepping;
    } else if (DxePlatformPchPolicy->SccConfig->eMMC45Enabled) {
      DEBUG ((EFI_D_ERROR, "B0 SCC eMMC 4.5 table\n"));
      mSccDeviceList = mSccDeviceListB0Stepping;
    } else {
      DEBUG ((EFI_D_ERROR, "eMMC device is not enabled!!!\n"));
    }
  }

  //
  // This driver only transfers eMMC 4.41/4.5 into ACPI mode, other SCC devices such as SDIO-SIFI and
  // SD card will be ignored, so we needn't to judge whether WIFI or SD card is enabled or not.
  //
  gRT->SetVariable (
          L"UefiBoot",
          &gSystemConfigurationGuid,
          EFI_VARIABLE_NON_VOLATILE |EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
          0,
          &UefiBoot
          );

       //
       // According to platform policy
       //
  if ((GetPolicyStatus == EFI_SUCCESS &&
      (DxePlatformPchPolicy->LpssConfig->LpssPciModeEnabled == 0) &&
      ((DxePlatformPchPolicy->SccConfig->eMMCEnabled != 0) || 
       (DxePlatformPchPolicy->SccConfig->eMMC45Enabled != 0) ||
       (DxePlatformPchPolicy->SccConfig->SdcardEnabled != 0))) || 
       //
       // Get policy fail, we adopt system configuration
       //
       (GetPolicyStatus != EFI_SUCCESS && 
        SystemConfiguration.LpssPciModeEnabled == 0 &&
        (MmcBootMode == 2 || MmcBootMode == 3 || SystemConfiguration.LpssSdcardEnabled))) {
  // S4682406 - Also register OnReadyToBoot4LPSS callback for legacy boot 
    Status = EfiCreateEventLegacyBootEx (
               TPL_CALLBACK,
              OnReadyToBoot4LPSSLegacy,
               NULL,
              &Event
               );
    ASSERT_EFI_ERROR (Status);

    Status = gBS->CreateEvent (
                     EVT_NOTIFY_SIGNAL,
                     TPL_CALLBACK,
                     OnReadyToBoot4LPSS,
                     NULL,
                     &Event
                     );
   
    Status = gBS->RegisterProtocolNotify (
                    &gExitPmAuthProtocolGuid,
                    Event,
                    &RegistrationExitPmAuth
                    );
    
    //Status = gBS->CreateEventEx (
    //                EVT_NOTIFY_SIGNAL,
    //                TPL_NOTIFY,
    //                OnReadyToBoot4LPSS,
    //                NULL,
    //                &gEfiEventExitBootServicesGuid,
    //                &Event
    //                );
  }

  return EFI_SUCCESS;
}
