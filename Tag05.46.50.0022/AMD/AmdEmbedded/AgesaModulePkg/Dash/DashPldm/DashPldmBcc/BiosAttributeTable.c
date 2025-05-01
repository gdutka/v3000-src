/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include "DashPldmBcc.h"
#include "BiosAttributeTable.h"
#include "BiosAttributePendingValueTable.h"

/**
  Append an atomic data piece to an expandable data buffer.

  @param [in] BiosAttributeTableBuffer  Pointer to BIOS Attribute Table buffer.
  @param [in] Data                      Pointer to the data piece
  @param [in] DataLength                Byte-size of the data piece.

  @retval EFI_SUCCESS                   Successful
  @retval EFI_INVALID_PARAMETER         Incorrect parameter(s).
  @retval EFI_OUT_OF_RESOURCES          Unable to allocate (more) memory.

**/
EFI_STATUS
EFIAPI
AppendBiosAttributeTableData (
  IN VOID   *ExpandableBuffer,
  IN VOID   *Data,
  IN UINT16 DataLength
  )
{
  if (ExpandableBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (DataLength == 0) {
    return EFI_SUCCESS;
  }

  BIOS_ATTRIBUTE_TABLE_BUFFER    *BiosAttributeTableBuffer = (BIOS_ATTRIBUTE_TABLE_BUFFER*) ExpandableBuffer;

  UINT32 OriginBufferSize = BiosAttributeTableBuffer->BiosAttributeTableEntries == NULL ? \
                              0: BiosAttributeTableBuffer->BufferSize;

  if (OriginBufferSize < ( \
        BiosAttributeTableBuffer->AppendedDataSize + DataLength + \
        PaddingCount4 (BiosAttributeTableBuffer->AppendedDataSize + DataLength))
  ) {
    // The "BIOS Attribute Table" buffer is implicitly:
    //   (1) zero-ed, and/or
    //   (2) cloned from the original copy when it has been allocated.
    BiosAttributeTableBuffer->BiosAttributeTableEntries = \
      (BIOS_ATTRIBUTE_TABLE_ENTRY*) ReallocatePool ( \
                                   OriginBufferSize, \
                                   OriginBufferSize+GRANULE_BIOS_TABLE_BUFFER_SIZE, \
                                   BiosAttributeTableBuffer->BiosAttributeTableEntries \
                                   );
    if (BiosAttributeTableBuffer->BiosAttributeTableEntries == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    BiosAttributeTableBuffer->BufferSize += GRANULE_BIOS_TABLE_BUFFER_SIZE;
  }

  // Append the data trunk.
  BIOS_ATTRIBUTE_TABLE_ENTRY *BiosAttributeTableEntriesTail = (BIOS_ATTRIBUTE_TABLE_ENTRY*) \
    ((UINT8*)(BiosAttributeTableBuffer->BiosAttributeTableEntries) + \
      BiosAttributeTableBuffer->AppendedDataSize);
  if (Data != NULL) {
    CopyMem (BiosAttributeTableEntriesTail, Data, DataLength);
  }
  else {
    ZeroMem (BiosAttributeTableEntriesTail, DataLength);
  }
  BiosAttributeTableBuffer->AppendedDataSize += DataLength;


  return EFI_SUCCESS;
}


/**
  Calculate and add CRC32 to the end of BIOS Attribute Table.

  @param [in] BiosAttributeTableBuffer     Pointer to BIOS Attribute Table buffer.

**/
EFI_STATUS
EFIAPI
Crc32BiosAttributeTable (
    IN VOID   *ExpandableBuffer
  )
{
  IN BIOS_ATTRIBUTE_TABLE_BUFFER *BiosAttributeTableBuffer = (BIOS_ATTRIBUTE_TABLE_BUFFER*) ExpandableBuffer;

  if ((BiosAttributeTableBuffer == NULL) || \
      (BiosAttributeTableBuffer->BiosAttributeTableEntries == NULL) || \
      (BiosAttributeTableBuffer->AppendedDataSize == 0)
  ) {
    return EFI_INVALID_PARAMETER;
  }

  // Round off the 4-byte aligned buffer with 0 or 1 or 2 or 3-byte padding.
  EFI_STATUS Status = AppendBiosAttributeTableData (
                        BiosAttributeTableBuffer,
                        NULL,
                        PaddingCount4 (BiosAttributeTableBuffer->AppendedDataSize)
                      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  UINT32 Crc32 = CalculateCrc32 (
                    BiosAttributeTableBuffer->BiosAttributeTableEntries,
                    BiosAttributeTableBuffer->AppendedDataSize
                    );
  return AppendBiosAttributeTableData (BiosAttributeTableBuffer, &Crc32, sizeof (Crc32));
}

/**
  Append one record of the BIOS Attribute Table Data.

  @param [in] BiosAttributeTableBuffer  Pointer to BIOS Attribute Table buffer.
  @param [in] AttributeHandle           BIOS Attribute Handle
  @param [in] AttributeType             BIOS Attribute Type (enum value)
  @param [in] AttributeNameHandle       BIOS Attribute Handle
  @param [in] ...                       specific data fields.

  @retval EFI_SUCCESS                   Successful
  @retval EFI_INVALID_PARAMETER         Incorrect parameter(s).
  @retval EFI_OUT_OF_RESOURCES          Unable to allocate memory.

**/
EFI_STATUS
EFIAPI
AppendBiosAttributeTable (
  IN BIOS_ATTRIBUTE_TABLE_BUFFER   *BiosAttributeTableBuffer,
  IN UINT16 AttributeHandle,
  IN UINT8  AttributeType,
  IN UINT16 AttributeNameHandle,
  OPTIONAL IN ...
  )
{
  if (BiosAttributeTableBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  EFI_STATUS Status;
  BIOS_ATTRIBUTE_TABLE_ENTRY BiosAttrubuteData;
  UINT8   NumberOfPossibleValues, i;
  UINT16  PossibleValueStringHandle;
  UINT8   NumberOfDefaultValues;
  UINT16  DefaultValueStringHandleIndex;
  UINT8   StringType;
  UINT16  MinimumStringLength;
  UINT16  MaximumStringLength;
  UINT16  DefaultStringLength;
  VOID*   DefaultString;
  UINT8   PasswordEncodingType;
  UINT16  MinimumPasswordLength;
  UINT16  MaximumPasswordLength;
  UINT16  DefaultPasswordLength;
  VOID    *DefaultPassword;
  UINT64  LowerBound;
  UINT64  UpperBound;
  UINT32  ScalarIncrement;
  UINT64  DefaultValue;
  UINT8   BootConfigType;
  UINT8   SupportedOrderedAndFailThroughModes;
  UINT8   MinimumNumberOfBootSourceSettings;
  UINT8   MaximumNumberOfBootSourceSettings;
  UINT8   NumberOfPossibleBootSourceSettings;
  UINT16  *PossibleBootSourceStringHandle;
  UINT16  CollectionNameStringHandle;
  UINT8   MaximumNumberOfAttributes;
  UINT8   CollectionType;
  UINT8   NumberOfPossibleBIOSConfigurations;
  UINT16  PossibleBIOSConfigStringHandle;


  BiosAttrubuteData.BiosAttributeHandle = AttributeHandle;
  BiosAttrubuteData.BiosAttributeType = AttributeType;
  BiosAttrubuteData.BiosAttributeNameHandle = AttributeNameHandle;
  Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &BiosAttrubuteData, sizeof (BiosAttrubuteData));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  VA_LIST VaArg;

  VA_START (VaArg, AttributeNameHandle);
  switch (AttributeType) {
  // DSP0247, Table 6 - Specific BIOS Attribute Table Fields for BIOSEnumeration and BIOSEnumerationReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_TYPE_BIOSEnumeration:
  case PLDM_ATTRIBUTE_TYPE_BIOSEnumerationReadOnly:
    NumberOfPossibleValues = VA_ARG (VaArg, UINT8);
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &NumberOfPossibleValues, sizeof (NumberOfPossibleValues));
    if (EFI_ERROR (Status)) break;
    for (i=0; i<NumberOfPossibleValues; i++) {
      PossibleValueStringHandle = VA_ARG (VaArg, UINT16);
      Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &PossibleValueStringHandle, sizeof (PossibleValueStringHandle));
      if (EFI_ERROR (Status)) break;
    }
    if (EFI_ERROR (Status)) break;

    NumberOfDefaultValues = VA_ARG (VaArg, UINT8);
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &NumberOfDefaultValues, sizeof (NumberOfDefaultValues));
    if (EFI_ERROR (Status)) break;
    if (NumberOfDefaultValues == 0) break;
    for (i=0; i<NumberOfDefaultValues; i++) {
      DefaultValueStringHandleIndex = VA_ARG (VaArg, UINT16);
      Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &DefaultValueStringHandleIndex, sizeof (DefaultValueStringHandleIndex));
      if (EFI_ERROR (Status)) break;
    }
    break;

  // DSP0247, Table 7 - Specific BIOS Attribute Table Fields for BIOSString and BIOSStringReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_TYPE_BIOSString:
  case PLDM_ATTRIBUTE_TYPE_BIOSStringReadOnly:
    StringType = VA_ARG (VaArg, UINT8);
    MinimumStringLength = VA_ARG (VaArg, UINT16);
    MaximumStringLength = VA_ARG (VaArg, UINT16);
    DefaultStringLength = VA_ARG (VaArg, UINT16);
    DefaultString = VA_ARG (VaArg, VOID*);

    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &StringType, sizeof (StringType));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &MinimumStringLength, sizeof (MinimumStringLength));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &MaximumStringLength, sizeof (MaximumStringLength));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &DefaultStringLength, sizeof (DefaultStringLength));
    if (EFI_ERROR (Status)) break;
    if (DefaultStringLength > 0) {
      Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, DefaultString, DefaultStringLength);
    }
    break;

  // DSP0247, Table 8 - Specific BIOS Attribute Table Fields for BIOSPassword and BIOSPasswordReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_TYPE_BIOSPassword:
  case PLDM_ATTRIBUTE_TYPE_BIOSPasswordReadOnly:
    PasswordEncodingType = VA_ARG (VaArg, UINT8);
    MinimumPasswordLength = VA_ARG (VaArg, UINT16);
    MaximumPasswordLength = VA_ARG (VaArg, UINT16);
    DefaultPasswordLength = VA_ARG (VaArg, UINT16);
    DefaultPassword = VA_ARG (VaArg, VOID*);

    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &PasswordEncodingType, sizeof (PasswordEncodingType));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &MinimumPasswordLength, sizeof (MinimumPasswordLength));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &MaximumPasswordLength, sizeof (MaximumPasswordLength));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &DefaultPasswordLength, sizeof (DefaultPasswordLength));
    if (EFI_ERROR (Status)) break;
    if (DefaultPasswordLength > 0) {
      Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, DefaultPassword, DefaultPasswordLength);
    }
    break;

  // DSP0247, Table 9 - Specific BIOS Attribute Table Fields for BIOSInteger and BIOSIntegerReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_TYPE_BIOSInteger:
  case PLDM_ATTRIBUTE_TYPE_BIOSIntegerReadOnly:
    LowerBound = VA_ARG (VaArg, UINT64);
    UpperBound = VA_ARG (VaArg, UINT64);
    ScalarIncrement = VA_ARG (VaArg, UINT32);
    DefaultValue = VA_ARG (VaArg, UINT64);

    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &LowerBound, sizeof (LowerBound));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &UpperBound, sizeof (UpperBound));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &ScalarIncrement, sizeof (ScalarIncrement));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &DefaultValue, sizeof (DefaultValue));
    break;

  // DSP0247, Table 10 - Specific BIOS Attribute Table Fields for BIOSBootConfigSetting and BIOSBootConfigSettingReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_TYPE_BIOSBootConfigSetting:
  case PLDM_ATTRIBUTE_TYPE_BIOSBootConfigSettingReadOnly:
    BootConfigType = VA_ARG (VaArg, UINT8);
    SupportedOrderedAndFailThroughModes = VA_ARG (VaArg, UINT8);
    MinimumNumberOfBootSourceSettings = VA_ARG (VaArg, UINT8);
    MaximumNumberOfBootSourceSettings = VA_ARG (VaArg, UINT8);
    NumberOfPossibleBootSourceSettings = VA_ARG (VaArg, UINT8);

    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &BootConfigType, sizeof (BootConfigType));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &SupportedOrderedAndFailThroughModes, sizeof (SupportedOrderedAndFailThroughModes));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &MinimumNumberOfBootSourceSettings, sizeof (MinimumNumberOfBootSourceSettings));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &MaximumNumberOfBootSourceSettings, sizeof (MaximumNumberOfBootSourceSettings));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &NumberOfPossibleBootSourceSettings, sizeof (NumberOfPossibleBootSourceSettings));
    if (EFI_ERROR (Status)) break;

    if (NumberOfPossibleBootSourceSettings == 0) break;

    PossibleBootSourceStringHandle = VA_ARG (VaArg, UINT16*);
    for (i=0; i<NumberOfPossibleBootSourceSettings; i++) {
      Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, PossibleBootSourceStringHandle+i, sizeof (*PossibleBootSourceStringHandle));
      if (EFI_ERROR (Status)) break;
    }
    break;

  // DSP0247, Table 11 - Specific BIOS Attribute Table Fields for BIOSCollection and BIOSCollectionReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_TYPE_BIOSCollection:
  case PLDM_ATTRIBUTE_TYPE_BIOSCollectionReadOnly:
    CollectionNameStringHandle = VA_ARG (VaArg, UINT16);
    MaximumNumberOfAttributes = VA_ARG (VaArg, UINT8);
    CollectionType = VA_ARG (VaArg, UINT8);

    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &CollectionNameStringHandle, sizeof (CollectionNameStringHandle));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &MaximumNumberOfAttributes, sizeof (MaximumNumberOfAttributes));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &CollectionType, sizeof (CollectionType));
    break;

  // DSP0247, Table 12 - Specific BIOS Attribute Table Fields for BIOSConfigSet and BIOSConfigSetReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_TYPE_BIOSConfigSet:
  case PLDM_ATTRIBUTE_TYPE_BIOSConfigSetReadOnly:
    NumberOfPossibleBIOSConfigurations = VA_ARG (VaArg, UINT8);
    Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &NumberOfPossibleBIOSConfigurations, sizeof (NumberOfPossibleBIOSConfigurations));
    if (EFI_ERROR (Status)) break;

    for(i=0; i<NumberOfPossibleBIOSConfigurations; i++) {
      PossibleBIOSConfigStringHandle = VA_ARG (VaArg, UINT16);
      Status = AppendBiosAttributeTableData (BiosAttributeTableBuffer, &PossibleBIOSConfigStringHandle, sizeof (PossibleBIOSConfigStringHandle));
      if (EFI_ERROR (Status)) break;
    }
    break;

  default:
    Status = EFI_INVALID_PARAMETER;
  }
  VA_END (VaArg);

  return Status;
}


