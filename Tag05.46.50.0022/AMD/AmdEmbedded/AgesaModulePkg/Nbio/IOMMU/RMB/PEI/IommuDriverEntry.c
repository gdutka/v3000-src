/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/* $NoKeywords:$ */
/**
 * @file
 *
 * AmdNbioIOMMUPei Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  AmdNbioIOMMUZPPei
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */
#include <AMD.h>
#include <Filecode.h>
#include <PiPei.h>
#include <GnbDxio.h>
#include <Library/AmdBaseLib.h>
#include <Library/PcdLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/IdsLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/FabricResourceManagerLib.h>
#include <Library/GnbPciLib.h>
#include <Library/GnbPciAccLib.h>
#include <Library/NbioTableBlastLib.h>
#include <Library/CcxApicLib.h>
#include <Library/AmdIOMMUDmarLib.h>
#include <Library/FchBaseLib.h>
#include <Library/BaseCoreLogicalIdLib.h>
#include <Guid/GnbPcieInfoHob.h>
#include <Ppi/NbioPcieServicesPpi.h>
#include <GnbRegistersRMB.h>
#define FILECODE   NBIO_IOMMU_RMB_PEI_IOMMUDRIVERENTRY_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
extern GNB_TABLE ROMDATA      GnbIommuEnvInitTable [];
extern GNB_TABLE ROMDATA      GnbIommuEnvInitTableBx [];

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



/*----------------------------------------------------------------------------------------
 *                    P P I   N O T I F Y   D E S C R I P T O R S
 *----------------------------------------------------------------------------------------
 */



/*----------------------------------------------------------------------------------------*/
/**
 * AmdNbio IOMMU PEI driver entry point
 *
 *
 *
 * @param[in]  FileHandle  Standard configuration header
 * @param[in]  PeiServices Pointer to EFI_PEI_SERVICES pointer
 * @retval     EFI_STATUS
 */

