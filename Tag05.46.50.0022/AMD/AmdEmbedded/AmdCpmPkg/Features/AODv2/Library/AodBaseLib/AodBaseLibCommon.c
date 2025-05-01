/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */
#include <Uefi.h>
#include <Library/PciLib.h>
#include <Features/AODv2/Include/Library/AodBaseLib.h>

/**
  Check if current Aod Ram is valid

  @param[in]  RamPointer           Pointer to the data buffer to be written.

  @retval FALSE               Aod debug ram is invalid
  @retval TRUE                Aod debug ram is valid
**/
BOOLEAN
AodRamDebugValid (
  IN  VOID           *RamPointer
  )
{
  UINT32     *pPtr;


  if (RamPointer == NULL) {
    return FALSE;
  }
  pPtr = (UINT32 *)RamPointer;
  if (*pPtr == SIGNATURE_32 ('A', 'O', 'D', 'D')) {
    return TRUE;
  }
  if (*(pPtr + 1) == RAM_DEBUG_LENGTH) {
    return TRUE;
  }
  return FALSE;
}

/**
  Get Current Aod Ram Length

  @param[in]  RamPointer           Pointer to the data buffer to be written.

  @retval FALSE               Aod debug ram is invalid
  @retval TRUE                Aod debug ram is valid
**/
UINT32
AodRamDebugGetLength (
  IN  VOID           *RamPointer
  )
{
  UINT32   *pPtr;
  pPtr = (UINT32 *)RamPointer;

  return  *(pPtr + 1);
}

/**
  Set Current Aod Ram Length

  @param[in]  RamPointer           Pointer to the data buffer to be written.
  @param[in]  Length               Length of total aod debug ram.

**/
VOID
AodRamDebugSetLength (
  IN  VOID           *RamPointer,
  IN  UINT32         Length
  )
{
  UINT32   *pPtr;
  pPtr = (UINT32 *)RamPointer;

  *(pPtr + 1) = Length;
}

/**
  Get Current Aod Ram Debug Message pointer offset

  @param[in]  RamPointer           Pointer to the data buffer to be written.
  @retval the Aod Ram Debug Message pointer offset

**/
UINT32
AodRamDebugGetOffset (
  IN  VOID           *RamPointer
  )
{
  UINT32   *pPtr;
  pPtr = (UINT32 *)RamPointer;

  return  *(UINT32 *)((UINT8 *)pPtr + RAM_DEBUG_MSG_OFFSET_OFFSET);
}

/**
  Set Current Aod Ram Debug Message pointer offset

  @param[in]  RamPointer           Pointer to the data buffer to be written.
  @param[in]  Offset               Offset of current aod debug message pointer.

**/
VOID
AodRamDebugSetOffset (
  IN  VOID           *RamPointer,
  IN  UINT32         Offset
  )
{
  UINT32   *pPtr;
  pPtr = (UINT32 *)RamPointer;
  if (Offset > RAM_DEBUG_MSG_OFFSET_MAX) {
    *(UINT32 *)((UINT8 *)pPtr + RAM_DEBUG_MSG_OFFSET_OFFSET) = 0;
    return ;
  }
  *(UINT32 *)((UINT8 *)pPtr + RAM_DEBUG_MSG_OFFSET_OFFSET) = Offset;
  return ;
}

/**
  Write a char into Aod Ram

  @param[in]  RamPointer           Pointer to the data buffer to be written.
  @param[in]  Data                a char

**/
VOID
AodRamDebugWriteByte (
  IN  VOID           *RamPointer,
  IN  CHAR8           Data
  )
{

  UINT32  LogOffset;
  LogOffset = AodRamDebugGetOffset (RamPointer);
  *(CHAR8 *)((UINT8 *)RamPointer + LogOffset + RAM_DEBUG_MSG_START) = Data;
  AodRamDebugSetOffset (RamPointer, LogOffset + 1);

}
/**
  Write data from buffer to RAM debug.

  Writes NumberOfBytes data bytes from Buffer to the serial device.
  The number of bytes actually written to the serial device is returned.
  If the return value is less than NumberOfBytes, then the write operation failed.
  If NumberOfBytes is zero, then return 0.

  @param[in]  RamPointer           Pointer to the data buffer to be written.
  @param[in]  Buffer               Pointer to the data buffer to be written.
  @param[in]  NumberOfBytes        Number of bytes to written to the serial device.

  @retval 0                NumberOfBytes is 0 or the first byte in Buffer is a '\0'
  @retval >0               The number of bytes written to the serial device.
                           If this value is less than NumberOfBytes, then the read operation failed.
**/
UINTN
EFIAPI
AodRamDebugWrite (
  IN  VOID          *RamPointer,
  IN  UINT8         *Buffer,
  IN  UINTN         NumberOfBytes
  )
{

  UINTN   Index;

  if (!AodRamDebugValid (RamPointer)) {
    return 0;
  }
  if (NumberOfBytes == 0) return NumberOfBytes;
  for (Index = 0; Index < NumberOfBytes; Index++) {
    if (Buffer[Index] == '\0') {
      break;
    }
    AodRamDebugWriteByte (RamPointer, Buffer[Index]);
  }

  return Index;
}


/**
  Read SMN register in DWord

  @param[in] DieBusNum       Die bus number
  @param[in] SmnAddress      Register SMN address
  @param[in] Value           Pointer to register value

**/
VOID
AodSmnRead (
  IN       UINT8               DieBusNum,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  )
{
  UINT32    RegIndex;
  UINTN     PciAddress;

  RegIndex = SmnAddress;
  PciAddress = (DieBusNum << 20) + 0xB8;
  PciWrite32 (PciAddress, RegIndex);
  PciAddress = (DieBusNum << 20) + 0xBC;
  *Value = PciRead32 (PciAddress);
}

/**
  Write SMN register in DWord

  @param[in] DieBusNum       Die bus number
  @param[in] SmnAddress      Register SMN address
  @param[in] Value           Pointer to register value

**/
VOID
AodSmnWrite (
  IN       UINT8               DieBusNum,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  )
{
  UINT32    RegIndex;
  UINTN     PciAddress;

  RegIndex = SmnAddress;
  PciAddress = (DieBusNum << 20) + 0xB8;
  PciWrite32 (PciAddress, RegIndex);
  PciAddress = (DieBusNum << 20) + 0xBC;
  PciWrite32 (PciAddress, *Value);
}


/**
  This function is used to get active bit counts

  @param[in] Value   the input value

  @retval the count number of active bits
**/
UINT32
GetSetBitCount (
  IN       UINT32 Value
  )
{
  UINT32  Count;
  UINT32  Temp;

  Count = 0;
  Temp = Value;
  while (Temp != 0) {
    if ((Temp & 1) != 0) {
      Count++;
    }
    Temp >>= 1;
  }
  return Count;
}