/**
  Set up the BIOS Attribute Tables.

  @param [in] BiosAttributeTableBuffer  Pointer to the BIOS Attribute Table buffer.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful
**/
EFI_STATUS
EFIAPI
SetupBiosAttributeTables (
  IN BIOS_ATTRIBUTE_TABLE_BUFFER *BiosAttributeTableBuffer
  )
{
  EFI_STATUS    Status;
  UINT16        *BootOrderList = NULL;
  UINTN         BootOrderListSize;

  // Get Boot Order List.
  Status = GetEfiGlobalVariable2 (L"BootOrder", (VOID **) &BootOrderList, &BootOrderListSize);
  if (EFI_ERROR (Status) || (BootOrderList == NULL) || (BootOrderListSize == 0)) {
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  for (UINT8 i=0; i<(BootOrderListSize/sizeof(UINT16)); i++) {
    BootOrderList[i] += PLDM_BIOS_STRING_HANDLE_EFI_BOOT_OPTION;
  }
  Status = AppendBiosAttributeTable (
             BiosAttributeTableBuffer,
             PLDM_BIOS_ATTRIBUTE_HANDLE_BOOT_CONFIG_SETTING,
             PLDM_ATTRIBUTE_TYPE_BIOSBootConfigSetting,
             PLDM_BIOS_STRING_HANDLE_BOOT_CONFIG_SETTING,
             BIOS_ATTRIBUTE_BOOT_CONFIG_TYPE_Default,
             BIOS_ATTRIBUTE_ORDERANDFAILTHROUGHMODE_OrderedAndFailThrough,
             0,                                     //MinimumNumberOfBootSourceSettings
             0xFF,                                  //MaximumNumberOfBootSourceSettings
             BootOrderListSize / sizeof(UINT16),    //NumberOfPendingBootSourceSettings
             BootOrderList                          //BootSourceStringHandleIndex[...]
             );

Exit:

  if (BootOrderList != NULL) {
    FreePool (BootOrderList);
    BootOrderList = NULL;
  }
  return Status;
}

/**
  Get the buffer's address and size of the BIOS Attribute Table Data

  @param [in] BiosAttributeTableBuffer     Pointer to BIOS Attribute Table buffer.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful

**/
EFI_STATUS
EFIAPI
GetBiosAttributeTableBuffer (
  IN BIOS_ATTRIBUTE_TABLE_BUFFER *BiosAttributeTableBuffer,
  IN OUT VOID **Buffer,
  IN OUT UINT32 *BufferSize
  )
{
  if ((Buffer == NULL) || (BufferSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  *Buffer = BiosAttributeTableBuffer->BiosAttributeTableEntries;
  *BufferSize = BiosAttributeTableBuffer->AppendedDataSize;
  if ((*Buffer == NULL) || (*BufferSize == 0)) {
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**
  Free any allocated buffer for BIOS Attribute Table Data.

  @param [in] BiosAttributeTableBuffer     Pointer to BIOS Attribute Table buffer.

**/
VOID
EFIAPI
CleanUpBiosAttributeTable (
  IN BIOS_ATTRIBUTE_TABLE_BUFFER *BiosAttributeTableBuffer
  )
{
  BiosAttributeTableBuffer->BufferSize = 0;
  BiosAttributeTableBuffer->AppendedDataSize = 0;
  if (BiosAttributeTableBuffer->BiosAttributeTableEntries != NULL) {
    FreePool (BiosAttributeTableBuffer->BiosAttributeTableEntries);
    BiosAttributeTableBuffer->BiosAttributeTableEntries = NULL;
  }
}
