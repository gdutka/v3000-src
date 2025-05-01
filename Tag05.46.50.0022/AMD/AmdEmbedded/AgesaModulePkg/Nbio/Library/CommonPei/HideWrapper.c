/**
 *  @file HideWrapper.c
 *  @brief Support wrapper removal from complex for harvesting or power gating
 */
/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <AMD.h>
#include <Gnb.h>
#include <PiPei.h>
#include <Filecode.h>
#include <GnbDxio.h>
#include <Library/PcieConfigLib.h>
#include <Library/NbioHandleLib.h>

#define FILECODE NBIO_LIBRARY_COMMONPEI_HIDEWRAPPER_FILECODE

#define PcieConfigAttachChild(P, C)  (P)->Child = (UINT16) ((UINT8 *) C - (UINT8 *) P);


typedef struct {
  UINT32               WrapperNum;
  PCIe_WRAPPER_CONFIG  *ThisWrapper;
  PCIe_WRAPPER_CONFIG  *PrevWrapper;
  PCIe_WRAPPER_CONFIG  *NextWrapper;
  PCIe_ENGINE_CONFIG   *FirstEngineOnNext; // First Engine on NextWrapper
  PCIe_ENGINE_CONFIG   *LastEngineOnThis; // Last Engine on ThisWrapper
  PCIe_ENGINE_CONFIG   *LastEngineOnPrev; // Last Engine on PrevWrapper
} HIDE_WRAPPER_INFO;

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
 * Find the last engine of a given wrapper (PrevWrapper) by saving the pointer for each engine.
 * The last time this is called will save the pointer to the last one. *
 *
 *
 * @param[in]     Engine    Engineer on this Wrapper
 * @param[in,out] Buffer    Save the hidden Wrapper's Info
 * @param[in]     Wrapper   Wrapper to be hidden
 */
