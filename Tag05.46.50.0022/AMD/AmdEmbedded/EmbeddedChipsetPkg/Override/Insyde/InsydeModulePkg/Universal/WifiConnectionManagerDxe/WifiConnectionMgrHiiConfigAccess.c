/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
 The Hii functions for wireless connection driver.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#include "WifiConnectionMgrDxe.h"

CHAR16  mVendorStorageName[] = L"WIFI_MANAGER_IFR_NVDATA";

HII_VENDOR_DEVICE_PATH  mWifiMgrDxeHiiVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    WIFI_CONFIG_FORM_SET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

//
// HII Config Access Protocol instance
//
GLOBAL_REMOVE_IF_UNREFERENCED
EFI_HII_CONFIG_ACCESS_PROTOCOL gWifiMgrDxeHiiConfigAccess = {
  WifiMgrDxeHiiConfigAccessExtractConfig,
  WifiMgrDxeHiiConfigAccessRouteConfig,
  WifiMgrDxeHiiConfigAccessCallback
};

CHAR16*   mSecurityType[] = {
  L"OPEN           ",
  L"WPA-Enterprise ",
  L"WPA2-Enterprise",
  L"WPA-Personal   ",
  L"WPA2-Personal  ",
  L"WEP            ",
  L"UnKnown        "
};

CHAR16*  mSignalStrengthBar[] = {
  L"[-----]",
  L"[*----]",
  L"[**---]",
  L"[***--]",
  L"[****-]",
  L"[*****]"
};

#define  RSSI_TO_SIGNAL_STRENGTH_BAR(Rssi)  mSignalStrengthBar[((Rssi + 19)/20)]

#define  NET_LIST_FOR_EACH_FROM_NODE(Entry, Node, ListHead) \
  for(Entry = Node->ForwardLink; Entry != (ListHead); Entry = Entry->ForwardLink)

extern EFI_GUID    mEfiWifiMgrPrivateGuid;
extern EFI_GUID    gWifiConfigFormSetGuid;

/**
  Create Hii Extend Label OpCode as the start opcode and end opcode.
  The caller is responsible for freeing the OpCode with HiiFreeOpCodeHandle().

  @param[in]  StartLabelNumber   The number of start label.
  @param[out] StartOpCodeHandle  Points to the start opcode handle.
  @param[out] EndOpCodeHandle    Points to the end opcode handle.

  @retval EFI_OUT_OF_RESOURCES   Do not have sufficient resource to finish this
                                 operation.
  @retval EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval EFI_SUCCESS            The operation is completed successfully.
  @retval Other Errors           Returned errors when updating the HII form.
**/
EFI_STATUS
WifiMgrCreateOpCode (
  IN  UINT16    StartLabelNumber,
  OUT VOID      **StartOpCodeHandle,
  OUT VOID      **EndOpCodeHandle
  )
{
  EFI_STATUS            Status;
  EFI_IFR_GUID_LABEL    *InternalStartLabel;
  EFI_IFR_GUID_LABEL    *InternalEndLabel;

  if (StartOpCodeHandle == NULL || EndOpCodeHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status             = EFI_OUT_OF_RESOURCES;
  *StartOpCodeHandle = NULL;
  *EndOpCodeHandle   = NULL;

  //
  // Initialize the container for dynamic opcodes.
  //
  *StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (*StartOpCodeHandle == NULL) {
    goto Exit;
  }
  *EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (*EndOpCodeHandle == NULL) {
    goto Exit;
  }

  //
  // Create Hii Extend Label OpCode as the start opcode.
  //
  InternalStartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                                *StartOpCodeHandle,
                                                &gEfiIfrTianoGuid,
                                                NULL,
                                                sizeof (EFI_IFR_GUID_LABEL)
                                                );
  if (InternalStartLabel == NULL) {
    goto Exit;
  }
  InternalStartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  InternalStartLabel->Number       = StartLabelNumber;

  //
  // Create Hii Extend Label OpCode as the end opcode.
  //
  InternalEndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                              *EndOpCodeHandle,
                                              &gEfiIfrTianoGuid,
                                              NULL,
                                              sizeof (EFI_IFR_GUID_LABEL)
                                              );
  if (InternalEndLabel == NULL) {
    goto Exit;
  }
  InternalEndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  InternalEndLabel->Number       = LABEL_END;

  return EFI_SUCCESS;

Exit:

  if (*StartOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (*StartOpCodeHandle);
  }
  if (*EndOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (*EndOpCodeHandle);
  }
  return Status;
}

/**
  Display the Nic list contains all available Nics.

  @param[in]  Private            The pointer to the global private data structure.

  @retval EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
WifiMgrShowNicList (
  IN  WIFI_MGR_PRIVATE_DATA    *Private
)
{
  EFI_STATUS              Status;
  CHAR16                  MacString[WIFI_MGR_MAX_MAC_STRING_LEN];
  EFI_STRING_ID           PortTitleToken;
  EFI_STRING_ID           PortTitleHelpToken;
  WIFI_MGR_DEVICE_DATA    *Nic;
  LIST_ENTRY              *Entry;
  VOID                    *StartOpCodeHandle;
  VOID                    *EndOpCodeHandle;

  if (Private == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WifiMgrCreateOpCode (
             LABEL_MAC_ENTRY,
             &StartOpCodeHandle,
             &EndOpCodeHandle
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  NET_LIST_FOR_EACH (Entry, &Private->NicList) {
    Nic = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_DEVICE_DATA, Link, WIFI_MGR_DEVICE_DATA_SIGNATURE);
    WifiMgrMacAddrToStr (&Nic->MacAddress, sizeof (MacString), MacString);
    UnicodeSPrint (Private->PortString, sizeof (Private->PortString), L"MAC %s", MacString);
    PortTitleToken = HiiSetString (
                       Private->RegisteredHandle,
                       0,
                       Private->PortString,
                       NULL
                       );
    if (PortTitleToken == 0) {
      Status = EFI_INVALID_PARAMETER;
      goto Exit;
    }

    UnicodeSPrint (Private->PortString, sizeof (Private->PortString), L"MAC Address");
    PortTitleHelpToken = HiiSetString (
                           Private->RegisteredHandle,
                           0,
                           Private->PortString,
                           NULL
                           );
    if (PortTitleHelpToken == 0) {
      Status = EFI_INVALID_PARAMETER;
      goto Exit;
    }

    HiiCreateGotoOpCode (
      StartOpCodeHandle,
      FORMID_WIFI_MAINPAGE,
      PortTitleToken,
      PortTitleHelpToken,
      EFI_IFR_FLAG_CALLBACK,
      (UINT16) (KEY_MAC_ENTRY_BASE + Nic->NicIndex)
      );
  }

  Status = HiiUpdateForm (
             Private->RegisteredHandle,       // HII handle
             &gWifiConfigFormSetGuid,         // Formset GUID
             FORMID_MAC_SELECTION,            // Form ID
             StartOpCodeHandle,               // Label for where to insert opcodes
             EndOpCodeHandle                  // Replace data
             );

Exit:

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
  return Status;
}

/**

  Retreive the unicode string of the AKM Suite list of a profile.

  @param[in]  Profile           The network profile contains a AKM suite list.

  @return the unicode string of AKM suite list or "None".

**/
CHAR16*
WifiMgrGetStrAKMList (
  IN  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile
)
{
  UINT8     Index;
  UINT16    AKMSuiteCount;
  CHAR16    *AKMListDisplay;

  AKMListDisplay = L"None";
  if (Profile == NULL || Profile->Network.AKMSuite == NULL) {
    return AKMListDisplay;
  }

  AKMSuiteCount   = Profile->Network.AKMSuite->AKMSuiteCount;
  if (AKMSuiteCount != 0) {

    //
    //Current AKM Suite is between 1-9
    //
    AKMListDisplay = (CHAR16 *) AllocateZeroPool(sizeof (CHAR16) * AKMSuiteCount * 2);
    if (AKMListDisplay != NULL) {
      for (Index = 0; Index < AKMSuiteCount; Index ++) {
        UnicodeSPrint (
          AKMListDisplay + (Index * 2),
          sizeof (CHAR16) * 2,
          L"%d ",
          Profile->Network.AKMSuite->AKMSuiteList[Index].SuiteType
          );
        if (Index == AKMSuiteCount - 1) {
          *(AKMListDisplay + (Index * 2 + 1)) = L'\0';
        }
      }
    }
  }

  return AKMListDisplay;
}

/**

  Retreive the unicode string of the Cipher Suite list of a profile.

  @param[in]  Profile           The network profile contains a Cipher suite list.

  @return the unicode string of Cipher suite list or "None".

**/
CHAR16*
WifiMgrGetStrCipherList (
  IN  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile
)
{
  UINT8     Index;
  UINT16    CipherSuiteCount;
  CHAR16    *CipherListDisplay;

  CipherListDisplay = L"None";
  if (Profile == NULL || Profile->Network.CipherSuite == NULL) {
    return CipherListDisplay;
  }

  CipherSuiteCount   = Profile->Network.CipherSuite->CipherSuiteCount;
  if (CipherSuiteCount != 0) {

    //
    //Current Cipher Suite is between 1-9
    //
    CipherListDisplay = (CHAR16 *) AllocateZeroPool(sizeof (CHAR16) * CipherSuiteCount * 2);
    if (CipherListDisplay != NULL) {
      for (Index = 0; Index < CipherSuiteCount; Index ++) {
        UnicodeSPrint (
          CipherListDisplay + (Index * 2),
          sizeof (CHAR16) * 2,
          L"%d ",
          Profile->Network.CipherSuite->CipherSuiteList[Index].SuiteType
          );
        if (Index == CipherSuiteCount - 1) {
          *(CipherListDisplay + (Index * 2 + 1)) = L'\0';
        }
      }
    }
  }

  return CipherListDisplay;
}

