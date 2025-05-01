/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH CONTROL SERVICE PROTOCOL
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH DXE
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 10:28:05 -0800 (Tue, 09 Dec 2014) $
 *
 */

#ifndef _FCH_CONTROL_SERVICE_PROTOCOL_H_
#define _FCH_CONTROL_SERVICE_PROTOCOL_H_


extern EFI_GUID gFchControlServiceProtocolGuid;

typedef struct _FCH_CONTROL_SERVICE_PROTOCOL FCH_CONTROL_SERVICE_PROTOCOL;

//
// Protocol prototypes
//



/** Get Fch Acpi Mmio Base Address
 *
 * This function is used to get the current ACPI MMIO Base address being used on the selected FCH.
 * @param This A pointer to the FCH_CONTROL_SERVICE_PROTOCOL instance.
 * @param FchDie The Die number of which to inquire about MMIO.
 * @param AcpiMmioBase A pointer to a caller allocated buffer where the current ACPI MMIO
 * base address of FCH will be placed.
 * @retval EFI_Success The function completed successfully.
 * @retval EFI_UNSUPPORTED The ACPI MMIO decoding is not supported on the selected FCH.
 * @retval EFI_INVALID_PARAMETER A parameter is invalid.
 */
typedef EFI_STATUS (EFIAPI *FP_FCH_GET_ACPI_MMIO) (
  IN       CONST FCH_CONTROL_SERVICE_PROTOCOL   *This,
  IN       UINT8                                FchDie,
     OUT   UINT64                               *AcpiMmioBase
);


/** FCH Control Service Protocol
 *
 * This Protocol provides FCH control and service functions for use by the platform BIOS.
 * @param Revision Revision of the protocol. presently 0x01
 * @param FchRev  (unused)
 * @param FpGetAcpiMmioBase Service function to locate the MMIO block base address.
 */
typedef struct _FCH_CONTROL_SERVICE_PROTOCOL {
  UINTN                     Revision;
  UINTN                     FchRev;
  FP_FCH_GET_ACPI_MMIO      FpGetAcpiMmioBase;
} FCH_CONTROL_SERVICE_PROTOCOL;

// current Protocol revision
#define FCH_CONTROL_SERVICE_PROTOCOL_REV  0x01

#endif // _FCH_CONTROL_SERVICE_PROTOCOL_H_


