/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/TimerLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IdsLib.h>

#include "AmdUsb4.h"
#include <AmdUsb4/ControlPacket.h>
#include <AmdUsb4/ConfigurationSpace.h>
#include <AmdUsb4/Thunderbolt3.h>

#include "Library/AmdUsb4Lib.h"
#include <AmdUsb4/Drom.h>

#define FILECODE LIBRARY_AMDUSB4LIB_TBT3_FILECODE

EFI_STATUS
Tbt3PortRegionSetting (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pLaneAdapter,
  IN     BOOLEAN                    bIsUFP
  );


BOOLEAN
IsTbt3Router (
  IN     AMD_USB4_CM_ROUTER   *pRouter
  )
{
  if ( ( ( pRouter->Usb4Version >> 5 ) & 7 ) == 0 ) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
 * @brief   Check TBT3 router type.
 *
 * @param   AMD_USB4_CM_ROUTER     *pRouter
 * @return  TBT3_ROUTER_TYPE       TBT3_TYPE_AR           AR type
 *                                 TBT3_TYPE_TR           TR type
 *                                 TBT3_TYPE_UNKNOW       Unknow type
 */
TBT3_ROUTER_TYPE
Tbt3RouterType (
  IN     AMD_USB4_CM_ROUTER   *pRouter
  )
{
  TBT3_ROUTER_TYPE Tbt3Type;

  Tbt3Type = TBT3_TYPE_UNKNOWN;

  if ( pRouter->VendorId == 0x8086 ) {
    switch (pRouter->ProductId) {
      case 0x1575:
      case 0x1576:
      case 0x1577:
      case 0x1578:
      case 0x15BF:
      case 0x15C0:
      case 0x15D2:
      case 0x15D3:
      case 0x15D9:
      case 0x15DA:
        Tbt3Type = TBT3_TYPE_AR;
        break;
      case 0x15E7:
      case 0x15E8:
      case 0x15EA:
      case 0x15EB:
      case 0x15EF:
        Tbt3Type = TBT3_TYPE_TR;
        break;
      default:
        break;
    }
  }

  return Tbt3Type;
}


/**
 * @brief   Get specific capability id offset.
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    *pRouter
 * @param   UINT32                CapabilityID
 *
 * @return  UINT32                Capability Header. Zero means not found.
 */
UINT32
Tbt3GetRouterCapability (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     UINT8                    FindCapabilityID
  )
{
  EFI_STATUS            Status;
  UINT32                DataSize;
  UINT32                *CapabilityReg;
  UINT8                 CapabilityPointer;
  UINT8                 NextCapabilityPointer;
  UINT8                 CapabilityID;

  Status                = EFI_SUCCESS;
  DataSize              = 0;
  CapabilityReg         = NULL;
  CapabilityPointer     = 0;
  NextCapabilityPointer = 0;
  CapabilityID          = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("[%a] - Entry\n", __FUNCTION__);

  if ( !IsTbt3Router (pRouter) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (" This is not TBT3 Router.\n");
    return EFI_SUCCESS;
  }

  CapabilityPointer = pRouter->CapabilityOffset;
  while ( CapabilityPointer != 0 ) {
    //
    //  Read Request: loop Capability link (Size = 1DW)
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ROUTER,                                    // CsType
               0,                                                   // AdapterNum
               (UINT32)CapabilityPointer,                           // ReadAddress
               1,                                                   // ReadSize
               (UINT32 **)&CapabilityReg,                           // DataOut
               &DataSize                                            // DataSize
               );
    if (EFI_ERROR (Status) || (DataSize != 1)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Failed to read capabilities\n");
      return 0;
    }

    NextCapabilityPointer  = (UINT8)(*CapabilityReg);
    CapabilityID  = (UINT8)((*CapabilityReg) >> 8);
    IDS_HDT_CONSOLE_USB4_DEBUG (
      " CapabilityID = 0x%x, NextCapabilityPointer = 0x%x\n",
      CapabilityID,
      NextCapabilityPointer
      );

    if ( CapabilityID == FindCapabilityID ) {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        " Find CapabilityID = 0x%x, Offset = 0x%x\n",
        CapabilityID,
        CapabilityPointer
        );
      return (UINT32)CapabilityPointer;
    }

    CapabilityPointer = NextCapabilityPointer;
  }

  return 0;
}

/**
 * @brief   Get specific capability id offset.
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    *pRouter
 * @param   UINT32                CapabilityID
 *
 * @return  UINT32                Zero means not found.
 */
UINT32
Tbt3DpGetCapability (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER   *pAdapter,
  IN     UINT8                    FindCapabilityID
  )
{
  EFI_STATUS            Status;
  UINT32                DataSize;
  UINT32                *CapabilityReg;
  UINT8                 CapabilityPointer;
  UINT8                 NextCapabilityPointer;
  UINT8                 CapabilityID;

  Status                = EFI_SUCCESS;
  DataSize              = 0;
  CapabilityReg         = NULL;
  CapabilityPointer     = 0;
  NextCapabilityPointer = 0;
  CapabilityID          = 0;

  if (FindCapabilityID == DP_TMU_ADAPTER_CONFIGURATION_CAPABILITY_ID) {
    if ( !IsTbt3Router ((AMD_USB4_CM_ROUTER*) pAdapter->Header.Parent) ) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("   DP Adapter does not belong to a TBT3 Router.\n");
      return 0;
    }
  }

  CapabilityPointer = (UINT8) pAdapter->CapabilityOffset;
  while ( CapabilityPointer != 0 ) {
    //
    //  Read Request: loop Capability link (Size = 1DW)
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pAdapter->Header.TopologyID,
               CS_TARGET_ADAPTER,                                   // CsType
               pAdapter->Header.AdapterNum,                         // AdapterNum
               (UINT32)CapabilityPointer,                           // ReadAddress
               1,                                                   // ReadSize
               (UINT32 **)&CapabilityReg,                           // DataOut
               &DataSize                                            // DataSize
               );
    if (EFI_ERROR (Status) || (DataSize != 1)) {
      IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to read capabilities\n");
      return 0;
    }

    NextCapabilityPointer  = (UINT8)(*CapabilityReg);
    CapabilityID  = (UINT8)((*CapabilityReg) >> 8);
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "   CapabilityID = 0x%x, NextCapabilityPointer = 0x%x\n",
      CapabilityID,
      NextCapabilityPointer
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

/**
 * @brief   Get specific capability id offset.
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    *pRouter
 * @param   UINT32                CapabilityID
 *
 * @return  UINT32                Zero means not found.
 */
