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
 * IOMMU IVRS Table Creation
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
#include  <AGESA.h>
#include  <Gnb.h>
#include  <PiDxe.h>
#include  <Filecode.h>
#include  <Library/IdsLib.h>
#include  <Library/AmdBaseLib.h>
#include  <Library/AmdHeapLib.h>
#include  <Library/GnbPcieConfigLib.h>
#include  <Library/GnbCommonLib.h>
#include  <Library/PcdLib.h>
#include  <Library/BaseLib.h>
#include  <OptionGnb.h>
#include  <GnbRegistersCommonV2.h>
#include  <Library/GnbHeapLib.h>
#include  <Protocol/AmdNbioPcieServicesProtocol.h>
#include  <Guid/GnbPcieInfoHob.h>
#include  <Include/GnbIommu.h>
#include  <Library/UefiBootServicesTableLib.h>
#include  <Protocol/AcpiTable.h>
#include  <Library/MemoryAllocationLib.h>
#include  <Library/BaseMemoryLib.h>
#include  <Library/NbioRegisterAccLib.h>
#include  <Library/GnbLib.h>
#include <Library/AmdPspMboxLibV2.h>

#include "IvrsLibV2.h"

#define FILECODE NBIO_LIBRARY_IVRSLIBV2_NBIOIVRSTABLE_FILECODE
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

IOMMU_IVRS_HEADER IvrsHeader = {
  {'I', 'V', 'R', 'S'},
  sizeof (IOMMU_IVRS_HEADER),
  2,
  0,
  {'A', 'M', 'D', ' ', ' ', 0},
  {'A', 'M', 'D', 'I', 'O', 'M', 'M', 'U'},
  1,
  {'A','M','D',' '},
  1,
  0,
  0
};

/*---------------------------------------------------------------------------------------*/
/**
 * Calculate an ACPI style checksum
 *
 * Computes the checksum and stores the value to the checksum
 * field of the passed in ACPI table's header.
 *
 * @param[in]  Table             ACPI table to checksum
 *
 */
