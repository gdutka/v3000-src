/** @file

;*******************************************************************************
;* Copyright (c) 2013 - 2017, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#include <KVMSmbiosDxe.h>

/**

  Force enter TerminateKvm Callback function when H2OBdsCpSendFormAfterGuid event is signaled

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.

**/

VOID
TerminateKvmCallBack (
  IN EFI_EVENT         Event,
  IN VOID              *Context
  )
{
  EFI_STATUS                        Status;
  AMD_PSP_KVM_SERVICE_PROTOCOL      *KvmService; 
  
  Status = gBS->LocateProtocol (&gAmdPspKvmServiceProtocolGuid, NULL, (VOID **)&KvmService);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KVM service not installed\n"));
    return;
  }  
  
  Status = KvmService->TerminateKvm();
  DEBUG((DEBUG_INFO, "TerminateKvm Status=%x\n", Status));  
  if (!EFI_ERROR (Status)) {
    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
  }

}

/**

  Connect PSP controller for install KVM keyboard & mouse protocol.

**/
EFI_STATUS
ConnectPspKvmKbMs (
) 
{
  EFI_STATUS                         Status;
  UINTN                              NumberOfPciIoHandles;
  UINTN                              Index; 
  EFI_HANDLE                         *PciIoHandles;   
  EFI_PCI_IO_PROTOCOL                *PciIo;
  PCI_DEVICE_INDEPENDENT_REGION      DeviceHdr;
    
  DEBUG((DEBUG_INFO, "ConnectPspKvmKbMs Entry...\n"));

  Status = gBS->LocateHandleBuffer (
          ByProtocol,
          &gEfiPciIoProtocolGuid,
          NULL,
          &NumberOfPciIoHandles,
          &PciIoHandles
          );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_INFO, "  Locate PciIo handle buffer failed: %r.\n", Status));
    return Status;
  }  
  
  for (Index = 0; Index < NumberOfPciIoHandles; Index++) {
    Status = gBS->HandleProtocol (
          PciIoHandles[Index],
          &gEfiPciIoProtocolGuid,
          &PciIo
          );
     if (!EFI_ERROR(Status)) {
       Status = PciIo->Pci.Read (
                  PciIo,
                  EfiPciIoWidthUint8,
                  0,
                  sizeof (DeviceHdr),
                  &DeviceHdr
                  );
       if (!EFI_ERROR(Status)) {
         if ((0x1022 == DeviceHdr.VendorId) && (0x15DF == DeviceHdr.DeviceId)) {
           DEBUG((DEBUG_INFO, "    Found PSP Controller, Connecting...\n"));   
           return gBS->ConnectController (PciIoHandles[Index], NULL, NULL, TRUE);      
         }
       }     
       
     }  
  }                 
  DEBUG((DEBUG_INFO, "ConnectPspKvmKbMs Exit...\n"));          
  return EFI_SUCCESS;
}


/**

  Force enter setup menu when H2OBdsCpBootDeviceSelectProtocol event is signaled

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.

**/
VOID
KVMEnterSetup (
  IN EFI_EVENT         Event,
  IN VOID              *Context
  )
{
  EFI_STATUS                        Status;
  H2O_BDS_CP_HANDLE                 CpHandle; 

  //
  // force enter the callback function to do TerminateKvm
  //
  BdsCpRegisterHandler (
    &gH2OBdsCpSendFormAfterGuid,
    TerminateKvmCallBack,
    H2O_BDS_CP_LOW + 1,
    &CpHandle
    ); 

  //
  // Close event, so it will not be invoked again.
  //
  gBS->CloseEvent (Event);
  ConnectPspKvmKbMs();    
  DEBUG ((DEBUG_INFO, "BdsLibStartSetupUtility ...Start\n"));
  Status = BdsLibStartSetupUtility (FALSE);

}

