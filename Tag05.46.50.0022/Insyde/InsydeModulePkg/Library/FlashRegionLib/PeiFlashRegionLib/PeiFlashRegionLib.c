/** @file
  Library for Getting Information of Flash Device Region Layout

;******************************************************************************
;* Copyright (c) 2014 - 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Guid/H2OCp.h>
#include <Library/BaseCryptLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/FlashRegionLib.h>
#include <Library/BaseLib.h>
#include <Library/H2OFvHashLib.h>
#include <Library/PerformanceLib.h>
#include <Library/H2OCpLib.h>
#include <Library/PostCodeLib.h>
#include <Library/DebugLib.h>
#include <Library/H2OLib.h>
#include <Library/HobLib.h>
#include <Ppi/Pcd.h>
#include <Ppi/PcdInfo.h>
//
// FDM signature, for not having signature in machine code of this library, assign value one by one byte
//
STATIC CHAR8   mSignature[] = {'H', 'F', 'D', 'M'};
STATIC EFI_GUID       mFdmAddressHobGuid = { 0xcfae18a2, 0x35e1, 0x4ff1, { 0x8f, 0x98, 0xeb, 0x84, 0xcd, 0x70, 0xff, 0xb0 } };
STATIC EFI_GUID       mFdmContentHobGuid = { 0x77f7a464, 0x7409, 0x4a19, { 0x8a, 0xae, 0x3f, 0x36, 0xa2, 0x02, 0x8b, 0x93 } };



/**
  Get header of FDM.

  @return      address of the FDM header.

**/
STATIC
H2O_FLASH_DEVICE_MAP_HEADER*
GetFdmHeader (
  VOID
)
{
  EFI_HOB_GUID_TYPE             *GuidHob;    
    
  GuidHob  = GetFirstGuidHob (&mFdmContentHobGuid);
  if (GuidHob != NULL) {
    return (H2O_FLASH_DEVICE_MAP_HEADER*) GET_GUID_HOB_DATA (GuidHob);
  } else {
    return (H2O_FLASH_DEVICE_MAP_HEADER*)(UINTN) PcdGet64(PcdH2OFlashDeviceMapStart);
  }
}  


/**
  Get Base Address

  @return    Base Address

**/
UINT64
EFIAPI
FdmGetBaseAddr (
  VOID
 ){

  H2O_FLASH_DEVICE_MAP_HEADER   *FdmHeader;
  UINT64                        *FdmRegionAddress;
  EFI_HOB_GUID_TYPE             *GuidHob;

  FdmHeader = GetFdmHeader ();
  if (FdmHeader == NULL){
    return 0;
  }

  if ( CompareMem(FdmHeader, mSignature, sizeof(mSignature))){
    return 0;
  }

  GuidHob = GetFirstGuidHob (&mFdmAddressHobGuid);
  if (GuidHob == NULL) {
     FdmRegionAddress = BuildGuidHob (&mFdmAddressHobGuid, sizeof(UINT64));
     if (FdmRegionAddress != NULL){
       *FdmRegionAddress = PcdGet64(PcdH2OFlashDeviceMapStart);
     }
  }

  return FdmHeader->FdBaseAddr;

 }


