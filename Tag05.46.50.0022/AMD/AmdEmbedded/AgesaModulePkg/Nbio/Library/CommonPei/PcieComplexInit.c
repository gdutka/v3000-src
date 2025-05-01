/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiPei.h>
#include <Filecode.h>
#include <GnbDxio.h>
//In AgesaPkg
#include <AmdPcieComplex.h>
#include <Ppi/FabricTopologyServices2Ppi.h>

#include <Library/AmdBaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/IdsLib.h>
#include <Library/PeiServicesLib.h>
#include <Guid/GnbPcieInfoHob.h>

#include <Library/NbioCommonLib.h>


#define FILECODE        NBIO_LIBRARY_COMMONPEI_PCIECOMPLEXINIT_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

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

#define PcieConfigAttachChild(P, C)  (P)->Child = (UINT16) ((UINT8 *) C - (UINT8 *) P);
#define PcieConfigAttachAlternate(P, C)  (P)->Peer = (UINT16) ((UINT8 *) C - (UINT8 *) P);
#define PcieConfigAttachParent(P, C) (C)->Parent = (UINT16) ((UINT8 *) C - (UINT8 *) P);

/*----------------------------------------------------------------------------------------*/
/**
 * Attach descriptors
 *
 *
 * @param[in]      Type       Descriptor type
 * @param[in,out]  Base       Base descriptor
 * @param[in,out]  New      New  descriptor
 */
