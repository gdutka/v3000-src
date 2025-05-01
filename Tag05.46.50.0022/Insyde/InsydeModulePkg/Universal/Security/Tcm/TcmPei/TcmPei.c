/** @file
  Initialize TCM device and measure FVs before handing off control to DXE.

;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
**/

#include <PiPei.h>

#include <IndustryStandard/Tpm12.h>
#include <IndustryStandard/UefiTcgPlatform.h>
#include <Ppi/FirmwareVolumeInfo.h>
#include <Ppi/FirmwareVolumeInfo2.h>
#include <Ppi/LockPhysicalPresence.h>
#include <Ppi/TpmInitialized.h>
#include <Ppi/FirmwareVolume.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/IoMmu.h>
#include <Guid/MeasuredFvHob.h>
#include <Guid/TcgEventHob.h>
#include <Guid/TpmInstance.h>
#include <Guid/TcmInstance.h>
#include <Guid/MemoryOverwriteControl.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryOverwriteLib.h>
#include <Library/H2OFvHashLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/TcmDeviceLib.h>
#include <Library/TcmCommandLib.h>

#include <Tcm.h>
#include <TpmPolicy.h>

BOOLEAN                 mImageInMemory  = FALSE;

EFI_PEI_PPI_DESCRIPTOR  mTpmInitializedPpiList = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gPeiTpmInitializedPpiGuid,
  NULL
};

EFI_PEI_PPI_DESCRIPTOR  mTpmInitializationDonePpiList = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gPeiTpmInitializationDonePpiGuid,
  NULL
};

//
// Number of firmware blobs to grow by each time we run out of room
//
#define FIRMWARE_BLOB_GROWTH_STEP 4

EFI_PLATFORM_FIRMWARE_BLOB *mMeasuredBaseFvInfo;
UINT32 mMeasuredMaxBaseFvIndex = 0;
UINT32 mMeasuredBaseFvIndex = 0;

EFI_PLATFORM_FIRMWARE_BLOB *mMeasuredChildFvInfo;
UINT32 mMeasuredMaxChildFvIndex = 0;
UINT32 mMeasuredChildFvIndex = 0;

#define DMA_PROTECTION_DISABLED_EVENT_STRING  "DMA Protection Disabled"

/**
  Measure DMA protection policy, and extend the measurement result into a specific PCR.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_DEVICE_ERROR      The operation was unsuccessful.

**/
EFI_STATUS
MeasureDmaProtectionDisabled (
  VOID
  );

/**
  Measure and record the Firmware Volum Information once FvInfoPPI install.

  @param[in] PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param[in] NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in] Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS          The FV Info is measured and recorded to TCM.
  @return Others               Fail to measure FV.

**/
EFI_STATUS
EFIAPI
FirmwareVolmeInfoPpiNotifyCallback (
  IN EFI_PEI_SERVICES              **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDescriptor,
  IN VOID                          *Ppi
  );

/**
  Record all measured Firmware Volum Information into a Guid Hob

  @param[in] PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param[in] NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in] Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS          The FV Info is measured and recorded to TCM.
  @return Others               Fail to measure FV.

**/
EFI_STATUS
EFIAPI
EndofPeiSignalNotifyCallBack (
  IN EFI_PEI_SERVICES              **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDescriptor,
  IN VOID                          *Ppi
  );

/**
  Clear memory below 1MB if MOR is set.

  @param[in] PeiServices        An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in] NotifyDescriptor   Address of the notification descriptor data structure.
  @param[in] Ppi                Address of the PPI that was installed.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval Others                Operation failed.

**/
STATIC
EFI_STATUS
EFIAPI
MemoryOverwriteCallback (
  IN EFI_PEI_SERVICES               **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR      *NotifyDescriptor,
  IN VOID                           *Ppi
  );

