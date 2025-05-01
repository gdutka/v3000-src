/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/DebugLib.h>
#include <Ppi/FchI3cMaster.h>
#include <AmdI3cMasterConsumerPei.h>

STATIC EFI_PEI_SPDI3C_PPI mSpdI3cPpi = {
  SPDI3C_PPI_REVISION,
  SpdI3cSet,
  SpdI3cGet
};

STATIC EFI_PEI_PPI_DESCRIPTOR mSpdI3cPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gSpdI3cPpiGuid,
  &mSpdI3cPpi
};


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM SPD PEIM
 *
 * This function registers the function to publish SPD I3C ppi.
 *
 * @param[in]     FileHandle     Pointer to the firmware file system header
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
SpdI3cPeiEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  DEBUG ((EFI_D_ERROR, "SpdI3cPeiEntryPoint\n"));

  // Publish SpdI3c control Ppi
  Status = (*PeiServices)->InstallPpi (
                           PeiServices,
                           &mSpdI3cPpiList
                           );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

EFI_STATUS
EFIAPI
SpdI3cSet (
  IN       CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Command,
  IN       UINT8             Data
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PEI_I3C_MASTER_PPI  *I3cMaster;
  EFI_I3C_REQUEST_PACKET  RequestPacket;
  EFI_I3C_OPERATION       *Operation;
  UINT8                   Buffer[2];

  // BusSelect is to select the instances on the PpiList, which will be 0, 1, 2, 3.
  DEBUG ((EFI_D_ERROR, "SpdI3cSet BusSelect = 0x%x, SlaveAddress=0x%x\n, Command = 0x%x, Data=0x%x \n",
                        BusSelect, SlaveAddress, Command, Data));

  Status = (*PeiServices)->LocatePpi (
     PeiServices,
     &gAmdFchSNI3cPpiGuid,
     BusSelect,
     NULL,
     &I3cMaster
     );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //Prepare write data buffer
  Buffer[0] = Command;
  Buffer[1] = Data;

  //Write Byte Operation
  Operation   = RequestPacket.Operation;
  RequestPacket.OperationCount = 1;
  Operation[0].Flags           = 0;
  // Note: Operation[0].LengthInBytes is hardcoded to 3 in this SPD Write transaction example.
  // In I3C mode, Operation[0].LengthInBytes should be set to 3, since the Command/Offset is 2-byte, and the data is 1-byte.
  // In I2C mode, Operation[0].LengthInBytes should be set to 2, since the Command/Offset is 1-byte, and the data is 1-byte.
  // Buffer[] should be configured properly. For example, if the Write Cmd has Offset = 0x010B, and the Write Data is 03,
  // then the Buffer will be:
  // Buffer[0] = 0x0B; // lower byte of Offset/Command
  // Buffer[1] = 0x01; // higher byte of Offset/Command
  // Buffer[2] = 0x03; // data byte
  Operation[0].LengthInBytes   = 3;
  Operation[0].Buffer          = &Buffer[0];

  Status = I3cMaster->StartRequest (I3cMaster, SlaveAddress, &RequestPacket);

  return Status;
}

EFI_STATUS
EFIAPI
SpdI3cGet (
  IN       CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Command,
  OUT      UINT8             *Data
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_PEI_I3C_MASTER_PPI  *I3cMaster;
  UINT8                   PacketBuffer[sizeof (EFI_I3C_REQUEST_PACKET) + sizeof (EFI_I3C_OPERATION)];
  EFI_I3C_REQUEST_PACKET  *RequestPacket;
  EFI_I3C_OPERATION       *Operation;

  // BusSelect is to select the instances on the PpiList, which will be 0, 1, 2, 3.
  DEBUG ((EFI_D_ERROR, "SpdI3cGet BusSelect = 0x%x, SlaveAddress=0x%x\n, Command = 0x%x, *Data=0x%x \n",
                        BusSelect, SlaveAddress, Command, *Data));

  Status = (*PeiServices)->LocatePpi (
     PeiServices,
     &gAmdFchSNI3cPpiGuid,
     BusSelect,
     NULL,
     &I3cMaster
     );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  RequestPacket = (EFI_I3C_REQUEST_PACKET*)PacketBuffer;
  Operation   = RequestPacket->Operation;
  RequestPacket->OperationCount = 2;
  Operation[0].Flags            = 0;
  // Note: Operation[0].LengthInBytes is hardcoded to 2 in this SPD Read transaction example.
  // In I3C mode, Operation[0].LengthInBytes should be set to 2, since the Command/Offset is 2-byte.
  // In I2C mode, Operation[0].LengthInBytes should be set to 1, since the Command/Offset is 1-byte.
  // Operation[0].Buffer[] should be configured properly. For example, if the Write Cmd has Offset = 0x010B,
  // then the Buffer will be:
  // Buffer[0] = 0x0B; // lower byte of Offset/Command
  // Buffer[1] = 0x01; // higher byte of Offset/Command
  Operation[0].LengthInBytes    = 2;
  Operation[0].Buffer           = &Command;
  Operation[1].Flags            = I3C_FLAG_READ;
  Operation[1].LengthInBytes    = 1; // Number of bytes to read. //sizeof (UINT8);
  Operation[1].Buffer           = (UINT8*)Data;

  Status = I3cMaster->StartRequest (I3cMaster, SlaveAddress, RequestPacket);

  return Status;
}

