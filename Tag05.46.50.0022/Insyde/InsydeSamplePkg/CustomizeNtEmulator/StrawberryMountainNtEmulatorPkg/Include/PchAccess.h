/** @file

;******************************************************************************
;* Copyright (c) 2012, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

/*++
  This file contains an 'Intel Peripheral Driver' and uniquely        
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your   
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 1999 - 2009 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PchAccess.h

Abstract:

  Macros that simplify accessing PCH devices's PCI registers.

  ** NOTE ** these macros assume the PCH device is on BUS 0

--*/
#ifndef _PCH_ACCESS_H_
#define _PCH_ACCESS_H_

#include "PchRegs.h"
#include "PchCommonDefinitions.h"

//
// Pch Controller PCI access macros
//
#define PCH_RCRB_BASE ( \
  PchMmio32 (PchPciDeviceMmBase (DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_LPC, \
  0), \
  R_PCH_LPC_RCBA) &~BIT0 \
  )

//
// Device 0x1b, Function 0
//
#define PchAzaliaPciCfg32(Register) \
  PchMmPci32 ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_AZALIA, \
  0, \
  Register \
  )

#define PchAzaliaPciCfg32Or(Register, OrData) \
  PchMmPci32Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_AZALIA, \
  0, \
  Register, \
  OrData \
  )

#define PchAzaliaPciCfg32And(Register, AndData) \
  PchMmPci32And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_AZALIA, \
  0, \
  Register, \
  AndData \
  )

#define PchAzaliaPciCfg32AndThenOr(Register, AndData, OrData) \
  PchMmPci32AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_AZALIA, \
  0, \
  Register, \
  AndData, \
  OrData \
  )

#define PchAzaliaPciCfg16(Register) \
  PchMmPci16 ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_AZALIA, \
  0, \
  Register \
  )

#define PchAzaliaPciCfg16Or(Register, OrData) \
  PchMmPci16Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_AZALIA, \
  0, \
  Register, \
  OrData \
  )

#define PchAzaliaPciCfg16And(Register, AndData) \
  PchMmPci16And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_AZALIA, \
  0, \
  Register, \
  AndData \
  )

#define PchAzaliaPciCfg16AndThenOr(Register, AndData, OrData) \
  PchMmPci16AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_AZALIA, \
  0, \
  Register, \
  AndData, \
  OrData \
  )

#define PchAzaliaPciCfg8(Register)  PchMmPci8 (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_AZALIA, 0, Register)

#define PchAzaliaPciCfg8Or(Register, OrData) \
  PchMmPci8Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_AZALIA, \
  0, \
  Register, \
  OrData \
  )

#define PchAzaliaPciCfg8And(Register, AndData) \
  PchMmPci8And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_AZALIA, \
  0, \
  Register, \
  AndData \
  )

#define PchAzaliaPciCfg8AndThenOr(Register, AndData, OrData) \
  PchMmPci8AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_AZALIA, \
  0, \
  Register, \
  AndData, \
  OrData \
  )

//
// Device 0x1e, Function 0
//
#define PchP2pPciCfg32(Register)  PchMmPci32 (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_PCI_PCI, 0, Register)

#define PchP2pPciCfg32Or(Register, OrData) \
  PchMmPci32Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_PCI_PCI, \
  0, \
  Register, \
  OrData \
  )

#define PchP2pPciCfg32And(Register, AndData) \
  PchMmPci32And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_PCI_PCI, \
  0, \
  Register, \
  AndData \
  )

#define PchP2pPciCfg32AndThenOr(Register, AndData, OrData) \
  PchMmPci32AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_PCI_PCI, \
  0, \
  Register, \
  AndData, \
  OrData \
  )

#define PchP2pPciCfg16(Register)  PchMmPci16 (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_PCI_PCI, 0, Register)

#define PchP2pPciCfg16Or(Register, OrData) \
  PchMmPci16Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_PCI_PCI, \
  0, \
  Register, \
  OrData \
  )

#define PchP2pPciCfg16And(Register, AndData) \
  PchMmPci16And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_PCI_PCI, \
  0, \
  Register, \
  AndData \
  )