/**
  Refresh the network list display of the current Nic.

  @param[in]   Private           The pointer to the global private data structure.
  @param[out]  IfrNvData         The IFR NV data.

  @retval EFI_SUCCESS            The operation is completed successfully.
  @retval EFI_OUT_OF_RESOURCES   Failed to allocate memory.
  @retval Other Errors           Returned errors when creating Opcodes or updating the
                                 Hii form.
**/
EFI_STATUS
WifiMgrRefreshNetworkList (
  IN    WIFI_MGR_PRIVATE_DATA      *Private,
  OUT   WIFI_MANAGER_IFR_NVDATA    *IfrNvData
  )
{
  EFI_STATUS                         Status;
  EFI_TPL                            OldTpl;
  UINT32                             AvailableCount;
  EFI_STRING_ID                      PortPromptToken;
  EFI_STRING_ID                      PortTextToken;
  EFI_STRING_ID                      PortHelpToken;
  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile;
  LIST_ENTRY                         *Entry;
  VOID                               *StartOpCodeHandle;
  VOID                               *EndOpCodeHandle;
  CHAR16                             *AKMListDisplay;
  CHAR16                             *CipherListDisplay;

  if (Private->CurrentNic == NULL) {
    return EFI_SUCCESS;
  }

  Status = WifiMgrCreateOpCode (
             LABEL_NETWORK_LIST_ENTRY,
             &StartOpCodeHandle,
             &EndOpCodeHandle
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  AvailableCount = 0;

  if (Private->CurrentNic->ConnectState == WifiMgrConnectedToAp) {

    //
    // Display the current connected network.
    // Find the current operate network under connected status.
    //
    if (Private->CurrentNic->CurrentOperateNetwork != NULL &&
      Private->CurrentNic->CurrentOperateNetwork->IsAvailable == TRUE) {

      Profile           = Private->CurrentNic->CurrentOperateNetwork;
      AKMListDisplay    = WifiMgrGetStrAKMList (Profile);
      CipherListDisplay = WifiMgrGetStrCipherList(Profile);

      AvailableCount ++;
      UnicodeSPrint (
        Private->PortString,
        sizeof (Private->PortString),
        L"%s (Connected)",
        Profile->SSId
        );
      PortPromptToken = HiiSetString (
                          Private->RegisteredHandle,
                          0,
                          Private->PortString,
                          NULL
                          );
      if (PortPromptToken == 0) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
      }

      if (Profile->SecurityType == SECURITY_TYPE_NONE) {
        PortHelpToken = 0;
      } else {
        UnicodeSPrint (
        Private->PortString,
        sizeof (Private->PortString),
        L"AKMSuite: %s CipherSuite: %s",
        AKMListDisplay, CipherListDisplay
        );
        PortHelpToken = HiiSetString (
                            Private->RegisteredHandle,
                            0,
                            Private->PortString,
                            NULL
                            );
        if (PortHelpToken == 0) {
          Status = EFI_OUT_OF_RESOURCES;
          goto Exit;
        }
      }

      HiiCreateGotoOpCode (
        StartOpCodeHandle,
        FORMID_CONNECT_NETWORK,
        PortPromptToken,
        PortHelpToken,
        EFI_IFR_FLAG_CALLBACK,
        (UINT16) (KEY_AVAILABLE_NETWORK_ENTRY_BASE + Profile->ProfileIndex)
        );

      UnicodeSPrint (
        Private->PortString,
        sizeof (Private->PortString),
        L"%s       %s %s",
        (Profile->SecurityType != SECURITY_TYPE_NONE ? L"Secured" : L"Open   "),
        mSecurityType[Profile->SecurityType],
        RSSI_TO_SIGNAL_STRENGTH_BAR(Profile->NetworkQuality)
        );
      PortTextToken = HiiSetString (
                         Private->RegisteredHandle,
                         0,
                         Private->PortString,
                         NULL
                         );
      if (PortTextToken == 0) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
      }
      HiiCreateTextOpCode (
        StartOpCodeHandle,
        PortTextToken,
        0,
        0
      );
    } else {
      Private->CurrentNic->HasDisconnectPendingNetwork = TRUE;
    }
  }

  //
  // Display all supported available networks.
  //
  NET_LIST_FOR_EACH (Entry, &Private->CurrentNic->ProfileList) {
    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);
    if (Private->CurrentNic->ConnectState == WifiMgrConnectedToAp &&
      Private->CurrentNic->CurrentOperateNetwork == Profile) {

      //
      // Current connected network will be shown at the top.
      //
      continue;
    }
    if (Profile->IsAvailable == TRUE && Profile->CipherSuiteSupported == TRUE) {

      AvailableCount ++;
      AKMListDisplay    = WifiMgrGetStrAKMList (Profile);
      CipherListDisplay = WifiMgrGetStrCipherList(Profile);
      PortPromptToken = HiiSetString (
                          Private->RegisteredHandle,
                          0,
                          Profile->SSId,
                          NULL
                          );
      if (PortPromptToken == 0) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
      }

      if (Profile->SecurityType == SECURITY_TYPE_NONE) {
        PortHelpToken = 0;
      } else {
        UnicodeSPrint (
          Private->PortString,
          sizeof (Private->PortString),
          L"AKMSuite: %s CipherSuite: %s",
          AKMListDisplay, CipherListDisplay
          );
        PortHelpToken = HiiSetString (
                          Private->RegisteredHandle,
                          0,
                          Private->PortString,
                          NULL
                          );
        if (PortHelpToken == 0) {
          Status = EFI_OUT_OF_RESOURCES;
          goto Exit;
        }
      }

      HiiCreateGotoOpCode (
        StartOpCodeHandle,
        FORMID_CONNECT_NETWORK,
        PortPromptToken,
        PortHelpToken,
        EFI_IFR_FLAG_CALLBACK,
        (UINT16) (KEY_AVAILABLE_NETWORK_ENTRY_BASE + Profile->ProfileIndex)
        );

      UnicodeSPrint (
        Private->PortString,
        sizeof (Private->PortString),
        L"%s       %s %s",
        (Profile->SecurityType != SECURITY_TYPE_NONE ? L"Secured" : L"Open   "),
        mSecurityType[Profile->SecurityType],
        RSSI_TO_SIGNAL_STRENGTH_BAR(Profile->NetworkQuality)
        );
      PortTextToken = HiiSetString (
                        Private->RegisteredHandle,
                        0,
                        Private->PortString,
                        NULL
                        );
      if (PortTextToken == 0) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
      }
      HiiCreateTextOpCode (
        StartOpCodeHandle,
        PortTextToken,
        0,
        0
        );
    }
  }

  //
  // Display all unsupported available networks.
  //
  NET_LIST_FOR_EACH (Entry, &Private->CurrentNic->ProfileList) {
    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);
    if (Private->CurrentNic->ConnectState == WifiMgrConnectedToAp &&
      Private->CurrentNic->CurrentOperateNetwork == Profile) {

      //
      // Current connected network will be shown at the top.
      //
      continue;
    }
    if (Profile->IsAvailable == TRUE && Profile->CipherSuiteSupported == FALSE) {

      AvailableCount ++;
      AKMListDisplay    = WifiMgrGetStrAKMList (Profile);
      CipherListDisplay = WifiMgrGetStrCipherList(Profile);
      PortPromptToken = HiiSetString (
                          Private->RegisteredHandle,
                          0,
                          Profile->SSId,
                          NULL
                          );
      if (PortPromptToken == 0) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
      }
      if (Profile->AKMSuiteSupported) {
        UnicodeSPrint (
          Private->PortString,
          sizeof (Private->PortString),
          L"AKMSuite: %s CipherSuite(UnSupported): %s",
          AKMListDisplay, CipherListDisplay
          );
      } else {
        UnicodeSPrint (
          Private->PortString,
          sizeof (Private->PortString),
          L"AKMSuite(UnSupported): %s CipherSuite(UnSupported): %s",
          AKMListDisplay, CipherListDisplay
          );
      }
      PortHelpToken = HiiSetString (
                        Private->RegisteredHandle,
                        0,
                        Private->PortString,
                        NULL
                        );
      if (PortHelpToken == 0) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
      }
      HiiCreateGotoOpCode (
        StartOpCodeHandle,
        FORMID_CONNECT_NETWORK,
        PortPromptToken,
        PortHelpToken,
        EFI_IFR_FLAG_CALLBACK,
        (UINT16) (KEY_AVAILABLE_NETWORK_ENTRY_BASE + Profile->ProfileIndex)
        );

      UnicodeSPrint (
        Private->PortString,
        sizeof (Private->PortString),
        L"%s       %s %s",
        L"UnSupported",
        mSecurityType[Profile->SecurityType],
        RSSI_TO_SIGNAL_STRENGTH_BAR(Profile->NetworkQuality)
        );
      PortTextToken = HiiSetString (
                        Private->RegisteredHandle,
                        0,
                        Private->PortString,
                        NULL
                        );
      if (PortTextToken == 0) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
      }
      HiiCreateTextOpCode (
        StartOpCodeHandle,
        PortTextToken,
        0,
        0
        );
    }
  }

  Status = HiiUpdateForm (
             Private->RegisteredHandle,       // HII handle
             &gWifiConfigFormSetGuid,         // Formset GUID
             FORMID_NETWORK_LIST,             // Form ID
             StartOpCodeHandle,               // Label for where to insert opcodes
             EndOpCodeHandle                  // Replace data
             );

Exit:

  gBS->RestoreTPL (OldTpl);
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
  return Status;
}

/**
  Callback function for user to select a Nic.

  @param[in]  Private            The pointer to the global private data structure.
  @param[in]  KeyValue           The key value received from HII input.

  @retval EFI_NOT_FOUND          The corresponding Nic is not found.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
WifiMgrSelectNic (
  IN     WIFI_MGR_PRIVATE_DATA         *Private,
  IN     EFI_QUESTION_ID               KeyValue
  )
{
  WIFI_MGR_DEVICE_DATA    *Nic;
  UINT32                  NicIndex;
  CHAR16                  MacString[WIFI_MGR_MAX_MAC_STRING_LEN];

  NicIndex = KeyValue - KEY_MAC_ENTRY_BASE;
  Nic      = WifiMgrGetNicByIndex (Private, NicIndex);
  if (Nic == NULL) {
    return EFI_NOT_FOUND;
  }
  Private->CurrentNic = Nic;

  WifiMgrMacAddrToStr (&Nic->MacAddress, sizeof (MacString), MacString);
  HiiSetString (Private->RegisteredHandle, STRING_TOKEN(STR_MAC_ADDRESS), MacString, NULL);
  return EFI_SUCCESS;
}

/**
  Display the saved network list to user.

  @param[in]   Private            The pointer to the global private data structure.

  @retval EFI_NOT_FOUND           The corresponding Nic is not found.
  @retval EFI_INVALID_PARAMETER   Any input parameter is invalid.
  @retval EFI_SUCCESS             The operation is completed successfully.
  @retval Other Errors            Returned errors when creating Opcodes or updating the
                                  Hii form.
**/
EFI_STATUS
WifiMgrDisplaySavedList (
  IN     WIFI_MGR_PRIVATE_DATA          *Private
  )
{
  EFI_STATUS                            Status;
  EFI_TPL                               OldTpl;
  WIFI_MGR_NETWORK_PROFILE_NVDATA       *Profile;
  EFI_STRING_ID                         PortPromptToken;
  VOID                                  *StartOpCodeHandle;
  VOID                                  *EndOpCodeHandle;
  LIST_ENTRY                            *Entry;

  if (Private->CurrentNic == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = WifiMgrCreateOpCode (
             LABEL_SAVED_LIST_ENTRY,
             &StartOpCodeHandle,
             &EndOpCodeHandle
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  NET_LIST_FOR_EACH (Entry, &Private->CurrentNic->ProfileList) {
    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);
    if (Profile->IsSaved == TRUE) {
      UnicodeSPrint (
        Private->PortString,
        sizeof (Private->PortString),
        L"%s      %s",
        Profile->SSId,
        mSecurityType[Profile->SecurityType]
        );
      PortPromptToken = HiiSetString (
                          Private->RegisteredHandle,
                          0,
                          Private->PortString,
                          NULL
                          );
      if (PortPromptToken == 0) {
        Status = EFI_INVALID_PARAMETER;
        goto Exit;
      }
      HiiCreateGotoOpCode (
        StartOpCodeHandle,                      // Container for dynamic created opcodes
        FORMID_MANAGE_NETWORK,
        PortPromptToken,
        0,
        EFI_IFR_FLAG_CALLBACK,                  // Question flag
        (UINT16) (KEY_SAVED_NETWORK_ENTRY_BASE + Profile->ProfileIndex)
        );
    }
  }

  Status = HiiUpdateForm (
             Private->RegisteredHandle,       // HII handle
             &gWifiConfigFormSetGuid,         // Formset GUID
             FORMID_SAVED_LIST,               // Form ID
             StartOpCodeHandle,               // Label for where to insert opcodes
             EndOpCodeHandle                  // Replace data
             );

Exit:
  gBS->RestoreTPL (OldTpl);
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);

  return Status;
}

