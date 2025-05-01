/** @file
  Tag Version library functions with no library constructor/destructor.

;******************************************************************************
;* Copyright (c) 2017, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

**/

#include <Library/DxeTagVersionLib.h>

/**
  Check TagVersion string is valid or not.

  @param[IN]  Str   String pointer.
  @param[IN]  Size  Size of String.

  @retval     True  String is valid.
  @retval     False String is not valid.
**/
BOOLEAN
IsValidTagVersion (
  IN CHAR8     *Str,
  IN UINTN     Size 
  )
{
  if (Size == 0) {
    return FALSE;    
  }
  
  if (Str[Size-1] != '\0') {
    return FALSE;
  }

  while (*Str != '\0' && *Str != ':') {
    Str++;
  }

  if (*Str == ':') {
    return TRUE;
  }

  return FALSE;
}

/**
  Use token number to get Tag Version. 

  @param[in]  TokenNumber        The previous PCD token number.  If 0, then retrieves the first PCD
                                 token number.
  @param[OUT] NameVersion        NameVersion string for current Pcd token number.
  @param[OUT] NameVersionLen     NameVersionLen for current NameVersion.
  
  @retval                        The next token number.

**/
UINTN
EFIAPI
GetNextTagVersion (
  IN  UINTN    TokenNumber,
  OUT CHAR8    **NameVerStr,
  OUT UINTN    *NameVerSize,
  IN  BOOLEAN  *ValidFlag
  )
{
  CHAR8        *Ptr;

  TokenNumber = LibPcdGetNextToken (&gSegFeatureVerTokenSpaceGuid, TokenNumber);
  if (TokenNumber == 0) {
    return TokenNumber;
  }
  
  Ptr = LibPcdGetExPtr (&gSegFeatureVerTokenSpaceGuid, TokenNumber);
  
  *NameVerStr = Ptr;
  *NameVerSize = LibPcdGetExSize (&gSegFeatureVerTokenSpaceGuid, TokenNumber);

  if (IsValidTagVersion (Ptr, *NameVerSize) == FALSE) {
    DEBUG ((DEBUG_ERROR, "[DxeTagVersion] ERROR : invliad Tag version string. TokenNumber = %d\n", TokenNumber));
    ASSERT (FALSE);
    *ValidFlag = FALSE;
  }

  return TokenNumber;
}