UINT32
Tbt3GetRouterVSCOffset (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     UINT8                    NextCapability,
  IN     UINT8                    FindCapabilityID,
  IN     UINT8                    FindVSCID
  )
{
  EFI_STATUS                            Status;
  UINT32                                DataSize;
  UINT32                                *CapabilityReg;
  UINT8                                 CapabilityPointer;
  UINT8                                 NextCapabilityPointer;
  UINT8                                 CapabilityID;
  UINT8                                 VSCID;

  Status                = EFI_SUCCESS;
  DataSize              = 0;
  CapabilityReg         = NULL;
  CapabilityPointer     = 0;
  NextCapabilityPointer = 0;
  CapabilityID          = 0;
  VSCID                 = 0;

  CapabilityPointer = NextCapability;
  while ( CapabilityPointer != 0 ) {
    //
    //  Read Request: loop Capability link (Size = 1DW)
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ROUTER ,                                   // CsType
               0,                                                   // AdapterNum
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

    if ( CapabilityID == FindCapabilityID && VSCID == FindVSCID ) {
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


/**
 * @brief   Get specific capability id offset.
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    *pRouter
 * @param   UINT32                CapabilityID
 *
 * @return  UINT32                Zero means not found.
 */
UINT32
Tbt3GetAdapterVSCOffset (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_CELL         *pCell,
  IN     UINT8                    NextCapability,
  IN     UINT8                    FindCapabilityID,
  IN     UINT8                    FindVSCID
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

    if ( CapabilityID == FindCapabilityID && VSCID == FindVSCID ) {
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


/**
 * @brief   Check if there is router connect to this TBT3 router.
 *
 * @param   AMD_USB4_CM               *pCM
 * @param   AMD_USB4_CM_LANE_ADAPTER  *pAdapter
 *
 * @return  BOOLEAN               TRUE
 *                                FALSE
 */
BOOLEAN
Tbt3RouterDetected (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER *pAdapter
  )
{
  EFI_STATUS             Status;
  UINT32                 DataSize;
  UINT32                 *Ptr32;

  Status  = EFI_SUCCESS;
  DataSize  = 0;
  Ptr32     = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Start\n", __FUNCTION__);

  if ( !IsTbt3Router ((AMD_USB4_CM_ROUTER*)pAdapter->Header.Parent) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (" This is not TBT3 Router.\n");
    return EFI_SUCCESS;
  }

  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pAdapter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,                                                // AdapterNum
             pAdapter->Usb4Capability + VSEC_6_PORT_CS_157,    // ReadAddress     VSEC_6_CS_157
             1,                                                // ReadSize 1 DWs
             (UINT32**)&Ptr32,                                 // DataOut
             &DataSize                                         // DataSize
             );
  IDS_HDT_CONSOLE_USB4_DEBUG (" Status = %r, VSEC_6_PORT_CS_157 value = 0x%x\n", Status, *Ptr32);
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    return FALSE;
  }

  return ( ( (*Ptr32) >> ROUTER_DETECTED_OFFSET ) == ROUTER_DETECTED_VALUE ) ? TRUE : FALSE;
}


/**
 * @brief   Enumerate TBT3 Host Interface Adapter
 *
 * @param   AMD_USB4_CM                           *pCM
 * @param   UINT32                                AdapterNum
 * @param   AMD_USB4_CM_ROUTER                    *pRouter
 * @param   AMD_USB4_CM_HI_ADAPTER                *pAdapter
 * @param   AMD_USB4_ADAPTER_CONFIGURATION_SPACE  *pAdapterCs
 *
 * @return  EFI_STATUS
 */
EFI_STATUS
Tbt3EnumerateHIAdapter (
  IN      AMD_USB4_CM                           *pCM,
  IN      UINT32                                AdapterNum,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter,
  IN  OUT AMD_USB4_CM_HI_ADAPTER                *pAdapter,
  IN      AMD_USB4_ADAPTER_CONFIGURATION_SPACE  *pAdapterCs
  )
{
  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  if ( !IsTbt3Router (pRouter) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   This is not TBT3 Router.\n");
    return EFI_SUCCESS;
  }

  pAdapter->Header.TopologyID         = pRouter->Header.TopologyID;
  pAdapter->Header.AdapterNum         = (UINT8)AdapterNum;
  pAdapter->Header.Parent             = (AMD_USB4_CM_CELL_HEADER*)pRouter;
  pAdapter->Header.CellType           = CELL_TYPE_HI_ADAPTER;

  pRouter->pAdapterHostInterface      = (AMD_USB4_CM_CELL*)pAdapter;

  pAdapter->AdpHeader.MaxInputHopId   = (UINT16)(pAdapterCs->AdpCs5.Field.MaxInputHopID);
  pAdapter->AdpHeader.MaxOutputHopId  = (UINT16)(pAdapterCs->AdpCs5.Field.MaxOutputHopID);
  pAdapter->AdpHeader.pSiblingAdapter = NULL;
  pAdapter->AdpHeader.pLinkPartner    = NULL;

  pAdapter->CapabilityOffset  = pAdapterCs->AdpCs1.Field.NextCapabilityPointer;
  pAdapter->VSC1Offset        = (UINT8)Tbt3GetAdapterVSCOffset (
                                         pCM,
                                         (AMD_USB4_CM_CELL*)pAdapter,
                                         (UINT8)pAdapter->CapabilityOffset,
                                         VENDOR_SPECIFIC_CAPABILITY_ID,
                                         VSC_ID_1
                                         );

  IDS_HDT_CONSOLE_USB4_DEBUG (" TBT3 HI adapter:\n");
  IDS_HDT_CONSOLE_USB4_DEBUG ("  AdapterNum       = 0x%x\n", pAdapter->Header.AdapterNum);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  CellType         = 0x%x\n", pAdapter->Header.CellType);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaxInputHopId    = 0x%x\n", pAdapter->AdpHeader.MaxInputHopId);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  MaxOutputHopId   = 0x%x\n", pAdapter->AdpHeader.MaxOutputHopId);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  CapabilityOffset = 0x%x\n", pAdapter->CapabilityOffset);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  VSC1Offset       = 0x%x\n", pAdapter->VSC1Offset);

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);

  return EFI_SUCCESS;
}

/**
 * @brief   Read Tbt3 DROM
 *
 * @param   AMD_USB4_CM              *pCM
 * @param   AMD_USB4_CM_ROUTER       *pRouter
 * @param   UINT32                   Offset
 * @param   UINT32                   Size
 * @param   UINT32                   *Data
 *
 * @return  EFI_STATUS
 */
EFI_STATUS
TBT3ReadDrom (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_HI_ADAPTER   *pHIAdapter,
  IN     UINT32                   Offset,
  IN     UINT32                   Size,
     OUT UINT32                   *Data
  )
{
  EFI_STATUS                Status;
  EFI_STATUS                ReturnStatus;
  UINT32                    DromBase;
  AMD_TBT3_NVM_READ         Request;
  AMD_TBT3_NVM_READ         *Response;
  UINT32                    DataSize;
  UINT32                    *Ptr32;
  UINT32                    i;
  UINT32                    AndMask;
  UINT32                    OrValue;

  Status          = EFI_SUCCESS;
  ReturnStatus    = EFI_SUCCESS;
  DromBase        = 0;
  Request.Value   = 0;
  DataSize        = 0;
  Response        = NULL;
  Ptr32           = NULL;
  i               = 0;
  AndMask         = 0;
  OrValue         = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  IDS_HDT_CONSOLE_USB4_ASSERT (Data != NULL);

  if ( pHIAdapter == NULL ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (" Invalid Parameter.\n");
    return EFI_SUCCESS;
  }

  IDS_HDT_CONSOLE_USB4_ASSERT (pHIAdapter->VSC1Offset != 0);

  Offset  = Offset >> 2;
  Size    = ALIGN_VALUE (Size, 4) >> 2;

  // Write the Mail In command to TBT3_CS.ADP_VSEC_1_CS_17
  IDS_HDT_CONSOLE_USB4_DEBUG (" Write the Mail In command to TBT3_CS.ADP_VSEC_1_CS_17\n");
  Request.Value                   = 0;
  Request.MailIn.OperationRequest = 1;
  Request.MailIn.CommandCode      = MAIL_IN_NVM_READ_CMD;
  Request.MailIn.StartDWAddress   = Offset;
  Request.MailIn.NumberOfDWRead   = ( Size == 16 ) ? 0 : Size;  // 0h indicates 16 DWORDS read.
  DataSize                        = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pHIAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             pHIAdapter->Header.AdapterNum,
             pHIAdapter->VSC1Offset + HI_VSC_1_CS_17,
             &DataSize,
             &(Request.Value)
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Status = %r\n", Status);
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "   Failed to write Tbt3 HI Adapter (AdpNum: %d) CS (Offset: %d)\n",
      pHIAdapter->Header.AdapterNum,
      pHIAdapter->VSC1Offset + HI_VSC_1_CS_17
      );
    return EFI_DEVICE_ERROR;
  }

