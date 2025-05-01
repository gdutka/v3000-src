/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
 The Mac Connection2 Protocol adapter functions for wireless connection manager.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#ifndef __EFI_WIFI_IMPL__
#define __EFI_WIFI_IMPL__

/**
  Start scan operation, and send out a token to collect available networks.

  @param[in]  Nic                 Pointer to the device data of the selected NIC.

  @retval EFI_SUCCESS             The operation is completed.
  @retval EFI_ALREADY_STARTED     A former scan operation is already ongoing.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
  @retval Other Errors            Return errors when getting networks from low layer.

**/
EFI_STATUS
WifiMgrStartScan (
  IN      WIFI_MGR_DEVICE_DATA        *Nic
  );

/**
  Get current link state from low layer.

  @param[in]   Nic                Pointer to the device data of the selected NIC.
  @param[out]  LinkState          The pointer to buffer to retrieve link state.

  @retval EFI_SUCCESS             The operation is completed.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_UNSUPPORTED         Adapter information protocol is not supported.
  @retval Other Errors            Returned errors when retrieving link state from low layer.

**/
EFI_STATUS
WifiMgrGetLinkState (
  IN    WIFI_MGR_DEVICE_DATA       *Nic,
  OUT   EFI_ADAPTER_INFO_MEDIA_STATE  *LinkState
  );

/**
  Start connect operation, and send out a token to connect to a target network.

  @param[in]  Nic                 Pointer to the device data of the selected NIC.
  @param[in]  Profile             The target network to be connected.

  @retval EFI_SUCCESS             The operation is completed.
  @retval EFI_ALREADY_STARTED     Already in "connected" state, need to perform a disconnect
                                  operation first.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
  @retval Other Errors            Return errors when connecting network on low layer.

**/
EFI_STATUS
WifiMgrConnectToNetwork (
  IN    WIFI_MGR_DEVICE_DATA              *Nic,
  IN    WIFI_MGR_NETWORK_PROFILE_NVDATA   *Profile
  );

/**
  Start disconnect operation, and send out a token to disconnect from current connected
  network.

  @param[in]  Nic                 Pointer to the device data of the selected NIC.

  @retval EFI_SUCCESS             The operation is completed.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
  @retval Other Errors            Return errors when disconnecting a network on low layer.

**/
EFI_STATUS
WifiMgrDisconnectToNetwork (
  IN    WIFI_MGR_DEVICE_DATA              *Nic
  );

/**
  The state machine of the connection manager, periodically check the state and
  perform a corresponding operation.

  @param[in]  Event                   The timer event to be triggered.
  @param[in]  Context                 The context of the Nic device data.

**/
VOID
EFIAPI
WifiMgrOnTimerTick (
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  );

#endif