STATIC
BOOLEAN
EFIAPI
IsFdmHeaderValid (
  IN H2O_FLASH_DEVICE_MAP_HEADER    *FdmHeader
 ){

  UINT32 Index;
  UINT8  Sum;
  UINT8  *Data;

  Sum = 0;
  Data = (UINT8*) FdmHeader;

  //
  // Verify check sum
  //
  for (Index = 0; Index < sizeof (H2O_FLASH_DEVICE_MAP_HEADER); Index++, Data++){
    Sum += (*Data);
  }

  return (Sum == 0);

}


 /**
 SHA256 HASH calculation

 @param [in]   Message          The message data to be calculated
 @param [in]   MessageSize      The size in byte of the message data
 @param [out]  Digest           The caclulated HASH digest

 @return EFI_SUCCESS            The HASH value is calculated
 @return EFI_SECURITY_VIOLATION  Failed to calculate the HASH

**/
STATIC
EFI_STATUS
EFIAPI
CalculateSha256Hash (
  IN UINT8                      *Message,
  IN UINTN                      MessageSize,
  OUT UINT8                     *Digest
  )
{
  VOID       *HashCtx;
  UINTN      CtxSize;
  EFI_STATUS Status;

  SetMem (Digest, SHA256_DIGEST_SIZE, 0);
  CtxSize = Sha256GetContextSize ();
  HashCtx = NULL;
  HashCtx = AllocatePool (CtxSize);
  if (HashCtx == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (!Sha256Init (HashCtx)) {
    Status = EFI_SECURITY_VIOLATION;
    goto Done;
  }
  if(!Sha256Update (HashCtx, Message, MessageSize)) {
    Status = EFI_SECURITY_VIOLATION;
    goto Done;
  }
  if(!Sha256Final (HashCtx, Digest)) {
    Status = EFI_SECURITY_VIOLATION;
  } else {
    Status = EFI_SUCCESS;
  }

Done:
  FreePool (HashCtx);
  return Status;
}

STATIC
EFI_STATUS
EFIAPI
VerifyHash (
 IN UINT8                         HashMethod,
 IN UINT8                         *Hash,
 IN UINT8                         *Message,
 IN UINTN                         MessageSize
 ){

  EFI_STATUS    Status;
  UINT8         *Digest;
  UINT8         *HashBuffer;
  UINTN         DigestSize;


  if (Hash == NULL){
    return EFI_INVALID_PARAMETER;
  }

  if (Message == NULL){
    return EFI_INVALID_PARAMETER;
  }

  switch (HashMethod){

  case ENTRY_HASH_SHA256:

    Digest = AllocatePool (SHA256_DIGEST_SIZE);
    if (Digest == NULL){
      return EFI_OUT_OF_RESOURCES;
    }

    HashBuffer = H2OGetFvHash (H2O_SHA256_FV_HASH, (UINTN) Message, MessageSize);
    if (HashBuffer != NULL) {
      CopyMem (Digest, HashBuffer, SHA256_DIGEST_SIZE);
    } else {
      Status = CalculateSha256Hash (Message, MessageSize, Digest);
      if (EFI_ERROR(Status)){
        return Status;
      }
      H2OSetFvHash (H2O_SHA256_FV_HASH, (UINTN) Message, MessageSize, Digest);
    }

    DigestSize =  SHA256_DIGEST_SIZE;
    break;

  default:
    return EFI_UNSUPPORTED;
    break;

  }

  if (CompareMem (Digest, Hash, DigestSize)){
    Status = EFI_SECURITY_VIOLATION;
    goto done;
  } else {
    Status = EFI_SUCCESS;
    goto done;
  }

done:

  if (Digest != NULL){
    FreePool (Digest);
  }

  return Status;
 }

/**
  Get count of entries that FDM recorded

  @param  Count                 Unsigned integer that specifies the count of entries that FDM recorded.
  @return EFI_SUCCESS           find region type success.
  @return EFI_NOT_FOUND         Can't find region type in the FDM.

**/
EFI_STATUS
EFIAPI
FdmGetCount (
  OUT  UINT8  *Count
)
{

  H2O_FLASH_DEVICE_MAP_HEADER   *FdmHeader;
  UINT8                         *Entry;
  UINT32                        FirstEntryOffset;
  UINT32                        EntrySize;
  UINT32                        Size;
  UINT8                         *Entries;
  UINT8                         *EndPtr;
  UINT8                         InstanceNotIgnored;

  *Count = 0;
  
  FdmHeader = GetFdmHeader ();
  if ((FdmHeader == NULL) || CompareMem(FdmHeader, mSignature, sizeof(mSignature))){
    return EFI_NOT_FOUND;
  }

  if (!IsFdmHeaderValid ((H2O_FLASH_DEVICE_MAP_HEADER *) FdmHeader)) {
    return EFI_SECURITY_VIOLATION;
  }

  FirstEntryOffset = FdmHeader->Offset;
  Size        = FdmHeader->Size;
  EntrySize   = FdmHeader->EntrySize;

  Entries = ((UINT8*) FdmHeader) + FirstEntryOffset;
  EndPtr = ((UINT8*) FdmHeader) + Size;
  InstanceNotIgnored = 0;

  for (Entry = Entries;;){

    if (Entry >= EndPtr){
      break;
    }

    if ((((H2O_FLASH_DEVICE_MAP_ENTRY*) Entry)->Attribs & H2O_FLASH_DEVICE_MAP_ENTRY_ATTRIB_IGNORE) != H2O_FLASH_DEVICE_MAP_ENTRY_ATTRIB_IGNORE){
      InstanceNotIgnored++;
    }
    Entry += EntrySize;
  }
  *Count = InstanceNotIgnored;

  return EFI_SUCCESS;
}

/**
  Get Region type record in FDM by specifying order in the list.

  @param  RegionType            EFI_GUID that specifies the type of region that search for.
  @param  Instance              Unsigned integer that specifies the N-th Region type instance
  @param  RegionId              Identifier that specifies the identifier of this region..
  @param  RegionOffset          Unsigned integer that specifies the offset of this region relative to the
                                base of the flash device image.
  @param  RegionSize            Unsigned integer that specifies the region size.
  @param  Attribs               Bitmap that specifies the attributes of the flash device map entry.


  @return EFI_SUCCESS           find region type success.
  @return EFI_INVALID_PARAMETER Incorrect parameter.
  @return EFI_SECURITY_VIOLATION  Region hash is not correct.
  @return EFI_NOT_FOUND         Can't find region type in the FDM.

**/
EFI_STATUS
EFIAPI
FdmGetNAt (
  CONST IN  EFI_GUID  *RegionType,
  CONST IN  UINT8     Instance,
  OUT UINT8           *RegionId,
  OUT UINT64          *RegionOffset,
  OUT UINT64          *RegionSize,
  OUT UINT32          *Attribs
  ){

  UINT8                         *EndPtr;
  UINT8                         Matched;
  UINT32                        EntrySize;
  H2O_FLASH_DEVICE_MAP_ENTRY    *Entry;
  UINT8                         *Fdm;

  *RegionId     = 0;
  *RegionOffset = 0;
  *RegionSize   = 0;
  *Attribs      = 0;

  //
  // Instance number start from 1
  //
  if (Instance == 0){
    return EFI_INVALID_PARAMETER;
  }

  Fdm = (UINT8*)GetFdmHeader ();  
  if ((Fdm == NULL) || CompareMem(Fdm, mSignature, sizeof(mSignature))){
    return EFI_NOT_FOUND;
  }

  if (!IsFdmHeaderValid ((H2O_FLASH_DEVICE_MAP_HEADER *) Fdm)) {
    return EFI_SECURITY_VIOLATION;
  }

  EntrySize  = ((H2O_FLASH_DEVICE_MAP_HEADER*)Fdm)->EntrySize;
  EndPtr     = Fdm + ((H2O_FLASH_DEVICE_MAP_HEADER*)Fdm)->Size;
  for (Matched = 0, Entry = (H2O_FLASH_DEVICE_MAP_ENTRY*)(Fdm + ((H2O_FLASH_DEVICE_MAP_HEADER*)Fdm)->Offset);
       (UINT8*)Entry < EndPtr;
       Entry = (H2O_FLASH_DEVICE_MAP_ENTRY*)(((UINT8*)Entry) + EntrySize)){
    if (!CompareMem(&Entry->RegionType, RegionType, sizeof(EFI_GUID)) &&
        ((Entry->Attribs & H2O_FLASH_DEVICE_MAP_ENTRY_ATTRIB_IGNORE) != H2O_FLASH_DEVICE_MAP_ENTRY_ATTRIB_IGNORE)){
      Matched++;
      if (Matched == Instance){
        break;
      }
    }
  }
  if (Matched != Instance){
    return EFI_NOT_FOUND;
  }

  CopyMem (RegionId, Entry->RegionId, FDM_ENTRY_REGION_ID_SIZE);
  *RegionOffset = Entry->RegionOffset;
  *RegionSize   = Entry->RegionSize;
  *Attribs      = Entry->Attribs;

  return EFI_SUCCESS;

}


/**
  Get entry recorded in FDM

  @param  RegionType            EFI_GUID that specifies the type of region that N-th entry is.
  @param  Instance              Unsigned integer that specifies entry instance of FDM
  @param  RegionId              Identifier that specifies the identifier of this region..
  @param  RegionOffset          Unsigned integer that specifies the offset of this region relative to the
                                base of the flash device image.
  @param  RegionSize            Unsigned integer that specifies the region size.
  @param  Attribs               Bitmap that specifies the attributes of the flash device map entry.


  @return EFI_SUCCESS           find region type success.
  @return EFI_INVALID_PARAMETER Incorrect parameter.
  @return EFI_SECURITY_VIOLATION  Region hash is not correct.
  @return EFI_NOT_FOUND         Can't find region type in the FDM.

**/
EFI_STATUS
EFIAPI
FdmGetAt (
  CONST IN  UINT8     Instance,
  OUT EFI_GUID       *RegionType,
  OUT UINT8          *RegionId,
  OUT UINT64         *RegionOffset,
  OUT UINT64         *RegionSize,
  OUT UINT32         *Attribs
  ){

  UINT8                        *EndPtr;
  UINT8                        InstanceNotIgnored;
  UINT32                       EntrySize;
  H2O_FLASH_DEVICE_MAP_ENTRY   *Entry;
  UINT8                        *Fdm;

  ZeroMem(RegionType, sizeof(EFI_GUID));
  *RegionId     = 0;
  *RegionOffset = 0;
  *RegionSize   = 0;
  *Attribs      = 0;

  //
  // Instance number start from 1
  //
  if (Instance == 0){
    return EFI_INVALID_PARAMETER;
  }

  Fdm = (UINT8*)GetFdmHeader ();  
  if ((Fdm == NULL) || CompareMem(Fdm, mSignature, sizeof(mSignature))){
    return EFI_NOT_FOUND;
  }

  if (!IsFdmHeaderValid ((H2O_FLASH_DEVICE_MAP_HEADER *) Fdm)) {
    return EFI_SECURITY_VIOLATION;
  }

  EntrySize = ((H2O_FLASH_DEVICE_MAP_HEADER*)Fdm)->EntrySize;
  EndPtr    = Fdm + ((H2O_FLASH_DEVICE_MAP_HEADER*)Fdm)->Size;
  for (InstanceNotIgnored = 0, Entry = (H2O_FLASH_DEVICE_MAP_ENTRY*) (Fdm + ((H2O_FLASH_DEVICE_MAP_HEADER*)Fdm)->Offset);
       ;
       Entry = (H2O_FLASH_DEVICE_MAP_ENTRY*)(((UINT8*)Entry) + EntrySize)){

    if (Entry >= (H2O_FLASH_DEVICE_MAP_ENTRY*)EndPtr){
      return EFI_NOT_FOUND;
    }
    if ((Entry->Attribs & H2O_FLASH_DEVICE_MAP_ENTRY_ATTRIB_IGNORE) != H2O_FLASH_DEVICE_MAP_ENTRY_ATTRIB_IGNORE){
      InstanceNotIgnored++;
    }
    if(InstanceNotIgnored == Instance) {
      break;
    }
  }

  CopyMem (RegionType, &Entry->RegionType, sizeof(Entry->RegionType));
  CopyMem (RegionId, Entry->RegionId, FDM_ENTRY_REGION_ID_SIZE);
  *RegionOffset = Entry->RegionOffset;
  *RegionSize   = Entry->RegionSize;
  *Attribs      = Entry->Attribs;
  return EFI_SUCCESS;

}

EFI_STATUS
EFIAPI
FdmGetNAtWorker (
  IN  CONST EFI_GUID  *RegionType,
  IN  UINT8           Instance,
  OUT UINT64          *Address,
  OUT UINT64          *Size
){

  UINT8   Id[FDM_ENTRY_REGION_ID_SIZE];
  UINT64  Offset;
  UINT64  RegionSize;
  UINT32  Attr;

  if (EFI_ERROR (FdmGetNAt (RegionType, Instance, Id, &Offset, &RegionSize, &Attr))){
    return EFI_NOT_FOUND;
  }

  *Address = Offset + FdmGetBaseAddr ();
  *Size = RegionSize;

  return EFI_SUCCESS;

}

/**
  Get Address of Region recorded in FDM

  @param[in]   RegionType            GUID that specifies the type of region
  @param[in]   Instance              Unsigned integer that specifies entry instance of Region Type in FDM

  @return      address of the Region.

**/
UINT64
EFIAPI
FdmGetNAtAddr (
  IN CONST EFI_GUID  *RegionType,
  IN UINT8           Instance
  ){

  UINT64 Address;
  UINT64 Size;

  if ( EFI_ERROR (FdmGetNAtWorker (RegionType, Instance, &Address, &Size))){
    return 0;
  }
  return Address;
}


/**
  Get Size of Region recorded in FDM

  @param[in]   RegionType            GUID that specifies the type of region
  @param[in]   Instance              Unsigned integer that specifies entry instance of Region Type in FDM

  @return      size of the Region.

**/
UINT64
EFIAPI
FdmGetNAtSize (
  IN CONST EFI_GUID  *RegionType,
  IN UINT8           Instance
){

  UINT64 Address;
  UINT64 Size;

  if ( EFI_ERROR (FdmGetNAtWorker (RegionType, Instance, &Address, &Size))){
    return 0;
  }
  return Size;
}


EFI_STATUS
EFIAPI
FdmGetByIdWorker (
  IN CONST EFI_GUID  *RegionType,
  IN CONST EFI_GUID  *Id,
  IN  UINT8          Instance,
  OUT UINT64         *Address,
  OUT UINT64         *Size
){
  UINT8          MaxRegion;
  UINT8          Index;
  UINT8          Matched;
  EFI_STATUS     Status;
  UINT8          RegionId[FDM_ENTRY_REGION_ID_SIZE];
  UINT64         RegionOffset;
  UINT64         RegionSize;
  UINT32         Attribs;


  Status = FdmGetCount (&MaxRegion);

  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Matched = 0;

  for (Index = 1; Index < MaxRegion + 1; Index++){
    Status = FdmGetNAt (
               RegionType,
               Index,
               RegionId,
               &RegionOffset,
               &RegionSize,
               &Attribs
               );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }

    if (!CompareMem(RegionId, Id, sizeof(EFI_GUID))){
      Matched++;
    }

    if (Matched == Instance) {
      *Address = RegionOffset + FdmGetBaseAddr ();
      *Size = RegionSize;
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}


/**
  Get address of Region recorded in FDM by specific Id and region type.

  @param[in]   RegionType            GUID that specifies the type of region.
  @param[in]   Id                    Pointer of value that specifies the Id field must matched.
  @param[in]   Instance              Unsigned integer that specifies instance of Region Type in FDM

  @return      address of the Region.

**/
UINT64
EFIAPI
FdmGetAddressById (
  IN CONST EFI_GUID  *RegionType,
  IN CONST EFI_GUID  *Id,
  IN UINT8           Instance
  ){

  EFI_STATUS     Status;
  UINT64         RegionAddress;
  UINT64         RegionSize;

  Status = FdmGetByIdWorker (
             RegionType,
             Id,
             Instance,
             &RegionAddress,
             &RegionSize
             );

  if (EFI_ERROR(Status)){
    return 0;
  }

  return RegionAddress;

}


/**
  Get size of Region recorded in FDM by specific Id and region type.

  @param[in]   RegionType            GUID that specifies the type of region.
  @param[in]   Id                    Pointer of value that specifies the Id field must matched.
  @param[in]   Instance              Unsigned integer that specifies instance of Region Type in FDM

  @return      size of the Region.

**/
UINT64
EFIAPI
FdmGetSizeById (
  IN CONST EFI_GUID  *RegionType,
  IN CONST EFI_GUID  *Id,
  IN UINT8           Instance
  ){

  EFI_STATUS     Status;
  UINT64         RegionAddress;
  UINT64         RegionSize;

  Status = FdmGetByIdWorker (
             RegionType,
             Id,
             Instance,
             &RegionAddress,
             &RegionSize
             );

  if (EFI_ERROR(Status)){
    return 0;
  }

  return RegionSize;

}





/**
  Get Flash Size

  @return    Flash Size

**/
UINT64
EFIAPI
FdmGetFlashAreaSize (
  VOID
){

  UINT64  FlashSize;
  UINT8   Index;
  UINT8   MaxRegion;
  EFI_STATUS              Status;
  UINT8                   RegionId[FDM_ENTRY_REGION_ID_SIZE];
  UINT64                  RegionOffset;
  UINT64                  RegionSize;
  UINT32                  Attribs;
  EFI_GUID                RegionType;

  FlashSize = 0;

  Status = FdmGetCount (&MaxRegion);

  if (EFI_ERROR (Status)) {
    return 0;
  }

  //
  // Cumulate all regions size to get flash size
  //
  for (Index = 1; Index < MaxRegion + 1; Index++){
    Status = FdmGetAt (
               Index,
               &RegionType,
               RegionId,
               &RegionOffset,
               &RegionSize,
               &Attribs
               );
    if (EFI_ERROR (Status)) {
      return 0;
    }
    FlashSize += RegionSize;
  }
  return FlashSize;
}

/**
  Internal function to verify the FV region by checking the hash value in FDM by region type
  ,ID and instance number.
  Also invoke gH2OBaseCpVerifyFvGuid checkpoint after verifying region. It may enter deadloop
  if verify failed.

  @param[in]  RegionType          EFI_GUID that specifies the type of region.
  @param[in]  Id                  Pointer of value that specifies the Id field must matched.
  @param[in]  Instance            Unsigned integer that specifies entry instance of FDM

  @retval EFI_SUCCESS             Region verified successfully.
  @retval EFI_NOT_FOUND           Can't find region type in the FDM.
  @retval EFI_SECURITY_VIOLATION  Region verified failed.

**/
STATIC
EFI_STATUS
FdmVerifyRegion (
  IN CONST EFI_GUID  *RegionType,    OPTIONAL
  IN CONST EFI_GUID  *Id,            OPTIONAL
  IN UINT8           Instance
  )
{

  UINT8                        EntryFormat;
  UINT8                        *Hash;
  UINT8                        InstanceNotIgnored;
  UINT32                       EntrySize;
  H2O_FLASH_DEVICE_MAP_ENTRY   *Entries;
  H2O_FLASH_DEVICE_MAP_ENTRY   *Entry;
  H2O_FLASH_DEVICE_MAP_ENTRY   *EndPtr;
  H2O_FLASH_DEVICE_MAP_HEADER  *Fdm;
  EFI_STATUS                   Status;


  Fdm = GetFdmHeader ();

  EntrySize          = Fdm->EntrySize;
  EntryFormat        = Fdm->EntryFormat;
  Entries            = (H2O_FLASH_DEVICE_MAP_ENTRY *)(((UINT8 *)Fdm) + Fdm->Offset);
  EndPtr             = (H2O_FLASH_DEVICE_MAP_ENTRY *)(((UINT8 *)Fdm) + Fdm->Size);
  InstanceNotIgnored = 0;

  Status = EFI_NOT_FOUND;
  for (Entry = Entries;
       Entry < EndPtr;
       Entry = (H2O_FLASH_DEVICE_MAP_ENTRY*)(((UINT8*)Entry) + EntrySize)) {
    if ((Entry->Attribs & H2O_FLASH_DEVICE_MAP_ENTRY_ATTRIB_IGNORE) != H2O_FLASH_DEVICE_MAP_ENTRY_ATTRIB_IGNORE &&
        (RegionType == NULL || CompareGuid (RegionType, &Entry->RegionType)) &&
        (Id == NULL || !CompareMem (Id, Entry->RegionId, sizeof(EFI_GUID)))) {
      InstanceNotIgnored++;
    }
    if(InstanceNotIgnored == Instance) {
      if ((Entry->Attribs & H2O_FLASH_DEVICE_MAP_ENTRY_ATTRIB_MUTABLE) == H2O_FLASH_DEVICE_MAP_ENTRY_ATTRIB_MUTABLE) {
        Status = EFI_SUCCESS;
        break;
      }
      Hash = (UINT8*)(Entry + 1);
      PERF_START (0, "CheckRgn", NULL, 0);
      Status = VerifyHash (
                 EntryFormat,
                 Hash,
                 (UINT8*)(UINTN)(Fdm->FdBaseAddr + Entry->RegionOffset),
                 (UINTN)Entry->RegionSize
                 );
      PERF_END (0, "CheckRgn", NULL, 0);
      if (EFI_ERROR (Status)) {
        Status = EFI_SECURITY_VIOLATION;
      }
      break;
    }
  }

  if (FeaturePcdGet (PcdH2OBaseCpVerifyFvSupported)) {
    H2O_BASE_CP_VERIFY_FV_DATA      VerifyFvData;

    VerifyFvData.Size     = sizeof (H2O_BASE_CP_VERIFY_FV_DATA);
    VerifyFvData.Status   = H2O_CP_TASK_NORMAL;
    VerifyFvData.FvBase   = Fdm->FdBaseAddr + Entry->RegionOffset;
    VerifyFvData.FvLength = Entry->RegionSize;
    VerifyFvData.Trusted  = !EFI_ERROR (Status) ? TRUE : FALSE;
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OBaseCpVerifyFvGuid));
    H2OCpTrigger (&gH2OBaseCpVerifyFvGuid, &VerifyFvData);
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", VerifyFvData.Status));
    if (VerifyFvData.Status == H2O_BDS_TASK_SKIP) {
      return Status;
    } else if (VerifyFvData.Status == H2O_BDS_TASK_UPDATE) {
      Status = VerifyFvData.Trusted ? EFI_SUCCESS : EFI_SECURITY_VIOLATION;
    }
  }

  if (Status == EFI_SECURITY_VIOLATION) {
    POST_CODE (0xDE);
    DEBUG((DEBUG_ERROR, "ERROR!!! Firmware Volume Base Address: 0x%lx, Size: 0x%lx is corrupted.\n", FdmGetNAtAddr (RegionType, Instance), FdmGetNAtSize (RegionType, Instance)));
    ASSERT (FALSE);
    CpuDeadLoop ();
  }
  return Status;
}

