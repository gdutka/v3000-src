/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include "MpmPldmBcc.h"
#include "BiosAttributeValueTable.h"
#include "BiosAttributePendingValueTable.h"
#include <Protocol/AmdMpmBccBiosPasswordProtocol.h>

#define FILECODE MPM_MPMPLDM_MPMPLDMBCC_BIOSATTRIBUTEVALUETABLE_FILECODE

/**
  Append an atomic data piece to an expandable data buffer.

  @param [in] BiosAttributeValueTableBuffer  Pointer to BIOS Attribute Table buffer.
  @param [in] Data                      Pointer to the data piece
  @param [in] DataLength                Byte-size of the data piece.

  @retval EFI_SUCCESS                   Successful
  @retval EFI_INVALID_PARAMETER         Incorrect parameter(s).
  @retval EFI_OUT_OF_RESOURCES          Unable to allocate (more) memory.

**/
EFI_STATUS
EFIAPI
AppendBiosAttributeValueTableData (
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

  BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER    *BiosAttributeValueTableBuffer = (BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER*) ExpandableBuffer;

  UINT32 OriginBufferSize = BiosAttributeValueTableBuffer->BiosAttributeValueTableEntries == NULL ? \
                              0: BiosAttributeValueTableBuffer->BufferSize;

  if (OriginBufferSize < ( \
        BiosAttributeValueTableBuffer->AppendedDataSize + DataLength + \
        PaddingCount4 (BiosAttributeValueTableBuffer->AppendedDataSize + DataLength))
  ) {
    // The "BIOS Attribute Table" buffer is implicitly:
    //   (1) zero-ed, and/or
    //   (2) cloned from the original copy when it has been allocated.
    BiosAttributeValueTableBuffer->BiosAttributeValueTableEntries = \
      (BIOS_ATTRIBUTE_VALUE_TABLE_ENTRY*) ReallocatePool ( \
                                   OriginBufferSize, \
                                   OriginBufferSize+GRANULE_BIOS_TABLE_BUFFER_SIZE, \
                                   BiosAttributeValueTableBuffer->BiosAttributeValueTableEntries \
                                   );
    if (BiosAttributeValueTableBuffer->BiosAttributeValueTableEntries == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    BiosAttributeValueTableBuffer->BufferSize += GRANULE_BIOS_TABLE_BUFFER_SIZE;
  }

  // Append the data trunk.
  BIOS_ATTRIBUTE_VALUE_TABLE_ENTRY *BiosAttributeValueTableEntriesTail = (BIOS_ATTRIBUTE_VALUE_TABLE_ENTRY*) \
    ((UINT8*)(BiosAttributeValueTableBuffer->BiosAttributeValueTableEntries) + \
      BiosAttributeValueTableBuffer->AppendedDataSize);
  if (Data != NULL) {
    CopyMem (BiosAttributeValueTableEntriesTail, Data, DataLength);
  }
  else {
    ZeroMem (BiosAttributeValueTableEntriesTail, DataLength);
  }
  BiosAttributeValueTableBuffer->AppendedDataSize += DataLength;


  return EFI_SUCCESS;
}


/**
  Calculate and add CRC32 to the end of BIOS Attribute Table.

  @param [in] BiosAttributeValueTableBuffer     Pointer to BIOS Attribute Table buffer.

**/
EFI_STATUS
EFIAPI
Crc32BiosAttributeValueTable (
    IN VOID   *ExpandableBuffer
  )
{
  IN BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER *BiosAttributeValueTableBuffer = (BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER*) ExpandableBuffer;

  if ((BiosAttributeValueTableBuffer == NULL) || \
      (BiosAttributeValueTableBuffer->BiosAttributeValueTableEntries == NULL) || \
      (BiosAttributeValueTableBuffer->AppendedDataSize == 0)
  ) {
    return EFI_INVALID_PARAMETER;
  }

  // Round off the 4-byte aligned buffer with 0 or 1 or 2 or 3-byte padding.
  EFI_STATUS Status = AppendBiosAttributeValueTableData (
                        BiosAttributeValueTableBuffer,
                        NULL,
                        PaddingCount4 (BiosAttributeValueTableBuffer->AppendedDataSize)
                      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  UINT32 Crc32 = CalculateCrc32 (
                    BiosAttributeValueTableBuffer->BiosAttributeValueTableEntries,
                    BiosAttributeValueTableBuffer->AppendedDataSize
                    );
  return AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &Crc32, sizeof (Crc32));
}

/**
  Append one record of the BIOS Attribute Table Data.

  @param [in] BiosAttributeValueTableBuffer  Pointer to BIOS Attribute Table buffer.
  @param [in] AttributeHandle           BIOS Attribute Handle
  @param [in] AttributeType             BIOS Attribute Type (enum value)
  @param [in] ...                       specific data fields.

  @retval EFI_SUCCESS                   Successful
  @retval EFI_INVALID_PARAMETER         Incorrect parameter(s).
  @retval EFI_OUT_OF_RESOURCES          Unable to allocate memory.

**/
EFI_STATUS
EFIAPI
AppendBiosAttributeValueTable (
  IN BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER   *BiosAttributeValueTableBuffer,
  IN UINT16 AttributeHandle,
  IN UINT8  AttributeType,
  OPTIONAL IN ...
  )
{
  UINT8 NumberOfCurrentValues;
  UINT16 CurrentValueStringHandleIndex;
  UINT16 CurrentStringLength;
  VOID *CurrentString;
  UINT16 CurrentPasswordLength;
  VOID *CurrentPassword;
  UINT64 CurrentValue;
  UINT8 BootConfigType;
  UINT8 OrderAndFailThroughMode;
  UINT8 NumberOfBootSourceSettings;
  UINT8 *BootSourceStringHandleIndex;
  UINT8 NumberOfAttributes;
  UINT8 _AttributeHandle;
  UINT8 CurrentConfigSetStringHandleIndex;

  CurrentValueStringHandleIndex = 0;
  CurrentStringLength = 0;
  CurrentString = 0;
  CurrentPasswordLength = 0;
  CurrentPassword = 0;
  CurrentValue = 0;
  BootConfigType = 0;
  OrderAndFailThroughMode = 0;
  NumberOfBootSourceSettings = 0;
  BootSourceStringHandleIndex = 0;
  NumberOfAttributes = 0;
  _AttributeHandle = 0;
  CurrentConfigSetStringHandleIndex = 0;

  if (BiosAttributeValueTableBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  EFI_STATUS Status;
  BIOS_ATTRIBUTE_VALUE_TABLE_ENTRY BiosAttrubuteValueData;

  BiosAttrubuteValueData.BiosAttributeHandle = AttributeHandle;
  BiosAttrubuteValueData.BiosAttributeType = AttributeType;
  Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &BiosAttrubuteValueData, sizeof (BiosAttrubuteValueData));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  VA_LIST VaArg;

  VA_START (VaArg, AttributeType);
  switch (AttributeType) {
  // DSP0247, Table 15 - Specific BIOS Attribute Value Table Fields for BIOSEnumeration and BIOSEnumerationReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSEnumeration:
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSEnumerationReadOnly:
    NumberOfCurrentValues = VA_ARG (VaArg, UINT8);
    Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &NumberOfCurrentValues, sizeof (NumberOfCurrentValues));
    if (EFI_ERROR (Status)) break;
    for (UINT8 i=0; i<NumberOfCurrentValues; i++) {
      CurrentValueStringHandleIndex = VA_ARG (VaArg, UINT8);
      Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &CurrentValueStringHandleIndex, sizeof (CurrentValueStringHandleIndex));
      if (EFI_ERROR (Status)) break;
    }
    break;

  // DSP0247, Table 16 - Specific BIOS Attribute Value Table Fields for BIOSString and BIOSStringReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSString:
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSStringReadOnly:
    CurrentStringLength = VA_ARG (VaArg, UINT16);
    CurrentString = VA_ARG (VaArg, VOID*);

    Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &CurrentStringLength, sizeof (CurrentStringLength));
    if (EFI_ERROR (Status)) break;
    if (CurrentString > 0) {
      Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, CurrentString, CurrentStringLength);
    }
    break;

  // DSP0247, Table 17 - Specific BIOS Attribute Value Table Fields for BIOSPassword and BIOSPasswordReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSPassword:
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSPasswordReadOnly:
    CurrentPasswordLength = VA_ARG (VaArg, UINT16);
    CurrentPassword = VA_ARG (VaArg, VOID*);

    Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &CurrentPasswordLength, sizeof (CurrentPasswordLength));
    if (EFI_ERROR (Status)) break;
    if (CurrentPasswordLength > 0) {
      Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, CurrentPassword, CurrentPasswordLength);
    }
    break;

  // DSP0247, Table 18 - Specific BIOS Attribute Value Table Fields for BIOSInteger and BIOSIntegerReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSInteger:
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSIntegerReadOnly:
    CurrentValue = VA_ARG (VaArg, UINT64);

    Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &CurrentValue, sizeof (CurrentValue));
    break;

  // DSP0247, Table 19 - Specific BIOS Attribute Value Table Fields for BIOSBootConfigSetting and BIOSBootConfigSettingReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSBootConfigSetting:
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSBootConfigSettingReadOnly:
    BootConfigType = VA_ARG (VaArg, UINT8);
    OrderAndFailThroughMode = VA_ARG (VaArg, UINT8);
    NumberOfBootSourceSettings = VA_ARG (VaArg, UINT8);

    Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &BootConfigType, sizeof (BootConfigType));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &OrderAndFailThroughMode, sizeof (OrderAndFailThroughMode));
    if (EFI_ERROR (Status)) break;
    Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &NumberOfBootSourceSettings, sizeof (NumberOfBootSourceSettings));
    if (EFI_ERROR (Status)) break;

    if (NumberOfBootSourceSettings == 0) break;

    BootSourceStringHandleIndex = VA_ARG (VaArg, UINT8*);
    for (UINT8 i=0; i<NumberOfBootSourceSettings; i++) {
      Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, BootSourceStringHandleIndex+i, sizeof (*BootSourceStringHandleIndex));
      if (EFI_ERROR (Status)) break;
    }
    break;

  // DSP0247, Table 20 - Specific BIOS Attribute Value Table Fields for BIOSCollection and BIOSCollectionReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSCollection:
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSCollectionReadOnly:
    NumberOfAttributes = VA_ARG (VaArg, UINT8);

    Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &NumberOfAttributes, sizeof (NumberOfAttributes));
    if (EFI_ERROR (Status)) break;

    for (UINT8 i=0; i<NumberOfAttributes; i++) {
      _AttributeHandle = VA_ARG (VaArg, UINT8);
      Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &_AttributeHandle, sizeof (_AttributeHandle));
      if (EFI_ERROR (Status)) break;
    }
    break;

  // DSP0247, Table 21 - Specific BIOS Attribute Value Table Fields for BIOSConfigSet and BIOSConfigSetReadOnly Types
  // WARNING: no error checking on the variable-arguments' sanity.
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSConfigSet:
  case PLDM_ATTRIBUTE_VALUE_TYPE_BIOSConfigSetReadOnly:
    CurrentConfigSetStringHandleIndex = VA_ARG (VaArg, UINT8);
    Status = AppendBiosAttributeValueTableData (BiosAttributeValueTableBuffer, &CurrentConfigSetStringHandleIndex, sizeof (CurrentConfigSetStringHandleIndex));
    break;

  default:
    Status = EFI_INVALID_PARAMETER;
  }
  VA_END (VaArg);

  return Status;
}


