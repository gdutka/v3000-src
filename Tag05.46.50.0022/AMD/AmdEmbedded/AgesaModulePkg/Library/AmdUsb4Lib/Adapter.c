/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/IdsLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/TimerLib.h>

#include "AmdUsb4.h"
#include "Library/AmdUsb4Lib.h"

#include "AmdUsb4/ConfigurationSpace.h"
#include "AmdUsb4/ControlPacket.h"

#define FILECODE LIBRARY_AMDUSB4LIB_ADAPTER_FILECODE


/*----------------------------------------------------------------------------------------*/
/**
 * Find next Adapter with specific type
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 * @param[in]  StartAdapterNum  Adapter number to start with
 * @param[in]  *pRouter         Point to Router CM Data structure
 * @param[in]  *pAdapter        Point to Adapter CM Data structure
 *
 * @param[out]  *pAdapter        Point to Adapter CM Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_NOT_FOUND          Cannot find Adapter.
**/
EFI_STATUS
FindNextAdapter (
  IN      AMD_USB4_CM             *pCM,
  IN      UINT32                  StartAdapterNum,
  IN      AMD_USB4_CM_ROUTER      *pRouter,
  IN      AMD_USB4_CM_CELL_TYPE   CellType,
  IN  OUT AMD_USB4_CM_CELL        *pAdapter
  )
{
  UINT32           AdapterNumber;
  AMD_USB4_CM_CELL *pAdapterBase;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Entry\n", __FUNCTION__);

  if ( NULL == pRouter || NULL == pCM ) {
    IDS_HDT_CONSOLE_USB4_ERROR (" One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  pAdapterBase = pRouter->pAdapter;

  for (AdapterNumber = StartAdapterNum; AdapterNumber <= (UINT32) (pRouter->MaxAdapter); AdapterNumber++) {
    pAdapter = (AMD_USB4_CM_CELL*) (&pAdapter[AdapterNumber]);
    if (pAdapter->Header.CellType == CellType) {
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Find First DFP Lane Adapter
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 * @param[in]  *pRouter         Point to Router CM Data structure
 * @param[in]  *pAdapter        Point to Adapter CM Data structure
 *
 * @param[out]  *pAdapter        Point to DFP Lane Adapter
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_NOT_FOUND          Cannot find Adapter.
**/
EFI_STATUS
FindDFPLaneAdapter (
  IN      AMD_USB4_CM                     *pCM,
  IN      AMD_USB4_CM_ROUTER              *pRouter,
  IN  OUT AMD_USB4_CM_LANE_ADAPTER        *pAdapter
  )
{
  UINT32           AdapterNum;
  EFI_STATUS       Status;

  AdapterNum = 0;
  Status = EFI_SUCCESS;
  IDS_HDT_CONSOLE_USB4_INFO ("%a Entry\n", __FUNCTION__);

  if ( NULL == pRouter || NULL == pCM ) {
    IDS_HDT_CONSOLE_USB4_ERROR (" One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  if (pRouter->IsHost == 0) {
    //
    // Device Router start from Upstream Adapter + 2
    //
    AdapterNum = (UINT32)pRouter->UpstreamAdapter + 2;
  }

  Status = FindNextAdapter (
             pCM,
             AdapterNum,
             pRouter,
             CELL_TYPE_LANE_ADAPTER,
             (AMD_USB4_CM_CELL*)pAdapter
             );

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Configure Lane adapters in router
 *
 * @param[in]  *pCM             Point to global structure AMD_USB4_CM
 * @param[in]  AdapterNum       Lane adapter number
 * @param[in]  *pRouter         Point to Router CM Data structure
 * @param[in]  *pAdapter        Point to Adapter CM Data structure
 * @param[in]  *pAdapterCs      Point to Adapter Configuration Space data
 *
 * @param[out]  *pRouter         Point to Router CM Data structure
 * @param[out]  *pAdapter        Point to Adapter CM Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_DEVICE_ERROR -     HW error.
**/
EFI_STATUS
EnumerateLaneAdapter (
  IN      AMD_USB4_CM                           *pCM,
  IN      UINT32                                AdapterNum,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter,
  IN  OUT AMD_USB4_CM_LANE_ADAPTER              *pAdapter,
  IN      AMD_USB4_ADAPTER_CONFIGURATION_SPACE  *pAdapterCs
)
{
  EFI_STATUS                                      Status;
  AMD_USB4_TMU_ADAPTER_CONFIGURATION_CAPABILITY   *TmuAdapterCapability;
  AMD_USB4_LANE_ADAPTER_CONFIGURATION_CAPABILITY  *LaneAdapterCapability;
  AMD_USB4_PORT_CAPABILITY                        *USB4AdapterCapability;
  AMD_USB4_PATH_CS_0                              *ControlPath;
  UINT8                                           CapabilityPointer;
  UINT8                                           CapabilityID;
  UINT32                                          *CapabilityReg;
  UINT32                                          DataSize;
  UINT8                                           NextCapabilityPointer;

  Status                 = EFI_SUCCESS;
  LaneAdapterCapability  = NULL;
  TmuAdapterCapability   = NULL;
  USB4AdapterCapability  = NULL;
  ControlPath            = NULL;
  CapabilityPointer      = 0;
  CapabilityReg          = NULL;
  CapabilityID           = 0;
  NextCapabilityPointer  = 0;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Entry\n", __FUNCTION__);

  if ( NULL == pRouter || NULL == pAdapter || NULL == pCM || NULL == pAdapterCs ) {
    IDS_HDT_CONSOLE_USB4_ERROR (" One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }
  //
  // 1. Base CS passed by up layer
  //   Update Lane Adapter Data structure
  //
  pAdapter->Header.CellType   = CELL_TYPE_LANE_ADAPTER;
  pAdapter->Header.TopologyID = pRouter->Header.TopologyID;
  pAdapter->Header.AdapterNum = (UINT8)AdapterNum;
  pAdapter->Header.Parent     = (AMD_USB4_CM_CELL_HEADER*)pRouter;

  pAdapter->AdpHeader.MaxInputHopId       = (UINT16)(pAdapterCs->AdpCs5.Field.MaxInputHopID);
  pAdapter->AdpHeader.MaxOutputHopId      = (UINT16)(pAdapterCs->AdpCs5.Field.MaxOutputHopID);
  pAdapter->AdpHeader.pSiblingAdapter     = NULL;
  pAdapter->AdpHeader.pLinkPartner        = NULL;

  pAdapter->TotalBuffer  = pAdapterCs->AdpCs4.Field.TotalBuffers;
  pAdapter->NFCBuffer    = pAdapterCs->AdpCs4.Field.NonFlowControlledBuffers;
  pAdapter->Plugged      = pAdapterCs->AdpCs4.Field.Plugged;
  pAdapter->Lock         = pAdapterCs->AdpCs4.Field.Lock;
  pAdapter->HE           = pAdapterCs->AdpCs3.Field.HECError;
  pAdapter->FCE          = pAdapterCs->AdpCs3.Field.FlowControlError;
  pAdapter->SBC          = pAdapterCs->AdpCs3.Field.SharedBufferingCapable;
  pAdapter->LinkCredit   = pAdapterCs->AdpCs5.Field.LinkCreditsAllocated;
  pAdapter->HEE          = pAdapterCs->AdpCs5.Field.HECErrorEnable;
  pAdapter->FCEE         = pAdapterCs->AdpCs5.Field.FlowControlErrorEnable;
  pAdapter->DHP          = pAdapterCs->AdpCs5.Field.DisableHotPlugEvents;

  CapabilityPointer = (UINT8)pAdapterCs->AdpCs1.Field.NextCapabilityPointer;

  IDS_HDT_CONSOLE_USB4_DEBUG (" Lane Adapter Information:\n");
  IDS_HDT_CONSOLE_USB4_DEBUG ("  AdapterNum     = 0x%lx\n",       pAdapter->Header.AdapterNum);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  CellType       = 0x%x\n",        pAdapter->Header.CellType);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaxInputHopId  = 0x%x\n",        pAdapter->AdpHeader.MaxInputHopId);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaxOutputHopId = 0x%x\n",        pAdapter->AdpHeader.MaxOutputHopId);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  CapabilityPointer = 0x%x\n",     CapabilityPointer);

  while ( CapabilityPointer != 0 ) {
    //
    //  Read Request: loop Capability link (Size = 1DW)
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ADAPTER,                                   // CsType
               AdapterNum,                                          // AdapterNum
               CapabilityPointer,                                   // ReadAddress
               1,                                                   // ReadSize
               (UINT32 **)&CapabilityReg,                           // DataOut
               &DataSize                                            // DataSize
               );
    if (EFI_ERROR (Status) || (DataSize != 1)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Error reading capabilities\n");
      return EFI_DEVICE_ERROR;
    }

    NextCapabilityPointer  = (UINT8)(*CapabilityReg);
    CapabilityID  = (UINT8)((*CapabilityReg) >> 8);
    IDS_HDT_CONSOLE_USB4_DEBUG (
      " CapabilityID = 0x%x, NextCapabilityPointer = 0x%x\n",
      CapabilityID,
      NextCapabilityPointer
      );
    switch (CapabilityID) {
      case TMU_ADAPTER_CONFIG_CAP_ID:
        //
        // i.Save TMUAdapterConfigurationAddress
        //
        IDS_HDT_CONSOLE_USB4_DEBUG (" Get Tmu Capability (0x%x)\n", CapabilityPointer);
        pAdapter->TMUCapability = CapabilityPointer;
        DataSize = sizeof (AMD_USB4_TMU_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32);
        Status = AmdUsb4Ring0ReadCS (
                   pCM,
                   pRouter->Header.TopologyID,
                   CS_TARGET_ADAPTER,                                   // CsType
                   AdapterNum,                                          // AdapterNum
                   CapabilityPointer,                                   // ReadAddress
                   DataSize,                                            // ReadSize
                   (UINT32**)&TmuAdapterCapability,                     // DataOut
                   &DataSize                                            // DataSize
                   );
        if (EFI_ERROR (Status)
          || ((sizeof (AMD_USB4_TMU_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32) ) != DataSize))
        {
          IDS_HDT_CONSOLE_USB4_DEBUG ("%a Error reading TMU capabilities\n", __FUNCTION__);
          return EFI_DEVICE_ERROR;
        }

#if 0
        if ( !IsUSB4Router (pRouter) && pRouter->UpstreamAdapter == pAdapter->Header.AdapterNum ) {
          UINT32 AndMask = (UINT32)(~0xC);
          UINT32 OrValue = (UINT32)0xC;
          Status = AmdUsb4Ring0RMWCS (
                     pCM,
                     pAdapter->Header.TopologyID,
                     CS_TARGET_ADAPTER,
                     pAdapter->Header.AdapterNum,
                     pAdapter->TMUCapability + 6,                           // ReadAddress
                     1,                                                     // ReadSize 1 DWs
                     &AndMask,                                              // AndMask
                     &OrValue                                               // OrValue
                     );
          if ( EFI_ERROR (Status) ) {
            IDS_HDT_CONSOLE_USB4_DEBUG ("%a Error Set TMU capabilities + 6\n", __FUNCTION__);
            return EFI_DEVICE_ERROR;
          }
        }
#endif

        Tbt3RouterUpdateUDM (pRouter, AdapterNum, TmuAdapterCapability->TmuAdpCs3.Field.EnableUniDirectionalMode);
        break;

      case LANE_ADAPTER_CONFIG_CAP_ID:
        //
        // i.Save LaneAdapterConfigurationAddress
        //
        IDS_HDT_CONSOLE_USB4_DEBUG ("Get Lane Capability (0x%x)\n", CapabilityPointer);
        pAdapter->LaneCapability = CapabilityPointer;

        do {
          IDS_HDT_CONSOLE_USB4_DEBUG ("     Check Lane Capability...\n");
          MicroSecondDelay (1000);   //1ms
          DataSize = sizeof (AMD_USB4_LANE_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32);
          Status = AmdUsb4Ring0ReadCS (
                     pCM,
                     pRouter->Header.TopologyID,
                     CS_TARGET_ADAPTER,                                   // CsType
                     AdapterNum,                                          // AdapterNum
                     pAdapter->LaneCapability,                            // ReadAddress
                     DataSize,                                            // ReadSize
                     (UINT32**)&LaneAdapterCapability,                    // DataOut
                     &DataSize                                            // DataSize
                     );
          if (EFI_ERROR (Status)
            || ((sizeof (AMD_USB4_LANE_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32) ) != DataSize))
          {
            IDS_HDT_CONSOLE_USB4_ERROR ("%a Error reading Lane capabilities\n", __FUNCTION__);
            return EFI_DEVICE_ERROR;
          }
        } while (LaneAdapterCapability->LaneAdpCs1.Field.AdapterState == 1); //looping if it's in Training state
        pAdapter->LaneAdp_SupportSpeed  = LaneAdapterCapability->LaneAdpCs0.Field.SupportedLinkSpeeds;
        pAdapter->LaneAdp_SupportWidth  = LaneAdapterCapability->LaneAdpCs0.Field.SupportedLinkWidths;
        pAdapter->LaneAdp_TargetSpeed   = LaneAdapterCapability->LaneAdpCs1.Field.TargetLinkSpeed;
        pAdapter->LaneAdp_TargetWidth   = LaneAdapterCapability->LaneAdpCs1.Field.TargetLinkWidth;
        pAdapter->LaneAdp_CurrentSpeed  = LaneAdapterCapability->LaneAdpCs1.Field.CurrentLinkSpeed;
        pAdapter->LaneAdp_CurrentWidth  = LaneAdapterCapability->LaneAdpCs1.Field.NegotiatedLinkWidth;
        pAdapter->LaneAdp_LinkDisable   = LaneAdapterCapability->LaneAdpCs1.Field.LinkDisable;
        pAdapter->LaneAdp_LinkBonding   = LaneAdapterCapability->LaneAdpCs1.Field.LaneBonding;
        pAdapter->LaneAdp_AdapterState  = LaneAdapterCapability->LaneAdpCs1.Field.AdapterState;
        IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdpCs0 = 0x%x\n", LaneAdapterCapability->LaneAdpCs0.Value);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdpCs1 = 0x%x\n", LaneAdapterCapability->LaneAdpCs1.Value);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_SupportSpeed = 0x%x\n", pAdapter->LaneAdp_SupportSpeed);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_SupportWidth = 0x%x\n", pAdapter->LaneAdp_SupportWidth);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_TargetSpeed = 0x%x\n", pAdapter->LaneAdp_TargetSpeed);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_TargetWidth = 0x%x\n", pAdapter->LaneAdp_TargetWidth);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_CurrentSpeed = 0x%x\n", pAdapter->LaneAdp_CurrentSpeed);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_CurrentWidth = 0x%x\n", pAdapter->LaneAdp_CurrentWidth);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_LinkDisable = 0x%x\n", pAdapter->LaneAdp_LinkDisable);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_LinkBonding = 0x%x\n", pAdapter->LaneAdp_LinkBonding);
        IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_AdapterState = 0x%x\n", pAdapter->LaneAdp_AdapterState);
        IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");

        break;

      case USB4_PORT_CAP_ID:
        //
        // i.Save USB4PortCapabilityAddress
        //

        IDS_HDT_CONSOLE_USB4_DEBUG ("Get USB4 Capability (0x%x)\n", CapabilityPointer);
        pAdapter->Usb4Capability = CapabilityPointer;

        DataSize = sizeof (AMD_USB4_PORT_CAPABILITY) / sizeof (UINT32);
        Status = AmdUsb4Ring0ReadCS (
                   pCM,
                   pRouter->Header.TopologyID,
                   CS_TARGET_ADAPTER,                                   // CsType
                   AdapterNum,                                          // AdapterNum
                   CapabilityPointer,                                   // ReadAddress
                   DataSize,                                            // ReadSize
                   (UINT32**)&USB4AdapterCapability,                    // DataOut
                   &DataSize                                            // DataSize
                   );
        if (EFI_ERROR (Status) || ((sizeof (AMD_USB4_PORT_CAPABILITY) / sizeof (UINT32) ) != DataSize)) {
          IDS_HDT_CONSOLE_USB4_DEBUG ("%a Error reading USB4 capabilities\n", __FUNCTION__);
          return EFI_DEVICE_ERROR;
        }
        pAdapter->Usb4Port_CableVersion  = USB4AdapterCapability->PortCs18.Field.CableUSB4Version;
        pAdapter->Usb4Port_BE            = USB4AdapterCapability->PortCs18.Field.BondingEnabled;
        pAdapter->Usb4Port_TCM           = USB4AdapterCapability->PortCs18.Field.TBT3CompatibleMode;
        pAdapter->Usb4Port_LCL           = USB4AdapterCapability->PortCs18.Field.LinkCLxSupport;
        pAdapter->Usb4Port_RE2           = USB4AdapterCapability->PortCs18.Field.RsFecEnabledGen2;
        pAdapter->Usb4Port_RE3           = USB4AdapterCapability->PortCs18.Field.RsFecEnabledGen3;
        pAdapter->Usb4Port_RD            = USB4AdapterCapability->PortCs18.Field.RouterDetected;
        IDS_HDT_CONSOLE_USB4_DEBUG ("Usb4Port_RE2 = 0x%x\n", pAdapter->Usb4Port_RE2);
        IDS_HDT_CONSOLE_USB4_DEBUG ("Usb4Port_RE3 = 0x%x\n", pAdapter->Usb4Port_RE3);
        break;

      default:
        IDS_HDT_CONSOLE_USB4_DEBUG ("The CapabilityID = 0x%x is not belong to Lane Adapter. Skip it.\n", CapabilityID);
        break;
    }

    CapabilityPointer = NextCapabilityPointer;
  };

  //
  // Update DFPAmount
  //
  if ( IsUSB4Router (pRouter) ) {
    if ((pRouter->IsHost) || (pAdapter->Header.AdapterNum > (pRouter->UpstreamAdapter + 1))) {
      //
      // skip Lane1
      //
      if (pAdapter->Usb4Capability != 0) {
        pRouter->DFPAmount++;
        IDS_HDT_CONSOLE_USB4_DEBUG ("pRouter[0x%lx]->DFPAmount = %d\n", pRouter->Header.TopologyID, pRouter->DFPAmount);
      } else {
        // Do DFP Link Configure after Lane1 enumeration
        Status = AmdUsb4PortLinkConfigure (
                   pCM,
                   (AMD_USB4_CM_LANE_ADAPTER *) ((UINTN)pAdapter - sizeof (AMD_USB4_CM_CELL))
                   );
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_USB4_DEBUG ("%a Error on Link Configuration!\n", __FUNCTION__);
        }
      }
    }
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Handle TBT3 router Lane Adapter later.\n");
  }

  //
  // Flow Control
  // Determine number of buffers reserved for the Control Path
  //
  DataSize = sizeof (AMD_USB4_PATH_CS_0) / sizeof (UINT32);
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_PATH,                        // CsType
             AdapterNum,                            // AdapterNum
             0,                                     // ReadAddress
             DataSize,                              // ReadSize
             (UINT32**)&ControlPath,                // DataOut
             &DataSize                              // DataSize
             );
  if ((EFI_ERROR (Status)) || ((sizeof (AMD_USB4_PATH_CS_0) / sizeof (UINT32) ) != DataSize)) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Failed to read AMD_USB4_PATH_CS_0 Register.\n");
    return EFI_DEVICE_ERROR;
  }
  pAdapter->ControlPathCredits = ControlPath->Field.PathCreditsAllocated;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Flow Control\n", __FUNCTION__);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  TotalBuffer        = 0x%lx\n", pAdapter->TotalBuffer);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  ControlPathCredits = 0x%lx\n", pAdapter->ControlPathCredits);

  //
  // Calculate the number of remaining buffers
  //
  if (pAdapter->TotalBuffer >= (UINT32)(pAdapter->ControlPathCredits)) {
    pAdapter->RemainingBuffers = (UINT32)(pAdapter->TotalBuffer - pAdapter->ControlPathCredits);
  } else {
    IDS_HDT_CONSOLE_USB4_ERROR (" Can not allocate minimum buffers.\n");
    return RETURN_BUFFER_TOO_SMALL;
  }
  IDS_HDT_CONSOLE_USB4_DEBUG ("  RemainingBuffers   = 0x%lx\n", pAdapter->RemainingBuffers);

  //
  // Bandwidth
  //
  if ((pAdapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
    && (pAdapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2))
  {
    pAdapter->RawBandwidth = USB4_RAW_BANDWIDTH_40GBPS;
  } else if ((pAdapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
    && (pAdapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1))
  {
    pAdapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
  } else if ((pAdapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
    && (pAdapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2))
  {
    pAdapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
  } else if ((pAdapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
    && (pAdapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1))
  {
    pAdapter->RawBandwidth = USB4_RAW_BANDWIDTH_10GBPS;
  }
  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Bandwidth\n", __FUNCTION__);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  Raw Bandwidth                   = %d\n", pAdapter->RawBandwidth);

  //
  // Values for Bandwidth have a scaling factor of USB4_RAW_BANDWIDTH_SCALING factored in
  //
  pAdapter->AvailableBandwidth              = (pAdapter->RawBandwidth * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                                              GUARD_BAND_BANDWIDTH_DENOMINATOR;
  pAdapter->TotalDownstreamDpPathBandwidth  = 0;
  pAdapter->DownstreamDpPath1Bandwidth      = 0;
  pAdapter->DownstreamDpPath2Bandwidth      = 0;
  pAdapter->TotalUpstreamDpPathBandwidth    = 0;
  pAdapter->UpstreamDpPath1Bandwidth        = 0;
  pAdapter->UpstreamDpPath2Bandwidth        = 0;
  pAdapter->DownstreamUsb3PathBandwidth     = 0;
  pAdapter->UpstreamUsb3PathBandwidth       = 0;

#if AMD_USB4_DEBUG_LANE_HOTPLUG_DISABLE
  // Set Hot plug to disable
  DisableHotplugEvent (
    pCM,
    pRouter->Header.TopologyID,
    AdapterNum
    );
#endif

  IDS_HDT_CONSOLE_USB4_DEBUG ("  Available Bandwidth    = %d\n", pAdapter->AvailableBandwidth);

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
* Enumerate all adapters in router
*
* @param[in] *pCM                 Pointer to Connection Manager structure
* @param[in] *pRouter              Pointer to router configuration space structure
*
* @retval EFI_SUCCESS             Adapters enumerated successfully.
* @retval EFI_DEVICE_ERROR        Error reading configuration space.
* @retval EFI_OUT_OF_RESOURCES    No resources.
**/

EFI_STATUS
Usb4EnumerateAllAdapters (
  IN     AMD_USB4_CM         *pCM,
  IN     AMD_USB4_CM_ROUTER  *pRouter
  )
{
  EFI_STATUS                            Status;
  UINT32                                AdapterNumber;
  UINT32                                DataSize;
  AMD_USB4_ADAPTER_CONFIGURATION_SPACE  *pAdapterCs;

  Status        = EFI_SUCCESS;
  AdapterNumber = 0;
  DataSize      = 0;
  pAdapterCs    = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "INFO: Enumerate Adapters under Router with TopologyID [0x%lx]\n",
    pRouter->Header.TopologyID
    );

  //
  // Set up default Adapter data
  //
  if ( MAX_CM_DATA_ARRAY_AMOUNT <= ( pCM->CmDataIndex + (UINT32)(pRouter->MaxAdapter + 1) ) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("WARNING: We have run out of CM data array. Quit enumerate adapters\n");
    return EFI_OUT_OF_RESOURCES;
  } else {
    pRouter->pAdapter = &(pCM->PtrCmData[pCM->CmDataIndex]);
    //no need SetMem (pRouter->pAdapter, sizeof (AMD_USB4_CM_CELL) * MAX_ADAPTER_AMOUNT_IN_ROUTER, 0);
    for (AdapterNumber = 0; AdapterNumber <= pRouter->MaxAdapter; AdapterNumber++) {
      pRouter->pAdapter[AdapterNumber].Header.CellType    = CELL_TYPE_UNUSED_ADAPTER;
      pRouter->pAdapter[AdapterNumber].Header.AdapterNum  = (UINT8)AdapterNumber;
      pRouter->pAdapter[AdapterNumber].Header.Parent      = (AMD_USB4_CM_CELL_HEADER*)pRouter;
      pRouter->pAdapter[AdapterNumber].Header.TopologyID  = pRouter->Header.TopologyID;
    }
    // Adapter[0] is Control Adapter
    pRouter->pAdapter[0].Header.CellType    = CELL_TYPE_CONTROL_ADAPTER;
  }

  //
  // a.Loop for Adapter # (<= Max Adapter#)
  //
  AdapterNumber = ( IsUSB4Router (pRouter) ) ? 1 : 0; // Start from adapter 0 if it is TBT3 router
  for (AdapterNumber = 1; AdapterNumber <= (UINT32)pRouter->MaxAdapter; AdapterNumber++) {
    IDS_HDT_CONSOLE_USB4_INFO ("Check DROM unused adapter entries for this adapter (%d).\n", AdapterNumber);
    if ( IsDROMUnusedAdapter (pRouter, AdapterNumber) ) {
      IDS_HDT_CONSOLE_USB4_INFO ("DROM shows this Adapter (%d) is unused. Skip this Adapter.\n", AdapterNumber);
      continue;
    } else {
      IDS_HDT_CONSOLE_USB4_INFO ("Adapter (%d) is not in DROM unused adapter entries.\n", AdapterNumber);
    }

    IDS_HDT_CONSOLE (MAIN_FLOW, "INFO: Try to get Adapter[%d] 9 DWs of Adapter CS\n", AdapterNumber);
    //
    // b. Read Request, Base Adapter CS registers
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               AdapterNumber,
               0,
               ADAPTER_BASE_CS_SIZE_DW,
               (UINT32**)&pAdapterCs,
               &DataSize
               );

    if ( EFI_SUCCESS == Status ) {
      if ( ADAPTER_BASE_CS_SIZE_DW != DataSize ) {
        IDS_HDT_CONSOLE_USB4_DEBUG (" Only get %d DWs of Adapter CS\n", DataSize);
        //do we need clear all read
        SetMem (pRouter->pAdapter, sizeof (AMD_USB4_CM_CELL) * (pRouter->MaxAdapter + 1), 0);
        pRouter->pAdapter = NULL;
        return EFI_DEVICE_ERROR;
      } else {
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "INFO: Get Adapter[%d] 9 DWs of Adapter CS\n",
          pAdapterCs->AdpCs3.Field.AdapterNumber
          );
      }
    } else if ( EFI_DEVICE_ERROR == Status ) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("WARNING: error code: %d \n", DataSize);
      pRouter->pAdapter[AdapterNumber].Header.CellType = CELL_TYPE_UNUSED_ADAPTER;
      //
      // Unsupported Adapter if ERR_ADDR
      //
      continue;
    } else {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "ERROR: Read CS from TopologyID[0x%lx].Adapter[%d] failed\n",
        pRouter->Header.TopologyID,
        AdapterNumber
        );
      SetMem (pRouter->pAdapter, sizeof (AMD_USB4_CM_CELL) * (pRouter->MaxAdapter + 1), 0);
      pRouter->pAdapter = NULL;
      return EFI_DEVICE_ERROR;
    }

#if AMD_USB4_DEBUG_LANE_ADAPTER_SKIP
    if ( ( (*(UINT32*)&(pAdapterCs->AdapterTypeSubType)) & 0xFFFFFF ) == LaneAdapter ) {
      *(UINT32*)&(pAdapterCs->AdapterTypeSubType) = UnsupportedAdapter;
      IDS_HDT_CONSOLE_USB4_DEBUG ("Skip Lane Adapter %d\n", AdapterNumber);
      continue;
    }
#endif

#if AMD_USB4_DEBUG_USB3_ADAPTER_SKIP
    if ( ((*(UINT32*)&(pAdapterCs->AdapterTypeSubType))&0xFFFFFF) == DownStreamUSB3Adapter
      || ((*(UINT32*)&(pAdapterCs->AdapterTypeSubType))&0xFFFFFF) == UpStreamUSB3Adapter ) {
      *(UINT32*)&(pAdapterCs->AdapterTypeSubType) = UnsupportedAdapter;
      IDS_HDT_CONSOLE_USB4_DEBUG ("Skip Usb3 Adapter %d\n", AdapterNumber);
      continue;
    }
#endif

#if AMD_USB4_DEBUG_PCIE_ADAPTER_SKIP
    if ( ((*(UINT32*)&(pAdapterCs->AdapterTypeSubType))&0xFFFFFF) == DownStreamPCIeAdapter
      || ((*(UINT32*)&(pAdapterCs->AdapterTypeSubType))&0xFFFFFF) == UpStreamPCIeAdapter ) {
      *(UINT32*)&(pAdapterCs->AdapterTypeSubType) = UnsupportedAdapter;
      IDS_HDT_CONSOLE_USB4_DEBUG ("Skip Pcie Adapter %d\n", AdapterNumber);
      continue;
    }
#endif

#if AMD_USB4_DEBUG_DPIN_ADAPTER_SKIP
    if ( ( (*(UINT32*)&(pAdapterCs->AdapterTypeSubType)) & 0xFFFFFF ) == DPInAdapter ) {
      *(UINT32*)&(pAdapterCs->AdapterTypeSubType) = UnsupportedAdapter;
      IDS_HDT_CONSOLE_USB4_DEBUG ("Skip DP In Adapter %d\n", AdapterNumber);
      continue;
    }
#endif

#if AMD_USB4_DEBUG_DPOUT_ADAPTER_SKIP
    if ( ( (*(UINT32*)&(pAdapterCs->AdapterTypeSubType)) & 0xFFFFFF ) == DPOutAdapter ) {
      *(UINT32*)&(pAdapterCs->AdapterTypeSubType) = UnsupportedAdapter;
      IDS_HDT_CONSOLE_USB4_DEBUG ("Skip DP Out Adapter %d\n", AdapterNumber);
      continue;
    }
#endif

    //
    // d. Enumerate for each type of Adapter
    // i. Unsupported Adapter -> skip and continue loop
    //
    IDS_HDT_CONSOLE_USB4_INFO (" Adapter Type = 0x%x\n", *(UINT32*)&(pAdapterCs->AdapterTypeSubType));
    switch (( *(UINT32*)&(pAdapterCs->AdapterTypeSubType) ) & 0xFFFFFF ) {
      case LaneAdapter:
        IDS_HDT_CONSOLE_USB4_INFO (" Find Lane Adapter (AdpNum: %d)\n", AdapterNumber);
        Status = EnumerateLaneAdapter (
                   pCM,
                   AdapterNumber,
                   pRouter,
                   (AMD_USB4_CM_LANE_ADAPTER*)&(pRouter->pAdapter[AdapterNumber]),
                   pAdapterCs
                   );
        if ( EFI_ERROR (Status) ) {
          IDS_HDT_CONSOLE_USB4_ERROR (" EnumerateLaneAdapter has error. AdapterNumber: %d\n", AdapterNumber);
          ((AMD_USB4_CM_LANE_ADAPTER*)&(pRouter->pAdapter[AdapterNumber]))->Header.CellType = CELL_TYPE_UNUSED_ADAPTER;
        }
        break;
      case DownStreamPCIeAdapter:
      case UpStreamPCIeAdapter:
        IDS_HDT_CONSOLE_USB4_INFO (" Find PCIe Adapter (AdpNum: %d)\n", AdapterNumber);
        Status = EnumeratePcieAdapter (
                   pCM,
                   AdapterNumber,
                   pRouter,
                   (AMD_USB4_CM_PCIE_ADAPTER*)&(pRouter->pAdapter[AdapterNumber]),
                   pAdapterCs
                   );
        if ( EFI_ERROR (Status) ) {
          IDS_HDT_CONSOLE_USB4_ERROR (" EnumeratePcieAdapter has error. AdapterNumber: %d\n", AdapterNumber);
          ((AMD_USB4_CM_PCIE_ADAPTER*)&(pRouter->pAdapter[AdapterNumber]))->Header.CellType = CELL_TYPE_UNUSED_ADAPTER;
        }
        break;
      case DPOutAdapter:
      case DPInAdapter:
        IDS_HDT_CONSOLE_USB4_INFO (" Find DP Adapter (AdpNum: %d)\n", AdapterNumber);
        Status = EnumerateDpAdapter (
                   pCM,
                   AdapterNumber,
                   pRouter,
                   (AMD_USB4_CM_DP_ADAPTER*)&(pRouter->pAdapter[AdapterNumber]),
                   pAdapterCs
                   );
        if ( EFI_ERROR (Status) ) {
          IDS_HDT_CONSOLE_USB4_ERROR (" EnumerateDpInAdapter has error. AdapterNumber: %d\n", AdapterNumber);
          ((AMD_USB4_CM_DP_ADAPTER*)&(pRouter->pAdapter[AdapterNumber]))->Header.CellType = CELL_TYPE_UNUSED_ADAPTER;
        } else {
          // Set HopID for Tbt3 Router DP adapter.
          IDS_HDT_CONSOLE_USB4_DEBUG (
            "Set HopID for Tbt3 Router (0x%lx) AdapterNumber (%d)\n",
            pRouter->Header.TopologyID,
            AdapterNumber
            );
          Status = Tbt3SetDpAdapterHopId (
                     pCM,
                     pRouter,
                     (AMD_USB4_CM_DP_ADAPTER*)&(pRouter->pAdapter[AdapterNumber])
                     );
          IDS_HDT_CONSOLE_USB4_DEBUG (
            "Set HopID for Tbt3 Router (0x%lx) AdapterNumber (%d) Result: %r\n",
            pRouter->Header.TopologyID,
            AdapterNumber,
            Status
            );
        }
        break;
      case DownStreamUSB3Adapter:
      case UpStreamUSB3Adapter:
        IDS_HDT_CONSOLE_USB4_INFO (" Find USB3 Adapter (AdpNum: %d)\n", AdapterNumber);
        if ( IsUSB4Router (pRouter) ) {
          Status = EnumerateUsb3Adapter (
                     pCM,
                     AdapterNumber,
                     pRouter,
                     (AMD_USB4_CM_USB3_ADAPTER*)&(pRouter->pAdapter[AdapterNumber]),
                     pAdapterCs
                     );
          if ( EFI_ERROR (Status) ) {
            IDS_HDT_CONSOLE_USB4_ERROR (" EnumerateUsb3Adapter has error. AdapterNumber: %d\n", AdapterNumber);
            ((AMD_USB4_CM_USB3_ADAPTER*)&(pRouter->pAdapter[AdapterNumber]))->Header.CellType = CELL_TYPE_UNUSED_ADAPTER;
          }
        } else {
          IDS_HDT_CONSOLE_USB4_ERROR (" TBT3 router do not support USB3 adapter. AdapterNumber: %d\n", AdapterNumber);
          ((AMD_USB4_CM_USB3_ADAPTER*)&(pRouter->pAdapter[AdapterNumber]))->Header.CellType = CELL_TYPE_UNUSED_ADAPTER;
        }
        break;
      case HostInterfaceAdapter:
        IDS_HDT_CONSOLE_USB4_INFO (
          " Find Host Interface Adapter. AdapterNumber = 0x%x, MaxInputHopID=0x%x, MaxOutputHopID=0x%x\n",
          AdapterNumber,
          pAdapterCs->AdpCs5.Field.MaxInputHopID,
          pAdapterCs->AdpCs5.Field.MaxOutputHopID
          );
        pRouter->pAdapter[AdapterNumber].Header.CellType = CELL_TYPE_HI_ADAPTER;
        if ( !IsUSB4Router (pRouter) ) {
          Status = Tbt3EnumerateHIAdapter (
                     pCM,
                     AdapterNumber,
                     pRouter,
                     (AMD_USB4_CM_HI_ADAPTER*)&(pRouter->pAdapter[AdapterNumber]),
                     pAdapterCs
                     );
        }
        break;
      case UnsupportedAdapter:
        IDS_HDT_CONSOLE_USB4_INFO (
          " Find Unsupported Adapter. AdapterNumber = 0x%x, MaxInputHopID=0x%x, MaxOutputHopID=0x%x\n",
          AdapterNumber,
          pAdapterCs->AdpCs5.Field.MaxInputHopID,
          pAdapterCs->AdpCs5.Field.MaxOutputHopID
          );
        pRouter->pAdapter[AdapterNumber].Header.CellType = CELL_TYPE_UNUSED_ADAPTER;
        break;
      default:
        IDS_HDT_CONSOLE_USB4_INFO (
          " Find unknown Adapter. AdapterNumber = 0x%x, MaxInputHopID=0x%x, MaxOutputHopID=0x%x\n",
          AdapterNumber,
          pAdapterCs->AdpCs5.Field.MaxInputHopID,
          pAdapterCs->AdpCs5.Field.MaxOutputHopID
          );
        pRouter->pAdapter[AdapterNumber].Header.CellType = CELL_TYPE_UNUSED_ADAPTER;
        break;
    }
  }

  // Update CM Data Index
  pCM->CmDataIndex += pRouter->MaxAdapter + 1;

  // Enable TBT3 Lane Adapter in Router VSEC6
  Tbt3LaneAdapterEnable (pCM, pRouter);

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
* Get first Adapter pointer of the specific type
*
* @param[in] *pCM                 Pointer to Connection Manager structure
* @param[in] *pRouter             Pointer to Connection Manager router structure
* @param[in] CellType             Type of Adapter
*
* @retval NULL                    Get Nothing.
* @retval AMD_USB4_CM_CELL*       Pointer of first Adapter structure.
**/