/**
  Use token number to get Tag Version. 

  @param[in]  NameVersion            NameVersion string to be parsed.
  @param[OUT] NamePtr                Name string pointer.
  @param[OUT] NameLen                The length of name string.
  @param[OUT] VerPtr                 Version string pointer.
  @param[OUT] VerLen                 The length of version string.
  
  @retval     EFI_INVALID_PARAMETER  NameVersion is NULL.
  @retval     EFI_SUCCESS            NameVersion parsed successfully.

**/
EFI_STATUS
EFIAPI
ParseFeatureStr (
  IN   CHAR8    *NameVersion, 
  OUT  CHAR8    **NamePtr, 
  OUT  UINTN    *NameLen, 
  OUT  CHAR8    **VerPtr, 
  OUT  UINTN    *VerLen
  )
{
  if (NameVersion == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *NameLen = 0;
  *VerLen = 0;
  
  *NamePtr = NameVersion;
  while (*NameVersion != ':') {
    (*NameLen)++;
    NameVersion++;
  }

  // Skip ':' char
  NameVersion++;
  *VerPtr = NameVersion;

  while (*NameVersion != '\0') {
    (*VerLen)++;
    NameVersion++;
  }

  return EFI_SUCCESS;
}

/**
  Copy string to the StringTable.
  
  @param[in] StrTable   Destination of CopyMem().
  @param[in] Str        Source of CopyMem().
  @param[in] Length     the length of string to copy.
  
**/
VOID
EFIAPI
CopyStrLen (
  IN CHAR8  *StrTable,
  IN CHAR8  *Str,
  IN UINTN  Length
  )
{
  CopyMem (StrTable, Str, Length);
  StrTable += Length;
  *StrTable = '\0';
}

/**
  Create H2O_FEATURE_VERSION_TABLE_PROTOCOL and install protocol.

  @param[OUT] Protocol              The pointer of H2O_FEATURE_VERSION_TABLE_PROTOCOL

  @retval     EFI_OUT_OF_RESOURCES  Allocate pool failed.
  @retval     EFI_SUCCESS           Create and install protocol successfully.
  
**/
EFI_STATUS
EFIAPI
InstallFeatureVersionTable (
  OUT H2O_FEATURES_VER_TABLE_PROTOCOL   **Protocol
  )
{
  H2O_FEATURES_VER_TABLE_PROTOCOL      *FeatureVersionProtocol;
  FEATURE_VERSION                      *NameVerTable;
  EFI_STATUS                           Status;
  CHAR8                                *NameVersion;
  CHAR8                                *StrTable;
  CHAR8                                *PkgName;
  CHAR8                                *TagVersion;
  UINTN                                Size;
  UINTN                                NameLen;
  UINTN                                VerLen;
  UINTN                                AllStrSize;
  UINTN                                TotalSize;
  UINTN                                TokenNumber;
  UINTN                                Count;
  BOOLEAN                              ValidFlag;

  TokenNumber = 0;
  TotalSize = 0;
  AllStrSize = 0;
  Count = 0;
  
  //
  // Calculator the protocol table size
  //
  do {
    ValidFlag = TRUE;
    TokenNumber = GetNextTagVersion (TokenNumber, &NameVersion, &Size, &ValidFlag);
    if (TokenNumber != 0 && ValidFlag == TRUE) {
      AllStrSize += Size;
      Count++;
    }

  } while (TokenNumber != 0);
  
  TotalSize = sizeof(H2O_FEATURES_VER_TABLE_PROTOCOL) + sizeof(FEATURE_VERSION)*(Count+1) + AllStrSize;
  
  FeatureVersionProtocol = AllocatePool (TotalSize);
  if (FeatureVersionProtocol == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  FeatureVersionProtocol->Count = Count;
  FeatureVersionProtocol->Table = (FEATURE_VERSION*)(FeatureVersionProtocol + 1);
  NameVerTable = FeatureVersionProtocol->Table;
  StrTable = (CHAR8*)(NameVerTable + (Count+1));
  
  //
  //Fill name and version into table.
  //
  TokenNumber = 0;
 
  do {
    NameLen = 0;
    VerLen = 0;
  	ValidFlag = TRUE;
    TokenNumber = GetNextTagVersion (TokenNumber, &NameVersion, &Size, &ValidFlag);
    
    if (TokenNumber != 0 && ValidFlag == TRUE) {
      Status = ParseFeatureStr (NameVersion, &PkgName, &NameLen, &TagVersion, &VerLen);
      if (EFI_ERROR(Status)) {
        return Status;
      }

      //
      // Copy string into Buffer.
      //
      NameVerTable->FeatureStr = StrTable;
      CopyStrLen (StrTable, PkgName, NameLen);
      StrTable += (NameLen+1);

      NameVerTable->VersionStr = StrTable;
      CopyStrLen (StrTable, TagVersion, VerLen);
      StrTable += (VerLen+1);
  	
      NameVerTable++;
    }
    
  } while (TokenNumber != 0);
  
  NameVerTable->FeatureStr = NULL;
  NameVerTable->VersionStr = NULL;

  //
  // Install protocol on gImageHandle
  //
  *Protocol = FeatureVersionProtocol;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  gImageHandle,
                  &gH2OSegFeatureVersionProtocolGuid,
                  FeatureVersionProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_INFO, "[DxeTagVersionLib] Install FeatureVersionProtocol failed!\n\n"));
  }
  
  return EFI_SUCCESS;
}