EFI_PEI_NOTIFY_DESCRIPTOR           mNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiPeiFirmwareVolumeInfoPpiGuid,
    FirmwareVolmeInfoPpiNotifyCallback
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiPeiFirmwareVolumeInfo2PpiGuid,
    FirmwareVolmeInfoPpiNotifyCallback
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiEndOfPeiSignalPpiGuid,
    EndofPeiSignalNotifyCallBack
  }
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR    mMorCallbackDescriptor = 
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiEndOfPeiSignalPpiGuid,
    MemoryOverwriteCallback
  };

/**
  Record all measured Firmware Volum Information into a Guid Hob
  Guid Hob payload layout is 

     UINT32 *************************** FIRMWARE_BLOB number
     EFI_PLATFORM_FIRMWARE_BLOB******** BLOB Array

  @param[in] PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param[in] NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in] Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS          The FV Info is measured and recorded to TCM.
  @return Others               Fail to measure FV.

**/
EFI_STATUS
EFIAPI
EndofPeiSignalNotifyCallBack (
  IN EFI_PEI_SERVICES              **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDescriptor,
  IN VOID                          *Ppi
  )
{  
  MEASURED_HOB_DATA *MeasuredHobData;
  EFI_STATUS        Status;
  EDKII_IOMMU_PPI   *IoMmu;

  if ((PcdGet32 (PcdPeiTpmPolicy) & DMA_PROTECTION_MEASUREMENT) != 0) {
    //
    // DMA protection measurement
    //
    Status = PeiServicesLocatePpi (
               &gEdkiiIoMmuPpiGuid,
               0,
               NULL,
               (VOID **)&IoMmu
               );
    if (EFI_ERROR (Status)) {
      //
      // DMA protection is disabled, extend an EV_EFI_ACTION event into PCR[7]
      //
      Status = MeasureDmaProtectionDisabled ();
      DEBUG ((EFI_D_INFO, "MeasureDmaProtectionDisabled - %r\n", Status));
    }
  }

  MeasuredHobData = NULL;

  //
  // Create a Guid hob to save all measured Fv 
  //
  MeasuredHobData = BuildGuidHob(
                      &gMeasuredFvHobGuid,
                      sizeof(UINTN) + sizeof(EFI_PLATFORM_FIRMWARE_BLOB) * (mMeasuredBaseFvIndex + mMeasuredChildFvIndex)
                      );

  if (MeasuredHobData != NULL) {
    //
    // Save measured FV info enty number
    //
    MeasuredHobData->Num = mMeasuredBaseFvIndex + mMeasuredChildFvIndex;

    //
    // Save measured base Fv info
    //
    CopyMem (MeasuredHobData->MeasuredFvBuf, mMeasuredBaseFvInfo, sizeof(EFI_PLATFORM_FIRMWARE_BLOB) * (mMeasuredBaseFvIndex));

    //
    // Save measured child Fv info
    //
    CopyMem (&MeasuredHobData->MeasuredFvBuf[mMeasuredBaseFvIndex] , mMeasuredChildFvInfo, sizeof(EFI_PLATFORM_FIRMWARE_BLOB) * (mMeasuredChildFvIndex));
  }

  return EFI_SUCCESS;
}

/**
  Single function calculates SM3 256 digest value for all raw data. It
  combines Sm3Init(), Sm3Update() and Sm3Final().

  @param[in]  Data          Raw data to be digested.
  @param[in]  DataLen       Size of the raw data.
  @param[out] Digest        Pointer to a buffer that stores the final digest.

  @retval     EFI_SUCCESS   Always successfully calculate the final digest.
**/
EFI_STATUS
EFIAPI
TcmCommHashAll (
  IN  CONST UINT8       *Data,
  IN        UINTN       DataLen,
  OUT       TCM_DIGEST  *Digest
  )
{
  VOID     *Sm3Ctx;
  UINTN    CtxSize;

  CtxSize = Sm3GetContextSize ();
  Sm3Ctx = AllocatePool (CtxSize);
  ASSERT (Sm3Ctx != NULL);

  Sm3Init (Sm3Ctx);
  Sm3Update (Sm3Ctx, Data, DataLen);
  Sm3Final (Sm3Ctx, (UINT8 *)Digest);

  FreePool (Sm3Ctx);

  return EFI_SUCCESS;
}