/**
  Display network priority list to user.

  @param[in]   Private            The pointer to the global private data structure.
  @param[out]  IfrNvData          The IFR NV data.

  @retval EFI_NOT_FOUND           The corresponding Nic is not found.
  @retval EFI_OUT_OF_RESOURCES    Do not have sufficient resource to finish this operation.
  @retval EFI_SUCCESS             The operation is completed successfully.
  @retval Other Errors            Returned errors when creating Opcodes or updating the
                                  Hii form.
**/
EFI_STATUS
WifiMgrDisplayPriorityList (
  IN     WIFI_MGR_PRIVATE_DATA         *Private,
  OUT    WIFI_MANAGER_IFR_NVDATA       *IfrNvData
  )
{
  EFI_STATUS                         Status;
  WIFI_MGR_DEVICE_DATA               *Nic;
  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile;
  UINT8                              Index;
  VOID                               *StartOpCodeHandle;
  VOID                               *EndOpCodeHandle;
  VOID                               *OptionsOpCodeHandle;
  LIST_ENTRY                         *Entry;

  OptionsOpCodeHandle = NULL;
  Index = 0;
  Status = WifiMgrCreateOpCode (
             LABEL_PRIORITY_ENTRY,
             &StartOpCodeHandle,
             &EndOpCodeHandle
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Nic = Private->CurrentNic;
  if (Nic == NULL || Nic->ProfileCount == 0) {
    goto Error;
  }

  //
  // Create Option OpCode.
  //
  OptionsOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (OptionsOpCodeHandle == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Error;
  }

  ZeroMem (IfrNvData->DynamicPriorityList, sizeof (IfrNvData->DynamicPriorityList));
  NET_LIST_FOR_EACH (Entry, &Nic->ProfileList) {
    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);
    if (Profile->IsSaved == TRUE) {
      IfrNvData->DynamicPriorityList[Index ++] = Profile->ProfileIndex;
      if (Profile->TitleToken == 0) {
        Profile->TitleToken = HiiSetString (
                                Private->RegisteredHandle,
                                0,
                                Profile->SSId,
                                NULL
                                );
      }
      HiiCreateOneOfOptionOpCode (
        OptionsOpCodeHandle,
        Profile->TitleToken,
        0,
        EFI_IFR_NUMERIC_SIZE_4,
        Profile->ProfileIndex
        );
    }
  }

  if (Index == 0) {
    Status = EFI_NOT_FOUND;
    goto Error;
  }

  //
  // create priority list
  //
  HiiCreateOrderedListOpCode (
    StartOpCodeHandle,                          // Container for dynamic created opcodes
    DYNAMIC_PRIORITY_LIST_QUESTION_ID,          // Question ID
    MANAGER_VARSTORE_ID,                        // VarStore ID
    DYNAMIC_PRIORITY_LIST_VAR_OFFSET,           // Offset in Buffer Storage
    STRING_TOKEN (STR_PRIORITY_LIST),           // Question prompt text
    STRING_TOKEN (STR_PRIORITY_LIST_HELP),      // Question help text
    0,                                          // Question flag
    EFI_IFR_UNIQUE_SET,                         // Ordered list flag, e.g. EFI_IFR_UNIQUE_SET
    EFI_IFR_NUMERIC_SIZE_4,                     // Data type of Question value
    WIFI_MGR_MAX_PROFILE_NUM,                   // Maximum container
    OptionsOpCodeHandle,                        // Option Opcode list
    NULL                                        // Default Opcode is NULL
    );

  Status = HiiUpdateForm (
             Private->RegisteredHandle,         // HII handle
             &gWifiConfigFormSetGuid,           // Formset GUID
             FORMID_CHANGE_PRIORITY,            // Form ID
             StartOpCodeHandle,                 // Label for where to insert opcodes
             EndOpCodeHandle                    // Replace data
             );

Error:
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
  if (OptionsOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (OptionsOpCodeHandle);
  }

  return Status;
}

/**

  Restore the NV data to be default.

  @param[in]  Private             The pointer to the global private data structure.
  @param[out] IfrNvData           The IFR NV data.

**/
VOID
WifiMgrCleanUserInput (
  IN  WIFI_MGR_PRIVATE_DATA      *Private
  )
{
  Private->AutoConnect  = FALSE;
  Private->ScanAnyway   = FALSE;

  Private->SecurityType = SECURITY_TYPE_NONE;
  Private->EapAuthMethod  = EAP_AUTH_METHOD_TTLS;
  Private->EapSecondAuthMethod = EAP_SEAUTH_METHOD_MSCHAPV2;

  ZeroMem (Private->SSId, sizeof (Private->SSId));
  ZeroMem (Private->PasswordString, sizeof (Private->PasswordString));
  ZeroMem (Private->EapIdentity, sizeof (Private->EapIdentity));
  ZeroMem (Private->EapPasswordString, sizeof (Private->EapPasswordString));

  ZeroMem (Private->CACertName, sizeof (Private->CACertName));
  ZeroMem (Private->ClientCertName, sizeof (Private->ClientCertName));
  ZeroMem (&Private->CACertGuid, sizeof (EFI_GUID));
  ZeroMem (&Private->ClientCertGuid, sizeof (EFI_GUID));
  ZeroMem (Private->PrivateKeyName, sizeof (Private->PrivateKeyName));
  ZeroMem (Private->PrivateKeyPassword, sizeof (Private->PrivateKeyPassword));

  Private->FileType = FileTypeMax;

  Private->CurrentNic->UserSelectedProfile = NULL;
}

