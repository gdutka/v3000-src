/** @file
 * 
***************************************************************************
* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
*
* You may not reproduce, distribute, publish, display, perform, modify, adapt,
* transmit, broadcast, present, recite, release, license or otherwise exploit
* any part of this publication in any form, by any means, without the prior
* written permission of Insyde Software Corporation.
*
******************************************************************************
*/

#ifndef _AMD_PSP_FLASHACCLIB_PEI_
#define _AMD_PSP_FLASHACCLIB_PEI_

#include <Uefi.h>
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/FdSupportLib.h>
#include <Library/BaseMemoryLib.h>

extern
EFI_STATUS
EFIAPI
SendSpiCmd (
  IN     UINT8                  OpCode,
  IN     UINT8                  *BufferIn,
  IN     UINT8                  SizeIn,
  IN OUT UINT8                  *BufferOut,
  IN OUT UINT8                  *SizeOut
  );

//
// Normal SPI OpCode
//
#define RDSR                            0x05        // Read Status Register.

//
// SFDP
//
#define SFDP_SIGNATURE                  0x50444653
#define RDSFDP                          0x5A

//
// RPMC Table
//
#define RPMC_OP1                        0x9B
#define RPMC_OP2                        0X96
#define RPMC_PARAMETER_ID               0x03

#define COMMAND_TYPE_RESERVED           0x04        // CmdType = 04-FFH. These OP1 commands are reserved and
                                                    // cannont be used.

//
// Define extended Status Register Error message.
//
#define RPMC_CMD_BUSY                   0x01
#define RPMC_CMD_SUCCESSFUL_COMPLETION  0x80        // Bit7.

typedef enum {
  CmdWriteRootKeyReg,
  CmdUpdateHmacKeyReg,
  CmdIncMc,
  CmdReqMc,
  CmdRsvd                        // CmdType = 04-0FFH. These OP1 commands are reserved and cannont be used.
} RPMC_CMD_TYPE;

#pragma pack(1)
typedef struct {
  UINT32  SfdpSignature;
  UINT8   SfdpMinorRevNum;
  UINT8   SfdpMajorRevNum;
  UINT8   NumOfParameterHeader;  // n: n + 1 parameter header.
  UINT8   Rsvd;
} SPI_SFDP_HEADER;

typedef struct {
  UINT8  ID;
  UINT8  ParameterTableMinorRev;
  UINT8  ParameterTableMajorRev;
  UINT8  ParameterTableLen;            // In double word
  UINT8  ParameterTablePointer[3];  
  UINT8  Reserve0;
} SPI_SFDP_PARAMETER_HEADER;

typedef union {
  struct {
    UINT8  FlashHardening:1;
    UINT8  McSize:1;
    UINT8  BusyPollingMethod:1;
    UINT8  Rsvd:1;
    UINT8  NumCounter:4;               // Number of supported counters-1.
  } Byte0;
  UINT8  Byte0Raw;
} SPI_RPMC_PARAMETER;

//
// RPMC Command: Increment Monotonic Counter.
//
typedef struct {
  UINT8   CmdType;               // Byte 1.
  UINT8   CounterAddr;           // Byte 2.
  UINT8   Rsvd;                  // Byte 3.
  UINT32  CounterData;           // Byte 4-7.
  UINT8   Signature[32];         // Byte 8-39.
} RPMC_INCREMENT_MC_CMD_INFO;

//
// RPMC Command: Request Monotonic Counter.
//
typedef struct {
  UINT8   CmdType;               // Byte 1.
  UINT8   CounterAddr;           // Byte 2.
  UINT8   Rsvd;                  // Byte 3.
  UINT8   Tag[12];               // Byte 4-15.
  UINT8   Signature[32];         // Byte 16-47.
} RPMC_REQUEST_MC_CMD_INFO;

//
// RPMC Command: Read Data.
//
typedef struct {
  UINT8   ExtendedStatus;        // Byte 2.
  UINT8   Tag[12];               // Byte 3-14.
  UINT32  CounterReadData;       // Byte 15-18
  UINT8   Signature[32];         // Byte 19-50.  
} RPMC_READ_DATA_CMD_INFO;
#pragma pack()

