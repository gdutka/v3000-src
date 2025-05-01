/******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef  _MPM_LIB_H_
#define _MPM_LIB_H_
#pragma pack (push, 1)
//Structure for BIOS_MPM_CMD_BATTERY_MSG

/**
 * @struct BATTERY_INFO_T
 * Structure for BIOS_MPM_CMD_BATTERY_MSG
 */
typedef struct{
  UINT32  BatteryStatus;          ///< battery status
/**
1-  Unknown
2-  Fully Charged
3-  Low
4-  Critical
5-  Charging
6-  Charging and High
7-  Charging and Low
8-  Charging and Critical
9-  Undefined
10-  Partially Charged
11-  Learning
12-  Overcharged
13-  DMTF Reserved
32768-65535 - Vendor Specific
*/
  UINT32  TimeOnBattery;      ///< how long running on battery in seconds
  UINT32  EstimatedRunTime;    ///<  in minutes
  UINT16  EstimatedChargeRemaining;  ///<  in %
  UINT16  Chemistry;      ///< SMBIOS
  UINT16  DesignCapacity;      ///< mWatts/hour
  CHAR8   SmartBatteryVersion[128];       ///< smart battery version array
  UINT32  TimeToFullCharge;    ///< time remainint to full change in minute
  UINT32  ExpectedLife;      ///< in minute
  UINT32  MaxRechargeTime;    ///< 0 to 100 in minute
  UINT32  RechargeCount;      ///< number of time battery recharged
  UINT32  MaxRechargeCount;    ///< how many time can recharge
  UINT32  RemainingCapacity;    ///< remaining % of charge in mWatt
  CHAR8   OtherChemistryDescription[128];  ///< e.g. Lith
  UINT16  ChargingStatus;      ///< charging status
  UINT8   HealthPercent;      ///<  in %
} BATTERY_INFO_T;

//32KB for Tx and Rx ring buffer
#define MPM_TEXT_MAX_RING_DATA_SIZE  (32 * 1024)
#define MPM_TEXT_MAX_RING_DATA_NUM  ((MPM_TEXT_MAX_RING_DATA_SIZE - 2 * sizeof(UINT32)) / sizeof (UINT8))
/**
 * @struct MPM_TEXT_RING_BUFFER
 * Ring buffer structure for text redirect
 */
typedef struct {
  UINT32                WritePointer;   ///< Write Pointer, Update by VNC consume by BIOS
  UINT32                ReadPointer;   ///< Read Pointer, Update by BIOS consume by VNC
  UINT8                 Data[MPM_TEXT_MAX_RING_DATA_NUM];
} MPM_TEXT_RING_BUFFER;

#pragma pack (pop)
/**
  * @brief Get the Text redirection TX buffer which used for text redirection
  * @param[in,out]  Buffer  point to the Address used as TX buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as TX buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmGetTxrTxBufferAddrSize (
  IN OUT VOID **Buffer,           ///< buffer ptr
  IN OUT UINT32 *BufferSize       ///< buffer size
  );

/**
  * @brief Get the Text redirection RX buffer which used for text redirection
  * @param[in,out]  Buffer  point to the Address used as RX buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as RX buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmGetTxrRxBufferAddrSize (
  IN OUT VOID **Buffer,             ///< buffer ptr
  IN OUT UINT32 *BufferSize         ///< buffer size
  );

/**
  * @brief BIOS send command to MPM to process PLDM message
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
  IN     VOID   *RequestBuffer,           ///< request buffer ptr
  IN     UINT32 RequestBufferSize,        ///< request buffer size
  IN OUT VOID   *ResponseBuffer,          ///< response buffer ptr
  IN OUT UINT32 *ResponseBufferSize       ///< response buffer size
  );

/**
  * @brief  BIOS send command to MPM to process Alert message
  * @param[in]   RequestBuffer         Point to the Alert request message memory buffer
                                       Caller need prepare it before calling this function
  * @param[in]   RequestBufferSize     Size of the Alert request message memory buffer, including header
   @retval EFI_STATUS                  0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmAlertMsg (
  IN     VOID   *RequestBuffer,             ///< request buffer
  IN     UINT32 RequestBufferSize           ///< request size buffer
  );

/**
  * @brief  Query is there any pending TextRedirect task
  * @param[in]   IsTaskPending        Point to the status of pending task,
                                      TRUE: there is some pending task,
                                      FALSE: No pending task
   @retval EFI_STATUS                 0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmIsTextRedirectPendingTask (
     OUT BOOLEAN *IsTaskPending
  );

/**
  * @brief  Query is there any pending KVM task
  * @param[in]   IsTaskPending        Point to the status of pending task,
                                      TRUE: there is some pending task,
                                      FALSE: No pending task
   @retval EFI_STATUS                 0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmIsKvmPendingTask (
     OUT BOOLEAN *IsTaskPending
  );

/**
  * @brief  BIOS send battery information to MPM
  * @param[in,out]  BatteryInfo  Point to the structure contain battery information

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmBatteryMsg (
  BATTERY_INFO_T * BatteryInfo          ///< battery information
  );

/**
  * @brief   Check if system is AIM-T capable by reading "Manageability Fuse Enable" Fuse Values: AIM-T + Manageability Enable
  from MPM_PRVIATE_HEAP_DATA_V1.ManageabilityConfig

   @retval BOOLEAN              0: AimT unsupported, 1 AimTCapable
**/
BOOLEAN
IsAimTCapable (
  );

/**
  * @brief  Check if Mpm enabled, by both HW capable IsAimTCapable and PCD value
  from MPM_PRVIATE_HEAP_DATA_V1.ManageabilityConfig

   @retval BOOLEAN              0: Mpm disabled, 1  Mpm enabled
**/
BOOLEAN
IsMpmEnabled (
  );

#endif //_MPM_LIB_H_
