/** @file
  Utility functions for computing storage device capacity.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Base.h>

#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/BlockIoCapacityLib.h>

/**
  Converts a 128-bit integer to quintillions (10^18) and undecillions (10^36).

  @param[in]  LowerQword   Lower 64 bits of the value to convert.
  @param[in]  UpperQword   Upper 64 bits of the value to convert.
  @param[out] Remainder    Remainder of the 128-bit value divided by a
                           quintillion (10^18).
  @param[out] Quintillion  Number of quintillions (10^18) in the value,
                           modulo 10^36 if the value is greater than or equals
                           one undecillion (10^36).
  @param[out] Undecillion  Number of undecillions (10^36) in the value.

  @return  Void.

**/
VOID
Uint128ToQuintillion (
  IN UINT64   LowerQword,
  IN UINT64   UpperQword,
  OUT UINT64  *Remainder,
  OUT UINT64  *Quintillion,
  OUT UINT16  *Undecillion
  )
{
  //
  // Upper and lower bits of 10^36
  // 10^36 = 54210108624275221 * 2^64 + 12919594847110692864
  //       = 54210108624275221 * 2^64 + (188004849 * 2^36)
  //
  CONST UINT64  UndecillionUpper = 54210108624275221ULL;
  CONST UINT64  UndecillionLower = 12919594847110692864ULL;
  //
  // 31 significant bits of (1 / 10^18)
  // 1 / 10^18 = (1 / 5^18) / 2^18
  //           = (1237940039 + (1088639354321 / 5^18)) / 2^72 / 2^18
  //
  CONST UINT32  QuintillionReciprocal = 1237940039UL;
  //
  // 10^18
  // = 232830643 * 2^32 + 2808348672
  // = 232830643 * 2^32 + (10713 * 2^18)
  //
  CONST UINT64  QuintillionValue = 1000000000000000000ULL;
  CONST UINT64  QuintillionUpper = (QuintillionValue >> 32);
  CONST UINT64  QuintillionLower = (QuintillionValue & MAX_UINT32);

  UINT64  Quotient;
  UINT64  Carry;
  UINT64  Subtrahend;
  UINT8   Shift;
  UINT64  QuotientLower;

  if ((Remainder == NULL) || (Quintillion == NULL) || (Undecillion == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The Remainder/Quintillion/Undecillion is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  //
  // Try an approximate division.
  //
  // UpperQword * 2^64 / 10^36
  // = UpperQword / (10^36 / 2^64)
  // = UpperQword / (54210108624275221 + (12919594847110692864 / 2^64))
  // > UpperQword / (54210108624275221 + 1)
  //
  *Undecillion = 0;
  Quotient = (UpperQword / (UndecillionUpper + 1U));

  while (TRUE) {
    //
    // Compute remainder. If the remainder is not less than the divisor,
    // increment quotient.
    //
    Carry = (Quotient * (UndecillionLower >> 36));
    Subtrahend = (Quotient * UndecillionUpper) + (Carry >> (64 - 36)) +
                 ((LowerQword < (Carry << 36)) ? 1U : 0U);
    ASSERT (UpperQword >= Subtrahend);
    UpperQword -= Subtrahend;
    LowerQword -= (UINT64) (Carry << 36);
    *Undecillion += (UINT16) Quotient;

    if (UpperQword < UndecillionUpper) {
      break;
    }
    if (UpperQword == UndecillionUpper &&
        LowerQword < UndecillionLower)
    {
      break;
    }
    Quotient = 1;
  }

  //
  // Approximate quotient by multiplying a value close to (1 / 10^18).
  // Without 128-bit multiplication, we need to shift bits carefully to prevent
  // overflow. We can have at most 31 significant bits for (1 / 10^18) without
  // overflowing a UINT64 data type.
  // (1 / 10^18) is approximately (1237940039 / 2^90).
  // The multiplicand greater than 14901161197 (34 bits) will overflow.
  //
  // The 8 most significant bits of UpperQword are always zero.
  //
  // UpperQword * 2^64 / 10^18
  // = UpperQword * 2^46 * (1 / 5^18)
  // = UpperQword * 2^46 * ((1237940039 + (1088639354321 / 5^18)) / 2^72)
  // = UpperQword * (1237940039 + (1088639354321 / 5^18)) / 2^26
  // > UpperQword * 1237940039 / 2^26
  //
  *Quintillion = 0;
  Shift = 64 - 8 - 34;
  Quotient = (((UpperQword >> Shift) * QuintillionReciprocal) >> (26 - Shift));
  while (TRUE) {
    //
    // Compute remainder.
    //
    ASSERT (UpperQword >= ((Quotient >> 32) * QuintillionUpper));
    UpperQword         -= ((Quotient >> 32) * QuintillionUpper);

    ASSERT (UpperQword >= (((Quotient >> 32) * QuintillionLower) >> 32));
    UpperQword         -= (((Quotient >> 32) * QuintillionLower) >> 32);
    Carry               = (((Quotient >> 32) * QuintillionLower) & MAX_UINT32);

    QuotientLower = (Quotient & MAX_UINT32);
    ASSERT (UpperQword >= ((QuotientLower * QuintillionUpper) >> 32));
    UpperQword         -= ((QuotientLower * QuintillionUpper) >> 32);
    Carry              += ((QuotientLower * QuintillionUpper) & MAX_UINT32);

    Carry += ((QuotientLower * QuintillionLower) >> 32);
    ASSERT (UpperQword >= (Carry >> 32));
    UpperQword         -= (Carry >> 32);

    Subtrahend = (Carry << 32) +
                 ((QuotientLower * QuintillionLower) & MAX_UINT32);
    if (LowerQword < Subtrahend) {
      ASSERT (UpperQword >= 1);
      UpperQword -= 1;
    }
    LowerQword -= Subtrahend;
    *Quintillion += Quotient;

    if (UpperQword == 0 && LowerQword < QuintillionValue) {
      break;
    }
    //
    // Guess additional quotient to add to (*Quintillion).
    // Experiment shows that UpperQword will not be greater than 0xFEB046 from
    // now on.
    //
    ASSERT (UpperQword <= 0xFFFFFF);
    Shift = 33 - 24;
    Quotient = ((((UpperQword << Shift) + (LowerQword >> (64 - Shift))) *
               QuintillionReciprocal) >> (26 + Shift));
    if (Quotient < 1) {
      Quotient = 1;
    }
  }
  *Remainder = LowerQword;
}

/**
  Computes storage capacity from a given BlockSize and LastBlock.

  Computes capacity as (BlockSize * (LastBlock + 1)) and outputs the total
  number of bytes in a 128-bit integer and an approximate representation
  (useful for output with unit prefixes).

  @param[in]  BlockSize               The number of bytes per logical block (or
                                      sector) of a block device.
  @param[in]  LastBlock               The last LBA of a block device. This
                                      equals to the total number of logical
                                      blocks (or sectors) minus one.
  @param[in]  Flags                   The formula option flags.
  @param[out] CapacityRepresentation  On output, a structure containing
                                      approximate representation of the
                                      device's capacity.
  @param[out] CapacityTotalBytes      On output, the device's capacity in total
                                      number of bytes.

  @return  Void.

**/
VOID
ComputeBlockIoCapacity (
  IN UINT32                        BlockSize,
  IN UINT64                        LastBlock,
  IN UINT32                        Flags,
  OUT H2O_CAPACITY_REPRESENTATION  *CapacityRepresentation OPTIONAL,
  OUT H2O_CAPACITY_UINT128         *CapacityTotalBytes OPTIONAL
  )
{
  UINT32  LastBlockLower;
  UINT32  LastBlockUpper;
  UINT64  Carry;
  UINT64  CapacityLowerBits;
  UINT64  CapacityUpperBits;
  UINT8   RoundUp;

  UINT64  Remainder;
  UINT64  Quintillion;
  UINT16  Undecillion;

  UINT8   ShiftBits;

  LastBlockLower = (UINT32) (LastBlock & MAX_UINT32);
  LastBlockUpper = (UINT32) (LastBlock >> 32);
  //
  // BlockSize * (LastBlock + 1)
  // = (BlockSize * (LastBlockUpper * 2^32 + LastBlockLower + 1)
  // = (BlockSize * LastBlockUpper * 2^32) + BlockSize * (LastBlockLower + 1)
  //
  Carry = ((BlockSize * ((UINT64) LastBlockLower + 1)) >> 32) +
            (BlockSize * (UINT64) LastBlockUpper);
  CapacityLowerBits = (UINT64) (BlockSize * (LastBlock + 1));
  CapacityUpperBits = (Carry >> 32);

  if (CapacityTotalBytes != NULL) {
    CapacityTotalBytes->LowerBits = CapacityLowerBits;
    CapacityTotalBytes->UpperBits = CapacityUpperBits;
  }

  if (CapacityRepresentation == NULL) {
    return;
  }
  CapacityRepresentation->Exponent            = 1;
  CapacityRepresentation->Hundredths          = 0;
  CapacityRepresentation->Significand         = 0;
  CapacityRepresentation->ExponentBase1024    = 1;
  CapacityRepresentation->HundredthsBase1024  = 0;
  CapacityRepresentation->SignificandBase1024 = 0;

  if (BlockSize == 0) {
    return;
  }
  ASSERT (CapacityLowerBits > 0 || CapacityUpperBits > 0);

  //
  // Multiply 100
  //
  ASSERT (CapacityUpperBits <= MAX_UINT32); // Can't overflow
  Carry = (((CapacityLowerBits & MAX_UINT32) * 100) >> 32) +
            ((CapacityLowerBits >> 32) * 100);
  CapacityLowerBits = (UINT64) (CapacityLowerBits * 100);
  CapacityUpperBits = (CapacityUpperBits * 100) + (Carry >> 32);

  RoundUp = ((Flags & FORMULA_ROUNDUP) == FORMULA_ROUNDUP) ? 1 : 0;

  //
  // Substract one for later rounding-up calculation.
  //
  if (RoundUp && CapacityLowerBits == 0) {
    CapacityUpperBits--;
  }
  CapacityLowerBits -= RoundUp;

  Uint128ToQuintillion (
    CapacityLowerBits,
    CapacityUpperBits,
    &Remainder,
    &Quintillion,
    &Undecillion
    );
  ASSERT (Undecillion == 0);
  Remainder /= 1000;
  if (Quintillion > 0) {
    //
    // Here the "1 quintillion" unit actually represents
    // (0.01 * (10^18) = 10 * (10^15)) bytes = 10 petabytes.
    // Begin with the "peta" unit prefix.
    //
    CapacityRepresentation->Exponent = 5;
    Remainder = (Remainder / 1000000000000ULL) + (Quintillion * 1000);
  }
  while (Remainder >= 99999) {
    CapacityRepresentation->Exponent++;
    Remainder /= 1000;
  }
  CapacityRepresentation->Hundredths  = (UINT8)  ((Remainder + RoundUp) % 100);
  CapacityRepresentation->Significand = (UINT16) ((Remainder + RoundUp) / 100);

  if (CapacityRepresentation->Exponent > 2) {
    CapacityRepresentation->ExponentBase1024 =
                              (CapacityRepresentation->Exponent - 1);
  }
  ShiftBits = (CapacityRepresentation->ExponentBase1024 * 10);
  if (CapacityRepresentation->ExponentBase1024 >= 7) {
    Remainder = (CapacityUpperBits >> (ShiftBits - 64));
  } else {
    Remainder = (CapacityLowerBits >> ShiftBits) +
                (CapacityUpperBits << (64 - ShiftBits));
  }
  if (Remainder >= 99999) {
    CapacityRepresentation->ExponentBase1024++;
    Remainder >>= 10;
  }
  CapacityRepresentation->HundredthsBase1024  =
                            (UINT8)  ((Remainder + RoundUp) % 100);
  CapacityRepresentation->SignificandBase1024 =
                            (UINT16) ((Remainder + RoundUp) / 100);
}

/**
  Converts a 128-bit integer to a Null-terminated ASCII string.

  If Buffer is NULL, then ASSERT().
  If the size of Buffer is less than UINT128_STR_MAX_SIZE bytes, then the
  results are undefined.

  @param[in]  LowerQword  Lower 64 bits of the value to convert.
  @param[in]  UpperQword  Upper 64 bits of the value to convert.
  @param[out] Buffer      The pointer to the output buffer for the produced
                          Null-terminated ASCII string. The size of Buffer must
                          be at least UINT128_STR_MAX_SIZE bytes.

  @return  The number of ASCII characters in the produced output buffer not
           including the Null-terminator.

**/
UINTN
Uint128ToDecimalString (
  IN UINT64  LowerQword,
  IN UINT64  UpperQword,
  OUT CHAR8  Buffer[UINT128_STR_MAX_SIZE]
  )
{
  UINT16  Undecillion;
  UINT64  Quintillion;
  UINT64  Remainder;
  UINTN   Length;

  ASSERT (Buffer != NULL);

  Uint128ToQuintillion (
    LowerQword,
    UpperQword,
    &Remainder,
    &Quintillion,
    &Undecillion
    );

  Length = 0;
  if (Undecillion > 0) {
    Length += AsciiSPrint (
                (Buffer + Length),
                (UINT128_STR_MAX_SIZE - Length),
                "%u",
                (unsigned int) Undecillion
                );
  }
  if (Undecillion > 0 || Quintillion > 0) {
    Length += AsciiSPrint (
                (Buffer + Length),
                (UINT128_STR_MAX_SIZE - Length),
                "%0*lu",
                ((Undecillion > 0) ? 18 : 1),
                Quintillion
                );
  }
  Length += AsciiSPrint (
              (Buffer + Length),
              (UINT128_STR_MAX_SIZE - Length),
              "%0*lu",
              ((Undecillion > 0 || Quintillion > 0) ? 18 : 1),
              Remainder
              );
  return Length;
}

/**
  Converts a "capacity representation" structure to an ASCII string.

  @param[in]  CapacityRepresentation  Pointer to a "capacity representation"
                                      structure to print.
  @param[in]  Flags                   Option flags.
  @param[out] Buffer                  The pointer to the output buffer for the
                                      produced Null-terminated ASCII string.
                                      The size of Buffer must be at least
                                      CAPACITY_REP_STR_MAX_SIZE bytes.

  @return  The number of ASCII characters in the produced output buffer not
           including the Null-terminator.

**/
UINTN
BlockIoCapacityRepToString (
  IN CONST H2O_CAPACITY_REPRESENTATION  *CapacityRepresentation,
  IN UINTN                              Flags,
  OUT CHAR8                             Buffer[CAPACITY_REP_STR_MAX_SIZE]
  )
{
  UINT8        Exponent;
  UINT8        Hundredths;
  UINT16       Significand;
  BOOLEAN      UseBase1024;
  CONST CHAR8  *Format;
  CHAR8        UnitPrefix;
  CONST CHAR8  UnitPrefixes[] =
                 {'k', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y', 'R', 'Q'};

  if ((CapacityRepresentation == NULL) || (Buffer == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The CapacityRepresentation/Buffer is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  Exponent    = CapacityRepresentation->Exponent;
  Hundredths  = CapacityRepresentation->Hundredths;
  Significand = CapacityRepresentation->Significand;

  UseBase1024 = ((Flags & CAPACITY_REP_TO_STR_BASE_1024) != 0);
  if (UseBase1024) {
    Exponent    = CapacityRepresentation->ExponentBase1024;
    Hundredths  = CapacityRepresentation->HundredthsBase1024;
    Significand = CapacityRepresentation->SignificandBase1024;
  }

  Buffer[0] = '\0';
  if (Hundredths > 99 || Significand > 999) {
    //
    // The representation is invalid if (Hundredths > 99) and the buffer could
    // possibly overflow if (Significand > 999).
    //
    return 0;
  }

  Format = "[overflow]";
  UnitPrefix = '\0';
  if (Exponent <= ARRAY_SIZE (UnitPrefixes)) {
    if (Exponent < 1) {
      Format = "%u B";
    } else {
      Format = "%u.%02u %c%aB";
      UnitPrefix = UnitPrefixes[Exponent - 1];
    }
  }
  if (UseBase1024) {
    UnitPrefix &= (~('a' ^ 'A')); // Convert to uppercase
  }
  return AsciiSPrint (
           Buffer,
           CAPACITY_REP_STR_MAX_SIZE,
           Format,
           (unsigned int) Significand,
           (unsigned int) Hundredths,
           UnitPrefix,
           (((Flags & (BIT1 | BIT0)) == CAPACITY_REP_TO_STR_BASE_1024) ?
             "i" : "")
           );
}