/*
  // Then set the Operation Request bit to 1b.
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Then set the Operation Request bit to 1b.\n");
  AndMask = (UINT32)(~MAILIN_OPERATION_REQUEST);
  OrValue = (UINT32)MAILIN_OPERATION_REQUEST;
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pHIAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             pHIAdapter->Header.AdapterNum,
             pHIAdapter->VSC1Offset + HI_VSC_1_CS_17,
             1,
             &AndMask,
             &OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Status = %r\n", Status);
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Failed to set the Operation Request bit to 1b\n");
    return EFI_DEVICE_ERROR;
  }
*/
  // Poll the Operation Request bit in the Mail In structure until it is set to 0b.
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Poll the Operation Request bit in the Mail In structure until it is set to 0b."
    );
  Status = AmdUsb4ReadBitTimeout (
             pCM,
             pHIAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             pHIAdapter->Header.AdapterNum,
             pHIAdapter->VSC1Offset + HI_VSC_1_CS_17,
             MAILIN_OPERATION_REQUEST,
             0,
             MAIL_IN_NVM_READ_CMD_TIMEOUT
             );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Get Polling Operation Request bit flip return status %r\n",
    Status
    );

  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("    Failed to get Operation Request bit flip.\n");
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("   Operation Request bit flip.\n");

  // Read the Status Response bit to make sure it is set to 1b.
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Read the Status Response bit to make sure it is set to 1b.\n"
    );
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pHIAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                                   // CsType
             pHIAdapter->Header.AdapterNum,                       // AdapterNum
             pHIAdapter->VSC1Offset + HI_VSC_1_CS_18,             // ReadAddress
             1,                                                   // ReadSize
             (UINT32 **)&Response,                                // DataOut
             &DataSize                                            // DataSize
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Status = %r\n", Status);
  if ( EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Fail to read Mail out.\n");
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Status Response bit: %d (1), Status: %d (0), Status for Cmd: %d (2)\n",
    Response->MailOut.StatusResponse,
    Response->MailOut.Status,
    Response->MailOut.StatusIsForCmd
    );

  if ( 0 == Response->MailOut.StatusResponse ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Status Response is not 1. Quit\n");
    return EFI_DEVICE_ERROR;
  }

  if ( 0 != Response->MailOut.Status ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Status is not 0 (Command completed). Quit\n");
    return EFI_DEVICE_ERROR;
  }

  if ( MAIL_IN_NVM_READ_CMD != Response->MailOut.StatusIsForCmd ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Status for Command is not 2 (NVM Read). Quit\n");
    return EFI_DEVICE_ERROR;
  }

  // Read data
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Read DROM data. Size = %d\n", Size);
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pHIAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                                   // CsType
             pHIAdapter->Header.AdapterNum,                       // AdapterNum
             pHIAdapter->VSC1Offset + HI_VSC_1_CS_1,              // ReadAddress
             Size,                                                // ReadSize
             &Ptr32,                                              // DataOut
             &DataSize                                            // DataSize
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Status = %r, DataSize = %d\n", Status, DataSize);
  if ( EFI_ERROR (Status) || Size != DataSize ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Fail to read DROM data.\n");
    ReturnStatus = EFI_DEVICE_ERROR;
  } else {
    // Copy Data out
    for (i = 0; i < Size; i++) {
      Data[i] = Ptr32[i];
    }
  }

  // Set the Status Response bit to 0b in the Mail Out structure.
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Set the Status Response bit to 0b in the Mail Out structure.\n");
  Request.Value                   = 0;
  Request.MailOut.StatusResponse  = 0;
  DataSize                        = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pHIAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             pHIAdapter->Header.AdapterNum,
             pHIAdapter->VSC1Offset + HI_VSC_1_CS_18,
             &DataSize,
             &(Request.Value)
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Status = %r\n", Status);
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "   Failed to write Tbt3 HI Adapter (AdpNum: %d) CS (Offset: %d)\n",
      pHIAdapter->Header.AdapterNum,
      pHIAdapter->VSC1Offset + HI_VSC_1_CS_18
      );
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);

  return ReturnStatus;
}


BOOLEAN
Tbt3GetDRomLane0 (
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     UINT8                    AdapterNumber
  )
{
  UINT32    i;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Start (AdapterNumber:%d)\n", __FUNCTION__, AdapterNumber);

  if ( pRouter->DromVersion != 0xFFFFFFFF ) {
    for (i = 0; i < pRouter->NumOfTbt3LaneAdapEntries; i++) {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "  #%d AdapterNumber=%d, AdapterDisable=%d, L1A=%d, DLC=%d, SecAdapterNum=%d\n",
        i,
        pRouter->Tbt3LaneAdapEntry[i].Field.AdapterNumber,
        pRouter->Tbt3LaneAdapEntry[i].Field.AdapterDisable,
        pRouter->Tbt3LaneAdapEntry[i].Field.L1A,
        pRouter->Tbt3LaneAdapEntry[i].Field.DLC,
        pRouter->Tbt3LaneAdapEntry[i].Field.SecAdapterNum
        );
      if ( pRouter->Tbt3LaneAdapEntry[i].Field.AdapterNumber  == AdapterNumber ) {
        return ( pRouter->Tbt3LaneAdapEntry[i].Field.L1A == 0 ) ? TRUE : FALSE;
      }
    }
  }

  return FALSE;
}

/**
 * @brief   Enable TBT3 Lane Adapter
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    *pRouter
 *
 * @return  EFI_STATUS
 */
EFI_STATUS
Tbt3LaneAdapterEnable (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter
  )
{
  EFI_STATUS          Status;
  UINT8               HIANumber;
  AMD_USB4_CM_CELL    *PtrCell;
  UINT32              i;
  UINT32              PortNum;
  UINT32              *Ptr32;
  UINT32              DataSize;

  Status    = EFI_SUCCESS;
  HIANumber = 0;
  PtrCell   = NULL;
  PortNum   = 0;
  Ptr32     = NULL;
  DataSize  = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  if ( !IsTbt3Router (pRouter) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   This is not TBT3 Router.\n");
    return EFI_SUCCESS;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Router (0x%lx) MaxAdapter is %d\n",
    pRouter->Header.TopologyID,
    pRouter->MaxAdapter
    );

  for (i = 1; i <= pRouter->MaxAdapter; i++) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      " Adapter #%d CellType=0x%x\n",
      i,
      pRouter->pAdapter[i].Header.CellType
      );

    switch (pRouter->pAdapter[i].Header.CellType) {
      case CELL_TYPE_LANE_ADAPTER:
        if ( Tbt3GetDRomLane0 (pRouter, pRouter->pAdapter[i].Header.AdapterNum ) ) {
          IDS_HDT_CONSOLE_USB4_DEBUG ("pRouter->pAdapter[%d].LaneAdapter is Lane 0\n", i);

          // 1. Get VSEC6 Offset
          pRouter->pAdapter[i].LaneAdapter.Usb4Capability =
            (UINT32)(pRouter->VSECapability + COMMON_REGION_LENGTH + PortNum * PORT_REGION_LENGTH);
          PortNum ++;

          // 2. Get Bonding Enable Capability
          Status = AmdUsb4Ring0ReadCS (
                     pCM,
                     pRouter->Header.TopologyID,
                     CS_TARGET_ROUTER,
                     0,
                     pRouter->pAdapter[i].LaneAdapter.Usb4Capability + VSEC_6_PORT_CS_141,
                     1,
                     &Ptr32,
                     &DataSize
                     );
          if ( !EFI_ERROR (Status) && ( 1 == DataSize ) && Ptr32 && ( (*Ptr32) & BE ) ) {
            pRouter->pAdapter[i].LaneAdapter.Usb4Port_BE = 1;
          }

          IDS_HDT_CONSOLE_USB4_DEBUG (
            " pRouter->pAdapter[%d].LaneAdapter.Usb4Port_BE    = 0x%x\n"
            " pRouter->pAdapter[%d].LaneAdapter.Usb4Capability = 0x%x\n",
            i,
            pRouter->pAdapter[i].LaneAdapter.Usb4Port_BE,
            i,
            pRouter->pAdapter[i].LaneAdapter.Usb4Capability
            );

          // 3. Enable Port
          if ( i == pRouter->UpstreamAdapter ) {  // Get UFP Lane 0 Adapter
            Tbt3PortRegionSetting (
              pCM,
              &(pRouter->pAdapter[i].LaneAdapter),
              TRUE
              );
          } else {
            pRouter->DFPAmount++;
            IDS_HDT_CONSOLE_USB4_DEBUG ("pRouter[0x%lx]->DFPAmount = %d\n", pRouter->Header.TopologyID, pRouter->DFPAmount);
            Tbt3PortRegionSetting (
              pCM,
              &(pRouter->pAdapter[i].LaneAdapter),
              FALSE
              );
          }
        } else {
          IDS_HDT_CONSOLE_USB4_DEBUG ("pRouter->pAdapter[%d].LaneAdapter is Lane 1\n", i);
          pRouter->pAdapter[i].LaneAdapter.Usb4Capability = 0;
        }
        break;
      default:
        break;
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);
  return Status;
}


BOOLEAN
Tbt3LinkControlLaneBondingPossible (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter
  )
{
  // We always think TBT3 Router support Lane Bonding.
  return TRUE;
}


EFI_STATUS
Tbt3QueryDPResource (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     AMD_USB4_CM_DP_ADAPTER   *pDpAdapter
  )
{
  return EFI_SUCCESS;
}


EFI_STATUS
Tbt3AllocDPResource (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     AMD_USB4_CM_DP_ADAPTER   *pDpAdapter
  )
{
  return EFI_SUCCESS;
}


EFI_STATUS
Tbt3DeallocDPResource (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     AMD_USB4_CM_DP_ADAPTER   *pDpAdapter
  )
{
  return EFI_SUCCESS;
}


