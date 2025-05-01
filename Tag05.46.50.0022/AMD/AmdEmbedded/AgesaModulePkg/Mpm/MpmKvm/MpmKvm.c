/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "MpmKvm.h"
#include <AmdPspDirectory.h>
#include <Library/MpmLib.h>
#include <Library/MpmDevLib.h>
#include <cpuRegisters.h>
#include <Library/MpmBiosCmdLib.h>
#include <Protocol/AmdKvmStaticIpInfoProtocol.h>
#include <Library/PrintLib.h>

#define FILECODE MPM_MPMKVM_MPMKVM_FILECODE

STATIC BOOLEAN mMpmKvmDriverStarted = FALSE;
BOOLEAN mKvmCapableDevDetected = FALSE;
UINT32  mKvmDeviceRequest = KVM_NOT_REQUESTED;

#define NO_KVM_CAPABLE_DEV   0
#define KVM_CAPABLE_DEV      1



EFI_COMPONENT_NAME_PROTOCOL  MpmKvmComponentName = {
  MpmKvmComponentNameGetDriverName,
  MpmKvmComponentNameGetControllerName,
  "eng"
};

EFI_COMPONENT_NAME2_PROTOCOL MpmKvmComponentName2 = {
  (EFI_COMPONENT_NAME2_GET_DRIVER_NAME) MpmKvmComponentNameGetDriverName,
  (EFI_COMPONENT_NAME2_GET_CONTROLLER_NAME) MpmKvmComponentNameGetControllerName,
  "en"
};

EFI_UNICODE_STRING_TABLE mMpmKvmDriverNameTable[] = {
  {
    "eng;en",
    L"MPM KVM Virtual KB/MS Driver"
  },
  {
    NULL,
    NULL
  }
};

EFI_DRIVER_BINDING_PROTOCOL gMpmKvmDriver = {
  MpmKvmDriverSupported,
  MpmKvmDriverStart,
  MpmKvmDriverStop,
  0xa,
  NULL,
  NULL
};

STATIC AMD_PSP_KVM_SERVICE_PROTOCOL mMpmKvmServiceProtocol = {
  AMD_PSP_KVM_SERVICE_PROTOCOL_VERSION_1,
  MpmKvmGetRequest,
  MpmKvmSetRequest,
  MpmKvmGetSessionProgress,
  MpmKvmSetSessionProgress,
  MpmKvmInitiateKvm,
  MpmKvmTerminateKvm,
  MpmKvmRetrieveSmbiosTable
};

STATIC MPM_KVM_DEV mMpmKvmDev;
UINT64 mKvmDmaHpaAddr = 0;
UINT64 mKvmDmaGpaAddr = 0;
UINT64 mKvmMpmC2pMsg2HpaAddr = 0;
UINT64 mKvmMpmC2pMsg2GpaAddr = 0;



/**
 * @brief Retrieve the smbios data for KVM
 *
 * @param KvmEnabled  Kvm Enabled or not, will be filled in the KVM SBIOS table structure
 * @param SmbiosTableTypeA0 Point to the address to hold SMBIOS_TABLE_TYPEA0 struture pointer
 * @return EFI_STATUS
 */
EFI_STATUS
MpmKvmRetrieveSmbiosTable (
  IN    UINT8                    KvmEnabled,
  OUT   SMBIOS_TABLE_TYPEA0    **SmbiosTableTypeA0
  )
{
  EFI_STATUS                        Status;
  UINT8                             Size;
  SMBIOS_TABLE_TYPEA0              *SmbiosTable;
  UINT32                            KvmRequest;

  Status = EFI_SUCCESS;
  Size = sizeof(SMBIOS_TABLE_TYPEA0);
  gBS->AllocatePool (EfiBootServicesData, Size, &SmbiosTable);
  gBS->SetMem (SmbiosTable, Size, 0);
  SmbiosTable->Hdr.Handle                           = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosTable->Hdr.Type                             = KVM_SMBIOS_TYPE_KVM_INFORMATION;
  SmbiosTable->Hdr.Length                           = Size;
  SmbiosTable->KvmInformation.Field.KvmEnabled      = KvmEnabled;
  SmbiosTable->KvmInformation.Field.Protocol        = KVM_SMBIOS_PROTOCOL_OTHER;
  SmbiosTable->KvmInformation.Field.Connectivity    = KVM_SMBIOS_CONNECTIVITY_SHARED;
  SmbiosTable->KvmInformation.Field.ConcurrentCount = KVM_SMBIOS_CONCURRENTCOUNT_SINGLE;
  SmbiosTable->KvmInformation.Field.KvmVersion      = 1;

  Status = mMpmKvmDev.KvmDevProtocol->ReadKvmRequest (mMpmKvmDev.KvmDevHandle, &KvmRequest);
  SmbiosTable->KvmInformation.Field.KvmRequested = (UINT8)KvmRequest;

  *SmbiosTableTypeA0 = SmbiosTable;

  return Status;
}

/**
 * @brief Get value of KvmRequest and init KvmRequest,SessionProgress to 0
 *
 * @param KvmRequest
 * @return EFI_STATUS
 */
