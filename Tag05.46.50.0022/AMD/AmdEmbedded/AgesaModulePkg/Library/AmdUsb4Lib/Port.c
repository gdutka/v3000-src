/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include "AmdUsb4.h"
#include <AmdUsb4/ConfigurationSpace.h>
#include <AmdUsb4/ControlPacket.h>
#include <Library/AmdUsb4Lib.h>
#include <Library/UefiLib.h>
#include <Library/TimerLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IdsLib.h>
#include <Library/BaseMemoryLib.h>

#define FILECODE LIBRARY_AMDUSB4LIB_PORT_FILECODE

EFI_STATUS
AmdUsb4SBRead (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pLaneAdapter,
  IN     UINT32                     Target,
  IN     UINT32                     RetimerIndex,
  IN     UINT32                     Address,
  IN     UINT32                     Length,
     OUT UINT32                     *pLengthOut,
     OUT UINT32                     **pDataOut32
  )
{
  EFI_STATUS        Status;
  AMD_USB4_PORT_CS1 PortCs1;
  UINT32            DataSize;
  UINT32            AndMask;
  UINT32            OrValue;
  UINT32            *Ptr32;

  Status        = EFI_SUCCESS;
  PortCs1.Value = 0;
  DataSize      = 0;
  AndMask       = 0;
  OrValue       = 0;
  Ptr32         = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  // 1. Write Target, Address, WnR, LEN, Index
  IDS_HDT_CONSOLE_USB4_DEBUG ("Write SB Target, Address, WnR, LEN, Index\n");
  PortCs1.Field.Address       = Address;                  // Address
  PortCs1.Field.Length        = Length << 2;              // Length (size in bytes)
  PortCs1.Field.Target        = Target;                   // Target
  PortCs1.Field.RetimerIndex  = RetimerIndex;             // Re-timer Index
  PortCs1.Field.WnR           = 0;                        // Write or Read
  PortCs1.Field.NoResponse    = 0;                        // No Response
  PortCs1.Field.ResultCode    = 0;                        // Result Code
  PortCs1.Field.Pending       = 0;                        // Pending
  DataSize                    = 1;

  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             pLaneAdapter->Header.AdapterNum,
             pLaneAdapter->Usb4Capability + 1,
             &DataSize,
             &(PortCs1.Value)
             );
  if ( EFI_ERROR(Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to Write SB Target, Address, WnR, LEN, Index\n");
    return EFI_DEVICE_ERROR;
  }

  // 2. Then sets the Pending bit to 1b
  //    which causes the Router to issue a SB Register access
  AndMask = 0x7FFFFFFF;
  OrValue = 0x80000000;
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             pLaneAdapter->Header.AdapterNum,
             pLaneAdapter->Usb4Capability + 1,  // PORT_CS_1
             1,
             &AndMask,
             &OrValue
             );
  if ( EFI_ERROR(Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to sets the Pending bit to 1b\n");
    return EFI_DEVICE_ERROR;
  }

  // 3. Polling Pending bit
  IDS_HDT_CONSOLE_USB4_DEBUG ("Start to Polling Pending bit flip\n");
  Status = AmdUsb4ReadBitTimeout (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             pLaneAdapter->Header.AdapterNum,
             pLaneAdapter->Usb4Capability + 1,
             BIT31,
             0,
             ROUTER_READY_TIMEOUT_US
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Get Polling Pending bit flip return status %r\n", Status);
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to get Pending bit flip.\n");

    // Clear PORT_CS_1 before exit
    IDS_HDT_CONSOLE_USB4_DEBUG ("Clear PORT_CS_1 before exit.\n");
    DataSize      = 1;
    PortCs1.Value = 0;
    AmdUsb4Ring0WriteCS (
      pCM,
      pLaneAdapter->Header.TopologyID,
      CS_TARGET_ADAPTER,
      pLaneAdapter->Header.AdapterNum,
      pLaneAdapter->Usb4Capability + 1,
      &DataSize,
      &(PortCs1.Value)
      );
    return Status;
  }

  // 5. Get result
  DataSize = 0;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                 // CsType
             pLaneAdapter->Header.AdapterNum,   // AdapterNum
             pLaneAdapter->Usb4Capability + 1,  // ReadAddress
             17,                                // ReadSize 17 DWs
             (UINT32**) &Ptr32,                 // DataOut
             &DataSize                          // DataSize
             );
  if ((EFI_ERROR (Status)) || (17 != DataSize) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read Adapter Configuration Space Register.\n");
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "PORT_CS_1 = 0x%x "
    "ResultCode = 0x%x, NoResponse = 0x%x, Length = 0x%x\n",
    ((AMD_USB4_PORT_CS1*)Ptr32)->Value,
    ((AMD_USB4_PORT_CS1*)Ptr32)->Field.ResultCode,
    ((AMD_USB4_PORT_CS1*)Ptr32)->Field.NoResponse,
    ((AMD_USB4_PORT_CS1*)Ptr32)->Field.Length
    );

  if ( ((AMD_USB4_PORT_CS1*)Ptr32)->Field.ResultCode == 0
    && ((AMD_USB4_PORT_CS1*)Ptr32)->Field.NoResponse == 0
    && ((AMD_USB4_PORT_CS1*)Ptr32)->Field.Length == (Length << 2) )
  {
    *pLengthOut = (((AMD_USB4_PORT_CS1*)Ptr32)->Field.Length) >> 2;

    IDS_HDT_CONSOLE_USB4_DEBUG ("Ptr32 = 0x%x\n", Ptr32);
    *pDataOut32 = ++Ptr32;
    IDS_HDT_CONSOLE_USB4_DEBUG ("*pDataOut32 = 0x%x\n", *pDataOut32);

    IDS_HDT_CONSOLE_USB4_DEBUG ("pLengthOut = 0x%x\n", *pLengthOut);
    IDS_HDT_CONSOLE_USB4_DEBUG ("completes successfully.\n");

    return EFI_SUCCESS;
  } else {
    IDS_HDT_CONSOLE_USB4_ERROR ("completes unsuccessfully.\n");
    return EFI_PROTOCOL_ERROR;
  }
}