VOID
STATIC
PcieConfigAttachDescriptors (
  IN       UINT32                         Type,
  IN OUT   PCIe_DESCRIPTOR_HEADER         *Base,
  IN OUT   PCIe_DESCRIPTOR_HEADER         *New
  )
{
  PCIe_DESCRIPTOR_HEADER  *Left;
  PCIe_DESCRIPTOR_HEADER  *Right;

  Left = PcieConfigGetPeer (DESCRIPTOR_TERMINATE_GNB, PcieConfigGetChild (Type, Base));
  ASSERT (Left != NULL);
  Right = PcieConfigGetChild (Type, New);
  Left->Peer = (UINT16) ((UINT8 *) Right - (UINT8 *) Left);
  PcieConfigResetDescriptorFlags (Left, DESCRIPTOR_TERMINATE_TOPOLOGY);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Attach configurations of two GNB to each other.
 *
 * Function will link all data structure to linked lists
 *
 * @param[in,out]  Base       Base complex descriptor
 * @param[in,out]  New        New complex descriptor
 */
VOID
STATIC
PcieConfigAttachComplexes (
  IN OUT   PCIe_COMPLEX_CONFIG            *Base,
  IN OUT   PCIe_COMPLEX_CONFIG            *New
  )
{
  // Connect Complex
  Base->Header.Peer = (UINT16) ((UINT8 *) New - (UINT8 *) Base);
  PcieConfigResetDescriptorFlags (Base, DESCRIPTOR_TERMINATE_TOPOLOGY);
  // Connect Silicon
  PcieConfigAttachDescriptors (DESCRIPTOR_SILICON, &Base->Header, &New->Header);
  // Connect Wrappers
  PcieConfigAttachDescriptors (DESCRIPTOR_PCIE_WRAPPER, &Base->Header, &New->Header);
  // Connect Engines
  PcieConfigAttachDescriptors (DESCRIPTOR_PCIE_ENGINE, &Base->Header, &New->Header);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Create internal PCIe configuration topology
 *
 *
 *
 * @param[in]  PeiServices       Pointer to EFI_PEI_SERVICES pointer
 * @retval     EFI_STATUS
 */
EFI_STATUS
NbioBuildPcieComplex (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                            Status;
  VOID                                  *Buffer;
  AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI *FabricTopologyServicesPpi;
  PCIe_PLATFORM_CONFIG                  *Pcie;
  PCIe_SILICON_CONFIG                   *Silicon;
  GNB_PCIE_INFORMATION_DATA_HOB         *GnbPcieInfoDataHob;
  AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP    *FabricIdMap;
  UINTN                                 ComplexesDataLength;
  UINTN                                 ComplexIndex;
  UINTN                                 NumberOfProcessors;
  UINTN                                 TotalNumberOfDie;
  UINTN                                 TotalNumberOfRootBridges;
  UINTN                                 SystemIdOffset;
  UINTN                                 SystemFabricID;
  UINTN                                 BusNumberBase;
  UINTN                                 BusNumberLimit;
  UINTN                                 SocketIndex;
  UINTN                                 DieIndex;
  UINTN                                 RootBridgeIndex;
  UINTN                                 PhysicalRootBridgeNumber;
  BOOLEAN                               HasFchDevice;
  BOOLEAN                               HasSystemMgmtUnit;
  BOOLEAN                               IsAlternate;
  BOOLEAN                               SkipLink;
  ROOT_BRIDGE_LOCATION                  SystemFchRootBridgeLocation;
  ROOT_BRIDGE_LOCATION                  SystemSmuRootBridgeLocation;

  Status = EFI_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  // Get number of sockets from CPU topology PPI
  // Locate CPU topology PPI
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdFabricTopologyServices2PpiGuid,
                             0,
                             NULL,
                             (VOID **)&FabricTopologyServicesPpi
                             );
  FabricTopologyServicesPpi->GetSystemInfo (&NumberOfProcessors, &TotalNumberOfDie, &TotalNumberOfRootBridges, &SystemFchRootBridgeLocation, &SystemSmuRootBridgeLocation);


  ComplexesDataLength = 0;
  for (SocketIndex = 0; SocketIndex < NumberOfProcessors; SocketIndex++) {
    IDS_HDT_CONSOLE (GNB_TRACE, "  Count Socket %d\n", SocketIndex);
    FabricTopologyServicesPpi->GetProcessorInfo (SocketIndex,
                                                 &TotalNumberOfDie,
                                                 &TotalNumberOfRootBridges
                                                 );
    for (DieIndex = 0; DieIndex < TotalNumberOfDie; DieIndex++) {
      IDS_HDT_CONSOLE (GNB_TRACE, "    Count Die %d\n", DieIndex);
      FabricTopologyServicesPpi->GetDieInfo (SocketIndex,
                                             DieIndex,
                                             &TotalNumberOfRootBridges,
                                             &SystemIdOffset,
                                             &FabricIdMap
                                             );
      for (RootBridgeIndex = 0; RootBridgeIndex < TotalNumberOfRootBridges; RootBridgeIndex++) {
        IDS_HDT_CONSOLE (GNB_TRACE, "      Count RootBridge %d\n", RootBridgeIndex);
        ComplexesDataLength += GetPcieComplexSize (SocketIndex, DieIndex, RootBridgeIndex);
        IDS_HDT_CONSOLE (GNB_TRACE, " New ComplexesDataLength = 0x%x\n", ComplexesDataLength);
      }
    }
  }

  ComplexesDataLength += GetPcieComplexSize (CHIPSET_SOCKET_INDEX, 0, 0);
  // Allocate PCIe Complex space (maximum possibly needed)
  Status = PeiServicesCreateHob (
    EFI_HOB_TYPE_GUID_EXTENSION,
    sizeof (GNB_PCIE_INFORMATION_DATA_HOB) + (UINT16) ComplexesDataLength,
    (VOID **)&GnbPcieInfoDataHob
    );

  GnbPcieInfoDataHob->EfiHobGuidType.Name = gGnbPcieHobInfoGuid;
  Pcie = &(GnbPcieInfoDataHob->PciePlatformConfigHob);
  ZeroMem (Pcie, sizeof (GNB_PCIE_INFORMATION_DATA_HOB));

  // Build PCIe Complex
  ComplexIndex = 0;
  if (Pcie != NULL) {
    PcieConfigAttachChild (&Pcie->Header, &Pcie->ComplexList[ComplexIndex].Header);
    PcieConfigSetDescriptorFlags (Pcie, DESCRIPTOR_PLATFORM | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_TOPOLOGY);
    IsAlternate = FALSE;
    SkipLink = FALSE;
    Buffer = (VOID *) &(GnbPcieInfoDataHob->ComplexConfigs);
    for (SocketIndex = 0; SocketIndex < NumberOfProcessors; SocketIndex++) {
      IDS_HDT_CONSOLE (GNB_TRACE, "  Process Socket %d\n", SocketIndex);
      for (DieIndex = 0; DieIndex < TotalNumberOfDie; DieIndex++) {
        IDS_HDT_CONSOLE (GNB_TRACE, "    Process Die %d\n", DieIndex);
        for (RootBridgeIndex = 0; RootBridgeIndex < TotalNumberOfRootBridges; RootBridgeIndex++) {
          IDS_HDT_CONSOLE (GNB_TRACE, "      Process  RootBridge %d\n", RootBridgeIndex);
          FabricTopologyServicesPpi->GetRootBridgeInfo (SocketIndex,
                                                DieIndex,
                                                RootBridgeIndex,
                                                &SystemFabricID,
                                                &BusNumberBase,
                                                &BusNumberLimit,
                                                &PhysicalRootBridgeNumber,
                                                &HasFchDevice,
                                                &HasSystemMgmtUnit
                                                );

          IDS_HDT_CONSOLE (GNB_TRACE, "        Bus Number 0x%x to 0x%x\n", BusNumberBase, BusNumberLimit);

          // Harvest the root bridge. If the root bridge is not present, don't add it to the complex.
          if (!IsRootBridgePresent(BusNumberBase, PhysicalRootBridgeNumber)) {
            continue;
          }

          if (BusNumberBase != 0xFF) {
            if ((IsAlternate == FALSE) && (IsAlternateComplex(SocketIndex, DieIndex, RootBridgeIndex) == TRUE)) {
              IsAlternate = TRUE;
              Pcie->Header.Peer = (UINT16)((UINT32) Buffer - (UINT32) &Pcie->Header);
              PcieConfigAttachAlternate(&Pcie->Header, &((PCIe_SILICON_CONFIG *) Buffer)->Header);
              SkipLink = TRUE;
            }
            //Attach Complex to Silicon which will be created by PcieFmBuildComplexConfiguration
            PcieConfigAttachChild (&Pcie->ComplexList[ComplexIndex].Header, &((PCIe_SILICON_CONFIG *) Buffer)->Header);
            //Attach Complex to Pcie
            PcieConfigAttachParent (&Pcie->Header, &Pcie->ComplexList[ComplexIndex].Header);
            PcieConfigSetDescriptorFlags (&Pcie->ComplexList[ComplexIndex], DESCRIPTOR_COMPLEX | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY);
            //Add copy from PcieComplex to Buffer using memcopy
            CopyMem(Buffer, GetPcieComplexPointer(SocketIndex, DieIndex, RootBridgeIndex), GetPcieComplexSize (SocketIndex, DieIndex, RootBridgeIndex));
            Silicon = PcieConfigGetChildSilicon (&Pcie->ComplexList[ComplexIndex]);
            if (Silicon == NULL) {
              IDS_HDT_CONSOLE (GNB_TRACE, "%a: Silicon structure came back NULL.\n", BusNumberBase);
              ASSERT(Silicon != NULL);
            } else {
              Silicon->SocketId = (UINT8) SocketIndex;
              Silicon->DieNumber = (UINT8) DieIndex;
              if (TotalNumberOfRootBridges > 1) {
                Silicon->RBIndex = (UINT8) PhysicalRootBridgeNumber;
              } else {
                Silicon->RBIndex = 3;
              }
              Silicon->LogicalRBIndex = (UINT8) RootBridgeIndex;
              Silicon->InstanceId = (UINT8) ComplexIndex;
              Silicon->Address.Address.Bus = BusNumberBase % MAX_PCI_BUS_NUMBER_PER_SEGMENT;
              Silicon->Address.Address.Segment = BusNumberBase / MAX_PCI_BUS_NUMBER_PER_SEGMENT;
              Silicon->BusNumberLimit = (UINT8) (BusNumberLimit % MAX_PCI_BUS_NUMBER_PER_SEGMENT);
              Silicon->StartLane += (UINT16) (0);
              if (TotalNumberOfRootBridges > 1) {
                Silicon->EndLane += (UINT16) (130);
              }
              Silicon->SbPresent = HasFchDevice;
              Silicon->SmuPresent = HasSystemMgmtUnit;
            }
            while (Silicon != NULL) {
              PcieConfigAttachParent (&Pcie->ComplexList[ComplexIndex].Header, &Silicon->Header);
              Silicon = (PCIe_SILICON_CONFIG *) PcieConfigGetNextTopologyDescriptor (Silicon, DESCRIPTOR_TERMINATE_TOPOLOGY);
            }
            if ((ComplexIndex > 0) && (SkipLink != TRUE)) {
              PcieConfigAttachComplexes (&Pcie->ComplexList[ComplexIndex - 1], &Pcie->ComplexList[ComplexIndex]);
            } else {
              SkipLink = FALSE;
            }
            Buffer = (VOID *) ((UINT8 *) Buffer + GetPcieComplexSize (SocketIndex, DieIndex, RootBridgeIndex));
            ComplexIndex++;
          }
        }
      }
    }
    // Initialize pointer to be optionally used for chipset
    Pcie->Header.Parent = (UINT16)((UINT32) Buffer - (UINT32) &Pcie->Header);
    IDS_HDT_CONSOLE (GNB_TRACE, "Pcie->Header.Parent = 0x%x\n", Pcie->Header.Parent);

    // Initialize remaining PcieComplexData defaults for all engines and wrappers
    PcieFixupComplexDefaults(Pcie);
    // Use logical lane numbers to generate physical as well as account for port reversals
    // For use with DXIO
    PcieFixupPlatformConfig (Pcie);

    // Debug output to view complex
    PcieConfigDebugDump (Pcie);

    // Harvest PCIe controllers
    EnumerateAndHarvestWrappers(Pcie);

  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit [0x%x]\n", __FUNCTION__, Status);
  return Status;
}