AMD_USB4_CM_CELL*
GetFirstAdapterPointer (
  IN     AMD_USB4_CM           *pCM,
  IN     AMD_USB4_CM_ROUTER    *pRouter,
  IN     AMD_USB4_CM_CELL_TYPE CellType
  )
{
  UINT32 i;

  for (i = 0; i <= pRouter->MaxAdapter; i++) {
    if ( pRouter->pAdapter[i].Header.CellType == CellType ) {
      return &(pRouter->pAdapter[i]);
    }
  }

  return NULL;
}


VOID
PrintAllAdapters (
  IN     AMD_USB4_CM         *pCM,
  IN     AMD_USB4_CM_ROUTER  *pRouter
  )
{
  UINT32  i;

  i = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a:\n", __FUNCTION__);
  IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================================\n");
  IDS_HDT_CONSOLE_USB4_DEBUG (
    " Router (0x%x): TopologyID=0x%lx Depth=%d, Max Adapter=%d\n",
    pRouter,
    pRouter->Header.TopologyID,
    pRouter->Depth,
    pRouter->MaxAdapter);
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "\n   pAdapter (0x%x)\n"
    "   pAdapterUsb3Up (0x%x)\n   pAdapterUsb3Down (0x%x)\n"
    "   pAdapterPcieUp (0x%x)\n   pAdapterPcieDown (0x%x)\n"
    "   pAdapterDpIn (0x%x)\n   pAdapterDpOut (0x%x)\n"
    "   pAdapterUFP (0x%x)\n   pAdapterDFP (0x%x)"
    "   DFPAmount (%d)\n",
    pRouter->pAdapter,
    pRouter->pAdapterUsb3Up,
    pRouter->pAdapterUsb3Down,
    pRouter->pAdapterPcieUp,
    pRouter->pAdapterPcieDown,
    pRouter->pAdapterDpIn,
    pRouter->pAdapterDpOut,
    pRouter->pAdapterUFP,
    pRouter->pAdapterDFP,
    pRouter->DFPAmount
    );
  IDS_HDT_CONSOLE_USB4_DEBUG ("----------------------------------------------------------------------\n");
  for (i=1; i<=pRouter->MaxAdapter; i++) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("CellType: #0x%x\n", pRouter->pAdapter[i].Header.CellType);
    switch (pRouter->pAdapter[i].Header.CellType) {
      case CELL_TYPE_DP_IN_ADAPTER:
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "DP_IN (0x%x): #%d, Sibling: 0x%x\n",
          &(pRouter->pAdapter[i]),
          pRouter->pAdapter[i].Header.AdapterNum,
          pRouter->pAdapter[i].DPAdapter.AdpHeader.pSiblingAdapter
          );
        break;
      case CELL_TYPE_DP_OUT_ADAPTER:
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "DP_OUT (0x%x): #%d, Sibling: 0x%x\n",
          &(pRouter->pAdapter[i]),
          pRouter->pAdapter[i].Header.AdapterNum,
          pRouter->pAdapter[i].DPAdapter.AdpHeader.pSiblingAdapter
          );
        break;
      case CELL_TYPE_USB3_UP_ADAPTER:
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "USB3_UP (0x%x): #%d, Link Partner: 0x%x, Sibling: 0x%x\n",
          &(pRouter->pAdapter[i]),
          pRouter->pAdapter[i].Header.AdapterNum,
          pRouter->pAdapter[i].USB3Adapter.AdpHeader.pLinkPartner,
          pRouter->pAdapter[i].USB3Adapter.AdpHeader.pSiblingAdapter
          );
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "   USB3 Capability: 0x%x\n",
          pRouter->pAdapter[i].USB3Adapter.USB3Capability
          );
        break;
      case CELL_TYPE_USB3_DOWN_ADAPTER:
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "USB3_DOWN (0x%x): #%d, Link Partner: 0x%x, Sibling: 0x%x\n",
          &(pRouter->pAdapter[i]),
          pRouter->pAdapter[i].Header.AdapterNum,
          pRouter->pAdapter[i].USB3Adapter.AdpHeader.pLinkPartner,
          pRouter->pAdapter[i].USB3Adapter.AdpHeader.pSiblingAdapter
          );
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "   USB3 Capability: 0x%x\n",
          pRouter->pAdapter[i].USB3Adapter.USB3Capability
          );
        break;
      case CELL_TYPE_PCIE_UP_ADAPTER:
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "PCIE_UP (0x%x): #%d, Link Partner: 0x%x, Sibling: 0x%x\n",
          &(pRouter->pAdapter[i]),
          pRouter->pAdapter[i].Header.AdapterNum,
          pRouter->pAdapter[i].PCIEAdapter.AdpHeader.pLinkPartner,
          pRouter->pAdapter[i].PCIEAdapter.AdpHeader.pSiblingAdapter
          );
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "   PCIe Capability: 0x%x\n",
          pRouter->pAdapter[i].PCIEAdapter.PcieCapability
          );
        break;
      case CELL_TYPE_PCIE_DOWN_ADAPTER:
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "PCIE_DOWN (0x%x): #%d, Link Partner: 0x%x, Sibling: 0x%x\n",
          &(pRouter->pAdapter[i]),
          pRouter->pAdapter[i].Header.AdapterNum,
          pRouter->pAdapter[i].PCIEAdapter.AdpHeader.pLinkPartner,
          pRouter->pAdapter[i].PCIEAdapter.AdpHeader.pSiblingAdapter
          );
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "   PCIe Capability: 0x%x\n",
          pRouter->pAdapter[i].PCIEAdapter.PcieCapability
          );
        break;
      case CELL_TYPE_LANE_ADAPTER:
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "LANE (0x%x): #%d, Link Partner: 0x%x, Sibling: 0x%x\n",
          &(pRouter->pAdapter[i]),
          pRouter->pAdapter[i].Header.AdapterNum,
          pRouter->pAdapter[i].LaneAdapter.AdpHeader.pLinkPartner,
          pRouter->pAdapter[i].LaneAdapter.AdpHeader.pSiblingAdapter
          );
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "   TMU Capability: 0x%x, Lane Capability: 0x%x, Usb4 Capability: 0x%x\n",
          pRouter->pAdapter[i].LaneAdapter.TMUCapability,
          pRouter->pAdapter[i].LaneAdapter.LaneCapability,
          pRouter->pAdapter[i].LaneAdapter.Usb4Capability
          );
        break;
      case CELL_TYPE_HI_ADAPTER:
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "HI (0x%x): #%d, Link Partner: 0x%x, Sibling: 0x%x\n",
          &(pRouter->pAdapter[i]),
          pRouter->pAdapter[i].Header.AdapterNum,
          pRouter->pAdapter[i].PCIEAdapter.AdpHeader.pLinkPartner,
          pRouter->pAdapter[i].PCIEAdapter.AdpHeader.pSiblingAdapter
          );
        break;
      case CELL_TYPE_CONTROL_ADAPTER:
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "CONTROL (0x%x): #%d, Link Partner: 0x%x, Sibling: 0x%x\n",
          &(pRouter->pAdapter[i]),
          pRouter->pAdapter[i].Header.AdapterNum,
          pRouter->pAdapter[i].PCIEAdapter.AdpHeader.pLinkPartner,
          pRouter->pAdapter[i].PCIEAdapter.AdpHeader.pSiblingAdapter
          );
        break;
      case CELL_TYPE_UNUSED_ADAPTER:
      case CELL_TYPE_HOST_ROUTER:
      case CELL_TYPE_DEVICE_ROUTER:
      default:
        IDS_HDT_CONSOLE_USB4_DEBUG (
          "UNKNOWN (0x%x): #%d, Link Partner: 0x%x, Sibling: 0x%x\n",
          &(pRouter->pAdapter[i]),
          pRouter->pAdapter[i].Header.AdapterNum,
          pRouter->pAdapter[i].PCIEAdapter.AdpHeader.pLinkPartner,
          pRouter->pAdapter[i].PCIEAdapter.AdpHeader.pSiblingAdapter
          );
        break;
    }
    IDS_HDT_CONSOLE_USB4_DEBUG ("----------------------------------------------------------------------\n");
  }
}


