/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

/**
 * @file AmdMpioSmiDef.h
 *
 * @brief  MPIO software SMI encoding definitions.
 *
 * @details Provide MPIO SMI encoding definition and structure.
 *
 */

#ifndef _AMD_MPIO_SWSMI_DEF_H_
#define _AMD_MPIO_SWSMI_DEF_H_


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

#define MPIO_SWSMI_HOTPLUG              (0x0001)

typedef union {
  struct {
    UINT8     InterruptVector:4;   ///< CXL VDM Interrupt Vector
    UINT8     :4;                  ///< Reserved
  } Field;
  UINT8 Value;
} CXL_VDM_PAYLOAD;


typedef union {
  struct {
    UINT32     HotplugEvent:1;      ///< Hotplug event from MPIO
    UINT32     CxlEvent:1;          ///< CXL VDM event from MPIO
    UINT32     :22;                 ///< Reserved
    UINT32     Payload:8;           ///< Payload
  } Field;
  UINT32 Value;
} MPIO_SW_SMI_CNTL;

#endif //_AMD_RAS_SMMRS_PROTOCOL_H_

