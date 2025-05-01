/** @file
  HddSpindown driver.

//;******************************************************************************
//;* Copyright (c) 2014 - 2015, Insyde Software Corporation. All Rights Reserved.
//;*
//;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
//;* transmit, broadcast, present, recite, release, license or otherwise exploit
//;* any part of this publication in any form, by any means, without the prior
//;* written permission of Insyde Software Corporation.
//;*
//;******************************************************************************
*/

#include "HddSpindown.h"

EFI_HANDLE                              mImageHandle = NULL;

/**
  Resolve controller mode.

  @param[in,out] PrivateData    Pointer to HDD SPINDOWN private data.

  @retval EFI_SUCCESS           Controller mode is set successfully.
  @retval EFI_NOT_FOUND         Controller mode can't be set.

**/
EFI_STATUS
EFIAPI
HddSpindownSetControllermode (
  IN OUT HDD_SPINDOWN_PRIVATE_DATA  *PrivateData
  )
{
  EFI_STATUS                     Status;
  UINT8                          HandleIndex;
  UINT8                          HandleCount;
  BOOLEAN                        DevicePathMatch;
  EFI_HANDLE                     *HandleBuffer;
  EFI_DISK_INFO_PROTOCOL         *DiskInfo;
  EFI_DEVICE_PATH_PROTOCOL       *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL       *DevicePathNode;

  HandleCount = 0;
  DevicePathMatch = FALSE;
  //
  // Collect all disk device information
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiDiskInfoProtocolGuid,
                  NULL,
                  ((UINTN*) &HandleCount),
                  &HandleBuffer
                  );
  if (Status != EFI_SUCCESS) {
    return EFI_NOT_FOUND;
  }
  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gEfiDiskInfoProtocolGuid,
                    ((VOID**) &DiskInfo)
                    );
    if (Status != EFI_SUCCESS) {
      continue;
    }
    if (!((CompareGuid (&DiskInfo->Interface, &gEfiDiskInfoIdeInterfaceGuid)) ||
          (CompareGuid (&DiskInfo->Interface, &gEfiDiskInfoAhciInterfaceGuid)))) {
      continue;
    }
    Status = gBS->HandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gEfiDevicePathProtocolGuid,
                    ((VOID**) &DevicePath)
                    );
    if (Status != EFI_SUCCESS) {
      continue;
    }
    DevicePathNode = DevicePath;
    while (!IsDevicePathEnd (DevicePathNode)) {
      if ((DevicePathType (DevicePathNode) == MESSAGING_DEVICE_PATH) &&
          (DevicePathSubType (DevicePathNode) == MSG_ATAPI_DP)) {
        //
        // IDE mode
        //
        PrivateData->ControllerMode = IdeType;
        DevicePathMatch = TRUE;
        break;
      }
      if ((DevicePathType (DevicePathNode) == MESSAGING_DEVICE_PATH) &&
          (DevicePathSubType (DevicePathNode) == MSG_SATA_DP)) {
        //
        // AHCI mode
        //
        PrivateData->ControllerMode = AhciType;
        DevicePathMatch = TRUE;
        break;
      }
      DevicePathNode = NextDevicePathNode (DevicePathNode);
    }
  }
  gBS->FreePool (HandleBuffer);
  if (!DevicePathMatch) {
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}