EFI_STATUS
Tbt3SetDpAdapterHopId (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     AMD_USB4_CM_DP_ADAPTER   *pDpAdapter
  )
{
  EFI_STATUS                Status;
  UINT32                    AndMask[2];
  UINT32                    OrValue[2];
  AMD_USB4_CM_LANE_ADAPTER  *pRouterUFP;
  AMD_USB4_CM_LANE_ADAPTER  *pParentDFP;

  Status        = EFI_SUCCESS;
  pRouterUFP    = NULL;
  pParentDFP    = NULL;
  ZeroMem ((VOID*)AndMask, sizeof (AndMask));
  ZeroMem ((VOID*)OrValue, sizeof (OrValue));

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  if ( NULL == pCM || NULL == pDpAdapter || NULL == pRouter ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Invalid parameter\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( !IsTbt3Router (pRouter) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   This is not TBT3 Router.\n");
    return EFI_SUCCESS;
  }

  AndMask[0] = (UINT32)(~(0x7FF << 16));  // Video HopID Bit[26:16]
  OrValue[0] = 9 << 16;                   // Video HopID is 9.
  AndMask[1] = (UINT32)(~0x3FFFFF);       // Aux Tx HopID Bit[10:0], Aux Rx HopID Bit[21:11]
  OrValue[1] = ( 8 << 11 ) | 8;           // Aux Tx HopID is 8 and Aux Rx HopID is 8.
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pDpAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                                   // CsType
             pDpAdapter->Header.AdapterNum,                       // AdapterNum
             (UINT32)(pDpAdapter->CapabilityOffset),              // Address
             2,                                                   // SizeInDW
             AndMask,                                             // AndMask
             OrValue                                              // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Set Router (0x%lx) Dp Adapter (%d) Video HopID, Aux Tx HopID and Aux Rx HopID result. %r\n",
    pDpAdapter->Header.TopologyID,
    pDpAdapter->Header.AdapterNum,
    Status
    );

  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "   Failed to set Router (0x%lx) Dp Adapter (%d) Video HopID, Aux Tx HopID and Aux Rx HopID.\n",
      pDpAdapter->Header.TopologyID,
      pDpAdapter->Header.AdapterNum,
      Status
      );
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);

  return EFI_SUCCESS;
}


EFI_STATUS
Tbt3PortRegionSetting (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pLaneAdapter,
  IN     BOOLEAN                    bIsUFP
  )
{
  EFI_STATUS  Status;
  UINT32      AndMask;
  UINT32      OrValue;

  Status  = EFI_SUCCESS;
  AndMask = (UINT32)(~(SLI + CM_COMPAT2 + CM_COMPAT1 + L0C + PORT_IS_UPSTREAM));
  if ( bIsUFP ) {
    OrValue = (UINT32)(SLI + CM_COMPAT2 + CM_COMPAT1 + L0C + PORT_IS_UPSTREAM);
  } else {
    OrValue = (UINT32)(SLI + CM_COMPAT2 + CM_COMPAT1 + L0C);
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("[%a] - Entry\n", __FUNCTION__);
  IDS_HDT_CONSOLE_USB4_DEBUG ("Set Tbt3 Router(0x%lx) VESC6.LC_SX_CTRL.\n", pLaneAdapter->Header.TopologyID);

  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             pLaneAdapter->Usb4Capability + VSEC_6_PORT_CS_150,
             1,
             &AndMask,
             &OrValue
             );
  if (EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to set Tbt3 Router(0x%lx) VESC6.LC_SX_CTRL.\n", pLaneAdapter->Header.TopologyID);
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Dp Out Amount %d\n",
    ((AMD_USB4_CM_ROUTER*)(pLaneAdapter->Header.Parent))->DpOutAmount
    );

  if ( ((AMD_USB4_CM_ROUTER*)(pLaneAdapter->Header.Parent))->DpOutAmount > 0 ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Set TBT3 DP out timer.\n");
    StartTimeout (&(pCM->HPDWaitTimeout), DPOUT_HPD_TIMEOUT);
  }

  return Status;
}


EFI_STATUS
Tbt3DpTmuConfiguration (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_ROUTER       *pRouter,
  IN     AMD_USB4_CM_DP_ADAPTER   *pDpOutAdapter
  )
{
  EFI_STATUS                  Status;
  AMD_USB4_TBT3_TMU_DP_CS_13  AndMask;
  AMD_USB4_TBT3_TMU_DP_CS_13  OrValue;

  Status                   = EFI_SUCCESS;
  AndMask.Value            = 0xFFFFFFFF;
  AndMask.Field.TmuStable  = 0x00;
  OrValue.Value            = 0x00000000;
  OrValue.Field.TmuStable  = 0x80;

  IDS_HDT_CONSOLE_USB4_DEBUG ("[%a] - Entry\n", __FUNCTION__);

  if ( NULL == pRouter || NULL == pCM || NULL == pDpOutAdapter ) {
    return EFI_INVALID_PARAMETER;
  }

  if ( !IsTbt3Router (pRouter) ) {
    return EFI_INVALID_PARAMETER;
  }

  if ((Tbt3RouterType (pRouter) == TBT3_TYPE_TR)) {
    Status = AmdUsb4Ring0RMWCS (
               pCM,
               pDpOutAdapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) pDpOutAdapter->Header.AdapterNum,
               (UINT32) pDpOutAdapter->TBT3DpTmuCapabilityOffset + DP_TMU_DP_CS_13,
               1,
               (UINT32*) &(AndMask.Value),
               (UINT32*) &(OrValue.Value)
               );
    if ( EFI_ERROR (Status) ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write TBT3_CS.TMU_DP_CS_13.TMU Stable Register.\n");
      return EFI_DEVICE_ERROR;
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("[%a] - Exit\n", __FUNCTION__);

  return EFI_SUCCESS;
}

BOOLEAN
IsTbt3DpPerformCMHandshake (
  IN     AMD_USB4_CM_ROUTER       *pRouter
  )
{
  if ( !IsTbt3Router (pRouter) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   This is not TBT3 Router. Performing CM handshake.\n");
    return TRUE;
  }

  if ( Tbt3RouterType (pRouter) == TBT3_TYPE_AR) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   AR Router does not support the CM handshake. Skipping.\n");
    return FALSE;
  }

  return TRUE;
}