EFI_STATUS
MpmKvmGetRequest (
  IN     UINT32                *KvmRequest
  )
{
  UINT32           KvmRequestVal;
  UINT32           KvmSessionProgressVal;
  EFI_STATUS       status;

  if (!mKvmCapableDevDetected) {
    return EFI_UNSUPPORTED;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tMpmKvmGetRequest >>\n");
  status = mMpmKvmDev.KvmDevProtocol->ReadKvmRequest (mMpmKvmDev.KvmDevHandle, &KvmRequestVal);
  if (KvmRequestVal == 0x1) {
    *KvmRequest = KVM_REQUESTED;
  } else {
    *KvmRequest = KVM_NOT_REQUESTED;
  }
  // Session is not required for wireless type, MPM will mantain it in its ownd NV
  if (mMpmKvmDev.KvmDevProtocol->DeviceType == MPM_NIC_WIRED) {
    status = mMpmKvmDev.KvmDevProtocol->ReadKvmSessionProgress (mMpmKvmDev.KvmDevHandle, &KvmSessionProgressVal);
    if (EFI_ERROR(status)) {
      return status;
    }
    if (KvmSessionProgressVal == 0x1) {
      KvmSessionProgressVal = 0x0;
      status = mMpmKvmDev.KvmDevProtocol->WriteKvmSessionProgress (mMpmKvmDev.KvmDevHandle, &KvmSessionProgressVal);
      if (EFI_ERROR(status)) {
        return status;
      }
    }
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tMpmKvmGetRequest %x <<\n", *KvmRequest);
  return EFI_SUCCESS;
}


/**
 * @brief Set value of KvmRequest
 *
 * @param KvmRequest
 * @return EFI_STATUS
 */
EFI_STATUS
MpmKvmSetRequest (
  IN     UINT32                *KvmRequest
  )
{
  EFI_STATUS       status;

  if (!mKvmCapableDevDetected) {
    return EFI_UNSUPPORTED;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tMpmKvmSetRequest >>\n");
  status = mMpmKvmDev.KvmDevProtocol->WriteKvmRequest (mMpmKvmDev.KvmDevHandle, KvmRequest);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tMpmKvmSetRequest <<\n");
  return status;
}

/**
 * @brief Get value of KvmSessionProgress
 *
 * @param KvmSessionProgress
 * @return EFI_STATUS
 */
EFI_STATUS
MpmKvmGetSessionProgress (
  IN     UINT32                *KvmSessionProgress
  )
{
  EFI_STATUS       status;

  status = EFI_UNSUPPORTED;
  if (!mKvmCapableDevDetected) {
    return EFI_UNSUPPORTED;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tMpmKvmGetSessionProgress >>\n");
  // Session is not required for wireless type, MPM will mantain it in its ownd NV
  if (mMpmKvmDev.KvmDevProtocol->DeviceType == MPM_NIC_WIRED) {
    status = mMpmKvmDev.KvmDevProtocol->ReadKvmSessionProgress (mMpmKvmDev.KvmDevHandle, KvmSessionProgress);
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tMpmKvmGetSessionProgress <<\n");
  return status;
}

/**
 * @brief Set value of KvmSessionProgress
 *
 * @param KvmSessionProgress
 * @return EFI_STATUS
 */
EFI_STATUS
MpmKvmSetSessionProgress (
  IN     UINT32                *KvmSessionProgress
  )
{
  EFI_STATUS       status;

  if (!mKvmCapableDevDetected) {
    return EFI_UNSUPPORTED;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tMpmKvmSetSessionProgress >>\n");
  // Session is not required for wireless type, MPM will mantain it in its ownd NV
  if (mMpmKvmDev.KvmDevProtocol->DeviceType == MPM_NIC_WIRED) {
    status = mMpmKvmDev.KvmDevProtocol->WriteKvmSessionProgress (mMpmKvmDev.KvmDevHandle, KvmSessionProgress);
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tMpmKvmSetSessionProgress <<\n");
  return EFI_SUCCESS;
}

/**
 * @brief Initiate Kvm params and Send Message to MPM
 *
 * @param ModeInfo
 * @param KvmIpInfo  Not used, just used for compatilbe with PSP KVM
 * @return EFI_STATUS
 */
EFI_STATUS
MpmKvmInitiateKvm (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *ModeInfo,
  IN KVM_IP_INFO                       *KvmIpInfo
  )
{
  EFI_STATUS                                   Status;
  MBOX_START_KVM                               StartKvmPara;
  AMD_KVM_STATIC_IP_INFO_PROTOCOL              *AmdKvmStaticIpInfo = NULL;
  UINT8                                        DhcpV4Enable;
  EFI_IPv4_ADDRESS                             IpV4Addr;
  EFI_IPv4_ADDRESS                             IpV4SubMask;
  EFI_IPv4_ADDRESS                             IpV4GateWay;

  if (!mKvmCapableDevDetected) {
    return EFI_UNSUPPORTED;
  }

  //Get IP information from RTK UEFI driver if KvmIpInfo is NULL
  if (KvmIpInfo == NULL) {
    Status = gBS->LocateProtocol (&gAmdKvmStaticIpInfoProtocolGuid, NULL, (VOID**)&AmdKvmStaticIpInfo);
    if (EFI_ERROR(Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspKvmInitiateKvm: failed to locate gAmdKvmStaticIpInfoProtocolGuid, Status = %r\n", Status);
    } else {
      AmdKvmStaticIpInfo->AddrQuery (AmdKvmStaticIpInfo, &DhcpV4Enable, &IpV4Addr, &IpV4SubMask, &IpV4GateWay);
      IDS_HDT_CONSOLE_PSP_TRACE ("Get IP address from RTK driver:\n");
      IDS_HDT_CONSOLE_PSP_TRACE ("DhcpV4Enable = %d\n", DhcpV4Enable);
      IDS_HDT_CONSOLE_PSP_TRACE ("IpV4Addr =%d.%d.%d.%d\n", IpV4Addr.Addr[0], IpV4Addr.Addr[1], IpV4Addr.Addr[2], IpV4Addr.Addr[3]);
      IDS_HDT_CONSOLE_PSP_TRACE ("IpV4SubMask =%d.%d.%d.%d\n", IpV4SubMask.Addr[0], IpV4SubMask.Addr[1], IpV4SubMask.Addr[2], IpV4SubMask.Addr[3]);
      IDS_HDT_CONSOLE_PSP_TRACE ("IpV4GateWay =%d.%d.%d.%d\n", IpV4GateWay.Addr[0], IpV4GateWay.Addr[1], IpV4GateWay.Addr[2], IpV4GateWay.Addr[3]);
      KvmIpInfo = AllocatePool (sizeof (KVM_IP_INFO));
      if (DhcpV4Enable == 0) {
        KvmIpInfo->NetworkType = 1;
      } else {
        KvmIpInfo->NetworkType = 0;
      }

      ZeroMem (KvmIpInfo->Ipv4Addr, 20);
      ZeroMem (KvmIpInfo->Ipv4SubMask, 20);
      ZeroMem (KvmIpInfo->Ipv4DefGateway, 20);
      AsciiSPrint (KvmIpInfo->Ipv4Addr, 20, "%d.%d.%d.%d", IpV4Addr.Addr[0], IpV4Addr.Addr[1], IpV4Addr.Addr[2], IpV4Addr.Addr[3]);
      AsciiSPrint (KvmIpInfo->Ipv4SubMask, 20, "%d.%d.%d.%d", IpV4SubMask.Addr[0], IpV4SubMask.Addr[1], IpV4SubMask.Addr[2], IpV4SubMask.Addr[3]);
      AsciiSPrint (KvmIpInfo->Ipv4DefGateway, 20, "%d.%d.%d.%d", IpV4GateWay.Addr[0], IpV4GateWay.Addr[1], IpV4GateWay.Addr[2], IpV4GateWay.Addr[3]);
    }
  }

  ZeroMem (&StartKvmPara, sizeof (MBOX_START_KVM));
  StartKvmPara.NetCardPciCfgAddr = mMpmKvmDev.NetCardMmioBase;
  StartKvmPara.PcieMmioCfgBaseAddr = AsmReadMsr64 (MSR_MMIO_Cfg_Base) & 0x0000FFFFFFF00000;
  StartKvmPara.FrameBufferBaseLo = (UINT32) (ModeInfo->FrameBufferBase & 0xFFFFFFFF);
  StartKvmPara.FrameBufferBaseHi = (UINT32) (ModeInfo->FrameBufferBase >> 32);
  StartKvmPara.FrameBufferSize = (UINT32) ((ModeInfo->Info->HorizontalResolution * ModeInfo->Info->VerticalResolution * 4) & 0xFFFFFFFF);
  StartKvmPara.KvmMouseProtocolSelection = PcdGet8(PcdMpmKvmMouseProtocol);
  CopyMem (&StartKvmPara.GfxModeInfo, ModeInfo->Info, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
  mKvmMouseDev.AbsoluteMode.AbsoluteMaxX = StartKvmPara.GfxModeInfo.HorizontalResolution;
  mKvmMouseDev.AbsoluteMode.AbsoluteMaxY = StartKvmPara.GfxModeInfo.VerticalResolution;
  //DMAr related
  StartKvmPara.DMArEnabled = PcdGetBool(PcdIvInfoDmaReMap);
  StartKvmPara.KvmDmaGpaAddrLo = (UINT32) (mKvmDmaGpaAddr & 0xFFFFFFFF);
  StartKvmPara.KvmDmaGpaAddrHi = (UINT32) (mKvmDmaGpaAddr >> 32);
  StartKvmPara.KvmDmaHpaAddrLo = (UINT32) (mKvmDmaHpaAddr & 0xFFFFFFFF);
  StartKvmPara.KvmDmaHpaAddrHi = (UINT32) (mKvmDmaHpaAddr >> 32);
  //Mpm_C2pMsg_2 HPA/GPA address
  StartKvmPara.KvmMpmC2pMsg2GpaAddrLo = (UINT32) (mKvmMpmC2pMsg2GpaAddr & 0xFFFFFFFF);
  StartKvmPara.KvmMpmC2pMsg2GpaAddrHi = (UINT32) (mKvmMpmC2pMsg2GpaAddr >> 32);
  StartKvmPara.KvmMpmC2pMsg2HpaAddrLo = (UINT32) (mKvmMpmC2pMsg2HpaAddr & 0xFFFFFFFF);
  StartKvmPara.KvmMpmC2pMsg2HpaAddrHi = (UINT32) (mKvmMpmC2pMsg2HpaAddr >> 32);
  CopyMem (&StartKvmPara.KvmIpInfo, KvmIpInfo, sizeof (KVM_IP_INFO));
  Status = BiosMpmCmdStartKvm (&StartKvmPara);
  return Status;
}
/**
 * @brief Terminate KVM session
 *
 * @return EFI_STATUS
 */
EFI_STATUS
MpmKvmTerminateKvm (
  VOID
  )
{
  UINT32 KvmSessionProgress;

  KvmSessionProgress = 0;
  // Session is not required for wireless type, MPM will mantain it in its ownd NV
  if (mMpmKvmDev.KvmDevProtocol->DeviceType == MPM_NIC_WIRED) {
    MpmKvmSetSessionProgress(&KvmSessionProgress);
  }
  BiosMpmCmdTerminateKvm ();
  return EFI_SUCCESS;
}

/**
 * @brief Supported function of driver binding protocol
 *
 * @param This
 * @param Controller
 * @param RemainingDevicePath
 * @return EFI_STATUS
 */
EFI_STATUS
EFIAPI
MpmKvmDriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS                            Status;
  EFI_PCI_IO_PROTOCOL                   *PciIo;
  PCI_DEVICE_INDEPENDENT_REGION         DeviceHdr;
  BOOLEAN                               KbMsDev;

  // IDS_HDT_CONSOLE_PSP_TRACE ("MpmKvmDriverSupported\n");

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );

  if (Status == EFI_ALREADY_STARTED) {
    // IDS_HDT_CONSOLE_PSP_TRACE ("Already Started\n");
    return EFI_SUCCESS;
  }

  if (EFI_ERROR (Status)) {
    // IDS_HDT_CONSOLE_PSP_TRACE ("OpenProtocol fail %x\n", Status);
    return EFI_UNSUPPORTED;
  }

  PciIo->Pci.Read (
                  PciIo,
                  EfiPciIoWidthUint8,
                  0,
                  sizeof (DeviceHdr),
                  &DeviceHdr
                  );

  // IDS_HDT_CONSOLE_PSP_TRACE ("KVM VID %x DID %x\n", DeviceHdr.VendorId, DeviceHdr.DeviceId);

  //Check if PSP Device ID & KVM capable device detected
  if (((AMD_RMB_PSP_VID == DeviceHdr.VendorId) && (AMD_RMB_PSP_DID == DeviceHdr.DeviceId))){
    KbMsDev = TRUE;
  } else {
    KbMsDev = FALSE;
  }
  if ((KbMsDev == TRUE) &&
      (mKvmDeviceRequest == KVM_REQUESTED) &&
      !mMpmKvmDriverStarted){
    IDS_HDT_CONSOLE_PSP_TRACE ("Dev for KVM KB/MS found.\n");
    Status = EFI_SUCCESS;
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Dev for KVM KB/MS Not found.\n");
    Status = EFI_UNSUPPORTED;
  }

  gBS->CloseProtocol (
         Controller,
         &gEfiPciIoProtocolGuid,
         This->DriverBindingHandle,
         Controller
         );

  return Status;
}


/**
 * @brief Start function of driver binding protocol
 *
 * @param This
 * @param Controller
 * @param RemainingDevicePath
 * @return EFI_STATUS
 */
EFI_STATUS
EFIAPI
MpmKvmDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS         Status;
  MPM_KVM_DEV    *KvmDev;
  UINT32             KvmMouseProtocolSelection;

  KvmDev = &mMpmKvmDev;
  KvmDev->Signature = MPM_KVM_DEV_SIGNATURE;

  Status = KvmKbdControllerDriverStart(This, Controller, RemainingDevicePath);
  if (EFI_ERROR(Status)){
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmKbdDriver Install Error!\n");
    return Status;
  }

  KvmMouseProtocolSelection = PcdGet8(PcdMpmKvmMouseProtocol);
  if (KvmMouseProtocolSelection == KVM_ABSOLUTE_MOUSE_PROTOCOL) {
    Status = KvmAbsoluteMouseDriverStart(This, Controller, RemainingDevicePath);
    IDS_HDT_CONSOLE_PSP_TRACE ("Install KvmAbsoluteMouseDriver\n");
  }
  else if (KvmMouseProtocolSelection == KVM_SIMPLE_MOUSE_PROTOCOL) {
    Status = KvmSimpleMouseDriverStart(This, Controller, RemainingDevicePath);
    IDS_HDT_CONSOLE_PSP_TRACE ("Install KvmSimpleMouseDriver\n");
  }
  else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Unknown Mouse Protocol Selection: 0x%x\n", KvmMouseProtocolSelection);
    return EFI_UNSUPPORTED;
  }
  if (EFI_ERROR(Status)){
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmMouseDriver Install Error!\n");
    return Status;
  }

  KvmDev->ControllerNameTable = NULL;
  AddUnicodeString2 (
    "eng",
    MpmKvmComponentName.SupportedLanguages,
    &KvmDev->ControllerNameTable,
    L"MPM KVM Virtual KB/MS Device",
    TRUE
    );

  AddUnicodeString2 (
    "en",
    MpmKvmComponentName2.SupportedLanguages,
    &KvmDev->ControllerNameTable,
    L"MPM KVM Virtual KB/MS Device",
    FALSE
    );

  mMpmKvmDriverStarted = TRUE;
  return Status;
}

/**
 * @brief STOP function of driver binding protocol
 *
 * @param This
 * @param Controller
 * @param NumberOfChildren
 * @param ChildHandleBuffer
 * @return EFI_STATUS
 */
EFI_STATUS
EFIAPI
MpmKvmDriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN  EFI_HANDLE                     Controller,
  IN  UINTN                          NumberOfChildren,
  IN  EFI_HANDLE                     *ChildHandleBuffer
  )
{
  EFI_STATUS Status;
  UINT32     KvmMouseProtocolSelection;
  Status = KvmKbdControllerDriverStop(This, Controller, NumberOfChildren, ChildHandleBuffer);
  if (EFI_ERROR(Status)){
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmKbdDriver Stop Error!\n");
    return Status;
  }
  KvmMouseProtocolSelection = PcdGet8(PcdMpmKvmMouseProtocol);
  if (KvmMouseProtocolSelection == KVM_ABSOLUTE_MOUSE_PROTOCOL) {
    Status = KvmAbsoluteMouseDriverStop(This, Controller, NumberOfChildren, ChildHandleBuffer);
    IDS_HDT_CONSOLE_PSP_TRACE ("Stop KvmAbsoluteMouseDriver\n");
  }
  else if (KvmMouseProtocolSelection == KVM_SIMPLE_MOUSE_PROTOCOL) {
    Status = KvmSimpleMouseDriverStop(This, Controller, NumberOfChildren, ChildHandleBuffer);
    IDS_HDT_CONSOLE_PSP_TRACE ("Stop KvmSimpleMouseDriver\n");
  }
  else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Unknown Mouse Protocol Selection: 0x%x\n", KvmMouseProtocolSelection);
    return EFI_UNSUPPORTED;
  }
  if (EFI_ERROR(Status)){
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmMouseDriver Stop Error!\n");
    return Status;
  }
  return Status;
}
/**
 * @brief Name function of driver binding protocol
 *
 * @param This
 * @param Language
 * @param DriverName
 * @return EFI_STATUS
 */
EFI_STATUS
EFIAPI
MpmKvmComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  )
{
  return LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           mMpmKvmDriverNameTable,
           DriverName,
           (BOOLEAN)(This == &MpmKvmComponentName)
           );
}
/**
 * @brief Name function of driver binding protocol
 *
 * @param This
 * @param ControllerHandle
 * @param OPTIONAL
 * @param Language
 * @param ControllerName
 * @return EFI_STATUS
 */
EFI_STATUS
EFIAPI
MpmKvmComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL                     *This,
  IN  EFI_HANDLE                                      ControllerHandle,
  IN  EFI_HANDLE                                      ChildHandle        OPTIONAL,
  IN  CHAR8                                           *Language,
  OUT CHAR16                                          **ControllerName
  )
{
  EFI_STATUS                                  Status;
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL              *ConIn;
  KVM_KEYBOARD_CONSOLE_IN_DEV                 *ConsoleIn;
  MPM_KVM_DEV                             *KvmDev;
  EFI_PCI_IO_PROTOCOL                         *PciIo;

  //
  // This is a device driver, so ChildHandle must be NULL.
  //
  if (ChildHandle != NULL) {
    return EFI_UNSUPPORTED;
  }
  //
  // Check Controller's handle
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  gMpmKvmDriver.DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (!EFI_ERROR (Status)) {
    gBS->CloseProtocol (
           ControllerHandle,
           &gEfiPciIoProtocolGuid,
           gMpmKvmDriver.DriverBindingHandle,
           ControllerHandle
           );

    return EFI_UNSUPPORTED;
  }

  if (Status != EFI_ALREADY_STARTED) {
    return EFI_UNSUPPORTED;
  }
  //
  // Get the device context
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiSimpleTextInProtocolGuid,
                  (VOID **) &ConIn,
                  gMpmKvmDriver.DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ConsoleIn = KVM_KEYBOARD_CONSOLE_IN_DEV_FROM_THIS (ConIn);
  KvmDev =  MPM_KVM_DEV_FROM_THIS(ConsoleIn);

  return LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           KvmDev->ControllerNameTable,
           ControllerName,
           (BOOLEAN)(This == &MpmKvmComponentName)
           );
}
//#define FORCE_ENABLE_QCA6390
/**
 * @brief Enable Bus Master for WLAN device and the root port which WLAN device is connected
 *
 * @param DeviceBusNumber WLAN device bus number
 * @return EFI_STATUS
 */

