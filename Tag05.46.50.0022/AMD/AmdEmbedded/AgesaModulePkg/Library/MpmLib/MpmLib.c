/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include "AGESA.h"
#include "Filecode.h"
#include <Library/BaseLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/MpmMboxLib.h>
#include <Library/MpmFuncLib.h>

#define FILECODE LIBRARY_MPMLIB_MPMLIB_FILECODE

/**
  Get the Text redirection TX buffer which used for text redirection
  * @param[in,out]  Buffer  point to the Address used as TX buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as TX buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmGetTxrTxBufferAddrSize (
  IN OUT VOID **Buffer,
  IN OUT UINT32 *BufferSize
  )
{
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;
  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }
  *Buffer = (VOID *) (UINTN) Data->TextTxBufferAddr;
  *BufferSize = Data->TextTxBufferSize;
  return EFI_SUCCESS;
}

/**
  Get the Text redirection RX buffer which used for text redirection
  * @param[in,out]  Buffer  point to the Address used as RX buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as RX buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmGetTxrRxBufferAddrSize (
  IN OUT VOID **Buffer,
  IN OUT UINT32 *BufferSize
  )
{
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;
  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }
  *Buffer = (VOID *) (UINTN) Data->TextRxBufferAddr;
  *BufferSize = Data->TextRxBufferSize;
  return EFI_SUCCESS;
}

/**
  BIOS send command to MPM to process PLDM message
  * @param[in]   RequestBuffer         Point to the PLDM request message memory buffer,
                                       Caller need prepare it before calling this function
  * @param[in]   RequestBufferSize     Size of the PLDM request message memory buffer, including header
  * @param[out]  ResponseBuffer        Point to the address of PLDM response message memory buffer,
                                       Keep as NULL, if message doesn't require response
                                       This field will be ignored and keep untouched, if response bit is not set by MPM
                                       For message require response, Caller need allocate the buffer prior calling this routine
                                       Callee will copy the content from MPM dedicate mailbox buffer the caller allocate one.
  * @param[out]  ResponseBufferSize    This field will be ignored and keep untouched, if response bit is not set by MPM
                                       On input, Point to the size of ResponseBuffer
                                       On Output, if ResponseBufferSize is smaller than required, update to the required value with
                                       Status EFI_BUFFER_TOO_SMALL

   @retval EFI_STATUS                  0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmPldmMsg (
  IN     VOID   *RequestBuffer,
  IN     UINT32 RequestBufferSize,
  IN OUT VOID   *ResponseBuffer,
  IN OUT UINT32 *ResponseBufferSize
  )
{
  return BiosMpmCmdPldmMsg (RequestBuffer, RequestBufferSize, ResponseBuffer, ResponseBufferSize);
}

/**
  BIOS send command to MPM to process Alert message
  * @param[in]   RequestBuffer         Point to the Alert request message memory buffer
                                       Caller need prepare it before calling this function
  * @param[in]   RequestBufferSize     Size of the Alert request message memory buffer, including header
   @retval EFI_STATUS                  0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmAlertMsg (
  IN     VOID   *RequestBuffer,
  IN     UINT32 RequestBufferSize
  )
{
  return BiosMpmCmdAlertMsg (RequestBuffer, RequestBufferSize);
}


/**
  Query is there any pending TextRedirect task
  * @param[in]   IsTaskPending        Point to the status of pending task,
                                      TRUE: there is some pending task,
                                      FALSE: No pending task
   @retval EFI_STATUS                 0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmIsTextRedirectPendingTask (
     OUT BOOLEAN *IsTaskPending
  )
{
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;


  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  *IsTaskPending = Data->WirelessTxrPending;
  return EFI_SUCCESS;
}

/**
  Query is there any pending KVM task
  * @param[in]   IsTaskPending        Point to the status of pending task,
                                      TRUE: there is some pending task,
                                      FALSE: No pending task
   @retval EFI_STATUS                 0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmIsKvmPendingTask (
     OUT BOOLEAN *IsTaskPending
  )
{
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;


  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  *IsTaskPending = Data->WirelessKvmPending;
  return EFI_SUCCESS;
}

/**
  BIOS send battery information to MPM
  * @param[in,out]  BatteryInfo  Point to the structure contain battery information

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmBatteryMsg (
  BATTERY_INFO_T * BatteryInfo
  )
{
  return BiosMpmCmdBatteryMsg (BatteryInfo);
}

/**
  Check if system is AIM-T capable by reading "Manageability Fuse Enable" Fuse Values: AIM-T + Manageability Enable
  from MPM_PRVIATE_HEAP_DATA_V1.ManageabilityConfig

   @retval BOOLEAN              0: AimT unsupported, 1 AimTCapable
**/
BOOLEAN
IsAimTCapable (
  )
{
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;
  BOOLEAN AimTCapable;

  AimTCapable = FALSE;
  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  AimTCapable = (Data->ManageabilityConfig) & AIM_T_CAPABLE_MASK;
  return AimTCapable;
}

/**
  Check if Mpm enabled, by both HW capable IsAimTCapable and PCD value
  from MPM_PRVIATE_HEAP_DATA_V1.ManageabilityConfig

   @retval BOOLEAN              0: Mpm disabled, 1 AimTCapable
**/
BOOLEAN
IsMpmEnabled (
  )
{
  if ((PcdGetBool (PcdMpmEnable) == TRUE) && (IsAimTCapable ())) {
    return TRUE;
  } else {
    return FALSE;
  }
}