EFI_STATUS
AmdUsb4SBWrite (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pLaneAdapter,
  IN     UINT32                     Target,
  IN     UINT32                     RetimerIndex,
  IN     UINT32                     Address,
  IN     UINT32                     Length,
  IN     UINT32                     *pDataIn32
  )
{
  EFI_STATUS        Status;
  AMD_USB4_PORT_CS1 PortCs1;
  UINT32            DataSize;
  UINT32            AndMask;
  UINT32            OrValue;
  UINT32            *Ptr32;

  Status        = EFI_SUCCESS;
  PortCs1.Value = 0;
  DataSize      = 0;
  AndMask       = 0;
  OrValue       = 0;
  Ptr32         = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  // 1. Write Target, Address, WnR, LEN, Index
  IDS_HDT_CONSOLE_USB4_DEBUG ("Write SB Target, Address, WnR, LEN, Index\n");
  PortCs1.Field.Address       = Address;                  // Address
  PortCs1.Field.Length        = Length << 2;              // Length (size in bytes)
  PortCs1.Field.Target        = Target;                   // Target
  PortCs1.Field.RetimerIndex  = RetimerIndex;             // Re-timer Index
  PortCs1.Field.WnR           = 1;                        // Write or Read
  PortCs1.Field.NoResponse    = 0;                        // No Response
  PortCs1.Field.ResultCode    = 0;                        // Result Code
  PortCs1.Field.Pending       = 0;                        // Pending
  DataSize                    = 1;

  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             pLaneAdapter->Header.AdapterNum,
             pLaneAdapter->Usb4Capability + 1,
             &DataSize,
             &(PortCs1.Value)
             );
  if ( EFI_ERROR(Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to Write SB Target, Address, WnR, LEN, Index\n");
    return EFI_DEVICE_ERROR;
  }

  // 2. Write Data
  if ( Length > 0 && Length <= 16 && pDataIn32 != NULL ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Write SB Data\n");
    DataSize = Length;
    Status = AmdUsb4Ring0WriteCS (
               pCM,
               pLaneAdapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               pLaneAdapter->Header.AdapterNum,
               pLaneAdapter->Usb4Capability + 2,
               &DataSize,
               pDataIn32
               );
    if ( EFI_ERROR(Status) ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to Write SB Data\n");
      return EFI_DEVICE_ERROR;
    }
  }

  // 3. Then sets the Pending bit to 1b
  //    which causes the Router to issue a SB Register access
  AndMask = 0x7FFFFFFF;
  OrValue = 0x80000000;
  Status = AmdUsb4Ring0RMWCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             pLaneAdapter->Header.AdapterNum,
             pLaneAdapter->Usb4Capability + 1,  // PORT_CS_1
             1,
             &AndMask,
             &OrValue
             );
  if ( EFI_ERROR(Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to sets the Pending bit to 1b\n");
    return EFI_DEVICE_ERROR;
  }

  // 4. Polling Pending bit
  IDS_HDT_CONSOLE_USB4_DEBUG ("Start to Polling Pending bit flip\n");
  Status = AmdUsb4ReadBitTimeout (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             pLaneAdapter->Header.AdapterNum,
             pLaneAdapter->Usb4Capability + 1,
             BIT31,
             0,
             ROUTER_READY_TIMEOUT_US
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("Get Polling Pending bit flip return status %r\n", Status);
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to get Pending bit flip.\n");

    // Clear PORT_CS_1 before exit
    IDS_HDT_CONSOLE_USB4_DEBUG ("Clear PORT_CS_1 before exit.\n");
    DataSize      = 1;
    PortCs1.Value = 0;
    AmdUsb4Ring0WriteCS (
      pCM,
      pLaneAdapter->Header.TopologyID,
      CS_TARGET_ADAPTER,
      pLaneAdapter->Header.AdapterNum,
      pLaneAdapter->Usb4Capability + 1,
      &DataSize,
      &(PortCs1.Value)
      );
    return Status;
  }

  // 5. Get result
  DataSize = 0;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                 // CsType
             pLaneAdapter->Header.AdapterNum,   // AdapterNum
             pLaneAdapter->Usb4Capability + 1,  // ReadAddress
             17,                                // ReadSize 17 DWs
             (UINT32**) &Ptr32,                 // DataOut
             &DataSize                          // DataSize
             );
  if ((EFI_ERROR (Status)) || (17 != DataSize) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read Adapter Configuration Space Register.\n");
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "PORT_CS_1 = 0x%x "
    "ResultCode = 0x%x, NoResponse = 0x%x, Length = 0x%x\n",
    ((AMD_USB4_PORT_CS1*)Ptr32)->Value,
    ((AMD_USB4_PORT_CS1*)Ptr32)->Field.ResultCode,
    ((AMD_USB4_PORT_CS1*)Ptr32)->Field.NoResponse,
    ((AMD_USB4_PORT_CS1*)Ptr32)->Field.Length
    );

  if ( ((AMD_USB4_PORT_CS1*)Ptr32)->Field.ResultCode == 0
    && ((AMD_USB4_PORT_CS1*)Ptr32)->Field.NoResponse == 0
    && ((AMD_USB4_PORT_CS1*)Ptr32)->Field.Length == (Length << 2) )
  {
    /*
    if ( Length != 0 ) {  // Length = 0 means write
      IDS_HDT_CONSOLE_USB4_DEBUG ("Ptr32 = 0x%x\n", Ptr32);
      *pDataOut32 = ++Ptr32;
      IDS_HDT_CONSOLE_USB4_DEBUG ("*DataOut = 0x%x\n", *pDataOut32);
      *pLengthOut = (((AMD_USB4_PORT_CS1*)Ptr32)->Field.Length) >> 2;
      IDS_HDT_CONSOLE_USB4_DEBUG ("pLengthOut = 0x%x\n", *pLengthOut);
      IDS_HDT_CONSOLE_USB4_DEBUG ("completes successfully.\n");
    }
    */
    IDS_HDT_CONSOLE_USB4_DEBUG ("completes successfully.\n");
    return EFI_SUCCESS;
  } else {
    IDS_HDT_CONSOLE_USB4_ERROR ("completes unsuccessfully.\n");
    return EFI_PROTOCOL_ERROR;
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * Port Operation
 *
 * @param[in] *pCm            Point to global structure AMD_USB4_CM
 * @param[in] *pLaneAdapter   Point to Lane Adapter
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
 * @retval EFI_NOT_FOUND      no DFP found.
**/
EFI_STATUS
AmdUsb4PortOperation (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pLaneAdapter,
  IN     UINT32                     Target,
  IN     UINT32                     RetimerIndex,
  IN     UINT32                     OpCode,
  IN     UINT32                     OpSize,
  IN     UINT32                     *OpData,
  IN     UINT32                     *pMetadata,
     OUT UINT32                     *CompletionMetadata,
     OUT UINT32                     **Data
  )
{
  EFI_STATUS        Status;
  UINT32            *Ptr32;
  USB4_TIMEOUT      Timeout;
  UINT32            DataSize;

  Status        = EFI_SUCCESS;
  Ptr32         = NULL;
  DataSize      = 0;
  ZeroMem (&Timeout, sizeof(USB4_TIMEOUT));

  if ( NULL == pLaneAdapter || NULL == pCM ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( CELL_TYPE_LANE_ADAPTER != pLaneAdapter->Header.CellType ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("It is not a Lane Adapter.\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( 0 == pLaneAdapter->Usb4Capability ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("It is not a Lane 0 Adapter which do not have USB4 capability.\n");
    return EFI_INVALID_PARAMETER;
  }

  // 1. Optionally write to the Metadata register in the SB Register Space of the target Port.
  if ( NULL != pMetadata ) {
    Status = AmdUsb4SBWrite (
               pCM,
               pLaneAdapter,
               Target,
               RetimerIndex,
               9,             // Metedata
               1,             // Size in DW
               pMetadata
               );
    if ( EFI_ERROR (Status) ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to set metadata. Status %r\n", Status);
      return Status;
    }
  }

  // 2. Optionally write to the Data register in the SB Register Space of the target Port.
  if ( OpSize > 0 && OpSize <= 16 && OpData != NULL ) {
    Status = AmdUsb4SBWrite (
               pCM,
               pLaneAdapter,
               Target,
               RetimerIndex,
               18,            // Data
               OpSize,        // Size in DW
               OpData
               );
    if ( EFI_ERROR (Status) ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to set Data. Status %r\n", Status);
      return Status;
    }
  }

  // 3. Write to the Opcode register in the SB Register Space of the target Port.
  IDS_HDT_CONSOLE_USB4_DEBUG ("Write to the Opcode register in the SB Register Space of the target Port.\n");
  Status = AmdUsb4SBWrite (
             pCM,
             pLaneAdapter,
             Target,
             RetimerIndex,
             8,             // Opcode
             1,             // Size in DW
             &OpCode
             );
  if ( EFI_ERROR (Status) ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to set Opcode. Status %r\n", Status);
    return Status;
  }

  // 4. Keep Polling Opcode register
  //    Until it changes to 0 or "!CMD" or "ERR "
  Status = EFI_SUCCESS;
  StartTimeout (&Timeout, ROUTER_READY_TIMEOUT_US);
  do {
    DataSize = 0;
    Status = AmdUsb4SBRead (
               pCM,
               pLaneAdapter,
               Target,
               RetimerIndex,
               8,
               1,
               &DataSize,
               &Ptr32
               );
    if ( EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read Opcode.\n");
      Status = EFI_DEVICE_ERROR;
      break;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG ("Read Opcode Value=0x%x.\n", *Ptr32);

    if ( *Ptr32 == 0x444D4321     // FourCC value of "!CMD"
      || *Ptr32 == 0x20525245     // FourCC value of "ERR "
      || *Ptr32 == 0x00000000 )   // successfully completed
    {
      Status = EFI_SUCCESS;
      break;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG ("Waiting...\n");

    Status = HasItTimedOut (&Timeout);
  }  while ( Status != EFI_TIMEOUT );

  if ( Status == EFI_SUCCESS ) {
    if ( *Ptr32 == 0 ) {
      // 4.1 Read Completion metadata
      DataSize = 0;
      Status = AmdUsb4SBRead (
                 pCM,
                 pLaneAdapter,
                 Target,
                 RetimerIndex,
                 9,
                 1,
                 &DataSize,
                 &Ptr32
                 );
      if ( EFI_ERROR(Status) ) {
        return Status;
      } else {
        *CompletionMetadata = *Ptr32;
        IDS_HDT_CONSOLE_USB4_DEBUG ("CompletionMetadata = 0x%x\n", *CompletionMetadata);
      }

      // 4.2 Read data
      DataSize = 0;
      Status = AmdUsb4SBRead (
                 pCM,
                 pLaneAdapter,
                 Target,
                 RetimerIndex,
                 18,
                 16,
                 &DataSize,
                 &Ptr32
                 );
      if ( EFI_ERROR(Status) ) {
        return Status;
      } else {
        *Data = Ptr32;
        if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
          IDS_HDT_CONSOLE_USB4_DEBUG ("Dump SB Data\n");
          Usb4DumpData (Ptr32, 64);
        }
      }

      return EFI_SUCCESS;
    } else if ( *Ptr32 == 0x444D4321 ) {  // FourCC value of "!CMD"
      IDS_HDT_CONSOLE_USB4_DEBUG ("The USB4 Port Operation is not supported by the Port.\n");
      return EFI_UNSUPPORTED;
    } else if ( *Ptr32 == 0x20525245 ) {  // FourCC value of "ERR "
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "The USB4 Port fails to execute a Port Operation or executed the Port Operation and failed.\n"
        );
      return EFI_ABORTED;
    } else {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "Should not be here.\n"
        );
      return EFI_LOAD_ERROR;
    }
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Failed to get Port Operation Done.\n");
    return Status;
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * Downstream Port Reset
 *
 * @param[in] *pCm            Point to global structure AMD_USB4_CM
 * @param[in] *pLaneAdapter   Point to Lane Adapter
 * @param[in] PortCs19        Preset Value of PORT_CS_19
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
 * @retval EFI_NOT_FOUND      no DFP found.
**/
EFI_STATUS
AmdUsb4DownstreamPortReset (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pLaneAdapter,
  IN     AMD_USB4_PORT_CS19         *pPortCs19
  )
{
  EFI_STATUS           Status;
  UINT32               DataSize;
  AMD_USB4_PORT_CS19   PortCs19;
  AMD_USB4_CM_ROUTER   *pRouter;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ( NULL == pLaneAdapter || NULL == pCM || NULL == pPortCs19) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_SUCCESS;
  PortCs19.Value = pPortCs19->Value;
  pRouter = (AMD_USB4_CM_ROUTER*)pLaneAdapter->Header.Parent;

  if ( !IsUSB4Router (pRouter) ) {
    return Tbt3DFPReset (pCM, pLaneAdapter);
  }

  //
  // Set the PORT_CS_19.Downstream Port Reset bit of the DFP to 1b.
  //
  PortCs19.Field.DownStreamPortReset = 1;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pLaneAdapter->Header.AdapterNum),       // AdapterNum
             (UINT32) (pLaneAdapter->Usb4Capability + 19),     // ReadAddress: PORT_CS_19
             &DataSize,
             &(PortCs19.Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write PORT_CS_19 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Wait for 10ms.
  // todo: Update delay to 10ms
  //
  MicroSecondDelay (DOWNSTREAM_PORT_RESET_WAIT); //10ms

  //
  // Set the PORT_CS_19.Downstream Port Reset bit of the DFP to 0b to allow Lane Initialization to take place.
  //
  PortCs19.Field.DownStreamPortReset = 0;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pLaneAdapter->Header.AdapterNum),       // AdapterNum
             (UINT32) (pLaneAdapter->Usb4Capability + 19),     // ReadAddress: PORT_CS_19
             &DataSize,
             &(PortCs19.Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write PORT_CS_19 Register to clear Downstream Port Reset.\n");
    return EFI_DEVICE_ERROR;
  }

  StartTimeout (&(pLaneAdapter->PMTimer), LANE_ADP_DFP_RESET_WAIT);

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Disable Lane
 *
 * @param[in] *pCm            Point to global structure AMD_USB4_CM
 * @param[in] *pLaneAdapter   Point to Lane Adapter
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
 *
**/
EFI_STATUS
AmdUsb4PortDisableLane (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pLaneAdapter
  )
{
  EFI_STATUS            Status;
  UINT32                DataSize;
  AMD_USB4_LANE_ADP_CS1 *pLaneAdpCs1;

  Status           = EFI_SUCCESS;
  DataSize         = 0;
  pLaneAdpCs1      = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pLaneAdapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pLaneAdapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             1,                                             // ReadSize 1 DWs
             (UINT32**)&pLaneAdpCs1,                        // DataOut
             &DataSize                                      // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  pLaneAdpCs1->Field.LinkDisable = 1;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pLaneAdapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pLaneAdapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pLaneAdapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             &DataSize,
             &(pLaneAdpCs1->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }
  pLaneAdapter->LaneAdp_LinkDisable   = 1;

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Re-enumerate Router
 *
 * @param[in]  *pCM               Point to global structure AMD_USB4_CM
 * @param[in]  *pRouter           Point to Router Cm Data structure
 *
 * @retval EFI_SUCCESS            Success.
 * @retval EFI_INVALID_PARAMETER  Invalid parameters.
 * @retval EFI_DEVICE_ERROR -     HW error.
**/

EFI_STATUS
Usb4ReEnumerateRouter (
  IN      AMD_USB4_CM                           *pCM,
  IN  OUT AMD_USB4_CM_ROUTER                    *pRouter
  )
{
  EFI_STATUS                            Status;
  UINT32                                DataSize;
  AMD_USB4_ROUTER_CONFIGURATION_SPACE   *pRouterCs;

  UINT32                                TimeOut;
  AMD_USB4_ROUTER_CS_6                  *pRouterCs6;

  Status        = EFI_SUCCESS;
  DataSize      = 0;
  pRouterCs     = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if ( NULL == pCM || NULL == pRouter ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("pCM or pRouter is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  pRouter->Enumerated = 0;

  //
  // Read Host Router Configuration Space in order to read ROUTER_CS_0 through ROUTER_CS_8
  //
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,      // CsType
             0,                     // AdapterNum
             0,                     // ReadAddress
             ROUTER_CS_BASE_SIZE_DW,// ReadSize 9 DWs
             (UINT32**)&pRouterCs,  // DataOut
             &DataSize              // DataSize
             );

  if (EFI_ERROR (Status) || ROUTER_CS_BASE_SIZE_DW != DataSize || NULL == pRouterCs ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read Host Router Configuration Space Register.\n");
    return EFI_DEVICE_ERROR;
  }

  // Call AmdUsb4WriteCS to write below fields.
  //   1) ROUTER_CS_1.Depth = Parameter.Depth
  //   2) ROUTER_CS_[3:2].TopologyID = Parameter.Topology
  //   3) ROUTER_CS_1.UpstreamAdapter = ROUTER_CS_1.UpstreamAdapter
  //      Note: If Depth == 0, UpstreamAdapter should be Host Interface Adapter Number.
  //   4) ROUTER_CS_4.CMUV = ( ROUTER_CS_4.USB4_Version == 0x20 ) ? 0x10 : 0;
  //   5) ROUTER_CS_3.TopologhID(Bit[31]) = 1;
  pRouterCs->RouterCs1.Field.Depth = pRouter->Depth;
  pRouterCs->RouterCs3.Field.TopologyIDHigh = (UINT32)((pRouter->Header.TopologyID)>>32);
  pRouterCs->RouterCs3.Field.TopologyIDValid = 1;
  pRouterCs->TopologyIDLow = (UINT32)(pRouter->Header.TopologyID);
  pRouterCs->RouterCs4.Field.CMUsb4Version = 0x10;

  DataSize = 4;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pRouter->Header.TopologyID,
             CS_TARGET_ROUTER,
             0,
             1,
             &DataSize,
             &(pRouterCs->RouterCs1.Value)
             );

  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write Host Router Configuration Space Registers.\n");
    return EFI_DEVICE_ERROR;
  }

  // Keep calling AmdUsb4ReadCS to get ROUTER_CS_6.
  Status = EFI_TIMEOUT;
  TimeOut = ROUTER_READY_TIMEOUT_US/10;
  do {
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pRouter->Header.TopologyID,
               CS_TARGET_ROUTER,      // CsType
               0,                     // AdapterNum
               6,                     // ReadAddress
               1,                     // ReadSize 1 DWs
               (UINT32**)&pRouterCs6, // DataOut
               &DataSize              // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read Host Router Configuration 6 Space Register.\n");
      Status = EFI_DEVICE_ERROR;
      break;
    }

    if ( pRouterCs6->Field.RouterReady ) {
      pRouter->Enumerated = 1;
      Status = EFI_SUCCESS;
      break;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG ("Wait Router Ready. TimeOut %d\n", TimeOut);
    MicroSecondDelay (10);  // TODO - Decide the minimum delay time
  } while (--TimeOut);

  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Router is not ready.\n");
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Link Bonding
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pDownstreamRouter   Point to Downstream Router
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
 * @retval EFI_NOT_FOUND      no DFP found.
**/
EFI_STATUS
AmdUsb4PortLinkBonding (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pDownstreamRouter
  )
{
  EFI_STATUS        Status;
  UINT32            DataSize;
  USB4_TIMEOUT      Timeout;
  AMD_USB4_CM_CELL           *pUsb4Cell;
  AMD_USB4_CM_LANE_ADAPTER   *pParentDfpLane0Adapter;
  AMD_USB4_CM_LANE_ADAPTER   *pParentDfpLane1Adapter;
  AMD_USB4_CM_LANE_ADAPTER   *pChildUfpLane0Adapter;
  AMD_USB4_CM_LANE_ADAPTER   *pChildUfpLane1Adapter;
  AMD_USB4_LANE_ADP_CS1      *pLaneAdpCs1;
  AMD_USB4_LANE_ADP_CS1      LaneAdpCs1;
  AMD_USB4_ADP_CS4           *pAdpCs4;
  UINT32                     AndMask;
  UINT32                     OrValue;

  Status           = EFI_SUCCESS;
  DataSize         = 0;
  LaneAdpCs1.Value = 0;
  pLaneAdpCs1      = NULL;
  pAdpCs4          = NULL;
  AndMask          = 0;
  OrValue          = 0;
  ZeroMem (&Timeout, sizeof(USB4_TIMEOUT));

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if (NULL == pDownstreamRouter || NULL == pCM) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  pUsb4Cell              = (AMD_USB4_CM_CELL *) (pDownstreamRouter->Header.Parent);
  pParentDfpLane0Adapter = (AMD_USB4_CM_LANE_ADAPTER *)pUsb4Cell;
  pParentDfpLane1Adapter = (AMD_USB4_CM_LANE_ADAPTER *) (pUsb4Cell + 1);
  pUsb4Cell              = &(pDownstreamRouter->pAdapter[pDownstreamRouter->UpstreamAdapter]);
  pChildUfpLane0Adapter  = (AMD_USB4_CM_LANE_ADAPTER *)pUsb4Cell;
  pChildUfpLane1Adapter  = (AMD_USB4_CM_LANE_ADAPTER *) (pUsb4Cell + 1);

  /*
  //
  // Set TBT3.VESC6.LC_SX_CTRL
  //
  if ( !IsUSB4Router (pDownstreamRouter) ) {
    Tbt3PortRegionSetting (pCM, pChildUfpLane0Adapter);
  }

  if ( !IsUSB4Router ((AMD_USB4_CM_ROUTER*)pParentDfpLane0Adapter->Header.Parent) ) {
    Tbt3PortRegionSetting (pCM, pParentDfpLane0Adapter);
  }
  */

  //
  // Check if Link Bonding is possible.
  //
  if ( !pChildUfpLane0Adapter->Usb4Port_BE ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "The conditions of Router (0x%lx) for Lane (%d) bonding are not met.\n",
      pChildUfpLane0Adapter->Header.TopologyID,
      pChildUfpLane0Adapter->Header.AdapterNum
      );
    return EFI_UNSUPPORTED;
  }

  //
  // Verify that both sides of the Link support Lane bonding.
  //
  if (!(pParentDfpLane0Adapter->LaneAdp_SupportWidth & SUPPORT_LINK_WIDTH_X2)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Upstream DFP does not support Dual Lane!\n");
    return EFI_UNSUPPORTED;
  }

  if (!(pChildUfpLane0Adapter->LaneAdp_SupportWidth & SUPPORT_LINK_WIDTH_X2)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Downstream UFP does not support Dual Lane!\n");
    return EFI_UNSUPPORTED;
  }

  //
  // Check Current Link Width
  //
  if (pParentDfpLane0Adapter->LaneAdp_CurrentWidth != pChildUfpLane0Adapter->LaneAdp_CurrentWidth) {
    IDS_HDT_CONSOLE_USB4_ERROR ("unmatch link width on Upstream DFP and Downstream UFP!\n");
    return EFI_DEVICE_ERROR;
  }

  if (pParentDfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Link is already running on x2!\n");
    return EFI_SUCCESS;
  }

  //
  // Verify that all Lane Adapters are in CL0 state.
  //
  if ((pParentDfpLane0Adapter->LaneAdp_AdapterState != ADAPTER_STATE_CL0)
    || (pParentDfpLane1Adapter->LaneAdp_AdapterState != ADAPTER_STATE_CL0)
    || (pChildUfpLane0Adapter->LaneAdp_AdapterState != ADAPTER_STATE_CL0)
    || (pChildUfpLane1Adapter->LaneAdp_AdapterState != ADAPTER_STATE_CL0))
  {
    IDS_HDT_CONSOLE_USB4_ERROR ("Lane Adapter of the port is not in CL0 state!\n");

    //
    // Disable Lane1 Adapters
    //
    AmdUsb4PortDisableLane (pCM, pParentDfpLane1Adapter);
    AmdUsb4PortDisableLane (pCM, pChildUfpLane1Adapter);
    return EFI_UNSUPPORTED;
  }

  //
  // On both sides of the Link, set the LANE_ADP_CS_1.Target Link Width field of all Lane Adapters to x2.
  //
  // Upstream DFP Lane1
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pParentDfpLane1Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane1Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane1Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             1,                                                       // ReadSize 1 DWs
             (UINT32**)&pLaneAdpCs1,                                  // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  pLaneAdpCs1->Field.TargetLinkWidth = TARGET_LINK_WIDTH_X2;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pParentDfpLane1Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane1Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane1Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             &DataSize,
             &(pLaneAdpCs1->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  // Downstream UFP Lane0
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pChildUfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pChildUfpLane0Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             1,                                                      // ReadSize 1 DWs
             (UINT32**)&pLaneAdpCs1,                                 // DataOut
             &DataSize                                               // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  pLaneAdpCs1->Field.TargetLinkWidth = TARGET_LINK_WIDTH_X2;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pChildUfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pChildUfpLane0Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             &DataSize,
             &(pLaneAdpCs1->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  // Downstream UFP Lane1
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pChildUfpLane1Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pChildUfpLane1Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pChildUfpLane1Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             1,                                                      // ReadSize 1 DWs
             (UINT32**)&pLaneAdpCs1,                                 // DataOut
             &DataSize                                               // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  pLaneAdpCs1->Field.TargetLinkWidth = TARGET_LINK_WIDTH_X2;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pChildUfpLane1Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pChildUfpLane1Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pChildUfpLane1Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             &DataSize,
             &(pLaneAdpCs1->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  // Upstream DFP Lane0
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane0Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             1,                                                       // ReadSize 1 DWs
             (UINT32**)&pLaneAdpCs1,                                  // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  LaneAdpCs1.Value = pLaneAdpCs1->Value;
  LaneAdpCs1.Field.TargetLinkWidth = TARGET_LINK_WIDTH_X2;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane0Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             &DataSize,
             &(LaneAdpCs1.Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // On one side of the Link, set the LANE_ADP_CS_1.Lane Bonding bit to 1b in one of the Lane 0 Adapters.
  //
  // Can we do a single LANE_ADP_CS_1 for both TargetLinkWidth and LaneBonding?
  LaneAdpCs1.Field.LaneBonding = 1;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane0Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             &DataSize,
             &(LaneAdpCs1.Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Verify that Lane Bonding was successful
  //
  StartTimeout (&Timeout, LINK_BONDING_TIMEOUT);
  do {
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pParentDfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
               (UINT32) (pParentDfpLane0Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
               1,                                                       // ReadSize 1 DWs
               (UINT32**)&pLaneAdpCs1,                                  // DataOut
               &DataSize                                                // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read LANE_ADP_CS1 Register.\n");
      return EFI_DEVICE_ERROR;
    }

    if (pLaneAdpCs1->Field.NegotiatedLinkWidth == CURRENT_LINK_WIDTH_X2) {
      break;
    }

  }  while ( HasItTimedOut (&Timeout) != EFI_TIMEOUT );

  //
  // Update Upstream Lane0 Adapter data
  //
  pParentDfpLane0Adapter->LaneAdp_TargetSpeed   = pLaneAdpCs1->Field.TargetLinkSpeed;
  pParentDfpLane0Adapter->LaneAdp_TargetWidth   = pLaneAdpCs1->Field.TargetLinkWidth;
  pParentDfpLane0Adapter->LaneAdp_CurrentSpeed  = pLaneAdpCs1->Field.CurrentLinkSpeed;
  pParentDfpLane0Adapter->LaneAdp_CurrentWidth  = pLaneAdpCs1->Field.NegotiatedLinkWidth;
  pParentDfpLane0Adapter->LaneAdp_LinkDisable   = pLaneAdpCs1->Field.LinkDisable;
  pParentDfpLane0Adapter->LaneAdp_LinkBonding   = pLaneAdpCs1->Field.LaneBonding;
  pParentDfpLane0Adapter->LaneAdp_AdapterState  = pLaneAdpCs1->Field.AdapterState;

  if (pParentDfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2) {
    //
    // Success! Update Downstream Lane0 Adapter data
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pChildUfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),    // AdapterNum
               (UINT32) (pChildUfpLane0Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
               1,                                                      // ReadSize 1 DWs
               (UINT32**)&pLaneAdpCs1,                                 // DataOut
               &DataSize                                               // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read LANE_ADP_CS1 Register.\n");
      return EFI_DEVICE_ERROR;
    }
    pChildUfpLane0Adapter->LaneAdp_TargetSpeed   = pLaneAdpCs1->Field.TargetLinkSpeed;
    pChildUfpLane0Adapter->LaneAdp_TargetWidth   = pLaneAdpCs1->Field.TargetLinkWidth;
    pChildUfpLane0Adapter->LaneAdp_CurrentSpeed  = pLaneAdpCs1->Field.CurrentLinkSpeed;
    pChildUfpLane0Adapter->LaneAdp_CurrentWidth  = pLaneAdpCs1->Field.NegotiatedLinkWidth;
    pChildUfpLane0Adapter->LaneAdp_LinkDisable   = pLaneAdpCs1->Field.LinkDisable;
    pChildUfpLane0Adapter->LaneAdp_LinkBonding   = pLaneAdpCs1->Field.LaneBonding;
    pChildUfpLane0Adapter->LaneAdp_AdapterState  = pLaneAdpCs1->Field.AdapterState;

    //
    // Updater Total Buffer info
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pChildUfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),    // AdapterNum
               4,                                                      // ReadAddress: ADP_CS_4
               1,                                                      // ReadSize 1 DWs
               (UINT32**)&pAdpCs4,                                     // DataOut
               &DataSize                                               // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read ADP_CS_4 Register.\n");
      return EFI_DEVICE_ERROR;
    }
    pChildUfpLane0Adapter->TotalBuffer  = pAdpCs4->Field.TotalBuffers;
    IDS_HDT_CONSOLE_USB4_DEBUG (
        "After bonding Router (0x%lx) for Lane0 adapter (%d) Total Buffer = 0x%x.\n",
        pChildUfpLane0Adapter->Header.TopologyID,
        pChildUfpLane0Adapter->Header.AdapterNum,
        pChildUfpLane0Adapter->TotalBuffer
      );
    //
    // Calculate the number of remaining buffers
    //
    if (pChildUfpLane0Adapter->TotalBuffer >= (UINT32)(pChildUfpLane0Adapter->ControlPathCredits)) {
      pChildUfpLane0Adapter->RemainingBuffers = (UINT32)(pChildUfpLane0Adapter->TotalBuffer -
                                                 pChildUfpLane0Adapter->ControlPathCredits);
    } else {
      IDS_HDT_CONSOLE_USB4_ERROR (" Can not allocate minimum buffers.\n");
      return RETURN_BUFFER_TOO_SMALL;
    }
    IDS_HDT_CONSOLE_USB4_DEBUG ("  RemainingBuffers   = 0x%lx\n", pChildUfpLane0Adapter->RemainingBuffers);

    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pParentDfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),   // AdapterNum
               4,                                                      // ReadAddress: ADP_CS_4
               1,                                                      // ReadSize 1 DWs
               (UINT32**)&pAdpCs4,                                     // DataOut
               &DataSize                                               // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read ADP_CS_4 Register.\n");
      return EFI_DEVICE_ERROR;
    }
    pParentDfpLane0Adapter->TotalBuffer  = pAdpCs4->Field.TotalBuffers;
    IDS_HDT_CONSOLE_USB4_DEBUG (
        "After bonding Router (0x%lx) for Lane0 adapter (%d) Total Buffer = 0x%x.\n",
        pParentDfpLane0Adapter->Header.TopologyID,
        pParentDfpLane0Adapter->Header.AdapterNum,
        pParentDfpLane0Adapter->TotalBuffer
      );
    //
    // Calculate the number of remaining buffers
    //
    if (pParentDfpLane0Adapter->TotalBuffer >= (UINT32)(pParentDfpLane0Adapter->ControlPathCredits)) {
      pParentDfpLane0Adapter->RemainingBuffers = (UINT32)(pParentDfpLane0Adapter->TotalBuffer -
                                                 pParentDfpLane0Adapter->ControlPathCredits);
    } else {
      IDS_HDT_CONSOLE_USB4_ERROR (" Can not allocate minimum buffers.\n");
      return RETURN_BUFFER_TOO_SMALL;
    }
    IDS_HDT_CONSOLE_USB4_DEBUG ("  RemainingBuffers   = 0x%lx\n", pParentDfpLane0Adapter->RemainingBuffers);

    //
    // Update Bandwidth Data
    //
    if (pParentDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3) {
      pParentDfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_40GBPS;
    } else if (pParentDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2) {
      pParentDfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
    }
    pParentDfpLane0Adapter->AvailableBandwidth = (pParentDfpLane0Adapter->RawBandwidth * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                                                  GUARD_BAND_BANDWIDTH_DENOMINATOR;

    if (pChildUfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3) {
      pChildUfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_40GBPS;
    } else if (pChildUfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2) {
      pChildUfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
    }
    pChildUfpLane0Adapter->AvailableBandwidth = (pChildUfpLane0Adapter->RawBandwidth * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                                                 GUARD_BAND_BANDWIDTH_DENOMINATOR;

    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Update Parent Router DFP Bandwidth Data after Link Bonding\n"
      );
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "  Raw Bandwidth                    = %d\n",
      pParentDfpLane0Adapter->RawBandwidth
      );
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "  Available Bandwidth              = %d\n",
      pParentDfpLane0Adapter->AvailableBandwidth
      );
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Update Child Router UFP Bandwidth Data after Link Bonding\n"
      );
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "  Raw Bandwidth                    = %d\n",
      pChildUfpLane0Adapter->RawBandwidth
      );
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "  Available Bandwidth              = %d\n",
      pChildUfpLane0Adapter->AvailableBandwidth
      );

    return EFI_SUCCESS;
  } else {
    //
    // return error if failed
    //
    IDS_HDT_CONSOLE_USB4_ERROR ("Link Bonding Failed! Disable Lane1\n");
    //
    // Disable Upstream DFP Lane1
    //
    AmdUsb4PortDisableLane (pCM, pParentDfpLane1Adapter);

    //
    // Clear Upstream DFP Lane0 ADP_CS_4.lock
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pParentDfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
               4,                                                       // ReadAddress
               1,                                                       // ReadSize 1 DWs
               (UINT32**)&pAdpCs4,                                      // DataOut
               &DataSize                                                // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read ADP_CS_4 Register.\n");
      return EFI_DEVICE_ERROR;
    }
    pAdpCs4->Field.Lock = 0;
    DataSize = 1;
    Status = AmdUsb4Ring0WriteCS (
               pCM,
               pParentDfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
               4,                                                       // ReadAddress
               &DataSize,
               &(pAdpCs4->Value)
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write ADP_CS_4 Register.\n");
      return EFI_DEVICE_ERROR;
    }

    //
    // Re-enumerate Downstream Router
    //
    Status = Usb4ReEnumerateRouter (pCM, pDownstreamRouter);
    if ( Status != EFI_SUCCESS ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Router Re-Eumeration fail!\n");
      return EFI_DEVICE_ERROR;
    }

    //
    // Update Downstream Lane0 Adapter data
    //
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pChildUfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),    // AdapterNum
               (UINT32) (pChildUfpLane0Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
               1,                                                // ReadSize 1 DWs
               (UINT32**)&pLaneAdpCs1,                           // DataOut
               &DataSize                                         // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read LANE_ADP_CS1 Register.\n");
      return EFI_DEVICE_ERROR;
    }
    pChildUfpLane0Adapter->LaneAdp_TargetSpeed   = pLaneAdpCs1->Field.TargetLinkSpeed;
    pChildUfpLane0Adapter->LaneAdp_TargetWidth   = pLaneAdpCs1->Field.TargetLinkWidth;
    pChildUfpLane0Adapter->LaneAdp_CurrentSpeed  = pLaneAdpCs1->Field.CurrentLinkSpeed;
    pChildUfpLane0Adapter->LaneAdp_CurrentWidth  = pLaneAdpCs1->Field.NegotiatedLinkWidth;
    pChildUfpLane0Adapter->LaneAdp_LinkDisable   = pLaneAdpCs1->Field.LinkDisable;
    pChildUfpLane0Adapter->LaneAdp_LinkBonding   = pLaneAdpCs1->Field.LaneBonding;
    pChildUfpLane0Adapter->LaneAdp_AdapterState  = pLaneAdpCs1->Field.AdapterState;

    // Disable Downstream UFP Lane1
    AmdUsb4PortDisableLane (pCM, pChildUfpLane1Adapter);

    return EFI_DEVICE_ERROR;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Link Management including Speed change
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pDownstreamRouter   Point to Downstream Router
 * @param[in] *LinkSpeedGen3       Requirement of Gen3 speed
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
 * @retval EFI_NOT_FOUND      no DFP found.
