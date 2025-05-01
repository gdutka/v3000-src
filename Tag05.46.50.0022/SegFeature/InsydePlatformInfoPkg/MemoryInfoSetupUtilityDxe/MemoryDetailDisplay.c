/** @file
  Memory Detail Display Function

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "MemoryInfoSetupUtilityDxe.h"

extern H2O_MEMORY_DEVICE_INFO_PROTOCOL  *mMemoryDeviceInfoProtocol;


/**
  Init the Dimm data in the memory detail information form

  @param[in]  OpCodeHandle        The OpCodeHandle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
InitDeviceLocatorInfo (
  IN    EFI_HII_HANDLE            OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  )
{
  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_DIMM_PROMPT),
    STRING_TOKEN (STR_DIMM_HELP),
    STRING_TOKEN (STR_DIMM_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  return EFI_SUCCESS;
}

/**
  Init the Channel data in the memory detail information form

  @param[in]  OpCodeHandle        The OpCodeHandle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
InitBankLocatorInfo (
  IN    EFI_HII_HANDLE            OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  )
{
  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_CHANNEL_PROMPT),
    STRING_TOKEN (STR_CHANNEL_HELP),
    STRING_TOKEN (STR_CHANNEL_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  return EFI_SUCCESS;
}

/**
  Init the Manufacture data in the memory detail information form

  @param[in]  OpCodeHandle        The OpCodeHandle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
InitManufacturerInfo (
  IN    EFI_HII_HANDLE            OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  )
{
  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_MANUFACTURER_PROMPT),
    STRING_TOKEN (STR_MANUFACTURER_HELP),
    STRING_TOKEN (STR_MANUFACTURER_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  return EFI_SUCCESS;
}

/**
  Init the Memory Type data in the memory detail information form

  @param[in]  OpCodeHandle        The OpCodeHandle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
InitMemoryTypeInfo (
  IN    EFI_HII_HANDLE            OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  )
{
  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_MEMORY_TYPE_PROMPT),
    STRING_TOKEN (STR_MEMORY_TYPE_HELP),
    STRING_TOKEN (STR_MEMORY_TYPE_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  return EFI_SUCCESS;
}

/**
  Init the Dram Type data in the memory detail information form

  @param[in]  OpCodeHandle        The OpCodeHandle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
InitDramTypeInfo (
  IN    EFI_HII_HANDLE            OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  )
{
  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_DRAM_TYPE_PROMPT),
    STRING_TOKEN (STR_DRAM_TYPE_HELP),
    STRING_TOKEN (STR_DRAM_TYPE_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  return EFI_SUCCESS;
}

/**
  Init the Serial Number data in the memory detail information form

  @param[in]  OpCodeHandle        The OpCodeHandle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
InitSerialNumberInfo (
  IN    EFI_HII_HANDLE            OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  )
{
  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_SERIAL_NO_PROMPT),
    STRING_TOKEN (STR_SERIAL_NO_HELP),
    STRING_TOKEN (STR_SERIAL_NO_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  return EFI_SUCCESS;
}

/**
  Init the Part Number data in the memory detail information form

  @param[in]  OpCodeHandle        The OpCodeHandle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
InitPartNumberInfo (
  IN    EFI_HII_HANDLE            OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  )
{
  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_PART_NO_PROMPT),
    STRING_TOKEN (STR_PART_NO_HELP),
    STRING_TOKEN (STR_PART_NO_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  return EFI_SUCCESS;
}

/**
  Init the Memory Size data in the memory detail information form

  @param[in]  OpCodeHandle        The OpCodeHandle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
InitMemorySizeInfo (
  IN    EFI_HII_HANDLE            OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  )
{
  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_MEMORY_SIZE_PROMPT),
    STRING_TOKEN (STR_MEMORY_SIZE_HELP),
    STRING_TOKEN (STR_MEMORY_SIZE_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  return EFI_SUCCESS;
}

/**
  Init the Maximum Capable Memory Speed data in the memory detail information form

  @param[in]  OpCodeHandle        The OpCodeHandle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
InitMaximumCapableMemorySpeedInfo (
  IN    EFI_HII_HANDLE            OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  )
{
  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_MAXIMUM_MEMORY_SPEED_PROMPT),
    STRING_TOKEN (STR_MAXIMUM_MEMORY_SPEED_HELP),
    STRING_TOKEN (STR_MAXIMUM_MEMORY_SPEED_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  return EFI_SUCCESS;
}

/**
  Init the Configured Memory Speed data in the memory detail information form

  @param[in]  OpCodeHandle        The OpCodeHandle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
InitConfigureMemorySpeedInfo (
  IN    EFI_HII_HANDLE            OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  )
{
  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_CONFIG_MEMORY_SPEED_PROMPT),
    STRING_TOKEN (STR_CONFIG_MEMORY_SPEED_HELP),
    STRING_TOKEN (STR_CONFIG_MEMORY_SPEED_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  return EFI_SUCCESS;
}

/**
  Init the Configured Voltage data in the memory detail information form

  @param[in]  OpCodeHandle        The OpCodeHandle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
InitConfigureVoltageInfo (
  IN    EFI_HII_HANDLE            OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  )
{
  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_CONFIG_VOLTAGE_PROMPT),
    STRING_TOKEN (STR_CONFIG_VOLTAGE_HELP),
    STRING_TOKEN (STR_CONFIG_VOLTAGE_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  return EFI_SUCCESS;
}

STATIC
EFI_STRING
GetInfoErrorText (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  )
{
  if (mMemoryDeviceInfoProtocol->GetMemoryInstallStatus (
                                    mMemoryDeviceInfoProtocol,
                                    MemoryInfoIndex
                                    )) {
    return HiiGetString (HiiHandle, STRING_TOKEN (STR_UNKNOWN), NULL);
  }

  return HiiGetString (HiiHandle, STRING_TOKEN (STR_NONE_MEMORY_NOT_INSTALLED), NULL);
}

/**
  Update the Dimm data in the memory detail information form

  @param[in]  HiiHandle           Hii Handle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
UpdateDeviceLocatorInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *String;

  //
  // Update Dimm
  //
  Status = mMemoryDeviceInfoProtocol->GetDeviceLocatorString (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &String
                                        );
  if (EFI_ERROR (Status)) {
    String = GetInfoErrorText (HiiHandle, MemoryInfoIndex);
    if (String == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  HiiSetString (HiiHandle, STRING_TOKEN (STR_DIMM_STRING), String, NULL);
  FreePool (String);

  return EFI_SUCCESS;
}

/**
  Update the Channel data in the memory detail information form

  @param[in]  HiiHandle           Hii Handle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
UpdateBankLocatorInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *String;

  //
  // Update Channel
  //
  Status = mMemoryDeviceInfoProtocol->GetBankLocatorString (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &String
                                        );
  if (EFI_ERROR (Status)) {
    String = GetInfoErrorText (HiiHandle, MemoryInfoIndex);
    if (String == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  HiiSetString (HiiHandle, STRING_TOKEN (STR_CHANNEL_STRING), String, NULL);
  FreePool (String);

  return EFI_SUCCESS;
}

/**
  Update the Manufacture data in the memory detail information form

  @param[in]  HiiHandle           Hii Handle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
UpdateManufacturerInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *String;

  //
  // Update Manufacture
  //
  Status = mMemoryDeviceInfoProtocol->GetManufacturerString (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &String
                                        );

  if (EFI_ERROR (Status)) {
    String = GetInfoErrorText (HiiHandle, MemoryInfoIndex);
    if (String == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  HiiSetString (HiiHandle, STRING_TOKEN (STR_MANUFACTURER_STRING), String, NULL);
  FreePool (String);

  return EFI_SUCCESS;
}

/**
  Update the Memory Type data in the memory detail information form

  @param[in]  HiiHandle           Hii Handle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
UpdateMemoryTypeInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *String;
  CHAR16                          TempString[MAX_STR_SIZE] = {0};
  CHAR16                          *MemoryTypeString;
  CHAR16                          *MemoryTypeDetailString;

  //
  // Update Memory Type
  //
  Status = mMemoryDeviceInfoProtocol->GetMemoryTypeString (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &MemoryTypeString,
                                        NULL
                                        );
  Status = mMemoryDeviceInfoProtocol->GetMemoryTypeDetailString (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &MemoryTypeDetailString,
                                        NULL
                                        );

  if (!EFI_ERROR (Status)) {
    UnicodeSPrint (
      TempString,
      sizeof (TempString),
      L"%s - %s",
      MemoryTypeString,
      MemoryTypeDetailString
      );
    HiiSetString (HiiHandle, STRING_TOKEN (STR_MEMORY_TYPE_STRING), TempString, NULL);
    FreePool (MemoryTypeString);
    FreePool (MemoryTypeDetailString);
  } else {
    String = GetInfoErrorText (HiiHandle, MemoryInfoIndex);
    if (String == NULL) {
      return EFI_NOT_FOUND;
    }
    HiiSetString (HiiHandle, STRING_TOKEN (STR_MEMORY_TYPE_STRING), String, NULL);
    FreePool (String);
  }

  return EFI_SUCCESS;
}

/**
  Update the Dram Type data in the memory detail information form

  @param[in]  HiiHandle           Hii Handle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
UpdateDramTypeInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *String;

  //
  // Update Dram Type
  //
  Status = mMemoryDeviceInfoProtocol->GetDramTypeString (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &String,
                                        NULL
                                        );

  if (EFI_ERROR (Status)) {
    String = GetInfoErrorText (HiiHandle, MemoryInfoIndex);
    if (String == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  HiiSetString (HiiHandle, STRING_TOKEN (STR_DRAM_TYPE_STRING), String, NULL);
  FreePool (String);

  return EFI_SUCCESS;
}

/**
  Update the Serial Number data in the memory detail information form

  @param[in]  HiiHandle           Hii Handle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
UpdateSerialNumberInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *String;

  //
  // Update Serial Number
  //
  Status = mMemoryDeviceInfoProtocol->GetSerialNumberString (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &String
                                        );

  if (EFI_ERROR (Status)) {
    String = GetInfoErrorText (HiiHandle, MemoryInfoIndex);
    if (String == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  HiiSetString (HiiHandle, STRING_TOKEN (STR_SERIAL_NO_STRING), String, NULL);
  FreePool (String);

  return EFI_SUCCESS;
}

/**
  Update the Part Number data in the memory detail information form

  @param[in]  HiiHandle           Hii Handle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
UpdatePartNumberInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *String;

  //
  // Update Part Number
  //
  Status = mMemoryDeviceInfoProtocol->GetPartNumberString (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &String
                                        );

  if (EFI_ERROR (Status)) {
    String = GetInfoErrorText (HiiHandle, MemoryInfoIndex);
    if (String == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  HiiSetString (HiiHandle, STRING_TOKEN (STR_PART_NO_STRING), String, NULL);
  FreePool (String);

  return EFI_SUCCESS;
}

/**
  Update the Memory Size data in the memory detail information form

  @param[in]  HiiHandle           Hii Handle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
UpdateMemorySizeInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *String;
  UINT32                          MemorySize;

  //
  // Update Memory Size
  //
  String = NULL;
  MemorySize = 0;
  Status = mMemoryDeviceInfoProtocol->GetMemorySize (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &MemorySize
                                        );

  if (!EFI_ERROR (Status)) {
    String = SingleMemorySizeStringFormat (MemorySize);
  } else {
    String = GetInfoErrorText (HiiHandle, MemoryInfoIndex);
    if (String == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  HiiSetString (HiiHandle, STRING_TOKEN (STR_MEMORY_SIZE_STRING), String, NULL);

  if (String != NULL) {
    FreePool (String);
  }

  return EFI_SUCCESS;
}

/**
  Update the Maximum Capable Memory Speed data in the memory detail information form

  @param[in]  HiiHandle           Hii Handle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
UpdateMaximumCapableMemorySpeedInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *String;
  UINT32                          MaximumCapableMemorySpeed;

  //
  // Update Maximum Capable Memory Speed
  //
  String = NULL;
  MaximumCapableMemorySpeed = 0;
  Status = mMemoryDeviceInfoProtocol->GetMaximumCapableMemorySpeed (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &MaximumCapableMemorySpeed
                                        );

  if (!EFI_ERROR (Status)) {
    String = MemorySpeedStringFormat (MaximumCapableMemorySpeed);
  } else {
    String = GetInfoErrorText (HiiHandle, MemoryInfoIndex);
    if (String == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  HiiSetString (HiiHandle, STRING_TOKEN (STR_MAXIMUM_MEMORY_SPEED_STRING), String, NULL);

  if (String != NULL) {
    FreePool (String);
  }

  return EFI_SUCCESS;
}

/**
  Update the Configured Memory Speed data in the memory detail information form

  @param[in]  HiiHandle           Hii Handle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
UpdateConfigureMemorySpeedInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *String;
  UINT32                          ConfigureMemorySpeed;

  //
  // Update Configured Memory Speed
  //
  String = NULL;
  ConfigureMemorySpeed = 0;
  Status = mMemoryDeviceInfoProtocol->GetConfigureMemorySpeed (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &ConfigureMemorySpeed
                                        );

  if (!EFI_ERROR (Status)) {
    String = MemorySpeedStringFormat (ConfigureMemorySpeed);
  } else {
    String = GetInfoErrorText (HiiHandle, MemoryInfoIndex);
    if (String == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  HiiSetString (HiiHandle, STRING_TOKEN (STR_CONFIG_MEMORY_SPEED_STRING), String, NULL);

  if (String != NULL) {
    FreePool (String);
  }

  return EFI_SUCCESS;
}

/**
  Update the Configured Voltage data in the memory detail information form

  @param[in]  HiiHandle           Hii Handle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
UpdateConfigureVoltageInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *String;
  UINT16                          ConfigureVoltage;

  //
  // Update Configured Voltage
  //
  String = NULL;
  ConfigureVoltage = 0;
  Status = mMemoryDeviceInfoProtocol->GetConfigureVoltage (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &ConfigureVoltage
                                        );

  if (!EFI_ERROR (Status)) {
    String = MemoryVoltageStringFormat (ConfigureVoltage);
  } else {
    String = GetInfoErrorText (HiiHandle, MemoryInfoIndex);
    if (String == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  HiiSetString (HiiHandle, STRING_TOKEN (STR_CONFIG_VOLTAGE_STRING), String, NULL);

  if (String != NULL) {
    FreePool (String);
  }

  return EFI_SUCCESS;
}