/**
 Program AHCI controller

 @retval EFI_UNSUPPORTED                AHCI is not found
 @retval EFI_SUCCESS                    AHCI controller is succesfully enabled

**/
EFI_STATUS
EFIAPI
HddSpindownProgramAhciController (
  )
{
  EFI_STATUS                    Status;
  UINT32                        AhciBar;
  UINT32                        Register;
  UINTN                         RegIndex;
  UINT8                         ClassCode;
  UINT8                         SubClassCode;
  EFI_PHYSICAL_ADDRESS          MemBaseAddress;

  //
  // Allocate the AHCI BAR
  //
  MemBaseAddress = 0x0ffffffff;

  Status = gDS->AllocateMemorySpace (
                  EfiGcdAllocateMaxAddressSearchBottomUp,
                  EfiGcdMemoryTypeMemoryMappedIo,
                  PCI_SATA_AHCI_BAR_ALIGNMENT,  /// 2^11: 2K Alignment
                  PCI_SATA_AHCI_BAR_LENGTH,     /// 2K Length
                  &MemBaseAddress,
                  mImageHandle,
                  NULL
                  );

  if (Status != EFI_SUCCESS) {
    return EFI_UNSUPPORTED;
  }

  AhciBar = (UINT32) MemBaseAddress;

  //
  // Discover AHCI
  //
  Register = MmPci16 (
               0x00,
               PCI_BUS_NUMBER_PCH_SATA,
               PCI_DEVICE_NUMBER_PCH_SATA,
               PCI_FUNCTION_NUMBER_PCH_SATA,
               0
               );

  if (Register == 0xffff) {
    //
    // Default controller is not initialized, abort procedure.
    //
    return EFI_UNSUPPORTED;
  }
  //
  // Check the class code
  //
  ClassCode    = MmPci8 (
                   0x00,
                   PCI_BUS_NUMBER_PCH_SATA,
                   PCI_DEVICE_NUMBER_PCH_SATA,
                   PCI_FUNCTION_NUMBER_PCH_SATA,
                   PCI_SATA_BASE_CLASS_CODE
                   );
  SubClassCode = MmPci8 (
                   0x00,
                   PCI_BUS_NUMBER_PCH_SATA,
                   PCI_DEVICE_NUMBER_PCH_SATA,
                   PCI_FUNCTION_NUMBER_PCH_SATA,
                   PCI_SATA_SUB_CLASS_CODE
                   );

  if ((ClassCode != PCI_CLASS_MASS_STORAGE) || (SubClassCode != PCI_SATA_SUB_CLASS_CODE_AHCI)) {
    //
    // Not AHCI.
    //
    return EFI_UNSUPPORTED;
  }

  //
  // Set Map to AHCI
  //
  Register = 0;
  Register |= PCI_SATA_MAP_SMS_AHCI;
  Register |= PCI_SATA_PORT_TO_CONTROLLER_CFG;

  MmPci16Or (
    0x00,
    PCI_BUS_NUMBER_PCH_SATA,
    PCI_DEVICE_NUMBER_PCH_SATA,
    PCI_FUNCTION_NUMBER_PCH_SATA,
    PCI_SATA_MAP,
    ((UINT16) Register)
    );

  //
  // Set PCS
  // default is enable all
  //
  Register = MmPci16 (0x00, PCI_BUS_NUMBER_PCH_SATA, PCI_DEVICE_NUMBER_PCH_SATA, PCI_FUNCTION_NUMBER_PCH_SATA, PCI_SATA_PCS) & 0xFFFF;

  for (RegIndex = 0; RegIndex < PCH_AHCI_MAX_PORTS ; RegIndex++) {
    Register |= (PCI_SATA_PCS_PORT0_EN << RegIndex);
  }

  MmPci16Or (
    0x00,
    PCI_BUS_NUMBER_PCH_SATA,
    PCI_DEVICE_NUMBER_PCH_SATA,
    PCI_FUNCTION_NUMBER_PCH_SATA,
    PCI_SATA_PCS,
    ((UINT16) Register)
    );
  //
  // Assign base address register to AHCI
  //
  MmPci32Or (
    0x00,
    PCI_BUS_NUMBER_PCH_SATA,
    PCI_DEVICE_NUMBER_PCH_SATA,
    PCI_FUNCTION_NUMBER_PCH_SATA,
    PCI_SATA_AHCI_BAR,
    AhciBar);

  //
  // Enable AHCI
  //
  Register = 0;
  Register = MmPci16 (
               0x00,
               PCI_BUS_NUMBER_PCH_SATA,
               PCI_DEVICE_NUMBER_PCH_SATA,
               PCI_FUNCTION_NUMBER_PCH_SATA,
               PCI_SATA_COMMAND
               );

  Register |= (PCI_SATA_COMMAND_BME | PCI_SATA_COMMAND_MSE | PCI_SATA_COMMAND_IOSE);
  MmPci16Or (
    0x00,
    PCI_BUS_NUMBER_PCH_SATA,
    PCI_DEVICE_NUMBER_PCH_SATA,
    PCI_FUNCTION_NUMBER_PCH_SATA,
    PCI_SATA_COMMAND,
    ((UINT16) Register)
    );

  Register = *(volatile UINT32*) ((UINTN) AhciBar + PCI_AHCI_HBA_GHC);
  Register |= PCI_SATA_AHCI_GHC_AE;
  *(volatile UINT32*) ((UINTN) AhciBar + PCI_AHCI_HBA_GHC) = Register;

  return EFI_SUCCESS;
}