/**

  Create SMBIOS and force enter setup menu when AmdPspKvmServiceProtocol event is signaled

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
PspKvmServiceCallBack (
  IN     EFI_EVENT                      Event,
  IN     VOID                           *Context
  )
{
  EFI_STATUS                            Status;
  BOOLEAN                               KvmInitiated;
  AMD_PSP_KVM_SERVICE_PROTOCOL          *KvmService;
  UINT32                                KvmRequest;
  UINT32                                Index;
  EFI_HANDLE                            *GraphicsOutputHandles;
  EFI_HANDLE                            GopHandle;
  UINTN                                 NoGraphicsOutputHandles;
  UINTN                                 SizeOfInfo;
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *GraphicsOutput;
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE     GopMode;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *Info;
  H2O_BDS_CP_HANDLE                     CpHandle; 
  CHIPSET_CONFIGURATION                 SystemConfiguration;   
  SMBIOS_TABLE_TYPEA0                   *SmbiosTableTypeA0;
  EFI_SMBIOS_PROTOCOL                   *Smbios;
  KVM_IP_INFO                           KvmIpInfo;
   
  KvmRequest = KVM_NOT_REQUESTED;
  KvmInitiated = FALSE;
  SmbiosTableTypeA0 = NULL;  

  Status = gBS->LocateProtocol (&gAmdPspKvmServiceProtocolGuid, NULL, (VOID **)&KvmService);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "KVM service not installed Status = %x.\n", Status));
    return;
  }
  //
  // Retrieve SBIOS data and install and Publish KVM information SBIOS table
  //
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  if (EFI_ERROR (Status)) {
    return;
  }
  Status = GetChipsetConfiguration (&SystemConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "GetKernelConfiguration failed Status = %x.\n", Status));
    return ;
  }
  if (SystemConfiguration.DashSupport == 0){
    DEBUG ((EFI_D_INFO, "Dash Support is Disabled\n"));
    return;
  }
  if (SystemConfiguration.AmdPspKvmControl == 0){
    Status = KvmService->RetrieveSmbiosTable (KVM_SMBIOS_KVM_DISABLED ,&SmbiosTableTypeA0);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "RetrieveSmbiosTable failed Status = %x.\n", Status));
      return;
    }
    Status = Smbios->Add (Smbios, NULL, &SmbiosTableTypeA0->Hdr.Handle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosTableTypeA0);
    return;
  }

  Status = KvmService->RetrieveSmbiosTable (KVM_SMBIOS_KVM_ENABLED ,&SmbiosTableTypeA0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "RetrieveSmbiosTable failed Status = %x.\n", Status));
    return;
  }  
  Status = Smbios->Add (Smbios, NULL, &SmbiosTableTypeA0->Hdr.Handle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosTableTypeA0); 
  
  //
  //  Prepare Graphic mode information used to send with InitiateKvm
  // 
  Status = KvmService->GetRequest (&KvmRequest);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "AmdPspKvmGetRequest Fail\n"));
  } else {
    if (KvmRequest == KVM_REQUESTED) {
      Status = gBS->LocateHandleBuffer (
          ByProtocol,
          &gEfiGraphicsOutputProtocolGuid,
          NULL,
          &NoGraphicsOutputHandles,
          &GraphicsOutputHandles
          );
      if (EFI_ERROR(Status) || NoGraphicsOutputHandles == 0) {
        DEBUG((DEBUG_INFO, "Locate Graphic Handle Fail\n"));
        return;
      }
      GopHandle = GraphicsOutputHandles[1];
      Status = gBS->HandleProtocol (
          GopHandle,
          &gEfiGraphicsOutputProtocolGuid,
          &GraphicsOutput
          );
      if (EFI_ERROR(Status)) {
        return;
      }
      for (Index = 0; Index < GraphicsOutput->Mode->MaxMode; Index++) {
        Status = GraphicsOutput->QueryMode (GraphicsOutput, Index, &SizeOfInfo, &Info);
        if (Info->HorizontalResolution == 1024 && Info->VerticalResolution == 768) {
          CopyMem (&GopMode, GraphicsOutput->Mode, sizeof(EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));
          GopMode.Info = Info;
		  KvmIpInfo.NetworkType = 0;
          KvmService->InitiateKvm(&GopMode, &KvmIpInfo);
          KvmInitiated = TRUE;
          break;
        }
      }
      if (!KvmInitiated) {
        DEBUG((DEBUG_INFO, "KVM Expected GOP Mode Not Found\n"));
      } else {
        DEBUG((DEBUG_INFO, "KVM Requested force setup\n"));
        //
        // force entersetup screen
        // 
        BdsCpRegisterHandler (
          &gH2OBdsCpBootDeviceSelectProtocolGuid,
          KVMEnterSetup,
          H2O_BDS_CP_LOW,
          &CpHandle
          ); 
      }
    }
  }



  gBS->CloseEvent(Event);

}

/**
  The user Entry Point for module KVMSmbiosDxe.  The user code starts with this function.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
KVMSmbiosEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                        Status;
  CHIPSET_CONFIGURATION             SystemConfiguration;    
  VOID                              *Registration;
  
  Status = GetChipsetConfiguration (&SystemConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "GetKernelConfiguration failed Status = %x.\n", Status));
    return Status;
  }

  // if (SystemConfiguration.AmdPspKvmControl == 1) {
    EfiCreateProtocolNotifyEvent (
      &gAmdPspKvmServiceProtocolGuid,
      TPL_CALLBACK,
      PspKvmServiceCallBack,
      NULL,
      &Registration
      );
  // }
  
  return Status;
}