/**
  Do a hash operation on a data buffer, extend a specific TCM PCR with the hash result,
  and build a GUIDed HOB recording the event which will be passed to the DXE phase and
  added into the Event Log.

  @param[in]      PeiServices   Describes the list of possible PEI Services.
  @param[in]      HashData      Physical address of the start of the data buffer
                                to be hashed, extended, and logged.
  @param[in]      HashDataLen   The length, in bytes, of the buffer referenced by HashData.
  @param[in]      NewEventHdr   Pointer to a TCM_PCR_EVENT_HDR data structure.
  @param[in]      NewEventData  Pointer to the new event data.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  No enough memory to log the new event.
  @retval EFI_DEVICE_ERROR      The command was unsuccessful.

**/
EFI_STATUS
HashLogExtendEvent (
  IN      EFI_PEI_SERVICES          **PeiServices,
  IN      UINT8                     *HashData,
  IN      UINTN                     HashDataLen,
  IN      TCM_PCR_EVENT_HDR         *NewEventHdr,
  IN      UINT8                     *NewEventData
  )
{
  EFI_STATUS                        Status;
  VOID                              *HobData;
  VOID                              *HashBuffer;

  HobData = NULL;
  if (HashDataLen != 0) {
    HashBuffer = NULL;
    HashBuffer = H2OGetFvHash (H2O_SM3_256_FV_HASH, (UINTN)HashData, HashDataLen);
    if (HashBuffer == NULL) {
      Status = TcmCommHashAll (
                 HashData,
                 HashDataLen,
                 &NewEventHdr->Digest
                 );
      ASSERT_EFI_ERROR (Status);
      //
      // Record the result
      //
      H2OSetFvHash (H2O_SM3_256_FV_HASH, (UINTN)HashData, HashDataLen, (VOID *)&NewEventHdr->Digest.digest);
    } else {
      //
      // Digest found
      //
      CopyMem ((VOID *)&NewEventHdr->Digest.digest, (VOID *)HashBuffer, sizeof (NewEventHdr->Digest.digest));
    }
  }

  Status = TcmExtend (
             &NewEventHdr->Digest,
             NewEventHdr->PCRIndex,
             NULL
             );
  ASSERT_EFI_ERROR (Status);

  HobData = BuildGuidHob (
             &gTcgEventEntryHobGuid,
             sizeof (*NewEventHdr) + NewEventHdr->EventSize
             );
  if (HobData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (HobData, NewEventHdr, sizeof (*NewEventHdr));
  HobData = (VOID *) ((UINT8*)HobData + sizeof (*NewEventHdr));
  CopyMem (HobData, NewEventData, NewEventHdr->EventSize);
  return EFI_SUCCESS;
}

/**
  Measure DMA protection policy, and extend the measurement result into a specific PCR.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_DEVICE_ERROR      The operation was unsuccessful.

**/
EFI_STATUS
MeasureDmaProtectionDisabled (
  VOID
  )
{
  TCM_PCR_EVENT_HDR                 TcmEventHdr;

  TcmEventHdr.PCRIndex  = 7;
  TcmEventHdr.EventType = EV_EFI_ACTION;
  TcmEventHdr.EventSize = sizeof (DMA_PROTECTION_DISABLED_EVENT_STRING) - 1;

  return HashLogExtendEvent (
           (EFI_PEI_SERVICES **) GetPeiServicesTablePointer(),
           (UINT8 *)DMA_PROTECTION_DISABLED_EVENT_STRING,
           sizeof (DMA_PROTECTION_DISABLED_EVENT_STRING) - 1,
           &TcmEventHdr,
           (UINT8 *)DMA_PROTECTION_DISABLED_EVENT_STRING
           );
}

/**
  Measure CRTM version.

  @param[in]      PeiServices   Describes the list of possible PEI Services.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  No enough memory to log the new event.
  @retval EFI_DEVICE_ERROR      The command was unsuccessful.

**/
EFI_STATUS
EFIAPI
MeasureCRTMVersion (
  IN      EFI_PEI_SERVICES          **PeiServices
  )
{
  TCM_PCR_EVENT_HDR                 TcmEventHdr;

  if ((PcdGet32 (PcdPeiTpmPolicy) & SKIP_MEASURE_CRTM_VERSION) != 0) {
    return EFI_SUCCESS;
  }
  //
  // Here, only a static GUID is measured instead of real CRTM version.
  // OEMs should get real CRTM version string and measure it.
  //

  TcmEventHdr.PCRIndex  = 0;
  TcmEventHdr.EventType = EV_S_CRTM_VERSION;
  TcmEventHdr.EventSize = (UINT32) StrSize((CHAR16*)PcdGetPtr (PcdFirmwareVersionString));
  return HashLogExtendEvent (
           PeiServices,
           (UINT8*)PcdGetPtr (PcdFirmwareVersionString),
           TcmEventHdr.EventSize,
           &TcmEventHdr,
           (UINT8*)PcdGetPtr (PcdFirmwareVersionString)
           );
}

/**
  Measure FV image.
  Add it into the measured FV list after the FV is measured successfully.

  @param[in]  FvBase            Base address of FV image.
  @param[in]  FvLength          Length of FV image.

  @retval EFI_SUCCESS           Fv image is measured successfully
                                or it has been already measured.
  @retval EFI_OUT_OF_RESOURCES  No enough memory to log the new event.
  @retval EFI_DEVICE_ERROR      The command was unsuccessful.

**/
EFI_STATUS
EFIAPI
MeasureFvImage (
  IN EFI_PHYSICAL_ADDRESS           FvBase,
  IN UINT64                         FvLength
  )
{
  UINT32                            Index;
  EFI_STATUS                        Status;
  EFI_PLATFORM_FIRMWARE_BLOB        FvBlob;
  TCM_PCR_EVENT_HDR                 TcmEventHdr;
  
  if ((PcdGet32 (PcdPeiTpmPolicy) & SKIP_MEASURE_FV_IMAGE) != 0) {
    return EFI_SUCCESS;
  }
  //
  // Check whether FV is in the measured FV list.
  //
  for (Index = 0; Index < mMeasuredBaseFvIndex; Index ++) {
    if (mMeasuredBaseFvInfo[Index].BlobBase == FvBase) {
      return EFI_SUCCESS;
    }
  }

  //
  // Measure and record the FV to the TCM
  //
  FvBlob.BlobBase   = FvBase;
  FvBlob.BlobLength = FvLength;

  DEBUG ((DEBUG_INFO, "The FV which is measured by TcmPei starts at: 0x%x\n", FvBlob.BlobBase));
  DEBUG ((DEBUG_INFO, "The FV which is measured by TcmPei has the size: 0x%x\n", FvBlob.BlobLength));

  TcmEventHdr.PCRIndex = 0;
  TcmEventHdr.EventType = EV_EFI_PLATFORM_FIRMWARE_BLOB;
  TcmEventHdr.EventSize = sizeof (FvBlob);

  Status = HashLogExtendEvent (
             (EFI_PEI_SERVICES **) GetPeiServicesTablePointer(),
             (UINT8*) (UINTN) FvBlob.BlobBase,
             (UINTN) FvBlob.BlobLength,
             &TcmEventHdr,
             (UINT8*) &FvBlob
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Add new FV into the measured FV list.
  //
  if (mMeasuredBaseFvIndex >= mMeasuredMaxBaseFvIndex) {
    mMeasuredBaseFvInfo = ReallocatePool (
                            sizeof (EFI_PLATFORM_FIRMWARE_BLOB) * mMeasuredMaxBaseFvIndex,
                            sizeof (EFI_PLATFORM_FIRMWARE_BLOB) * (mMeasuredMaxBaseFvIndex + FIRMWARE_BLOB_GROWTH_STEP),
                            mMeasuredBaseFvInfo
                            );
    ASSERT (mMeasuredBaseFvInfo != NULL);
    if (mMeasuredBaseFvInfo == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    mMeasuredMaxBaseFvIndex = mMeasuredMaxBaseFvIndex + FIRMWARE_BLOB_GROWTH_STEP;
  }

  mMeasuredBaseFvInfo[mMeasuredBaseFvIndex].BlobBase   = FvBase;
  mMeasuredBaseFvInfo[mMeasuredBaseFvIndex].BlobLength = FvLength;
  mMeasuredBaseFvIndex++;

  return Status;
}

/**
  Measure main BIOS.

  @param[in]      PeiServices   Describes the list of possible PEI Services.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  No enough memory to log the new event.
  @retval EFI_DEVICE_ERROR      The command was unsuccessful.

**/
EFI_STATUS
EFIAPI
MeasureMainBios (
  IN      EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_FV_HANDLE                 VolumeHandle;
  EFI_FV_INFO                       VolumeInfo;
  EFI_PEI_FIRMWARE_VOLUME_PPI       *FvPpi;

  if ((PcdGet32 (PcdPeiTpmPolicy) & SKIP_MEASURE_MAIN_BIOS) != 0) {
    return EFI_SUCCESS;
  }

  //
  // Only measure BFV at the very beginning. Other parts of Static Core Root of
  // Trust for Measurement(S-CRTM) will be measured later on FvInfoNotify.
  // BFV is processed without installing FV Info Ppi. Other FVs either inside BFV or
  // reported by platform will be installed with Fv Info Ppi
  // This firmware volume measure policy can be modified/enhanced by special
  // platform for special CRTM TCM measuring.
  //
  Status = PeiServicesFfsFindNextVolume (0, &VolumeHandle);
  ASSERT_EFI_ERROR (Status);

  //
  // Measure and record the firmware volume that is dispatched by PeiCore
  //
  Status = PeiServicesFfsGetVolumeInfo (VolumeHandle, &VolumeInfo);
  ASSERT_EFI_ERROR (Status);
  //
  // Locate the corresponding FV_PPI according to founded FV's format guid
  //
  Status = PeiServicesLocatePpi (
             &VolumeInfo.FvFormat,
             0,
             NULL,
             (VOID**)&FvPpi
             );
  ASSERT_EFI_ERROR (Status);

  Status = MeasureFvImage ((EFI_PHYSICAL_ADDRESS) (UINTN) VolumeInfo.FvStart, VolumeInfo.FvSize);

  return Status;
}

/**
  Measure and record the Firmware Volum Information once FvInfoPPI install.

  @param[in] PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param[in] NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in] Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS          The FV Info is measured and recorded to TCM.
  @return Others               Fail to measure FV.

**/
EFI_STATUS
EFIAPI
FirmwareVolmeInfoPpiNotifyCallback (
  IN EFI_PEI_SERVICES               **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR      *NotifyDescriptor,
  IN VOID                           *Ppi
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_FIRMWARE_VOLUME_INFO2_PPI FvInfo2Ppi;
  EFI_PEI_FIRMWARE_VOLUME_PPI       *FvPpi;
  UINTN                             Index;

  if (CompareGuid (NotifyDescriptor->Guid, &gEfiPeiFirmwareVolumeInfo2PpiGuid)) {
    //
    // It is FvInfo2PPI.
    //
    CopyMem (&FvInfo2Ppi, Ppi, sizeof (EFI_PEI_FIRMWARE_VOLUME_INFO2_PPI));
  } else {
    //
    // It is FvInfoPPI.
    //
    CopyMem (&FvInfo2Ppi, Ppi, sizeof (EFI_PEI_FIRMWARE_VOLUME_INFO_PPI));
    FvInfo2Ppi.AuthenticationStatus = 0;
  }

  //
  // The PEI Core can not dispatch or load files from memory mapped FVs that do not support FvPpi.
  //
  Status = PeiServicesLocatePpi (
             &FvInfo2Ppi.FvFormat,
             0,
             NULL,
             (VOID**)&FvPpi
             );
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  //
  // This is an FV from an FFS file, and the parent FV must have already been measured,
  // No need to measure twice, so just record the FV and return
  //
  if (FvInfo2Ppi.ParentFvName != NULL || FvInfo2Ppi.ParentFileName != NULL ) {

    if (mMeasuredChildFvIndex >= mMeasuredMaxChildFvIndex) {
      mMeasuredChildFvInfo = ReallocatePool (
                               sizeof (EFI_PLATFORM_FIRMWARE_BLOB) * mMeasuredMaxChildFvIndex,
                               sizeof (EFI_PLATFORM_FIRMWARE_BLOB) * (mMeasuredMaxChildFvIndex + FIRMWARE_BLOB_GROWTH_STEP),
                               mMeasuredChildFvInfo
                               );
      ASSERT (mMeasuredChildFvInfo != NULL);
      if (mMeasuredChildFvInfo == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      mMeasuredMaxChildFvIndex = mMeasuredMaxChildFvIndex + FIRMWARE_BLOB_GROWTH_STEP;
    }
    //
    // Check whether FV is in the measured child FV list.
    //
    for (Index = 0; Index < mMeasuredChildFvIndex; Index++) {
      if (mMeasuredChildFvInfo[Index].BlobBase == (EFI_PHYSICAL_ADDRESS) (UINTN) FvInfo2Ppi.FvInfo) {
        return EFI_SUCCESS;
      }
    }
    mMeasuredChildFvInfo[mMeasuredChildFvIndex].BlobBase   = (EFI_PHYSICAL_ADDRESS) (UINTN) FvInfo2Ppi.FvInfo;
    mMeasuredChildFvInfo[mMeasuredChildFvIndex].BlobLength = FvInfo2Ppi.FvInfoSize;
    mMeasuredChildFvIndex++;
    return EFI_SUCCESS;
  }

  return MeasureFvImage ((EFI_PHYSICAL_ADDRESS) (UINTN) FvInfo2Ppi.FvInfo, FvInfo2Ppi.FvInfoSize);
}

/**
  Check if TCM chip is activeated or not.

  @param[in]      PeiServices   Describes the list of possible PEI Services.

  @retval TRUE    TCM is activated.
  @retval FALSE   TCM is deactivated.

**/
BOOLEAN
IsTcmUsable (
  VOID
  )
{
  EFI_STATUS           Status;
  TCM_PERMANENT_FLAGS  TcmPermanentFlags;

  Status = TcmGetCapabilityFlagPermanent (&TcmPermanentFlags);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  return (BOOLEAN)(!TcmPermanentFlags.deactivated);
}

/**
  Clear memory below 1MB if MOR is set.

  @param[in] PeiServices        An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in] NotifyDescriptor   Address of the notification descriptor data structure.
  @param[in] Ppi                Address of the PPI that was installed.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval Others                Operation failed.

**/
STATIC
EFI_STATUS
EFIAPI
MemoryOverwriteCallback (
  IN EFI_PEI_SERVICES               **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR      *NotifyDescriptor,
  IN VOID                           *Ppi
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *Variable;
  UINTN                             VariableSize ;
  UINT8                             MorVariable;
  BOOLEAN                           TcmProperty;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **)&Variable
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //  Check MemoryOverwriteRequestControl variable
  //
  VariableSize = sizeof (MorVariable);
  Status = Variable->GetVariable ( 
                       Variable, 
                       MEMORY_OVERWRITE_REQUEST_VARIABLE_NAME,
                       &gEfiMemoryOverwriteControlDataGuid,
                       NULL,
                       &VariableSize,
                       &MorVariable
                       );
  if (!EFI_ERROR (Status) && (MOR_CLEAR_MEMORY_VALUE (MorVariable) == 0x0)) {
    //
    // No need to clear memory.
    //
    return EFI_SUCCESS;
  }

  //
  // Get property is moved from driver's entry to here,
  // since some TCM may fail in the command execution in MA.
  //
  TcmProperty = FALSE;
  if ((PcdGet32 (PcdPeiTpmPolicy) & SKIP_PEI_TPM_PROPERTY_CHECK) == 0) {    
    Status = TcmGetCapabilityPropertyOwner (&TcmProperty);
  } else {
    TcmProperty = TRUE;
  }

  if (EFI_ERROR (Status) || TcmProperty) {
    //
    // To clear the memory region below 1M here before CSM use it.
    // The other memory regions will be cleared in DXE phase.
    //
    Status = MemoryOverwriteBelow1M ();
  }
  return Status;
}

/**
  Do measurement after memory is ready.

  @param[in]      PeiServices   Describes the list of possible PEI Services.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  No enough memory to log the new event.
  @retval EFI_DEVICE_ERROR      The command was unsuccessful.

**/
EFI_STATUS
EFIAPI
PeimEntryMP (
  IN      EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS                        Status;

  Status = TisPcRequestUseTcm ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsTcmUsable ()) {
    Status = MeasureCRTMVersion (PeiServices);
    ASSERT_EFI_ERROR (Status);

    Status = MeasureMainBios (PeiServices);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    //
    // Post callbacks:
    // 1). for the FvInfoPpi services to measure and record
    // the additional Fvs to TCM
    //
    Status = PeiServicesNotifyPpi (&mNotifyList[0]);
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  Entry point of this module.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  Describes the list of possible PEI Services.

  @return Status.

**/
EFI_STATUS
EFIAPI
PeimEntryMA (
  IN       EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES         **PeiServices
  )
{
  EFI_STATUS                        Status;
  EFI_STATUS                        Status2;
  EFI_BOOT_MODE                     BootMode;

  if (!CompareGuid (PcdGetPtr (PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceTcmGuid)) {
    DEBUG ((EFI_D_ERROR, "No TCM instance required!\n"));
    return EFI_UNSUPPORTED;
  }

  if (PcdGetBool (PcdTpmHide)) {
    DEBUG((EFI_D_INFO, "TCM is hidden\n"));
    return EFI_UNSUPPORTED;
  }

  //
  // Initialize TCM device
  //
  Status = TisPcRequestUseTcm ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TCM not detected!\n"));
    return Status;
  }

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // In S3 path, skip shadow logic. no measurement is required
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    Status = (**PeiServices).RegisterForShadow(FileHandle);
    if (Status == EFI_ALREADY_STARTED) {
      mImageInMemory = TRUE;
    } else if (Status == EFI_NOT_FOUND) {
      ASSERT_EFI_ERROR (Status);
    }
  }

  if (!mImageInMemory) {
    if ((PcdGet32 (PcdPeiTpmPolicy) & SKIP_TPM_STARTUP) == 0) {
      if (BootMode == BOOT_ON_S3_RESUME) {
        Status = TcmStartup (TCM_ST_STATE);
      } else {
        Status = TcmStartup (TCM_ST_CLEAR);
      }
      if (EFI_ERROR (Status) ) {
        goto Done;
      }
    }
    //
    // SelfTest is optional on S3 path, skip it to save S3 time
    //
    if (BootMode != BOOT_ON_S3_RESUME) {
      if ((PcdGet32 (PcdPeiTpmPolicy) & SKIP_TPM_SELF_TEST) == 0) {
        Status = TcmContinueSelfTest ();
        if (EFI_ERROR (Status)) {
          goto Done;
        }
      }
      if ((BootMode != BOOT_ON_FLASH_UPDATE) && IsTcmUsable ()) {
        Status = PeiServicesNotifyPpi (&mMorCallbackDescriptor);
        ASSERT_EFI_ERROR (Status);
      }
    }
    //
    // Only intall TpmInitializedPpi on success
    //
    Status = PeiServicesInstallPpi (&mTpmInitializedPpiList);
    ASSERT_EFI_ERROR (Status);
  }

  if (mImageInMemory) {
    return PeimEntryMP ((EFI_PEI_SERVICES**)PeiServices);
  }

Done:
  //
  // Always intall TpmInitializationDonePpi no matter success or fail.
  // Other driver can know TCM initialization state by TpmInitializedPpi.
  //
  Status2 = PeiServicesInstallPpi (&mTpmInitializationDonePpiList);
  ASSERT_EFI_ERROR (Status2);

  return Status;
}
