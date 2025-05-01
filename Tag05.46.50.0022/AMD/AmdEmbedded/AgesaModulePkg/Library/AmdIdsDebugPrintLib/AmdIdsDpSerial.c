/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/

 /*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include <Pi/PiMultiPhase.h>
#include <Library/AmdBaseLib.h>
#include <Library/AmdIdsDebugPrintLib.h>
#include <AmdIdsDebugPrintLocal.h>
#include <Filecode.h>

#define FILECODE LIBRARY_AMDIDSDEBUGPRINTLIB_AMDIDSDPSERIAL_FILECODE

#define IDS_SERIAL_PORT (PcdGet64(PcdAmdIdsDebugPrintSerialPort))

#define IDS_SERIAL_PORT_LSR \
(IDS_SERIAL_PORT > 0xFFFF ? \
(IDS_SERIAL_PORT + 5*4) : \
(IDS_SERIAL_PORT + 5))

#define IDS_LSR_TRANSMIT_HOLDING_REGISTER_EMPTY_MASK BIT5 + BIT6

#define IDS_SERIAL_PORT_LSR_READ(Value) \
(IDS_SERIAL_PORT > 0xFFFF ? \
LibAmdMemRead (AccessWidth8, (UINT64) IDS_SERIAL_PORT_LSR, &Value, NULL) : \
LibAmdIoRead (AccessWidth8, (UINT16) IDS_SERIAL_PORT_LSR, &Value, NULL))

#define IDS_SERIAL_PORT_WRITE(ByteSended) \
(IDS_SERIAL_PORT > 0xFFFF ? \
LibAmdMemWrite (AccessWidth8, (UINT64) IDS_SERIAL_PORT, &ByteSended, NULL) : \
LibAmdIoWrite (AccessWidth8, (UINT16) IDS_SERIAL_PORT, &ByteSended, NULL))

#define IDS_SERIAL_PORT_MSR \
(IDS_SERIAL_PORT > 0xFFFF ? \
(IDS_SERIAL_PORT + 6*4) : \
(IDS_SERIAL_PORT + 6))

#define IDS_SERIAL_PORT_MSR_CTS      (BIT4)
#define IDS_SERIAL_PORT_MSR_DSR      (BIT5)
#define IDS_SERIAL_PORT_MSR_MASK     (IDS_SERIAL_PORT_MSR_CTS + IDS_SERIAL_PORT_MSR_DSR)

#define IDS_SERIAL_PORT_MSR_READ(Value) \
(IDS_SERIAL_PORT > 0xFFFF ? \
LibAmdMemRead(AccessWidth8, (UINT64) IDS_SERIAL_PORT_MSR, &Value, NULL) : \
LibAmdIoRead(AccessWidth8, (UINT16) IDS_SERIAL_PORT_MSR, &Value, NULL))

/**
 *  Determine whether IDS console is enabled.
 *
 *
 *  @retval       TRUE    Alway return true
 *
 **/
BOOLEAN
AmdIdsDpSerialSupport (
  VOID
  )
{
  UINT8 Value;

  if (AmdIdsEmulationAutoDetect()) {
    // Return FALSE if the system is emulation.
    return FALSE;
  }

  if (PcdGetBool (PcdAmdIdsDebugPrintSerialPortEnable)) {
    // MSR.DSR - bit5 is set if a cable is connected.
    // MSR.CTS - bit4 is set if it is ok to transmit data
    IDS_SERIAL_PORT_MSR_READ (Value);

    if (Value == 0xFF) {
      return FALSE;
    }

    if (PcdGetBool (PcdAmdIdsDebugPrintSerialPortDetectCableConnection) ) {
      // Wait for DSR to be set
      if ((Value & IDS_SERIAL_PORT_MSR_DSR) != IDS_SERIAL_PORT_MSR_DSR) {
        return FALSE;
      }
    }
    return TRUE;
  }
  return FALSE;
}

VOID
AmdIdsDpSerialConstructor (
  VOID
  )
{
  return;
}

/**
 *  Get Serial customize Filter
 *
 *  @param[in,out] Filter    Filter do be filled
 *
 *  @retval       FALSE    Alway return FALSE
 *
 **/
BOOLEAN
AmdIdsDpSerialGetFilter (
  IN OUT   UINT64 *Filter
  )
{
  return FALSE;
}

/**
 *  Send byte to  Serial Port
 *
 *  Before use this routine, please make sure Serial Communications Chip have been initialed
 *
 *  @param[in] ByteSended   Byte to be sended
 *
 *  @retval       TRUE    Byte sended successfully
 *  @retval       FALSE   Byte sended failed
 *
 **/
BOOLEAN
AmdIdsDpSerialSendByte (
  IN      CHAR8   ByteSended
  )
{
  UINT32 RetryCount;
  UINT32 RetryCountCTS;
  UINT8  Value;

  // Wait until LSR.Bit5 (Transmitter holding register Empty)
  RetryCount = 2000;
  do {
    IDS_SERIAL_PORT_LSR_READ (Value);
    RetryCount--;
  } while (((Value & (IDS_LSR_TRANSMIT_HOLDING_REGISTER_EMPTY_MASK)) != (IDS_LSR_TRANSMIT_HOLDING_REGISTER_EMPTY_MASK)) && (RetryCount > 0));

  if (RetryCount == 0) {
    // Time expired
    return FALSE;
  }

  RetryCountCTS = 2000;
  do {
    // Wait for the hardware flow control signal
    RetryCountCTS--;
  } while (!AmdIdsDpSerialWritable () && (RetryCountCTS > 0));

  if (RetryCountCTS == 0) {
    // Time expired
    return FALSE;
  }
  IDS_SERIAL_PORT_WRITE (ByteSended);
  return TRUE;
}


/**
 *  Print formated string
 *
 *  @param[in] Buffer  - Point to input buffer
 *  @param[in] BufferSize  - Buffer size
 *
**/
VOID
AmdIdsDpSerialPrint (
  IN      CHAR8   *Buffer,
  IN      UINTN BufferSize
  )
{
  while (BufferSize--) {
    (void)AmdIdsDpSerialSendByte (*Buffer);
    Buffer ++;
  }
}


CONST AMD_IDS_DEBUG_PRINT_INSTANCE   AmdIdsDebugPrintSerialInstance =
{
  AmdIdsDpSerialSupport,
  AmdIdsDpSerialConstructor,
  AmdIdsDpSerialGetFilter,
  AmdIdsDpSerialPrint
};