VOID
STATIC
FindLastEngineOfPrevCallback (
  IN      PCIe_ENGINE_CONFIG                *Engine,
  IN OUT  VOID                              *Buffer,
  IN      PCIe_WRAPPER_CONFIG               *Wrapper
  )
{
  HIDE_WRAPPER_INFO     *HideInfo = (HIDE_WRAPPER_INFO *)Buffer;
  HideInfo->LastEngineOnPrev = Engine;

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Given the 1st wrapper on a non-zero NBIO GenHandle, find its previous wrapper
 * The previous wrapper is Last wrapper on GnbHandle - 1.
 * Save the previous wrapper to PrevWrapper.
 *
 *
 * @param[in]     Buffer    Save the hidden Wrapper's Info
 * @param[in,out] Wrapper   Wrapper to be hidden
 */
VOID
STATIC
FindPrevWrapper (
  IN OUT  VOID                              *Buffer,
  IN      PCIe_WRAPPER_CONFIG               *Wrapper
  )
{
  PCIe_SILICON_CONFIG  *Silicon = NULL;
  PCIe_SILICON_CONFIG  *PrevSilicon = NULL;
  PCIe_WRAPPER_CONFIG  *PrevWrapper = NULL;
  UINT8                 InstanceID = 0;
  HIDE_WRAPPER_INFO     *HideInfo = (HIDE_WRAPPER_INFO *)Buffer;
  PCIe_PLATFORM_CONFIG  *Pcie = NULL;


  //IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry\n", __FUNCTION__);
  Pcie = PcieConfigGetPlatform (Wrapper);

  // Get parent Silicon of Wrapper
  Silicon = (PCIe_SILICON_CONFIG*) PcieConfigGetParentSilicon(Wrapper);
  InstanceID = Silicon->InstanceId;

  // IDS_HDT_CONSOLE (GNB_TRACE, "Silicon = 0x%08X, Silicon->Header.DescriptorFlags = 0x%08X, InstanceID = 0x%08X\n",
  //  Silicon, Silicon->Header.DescriptorFlags, InstanceID);

  // Invalid. InstanceID should be greater than 0 for this function.
  if (InstanceID == 0)
    return;

  // Get the Child Silicon from Pcie
  PrevSilicon = PcieConfigGetChildSilicon(Pcie);

  // IDS_HDT_CONSOLE (GNB_TRACE, "Pcie = 0x%08X, Pcie->Header.DescriptorFlags = 0x%08X, PrevSilicon = 0x%08X\n",
  //  Pcie, Pcie->Header.DescriptorFlags, PrevSilicon);

  while (PrevSilicon != NULL) {
    // Look for the previous Silicon
    if (PrevSilicon->InstanceId == (InstanceID - 1))
      break;
    // IDS_HDT_CONSOLE (GNB_TRACE, "PrevSi = 0x%08X, PrevSi_DescriptorFlags = 0x%08X, PrevSilicon->InstanceID = %d\n",
    //  PrevSilicon, PrevSilicon->Header.DescriptorFlags, PrevSilicon->InstanceId);

    // PrevSilicon = PcieConfigGetNextDescriptor(PrevSilicon); Not working since DESCRIPTOR_TERMINATE_LIST is set
    PrevSilicon = (PCIe_SILICON_CONFIG *)PcieConfigGetNextTopologyDescriptor (PrevSilicon, DESCRIPTOR_TERMINATE_TOPOLOGY);
  }
  // Added for solving coverity error, technically PrevSilicon will never be NULL.
  if(PrevSilicon == NULL)
    return;
  PrevWrapper = PcieConfigGetChildWrapper(PrevSilicon);

  // IDS_HDT_CONSOLE (GNB_TRACE, "PrevWrapper = 0x%08X\n", PrevWrapper);

  while (PrevWrapper) {
    // Eventually, the last Wrapper will be saved
    HideInfo->PrevWrapper = PrevWrapper;
    PrevWrapper = PcieConfigGetNextDescriptor(PrevWrapper);
  }

  // IDS_HDT_CONSOLE (GNB_TRACE, "HideInfo->PrevWrapper = 0x%08X\n", HideInfo->PrevWrapper);
  // IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Find the last engine of a given wrapper (ThisWrapper) by saving the pointer for each engine.
 * The last time this is called will save the pointer to the last one. *
 *
 *
 * @param[in]     Engine    Engineer on this Wrapper
 * @param[in,out] Buffer    Save the hidden Wrapper's Info
 * @param[in]     Wrapper   Wrapper to be hidden
 */
VOID
STATIC
FindLastEngineOfThisCallback (
  IN      PCIe_ENGINE_CONFIG                *Engine,
  IN OUT  VOID                              *Buffer,
  IN      PCIe_WRAPPER_CONFIG               *Wrapper
  )
{
  HIDE_WRAPPER_INFO     *HideInfo = (HIDE_WRAPPER_INFO *)Buffer;
  HideInfo->LastEngineOnThis = Engine;

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Find the "wrapper" to be removed, along with the pointer to the previous wrapper.
 *
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer points to the hidden Wrapper's Info
 * @param[in]     GnbHandle GnbHandle Pointer
 */
VOID
STATIC
FindWrapperCallback (
  IN      PCIe_WRAPPER_CONFIG               *Wrapper,
  IN OUT  VOID                              *Buffer,
  IN      GNB_HANDLE                        *GnbHandle
  )
{
  HIDE_WRAPPER_INFO   *HideInfo;
  HideInfo = (HIDE_WRAPPER_INFO *) Buffer;

  // IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry\n", __FUNCTION__);
  // IDS_HDT_CONSOLE (GNB_TRACE, "Wrapper->WrapId = %d, HideInfo->WrapperNum = %d, Wrapper = 0x%08X\n",
  // Wrapper->WrapId, HideInfo->WrapperNum, Wrapper);

  if (Wrapper->WrapId == HideInfo->WrapperNum) {
    // Found the matching Wrapper. Save it to ThisWrapper.
    HideInfo->ThisWrapper = Wrapper;
  }
  else {
    // If ThisWrapper hasn't been found, always set PrevWrapper with the current Wrapper
    if (HideInfo->ThisWrapper == NULL) {
      HideInfo->PrevWrapper = Wrapper;
    }
    else {
      // Only set NextWrapper once.
      if (HideInfo->NextWrapper == NULL) {
        HideInfo->NextWrapper = Wrapper;
      }
    }
  }
  // IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
  return;
}

/**
 *  @brief Function to remove wrapper from a GnbHandle
 *
 *  @param [in] GnbHandle Pointer to the Silicon Descriptor
 *  @param [in] WrapperNum WrapId to be hidden
 *  @return AGESA_STATUS
 *
 *  @details In the event of PCIe core power gating or harvesting, we may need to remove a wrapper from
 *  the PCIe complex.
 */
AGESA_STATUS
HideWrapper (
  IN      GNB_HANDLE     *GnbHandle,
  IN      UINT32         WrapperNum
  )
{
  HIDE_WRAPPER_INFO     HideInfo;
  PCIe_SILICON_CONFIG   *Silicon = (PCIe_SILICON_CONFIG *)GnbHandle;
  UINT8                 DeleteFirstWrapper = 0;
  UINT8                 DeleteLastWrapper = 0;

  HideInfo.WrapperNum = WrapperNum;
  HideInfo.PrevWrapper = NULL;
  HideInfo.ThisWrapper = NULL;
  HideInfo.NextWrapper = NULL;
  HideInfo.FirstEngineOnNext = NULL;
  HideInfo.LastEngineOnThis = NULL;
  HideInfo.LastEngineOnPrev  = NULL;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  // IDS_HDT_CONSOLE (GNB_TRACE, "% GnbHandle = 0x%08X\n", GnbHandle);

  // Update HideInfo with ThisWrapper, PrevWrapper, and NextWrapper
  PcieConfigRunProcForAllWrappersInNbio (
    DESCRIPTOR_ALL_WRAPPERS,
    FindWrapperCallback,
    &HideInfo,
    GnbHandle
    );

  // For the case of hiding "Wrapper 0 from NBIO 1, 2, 3, etc. Find the PrevWrapper from the last GnbHandle - 1
  if (HideInfo.PrevWrapper == NULL) {
    DeleteFirstWrapper = 1;
    if (Silicon->InstanceId != 0)
      FindPrevWrapper(&HideInfo, HideInfo.ThisWrapper);
  }

  // For the case of hiding the Last Wrapper, need to set NextWrapper to the first Wrapper from GnbHandle + 1
  if (HideInfo.NextWrapper == NULL && HideInfo.ThisWrapper != NULL) {
    HideInfo.NextWrapper = (PCIe_WRAPPER_CONFIG *) PcieConfigGetNextTopologyDescriptor (HideInfo.ThisWrapper,
                                                                                        DESCRIPTOR_TERMINATE_TOPOLOGY);
    DeleteLastWrapper = 1;
    // IDS_HDT_CONSOLE (GNB_TRACE, "HideInfo.NextWrapper = 0x%08Xd, WrapId = 0x%08X, DescriptorFlags = 0x%08X\n",
    // HideInfo.NextWrapper, HideInfo.NextWrapper->WrapId, HideInfo.NextWrapper->Header.DescriptorFlags);
  }

  // Find the 1st Engine on NextWrapper
  if (HideInfo.NextWrapper != NULL){
  HideInfo.FirstEngineOnNext = PcieConfigGetChildEngine(HideInfo.NextWrapper);
  }
  else{
    HideInfo.FirstEngineOnNext=NULL;
  }
  // Find the Last Engine on PrevWrapper
  PcieConfigRunProcForAllEnginesInWrapper (
    DESCRIPTOR_PCIE_ENGINE,
    FindLastEngineOfPrevCallback,
    &HideInfo,
    HideInfo.PrevWrapper
    );

  // Find the Last Engine on ThisWrapper
  PcieConfigRunProcForAllEnginesInWrapper (
    DESCRIPTOR_PCIE_ENGINE,
    FindLastEngineOfThisCallback,
    &HideInfo,
    HideInfo.ThisWrapper
    );

  // IDS_HDT_CONSOLE (GNB_TRACE, "WrapperNum = %d, PrevWrapper = 0x%08X, ThisWrapper = 0x%08X, NextWrapper = 0x%08X\n,
  //                             FirstEngineOnNext = 0x%08X, LastEngineOnThis = 0x%08X, LastEngineOnPrev 0x%08X\n",
  //                             HideInfo.WrapperNum, HideInfo.PrevWrapper, HideInfo.ThisWrapper, HideInfo.NextWrapper,
  //                             HideInfo.FirstEngineOnNext, HideInfo.LastEngineOnThis, HideInfo.LastEngineOnPrev);

  // First wrapper (Wrapper0) in GnbHandle
  if (DeleteFirstWrapper) {
    // If it's NOT on NBIO0, need to set the PEER
    // IDS_HDT_CONSOLE (GNB_TRACE, "Silicon->InstanceId = %d\n", Silicon->InstanceId);
    if (Silicon->InstanceId != 0) {
      PcieConfigAttachDescriptors (DESCRIPTOR_PCIE_WRAPPER, &HideInfo.PrevWrapper->Header, &HideInfo.NextWrapper->Header);
      PcieConfigAttachDescriptors (DESCRIPTOR_PCIE_ENGINE, &HideInfo.LastEngineOnPrev->Header, &HideInfo.FirstEngineOnNext->Header);
    }
    // Set Silicon's Child to Wrapper1
    PcieConfigAttachChild(&Silicon->Header, &HideInfo.NextWrapper->Header);
  }
  // Last wrapper in GnbHandle
  else if (DeleteLastWrapper) {
    PcieConfigSetDescriptorFlags(HideInfo.PrevWrapper, HideInfo.ThisWrapper->Header.DescriptorFlags);
    PcieConfigSetDescriptorFlags(HideInfo.LastEngineOnPrev, HideInfo.LastEngineOnThis->Header.DescriptorFlags);

    // Note: Must have this condition. Otherwise, the DescriptorFlags set above will be overwritten when NextWrapper = NULL.
    if (HideInfo.NextWrapper) {
      PcieConfigAttachDescriptors (DESCRIPTOR_PCIE_WRAPPER, &HideInfo.PrevWrapper->Header, &HideInfo.NextWrapper->Header);
      PcieConfigAttachDescriptors (DESCRIPTOR_PCIE_ENGINE, &HideInfo.LastEngineOnPrev->Header, &HideInfo.FirstEngineOnNext->Header);
    }
  }
  // wrapper that is neither the first nor the last on the GnbHandle
  else {
    // The existing PcieConfigAttachDescriptors() won't work. so, trying this way.
    HideInfo.PrevWrapper->Header.Peer = (UINT16) ((UINT8 *) &HideInfo.NextWrapper->Header - (UINT8 *) &HideInfo.PrevWrapper->Header);
    HideInfo.LastEngineOnPrev->Header.Peer = (UINT16) ((UINT8 *) &HideInfo.FirstEngineOnNext->Header - (UINT8 *) &HideInfo.LastEngineOnPrev->Header);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);

  return AGESA_SUCCESS;
}