STATIC
VOID
ChecksumAcpiTable (
  IN OUT   ACPI_TABLE_HEADER *Table
  )
{
  UINT8  *BuffTempPtr;
  UINT8  Checksum;
  UINT32 BufferOffset;

  Table->Checksum = 0;
  Checksum = 0;
  BuffTempPtr = (UINT8 *) Table;
  for (BufferOffset = 0; BufferOffset < Table->TableLength; BufferOffset++) {
    Checksum = Checksum - *(BuffTempPtr + BufferOffset);
  }

  Table->Checksum = Checksum;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get IOMMU PCI address
 *
 *
 * @param[in]  GnbHandle       GNB handle
 */
STATIC
PCI_ADDR
GnbGetIommuPciAddress (
  IN       GNB_HANDLE               *GnbHandle
  )
{
  PCI_ADDR  GnbIommuPciAddress;

  ASSERT (GnbHandle != NULL);
  GnbIommuPciAddress = GnbHandle->Address;
  GnbIommuPciAddress.Address.Function = 0x2;
  return  GnbIommuPciAddress;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Check if IOMMU unit present and enabled
 *
 *
 *
 *
 * @param[in]  GnbHandle       Gnb handle
 * @retval     BOOLEAN
 *
 */
BOOLEAN
GnbCheckIommuPresent (
  IN       GNB_HANDLE                 *GnbHandle
  )
{
  PCI_ADDR  GnbIommuPciAddress;
  if (GnbHandle->Address.Address.Bus != 0xFF) {
    GnbIommuPciAddress = GnbGetIommuPciAddress(GnbHandle);
    if (GnbLibPciIsDevicePresent (GnbIommuPciAddress.AddressValue, NULL)) {
      return TRUE;
    }
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Create IVRS entry
 *
 *
 * @param[in]  GnbHandle       Gnb handle
 * @param[in]  Type            Block type
 * @param[in]  Ivhd            IVHD header pointer
 *
 */
VOID
GnbCreateIvhdHeader10h (
  IN       GNB_HANDLE                 *GnbHandle,
  IN       IVRS_BLOCK_TYPE            Type,
     OUT   IVRS_IVHD_ENTRY_10H        *Ivhd
  )
{
  UINT32      Value;
  PCI_ADDR    GnbIommuPciAddress;
  MMIO_0x30        MMIO_x30_Value;
  MMIO_0x18        MMIO_x18_Value;
  MMIO_0x4000      MMIO_x4000_Value;
  CAPABILITY_REG   CapValue;
  UINT32           MsiNumPPR;

  GnbIommuPciAddress = GnbGetIommuPciAddress (GnbHandle);
  Ivhd->Ivhd.Type = (UINT8) Type;
  Ivhd->Ivhd.Length = sizeof (IVRS_IVHD_ENTRY_10H);
  Ivhd->Ivhd.DeviceId = (UINT16) (((NbioGetHostPciAddress (GnbHandle).AddressValue) >> 12) | 2);
  Ivhd->Ivhd.CapabilityOffset = GnbLibFindPciCapability (GnbIommuPciAddress.AddressValue, IOMMU_CAP_ID, NULL);
  Ivhd->Ivhd.PciSegment = (UINT16) NbioGetHostPciAddress (GnbHandle).Address.Segment;
  GnbLibPciRead (GnbIommuPciAddress.AddressValue | (Ivhd->Ivhd.CapabilityOffset + 0x4), AccessWidth32, &Ivhd->Ivhd.BaseAddress, NULL);
  GnbLibPciRead (GnbIommuPciAddress.AddressValue | (Ivhd->Ivhd.CapabilityOffset + 0x8), AccessWidth32, (UINT8 *) &Ivhd->Ivhd.BaseAddress + 4, NULL);
  Ivhd->Ivhd.BaseAddress = Ivhd->Ivhd.BaseAddress & 0xfffffffffffffffe;
  ASSERT (Ivhd->Ivhd.BaseAddress != 0x0);

  GnbLibMemRead (Ivhd->Ivhd.BaseAddress + 0x30, AccessWidth64, &(MMIO_x30_Value.Value), NULL);
  GnbLibMemRead (Ivhd->Ivhd.BaseAddress + 0x18, AccessWidth64, &(MMIO_x18_Value.Value), NULL);
  GnbLibPciRead (GnbIommuPciAddress.AddressValue | Ivhd->Ivhd.CapabilityOffset, AccessWidth32, &(CapValue.Value), NULL);
  Ivhd->Ivhd.Flags |= ((MMIO_x18_Value.Field.Coherent != 0) ? IVHD_FLAG_COHERENT : 0);
  Ivhd->Ivhd.Flags |= ((CapValue.Field.IommuIoTlbsup != 0) ? IVHD_FLAG_IOTLBSUP : 0);
  Ivhd->Ivhd.Flags |= ((MMIO_x18_Value.Field.Isoc != 0) ? IVHD_FLAG_ISOC : 0);
  Ivhd->Ivhd.Flags |= ((MMIO_x18_Value.Field.ResPassPW != 0) ? IVHD_FLAG_RESPASSPW : 0);
  Ivhd->Ivhd.Flags |= ((MMIO_x18_Value.Field.PassPW != 0) ? IVHD_FLAG_PASSPW : 0);
  Ivhd->Ivhd.Flags |= ((MMIO_x30_Value.Field.PPRSup != 0) ? IVHD_FLAG_PPRSUB : 0);
  Ivhd->Ivhd.Flags |= ((MMIO_x30_Value.Field.PreFSup != 0) ? IVHD_FLAG_PREFSUP : 0);
  Ivhd->Ivhd.Flags |= ((MMIO_x18_Value.Field.HtTunEn != 0) ? IVHD_FLAG_HTTUNEN : 0);

  GnbLibPciRead (GnbIommuPciAddress.AddressValue | (Ivhd->Ivhd.CapabilityOffset + 0x10), AccessWidth32, &Value, NULL);
  Ivhd->Ivhd.IommuInfo = (UINT16) (Value & 0x1f);
  MsiNumPPR = Value >> 27;
  GnbLibPciRead (GnbIommuPciAddress.AddressValue | (Ivhd->Ivhd.CapabilityOffset + 0xC), AccessWidth32, &Value, NULL);
  Ivhd->Ivhd.IommuInfo |= ((Value & 0x1f) << IVHD_INFO_UNITID_OFFSET);

  GnbLibMemRead (Ivhd->Ivhd.BaseAddress + 0x4000, AccessWidth64, &(MMIO_x4000_Value.Value), NULL);
  Ivhd->IommuEfr = (UINT32) ((MMIO_x30_Value.Field.XTSup << IVHD_EFR_XTSUP_OFFSET) |
                   (MMIO_x30_Value.Field.NXSup << IVHD_EFR_NXSUP_OFFSET) |
                   (MMIO_x30_Value.Field.GTSup << IVHD_EFR_GTSUP_OFFSET) |
                   (MMIO_x30_Value.Field.GLXSup << IVHD_EFR_GLXSUP_OFFSET) |
                   (MMIO_x30_Value.Field.IASup << IVHD_EFR_IASUP_OFFSET) |
                   (MMIO_x30_Value.Field.GASup << IVHD_EFR_GASUP_OFFSET) |
                   (MMIO_x30_Value.Field.HESup << IVHD_EFR_HESUP_OFFSET) |
                   (MMIO_x30_Value.Field.PASmax << IVHD_EFR_PASMAX_OFFSET) |
                   (MMIO_x4000_Value.Field.NCounter << IVHD_EFR_PNCOUNTERS_OFFSET) |
                   (MMIO_x4000_Value.Field.NCounterBanks << IVHD_EFR_PNBANKS_OFFSET) |
                   (MsiNumPPR << IVHD_EFR_MSINUMPPR_OFFSET) |
                   (MMIO_x30_Value.Field.GATS << IVHD_EFR_GATS_OFFSET) |
                   (MMIO_x30_Value.Field.HATS << IVHD_EFR_HATS_OFFSET));
}

/*----------------------------------------------------------------------------------------*/
/**
 * Create IVRS entry
 *
 *
 * @param[in]  GnbHandle       Gnb handle
 * @param[in]  Type            Block type
 * @param[in]  Ivhd            IVHD header pointer
 *
 */
VOID
GnbCreateIvhdHeader11h (
  IN       GNB_HANDLE                 *GnbHandle,
  IN       IVRS_BLOCK_TYPE            Type,
     OUT   IVRS_IVHD_ENTRY_11H        *Ivhd
  )
{
  UINT32      Value;
  PCI_ADDR    GnbIommuPciAddress;
  UINT64      BaseAddress;
  MMIO_0x30   MMIO_x30_Value;
  MMIO_0x18   MMIO_x18_Value;
  CAPABILITY_REG   CapValue;

  GnbIommuPciAddress = GnbGetIommuPciAddress (GnbHandle);
  Ivhd->Ivhd.Type = (UINT8) Type;
  Ivhd->Ivhd.Length = sizeof (IVRS_IVHD_ENTRY_11H);
  Ivhd->Ivhd.DeviceId = (UINT16) (((NbioGetHostPciAddress (GnbHandle).AddressValue) >> 12) | 2);
  Ivhd->Ivhd.CapabilityOffset = GnbLibFindPciCapability (GnbIommuPciAddress.AddressValue, IOMMU_CAP_ID, NULL);
  Ivhd->Ivhd.PciSegment = (UINT16) NbioGetHostPciAddress (GnbHandle).Address.Segment;
  GnbLibPciRead (GnbIommuPciAddress.AddressValue | (Ivhd->Ivhd.CapabilityOffset + 0x4), AccessWidth32, &Ivhd->Ivhd.BaseAddress, NULL);
  GnbLibPciRead (GnbIommuPciAddress.AddressValue | (Ivhd->Ivhd.CapabilityOffset + 0x8), AccessWidth32, (UINT8 *) &Ivhd->Ivhd.BaseAddress + 4, NULL);
  Ivhd->Ivhd.BaseAddress = Ivhd->Ivhd.BaseAddress & 0xfffffffffffffffe;
  ASSERT (Ivhd->Ivhd.BaseAddress != 0x0);

  GnbLibMemRead (Ivhd->Ivhd.BaseAddress + 0x30, AccessWidth64, &(MMIO_x30_Value.Value), NULL);
  GnbLibMemRead (Ivhd->Ivhd.BaseAddress + 0x18, AccessWidth64, &(MMIO_x18_Value.Value), NULL);
  GnbLibPciRead (GnbIommuPciAddress.AddressValue | Ivhd->Ivhd.CapabilityOffset, AccessWidth32, &(CapValue.Value), NULL);
  Ivhd->Ivhd.Flags |= ((MMIO_x18_Value.Field.Coherent != 0) ? IVHD_FLAG_COHERENT : 0);
  Ivhd->Ivhd.Flags |= ((CapValue.Field.IommuIoTlbsup != 0) ? IVHD_FLAG_IOTLBSUP : 0);
  Ivhd->Ivhd.Flags |= ((MMIO_x18_Value.Field.Isoc != 0) ? IVHD_FLAG_ISOC : 0);
  Ivhd->Ivhd.Flags |= ((MMIO_x18_Value.Field.ResPassPW != 0) ? IVHD_FLAG_RESPASSPW : 0);
  Ivhd->Ivhd.Flags |= ((MMIO_x18_Value.Field.PassPW != 0) ? IVHD_FLAG_PASSPW : 0);
  Ivhd->Ivhd.Flags |= ((MMIO_x18_Value.Field.HtTunEn != 0) ? IVHD_FLAG_HTTUNEN : 0);

  GnbLibPciRead (GnbIommuPciAddress.AddressValue | (Ivhd->Ivhd.CapabilityOffset + 0x10), AccessWidth32, &Value, NULL);
  Ivhd->Ivhd.IommuInfo = (UINT16) (Value & 0x1f);
  GnbLibPciRead (GnbIommuPciAddress.AddressValue | (Ivhd->Ivhd.CapabilityOffset + 0xC), AccessWidth32, &Value, NULL);
  Ivhd->Ivhd.IommuInfo |= ((Value & 0x1f) << IVHD_INFO_UNITID_OFFSET);

  // Assign attributes
  GnbLibPciRead (GnbIommuPciAddress.AddressValue | (Ivhd->Ivhd.CapabilityOffset + 0x10), AccessWidth32, &Value, NULL);
  Ivhd->IommuAttributes = ((Value & 0xf8000000) >> 27) << 23;
  IDS_HDT_CONSOLE (GNB_TRACE, "Attribute cap offset 0x10 = %x\n", Value);
  BaseAddress = Ivhd->Ivhd.BaseAddress;
  GnbLibMemRead (BaseAddress + 0x4000, AccessWidth32, &Value, NULL);

  Ivhd->IommuAttributes |= (((Value & 0x3f000) >> 12) << 17) | (((Value & 0x780) >> 7) << 13);
  IDS_HDT_CONSOLE (GNB_TRACE, "Attribute MMIO 0x4000 = %x\n", Value);

  // Assign 64bits EFR for type 11, 41h
  GnbLibMemRead (BaseAddress + 0x30, AccessWidth32, &Ivhd->IommuEfr, NULL);
  GnbLibMemRead (BaseAddress + 0x34, AccessWidth32, (UINT8 *) &Ivhd->IommuEfr + 4, NULL);
  Ivhd->IommuEfr |= BIT46;

  IDS_HDT_CONSOLE (GNB_TRACE, "IommuEfr = %x\n", Ivhd->IommuEfr);

}

/*----------------------------------------------------------------------------------------*/
/**
 * Get bus range decoded by GNB
 *
 * Final bus allocation can not be assumed until AmdInitMid
 *
 * @param[in]   GnbHandle       GNB handle
 * @param[out]  SegmentNumber   The segment number
 * @param[out]  StartBusNumber  Beginning of the Bus Range
 * @param[out]  EndBusNumber    End of the Bus Range
 * @retval                      Status
 */
STATIC
AGESA_STATUS
GnbGetBusDecodeRange (
  IN       GNB_HANDLE                 *GnbHandle,
     OUT   UINT16                     *SegmentNumber,
     OUT   UINT8                      *StartBusNumber,
     OUT   UINT8                      *EndBusNumber
  )
{
  PCI_ADDR            GnbPciAddress;
  GNB_HANDLE          *NextHandle;

  GnbPciAddress = NbioGetHostPciAddress (GnbHandle);
  *SegmentNumber = (UINT16)GnbPciAddress.Address.Segment;
  *StartBusNumber = (UINT8)GnbPciAddress.Address.Bus;
  if (GnbHandle->BusNumberLimit !=0) {
    *EndBusNumber = GnbHandle->BusNumberLimit;
  } else {
    NextHandle = GnbGetNextHandle(GnbHandle);

    if (NextHandle == NULL) {
      *EndBusNumber = 0xFF;
    } else {
      GnbPciAddress = NbioGetHostPciAddress (NextHandle);
      *EndBusNumber = (UINT8)(GnbPciAddress.Address.Bus - 1);
    }
  }

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Create IVHD entry
 *
 *
 * @param[in]  GnbHandle       Gnb handle
 * @param[in]  Ivhd            IVHD header pointer
 *
 */
VOID
GnbCreateIvhd (
  IN       GNB_HANDLE                 *GnbHandle,
     OUT   IVRS_IVHD_ENTRY            *Ivhd
  )
{
  AGESA_STATUS  Status;
  PCI_ADDR      Start;
  PCI_ADDR      End;
  UINT16        SegmentNumber;
  UINT8         StartBusNumber;
  UINT8         EndBusNumber;

  Status = GnbGetBusDecodeRange (GnbHandle, &SegmentNumber, &StartBusNumber, &EndBusNumber);
  ASSERT (Status == AGESA_SUCCESS);
  Start.AddressValue = MAKE_SBDFO (SegmentNumber, StartBusNumber, 1, 0, 0);
  End.AddressValue = MAKE_SBDFO (SegmentNumber, EndBusNumber, 0x1F, 6, 0);
  IDS_HDT_CONSOLE (GNB_TRACE, "StartBusNumber = %x     EndBusNumber = %x  \n", StartBusNumber, EndBusNumber);
  NbioIvhdAddDeviceRangeEntry (Start, End, 0, Ivhd);
  if (GnbHandle->Address.AddressValue == 0) {
    SbCreateIvhdEntries (Ivhd);
  }
  NbioIvhdAddApicEntry (GnbHandle, Ivhd);
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Create IVRS entry
 *
 *
 * @param[in]  GnbHandle       Gnb handle
 * @param[in]  Type            Entry type
 * @param[in]  Ivrs            IVRS table pointer
 * @retval     AGESA_STATUS
 *
 */

AGESA_STATUS
GnbCreateIvrsEntry (
  IN       GNB_HANDLE                 *GnbHandle,
  IN       IVRS_BLOCK_TYPE            Type,
  IN       VOID                       *Ivrs
  )
{
  IVRS_IVHD_ENTRY   *Ivhd;
  UINT8             IommuCapabilityOffset;
  UINT32            Value;
  PCI_ADDR          GnbIommuPciAddress;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbCreateIvrsEntry Entry\n");
  if (Type == IvrsIvhdBlock10h || Type == IvrsIvhdBlock11h || Type == IvrsIvhdrBlock40h) {
    GnbIommuPciAddress = GnbGetIommuPciAddress (GnbHandle);
    // Update IVINFO
    IommuCapabilityOffset = GnbLibFindPciCapability (GnbIommuPciAddress.AddressValue, IOMMU_CAP_ID, NULL);
    GnbLibPciRead (GnbIommuPciAddress.AddressValue | (IommuCapabilityOffset + 0x10), AccessWidth32, &Value, NULL);
    ((IOMMU_IVRS_HEADER *) Ivrs)->IvInfo = Value & (IVINFO_HTATSRESV_MASK | IVINFO_VASIZE_MASK | IVINFO_GASIZE_MASK | IVINFO_PASIZE_MASK);
    // EFRSup: IVINFO[0] = bit 27 of Cap+0x10
    GnbLibPciRead (GnbIommuPciAddress.AddressValue | (IommuCapabilityOffset + 0x00), AccessWidth32, &Value, NULL);
    if ((Value & BIT27) != 0) {
      ((IOMMU_IVRS_HEADER *) Ivrs)->IvInfo |= IVINFO_EFRSUP_MASK;
    }
    if (PcdGetBool (PcdDmaProtection)) {
      ((IOMMU_IVRS_HEADER *) Ivrs)->IvInfo |= IVINFO_DMAREMAP_MASK;
    }

    // Address of IVHD entry
    Ivhd = (IVRS_IVHD_ENTRY*) ((UINT8 *)Ivrs + ((IOMMU_IVRS_HEADER *) Ivrs)->TableLength);
    if (Type == IvrsIvhdBlock10h) {
      GnbCreateIvhdHeader10h (GnbHandle, Type, (IVRS_IVHD_ENTRY_10H*) Ivhd);
    }
    if (Type == IvrsIvhdBlock11h || Type == IvrsIvhdrBlock40h) {
      GnbCreateIvhdHeader11h (GnbHandle, Type, (IVRS_IVHD_ENTRY_11H*) Ivhd);
    }

    switch(Type){
      case IvrsIvhdBlock10h:
      case IvrsIvhdBlock11h:
        GnbCreateIvhd (GnbHandle, Ivhd);
        break;
      case IvrsIvhdrBlock40h:
        GnbCreateIvhd (GnbHandle, Ivhd);
        NbioIvhdAddF0DeviceEntries (Ivhd);
        break;
      default:
        break;
      }
    ((IOMMU_IVRS_HEADER *) Ivrs)->TableLength = ((IOMMU_IVRS_HEADER *) Ivrs)->TableLength + Ivhd->Length;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbCreateIvrsEntry Exit\n");
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Build IVRS table
 *
 *
 *
 * @retval        AGESA_SUCCESS
 * @retval        AGESA_ERROR
 */

AGESA_STATUS
GnbIommuIvrsTable (
  )
{
  EFI_STATUS                          Status;
  AGESA_STATUS                        AgesaStatus;
  VOID                                *Ivrs;
  BOOLEAN                             IvrsSupport;
  GNB_HANDLE                          *GnbHandle;
  BOOLEAN                             IvrsRelativeAddrNamesSupport;
  DXE_AMD_NBIO_PCIE_SERVICES_PROTOCOL *PcieServicesProtocol;
  PCIe_PLATFORM_CONFIG                *Pcie;
  GNB_PCIE_INFORMATION_DATA_HOB       *PciePlatformConfigHobData;
  EFI_ACPI_TABLE_PROTOCOL             *AcpiTable;
  UINTN                               TableHandle;
  UINT32                              Value;
  UINT32                              FchIoapicValue;
  UINT8                               Counter;
  UINT8                               InitialGnbIoapicId;
  UINT8                               InitialFchIoapicId;
  IVRS_ACPI_TABLE_INFO                PspMessage;
  BOOLEAN                             AlternateList;

  if (TRUE == PcdGetBool (PcdDmaProtection)) {
    IvrsRelativeAddrNamesSupport = TRUE;
  } else {
    IvrsRelativeAddrNamesSupport = PcdGetBool(PcdIvrsRelativeAddrNamesSupport);
  }

  AgesaStatus = AGESA_SUCCESS;
  IvrsSupport = FALSE;
  Counter = 0;
  Ivrs = GnbAllocateHeapBuffer (
           AMD_ACPI_IVRS_BUFFER_HANDLE,
           IVRS_TABLE_LENGTH,
           (AMD_CONFIG_PARAMS *)NULL
           );
  ASSERT (Ivrs != NULL);
  if (Ivrs == NULL) {
    return  AGESA_ERROR;
  }
  Ivrs = AllocateZeroPool (IVRS_TABLE_LENGTH);
  CopyMem (Ivrs, &IvrsHeader, sizeof (IvrsHeader));

  // Update table OEM fields.
  LibAmdMemCopy (
    (VOID *) &((ACPI_TABLE_HEADER*) Ivrs)->OemId,
    (VOID *) PcdGetPtr (PcdAmdAcpiTableHeaderOemId),
    AsciiStrnLenS ((CHAR8 *)PcdGetPtr (PcdAmdAcpiTableHeaderOemId), 6),
    (AMD_CONFIG_PARAMS *)NULL
    );

  LibAmdMemCopy (
    (VOID *) &((ACPI_TABLE_HEADER*) Ivrs)->OemTableId,
    (VOID *) PcdGetPtr (PcdAmdAcpiIvrsTableHeaderOemTableId),
    AsciiStrnLenS ((CHAR8 *)PcdGetPtr (PcdAmdAcpiIvrsTableHeaderOemTableId), 8),
    (AMD_CONFIG_PARAMS *)NULL
    );

  Status = gBS->LocateProtocol (
                  &gAmdNbioPcieServicesProtocolGuid,
                  NULL,
                  (VOID **)&PcieServicesProtocol
                  );

  PcieServicesProtocol->PcieGetTopology (PcieServicesProtocol, (UINT32 **) &PciePlatformConfigHobData);
  Pcie = &(PciePlatformConfigHobData->PciePlatformConfigHob);

  InitialGnbIoapicId = PcdGet8 (PcdCfgGnbIoapicId);
  InitialFchIoapicId = PcdGet8 (PcdCfgFchIoapicId);

  GnbHandle = NbioGetHandle (Pcie);
  AlternateList = FALSE;
  while (GnbHandle != NULL) {
    if (GnbCheckIommuPresent (GnbHandle)) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "Build IVRS for Socket %d Silicon %d\n", GnbGetSocketId (GnbHandle) , GnbGetDieNumber (GnbHandle));
      IvrsSupport = TRUE;

      if (InitialGnbIoapicId == 0x0) {
        NbioRegisterRead (GnbHandle, TYPE_SMN, 0x2801000, &Value, 0);
        IDS_HDT_CONSOLE (MAIN_FLOW, "PcdCfgGnbIoapicId Value 0x%x\n", Value);
        PcdSet8S (PcdCfgGnbIoapicId, (Value & 0xFF000000) >> 24);
      } else {
        PcdSet8S (PcdCfgGnbIoapicId, InitialGnbIoapicId + Counter);
      }

      if (InitialFchIoapicId == 0x0) {
        LibAmdMemRead (AccessWidth32, 0xFEC00010, &FchIoapicValue, (AMD_CONFIG_PARAMS *)NULL);
        IDS_HDT_CONSOLE (GNB_TRACE, "FchIoapicValue Value 0x%x\n", FchIoapicValue);
        PcdSet8S (PcdCfgFchIoapicId, (FchIoapicValue & 0xFF000000) >> 24);
      }

      IDS_HDT_CONSOLE (MAIN_FLOW, "PcdCfgGnbIoapicId 0x%x\n", PcdGet8 (PcdCfgGnbIoapicId));
      IDS_HDT_CONSOLE (MAIN_FLOW, "PcdCfgFchIoapicId 0x%x\n", PcdGet8 (PcdCfgFchIoapicId));

      GnbCreateIvrsEntry (GnbHandle, IvrsIvhdBlock10h, Ivrs);
      GnbCreateIvrsEntry (GnbHandle, IvrsIvhdBlock11h, Ivrs);
      GnbBuildIvmdList (IvrsIvmdBlock, Ivrs);
      if (IvrsRelativeAddrNamesSupport) {
        GnbCreateIvrsEntry (GnbHandle, IvrsIvhdrBlock40h, Ivrs);
        GnbBuildIvmdList (IvrsIvmdrBlock, Ivrs);
      }
    }
    Counter++;
    GnbHandle = GnbGetNextHandle (GnbHandle);
    if ((GnbHandle == NULL) && (AlternateList == FALSE)) {
      GnbHandle = NbioGetAltHandle (Pcie);
      AlternateList = TRUE;
    }
  }
  // Restore original PCD values probably unnecessary
  PcdSet8S (PcdCfgGnbIoapicId, InitialGnbIoapicId);
  PcdSet8S (PcdCfgFchIoapicId, InitialFchIoapicId);

  if (IvrsSupport == TRUE) {
    ChecksumAcpiTable ((ACPI_TABLE_HEADER*) Ivrs);
    GnbIommuIvrsTableDump (Ivrs);
    Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);
    if (!EFI_ERROR (Status)) {
      PspMessage.IvrsBufferAddrLo = (UINT32) ((UINTN) Ivrs & 0xFFFFFFFF);
      PspMessage.IvrsBufferAddrHi = (UINT32) (((UINTN) Ivrs >> 32) & 0xFFFFFFFF);
      PspMessage.IvrsAcpiTableSize = ((ACPI_TABLE_HEADER*) Ivrs)->TableLength;
      PspMboxBiosSendIvrsAcpiTable (&PspMessage);
      Status = AcpiTable->InstallAcpiTable (AcpiTable, Ivrs, ((ACPI_TABLE_HEADER*) Ivrs)->TableLength, &TableHandle);
    }
    if (EFI_ERROR (Status)) {
      AgesaStatus = AGESA_ERROR;
    }
  } else {
    IDS_HDT_CONSOLE (MAIN_FLOW, "  IVRS table not generated\n");
  }

  return AgesaStatus;
}


