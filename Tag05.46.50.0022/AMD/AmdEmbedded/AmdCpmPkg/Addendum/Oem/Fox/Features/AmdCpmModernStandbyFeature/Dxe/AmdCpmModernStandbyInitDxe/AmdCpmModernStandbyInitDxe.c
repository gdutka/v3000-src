/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>
#include <Protocol/PciIo.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <RMB/ApcbV3TokenUid.h>
#include <RMB/ApcbV3Priority.h>
#include <Protocol/AmdApcbProtocol.h>

#define PCI_CLASS_MASS_STORAGE_NVM     0x08   // mass storage sub-class non-volatile memory
#define PCI_IF_NVMHCI                  0x02   // mass storage programming interface NVMHCI
#define NVME_CLASS_CODE                (UINT32) ((PCI_CLASS_MASS_STORAGE << 24)     | \
                                                 (PCI_CLASS_MASS_STORAGE_NVM << 16) | \
                                                 (PCI_IF_NVMHCI <<8 ))  //0x01080200

#define QCOM_VENDOR_ID                 0x17CB
#define QCOM_DEVICE_NFA765_ID          0x1103

SCREENON_CONTROL_LIST ScreenOnGpioList = {
  1,    //Version
  1,    //Flag
  {
    3, //NumOfGpio
    { //GPIO PIns
      0,   //pWR_BUTTON
      58,  //Int_usb_xhc_0_acpi_pme
      59,  //Int_usb_xhc_1_acpi_pme
    }
  }
};

EFI_GUID AMD_UPEP_SSDT_GUID = {
  0x2d799fea, 0x7998, 0x4c23, 0xb0, 0x78, 0x97, 0xbe, 0x1, 0x28, 0x2d, 0xa2
};

EFI_GUID AMD_GPP0_SSDT_GUID = {
  0x55537793, 0xfdb1, 0x46e7, 0xa4, 0x8c, 0x7d, 0x07, 0x36, 0xbd, 0x26, 0xeb
};

EFI_GUID AMD_NVME_D3_SSDT_GUID = {
  0x04105aad, 0xf74d, 0x464c, 0x96, 0x85, 0xcb, 0x0b, 0x78, 0x29, 0xff, 0x29
};

EFI_GUID AMD_NVME_RTD3_SSDT_GUID = {
  0xb56f6f6e, 0x71d8, 0x47ab, 0xa2, 0x9b, 0xb3, 0xec, 0xf3, 0x95, 0x4a, 0xa9
};

EFI_GUID AMD_GPPMS_SSDT_GUID = {
  0xAC0BDF5C, 0x8D30, 0x45BE, 0x98, 0xE7, 0x5F, 0xEE, 0x43, 0xD6, 0x8F, 0xA4
};

VOID
EFIAPI
AmdCpmOemUpepSsdtInstall (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  );

VOID
EFIAPI
AmdCpmOemGpp0SsdtInstall (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  );

VOID
EFIAPI
AmdCpmOemGpp1SsdtInstall (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  );

BOOLEAN
EFIAPI
CheckNvmeRtd3Device (
  IN AMD_CPM_NVME_RTD3_TABLE  *NvmeRtd3TablePtr,
  IN UINT8                    RpDevice,
  IN UINT8                    RpFunction
  );

BOOLEAN
EFIAPI
CheckNvmeDevice (
  IN UINT8 RpDevice,
  IN UINT8 RpFunction
  );

VOID
EFIAPI
AmdCpmNvmeD3SsdtInstall (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  );

VOID
EFIAPI
AmdCpmOemGppMsSsdtInstall (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  );

BOOLEAN
EFIAPI
AmdCpmUpepSsdtCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  );

BOOLEAN
EFIAPI
AmdCpmGpp0CallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  );

BOOLEAN
EFIAPI
AmdCpmNvmeD3CallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  );

BOOLEAN
EFIAPI
AmdCpmGppMsCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  );

BOOLEAN
EFIAPI
AmdCpmOemGpioSsdtCallback (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  );

/*----------------------------------------------------------------------------------------*/
/**
 * The function update APCB
 *
 * @param[in]   ApcbDxeServiceProtocol   located AMD_APCB_SERVICE_PROTOCOL
 *
 */
