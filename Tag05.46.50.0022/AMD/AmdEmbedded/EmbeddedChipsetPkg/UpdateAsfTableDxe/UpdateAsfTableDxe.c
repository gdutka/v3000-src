/** @file

;*******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#include <UpdateAsfTableDxe.h>

EFI_STATUS
RemoveAsfTable (
  VOID
  )
{
  EFI_STATUS                              Status;
  EFI_ACPI_SUPPORT_PROTOCOL               *AcpiSupport;
  INTN                                    Index;
  UINTN                                   Handle;
  EFI_ACPI_TABLE_VERSION                  Version;
  EFI_ACPI_DESCRIPTION_HEADER             *Table;

  //
  // Found  table
  //
  Status = gBS->LocateProtocol (
                  &gEfiAcpiSupportProtocolGuid,
                  NULL,
                  &AcpiSupport
                  );
  if (EFI_ERROR (Status)) {
	return EFI_UNSUPPORTED;
  }

  Index  = 0;
  Handle = 0;
  do {
    Table = NULL;
    Status = AcpiSupport->GetAcpiTable (
                            AcpiSupport,
                            Index,
                            &Table,
                            &Version,
                            &Handle
                            );
    if (EFI_ERROR (Status)) {
      break;
    }

    //
    // Check Signture and update table
    //
    if (Table->Signature == EFI_ACPI_1_0_ASF_DESCRIPTION_TABLE_SIGNATURE) {
      //
      // Delete the table
      //
      gBS->FreePool (Table);
      Table = NULL;
      Status = AcpiSupport->SetAcpiTable (
                              AcpiSupport,
                              Table,
                              TRUE,
                              Version,
                              &Handle
                              );
      if (EFI_ERROR (Status)) {
        return EFI_UNSUPPORTED;
      }
      return EFI_SUCCESS;
    }
    //
    // Not found, get next
    //
    gBS->FreePool (Table);
    Index++;
  } while (TRUE);

  return EFI_SUCCESS;
}

/**
  This function calculates and updates an UINT8 checksum.

  @param  Buffer          Pointer to buffer to checksum
  @param  Size            Number of bytes to checksum

**/
VOID
AcpiPlatformChecksum (
  IN UINT8      *Buffer,
  IN UINTN      Size
  )
{
  UINTN ChecksumOffset;

  ChecksumOffset = OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER, Checksum);

  //
  // Set checksum to 0 first
  //
  Buffer[ChecksumOffset] = 0;

  //
  // Update checksum value
  //
  Buffer[ChecksumOffset] = CalculateCheckSum8(Buffer, Size);
}

