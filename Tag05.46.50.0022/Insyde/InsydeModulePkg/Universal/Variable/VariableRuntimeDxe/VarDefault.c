/** @file
    Implementation functions and structures for var default services.

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "VarDefault.h"

/**
  Internal function to get the start address of variable default header according to
  input SKU ID.

  @param[in]  SkuId   Input SKU ID.

  @return  The address of variable store header or NULL if not found.
**/
STATIC
VOID *
GetVariableDefaultStoreHeaderBySkuId (
  IN  H2O_BOARD_ID       SkuId
  )
{
  VARIABLE_STORE_HEADER           *VariableStoreHeader;
  ECP_VARIABLE_STORE_HEADER       *EcpVariableStoreHeader;

  if (PcdGetBool (PcdUseEcpVariableStoreHeader)) {
    for (EcpVariableStoreHeader = (ECP_VARIABLE_STORE_HEADER *) (UINTN) mVariableModuleGlobal->VariableDefaultCache;
         EcpVariableStoreHeader != NULL &&
         EcpVariableStoreHeader->Signature == ECP_VARIABLE_STORE_SIGNATURE &&
         EcpVariableStoreHeader->Format == VARIABLE_STORE_FORMATTED &&
         EcpVariableStoreHeader->State == VARIABLE_STORE_HEALTHY &&
         (EcpVariableStoreHeader->Flags & VARIABLE_STORE_ACTIVE_MASK) == VARIABLE_STORE_ACTIVE &&
         EcpVariableStoreHeader->Size >= sizeof (ECP_VARIABLE_STORE_HEADER) &&
         (UINTN) EcpVariableStoreHeader < (UINTN) mVariableModuleGlobal->VariableDefaultCache + mVariableModuleGlobal->VariableDefaultSize;
         EcpVariableStoreHeader = (ECP_VARIABLE_STORE_HEADER *) ((UINT8 *) EcpVariableStoreHeader + EcpVariableStoreHeader->Size)) {
      if (EcpVariableStoreHeader->DefaultId == 0 && SkuId == GetBoardIdFromVariableStore ((VARIABLE_STORE_HEADER*) EcpVariableStoreHeader)) {
        return (VOID *) EcpVariableStoreHeader;
      }
    }
  } else {
    for (VariableStoreHeader = (VARIABLE_STORE_HEADER *) (UINTN) mVariableModuleGlobal->VariableDefaultCache;
         VariableStoreHeader != NULL &&
         (CompareGuid (&VariableStoreHeader->Signature, &gEfiAuthenticatedVariableGuid) ||
          CompareGuid (&VariableStoreHeader->Signature, &gEfiVariableGuid)) &&
         VariableStoreHeader->Format == VARIABLE_STORE_FORMATTED &&
         VariableStoreHeader->State == VARIABLE_STORE_HEALTHY &&
         (VariableStoreHeader->Flags & VARIABLE_STORE_ACTIVE_MASK) == VARIABLE_STORE_ACTIVE &&
         VariableStoreHeader->Size >= sizeof (VARIABLE_STORE_HEADER) &&
         (UINTN) VariableStoreHeader < (UINTN) mVariableModuleGlobal->VariableDefaultCache + mVariableModuleGlobal->VariableDefaultSize;
         VariableStoreHeader = (VARIABLE_STORE_HEADER *) ((UINT8 *) VariableStoreHeader + VariableStoreHeader->Size)) {
      if (VariableStoreHeader->DefaultId == 0 && SkuId == GetBoardIdFromVariableStore (VariableStoreHeader)) {
        return (VOID *) VariableStoreHeader;
      }
    }
  }
  return NULL;
}

/**
  Get default SKU ID variable store header in variable default store.

  @return  The address of variable store header or NULL if not found.
**/
STATIC
VOID *
GetDefaultVariableDefaultStoreHeader (
  VOID
  )
{
  return GetVariableDefaultStoreHeaderBySkuId (0);
}

