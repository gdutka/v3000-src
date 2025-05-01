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
Copyright (c) 2018 - 2020, ARM Limited. All rights reserved.<BR>
**/

#include <Library/AmdRtcLib.h>

#define FILECODE LIBRARY_AMDRTCLIB_AMDRTC_FILECODE
//
// Days of month.
//
UINTN mDayOfMonth[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/**
  Compare the Hour, Minute and Second of the From time and the To time.

  Only compare H/M/S in EFI_TIME and ignore other fields here.

  @param From   the first time
  @param To     the second time

  @return  >0   The H/M/S of the From time is later than those of To time
  @return  ==0  The H/M/S of the From time is same as those of To time
  @return  <0   The H/M/S of the From time is earlier than those of To time
**/
INTN
CompareHMS (
  IN EFI_TIME   *From,
  IN EFI_TIME   *To
  );

/**
  To check if second date is later than first date within 24 hours.

  @param  From   the first date
  @param  To     the second date

  @retval TRUE   From is previous to To within 24 hours.
  @retval FALSE  From is later, or it is previous to To more than 24 hours.
**/
BOOLEAN
IsWithinOneDay (
  IN EFI_TIME   *From,
  IN EFI_TIME   *To
  );

/**
  Read RTC content through its registers using IO access.

  @param  Address   Address offset of RTC. It is recommended to use
                    macros such as RTC_ADDRESS_SECONDS.

  @return The data of UINT8 type read from RTC.
**/
STATIC
UINT8
IoRtcRead (
  IN  UINTN Address
  )
{
  IoWrite8 (
    RTC_INDEX_REGISTER,
    (UINT8)(Address | (UINT8)(IoRead8 (RTC_INDEX_REGISTER) & 0x80))
    );
  return IoRead8 (RTC_TARGET_REGISTER);
}

/**
  Write RTC through its registers  using IO access.

  @param  Address   Address offset of RTC. It is recommended to use
                    macros such as RTC_ADDRESS_SECONDS.
  @param  Data      The content you want to write into RTC.

**/
STATIC
VOID
IoRtcWrite (
  IN  UINTN   Address,
  IN  UINT8   Data
  )
{
  IoWrite8 (
    RTC_INDEX_REGISTER,
    (UINT8)(Address | (UINT8)(IoRead8 (RTC_INDEX_REGISTER) & 0x80))
    );
  IoWrite8 (RTC_TARGET_REGISTER, Data);
}

/**
  Read RTC content through its registers.

  @param  Address   Address offset of RTC. It is recommended to use
                    macros such as RTC_ADDRESS_SECONDS.

  @return The data of UINT8 type read from RTC.
**/
UINT8
RtcRead (
  IN  UINTN Address
  )
{
  return IoRtcRead (Address);
}

/**
  Write RTC through its registers.

  @param  Address   Address offset of RTC. It is recommended to use
                    macros such as RTC_ADDRESS_SECONDS.
  @param  Data      The content you want to write into RTC.

**/
VOID
RtcWrite (
  IN  UINTN   Address,
  IN  UINT8   Data
  )
{
  IoRtcWrite (Address, Data);
}

/**
  Checks an 8-bit BCD value, and converts to an 8-bit value if valid.

  This function checks the 8-bit BCD value specified by Value.
  If valid, the function converts it to an 8-bit value and returns it.
  Otherwise, return 0xff.

  @param   Value The 8-bit BCD value to check and convert

  @return  The 8-bit value converted. Or 0xff if Value is invalid.

**/
UINT8
CheckAndConvertBcd8ToDecimal8 (
  IN  UINT8  Value
  )
{
  if ((Value < 0xa0) && ((Value & 0xf) < 0xa)) {
    return BcdToDecimal8 (Value);
  }

  return 0xff;
}

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
  )
{
  BOOLEAN IsPM;
  UINT8   Century;

  if ((Time->Hour & 0x80) != 0) {
    IsPM = TRUE;
  } else {
    IsPM = FALSE;
  }

  Time->Hour = (UINT8) (Time->Hour & 0x7f);

  if (RegisterB.Bits.Dm == 0) {
    Time->Year    = CheckAndConvertBcd8ToDecimal8 ((UINT8) Time->Year);
    Time->Month   = CheckAndConvertBcd8ToDecimal8 (Time->Month);
    Time->Day     = CheckAndConvertBcd8ToDecimal8 (Time->Day);
    Time->Hour    = CheckAndConvertBcd8ToDecimal8 (Time->Hour);
    Time->Minute  = CheckAndConvertBcd8ToDecimal8 (Time->Minute);
    Time->Second  = CheckAndConvertBcd8ToDecimal8 (Time->Second);
  }

  if (Time->Year == 0xff || Time->Month == 0xff || Time->Day == 0xff ||
      Time->Hour == 0xff || Time->Minute == 0xff || Time->Second == 0xff) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // For minimal/maximum year range [1970, 2069],
  //   Century is 19 if RTC year >= 70,
  //   Century is 20 otherwise.
  //
  Century = (UINT8) (MINIMAL_VALID_YEAR / 100);
  if (Time->Year < (MAXIMAL_VALID_YEAR % 100)) {
    Century++;
  }
  Time->Year = (UINT16) (Century * 100 + Time->Year);

  //
  // If time is in 12 hour format, convert it to 24 hour format
  //
  if (RegisterB.Bits.Mil == 0) {
    if (IsPM && Time->Hour < 12) {
      Time->Hour = (UINT8) (Time->Hour + 12);
    }

    if (!IsPM && Time->Hour == 12) {
      Time->Hour = 0;
    }
  }

  Time->Nanosecond  = 0;

  return EFI_SUCCESS;
}