/**
  The user Entry Point for module UpdateAsfTableDxe.  The user code starts with this function.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
UpdateAsfTableDxeEntryPoint (
  IN EFI_HANDLE                      ImageHandle,
  IN EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                         Status;
  UINTN                              TableSize;
  EFI_ACPI_1_0_ASF_DESCRIPTION_TABLE *AsfTable;
  EFI_ACPI_TABLE_PROTOCOL            *AcpiTableProtocol;
  DASH_IO_CFG_PROTOCOL               *DashIoCfg;
  UINT8                              AsfSmbusAddr;
  UINTN                              TableHandle;
  UINT8                              Index;
  UINT8                              ControlCommand[ASF_RCTL_SUPPORT_COMMAND_COUNT] = {0x50, 0x52, 0x51, 0x53};

  EFI_ASF_ALERTDATA                  DeviceArray[ASF_ALERT_DEVICE_COUNT] = {
    0x80, 0x23, 0x01, 0x01, 0x01, 0x01, 0x00, 0x39, 0x10, 0x00, 0x07, 0x00,  // Legacy Sensor 1_1
    0x80, 0x23, 0x02, 0x02, 0x01, 0x01, 0x00, 0x39, 0x10, 0x01, 0x07, 0x00,  // Legacy Sensor 1_2
    0x80, 0x23, 0x04, 0x04, 0x01, 0x01, 0x00, 0x39, 0x10, 0x02, 0x07, 0x00,  // Legacy Sensor 1_3
    };

  UINT8                              RemoteControlCapabilities[ASF_RECORE_TYPE_RMC_COUNT] = {
    // System Firmware Capabilities Bit Mask
    0x21,                       // Supports Lock Keyboard(BIT5)
                                // Supports Firmware Verbosity/Screen Blank(BIT0),
    0xF8,                       // Supports Configuration Data Reset(BIT7),
                                // Supports Firmware Verbosity/Quiet(BIT6),
                                // Supports Firmware Verbosity/Verbose(BIT5),
                                // Supports Forced Progress Events(BIT4),
                                // Supports User Password Bypass(BIT3),
    0x00,                       // Reserve for future
    0x00,                       // Reserve for future
    // Special Commands Bit Mask
    0x00,                       // Reserved
    0x1B,                       // Supports Force CD/DVD Boot command(BIT4)
                                // Supports Force Diagnostic Boot command(BIT3)
                                // Supports Force Hard-drive Boot command(BIT1)
                                // Supports Force PXE Boot command(BIT0)
    // System Capabilities Bit Mask
    0xF0,                       // Supports Reset on either the compatibility or secure port(BIT7)
                                // Supports Power-Up on either the compatibility or secure port(BIT6)
                                // Supports Power-Down on either the compatibility or secure port(BIT5)
                                // Supports Power Cycle Reset on either the compatibility or secure port(BIT4)
    };
  //
  // If this driver will be dispatched, it means DASH is enable and might be support
  //
  Status = EFI_SUCCESS;
  TableSize = 0;
  AsfTable = NULL;
  TableHandle = 0;

  //
  // Find the AcpiTable protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  (VOID**)&AcpiTableProtocol
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return EFI_ABORTED;
  }

  //
  // 1. Allocate the buffer for ASF table
  //
  TableSize = sizeof (EFI_ACPI_1_0_ASF_DESCRIPTION_TABLE);
  AsfTable = AllocateReservedPool (TableSize);
  if (AsfTable == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((EFI_D_ERROR, "AllocatePool for ASF table : %r\n", Status));
    return Status;
  }


  Status = gBS->LocateProtocol (
                  &gDashIoCfgProtocolGuid,
                  NULL,
                  &DashIoCfg
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  AsfSmbusAddr = DashIoCfg->AsfReadReg (DashIoCfg, ASF_REMOTE_CTRL_ADR);
  //
  // 2. Allocate Pool Success and can remove original ASF table
  //
  RemoveAsfTable();

  //
  // 3. Set the parameter to ASF table from the PCD which will also set into the relative protocol of DASH
  //
  AsfTable->Header.Signature = EFI_ACPI_1_0_ASF_DESCRIPTION_TABLE_SIGNATURE;
  AsfTable->Header.Length = sizeof(EFI_ACPI_1_0_ASF_DESCRIPTION_TABLE);
  AsfTable->Header.Revision = EFI_ACPI_1_0_ASF_DESCRIPTION_TABLE_REVISION;
  ZeroMem (AsfTable->Header.OemId, sizeof (AsfTable->Header.OemId));
  CopyMem (
    AsfTable->Header.OemId,
    PcdGetPtr (PcdAcpiDefaultOemId),
    MIN (PcdGetSize (PcdAcpiDefaultOemId), sizeof (AsfTable->Header.OemId))
    );
  WriteUnaligned64 (&AsfTable->Header.OemTableId, PcdGet64 (PcdAcpiDefaultOemTableId));
  AsfTable->Header.OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  AsfTable->Header.CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  AsfTable->Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  //
  // ASF INFO
  //
  AsfTable->AsfInfo.RecordHeader.Type = ASF_RECORE_TYPE_INFO;
  AsfTable->AsfInfo.RecordHeader.Reserved = 0x00;
  AsfTable->AsfInfo.RecordHeader.RecordLength = sizeof (EFI_ACPI_ASF_INFO);

  AsfTable->AsfInfo.MinWatchDogResetValue = 0x00;
  AsfTable->AsfInfo.MinPollingInterval = 0xFF;
  AsfTable->AsfInfo.SystemID = 0x01;
  AsfTable->AsfInfo.IANAManufactureID = 0x57010000;
  AsfTable->AsfInfo.FeatureFlags = 0x00;
  for (Index = 0; Index < 3; Index++) {
    AsfTable->AsfInfo.Reserved[Index] = 0x00;
  }

  //
  // ASF ALRT
  //
  AsfTable->AsfAlert.RecordHeader.Type = ASF_RECORE_TYPE_ALRT;
  AsfTable->AsfAlert.RecordHeader.Reserved = 0x00;
  AsfTable->AsfAlert.RecordHeader.RecordLength = sizeof (EFI_ACPI_ASF_ALRT);

  AsfTable->AsfAlert.AssertionEventBitMask = 0x00;
  AsfTable->AsfAlert.DeassertionEventBitMask = 0x00;
  AsfTable->AsfAlert.NumberOfAlerts = ASF_ALERT_DEVICE_COUNT;
  AsfTable->AsfAlert.ArrayElementLength = sizeof(EFI_ASF_ALERTDATA);
  CopyMem (
    AsfTable->AsfAlert.DeviceArray,
    DeviceArray,
    (sizeof(EFI_ASF_ALERTDATA) * ASF_ALERT_DEVICE_COUNT)
    );

  //
  // ASF RCTL
  //
  AsfTable->AsfRctl.RecordHeader.Type = ASF_RECORE_TYPE_RCTL;
  AsfTable->AsfRctl.RecordHeader.Reserved = 0x00;
  AsfTable->AsfRctl.RecordHeader.RecordLength = sizeof (EFI_ACPI_ASF_RCTL);

  AsfTable->AsfRctl.NumberOfControls = ASF_RCTL_SUPPORT_COMMAND_COUNT;
  AsfTable->AsfRctl.ArrayElementLength = sizeof(EFI_ASF_CONTROLDATA);
  AsfTable->AsfRctl.RctlReserved = 0x0000;

  for (Index = 0; Index < ASF_RCTL_SUPPORT_COMMAND_COUNT; Index++) {
    AsfTable->AsfRctl.ControlArray[Index].Function = Index;
    AsfTable->AsfRctl.ControlArray[Index].DeviceAddress = (AsfSmbusAddr | ASF_RCTL_WITH_PEC);
    AsfTable->AsfRctl.ControlArray[Index].Command = ControlCommand[Index];
    AsfTable->AsfRctl.ControlArray[Index].DataValue = 0x00;
  }

  //
  // ASF RMCP
  //
  AsfTable->AsfRmcp.RecordHeader.Type = ASF_RECORE_TYPE_RMCP;
  AsfTable->AsfRmcp.RecordHeader.Reserved = 0x00;
  AsfTable->AsfRmcp.RecordHeader.RecordLength = sizeof (EFI_ACPI_ASF_RMCP);

  CopyMem (
    AsfTable->AsfRmcp.RemoteControlCapabilities,
    RemoteControlCapabilities,
    ASF_RECORE_TYPE_RMC_COUNT
    );
  AsfTable->AsfRmcp.RMCPCompletionCode = 0x00;
  AsfTable->AsfRmcp.RMCPIANA = 0x57010000;
  AsfTable->AsfRmcp.RMCPSpecialCommand = 0x00;
  AsfTable->AsfRmcp.RMCPSpecialCommandParameter[0] = 0x00;
  AsfTable->AsfRmcp.RMCPSpecialCommandParameter[1] = 0x00;
  AsfTable->AsfRmcp.RMCPBootOptions[0] = 0x01;
  AsfTable->AsfRmcp.RMCPBootOptions[1] = 0x00;
  AsfTable->AsfRmcp.RMCPOEMParameters[0] = 0x00;
  AsfTable->AsfRmcp.RMCPOEMParameters[1] = 0x00;

  //
  // ASF ADDR (the last type might need to set BIT7)
  //
  AsfTable->AsfAddr.RecordHeader.Type = (ASF_RECORE_TYPE_ADDR | ASF_RECORE_TYPE_LAST);
  AsfTable->AsfAddr.RecordHeader.Reserved = 0x00;
  AsfTable->AsfAddr.RecordHeader.RecordLength = sizeof (EFI_ACPI_ASF_ADDR);

  AsfTable->AsfAddr.SEEPROMAddress = 0x00;
  AsfTable->AsfAddr.NumberOfDevices = ASF_ADDR_DEVICE_ARRAY_LENGTH;
  ZeroMem (AsfTable->AsfAddr.FixedSmbusAddresses, ASF_ADDR_DEVICE_ARRAY_LENGTH);

  AsfTable->AsfAddr.FixedSmbusAddresses[0] = 0xA0;
  AsfTable->AsfAddr.FixedSmbusAddresses[1] = 0xA2;
  AsfTable->AsfAddr.FixedSmbusAddresses[2] = 0xA4;
  //
  // Might update checksum after all information updated
  //
  AcpiPlatformChecksum ((UINT8*)AsfTable, TableSize);

  //
  // Install ACPI table
  //
  Status = AcpiTableProtocol->InstallAcpiTable (
                                AcpiTableProtocol,
                                AsfTable,
                                TableSize,
                                &TableHandle
                                );
  return Status;
}