EFI_STATUS
KvmWlanRootBridgeBusMasterEnable (
  IN UINTN  DeviceBusNumber
)
{
   EFI_STATUS           Status;
   EFI_HANDLE           *PciIoHandles;
   UINTN                NoPciIoHandles;
   UINTN                Index;
   EFI_PCI_IO_PROTOCOL  *PciIo;
   UINTN                SegmentNumber;
   UINTN                BusNumber;
   UINTN                DeviceNumber;
   UINTN                FunctionNumber;
   UINT32               ClassCodeValue;
   UINT8                SubordinateBusNumber;
   UINT8                CommandValue;

   IDS_HDT_CONSOLE_PSP_TRACE ("KvmWlanRootBridgeBusMasterEnable DeviceBusNumber is %d\n", DeviceBusNumber);
   Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &NoPciIoHandles,
                  &PciIoHandles
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  for (Index = 0; Index < NoPciIoHandles; Index++) {
    Status = gBS->HandleProtocol (
        PciIoHandles[Index],
        &gEfiPciIoProtocolGuid,
        &PciIo
        );
    if (EFI_ERROR (Status)) {
      continue;
    }
    PciIo->GetLocation(
              PciIo,
              &SegmentNumber,
              &BusNumber,
              &DeviceNumber,
              &FunctionNumber
    );
    PciIo->Pci.Read (
      PciIo,
      EfiPciIoWidthUint32,
      CLASS_CODE_REGISTER_OFFSET,
      1,
      &ClassCodeValue
      );
    if ((ClassCodeValue & HOST_BRIDGE_CLASS_CODE) == HOST_BRIDGE_CLASS_CODE) {
      PciIo->Pci.Read (
        PciIo,
        EfiPciIoWidthUint8,
        SUBORDINATE_BUS_NUMBER,
        1,
        &SubordinateBusNumber
      );
      if (SubordinateBusNumber == DeviceBusNumber) {
        IDS_HDT_CONSOLE_PSP_TRACE ("SubordinateBusNumber is %d\n", SubordinateBusNumber);
        IDS_HDT_CONSOLE_PSP_TRACE ("RootBridge connected to WLAN device is found, set Bus master enable\n");
        CommandValue = COMMAND_REGISTER_VALUE;
        Status = PciIo->Pci.Write (
                   PciIo,
                   EfiPciIoWidthUint32,
                   COMMAND_REGISTER_OFFSET,
                   1,
                   &CommandValue
                 );
      }
    }
  }
  return Status;
}
/**
 * @brief Detect KVM supported NIC through PCI scan on given type
 *
 * @param KvmRequest Point to value use to hold request status
 * @return EFI_STATUS
 */