/**
  UI handle function when user select a network to connect.

  @param[in]  Private             The pointer to the global private data structure.
  @param[in]  ProfileIndex        The profile index user selected to connect.

  @retval EFI_INVALID_PARAMETER   Nic is null.
  @retval EFI_NOT_FOUND           Profile could not be found.
  @retval EFI_OUT_OF_RESOURCES    Do not have sufficient resource to finish this operation.
  @retval EFI_SUCCESS             The operation is completed successfully.

**/
EFI_STATUS
WifiMgrUserSelectProfileToConnect(
  IN     WIFI_MGR_PRIVATE_DATA         *Private,
  IN     UINT32                        ProfileIndex
  )
{
  WIFI_MGR_NETWORK_PROFILE_NVDATA  *Profile;
  WIFI_MGR_DEVICE_DATA             *Nic;

  Nic = Private->CurrentNic;
  if (Nic == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  //Initialize the connection page
  //

  WifiMgrCleanUserInput(Private);

  Profile = WifiMgrGetProfileByProfileIndex (ProfileIndex, &Nic->ProfileList);
  if (Profile == NULL) {
    return EFI_NOT_FOUND;
  }
  Private->CurrentNic->UserSelectedProfile = Profile;

  if (Profile->SecurityType == SECURITY_TYPE_WPA2_PERSONAL) {
    //
    // When user reenters the connection page, does not need to reconfigure password
    //
    StrCpyS (Private->PasswordString, PASSWORD_STORAGE_SIZE, Profile->Password);
  } else if (Profile->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE ) {

    Private->EapAuthMethod        = Profile->EapAuthMethod;
    StrCpyS (Private->EapIdentity, EAP_IDENTITY_SIZE, Profile->EapIdentity);
    StrCpyS (Private->EapPasswordString, PASSWORD_STORAGE_SIZE, Profile->EapPassword);

    if (Profile->EapAuthMethod == EAP_AUTH_METHOD_TTLS ||
      Profile->EapAuthMethod == EAP_AUTH_METHOD_PEAP) {

      CopyGuid(&Private->CACertGuid, &Profile->CACertGUID);
      StrCpyS(Private->CACertName, WIFI_FILENAME_STR_MAX_SIZE, Profile->CACertName);
      Private->EapSecondAuthMethod  = Profile->EapSecondAuthMethod;
    } else if (Profile->EapAuthMethod == EAP_AUTH_METHOD_TLS) {

      CopyGuid(&Private->CACertGuid, &Profile->CACertGUID);
      StrCpyS(Private->CACertName, WIFI_FILENAME_STR_MAX_SIZE, Profile->CACertName);
      CopyGuid(&Private->ClientCertGuid, &Profile->ClientCertGUID);
      StrCpyS(Private->ClientCertName, WIFI_FILENAME_STR_MAX_SIZE, Profile->ClientCertName);
      StrCpyS(Private->PrivateKeyName, WIFI_FILENAME_STR_MAX_SIZE, Profile->PrivateKeyName);
      StrCpyS(Private->PrivateKeyPassword, PASSWORD_STORAGE_SIZE, Profile->PrivateKeyPassword);
    }
  }

  return EFI_SUCCESS;
}

/**
  UI handle function when user select a network to manage.

  @param[in]  Private             The pointer to the global private data structure.
  @param[in]  ProfileIndex        The profile index user selected to manage.

  @retval EFI_INVALID_PARAMETER   Nic is null.
  @retval EFI_NOT_FOUND           Profile could not be found.
  @retval EFI_OUT_OF_RESOURCES    Do not have sufficient resource to finish this operation.
  @retval EFI_SUCCESS             The operation is completed successfully.

**/
EFI_STATUS
WifiMgrUserSelectProfileToManage(
  IN     WIFI_MGR_PRIVATE_DATA         *Private,
  IN     UINT32                        ProfileIndex
  )
{
  WIFI_MGR_NETWORK_PROFILE_NVDATA  *Profile;
  WIFI_MGR_DEVICE_DATA             *Nic;

  Nic = Private->CurrentNic;
  if (Nic == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  WifiMgrCleanUserInput(Private);

  Profile = WifiMgrGetProfileByProfileIndex (ProfileIndex, &Nic->ProfileList);

  if (Profile == NULL) {
    return EFI_NOT_FOUND;
  }

  Private->CurrentNic->UserSelectedProfile = Profile;

  Private->SecurityType = Profile->SecurityType;
  if (Profile->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE) {

    Private->EapAuthMethod        = Profile->EapAuthMethod;
    StrCpyS (Private->EapIdentity, EAP_IDENTITY_SIZE, Profile->EapIdentity);
    StrCpyS (Private->EapPasswordString, PASSWORD_STORAGE_SIZE, Profile->EapPassword);

    if (Profile->EapAuthMethod == EAP_AUTH_METHOD_TTLS ||
      Profile->EapAuthMethod == EAP_AUTH_METHOD_PEAP) {

      CopyGuid(&Private->CACertGuid, &Profile->CACertGUID);
      StrCpyS(Private->CACertName, WIFI_FILENAME_STR_MAX_SIZE, Profile->CACertName);
      Private->EapSecondAuthMethod  = Profile->EapSecondAuthMethod;
    } else if (Profile->EapAuthMethod == EAP_AUTH_METHOD_TLS) {

      CopyGuid(&Private->CACertGuid, &Profile->CACertGUID);
      StrCpyS(Private->CACertName, WIFI_FILENAME_STR_MAX_SIZE, Profile->CACertName);
      CopyGuid(&Private->ClientCertGuid, &Profile->ClientCertGUID);
      StrCpyS(Private->ClientCertName, WIFI_FILENAME_STR_MAX_SIZE, Profile->ClientCertName);
      StrCpyS(Private->PrivateKeyName, WIFI_FILENAME_STR_MAX_SIZE, Profile->PrivateKeyName);
      StrCpyS(Private->PrivateKeyPassword, PASSWORD_STORAGE_SIZE, Profile->PrivateKeyPassword);
    }
  }
  Private->AutoConnect  = Profile->AutoConnect;
  Private->ScanAnyway   = Profile->ScanAnyway;


  return EFI_SUCCESS;
}

/**
  Record password from a HII input string.

  @param[in]  Private             The pointer to the global private data structure.
  @param[in]  StringId            The QuestionId received from HII input.
  @param[in]  StringBuffer        The unicode string buffer to store password.
  @param[in]  StringBufferLen     The len of unicode string buffer.

  @retval EFI_INVALID_PARAMETER   Any input parameter is invalid.
  @retval EFI_NOT_FOUND           The password string is not found or invalid.
  @retval EFI_SUCCESS             The operation is completed successfully.

**/
EFI_STATUS
WifiMgrRecordPassword (
  IN   WIFI_MGR_PRIVATE_DATA      *Private,
  IN   EFI_STRING_ID              StringId,
  IN   CHAR16                     *StringBuffer,
  IN   UINTN                      StringBufferLen
  )
{
  CHAR16                          *Password;

  if (StringId == 0 || StringBuffer == NULL || StringBufferLen <= 0) {
    return EFI_INVALID_PARAMETER;
  }

  Password = HiiGetString (Private->RegisteredHandle, StringId, NULL);
  if (Password == NULL) {
    return EFI_NOT_FOUND;
  }
  if (StrLen (Password) > StringBufferLen) {
    FreePool (Password);
    return EFI_NOT_FOUND;
  }
  StrnCpyS (StringBuffer, StringBufferLen, Password, StrLen (Password));
  FreePool (Password);

  return EFI_SUCCESS;
}

/**
  Update a network profile to storage from HII input.

  @param[in]      Private          The pointer to the global private data structure.
  @param[in]      IfrNvData        The IFR NV data.
  @param[in, out] Profile          The network profile to update.

  @retval EFI_SUCCESS              The network profile is updated successfully.
  @retval EFI_INVALID_PARAMETER    Any input parameter is invalid.
  @retval EFI_OUT_OF_RESOURCES     Do not have sufficient resource to finish this operation.
  @retval Other Errors             Returned errors when updating profile to variable
                                   storage.

**/
EFI_STATUS
WifiMgrUpdateProfile (
  IN     WIFI_MGR_PRIVATE_DATA              *Private,
  IN     WIFI_MANAGER_IFR_NVDATA            *IfrNvData,
  IN OUT WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile
  )
{
  EFI_STATUS                        Status;
  EFI_TPL                           OldTpl;
  WIFI_MGR_NETWORK_PROFILE_NVDATA   *TempProfile;

  TempProfile = AllocateCopyPool(sizeof (WIFI_MGR_NETWORK_PROFILE_NVDATA), Profile);
  if (TempProfile == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status      = EFI_SUCCESS;
  OldTpl      = gBS->RaiseTPL (TPL_CALLBACK);
  
  Profile->AutoConnect         = IfrNvData->AutoConnect;
  Profile->ScanAnyway          = IfrNvData->ScanAnyway;

  if (Profile->SecurityType == SECURITY_TYPE_WPA2_PERSONAL) {
    if (StrLen(Private->PasswordString) > 0) {
      StrCpyS (Profile->Password, PASSWORD_STORAGE_SIZE, Private->PasswordString);
    }
  } else if (Profile->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE) {

    Profile->EapAuthMethod       = IfrNvData->EapAuthMethod;
    StrCpyS (Profile->EapIdentity, EAP_IDENTITY_SIZE, IfrNvData->EapIdentity);
    if (StrLen(Private->EapPasswordString) > 0) {
      StrCpyS (Profile->EapPassword, PASSWORD_STORAGE_SIZE, Private->EapPasswordString);
    }

    if (Profile->EapAuthMethod == EAP_AUTH_METHOD_PEAP ||
      Profile->EapAuthMethod == EAP_AUTH_METHOD_TTLS) {

      StrCpyS (Profile->CACertName, WIFI_FILENAME_STR_MAX_SIZE, Private->CACertName);
      CopyGuid (&Profile->CACertGUID, &Private->CACertGuid);
      Profile->EapSecondAuthMethod = IfrNvData->EapSecondAuthMethod;
    } else if (Profile->EapAuthMethod == EAP_AUTH_METHOD_TLS) {

      StrCpyS (Profile->CACertName, WIFI_FILENAME_STR_MAX_SIZE, Private->CACertName);
      CopyGuid (&Profile->CACertGUID, &Private->CACertGuid);
      StrCpyS (Profile->ClientCertName, WIFI_FILENAME_STR_MAX_SIZE, Private->ClientCertName);
      CopyGuid (&Profile->ClientCertGUID, &Private->ClientCertGuid);
      StrCpyS (Profile->PrivateKeyName, WIFI_FILENAME_STR_MAX_SIZE, Private->PrivateKeyName);
      if (StrLen(Private->PrivateKeyPassword) > 0) {
        StrCpyS (Profile->PrivateKeyPassword, PASSWORD_STORAGE_SIZE, Private->PrivateKeyPassword);
      }
    }
  }

  Status = WifiMgrWriteProfileDataToNv (Private->CurrentNic, Profile, FALSE);
  if (EFI_ERROR (Status)) {

    //
    // Save profile failed, recover from restored temp profile!
    //
    CopyMem (Profile, TempProfile, sizeof (WIFI_MGR_NETWORK_PROFILE_NVDATA));
  }
  FreePool (TempProfile);

  gBS->RestoreTPL (OldTpl);
  return Status;
}

/**
  Update the profile list by profiles' priorities.

  @param[in]      PriorityList       The priority list for all saved networks.
  @param[in]      PriorityListLen    The length of priority list.
  @param[in, out] ProfileList        The network profile list.

**/
VOID
WifiMgrUpdateProfileListByPriority (
  IN     UINT32                      *PriorityList,
  IN     UINT16                      PriorityListLen,
  IN OUT LIST_ENTRY                  *ProfileList
  )
{
  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile;
  WIFI_MGR_NETWORK_PROFILE_NVDATA    *TempProfile;
  UINT16                             Index;
  LIST_ENTRY                         *Entry;
  LIST_ENTRY                         *TempEntry;

  Index = 0;
  NET_LIST_FOR_EACH (Entry, ProfileList) {
    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);
    if (PriorityListLen <= Index) {
      break;
    }

    if (Profile->IsSaved == FALSE || PriorityList[Index] == Profile->ProfileIndex) {
      if (PriorityList[Index] == Profile->ProfileIndex) {
        Index ++;
      }
      continue;
    }

    NET_LIST_FOR_EACH_FROM_NODE (TempEntry, Entry, ProfileList) {
      TempProfile = NET_LIST_USER_STRUCT_S (TempEntry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                      Link, WIFI_MGR_PROFILE_SIGNATURE);
      if (TempProfile->ProfileIndex == PriorityList[Index]) {
        RemoveEntryList(TempEntry);
        TempEntry->ForwardLink        = Entry;
        TempEntry->BackLink           = Entry->BackLink;
        Entry->BackLink->ForwardLink  = TempEntry;
        Entry->BackLink               = TempEntry;

        Entry  = Entry->BackLink;
        break;
      }
    }
    Index ++;
  }
}

/**
  Save network profile from HII to storage.

  @param[in]      Private       The pointer to the global private data structure.
  @param[in]      IfrNvData     The IFR NV data.

  @retval EFI_SUCCESS              The network profile is updated successfully.
  @retval EFI_INVALID_PARAMETER    Any input parameter is invalid.
  @retval EFI_OUT_OF_RESOURCES     Do not have sufficient resource to finish this operation.

**/
EFI_STATUS
WifiMgrSaveNetworkProfile (
  IN  WIFI_MGR_PRIVATE_DATA      *Private,
  IN  WIFI_MANAGER_IFR_NVDATA    *IfrNvData
  )
{
  EFI_STATUS                         Status;
  EFI_TPL                            OldTpl;
  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile;

  if (IfrNvData == NULL || Private == NULL || 
    Private->CurrentNic == NULL || StrLen (IfrNvData->SSId) == 0) {
    
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_SUCCESS;
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);

  Profile = WifiMgrGetProfileByUnicodeSSId (IfrNvData->SSId,
              IfrNvData->SecurityType, &Private->CurrentNic->ProfileList);
  if (Profile == NULL) {
    //
    //Profile not in both saved list and available list, need to create
    //a new profile
    //
    if (Private->CurrentNic->ProfileCount >= WIFI_MGR_MAX_PROFILE_NUM) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }
    Profile = AllocateZeroPool (sizeof (WIFI_MGR_NETWORK_PROFILE_NVDATA));
    if (Profile == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }
    Profile->Signature    = WIFI_MGR_PROFILE_SIGNATURE;
    Profile->NicIndex     = Private->CurrentNic->NicIndex;
    Profile->IsAvailable  = FALSE;
    Profile->ProfileIndex = Private->CurrentNic->MaxProfileIndex + 1;
    StrCpyS (Profile->SSId, SSID_STORAGE_SIZE, IfrNvData->SSId);
    Profile->SecurityType = IfrNvData->SecurityType;

      //
      // Created a new network profile, add it to profile list.
      //
      Private->CurrentNic->ProfileCount ++;
      Private->CurrentNic->MaxProfileIndex ++;
      InsertTailList (&Private->CurrentNic->ProfileList, &Profile->Link);
    }

  Status = WifiMgrUpdateProfile (Private, IfrNvData, Profile);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  if (Profile->IsSaved == FALSE) {

    //
    // Add this network to saved network list
    //
    Profile->IsSaved = TRUE;
    Status = WifiMgrUpdatePriorityList (Private->CurrentNic);
    if (EFI_ERROR (Status)) {
      Profile->IsSaved = FALSE;
      goto Exit;
    }
  }

Exit:

  gBS->RestoreTPL (OldTpl);
  return Status;
}

/**
  Save priority list from HII to storage.

  @param[in]      Nic           The pointer to Nic on operate.
  @param[in]      IfrNvData     The IFR NV data.

  @retval EFI_SUCCESS           The priority list is saved successfully.
  @retval Other Errors          Returned errors when updating priority list to
                                variable storage.
**/
EFI_STATUS
WifiMgrSavePriorityList (
  IN  WIFI_MGR_DEVICE_DATA          *Nic,
  IN  WIFI_MANAGER_IFR_NVDATA       *IfrNvData
  )
{
  UINT32                            *PriorityList;
  EFI_TPL                           OldTpl;
  UINT16                            Index;
  EFI_STATUS                        Status;

  if (Nic == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = EFI_SUCCESS;
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  //
  // Update ProfileList sequence according to DynamicPriorityList
  //
  PriorityList = IfrNvData->DynamicPriorityList;
  Index = 0;

  while (PriorityList[Index] != 0) {
    Index ++;
  }

  WifiMgrUpdateProfileListByPriority(PriorityList, Index, &Nic->ProfileList);
  Status = WifiMgrUpdatePriorityList (Nic);

  gBS->RestoreTPL (OldTpl);
  return Status;
}

/**
  Delete a network profile from varibale storage.

  @param[in]      Nic                The pointer to Nic on operate.
  
  @retval EFI_SUCCESS                The network profile is deleted successfully.
  @retval EFI_NOT_FOUND
  @retval EFI_INVALID_PARAMETER      One or more input parameters are invalid.
  @retval Other Errors               Returned Errors when deleting the profile on
                                     variable storage or updating the priority list.
**/
EFI_STATUS
WifiMgrDeleteProfile (
  IN  WIFI_MGR_DEVICE_DATA           *Nic
  )
{
  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile;
  EFI_TPL                            OldTpl;
  EFI_STATUS                         Status;

  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);

  //
  // Delete network action can only be triggered by user
  //
  Profile = Nic->UserSelectedProfile;
  ASSERT (Profile != NULL);

  if (Profile == NULL) {
    return EFI_NOT_FOUND;
  }
  Status = WifiMgrWriteProfileDataToNv (Nic, Profile, TRUE);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  Profile->IsSaved = FALSE;
  if (Profile->SecurityType == SECURITY_TYPE_WPA2_PERSONAL) {
    ZeroMem (Profile->Password, sizeof (CHAR16) * PASSWORD_STORAGE_SIZE);
  }

  Status = WifiMgrUpdatePriorityList(Nic);

Exit:

  gBS->RestoreTPL (OldTpl);
  return Status;
}

/**
  Convert the driver configuration data into the IFR data.

  @param[in]   Private            The pointer to the global private data structure.
  @param[out]  IfrNvData          The IFR NV data.

  @retval EFI_SUCCESS             The operation is completed successfully.
**/
EFI_STATUS
WifiMgrConvertConfigDataToIfrNvData (
  IN   WIFI_MGR_PRIVATE_DATA      *Private,
  OUT  WIFI_MANAGER_IFR_NVDATA    *IfrNvData
  )
{
  //
  // Private shouldn't be NULL here, assert if Private is NULL.
  //
  ASSERT (Private != NULL);

  if (Private->CurrentNic != NULL) {
    IfrNvData->ProfileCount = Private->CurrentNic->AvailableCount;
  } else {
    IfrNvData->ProfileCount = 0;
  }

  return EFI_SUCCESS;
}

/**
  Convert the IFR data into the driver configuration data.

  @param[in]       Private             The pointer to the global private data structure.
  @param[in, out]  IfrNvData           The IFR NV data.

  @retval EFI_SUCCESS                  The operation is completed successfully.
**/
EFI_STATUS
WifiMgrConvertIfrNvDataToConfigData (
  IN     WIFI_MGR_PRIVATE_DATA         *Private,
  IN OUT WIFI_MANAGER_IFR_NVDATA       *IfrNvData
  )
{
  return EFI_SUCCESS;
}

/**

  This function allows the caller to request the current
  configuration for one or more named elements. The resulting
  string is in <ConfigAltResp> format. Any and all alternative
  configuration strings shall also be appended to the end of the
  current configuration string. If they are, they must appear
  after the current configuration. They must contain the same
  routing (GUID, NAME, PATH) as the current configuration string.
  They must have an additional description indicating the type of
  alternative configuration the string represents,
  "ALTCFG=<StringToken>". That <StringToken> (when
  converted from Hex UNICODE to binary) is a reference to a
  string in the associated string pack.

  @param This       Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.

  @param Request    A null-terminated Unicode string in
                    <ConfigRequest> format. Note that this
                    includes the routing information as well as
                    the configurable name / value pairs. It is
                    invalid for this string to be in
                    <MultiConfigRequest> format.
                    If a NULL is passed in for the Request field,
                    all of the settings being abstracted by this function
                    will be returned in the Results field.  In addition,
                    if a ConfigHdr is passed in with no request elements,
                    all of the settings being abstracted for that particular
                    ConfigHdr reference will be returned in the Results Field.

  @param Progress   On return, points to a character in the
                    Request string. Points to the string's null
                    terminator if request was successful. Points
                    to the most recent "&" before the first
                    failing name / value pair (or the beginning
                    of the string if the failure is in the first
                    name / value pair) if the request was not
                    successful.

  @param Results    A null-terminated Unicode string in
                    <MultiConfigAltResp> format which has all values
                    filled in for the names in the Request string.
                    String to be allocated by the called function.

  @retval EFI_SUCCESS             The Results string is filled with the
                                  values corresponding to all requested
                                  names.

  @retval EFI_OUT_OF_RESOURCES    Not enough memory to store the
                                  parts of the results that must be
                                  stored awaiting possible future
                                  protocols.

  @retval EFI_NOT_FOUND           Routing data doesn't match any
                                  known driver. Progress set to the
                                  first character in the routing header.
                                  Note: There is no requirement that the
                                  driver validate the routing data. It
                                  must skip the <ConfigHdr> in order to
                                  process the names.

  @retval EFI_INVALID_PARAMETER   Illegal syntax. Progress set
                                  to most recent "&" before the
                                  error or the beginning of the
                                  string.

  @retval EFI_INVALID_PARAMETER   Unknown name. Progress points
                                  to the & before the name in
                                  question.

**/
EFI_STATUS
EFIAPI
WifiMgrDxeHiiConfigAccessExtractConfig (
  IN CONST  EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST  EFI_STRING                      Request,
  OUT       EFI_STRING                      *Progress,
  OUT       EFI_STRING                      *Results
  )
{
  WIFI_MGR_PRIVATE_DATA             *Private;
  WIFI_MANAGER_IFR_NVDATA           *IfrNvData;
  EFI_STRING                        ConfigRequestHdr;
  EFI_STRING                        ConfigRequest;
  UINTN                             Size;
  BOOLEAN                           AllocatedRequest;
  UINTN                             BufferSize;
  EFI_STATUS                        Status;

  if (This == NULL || Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Request;
  if ((Request != NULL) &&
      !HiiIsConfigHdrMatch (Request, &gWifiConfigFormSetGuid, mVendorStorageName)) {
    return EFI_NOT_FOUND;
  }

  ConfigRequestHdr = NULL;
  ConfigRequest    = NULL;
  AllocatedRequest = FALSE;
  Size             = 0;

  Private   = WIFI_MGR_PRIVATE_DATA_FROM_CONFIG_ACCESS (This);

  BufferSize = sizeof (WIFI_MANAGER_IFR_NVDATA);
  IfrNvData = AllocateZeroPool (BufferSize);
  if (IfrNvData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  WifiMgrConvertConfigDataToIfrNvData (Private, IfrNvData);

  ConfigRequest = Request;
  if ((Request == NULL) || (StrStr (Request, L"OFFSET") == NULL)) {
    //
    // Request has no request element, construct full request string.
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator.
    //
    ConfigRequestHdr = HiiConstructConfigHdr (
                         &gWifiConfigFormSetGuid, 
                         mVendorStorageName, 
                         Private->DriverHandle);
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    ASSERT (ConfigRequest != NULL);
    AllocatedRequest = TRUE;
    UnicodeSPrint (
      ConfigRequest,
      Size,
      L"%s&OFFSET=0&WIDTH=%016LX",
      ConfigRequestHdr,
      (UINT64) BufferSize
      );
    FreePool (ConfigRequestHdr);
  }
  
  //
  // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
  //
  Status = gHiiConfigRouting->BlockToConfig (
                                gHiiConfigRouting,
                                ConfigRequest,
                                (UINT8 *) IfrNvData,
                                BufferSize,
                                Results,
                                Progress
                                );

  FreePool (IfrNvData);
  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
    ConfigRequest = NULL;
  }
  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = Request + StrLen (Request);
  }

  return Status;
}

/**

  This function applies changes in a driver's configuration.
  Input is a Configuration, which has the routing data for this
  driver followed by name / value configuration pairs. The driver
  must apply those pairs to its configurable storage. If the
  driver's configuration is stored in a linear block of data
  and the driver's name / value pairs are in <BlockConfig>
  format, it may use the ConfigToBlock helper function (above) to
  simplify the job.

  @param This           Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.

  @param Configuration  A null-terminated Unicode string in
                        <ConfigString> format.

  @param Progress       A pointer to a string filled in with the
                        offset of the most recent '&' before the
                        first failing name / value pair (or the
                        beginn ing of the string if the failure
                        is in the first name / value pair) or
                        the terminating NULL if all was
                        successful.

  @retval EFI_SUCCESS             The results have been distributed or are
                                  awaiting distribution.

  @retval EFI_OUT_OF_RESOURCES    Not enough memory to store the
                                  parts of the results that must be
                                  stored awaiting possible future
                                  protocols.

  @retval EFI_INVALID_PARAMETERS  Passing in a NULL for the
                                  Results parameter would result
                                  in this type of error.

  @retval EFI_NOT_FOUND           Target for the specified routing data
                                  was not found

**/
EFI_STATUS
EFIAPI
WifiMgrDxeHiiConfigAccessRouteConfig (
  IN CONST  EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST  EFI_STRING                      Configuration,
  OUT       EFI_STRING                      *Progress
  )
{
  EFI_STATUS                     Status;
  UINTN                          BufferSize;
  WIFI_MGR_PRIVATE_DATA          *Private;
  WIFI_MANAGER_IFR_NVDATA        *IfrNvData;

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  IfrNvData  = NULL;
  *Progress  = Configuration;
  BufferSize = sizeof (WIFI_MANAGER_IFR_NVDATA);
  Private    = WIFI_MGR_PRIVATE_DATA_FROM_CONFIG_ACCESS (This);

  //
  // Check routing data in <ConfigHdr>.
  // Note: if only one Storage is used, then this checking could be skipped.
  //
  if (!HiiIsConfigHdrMatch (Configuration, &gWifiConfigFormSetGuid, mVendorStorageName)) {
    return EFI_NOT_FOUND;
  }

  IfrNvData = AllocateZeroPool (BufferSize);
  if (IfrNvData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  WifiMgrConvertConfigDataToIfrNvData (Private, IfrNvData);

  Status = gHiiConfigRouting->ConfigToBlock (
                                gHiiConfigRouting,
                                Configuration,
                                (UINT8*) IfrNvData,
                                &BufferSize,
                                Progress
                                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = WifiMgrConvertIfrNvDataToConfigData (Private, IfrNvData);
  FreePool (IfrNvData);

  return Status;
}

extern WIFI_MGR_PRIVATE_DATA    *mPrivate;


VOID
UpdateConnectStatusStrToken (
  VOID
  )
{
}

VOID
UpdateEnrollCertNameStrToken (
  IN WIFI_MGR_PRIVATE_DATA              *Private,
  IN WIFI_MANAGER_IFR_NVDATA            *IfrNvData
  )
{
  //
  //Enter the key enrollment page
  //For TTLS and PEAP, only CA cert needs to be cared
  //
  if (Private->FileType == FileTypeCACert) {

    if (GetFromCertDatabase(EFI_WIFI_MGR_CERT_VARIABLE, &Private->CACertGuid, NULL) != NULL) {

      GuidToString(&Private->CACertGuid, IfrNvData->CertGuid, sizeof (IfrNvData->CertGuid));
      HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_EAP_ENROLLED_CERT_NAME),
        Private->CACertName, NULL);
    } else {

      HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_EAP_ENROLLED_CERT_NAME), L"", NULL);
      ZeroMem(IfrNvData->CertGuid, sizeof (IfrNvData->CertGuid));
    }
  } else if (Private->FileType == FileTypeClientCert) {

    if (GetFromCertDatabase(EFI_WIFI_MGR_CERT_VARIABLE, &Private->ClientCertGuid, NULL) != NULL) {

      GuidToString(&Private->ClientCertGuid, IfrNvData->CertGuid, sizeof (IfrNvData->CertGuid));
      HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_EAP_ENROLLED_CERT_NAME),
        Private->ClientCertName, NULL);
    } else {

      HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_EAP_ENROLLED_CERT_NAME), L"", NULL);
      ZeroMem(IfrNvData->CertGuid, sizeof (IfrNvData->CertGuid));
    }
  }
}