/**
  Wait for memory set to the test value.

  @param[in] MemTestAddr        The memory address to test
  @param[in] MaskValue          The mask value of memory
  @param[in] TestValue          The test value of memory
  @param[in] WaitTimeOutInMs    The time out value for wait memory set

  @retval EFI_SUCCESS           The memory is correctly set.
  @retval EFI_TIMEOUT           The memory is not set until timeout.

**/
EFI_STATUS
EFIAPI
HddSpindownWaitAhciMemSet (
  IN  UINT32                    *Address,
  IN  UINT32                    MaskValue,
  IN  UINT32                    TestValue,
  IN  UINT32                    Timeout
  )
{
  UINT32                        Value;
  UINT32                        Delay;

  Delay = (Timeout / 100) + 1;

  do {
    //
    // Access sytem memory to see if the value is the tested one.
    //
    // The system memory pointed by Address will be updated by the
    // SATA Host Controller, "volatile" is introduced to prevent
    // compiler from optimizing the access to the memory address
    // to only read once.
    //
    Value  = *(volatile UINT32 *) (UINTN) Address;
    Value &= MaskValue;

    if (Value == TestValue) {
      return EFI_SUCCESS;
    }

    //
    // Stall for 100 microseconds.
    //
    gBS->Stall (100);
    Delay--;

  } while (Delay > 0);

  return EFI_TIMEOUT;
}