/**
  Verify the FV region by checking the hash value in FDM by instance number.

  NOTE: This function will hang if the region has a security issue with the deadloop.

  @param[in]  Instance             Unsigned integer that specifies entry instance of FDM

  @retval EFI_SUCCESS              Region verified successfully.
  @retval EFI_NOT_FOUND            Can't find region type in the FDM.
  @retval EFI_SECURITY_VIOLATION   Region verified failed.

**/
EFI_STATUS
EFIAPI
FdmVerifyAt (
  IN CONST UINT8   Instance
  )
{
  return FdmVerifyRegion (NULL, NULL, Instance);
}

/**
  Verify the FV region by checking the hash value in FDM by region type and instance number.

  NOTE: This function will hang if the region has a security issue with the deadloop.

  @param[in]  RegionType          EFI_GUID that specifies the type of region.
  @param[in]  Instance            Unsigned integer that specifies entry instance of FDM

  @retval EFI_SUCCESS             Region verified successfully.
  @retval EFI_NOT_FOUND           Can't find region type in the FDM.
  @retval EFI_SECURITY_VIOLATION  Region verified failed.

**/
EFI_STATUS
EFIAPI
FdmVerifyNAt (
  IN CONST  EFI_GUID  *RegionType,
  IN CONST  UINT8     Instance
  )
{
  return FdmVerifyRegion (RegionType, NULL, Instance);
}