EFI_STATUS
DummyCleanScreen (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL   *This
  )
{
  return EFI_SUCCESS;
}

/**

  This function is called to provide results data to the driver.
  This data consists of a unique key that is used to identify
  which data is either being passed back or being asked for.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Action                 Specifies the type of action taken by the browser.
  @param  QuestionId             A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect. The format of the data tends to
                                 vary based on the opcode that generated the callback.
  @param  Type                   The type of value for the question.
  @param  Value                  A pointer to the data being sent to the original
                                 exporting driver.
  @param  ActionRequest          On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.
**/
EFI_STATUS
EFIAPI
WifiMgrDxeHiiConfigAccessCallback (
  IN     CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
  IN     EFI_BROWSER_ACTION                      Action,
  IN     EFI_QUESTION_ID                         QuestionId,
  IN     UINT8                                   Type,
  IN OUT EFI_IFR_TYPE_VALUE                      *Value,
  OUT    EFI_BROWSER_ACTION_REQUEST              *ActionRequest
  )
{
  EFI_STATUS                         Status;
  EFI_INPUT_KEY                      Key;
  EFI_GUID                           CertGuid;
  UINTN                              BufferSize;
  WIFI_MGR_PRIVATE_DATA              *Private;
  WIFI_MANAGER_IFR_NVDATA            *IfrNvData;
  EFI_DEVICE_PATH_PROTOCOL           *FilePath;
  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile;
  UINTN                              PrivateKeySize;
  CHAR16                             PrivateKeyVariableName[WIFI_MGR_MAX_MAC_STRING_LEN];

  if (Action != EFI_BROWSER_ACTION_FORM_OPEN &&
      Action != EFI_BROWSER_ACTION_FORM_CLOSE &&
      Action != EFI_BROWSER_ACTION_CHANGING &&
      Action != EFI_BROWSER_ACTION_CHANGED &&
      Action != EFI_BROWSER_ACTION_RETRIEVE) {

    return EFI_UNSUPPORTED;
  }
  if ((Value == NULL) || (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status  = EFI_SUCCESS;
  Private = WIFI_MGR_PRIVATE_DATA_FROM_CONFIG_ACCESS (This);
  if (Private->CurrentNic == NULL) {

    return EFI_DEVICE_ERROR;
  }

  //
  // Retrieve uncommitted data from Browser
  //
  BufferSize = sizeof (WIFI_MANAGER_IFR_NVDATA);
  IfrNvData = AllocateZeroPool (BufferSize);
  if (IfrNvData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  HiiGetBrowserData (&gWifiConfigFormSetGuid, mVendorStorageName, BufferSize, (UINT8 *) IfrNvData);

  if (Action == EFI_BROWSER_ACTION_FORM_OPEN) {
    switch (QuestionId) {

    case KEY_MAC_LIST:

      //
      //Enter the Mac selection page
      //
//[-start-180531-IB02950599-modify]//
      Status = WifiMgrInputAdminPassword(Private);
      if (Status == EFI_SUCCESS) {
        Status = WifiMgrShowNicList (Private);
      }
//[-end-180531-IB02950599-modify]//
      break;

    case KEY_NETWORK_LIST:

      //
      //Enter the WCM main page
      //
      Private->CurrentFormId = FORMID_WIFI_MAINPAGE;
      break;

    case KEY_REFRESH_NETWORK_LIST:

      //
      //Enter the available network list page
      //
      Private->CurrentFormId  = FORMID_NETWORK_LIST;
      break;

    case KEY_CONNECT_ACTION:

      //
      //Enter the network connection configuration page
      //Recovery from restored data
      //
      if (Private->CurrentNic->UserSelectedProfile != NULL) {

        Private->CurrentFormId = FORMID_CONNECT_NETWORK;
        Profile = Private->CurrentNic->UserSelectedProfile;

        if (HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_SSID),
          Profile->SSId, NULL) == 0) {
          return EFI_OUT_OF_RESOURCES;
        }
        IfrNvData->SecurityType = Profile->SecurityType;
        if (HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_SECURITY_TYPE),
          mSecurityType[IfrNvData->SecurityType], NULL) == 0) {
          return EFI_OUT_OF_RESOURCES;
        }

        if (IfrNvData->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE) {

          IfrNvData->EapAuthMethod        = Private->EapAuthMethod;
          IfrNvData->EapSecondAuthMethod  = Private->EapSecondAuthMethod;

          StrCpyS (IfrNvData->EapIdentity, EAP_IDENTITY_SIZE, Private->EapIdentity);
        }
      }
      break;

    case KEY_UPDATE_NETWORK_ACTION:

      //
      //Enter the network management page
      //Recovery from restored data
      //
      if (Private->CurrentNic->UserSelectedProfile != NULL) {

        Profile = Private->CurrentNic->UserSelectedProfile;
        StrCpyS (IfrNvData->SSId, SSID_STORAGE_SIZE, Profile->SSId);
        IfrNvData->SecurityType = Private->SecurityType;
        HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_SECURITY_TYPE),
          mSecurityType[IfrNvData->SecurityType], NULL);

        if (IfrNvData->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE) {

          IfrNvData->EapAuthMethod        = Private->EapAuthMethod;
          IfrNvData->EapSecondAuthMethod  = Private->EapSecondAuthMethod;
          StrCpyS (IfrNvData->EapIdentity, EAP_IDENTITY_SIZE, Private->EapIdentity);
        }
        IfrNvData->AutoConnect  = Private->AutoConnect;
        IfrNvData->ScanAnyway   = Private->ScanAnyway;
      }
      break;

    case KEY_ADD_NETWORK_ACTION:

      //
      //Enter the add new network page
      //Recovery from restored data
      //
      StrCpyS (IfrNvData->SSId, SSID_STORAGE_SIZE, Private->SSId);
      IfrNvData->SecurityType = Private->SecurityType;

      if (IfrNvData->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE) {

        IfrNvData->EapAuthMethod        = Private->EapAuthMethod;
        IfrNvData->EapSecondAuthMethod  = Private->EapSecondAuthMethod;
        StrCpyS (IfrNvData->EapIdentity, EAP_IDENTITY_SIZE, Private->EapIdentity);
      }
      IfrNvData->AutoConnect  = Private->AutoConnect;
      IfrNvData->ScanAnyway   = Private->ScanAnyway;

      break;

    case KEY_ENROLLED_CERT_NAME:
      //
      // If returned from file explorer, no need to check CertGuid to avoid clearing cert name.
      //
      if (mWifiMgrFileExplorerData.EnrollFormId != 0) {
        mWifiMgrFileExplorerData.EnrollFormId = 0;
        //continue;
        break;
      }
      //
      //Enter the key enrollment page
      //For TTLS and PEAP, only CA cert needs to be cared
      //
      if (Private->FileType == FileTypeCACert) {

        if (GetFromCertDatabase(EFI_WIFI_MGR_CERT_VARIABLE, &Private->CACertGuid, NULL) != NULL) {

          GuidToString(&Private->CACertGuid, IfrNvData->CertGuid, sizeof (IfrNvData->CertGuid));
          HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_EAP_ENROLLED_CERT_NAME),
            Private->CACertName, NULL);
        } else {

          HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_EAP_ENROLLED_CERT_NAME), L"", NULL);
          ZeroMem(IfrNvData->CertGuid, sizeof (IfrNvData->CertGuid));
        }
      } else if (Private->FileType == FileTypeClientCert) {

        if (GetFromCertDatabase(EFI_WIFI_MGR_CERT_VARIABLE, &Private->ClientCertGuid, NULL) != NULL) {

          GuidToString(&Private->ClientCertGuid, IfrNvData->CertGuid, sizeof (IfrNvData->CertGuid));
          HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_EAP_ENROLLED_CERT_NAME), 
            Private->ClientCertName, NULL);
        } else {

          HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_EAP_ENROLLED_CERT_NAME), L"", NULL);
          ZeroMem(IfrNvData->CertGuid, sizeof (IfrNvData->CertGuid));
        }
      }
      break;

    case KEY_ENROLLED_PRIVATE_KEY_NAME:

      if (StrLen (Private->PrivateKeyName) > 0) {
        UnicodeSPrint(
          PrivateKeyVariableName,
          sizeof (PrivateKeyVariableName),
          L"%s-%s",
          EFI_WIFI_MGR_PRIVATE_KEY_VARIABLE,
          Private->PrivateKeyName);

        PrivateKeySize = 0;
        Status = WifiMgrGetDataFromKMS (PrivateKeyVariableName, &PrivateKeySize, NULL);
        if (Status == EFI_BUFFER_TOO_SMALL) {
          
          HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_EAP_ENROLLED_PRIVATE_KEY_NAME), 
            Private->PrivateKeyName, NULL);
          StrCpyS (IfrNvData->PrivateKeyPassword, PASSWORD_STORAGE_SIZE, Private->PrivateKeyPassword);
        } else {

          HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_EAP_ENROLLED_PRIVATE_KEY_NAME), 
            L"", NULL);
          ZeroMem (IfrNvData->PrivateKeyPassword, sizeof (IfrNvData->PrivateKeyPassword));
        }
      }
      break;

    default:
      break;
    }
  } else if (Action == EFI_BROWSER_ACTION_FORM_CLOSE) {
    switch (QuestionId) {

    case KEY_NETWORK_LIST:

      //
      //Leave the WCM main page
      //
      Private->CurrentFormId = FORMID_NONE_FORM;
      break;

    case KEY_SAVE_PRIORITY:

      //
      //Quit the change priority page
      //
      ZeroMem (IfrNvData->DynamicPriorityList, sizeof (IfrNvData->DynamicPriorityList));
      Status = WifiMgrDisplaySavedList (Private);
      if (EFI_ERROR (Status)) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"ERROR: Fail to show the saved network list!",
          NULL
        );
      }
      break;

    case KEY_CONNECT_ACTION:

      //
      //Quit the network connection configuration page
      //
      if (Private->CurrentNic->UserSelectedProfile != NULL) {

        Private->CurrentFormId = FORMID_NONE_FORM;
        //
        //Restore User Config Data for Page recovery
        //
        if (IfrNvData->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE) {

          Private->EapAuthMethod        = IfrNvData->EapAuthMethod;
          Private->EapSecondAuthMethod  = IfrNvData->EapSecondAuthMethod;

          StrCpyS (Private->EapIdentity, EAP_IDENTITY_SIZE, IfrNvData->EapIdentity);
        }
      }
      break;

    case KEY_UPDATE_NETWORK_ACTION:

      //
      //Quit the network management page
      //
      if (Private->CurrentNic->UserSelectedProfile != NULL) {

        //
        //Restore User Config Data for Page recovery
        //
        Private->SecurityType = IfrNvData->SecurityType;
        if (IfrNvData->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE) {

          Private->EapAuthMethod        = IfrNvData->EapAuthMethod;
          Private->EapSecondAuthMethod  = IfrNvData->EapSecondAuthMethod;
          StrCpyS (Private->EapIdentity, EAP_IDENTITY_SIZE, IfrNvData->EapIdentity);
        }
        Private->AutoConnect  = IfrNvData->AutoConnect;
        Private->ScanAnyway   = IfrNvData->ScanAnyway;
      }
      break;

      case KEY_ADD_NETWORK_ACTION:

        //
        //Quit the add network page
        //Restore User Config Data for Page recovery
        //
        StrCpyS (Private->SSId, SSID_STORAGE_SIZE, IfrNvData->SSId);
        Private->SecurityType = IfrNvData->SecurityType;

        if (Private->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE) {

          Private->EapAuthMethod        = IfrNvData->EapAuthMethod;
          Private->EapSecondAuthMethod  = IfrNvData->EapSecondAuthMethod;
          StrCpyS (Private->EapIdentity, EAP_IDENTITY_SIZE, IfrNvData->EapIdentity);
        }
        Private->AutoConnect  = IfrNvData->AutoConnect;
        Private->ScanAnyway   = IfrNvData->ScanAnyway;

        break;

    default:
      break;
    }
  } else if (Action == EFI_BROWSER_ACTION_CHANGING) {
    switch (QuestionId) {

    case KEY_NETWORK_LIST:

      //
      //User triggered a scan process.
      //
      Private->CurrentNic->OneTimeScanRequest = TRUE;
      break;

    case KEY_PASSWORD_CONNECT_NETWORK:
    case KEY_PASSWORD_ADD_NETWROK:
    case KEY_PASSWORD_MANAGE_NETWORK:

      //
      //Network profile password is configured by user
      //
      Status = WifiMgrRecordPassword (Private, Value->string, Private->PasswordString, PASSWORD_STORAGE_SIZE);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Failed to set the password!"));
      }
      //
      // To indicate this password is not a new created password,
      // and do not need to confirm.
      //
      Status = EFI_NOT_FOUND;
      break;

    case KEY_EAP_PASSWORD_CONNECT_NETWORK:
    case KEY_EAP_PASSWORD_ADD_NETWORK:
    case KEY_EAP_PASSWORD_MANAGE_NETWORK:

      //
      //Network profile Eap password is configured by user
      //
      Status = WifiMgrRecordPassword (Private, Value->string, Private->EapPasswordString, PASSWORD_STORAGE_SIZE);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Failed to set the password!"));
      }
      //
      // To indicate this password is not a new created password,
      // and do not need to confirm.
      //
      Status = EFI_NOT_FOUND;
      break;

    case KEY_PRIVATE_KEY_PASSWORD:

      //
      //Private key password is configured by user
      //
      Status = WifiMgrRecordPassword (Private, Value->string, Private->PrivateKeyPassword, PASSWORD_STORAGE_SIZE);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Failed to set the password!"));
      }

      //
      // To indicate this password is not a new created password,
      // and do not need to confirm.
      //
      Status = EFI_NOT_FOUND;
      break;

    case KEY_FORGET_NETWORK_ACTION:

      //
      //User triggered delete network
      //
      Status = WifiMgrDeleteProfile (Private->CurrentNic);
      if (EFI_ERROR (Status)) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"ERROR: Fail to delete this network!",
          NULL
        );
      } else {
        WifiMgrDisplaySavedList(Private);
        //
        // If user delete network is currently connected, disconnect it
        //
        if (Private->CurrentNic->ConnectState == WifiMgrConnectedToAp &&
          Private->CurrentNic->UserSelectedProfile == Private->CurrentNic->CurrentOperateNetwork) {
          Private->CurrentNic->HasDisconnectPendingNetwork = TRUE;
        }
      }
      break;

    case KEY_SAVE_PRIORITY:

      //
      //User triggered save priority
      //
      Status = WifiMgrSavePriorityList (Private->CurrentNic, IfrNvData);
      if (EFI_ERROR (Status)) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"ERROR: Fail to save the priority list!",
          NULL
        );
      }
      break;

    case KEY_UPDATE_NETWORK_ACTION:

      if (Private->CurrentNic->UserSelectedProfile == NULL) {
        break;
      }

      //
      //User triggered save the managed network profile
      //
      if (StrLen (IfrNvData->SSId) < SSID_MIN_LEN) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"Can't update this network: SSId is Invalid!",
          NULL
          );
        break;
      }

      Profile = WifiMgrGetProfileByUnicodeSSId (IfrNvData->SSId, IfrNvData->SecurityType,
                  &Private->CurrentNic->ProfileList);

      if (Profile != NULL && Profile != Private->CurrentNic->UserSelectedProfile && 
        Profile->IsSaved) {

        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"Can't update this network: Network already exists!",
          NULL
          );
        break;
      } else if (Profile == Private->CurrentNic->UserSelectedProfile) {
        //
        // SSId and security type not changed, just update this network
        //
        Status = WifiMgrUpdateProfile (Private, IfrNvData, Private->CurrentNic->UserSelectedProfile);
        if (EFI_ERROR (Status)) {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"ERROR: Fail to save the network profile!",
            NULL
          );
        } else {
          WifiMgrDisplaySavedList(Private);
        }
      } else {
        //
        // SSId or security type has changed, created a new profile,
        // and delete the old one
        //
        Status = WifiMgrSaveNetworkProfile(Private, IfrNvData);
        if (EFI_ERROR (Status)) {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"ERROR: Failed to create this profile!",
            NULL
          );
          break;
        }
        
        Status = WifiMgrDeleteProfile (Private->CurrentNic);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_INFO, "Failed to delete the old network: %r\n", Status));
        } else {
          WifiMgrDisplaySavedList(Private);

          //
          // If deleted network is currently connected, disconnect it
          //
          if (Private->CurrentNic->ConnectState == WifiMgrConnectedToAp &&
            Private->CurrentNic->UserSelectedProfile == Private->CurrentNic->CurrentOperateNetwork) {
            Private->CurrentNic->HasDisconnectPendingNetwork = TRUE;
          }
        }
      }
      break;

    case KEY_CONNECT_ACTION:

      ASSERT (Private->CurrentNic->UserSelectedProfile != NULL);
      if (Private->CurrentNic->ConnectState != WifiMgrConnectedToAp||
        Private->CurrentNic->UserSelectedProfile != Private->CurrentNic->CurrentOperateNetwork) {

        //
        // When this network is not currently connected, pend it to connect.
        //
        if (Private->CurrentNic->UserSelectedProfile->AKMSuiteSupported == TRUE &&
          Private->CurrentNic->UserSelectedProfile->CipherSuiteSupported == TRUE) {

          if (Private->CurrentNic->UserSelectedProfile->SecurityType == SECURITY_TYPE_NONE) {

            //
            //For Open network, connect directly.
            //
            Private->CurrentNic->OneTimeConnectRequest = TRUE;
            Private->CurrentNic->ConnectPendingNetwork = Private->CurrentNic->UserSelectedProfile;
          } else if (Private->CurrentNic->UserSelectedProfile->SecurityType == SECURITY_TYPE_WPA2_PERSONAL) {

            //
            //For WPA/WPA2-Personal network, set password and connect.
            //
            StrCpyS (Private->CurrentNic->UserSelectedProfile->Password, PASSWORD_STORAGE_SIZE,
              Private->PasswordString);

            Private->CurrentNic->OneTimeConnectRequest = TRUE;
            Private->CurrentNic->ConnectPendingNetwork = Private->CurrentNic->UserSelectedProfile;
          } else if (Private->CurrentNic->UserSelectedProfile->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE) {

            //
            //For WPA/WPA2-Enterprise network, conduct eap configuration first.
            //Only EAP-TLS, TTLS and PEAP is supported now!
            //
            Private->CurrentNic->UserSelectedProfile->EapAuthMethod = IfrNvData->EapAuthMethod;
            StrCpyS (Private->CurrentNic->UserSelectedProfile->EapIdentity, EAP_IDENTITY_SIZE,
              IfrNvData->EapIdentity);
            StrCpyS (Private->CurrentNic->UserSelectedProfile->EapPassword, PASSWORD_STORAGE_SIZE,
              Private->EapPasswordString);

            if (IfrNvData->EapAuthMethod == EAP_AUTH_METHOD_TTLS ||
              IfrNvData->EapAuthMethod == EAP_AUTH_METHOD_PEAP) {

              CopyGuid (&Private->CurrentNic->UserSelectedProfile->CACertGUID, &Private->CACertGuid);
              StrCpyS (Private->CurrentNic->UserSelectedProfile->CACertName, WIFI_FILENAME_STR_MAX_SIZE,
                Private->CACertName);
              Private->CurrentNic->UserSelectedProfile->EapSecondAuthMethod = IfrNvData->EapSecondAuthMethod;

              Private->CurrentNic->OneTimeConnectRequest = TRUE;
              Private->CurrentNic->ConnectPendingNetwork = Private->CurrentNic->UserSelectedProfile;
            } else if (IfrNvData->EapAuthMethod == EAP_AUTH_METHOD_TLS) {

              CopyGuid (&Private->CurrentNic->UserSelectedProfile->CACertGUID, &Private->CACertGuid);
              StrCpyS (Private->CurrentNic->UserSelectedProfile->CACertName, WIFI_FILENAME_STR_MAX_SIZE, 
                Private->CACertName);
              CopyGuid (&Private->CurrentNic->UserSelectedProfile->ClientCertGUID, &Private->ClientCertGuid);
              StrCpyS (Private->CurrentNic->UserSelectedProfile->ClientCertName, WIFI_FILENAME_STR_MAX_SIZE, 
                Private->ClientCertName);
              StrCpyS (Private->CurrentNic->UserSelectedProfile->PrivateKeyName, WIFI_FILENAME_STR_MAX_SIZE, 
                Private->PrivateKeyName);
              StrCpyS (Private->CurrentNic->UserSelectedProfile->PrivateKeyPassword, PASSWORD_STORAGE_SIZE, 
                Private->PrivateKeyPassword);

              Private->CurrentNic->OneTimeConnectRequest = TRUE;
              Private->CurrentNic->ConnectPendingNetwork = Private->CurrentNic->UserSelectedProfile;
            } else {
              CreatePopUp (
                EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
                &Key,
                L"ERROR: Only EAP-TLS, TTLS or PEAP is supported now!",
                NULL
              );
            }
          } else {
            CreatePopUp (
              EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
              &Key,
              L"ERROR: Can't connect to this network!",
              NULL
            );
          }
        } else {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"ERROR: This network is not supported!",
            NULL
          );
        }
        
      } else {

        //
        // This network is currently connected, just disconnect from it.
        //
        Private->CurrentNic->OneTimeDisconnectRequest    = TRUE;
        Private->CurrentNic->HasDisconnectPendingNetwork = TRUE;
      }
      break;

    case KEY_ADD_NETWORK:

      //
      //Will forward to the add new network page
      //
      WifiMgrCleanUserInput(Private);
      HiiSetString (Private->RegisteredHandle, STRING_TOKEN (STR_EAP_ENROLLED_CERT_NAME), L"", NULL);

      break;

    case KEY_ADD_NETWORK_ACTION:

      //
      //User triggered an add network action, will save this new network
      //
      if (StrLen (IfrNvData->SSId) < SSID_MIN_LEN) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"Can't save this network: SSId is Invalid!",
          NULL
          );
        break;
      }

      if (IfrNvData->SecurityType == SECURITY_TYPE_WPA2_PERSONAL) {
        if (StrLen (Private->PasswordString) == 0) {
          //
          //Password is required for WPA2-Personal network
          //
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"Can't save this network: Password is required!",
            NULL
            );
          break;
        }
      }

      Status = WifiMgrSaveNetworkProfile(Private, IfrNvData);
      if (EFI_ERROR (Status)) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"ERROR: Fail to create this profile!",
          NULL
        );
      }
      break;

    case KEY_SAVED_LIST:

      Status = WifiMgrDisplaySavedList (Private);
      if (EFI_ERROR (Status)) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"ERROR: Fail to show the saved network list!",
          NULL
        );
      }
      break;

    case KEY_CHANGE_PRIORITY:

      Status = WifiMgrDisplayPriorityList (Private, IfrNvData);
      if (EFI_ERROR (Status)) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"ERROR: Fail to show the priority list!",
          NULL
        );
      }
      break;

    case KEY_ENROLL_CA_CERT_MANAGE_NETWORK:
    case KEY_ENROLL_CA_CERT_CONNECT_NETWORK:
    case KEY_ENROLL_CA_CERT_ADD_NETWORK:

      Private->FileType = FileTypeCACert;
      break;

    case KEY_ENROLL_CLIENT_CERT_MANAGE_NETWORK:
    case KEY_ENROLL_CLIENT_CERT_CONNECT_NETWORK:
    case KEY_ENROLL_CLIENT_CERT_ADD_NETWORK:

      Private->FileType = FileTypeClientCert;
      break;

    case KEY_EAP_ENROLL_PRIVATE_KEY_FROM_FILE:
      InitializeEnrollMenu (FORMID_ENROLL_PRIVATE_KEY);