/**
  According to current SKU ID to get variable store header in variable default store.

  @return  The address of variable store header or NULL if not found.
**/
STATIC
VOID *
GetCurrentVariableDefaultStoreHeader (
  VOID
  )
{
  //
  // BUG! BUG!
  // Sku value may be changed after PcdSkuInit () is invoked in PlatformStage1Pei entrypoint.
  // Different SKU value may cause the variable data is different.
  //
  //
  return GetVariableDefaultStoreHeaderBySkuId ((H2O_BOARD_ID) LibPcdGetSku ());
}

/**
  Internal function to check whether the variable is stored in variable store.

  @param[in]  VariableName  Name of the variable to be found
  @param[in]  VendorGuid    Vendor GUID to be found.

  @retval TRUE              Can find variable in variable store.
  @retval FALSE             Cannot find variable in variable store.
**/
STATIC
BOOLEAN
VariableInVariableStore (
  IN CONST  CHAR16            *VariableName,
  IN CONST  EFI_GUID          *VendorGuid
  )
{
  VARIABLE_POINTER_TRACK  Variable;
  EFI_STATUS              Status;
  UINTN                   VariableCount;

  Status = FindVariable (
             VariableName,
             VendorGuid,
             &Variable,
             &VariableCount,
             &mVariableModuleGlobal->VariableBase
             );
  return Status == EFI_SUCCESS;
}


/**
  Get variable from specific variable store region by lifetime.

  @param[in]  VariableName         Name of the variable to be found
  @param[in]  VendorGuid           Vendor GUID to be found.
  @param[in]  VariableStoreHeader  Pointer to the start address of variable store header.
  @param[in]  ByLifetime           BOOLENA value to determine by lifetime or not.
  @param[out] PtrTrack             Variable Track Pointer structure that contains Variable Information.

  @return  The address of variable store header or NULL if not found.
**/
STATIC
EFI_STATUS
GetVariableFromSpecificDefaultRegionByLifetime (
  IN CONST  CHAR16                           *VariableName,
  IN CONST  EFI_GUID                         *VendorGuid,
  IN CONST  VOID                             *VariableStoreHeader,
  IN        BOOLEAN                          ByLifetime,
  OUT VARIABLE_POINTER_TRACK                 *PtrTrack
  )
{
  VARIABLE_HEADER                 *Variable;

  PtrTrack->StartPtr = (VARIABLE_HEADER *) ((UINT8 *) VariableStoreHeader + GetVariableStoreHeaderSize ());
  PtrTrack->EndPtr   = (VARIABLE_HEADER *) ((UINT8 *) VariableStoreHeader + GetVariableStoreSize (VariableStoreHeader));
  Variable           = PtrTrack->StartPtr;
  while ((GetNextVariablePtr (Variable) <= PtrTrack->EndPtr) && IsValidVariableHeader (Variable)) {
    if ((CompareGuid (VendorGuid, &Variable->VendorGuid) &&
        !StrCmp (VariableName, GET_VARIABLE_NAME_PTR (Variable))) ||
        VariableName [0] == 0) {
      if (Variable->State == VAR_ADDED && !VariableInVariableStore (GET_VARIABLE_NAME_PTR (Variable), &Variable->VendorGuid) &&
          !(ByLifetime && VariableAtRuntime () && (Variable->Attributes & EFI_VARIABLE_RUNTIME_ACCESS) != EFI_VARIABLE_RUNTIME_ACCESS)) {
        PtrTrack->CurrPtr = Variable;
        return EFI_SUCCESS;
      }
    }
    Variable = GetNextVariablePtr (Variable);
  }

  return EFI_NOT_FOUND;
}

/**
  Get variable from specific variable store region.

  @param[in]  VariableName         Name of the variable to be found
  @param[in]  VendorGuid           Vendor GUID to be found.
  @param[in]  VariableStoreHeader  Pointer to the start address of variable store header.
  @param[out] PtrTrack             Variable Track Pointer structure that contains Variable Information.

  @return  The address of variable store header or NULL if not found.
**/
STATIC
EFI_STATUS
GetVariableFromSpecificDefaultRegion (
  IN CONST  CHAR16                           *VariableName,
  IN CONST  EFI_GUID                         *VendorGuid,
  IN CONST  VOID                             *VariableStoreHeader,
  OUT VARIABLE_POINTER_TRACK                 *PtrTrack
  )
{
  return GetVariableFromSpecificDefaultRegionByLifetime (
           VariableName,
           VendorGuid,
           VariableStoreHeader,
           FALSE,
           PtrTrack
           );
}

