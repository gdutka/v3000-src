/** @file
  REST protocol implementation.

;******************************************************************************
;* Copyright (c) 2012 - 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "BdsCpUpdateMacAddress.h"

CHAR16   mModifyMacAddrVarString[] = L"ModifyMacAddrVarString";

BOOLEAN
CompareMacAddress (
  IN  EFI_MAC_ADDRESS            *Addr1,
  IN  EFI_MAC_ADDRESS            *Addr2,
  IN  UINTN                      Length
  )
{
  UINTN                          Index;
  
  for (Index = 0; Index < Length; Index++) {
    if (Addr1->Addr[Index] != Addr2->Addr[Index]) {
      return FALSE;
    }
  }
  return TRUE;
}

VOID
BdsCpUpdateMacAddressCallback (
  IN EFI_EVENT         Event,
  IN H2O_CP_HANDLE     Handle
  )
{
  EFI_STATUS                               Status;
  H2O_BDS_CP_NETWORK_UPDATE_MAC_ADDR_DATA  *BdsNetworkUpdateMacAddrData;
  EFI_DEVICE_PATH_PROTOCOL                 *UndiDevicePath;
  EFI_DEVICE_PATH_PROTOCOL                 *TempDevicePath;
  MAC_ADDR_DEVICE_PATH                     *MacAddrDevicePath;
  EFI_MAC_ADDRESS                          *MacAddress;

  MAC_ADDR_VARIABLE                        *ModifyMacAddrVariable;
  UINTN                                    ModifyMacAddrVariableSize;
  UINTN                                    PtrOffset;
  UINTN                                    MacAddrLength;
  UINT8                                    *OriginalMacAddr;
  UINT8                                    *ModifyMacAddr;

  Status = GetVariable2 (
             mModifyMacAddrVarString,
             &gH2OBdsCpNetworkUpdateMacAddrGuid,
             (VOID **) &ModifyMacAddrVariable,
             &ModifyMacAddrVariableSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "No MAC address modify request\n"));
    return;
  }
  
  //
  // Find out the modify MAC address
  //
  Status = H2OCpLookup (Handle, (VOID **) &BdsNetworkUpdateMacAddrData, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Checkpoint Data Not Found: %x (%r)\n", Handle, Status));
    DEBUG ((DEBUG_ERROR, "   %a\n", __FUNCTION__));
    goto ON_EXIT;
  }
  
  //
  // Check UNDI device to get MAC address.
  //
  Status = gBS->HandleProtocol (BdsNetworkUpdateMacAddrData->NicHandle, &gEfiDevicePathProtocolGuid, (VOID **) &UndiDevicePath);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "DevicePath not found in Handle: %x (%r)\n", Handle, Status));
    goto ON_EXIT;
  }
  
  MacAddrDevicePath = NULL;
  TempDevicePath    = UndiDevicePath;
  while (!IsDevicePathEndType (TempDevicePath)) {
    if ((DevicePathType (TempDevicePath) == MESSAGING_DEVICE_PATH) && ( DevicePathSubType (TempDevicePath) == MSG_MAC_ADDR_DP)) {
      MacAddrDevicePath = (MAC_ADDR_DEVICE_PATH *) TempDevicePath;
      break;
    }
    TempDevicePath = NextDevicePathNode (TempDevicePath);
  }
  
  if (MacAddrDevicePath == NULL) {
    DEBUG ((DEBUG_ERROR, "MAC DevicePath not found in DevicePath: %x (%r)\n", UndiDevicePath, Status));
    goto ON_EXIT;
  }
  
  //
  // Compare MAC address with variable
  //   structure : [MAC SIZE] [original MAC : array of MAC SIZE] [Modify MAC : array of MAC SIZE]
  //   example   : 06          88 88 88 88 87 88                  90 49 FA 55 66 00
  //               06          00 0E C6 AA 53 52                  00 0E C6 AA 55 66
  //               00 (END)
  //
  PtrOffset = 0;
  while (PtrOffset < ModifyMacAddrVariableSize) {
    if (ModifyMacAddrVariable->MacAddrLength == 0) break;

    MacAddrLength = ModifyMacAddrVariable->MacAddrLength;

    if ((PtrOffset + sizeof (MAC_ADDR_VARIABLE) + (MacAddrLength * 2)) > ModifyMacAddrVariableSize) break;

    OriginalMacAddr = (VOID *)((UINTN)ModifyMacAddrVariable + 1);
    ModifyMacAddr   = (VOID *)((UINTN)ModifyMacAddrVariable + 1 + MacAddrLength);
    PtrOffset += sizeof (MAC_ADDR_VARIABLE) + (MacAddrLength * 2);
    ModifyMacAddrVariable = ModifyMacAddrVariable + sizeof (MAC_ADDR_VARIABLE) + (MacAddrLength * 2);

    MacAddress = &MacAddrDevicePath->MacAddress;
    if (!CompareMacAddress (MacAddress, (EFI_MAC_ADDRESS *) OriginalMacAddr, MacAddrLength)) {
      continue;
    }
    BdsNetworkUpdateMacAddrData->Status     = H2O_CP_TASK_UPDATE;
    CopyMem (&BdsNetworkUpdateMacAddrData->NicMacAddr, ModifyMacAddr, MacAddrLength);
    goto ON_EXIT;
  }
  
ON_EXIT:
  if (ModifyMacAddrVariable != NULL) {
    FreePool (ModifyMacAddrVariable);
  }
  return;
}

/**
  This function is sample to update MAC addresses.
  Modify addresses defined in variable ModifyMacAddrVarString:gH2OBdsCpNetworkUpdateMacAddrGuid

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.

**/
EFI_STATUS
EFIAPI
BdsCpUpdateMacAddressDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS           Status;
  H2O_CP_HANDLE        CpHandle;

  Status = EFI_SUCCESS;
  if (FeaturePcdGet (PcdH2OBdsCpNetworkUpdateMacAddrSupported)) {
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpNetworkUpdateMacAddrGuid,
               BdsCpUpdateMacAddressCallback,
               H2O_CP_LOW,
               &CpHandle
               );
  }
  return Status;
}