/** @file
  Hardware IO Interface for DASH

;*******************************************************************************
;* Copyright (c) 2013 - 2016, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#ifndef _DASH_IO_CFG_DXE_H_
#define _DASH_IO_CFG_DXE_H_
#include <Protocol/DashIoCfg.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciIo.h>
#include <Protocol/SetupUtility.h>

#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/ChipsetConfigLib.h>

#include <ChipsetSetupConfig.h>

#include <FchRegs.h>

#define DASH_IO_CFG_SIGNATURE           SIGNATURE_32('D', 'A', 'I', 'O')

#define DASH_SOL_DEVICE_ID_BROADCOM                 0x160A

#define DASH_SOL_DEVICE_ID_REALTEK                  0x816A

#define ASF_IO_BASE                                 0xB20

#define ASF_PEC                                     0x08
#define ASF_REMOTE_CTRL_ADR                         0x0E
#define ASF_SLAVE_EN                                0x15
#define SUSPEND_SLAVE                               0x08

#define ASF_HOST_SMBUS_SLAVE_ADDRESS                0xA8

#define MCTP_HOST_SMBUS_SLAVE_ADD_BROARDCOM         0x88
#define MCTP_HOST_SMBUS_SLAVE_ADD_REALTEK           0xA8

#define MCTP_MC_SMBUS_ADDRESS_BOARDCOM              0x3C
#define MCTP_MC_SMBUS_ADDRESS_REALTEK               0xC8

#define MCTP_HOST_ENDPOINT_ID                       0x08
#define MCTP_MC_ENDPOINT_ID                         0x09

#define PCI_VENDOR_ID_BROADCOM                      0x14E4
#define PCI_DEVICE_ID_B5761                         0x1681
#define PCI_DEVICE_ID_B5761E                        0x1680
#define PCI_DEVICE_ID_B5762                         0x1687

#define PCI_VENDOR_ID_REALTEK                       0x10EC
#define PCI_DEVICE_ID_8168                          0x8168

#define MEMORY_RESOURCE_OFFSET          0x18

typedef struct _DASH_IO_CFG_CONTEXT {
  UINT32                              Signature;
  DASH_IO_CFG_PROTOCOL                DashIoCfg;
  UINT16                              AsfIoBase;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL     *PciRootBridgeIo;
  EFI_DEVICE_PATH_PROTOCOL            *SolDevicePath;

} DASH_IO_CFG_CONTEXT;

#define DASH_IO_CFG_CONTEXT_FROM_THIS(This)   CR(This, DASH_IO_CFG_CONTEXT, DashIoCfg, DASH_IO_CFG_SIGNATURE)
#endif
