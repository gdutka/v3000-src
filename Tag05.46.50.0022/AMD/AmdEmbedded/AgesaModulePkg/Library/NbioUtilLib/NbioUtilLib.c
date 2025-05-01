/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * NBIO Common Library. This library is a home for common functions and routines
 * meant to reduce code size and encourage code sharing.
 *
 *
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  <Library/NbioUtilLib.h>
#include  <Library/SmnAccessLib.h>
#include  <Library/FabricResourceManagerLib.h>
#include  <Library/BaseLib.h>
#include  <Library/BaseMemoryLib.h>

#include  "Filecode.h"
#define FILECODE LIBRARY_NBIOUTILLIB_NBIOUTILLIB_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------*/
/**
 * This function assigns MMIO for a non-pci device.
 *
 *
 * @param[in]  GnbHandle       Silicon handle to assign
 * @param[in]  MmioBarLow      Address of low byte
 * @param[in]  MmioBarHigh     Address of high byte
 * @param[in]  MemorySize      Size of the allocated bar required
 * @param[in]  Enable          Set enable bit in BAR
 * @param[in]  LockSettings    If the allocated memory range should be locked or not
 * @param[in]  Above4G         If MMIO above the 4G boundary should be allocated
 */
VOID
NonPCIBarInit (
  IN       GNB_HANDLE   *GnbHandle,
  IN       UINT32       MmioBarLow,
  IN       UINT32       MmioBarHigh,
  IN       UINT32       MemorySize,
  IN       BOOLEAN      Enable,
  IN       BOOLEAN      LockSettings,
  IN       BOOLEAN      Above4G
  )
{
  EFI_STATUS             Status;
  FABRIC_TARGET          MmioTarget;
  FABRIC_MMIO_ATTRIBUTE  Attributes;
  UINT64                 MmioBase, Length;
  UINT32                 BarLow, BarHigh;

  BarLow = 0;
  BarHigh = 0;
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Begin to allocate bars for SMN low %x high %x, size %x\n", __FUNCTION__, MmioBarLow, MmioBarHigh, MemorySize);

  ///
  /// See if the given BAR have already been assigned
  ///
  SmnRegisterReadS (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarLow, &BarLow);
  SmnRegisterReadS (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarHigh, &BarHigh);

  if (BarLow == 0 && BarHigh == 0) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Bars have not been assigned, attempting to allocate MMIO \n", __FUNCTION__);
    ///
    /// Assign bars:
    /// Allocate a chunk of MMIO first
    ///
    ZeroMem (&Attributes, sizeof(Attributes));
    ZeroMem (&MmioTarget, sizeof(MmioTarget));
    Length = MemorySize;
    MmioTarget.TgtType = TARGET_PCI_BUS;
    MmioTarget.SocketNum = GnbHandle->SocketId;
    MmioTarget.PciBusNum = (UINT16) GnbHandle->Address.Address.Bus;
    MmioTarget.PciSegNum = (UINT16) GnbHandle->Address.Address.Segment;
    MmioTarget.RbNum = GnbHandle->RBIndex;
    Attributes.ReadEnable = 1;
    Attributes.WriteEnable = 1;
    Attributes.NonPosted = 0;
    MmioBase = 0;
    if (Above4G) {
      Attributes.MmioType = NON_PCI_DEVICE_ABOVE_4G;
    }
    else {
      Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;
    }

    IDS_HDT_CONSOLE (MAIN_FLOW, "%a : FabricAllocateMmio : Socket %d , RB # %d\n", __FUNCTION__, MmioTarget.SocketNum, MmioTarget.RbNum);
    Status = FabricAllocateMmio (&MmioBase, &Length, ALIGN_1M, MmioTarget, &Attributes);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Allocate MMIO Fail\n", __FUNCTION__);
      return;
    } else {
      IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Allocate MMIO @0x%lx\n", __FUNCTION__, MmioBase);
    }
  }
  else {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Bars have already been assigned!\n", __FUNCTION__);
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a : End\n", __FUNCTION__);
    return;
  }

  ///
  /// Write the assigned memory address registers to SMN
  ///
  BarLow  = (UINT32) MmioBase;
  BarHigh = (UINT32) RShiftU64 (MmioBase, 32);

  SmnRegisterWriteS (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarLow,  &BarLow, GNB_REG_ACC_FLAG_S3SAVE);
  SmnRegisterWriteS (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarHigh, &BarHigh, GNB_REG_ACC_FLAG_S3SAVE);

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a : BarLow = %x , BarHigh %x\n", __FUNCTION__, BarLow, BarHigh);

  // Set enable bit separate from other bits
  if (Enable) {
    BarLow = BarLow | BIT0; /// Set enable bit
    SmnRegisterWriteS (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarLow, &BarLow, GNB_REG_ACC_FLAG_S3SAVE);
    SmnRegisterWriteS (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarHigh, &BarHigh, GNB_REG_ACC_FLAG_S3SAVE);
  }

  // Set lock bit separate from other bits
  if (LockSettings) {
    BarLow = BarLow | BIT1; /// Set lock bit
    SmnRegisterWriteS (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarLow, &BarLow, GNB_REG_ACC_FLAG_S3SAVE);
    SmnRegisterWriteS (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarHigh, &BarHigh, GNB_REG_ACC_FLAG_S3SAVE);
  }


  IDS_HDT_CONSOLE (MAIN_FLOW, "%a : End\n", __FUNCTION__);
  return;
}



