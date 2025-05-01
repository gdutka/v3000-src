/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP Base Library
 *
 * Contains interface to the PSP library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 */
#ifndef _AMD_PSP_BASELIB_V2_H_
#define _AMD_PSP_BASELIB_V2_H_

#include <AMD.h>
#include "AmdDirectoryBaseLib.h"

#define PSP_MAILBOX_BASE            0x70    ///< Mailbox base offset on PCIe BAR
#define PSP_MAILBOX_STATUS_OFFSET   0x4     ///< Staus Offset

//
// offset between Active Image Slot address and PSP L2 Directory
//
#define PSP_L2_DIR_OFFSET 0


#pragma pack (push, 1)

///
/// X86 to PSP Buffer which start mapping from C2PMSG_28
///
typedef volatile struct {
  UINT32                    Status:16;      ///< Set by the target to indicate the execution status of last command
  UINT32                    CommandId:8;    ///< Command ID set by host
  UINT32                    Reserved:5;     ///< Reserved
  UINT32                    ResetRequired:1;  //< Set by the target to indicate that the host has to execute warm reset if corrupted detected in tOS
  UINT32                    Recovery:1;     ///< Set by the target to indicate that the host has to execute FW recovery sequence
  UINT32                    Ready:1;        ///< Set by the target to indicate the mailbox interface state.
} PSP_MBOX_V2_CMD_EXT;

typedef volatile union {
  IN  UINT32                 Value;               ///< Cmd register value
  IN  PSP_MBOX_V2_CMD_EXT    Field;               ///< Extended Cmd register with field definition
} PSP_MBOX_V2_CMD;

typedef volatile struct {
  PSP_MBOX_V2_CMD           Cmd;
  UINT64                    Buffer;        ///< 64 bit Ponter to memory with additional parameter.
} PSP_MBOX_V2;

// define PSB & HSTI status structure(MP0_C2PMSG_38)
typedef struct {
  UINT32 PsbTestStatus : 8 ;          ///< [00:07] | Set to the PSB Test result as one byte value, definition varies between program
  UINT32 PsbFusingReadiness : 1;      ///< [08:08] | 1 if the PSB fusing is allowed by a BIOS command, 0 otherwise
  UINT32 Reserved0 : 3;               ///< [09:11] | Set to zero
  UINT32 SplFuseReq : 1;              ///< [12:12] | 0 No update required; 1 update required;
  UINT32 SplFuseError : 1;            ///< [13:13] |1 SPL value of table itself does not meet requirements as set in fuse;
  UINT32 SplEntryError : 1;           ///< [14:14] | 1 Either: Entry of critical firmware not found in table
                                      ///or SPL value of present firmware does not meet requirements of the table entry
  UINT32 SplTableIsMissing: 1;        ///< [15:15] | 1 Table containing required SPL values of critical firmware entries is not present
  UINT32 Reserved1 : 12;              ///< [16:27] | Set to zero
  UINT32 HstiState : 4;               ///< [28:31] | HstiState
} PSB_HSTI_STATUS_1;

//MP0_C2PMSG_37
typedef struct {
  UINT32 PlatformVendorId : 8 ;             ///< Platform Vendor ID is a 8-bit integer value assigned by AMD for each OEM/ODM.
  UINT32 PlatformModelId : 4;               ///< Platform Model ID is 4-bit integer value assigned to a platform by the OEM/ODM
  UINT32 BiosKeyRevisionId : 4;             ///< BIOS Key Revision ID is an integer value assigned by OEM/ODM.
  UINT32 RootKeySelect : 4;                 ///< Current selected AMD Root Key ID
  UINT32 Reserved1 : 4;                     ///< Set to zero
  UINT32 PlatformSecureBootEn : 1;          ///< Fuse bit that controls Platform Secure Boot Enable & Enforced mode
  UINT32 DisableBiosKeyAntiRollback : 1;    ///< Fuse bit that controls if BIOS Key Anti-Rollback feature is enabled or not
  UINT32 DisableAmdKeyUsage : 1;            ///< Fuse bit that controls of BIOS signed by an AMD Key (with vendor ID == 0) is permitted to boot on a CPU with non-zero Vendor ID
  UINT32 DisableSecureDebugUnlock : 1;      ///< Fuse bit that controls if Secure Debug Unlock feature is disabled permanently or not
  UINT32 CustomerKeyLock : 1;               ///< Fuse bit that controls of customer region fuse bits can be programmed or not.
  UINT32 Reserved2 : 3;                     ///< Set to zero
} PSB_STATUS_2;

#pragma pack (pop)


BOOLEAN
EFIAPI
CheckPspDevicePresentV2 (
  VOID
  );

BOOLEAN
CheckPspPlatformSecureEnableV2 (
  VOID
  );

BOOLEAN
CheckPspRecoveryFlagV2 (
  VOID
  );

BOOLEAN
GetPspMmioBase (
  IN OUT   UINT32 *PspMmioBase
  );

BOOLEAN
GetPspMboxLocation (
  IN OUT   PSP_MBOX_V2 **PspMbox
  );

VOID
PspBarInitEarlyV2 (
  VOID
  );

typedef
BOOLEAN
(* FP_CONDITIONER) (
  IN       VOID        *Context
  );

BOOLEAN
EFIAPI
PspLibTimeOutV2 (
  IN       UINT64              uSec,
  IN       FP_CONDITIONER      Conditioner,
  IN       VOID                *Context
  );

BOOLEAN
EFIAPI
GetFtpmControlAreaV2 (
  IN OUT   VOID **FtpmControlArea
  );

VOID
EFIAPI
SwitchPspMmioDecodeV2 (
  IN       BOOLEAN SwitchFlag,
  IN OUT   UINT32 *RegisterCopy
  );

VOID
AcquirePspSmiRegMutexV2 (
  VOID
  );

VOID
ReleasePspSmiRegMutexV2 (
  VOID
  );

BOOLEAN
GetPsbHstiStatus1 (
  PSB_HSTI_STATUS_1 *PsbHstiStatus1
  );

BOOLEAN
GetPsbStatus2 (
  PSB_STATUS_2 *PsbStatus2
  );

VOID
DumpRdInstructionInfo (
  );


/**
 * @brief Loads the destination register with a hardware-generated random "seed" value
 *
 * @param Seed  buffer to store 32bits seed value
 * @return BOOLEAN TRUE: RDSEED succeed FALSE: error
 */
BOOLEAN
EFIAPI
AsmRdSeed32 (
  OUT     UINT32                    *Seed
  );

#endif // _AMD_LIB_H_