#if 0
      FilePath = NULL;
      ChooseFile (NULL, NULL, NULL, &FilePath);

      if (FilePath != NULL) {
        UpdatePrivateKeyFromFile(Private, FilePath);
      }
#endif
      break;

    case KEY_EAP_ENROLL_CERT_FROM_FILE:
      InitializeEnrollMenu (FORMID_ENROLL_CERT);
#if 0
      //
      //User will select a cert file from File Explore
      //
      FilePath = NULL;
      ChooseFile( NULL, NULL, NULL, &FilePath);

      if (FilePath != NULL) {
        UpdateCAFromFile(Private, FilePath);
      }
#endif
      break;

    case KEY_SAVE_PRIVATE_KEY:

      if (Private->FileContext != NULL && Private->FileContext->FileName != NULL) {
        UnicodeSPrint(
          PrivateKeyVariableName,
          sizeof (PrivateKeyVariableName),
          L"%s-%s",
          EFI_WIFI_MGR_PRIVATE_KEY_VARIABLE,
          (CHAR16 *)Private->FileContext->FileName);

        Status = WifiMgrWritePrivateKeyToKMS (PrivateKeyVariableName,
                   Private->FileContext, Private->PrivateKeyPassword);
        if (EFI_ERROR (Status)) {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"ERROR: Can't enroll this private key file!",
            NULL
            );
        } else {
          StrCpyS(Private->PrivateKeyName, WIFI_FILENAME_STR_MAX_SIZE, Private->FileContext->FileName);
        }
      } else {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"ERROR: A private key file is needed!",
          NULL
          );
      }
      break;

    case KEY_SAVE_CERT:

      //
      //A cert file has been enrolled and will be saved
      //
      if (IfrNvData->CertGuid != NULL && Private->FileContext->FHandle) {

        Status = StringToGuid (
                   IfrNvData->CertGuid,
                   StrLen (IfrNvData->CertGuid),
                   &CertGuid
                   );
        if (EFI_ERROR (Status)) {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"ERROR: Cert GUID is not valid!",
            NULL
            );
          break;
        }

        if (GetFromCertDatabase(EFI_WIFI_MGR_CERT_VARIABLE, &CertGuid, NULL) != NULL) {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"ERROR: Cert GUID has exsited, choose another one!",
            NULL
            );
        } else {

          if (Private->FileType == FileTypeCACert) {

            Status = EnrollToCertDatabase(Private->FileContext, &CertGuid, EFI_WIFI_MGR_CERT_VARIABLE);
            if (EFI_ERROR (Status)) {
              CreatePopUp (
                EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
                &Key,
                L"ERROR: Failed to enroll CA cert!",
                NULL
                );
            } else {
              CopyGuid(&Private->CACertGuid, &CertGuid);
              StrCpyS(Private->CACertName, WIFI_FILENAME_STR_MAX_SIZE, Private->FileContext->FileName);
            }
          } else if (Private->FileType == FileTypeClientCert) {

            Status = EnrollToCertDatabase(Private->FileContext, &CertGuid, EFI_WIFI_MGR_CERT_VARIABLE);
            if (EFI_ERROR (Status)) {
              CreatePopUp (
                EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
                &Key,
                L"ERROR: Failed to enroll client cert!",
                NULL
                );
            } else {
              CopyGuid(&Private->ClientCertGuid, &CertGuid);
              StrCpyS(Private->ClientCertName, WIFI_FILENAME_STR_MAX_SIZE, Private->FileContext->FileName);
            }

          } else {

            CreatePopUp (
              EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
              &Key,
              L"ERROR: This Cert is not supported!",
              NULL
              );
          }
        }
      } else {

        if (IfrNvData->CertGuid == NULL) {

          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"ERROR: Please input a new cert guid!",
            NULL
            );
        } else {

          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"ERROR: Please enroll a new cert!",
            NULL
            );
        }
      }
      break;

    default:

      if (QuestionId >= KEY_MAC_ENTRY_BASE && QuestionId < KEY_MAC_ENTRY_BASE + Private->NicCount) {
        //
        // User selects a wireless NIC.
        //
        Status = WifiMgrSelectNic (Private, QuestionId);
        if (EFI_ERROR (Status)) {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"ERROR: Fail to operate the wireless NIC!",
            NULL
          );
        }
      } else if (QuestionId >= KEY_FILE_EXPLORER_FILE_OPTION_BASE && QuestionId <= KEY_FILE_EXPLORER_FILE_OPTION_END) {
        FilePath = NULL;
        if (UpdateEmFileExplorer (QuestionId, &FilePath)) {
          mWifiMgrFileExplorerData.EmDisplayContext = EM_UNKNOWN_CONTEXT;
          UpdateEmFileExplorer (0, NULL);
        } else {
          if (FilePath != NULL) {
            if (mWifiMgrFileExplorerData.EnrollFormId == FORMID_ENROLL_CERT) {
              UpdateCAFromFile(Private, FilePath);
            } else if (mWifiMgrFileExplorerData.EnrollFormId == FORMID_ENROLL_PRIVATE_KEY) {
              UpdatePrivateKeyFromFile(Private, FilePath);
            }
          }
        }
      } else if (Private->CurrentNic != NULL) {
        if (QuestionId >= KEY_SAVED_NETWORK_ENTRY_BASE &&
              QuestionId <= KEY_SAVED_NETWORK_ENTRY_BASE + Private->CurrentNic->MaxProfileIndex) {

          Status = WifiMgrUserSelectProfileToManage (Private, QuestionId - KEY_SAVED_NETWORK_ENTRY_BASE);
        } else if (QuestionId >= KEY_AVAILABLE_NETWORK_ENTRY_BASE &&
                     QuestionId <= KEY_AVAILABLE_NETWORK_ENTRY_BASE + Private->CurrentNic->MaxProfileIndex) {

          Status = WifiMgrUserSelectProfileToConnect (Private, QuestionId - KEY_AVAILABLE_NETWORK_ENTRY_BASE);
        }

        if (EFI_ERROR (Status)) {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"ERROR: Fail to operate this profile!",
            NULL
          );
        }
      }

      break;
    }
  } else if (Action == EFI_BROWSER_ACTION_CHANGED) {
    switch (QuestionId) {

    case KEY_ADD_NETWORK_ACTION:
    case KEY_UPDATE_NETWORK_ACTION:
    case KEY_SAVE_PRIORITY:
    case KEY_FORGET_NETWORK_ACTION:
    case KEY_SAVE_CERT:

      *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_SUBMIT_EXIT;
      break;

    case KEY_SAVE_PRIVATE_KEY:
    case KEY_NO_SAVE_CERT:
    case KEY_NO_SAVE_PRIVATE_KEY:

      *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_DISCARD_EXIT;
      break;

    default:
      break;
    }
  } else if (Action == EFI_BROWSER_ACTION_RETRIEVE) {

    switch (QuestionId) {

    case KEY_REFRESH_NETWORK_LIST:

      WifiMgrRefreshNetworkList (Private, IfrNvData);
      break;

    case KEY_REFRESH_TITLE_CONNECTION_STATUS:
      
      if (Private->CurrentNic->ConnectState == WifiMgrConnectedToAp) {
        ASSERT (Private->CurrentNic->CurrentOperateNetwork != NULL);

        HiiSetString (Private->RegisteredHandle,
          STRING_TOKEN (STR_CONNECTION_INFO), L"Connected to", NULL);
        HiiSetString (Private->RegisteredHandle,
          STRING_TOKEN (STR_CONNECTED_SSID), Private->CurrentNic->CurrentOperateNetwork->SSId, NULL);
      } else {

        HiiSetString (Private->RegisteredHandle,
          STRING_TOKEN (STR_CONNECTION_INFO), L"Disconnected", NULL);
        HiiSetString (Private->RegisteredHandle,
          STRING_TOKEN (STR_CONNECTED_SSID), L"", NULL);
      }
      break;

    case KEY_REFRESH_CONNECT_CONFIGURATION:

      if (Private->CurrentNic->UserSelectedProfile == NULL) {
        break;
      }
      if (Private->CurrentNic->ConnectState == WifiMgrConnectedToAp) {
        ASSERT (Private->CurrentNic->CurrentOperateNetwork != NULL);
        
        if (Private->CurrentNic->CurrentOperateNetwork == Private->CurrentNic->UserSelectedProfile) {

          //
          // This network is currently connected!
          //
          HiiSetString (Private->RegisteredHandle,
            STRING_TOKEN (STR_CONNECT_NOW), L"Disconnect from this network", NULL);
        } else {

          //
          // This network is not the one currently connected!
          //
          HiiSetString (Private->RegisteredHandle,
            STRING_TOKEN (STR_CONNECT_NOW), L"Connect to this network", NULL);
        }
      } else if (Private->CurrentNic->ConnectState == WifiMgrDisconnected){

        //
        // No network is currently connected!
        //
        HiiSetString (Private->RegisteredHandle,
          STRING_TOKEN (STR_CONNECT_NOW), L"Connect to this network", NULL);
      } else if (Private->CurrentNic->ConnectState == WifiMgrConnectingToAp) {
        ASSERT (Private->CurrentNic->CurrentOperateNetwork != NULL);

        if (Private->CurrentNic->CurrentOperateNetwork == Private->CurrentNic->UserSelectedProfile) {

          //
          // This network is in connecting!
          //
          HiiSetString (Private->RegisteredHandle,
            STRING_TOKEN (STR_CONNECT_NOW), L"Connecting ...", NULL);
        } else {

          //
          // This network is not the one currently in connecting!
          //
          HiiSetString (Private->RegisteredHandle,
            STRING_TOKEN (STR_CONNECT_NOW), L"Connect to this network", NULL);
        }
      } else if (Private->CurrentNic->ConnectState == WifiMgrDisconnectingToAp) {
        ASSERT (Private->CurrentNic->CurrentOperateNetwork != NULL);

        if (Private->CurrentNic->CurrentOperateNetwork == Private->CurrentNic->UserSelectedProfile) {

          //
          // This network is in disconnecting!
          //
          HiiSetString (Private->RegisteredHandle,
            STRING_TOKEN (STR_CONNECT_NOW), L"Disconnecting ...", NULL);
        } else {

          //
          // This network is not the one currently in disconnecting!
          //
          HiiSetString (Private->RegisteredHandle,
            STRING_TOKEN (STR_CONNECT_NOW), L"Connect to this network", NULL);
        }
      }
      break;

    default:

      break;
    }
  }

  if (!EFI_ERROR (Status)) {
    //
    // Pass changed uncommitted data back to Form Browser.
    //
    BufferSize = sizeof (WIFI_MANAGER_IFR_NVDATA);
    HiiSetBrowserData (&gWifiConfigFormSetGuid, mVendorStorageName, BufferSize, (UINT8 *) IfrNvData, NULL);
  }

  FreePool (IfrNvData);
  return Status;
}