EFI_STATUS
KvmDeviceDetect (
  IN     UINT32                *KvmRequest
  )
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  EFI_HANDLE           *PciIoHandles;
  UINTN                NoPciIoHandles;
  UINTN                Index;
  UINT32               KvmDevNum;
  EFI_STATUS           Status;
  UINT32               KvmCapSts;
  MPM_PRVIATE_HEAP_DATA_V1  *MpmData;
  UINTN                SegmentNumber;
  UINTN                BusNumber;
  UINTN                DeviceNumber;
  UINTN                FunctionNumber;

  PciIoHandles = NULL;
  PciIo = NULL;
  *KvmRequest = KVM_NOT_REQUESTED;
  KvmCapSts  = NO_KVM_CAPABLE_DEV;

  IDS_HDT_CONSOLE_PSP_TRACE ("KvmPciScan\n");
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &NoPciIoHandles,
                  &PciIoHandles
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = LocateMpmPrivateData (&MpmData);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("LocateMpmPrivateData Fail!\n");
    return Status;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("%x PciIoHandles for Scan\n", NoPciIoHandles);
  //Loop All PCI handles
  for (Index = 0; Index < NoPciIoHandles; Index++) {
    Status = gBS->HandleProtocol (
        PciIoHandles[Index],
        &gEfiPciIoProtocolGuid,
        &PciIo
        );
    if (EFI_ERROR (Status)) {
      continue;
    }
    //Search if current handle support KVM
    for (KvmDevNum = 0; KvmDevNum < GetMpmDevProtocolNum (); KvmDevNum++) {
      //If wired KVM is disabled, skip wired device detection
      if ((PcdGetBool (PcdMpmWiredKvmEnable) == FALSE) && mMpmDevProtocols [KvmDevNum]->DeviceType == MPM_NIC_WIRED) {
        continue;
      }
      //If wireless KVM is disabled, skip wireless device detection
      if ((PcdGetBool (PcdMpmWirelessKvmEnable) == FALSE) && mMpmDevProtocols [KvmDevNum]->DeviceType == MPM_NIC_WIRELESS) {
        continue;
      }
      if (mMpmDevProtocols [KvmDevNum]->IsSupported (PciIoHandles[Index])) {
        //Record the last supported device, but keep searching the list for the support device with request set
        KvmCapSts = KVM_CAPABLE_DEV;
        mMpmKvmDev.KvmDevProtocol = mMpmDevProtocols [KvmDevNum];
        mMpmKvmDev.KvmDevPciIo = PciIo;
        mMpmKvmDev.KvmDevHandle = PciIoHandles[Index];
        mMpmDevProtocols [KvmDevNum]->ReadKvmRequest (mMpmKvmDev.KvmDevHandle, KvmRequest);
        //We found the device with request bit set, it is what we are searching for, exit the loop
        if (*KvmRequest == KVM_REQUESTED) {
          //If wired NIC detected, update the status to 0
          MpmData->WirelessKvmFlag = (mMpmDevProtocols [KvmDevNum]->DeviceType == MPM_NIC_WIRED)? 0:1;
          //Always take wired KVM as priority
          if (mMpmDevProtocols [KvmDevNum]->DeviceType == MPM_NIC_WIRED) {
            break;
          }
          //Enable Root Port Bus Master Enable bit which WLAN device is connected
          //Get the Root Port Bus/Dev/Fun which WLAN device connected
          Status = PciIo->GetLocation(
                            PciIo,
                            &SegmentNumber,
                            &BusNumber,
                            &DeviceNumber,
                            &FunctionNumber
          );
          if (EFI_ERROR(Status)) {
             return Status;
          }
          IDS_HDT_CONSOLE_PSP_TRACE ("WLAN device Bus number is %d\n", BusNumber);
          KvmWlanRootBridgeBusMasterEnable (BusNumber);
        }
      }
    }
    //Exit main loop
    if (*KvmRequest == KVM_REQUESTED) {
      break;
    }
  }