VOID
UpdateModernStandbyEnableApcb (
  IN AMD_APCB_SERVICE_PROTOCOL   *ApcbDxeServiceProtocol
  )
{
  EFI_STATUS                      Status;
  UINT8                           ApcbPurpose;
  BOOLEAN                         PcdValue;
  UINT8                           Value;
  UINT8                           IsConfigChanged;

  Status = ApcbDxeServiceProtocol->ApcbAcquireMutex (ApcbDxeServiceProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "ApcbAcquireMutex failed, Status:%r\n", Status));
    return;
  }

  IsConfigChanged = FALSE;
  PcdValue = PcdGetBool (PcdModernStandbyEnable);
  Status = ApcbDxeServiceProtocol->ApcbGetTokenBool (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_MODERN_STANDBY_ENABLE, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "1. Unable to get APCB token APCB_TOKEN_UID_MODERN_STANDBY_ENABLE, Status:%r\n", Status));
  } else {
    if (PcdValue != Value) {
      DEBUG ((DEBUG_INFO, "PcdValue: %d, Value: %d, update in APCB\n", PcdValue, Value));
      Status = ApcbDxeServiceProtocol->ApcbSetTokenBool (ApcbDxeServiceProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MODERN_STANDBY_ENABLE, PcdValue);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Unable to set APCB token APCB_TOKEN_UID_MODERN_STANDBY_ENABLE, Status:%r\n", Status));
      } else {
        Status = ApcbDxeServiceProtocol->ApcbGetTokenBool (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_MODERN_STANDBY_ENABLE, &Value);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_INFO, "2. Unable to get APCB token APCB_TOKEN_UID_MODERN_STANDBY_ENABLE, Status:%r\n", Status));
        } else {
          if (PcdValue == Value) {
            // Update successfully
            IsConfigChanged = TRUE;
          } else {
            // Update failed
            DEBUG ((DEBUG_INFO, "Critial error: update to APCB token APCB_TOKEN_UID_MODERN_STANDBY_ENABLE doesn't take effect\n"));
          }
        }
      }
    }
  }

  if (IsConfigChanged) {
    DEBUG ((DEBUG_INFO, "IsConfigChanged = TRUE\n"));
    // Write back updated data to SPI
    ApcbDxeServiceProtocol->ApcbFlushData (ApcbDxeServiceProtocol);
  }

  ApcbDxeServiceProtocol->ApcbReleaseMutex (ApcbDxeServiceProtocol);

  if (IsConfigChanged) {
    DEBUG ((DEBUG_INFO, "Issue warm reset since APCB changed...\n"));
    gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
    // Should not come here
    CpuDeadLoop ();
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * The callback function AMD_APCB_SERVICE_PROTOCOL ready and the PSP can flush data.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
ApcbAccReadyCallBack (
  IN EFI_EVENT                   Event,
  IN VOID                        *Context
  )
{
  EFI_STATUS                     Status;
  AMD_APCB_SERVICE_PROTOCOL      *ApcbDxeServiceProtocol;

  DEBUG ((DEBUG_INFO, "CPM %a\n", __FUNCTION__));

  // Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &ApcbDxeServiceProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Unable to locate APCB Protocol"));
    return;
  }

  UpdateModernStandbyEnableApcb (ApcbDxeServiceProtocol);

  DEBUG ((DEBUG_INFO, "CPM Callback ApcbAccReadyCallBack Exit\n"));
}