/**
  Get the table that contains name string pointer and version string pointer.
  If count != NULL, get the number of name and version pairs.

  H2O_FEATURES_VER_TABLE_PROTOCOL
  -------------------------------
  |              |              |
  |    Count     |    *Table    |
  |              |              |
  |-----------------------------|
  |              |              |
  |  *FeatureStr |  *VersionStr |
  |              |              |
  |-----------------------------|
  |      .       |      .       |
  |      .       |      .       |
  |      .       |      .       |
  |-----------------------------|
  |              |              |
  |     NULL     |     NULL     |
  |              |              |
  |-----------------------------|
  |                             |
  |        String Section       |
  |                             |
  |                             |
  -------------------------------
  @param[IN] Table       FEATURE_VERSION pointer.
  @param[IN] Count       The number of name and version pairs.

  @retval    EFI_SUCCESS Get table successfully.
**/
EFI_STATUS
EFIAPI
GetFeatureVersionTable (
  IN FEATURE_VERSION  **Table,
  IN UINTN            *Count OPTIONAL
  )
{
  EFI_STATUS                           Status;
  H2O_FEATURES_VER_TABLE_PROTOCOL      *Protocol;
  
  Status = gBS->LocateProtocol (&gH2OSegFeatureVersionProtocolGuid, NULL, &Protocol);
  if (EFI_ERROR(Status)) {
    Status = InstallFeatureVersionTable (&Protocol);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  *Table = Protocol->Table;
  if (Count != NULL) {
    *Count = Protocol->Count;
  }

  return EFI_SUCCESS;
}

/**
  Get the string pointer of version.

  @param[IN] FeatureName  Name string of package.

  @retval    The pointer of version string. 
**/
CHAR8 *
EFIAPI
GetFeatureVersionStr (
  IN CHAR8  *FeatureName
  )
{
  EFI_STATUS          Status;
  FEATURE_VERSION     *Table;

  Status = GetFeatureVersionTable (&Table, NULL);
  if (EFI_ERROR(Status)) {
    return NULL;
  }

  while (Table->FeatureStr != NULL) {
    if (AsciiStrCmp (FeatureName, Table->FeatureStr) == 0) {
      return Table->VersionStr;
    }
    Table++;
  }

  return NULL;
}

/**
  Check whether the char is decimal digit.

  @param[IN]  Ch    The char to check.

  @retval     True  The char is decimal digit.
  @retval     False The char is not decimal digit.
**/
BOOLEAN
IsDecimalDigitChar (
  CHAR8     Ch
  )
{
  if (Ch >= '0' && Ch <= '9') {
    return TRUE;
  }

  return FALSE;
}

/**
  Get version string and convert it into hex UINTN.
  Example: 01.01.01.0001 will turn into 0x0101010001.
  
  @param[IN] FeatureName  Name string of package.

  @retval    UINTN of the version string.
             If FeatureName == NULL, return -1.
             If VersionStr is invalid , return 0.
**/
UINTN
EFIAPI
GetFeatureVersionUint (
  IN CHAR8  *FeatureName
  )
{
  EFI_STATUS          Status;
  CHAR8               *StrPtr;
  UINTN               Val;
  UINTN               Result;
  UINTN               DotCount;

  DotCount = 0;
  Result = 0;

  StrPtr = GetFeatureVersionStr (FeatureName);
  if (StrPtr == NULL) {
    return (UINTN) -1;
  }
  
  while (*StrPtr) {
    if (!IsDecimalDigitChar (*StrPtr)) {
      return 0;
    }
    
    Status = AsciiStrHexToUintnS (StrPtr, &StrPtr, &Val);

    if (EFI_ERROR(Status)) {
      return 0;
    }

    if (Val > 0x9999) {
      return 0;
    } else if (DotCount < 3 && Val > 0x99) {
      return 0;
    }

    if (DotCount < 3) {
      Result <<= 8;
    } else {
      Result <<= 16;
    }

    Result |= Val;

    if(*StrPtr != '.' && *StrPtr != '\0') {
      return 0;
    }
    
    if (*StrPtr == '.') {
      // skip '.'
      StrPtr++;
      DotCount++;
    }
  }

  if (DotCount != 3) {
    return 0;
  }

  return Result;
}  