#ifdef FORCE_ENABLE_QCA6390
  // For test purpose only, if no capable device found, force to use QCA6390
  if (KvmCapSts != KVM_CAPABLE_DEV) {
    mMpmKvmDev.KvmDevProtocol = &mQcaHastingsProtocol;
    mMpmKvmDev.KvmDevPciIo = PciIo;
    mMpmKvmDev.KvmDevHandle = PciIoHandles[0];
    KvmCapSts = KVM_CAPABLE_DEV;
    *KvmRequest = KVM_REQUESTED;
  }
#endif
  if (KvmCapSts == KVM_CAPABLE_DEV) {
    // mMpmKvmDev.NetCardInfo.NetCardDidVid = VendorDevice;
    // mMpmKvmDev.NetCardInfo.NetCardClassRevID = ClassRevID;
    // mMpmKvmDev.KvmDevProtocol->GetNetCardBarBase(mMpmKvmDev.KvmDevHandle, &mMpmKvmDev.NetCardInfo);
    Status = mMpmKvmDev.KvmDevProtocol->GetMmioBase(mMpmKvmDev.KvmDevHandle, &mMpmKvmDev.NetCardMmioBase);
    IDS_HDT_CONSOLE_PSP_TRACE ("Netcard mmioBase %x\n", mMpmKvmDev.NetCardMmioBase);
    Status = mMpmKvmDev.KvmDevProtocol->GetMmioSize(mMpmKvmDev.KvmDevHandle, &mMpmKvmDev.NetCardMmioSize);
    IDS_HDT_CONSOLE_PSP_TRACE ("Netcard mmioSize %x\n", mMpmKvmDev.NetCardMmioSize);
    mKvmCapableDevDetected = TRUE;
  }

  return EFI_SUCCESS;
}

