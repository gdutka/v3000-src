/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file
 *
 * AMD Integrated Debug Debug_library Routines
 *
 * Contains all functions related to HDTOUT
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  IDS
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 */
#ifndef _AMD_IDS_DEBUGPRINT_LIB_H_
#define _AMD_IDS_DEBUGPRINT_LIB_H_
#define AMD_IDS_DP_FLAG_ALL       0xFFFFFFFFFFFFFFFFull

VOID
AmdIdsDebugPrintf (
  IN      UINT64      FilterFlag,
  IN      CONST CHAR8 *Format,
  IN      VA_LIST     Marker
  );

VOID
LightSmnRegRead (
  UINT32    Address,
  UINT8     OpFlag,
  VOID      *ValuePtr
  );

VOID
LightSmnRegWrite (
  UINT32    Address,
  UINT8     OpFlag,
  VOID      *ValuePtr
  );

/**
 *      Detect the system is emulation or real platform.
 *
 *
 *  @retval       TRUE    The system is emulation
 *  @retval       FALSE   The system is real platform
 *
 **/
BOOLEAN
AmdIdsEmulationAutoDetect (
  VOID
  );

/**
 *      Power up SOC UART and setup UART signal IOMUX function.
 *
 **/
VOID
AmdIdsInitUartIomux (
  VOID
  );

/**
 *      Check SOC UART Line Control and Divisor setting.
 *
 *
 *  @retval       TRUE    Line Control and Divisor setting is correct.
 *  @retval       FALSE   Line Control and Divisor setting is not correct.
 *
 **/
BOOLEAN
AmdIdsCheckUartDivisor (
  VOID
  );

/**
 *      Initialize SOC UART Line Control and Divisor setting.
 *
 **/
VOID
AmdIdsInitUartDivisor (
  VOID
  );

/**
 *      Check Serial Port MSR register CTS bit. (Hardware Flow Control)
 *
 *
 *  @retval       TRUE    CTS bit is high.
 *  @retval       FALSE   CTS bit is low.
 *
 **/
BOOLEAN
AmdIdsDpSerialWritable (
  VOID
  );

VOID
AmdIdsDpSerialPrint (
  IN CHAR8  *Buffer,
  IN UINTN  BufferSize
  );

#endif //_AMD_IDS_DEBUGPRINT_LIB_H_

