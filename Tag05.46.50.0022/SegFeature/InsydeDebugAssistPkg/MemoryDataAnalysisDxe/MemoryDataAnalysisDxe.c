/** @file
  Memory Data Analysis Dxe implementation.

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <MemoryDataAnalysisDxe.h>

//
// The update loaded images information event
//
EFI_EVENT mUpdateLoadedImagesInfoEvent = NULL;

//
// Current loaded images information
//
LOADED_IMAGES_INFORMATION  mLoadedImagesInfo = {
  0,
  NULL
};

//
// Current memory max address
//
EFI_PHYSICAL_ADDRESS mMemoryMaxAddress = 0;

//
// Memory Data Analysis protocol interface
// 
EFI_MEMORY_DATA_ANALYSIS_PROTOCOL mMemoryDataAnalysisProtocol = {
  MemoryDataAnalysis
};

/**
  Free all data in the mLoadedImagesInfo.
  
**/
VOID
EFIAPI
FreeLoadedImagesInfo (
  VOID
  )
{
  UINTN  Index;
 
  if (mLoadedImagesInfo.DataCount != 0) {
    if (mLoadedImagesInfo.ImageInfo != NULL) {
      for (Index = 0; Index < mLoadedImagesInfo.DataCount; Index++) {
        if (mLoadedImagesInfo.ImageInfo[Index].ImageName != NULL) {
          FreePool (mLoadedImagesInfo.ImageInfo[Index].ImageName);
        }
      }
      FreePool (mLoadedImagesInfo.ImageInfo);
      mLoadedImagesInfo.ImageInfo = NULL;
    }
    mLoadedImagesInfo.DataCount = 0;
  }
}

/**
  This function returns the image name of the LoadedImage. It is caller's responsibility
  to free the image name string if caller no longer requires it.

  @param [in]  LoadedImage     The loaded image protocol interface.

  @retval A Null-terminated ASCII string which is the image name of the LoadedImage or NULL
          if fail to retrieve.
  
**/
CHAR8 *
EFIAPI
GetImageName (
  IN  EFI_LOADED_IMAGE_PROTOCOL  *LoadedImage
  )
{
  CHAR8   *PdbPointer;
  UINTN   Index;
  UINTN   StartIndex;
  UINTN   PdbNameSize;
  CHAR8   *ImageName;
  CHAR16  *TempString;
  UINTN   TempStringSize;

  ImageName = NULL;

  //
  // Get image name from PDB
  //
  PdbPointer = PeCoffLoaderGetPdbPointer (LoadedImage->ImageBase);
  if (PdbPointer != NULL) {
    //
    // Search PDB file name start
    //
    StartIndex = 0;
    for (Index = 0; PdbPointer[Index] != '\0'; Index++) {
      if ((PdbPointer[Index] == '\\') || (PdbPointer[Index] == '/')) {
        StartIndex = Index + 1;
      }
    }
    
    //
    // Allocate memory for the output image name string
    //
    PdbNameSize = AsciiStrSize (&(PdbPointer[StartIndex]));
    ImageName = AllocateZeroPool (PdbNameSize + 4);
    if (ImageName == NULL) {
      return ImageName;
    }
    
    //
    // Copy the PDB file name to the output string, and replace .pdb with .efi
    //
    for (Index = 0; Index < PdbNameSize; Index++) {
      ImageName[Index] = PdbPointer[StartIndex + Index];
      if (ImageName[Index] == '\0') {
        ImageName[Index] = '.';
      }
      if (ImageName[Index] == '.') {
        ImageName[Index + 1] = 'e';
        ImageName[Index + 2] = 'f';
        ImageName[Index + 3] = 'i';
        ImageName[Index + 4] = '\0';
        break;
      }
    }
  
    if (Index == PdbNameSize) {
      ImageName[Index] = '\0';
    }
  }

  //
  // If get image name from PDB failure, try to get it from FilePath
  //
  if (ImageName == NULL) {
    TempString = ConvertDevicePathToText (LoadedImage->FilePath, FALSE, FALSE);
    if (TempString != NULL) {
      StartIndex = 0;
      for (Index = 0; TempString[Index] != L'\0'; Index++) {
        if ((TempString[Index] == L'\\') || (TempString[Index] == L'/')) {
          StartIndex = Index + 1;
        }
      }  
      TempStringSize = StrSize (&(TempString[StartIndex]));
      ImageName = AllocateZeroPool (TempStringSize / 2);
      if (ImageName != NULL) {
//[-start-190425-IB05820464-modify]//
        UnicodeStrToAsciiStrS (&(TempString[StartIndex]), ImageName, (TempStringSize / 2));
//[-end-190425-IB05820464-modify]//
      }
      FreePool (TempString);
    }
  }

  return ImageName;
}