/*----------------------------------------------------------------------------------------*/
/**
* Link Adapters after enumerate all adapters
*
* @param[in] *pCM                 Pointer to Connection Manager structure
* @param[in] *pRouter             Pointer to router configuration space structure
*
* @retval EFI_SUCCESS             Adapters enumerated successfully.
* @retval EFI_INVALID_PARAMETER   Error reading configuration space.
**/

EFI_STATUS
Usb4LinkAdapters (
  IN     AMD_USB4_CM         *pCM,
  IN     AMD_USB4_CM_ROUTER  *pRouter
  )
{
  EFI_STATUS        Status;
  UINT32            i;
  UINT32            UFPAmount;
  AMD_USB4_CM_CELL  *pAdapterUsb3Up;
  AMD_USB4_CM_CELL  *pAdapterUsb3Down;
  AMD_USB4_CM_CELL  *pAdapterPcieUp;
  AMD_USB4_CM_CELL  *pAdapterPcieDown;
  AMD_USB4_CM_CELL  *pAdapterDpIn;
  AMD_USB4_CM_CELL  *pAdapterDpOut;
  AMD_USB4_CM_CELL  *pAdapterUFP;
  AMD_USB4_CM_CELL  *pAdapterDFP;

  Status            = EFI_SUCCESS;
  pAdapterUsb3Up    = NULL;
  pAdapterUsb3Down  = NULL;
  pAdapterPcieUp    = NULL;
  pAdapterPcieDown  = NULL;
  pAdapterDpIn      = NULL;
  pAdapterDpOut     = NULL;
  pAdapterUFP       = NULL;
  pAdapterDFP       = NULL;
  UFPAmount         = 0;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Start\n", __FUNCTION__);

  if ( NULL == pRouter || NULL == pCM ) {
    IDS_HDT_CONSOLE_USB4_ERROR (" One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  //
  // Scan all the adapters and link sibling adapter.
  //
  for (i = 1; i <= pRouter->MaxAdapter; i++) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Adapter #%d, CellType 0x%x\n", i, pRouter->pAdapter[i].Header.CellType);
    switch (pRouter->pAdapter[i].Header.CellType) {
      case CELL_TYPE_DP_IN_ADAPTER:
        if ( pAdapterDpIn ) {
          pAdapterDpIn->DPAdapter.AdpHeader.pSiblingAdapter = &(pRouter->pAdapter[i]);
        } else {
          pRouter->pAdapterDpIn = &(pRouter->pAdapter[i]);
        }
        pAdapterDpIn = &(pRouter->pAdapter[i]);
        pAdapterDpIn->DPAdapter.AdpHeader.pSiblingAdapter = NULL;
        break;
      case CELL_TYPE_DP_OUT_ADAPTER:
        if ( pAdapterDpOut ) {
          pAdapterDpOut->DPAdapter.AdpHeader.pSiblingAdapter = &(pRouter->pAdapter[i]);
        } else {
          pRouter->pAdapterDpOut = &(pRouter->pAdapter[i]);
        }
        pAdapterDpOut = &(pRouter->pAdapter[i]);
        pAdapterDpOut->DPAdapter.AdpHeader.pSiblingAdapter = NULL;
        break;
      case CELL_TYPE_USB3_UP_ADAPTER:
        if ( pAdapterUsb3Up ) {
          pAdapterUsb3Up->USB3Adapter.AdpHeader.pSiblingAdapter = &(pRouter->pAdapter[i]);
        } else {
          pRouter->pAdapterUsb3Up = &(pRouter->pAdapter[i]);
        }
        pAdapterUsb3Up = &(pRouter->pAdapter[i]);
        pAdapterUsb3Up->USB3Adapter.AdpHeader.pSiblingAdapter = NULL;
        break;
      case CELL_TYPE_USB3_DOWN_ADAPTER:
        if ( pAdapterUsb3Down ) {
          pAdapterUsb3Down->USB3Adapter.AdpHeader.pSiblingAdapter = &(pRouter->pAdapter[i]);
        } else {
          pRouter->pAdapterUsb3Down = &(pRouter->pAdapter[i]);
        }
        pAdapterUsb3Down = &(pRouter->pAdapter[i]);
        pAdapterUsb3Down->USB3Adapter.AdpHeader.pSiblingAdapter = NULL;
        break;
      case CELL_TYPE_PCIE_UP_ADAPTER:
        if ( pAdapterPcieUp ) {
          pAdapterPcieUp->PCIEAdapter.AdpHeader.pSiblingAdapter = &(pRouter->pAdapter[i]);
        } else {
          pRouter->pAdapterPcieUp = &(pRouter->pAdapter[i]);
        }
        pAdapterPcieUp = &(pRouter->pAdapter[i]);
        pAdapterPcieUp->PCIEAdapter.AdpHeader.pSiblingAdapter = NULL;
        break;
      case CELL_TYPE_PCIE_DOWN_ADAPTER:
        if ( pAdapterPcieDown ) {
          pAdapterPcieDown->PCIEAdapter.AdpHeader.pSiblingAdapter = &(pRouter->pAdapter[i]);
        } else {
          pRouter->pAdapterPcieDown = &(pRouter->pAdapter[i]);
        }
        pAdapterPcieDown = &(pRouter->pAdapter[i]);
        pAdapterPcieDown->PCIEAdapter.AdpHeader.pSiblingAdapter = NULL;
        break;
      case CELL_TYPE_LANE_ADAPTER:
        if ( !IsUSB4Router (pRouter) ) {
          // TBT3 router Lane adpater do not have Usb4 Capability, but we still use it as Lane 0 adapter.
          // Because DROM TBT3-Lane Adapter Entry L1A shows which lane is lane 0.
          if ( pRouter->pAdapter[i].LaneAdapter.Usb4Capability != 0 ) {
            // We assume the Lane Adapter which has same adapter number as Router Upstream adapter
            // is the UFP Lane 0.
            if ( pRouter->pAdapter[i].LaneAdapter.Header.AdapterNum == pRouter->UpstreamAdapter ) {
              pRouter->pAdapterUFP = &(pRouter->pAdapter[i]);
              pAdapterUFP = &(pRouter->pAdapter[i]);
              pRouter->pAdapter[i].LaneAdapter.AdpHeader.pSiblingAdapter = NULL;
              UFPAmount++;
            } else {
              if ( pAdapterDFP ) {
                pAdapterDFP->LaneAdapter.AdpHeader.pSiblingAdapter = &(pRouter->pAdapter[i]);
              } else {
                pRouter->pAdapterDFP = &(pRouter->pAdapter[i]);
              }
              pAdapterDFP = &(pRouter->pAdapter[i]);
              pAdapterDFP->LaneAdapter.AdpHeader.pSiblingAdapter = NULL;
            }
          }
        } else {
          if ( pRouter->pAdapter[i].LaneAdapter.Usb4Capability != 0 ) { // Only Lane 0 Adapter has Usb4Capability
            if ( pRouter->IsHost ) {
              // Host Router only have DFP
              if ( pAdapterDFP ) {
                pAdapterDFP->LaneAdapter.AdpHeader.pSiblingAdapter = &(pRouter->pAdapter[i]);
              } else {
                pRouter->pAdapterDFP = &(pRouter->pAdapter[i]);
              }
              pAdapterDFP = &(pRouter->pAdapter[i]);
              pAdapterDFP->LaneAdapter.AdpHeader.pSiblingAdapter = NULL;
            } else {
              // For a Device Router, the Lane 0 Adapter in the Upstream Facing Port shall be assigned
              // the lowest Adapter Number among all Lane Adapters.
              if ( 1 != UFPAmount ) {
                // @todo Verify with TBT3
                // First Lane Adapters should be UFP
                //if ( pAdapterUFP ) {
                //  pAdapterUFP->LaneAdapter.AdpHeader.pSiblingAdapter = &(pRouter->pAdapter[i]);
                //} else {
                if ( pRouter->pAdapter[i].LaneAdapter.Header.AdapterNum != pRouter->UpstreamAdapter ) {
                  IDS_HDT_CONSOLE_USB4_DEBUG (
                    "ERROR: 1st UFP adapter number does not match to Router's upstream adapter number.\n"
                    );
                  IDS_DEADLOOP ();
                }
                pRouter->pAdapterUFP = &(pRouter->pAdapter[i]);
                //}
                pAdapterUFP = &(pRouter->pAdapter[i]);
                pRouter->pAdapter[i].LaneAdapter.AdpHeader.pSiblingAdapter = NULL;
                UFPAmount++;
              } else {
                // It is DFP
                if ( pAdapterDFP ) {
                  pAdapterDFP->LaneAdapter.AdpHeader.pSiblingAdapter = &(pRouter->pAdapter[i]);
                } else {
                  pRouter->pAdapterDFP = &(pRouter->pAdapter[i]);
                }
                pAdapterDFP = &(pRouter->pAdapter[i]);
                pAdapterDFP->LaneAdapter.AdpHeader.pSiblingAdapter = NULL;
              }
            }
          }
        }
        break;
      default:
        break;
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("Link PCIe Adapters to Lane Adapters\n");
  IDS_HDT_CONSOLE_USB4_DEBUG ("pRouter->DFPAmount      = %d\n", pRouter->DFPAmount);
  IDS_HDT_CONSOLE_USB4_DEBUG ("pRouter->PcieDownAmount = %d\n", pRouter->PcieDownAmount);
  IDS_HDT_CONSOLE_USB4_DEBUG ("pRouter->Usb3DownAmount = %d\n", pRouter->Usb3DownAmount);

  //
  // Link PCIe Adapters & USB3 Adapters to Lane Adapters
  //
  // 1. PCIe
  // PCIe protocol adapter amount should be large or equal to DFP amount
  if ( pRouter->PcieDownAmount && pRouter->DFPAmount ) {
    if ( pRouter->PcieDownAmount < pRouter->DFPAmount ) {
      IDS_HDT_CONSOLE_USB4_ERROR (
        " PcieDownAmount: %d, DFPAmount: %d, not pair\n",
        pRouter->PcieDownAmount,
        pRouter->DFPAmount
        );
      //IDS_DEADLOOP ();
    } else {
      pAdapterPcieDown  = pRouter->pAdapterPcieDown;
      pAdapterDFP       = pRouter->pAdapterDFP;
      for (i = 0; i < pRouter->DFPAmount; i++) {
        if ((pAdapterPcieDown != NULL) && (pAdapterDFP != NULL)) {
          pAdapterPcieDown->PCIEAdapter.AdpHeader.pLinkPartner = pAdapterDFP;
          pAdapterPcieDown  = pAdapterPcieDown->PCIEAdapter.AdpHeader.pSiblingAdapter;
          pAdapterDFP       = pAdapterDFP->LaneAdapter.AdpHeader.pSiblingAdapter;
        } else {
          IDS_HDT_CONSOLE_USB4_ERROR (" Invalid pAdapterPcieDown %x, pAdapterDFP %x\n", pAdapterPcieDown, pAdapterDFP);
          IDS_HDT_CONSOLE_USB4_ASSERT (FALSE);
        }
      }
    }
  }else {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Pcie Down Adapter Amount is 0 or DPF Amout is 0.\n");
  }

  if ( pRouter->pAdapterPcieUp ) {
    pRouter->pAdapterPcieUp->PCIEAdapter.AdpHeader.pLinkPartner = pRouter->pAdapterUFP;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("Link USB3 Adapters to Lane Adapters\n");

  // 2. Usb3
  // Usb3 protocol adapter amount should be large or equal to DFP amount
  if ( pRouter->Usb3DownAmount && pRouter->DFPAmount ) {
    if ( pRouter->Usb3DownAmount < pRouter->DFPAmount ) {
      IDS_HDT_CONSOLE_USB4_ERROR (
        " Usb3DownAmount: %d, DFPAmount: %d, not pair\n",
        pRouter->Usb3DownAmount,
        pRouter->DFPAmount
        );
      //IDS_DEADLOOP ();
    } else {
      pAdapterUsb3Down  = pRouter->pAdapterUsb3Down;
      pAdapterDFP       = pRouter->pAdapterDFP;
      for (i = 0; i < pRouter->DFPAmount; i++) {
        if ((pAdapterUsb3Down != NULL) && (pAdapterDFP != NULL)) {
          pAdapterUsb3Down->USB3Adapter.AdpHeader.pLinkPartner = pAdapterDFP;
          pAdapterUsb3Down  = pAdapterUsb3Down->USB3Adapter.AdpHeader.pSiblingAdapter;
          pAdapterDFP       = pAdapterDFP->LaneAdapter.AdpHeader.pSiblingAdapter;
        } else {
          IDS_HDT_CONSOLE_USB4_ERROR (" Invalid pAdapterUsb3Down %x, pAdapterDFP %x\n", pAdapterUsb3Down, pAdapterDFP);
          IDS_HDT_CONSOLE_USB4_ASSERT (FALSE);
        }
      }
    }
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG ("USB3 Down Adapter Amount is 0 or DPF Amout is 0.\n");
  }

  if ( pRouter->pAdapterUsb3Up ) {
    pRouter->pAdapterUsb3Up->USB3Adapter.AdpHeader.pLinkPartner = pRouter->pAdapterUFP;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("call PrintAllAdapters\n");
  if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
    PrintAllAdapters (pCM, pRouter);
  }
  IDS_HDT_CONSOLE_USB4_DEBUG ("exit Usb4LinkAdapters\n");
  return EFI_SUCCESS;
}


/**
 * @brief   Get capability id offset.
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    *pRouter
 * @param   UINT32                CapabilityID
 *
 * @return  UINT32                Zero means not found.
 */
UINT32
GetCapability (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_CELL         *pCell,
  IN     UINT8                    NextCapability,
  IN     UINT8                    FindCapabilityID
  )
{
  EFI_STATUS                            Status;
  UINT32                                DataSize;
  UINT32                                *CapabilityReg;
  UINT8                                 CapabilityPointer;
  UINT8                                 NextCapabilityPointer;
  UINT8                                 CapabilityID;
  UINT8                                 VSCID;
  AMD_USB4_ADAPTER_CONFIGURATION_SPACE  *pAdapterCs;

  Status                = EFI_SUCCESS;
  DataSize              = 0;
  CapabilityReg         = NULL;
  CapabilityPointer     = 0;
  NextCapabilityPointer = 0;
  CapabilityID          = 0;
  VSCID                 = 0;
  pAdapterCs            = NULL;

  CapabilityPointer = NextCapability;
  while ( CapabilityPointer != 0 ) {
    //
    //  Read Request: loop Capability link (Size = 1DW)
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pCell->Header.TopologyID,
               CS_TARGET_ADAPTER,                                   // CsType
               pCell->Header.AdapterNum,                            // AdapterNum
               (UINT32)CapabilityPointer,                           // ReadAddress
               1,                                                   // ReadSize
               (UINT32 **)&CapabilityReg,                           // DataOut
               &DataSize                                            // DataSize
               );
    if (EFI_ERROR (Status) || (DataSize != 1)) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Failed to read capabilities\n");
      return 0;
    }

    NextCapabilityPointer  = (UINT8)(*CapabilityReg);
    CapabilityID  = (UINT8)((*CapabilityReg) >> 8);
    VSCID         = (UINT8)((*CapabilityReg) >> 16);
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "   CapabilityID = 0x%x, NextCapabilityPointer = 0x%x, VSCID = 0x%x\n",
      CapabilityID,
      NextCapabilityPointer,
      VSCID
      );

    if ( CapabilityID == FindCapabilityID ) {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "Find CapabilityID = 0x%x, Offset = 0x%x\n",
        CapabilityID,
        CapabilityPointer
        );
      return (UINT32)CapabilityPointer;
    }

    CapabilityPointer = NextCapabilityPointer;
  }

  return 0;
}

