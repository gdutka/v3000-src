/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/DebugLib.h>
#include <Protocol/FchI3cMasterProtocol.h>
#include <AmdI3cMasterConsumerDxe.h>

extern  EFI_BOOT_SERVICES       *gBS;

#ifndef SPDI3C_DEADLOOP
  #define SPDI3C_DEADLOOP()    { volatile UINTN __i; __i = 1; while (__i); }
#endif

STATIC EFI_DXE_SPDI3C_PROTOCOL mSpdI3cProtocol = {
  SPDI3C_PROTOCOL_REVISION,
  SpdI3cSet,
  SpdI3cGet
};


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM SPD DXE
 *
 * This function registers the function to publish SPD - 8 Bit IO Expander control ppi.
 *
 * @param[in]     FileHandle     Pointer to the firmware file system header
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
SpdI3cDxeEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_HANDLE          Handle;

  Handle = ImageHandle;

  DEBUG ((EFI_D_ERROR, "SpdI3cDxeEntryPoint\n"));

  // Publish SpdI3c control Protocol
  // Publish Error Log service Protocol For IP driver
  Status = gBS->InstallProtocolInterface (
                &Handle,
                &gSpdI3cProtocolGuid,
                EFI_NATIVE_INTERFACE,
                &mSpdI3cProtocol
                );

  return Status;
}

EFI_STATUS
EFIAPI
SpdI3cSet (
  IN CONST EFI_DXE_SPDI3C_PROTOCOL  *This,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Command,
  IN       UINT8             Data
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_DXE_I3C_MASTER_PROTOCOL  *I3cMaster;
  EFI_I3C_REQUEST_PACKET  RequestPacket;
  EFI_I3C_OPERATION       *Operation;
  UINT8                   Buffer[3];

  UINTN HandleCount           = 0;
  EFI_HANDLE *HandleBuffer          = NULL;
  UINTN Index = 0;

  DEBUG ((EFI_D_ERROR, "DXE SpdI3cSet BusSelect = 0x%x, SlaveAddress=0x%x\n, Command = 0x%x, Data=0x%x \n",
                        BusSelect, SlaveAddress, Command, Data));

  //
  // Get all I3C protocols
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gAmdFchSNI3cProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );

  DEBUG ((EFI_D_ERROR, "I3C LocateHandleBuffer Status = %r, HandleCount = %d\n", Status, HandleCount));

  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Find the matching I3C controller Number
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gAmdFchSNI3cProtocolGuid,
                    (VOID**)&I3cMaster
                    );
    DEBUG((EFI_D_ERROR, "I3C HandleProtocol[%d] Status = %r, HandleCount = %d\n", Index, Status, HandleCount));

    if ( EFI_ERROR (Status) ) {
      continue;
    }

    // BusSelect is the I3C controller number
    DEBUG((EFI_D_ERROR, "ControllerNum = 0x%x\n", I3cMaster->ControllerNum));
    if (I3cMaster->ControllerNum == BusSelect)
      break;

  }

  //Prepare write data buffer
  Buffer[0] = Command;
  Buffer[1] = 0;
  Buffer[2] = Data;

  //Write Byte Operation
  Operation   = RequestPacket.Operation;
  RequestPacket.OperationCount = 1;
  Operation[0].Flags           = 0;
  Operation[0].LengthInBytes   = 3; //2;
  Operation[0].Buffer          = &Buffer[0];

  Status = I3cMaster->StartRequest (I3cMaster, SlaveAddress, &RequestPacket);

  return Status;
}

EFI_STATUS
EFIAPI
SpdI3cGet (
  IN CONST EFI_DXE_SPDI3C_PROTOCOL  *This,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Command,
  OUT      UINT8             *Data
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_DXE_I3C_MASTER_PROTOCOL  *I3cMaster;
  UINT8                   PacketBuffer[sizeof (EFI_I3C_REQUEST_PACKET) + sizeof (EFI_I3C_OPERATION)];
  EFI_I3C_REQUEST_PACKET  *RequestPacket;
  EFI_I3C_OPERATION       *Operation;

  UINTN HandleCount           = 0;
  EFI_HANDLE *HandleBuffer          = NULL;
  UINTN Index = 0;

  DEBUG ((EFI_D_ERROR, "DXE SpdI3cGet BusSelect = 0x%x, SlaveAddress=0x%x\n, Command = 0x%x, Data=0x%x \n",
                        BusSelect, SlaveAddress, Command, Data));

  //
  // Get all I3C protocols
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gAmdFchSNI3cProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );

  DEBUG ((EFI_D_ERROR, "I3C LocateHandleBuffer Status = %r, HandleCount = %d\n", Status, HandleCount));

  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Find the matching I3C controller Number
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gAmdFchSNI3cProtocolGuid,
                    (VOID**)&I3cMaster
                    );
    DEBUG((EFI_D_ERROR, "I3C HandleProtocol[%d] Status = %r, HandleCount = %d\n", Index, Status, HandleCount));

    if ( EFI_ERROR (Status) ) {
      continue;
    }

    // BusSelect is the I3C controller number
    DEBUG((EFI_D_ERROR, "ControllerNum = 0x%x\n", I3cMaster->ControllerNum));
    if (I3cMaster->ControllerNum == BusSelect)
      break;

  }

  RequestPacket = (EFI_I3C_REQUEST_PACKET*)PacketBuffer;
  Operation   = RequestPacket->Operation;
  RequestPacket->OperationCount = 2;
  Operation[0].Flags            = 0;
  Operation[0].LengthInBytes    = 2; //sizeof (UINT8);
  Operation[0].Buffer           = &Command;
  Operation[1].Flags            = I3C_FLAG_READ;
  Operation[1].LengthInBytes    = 200 ; //sizeof (UINT8);
  Operation[1].Buffer           = (UINT8*)Data;

  Status = I3cMaster->StartRequest (I3cMaster, SlaveAddress, RequestPacket);

  return Status;
}