/**
  Get current loaded images information and update the mLoadedImagesInfo.

  @retval EFI_SUCCESS     Update mLoadedImagesInfo successfully.
  @retval Others          Locate gEfiLoadedImageProtocolGuid error 
                          or allocate memory error.
  
**/
EFI_STATUS
EFIAPI
UpdateLoadedImagesInfo (
  VOID
  )
{
  EFI_STATUS                  Status;
  EFI_HANDLE                  *HandleBuffer;
  UINTN                       HandleCount;
  UINTN                       Index;
  EFI_LOADED_IMAGE_PROTOCOL   *LoadedImage;
  CHAR8                       *ImageName;

  //
  // Free all data in the mLoadedImagesInfo 
  //
  FreeLoadedImagesInfo ();

  //
  // Get new loaded image handles
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiLoadedImageProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Allocate new space for the mLoadedImagesInfo
  //
  mLoadedImagesInfo.ImageInfo = AllocateZeroPool (HandleCount * sizeof(IMAGE_INFORMATION));
  if (mLoadedImagesInfo.ImageInfo == NULL) {
    FreePool (HandleBuffer);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Get all loaded image information and update the mLoadedImagesInfo.
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiLoadedImageProtocolGuid,
                    (VOID**) &LoadedImage
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Get image name
    //
    ImageName = GetImageName (LoadedImage);
    
    //
    // Fill loaded image information into the mLoadedImageInfo
    //
    mLoadedImagesInfo.ImageInfo[mLoadedImagesInfo.DataCount].ImageBase     = (EFI_PHYSICAL_ADDRESS)LoadedImage->ImageBase;
    mLoadedImagesInfo.ImageInfo[mLoadedImagesInfo.DataCount].ImageSize     = LoadedImage->ImageSize;
    mLoadedImagesInfo.ImageInfo[mLoadedImagesInfo.DataCount].ImageName     = ImageName;
        

    //
    // Add the data count
    //
    mLoadedImagesInfo.DataCount++;
  }

  FreePool (HandleBuffer);

  return EFI_SUCCESS;
}

/**
  When gEfiLoadedImageProtocolGuid is installed, update the mLoadedImagesInfo.

  @param [in]  Event       The instance of EFI_EVENT.
  @param [in]  Context     The parameter passed in.
  
**/
VOID
EFIAPI
UpdateLoadedImagesInfoNotify (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_STATUS  Status;
  
  Status = UpdateLoadedImagesInfo ();
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "[StackContentDxe] Update loaded image information Fail!\n"));  
  }
}

/**
  When ReadyToBoot event is signaled, stop updating the mLoadedImagesInfo.

  @param [in]  Event       The instance of EFI_EVENT.
  @param [in]  Context     The parameter passed in.
  
**/
VOID
EFIAPI
StopUpdatingLoadedImagesNotify (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  gBS->CloseEvent (mUpdateLoadedImagesInfoEvent);
}

