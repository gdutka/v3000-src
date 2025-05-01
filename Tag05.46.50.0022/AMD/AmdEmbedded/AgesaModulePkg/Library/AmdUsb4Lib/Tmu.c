/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/BaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/TimerLib.h>
#include "AmdUsb4.h"
#include "Library/AmdUsb4Lib.h"
#include <AmdUsb4/ControlPacket.h>
#include <AmdUsb4/ConfigurationSpace.h>

#define FILECODE LIBRARY_AMDUSB4LIB_TMU_FILECODE

/*----------------------------------------------------------------------------------------*/
/**
 * @brief Enable or Disable Router Time Disruption
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pRouter             Point to the Router
 * @param[in] Enable               Enable or Disable Time Disruption
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4TmuTimeDisruption (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pRouter,
  IN     BOOLEAN                    Enable
  )
{
  EFI_STATUS             Status;
  UINT32                 DataSize;
  AMD_USB4_TMU_RTR_CS_0  *pTmuRtrCs0;

  Status           = EFI_SUCCESS;
  DataSize         = 0;
  pTmuRtrCs0       = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if (NULL == pRouter || NULL == pCM) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  // Read TMU_RTR_CS_0
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,                                          // CsType
             0,                                                         // AdapterNum
             (UINT32)(pRouter->TMUCapability),                          // ReadAddress
             1,                                                         // ReadSize
             (UINT32**)&pTmuRtrCs0,                                     // DataOut
             &DataSize                                                  // DataSize
             );
  if ( EFI_ERROR (Status) || ( 1 != DataSize ) ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "Failed to read Router (0x%lx) TMU_RTR_CS_0.\n",
      pRouter->Header.TopologyID
      );
    return EFI_DEVICE_ERROR;
  }

  if (Enable) {
    pTmuRtrCs0->Field.TimeDescription = 1;
  } else {
    pTmuRtrCs0->Field.TimeDescription = 0;
  }

  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,                                    // AdapterNum
             (UINT32) (pRouter->TMUCapability),    // ReadAddress: TMU_RTR_CS_0
             &DataSize,
             &(pTmuRtrCs0->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "Failed to write Router (0x%lx) TMU_RTR_CS_0.\n",
      pRouter->Header.TopologyID
      );
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief Disable Router from sending TMU Packet on the link
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pDownstreamRouter   Point to Downstream Router
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4TmuDisableTimeSync (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pDownstreamRouter
  )
{
  EFI_STATUS        Status;
  UINT32            DataSize;
  AMD_USB4_CM_CELL           *pUsb4Cell;
  AMD_USB4_CM_LANE_ADAPTER   *pParentDfpLane0Adapter;
  AMD_USB4_CM_LANE_ADAPTER   *pChildUfpLane0Adapter;
  AMD_USB4_TMU_ADP_CS6       *pTmuAdpCs6;

  Status           = EFI_SUCCESS;
  DataSize         = 0;
  pTmuAdpCs6       = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if (NULL == pDownstreamRouter || NULL == pCM) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  pUsb4Cell              = (AMD_USB4_CM_CELL *) (pDownstreamRouter->Header.Parent);
  pParentDfpLane0Adapter = (AMD_USB4_CM_LANE_ADAPTER *)pUsb4Cell;
  pUsb4Cell              = &(pDownstreamRouter->pAdapter[pDownstreamRouter->UpstreamAdapter]);
  pChildUfpLane0Adapter  = (AMD_USB4_CM_LANE_ADAPTER *)pUsb4Cell;

  // Disable Time Sync on Upstream DFP Lane0
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane0Adapter->TMUCapability + 6),    // ReadAddress: TMU_ADP_CS6
             1,                                                       // ReadSize 1 DWs
             (UINT32**)&pTmuAdpCs6,                                   // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read DFP Lane0 TMU_ADP_CS6 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  pTmuAdpCs6->Field.DisableTimeSync = 1;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane0Adapter->TMUCapability + 6),    // ReadAddress: TMU_ADP_CS6
             &DataSize,
             &(pTmuAdpCs6->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write DFP Lane0 TMU_ADP_CS6 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  // Disable Time Sync on Downstream UFP Lane0
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pChildUfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),     // AdapterNum
             (UINT32) (pChildUfpLane0Adapter->TMUCapability + 6),     // ReadAddress: TMU_ADP_CS6
             1,                                                       // ReadSize 1 DWs
             (UINT32**)&pTmuAdpCs6,                                   // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read UFP Lane0 TMU_ADP_CS6 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  pTmuAdpCs6->Field.DisableTimeSync = 1;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pChildUfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),     // AdapterNum
             (UINT32) (pChildUfpLane0Adapter->TMUCapability + 6),     // ReadAddress: TMU_ADP_CS6
             &DataSize,
             &(pTmuAdpCs6->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write DFP Lane0 TMU_ADP_CS6 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief Enable Router to send TMU Packet on the link
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pDownstreamRouter   Point to Downstream Router
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4TmuEnableTimeSync (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pDownstreamRouter
  )
{
  EFI_STATUS        Status;
  UINT32            DataSize;
  AMD_USB4_CM_CELL           *pUsb4Cell;
  AMD_USB4_CM_LANE_ADAPTER   *pParentDfpLane0Adapter;
  AMD_USB4_CM_LANE_ADAPTER   *pChildUfpLane0Adapter;
  AMD_USB4_TMU_ADP_CS6       *pTmuAdpCs6;

  Status           = EFI_SUCCESS;
  DataSize         = 0;
  pTmuAdpCs6       = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if (NULL == pDownstreamRouter || NULL == pCM) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  pUsb4Cell              = (AMD_USB4_CM_CELL *) (pDownstreamRouter->Header.Parent);
  pParentDfpLane0Adapter = (AMD_USB4_CM_LANE_ADAPTER *)pUsb4Cell;
  pUsb4Cell              = &(pDownstreamRouter->pAdapter[pDownstreamRouter->UpstreamAdapter]);
  pChildUfpLane0Adapter  = (AMD_USB4_CM_LANE_ADAPTER *)pUsb4Cell;

  // Enable Time Sync on Upstream DFP Lane0
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane0Adapter->TMUCapability + 6),    // ReadAddress: TMU_ADP_CS6
             1,                                                       // ReadSize 1 DWs
             (UINT32**)&pTmuAdpCs6,                                   // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read DFP Lane0 TMU_ADP_CS6 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  pTmuAdpCs6->Field.DisableTimeSync = 0;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane0Adapter->TMUCapability + 6),    // ReadAddress: TMU_ADP_CS6
             &DataSize,
             &(pTmuAdpCs6->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write DFP Lane0 TMU_ADP_CS6 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  // Enable Time Sync on Downstream UFP Lane0
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pChildUfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),     // AdapterNum
             (UINT32) (pChildUfpLane0Adapter->TMUCapability + 6),     // ReadAddress: TMU_ADP_CS6
             1,                                                       // ReadSize 1 DWs
             (UINT32**)&pTmuAdpCs6,                                   // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read UFP Lane0 TMU_ADP_CS6 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  pTmuAdpCs6->Field.DisableTimeSync = 0;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pChildUfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),     // AdapterNum
             (UINT32) (pChildUfpLane0Adapter->TMUCapability + 6),     // ReadAddress: TMU_ADP_CS6
             &DataSize,
             &(pTmuAdpCs6->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write DFP Lane0 TMU_ADP_CS6 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief Set Local time
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pRouter             Point to Router
 *
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4TmuSetLocalTime (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pRouter
  )
{
  EFI_STATUS           Status;
  UINT32               TimeOut;
  UINT32               DataSize;
  UINT32               HostTimeLow;
  UINT32               HostTimeMiddle;
  UINT16               HostTimeHigh;
  UINT32               *pPostTimeLow;
  AMD_USB4_CM_ROUTER   *pHostRouter;
  AMD_USB4_TMU_ROUTER_CONFIGURATION_CAPABILITY *pTmuRtrCs;

  Status           = EFI_SUCCESS;
  DataSize         = 0;
  pTmuRtrCs        = NULL;
  pPostTimeLow     = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if (NULL == pRouter || NULL == pCM) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  pHostRouter = (AMD_USB4_CM_ROUTER*)&(pCM->PtrCmData[0]);

  // 1, Read the Host Router Time of the Domain.
  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Read Host Router (0x%lx) TMU Capability ---\n", pHostRouter->Header.TopologyID);
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pHostRouter->Header.TopologyID,
             CS_TARGET_ROUTER,                                          // CsType
             0,                                                         // AdapterNum
             (UINT32)(pHostRouter->TMUCapability),                      // ReadAddress
             sizeof (AMD_USB4_TMU_ROUTER_CONFIGURATION_CAPABILITY) / 4, // ReadSize
             (UINT32**)&pTmuRtrCs,                                      // DataOut
             &DataSize                                                  // DataSize
             );
  if ( EFI_ERROR (Status) || ( (sizeof (AMD_USB4_TMU_ROUTER_CONFIGURATION_CAPABILITY) / 4) != DataSize ) ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "Failed to read Host Router (0x%lx) TMU capability.\n",
      pHostRouter->Header.TopologyID
      );
    return EFI_DEVICE_ERROR;
  }
  HostTimeLow    = pTmuRtrCs->LocalTimeLow;
  HostTimeMiddle = pTmuRtrCs->LocalTimeMiddle;
  HostTimeHigh   = pTmuRtrCs->LocalTimeHigh;


  // 2, Read current Router TMU Capability
  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Read Router (0x%lx) TMU Capability ---\n", pRouter->Header.TopologyID);
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,                                          // CsType
             0,                                                         // AdapterNum
             (UINT32)(pRouter->TMUCapability),                          // ReadAddress
             sizeof (AMD_USB4_TMU_ROUTER_CONFIGURATION_CAPABILITY) / 4, // ReadSize
             (UINT32**)&pTmuRtrCs,                                      // DataOut
             &DataSize                                                  // DataSize
             );
  if ( EFI_ERROR (Status) || ( (sizeof (AMD_USB4_TMU_ROUTER_CONFIGURATION_CAPABILITY) / 4) != DataSize ) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to read Router (0x%lx) TMU capability.\n", pRouter->Header.TopologyID);
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Read Router (0x%lx) TMU capability Success---\n", pRouter->Header.TopologyID);


  // 3, Write the Host Router Time to the Post Local Time registers in the TMU Router Configuration Capability of
  //     the Router.
  // 4, Write a value of 0x1 to the TMU_RTR_CS_24.Post Time Low field
  //     and a value of 0xFFFFFFFF to the TMU_RTR_CS_25.Post Time High field of the Router.
  pTmuRtrCs->PostLocalTimeLow  = ((HostTimeMiddle & 0xFFFF) << 16) + ((HostTimeLow & 0xFFFF0000) >> 16);
  pTmuRtrCs->PostLocalTimeHigh =  ((UINT32)HostTimeHigh << 16) + ((HostTimeMiddle & 0xFFFF0000) >> 16);
  pTmuRtrCs->PostTimeLow       = 0x1;
  pTmuRtrCs->PostTimeHigh      = 0xFFFFFFFF;

  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Write Router (0x%lx) TMU_RTR_CS_22/23/24/25 ---\n", pRouter->Header.TopologyID);
  DataSize = 4;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             (UINT32) (pRouter->TMUCapability + 22),
             &DataSize,
             &(pTmuRtrCs->PostLocalTimeLow)
             );

  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "Failed to write Router (0x%lx) TMU_RTR_CS_22/23/24/25. \n",
      pRouter->Header.TopologyID
      );
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "--- Write Router (0x%lx) TMU_RTR_CS_22/23/24/25 Success---\n",
    pRouter->Header.TopologyID
    );

  // 5, Write a value of 0x0 to the TMU_RTR_CS_25.Post Time High field.
  //     This causes the Router to immediately update its local time to the value in the Post Local Time registers.
  pTmuRtrCs->PostTimeHigh      = 0x0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Write Router (0x%lx) TMU_RTR_CS_25 ---\n", pRouter->Header.TopologyID);
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             (UINT32) (pRouter->TMUCapability + 25),
             &DataSize,
             &(pTmuRtrCs->PostTimeHigh)
             );

  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write Router (0x%lx) TMU_RTR_CS_25. \n", pRouter->Header.TopologyID);
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("--- Write Router (0x%lx) TMU_RTR_CS_25 Success---\n", pRouter->Header.TopologyID);

  // 6. Periodically read the Post Time field of the Router to determine when the Router is done updating its
  //     local time. The Router sets the Post Time field to 0x0 after it has updated its local time.
  IDS_HDT_CONSOLE_USB4_DEBUG ("Check Post Time field.\n");
  Status = EFI_TIMEOUT;
  TimeOut = UPDATE_LOCAL_TIME_TIMEOUT/100;
  do {
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ROUTER,                                           // CsType
               0,                                                          // AdapterNum
               (UINT32) (pRouter->TMUCapability + 24),                     // ReadAddress
               1,                                                          // ReadSize 1 DWs
               &pPostTimeLow,                                              // DataOut
               &DataSize                                                   // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read Router (0x%lx) TMU_RTR_CS_24.\n", pRouter->Header.TopologyID);
      Status = EFI_DEVICE_ERROR;
      break;
    }
    IDS_HDT_CONSOLE_USB4_DEBUG ("TMU_RTR_CS_24 = 0x%x.\n", *pPostTimeLow);
    if ( *pPostTimeLow == 0 ) {
      Status = EFI_SUCCESS;
      break;
    }

    Status = EFI_TIMEOUT;
    IDS_HDT_CONSOLE_USB4_DEBUG ("Wait for Router updating Local Time. TimeOut %d\n", TimeOut);
    MicroSecondDelay (100);  // TODO - Decide the minimum delay time
  } while (--TimeOut);

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief Enable Host Router TMU
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pHostRouter         Point to Host Router
 * @param[in] TmuMode              TMU mode
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4HostRouterTmuEnable (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pHostRouter,
  IN     UINT8                      TmuMode
  )
{
  EFI_STATUS        Status;
  UINT32            DataSize;
  UINT32            TmuRtrCs3;

  Status           = EFI_SUCCESS;
  DataSize         = 0;
  TmuRtrCs3        = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if (NULL == pHostRouter || NULL == pCM || TmuMode > TMU_MODE_HIFI_BI) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( TmuMode == TMU_MODE_HIFI_BI) {
    // TMU parameters
    //   Freq Measurement Window = 800 (default)
    //   ErrorAvgConst  = 8 (default)
    //   OffsetAvgConst = 8 (default)
    //   DelayAvgConst  = 8 (default)
    //   FreqAvgConst   = 8 (default)
    // all default reset value, we can skip programming parameters.

    // Set the TMU_RTR_CS_3.TSPacketInterval field.
    TmuRtrCs3     = TMU_TSPacketInterval_HIFI << 16;

    IDS_HDT_CONSOLE_USB4_DEBUG ("--- Write Router (0x%lx) TMU_RTR_CS_3 ---\n", pHostRouter->Header.TopologyID);
    DataSize = 1;
    Status = AmdUsb4Ring0WriteCS (
               pCM,
               pHostRouter->Header.TopologyID,
               CS_TARGET_ROUTER,
               0,
               (UINT32) (pHostRouter->TMUCapability + 3),
               &DataSize,
               &TmuRtrCs3
               );

    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR (
        "Failed to write Router (0x%lx) TMU_RTR_CS_3. \n",
        pHostRouter->Header.TopologyID
        );
      return EFI_DEVICE_ERROR;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG ("--- Write Router (0x%lx) TMU_RTR_CS_3 Success---\n", pHostRouter->Header.TopologyID);

    // Update Router Structure Data.
    pHostRouter->TMUInfo.UniDirectionalMode = 0;
    pHostRouter->TMUInfo.TSPacketInterval   = TMU_TSPacketInterval_HIFI;
  } else {
    return EFI_UNSUPPORTED;               // only support HifiBi in pre-OS CM
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief Enable Device Router TMU
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pRouter             Point to Router
 * @param[in] TmuMode              TMU mode
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4DeviceRouterTmuEnable (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pRouter,
  IN     UINT8                      TmuMode
  )
{
  EFI_STATUS        Status;
  UINT32            DataSize;
  UINT32            TmuRtrCs3;
  AMD_USB4_CM_CELL           *pUsb4Cell;
  AMD_USB4_CM_LANE_ADAPTER   *pParentDfpLane0Adapter;
  AMD_USB4_CM_LANE_ADAPTER   *pChildUfpLane0Adapter;
  AMD_USB4_TMU_ADP_CS3       *pTmuAdpCs3;

  Status           = EFI_SUCCESS;
  DataSize         = 0;
  TmuRtrCs3        = 0;
  pTmuAdpCs3       = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if (NULL == pRouter || NULL == pCM || TmuMode > TMU_MODE_HIFI_BI) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  pUsb4Cell              = (AMD_USB4_CM_CELL *) (pRouter->Header.Parent);
  pParentDfpLane0Adapter = (AMD_USB4_CM_LANE_ADAPTER *)pUsb4Cell;
  pUsb4Cell              = &(pRouter->pAdapter[pRouter->UpstreamAdapter]);
  pChildUfpLane0Adapter  = (AMD_USB4_CM_LANE_ADAPTER *)pUsb4Cell;

  if ( TmuMode == TMU_MODE_HIFI_BI) {
    // TMU parameters
    //   Freq Measurement Window = 800 (default)
    //   ErrorAvgConst  = 8 (default)
    //   OffsetAvgConst = 8 (default)
    //   DelayAvgConst  = 8 (default)
    //   FreqAvgConst   = 8 (default)
    // all default reset value, we can skip programming parameters.

    // Set the TMU_ADP_CS_3.EnableUniDirectionalMode bit to 0b in both Ports.
    //  Upstream DFP
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pParentDfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
               (UINT32) (pParentDfpLane0Adapter->TMUCapability + 3),    // ReadAddress: TMU_ADP_CS6
               1,                                                       // ReadSize 1 DWs
               (UINT32**)&pTmuAdpCs3,                                   // DataOut
               &DataSize                                                // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read DFP Lane0 TMU_ADP_CS6 Register.\n");
      return EFI_DEVICE_ERROR;
    }

    pTmuAdpCs3->Field.EnableUniDirectionalMode = 0;
    DataSize = 1;
    Status = AmdUsb4Ring0WriteCS (
               pCM,
               pParentDfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
               (UINT32) (pParentDfpLane0Adapter->TMUCapability + 3),    // ReadAddress: TMU_ADP_CS6
               &DataSize,
               &(pTmuAdpCs3->Value)
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write DFP Lane0 TMU_ADP_CS3 Register.\n");
      return EFI_DEVICE_ERROR;
    }

    //  Dowmstream UFP
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pChildUfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),     // AdapterNum
               (UINT32) (pChildUfpLane0Adapter->TMUCapability + 3),     // ReadAddress: TMU_ADP_CS6
               1,                                                       // ReadSize 1 DWs
               (UINT32**)&pTmuAdpCs3,                                   // DataOut
               &DataSize                                                // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read UFP Lane0 TMU_ADP_CS3 Register.\n");
      return EFI_DEVICE_ERROR;
    }

    pTmuAdpCs3->Field.EnableUniDirectionalMode = 0;
    DataSize = 1;
    Status = AmdUsb4Ring0WriteCS (
               pCM,
               pChildUfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),      // AdapterNum
               (UINT32) (pChildUfpLane0Adapter->TMUCapability + 3),      // ReadAddress: TMU_ADP_CS6
               &DataSize,
               &(pTmuAdpCs3->Value)
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write UFP Lane0 TMU_ADP_CS3 Register.\n");
      return EFI_DEVICE_ERROR;
    }

    // Set the TMU_RTR_CS_3.TSPacketInterval field.
    TmuRtrCs3     = TMU_TSPacketInterval_HIFI << 16;

    IDS_HDT_CONSOLE_USB4_DEBUG ("--- Write Router (0x%lx) TMU_RTR_CS_3 ---\n", pRouter->Header.TopologyID);
    DataSize = 1;
    Status = AmdUsb4Ring0WriteCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ROUTER,
               0,
               (UINT32) (pRouter->TMUCapability + 3),
               &DataSize,
               &TmuRtrCs3
               );

    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write Router (0x%lx) TMU_RTR_CS_3. \n", pRouter->Header.TopologyID);
      return EFI_DEVICE_ERROR;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG ("--- Write Router (0x%lx) TMU_RTR_CS_3 Success---\n", pRouter->Header.TopologyID);

    // Update Router Structure Data.
    pRouter->TMUInfo.UniDirectionalMode = 0;
    pRouter->TMUInfo.TSPacketInterval   = TMU_TSPacketInterval_HIFI;
  } else {
    return EFI_UNSUPPORTED;               // only support HifiBi in pre-OS CM
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief Configure Device Router TMU
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pRouter             Point to Router
 *
 * @retval EFI_SUCCESS             Success.
 * @retval EFI_INVALID_PARAMETER   Invalid Parameter.
 * @retval EFI_DEVICE_ERROR        Failed.
**/
EFI_STATUS
AmdUsb4DeviceRouterTmuInit (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pRouter
  )
{
  EFI_STATUS        Status;

  Status           = EFI_SUCCESS;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  Status = AmdUsb4TmuTimeDisruption (pCM, pRouter, TRUE);
  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Cannot Disable Time Sync!\n");
    return Status;
  }

  Status = AmdUsb4TmuDisableTimeSync (pCM, pRouter);
  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Cannot Disable Time Sync!\n");
    return Status;
  }

  Status = AmdUsb4TmuSetLocalTime (pCM, pRouter);
  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Cannot Set Local Time!\n");
    return Status;
  }

  Status = AmdUsb4DeviceRouterTmuEnable (pCM, pRouter, pCM->CmTmuMode);
  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Cannot Enable TMU!\n");
    return Status;
  }

  Status = AmdUsb4TmuEnableTimeSync (pCM, pRouter);
  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Cannot Enable Time Sync!\n");
    return Status;
  }

  Status = AmdUsb4TmuTimeDisruption (pCM, pRouter, FALSE);
  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Cannot Disable Time Sync!\n");
    return Status;
  }

  return Status;
}
