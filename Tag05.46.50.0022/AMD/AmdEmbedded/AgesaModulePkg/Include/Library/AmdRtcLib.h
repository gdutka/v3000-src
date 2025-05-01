/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
/** This file refer EDKII PcAtChipsetPkg\PcatRealTimeClockRuntimeDxe **/
/** @file
  RTC Architectural Protocol GUID as defined in DxeCis 0.96.

Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2017, AMD Inc. All rights reserved.<BR>
**/


#ifndef _RTC_H_
#define _RTC_H_


#include <Uefi.h>

#include <Guid/Acpi.h>

#include <Protocol/RealTimeClock.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/ReportStatusCodeLib.h>

typedef struct {
  EFI_LOCK  RtcLock;
  INT16     SavedTimeZone;
  UINT8     Daylight;
  UINT8     CenturyRtcAddress;
} PC_RTC_MODULE_GLOBALS;

#define RTC_INDEX_REGISTER            0x70
#define RTC_TARGET_REGISTER           0x71
#define REAL_TIME_CLOCK_UPDATE_TIMEOUT 100000
#define MINIMAL_VALID_YEAR           1900
#define MAXIMAL_VALID_YEAR           2999

//
// Dallas DS12C887 Real Time Clock
//
#define RTC_ADDRESS_SECONDS           0   // R/W  Range 0..59
#define RTC_ADDRESS_SECONDS_ALARM     1   // R/W  Range 0..59
#define RTC_ADDRESS_MINUTES           2   // R/W  Range 0..59
#define RTC_ADDRESS_MINUTES_ALARM     3   // R/W  Range 0..59
#define RTC_ADDRESS_HOURS             4   // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
#define RTC_ADDRESS_HOURS_ALARM       5   // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
#define RTC_ADDRESS_DAY_OF_THE_WEEK   6   // R/W  Range 1..7
#define RTC_ADDRESS_DAY_OF_THE_MONTH  7   // R/W  Range 1..31
#define RTC_ADDRESS_MONTH             8   // R/W  Range 1..12
#define RTC_ADDRESS_YEAR              9   // R/W  Range 0..99
#define RTC_ADDRESS_REGISTER_A        10  // R/W[0..6]  R0[7]
#define RTC_ADDRESS_REGISTER_B        11  // R/W
#define RTC_ADDRESS_REGISTER_C        12  // RO
#define RTC_ADDRESS_REGISTER_D        13  // RO
//
// Date and time initial values.
// They are used if the RTC values are invalid during driver initialization
//
#define RTC_INIT_SECOND 0
#define RTC_INIT_MINUTE 0
#define RTC_INIT_HOUR   0
#define RTC_INIT_DAY    1
#define RTC_INIT_MONTH  1

#pragma pack(1)
//
// Register A
//
typedef struct {
  UINT8 Rs : 4;   // Rate Selection Bits
  UINT8 Dv : 3;   // Divisor
  UINT8 Uip : 1;  // Update in progress
} RTC_REGISTER_A_BITS;

typedef union {
  RTC_REGISTER_A_BITS Bits;
  UINT8               Data;
} RTC_REGISTER_A;

//
// Register B
//
typedef struct {
  UINT8 Dse : 1;  // 0 - Daylight saving disabled  1 - Daylight savings enabled
  UINT8 Mil : 1;  // 0 - 12 hour mode              1 - 24 hour mode
  UINT8 Dm : 1;   // 0 - BCD Format                1 - Binary Format
  UINT8 Sqwe : 1; // 0 - Disable SQWE output       1 - Enable SQWE output
  UINT8 Uie : 1;  // 0 - Update INT disabled       1 - Update INT enabled
  UINT8 Aie : 1;  // 0 - Alarm INT disabled        1 - Alarm INT Enabled
  UINT8 Pie : 1;  // 0 - Periodic INT disabled     1 - Periodic INT Enabled
  UINT8 Set : 1;  // 0 - Normal operation.         1 - Updates inhibited
} RTC_REGISTER_B_BITS;

typedef union {
  RTC_REGISTER_B_BITS Bits;
  UINT8               Data;
} RTC_REGISTER_B;