/**
  This function uses to check the allocated memory size of runtime types memory is whether over than
  pre-allocated memory size of runtime memory types.

  @param[out] MemoryMapSize   A pointer to the size, in bytes, of the MemoryMap buffer.
  @param[out] DescriptorSize  A pointer to the location in which firmware returns the size, in
                              bytes, of an individual EFI_MEMORY_DESCRIPTOR.

  @return  A pointer to the current memory map is returned.
           NULL is returned if space for the memory map could not be allocated from pool.
           It is up to the caller to free the memory if they are no longer needed.
**/
EFI_MEMORY_DESCRIPTOR *
EFIAPI
GetMemoryMap (
  OUT UINTN  *MemoryMapSize,
  OUT UINTN  *DescriptorSize
  )
{
  EFI_STATUS             Status;
  EFI_MEMORY_DESCRIPTOR  *EfiMemoryMap;
  UINTN                  EfiMemoryMapSize;
  UINTN                  EfiMapKey;
  UINTN                  EfiDescriptorSize;
  UINT32                 EfiDescriptorVersion;

  EfiMemoryMapSize = 0;
  EfiMemoryMap     = NULL;
  Status = gBS->GetMemoryMap (
                  &EfiMemoryMapSize,
                  EfiMemoryMap,
                  &EfiMapKey,
                  &EfiDescriptorSize,
                  &EfiDescriptorVersion
                  );
  while (Status == EFI_BUFFER_TOO_SMALL) {
    EfiMemoryMap = AllocateZeroPool (EfiMemoryMapSize);
    if (EfiMemoryMap == NULL) {
      return NULL;
    }
    Status = gBS->GetMemoryMap (
                    &EfiMemoryMapSize,
                    EfiMemoryMap,
                    &EfiMapKey,
                    &EfiDescriptorSize,
                    &EfiDescriptorVersion
                    );
    if (EFI_ERROR (Status)) {
      FreePool (EfiMemoryMap);
    }
  }

  if (!EFI_ERROR (Status)) {
    *MemoryMapSize  = EfiMemoryMapSize;
    *DescriptorSize = EfiDescriptorSize;
  }

  return EfiMemoryMap;
}

/**
  Get current memory max address from Memory Map.

  @retval The memory max address or 0 if fail to get the address
  
**/
EFI_PHYSICAL_ADDRESS
EFIAPI
GetMemoryMaxAddress (
  )
{
  EFI_MEMORY_DESCRIPTOR  *MemoryMap;
  UINTN                  MemoryMapSize;
  UINTN                  DescriptorSize;
  EFI_PHYSICAL_ADDRESS   MemoryMaxAddress;
  EFI_PHYSICAL_ADDRESS   TempAddress;
  EFI_MEMORY_DESCRIPTOR  *MemoryMapEntry;
  EFI_MEMORY_DESCRIPTOR  *MemoryMapEnd;

  MemoryMap = GetMemoryMap (&MemoryMapSize, &DescriptorSize);
  if (MemoryMap == NULL) {
    return 0;
  }

  MemoryMaxAddress = 0;
  MemoryMapEntry = MemoryMap;
  MemoryMapEnd   = (EFI_MEMORY_DESCRIPTOR *) ((UINT8 *) MemoryMap + MemoryMapSize);
  while ((UINTN)MemoryMapEntry < (UINTN)MemoryMapEnd) {
    if ((MemoryMapEntry->Type != EfiMemoryMappedIO) && (MemoryMapEntry->Type != EfiMemoryMappedIOPortSpace)) {
      TempAddress = MemoryMapEntry->PhysicalStart + MemoryMapEntry->NumberOfPages * EFI_PAGE_SIZE - 1;
      if (TempAddress > MemoryMaxAddress) {
        MemoryMaxAddress = TempAddress;
      }
    }
    MemoryMapEntry = NEXT_MEMORY_DESCRIPTOR (MemoryMapEntry, DescriptorSize);
  }
  
  FreePool (MemoryMap);
  
  return MemoryMaxAddress;
}

