/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _IO_H_
#define _IO_H_


/***********************************************************
* Register Name : IoCfgAddr
* Register Description :
* IO::IoCfgAddr, and IO::IoCfgData are used to access system configuration space, as defined by the PCI specification. IO::IoCfgAddr provides the address register and IO::IoCfgData provides the data port. Software sets up the configuration address by writing to IO::IoCfgAddr. Then, when an access is made to IO::IoCfgData, the processor generates the corresponding configuration access to the address specified in IO::IoCfgAddr. See 2.1.10 [Configuration Space].
IO::IoCfgAddr may only be accessed through aligned, DW IO Reads and Writes; otherwise, the accesses are passed to the appropriate IO link. Accesses to IO::IoCfgAddr and IO::IoCfgData received from an IO link are treated as all other IO transactions received from an IO link and are forwarded based on the settings in DF::X86IOBaseAddress and DF::X86IOLimitAddress. IO::IoCfgAddr and IO::IoCfgData in the processor are not accessible from an IO link.
* Visibility : 0x1
************************************************************/

// Bitfield Description : 
#define IoCfgAddr_Reserved_1_0_OFFSET      0
#define IoCfgAddr_Reserved_1_0_MASK        0x3

// Bitfield Description : See IO::IoCfgAddr[ExtRegNo].
#define IoCfgAddr_RegNo_OFFSET      2
#define IoCfgAddr_RegNo_MASK        0xfc

// Bitfield Description : Specifies the function number of the configuration cycle.
#define IoCfgAddr_Function_OFFSET      8
#define IoCfgAddr_Function_MASK        0x700

// Bitfield Description : Specifies the device number of the configuration cycle.
#define IoCfgAddr_Device_OFFSET      11
#define IoCfgAddr_Device_MASK        0xf800

// Bitfield Description : Specifies the bus number of the configuration cycle.
#define IoCfgAddr_BusNo_OFFSET      16
#define IoCfgAddr_BusNo_MASK        0xff0000

// Bitfield Description : ExtRegNo provides bits[11:8] and RegNo provides bits[7:2] of the byte address of the configuration register. ExtRegNo is Reserved unless it is enabled by DF::CoreMasterAccessCtrl[EnableCf8ExtCfg].
#define IoCfgAddr_ExtRegNo_OFFSET      24
#define IoCfgAddr_ExtRegNo_MASK        0xf000000

// Bitfield Description : 
#define IoCfgAddr_Reserved_30_28_OFFSET      28
#define IoCfgAddr_Reserved_30_28_MASK        0x70000000

// Bitfield Description : 
#define IoCfgAddr_ConfigEn_OFFSET      31
#define IoCfgAddr_ConfigEn_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_1_0:2;
    UINT32                            RegNo:6;
    UINT32                            Function:3;
    UINT32                            Device:5;
    UINT32                            BusNo:8;
    UINT32                            ExtRegNo:4;
    UINT32                            Reserved_30_28:3;
    UINT32                            ConfigEn:1;
  } Field;
  UINT32 Value;
} IoCfgAddr_STRUCT;


/***********************************************************
* Register Name : IoCfgData
* Register Description :
* 
* Visibility : 0x1
************************************************************/

// Bitfield Description : See IO::IoCfgAddr.
#define IoCfgData_Data_OFFSET      0
#define IoCfgData_Data_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Data:32;
  } Field;
  UINT32 Value;
} IoCfgData_STRUCT;

#endif /* _IO_H_ */