/**
 * Retrieve block size of Flash Device
 *
 * @param[in]       BlockSize       Size of each block
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspGetFlashBlockSize (
  OUT UINTN                                       *BlockSize
  );

/**
 * Read data from Flash device
 *
 * @param[in]       FlashAddress Physical flash address
 * @param[in, out]  NumBytes     Number in Byte; return Bytes been read on output
 * @param[in]       Buffer       Buffer contain the read data (Allocated by caller)
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspReadFlash (
  IN UINTN                                        FlashAddress,
  IN OUT UINTN                                    *NumBytes,
  IN UINT8                                        *Buffer
  );


/**
 * Write data to Flash device
 *
 * @param[in]       FlashAddress Physical flash address
 * @param[in, out]  NumBytes     Number in Byte; return Bytes been written on output
 * @param[in]       Buffer       Buffer contain the written data (Allocated by caller)
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspWriteFlash (
  IN UINTN                                        FlashAddress,
  IN OUT UINTN                                    *NumBytes,
  IN UINT8                                        *Buffer
  );


/**
 * Erase Flash region according to input in unit of block size
 *
 *
 * @param[in]       FlashAddress Physical flash address
 * @param[in, out]  NumBytes     Number in Byte; return Bytes been erased on output
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspEraseFlash (
  IN UINTN                                        FlashAddress,
  IN OUT UINTN                                    *NumBytes
  );

/**
 * Increment the Monotonic counter by 1 inside the Serial Flash Device
 *
 *
 * @param[in]       CounterAddr   CounterAddr for IncMc command
 * @param[in]       CounterData   CounterData for IncMc command
 * @param[in]       Signature     Point to the 32 bytes Signature for IncMc command
 *
 * @retval EFI_SUCCESS      Command success
 * @retval Others           Error happens during handling the command
 */
EFI_STATUS
EFIAPI
PspRpmcIncMc (
  IN UINT8   CounterAddr,
  IN UINT32  CounterData,
  IN UINT8*  Signature
  );

/**
 * Request the Monotonic counter value inside the Serial Flash Device
 *
 *
 * @param[in]       CounterAddr   CounterAddr for ReqMc command
 * @param[in]       Tag           Tag for ReqMc command
 * @param[in]       Signature     Point to the 32 bytes Signature for ReqMc command
 * @param[in, out]  CounterData   Point to the output CounterData for ReqMc command
 
 * @retval EFI_SUCCESS      Command success, CounterData is valid
 * @retval Others           Error happens during handling the command
 */
EFI_STATUS
EFIAPI
PspRpmcReqMc (
  IN   UINT8   CounterAddr,
  IN   UINT8   *Tag,
  IN   UINT8   *Signature,
  IN  OUT UINT32  *CounterData
  );


/**
  Check the ROM part whether supports the RPMC function.

  @retval TRUE                  The ROM part supported RPMC function.
  @retval FALSE                 The ROM part unsupported RPMC function.
**/
BOOLEAN
EFIAPI
RpmcSupportStatus (
  VOID
  );

/**
  Increment the Monotonic counter by 1 inside the Serial Flash Device.

  @param[in] CounterAddr        Target Monotonic counter address.
  @param[in] CounterData        Current Monotonic counter value.
  @param[in] Signature          Point to the 32 bytes Signature.

  @retval EFI_SUCCESS           Function successfully returned.
  @retval Others                Some error occurs when executing this function.
**/
EFI_STATUS
EFIAPI
RpmcIncMc (
  IN UINT8   CounterAddr,
  IN UINT32  CounterData,
  IN UINT8   *Signature
  );

/**
  Request the Monotonic counter value inside the Serial Flash Devicee.

  @param[in]      CounterAddr   Target Monotonic counter address.
  @param[in]      Tag           Point to the 12 bytes payload to be added.
  @param[in]      Signature     Point to the 32 bytes Signature.
  @param[in, out] CounterData   The latest Monotonic counter value.

  @retval EFI_SUCCESS           Function successfully returned.
  @retval Others                Some error occurs when executing this function.
**/
EFI_STATUS
EFIAPI
RpmcReqMc (
  IN     UINT8   CounterAddr,
  IN     UINT8   *Tag,
  IN     UINT8   *Signature,
  IN OUT UINT32  *CounterData
  );
  
/**
  Confirm previous OP1 command executed status.
  If previous OP1 command is Request Monotonic Counter and if Serial Flash returns successful
  completion extended status then it must also return valid value in the Tag, Counter Data and
  Signature field.

  @param[out] ReadData          Return the current counter data and tag, signature if it is not NULL.

  @retval EFI_SUCCESS           Function successfully returned.
  @retval Others                Some error occurs when executing this function.
**/
EFI_STATUS
EFIAPI
CmdStatus (
  OUT RPMC_READ_DATA_CMD_INFO  *ReadData
  );

EFI_STATUS
EFIAPI
ExecuteStatus (
  OUT RPMC_READ_DATA_CMD_INFO  *ReadData
  );

#endif