**/
EFI_STATUS
AmdUsb4PortLinkManage (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_ROUTER         *pDownstreamRouter,
  IN     BOOLEAN                    LinkSpeedGen3
  )
{
  EFI_STATUS        Status;
  UINT32            DataSize;
  AMD_USB4_CM_CELL           *pUsb4Cell;
  AMD_USB4_CM_LANE_ADAPTER   *pParentDfpLane0Adapter;
  AMD_USB4_CM_LANE_ADAPTER   *pParentDfpLane1Adapter;
  AMD_USB4_CM_LANE_ADAPTER   *pChildUfpLane0Adapter;
  AMD_USB4_CM_LANE_ADAPTER   *pChildUfpLane1Adapter;
  AMD_USB4_LANE_ADP_CS1      *pLaneAdpCs1;
  AMD_USB4_PORT_CS18         *pPortCs18;
  AMD_USB4_PORT_CS19         *pPortCs19;
  AMD_USB4_ADP_CS4           *pAdpCs4;

  Status        = EFI_SUCCESS;
  DataSize      = 0;
  pLaneAdpCs1   = NULL;
  pPortCs18     = NULL;
  pPortCs19     = NULL;
  pAdpCs4       = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if (NULL == pDownstreamRouter || NULL == pCM) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  pUsb4Cell         = (AMD_USB4_CM_CELL *) (pDownstreamRouter->Header.Parent);
  pParentDfpLane0Adapter   = (AMD_USB4_CM_LANE_ADAPTER *)pUsb4Cell;
  pParentDfpLane1Adapter   = (AMD_USB4_CM_LANE_ADAPTER *) (pUsb4Cell+1);
  pUsb4Cell         = &(pDownstreamRouter->pAdapter[pDownstreamRouter->UpstreamAdapter]);
  pChildUfpLane0Adapter = (AMD_USB4_CM_LANE_ADAPTER *)pUsb4Cell;
  pChildUfpLane1Adapter = (AMD_USB4_CM_LANE_ADAPTER *) (pUsb4Cell+1);

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane0Adapter->LaneAdp_CurrentSpeed = 0x%x\n",
    pParentDfpLane0Adapter->LaneAdp_CurrentSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane0Adapter->LaneAdp_SupportSpeed = 0x%x\n",
    pParentDfpLane0Adapter->LaneAdp_SupportSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "=======================================================\n"
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane1Adapter->LaneAdp_CurrentSpeed = 0x%x\n",
    pParentDfpLane1Adapter->LaneAdp_CurrentSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane1Adapter->LaneAdp_SupportSpeed = 0x%x\n",
    pParentDfpLane1Adapter->LaneAdp_SupportSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "=======================================================\n"
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane0Adapter->LaneAdp_CurrentSpeed  = 0x%x\n",
    pChildUfpLane0Adapter->LaneAdp_CurrentSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane0Adapter->LaneAdp_SupportSpeed  = 0x%x\n",
    pChildUfpLane0Adapter->LaneAdp_SupportSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "=======================================================\n"
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane1Adapter->LaneAdp_CurrentSpeed  = 0x%x\n",
    pChildUfpLane1Adapter->LaneAdp_CurrentSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane1Adapter->LaneAdp_SupportSpeed  = 0x%x\n",
    pChildUfpLane1Adapter->LaneAdp_SupportSpeed
    );

  //
  // Check Link Speed
  //
  if (pParentDfpLane0Adapter->LaneAdp_CurrentSpeed != pChildUfpLane0Adapter->LaneAdp_CurrentSpeed) {
    IDS_HDT_CONSOLE_USB4_ERROR ("unmatch link speed on DFP and UFP Lane Adapters!\n");
    return EFI_DEVICE_ERROR;
  }

  if ((pParentDfpLane0Adapter->LaneAdp_CurrentSpeed != pParentDfpLane1Adapter->LaneAdp_CurrentSpeed)
    || (pParentDfpLane0Adapter->LaneAdp_SupportSpeed != pParentDfpLane1Adapter->LaneAdp_SupportSpeed)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("unmatch link speed on Upstream DFP Lane0 and Lane1 Adapters!\n");
    return EFI_DEVICE_ERROR;
  }

  if ((pChildUfpLane0Adapter->LaneAdp_CurrentSpeed != pChildUfpLane1Adapter->LaneAdp_CurrentSpeed)
    || (pChildUfpLane0Adapter->LaneAdp_SupportSpeed != pChildUfpLane1Adapter->LaneAdp_SupportSpeed)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("unmatch link speed on Downstream UFP Lane0 and Lane1 Adapters!\n");
    return EFI_DEVICE_ERROR;
  }

  if (LinkSpeedGen3) {
    if (pParentDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("Link is already running on Gen3!\n");
      return EFI_SUCCESS;
    }
    if ((pParentDfpLane0Adapter->LaneAdp_SupportSpeed == SUPPORTED_LINK_SPEED_GEN2)
      || (pChildUfpLane0Adapter->LaneAdp_SupportSpeed == SUPPORTED_LINK_SPEED_GEN2)) {
      IDS_HDT_CONSOLE_USB4_ERROR ("USB4 Port does not support Gen3!\n");
      return EFI_DEVICE_ERROR;
    }
  }

  if ((!LinkSpeedGen3) && (pParentDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Link is already running on Gen2!\n");
    return EFI_SUCCESS;
  }

  //
  // Target Link Speed field for both Lane Adapter in DFP
  //
  // Lane0
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane0Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             1,                                                       // ReadSize 1 DWs
             (UINT32**)&pLaneAdpCs1,                                  // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  pLaneAdpCs1->Field.TargetLinkSpeed = ( LinkSpeedGen3 ) ? TARGET_LINK_SPEED_GEN3: TARGET_LINK_SPEED_GEN2;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane0Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             &DataSize,
             &(pLaneAdpCs1->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  // Lane1
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pParentDfpLane1Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane1Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane1Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             1,                                                       // ReadSize 1 DWs
             (UINT32**)&pLaneAdpCs1,                                  // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  pLaneAdpCs1->Field.TargetLinkSpeed = ( LinkSpeedGen3 ) ? TARGET_LINK_SPEED_GEN3: TARGET_LINK_SPEED_GEN2;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pParentDfpLane1Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane1Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane1Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             &DataSize,
             &(pLaneAdpCs1->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Get PORT_CS_19
  //
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane0Adapter->Usb4Capability + 19),  // ReadAddress: PORT_CS_19
             1,                                                       // ReadSize 1 DWs
             (UINT32**)&pPortCs19,                                    // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read PORT_CS_19 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  IDS_HDT_CONSOLE_USB4_DEBUG ("pParentDfpLane0Adapter PORT_CS_19 = 0x%x\n", pPortCs19->Value);

  //
  // Update PORT_CS_19 accordingly with RS2/RS3 and other bits
  //

  //
  // Downstream Port Reset
  //
  Status = AmdUsb4DownstreamPortReset (
             pCM,
             pParentDfpLane0Adapter,
             pPortCs19
             );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Cannot do Downstream Port Reset.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Wait until Lane 0 adapter reach CL0
  //
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "send out Router 0x%lx Adapter %d before wait CL0 PMTimer=0x%x\n",
    pParentDfpLane0Adapter->Header.TopologyID,
    pParentDfpLane0Adapter->Header.AdapterNum,
    GetACPITimer ()
    );
  do {
    DataSize = sizeof (AMD_USB4_LANE_ADP_CS1) / sizeof (UINT32);
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pParentDfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,                                   // CsType
               pParentDfpLane0Adapter->Header.AdapterNum,           // AdapterNum
               pParentDfpLane0Adapter->LaneCapability + 1,          // ReadAddress
               DataSize,                                            // ReadSize
               (UINT32**)&pLaneAdpCs1,                              // DataOut
               &DataSize                                            // DataSize
               );
    if (EFI_ERROR (Status)
      || ((sizeof (AMD_USB4_LANE_ADP_CS1) / sizeof (UINT32) ) != DataSize))
    {
      IDS_HDT_CONSOLE_USB4_ERROR ("%a Error reading Lane capabilities\n", __FUNCTION__);
      return EFI_DEVICE_ERROR;
    }

    if ( pLaneAdpCs1->Field.AdapterState == ADAPTER_STATE_CL0 ) {
      break;
    }

    MicroSecondDelay (10000);
  } while (HasItTimedOut (&(pParentDfpLane0Adapter->PMTimer)) == EFI_SUCCESS);

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "send out Router 0x%lx Adapter %d after wait CL0 PMTimer=0x%x\n",
    pParentDfpLane0Adapter->Header.TopologyID,
    pParentDfpLane0Adapter->Header.AdapterNum,
    GetACPITimer ()
    );

  if (pParentDfpLane0Adapter->LaneAdp_AdapterState != ADAPTER_STATE_CL0) {
    //
    // No Router detected
    //
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "We lose Router which is connected to Router 0x%lx Adapter %d.\n",
      pParentDfpLane0Adapter->Header.TopologyID,
      pParentDfpLane0Adapter->Header.AdapterNum
      );
    return EFI_DEVICE_ERROR;
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "send out Router 0x%lx Adapter %d state change to CL0.\n",
      pParentDfpLane0Adapter->Header.TopologyID,
      pParentDfpLane0Adapter->Header.AdapterNum
      );
  }

  //
  // Clear ADP_CS_4.lock
  //
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             4,                                                       // ReadAddress
             1,                                                       // ReadSize 1 DWs
             (UINT32**)&pAdpCs4,                                      // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read ADP_CS_4 Register.\n");
    return EFI_DEVICE_ERROR;
  }
  pAdpCs4->Field.Lock = 0;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             4,                                                       // ReadAddress
             &DataSize,
             &(pAdpCs4->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write ADP_CS_4 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Re-enumerate Downstream Router
  //
  Status = Usb4ReEnumerateRouter (pCM, pDownstreamRouter);
  if ( Status != EFI_SUCCESS ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Router Re-Enumeration fail!\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // Update DFP Lane Adapter Info
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("Update DFP Adapter fields after Downstream Port Reset.\n");
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                                       // CsType
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane0Adapter->LaneCapability + 1),   // ReadAddress
             DataSize,                                                // ReadSize
             (UINT32**)&pLaneAdpCs1,                                  // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || (1 != DataSize)) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("%a Error reading Lane capabilities\n", __FUNCTION__);
    return EFI_DEVICE_ERROR;
  }
  pParentDfpLane0Adapter->LaneAdp_TargetSpeed   = pLaneAdpCs1->Field.TargetLinkSpeed;
  pParentDfpLane0Adapter->LaneAdp_TargetWidth   = pLaneAdpCs1->Field.TargetLinkWidth;
  pParentDfpLane0Adapter->LaneAdp_CurrentSpeed  = pLaneAdpCs1->Field.CurrentLinkSpeed;
  pParentDfpLane0Adapter->LaneAdp_CurrentWidth  = pLaneAdpCs1->Field.NegotiatedLinkWidth;
  pParentDfpLane0Adapter->LaneAdp_LinkDisable   = pLaneAdpCs1->Field.LinkDisable;
  pParentDfpLane0Adapter->LaneAdp_LinkBonding   = pLaneAdpCs1->Field.LaneBonding;

  IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane0Adapter LaneAdpCs1 = 0x%x\n",
    pLaneAdpCs1->Value
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane0Adapter LaneAdp_TargetSpeed  = 0x%x\n",
    pParentDfpLane0Adapter->LaneAdp_TargetSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane0Adapter LaneAdp_TargetWidth  = 0x%x\n",
    pParentDfpLane0Adapter->LaneAdp_TargetWidth
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane0Adapter LaneAdp_CurrentSpeed = 0x%x\n",
    pParentDfpLane0Adapter->LaneAdp_CurrentSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane0Adapter LaneAdp_CurrentWidth = 0x%x\n",
    pParentDfpLane0Adapter->LaneAdp_CurrentWidth
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane0Adapter LaneAdp_LinkDisable  = 0x%x\n",
    pParentDfpLane0Adapter->LaneAdp_LinkDisable
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane0Adapter LaneAdp_LinkBonding  = 0x%x\n",
    pParentDfpLane0Adapter->LaneAdp_LinkBonding
    );
  IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");

  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pParentDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                                       // CsType
             (UINT32) (pParentDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane0Adapter->Usb4Capability + 18),  // ReadAddress: PORT_CS_18
             1,                                                       // ReadSize
             (UINT32**)&pPortCs18,                                    // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || (1 != DataSize)) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("%a Error reading USB4 capabilities\n", __FUNCTION__);
    return EFI_DEVICE_ERROR;
  }
  pParentDfpLane0Adapter->Usb4Port_CableVersion  = pPortCs18->Field.CableUSB4Version;
  pParentDfpLane0Adapter->Usb4Port_BE            = pPortCs18->Field.BondingEnabled;
  pParentDfpLane0Adapter->Usb4Port_TCM           = pPortCs18->Field.TBT3CompatibleMode;
  pParentDfpLane0Adapter->Usb4Port_LCL           = pPortCs18->Field.LinkCLxSupport;
  pParentDfpLane0Adapter->Usb4Port_RE2           = pPortCs18->Field.RsFecEnabledGen2;
  pParentDfpLane0Adapter->Usb4Port_RE3           = pPortCs18->Field.RsFecEnabledGen3;
  pParentDfpLane0Adapter->Usb4Port_RD            = pPortCs18->Field.RouterDetected;

  //
  // Update Bandwidth Data
  //
  if ((pParentDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
    && (pParentDfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2)) {
    pParentDfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_40GBPS;
  } else if ((pParentDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
    && (pParentDfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1)) {
    pParentDfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
  } else if ((pParentDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
    && (pParentDfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2)) {
    pParentDfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
  } else if ((pParentDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
    && (pParentDfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1)) {
    pParentDfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_10GBPS;
  }
  pParentDfpLane0Adapter->AvailableBandwidth = (pParentDfpLane0Adapter->RawBandwidth * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                                               GUARD_BAND_BANDWIDTH_DENOMINATOR;
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Update Parent Router DFP Lane0 Bandwidth Data after Link speed change\n"
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "  Raw Bandwidth                   = %d\n",
    pParentDfpLane0Adapter->RawBandwidth
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "  Available Bandwidth             = %d\n",
    pParentDfpLane0Adapter->AvailableBandwidth
    );


  //
  // Lane1
  //
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pParentDfpLane1Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                                       // CsType
             (UINT32) (pParentDfpLane1Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pParentDfpLane1Adapter->LaneCapability + 1),   // ReadAddress
             DataSize,                                                // ReadSize
             (UINT32**)&pLaneAdpCs1,                                  // DataOut
             &DataSize                                                // DataSize
             );
  if (EFI_ERROR (Status) || (1 != DataSize)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("%a Error reading Lane capabilities\n", __FUNCTION__);
    return EFI_DEVICE_ERROR;
  }
  pParentDfpLane1Adapter->LaneAdp_TargetSpeed   = pLaneAdpCs1->Field.TargetLinkSpeed;
  pParentDfpLane1Adapter->LaneAdp_TargetWidth   = pLaneAdpCs1->Field.TargetLinkWidth;
  pParentDfpLane1Adapter->LaneAdp_CurrentSpeed  = pLaneAdpCs1->Field.CurrentLinkSpeed;
  pParentDfpLane1Adapter->LaneAdp_CurrentWidth  = pLaneAdpCs1->Field.NegotiatedLinkWidth;
  pParentDfpLane1Adapter->LaneAdp_LinkDisable   = pLaneAdpCs1->Field.LinkDisable;
  pParentDfpLane1Adapter->LaneAdp_LinkBonding   = pLaneAdpCs1->Field.LaneBonding;

  IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane1Adapter LaneAdpCs1 = 0x%x\n",
    pLaneAdpCs1->Value
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane1Adapter LaneAdp_TargetSpeed  = 0x%x\n",
    pParentDfpLane1Adapter->LaneAdp_TargetSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane1Adapter LaneAdp_TargetWidth  = 0x%x\n",
    pParentDfpLane1Adapter->LaneAdp_TargetWidth
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane1Adapter LaneAdp_CurrentSpeed = 0x%x\n",
    pParentDfpLane1Adapter->LaneAdp_CurrentSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane1Adapter LaneAdp_CurrentWidth = 0x%x\n",
    pParentDfpLane1Adapter->LaneAdp_CurrentWidth
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane1Adapter LaneAdp_LinkDisable  = 0x%x\n",
    pParentDfpLane1Adapter->LaneAdp_LinkDisable
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pParentDfpLane1Adapter LaneAdp_LinkBonding  = 0x%x\n",
    pParentDfpLane1Adapter->LaneAdp_LinkBonding
    );
  IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");

  //
  // Update Bandwidth Data
  //
  if ((pParentDfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
    && (pParentDfpLane1Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2)) {
    pParentDfpLane1Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_40GBPS;
  } else if ((pParentDfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
    && (pParentDfpLane1Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1)) {
    pParentDfpLane1Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
  } else if ((pParentDfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
    && (pParentDfpLane1Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2)) {
    pParentDfpLane1Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
  } else if ((pParentDfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
    && (pParentDfpLane1Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1)) {
    pParentDfpLane1Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_10GBPS;
  }
  pParentDfpLane1Adapter->AvailableBandwidth = (pParentDfpLane1Adapter->RawBandwidth * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                                                GUARD_BAND_BANDWIDTH_DENOMINATOR;
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Update Parent Router DFP Lane1 Bandwidth Data after Link speed change\n"
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "  Raw Bandwidth                   = %d\n",
    pParentDfpLane1Adapter->RawBandwidth
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "  Available Bandwidth             = %d\n",
    pParentDfpLane1Adapter->AvailableBandwidth
    );


  //
  // Update UFP Lane Adapter Info
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("Update UFP Adapter fields after Downstream Port Reset.\n");
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pChildUfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                                    // CsType
             (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),  // AdapterNum
             (UINT32) (pChildUfpLane0Adapter->LaneCapability + 1), // ReadAddress
             DataSize,                                             // ReadSize
             (UINT32**)&pLaneAdpCs1,                               // DataOut
             &DataSize                                             // DataSize
             );
  if (EFI_ERROR (Status) || (1 != DataSize)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("%a Error reading Lane capabilities\n", __FUNCTION__);
    return EFI_DEVICE_ERROR;
  }
  pChildUfpLane0Adapter->LaneAdp_TargetSpeed   = pLaneAdpCs1->Field.TargetLinkSpeed;
  pChildUfpLane0Adapter->LaneAdp_TargetWidth   = pLaneAdpCs1->Field.TargetLinkWidth;
  pChildUfpLane0Adapter->LaneAdp_CurrentSpeed  = pLaneAdpCs1->Field.CurrentLinkSpeed;
  pChildUfpLane0Adapter->LaneAdp_CurrentWidth  = pLaneAdpCs1->Field.NegotiatedLinkWidth;
  pChildUfpLane0Adapter->LaneAdp_LinkDisable   = pLaneAdpCs1->Field.LinkDisable;
  pChildUfpLane0Adapter->LaneAdp_LinkBonding   = pLaneAdpCs1->Field.LaneBonding;

  IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane0Adapter LaneAdpCs1 = 0x%x\n",
    pLaneAdpCs1->Value
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane0Adapter LaneAdp_TargetSpeed  = 0x%x\n",
    pChildUfpLane0Adapter->LaneAdp_TargetSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane0Adapter LaneAdp_TargetWidth  = 0x%x\n",
    pChildUfpLane0Adapter->LaneAdp_TargetWidth
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane0Adapter LaneAdp_CurrentSpeed = 0x%x\n",
    pChildUfpLane0Adapter->LaneAdp_CurrentSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane0Adapter LaneAdp_CurrentWidth = 0x%x\n",
    pChildUfpLane0Adapter->LaneAdp_CurrentWidth
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane0Adapter LaneAdp_LinkDisable  = 0x%x\n",
    pChildUfpLane0Adapter->LaneAdp_LinkDisable
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane0Adapter LaneAdp_LinkBonding  = 0x%x\n",
    pChildUfpLane0Adapter->LaneAdp_LinkBonding
    );
  IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");

  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pChildUfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                                     // CsType
             (UINT32) (pChildUfpLane0Adapter->Header.AdapterNum),   // AdapterNum
             (UINT32) (pChildUfpLane0Adapter->Usb4Capability + 18), // ReadAddress: PORT_CS_18
             1,                                                     // ReadSize
             (UINT32**)&pPortCs18,                                  // DataOut
             &DataSize                                              // DataSize
             );
  if (EFI_ERROR (Status) || (1 != DataSize)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("%a Error reading USB4 capabilities\n", __FUNCTION__);
    return EFI_DEVICE_ERROR;
  }
  pChildUfpLane0Adapter->Usb4Port_CableVersion  = pPortCs18->Field.CableUSB4Version;
  pChildUfpLane0Adapter->Usb4Port_BE            = pPortCs18->Field.BondingEnabled;
  pChildUfpLane0Adapter->Usb4Port_TCM           = pPortCs18->Field.TBT3CompatibleMode;
  pChildUfpLane0Adapter->Usb4Port_LCL           = pPortCs18->Field.LinkCLxSupport;
  pChildUfpLane0Adapter->Usb4Port_RE2           = pPortCs18->Field.RsFecEnabledGen2;
  pChildUfpLane0Adapter->Usb4Port_RE3           = pPortCs18->Field.RsFecEnabledGen3;
  pChildUfpLane0Adapter->Usb4Port_RD            = pPortCs18->Field.RouterDetected;

  //
  // Update Bandwidth Data
  //
  if ((pChildUfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
    && (pChildUfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2)) {
    pChildUfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_40GBPS;
  } else if ((pChildUfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
    && (pChildUfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1)) {
    pChildUfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
  } else if ((pChildUfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
    && (pChildUfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2)) {
    pChildUfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
  } else if ((pChildUfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
    && (pChildUfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1)) {
    pChildUfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_10GBPS;
  }
  pChildUfpLane0Adapter->AvailableBandwidth = (pChildUfpLane0Adapter->RawBandwidth * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                                               GUARD_BAND_BANDWIDTH_DENOMINATOR;
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "Update Child Router UFP Lane0 Bandwidth Data after Link speed change\n"
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "  Raw Bandwidth                   = %d\n",
    pChildUfpLane0Adapter->RawBandwidth
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "  Available Bandwidth             = %d\n",
    pChildUfpLane0Adapter->AvailableBandwidth
    );

  //
  // Lane1
  //
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pChildUfpLane1Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,                                    // CsType
             (UINT32) (pChildUfpLane1Adapter->Header.AdapterNum),  // AdapterNum
             (UINT32) (pChildUfpLane1Adapter->LaneCapability + 1), // ReadAddress
             DataSize,                                             // ReadSize
             (UINT32**)&pLaneAdpCs1,                               // DataOut
             &DataSize                                             // DataSize
             );
  if (EFI_ERROR (Status) || (1 != DataSize)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("%a Error reading Lane capabilities\n", __FUNCTION__);
    return EFI_DEVICE_ERROR;
  }
  pChildUfpLane1Adapter->LaneAdp_TargetSpeed   = pLaneAdpCs1->Field.TargetLinkSpeed;
  pChildUfpLane1Adapter->LaneAdp_TargetWidth   = pLaneAdpCs1->Field.TargetLinkWidth;
  pChildUfpLane1Adapter->LaneAdp_CurrentSpeed  = pLaneAdpCs1->Field.CurrentLinkSpeed;
  pChildUfpLane1Adapter->LaneAdp_CurrentWidth  = pLaneAdpCs1->Field.NegotiatedLinkWidth;
  pChildUfpLane1Adapter->LaneAdp_LinkDisable   = pLaneAdpCs1->Field.LinkDisable;
  pChildUfpLane1Adapter->LaneAdp_LinkBonding   = pLaneAdpCs1->Field.LaneBonding;

  IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane1Adapter LaneAdpCs1 = 0x%x\n",
    pLaneAdpCs1->Value
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane1Adapter LaneAdp_TargetSpeed  = 0x%x\n",
    pChildUfpLane1Adapter->LaneAdp_TargetSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane1Adapter LaneAdp_TargetWidth  = 0x%x\n",
    pChildUfpLane1Adapter->LaneAdp_TargetWidth
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane1Adapter LaneAdp_CurrentSpeed = 0x%x\n",
    pChildUfpLane1Adapter->LaneAdp_CurrentSpeed
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane1Adapter LaneAdp_CurrentWidth = 0x%x\n",
    pChildUfpLane1Adapter->LaneAdp_CurrentWidth
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane1Adapter LaneAdp_LinkDisable  = 0x%x\n",
    pChildUfpLane1Adapter->LaneAdp_LinkDisable
    );
  IDS_HDT_CONSOLE_USB4_DEBUG (
    "pChildUfpLane1Adapter LaneAdp_LinkBonding  = 0x%x\n",
    pChildUfpLane1Adapter->LaneAdp_LinkBonding
    );
  IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");

  //
  // Update Bandwidth Data
  //
  if ((pChildUfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
    && (pChildUfpLane1Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2)) {
    pChildUfpLane1Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_40GBPS;
  } else if ((pChildUfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
    && (pChildUfpLane1Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1)) {
    pChildUfpLane1Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
  } else if ((pChildUfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
    && (pChildUfpLane1Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2)) {
    pChildUfpLane1Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
  } else if ((pChildUfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
    && (pChildUfpLane1Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1)) {
    pChildUfpLane1Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_10GBPS;
  }
  pChildUfpLane1Adapter->AvailableBandwidth = (pChildUfpLane1Adapter->RawBandwidth * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                                                         GUARD_BAND_BANDWIDTH_DENOMINATOR;
  IDS_HDT_CONSOLE_USB4_DEBUG ("Update Child Router UFP Lane1 Bandwidth Data after Link speed change\n");
  IDS_HDT_CONSOLE_USB4_DEBUG ("  Raw Bandwidth                   = %d\n", pChildUfpLane1Adapter->RawBandwidth);
  IDS_HDT_CONSOLE_USB4_DEBUG ("  Available Bandwidth              = %d\n", pChildUfpLane1Adapter->AvailableBandwidth);

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Set target link speed on DFP
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pDfpLane0Adapter    Point to Down Facing Port Lane0 Adapter
 * @param[in] LinkSpeedGen3        Target link Speed is Gen3
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
**/
EFI_STATUS
AmdUsb4PortSetDfpTargetSpeed (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pDfpLane0Adapter,
  IN     BOOLEAN                    LinkSpeedGen3
  )
{
  EFI_STATUS                 Status;
  UINT32                     DataSize;
  AMD_USB4_LANE_ADP_CS1      *pLaneAdpCs1;
  AMD_USB4_CM_LANE_ADAPTER   *pDfpLane1Adapter;

  Status                = EFI_SUCCESS;
  DataSize              = 0;
  pLaneAdpCs1           = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if (NULL == pDfpLane0Adapter || NULL) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  pDfpLane1Adapter      = (AMD_USB4_CM_LANE_ADAPTER *) ((UINTN)pDfpLane0Adapter + sizeof (AMD_USB4_CM_CELL));

  //
  // Target Link Speed field for both Lane Adapter in DFP
  //
  // Lane0
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pDfpLane0Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             1,                                                 // ReadSize 1 DWs
             (UINT32**)&pLaneAdpCs1,                            // DataOut
             &DataSize                                          // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  pLaneAdpCs1->Field.TargetLinkSpeed = ( LinkSpeedGen3 ) ? TARGET_LINK_SPEED_GEN3: TARGET_LINK_SPEED_GEN2;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pDfpLane0Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pDfpLane0Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pDfpLane0Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             &DataSize,
             &(pLaneAdpCs1->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  // Lane1
  DataSize = 1;
  Status = AmdUsb4Ring0ReadCS (
             pCM,
             pDfpLane1Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pDfpLane1Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pDfpLane1Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             1,                                                 // ReadSize 1 DWs
             (UINT32**)&pLaneAdpCs1,                            // DataOut
             &DataSize                                          // DataSize
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  pLaneAdpCs1->Field.TargetLinkSpeed = ( LinkSpeedGen3 ) ? TARGET_LINK_SPEED_GEN3: TARGET_LINK_SPEED_GEN2;
  DataSize = 1;
  Status = AmdUsb4Ring0WriteCS (
             pCM,
             pDfpLane1Adapter->Header.TopologyID,
             CS_TARGET_ADAPTER,
             (UINT32) (pDfpLane1Adapter->Header.AdapterNum),    // AdapterNum
             (UINT32) (pDfpLane1Adapter->LaneCapability + 1),   // ReadAddress: LANE_ADP_CS1
             &DataSize,
             &(pLaneAdpCs1->Value)
             );
  if (EFI_ERROR (Status) || 1 != DataSize ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("Failed to write LANE_ADP_CS1 Register.\n");
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Link Configuration before Router Enumeration
 *
 * @param[in] *pCm                 Point to global structure AMD_USB4_CM
 * @param[in] *pDfpLane0Adapter    Point to Down Facing Port Lane0 Adapter
 *
 * @retval EFI_SUCCESS        Success.
 * @retval EFI_DEVICE_ERROR   Failed.
**/
EFI_STATUS
AmdUsb4PortLinkConfigure (
  IN     AMD_USB4_CM                *pCM,
  IN     AMD_USB4_CM_LANE_ADAPTER   *pDfpLane0Adapter
  )
{
  EFI_STATUS                                      Status;
  BOOLEAN                                         RsFecChange;
  BOOLEAN                                         LinkSpeedChange;
  UINT32                                          DataSize;
  AMD_USB4_PORT_CS19                              *pPortCs19;
  AMD_USB4_LANE_ADAPTER_CONFIGURATION_CAPABILITY  *LaneAdapterCapability;
  AMD_USB4_PORT_CAPABILITY                        *USB4AdapterCapability;
  AMD_USB4_CM_LANE_ADAPTER                        *pDfpLane1Adapter;

  Status                = EFI_SUCCESS;
  RsFecChange           = FALSE;
  LinkSpeedChange       = FALSE;
  DataSize              = 0;
  pPortCs19             = NULL;
  USB4AdapterCapability = NULL;
  LaneAdapterCapability = NULL;

  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Start\n", __FUNCTION__);

  if (NULL == pDfpLane0Adapter || NULL == pCM) {
    IDS_HDT_CONSOLE_USB4_ERROR ("One of the input parameters is NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  pDfpLane1Adapter      = (AMD_USB4_CM_LANE_ADAPTER *) ((UINTN)pDfpLane0Adapter + sizeof (AMD_USB4_CM_CELL));

  //
  // Check if need to change link speed
  //
  switch (pCM->LinkSpeed) {
    case 0:
      // Gen2
      if ((pDfpLane0Adapter->LaneAdp_AdapterState == ADAPTER_STATE_CL0)
        && (pDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
        && (pDfpLane1Adapter->LaneAdp_AdapterState == ADAPTER_STATE_CL0)
        && (pDfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)) {
        IDS_HDT_CONSOLE_USB4_DEBUG ("Port is already running on Gen2 speed! \n");
        LinkSpeedChange = FALSE;
      } else if ((pDfpLane0Adapter->LaneAdp_TargetSpeed == TARGET_LINK_SPEED_GEN2)
        && (pDfpLane1Adapter->LaneAdp_TargetSpeed == TARGET_LINK_SPEED_GEN2)) {
        IDS_HDT_CONSOLE_USB4_DEBUG ("Port is already set Target Speed to Gen2 speed! \n");
        LinkSpeedChange = FALSE;
      } else {
        IDS_HDT_CONSOLE_USB4_DEBUG ("Port Change to Gen2 speed! \n");
        LinkSpeedChange = TRUE;
        Status = AmdUsb4PortSetDfpTargetSpeed (pCM, pDfpLane0Adapter, FALSE);
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_USB4_ERROR ("Failed to change DFP speed.\n");
          return EFI_DEVICE_ERROR;
        }
      }
      break;

    case 1:
      // Gen3
      if ((pDfpLane0Adapter->LaneAdp_AdapterState == ADAPTER_STATE_CL0)
        && (pDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
        && (pDfpLane1Adapter->LaneAdp_AdapterState == ADAPTER_STATE_CL0)
        && (pDfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)) {
        IDS_HDT_CONSOLE_USB4_DEBUG ("Port is already running on Gen3 speed! \n");
        LinkSpeedChange = FALSE;
      } else if ((pDfpLane0Adapter->LaneAdp_TargetSpeed == TARGET_LINK_SPEED_GEN3)
        && (pDfpLane1Adapter->LaneAdp_TargetSpeed == TARGET_LINK_SPEED_GEN3)) {
        IDS_HDT_CONSOLE_USB4_DEBUG ("Port is already set Target Speed to Gen3 speed! \n");
        LinkSpeedChange = FALSE;
      } else {
        IDS_HDT_CONSOLE_USB4_DEBUG ("Port Change to Gen3 speed! \n");
        LinkSpeedChange = TRUE;
        Status = AmdUsb4PortSetDfpTargetSpeed (pCM, pDfpLane0Adapter, TRUE);
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_USB4_ERROR ("Failed to change DFP speed.\n");
          return EFI_DEVICE_ERROR;
        }
      }
      break;

    default:
      //  Check if need to downgrade to Gen2 if not speific speed specified
      LinkSpeedChange = FALSE;
      break;
  }

  // check if RS2/RS3 is set to non-Auto
  if ((pCM->RequestRsFecGen2 != 0xFF) || (pCM->RequestRsFecGen3 != 0xFF)) {
    //
    // Get PORT_CS_19
    //
    DataSize = 1;
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pDfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pDfpLane0Adapter->Header.AdapterNum),          // AdapterNum
               (UINT32) (pDfpLane0Adapter->Usb4Capability + 19),        // ReadAddress: PORT_CS_19
               1,                                                       // ReadSize 1 DWs
               (UINT32**)&pPortCs19,                                    // DataOut
               &DataSize                                                // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read PORT_CS_19 Register.\n");
      return EFI_DEVICE_ERROR;
    }

    IDS_HDT_CONSOLE_USB4_DEBUG ("pDfpLane0Adapter PORT_CS_19 = 0x%x\n", pPortCs19->Value);

    if ( pCM->RequestRsFecGen2 == pPortCs19->Field.RequestRsFecGen2
      && pCM->RequestRsFecGen3 == pPortCs19->Field.RequestRsFecGen3 )
    {
      IDS_HDT_CONSOLE_USB4_DEBUG ("No need to re-set RS-FEC GEN2 & GEN3.\n");
      RsFecChange = FALSE;
    } else {
      RsFecChange = TRUE;
    }
  }

  if (RsFecChange || LinkSpeedChange) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("Start DFP RESET to change Link Configuration!!!\n");
    IDS_HDT_CONSOLE_USB4_DEBUG ("  1, Get PORT_CS_19.\n");
    //
    // Get PORT_CS_19
    //
    DataSize = 1;
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pDfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,
               (UINT32) (pDfpLane0Adapter->Header.AdapterNum),          // AdapterNum
               (UINT32) (pDfpLane0Adapter->Usb4Capability + 19),        // ReadAddress: PORT_CS_19
               1,                                                       // ReadSize 1 DWs
               (UINT32**)&pPortCs19,                                    // DataOut
               &DataSize                                                // DataSize
               );
    if (EFI_ERROR (Status) || 1 != DataSize ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Failed to read PORT_CS_19 Register.\n");
      return EFI_DEVICE_ERROR;
    }
    IDS_HDT_CONSOLE_USB4_DEBUG ("    pDfpLane0Adapter PORT_CS_19 = 0x%x\n", pPortCs19->Value);

    //
    // Update PORT_CS_19 accordingly with RS2/RS3 and other bits
    //
    IDS_HDT_CONSOLE_USB4_DEBUG ("  2, Set RS2 and RS3 in PORT_CS_19.\n");
    if (pCM->RequestRsFecGen2) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("    Set PORT_CS_19 RS2 = 1\n");
      pPortCs19->Field.RequestRsFecGen2 = 1;
    } else {
      IDS_HDT_CONSOLE_USB4_DEBUG ("    Set PORT_CS_19 RS2 = 0\n");
      pPortCs19->Field.RequestRsFecGen2 = 0;
    }

    if (pCM->RequestRsFecGen3) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("    Set PORT_CS_19 RS3 = 1\n");
      pPortCs19->Field.RequestRsFecGen3 = 1;
    } else {
      IDS_HDT_CONSOLE_USB4_DEBUG ("    Set PORT_CS_19 RS3 = 0\n");
      pPortCs19->Field.RequestRsFecGen3 = 0;
    }

    //
    // Downstream Port Reset
    //
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "  3, Reset DFP of Router 0x%lx Adapter %d\n",
      pDfpLane0Adapter->Header.TopologyID,
      pDfpLane0Adapter->Header.AdapterNum
      );
    Status = AmdUsb4DownstreamPortReset (
               pCM,
               pDfpLane0Adapter,
               pPortCs19
               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_USB4_ERROR ("Cannot do Downstream Port Reset.\n");
      return EFI_DEVICE_ERROR;
    }

    //
    // Update Data
    //
    IDS_HDT_CONSOLE_USB4_DEBUG ("  4, Update data after DPR.\n");
    // Port Capability
    IDS_HDT_CONSOLE_USB4_DEBUG ("     Read Port Capability...\n");
    DataSize = sizeof (AMD_USB4_PORT_CAPABILITY) / sizeof (UINT32);
    Status = AmdUsb4Ring0ReadCS (
               pCM,
               pDfpLane0Adapter->Header.TopologyID,
               CS_TARGET_ADAPTER,                                   // CsType
               (UINT32) (pDfpLane0Adapter->Header.AdapterNum),      // AdapterNum
               (UINT32) (pDfpLane0Adapter->Usb4Capability),         // ReadAddress
               DataSize,                                            // ReadSize
               (UINT32**)&USB4AdapterCapability,                    // DataOut
               &DataSize                                            // DataSize
               );
    if (EFI_ERROR (Status) || ((sizeof (AMD_USB4_PORT_CAPABILITY) / sizeof (UINT32) ) != DataSize)) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("%a Error reading USB4 capabilities\n", __FUNCTION__);
      return EFI_DEVICE_ERROR;
    }
    pDfpLane0Adapter->Usb4Port_CableVersion  = USB4AdapterCapability->PortCs18.Field.CableUSB4Version;
    pDfpLane0Adapter->Usb4Port_BE            = USB4AdapterCapability->PortCs18.Field.BondingEnabled;
    pDfpLane0Adapter->Usb4Port_TCM           = USB4AdapterCapability->PortCs18.Field.TBT3CompatibleMode;
    pDfpLane0Adapter->Usb4Port_LCL           = USB4AdapterCapability->PortCs18.Field.LinkCLxSupport;
    pDfpLane0Adapter->Usb4Port_RE2           = USB4AdapterCapability->PortCs18.Field.RsFecEnabledGen2;
    pDfpLane0Adapter->Usb4Port_RE3           = USB4AdapterCapability->PortCs18.Field.RsFecEnabledGen3;
    pDfpLane0Adapter->Usb4Port_RD            = USB4AdapterCapability->PortCs18.Field.RouterDetected;
    IDS_HDT_CONSOLE_USB4_DEBUG ("       Usb4Port_RE2 = 0x%x\n", pDfpLane0Adapter->Usb4Port_RE2);
    IDS_HDT_CONSOLE_USB4_DEBUG ("       Usb4Port_RE3 = 0x%x\n", pDfpLane0Adapter->Usb4Port_RE3);

    //
    // Lane Capability
    //
    // Lane0
    do {
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Read Lane Capability of DFP Lane0...\n");
      MicroSecondDelay (1000);   //1ms
      DataSize = sizeof (AMD_USB4_LANE_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32);
      Status = AmdUsb4Ring0ReadCS (
                 pCM,
                 pDfpLane0Adapter->Header.TopologyID,
                 CS_TARGET_ADAPTER,                                   // CsType
                 (UINT32) (pDfpLane0Adapter->Header.AdapterNum),      // AdapterNum
                 pDfpLane0Adapter->LaneCapability,                    // ReadAddress
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
    pDfpLane0Adapter->LaneAdp_SupportSpeed  = LaneAdapterCapability->LaneAdpCs0.Field.SupportedLinkSpeeds;
    pDfpLane0Adapter->LaneAdp_SupportWidth  = LaneAdapterCapability->LaneAdpCs0.Field.SupportedLinkWidths;
    pDfpLane0Adapter->LaneAdp_TargetSpeed   = LaneAdapterCapability->LaneAdpCs1.Field.TargetLinkSpeed;
    pDfpLane0Adapter->LaneAdp_TargetWidth   = LaneAdapterCapability->LaneAdpCs1.Field.TargetLinkWidth;
    pDfpLane0Adapter->LaneAdp_CurrentSpeed  = LaneAdapterCapability->LaneAdpCs1.Field.CurrentLinkSpeed;
    pDfpLane0Adapter->LaneAdp_CurrentWidth  = LaneAdapterCapability->LaneAdpCs1.Field.NegotiatedLinkWidth;
    pDfpLane0Adapter->LaneAdp_LinkDisable   = LaneAdapterCapability->LaneAdpCs1.Field.LinkDisable;
    pDfpLane0Adapter->LaneAdp_LinkBonding   = LaneAdapterCapability->LaneAdpCs1.Field.LaneBonding;
    pDfpLane0Adapter->LaneAdp_AdapterState  = LaneAdapterCapability->LaneAdpCs1.Field.AdapterState;
    IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdpCs0 = 0x%x\n", LaneAdapterCapability->LaneAdpCs0.Value);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdpCs1 = 0x%x\n", LaneAdapterCapability->LaneAdpCs1.Value);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_SupportSpeed = 0x%x\n", pDfpLane0Adapter->LaneAdp_SupportSpeed);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_SupportWidth = 0x%x\n", pDfpLane0Adapter->LaneAdp_SupportWidth);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_TargetSpeed = 0x%x\n", pDfpLane0Adapter->LaneAdp_TargetSpeed);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_TargetWidth = 0x%x\n", pDfpLane0Adapter->LaneAdp_TargetWidth);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_CurrentSpeed = 0x%x\n", pDfpLane0Adapter->LaneAdp_CurrentSpeed);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_CurrentWidth = 0x%x\n", pDfpLane0Adapter->LaneAdp_CurrentWidth);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_LinkDisable = 0x%x\n", pDfpLane0Adapter->LaneAdp_LinkDisable);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_LinkBonding = 0x%x\n", pDfpLane0Adapter->LaneAdp_LinkBonding);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_AdapterState = 0x%x\n", pDfpLane0Adapter->LaneAdp_AdapterState);
    IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");

    //
    // Bandwidth
    //
    if ((pDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
      && (pDfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2))
    {
      pDfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_40GBPS;
    } else if ((pDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
      && (pDfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1))
    {
      pDfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
    } else if ((pDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
      && (pDfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2))
    {
      pDfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
    } else if ((pDfpLane0Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
      && (pDfpLane0Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1))
    {
      pDfpLane0Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_10GBPS;
    }
    IDS_HDT_CONSOLE_USB4_DEBUG ("%a Bandwidth\n", __FUNCTION__);
    IDS_HDT_CONSOLE_USB4_DEBUG ("  Raw Bandwidth                   = %d\n", pDfpLane0Adapter->RawBandwidth);

    //
    // Values for Bandwidth have a scaling factor of USB4_RAW_BANDWIDTH_SCALING factored in
    //
    pDfpLane0Adapter->AvailableBandwidth = (pDfpLane0Adapter->RawBandwidth * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                                              GUARD_BAND_BANDWIDTH_DENOMINATOR;

    // Lane1
    do {
      IDS_HDT_CONSOLE_USB4_DEBUG ("     Read Lane Capability of DFP Lane1...\n");
      MicroSecondDelay (1000);   //1ms
      DataSize = sizeof (AMD_USB4_LANE_ADAPTER_CONFIGURATION_CAPABILITY) / sizeof (UINT32);
      Status = AmdUsb4Ring0ReadCS (
                 pCM,
                 pDfpLane1Adapter->Header.TopologyID,
                 CS_TARGET_ADAPTER,                                   // CsType
                 (UINT32) (pDfpLane1Adapter->Header.AdapterNum),      // AdapterNum
                 pDfpLane1Adapter->LaneCapability,                    // ReadAddress
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
    pDfpLane1Adapter->LaneAdp_SupportSpeed  = LaneAdapterCapability->LaneAdpCs0.Field.SupportedLinkSpeeds;
    pDfpLane1Adapter->LaneAdp_SupportWidth  = LaneAdapterCapability->LaneAdpCs0.Field.SupportedLinkWidths;
    pDfpLane1Adapter->LaneAdp_TargetSpeed   = LaneAdapterCapability->LaneAdpCs1.Field.TargetLinkSpeed;
    pDfpLane1Adapter->LaneAdp_TargetWidth   = LaneAdapterCapability->LaneAdpCs1.Field.TargetLinkWidth;
    pDfpLane1Adapter->LaneAdp_CurrentSpeed  = LaneAdapterCapability->LaneAdpCs1.Field.CurrentLinkSpeed;
    pDfpLane1Adapter->LaneAdp_CurrentWidth  = LaneAdapterCapability->LaneAdpCs1.Field.NegotiatedLinkWidth;
    pDfpLane1Adapter->LaneAdp_LinkDisable   = LaneAdapterCapability->LaneAdpCs1.Field.LinkDisable;
    pDfpLane1Adapter->LaneAdp_LinkBonding   = LaneAdapterCapability->LaneAdpCs1.Field.LaneBonding;
    pDfpLane1Adapter->LaneAdp_AdapterState  = LaneAdapterCapability->LaneAdpCs1.Field.AdapterState;
    IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdpCs0 = 0x%x\n", LaneAdapterCapability->LaneAdpCs0.Value);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdpCs1 = 0x%x\n", LaneAdapterCapability->LaneAdpCs1.Value);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_SupportSpeed = 0x%x\n", pDfpLane1Adapter->LaneAdp_SupportSpeed);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_SupportWidth = 0x%x\n", pDfpLane1Adapter->LaneAdp_SupportWidth);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_TargetSpeed = 0x%x\n", pDfpLane1Adapter->LaneAdp_TargetSpeed);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_TargetWidth = 0x%x\n", pDfpLane1Adapter->LaneAdp_TargetWidth);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_CurrentSpeed = 0x%x\n", pDfpLane1Adapter->LaneAdp_CurrentSpeed);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_CurrentWidth = 0x%x\n", pDfpLane1Adapter->LaneAdp_CurrentWidth);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_LinkDisable = 0x%x\n", pDfpLane1Adapter->LaneAdp_LinkDisable);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_LinkBonding = 0x%x\n", pDfpLane1Adapter->LaneAdp_LinkBonding);
    IDS_HDT_CONSOLE_USB4_DEBUG ("LaneAdp_AdapterState = 0x%x\n", pDfpLane1Adapter->LaneAdp_AdapterState);
    IDS_HDT_CONSOLE_USB4_DEBUG ("=======================================================\n");

    //
    // Bandwidth
    //
    if ((pDfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
      && (pDfpLane1Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2))
    {
      pDfpLane1Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_40GBPS;
    } else if ((pDfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN3)
      && (pDfpLane1Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1))
    {
      pDfpLane1Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
    } else if ((pDfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
      && (pDfpLane1Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X2))
    {
      pDfpLane1Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_20GBPS;
    } else if ((pDfpLane1Adapter->LaneAdp_CurrentSpeed == CURRENT_LINK_SPEED_GEN2)
      && (pDfpLane1Adapter->LaneAdp_CurrentWidth == CURRENT_LINK_WIDTH_X1))
    {
      pDfpLane1Adapter->RawBandwidth = USB4_RAW_BANDWIDTH_10GBPS;
    }
    IDS_HDT_CONSOLE_USB4_DEBUG ("%a Bandwidth\n", __FUNCTION__);
    IDS_HDT_CONSOLE_USB4_DEBUG ("  Raw Bandwidth                   = %d\n", pDfpLane1Adapter->RawBandwidth);

    //
    // Values for Bandwidth have a scaling factor of USB4_RAW_BANDWIDTH_SCALING factored in
    //
    pDfpLane1Adapter->AvailableBandwidth = (pDfpLane1Adapter->RawBandwidth * GUARD_BAND_BANDWIDTH_NUMERATOR) /
                                              GUARD_BAND_BANDWIDTH_DENOMINATOR;

  }

  return EFI_SUCCESS;
}