VOID
FlashAccSmmReadyCallBack (
  IN EFI_EVENT                   Event,
  IN VOID                        *Context
  )
{
  EFI_STATUS                     Status;
  EFI_EVENT                      SecondaryEvent;
  VOID                           *Interface;
  VOID                           *Registration;

  DEBUG ((DEBUG_INFO, "CPM %a\n", __FUNCTION__));
  //
  // Try to locate it because EfiCreateProtocolNotifyEvent will trigger it once when registration.
  // Just return if it is not found.
  //
  Status = gBS->LocateProtocol (
                  &gPspFlashAccSmmCommReadyProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }

  // Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  DEBUG ((DEBUG_INFO, "\tCreate Protocol event for Install APCB Protocol\n"));
  SecondaryEvent = NULL;
  SecondaryEvent = EfiCreateProtocolNotifyEvent (
                      &gAmdApcbDxeServiceProtocolGuid,
                      TPL_NOTIFY,
                      ApcbAccReadyCallBack,
                      NULL,
                      &Registration
                      );
  ASSERT (SecondaryEvent != NULL);
  if (SecondaryEvent == NULL) {
     DEBUG ((DEBUG_INFO, "\tCreate event fail\n"));
  }

  DEBUG ((DEBUG_INFO, "CPM Callback FlashAccSmmReadyCallBack Exit\n"));
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load AMD CPM OEM GPP0 SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmOemGpp0SsdtInstall (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  STATIC BOOLEAN                        InitlateInvoked = FALSE;
  EFI_STATUS                            Status;
  UINT32                                Buffer[1];
  UINT32                                i;
  AMD_CPM_TABLE_PROTOCOL                *CpmTableProtocolPtr;
  AMD_CPM_DISPLAY_FEATURE_PROTOCOL      *CpmDisplayFeatureProtocolPtr;
  CPM_DISPLAY_FEATURE_PRIVATE           *DisplayFeatureDataPtr;
  AMD_CPM_DEVICE_PATH_TABLE             *DevicePathTablePtr;
  BOOLEAN                               Gpp0DeviceAslInstalled = FALSE;

  if (!InitlateInvoked) {

    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
    if (EFI_ERROR (Status)) {
      return;
    }

    Status = gBS->LocateProtocol (&gAmdCpmDisplayFeatureProtocolGuid, NULL, (VOID**)&CpmDisplayFeatureProtocolPtr);
    if (EFI_ERROR (Status)) {
      return;
    }

    DisplayFeatureDataPtr     = &CpmDisplayFeatureProtocolPtr->DisplayFeatureData;
    DevicePathTablePtr        = DisplayFeatureDataPtr->DevicePathTablePtr;

    for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {
      if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid == 0) {
        break;
      }
      if (DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid && DevicePathTablePtr->Path[i].FeatureMask.Mask.Exist) {
        if ((DevicePathTablePtr->Path[i].Bridge.Device == 0x1) && (DevicePathTablePtr->Path[i].Bridge.Function == 0x1)) {
          // Gpp0 connects GPU.
          Gpp0DeviceAslInstalled = TRUE;
        }
      }
    }

    if (!Gpp0DeviceAslInstalled) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-AmdCpmOemGpp0SsdtInstall-Install-GPP0-ASL.\n"));
      Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                        1,
                                                                        1 );

      CpmTableProtocolPtr->CommonFunction.AddSsdtTable ((VOID*)CpmTableProtocolPtr,
                                                        &AMD_GPP0_SSDT_GUID,
                                                        NULL,
                                                        AmdCpmGpp0CallBack,
                                                        &Buffer[0]);
    }

  }

  InitlateInvoked = TRUE;

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load AMD CPM OEM GPP1 SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmOemGpp1SsdtInstall (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  STATIC BOOLEAN                        InitlateInvoked = FALSE;
  EFI_STATUS                            Status;
  UINT32                                Buffer[1];
  AMD_CPM_TABLE_PROTOCOL                *CpmTableProtocolPtr;

  if (!InitlateInvoked) {
    Status = gBS->LocateProtocol (
                    &gAmdCpmTableProtocolGuid,
                    NULL,
                    (VOID**)&CpmTableProtocolPtr
                    );
    if (EFI_ERROR (Status)) {
      return;
    }

    DEBUG ((DEBUG_INFO, "OEM-DXE-AmdCpmOemGpp1SsdtInstall-Install-GPP1-ASL.\n"));
    Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (
                                                      CpmTableProtocolPtr,
                                                      1,
                                                      2
                                                      );

      CpmTableProtocolPtr->CommonFunction.AddSsdtTable (
                                            (VOID*)CpmTableProtocolPtr,
                                            &AMD_GPP0_SSDT_GUID,
                                            NULL,
                                            AmdCpmGpp0CallBack,
                                            &Buffer[0]
                                            );
  }

  InitlateInvoked = TRUE;

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 *
 * This function is used to check the Nvme device.
 *
 * @param[in]     RpDevice              Pci Device number of root port
 * @param[in]     RpFunction            Pci function number of root port
 *
 * @retval        TRUE                  Device has been found
 * @retval        FALSE                 Device has not been found
 */
BOOLEAN
EFIAPI
CheckNvmeDevice (
  IN UINT8 RpDevice,
  IN UINT8 RpFunction
  )
{

  EFI_STATUS   Status;
  UINT32       ClassCode = 0;
  UINT8        SecBusNum = 0;
  UINT16       VendorId  = 0;
  UINT16       DeviceId  = 0;

  AMD_CPM_TABLE_PROTOCOL   *CpmTableProtocolPtr;

  Status = gBS->LocateProtocol (
                  &gAmdCpmTableProtocolGuid,
                  NULL,
                  (VOID**)&CpmTableProtocolPtr
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Locate AmdCpmTableProtocol failed\n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "OEM-DXE-%a-RootPort Dev:%d Func:%d\n", __FUNCTION__, RpDevice, RpFunction));
  VendorId = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, 0, RpDevice, RpFunction, 0x0);
  DeviceId = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, 0, RpDevice, RpFunction, 0x2);
  DEBUG ((DEBUG_INFO, "OEM-DXE-%a-RootPort-VendorId: 0x%04X DeviceId: 0x%04X\n", __FUNCTION__, VendorId, DeviceId));
  if (VendorId == 0xFFFF && DeviceId == 0xFFFF) {
    return FALSE;
  }

  SecBusNum = CpmTableProtocolPtr->CommonFunction.PciRead8(CpmTableProtocolPtr, 0, RpDevice, RpFunction, 0x19);
  DEBUG ((DEBUG_INFO, "OEM-DXE-%a-SecBusNum: 0x%02X\n", __FUNCTION__, SecBusNum));

  ClassCode = CpmTableProtocolPtr->CommonFunction.PciRead32 (CpmTableProtocolPtr, SecBusNum, 0, 0, 0x08);
  DEBUG ((DEBUG_INFO, "\tClassCode read is 0x%X : NVME_CLASS_CODE is 0x%X\n", ClassCode, NVME_CLASS_CODE));

  if ((ClassCode & 0xFFFFFF00) == NVME_CLASS_CODE) {
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Device (Dev:%d, Func:%d) found\n", __FUNCTION__, RpDevice, RpFunction));
    return TRUE;
  }

  return FALSE;
}