EFI_STATUS
EFIAPI
AmdNbioIOMMUPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{

  PEI_AMD_NBIO_PCIE_SERVICES_PPI     *PcieServicesPpi;
  GNB_PCIE_INFORMATION_DATA_HOB      *PciePlatformConfigHob;
  FABRIC_TARGET                      MmioTarget;
  FABRIC_MMIO_ATTRIBUTE              MmioAttr;
  UINT64                             IommMmioSize;
  UINT64                             IommMmioBase;
  GNB_HANDLE                         *GnbHandle;
  EFI_STATUS                         Status;
  PCIe_PLATFORM_CONFIG               *Pcie;
  PCI_ADDR                           IommuPciAddress;
  UINT32                             Value;
  SOC_LOGICAL_ID                     LogicalId;
  UINT32                             Property;

  Property = TABLE_PROPERTY_DEFAULT;

  AGESA_TESTPOINT (TpNbioIommuPEIEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);

  // Need topology structure
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdNbioPcieServicesPpiGuid,
                             0,
                             NULL,
                             (VOID **)&PcieServicesPpi
                             );
  ASSERT (Status == EFI_SUCCESS);

  PcieServicesPpi->PcieGetTopology (PcieServicesPpi, &PciePlatformConfigHob);
  Pcie = &(PciePlatformConfigHob->PciePlatformConfigHob);
  GnbHandle = NbioGetHandle (Pcie);

  // Allocate BAR for IOMMU
  if (PcdGetBool (PcdCfgIommuMMIOAddressReservedEnable)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Will reserve IOMMU Bars \n", __FUNCTION__);
    MmioTarget.TgtType = TARGET_RB;
    MmioTarget.SocketNum = GnbHandle->SocketId;
    MmioTarget.RbNum = GnbHandle->DieNumber;
    IommMmioSize = SIZE_512KB;
    MmioAttr.MmioType = NON_PCI_DEVICE_BELOW_4G;
    FabricAllocateMmio (&IommMmioBase, &IommMmioSize, ALIGN_512K, MmioTarget, &MmioAttr);
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a : IOMMU MMIO at address 0x%x for Socket 0x%x Silicon 0x%x\n", \
                                     __FUNCTION__ , IommMmioBase, GnbHandle->SocketId, GnbHandle->DieNumber);

    Value = (UINT32)IommMmioBase;

    IommuPciAddress = NbioGetHostPciAddress (GnbHandle);
    IommuPciAddress.Address.Function = 0x2;

    // If IOMMU is enabled set MMIO enable bit
    if (PcdGetBool(PcdCfgIommuSupport)) {
      Value = Value | 1;
      IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Hit enable bit for IOMMU BAR 0x%x\n", __FUNCTION__, Value);
    }
    else {
      IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Did NOT enable bit for IOMMU BAR 0x%x\n", __FUNCTION__, Value);
    }

    GnbLibPciWrite (IommuPciAddress.AddressValue | 0x44, AccessWidth32, &Value, NULL);
  }
  else {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a : System is configured to not enable MMIO \n", __FUNCTION__);
  }

  if (PcdGetBool(PcdCfgIommuSupport)) {
    // If IOMMU is enabled set BUS_MASTER bit for PPR requirement
    SmnRegisterRMW (GnbHandle->Address.Address.Bus, SMN_IOMMU_COMMAND_ADDRESS,
                          (UINT32) ~(IOMMU_COMMAND_BUS_MASTER_EN_MASK), (1 << IOMMU_COMMAND_BUS_MASTER_EN_OFFSET), 0);
  } else {
    // Hide IOMMU function if disabled
    SmnRegisterRMW (GnbHandle->Address.Address.Bus, SMN_CFG_IOHC_PCI_ADDRESS,
                         (UINT32) ~(CFG_IOHC_PCI_CFG_IOHC_PCI_Dev0Fn2RegEn_MASK),
                         (0x0 << CFG_IOHC_PCI_CFG_IOHC_PCI_Dev0Fn2RegEn_OFFSET), 0);
  }
  if (PcdGet8 (PcdNbioGlobalCgOverride) == 0) {
      PcdSetBoolS (PcdIommuL2ClockGatingEnable, FALSE);
      PcdSetBoolS (PcdIommuL1ClockGatingEnable, FALSE);
      PcdSetBoolS (PcdCfgIOMMUDynamicPgEnable, FALSE);
      PcdSetBoolS (PcdCfgIOMMUL1MemoryPGEnable, FALSE);
      PcdSetBoolS (PcdCfgIOMMUL2MemoryPGEnable, FALSE);
  }

  if (FALSE == PcdGetBool (PcdCfgIommuSupport)) {
    Property |= TABLE_PROPERTY_IOMMU_DISABLED;
  }
  // Program up IOMMU NBIO Tables
  Status = GnbBlastTable (
             GnbHandle,
             GnbIommuEnvInitTable,
             Property,
             0,
             NULL
             );

  BaseGetLogicalIdOnExecutingCore (&LogicalId);
  if ((LogicalId.Family == AMD_FAMILY_RMB) && ((LogicalId.Revision & AMD_REV_F19_RMB_Bx) != 0)) {
    Status = GnbBlastTable (
               GnbHandle,
               GnbIommuEnvInitTableBx,
               Property,
               0,
               NULL
               );
  }

  if (PcdGet8(PcdAmdApicMode) != xApicMode) {
    //
    // If X2 APIC is enabled, enable XT_SUP in IOMMU
    //

    // IOMMUL1::L1_FEATURE_SUP_CNTRL_inst[PCIE[3:2],IOAGR,PCIE0]
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_PCIE0_L1_FEATURE_SUP_CNTRL_ADDRESS,
                    (UINT32) ~(L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_MASK),
                    (1 << L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_OFFSET),
                    0
                    );

    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_IOAGR_L1_FEATURE_SUP_CNTRL_ADDRESS,
                    (UINT32) ~(L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_MASK),
                    (1 << L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_OFFSET),
                    0
                    );

    if ((LogicalId.Family == AMD_FAMILY_RMB) && ((LogicalId.Revision & AMD_REV_F19_RMB_Bx) != 0)) {
      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                      SMN_PCIE2_L1_FEATURE_SUP_CNTRL_ADDRESS,
                      (UINT32) ~(L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_MASK),
                      (1 << L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_OFFSET),
                      0
                      );

      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                      SMN_PCIE3_L1_FEATURE_SUP_CNTRL_ADDRESS,
                      (UINT32) ~(L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_MASK),
                      (1 << L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_OFFSET),
                      0
                      );
    }

    // IOMMUL2::IOMMU_MMIO_CONTROL0_W
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                    SMN_IOMMU_MMIO_CONTROL0_W_ADDRESS,
                    (UINT32) ~(IOMMU_MMIO_CONTROL0_W_XT_SUP_W_MASK),
                    (1 << IOMMU_MMIO_CONTROL0_W_XT_SUP_W_OFFSET),
                    0
                    );

  }

  // If DMAr Mitigation is enabled, turn on IOMMU and setup translations
  if (PcdGetBool(PcdIvInfoDmaReMap)) {
    if (FchReadSleepType () == 0x3) {
      GnbHandle = NbioGetHandle (Pcie);
      SetupAmdIommuDmar (GnbHandle);
    }
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);

  AGESA_TESTPOINT (TpNbioIommuPEIExit, NULL);
  return EFI_SUCCESS;
}


