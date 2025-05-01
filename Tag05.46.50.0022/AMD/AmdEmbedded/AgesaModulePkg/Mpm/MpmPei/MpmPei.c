/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


#include "PiPei.h"
#include "AGESA.h"
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/BaseLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/MpmFuncLib.h>
#include <Library/MpmMboxLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/FabricResourceManagerLib.h>
#include <Filecode.h>

#define FILECODE MPM_MPMPEI_MPMPEI_FILECODE

#define MPM_MMIO_SIZE 0x20000

STATIC UINT32 mMpmDataInitializedPpi = {
  0x01
};

STATIC EFI_PEI_PPI_DESCRIPTOR mMpmDataInitializededPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdMpmDataInitializedPpiGuid,
  &mMpmDataInitializedPpi
};

/*++

Routine Description:

  Initialize MPM MMIO

Arguments:

Returns:

  AGESA_SUCCESS: MPM MMIO init succeed
  AGESA_CRITICAL: MMIO allocated fail
  AGESA_ERROR: MMIO address doesn't match requirement

--*/
AGESA_STATUS
MpmMmioInit (
  VOID
  )
{
  UINT32                 Value32;
  UINT64                 MmioBase;
  UINT64                 Length;
  FABRIC_TARGET          MmioTarget;
  EFI_STATUS             EfiStatus;
  FABRIC_MMIO_ATTRIBUTE  Attributes;
  UINT8                  RbNumber;

  RbNumber = 0;

  //Allocate MMIO Region from MMIO manager for MPM
  Length = MPM_MMIO_SIZE;
  MmioTarget.TgtType = TARGET_RB;
  MmioTarget.SocketNum = 0;
  MmioTarget.RbNum = RbNumber;
  Attributes.ReadEnable = 1;
  Attributes.WriteEnable = 1;
  Attributes.NonPosted = 0;
  Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;
  MmioBase  = 0;
  IDS_HDT_CONSOLE_PSP_TRACE ("MpmMmioInit size 0x%x\n", Length);
  EfiStatus = FabricAllocateMmio (&MmioBase, &Length, ALIGN_1M, MmioTarget, &Attributes);
  ASSERT (EfiStatus == EFI_SUCCESS );
  if (EFI_ERROR (EfiStatus)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Allocate MMIO Fail\n");
    return AGESA_CRITICAL;
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Allocate MMIO @0x%lx\n", MmioBase);
  }

  //Program  IOHC::MPM_BASE_ADDR_LO, Lock and enable
  //Make sure 1M aligned
  ASSERT ((MmioBase & 0xFFFFF) == 0);
  if ((MmioBase & 0xFFFFF) != 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("MMIO not 1M aligned\n");
    return AGESA_ERROR;
  }
  Value32 = (UINT32) (MmioBase | (BIT0 + BIT1));
  SmnRegisterWrite (0, IOHC_MPM_BASE_ADDR_LO_SMN_ADDR, &Value32, 0);

  //Program  IOHC::MPM_BASE_ADDR_HI
  Value32 = (UINT32) RShiftU64 (MmioBase, 32);
  ASSERT (Value32 == 0);
  if (Value32 != 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("MMIO not alocated below 4G\n");
    return AGESA_ERROR;
  }
  SmnRegisterWrite (0, IOHC_MPM_BASE_ADDR_HI_SMN_ADDR, &Value32, 0);
  return AGESA_SUCCESS;
}
/*++

Routine Description:

  MPM Driver Entry. Initialize MPM device, allocate MMIO for MPM BAR

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
EFIAPI
MpmPeiDriverEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS            EfiStatus;
  AGESA_STATUS          AStatus;

  EfiStatus = EFI_SUCCESS;
  AGESA_TESTPOINT (TpMpmPeiEntry, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("MpmPei Enter\n");

  //Print Related PCD setting
  IDS_HDT_CONSOLE_PSP_TRACE ("PcdMpmEnable=%x PcdMpmWiredKvmEnable=%x PcdMpmWirelessKvmEnable=%x PcdMpmKvmMouseProtocol=%x\n",
    PcdGetBool (PcdMpmEnable), PcdGetBool (PcdMpmWiredKvmEnable), PcdGetBool (PcdMpmWirelessKvmEnable), PcdGet8 (PcdMpmKvmMouseProtocol));

  // Check PCD master token is enabled
  if (PcdGetBool (PcdMpmEnable) == FALSE) {
    // Set child PCDs to disable
    PcdSetBoolS (PcdMpmWirelessManageability, FALSE);
  }

  if (PcdGetBool (PcdMpmWirelessManageability) == FALSE) {
    // Set child PCDs to disable
    PcdSetBoolS (PcdMpmWirelessKvmEnable, FALSE);
    PcdSetBoolS (PcdMpmWirelessTcrEnable, FALSE);
  }

  if (PcdGetBool (PcdMpmEnable) == FALSE) {
    goto Exit;
  }
  //Initial Mpm PrivateData, the data is stored in AGESA HEAP
  //Multiple MPM mailbox will be called inside of the routine
  EfiStatus = InitMpmPrivateData (NULL);
  if (EFI_ERROR (EfiStatus)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("InitMpmPrivateData Fail\n");
    goto Exit;
  }
  //Install PPI to let other driver aware MpmPrivateData is ready
  //API in AgesaModulePkg\Include\Library\MpmLib.h can now be used
  //Driver which depend on this Lib, can use this PPI as depex, or notify this PPI
  EfiStatus = (**PeiServices).InstallPpi (PeiServices, &mMpmDataInitializededPpiList);
  if (EFI_ERROR (EfiStatus)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("InstallPpi Fail\n");
    goto Exit;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("MpmMmioInit\n");
  AStatus = MpmMmioInit ();
  EfiStatus = (AStatus == AGESA_SUCCESS) ? EFI_SUCCESS : EFI_OUT_OF_RESOURCES;
  if (EFI_ERROR (EfiStatus)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("MpmMmioInit Fail\n");
    goto Exit;
  }
Exit:
  AGESA_TESTPOINT (TpMpmPeiExit, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("MpmPei Exit: %x\n", EfiStatus);
  return EfiStatus;
}