/**
 * @brief Post Kvm Service init, support both wireless and wired KVM
 *
 * @return EFI_STATUS
 */
EFI_STATUS
EFIAPI
PostKvmServiceInit (
  VOID
  )
{
  EFI_STATUS                            Status;
  EFI_SMBIOS_PROTOCOL                   *Smbios;
  SMBIOS_TABLE_TYPEA0                   *AmdSmbiosTableTypeA0 = NULL;
  UINT32                                AmdKvmRequest = KVM_NOT_REQUESTED;
  UINT32                                Index;
  EFI_HANDLE                            KvmQuestHandle = NULL;
  EFI_HANDLE                            *GraphicsOutputHandles;
  EFI_HANDLE                            GopHandle;
  UINTN                                 NoGraphicsOutputHandles;
  UINTN                                 SizeOfInfo;
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *GraphicsOutput;
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE     GopMode;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *Info;
  BOOLEAN                               KvmInitiated = FALSE;
  VOID                                  *Interface;

  IDS_HDT_CONSOLE_PSP_TRACE("PostKvmServiceInit - enter\n");

  //Call AmdPspKvmService.RetrieveSmbiosTable to get SMBIOS data
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE("Locate gEfiSmbiosProtocolGuid failed, status = %r.\n", Status);
    return Status;
  }

  Status = gBS->LocateProtocol (&gAmdNbioIommuProtocolGuid, NULL, &Interface);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE("PostKvmServiceInit: IOMMU is disabled\n");
  }

  Status = mMpmKvmServiceProtocol.RetrieveSmbiosTable (KVM_SMBIOS_KVM_ENABLED ,&AmdSmbiosTableTypeA0);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("RetrieveSmbiosTable failed, Status = %r.\n", Status);
    return Status;
  }
  Status = Smbios->Add (Smbios, NULL, &AmdSmbiosTableTypeA0->Hdr.Handle, (EFI_SMBIOS_TABLE_HEADER *)AmdSmbiosTableTypeA0);

  //Call AmdPspKvmService.GetRequest, if yes:
  //Prepare Graphic mode information used to send with InitiateKvm
  //Call AmdPspKvmService.InitiateKvm
  Status = mMpmKvmServiceProtocol.GetRequest (&AmdKvmRequest);
  if (!EFI_ERROR (Status)) {
    if (AmdKvmRequest == KVM_REQUESTED) {
      Status = gBS->LocateHandleBuffer (
          ByProtocol,
          &gEfiGraphicsOutputProtocolGuid,
          NULL,
          &NoGraphicsOutputHandles,
          &GraphicsOutputHandles
          );
      if (EFI_ERROR(Status) || NoGraphicsOutputHandles == 0) {
        IDS_HDT_CONSOLE_PSP_TRACE("Locate Graphic Handle Fail\n");
        return Status;
      }
      GopHandle = GraphicsOutputHandles[0];
      Status = gBS->HandleProtocol (
          GopHandle,
          &gEfiGraphicsOutputProtocolGuid,
          &GraphicsOutput
          );
      if (EFI_ERROR(Status)) {
        return Status;
      }
      for (Index = 0; Index < GraphicsOutput->Mode->MaxMode; Index++) {
        Status = GraphicsOutput->QueryMode (GraphicsOutput, Index, &SizeOfInfo, &Info);
        if (Info->HorizontalResolution == PcdGet16 (PcdMpmKvmHorizontalResolution) && Info->VerticalResolution == PcdGet16(PcdMpmKvmVerticalResolution)) {
          CopyMem (&GopMode, GraphicsOutput->Mode, sizeof(EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));
          GopMode.Info = Info;
          mMpmKvmServiceProtocol.InitiateKvm(&GopMode, NULL);
          KvmInitiated = TRUE;
          break;
        }
        if (Status == EFI_SUCCESS) {
          FreePool(Info); //QueryMode will allocate memory, need free it
        }
      }
      if (KvmInitiated) {
        Status = gBS->InstallProtocolInterface (
                  &KvmQuestHandle,
                  &gAmdMpmKvmInitializedProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
        if (EFI_ERROR (Status)) {
          return Status;
        }
      }
    }
    IDS_HDT_CONSOLE_PSP_TRACE( "PostKvmServiceInit - exit\n");
    return Status;
  }
  return Status;
}

