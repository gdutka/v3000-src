/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */
#ifndef __AOD_DXE_H__
#define __AOD_DXE_H__
#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/FormBrowser2.h>
#include <Guid/MdeModuleHii.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiString.h>
#include <Library/HiiLib.h>
#include <Library/UefiHiiServicesLib.h>
#include <Protocol/HiiDatabase.h>
#include <Library/DevicePathLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Library/DxeServicesLib.h>
#include <Library/IoLib.h>
#include <Features/AODv2/Include/Aod.h>
#include <Features/AODv2/Include/AodSetup.h>
#include <Features/AODv2/Include/Protocol/AodFormsetManageProtocol.h>
#include <Features/AODv2/Include/Protocol/AodCmdProtocol.h>
#include <Features/AODv2/Include/Library/AodBaseLib.h>
//[-start-220906-IB14740262-add]//
#include <Protocol/AmdApcbProtocol.h>
#include <Protocol/VarApcbSyncProtocol.h>
//[-end-220906-IB14740262-add]//
#pragma pack(push, 1)
///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  VENDOR_DEVICE_PATH                VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL          End;
} HII_VENDOR_DEVICE_PATH;
typedef enum {
  APPEND_BYTE  = 0,
  APPEND_WORD,
  APPEND_DWORD,
  APPEND_BUFFER,
  APPEND_AMLBYTE,
  APPEND_AMLWORD,
  APPEND_AMLDWORD,
  APPEND_AMLSTRING,
  APPEND_AMLPKGLEN,
  APPEND_AMLBUFSIZE,
  APPEND_AMLNAMEDATA,
  APPEND_AMLONE,
  APPEND_AMLZERO
} APPEND_TYPE;

typedef struct {
  UINTN            MaxLength;
  UINTN            Length;
  UINT8            *pPtr;
} ASTREAM;

#pragma pack(pop)

typedef struct {
  UINT32                Signature;
  LIST_ENTRY            Link;
  UINT32                CmdId;
  CHAR8                 *pName;
  UINT8                 Property;
  UINT32                DataCount;
  UINT32                Current;
  UINT32                CurrentNvsIndex;
  UINT32                *pValue;
} AOD_CMD;
#define AOD_CMD_SIGNATURE     SIGNATURE_32('A', 'O', 'D', 'C')
#define AOD_CMD_FROM_LINK(a)  CR (a, AOD_CMD, Link, AOD_CMD_SIGNATURE)

#define AOD_DXE_PRIVATE_SIGNATURE SIGNATURE_32 ('A', 'O', 'D', 'D')
typedef struct {
  UINT32                               Signature;
  EFI_HII_HANDLE                       HiiHandle;
  EFI_HANDLE                           DriverHandle;
  EFI_HANDLE                           Handle;
  UINT8                                CmdCount;
  LIST_ENTRY                           CmdList;
  LIST_ENTRY                           ItemList;
  LIST_ENTRY                           CallbackList;
  EFI_HII_CONFIG_ACCESS_PROTOCOL       ConfigAccess;
  AOD_CMD_PROTOCOL                     CmdProtocol;
  AOD_FORMSET_MANAGE_PROTOCOL          FormsetManageProtocol;
  UINT8                                *IfrData;
  EFI_HII_DATABASE_PROTOCOL            *HiiDatabase;
  EFI_HII_STRING_PROTOCOL              *HiiString;
  EFI_HII_CONFIG_ROUTING_PROTOCOL      *HiiConfigRouting;
} AOD_DXE_PRIVATE_DATA;



#define AOD_DXE_PRIVATE_FROM_CONFIGACCESS(a)  CR (a, AOD_DXE_PRIVATE_DATA, ConfigAccess, AOD_DXE_PRIVATE_SIGNATURE)
#define AOD_DXE_PRIVATE_FROM_CMDPROTCOL(a)  CR (a, AOD_DXE_PRIVATE_DATA, CmdProtocol, AOD_DXE_PRIVATE_SIGNATURE)

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.

  @param[in]   This              Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]   Request           A null-terminated Unicode string in
                                 <ConfigRequest> format.
  @param[out]  Progress          On return, points to a character in the Request
                                 string. Points to the string's null terminator if
                                 request was successful. Points to the most recent
                                 '&' before the first failing name/value pair (or
                                 the beginning of the string if the failure is in
                                 the first name/value pair) if the request was not
                                 successful.
  @param[out]  Results           A null-terminated Unicode string in
                                 <ConfigAltResp> format which has all values filled
                                 in for the names in the Request string. String to
                                 be allocated by the called function.

  @retval EFI_SUCCESS            The Results is filled with the requested values.

**/
EFI_STATUS
EFIAPI
AodExtractConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL        *This,
  IN CONST EFI_STRING                            Request,
       OUT EFI_STRING                            *Progress,
       OUT EFI_STRING                            *Results
  );

/**
  This function is called to provide results data to the driver.

  @param[in]  This               Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  QuestionId         A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[out] ActionRequest      On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.

**/
EFI_STATUS
EFIAPI
AodCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL      *This,
  IN     EFI_BROWSER_ACTION                    Action,
  IN     EFI_QUESTION_ID                       QuestionId,
  IN     UINT8                                 Type,
  IN     EFI_IFR_TYPE_VALUE                    *Value,
     OUT EFI_BROWSER_ACTION_REQUEST            *ActionRequest
  );

/**
  This function processes the results of changes in configuration.

  @param[in]  This               Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Configuration      A null-terminated Unicode string in <ConfigResp>
                                 format.
  @param[out] Progress           A pointer to a string filled in with the offset of
                                 the most recent '&' before the first failing
                                 name/value pair (or the beginning of the string if
                                 the failure is in the first name/value pair) or
                                 the terminating NULL if all was successful.

  @retval EFI_SUCCESS            The Results is processed successfully.
  @retval EFI_INVALID_PARAMETER  Configuration is NULL.
  @retval EFI_NOT_FOUND          Routing data doesn't match any storage in this
                                 driver.

**/
EFI_STATUS
EFIAPI
AodRouteConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL      *This,
  IN CONST EFI_STRING                          Configuration,
       OUT EFI_STRING                          *Progress
  );



/**
  This function used to install AOD SSDT table and initial all AOD ACPI modules

  @param[in]     Private          Private data pointer of AodDxe

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
AodInstallAcpi (
  IN  AOD_DXE_PRIVATE_DATA                    *Private
  );

/**
  This function used to install AOD cmd portocol

  @param[in]     Private          Private data pointer of AodDxe

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
AodInstallCmdProtocol (
  IN  AOD_DXE_PRIVATE_DATA                    *Private
  );


/**
  This function allows a caller to set defulat for AOD varstore

  @param[in]   This              Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.

  @retval EFI_SUCCESS            The Results is filled with the requested values.

**/
EFI_STATUS
EFIAPI
AodLoadDefault (
  IN CONST AOD_FORMSET_MANAGE_PROTOCOL           *This,
    IN  CHAR16                                   *Name OPTIONAL
  );
#endif