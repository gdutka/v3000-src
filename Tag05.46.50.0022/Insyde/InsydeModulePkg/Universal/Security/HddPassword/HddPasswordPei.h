/** @file
  HddPassword PEI module which is used to unlock HDD password for S3 and recovery

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#ifndef _HDD_PASSWORD_PEI_H_
#define _HDD_PASSWORD_PEI_H_

#include <PiPei.h>
#include <IndustryStandard/Atapi.h>
#include <Protocol/DevicePath.h>
#include <Protocol/HddPasswordService.h>

#include <Guid/HddPasswordVariable.h>
#include <Guid/H2OCp.h>

#include <Ppi/AtaPassThru.h>
#include <Ppi/StorageSecurityCommand.h>
#include <Ppi/NvmExpressPassThru.h>
#include <Ppi/NvmExpressHostController.h>
#include <Ppi/AtaAhciController.h>
#include <Ppi/Stall.h>

#include <IndustryStandard/Atapi.h>
#include <Library/TcgStorageOpalLib.h>
#include <Library/Tcg2PhysicalPresenceLib.h>
#include <Library/VariableLib.h>
#include <Library/PcdLib.h>
#include <Library/H2OLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/H2OHddPasswordTableLib.h>
#include <Library/H2OLib.h>

#include <Library/H2OHddPasswordTableLib.h>
#include <Library/H2OCpLib.h>
#include <Library/PostCodeLib.h>



#define ATA_SECURITY_SET_PASSWORD_CMD   0xF1
#define ATA_SECURITY_UNLOCK_CMD         0xF2
#define ATA_SECURITY_FREEZE_LOCK_CMD    0xF5
#define ATA_SECURITY_DIS_PASSWORD_CMD   0xF6
//
// Time out value for ATA PassThru PPI
//
#define ATA_TIMEOUT                          30000000
#define SSC_PPI_GENERIC_TIMEOUT                  30000000
#define BUFFER_SIZE                     512
#define TCG2_BIOS_STORAGE_MANAGEMENT_FLAG_ENABLE_BLOCK_SID                   BIT18
#define ATA_IDENTIFY_CMD                      0xEC
#define ATA_SECURITY_SET_PASSWORD      1
#define ATA_SECURITY_UNLOCK            2
#define ATA_SECURITY_ERASE_PREPARE     3
#define ATA_SECURITY_ERASE             4
#define ATA_SECURITY_FREEZE_LOCK       5
#define ATA_SECURITY_DISABLE_PASSWORD  6
#define ATA_COMMAND_RETRY_TIMES        10
#define OPAL_MAX_PASSWORD_SIZE      32

#define TRUSTED_COMMAND_TIMEOUT_NS      ((UINT64) 5 * ((UINT64)(1000000)) * 10) // 5 seconds
#define BUFFER_SIZE                      512
#define ATTR_NAME_PCIE_PEI_BAR5          "pcie_pei_bar5"

#pragma pack(1)
#define OPAL_PEI_DEVICE_SIGNATURE       SIGNATURE_32 ('o', 'p', 'd', 's')

typedef struct {
  UINT16      SecurityCmdType;
  UINT8       HddPassword[32];
  UINT16      MasterPasswordIdentifier;
  UINT16      Reserved[238];
} STORAGE_SECURITY_COMMAND_PACKET;


typedef struct {
  UINTN                                    Signature;
  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL    Sscp;
  EDKII_PEI_STORAGE_SECURITY_CMD_PPI       *SscPpi;
  UINTN                                    DeviceIndex;
  UINT8                                    PasswordLength;
  CHAR8                                    Password[OPAL_MAX_PASSWORD_SIZE];
  UINT16                                   OpalBaseComId;          // Opal SSC 1 base com id.  
} OPAL_PEI_DEVICE;

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

#define OPAL_PEI_DEVICE_FROM_THIS(a)    \
  CR (a, OPAL_PEI_DEVICE, Sscp, OPAL_PEI_DEVICE_SIGNATURE)
#pragma pack()
#endif
