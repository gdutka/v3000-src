/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe ALIB
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 *
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */


#include  <GnbDxio.h>
#include  <GnbIommu.h>
#include  <Library/IdsLib.h>
#include  <Library/AmdBaseLib.h>
#include  <Library/GnbCommonLib.h>

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  <AGESA.h>
#include  <Gnb.h>
#include  <PiDxe.h>
#include  <Library/IdsLib.h>
#include  <Library/GnbPcieConfigLib.h>
#include  <Library/PcdLib.h>
#include  <Library/BaseLib.h>
#include  <GnbRegistersCommonV2.h>
#include  <Include/GnbIommu.h>
#include  <Library/NbioRegisterAccLib.h>
#include  <Library/GnbLib.h>
#include  <Library/IvrsDeviceInfoLib.h>

#include  "IvrsLibV2.h"

#define FILECODE NBIO_LIBRARY_IVRSLIBV2_NBIOIVRSLIB_FILECODE
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
 * Create IVHDR entry for device range
 *
 *
 * @param[in]  StartRange      Address of start range
 * @param[in]  EndRange        Address of end range
 * @param[in]  DataSetting     Data setting
 * @param[in]  Ivhd            Pointer to IVHD entry
 *
 */
VOID
NbioIvhdAddDeviceRangeEntry (
  IN       PCI_ADDR             StartRange,
  IN       PCI_ADDR             EndRange,
  IN       UINT8                DataSetting,
  IN       IVRS_IVHD_ENTRY      *Ivhd
  )
{
  IVHD_GENERIC_ENTRY  *Entry;
  Entry = (IVHD_GENERIC_ENTRY *) ((UINT8 *) Ivhd + Ivhd->Length);
  Entry->Type = IvhdEntryStartRange;
  Entry->DeviceId = DEVICE_ID (StartRange);
  Entry->DataSetting = DataSetting;
  Ivhd->Length += sizeof (IVHD_GENERIC_ENTRY);
  Entry = (IVHD_GENERIC_ENTRY *) ((UINT8 *) Ivhd + Ivhd->Length);
  Entry->Type = IvhdEntryEndRange;
  Entry->DeviceId = DEVICE_ID (EndRange);
  Ivhd->Length += sizeof (IVHD_GENERIC_ENTRY);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Create IVHDR entry for aliased range
 *
 *
 * @param[in]  StartRange      Address of start range
 * @param[in]  EndRange        Address of end range
 * @param[in]  Alias           Address of alias requestor ID for range
 * @param[in]  DataSetting     Data setting
 * @param[in]  Ivhd            Pointer to IVHD entry
 *
 */
VOID
NbioIvhdAddDeviceAliasRangeEntry (
  IN       PCI_ADDR             StartRange,
  IN       PCI_ADDR             EndRange,
  IN       PCI_ADDR             Alias,
  IN       UINT8                DataSetting,
  IN       IVRS_IVHD_ENTRY      *Ivhd
  )
{
  IVHD_ALIAS_ENTRY    *RangeEntry;
  IVHD_GENERIC_ENTRY  *Entry;

  RangeEntry = (IVHD_ALIAS_ENTRY *) ((UINT8 *) Ivhd + Ivhd->Length);
  RangeEntry->Type = IvhdEntryAliasStartRange;
  RangeEntry->DeviceId = DEVICE_ID (StartRange);
  RangeEntry->AliasDeviceId = DEVICE_ID (Alias);
  RangeEntry->DataSetting = DataSetting;
  Ivhd->Length += sizeof (IVHD_ALIAS_ENTRY);
  Entry = (IVHD_GENERIC_ENTRY *) ((UINT8 *) Ivhd + Ivhd->Length);
  Entry->Type = IvhdEntryEndRange;
  Entry->DeviceId = DEVICE_ID (EndRange);
  Ivhd->Length += sizeof (IVHD_GENERIC_ENTRY);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Create IVHDR entry for special device
 *
 *
 * @param[in]  SpecialDevice   Special device Type
 * @param[in]  Device          Address of requestor ID for special device
 * @param[in]  Id              Apic ID/ Hpet ID
 * @param[in]  DataSetting     Data setting
 * @param[in]  Ivhd            Pointer to IVHD entry
 *
 */
VOID
NbioIvhdAddSpecialDeviceEntry (
  IN       IVHD_SPECIAL_DEVICE  SpecialDevice,
  IN       PCI_ADDR             Device,
  IN       UINT8                Id,
  IN       UINT8                DataSetting,
  IN       IVRS_IVHD_ENTRY      *Ivhd
  )
{
  IVHD_SPECIAL_ENTRY  *SpecialEntry;

  SpecialEntry = (IVHD_SPECIAL_ENTRY *) ((UINT8 *) Ivhd + Ivhd->Length);
  SpecialEntry->Type = IvhdEntrySpecialDevice;
  SpecialEntry->AliasDeviceId = DEVICE_ID (Device);
  SpecialEntry->Variety = (UINT8) SpecialDevice;
  SpecialEntry->Handle = Id;
  SpecialEntry->DataSetting = DataSetting;
  Ivhd->Length += sizeof (IVHD_SPECIAL_ENTRY);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Create gnb ioapic IVHD entry
 *
 *
 * @param[in]  GnbHandle       Gnb handle
 * @param[in]  Ivhd            IVHD header pointer
 *
 */
VOID
NbioIvhdAddApicEntry (
  IN       GNB_HANDLE                 *GnbHandle,
     OUT   IVRS_IVHD_ENTRY            *Ivhd
  )
{
  UINT32              AddressLow;
  UINT32              AddressHigh;
  UINT64              IoapicAddress;
  PCI_ADDR            GnbPciAddress;
  PCI_ADDR            GnbIoapicPciId;

  // Get the PCI address of the GNB
  GnbPciAddress.AddressValue = GnbHandle->Address.AddressValue;

  NbioRegisterRead (GnbHandle, TYPE_SMN, 0x13b102f4, &AddressHigh, 0);
  NbioRegisterRead (GnbHandle, TYPE_SMN, 0x13b102f0 , &AddressLow, 0);

  IoapicAddress = ((UINT64) AddressHigh) << 32;
  IoapicAddress |= ((UINT64) AddressLow) & 0xffffff00;

  if ((IoapicAddress != 0) && ((AddressLow & 0x01) == 1) && (PcdGet8 (PcdCfgGnbIoapicId) != 0xff)) {
    GnbIoapicPciId.AddressValue = GnbPciAddress.AddressValue;
    GnbIoapicPciId.Address.Function = 1;
    NbioIvhdAddSpecialDeviceEntry (
      IvhdSpecialDeviceIoapic,
      GnbIoapicPciId,
      PcdGet8 (PcdCfgGnbIoapicId),
      0,
      Ivhd
      );
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Create IVHDR entry for F0 device
 *
 *
 * @param[in]  Ivhd            Pointer to IVHD entry
 *
 */
VOID
NbioIvhdAddF0DeviceEntries (
  IN       IVRS_IVHD_ENTRY      *Ivhd
  )
{
  IVRS_DEVICE_LIST      *DeviceInfo;
  IVHD_TYPEF0_ENTRY     *TypeF0Entry;
  UINT8                 *UidPointer;
  UINT8                 Index;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  AgesaIvrsDeviceInfo (&DeviceInfo, PcdGet8 (PcdEmmcDriverType));
  IDS_HDT_CONSOLE (GNB_TRACE, "Returned from IVRS Device Info\n");

  if (DeviceInfo == NULL) {
    return;
  }

  GnbLibDebugDumpBuffer (DeviceInfo, 2 * sizeof (IVRS_DEVICE_LIST), 1, sizeof (IVRS_DEVICE_LIST));

  while (DeviceInfo->DeviceId != 0xFFFF) {
    TypeF0Entry = (IVHD_TYPEF0_ENTRY *) ((UINT8 *) Ivhd + Ivhd->Length);

    IDS_HDT_CONSOLE (GNB_TRACE, "Processing Entry.. \n - DeviceId = 0x%x\n - UidFormat = 0x%x\n - UidLength = 0x%x\n", DeviceInfo->DeviceId, DeviceInfo->UidFormat, DeviceInfo->UidLength);

    TypeF0Entry->Type = 0xF0;

    TypeF0Entry->DeviceId = DeviceInfo->DeviceId;
    TypeF0Entry->DataSetting = DeviceInfo->DataSetting;
    TypeF0Entry->HardwareId.IdByte0 = DeviceInfo->HardwareId[0];
    TypeF0Entry->HardwareId.IdByte1 = DeviceInfo->HardwareId[1];
    TypeF0Entry->HardwareId.IdByte2 = DeviceInfo->HardwareId[2];
    TypeF0Entry->HardwareId.IdByte3 = DeviceInfo->HardwareId[3];
    TypeF0Entry->HardwareId.IdByte4 = DeviceInfo->HardwareId[4];
    TypeF0Entry->HardwareId.IdByte5 = DeviceInfo->HardwareId[5];
    TypeF0Entry->HardwareId.IdByte6 = DeviceInfo->HardwareId[6];
    TypeF0Entry->HardwareId.IdByte7 = DeviceInfo->HardwareId[7];

    TypeF0Entry->CompatibleId.IdByte0 = DeviceInfo->CompatibleId[0];
    TypeF0Entry->CompatibleId.IdByte1 = DeviceInfo->CompatibleId[1];
    TypeF0Entry->CompatibleId.IdByte2 = DeviceInfo->CompatibleId[2];
    TypeF0Entry->CompatibleId.IdByte3 = DeviceInfo->CompatibleId[3];
    TypeF0Entry->CompatibleId.IdByte4 = DeviceInfo->CompatibleId[4];
    TypeF0Entry->CompatibleId.IdByte5 = DeviceInfo->CompatibleId[5];
    TypeF0Entry->CompatibleId.IdByte6 = DeviceInfo->CompatibleId[6];
    TypeF0Entry->CompatibleId.IdByte7 = DeviceInfo->CompatibleId[7];

    TypeF0Entry->UidFormat = DeviceInfo->UidFormat;
    TypeF0Entry->UidLength = DeviceInfo->UidLength;

    UidPointer = (UINT8 *) TypeF0Entry;
    UidPointer += sizeof (IVHD_TYPEF0_ENTRY);
    for (Index = 0; Index < DeviceInfo->UidLength; Index++) {
      *UidPointer = DeviceInfo->Uid[Index];
      UidPointer++;
    }
    GnbLibDebugDumpBuffer (TypeF0Entry, sizeof (IVHD_TYPEF0_ENTRY) + DeviceInfo->UidLength, 1, 32);
    IDS_HDT_CONSOLE (GNB_TRACE, "\n");
    Ivhd->Length += (sizeof (IVHD_TYPEF0_ENTRY) + DeviceInfo->UidLength);
    DeviceInfo++;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "Ivhd Length = 0x%x\n", Ivhd->Length);
  return;
}


