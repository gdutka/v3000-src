/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _SPD_I3C_DXE_H_
#define _SPD_I3C_DXE_H_

typedef struct _EFI_DXE_SPDI3C_PROTOCOL EFI_DXE_SPDI3C_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *SPDI3C_SET_PROTOCOL) (
  IN CONST EFI_DXE_SPDI3C_PROTOCOL  *This,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Command,
  IN       UINT8             Data
  );

typedef
EFI_STATUS
(EFIAPI *SPDI3C_GET_PROTOCOL) (
  IN CONST EFI_DXE_SPDI3C_PROTOCOL  *This,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Command,
  OUT      UINT8             *Data
  );

///
/// This PROTOCOL provide interface to set SPDI3C - 8 Bit IO Expander chip.
///
struct _EFI_DXE_SPDI3C_PROTOCOL {
  UINTN    Revision;                  ///< Revision Number
  SPDI3C_SET_PROTOCOL Set;                ///< Write Register
  SPDI3C_GET_PROTOCOL Get;                ///< Read Register
};

// Current PROTOCOL revision
#define SPDI3C_PROTOCOL_REVISION   (0x00)

extern EFI_GUID gSpdI3cProtoclGuid;

EFI_STATUS
EFIAPI
SpdI3cSet (
  IN CONST EFI_DXE_SPDI3C_PROTOCOL  *This,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Command,
  IN       UINT8             Data
  );

EFI_STATUS
EFIAPI
SpdI3cGet (
  IN CONST EFI_DXE_SPDI3C_PROTOCOL  *This,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Command,
  OUT      UINT8             *Data
  );

EFI_STATUS
EFIAPI
SpdI3cDxeEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  );

#endif // _SPD_I3C_DXE_H_