/**
  Get variable from variable default store.

  @param[in]  VariableName  Name of the variable to be found
  @param[in]  VendorGuid    Vendor GUID to be found.
  @param[out] PtrTrack      Variable Track Pointer structure that contains Variable Information.

  @retval EFI_SUCCESS   Find variable in variable default store successfully.
  @retval EFI_NOT_FOUND Cannot find variable in variable default store.
**/
EFI_STATUS
GetVariableFromVariableDefault (
  IN CONST  CHAR16            *VariableName,
  IN CONST  EFI_GUID          *VendorGuid,
  OUT VARIABLE_POINTER_TRACK  *PtrTrack
  )
{
  VOID                            *VariableStoreHeader;
  VOID                            *DefaultVariableStoreHeader;
  EFI_STATUS                      Status;


  PtrTrack->CurrPtr   = NULL;
  VariableStoreHeader = GetCurrentVariableDefaultStoreHeader ();
  if (VariableStoreHeader != NULL) {
    Status = GetVariableFromSpecificDefaultRegion (VariableName, VendorGuid, VariableStoreHeader, PtrTrack);
    if (Status == EFI_SUCCESS) {
      return Status;
    }
  }
  DefaultVariableStoreHeader = GetDefaultVariableDefaultStoreHeader ();
  if (DefaultVariableStoreHeader != NULL && DefaultVariableStoreHeader != VariableStoreHeader) {
    Status = GetVariableFromSpecificDefaultRegion (VariableName, VendorGuid, DefaultVariableStoreHeader, PtrTrack);
    if (Status == EFI_SUCCESS) {
      return Status;
    }
  }
  return EFI_NOT_FOUND;
}


/**
  Get default SKUID variable from variable default store.

  @param[in]  VariableName  Name of the variable to be found
  @param[in]  VendorGuid    Vendor GUID to be found.
  @param[out] PtrTrack      Variable Track Pointer structure that contains Variable Information.

  @retval EFI_SUCCESS   Find variable in variable default store successfully.
  @retval EFI_NOT_FOUND Cannot find variable in variable default store.
**/
STATIC
EFI_STATUS
GetDefaultSkuIdDefaultVariableByLifetime (
  IN CONST  CHAR16            *VariableName,
  IN CONST  EFI_GUID          *VendorGuid,
  OUT VARIABLE_POINTER_TRACK  *PtrTrack
  )
{
  VOID                            *DefaultVariableStoreHeader;
  EFI_STATUS                      Status;

  PtrTrack->CurrPtr   = NULL;
  DefaultVariableStoreHeader = GetDefaultVariableDefaultStoreHeader ();
  if (DefaultVariableStoreHeader != NULL ) {
    Status = GetVariableFromSpecificDefaultRegionByLifetime (VariableName, VendorGuid, DefaultVariableStoreHeader, TRUE, PtrTrack);
    if (Status == EFI_SUCCESS) {
      return Status;
    }
  }
  return EFI_NOT_FOUND;
}

/**
  Get current SKUID variable variable from variable default store.

  @param[in]  VariableName  Name of the variable to be found
  @param[in]  VendorGuid    Vendor GUID to be found.
  @param[out]   PtrTrack      Variable Track Pointer structure that contains Variable Information.

  @retval EFI_SUCCESS   Find variable in variable default store successfully.
  @retval EFI_NOT_FOUND Cannot find variable in variable default store.
**/
STATIC
EFI_STATUS
GetCurrentSkuIdDefaultVariableByLifetime (
  IN CONST  CHAR16            *VariableName,
  IN CONST  EFI_GUID          *VendorGuid,
  OUT VARIABLE_POINTER_TRACK  *PtrTrack
  )
{
  VOID                            *VariableStoreHeader;
  EFI_STATUS                      Status;

  PtrTrack->CurrPtr   = NULL;
  VariableStoreHeader = GetCurrentVariableDefaultStoreHeader ();
  if (VariableStoreHeader != NULL) {
    Status = GetVariableFromSpecificDefaultRegionByLifetime (VariableName, VendorGuid, VariableStoreHeader, TRUE, PtrTrack);
    if (Status == EFI_SUCCESS) {
      return Status;
    }
  }
  return EFI_NOT_FOUND;
}

