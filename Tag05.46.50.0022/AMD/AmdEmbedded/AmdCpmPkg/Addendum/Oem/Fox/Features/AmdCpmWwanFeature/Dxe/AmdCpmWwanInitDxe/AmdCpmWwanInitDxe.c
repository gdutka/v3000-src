/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>


EFI_GUID AMD_WWAN_SSDT_GUID = {
  0xFD195A94, 0x3730, 0xCD78, 0xAE, 0xFD, 0x55, 0xBD, 0x9A, 0x6F, 0x40, 0x0D
};

VOID
EFIAPI
AmdCpmOemWwanSsdtInstall (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  );

BOOLEAN
EFIAPI
AmdCpmWwanCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to override WWAN SSDT Table
 *
 * This function is used to update WWAN name.
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
AmdCpmWwanCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{

  return FALSE;
}

/**
 * The function to load AMD CPM WWAN SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmOemWwanSsdtInstall (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  )
{
  STATIC BOOLEAN           InitlateInvoked = FALSE;
  EFI_STATUS               Status;
  AMD_CPM_TABLE_PROTOCOL   *CpmTableProtocolPtr;
  AMD_PBS_SETUP_OPTION     AmdPbsConfiguration;
  UINT32                    u32PcieMmioAddr;

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemWwanSsdtInstall-start\n"));

  if (!InitlateInvoked) {

    Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
    if (EFI_ERROR (Status) || AmdPbsConfiguration.WwanPowerEn == 0) {
      return;
    }
    u32PcieMmioAddr = (UINT32)PcdGet64 (PcdPciExpressBaseAddress) + (0x0 << 20) + (0x1 << 15) + (0x3 << 12) + 0x00;
    if (0x1022 != *((volatile UINT16*)(UINTN)(u32PcieMmioAddr)))
    {
      InitlateInvoked = TRUE;
      return;
    }
    if (0xFF == *((volatile UINT8*)(UINTN)(u32PcieMmioAddr + 0x19)))
    {
      InitlateInvoked = TRUE;
      return;
    }

    Status = gBS->LocateProtocol (
                    &gAmdCpmTableProtocolGuid,
                    NULL,
                    (VOID**)&CpmTableProtocolPtr
                    );
    if (EFI_ERROR (Status)) {
      return;
    }
    switch (AmdPbsConfiguration.PbsWWANDeviceSupport) {
    case 0:
    case 1: // Fibocom L860 R+
      CpmTableProtocolPtr->CommonFunction.AddSsdtTable (
                                            (VOID*)CpmTableProtocolPtr,
                                            &AMD_WWAN_SSDT_GUID,
                                            NULL,
                                            NULL,
                                            NULL
                                            );
      break;
    default:
      break;
    }
  }

  InitlateInvoked = TRUE;

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemWwanSsdtInstall-end\n"));

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM WWAN Init DXE driver
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmWwanInitDxeEntryPoint (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               ReadyToBootEvent;

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmWwanInitDxeEntryPoint-Start\n"));

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  //install WWAN SSDT
  Status = gBS->CreateEventEx (
                  CPM_EVENT_NOTIFY_SIGNAL,
                  CPM_TPL_CALLBACK,
                  AmdCpmOemWwanSsdtInstall,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmWwanInitDxeEntryPoint-End-Status=%r\n", Status));
  return EFI_SUCCESS;
}
