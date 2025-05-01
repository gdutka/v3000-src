/** @file
  PEIM for autounlock Intel Remap devices for S3 resume

;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

/** @file
  Opal Password PEI driver which is used to unlock Opal Password for S3.

Copyright (c) 2016 - 2018, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _OPAL_PASSWORD_PEI_H_
#define _OPAL_PASSWORD_PEI_H_

#include <PiPei.h>
#include <IndustryStandard/Atapi.h>
#include <IndustryStandard/Pci.h>

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/TcgStorageOpalLib.h>
#include <Library/Tcg2PhysicalPresenceLib.h>
#include <Library/PchInfoLib.h>
#include <Library/VariableLib.h>
#include <Library/TimerLib.h>
#include <Library/SataLib.h>

#include <Guid/HddPasswordVariable.h>
#include <Guid/IntelRemapDevInfoVariable.h>

#include <Protocol/StorageSecurityCommand.h>

#include <Ppi/IoMmu.h>
#include <Ppi/OpalDeviceInfo.h>  
#include "OpalPasswordCommon.h"
#include "OpalAhciMode.h"
#include "OpalNvmeMode.h"

//
// Time out Value for ATA pass through protocol
//
#define ATA_TIMEOUT                      30000000

//
// The payload Length of HDD related ATA commands
//
#define HDD_PAYLOAD                      512
//
// According to ATA spec, the max Length of hdd password is 32 bytes
//
#define OPAL_PASSWORD_MAX_LENGTH         32


#define USER_PSW                        0
#define MASTER_PSW                      1


#define ATA_SECURITY_SET_PASSWORD      1
#define ATA_SECURITY_UNLOCK            2
#define ATA_SECURITY_ERASE_PREPARE     3
#define ATA_SECURITY_ERASE             4
#define ATA_SECURITY_FREEZE_LOCK       5
#define ATA_SECURITY_DISABLE_PASSWORD  6

#define TRUSTED_COMMAND_TIMEOUT_NS      ((UINT64) 5 * ((UINT64)(1000000)) * 10) // 5 seconds
#define BUFFER_SIZE                      512
#define ATTR_NAME_PCIE_PEI_BAR5          "pcie_pei_bar5"

#pragma pack(1)
typedef struct {
  UINT8                         Byte0;
  //
  // Bit0:
  // Master password Flag
  //
  //
  UINT8                         Byte1;
  UINT8                         Password[32];
  UINT8                         MasterPasswordIdentifier[2];
} ATA_DEVICE_SERVER_SEND_CMD_DATA;

/**
* Opal I/O Type utilized by the Trusted IO callback
*
* The type indicates if the I/O is a send or receive
*/
typedef enum {
    //
    // I/O is a TCG Trusted Send command
    //
    OpalSend,

    //
    // I/O is a TCG Trusted Receive command
    //
    OpalRecv
} OPAL_IO_TYPE;

#define OPAL_PEI_DEVICE_SIGNATURE SIGNATURE_32 ('o', 'p', 'd', 's')

typedef struct {
  UINTN                                     Signature;
  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL     Sscp;
  UINT8                                     DeviceType;
  OPAL_DEVICE_COMMON                        *Device;
  VOID                                      *Context;
} OPAL_PEI_DEVICE;

#define OPAL_PEI_DEVICE_FROM_THIS(a)  CR (a, OPAL_PEI_DEVICE, Sscp, OPAL_PEI_DEVICE_SIGNATURE)

#pragma pack()

/**
  Allocates pages that are suitable for an OperationBusMasterCommonBuffer or
  OperationBusMasterCommonBuffer64 mapping.

  @param Pages                  The number of pages to allocate.
  @param HostAddress            A pointer to store the base system memory address of the
                                allocated range.
  @param DeviceAddress          The resulting map address for the bus master PCI controller to use to
                                access the hosts HostAddress.
  @param Mapping                A resulting value to pass to Unmap().

  @retval EFI_SUCCESS           The requested memory pages were allocated.
  @retval EFI_UNSUPPORTED       Attributes is unsupported. The only legal attribute bits are
                                MEMORY_WRITE_COMBINE and MEMORY_CACHED.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The memory pages could not be allocated.

**/
EFI_STATUS
IoMmuAllocateBuffer (
  IN UINTN                  Pages,
  OUT VOID                  **HostAddress,
  OUT EFI_PHYSICAL_ADDRESS  *DeviceAddress,
  OUT VOID                  **Mapping
  );

/**
  Frees memory that was allocated with AllocateBuffer().

  @param Pages              The number of pages to free.
  @param HostAddress        The base system memory address of the allocated range.
  @param Mapping            The mapping value returned from Map().

**/
VOID
IoMmuFreeBuffer (
  IN UINTN                  Pages,
  IN VOID                   *HostAddress,
  IN VOID                   *Mapping
  );

#endif // _OPAL_PASSWORD_PEI_H_

