/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _SPD_I3C_PEI_H_
#define _SPD_I3C_PEI_H_

typedef
EFI_STATUS
(EFIAPI *SPDI3C_SET_PPI) (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Command,
  IN       UINT8             Data
  );

typedef
EFI_STATUS
(EFIAPI *SPDI3C_GET_PPI) (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Command,
  OUT      UINT8             *Data
  );

///
/// This PPI provide interface to set SPDI3C - 8 Bit IO Expander chip.
///
typedef struct _EFI_PEI_SPDI3C_PPI {
  UINTN    Revision;                  ///< Revision Number
  SPDI3C_SET_PPI Set;                ///< Write Register
  SPDI3C_GET_PPI Get;                ///< Read Register
} EFI_PEI_SPDI3C_PPI;

// Current PPI revision
#define SPDI3C_PPI_REVISION   (0x00)

extern EFI_GUID gSpdI3cPpiGuid;

EFI_STATUS
EFIAPI
SpdI3cSet (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Command,
  IN       UINT8             Data
  );

EFI_STATUS
EFIAPI
SpdI3cGet (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINTN             BusSelect,
  IN       UINTN             SlaveAddress,
  IN       UINT8             Command,
  OUT      UINT8             *Data
  );

#endif // _SPD_I3C_PEI_H_