EFI_STATUS
Tbt3DpInitCMHandshake (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER   *pAdapter
  )
{
  EFI_STATUS                    Status;
  UINT32                        DataSize;
  UINT32                        Data32;
  UINT32                        DpIpData;
  USB4_TIMEOUT                  Timeout;
  AMD_USB4_CM_ROUTER            *pRouter;
  AMD_USB4_TBT3_ADP_DP_IP_ADDR  *AddrPtr;
  AMD_USB4_TBT3_ADP_DP_IP_CMD   *CmdPtr;
  UINT32                        *DataPtr;

  Status    = EFI_SUCCESS;
  DataSize  = 0;
  AddrPtr   = NULL;
  CmdPtr    = NULL;
  DataPtr   = NULL;
  Data32    = 0;
  DpIpData  = 0;
  pRouter   = (AMD_USB4_CM_ROUTER*) pAdapter->Header.Parent;
  ZeroMem (&Timeout, sizeof(USB4_TIMEOUT));

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  if ( !IsTbt3Router (pRouter) ) {
    return EFI_INVALID_PARAMETER;
  }

  if ( Tbt3RouterType (pRouter) != TBT3_TYPE_TR) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Only TR Router needs to perform prerequiste for CM handshake.\n");
    return EFI_UNSUPPORTED;
  }

  //
  // Initialization for CM Handshake shall only be performed for Adapter Numbers 5h, 6h and Ah
  //
  if ( (pAdapter->Header.AdapterNum != 0x5)
    && (pAdapter->Header.AdapterNum != 0x6)
    && (pAdapter->Header.AdapterNum != 0xA))
  {
    return EFI_INVALID_PARAMETER;
  }

  //
  // DP IP Read Sequence
  //

  //
  // 1. Write target address to TBT3_CS.IP_ADDR.Address
  //
  AddrPtr = (AMD_USB4_TBT3_ADP_DP_IP_ADDR*)&Data32;
  AddrPtr->Value          = 0x00000000;
  AddrPtr->Field.Address  = DP_IP_INIT_CMHS_ADDRESS;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) pAdapter->Header.AdapterNum,
             (UINT32) pAdapter->TBT3DpVSCCapabilityOffset + 2,
             &DataSize,
             &Data32
             );
  if (EFI_ERROR (Status) || DataSize != 1) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Failed to write DP Adapter Vendor Specific Capability 1 Register - IP_ADDR.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // 2. Clear TBT3_CS.IP_CMD.CMD bit and set TBT3_CS.IP_CMD.Valid bit with one Configuration Space write access
  //
  CmdPtr = (AMD_USB4_TBT3_ADP_DP_IP_CMD*)&Data32;
  CmdPtr->Value        = 0x00000000;
  CmdPtr->Field.Cmd    = 0x0;
  CmdPtr->Field.Valid  = 0x1;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) pAdapter->Header.AdapterNum,
             (UINT32) pAdapter->TBT3DpVSCCapabilityOffset + 1,
             &DataSize,
             &Data32
             );
  if (EFI_ERROR (Status) || DataSize != 1) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Failed to write DP Adapter Vendor Specific Capability 1 Register - IP_CMD\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // 3. Poll the TBT3_CS.IP_CMD.Valid bit until it is 0b
  //
  StartTimeout (&Timeout, DP_IP_REGISTER_ACCESS_TIMEOUT);
  do {
    DataSize = 1;
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pAdapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) pAdapter->Header.AdapterNum,
               (UINT32) pAdapter->TBT3DpVSCCapabilityOffset + 1,
               DataSize,
               (UINT32**) &DataPtr,
               &DataSize
               );
    if ((EFI_ERROR (Status)) || (DataSize != 1)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Failed to read DP Adapter Vendor Specific Capability 1 Register - IP_CMD\n");
      return EFI_DEVICE_ERROR;
    }

    if (((AMD_USB4_TBT3_ADP_DP_IP_CMD *) DataPtr)->Field.Valid == 0x0) {
      break;
    }

    Status = HasItTimedOut (&Timeout);
  }  while ( Status != EFI_TIMEOUT );

  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Timed out reading DP Adapter Vendor Specific Capability 1 Register - IP_CMD\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // 4. Read data to the TBT3_CS.IP_RDATA field
  //
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
                 pCM,
                 pAdapter->Header.TopologyID,
                 CS_TARGET_ADAPTER,                                 // CsType
                 (UINT32) pAdapter->Header.AdapterNum,              // AdapterNum
                 (UINT32) pAdapter->TBT3DpVSCCapabilityOffset + 4,  // ReadAddress
                 DataSize,                                          // ReadSize
                 (UINT32**) &DataPtr,                               // DataOut
                 &DataSize                                          // DataSize
                 );

  if (EFI_ERROR (Status) || DataSize != 1) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Failed to read DP Adapter Vendor Specific Capability 1 Register - IP_RDATA\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // DP IP Data Modify
  // Before performing the CMHS handshake, a Connection Manager shall Write 0 to bits [3:0] of the DP IP register
  //
  DpIpData  = *DataPtr;
  DpIpData &= ~0xF;

  //
  // DP IP Write Sequence
  //

  //
  // 1. Write target address to TBT3_CS.IP_ADDR.Address
  //
  AddrPtr = (AMD_USB4_TBT3_ADP_DP_IP_ADDR*)&Data32;
  AddrPtr->Value          = 0x00000000;
  AddrPtr->Field.Address  = DP_IP_INIT_CMHS_ADDRESS;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) pAdapter->Header.AdapterNum,
             (UINT32) pAdapter->TBT3DpVSCCapabilityOffset + 2,
             &DataSize,
             &Data32
             );
  if (EFI_ERROR (Status) || DataSize != 1) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Failed to write DP Adapter Vendor Specific Capability 1 Register - IP_ADDR.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // 2. Write data to the TBT3_CS.IP_WDATA field
  //
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) pAdapter->Header.AdapterNum,
             (UINT32) pAdapter->TBT3DpVSCCapabilityOffset + 3,
             &DataSize,
             &DpIpData
             );
  if (EFI_ERROR (Status) || DataSize != 1) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Failed to write DP Adapter Vendor Specific Capability 1 Register - IP_WDATA\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // 3. Set TBT3_CS.IP_CMD.CMD bit and TBT3_CS.IP_CMD.Valid bit with one Configuration Space write access
  //
  CmdPtr = (AMD_USB4_TBT3_ADP_DP_IP_CMD*)&Data32;
  CmdPtr->Value        = 0x00000000;
  CmdPtr->Field.Cmd    = 0x1;
  CmdPtr->Field.Valid  = 0x1;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) pAdapter->Header.AdapterNum,
             (UINT32) pAdapter->TBT3DpVSCCapabilityOffset + 1,
             &DataSize,
             &Data32
             );
  if (EFI_ERROR (Status) || DataSize != 1) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Failed to write DP Adapter Vendor Specific Capability 1 Register - IP_CMD\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // 4. Poll the TBT3_CS.IP_CMD.Valid bit until it is 0b
  //
  StartTimeout (&Timeout, DP_IP_REGISTER_ACCESS_TIMEOUT);
  do {
    DataSize = 1;
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pAdapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) pAdapter->Header.AdapterNum,
               (UINT32) pAdapter->TBT3DpVSCCapabilityOffset + 1,
               DataSize,
               (UINT32**) &DataPtr,
               &DataSize
               );
    if ((EFI_ERROR (Status)) || (DataSize != 1)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Failed to read DP Adapter Vendor Specific Capability 1 Register - IP_CMD\n");
      return EFI_DEVICE_ERROR;
    }

    if (((AMD_USB4_TBT3_ADP_DP_IP_CMD *) DataPtr)->Field.Valid == 0x0) {
      break;
    }

    Status = HasItTimedOut (&Timeout);
  }  while ( Status != EFI_TIMEOUT );

  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Timed out reading DP Adapter Vendor Specific Capability 1 Register - IP_CMD\n");
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}


EFI_STATUS
Tbt3DpDisableLTTPRSupport (
  IN     AMD_USB4_CM              *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER   *pDpOut
  )
{
  EFI_STATUS                Status;
  AMD_USB4_CM_ROUTER        *pRouter;
  UINT8                     Data8[16];
  UINT8                     NVMVersion;
  UINT32                    Offset;
  UINT32                    CurrentDromOffset;
  UINT32                    DromReadSize;

  Status             = EFI_SUCCESS;
  pRouter            = (AMD_USB4_CM_ROUTER *) pDpOut->Header.Parent;
  Offset             = 0;
  CurrentDromOffset  = 0;
  DromReadSize       = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  ZeroMem (Data8, sizeof(Data8));

  if ( !IsTbt3Router (pRouter) ) {
    return EFI_INVALID_PARAMETER;
  }

  if ( Tbt3RouterType (pRouter) != TBT3_TYPE_TR) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Only TR Router needs to Disable LTTPR Support.\n");
    return EFI_UNSUPPORTED;
  }

  //
  // Read the NVM version by issuing an NVM Read command from byte offset Ah
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("pRouter->pAdapterHostInterface = 0x%x\n", pRouter->pAdapterHostInterface);
  IDS_HDT_CONSOLE_USB4_ASSERT (pRouter->pAdapterHostInterface != NULL);
  Status = TBT3ReadDrom (
             pCM,
             (AMD_USB4_CM_HI_ADAPTER*) pRouter->pAdapterHostInterface,
             8,
             4,
             (UINT32*)Data8
             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to Read TBT3 Mailbox Read. Status: %r\n", Status);
    return EFI_DEVICE_ERROR;
  }

  //
  // If the NVM version is lower than 62h, set DP OUT DP_LOCAL_CAP.LTTPR Not Supported
  //
  NVMVersion = Data8[2];
  IDS_HDT_CONSOLE_USB4_INFO ("NVM Version: %x\n", NVMVersion);

  if (NVMVersion < 0x62) {
    pDpOut->LTTPRNotSupported = 1;
    IDS_HDT_CONSOLE_USB4_INFO ("DP Out Adapter's LTTPRNotSupported set to 0x1: %x\n", pDpOut->LTTPRNotSupported);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
Tbt3SetDpNonFlowControlBuffers (
  IN     AMD_USB4_CM                          *pCM,
  IN     AMD_USB4_CM_DP_ADAPTER               *pDpAdapter,
  IN     UINT32                               BufferAmount,
  IN     BOOLEAN                              IsDpMainPath

  )
{
  EFI_STATUS                Status;
  AMD_USB4_CM_ROUTER        *pRouter;
  UINT32                    DataSize;
  UINT32                    *DataPtr;

  Status                  = EFI_SUCCESS;
  pRouter                 = (AMD_USB4_CM_ROUTER *) pDpAdapter->Header.Parent;
  DataSize                = 0;
  DataPtr                 = 0;

  //
  // Flow Control Disabled Scheme for DP Main Path
  //
  if (IsDpMainPath) {
    DataSize = 1;
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ADAPTER,                       // CsType
               (UINT32) pDpAdapter->Header.AdapterNum,  // AdapterNum
               4,                                       // ReadAddress
               DataSize,                                // ReadSize
               (UINT32**) &DataPtr,                     // DataOut
               &DataSize                                // DataSize
               );
    if ((EFI_ERROR (Status)) || (DataSize != 1)) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Failed to read AMD_USB4_ADP_DP_OUT_STATUS_CTRL Register.\n");
      return EFI_DEVICE_ERROR;
    }

    ((AMD_USB4_ADP_CS4 *) DataPtr)->Field.NonFlowControlledBuffers = BufferAmount;

    Status = AmdUsb4Ring0WriteCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) pRouter->UpstreamAdapter,
               4,
               &DataSize,
               DataPtr
               );
    if (EFI_ERROR (Status) || DataSize != 1) {
      IDS_HDT_CONSOLE_USB4_DEBUG (" Failed to write AMD_USB4_ADP_CS4.NonFlowControlledBuffers Register.\n");
      return EFI_DEVICE_ERROR;
    }
  }

  return EFI_SUCCESS;
}