/**
  Get the code section range of a PE/COFF image

  @param [in]   ImageBase        The PE/COFF image base.
  @param [out]  StartAddress     The start address of the image code section.
  @param [out]  EndAddress       The end address of the image code section.

  @retval EFI_SUCCESS            Get image code section range successfully.
  @retval EFI_UNSUPPORTED        Can not identify the image header.
  
**/
EFI_STATUS
EFIAPI
GetImageCodeSectionRange (
  IN  EFI_PHYSICAL_ADDRESS  ImageBase,
  OUT EFI_PHYSICAL_ADDRESS  *StartAddress,
  OUT EFI_PHYSICAL_ADDRESS  *EndAddress
  )
{
  EFI_IMAGE_DOS_HEADER                 *DosHdr;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION  Hdr;

  DosHdr = (EFI_IMAGE_DOS_HEADER*) ImageBase;
  
  //
  // Get PE handler
  //
  if (DosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE) {
    Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32*) (((UINT8*) DosHdr) + DosHdr->e_lfanew);
  } else {
    Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32*) DosHdr;
  }

  //
  // Get code section range
  // It is valid for PE32 and PE32+ images
  //
  if (Hdr.Pe32->Signature == EFI_IMAGE_NT_SIGNATURE) {
    *StartAddress = ImageBase + Hdr.Pe32->OptionalHeader.BaseOfCode;
    *EndAddress   = *StartAddress + Hdr.Pe32->OptionalHeader.SizeOfCode - 1;
  } else {
    return EFI_UNSUPPORTED;
  }
  
  return EFI_SUCCESS;
}

/**
  Search the Address in the mLoadedImagesInfo. 
  If the Address is in the code section of a loaded image, return its image base and image name.

  @param [in]   Address             The address which will be searched.
  @param [out]  ImageBase           The image base of the Address.
  @param [out]  ImageName           The image name of the Address.

  @retval TRUE                      The Address is in the code section of a loaded image.
  @retval FALSE                     The Address is not in the code section of a loaded image.
                                    The input ImageName or ImageBase is NULL.

**/
BOOLEAN
EFIAPI
IsCodeAddressInLoadedImage (
  IN  EFI_PHYSICAL_ADDRESS  Address,
  OUT EFI_PHYSICAL_ADDRESS  *ImageBase,
  OUT CHAR8                 **ImageName
  )
{
  UINTN                 Index;
  EFI_PHYSICAL_ADDRESS  ImageEnd;
  EFI_PHYSICAL_ADDRESS  CodeSectionStart;
  EFI_PHYSICAL_ADDRESS  CodeSectionEnd;
  EFI_STATUS            Status;

  if ((ImageBase == NULL) || (ImageName == NULL)) {
    return FALSE;
  }

  //
  // Check if the Address is in a loaded image
  //
  for (Index = 0; Index < mLoadedImagesInfo.DataCount; Index++) {
    ImageEnd = (mLoadedImagesInfo.ImageInfo[Index].ImageBase + 
                mLoadedImagesInfo.ImageInfo[Index].ImageSize - 1);
    if ((Address >= mLoadedImagesInfo.ImageInfo[Index].ImageBase) && 
        (Address <= ImageEnd)) {
      break;
    }
  }
  
  if (Index >= mLoadedImagesInfo.DataCount) {
    return FALSE;
  }

  //
  // Get the code section range of the loaed image
  //
  Status = GetImageCodeSectionRange (
             mLoadedImagesInfo.ImageInfo[Index].ImageBase,
             &CodeSectionStart,
             &CodeSectionEnd
             );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "[MemoryDataAnalysisDxe] Find an unsupported image type!\n")); 
    return FALSE;
  }

  //
  // Check if the Address is in the code section of the loaded image
  //
  if (!((Address >= CodeSectionStart) && (Address <= CodeSectionEnd))) {
    return FALSE;
  }

  //
  // The Address is a code address in a loaded image
  // Return the image information
  //
  *ImageBase = mLoadedImagesInfo.ImageInfo[Index].ImageBase;
  *ImageName = mLoadedImagesInfo.ImageInfo[Index].ImageName;
    
  return TRUE;
}