//
// Register C
//
typedef struct {
  UINT8 Reserved : 4; // Read as zero.  Can not be written.
  UINT8 Uf : 1;       // Update End Interrupt Flag
  UINT8 Af : 1;       // Alarm Interrupt Flag
  UINT8 Pf : 1;       // Periodic Interrupt Flag
  UINT8 Irqf : 1;     // Interrupt Request Flag = PF & PIE | AF & AIE | UF & UIE
} RTC_REGISTER_C_BITS;

typedef union {
  RTC_REGISTER_C_BITS Bits;
  UINT8               Data;
} RTC_REGISTER_C;

//
// Register D
//
typedef struct {
  UINT8 Reserved : 7; // Read as zero.  Can not be written.
  UINT8 Vrt : 1;      // Valid RAM and Time
} RTC_REGISTER_D_BITS;

typedef union {
  RTC_REGISTER_D_BITS Bits;
  UINT8               Data;
} RTC_REGISTER_D;

#pragma pack()

/**
  Read RTC content through its registers.

  @param  Address   Address offset of RTC. It is recommended to use
                    macros such as RTC_ADDRESS_SECONDS.

  @return The data of UINT8 type read from RTC.
**/
UINT8
RtcRead (
  IN  UINTN Address
  );


/**
  See if all fields of a variable of EFI_TIME type is correct.

  @param   Time   The time to be checked.

  @retval  EFI_INVALID_PARAMETER  Some fields of Time are not correct.
  @retval  EFI_SUCCESS            Time is a valid EFI_TIME variable.

**/
EFI_STATUS
RtcTimeFieldsValid (
  IN EFI_TIME *Time
  );

/**
  Converts time from EFI_TIME format defined by UEFI spec to RTC format.

  This function converts time from EFI_TIME format defined by UEFI spec to RTC format.
  If data mode of RTC is BCD, then converts EFI_TIME to it.
  If RTC is in 12-hour format, then converts EFI_TIME to it.

  @param   Time       On input, the time data read from UEFI to convert
                      On output, the time converted to RTC format
  @param   RegisterB  Value of Register B of RTC, indicating data mode
**/
VOID
ConvertEfiTimeToRtcTime (
  IN OUT EFI_TIME        *Time,
  IN     RTC_REGISTER_B  RegisterB
  );


/**
  Converts time read from RTC to EFI_TIME format defined by UEFI spec.

  This function converts raw time data read from RTC to the EFI_TIME format
  defined by UEFI spec.
  If data mode of RTC is BCD, then converts it to decimal,
  If RTC is in 12-hour format, then converts it to 24-hour format.

  @param   Time       On input, the time data read from RTC to convert
                      On output, the time converted to UEFI format
  @param   RegisterB  Value of Register B of RTC, indicating data mode
                      and hour format.

  @retval  EFI_INVALID_PARAMETER  Parameters passed in are invalid.
  @retval  EFI_SUCCESS            Convert RTC time to EFI time successfully.

**/
EFI_STATUS
ConvertRtcTimeToEfiTime (
  IN OUT EFI_TIME        *Time,
  IN     RTC_REGISTER_B  RegisterB
  );

/**
  Wait for a period for the RTC to be ready.

  @param    Timeout  Tell how long it should take to wait.

  @retval   EFI_DEVICE_ERROR   RTC device error.
  @retval   EFI_SUCCESS        RTC is updated and ready.
**/
EFI_STATUS
RtcWaitToUpdate (
  UINTN Timeout
  );

/**
  See if field Day of an EFI_TIME is correct.

  @param    Time   Its Day field is to be checked.

  @retval   TRUE   Day field of Time is correct.
  @retval   FALSE  Day field of Time is NOT correct.
**/
BOOLEAN
DayValid (
  IN  EFI_TIME  *Time
  );

/**
  Check if it is a leapyear.

  @param    Time   The time to be checked.

  @retval   TRUE   It is a leapyear.
  @retval   FALSE  It is NOT a leapyear.
**/
BOOLEAN
IsLeapYear (
  IN EFI_TIME   *Time
  );

#endif