/**
  Initialize the WiFi configuration form.

  @param[in]  Private             The pointer to the global private data structure.

  @retval EFI_SUCCESS             The configuration form is initialized.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
  @retval EFI_INVALID_PARAMETER   Any input parameter is invalid.
  @retval Other Erros             Returned Errors when installing protocols.

**/
EFI_STATUS
WifiMgrDxeConfigFormInit (
  WIFI_MGR_PRIVATE_DATA    *Private
)
{
  EFI_STATUS                      Status;

  if (Private == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Private->ConfigAccess.ExtractConfig = WifiMgrDxeHiiConfigAccessExtractConfig;
  Private->ConfigAccess.RouteConfig   = WifiMgrDxeHiiConfigAccessRouteConfig;
  Private->ConfigAccess.Callback      = WifiMgrDxeHiiConfigAccessCallback;

  //
  // Install Device Path Protocol and Config Access protocol to driver handle.
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Private->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mWifiMgrDxeHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &Private->ConfigAccess,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Publish our HII data.
  //
  Private->RegisteredHandle = HiiAddPackages (
                                &gWifiConfigFormSetGuid,
                                Private->DriverHandle,
                                WifiConnectionManagerDxeStrings,
                                WifiConnectionManagerDxeBin,
                                NULL
                                );
  if (Private->RegisteredHandle == NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           Private->DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &mWifiMgrDxeHiiVendorDevicePath,
           &gEfiHiiConfigAccessProtocolGuid,
           &Private->ConfigAccess,
           NULL
           );
    return EFI_OUT_OF_RESOURCES;
  }

  Private->FileContext = AllocateZeroPool (sizeof (WIFI_MGR_FILE_CONTEXT));
  if (Private->FileContext == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  WifiMgrFileExplorerInit (Private);
  return EFI_SUCCESS;
}

/**
  Unload the WiFi configuration form.

  @param[in]  Private      The pointer to the global private data structure.

  @retval EFI_SUCCESS             The configuration form is unloaded successfully.
  @retval EFI_INVALID_PARAMETER   Any input parameter is invalid.
  @retval Other Errors            Returned Erros when uninstalling protocols.
**/
EFI_STATUS
WifiMgrDxeConfigFormUnload (
  WIFI_MGR_PRIVATE_DATA    *Private
)
{
  EFI_STATUS    Status;

  if (Private == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Private->FileContext != NULL) {
    if (Private->FileContext->FileName != NULL) {
      FreePool (Private->FileContext->FileName);
    }
    FreePool (Private->FileContext);
  }

  HiiRemovePackages(Private->RegisteredHandle);

  Status = gBS->UninstallMultipleProtocolInterfaces (
             Private->DriverHandle,
             &gEfiDevicePathProtocolGuid,
             &mWifiMgrDxeHiiVendorDevicePath,
             &gEfiHiiConfigAccessProtocolGuid,
             &Private->ConfigAccess,
             NULL
             );

  return Status;
}



