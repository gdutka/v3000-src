/** @file
 Implement the Chipset Servcie IHISI OemExtra Data subfunction for this driver.

***************************************************************************
* Copyright (c) 2014 - 2019, Insyde Software Corp. All Rights Reserved.
*
* You may not reproduce, distribute, publish, display, perform, modify, adapt,
* transmit, broadcast, present, recite, release, license or otherwise exploit
* any part of this publication in any form, by any means, without the prior
* written permission of Insyde Software Corporation.
*
******************************************************************************
*/
#include <Uefi.h>
#include <H2OIhisi.h>
#include "IndustryStandard/Oa3_0.h"
#include <CsSvcIhisiOemExtraDataCommunication.h>
#include <Library/DebugLib.h>
//#include <Library/SmmOemSvcChipsetLib.h>
#include <Library/SmmOemSvcKernelLib.h>
#include <Library/FlashRegionLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/BaseOemSvcKernelLib.h>
#include <Protocol/SmmFwBlockService.h>

extern H2O_IHISI_PROTOCOL                  *mH2OIhisi;
extern EFI_SMM_FW_BLOCK_SERVICE_PROTOCOL   *mSmmFwBlockService;
STATIC
BOOLEAN
MsdmExist (
  VOID
  )
{
  EFI_STATUS                       Status;
  UINTN                            DataSize;
  BOOLEAN                          MsdmExist;
  UINTN                            RomBaseAddress;
  EFI_ACPI_MSDM_DATA_STRUCTURE     *MsdmData;

  MsdmData = NULL;
  MsdmExist = FALSE;

  DataSize = sizeof (EFI_ACPI_MSDM_DATA_STRUCTURE);
  RomBaseAddress = (UINTN) FdmGetNAtAddr (&gH2OFlashMapRegionMsdmGuid, 1);

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    DataSize,
                    (VOID **)&MsdmData
                    );
  if (EFI_ERROR (Status)) {
    goto Done;
  }
  Status = mSmmFwBlockService->Read (
                                 mSmmFwBlockService,
                                 RomBaseAddress,
                                 0,
                                 &DataSize,
                                 (UINT8*) MsdmData
                                 );
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  if ((MsdmData->MsdmVersion == 0xFFFFFFFF) &&
      (MsdmData->MdsmDataType == 0xFFFFFFFF) &&
      (MsdmData->MsdmDataLength == 0xFFFFFFFF)) {
    goto Done;
  }

  MsdmExist = TRUE;

Done:
  if (MsdmData != NULL) {
    gSmst->SmmFreePool (MsdmData);
  }
  return MsdmExist;
}

/**
  AH=41h, OEM Extra Data Communication type 50h to read/write OA3.0.

  @param[in]  ApCommDataBuffer   Pointer to AP communication data buffer.
  @param[out] BiosCommDataBuffer Pointer to BIOS communication data buffer.

  @retval EFI_SUCCESS  Read or write OA3.0 successful.
**/
EFI_STATUS
Oa30ReadWriteFunction (
  IN  AP_COMMUNICATION_DATA_TABLE          *ApCommDataBuffer,
  OUT BIOS_COMMUNICATION_DATA_TABLE        *BiosCommDataBuffer
  )
{
  UINT32                      MsdmDataSize;

  MsdmDataSize = sizeof (EFI_ACPI_MSDM_DATA_STRUCTURE);
  switch (ApCommDataBuffer->DataSize) {

  case OemExtraReportReadSize:
    BiosCommDataBuffer->Signature        = BIOS_COMMUNICATION_SIGNATURE;
    BiosCommDataBuffer->StructureSize    = sizeof (BIOS_COMMUNICATION_DATA_TABLE);
    BiosCommDataBuffer->BlockSize        = OemExtraBlockSize4k;
    BiosCommDataBuffer->DataSize         = OemExtraReportReadSize;
    BiosCommDataBuffer->PhysicalDataSize = MsdmDataSize;
    break;

  case OemExtraSkipSizeCheck:
    BiosCommDataBuffer->Signature        = BIOS_COMMUNICATION_SIGNATURE;
    BiosCommDataBuffer->StructureSize    = sizeof (BIOS_COMMUNICATION_DATA_TABLE);
    BiosCommDataBuffer->BlockSize        = OemExtraBlockSize4k;
    BiosCommDataBuffer->DataSize         = OemExtraSkipSizeCheck;   //Don't care
    BiosCommDataBuffer->PhysicalDataSize = 0x00;                   //Don't care
    break;

  case OemExtraReportWriteSize:
    BiosCommDataBuffer->Signature        = BIOS_COMMUNICATION_SIGNATURE;
    BiosCommDataBuffer->StructureSize    = sizeof (BIOS_COMMUNICATION_DATA_TABLE);
    BiosCommDataBuffer->BlockSize        = OemExtraBlockSize4k;
    BiosCommDataBuffer->DataSize         = OemExtraReportWriteSize;
    BiosCommDataBuffer->PhysicalDataSize = MsdmDataSize;   //bin size
    break;

  default:
    break;
  }

  if (!MsdmExist ()) {
    BiosCommDataBuffer->ErrorReturn    = (BiosCommDataBuffer->ErrorReturn | ERROR_RETURE_OA30_NOT_EXIST);
  }

  return EFI_SUCCESS;
}

