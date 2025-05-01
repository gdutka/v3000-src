/** @file

;*******************************************************************************
;* Copyright (c) 2013, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

// Define a OEM Global region of ACPI NVS Region that may be used for any
// type of implementation.  The starting offset and size will be fixed
// up by the System BIOS during POST.  Note that the Size must be a word
// in size to be fixed up correctly.

OperationRegion(OGNS, SystemMemory, 0x55AA55AA, 0x55AA55AA)
Field(OGNS, AnyAcc, Lock, Preserve)
{
  // The definitions below need to be matched OemGlobalNvsArea.h
  // and can be modified by OEM
    OG00,   8,  //   (00) Oem GlobalNvStore Data 00
    OG01,   8,  //   (01) Oem GlobalNvStore Data 01
    M2WL,   8,  //   (02) M2WLan Enable/Disable
    THPN,   8,  //   (03) TouchPanel
    PBAR,   8,  //   (04) PCIE Bar
    THPD,   8,  //   (05) TouchPad
    DTEN,   8,  //   (06) DtSlotEn
    SDMO,   8,  //   (07) SerialDbgMsgUnderOs
    TBEN,   8,  //   (08) TBSupport
    TBNH,   8,  //   (09) TBPCIeNativeHotPlug
    RV2I,   8,  //   (10) 0:RV, 1:RV2
    ISDS,   8,  //   (11) 0:Single source 1:Deual Source    
    PSEL,   8,  //   (12) Platform Selection, 0:LilacTv, 1:MayanTv,
    TPPL,  16,  //   (13) M.2 NVME Tpvperl
    TRCF,  16,  //   (15) M.2 NVME Trst-cfg
}