/**
 Send STANDBY_IMMEDIATE_CMD via AHCI controller.

 @retval EFI_UNSUPPORTED                AHCI is not found
 @retval EFI_SUCCESS                    AHCI controller is succesfully enabled

**/
EFI_STATUS
EFIAPI
HddSpindownSendAhciHddSpinDownCmd (
  UINT8                                   Port
  )
{
  EFI_STATUS                              Status;
  AHCI_COMMAND_FIS                        *CFis;
  AHCI_COMMAND_LIST                       *CmdList;
  AHCI_COMMAND_TABLE                      *CommandTable;
  AHCI_COMMAND_FIS                        AhciCFis;
  AHCI_COMMAND_LIST                       AhciCmdList;
  AHCI_COMMAND_TABLE                      AhciCommandTable;
  UINT32                                  PortCmdReg;
  UINTN                                   HandleCount;
  UINT64                                  FisBaseAddr;
  UINT64                                  Data;
  EFI_PHYSICAL_ADDRESS                    AhciMemAddr;
  UINTN                                   RfPages;
  UINTN                                   ClPages;
  UINTN                                   CtPages;
  AHCI_RECEIVED_FIS                       *AhciRFis;
  EFI_PHYSICAL_ADDRESS                    AllocateBuffer;
  UINT8                                   *Buffer;

  ZeroMem (&AhciCmdList, sizeof (AHCI_COMMAND_LIST));
  ZeroMem (&AhciCFis, sizeof (AHCI_COMMAND_FIS));

  FisBaseAddr        = 0;
  HandleCount        = 0;
  AhciMemAddr        = 0;
  CmdList            = &AhciCmdList;
  CFis               = &AhciCFis;
  CommandTable       = &AhciCommandTable;

  //
  // Retrieve AhciMemAddr
  //
  AhciMemAddr = MmPci32 (0x00, PCI_BUS_NUMBER_PCH_SATA, PCI_DEVICE_NUMBER_PCH_SATA, PCI_FUNCTION_NUMBER_PCH_SATA, PCI_SATA_AHCI_BAR);

  RfPages = EFI_SIZE_TO_PAGES (MAX_RECEIVE_FIS_SIZE);
  ClPages = EFI_SIZE_TO_PAGES (MAX_COMMAND_LIST_SIZE);
  CtPages = EFI_SIZE_TO_PAGES (MAX_COMMAND_TABLE_SIZE);

  //
  // Stall 500ms for compatibility with some PCIe SSD.
  //
  gBS->Stall (500 * 1000);

  //
  // Allocate page in ReservedMemory
  //
  AllocateBuffer = 0xffffffff;
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiReservedMemoryType,
                  (RfPages + ClPages + CtPages),
                  &AllocateBuffer
                  );
  Buffer = (VOID*) ((UINTN) (AllocateBuffer));
  if ((Status != EFI_SUCCESS) || (Buffer == NULL)) {
    if (Buffer != NULL) {
      gBS->FreePages (AllocateBuffer, (UINTN) (RfPages + ClPages + CtPages));
    }
    return Status;
  }
  ZeroMem ((VOID*) ((UINTN) (Buffer)), ((RfPages + ClPages + CtPages) * EFI_PAGE_SIZE));
  AhciRFis         = (AHCI_RECEIVED_FIS*) Buffer;
  CmdList          = (AHCI_COMMAND_LIST*) (Buffer + (RfPages * EFI_PAGE_SIZE));
  CommandTable     = (AHCI_COMMAND_TABLE*) (Buffer + ((RfPages + ClPages) * EFI_PAGE_SIZE));

  //
  // Setup port FisBaseAddr, accessing by dword for compatibility
  //
  Data = (UINT64) (UINTN) (AhciRFis) + RECEIVE_FIS_SIZE * Port;
  *(UINT32*) HBA_PORT_MEM_ADDR (AhciMemAddr, Port, HBA_PORTS_FB)  = *(UINT32*) ((UINTN)&Data + 0);
  *(UINT32*) HBA_PORT_MEM_ADDR (AhciMemAddr, Port, HBA_PORTS_FBU) = *(UINT32*) ((UINTN)&Data + 4);
  Data = (UINT64) (UINTN) (CmdList);
  *(UINT32*) HBA_PORT_MEM_ADDR (AhciMemAddr, Port, HBA_PORTS_CLB)  = *(UINT32*) ((UINTN)&Data + 0);
  *(UINT32*) HBA_PORT_MEM_ADDR (AhciMemAddr, Port, HBA_PORTS_CLBU) = *(UINT32*) ((UINTN)&Data + 4);

  //
  // Set CFis type to FIS_REGISTER_H2D;
  //
  CFis->Ahci_CFis_Type     = 0x27;
  //
  // Indicate it's a command
  //
  CFis->Ahci_CFis_CmdInd   = 1;
  CFis->Ahci_CFis_Cmd      = 0xE0;

  CmdList->Ahci_Cmd_Cfl = 5;
  *(UINT32*) ((UINT8*) &FisBaseAddr + 0) = *(UINT32*) HBA_PORT_MEM_ADDR (AhciMemAddr, Port, HBA_PORTS_FB);
  *(UINT32*) ((UINT8*) &FisBaseAddr + 4) = *(UINT32*) HBA_PORT_MEM_ADDR (AhciMemAddr, Port, HBA_PORTS_FBU);
  //
  // Clear RFis buffer
  //
  ZeroMem ((VOID*) ((UINTN) FisBaseAddr), 0x100);
  //
  // Build command
  //
  ZeroMem (CommandTable, (COMMAND_TABLE_SIZE + PRDT_SIZE * SUPPORT_PRDT_NUMBER));
  CFis->Ahci_CFis_DevHead  = 0xE0;
  CopyMem (&CommandTable->CFis, CFis, CFIS_TABLE_SIZE);
  HBA_PORT_REG32_AND (
        AhciMemAddr,
        Port,
        HBA_PORTS_CMD,
        ~(HBA_PORTS_CMD_DLAE | HBA_PORTS_CMD_ATAPI)
        );
  //
  // Setup Command List
  //
  *(UINTN*) (&CmdList->Ahci_Cmd_Ctba) = (UINTN) CommandTable;

  //
  // Clear any error status
  //
  HBA_PORT_CLEAR_STS (AhciMemAddr, Port, HBA_PORTS_SERR);
  //
  // Clear any port interrupt status
  //
  HBA_PORT_CLEAR_STS (AhciMemAddr, Port, HBA_PORTS_IS);

  //
  // Clear any hba interrupt status
  //
  HBA_CLEAR_STS (AhciMemAddr, HBA_IS);
  //
  // Enable FIS receive
  //
  HBA_PORT_REG32_OR (AhciMemAddr, Port, HBA_PORTS_CMD, HBA_PORTS_CMD_FRE);

  HddSpindownWaitAhciMemSet (
    HBA_PORT_MEM_ADDR (AhciMemAddr, Port, HBA_PORTS_CMD),
    HBA_PORTS_CMD_FR,
    HBA_PORTS_CMD_FR,
    HBA_RFIS_START_TIME_OUT
    );

  HBA_PORT_REG32_OR (AhciMemAddr, Port, HBA_PORTS_CMD, HBA_PORTS_CMD_ST);
  //
  // Setting the command on specific slot
  //
  HBA_PORT_REG32_AND_OR (AhciMemAddr, Port, HBA_PORTS_SACT, 0, 1);

  HBA_PORT_REG32_AND_OR (AhciMemAddr, Port, HBA_PORTS_CI, 0, 1);
  //
  // Wait device sends the Response Fis
  //
  Status = HddSpindownWaitAhciMemSet (
               HBA_PORT_MEM_ADDR (AhciMemAddr, Port, HBA_PORTS_CI),
               0xFFFFFFFF,
               0,
               HBA_NONDATA_CMD_ISSUED_TIMEOUT
               );

  Status = HddSpindownWaitAhciMemSet (
             (UINT32*)((UINTN)(FisBaseAddr + D2H_FIS_OFFSET)),
             FIS_TYPE_MASK, FIS_REGISTER_D2H,
             HBA_NONDATA_DEVICE_ACK_TIMEOUT
             );

  //
  // Stop command
  //
  PortCmdReg = HBA_PORT_REG32 (AhciMemAddr, Port, HBA_PORTS_CMD);
  if ((PortCmdReg & (HBA_PORTS_CMD_ST |  HBA_PORTS_CMD_CR)) != 0) {
    if ((PortCmdReg & HBA_PORTS_CMD_ST) != 0) {
      HBA_PORT_REG32_AND (AhciMemAddr, Port, HBA_PORTS_CMD, ~(HBA_PORTS_CMD_ST));
    }
    HddSpindownWaitAhciMemSet (
      HBA_PORT_MEM_ADDR (AhciMemAddr, Port, HBA_PORTS_CMD),
      HBA_PORTS_CMD_CR,
      0,
      HBA_COMMAND_STOP_TIME_OUT
      );
  }
  //
  // Disable FIS receive
  //
  if (!AHCI_COMMAND_RUNNING (HBA_PORT_REG32 (AhciMemAddr, Port, HBA_PORTS_CMD))){
    if (!AHCI_FIS_RUNNING (HBA_PORT_REG32 (AhciMemAddr, Port, HBA_PORTS_CMD))){
      //
      // if the CMD.FR bit is non-zero means that the CMD.FRE bit has not been cleared.
      //
      HBA_PORT_REG32_AND (AhciMemAddr, Port, HBA_PORTS_CMD, ~(HBA_PORTS_CMD_FRE));
      HddSpindownWaitAhciMemSet (
        HBA_PORT_MEM_ADDR (AhciMemAddr, Port, HBA_PORTS_CMD),
        HBA_PORTS_CMD_FR,
        0,
        HBA_RFIS_STOP_TIME_OUT
        );
    }
  }
  gBS->FreePages (AllocateBuffer, (UINTN) (RfPages + ClPages + CtPages));
  return Status;
}