/**
  AH=41h, OEM Extra Data Communication type 51h to erase OA3.0 (reset to default).

  @param[in]  ApCommDataBuffer   Pointer to AP communication data buffer.
  @param[out] BiosCommDataBuffer Pointer to BIOS communication data buffer.

  @retval EFI_SUCCESS  Erase OA3.0 successful.
**/
EFI_STATUS
Oa30EraseFunction (
  IN  AP_COMMUNICATION_DATA_TABLE          *ApCommDataBuffer,
  OUT BIOS_COMMUNICATION_DATA_TABLE        *BiosCommDataBuffer
  )
{
  EFI_STATUS                   Status;
  EFI_STATUS                   OemSvcStatus;
  UINT8                        LoopCount;
  UINTN                        RomBaseAddress;
  UINTN                        EraseSize;

  RomBaseAddress = (UINTN) FixedPcdGet32 (PcdFlashNvStorageMsdmDataBase);
  LoopCount = 0;
  EraseSize = 0x1000;
  Status = EFI_SUCCESS;

  if (!MsdmExist ()) {
    BiosCommDataBuffer->ErrorReturn    = (BiosCommDataBuffer->ErrorReturn | ERROR_RETURE_OA30_NOT_EXIST);
    goto Done;
  }

  if (PcdGetBool (PcdH2OEcSharedFlashSupported)) {
    DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices Call: OemSvcEcIdle \n"));
    OemSvcStatus = OemSvcEcIdle (TRUE);
    DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices OemSvcEcIdle Status: %r\n", OemSvcStatus));
  }
  Status = EFI_UNSUPPORTED;
  while ((EFI_ERROR (Status)) && (LoopCount < 100)) {
    Status = mSmmFwBlockService->EraseBlocks (
                                   mSmmFwBlockService,
                                   RomBaseAddress,
                                   &EraseSize
                                   );
    LoopCount++;
  }

  if (PcdGetBool (PcdH2OEcSharedFlashSupported)) {
    DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices Call: OemSvcEcIdle \n"));
    OemSvcStatus = OemSvcEcIdle (FALSE);
    DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices OemSvcEcIdle Status: %r\n", OemSvcStatus));
  }

Done:
  BiosCommDataBuffer->DataSize = ApCommDataBuffer->DataSize;
  BiosCommDataBuffer->PhysicalDataSize = ApCommDataBuffer->PhysicalDataSize;

  return Status;
}

/**
  Function to write OA3.0 data and do action which request from IHISI function 42h.

  @retval EFI_SUCCESS   Successfully returns.
**/
EFI_STATUS
Oa30DataWrite (
  VOID
  )
{
  EFI_STATUS                       Status;
  EFI_STATUS                       OemSvcStatus;
  UINT32                           WriteSize;
  UINT8                            LoopCount;
  UINT8                            ShutdownMode;
  UINT32                           MsdmDataSize;
  UINTN                            RomBaseAddress;
  UINT8                           *WriteDataBuffer;

  WriteDataBuffer = (UINT8 *) (UINTN) mH2OIhisi->ReadCpuReg32(EFI_SMM_SAVE_STATE_REGISTER_RSI);
  WriteSize = (UINT32) mH2OIhisi->ReadCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RDI);
  RomBaseAddress = (UINTN) FixedPcdGet32 (PcdFlashNvStorageMsdmDataBase);
  MsdmDataSize = sizeof (EFI_ACPI_MSDM_DATA_STRUCTURE);

  if (WriteSize == MsdmDataSize) {
    UINT8                          *ReturnDataBuffer = NULL;
    UINTN                          Index2 = 0;
    UINTN                          EraseSize = 0x1000;
    UINT8                          *TEMP;

    Status = gSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      0x1000,
                      (VOID **)&ReturnDataBuffer
                      );
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }

    Status = mSmmFwBlockService->Read (
                                   mSmmFwBlockService,
                                   RomBaseAddress,
                                   0,
                                   &EraseSize,
                                   ReturnDataBuffer
                                   );
    //
    // Only modify the first 0x31 bytes
    //
    TEMP = ReturnDataBuffer;
    for (Index2 = 0; Index2 < MsdmDataSize; Index2++) {
      TEMP[Index2] = WriteDataBuffer[Index2];
    }

    LoopCount = 0;
    Status = EFI_UNSUPPORTED;
    *(mSmmFwBlockService->FlashMode) = SMM_FW_FLASH_MODE;
    if (PcdGetBool (PcdH2OEcSharedFlashSupported)) {
      DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices Call: OemSvcEcIdle \n"));
      OemSvcStatus = OemSvcEcIdle (TRUE);
      DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices OemSvcEcIdle Status: %r\n", OemSvcStatus));
    }

    while ((EFI_ERROR (Status)) && (LoopCount < 100)) {
      Status = mSmmFwBlockService->EraseBlocks (
                                     mSmmFwBlockService,
                                     RomBaseAddress,
                                     &EraseSize
                                     );
      Status = mSmmFwBlockService->Write (
                                     mSmmFwBlockService,
                                     RomBaseAddress,
                                     &EraseSize,
                                     ReturnDataBuffer
                                     );
      LoopCount++;
    }
    gSmst->SmmFreePool (ReturnDataBuffer);
    if (PcdGetBool (PcdH2OEcSharedFlashSupported)) {
      DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices Call: OemSvcEcIdle \n"));
      OemSvcStatus = OemSvcEcIdle (FALSE);
      DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices OemSvcEcIdle Status: %r\n", OemSvcStatus));
    }
  } else {
    return EFI_INVALID_PARAMETER;
  }

  ShutdownMode = (UINT8) mH2OIhisi->ReadCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RCX);
  switch (ShutdownMode) {

  case DoNothing:
    Status = EFI_SUCCESS;
    break;

  case WindowsReboot:
    Status = EFI_SUCCESS;
    break;

  case WindowsShutdown:
    Status = EFI_SUCCESS;
    break;

  // case DosReboot:
  //   //
  //   // Note: Reboot by Oem hook
  //   //
  //   Status = IhisiFbtsReboot ();
  //   break;

  // case DosShutdown:
  //   Status = IhisiFbtsShutDown ();
  //   break;

  default:
    Status = EFI_UNSUPPORTED;
    break;
  }

  return EFI_SUCCESS;
}

