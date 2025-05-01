/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_PROM21_INIT_PROTOCOL_H_
#define _FCH_PROM21_INIT_PROTOCOL_H_

#define AMD_FCH_PROM21_INIT_PROTOCOL_GUID \
          { 0x53F9A3C8, 0x8A2B, 0x43DA, 0x94, 0x59, 0x5D, 0x7B, 0x5C, 0x91, 0xF9, 0x36 }

extern EFI_GUID gAmdFchProm21InitProtocolGuid;

typedef struct _PT21_INIT_PROTOCOL PT21_INIT_PROTOCOL;

/// FCH INIT Protocol
typedef struct _PT21_INIT_PROTOCOL {
  UINTN                     Revision;                 ///< Protocol Revision
} PT21_INIT_PROTOCOL;

// current Protocol revision
#define PT21_INIT_REV  0x01

#define PT_21_DXE_PRIVATE_DATA_SIGNATURE   SIGNATURE_32 ('P', 'T', '2', '1')

typedef struct _PT_21_DXE_PRIVATE {
  UINTN                            Signature;           ///< Signature
  PT21_INIT_PROTOCOL               PtInit;              ///< Protocol data
  EFI_EVENT                        EventAfterPciInit;   ///< Event related data
  EFI_EVENT                        EventReadyToBoot;    ///< Event related data
} PT_21_DXE_PRIVATE;

#endif