/**
 *
 * This function is used to check the Nvme RTD3 device.
 *
 * @param[in]     NvmeRtd3TablePtr      Pointer to AMD_CPM_NVME_RTD3_TABLE
 * @param[in]     RpDevice              Pci Device number of root port
 * @param[in]     RpFunction            Pci function number of root port
 *
 * @retval        TRUE                  Device has been found
 * @retval        FALSE                 Device has not been found
 */
BOOLEAN
EFIAPI
CheckNvmeRtd3Device (
  IN AMD_CPM_NVME_RTD3_TABLE  *NvmeRtd3TablePtr,
  IN UINT8                    RpDevice,
  IN UINT8                    RpFunction
  )
{
  UINT8   Index = 0;

  for (Index = 0; NvmeRtd3TablePtr->NvmeList[Index].Dev != 0xFF; Index++) {
    if ((RpDevice == NvmeRtd3TablePtr->NvmeList[Index].Dev) &&
        (RpFunction == NvmeRtd3TablePtr->NvmeList[Index].Func)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Device (Dev:%d, Func:%d) found\n", __FUNCTION__, RpDevice, RpFunction));
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Callback function to override NVME D3 SSDT Table
 *
 * This function is used to update GPPX name.
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
AmdCpmNvmeD3CallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT32  *BufferPtr;
  BufferPtr = (UINT32*)Context;

  switch (*((UINT32*)AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('G', 'P', 'P', 'X')):
    if (*BufferPtr) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr);
      DEBUG ((DEBUG_INFO, "\t%a(%d) GPPX is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  }

  return FALSE;
}

/**
 * The function to load AMD CPM OEM NVME D3 SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmNvmeD3SsdtInstall (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  )
{
  STATIC BOOLEAN           InitlateInvoked = FALSE;
  EFI_STATUS               Status;
  AMD_CPM_TABLE_PROTOCOL   *CpmTableProtocolPtr;
  AMD_CPM_NVME_RTD3_TABLE  *NvmeRtd3TablePtr;
  UINT32                   Buffer;
  UINT8                    RpDev  = 0;
  UINT8                    RpFunc = 0;

  DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Start\n", __FUNCTION__));
  if (!InitlateInvoked) {
    Status = gBS->LocateProtocol (
                    &gAmdCpmTableProtocolGuid,
                    NULL,
                    (VOID**)&CpmTableProtocolPtr
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return;
    }

    for (RpDev = 1; RpDev < 3; RpDev++) {
      for (RpFunc = 1; RpFunc < 8; RpFunc++) {
        if (CheckNvmeDevice (RpDev, RpFunc)) {
          Buffer = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (
                                                         CpmTableProtocolPtr,
                                                         RpDev,
                                                         RpFunc
                                                         );
          DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Buffer: %x\n", __FUNCTION__, Buffer));
          CpmTableProtocolPtr->CommonFunction.AddSsdtTable (
                                                (VOID*)CpmTableProtocolPtr,
                                                &AMD_NVME_D3_SSDT_GUID,
                                                NULL,
                                                AmdCpmNvmeD3CallBack,
                                                &Buffer
                                                );

          NvmeRtd3TablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_NVME_STD3);
          if (NvmeRtd3TablePtr) {
            DEBUG ((DEBUG_INFO, "OEM-DXE-%a-NvmeD3ColdSupported: %x\n", __FUNCTION__, NvmeRtd3TablePtr->NvmeD3ColdSupported));
            if (NvmeRtd3TablePtr->NvmeD3ColdSupported) {
              if (CheckNvmeRtd3Device (NvmeRtd3TablePtr, RpDev, RpFunc)) {
                CpmTableProtocolPtr->CommonFunction.AddSsdtTable (
                                                      (VOID*)CpmTableProtocolPtr,
                                                      &AMD_NVME_RTD3_SSDT_GUID,
                                                      NULL,
                                                      AmdCpmNvmeD3CallBack,
                                                      &Buffer
                                                      );
              }
            }
          }
        }
      }
    }
  }
  InitlateInvoked = TRUE;
  DEBUG ((DEBUG_INFO, "OEM-DXE-%a-End\n", __FUNCTION__));
  return;
}

/**
 * Callback function to override Gpp MS SSDT Table
 *
 * This function is used to update GPPX name.
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
AmdCpmGppMsCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  UINT32                          *BufferPtr;
  AMD_PBS_SETUP_OPTION            AmdPbsConfiguration;

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return FALSE;
  }
  BufferPtr = (UINT32*)Context;

  switch (*((UINT32*)AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('C', 'R', 'S', '1')):
      if (!CheckNvmeDevice(1,2)) { //Use Hardcoded value in CPM Table (X4Slot: D1::F2)
        *(UINT32*) AmlObjPtr = SIGNATURE_32 ('_', 'C', 'R', 'S');
        DEBUG ((DEBUG_INFO, "%a(%d) - CRS1 is updated\n", __FUNCTION__, __LINE__));
      }
    break;

  case (CPM_SIGNATURE_32 ('C', 'R', 'S', '2')):
      if (AmdPbsConfiguration.PbsWWANDeviceSupport == 1) { // Fibocom L860 R+
        // Use _PRW instead of _CRS, so modify the name.
        *(UINT32*) AmlObjPtr = SIGNATURE_32 ('W', 'W', 'R', 'S');
        DEBUG ((DEBUG_INFO, "%a(%d) - CRS2 is updated\n", __FUNCTION__, __LINE__));
        break;
      }
      if (!CheckNvmeDevice(1,3)) { //Use Hardcoded value in CPM Table (X1Slot: D1::F3)
        *(UINT32*) AmlObjPtr = SIGNATURE_32 ('_', 'C', 'R', 'S');
        DEBUG ((DEBUG_INFO, "%a(%d) - CRS2 is updated\n", __FUNCTION__, __LINE__));
      }
    break;

  case (SIGNATURE_32 ('E', 'W', 'P', 'M')): // EWPM name object
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr));
        DEBUG ((DEBUG_INFO, "%a(%d) - EWPM is updated\n", __FUNCTION__, __LINE__));
      }
    break;

  case (SIGNATURE_32 ('Q', 'D', 'S', 'W')): // QDSW name object
      if (*(BufferPtr + 1)) {
        *(UINT32*) AmlObjPtr = SIGNATURE_32 ('_', 'D', 'S', 'W');
        DEBUG ((DEBUG_INFO, "%a(%d) - QDSW is updated\n", __FUNCTION__, __LINE__));
      }
    break;
  }

  return FALSE;
}

/**
 * The function to load AMD CPM OEM GPP MS SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmOemGppMsSsdtInstall (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  )
{
  STATIC BOOLEAN           InitlateInvoked = FALSE;
  EFI_STATUS               Status;
  AMD_CPM_TABLE_PROTOCOL   *CpmTableProtocolPtr;
  AMD_PBS_SETUP_OPTION     AmdPbsConfiguration;
  BOOLEAN                  IsQcomWlan = FALSE;
  UINT8                    SecBusNum  = 0;
  UINT16                   VendorId   = 0;
  UINT16                   DeviceId   = 0;
  UINT32                   Buffer[2]  = {0};

  if (!InitlateInvoked) {
    Status = gBS->LocateProtocol (
                    &gAmdCpmTableProtocolGuid,
                    NULL,
                    (VOID**)&CpmTableProtocolPtr
                    );
    if (EFI_ERROR (Status)) {
      return;
    }

    Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-ERROR-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return;
    }

    // Check device if it's Qualcomm WLAN device
    VendorId = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, 0, 2, 2, 0x0);
    DeviceId = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, 0, 2, 2, 0x2);
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-RootPort-VendorId: 0x%04X DeviceId: 0x%04X\n", __FUNCTION__, VendorId, DeviceId));
    if (VendorId != 0xFFFF && DeviceId != 0xFFFF) {
      SecBusNum = CpmTableProtocolPtr->CommonFunction.PciRead8(CpmTableProtocolPtr, 0, 2, 2, 0x19);
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a-SecBusNum: 0x%02X\n", __FUNCTION__, SecBusNum));
      VendorId = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, SecBusNum, 0, 0, 0x0);
      DeviceId = CpmTableProtocolPtr->CommonFunction.PciRead16 (CpmTableProtocolPtr, SecBusNum, 0, 0, 0x2);
      if (VendorId == QCOM_VENDOR_ID && DeviceId == QCOM_DEVICE_NFA765_ID) {
        IsQcomWlan = TRUE;
        DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Detect QualcommWlan\n", __FUNCTION__));
      }
    }

    Buffer[0] = (UINT32) AmdPbsConfiguration.WakeOnPME;
    Buffer[1] = (UINT32) IsQcomWlan;

    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-WakeOnPME     : 0x%d\n", __FUNCTION__, Buffer[0]));
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-IsQualcommWlan: 0x%d\n", __FUNCTION__, Buffer[1]));

    CpmTableProtocolPtr->CommonFunction.AddSsdtTable (
                                          (VOID*)CpmTableProtocolPtr,
                                          &AMD_GPPMS_SSDT_GUID,
                                          NULL,
                                          AmdCpmGppMsCallBack,
                                          &Buffer[0]
                                          );
  }

  InitlateInvoked = TRUE;
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load AMD CPM OEM uPEP SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmOemUpepSsdtInstall (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  STATIC BOOLEAN                        InitlateInvoked = FALSE;
  EFI_STATUS                            Status;
  AMD_CPM_TABLE_PROTOCOL                *CpmTableProtocolPtr;
  UINT32                                Buffer[1] = {0x00};
  AMD_PBS_SETUP_OPTION                  AmdPbsConfiguration;

  if (!InitlateInvoked) {

    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
    if (EFI_ERROR (Status)) {
      return;
    }
    Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "%a(%d)-GetAmdPbsConfiguration: %r\n", __FUNCTION__, __LINE__, Status));
      return;
    }
    Buffer[0] = AmdPbsConfiguration.PbsWWANDeviceSupport;

    CpmTableProtocolPtr->CommonFunction.AddSsdtTable ((VOID*)CpmTableProtocolPtr,
                                                      &AMD_UPEP_SSDT_GUID,
                                                      NULL,
                                                      AmdCpmUpepSsdtCallBack,
                                                      Buffer);

  }

  InitlateInvoked = TRUE;

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 *  Callback function to override UPEP SSDT Table.
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
AmdCpmUpepSsdtCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT32  *BufferPtr;
  BufferPtr = (UINT32*)Context;

  switch (*((UINT32*)AmlObjPtr)) {
  case CPM_SIGNATURE_32 ('W', 'W', 'S', 'W'): // WWSW
    if (BufferPtr) {
      if (*(((UINT8*) AmlObjPtr) - 1) == AML_NAME_OP && *(((UINT8*) AmlObjPtr) + 4) == AML_BYTE_PREFIX) {
        *(((UINT8*) AmlObjPtr) + 5) = (UINT8)(*(BufferPtr + 0));
        DEBUG ((DEBUG_INFO, "%a(%d)-WWSW is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 *  Callback function to override AZALIA SSDT Table.
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
AmdCpmOemGpioSsdtCallback (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT32  *BufferPtr;
  BufferPtr = (UINT32*)Context;

  switch (*((UINT32*)AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('E', 'C', '0', '_')):
    if (*BufferPtr) {
      *(UINT32*) AmlObjPtr = *(UINT32*)BufferPtr;
    }
    break;
  case (CPM_SIGNATURE_32 ('M', '0', '4', '2')):     // CpmKbcMutex: M042
    if (*(BufferPtr + 1)) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr + 1);
    }
    break;
  case (CPM_SIGNATURE_32 ('L', 'P', 'C', '0')):
    if (*(BufferPtr + 2)) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr + 2);
    }
    break;
  case (CPM_SIGNATURE_32 ('O', 'K', 'E', 'C')):
    if (*(BufferPtr + 3)) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr + 3);
    }
    break;
  }

  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to override Gpp0 SSDT Table
 *
 * This function is used to update ASL name.
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
AmdCpmGpp0CallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT32  *BufferPtr;
  BufferPtr = (UINT32*)Context;

  switch (*((UINT32*)AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('G', 'P', 'P', '0')):
    if (*(BufferPtr)) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr);
    }
    break;
  }

  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM ModernStandby Init DXE driver
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmModernStandbyInitDxeEntryPoint (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               Event = NULL;
  AMD_PBS_SETUP_OPTION                    AmdPbsConfiguration;
  EFI_EVENT                               ReadyToBootEvent;
  VOID                                    *Registration;

  DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Start\n", __FUNCTION__));
  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  if (PcdGetBool (PcdModernStandbyEnable) == TRUE) {
    // Install UPEP SSDT
    Status = gBS->CreateEventEx (
                    CPM_EVENT_NOTIFY_SIGNAL,
                    CPM_TPL_CALLBACK,
                    AmdCpmOemUpepSsdtInstall,
                    NULL,
                    &gEfiEventReadyToBootGuid,
                    &ReadyToBootEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    }

    // Install Gpp0 SSDT
    Status = gBS->CreateEventEx (
                    CPM_EVENT_NOTIFY_SIGNAL,
                    CPM_TPL_CALLBACK,
                    AmdCpmOemGpp0SsdtInstall,
                    NULL,
                    &gEfiEventReadyToBootGuid,
                    &ReadyToBootEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    }

    // Install Gpp1 SSDT
    if (AmdPbsConfiguration.DtSlotPowerControl) {
      Status = gBS->CreateEventEx (
                      CPM_EVENT_NOTIFY_SIGNAL,
                      CPM_TPL_CALLBACK,
                      AmdCpmOemGpp1SsdtInstall,
                      NULL,
                      &gEfiEventReadyToBootGuid,
                      &ReadyToBootEvent
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      }
    }

    // Install GppMs SSDT
    Status = gBS->CreateEventEx (
                    CPM_EVENT_NOTIFY_SIGNAL,
                    CPM_TPL_CALLBACK,
                    AmdCpmOemGppMsSsdtInstall,
                    NULL,
                    &gEfiEventReadyToBootGuid,
                    &ReadyToBootEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    }

    // Install NVME SSDT
    Status = gBS->CreateEventEx (
                    CPM_EVENT_NOTIFY_SIGNAL,
                    CPM_TPL_CALLBACK,
                    AmdCpmNvmeD3SsdtInstall,
                    NULL,
                    &gEfiEventReadyToBootGuid,
                    &ReadyToBootEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    }
  }

  DEBUG ((DEBUG_INFO, "\tCreate Protocol NotifyEvent\n"));
  Event = EfiCreateProtocolNotifyEvent (&gPspFlashAccSmmCommReadyProtocolGuid, TPL_NOTIFY, FlashAccSmmReadyCallBack, NULL, &Registration);
  if (Event == NULL) {
    DEBUG ((DEBUG_INFO, "\tCreate Protocol NotifyEvent fail\n"));
  }
  ASSERT (Event != NULL);

  if (PcdGet8 (PcdCfgIgpuContorl) != 0) {
    DEBUG ((DEBUG_INFO, "\tPcdCfgIgpuContorl = %d Call PspMboxBiosScreenOnGpio\n", PcdGet8 (PcdCfgIgpuContorl)));
    Status = PspMboxBiosScreenOnGpio (&ScreenOnGpioList);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    }
  }

  DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-End-Status = EFI_SUCCESS\n", __FUNCTION__, __LINE__));
  return EFI_SUCCESS;
}