#define PchP2pPciCfg16AndThenOr(Register, AndData, OrData) \
  PchMmPci16AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_PCI_PCI, \
  0, \
  Register, \
  AndData, \
  OrData \
  )

#define PchP2pPciCfg8(Register) PchMmPci8 (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_PCI_PCI, 0, Register)

#define PchP2pPciCfg8Or(Register, OrData) \
  PchMmPci8Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_PCI_PCI, \
  0, \
  Register, \
  OrData \
  )

#define PchP2pPciCfg8And(Register, AndData) \
  PchMmPci8And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_PCI_PCI, \
  0, \
  Register, \
  AndData \
  )

#define PchP2pPciCfg8AndThenOr(Register, AndData, OrData) \
  PchMmPci8AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_PCI_PCI, \
  0, \
  Register, \
  AndData, \
  OrData \
  )

//
// Device 0x1f, Function 0
//
#define PchLpcPciCfg32(Register)  PchMmPci32 (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, Register)

#define PchLpcPciCfg32Or(Register, OrData) \
  PchMmPci32Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_LPC, \
  0, \
  Register, \
  OrData \
  )

#define PchLpcPciCfg32And(Register, AndData) \
  PchMmPci32And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_LPC, \
  0, \
  Register, \
  AndData \
  )

#define PchLpcPciCfg32AndThenOr(Register, AndData, OrData) \
  PchMmPci32AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_LPC, \
  0, \
  Register, \
  AndData, \
  OrData \
  )

#define PchLpcPciCfg16(Register)  (0)

#define PchLpcPciCfg16Or(Register, OrData) \
  PchMmPci16Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_LPC, \
  0, \
  Register, \
  OrData \
  )

#define PchLpcPciCfg16And(Register, AndData) \
  PchMmPci16And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_LPC, \
  0, \
  Register, \
  AndData \
  )

#define PchLpcPciCfg16AndThenOr(Register, AndData, OrData) \
  PchMmPci16AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_LPC, \
  0, \
  Register, \
  AndData, \
  OrData \
  )

#define PchLpcPciCfg8(Register) PchMmPci8 (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, Register)

#define PchLpcPciCfg8Or(Register, OrData) \
  PchMmPci8Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_LPC, \
  0, \
  Register, \
  OrData \
  )

#define PchLpcPciCfg8And(Register, AndData) \
  PchMmPci8And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_LPC, \
  0, \
  Register, \
  AndData \
  )

#define PchLpcPciCfg8AndThenOr(Register, AndData, OrData) \
  PchMmPci8AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_LPC, \
  0, \
  Register, \
  AndData, \
  OrData \
  )

//
// SATA 1 device 0x1f, Function 2
//
#define PchSataPciCfg32(Register) PchMmPci32 (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SATA, 2, Register)

#define PchSataPciCfg32Or(Register, OrData) \
  PchMmPci32Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA, \
  2, \
  Register, \
  OrData \
  )

#define PchSataPciCfg32And(Register, AndData) \
  PchMmPci32And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA, \
  2, \
  Register, \
  AndData \
  )

#define PchSataPciCfg32AndThenOr(Register, AndData, OrData) \
  PchMmPci32AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA, \
  2, \
  Register, \
  AndData, \
  OrData \
  )

#define PchSataPciCfg16(Register) PchMmPci16 (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SATA, 2, Register)

#define PchSataPciCfg16Or(Register, OrData) \
  PchMmPci16Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA, \
  2, \
  Register, \
  OrData \
  )

#define PchSataPciCfg16And(Register, AndData) \
  PchMmPci16And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA, \
  2, \
  Register, \
  AndData \
  )

#define PchSataPciCfg16AndThenOr(Register, AndData, OrData) \
  PchMmPci16AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA, \
  2, \
  Register, \
  AndData, \
  OrData \
  )

#define PchSataPciCfg8(Register)  PchMmPci8 (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SATA, 2, Register)

#define PchSataPciCfg8Or(Register, OrData) \
  PchMmPci8Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA, \
  2, \
  Register, \
  OrData \
  )

