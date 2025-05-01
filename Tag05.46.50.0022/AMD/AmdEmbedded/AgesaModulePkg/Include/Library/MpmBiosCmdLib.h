/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef  _MPM_BIOS_CMD_LIB_H_
#define _MPM_BIOS_CMD_LIB_H_

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

//Bit fields of Status [15:0] in "MPM_BIOS_MBOX"
//Set by BIOS, consumed by MPM
#define MPM_BIOS_NO_ERROR              0x00000000
#define MPM_BIOS_NOT_SUPPORTED         0x00000001
#define MPM_BIOS_GENERIC_ERROR         0x00000002

//Definition of Command [23:16] in "MPM_BIOS_MBOX"
//Set by MPM, consumed by BIOS
#define MPM_BIOS_CMD_COLD_REBOOT       0x20 //BIOS to perform Cold reboot in KVM/TCR mode
#define MPM_BIOS_CMD_WARM_REBOOT       0x21 //BIOS to perform Warm reboot in KVM/TCR mode
#define MPM_BIOS_CMD_SHUT_DOWN         0x22 //BIOS to perform Shutdown in KVM/TCR mode

#define MPM_BIOS_CMD_READY             0x80000000 //Bit31 = 1: BIOS is ready to read command from MPM
                                                  //BIT31 = 0: BIOS is processing the command

#pragma pack (push, 1)
///
/// MPM to X86 command structure
///
typedef volatile struct {
  UINT32                    Status:16;              ///< Set by BIOS to indicate the execution status of last command
  UINT32                    CommandId:8;            ///< Command ID set by MPM
  UINT32                    Reserved:7;             ///< Reserved
  UINT32                    Ready:1;                ///< Set by the BIOS to indicate the mailbox interface state.
} MPM_BIOS_MBOX_REG;

typedef union {
  IN  UINT32                  Value;               ///< Cmd register RAW value
  IN  MPM_BIOS_MBOX_REG       Field;               ///< Extended Cmd register with field definition
} MPM_BIOS_MBOX;

#pragma pack (pop)

/**
  MPM to BIOS command
  * @param[in,out]

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
RegisterMpmBiosCmdTimerEvent (VOID);

#endif //_MPM_BIOS_CMD_LIB_H_