/**
  Get first current SKUID variable from variable default store by life type.

  @param[out]  PtrTrack    Variable Track Pointer structure that contains Variable Information.

  @retval EFI_SUCCESS   Find variable in variable default store successfully.
  @retval EFI_NOT_FOUND Cannot find variable in variable default store.
**/
STATIC
EFI_STATUS
GetCurrentSkuIdFirstDefaultVariableByLifetime (
  OUT  VARIABLE_POINTER_TRACK     *VariableTrack
  )
{
  VARIABLE_HEADER                 *Variable;
  VARIABLE_POINTER_TRACK          TempVariableTrack;
  VOID                            *VariableStoreHeader;

  VariableTrack->CurrPtr   = NULL;
  VariableStoreHeader = GetCurrentVariableDefaultStoreHeader ();
  if (VariableStoreHeader == NULL) {
    return EFI_NOT_FOUND;
  }
  VariableTrack->StartPtr = (VARIABLE_HEADER *) ((UINT8 *) VariableStoreHeader + GetVariableStoreHeaderSize ());
  VariableTrack->EndPtr   = (VARIABLE_HEADER *) ((UINT8 *) VariableStoreHeader + GetVariableStoreSize (VariableStoreHeader));
  Variable                = VariableTrack->StartPtr;
  while ((GetNextVariablePtr (Variable) <= VariableTrack->EndPtr) && IsValidVariableHeader (Variable)) {
    if (Variable->State == VAR_ADDED && !VariableInVariableStore (GET_VARIABLE_NAME_PTR (Variable), &Variable->VendorGuid) &&
        GetDefaultSkuIdDefaultVariableByLifetime (GET_VARIABLE_NAME_PTR (Variable), &Variable->VendorGuid, &TempVariableTrack) != EFI_SUCCESS ) {
      VariableTrack->CurrPtr = Variable;
      return EFI_SUCCESS;
    }
    Variable = GetNextVariablePtr (Variable);
  }
  VariableTrack->StartPtr = NULL;
  VariableTrack->EndPtr   = NULL;
  return EFI_NOT_FOUND;

}