/**
  Updata the input image name to PDB image name. 

  @param [in]       ImageBase       The image base which is used to get the PDB pointer.
  @param [in, out]  ImageName       The image name buffer which will be updated.

  @retval TRUE                      Update image name successfully.
  @retval FALSE                     ImageName is NULL or can not get the PDB pointer.

**/
BOOLEAN
EFIAPI
UpdateImageNameFromPdb (
  IN     EFI_PHYSICAL_ADDRESS  ImageBase,
  IN OUT CHAR8                 *ImageName
  )
{
  UINTN    PdbNameSize;
  CHAR8    *PdbPointer;
  UINTN    Index;
  UINTN    StartIndex;
  UINTN    BufferSize;

  if (ImageName == NULL) {
    return FALSE;
  }
  
  PdbPointer = PeCoffLoaderGetPdbPointer ((VOID *)ImageBase);
  if (PdbPointer == NULL) {
    return FALSE;
  }

  //
  // Search PDB file name start
  //
  StartIndex = 0;
  for (Index = 0; PdbPointer[Index] != '\0'; Index++) {
    if ((PdbPointer[Index] == '\\') || (PdbPointer[Index] == '/')) {
      StartIndex = Index + 1;
    }
  }
  
  PdbNameSize = AsciiStrSize (&(PdbPointer[StartIndex]));

  //
  // Use input string buffer to output PDB image name
  //
  BufferSize = AsciiStrSize (ImageName);
    
  //
  // Copy the PDB file name to the output string, and replace .pdb with .efi
  //
  for (Index = 0; Index < PdbNameSize && Index < (BufferSize - 4); Index++) {
    ImageName[Index] = PdbPointer[StartIndex + Index];
    if (ImageName[Index] == '\0') {
      ImageName[Index] = '.';
    }
    if (ImageName[Index] == '.') {
      ImageName[Index + 1] = 'e';
      ImageName[Index + 2] = 'f';
      ImageName[Index + 3] = 'i';
      ImageName[Index + 4] = '\0';
      break;
    }
  }

  if ((Index == PdbNameSize) || (Index == (BufferSize - 4))){
    ImageName[Index] = '\0';
  }

  return TRUE;
}

/**
  Convert memory data to a Null-terminated ASCII string.

  This function will convert non-printable character to '.' character.
  
  @param [out]  StringBuffer         The output string buffer.
  @param [in]   StringBufferSize     The size of the StringBuffer.
  @param [in]   MemoryData           The memory data which will be converted to 
                                     a Null-terminated ASCII string.

  @retval The number of ASCII characters in the produced output StringBuffer 
          not including the Null-terminator.
          
**/
UINTN
EFIAPI
ConvertMemoryDataToString (
  OUT CHAR8  *StringBuffer,
  IN  UINTN  StringBufferSize,
  IN  UINTN  MemoryData
  )
{
  UINTN  Size;
  UINTN  Index;
  UINT8  *TempByte;

  Size = sizeof (MemoryData);
  
  if (StringBufferSize < Size + 1) {
    return 0;
  }

  TempByte = (UINT8*)&MemoryData;
  for (Index = 0; Index < Size; Index++) {
    if (TempByte[Index] < ' ' || TempByte[Index] > '~') {
      StringBuffer[Index] = '.';
    } else {
      StringBuffer[Index] = TempByte[Index];
    }
  }
  StringBuffer[Index] = '\0';
  
  return Size;
}

/**
  Send a formatted ASCII string to serial port.

  @param [in]   FormatString     A Null-terminated ASCII format string.
  @param [in]   ...              A Variable argument list whose contents are accessed based
                                 on the format string specified by FormatString.

  @retval The number of ASCII characters sent to serial port.
  
**/
UINTN
EFIAPI
SerialPortPrint (
  IN  CONST CHAR8  *FormatString,
  ...  
  )
{
  UINT8    Buffer[SERIAL_OUTPUT_BUFFER_SIZE];
  VA_LIST  Marker;
  UINTN    CharCount;

  VA_START (Marker, FormatString);
  CharCount = AsciiVSPrint ((CHAR8 *)Buffer, sizeof (Buffer), FormatString, Marker);
  VA_END (Marker);

  CharCount = SerialPortWrite (Buffer, CharCount);
  
  return CharCount;
}