#define PchSataPciCfg8And(Register, AndData) \
  PchMmPci8And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA, \
  2, \
  Register, \
  AndData \
  )

#define PchSataPciCfg8AndThenOr(Register, AndData, OrData) \
  PchMmPci8AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA, \
  2, \
  Register, \
  AndData, \
  OrData \
  )

//
// SATA 2 device 0x1f, Function 5
//
#define PchSata2PciCfg32(Register)  PchMmPci32 (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SATA2, 5, Register)

#define PchSata2PciCfg32Or(Register, OrData) \
  PchMmPci32Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA2, \
  5, \
  Register, \
  OrData \
  )

#define PchSata2PciCfg32And(Register, AndData) \
  PchMmPci32And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA2, \
  5, \
  Register, \
  AndData \
  )

#define PchSata2PciCfg32AndThenOr(Register, AndData, OrData) \
  PchMmPci32AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA2, \
  5, \
  Register, \
  AndData, \
  OrData \
  )

#define PchSata2PciCfg16(Register)  PchMmPci16 (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SATA2, 5, Register)

#define PchSata2PciCfg16Or(Register, OrData) \
  PchMmPci16Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA2, \
  5, \
  Register, \
  OrData \
  )

#define PchSata2PciCfg16And(Register, AndData) \
  PchMmPci16And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA2, \
  5, \
  Register, \
  AndData \
  )

#define PchSata2PciCfg16AndThenOr(Register, AndData, OrData) \
  PchMmPci16AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA2, \
  5, \
  Register, \
  AndData, \
  OrData \
  )

#define PchSata2PciCfg8(Register) PchMmPci8 (0, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SATA2, 5, Register)

#define PchSata2PciCfg8Or(Register, OrData) \
  PchMmPci8Or ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA2, \
  5, \
  Register, \
  OrData \
  )

#define PchSata2PciCfg8And(Register, AndData) \
  PchMmPci8And ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA2, \
  5, \
  Register, \
  AndData \
  )

#define PchSata2PciCfg8AndThenOr(Register, AndData, OrData) \
  PchMmPci8AndThenOr ( \
  0, \
  DEFAULT_PCI_BUS_NUMBER_PCH, \
  PCI_DEVICE_NUMBER_PCH_SATA2, \
  5, \
  Register, \
  AndData, \
  OrData \
  )

//
// Root Complex Register Block
//
#define PchMmRcrb32(Register)                           PchMmio32 (PCH_RCRB_BASE, Register)

#define PchMmRcrb32Or(Register, OrData)                 PchMmio32Or (PCH_RCRB_BASE, Register, OrData)

#define PchMmRcrb32And(Register, AndData)               PchMmio32And (PCH_RCRB_BASE, Register, AndData)

#define PchMmRcrb32AndThenOr(Register, AndData, OrData) PchMmio32AndThenOr (PCH_RCRB_BASE, Register, AndData, OrData)

#define PchMmRcrb16(Register)                           PchMmio16 (PCH_RCRB_BASE, Register)

#define PchMmRcrb16Or(Register, OrData)                 PchMmio16Or (PCH_RCRB_BASE, Register, OrData)

#define PchMmRcrb16And(Register, AndData)               PchMmio16And (PCH_RCRB_BASE, Register, AndData)

#define PchMmRcrb16AndThenOr(Register, AndData, OrData) PchMmio16AndThenOr (PCH_RCRB_BASE, Register, AndData, OrData)

#define PchMmRcrb8(Register)                            PchMmio8 (PCH_RCRB_BASE, Register)

#define PchMmRcrb8Or(Register, OrData)                  PchMmio8Or (PCH_RCRB_BASE, Register, OrData)

#define PchMmRcrb8And(Register, AndData)                PchMmio8And (PCH_RCRB_BASE, Register, AndData)

#define PchMmRcrb8AndThenOr(Register, AndData, OrData)  PchMmio8AndThenOr (PCH_RCRB_BASE, Register, AndData, OrData)

#endif