/**
  This code Finds the Next available variable in variable default region.

  @param[in, out] VariableNameSize Size of the variable.
  @param[in, out] VariableName     Pointer to variable name.
  @param[in, out] VendorGuid       Variable Vendor Guid.

  @retval EFI_SUCCESS              Get variable successfully.
  @retval EFI_NOT_FOUND            Not found.
**/
EFI_STATUS
GetNextDefaultVariableNameByLifetime (
  IN OUT  UINTN             *VariableNameSize,
  IN OUT  CHAR16            *VariableName,
  IN OUT  EFI_GUID          *VendorGuid
  )
{
  EFI_STATUS              Status;
  VARIABLE_POINTER_TRACK  VariableTrack;
  VARIABLE_POINTER_TRACK  TempVariableTrack;
  UINTN                   VarNameSize;
  VARIABLE_HEADER         *Variable;
  BOOLEAN                 InDefaultSku;


  //
  // Need find the default variable from default SKUID region and current SKUID region.
  // 1. Find all default variables from default SKUID region. (the variable must doesn't exist in variable region)
  // 2. Find all default variables from current SKUID region. (the variable must doesn't exist in variable region
  //    and default SKUID variable region)
  //
  InDefaultSku          = TRUE;
  VariableTrack.CurrPtr = NULL;
  Status = GetDefaultSkuIdDefaultVariableByLifetime (VariableName, VendorGuid, &VariableTrack);
  if (Status != EFI_SUCCESS || VariableTrack.CurrPtr == NULL) {
    if (LibPcdGetSku ()  == 0) {
      return EFI_INVALID_PARAMETER;
    }
    Status = GetCurrentSkuIdDefaultVariableByLifetime (VariableName, VendorGuid, &VariableTrack);
    if (Status != EFI_SUCCESS || VariableTrack.CurrPtr == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    InDefaultSku = FALSE;
  }

  //
  // Only need try to find next variable if first character isn't 0.
  // If first character is 0 and GetDefaultSkuIdDefaultVariableByLifetime () or
  // GetCurrentSkuIdDefaultVariableByLifetime return EFI_SUCCESS,
  // It means we already find first valid variable.
  //
  if (VariableName[0] != 0) {
    VariableTrack.CurrPtr = GetNextVariablePtr (VariableTrack.CurrPtr);
    if (VariableTrack.CurrPtr == NULL) {
      return EFI_NOT_FOUND;
    }

    Variable          = VariableTrack.CurrPtr;
    VariableTrack.CurrPtr  = NULL;
    while ((GetNextVariablePtr (Variable) <= VariableTrack.EndPtr) && IsValidVariableHeader (Variable)) {
      if (Variable->State == VAR_ADDED && !VariableInVariableStore (GET_VARIABLE_NAME_PTR (Variable), &Variable->VendorGuid) &&
         (InDefaultSku || GetDefaultSkuIdDefaultVariableByLifetime (GET_VARIABLE_NAME_PTR (Variable), &Variable->VendorGuid, &TempVariableTrack) != EFI_SUCCESS) &&
         !(VariableAtRuntime () && (Variable->Attributes & EFI_VARIABLE_RUNTIME_ACCESS) != EFI_VARIABLE_RUNTIME_ACCESS)) {
        VariableTrack.CurrPtr = Variable;
        break;
      }
      Variable = GetNextVariablePtr (Variable);
    }

    if (VariableTrack.CurrPtr == NULL ) {
      //
      // Once all of the variables in default SKUID region are found, try to find first
      // variable in current SKUID region.
      //
      if (!InDefaultSku || LibPcdGetSku () == 0) {
        return EFI_NOT_FOUND;
      }
      Status = GetCurrentSkuIdFirstDefaultVariableByLifetime (&VariableTrack);
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }
  }

  VarNameSize = NameSizeOfVariable (VariableTrack.CurrPtr);
  if (VarNameSize <= *VariableNameSize) {
    CopyMem (VariableName, GET_VARIABLE_NAME_PTR (VariableTrack.CurrPtr), VarNameSize);
    CopyMem (VendorGuid, &VariableTrack.CurrPtr->VendorGuid, sizeof (EFI_GUID));
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_BUFFER_TOO_SMALL;
  }
  *VariableNameSize = VarNameSize;
  return Status;

}

/**
  This code Finds the first available variable in variable default region.

  @param[in, out] VariableNameSize Size of the variable.
  @param[in, out] VariableName     Pointer to variable name.
  @param[in, out] VendorGuid       Variable Vendor Guid.

  @retval EFI_SUCCESS              Get variable successfully.
  @retval EFI_NOT_FOUND            Not found.
**/
EFI_STATUS
GetFirstDefaultVariableNameByLifetime (
  IN OUT  UINTN             *VariableNameSize,
  IN OUT  CHAR16            *VariableName,
  IN OUT  EFI_GUID          *VendorGuid
  )
{
  CHAR16          SavedChar;
  EFI_STATUS      Status;


  SavedChar = VariableName[0];
  VariableName[0] = 0;
  Status = GetNextDefaultVariableNameByLifetime (VariableNameSize, VariableName, VendorGuid);
  if (EFI_ERROR (Status)) {
    VariableName[0] = SavedChar;
  }
  return Status;
}

/**
  Function to check the input address is whether in variable default region.

  @param[in] Address   Input physical address to check.

  @retval TRUE         The address is in the variable default region.
  @retval FALSE        The address isn't in the variable default region.
**/
BOOLEAN
AddressInVariableDefaultRegion (
  IN    PHYSICAL_ADDRESS   Address
  )
{

  if (Address >= (UINTN) mVariableModuleGlobal->VariableDefaultCache &&
      Address < (UINTN) mVariableModuleGlobal->VariableDefaultCache + mVariableModuleGlobal->VariableDefaultSize) {
    return TRUE;
  }
  return FALSE;
}