/**
  Analyze memory data and print them on serial port

  If each memory data(sizeof (UINTN)) is an address in the code section of a loaded image, 
  it will show the image information. 
  The actual analysis length will be aligned to sizeof (UINTN).

  Note: If the StartAddress is not aligned to sizeof (UINTN), 
        it will get incorrect image information.

  @param [in]  StartAddress     The start address of memory 
  @param [in]  Length           The length, in bytes, of memory 
  
  @retval EFI_SUCCESS           Analyze memory data successfully.
  
**/
EFI_STATUS
EFIAPI
MemoryDataAnalysis (
  IN  EFI_PHYSICAL_ADDRESS    StartAddress,
  IN  UINTN                   Length
  )
{
  UINTN                        *DataPointer;
  EFI_PHYSICAL_ADDRESS         ImageBase;
  CHAR8                        *ImageName;
  CHAR8                        TempString[8];
  CHAR8                        StringBuffer[sizeof(UINTN)+1]; 
  
  if (StartAddress + Length - 1 > mMemoryMaxAddress) {
    return EFI_INVALID_PARAMETER;
  }

  SerialPortPrint ("Memory Content\n");
  SerialPortPrint ("Address            Content           Information\n");

  //
  // Print each memory data
  // Each data size is sizeof (UINTN)
  //
  DataPointer = (UINTN *)StartAddress;
  while ((UINTN)DataPointer < StartAddress + Length) {
    //
    // Print memory address and data
    //
    SerialPortPrint ("%016Lx:  %016Lx  ", DataPointer, *DataPointer);

    //
    // Print memory data information
    //
    if (IsCodeAddressInLoadedImage (*DataPointer, &ImageBase, &ImageName)) {
      //
      // This driver can not get the SMM image PDB name 
      // outside SMM mode (UpdateLoadedImagesInfo).
      // Provides an opportunity to update the image name in SMM mode.
      //
      CopyMem (TempString, ImageName, 7);
      TempString[7] = '\0';
      if (AsciiStrCmp (TempString, "FvFile(") == 0) {
        UpdateImageNameFromPdb (ImageBase, ImageName);
      }
      
      //
      // If the data is an address in the code section of a loaded image, 
      // print its image information
      //
      SerialPortPrint (
        "IMB: %016Lx + %08Lx,  %a\n", 
        ImageBase, 
        *DataPointer - ImageBase, 
        ImageName
        );
    } else {
      //
      // Convert the content to a string and print it
      //
      ConvertMemoryDataToString (
        StringBuffer, 
        sizeof (StringBuffer), 
        *DataPointer
        );
      SerialPortPrint ("%a\n", StringBuffer);
    }

    //
    // Next stack element
    //
    DataPointer++;
  }
  
  return EFI_SUCCESS;
}

/**
  Entry point of Memory Data Analysis Dxe Driver.

  This driver provides a protocol that analyzes memory content and prints it on serial port.

  @param [in]  ImageHandle     The firmware allocated handle for the UEFI image.
  @param [in]  SystemTable     A pointer to the EFI System Table.

  @retval EFI_SUCCESS          The operation completed successfully.
  @retval Others               An unexpected error occurred.
  
**/
EFI_STATUS
EFIAPI
MemoryDataAnalysisDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;
  VOID        *Registration;

  //
  // Get current loaded images information
  //
  Status = UpdateLoadedImagesInfo ();
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "[MemoryDataAnalysisDxe] Can not get loaded image information!\n"));
  }

  //
  // Get current memory max address
  //
  mMemoryMaxAddress = GetMemoryMaxAddress ();
  if (mMemoryMaxAddress == 0) {
    Status = EFI_UNSUPPORTED;
    goto Error;
  }
  
  //
  // Register a notify function to update loaded images information
  //
  Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    UpdateLoadedImagesInfoNotify,
                    NULL,
                    &mUpdateLoadedImagesInfoEvent
                    );
  if (EFI_ERROR (Status)) {
    goto Error;
  }
  Status = gBS->RegisterProtocolNotify (
                  &gEfiLoadedImageProtocolGuid,
                  mUpdateLoadedImagesInfoEvent,
                  &Registration
                  );
  if (EFI_ERROR (Status)) {
    goto Error;
  }

  //
  // Register a notify function to close the mUpdateLoadedImagesInfoEvent 
  // when ReadyToBoot event is signaled
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             StopUpdatingLoadedImagesNotify,
             NULL,
             &Event
             );
  if (EFI_ERROR (Status)) {
    goto Error;
  }

  //
  // Install Memory Data Analysis protocol
  //
  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gH2ODebugAssistMemoryDataAnalysisProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mMemoryDataAnalysisProtocol
                  );
  
Error:
  if (EFI_ERROR (Status)) {
    FreeLoadedImagesInfo ();
  }
  
  return Status;
}