/**
  Wait for a period for the RTC to be ready.

  @param    Timeout  Tell how long it should take to wait.

  @retval   EFI_DEVICE_ERROR   RTC device error.
  @retval   EFI_SUCCESS        RTC is updated and ready.
**/
EFI_STATUS
RtcWaitToUpdate (
  UINTN Timeout
  )
{
  RTC_REGISTER_A  RegisterA;
  RTC_REGISTER_D  RegisterD;

  //
  // See if the RTC is functioning correctly
  //
  RegisterD.Data = RtcRead (RTC_ADDRESS_REGISTER_D);

  if (RegisterD.Bits.Vrt == 0) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Wait for up to 0.1 seconds for the RTC to be ready.
  //
  Timeout         = (Timeout / 10) + 1;
  RegisterA.Data  = RtcRead (RTC_ADDRESS_REGISTER_A);
  while (RegisterA.Bits.Uip == 1 && Timeout > 0) {
    MicroSecondDelay (10);
    RegisterA.Data = RtcRead (RTC_ADDRESS_REGISTER_A);
    Timeout--;
  }

  RegisterD.Data = RtcRead (RTC_ADDRESS_REGISTER_D);
  if (Timeout == 0 || RegisterD.Bits.Vrt == 0) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  See if all fields of a variable of EFI_TIME type is correct.

  @param   Time   The time to be checked.

  @retval  EFI_INVALID_PARAMETER  Some fields of Time are not correct.
  @retval  EFI_SUCCESS            Time is a valid EFI_TIME variable.

**/
EFI_STATUS
RtcTimeFieldsValid (
  IN EFI_TIME *Time
  )
{
  if (Time->Year < MINIMAL_VALID_YEAR ||
      Time->Year > MAXIMAL_VALID_YEAR ||
      Time->Month < 1 ||
      Time->Month > 12 ||
      (!DayValid (Time)) ||
      Time->Hour > 23 ||
      Time->Minute > 59 ||
      Time->Second > 59 ||
      Time->Nanosecond > 999999999 ||
      (!(Time->TimeZone == EFI_UNSPECIFIED_TIMEZONE || (Time->TimeZone >= -1440 && Time->TimeZone <= 1440))) ||
      ((Time->Daylight & (~(EFI_TIME_ADJUST_DAYLIGHT | EFI_TIME_IN_DAYLIGHT))) != 0)) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/**
  See if field Day of an EFI_TIME is correct.

  @param    Time   Its Day field is to be checked.

  @retval   TRUE   Day field of Time is correct.
  @retval   FALSE  Day field of Time is NOT correct.
**/
BOOLEAN
DayValid (
  IN  EFI_TIME  *Time
  )
{
  //
  // The validity of Time->Month field should be checked before
  //
  ASSERT (Time->Month >=1);
  ASSERT (Time->Month <=12);
  if (Time->Day < 1 ||
      Time->Day > mDayOfMonth[Time->Month - 1] ||
      (Time->Month == 2 && (!IsLeapYear (Time) && Time->Day > 28))
      ) {
    return FALSE;
  }

  return TRUE;
}

/**
  Check if it is a leap year.

  @param    Time   The time to be checked.

  @retval   TRUE   It is a leap year.
  @retval   FALSE  It is NOT a leap year.
**/
BOOLEAN
IsLeapYear (
  IN EFI_TIME   *Time
  )
{
  if (Time->Year % 4 == 0) {
    if (Time->Year % 100 == 0) {
      if (Time->Year % 400 == 0) {
        return TRUE;
      } else {
        return FALSE;
      }
    } else {
      return TRUE;
    }
  } else {
    return FALSE;
  }
}

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
  )
{
  BOOLEAN IsPM;

  IsPM = TRUE;
  //
  // Adjust hour field if RTC is in 12 hour mode
  //
  if (RegisterB.Bits.Mil == 0) {
    if (Time->Hour < 12) {
      IsPM = FALSE;
    }

    if (Time->Hour >= 13) {
      Time->Hour = (UINT8) (Time->Hour - 12);
    } else if (Time->Hour == 0) {
      Time->Hour = 12;
    }
  }
  //
  // Set the Time/Date values.
  //
  Time->Year  = (UINT16) (Time->Year % 100);

  if (RegisterB.Bits.Dm == 0) {
    Time->Year    = DecimalToBcd8 ((UINT8) Time->Year);
    Time->Month   = DecimalToBcd8 (Time->Month);
    Time->Day     = DecimalToBcd8 (Time->Day);
    Time->Hour    = DecimalToBcd8 (Time->Hour);
    Time->Minute  = DecimalToBcd8 (Time->Minute);
    Time->Second  = DecimalToBcd8 (Time->Second);
  }
  //
  // If we are in 12 hour mode and PM is set, then set bit 7 of the Hour field.
  //
  if (RegisterB.Bits.Mil == 0 && IsPM) {
    Time->Hour = (UINT8) (Time->Hour | 0x80);
  }
}

/**
  Compare the Hour, Minute and Second of the From time and the To time.

  Only compare H/M/S in EFI_TIME and ignore other fields here.

  @param From   the first time
  @param To     the second time

  @return  >0   The H/M/S of the From time is later than those of To time
  @return  ==0  The H/M/S of the From time is same as those of To time
  @return  <0   The H/M/S of the From time is earlier than those of To time
**/
INTN
CompareHMS (
  IN EFI_TIME   *From,
  IN EFI_TIME   *To
  )
{
  if ((From->Hour > To->Hour) ||
     ((From->Hour == To->Hour) && (From->Minute > To->Minute)) ||
     ((From->Hour == To->Hour) && (From->Minute == To->Minute) && (From->Second > To->Second))) {
    return 1;
  } else if ((From->Hour == To->Hour) && (From->Minute == To->Minute) && (From->Second == To->Second)) {
    return 0;
  } else {
    return -1;
  }
}

/**
  To check if second date is later than first date within 24 hours.

  @param  From   the first date
  @param  To     the second date

  @retval TRUE   From is previous to To within 24 hours.
  @retval FALSE  From is later, or it is previous to To more than 24 hours.
**/
BOOLEAN
IsWithinOneDay (
  IN EFI_TIME  *From,
  IN EFI_TIME  *To
  )
{
  BOOLEAN Adjacent;

  Adjacent = FALSE;

  //
  // The validity of From->Month field should be checked before
  //
  ASSERT (From->Month >=1);
  ASSERT (From->Month <=12);

  if (From->Year == To->Year) {
    if (From->Month == To->Month) {
      if ((From->Day + 1) == To->Day) {
        if ((CompareHMS(From, To) >= 0)) {
          Adjacent = TRUE;
        }
      } else if (From->Day == To->Day) {
        if ((CompareHMS(From, To) <= 0)) {
          Adjacent = TRUE;
        }
      }
    } else if (((From->Month + 1) == To->Month) && (To->Day == 1)) {
      if ((From->Month == 2) && !IsLeapYear(From)) {
        if (From->Day == 28) {
          if ((CompareHMS(From, To) >= 0)) {
            Adjacent = TRUE;
          }
        }
      } else if (From->Day == mDayOfMonth[From->Month - 1]) {
        if ((CompareHMS(From, To) >= 0)) {
           Adjacent = TRUE;
        }
      }
    }
  } else if (((From->Year + 1) == To->Year) &&
             (From->Month == 12) &&
             (From->Day   == 31) &&
             (To->Month   == 1)  &&
             (To->Day     == 1)) {
    if ((CompareHMS(From, To) >= 0)) {
      Adjacent = TRUE;
    }
  }

  return Adjacent;
}