/**
  Set up the generic BIOS Attribute Table.

  @param [in] BiosAttributeValueTableBuffer     Pointer to BIOS Attribute Table buffer.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful
**/
EFI_STATUS
EFIAPI
SetupGenericBiosAttributeValueTable (
  IN OUT BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER *BiosAttributeValueTableBuffer
  )
{
  EFI_STATUS Status;
  AMD_MPM_BCC_GET_OEM_BIOS_PASSWORD_PROTOCOL *AmdMpmBccGetOemBiosPasswordInfo = NULL;
  UINT8   AdminBiosPasswordLength = MAX_BIOS_PASSWORD_LENGTH;
  UINT8   UserBiosPasswordLength = MAX_BIOS_PASSWORD_LENGTH;
  CHAR8   AmdinBiosPassword[MAX_BIOS_PASSWORD_LENGTH];
  CHAR8   UserBiosPassword[MAX_BIOS_PASSWORD_LENGTH];
  UINT8   BiosPasswordType = 0;

  ZeroMem (AmdinBiosPassword, MAX_BIOS_PASSWORD_LENGTH);
  ZeroMem (UserBiosPassword, MAX_BIOS_PASSWORD_LENGTH);
  // Locate gAmdMpmBccGetUserBiosPasswordProtocolGuid to get user password
  Status = gBS->LocateProtocol (&gAmdMpmBccGetOemBiosPasswordProtocolGuid, NULL, (VOID**)&AmdMpmBccGetOemBiosPasswordInfo);
  if (!EFI_ERROR(Status)) {
    Status = AmdMpmBccGetOemBiosPasswordInfo->AmdMpmBccGetOemBiosPassword(&AdminBiosPasswordLength,\
                                                                          &UserBiosPasswordLength,\
                                                                          AmdinBiosPassword,\
                                                                          UserBiosPassword,\
                                                                          &BiosPasswordType);
    if (!EFI_ERROR (Status)) {
      if ((BiosPasswordType & PASSWORD_ADMIN) == PASSWORD_ADMIN) {
        #if defined(PASSWORD_PROFILE_DEBUG)
        IDS_HDT_CONSOLE_PSP_TRACE ("BiosAdminPassword Length = %d\n", AdminBiosPasswordLength);
        IDS_HDT_CONSOLE_PSP_TRACE ("BiosAdminPassword = ");
        for (UINT8 i = 0; i < AdminBiosPasswordLength; i++) {
          IDS_HDT_CONSOLE_PSP_TRACE ("%x ", AmdinBiosPassword[i]);
        }
        IDS_HDT_CONSOLE_PSP_TRACE ("\n");
        #endif
        Status = AppendBiosAttributeValueTable (
                 BiosAttributeValueTableBuffer,
                 PLDM_BIOS_STRING_HANDLE_BIOS_ADMIN_PASSWORD,
                 PLDM_ATTRIBUTE_VALUE_TYPE_BIOSPassword,
                 AdminBiosPasswordLength,
                 &AmdinBiosPassword
                 );
        IDS_HDT_CONSOLE_PSP_TRACE ("AppendBiosAttributeValue BiosAdminPassword Status = %r\n", Status);
      }

      if ((BiosPasswordType & PASSWORD_USER) == PASSWORD_USER) {
        #if defined(PASSWORD_PROFILE_DEBUG)
        IDS_HDT_CONSOLE_PSP_TRACE ("BiosUserPassword Length = %d\n", UserBiosPasswordLength);
        IDS_HDT_CONSOLE_PSP_TRACE ("UserBiosPassword = ");
        for (UINT8 j = 0; j < UserBiosPasswordLength; j++) {
        IDS_HDT_CONSOLE_PSP_TRACE ("%x ", UserBiosPassword[j]);
        }
        IDS_HDT_CONSOLE_PSP_TRACE ("\n");
        #endif
        Status = AppendBiosAttributeValueTable (
                 BiosAttributeValueTableBuffer,
                 PLDM_BIOS_STRING_HANDLE_BIOS_USER_PASSWORD,
                 PLDM_ATTRIBUTE_VALUE_TYPE_BIOSPassword,
                 UserBiosPasswordLength,
                 &UserBiosPassword
                 );
        IDS_HDT_CONSOLE_PSP_TRACE ("AppendBiosAttributeValue BiosUserPassword Status = %r\n", Status);
      }
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("SetupGenericBiosAttributeValueTable: failed to locate gAmdMpmBccGetOemBiosPasswordProtocolGuid, Status = %r\n", Status);
  }

  return EFI_SUCCESS;
}

/**
  Set up the BIOS Attribute Value Tables.

  @param [in] BiosAttributeValueTableBuffer     Pointer to BIOS Attribute Table buffer.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful
**/
EFI_STATUS
EFIAPI
SetupBiosAttributeValueTables (
  IN BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER *BiosAttributeValueTableBuffer
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  UINT16        *BootOrderList = NULL;
  UINTN         BootOrderListSize;

  Status = SetupGenericBiosAttributeValueTable (BiosAttributeValueTableBuffer);
  if (EFI_ERROR (Status)) goto Exit;

  // Get Boot Order List.
  Status = GetEfiGlobalVariable2 (L"BootOrder", (VOID **) &BootOrderList, &BootOrderListSize);
  if (EFI_ERROR (Status) || (BootOrderList == NULL) || (BootOrderListSize == 0)) {
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  Status = AppendBiosAttributeValueTable (
             BiosAttributeValueTableBuffer,
             PLDM_BIOS_ATTRIBUTE_HANDLE_BOOT_CONFIG_SETTING,
             PLDM_ATTRIBUTE_VALUE_TYPE_BIOSBootConfigSetting,
             BIOS_ATTRIBUTE_VALUE_BOOT_CONFIG_TYPE_Default,                         //BootConfigType
             BIOS_ATTRIBUTE_VALUE_ORDERANDFAILTHROUGHMODE_OrderedAndFailThrough,    //OrderAndFailThroughMode
             BootOrderListSize / sizeof(UINT16),    //NumberOfBootSourceSettings
             BootOrderList                       //BootSourceStringHandleIndex[...]
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

  @param [in] BiosAttributeValueTableBuffer     Pointer to BIOS Attribute Table buffer.

  @retval EFI_SUCCESS                   Successful
  @retval Other values                  Unsuccessful

**/
EFI_STATUS
EFIAPI
GetBiosAttributeValueTableBuffer (
  IN BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER *BiosAttributeValueTableBuffer,
  IN OUT VOID **Buffer,
  IN OUT UINT32 *BufferSize
  )
{
  if ((Buffer == NULL) || (BufferSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  *Buffer = BiosAttributeValueTableBuffer->BiosAttributeValueTableEntries;
  *BufferSize = BiosAttributeValueTableBuffer->AppendedDataSize;
  if ((*Buffer == NULL) || (*BufferSize == 0)) {
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**
  Free any allocated buffer for BIOS Attribute Table Data.

  @param [in] BiosAttributeValueTableBuffer     Pointer to BIOS Attribute Table buffer.

**/
VOID
EFIAPI
CleanUpBiosAttributeValueTable (
  IN BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER *BiosAttributeValueTableBuffer
  )
{
  BiosAttributeValueTableBuffer->BufferSize = 0;
  BiosAttributeValueTableBuffer->AppendedDataSize = 0;
  if (BiosAttributeValueTableBuffer->BiosAttributeValueTableEntries != NULL) {
    FreePool (BiosAttributeValueTableBuffer->BiosAttributeValueTableEntries);
    BiosAttributeValueTableBuffer->BiosAttributeValueTableEntries = NULL;
  }
}