/**
 * @brief Entry point of MPM KVM driver
 *
 * @param ImageHandle
 * @param SystemTable
 * @return EFI_STATUS
 */
EFI_STATUS
EFIAPI
InitializeKvmDriver(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
  EFI_STATUS              Status;
  VOID                    *TempBuffer;
  VOID                    *Interface;
  EFI_HANDLE              Handle;
  BOOLEAN                 AmfFwBinaryExist;
  UINT32                  KvmRequest;
  UINT32                  HSTIState;
  UINTN                   BufferSize;
  VOID                    *Mapping;
  EFI_PHYSICAL_ADDRESS    MappedAddr;
  MBOX_QUERY_KVM          MboxQueryKvm;
  MPM_PRVIATE_HEAP_DATA_V1  *MpmData;
  UINT32                  MpmMmioBase = 0;

  KvmRequest = KVM_NOT_REQUESTED;
  HSTIState = 0;
  if ((PcdGetBool (PcdMpmWiredKvmEnable) == FALSE) && (PcdGetBool (PcdMpmWirelessKvmEnable) == FALSE)) {
    return EFI_UNSUPPORTED;
  }

  // Check MPM Enable
  if (!IsMpmEnabled()) {
    return EFI_UNSUPPORTED;
  }

  //Family supported check
  // if (SocFamilyIdentificationCheck (F19_RMB_RAW_ID)) {
    //Check KVM capability
    //todo Temp disable PRO part check, need added it back
    // PspMboxBiosQueryHSTIState (&HSTIState);
    // if ((HSTIState & PSP_RYZEN_PRO_SKU_FLAG) == 0) {
    //   return EFI_SUCCESS;
    // }
  // }

  IDS_HDT_CONSOLE_PSP_TRACE ("InitializeKvmDriver >>\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("KVM PCI SCAN\n");
  Status = gBS->LocateProtocol (
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Locate PciIO protocol Fail\n");
    return Status;
  }
  //Scan PCI to find KVM capable device, check if any device set request

  Status = KvmDeviceDetect (&mKvmDeviceRequest);

  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmDeviceDetect Fail!\n");
    return Status;
  }
  //Search for the Entry address and size of KVM Engine binary
  AmfFwBinaryExist = PSPEntryInfoV2(MPM_FW_2, &mMpmKvmDev.KvmEntryAddress, &mMpmKvmDev.KvmEntrySize);
  if (mKvmCapableDevDetected && AmfFwBinaryExist) {
    if (mKvmDeviceRequest == KVM_REQUESTED) {
      Status = LocateMpmPrivateData (&MpmData);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("LocateMpmPrivateData Fail!\n");
        return Status;
      }
      //Enable MPM PCI Access
      PspMboxBiosMpmPciAccess (ENABLE_MPM_PCI_ACC, MpmData->WirelessKvmFlag);
      if ((MpmData->KvmKeyboardBufAddr == 0xFFFFFFFFFFFFFFFFull)) {
        ZeroMem (&MboxQueryKvm, sizeof (MBOX_QUERY_KVM));
        Status = BiosMpmCmdQueryKvm (&MboxQueryKvm);
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_PSP_TRACE ("BiosMpmCmdQueryKvm Fail!\n");
          return Status;
        }
        //Initial KB MS ring buffer
        mKbRingBuffer = (KB_RING_BUFFER *) (UINTN) (MpmData->MpmDramAddr + MboxQueryKvm.KeyboardBufOffset);
        mMRingBuffer = (KVM_MOUSE_BUFFER *) (UINTN) (MpmData->MpmDramAddr + MboxQueryKvm.MouseBufOffset);
      } else {
        mKbRingBuffer = (KB_RING_BUFFER *) (UINTN) (MpmData->KvmKeyboardBufAddr);
        mMRingBuffer = (KVM_MOUSE_BUFFER *) (UINTN) (MpmData->KvmMouseBufAddr);
      }

      IDS_HDT_CONSOLE_PSP_TRACE ("mKbRingBuffer %x mMRingBuffer %x\n", mKbRingBuffer, mMRingBuffer);
      //Buffer for DMA
      TempBuffer = (VOID *) (UINTN) MpmData->WirelessDmaBufAddr;
      mMpmKvmDev.KvmDmaAddr = (UINT64) (UINTN) (TempBuffer);
      mKvmDmaHpaAddr = mMpmKvmDev.KvmDmaAddr;
      if (PcdGetBool(PcdIvInfoDmaReMap)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Allocate Buffer for DMA\n");
        ASSERT (mMpmKvmDev.KvmDevPciIo != NULL);
        if (mMpmKvmDev.KvmDevPciIo != NULL) {
          BufferSize = MPM_DMA_HPA_BUF_SIZE;
          Status = mMpmKvmDev.KvmDevPciIo->Map (mMpmKvmDev.KvmDevPciIo,
                          EfiPciIoOperationBusMasterCommonBuffer,
                          (VOID *)TempBuffer,
                          &BufferSize,
                          &MappedAddr,
                          &Mapping
                          );
          if (EFI_ERROR (Status)) {
            IDS_HDT_CONSOLE_PSP_TRACE ("KvmDevPciIo->Map error\n");
            return Status;
          }
          mKvmDmaGpaAddr = (UINT64) MappedAddr;
          IDS_HDT_CONSOLE_PSP_TRACE (" GPA:MappedAddr = 0x%08x%08x, BufferSize = 0x%x\n", (UINT32)(mKvmDmaGpaAddr >> 32), (UINT32)mKvmDmaGpaAddr, BufferSize);

          MpmMmioBase = GetMpmBarAddr();
          if (MpmMmioBase == 0) {
            IDS_HDT_CONSOLE_PSP_TRACE ("MPM MMIO not initialized\n");
            return EFI_UNSUPPORTED;
          }
          BufferSize = MPM_C2PMSG_2_SIZE;
          mKvmMpmC2pMsg2HpaAddr = MpmMmioBase + MPM_C2PMSG_2_OFFSET; //Wlan FW get interrpt number from this register
          TempBuffer = (VOID *) (UINTN) mKvmMpmC2pMsg2HpaAddr;
          Status = mMpmKvmDev.KvmDevPciIo->Map (mMpmKvmDev.KvmDevPciIo,
                          EfiPciIoOperationBusMasterCommonBuffer,
                          (VOID *)TempBuffer,
                          &BufferSize,
                          &MappedAddr,
                          &Mapping
                          );
          if (EFI_ERROR (Status)) {
            IDS_HDT_CONSOLE_PSP_TRACE ("KvmDevPciIo->Map error\n");
            return Status;
          }
          mKvmMpmC2pMsg2GpaAddr = (UINT64) MappedAddr;
          IDS_HDT_CONSOLE_PSP_TRACE (" MpmC2pMsg2 HPA address: 0x%x, MpmC2pMsg2 GPA:MappedAddr = 0x%08x%08x, BufferSize = 0x%x\n", mKvmMpmC2pMsg2HpaAddr, (UINT32)(mKvmMpmC2pMsg2GpaAddr >> 32), (UINT32)mKvmMpmC2pMsg2GpaAddr, BufferSize);
        }
      }

      //Install Driver binding protocol
      Status = EfiLibInstallDriverBindingComponentName2 (
                 ImageHandle,
                 SystemTable,
                 &gMpmKvmDriver,
                 ImageHandle,
                 &MpmKvmComponentName,
                 &MpmKvmComponentName2
                 );
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Locate Kvm Driver Fail\n");
        return Status;
      }
      RegisterMpmBiosCmdTimerEvent();
    }
    //Publish KVM service
    IDS_HDT_CONSOLE_PSP_TRACE ("Install PSP KVM protocol\n");
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gAmdPspKvmServiceProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mMpmKvmServiceProtocol
                    );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Install PSP KVM protocol Fail\n");
      return Status;
    }
    Status = PostKvmServiceInit();
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PostKvmServiceInit Fail\n");
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("KVM Dev Detect Fail or AmfFwBinary Not Exist\n");
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("InitializeKvmDriver <<\n");
  return Status;
}