/**
  Get the AtaPassThru protocol installed on the target device and issue command.

  @param[in] ControllerMode     Indicate that now is IDE mode or AHCI mode.
  @param[in] CmdPacket          AtaPassThru command packet.
  @param[in] MappingTable       Port mapping table.

  @return Status Code

**/
EFI_STATUS
EFIAPI
CommandIssue (
  IN UINT8                             ControllerMode,
  IN EFI_ATA_PASS_THRU_COMMAND_PACKET  *CmdPacket,
  IN PORT_NUMBER_MAP                   *MappingTable
  )
{
  EFI_STATUS                   Status;
  UINT8                        HandleCount;
  UINTN                        DxeHandleCount;
  UINT8                        HandleIndex;
  UINTN                        BufferSize;
  UINT16                       Port;
  UINT16                       MultiplierPort;
  BOOLEAN                      SmmLocateAtaPassThruFlag;
  EFI_HANDLE                   *HandleBuffer;
  EFI_DEVICE_PATH_PROTOCOL     *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL     *DevicePathNode;
  PCI_DEVICE_PATH              *PciDevicePath;
  EFI_ATA_PASS_THRU_PROTOCOL   *AtaPassThruPtr;

  Port = 0;
  MultiplierPort = 0;
  HandleCount = 0;
  DxeHandleCount = 0;
  BufferSize = 0;
  HandleBuffer = NULL;
  PciDevicePath = NULL;
  DevicePath = NULL;
  AtaPassThruPtr = NULL;
  SmmLocateAtaPassThruFlag = TRUE;
  //
  // Get the target AtaPassThruProtocol.
  //
  Status = gSmst->SmmLocateHandle (
                     ByProtocol,
                     &gEfiAtaPassThruProtocolGuid,
                     NULL,
                     &BufferSize,
                     HandleBuffer
                     );
  if ((Status != EFI_SUCCESS) && (Status != EFI_BUFFER_TOO_SMALL)) {
    //
    // Try gBS to get the target AtaPassThruProtocol.
    //
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiAtaPassThruProtocolGuid,
                    NULL,
                    &DxeHandleCount,
                    &HandleBuffer
                  );
    if (Status != EFI_SUCCESS) {
      return EFI_UNSUPPORTED;
    }
    SmmLocateAtaPassThruFlag = FALSE;
  } else {
    if (Status == EFI_BUFFER_TOO_SMALL) {
      Status = gSmst->SmmAllocatePool (
                        EfiRuntimeServicesData,
                        BufferSize,
                        (VOID*) &HandleBuffer
                        );
      if (HandleBuffer == NULL) {
        return EFI_NOT_FOUND;
      }
      Status = gSmst->SmmLocateHandle (
                        ByProtocol,
                        &gEfiAtaPassThruProtocolGuid,
                        NULL,
                        &BufferSize,
                        HandleBuffer
                        );
    }
  }
  if (SmmLocateAtaPassThruFlag) {
    HandleCount = (UINT8) (BufferSize / sizeof (EFI_HANDLE));
  } else {
    HandleCount = (UINT8) DxeHandleCount;
  }
  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    if (SmmLocateAtaPassThruFlag) {
      Status = gSmst->SmmHandleProtocol (
                        HandleBuffer[HandleIndex],
                        &gEfiDevicePathProtocolGuid,
                        (VOID *) &DevicePath
                        );
    } else {
      Status = gBS->HandleProtocol (
                      HandleBuffer[HandleIndex],
                      &gEfiDevicePathProtocolGuid,
                      (VOID *) &DevicePath
                      );
    }
    if ((Status == EFI_SUCCESS)) {
      DevicePathNode = DevicePath;
      while (!IsDevicePathEnd (DevicePathNode)) {
        if (DevicePathType (DevicePathNode) == HARDWARE_DEVICE_PATH &&
            DevicePathSubType (DevicePathNode) == HW_PCI_DP) {
          PciDevicePath = (PCI_DEVICE_PATH *) DevicePathNode;
          break;
        }
        DevicePathNode = NextDevicePathNode (DevicePathNode);
      }

      if (PciDevicePath == NULL) {
        continue;
      }

      if (((ControllerMode == IdeType) &&
           (PciDevicePath->Device == MappingTable->Device) &&
           (PciDevicePath->Function== MappingTable->Function)) ||
          (ControllerMode == AhciType)) {
        if (SmmLocateAtaPassThruFlag) {
          Status = gSmst->SmmHandleProtocol (
                            HandleBuffer[HandleIndex],
                            &gEfiAtaPassThruProtocolGuid,
                            &AtaPassThruPtr
                            );
        } else {
          Status = gBS->HandleProtocol (
                          HandleBuffer[HandleIndex],
                          &gEfiAtaPassThruProtocolGuid,
                          &AtaPassThruPtr
                          );
        }
        if (Status != EFI_SUCCESS) {
          continue;
        }
        if (ControllerMode == IdeType) {
          Port = MappingTable->PrimarySecondary;
          MultiplierPort = MappingTable->SlaveMaster;
        } else {
          Port = (UINT16) MappingTable->PortNum;
          MultiplierPort = 0;
        }
        break;
      }
    }
  }

  if ((AtaPassThruPtr == NULL) || (Status != EFI_SUCCESS)) {
    gSmst->SmmFreePool (HandleBuffer);
    return EFI_NOT_FOUND;
  }
  Status = AtaPassThruPtr->PassThru (
                             AtaPassThruPtr,
                             Port,
                             MultiplierPort,
                             CmdPacket,
                             0
                             );

  gSmst->SmmFreePool (HandleBuffer);

  return Status;
}