/**
 * @brief   Disable TMU by writing 0b to TMU_RTR_CS_3.TSPacketInterval.
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    *pRouter
 *
 * @return  EFI_STATUS
 */
EFI_STATUS
Tbt3RouterTmuDisable (
  IN     AMD_USB4_CM          *pCM,
  IN     AMD_USB4_CM_ROUTER   *pRouter
  )
{
  EFI_STATUS                Status;
  UINT32                    TMUCapability;
  UINT32                    AndMask;
  UINT32                    OrValue;

  Status        = EFI_SUCCESS;
  TMUCapability = 0;
  AndMask       = 0x0000FFFF;
  OrValue       = 0x00000000;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  if ( NULL == pRouter || NULL == pCM ) {
    return EFI_INVALID_PARAMETER;
  }

  if ( !IsTbt3Router (pRouter) ) {
    return EFI_INVALID_PARAMETER;
  }

  TMUCapability = Tbt3GetRouterCapability (
                    pCM,
                    pRouter,
                    TMU_ROUTER_CONFIGURATION_CAPABILITY_ID
                    );
  IDS_HDT_CONSOLE_USB4_DEBUG (" TMUCapability = 0x%x.\n", TMUCapability);
  if ( 0 == TMUCapability ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (" Do not find TMU. Quit.\n");
    return EFI_NOT_FOUND;
  }

  pRouter->TMUCapability = (UINT8)TMUCapability;

  IDS_HDT_CONSOLE_USB4_DEBUG (" Disable TMU.\n");
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,                                        // CsType
             0,                                                       // AdapterNum
             TMUCapability + TMU_RTR_CS_3,                            // Address
             1,                                                       // SizeInDW
             &AndMask,                                                // AndMask
             &OrValue                                                 // OrValue
             );
  IDS_HDT_CONSOLE_USB4_DEBUG (" Status = %r\n", Status);

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Enable Tbt3 TMU
 *
 * @param[in]  *pCm        Point to global structure AMD_USB4_CM
 * @param[in]  *pRouter    Point to Current Router
 *
 * @retval EFI_SUCCESS                Success.
 * @retval EFI_DEVICE_ERROR           Failed.
 * @retval EFI_INVALID_PARAMETER      Invalid Parameter
**/
EFI_STATUS
Tbt3RouterTmuEnable (
  IN     AMD_USB4_CM          *pCM,
  IN     AMD_USB4_CM_ROUTER   *pRouter
  )
{
  EFI_STATUS                Status;
  UINT32                    AndMask;
  UINT32                    OrValue;
  AMD_USB4_CM_LANE_ADAPTER  *pRouterUFP;
  AMD_USB4_CM_LANE_ADAPTER  *pParentDFP;

  Status        = EFI_SUCCESS;
  AndMask       = (UINT32)~BIT27;
  OrValue       = BIT27;
  pRouterUFP    = NULL;
  pParentDFP    = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  if ( NULL == pRouter  ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   pRouter is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( !IsTbt3Router (pRouter) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   This is not TBT3 Router.\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( TS_PACKET_INTERVAL_HIFI == pRouter->TMUInfo.TSPacketInterval
    && 0 == pRouter->TMUInfo.UniDirectionalMode ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   No need to set TMU\n");
    return EFI_SUCCESS;
  }

  pParentDFP = (AMD_USB4_CM_LANE_ADAPTER*)pRouter->Header.Parent;
  pRouterUFP = (AMD_USB4_CM_LANE_ADAPTER*)pRouter->pAdapterUFP;

  // 1. Set TMU_RTR_CS_0.Time Disruption to 1
  AndMask = (UINT32)~BIT27;
  OrValue = BIT27;
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,                                    // CsType
             0,                                                   // AdapterNum
             (UINT32)(pRouter->TMUCapability),                    // Address
             1,                                                   // SizeInDW
             &AndMask,                                            // AndMask
             &OrValue                                             // OrValue
             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "   Failed to set Router (0x%lx) TMU_RTR_CS_0.Time Disruption (TD) to 1.\n",
      pRouter->Header.TopologyID
      );
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Set Router (0x%lx) TMU_RTR_CS_0.Time Disruption (TD) to 1.\n",
    pRouter->Header.TopologyID
    );

  // 2. Set Parent Router DFP to bi-directional mode
  AndMask = (UINT32)~BIT29;
  OrValue = 0;
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pParentDFP->Header.TopologyID,
             CS_TARGET_ADAPTER,                                   // CsType
             pParentDFP->Header.AdapterNum,                       // AdapterNum
             (UINT32)(pParentDFP->TMUCapability + 3),             // Address
             1,                                                   // SizeInDW
             &AndMask,                                            // AndMask
             &OrValue                                             // OrValue
             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "   Failed to set Parent Router (0x%lx) DFP (0x%lx) to Bi-Directional mode.\n",
      pParentDFP->Header.TopologyID,
      pParentDFP
      );
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Set Parent Router (0x%lx) DFP (0x%lx) to Bi-Directional mode.\n",
    pParentDFP->Header.TopologyID,
    pParentDFP
    );

  // 3. Set Current Router to HIFI mode
  AndMask = 0xFFFF;
  OrValue = 16 << 16;
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,                                     // CsType
             0,                                                    // AdapterNum
             (UINT32)(pRouter->TMUCapability + 3),                 // Address
             1,                                                    // SizeInDW
             &AndMask,                                             // AndMask
             &OrValue                                              // OrValue
             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to set current Router (0x%lx) to HiFi mode.\n", pRouter->Header.TopologyID);
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Set current Router (0x%lx) to HiFi mode.\n", pRouter->Header.TopologyID);

  // 4. Set Current Router UFP to bi-directional mode
  AndMask = (UINT32)~BIT29;
  OrValue = 0;
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pRouterUFP->Header.TopologyID,
             CS_TARGET_ADAPTER,                                    // CsType
             pRouterUFP->Header.AdapterNum,                        // AdapterNum
             (UINT32)(pRouterUFP->TMUCapability + 3),              // Address
             1,                                                    // SizeInDW
             &AndMask,                                             // AndMask
             &OrValue                                              // OrValue
             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "   Failed to set Current Router (0x%lx) UFP (0x%lx) to Bi-Directional mode.\n",
      pRouterUFP->Header.TopologyID, pRouterUFP
      );
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Set Parent Router (0x%lx) UFP (0x%lx) to Bi-Directional mode.\n",
    pRouterUFP->Header.TopologyID, pRouterUFP
    );

  // 5. Set Current Router to HIFI mode again
  AndMask = 0xFFFF;
  OrValue = 16 << 16;
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,                                     // CsType
             0,                                                    // AdapterNum
             (UINT32)(pRouter->TMUCapability + 3),                 // Address
             1,                                                    // SizeInDW
             &AndMask,                                             // AndMask
             &OrValue                                              // OrValue
             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to set current Router (0x%lx) to HiFi mode.\n", pRouter->Header.TopologyID);
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Set current Router (0x%lx) to HiFi mode.\n", pRouter->Header.TopologyID);

  // 6. Set TMU_RTR_CS_0.Time Disruption to 0
  AndMask = (UINT32)~BIT27;
  OrValue = 0;
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,                                    // CsType
             0,                                                   // AdapterNum
             (UINT32)(pRouter->TMUCapability),                    // Address
             1,                                                   // SizeInDW
             &AndMask,                                            // AndMask
             &OrValue                                             // OrValue
             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "   Failed to set Router (0x%lx) TMU_RTR_CS_0.Time Disruption (TD) to 0.\n",
      pRouter->Header.TopologyID
      );
    return EFI_DEVICE_ERROR;
  }
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Set Router (0x%lx) TMU_RTR_CS_0.Time Disruption (TD) to 0.\n",
    pRouter->Header.TopologyID
    );

  // 7. Update Router Structure Data.
  pRouter->TMUInfo.UniDirectionalMode = 0;
  pRouter->TMUInfo.TSPacketInterval   = TS_PACKET_INTERVAL_HIFI;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);

  return EFI_SUCCESS;
}