/**
  AH=41h(OemExtraDataCommunication),
  This function offers an interface to do IHISI Sub function AH=41h.

 @param[in]         ApCommDataBuffer    Pointer to AP communication data buffer.
 @param[in, out]    BiosCommDataBuffer  On entry, pointer to BIOS communication data buffer.
                                        On exit, points to updated BIOS communication data buffer.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter or hook code.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
EFIAPI
IhisiOemExtCommunication (
  IN  AP_COMMUNICATION_DATA_TABLE          *ApCommDataBuffer,
  IN OUT BIOS_COMMUNICATION_DATA_TABLE     *BiosCommDataBuffer
  )
{
  EFI_STATUS        Status;

  switch (ApCommDataBuffer->DataType) {

    case Oa30ReadWrite:
      // Status = OemSvcIhisiS41T50HookOa30ReadWriteFunction (ApCommDataBuffer, BiosCommDataBuffer);
      // DEBUG ((EFI_D_ERROR | EFI_D_INFO, "SmmOemSvcChipsetLib OemSvcIhisiS41T50HookOa30ReadWriteFunction, Status : %r\n", Status));
      Status = Oa30ReadWriteFunction (ApCommDataBuffer, BiosCommDataBuffer);
      break;

    case Oa30Erase:
      Status = Oa30EraseFunction (ApCommDataBuffer, BiosCommDataBuffer);
      break;

    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  } else {
    CopyMem (ApCommDataBuffer, BiosCommDataBuffer, sizeof (BIOS_COMMUNICATION_DATA_TABLE));
    return EFI_SUCCESS;
  }
}

/**
  AH=42h(OemExtraDataWrite)
  This function offers an interface to do IHISI Sub function AH=42h,

 @param[in]         FunctionType      Function type.
 @param[in, out]    WriteDataBuffer   Pointer to input file data buffer.
 @param[in, out]    WriteSize         Write size.
 @param[in, out]    RomBaseAddress    Target offset to write.
 @param[in]         ShutdownMode      Shutdown mode.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter or hook code.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
EFIAPI
IhisiOemExtDataWrite (
  IN      UINT8                   FunctionType,
  IN OUT  UINT8                  *WriteDataBuffer,
  IN OUT  UINTN                  *WriteSize,
  IN OUT  UINTN                  *RomBaseAddress,
  IN      UINT8                   ShutdownMode
  )
{
  EFI_STATUS                       Status;
  Status = EFI_UNSUPPORTED;

  switch (FunctionType) {
    case Oa30ReadWrite:
      Status = Oa30DataWrite ();
      break;

    default:
      Status = EFI_UNSUPPORTED;
      break;
    }

  return Status;
}

/**
 AH=47h(OemExtraDataRead),
 This function offers an interface to do IHISI Sub function AH=47h,

 @param[in]         FunctionType        Function type
 @param[in, out]    DataBuffer          Pointer to return data buffer.
 @param[in, out]    Size                Read size.
 @param[in, out]    RomBaseAddress      Read address.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter or hook code.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
EFIAPI
IhisiOemExtDataRead (
  IN UINT8                        FunctionType,
  IN OUT  UINT8                  *DataBuffer,
  IN OUT  UINTN                  *Size,
  IN OUT  UINTN                  *RomBaseAddress
  )
{
  return EFI_UNSUPPORTED;
}

