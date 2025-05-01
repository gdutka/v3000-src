/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  <PiPei.h>
#include  <AGESA.h>
#include  <GnbDxio.h>
#include  <AmdPcieComplex.h>
#include  <Library/BaseLib.h>
#include  <Library/BaseMemoryLib.h>
#include  <Library/PcieConfigLib.h>
#include  <Library/NbioHandleLib.h>
#include  <Library/BxbAccessLib.h>
#include  <Library/AmdBaseLib.h>
#include  <Library/IdsLib.h>
#include  <Filecode.h>
#include  "BxbComplexData.h"

#define FILECODE LIBRARY_BXBNBIO_BXBINITLIBV1_BXBINITLIB_FILECODE
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
extern BXB_COMPLEX_CONFIG            BixbyComplexData;


#define PcieConfigAttachChild(P, C)  (P)->Child = (UINT16) ((UINT8 *) C - (UINT8 *) P);
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
 * @param[in]  ComplexData       Pointer to Bixby Complex Data
 * @param[in]  ComplexDataSize   Size of Bixby Complex Data
 * @param[in]  Pcie              Pointer PCIe_PLATFORM_CONFIG structure
 * @retval     EFI_STATUS
 */

VOID
STATIC
BixbyAddHandle (
  IN       VOID                 *ComplexData,
  IN       UINT32               ComplexDataSize,
  IN       PCIe_PLATFORM_CONFIG *Pcie
  )
{
  EFI_STATUS                Status;
  PCIe_SILICON_CONFIG       *Silicon;
  UINTN                     ComplexIndex;
//  UINTN                     BufferSize;
  VOID                      *Buffer;
  GNB_HANDLE                *GnbHandle;

  Status = EFI_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  ComplexIndex = 0;
  GnbHandle = NbioGetHandle (Pcie);

  ASSERT (Pcie->Header.Parent != NULL);
  Pcie->Header.Peer = Pcie->Header.Parent;
  Buffer = (VOID *) ((UINT8 *) Pcie + Pcie->Header.Parent);
  Pcie->Header.Parent = NULL;

  while (GnbHandle != NULL) {
    ComplexIndex++;
    GnbHandle = GnbGetNextHandle (GnbHandle);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "Buffer = 0x%x\n", Buffer);
  IDS_HDT_CONSOLE (GNB_TRACE, "ComplexIndex = %d\n", ComplexIndex);
  IDS_HDT_CONSOLE (GNB_TRACE, "ComplexIndex[%d] = 0x%x\n", ComplexIndex, &Pcie->ComplexList[ComplexIndex].Header);


  //Attach Complex to Silicon which will be created by PcieFmBuildComplexConfiguration
  PcieConfigAttachChild (&Pcie->ComplexList[ComplexIndex].Header, &((PCIe_SILICON_CONFIG *) Buffer)->Header);
  PcieConfigSetDescriptorFlags (&Pcie->ComplexList[ComplexIndex], DESCRIPTOR_COMPLEX | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY);

  IDS_HDT_CONSOLE (GNB_TRACE, "Pcie->Header.DescriptorFlags = 0x%x\n", Pcie->Header.DescriptorFlags);
  IDS_HDT_CONSOLE (GNB_TRACE, "Pcie->Header.Parent = 0x%x\n", Pcie->Header.Parent);
  IDS_HDT_CONSOLE (GNB_TRACE, "Pcie->Header.Peer = 0x%x\n", Pcie->Header.Peer);
  IDS_HDT_CONSOLE (GNB_TRACE, "Pcie->Header.Child = 0x%x\n", Pcie->Header.Child);

  //Add copy from PcieComplex to Buffer using memcopy
  CopyMem(Buffer, ComplexData, ComplexDataSize);
  Silicon = (PCIe_SILICON_CONFIG *) BxbGetHandle (Pcie);
  PcieConfigAttachParent (&Pcie->ComplexList[ComplexIndex].Header, &Silicon->Header);
  Silicon->SocketId = DxioBixbySocket;
  Silicon->DieNumber = 0;
  Silicon->RBIndex = 0;
  Silicon->LogicalRBIndex = 0;
  Silicon->InstanceId = 0;
  Silicon->Address.Address.Bus = 0x5;   // @todo get this from NBIO base bus?
  Silicon->BusNumberLimit = 0xF;
  Silicon->StartLane = (UINT16) (0);
  Silicon->EndLane = (UINT16) (31);
  Silicon->SbPresent = FALSE;
  Silicon->SmuPresent = FALSE;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit [0x%x]\n", __FUNCTION__, Status);
  return;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to write all register spaces.
 *
 *
 * @param[in]  Pcie              Pointer PCIe_PLATFORM_CONFIG structure
 * @retval     VOID
 */
VOID
BixbyComplexInit (
  IN       PCIe_PLATFORM_CONFIG *Pcie
  )
{
  // Add Bixby complex to PCIe structure
  BixbyAddHandle (&BixbyComplexData, sizeof (BixbyComplexData), Pcie);
  return;
}