VOID
Tbt3RouterUpdateUDM (
  IN     AMD_USB4_CM_ROUTER   *pRouter,
  IN     UINT32               AdapterNum,
  IN     UINT32               EnableUniDirectionalMode
  )
{
  if ( pRouter->UpstreamAdapter == AdapterNum ) {
    pRouter->TMUInfo.UniDirectionalMode = EnableUniDirectionalMode;
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Router (0x%lx) TMUInfo.UniDirectionalMode = %d\n",
      pRouter->Header.TopologyID,
      pRouter->TMUInfo.UniDirectionalMode
      );
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Setup Tbt3
 *
 * @param[in]  *pCm                   Point to global structure AMD_USB4_CM
 * @param[in]  *pRouter    Point to Parent Pcie Downstream Adapter
 *
 * @retval EFI_SUCCESS           Success.
 * @retval EFI_DEVICE_ERROR      Failed.  Router return Error Event code.
 *                                        WriteData[0] will have the Event code.
 *
 * @retval EFI_NOT_READY         Failed.  Something wrong in Transmit or Receive routine.
 * @retval EFI_NO_RESPONSE       Failed   Send out WRITE_REQUEST packet, but do not get paired WRITE_RESPONSE or NOTIFICATION.
 * @retval EFI_INVALID_PARAMETER Failed   Input parameter is not good.
**/
EFI_STATUS
Tbt3RouterProvision (
  IN     AMD_USB4_CM          *pCM,
  IN     AMD_USB4_CM_ROUTER   *pRouter
  )
{
  EFI_STATUS              Status;
  UINT32                  DataSize;
  UINT32                  *Ptr32;

  Status      = EFI_SUCCESS;
  DataSize    = 0;
  Ptr32       = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  if ( NULL == pRouter ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (" pRouter is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( !IsTbt3Router (pRouter) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      " Router (0x%lx) is a USB4 Router. Exit Tbt3RouterProvision.\n",
      pRouter->Header.TopologyID
      );
    return EFI_UNSUPPORTED;
  }

  // Get VSC1, VSC3, VSC4 and VESC address in DW.
  pRouter->VSCapability[VSC_ID_1] = (UINT8)Tbt3GetRouterVSCOffset (
                                             pCM,
                                             pRouter,
                                             pRouter->CapabilityOffset,
                                             VENDOR_SPECIFIC_CAPABILITY_ID,
                                             VSC_ID_1
                                             );
  IDS_HDT_CONSOLE_USB4_DEBUG (" VSC1 Offset: %d\n", pRouter->VSCapability[VSC_ID_1]);

  pRouter->VSCapability[VSC_ID_3] = (UINT8)Tbt3GetRouterVSCOffset (
                                             pCM,
                                             pRouter,
                                             pRouter->CapabilityOffset,
                                             VENDOR_SPECIFIC_CAPABILITY_ID,
                                             VSC_ID_3
                                             );
  IDS_HDT_CONSOLE_USB4_DEBUG (" VSC3 Offset: %d\n", pRouter->VSCapability[VSC_ID_3]);

  pRouter->VSCapability[VSC_ID_4] = (UINT8)Tbt3GetRouterVSCOffset (
                                             pCM,
                                             pRouter,
                                             pRouter->CapabilityOffset,
                                             VENDOR_SPECIFIC_CAPABILITY_ID,
                                             VSC_ID_4
                                             );
  IDS_HDT_CONSOLE_USB4_DEBUG (" VSC4 Offset: %d\n", pRouter->VSCapability[VSC_ID_4]);

  pRouter->VSECapability = (UINT16)Tbt3GetRouterVSCOffset (
                                     pCM,
                                     pRouter,
                                     pRouter->CapabilityOffset,
                                     VENDOR_SPECIFIC_CAPABILITY_ID,
                                     VESC_ID_6
                                     );
  IDS_HDT_CONSOLE_USB4_DEBUG (" VESC Offset: %d\n", pRouter->VSECapability);
  IDS_HDT_CONSOLE_USB4_ASSERT (pRouter->VSECapability != 0);
  // Get DROM Base
  pRouter->Tbt3DROMBase = 0;    // Zero means no DROM base.
  if ( 0 != pRouter->VSCapability[VSC_ID_1] ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (" Read Router VSC1.CS12\n");
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ROUTER,
               0,                                                     // AdapterNum
               pRouter->VSCapability[VSC_ID_1] + ROUTER_VSC_1_CS_12, // ReadAddress
               1,                                                     // ReadSize 1 DWs
               (UINT32**)&Ptr32,                                      // DataOut
               &DataSize                                              // DataSize
               );
    IDS_HDT_CONSOLE_USB4_DEBUG (" Status = %r, DataSize = %d\n", Status, DataSize);
    if ( !EFI_ERROR (Status) && ( 1 == DataSize ) ) {
      pRouter->Tbt3DROMBase = ( (*Ptr32) > 0xFFFF ) ? 0 : (*Ptr32);
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (" Tbt3DROMBase = %d\n", pRouter->Tbt3DROMBase);

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);

  return EFI_SUCCESS;
}


/**
 * @brief   Enable Tbt3 internal XHCI controller.
 *
 * @param   AMD_USB4_CM           *pCM
 * @param   AMD_USB4_CM_ROUTER    **pRouter
 * @param   UINT32                Router Amount
 *
 * @return  VOID
 */
VOID
Tbt3EnablexHC (
  IN     AMD_USB4_CM                          *pCM,
  IN     AMD_USB4_CM_ROUTER                   **pRouterArray,
  IN     UINT32                                RouterAmount
  )
{
  EFI_STATUS          Status;
  AMD_USB4_CM_ROUTER  *pTbt3Router;
  UINT32              i;
  UINT32              j;
  UINT32              *Ptr32;
  UINT32              DataSize;
  UINT32              PortAmount;
  UINT32              AndMask;
  UINT32              OrValue;

  Status      = EFI_SUCCESS;
  pTbt3Router = NULL;
  i           = 0;
  j           = 0;
  Ptr32       = NULL;
  DataSize    = 0;
  PortAmount  = 0;
  AndMask     = 0;
  OrValue     = 0;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry (%d)\n", __FUNCTION__, RouterAmount);

  for (i = 0; i < RouterAmount; i++) {
    pTbt3Router = pRouterArray[i];
    if ( IsTbt3Router (pTbt3Router) && ( TBT3_TYPE_TR == Tbt3RouterType(pTbt3Router) ) ) {
      DataSize    = 0;
      Status = AmdUsb4Ring0ReadCS (
                 pCM,
                 pTbt3Router->Header.TopologyID,
                 CS_TARGET_ROUTER,
                 0,                                                     // AdapterNum
                 pTbt3Router->VSECapability + 2,
                 1,                                                     // ReadSize 1 DWs
                 (UINT32**)&Ptr32,                                      // DataOut
                 &DataSize                                              // DataSize
                 );
      if ( EFI_ERROR (Status) || 1 != DataSize ) {
        IDS_HDT_CONSOLE_USB4_ERROR ("Get Port Number failed\n");
        continue;
      } else {
        PortAmount = (*Ptr32) & 0xF;
      }

      IDS_HDT_CONSOLE_USB4_DEBUG ("PortAmount = %d\n", PortAmount);

      for (j = 0; j < PortAmount; j++) {
        IDS_HDT_CONSOLE_USB4_DEBUG (" Set xHC Connect to 1\n");
        AndMask = (UINT32)(~XHC_CONNECT);
        OrValue = (UINT32)XHC_CONNECT;
        Status = AmdUsb4Ring0RMWCS (
                   pCM,
                   pTbt3Router->Header.TopologyID,
                   CS_TARGET_ROUTER,
                   0,                                                     // AdapterNum
                   (UINT32)(pTbt3Router->VSECapability) +            \
                     COMMON_REGION_LENGTH + j * PORT_REGION_LENGTH + \
                     VSEC_6_PORT_CS_173,
                   1,                                                     // ReadSize 1 DWs
                   &AndMask,                                              // AndMask
                   &OrValue                                               // OrValue
                   );
        IDS_HDT_CONSOLE_USB4_DEBUG (" Set xHC Connect to 1 status = %r\n", Status);
      }
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);
}


EFI_STATUS
Tbt3DFPReset (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pLaneAdapter
  )
{
  EFI_STATUS  Status;
  UINT32      AndMask;
  UINT32      OrValue;

  Status    = EFI_SUCCESS;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);
  IDS_HDT_CONSOLE_USB4_ASSERT (pLaneAdapter->Usb4Capability != 0);

  AndMask = (UINT32)(~PORT_DPR);
  OrValue = (UINT32)PORT_DPR;
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             pLaneAdapter->Usb4Capability + VSEC_6_PORT_CS_38,
             1,                                                     // ReadSize 1 DWs
             &AndMask,                                              // AndMask
             &OrValue                                               // OrValue
             );
  if (EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write PORT_MODE.DPR to 1.\n");
    return Status;
  }

  //
  // Wait for 10ms.
  // todo: Update delay to 10ms
  //
  MicroSecondDelay (DOWNSTREAM_PORT_RESET_WAIT); //10ms

  AndMask = (UINT32)(~PORT_DPR);
  OrValue = 0;
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             pLaneAdapter->Usb4Capability + VSEC_6_PORT_CS_38,
             1,                                                     // ReadSize 1 DWs
             &AndMask,                                              // AndMask
             &OrValue                                               // OrValue
             );
  if (EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write PORT_MODE.DPR to 0.\n");
    return Status;
  }

  StartTimeout (&(pLaneAdapter->PMTimer), LANE_ADP_DFP_RESET_WAIT);
  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);
  return Status;
}