/**
  Issue STANDBY_IMMEDIATE command by AtaPassThru protocol.

  @param[in] This               HDD_SPINDOWN_PROTOCOL instance.
  @param[in] TargetPort         Target port (device).

  @return Status Code

**/
EFI_STATUS
EFIAPI
HDDSpinDown (
  IN HDD_SPINDOWN_PROTOCOL            *This,
  IN UINT16                           TargetPort
  )
{
  EFI_STATUS                          Status;
  EFI_ATA_PASS_THRU_COMMAND_PACKET    AtaPassThruCmdPacket;
  EFI_ATA_STATUS_BLOCK                Asb;
  EFI_ATA_COMMAND_BLOCK               Acb;
  UINT8                               NumOfPorts;
  UINT8                               Index;
  HDD_SPINDOWN_PRIVATE_DATA           *PrivateData;
  PORT_NUMBER_MAP                     *PortMappingTable;
  PORT_NUMBER_MAP                     EndEntry;

  PrivateData = HDD_SPINDOWN_FROM_HDDSPINDOWN (This);

  //
  // Attempt to set controller mode if it is still unknown.
  //
  if (PrivateData->ControllerMode == Unknown) {
    Status = HddSpindownSetControllermode (PrivateData);
    if (Status != EFI_SUCCESS) {
      //
      // SATA driver is not started, try to send command via AHCI controller.
      //
      if (MmPci32 (
            0x00,
            PCI_BUS_NUMBER_PCH_SATA,
            PCI_DEVICE_NUMBER_PCH_SATA,
            PCI_FUNCTION_NUMBER_PCH_SATA,
            PCI_SATA_AHCI_BAR) == 0) {
        //
        // AHCI controller has not been initialized, intialize it first.
        //
        Status = HddSpindownProgramAhciController ();
        if (Status != EFI_SUCCESS) {
          return Status;
        }
      }
      return HddSpindownSendAhciHddSpinDownCmd (((UINT8) TargetPort));
    }
  }

  //
  // Initialize Command packet
  //
  ZeroMem (&AtaPassThruCmdPacket, sizeof (EFI_ATA_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Asb, sizeof (EFI_ATA_STATUS_BLOCK));
  ZeroMem (&Acb, sizeof (EFI_ATA_COMMAND_BLOCK));

  AtaPassThruCmdPacket.Asb = &Asb;
  AtaPassThruCmdPacket.Acb = &Acb;

  PortMappingTable = (PORT_NUMBER_MAP*) PcdGetPtr (PcdPortNumberMapTable);
  ZeroMem (&EndEntry, sizeof (PORT_NUMBER_MAP));
  NumOfPorts = 0;
  while (CompareMem (&EndEntry, &PortMappingTable[NumOfPorts], sizeof (PORT_NUMBER_MAP)) != 0) {
    NumOfPorts++;
  }

  if (NumOfPorts == 0) {
    return EFI_NOT_FOUND;
  }

  for (Index = 0; Index < NumOfPorts; Index++) {
    if (PortMappingTable[Index].PortNum == TargetPort) {
      break;
    }
  }

  if (PrivateData->ControllerMode == IdeType) {
    //
    // IDE mode
    //
    AtaPassThruCmdPacket.Acb->AtaDeviceHead |= ((PortMappingTable[Index].SlaveMaster << 4) | 0xE0);
  }
  //
  // Send "STANDBY IMMEDIATE"command to HDD.
  //
  AtaPassThruCmdPacket.Acb->AtaCommand = STANDBY_IMMEDIATE_CMD;
  AtaPassThruCmdPacket.Protocol = EFI_ATA_PASS_THRU_PROTOCOL_ATA_NON_DATA;
  AtaPassThruCmdPacket.Length = EFI_ATA_PASS_THRU_LENGTH_NO_DATA_TRANSFER;

  Status = CommandIssue (
             PrivateData->ControllerMode,
             &AtaPassThruCmdPacket,
             &PortMappingTable[Index]
             );
  return Status;
}


/**
  HddSpindown SMM AtaPassThru Protocol callback function.

  @param[in] Protocol           Points to the protocol's unique identifier.
  @param[in] Interface          Points to the interface instance.
  @param[in] Handle             The handle on which the interface was installed.

  @return Status Code

**/
EFI_STATUS
EFIAPI
HddSpindownAtaPassThruCallBack (
  IN CONST EFI_GUID             *Protocol,
  IN       VOID                 *Interface,
  IN       EFI_HANDLE           Handle
  )
{
  EFI_STATUS                    Status;
  HDD_SPINDOWN_PROTOCOL         *HddSpindown;
  HDD_SPINDOWN_PRIVATE_DATA     *PrivateData;

  Status = gSmst->SmmLocateProtocol (&gHddSpindownProtocolGuid, NULL, (VOID **)&HddSpindown);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  PrivateData = HDD_SPINDOWN_FROM_HDDSPINDOWN (HddSpindown);
  return HddSpindownSetControllermode (PrivateData);

}

/**
  HddSpindown driver Entry Point.

  @param[in] ImageHnadle        While the driver image loaded be the ImageLoader(), an image handle is assigned to this
                                driver binary, all activities of the driver is tied to this ImageHandle.
  @param[in] SystemTable        A pointer to the system table, for all BS(Boot Services) and RT(Runtime Services).
  @param[in] Handle             The handle on which the interface was installed.

  @return Status Code

**/
EFI_STATUS
EFIAPI
HddSpindownEntry (
  IN EFI_HANDLE                  ImageHandle,
  IN EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                     Status;
  EFI_HANDLE                     Handle;
  HDD_SPINDOWN_PRIVATE_DATA      *PrivateData;
  VOID                           *Registration;

  mImageHandle = ImageHandle;

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (HDD_SPINDOWN_PRIVATE_DATA),
                    &PrivateData
                    );
  if (Status != EFI_SUCCESS) {
    return EFI_OUT_OF_RESOURCES;
  }
  PrivateData->Signature = EFI_HDD_SPINDOWN_SIGNATURE;
  PrivateData->HddSpindown.HddSpinDown = HDDSpinDown;

  Status = HddSpindownSetControllermode (PrivateData);
  if (Status != EFI_SUCCESS) {
    //
    // SATA initialization may have not been started, install protocol first, register call back function,
    // and check later.
    //
    PrivateData->ControllerMode = Unknown;
    Status = gSmst->SmmRegisterProtocolNotify (
                      &gEfiAtaPassThruProtocolGuid,
                      HddSpindownAtaPassThruCallBack ,
                      &Registration
                      );
    if (Status != EFI_SUCCESS) {
      return Status;
    }
  }

  Handle = NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                     &Handle,
                     &gHddSpindownProtocolGuid,
                     EFI_NATIVE_INTERFACE,
                     &PrivateData->HddSpindown
                    );
  return Status;
}
