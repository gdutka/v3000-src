/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Agesa structures and definitions
 *
 * Contains AMD AGESA core interface
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Include
 * @e \$Revision: 317069 $   @e \$Date: 2015-04-20 11:25:50 -0700 (Mon, 20 Apr 2015) $
 */


#ifndef _FCHUSBSMUMSGBLOCK_H_
#define _FCHUSBSMUMSGBLOCK_H_

#define MaxConfigRequestEntry 100;    // Config Request Entry Limit

//
// Assume standard IA-32 alignment.
// BugBug: Need to check portability of long long
//
#ifndef SMU_DATA_TYPES_H
#define SMU_DATA_TYPES_H
typedef unsigned long long  uint64_t;
typedef long long           int64_t;
typedef unsigned int        uint32_t;
typedef int                 int32_t;
typedef unsigned short      uint16_t;
typedef short               int16_t;
typedef unsigned char       uint8_t;
typedef signed char         int8_t;
#endif


#pragma pack (push, 1)

///
/// USB Enable Structure
///
typedef struct {
  uint32_t usb_hc_0             : 1;  ///< 1: Controller enabled
  uint32_t usb_hc_1             : 1;  ///< 1: Controller enabled
  uint32_t usb_hc_2             : 1;  ///< 1: Controller enabled
  uint32_t usb_hc_3             : 1;  ///< 1: Controller enabled
  uint32_t usb_hc_4             : 1;  ///< 1: Controller enabled
  uint32_t reserved0            : 11; ///< spare
  uint32_t usb4_rt_0            : 1;  ///< 1: Controller enabled; pair with usb_hc_3
  uint32_t usb4_rt_1            : 1;  ///< 1: Controller enabled; pair with usb_hc_4
  uint32_t reserved1            : 6;  ///< spare
  uint32_t usb4_phy_0           : 1;  ///< 1: Controller enabled
  uint32_t usb4_phy_1           : 1;  ///< 1: Controller enabled
  uint32_t reserved2            : 5;  ///< spare
  uint32_t usb_init_combined    : 1;  ///< 1: USB 3 and 4 initializations are combined
} BIOSSMC_MSG_Usb_Enable_t;

///
/// USB Combo Phy Static Config Structure
///
typedef struct {
  uint32_t usb_hc_0             : 8;  ///< 8: Controller 0
  uint32_t usb_hc_1             : 8;  ///< 8: Controller 1
  uint32_t usb_hc_2             : 8;  ///< 8: Controller 2, Unsupported
  uint32_t reserved0            : 8;  ///< spare
  uint32_t usb_hc_3             : 8;  ///< 8: Controller 3
  uint32_t usb_hc_4             : 8;  ///< 8: Controller 4
  uint32_t reserved1            : 16; ///< spare
} BIOSSMC_MSG_Usb_Combo_Phy_Static_Config_t;

///
/// USB Register Selection Structure
///
typedef union {
  uint32_t Value;
  struct {
    uint32_t Parameter            : 16; ///< Parameter (bits[15:0]) defines other info associated with the opcode.
    uint32_t reserved0            : 4;  ///< Reserved (bits[19:16) is for future use and should be 0.
    uint32_t hc_rt_sel            : 4;  ///< Host/RT Select (bits[23:20]) indicates the host number
    uint32_t Opcode               : 8;  ///< Opcode (bits[31:24]) defines the operations associated with the message
  } Field;
} RegSel_u;

///
/// USB Config Request Structure
///
typedef struct {
  uint32_t Addr; // Same to Msg Argument 0
  uint32_t Mask; // Same to Msg Argument 1
  uint32_t Data; // Same to Msg Argument 2
  RegSel_u Sel;  // Same to Msg Argument 3
} UsbConfigRequest_t;

///
/// BIOSSMC MSG UsbInit Data Structure
///
typedef struct {
  BIOSSMC_MSG_Usb_Enable_t                  Enable;               ///< USB Init enable
  BIOSSMC_MSG_Usb_Combo_Phy_Static_Config_t ComboPhyStaticConfig; ///< Combo Phy Static Config
  uint16_t                                  NumUsb3Entries;       ///< Usb3 entry number
  uint16_t                                  NumUsb4Entries;       ///< Usb4 entry number
  uint16_t                                  Usb3EntriesOffset;    ///< Usb3 offset, 0 means no entry
  uint16_t                                  Usb4EntriesOffset;    ///< Usb4 offset, 0 means no entry
} BIOSSMC_MSG_UsbInit_Data_t;

//Following BIOSSMC_MSG_UsbInit_Data_t, Bios will reserve space (MaxConfigRequestEntry)*(sizeof UsbConfigRequest_t)
//USB3/USB4 entries will end up an with empty UsbConfigRequest_t.

// BIOSSMC_MSG_UsbInit (ID = 0x2B)
// arg0 = Dram address low for BIOSSMC_MSG_UsbInit_t
// arg1 = Dram address high for BIOSSMC_MSG_UsbInit_t
//extern BIOSSMC_MSG_UsbInit_Data_t* pUsbInitData;
//extern UsbConfigRequest_t* pUsbConfigRequest;

#pragma pack (pop)

#endif // _FCHUSBSMUMSGBLOCK_H_