EFI_STATUS
Tbt3RouterEnumeration (
  IN     AMD_USB4_CM                          *pCM,
  IN     AMD_USB4_CM_ROUTER                   *pRouter,
  IN     AMD_USB4_ROUTER_CONFIGURATION_SPACE  *pRouterCs
  )
{
  EFI_STATUS  Status;
  UINT32      *Ptr32;
  UINT32      Data32[4];
  UINT32      DataSize;

  Status    = EFI_SUCCESS;
  Ptr32     = NULL;
  DataSize  = 0;

  ZeroMem (Data32, sizeof (Data32));

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  if ( !IsTbt3Router (pRouter) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (" This is not TBT3 Router.\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( TBT3_TYPE_UNKNOWN == Tbt3RouterType (pRouter) ) {
    // In TBT3-Compatibility Guide for USB4 Connection Managers Chapter 3.2.1,
    // if the TBT3 router is not AR or TR type, CM should ignore this router.
    IDS_HDT_CONSOLE_USB4_ERROR ("Unsupported TBT3 Router.\n");
    return EFI_UNSUPPORTED;
  }

  // Update TBT3 Upstream adapter
  Ptr32 = (UINT32*)pRouterCs;
  Ptr32--;
  IDS_HDT_CONSOLE_USB4_ASSERT (Ptr32 != 0);
  pRouter->UpstreamAdapter  = (UINT8)(((AMD_USB4_READ_WRITE_HEADER*)Ptr32)->Field.AdapterNum);

  IDS_HDT_CONSOLE_USB4_DEBUG (" Updated TBT3 value:\n");
  IDS_HDT_CONSOLE_USB4_DEBUG ("  TBT3Support     = 0x%x\n", pRouter->TBT3Support);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  InternalHost    = 0x%x\n", pRouter->InternalHost);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  InternalHostOn  = 0x%x\n", pRouter->InternalHostOn);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  UpstreamAdapter = 0x%x\n", pRouter->UpstreamAdapter);

  // Save Router CS DW [3:1] to Data32 because we need disable TMU before enumerate TBT3 router.
  pRouterCs->RouterCs1.Field.UpstreamAdapter     = pRouter->UpstreamAdapter;
  pRouterCs->RouterCs1.Field.Depth               = pRouter->Depth;
  pRouterCs->TopologyIDLow                       = (UINT32)(pRouter->Header.TopologyID);
  pRouterCs->RouterCs3.Field.TopologyIDHigh      = (UINT32)((pRouter->Header.TopologyID)>>32);
  pRouterCs->RouterCs3.Field.TopologyIDValid     = 1;
  pRouterCs->RouterCs4.Field.NotificationTimeout = 0xFE;
  Data32[0] = pRouterCs->RouterCs1.Value;
  Data32[1] = pRouterCs->TopologyIDLow;
  Data32[2] = pRouterCs->RouterCs3.Value;
  Data32[3] = pRouterCs->RouterCs4.Value;

  // Disable the TMU by writing 0b to the TMU_RTR_CS_3.TSPacketInterval field.
  Status = Tbt3RouterTmuDisable (pCM, pRouter);
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to enumerate TBT3 router.\n");
    return EFI_DEVICE_ERROR;
  }

  // Enumerate TBT3 router
  DataSize = 4;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             1,
             &DataSize,
             Data32
             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to enumerate TBT3 router.\n");
    return EFI_DEVICE_ERROR;
  }

  // Handle Tbt3 stuff
  Tbt3RouterProvision (pCM, pRouter);

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Exit\n", __FUNCTION__);
  return EFI_SUCCESS;
}


EFI_STATUS
Tbt3FindHostInterfaceAdapter (
  IN     AMD_USB4_CM             *pCM,
  IN     AMD_USB4_CM_ROUTER      *pRouter,
  IN     AMD_USB4_CM_HI_ADAPTER  *pHIAdapter
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

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Entry\n", __FUNCTION__);

  if ((pCM == NULL) || (pRouter == NULL) || (pHIAdapter == NULL) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Invalid input parameters\n");
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (pHIAdapter, sizeof (AMD_USB4_CM_HI_ADAPTER));

  for (AdapterNumber = 1; AdapterNumber <= (UINT32)pRouter->MaxAdapter; AdapterNumber++) {
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

    if ( !EFI_ERROR (Status)
      && ADAPTER_BASE_CS_SIZE_DW == DataSize
      && ((*(UINT32*)&(pAdapterCs->AdapterTypeSubType)) & 0xFFFFFF) == HostInterfaceAdapter )
    {
      pHIAdapter->Header.TopologyID = pRouter->Header.TopologyID;
      pHIAdapter->Header.CellType   = CELL_TYPE_HI_ADAPTER;
      pHIAdapter->Header.AdapterNum = (UINT8)AdapterNumber;
      pHIAdapter->CapabilityOffset  = pAdapterCs->AdpCs1.Field.NextCapabilityPointer;
      pHIAdapter->VSC1Offset        = (UINT8)Tbt3GetAdapterVSCOffset (
                                               pCM,
                                               (AMD_USB4_CM_CELL*)pHIAdapter,
                                               (UINT8)pHIAdapter->CapabilityOffset,
                                               VENDOR_SPECIFIC_CAPABILITY_ID,
                                               VSC_ID_1
                                               );
      IDS_HDT_CONSOLE_USB4_DEBUG (" Find TBT3 HI adapter:\n");
      IDS_HDT_CONSOLE_USB4_DEBUG ("  AdapterNum       = 0x%x\n", pHIAdapter->Header.AdapterNum);
      IDS_HDT_CONSOLE_USB4_DEBUG ("  CellType         = 0x%x\n", pHIAdapter->Header.CellType);
      IDS_HDT_CONSOLE_USB4_DEBUG ("  CapabilityOffset = 0x%x\n", pHIAdapter->CapabilityOffset);
      IDS_HDT_CONSOLE_USB4_DEBUG ("  VSC1Offset       = 0x%x\n", pHIAdapter->VSC1Offset);

      break;
    }
  }

  if ( pHIAdapter->VSC1Offset ) {
    pRouter->pAdapterHostInterface = (AMD_USB4_CM_CELL*)pHIAdapter;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

