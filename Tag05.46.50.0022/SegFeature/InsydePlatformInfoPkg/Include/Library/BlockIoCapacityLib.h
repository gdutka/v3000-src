/** @file
  Utility functions for computing storage device capacity.

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef BLOCK_IO_CAPACITY_LIB_H_
#define BLOCK_IO_CAPACITY_LIB_H_

#include <Base.h>

typedef struct {
  UINT8   Exponent;
  UINT8   Hundredths;
  UINT16  Significand;
  UINT8   ExponentBase1024;
  UINT8   HundredthsBase1024;
  UINT16  SignificandBase1024;
} H2O_CAPACITY_REPRESENTATION;

typedef struct {
  UINT64  LowerBits;
  UINT64  UpperBits;
} H2O_CAPACITY_UINT128;

//
// Maxinum buffer size for Uint128ToDecimalString()
//
#define UINT128_STR_MAX_SIZE  sizeof("340282366920938463463374607431768211455")

#define CAPACITY_REP_TO_STR_DEPRECATED_BIN_PREFIX  BIT1
#define CAPACITY_REP_TO_STR_BASE_1024              BIT0
#define CAPACITY_REP_TO_STR_BASE_1000              0

//
// Maxinum buffer size for BlockIoCapacityRepToString()
//
#define CAPACITY_REP_STR_MAX_SIZE  sizeof("999.99 QiB")

//
// Compute formula for ComputeBlockIoCapacity()
//
#define FORMULA_NORMAL   0
#define FORMULA_ROUNDUP  BIT0

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
  );

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
  );

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
  );

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
  );

#endif // BLOCK_IO_CAPACITY_LIB_H_
