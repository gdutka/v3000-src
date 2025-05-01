/** @file
  A SMM driver for SMM platform initialization.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "DxeSmmIoLibSmm.h"

UINT64 
EFIAPI
IoRead(
  UINTN Port, 
  UINTN Size
  )
{
  switch (Size) { 
    case SMM_IO_UINT8:
           return IoRead8(Port);
           
    case SMM_IO_UINT16:
           return IoRead16(Port);

    case SMM_IO_UINT32:
           return IoRead32(Port);

    case SMM_IO_UINT64:
           return IoRead64(Port);

    default:
      ASSERT (FALSE);
      return 0;
  }
}


UINT64 
EFIAPI  
IoWrite(
  UINTN Port, 
  UINTN Size, 
  UINTN Value
  ) 
{
  switch (Size) { 
    case SMM_IO_UINT8:
           return IoWrite8(Port, (UINT8)Value);
           
    case SMM_IO_UINT16:
           return IoWrite16(Port, (UINT16)Value);

    case SMM_IO_UINT32:
           return IoWrite32(Port, (UINT32)Value);

    case SMM_IO_UINT64:
           return IoWrite64(Port, Value);

    default:
      ASSERT (FALSE);
      return 0;
  }
}


VOID
EFIAPI
IoReadFifo (
  IN UINTN                           Port, 
  IN UINTN                           Size,
  IN UINTN                           Count,
  OUT VOID                           *Buffer
  )
{

  switch (Size) { 
    case SMM_IO_UINT8:
           IoReadFifo8(Port, Count, Buffer);
           break;
           
    case SMM_IO_UINT16:
           IoReadFifo16(Port, Count, Buffer);
           break;
           
    case SMM_IO_UINT32:
           IoReadFifo32(Port, Count, Buffer);
           break;

    default:
      ASSERT (FALSE);
      return;
  }

}



VOID
EFIAPI 
IoWriteFifo (
  IN UINTN                           Port, 
  IN UINTN                           Size,
  IN UINTN                           Count,  
  IN VOID                            *Buffer
  )
{
  switch (Size) { 
    case SMM_IO_UINT8:
           IoWriteFifo8(Port, Count, Buffer);
           break;
           
    case SMM_IO_UINT16:
           IoWriteFifo16(Port, Count, Buffer);
           break;
           
    case SMM_IO_UINT32:
           IoWriteFifo32(Port, Count, Buffer);
           break;
           
    default:
      ASSERT (FALSE);
      return;
  }
  return;
}


UINT64
EFIAPI 
MmioRead (
  IN UINTN                           Address, 
  IN UINTN                           Size
  )
{
  switch (Size) { 
    case SMM_IO_UINT8:
           return MmioRead8(Address);
           
    case SMM_IO_UINT16:
           return MmioRead16(Address);
           
    case SMM_IO_UINT32:
           return MmioRead32(Address);

    case SMM_IO_UINT64:
           return MmioRead64(Address);
           
    default:
      ASSERT (FALSE);
      return 0;
  }
}


UINT64
EFIAPI 
MmioWrite (
  IN UINTN                           Address, 
  IN UINTN                           Size,
  IN UINT64                           Value
  )
{
  switch (Size) { 
    case SMM_IO_UINT8:
           return MmioWrite8(Address, (UINT8)Value);
           
    case SMM_IO_UINT16:
           return MmioWrite16(Address, (UINT16)Value);
           
    case SMM_IO_UINT32:
           return MmioWrite32(Address, (UINT32)Value);

    case SMM_IO_UINT64:
           return MmioWrite64(Address, Value);
           
    default:
      ASSERT (FALSE);
      return 0;
  }
}


EFI_STATUS
DxeSmmIoLibSmmEntryPoint (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{

   H2O_DXE_SMM_IO_LIB_PROTOCOL *DxeSmmIoLibProtocol; 
   EFI_HANDLE                  DxeSmmIoLibProtocolHandle;
   EFI_STATUS                  Status;


   DxeSmmIoLibProtocol = AllocateZeroPool(sizeof(H2O_DXE_SMM_IO_LIB_PROTOCOL));
   if (DxeSmmIoLibProtocol == NULL) {
     return EFI_OUT_OF_RESOURCES;
   }

   DxeSmmIoLibProtocol->IoRead = IoRead;
   DxeSmmIoLibProtocol->IoWrite = IoWrite;
   DxeSmmIoLibProtocol->IoReadFifo = IoReadFifo;
   DxeSmmIoLibProtocol->IoWriteFifo = IoWriteFifo;
   DxeSmmIoLibProtocol->MmioRead = MmioRead;
   DxeSmmIoLibProtocol->MmioWrite = MmioWrite;
   DxeSmmIoLibProtocolHandle   = NULL;
   if (gSmst != NULL) {
     Status = gSmst->SmmInstallProtocolInterface (
                        &DxeSmmIoLibProtocolHandle,
                        &gH2ODxeSmmIoLibProtocolGuid,
                        EFI_NATIVE_INTERFACE,
                        DxeSmmIoLibProtocol
                        );
   } else {
      Status = gBS->InstallProtocolInterface (
                      &DxeSmmIoLibProtocolHandle,
                      &gH2ODxeSmmIoLibProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      DxeSmmIoLibProtocol
                      );

   }
   return EFI_SUCCESS;
}