/**
  Verify the FV region by checking the hash value in FDM by region type , ID and instance number.

  NOTE: This function will hang if the region has a security issue with the deadloop.

  @param[in]  RegionType          EFI_GUID that specifies the type of region.
  @param[in]  Id                  Pointer of value that specifies the Id field must matched.
  @param[in]  Instance            Unsigned integer that specifies entry instance of FDM

  @retval EFI_SUCCESS             Region verified successfully.
  @retval EFI_NOT_FOUND           Can't find region type in the FDM.
  @retval EFI_SECURITY_VIOLATION  Region verified failed.

**/
EFI_STATUS
EFIAPI
FdmVerifyById (
  IN CONST EFI_GUID  *RegionType,
  IN CONST EFI_GUID  *Id,
  IN UINT8           Instance
  )
{
  return FdmVerifyRegion (RegionType, Id, Instance);
}

/**
  Get the FDM entry by using UINT64 board ID and region type.

  @param[in]      BoardId              Board identifier.
  @param[in]      RegionType           The EFI_GUID that specifies the type of region.
  @param[out]     DeviceMapEntries     Record the actual found entries.
  @param[in, out] DeviceEntryCount     Unsigned integer that specifies the count of specific Region in FDM.

  @retval EFI_SUCCESS                  Function completed successfully.
  @retval EFI_OUT_OF_RESOURCES         Allocate memory fail.
  @retval EFI_UNSUPPORTED              FDM version is not 3.
  @retval EFI_NOT_FOUND                FDM header or FDM entry is not found.

**/
STATIC
EFI_STATUS
GetFdmEntryData (
  IN H2O_BOARD_ID                   BoardId,
  IN CONST EFI_GUID                 *RegionType,
  OUT H2O_FLASH_DEVICE_MAP_ENTRY    **DeviceMapEntries,
  IN OUT UINT8                      *DeviceEntryCount
  )
{
  H2O_FLASH_DEVICE_MAP_HEADER   *FdmHeader;
  H2O_FLASH_DEVICE_MAP_EXTS     *FdmExt;
  H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP *Map;
  H2O_FLASH_DEVICE_MAP_ENTRY    *FirstEntry;
  H2O_FLASH_DEVICE_MAP_ENTRY    *ThisEntry;
  H2O_FLASH_DEVICE_MAP_ENTRY    *NewEntryList;
  UINT32                        ThisEntryIndex;
  UINT32                        Index;
  UINT32                        IdIndex;
  UINT64                        *BoardIds;
  UINT32                        BoardIdCount;
  UINT8                         Count;

  Count        = 0;
  BoardIdCount = 0;
  NewEntryList = NULL;
  
  FdmHeader = GetFdmHeader ();
  if ((FdmHeader == NULL) || CompareMem(FdmHeader, mSignature, sizeof(mSignature))){
    return EFI_NOT_FOUND;
  }

  //
  //  In Revision 3, reserved byte in Fdm header will be ExtensionCount.See BTR 3.17.1.
  //  The second ext block indicates the location of the first region-to-board-id-map.
  //
  if (FdmHeader->Revision < 3 || FdmHeader->ExtensionCount <= H2O_FLASH_DEVICE_MAP_EXT_BLOCK_ID_MAP) {
    return EFI_UNSUPPORTED;
  }

  FdmExt = (H2O_FLASH_DEVICE_MAP_EXTS *)((UINT8 *)FdmHeader + sizeof(H2O_FLASH_DEVICE_MAP_HEADER));

  //
  // Check the FDM extension is bigger than or equal FDM offset.
  // If yes, return EFI_UNSUPPORTED.
  //
  if ((UINT32)((UINT8 *)FdmExt - (UINT8 *)FdmHeader) >= (FdmHeader->Offset)) {
    return EFI_UNSUPPORTED;
  }

  if (*DeviceEntryCount > 0) {
    //
    // If "Count" is bigger than zero, it means unless one FDM entry is found.
    // Then allocate the pool for record the FDM entry.
    //
    NewEntryList = AllocatePool(sizeof(H2O_FLASH_DEVICE_MAP_ENTRY) * (*DeviceEntryCount));
    if (NewEntryList == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  //
  //  Jump to second extension block.
  //
  FdmExt = &FdmExt[H2O_FLASH_DEVICE_MAP_EXT_BLOCK_ID_MAP];

  Map = (H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP *)((UINT8 *)FdmHeader + FdmExt->ExtOffset);
  FirstEntry = (H2O_FLASH_DEVICE_MAP_ENTRY *)((UINT8 *)FdmHeader + FdmHeader->Offset);
  for (Index = 0; Index < FdmExt->ExtCount; Index++) {
    // typedef struct _H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP {
    //   UINT32       RegionIndex;   // Index of the region in the Flash Device Map.
    //   UINT32       BoardIdCount;  // Number of board identifiers in BoardIds.
    // //H2O_BOARD_ID BoardIds[];    // Board identifiers that are associated with the specified region index.
    // } H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP
    BoardIds = (UINT64 *)((UINT8 *)Map + sizeof(H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP));
    BoardIdCount = Map->BoardIdCount;
    for (IdIndex = 0; IdIndex < BoardIdCount; IdIndex++) {
      //
      // Check the BoardId.
      //
      if (BoardIds[IdIndex] == BoardId) {
        ThisEntryIndex = Map->RegionIndex;
        ThisEntry = (H2O_FLASH_DEVICE_MAP_ENTRY *)((UINT8 *)FirstEntry + FdmHeader->EntrySize * ThisEntryIndex);

        //
        // Check the region type.
        //
        if (CompareGuid(&ThisEntry->RegionType, RegionType)) {
          //
          // Record FDM entries.
          //
          if (*DeviceEntryCount > 0) {
            NewEntryList[Count] = *ThisEntry;
          }
          Count++;
          break;
        }
      }
    }

    // typedef struct _H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP {
    //   UINT32       RegionIndex;   // Index of the region in the Flash Device Map.
    //   UINT32       BoardIdCount;  // Number of board identifiers in BoardIds.
    // //H2O_BOARD_ID BoardIds[];    // Board identifiers that are associated with the specified region index.
    // } H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP

    //
    // Jump one "H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP" structure.
    //   //UINT32       RegionIndex;   // Index of the region in the Flash Device Map.
    //   //UINT32       BoardIdCount;  // Number of board identifiers in BoardIds.
    //
    Map = Map + 1;

    //
    // Add "H2O_BOARD_ID BoardIds[]" to next MAP header.
    //
    Map = (H2O_FLASH_DEVICE_MAP_REGION_BOARD_ID_MAP *)((UINT64 *)Map + BoardIdCount);
  }
  if (Count == 0) {
    return EFI_NOT_FOUND;
  }

  //
  // Provide calculated result and data to caller.
  //
  *DeviceEntryCount  = Count;
  *DeviceMapEntries  = NewEntryList;

  return EFI_SUCCESS;

}

/**
  Find the FDM entry by using UINT64 board ID and region type.

  @param[in]  BoardId             Board identifier.
  @param[in]  RegionType          The EFI_GUID that specifies the type of region.
  @param[out] DeviceMapEntries    Record the actual found entries.
  @param[out] DeviceEntryCount    Unsigned integer that specifies the count of specific Region in FDM.

  @retval EFI_SUCCESS             Function completed successfully.
  @retval EFI_OUT_OF_RESOURCES    Allocate memory fail.
  @retval EFI_UNSUPPORTED         FDM version is not 3.
  @retval EFI_NOT_FOUND           FDM header or FDM entry is not found.

**/
EFI_STATUS
EFIAPI
FindRegionsByBoardId (
  IN H2O_BOARD_ID                   BoardId,
  IN CONST EFI_GUID                 *RegionType,
  OUT H2O_FLASH_DEVICE_MAP_ENTRY    **DeviceMapEntries,
  OUT UINT8                         *DeviceEntryCount
  )
{
  EFI_STATUS                    Status;
  UINT8                         Count;
  H2O_FLASH_DEVICE_MAP_ENTRY    *Entries;
  UINT8                         Index;

  Entries = NULL;
  Count = 0;
  Index = 0;
  Status = EFI_NOT_FOUND;

  if (DeviceMapEntries == NULL || DeviceEntryCount == NULL){
    return EFI_INVALID_PARAMETER;
  }

  //
  // First Loop
  // When the correct entries are found, then calculate and allocate buffer.
  //
  // Second Loop
  // Get and put the entries into allocated buffer.
  // When the "Count" is bigger than zero from Part I result.
  // GetFdmEntryData() will be run second time to get FDM entries.
  //
  for (Index = 0; Index < 2; Index++) {
    Status = GetFdmEntryData (BoardId, RegionType, &Entries, &Count);
    if (EFI_ERROR(Status)) {
        return Status;
    }
  }

  *DeviceEntryCount  = Count;
  *DeviceMapEntries  = Entries;
  return Status;

}

