/** @file
  PCI Lookup table DXE

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PciLookupDxe.h>
#include <Library/PciCapabilityIdTableLib.h>
#include <Library/PciClassCodeTableLib.h>
#include <Library/PciDeviceIdTableLib.h>
#include <Library/PciVendorIdTableLib.h>

EFI_HANDLE                                mDriverHandle = NULL;
EFI_HII_HANDLE                            mStringPackHandle = NULL;
extern UINT8                              PciLookupTableDxeStrings[];

//
// Linkspeed table
//
PCIE_LINKSPEED_INFO mPcieLinkSpeed[]    = { {0x00, STRING_TOKEN (STR_EMPTY_STRING)           , STRING_TOKEN (STR_PCIE_LINK_NOT_READY_STRING)},
                                            {0x01, STRING_TOKEN (STR_PCIE_LNKSPEED01_STRING) , STRING_TOKEN (STR_PCIE_REVISION01_STRING)}, //  2,5GT/s Gen1
                                            {0x02, STRING_TOKEN (STR_PCIE_LNKSPEED02_STRING) , STRING_TOKEN (STR_PCIE_REVISION02_STRING)}, //  5.0GT/s Gen2
                                            {0x03, STRING_TOKEN (STR_PCIE_LNKSPEED03_STRING) , STRING_TOKEN (STR_PCIE_REVISION03_STRING)}, //  8.0GT/s Gen3
                                            {0x04, STRING_TOKEN (STR_PCIE_LNKSPEED04_STRING) , STRING_TOKEN (STR_PCIE_REVISION04_STRING)}, // 16.0GT/s Gen4
                                            {0x05, STRING_TOKEN (STR_PCIE_LNKSPEED05_STRING) , STRING_TOKEN (STR_PCIE_REVISION05_STRING)}, // 32.0GT/s Gen5
                                            {0x06, STRING_TOKEN (STR_PCIE_LNKSPEED06_STRING) , STRING_TOKEN (STR_PCIE_REVISION06_STRING)}, // 64.0GT/s Gen6
                                            {0xff, STRING_TOKEN (STR_EMPTY_STRING)           , STRING_TOKEN (STR_UNDEFINED_STRING)}
                                          };
//
// Header type Lookup Table
//
EFI_STRING_ID       mPciMultiStr[]      = { STRING_TOKEN (STR_PCI_SINGLE_FUNCTION_STRING),
                                            STRING_TOKEN (STR_PCI_MULTI_FUNCTION_STRING)
                                          };

EFI_STRING_ID       mPciHeaderTypeStr[] = { STRING_TOKEN (STR_PCI_HEADER_DEVICE_STRING),
                                            STRING_TOKEN (STR_PCI_HEADER_BRIDGE_STRING),
                                            STRING_TOKEN (STR_PCI_HEAFER_CARDBUS_STRING),
                                            STRING_TOKEN (STR_UNDEFINED_STRING)
                                          };

//
// PCI Hii String Token Initialized Function Structure
//
PCI_STRING_INIT_FUNCTION    mPciStringInit[] = { PciCommonStringInit,
                                                 PciVendorIdStringInit,
                                                 PciDeviceIdStringInit,
                                                 PciClassCodeStringInit,
                                                 PciCapabilityIdStringInit
                                               };


/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
EFIAPI
PciCommonStringInit (
  VOID
) {
  mStringPackHandle = HiiAddPackages (
                        &gEfiCallerIdGuid,
                        NULL,
                        PciLookupTableDxeStrings,
                        NULL
                        );
  if (mStringPackHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  With giving Vendor ID, this function will return a short vendor name of the Vendor ID,
  if find the Vendor Id but the Vendor name isn's exist. Call GetVerdorIdFullName to get full name.

  @param[in]      VendorId        - Vendor ID for the PCI device.
  @param[in]      DeviceId        - Device ID for the PCI device.
  @param[in, out] **VendorName    - Vendor name for the Vendor ID.
  @param[in, out] **DeviceName    - Device name for the Device ID.
  @param[in]      *Languages      - String Language.


  @retval EFI_SUCCESS             - Find the String Success.
  @retval EFI_INVALID_PARAMETER   - Invalid PCI Vendor Id.
  @retval EFI_UNSUPPORTED         - All String pointer are NULL pointer or Feature closed.
  @retval EFI_NOT_FOUND           - Vendor Id not found or Device Id valued but device name not found
**/
EFI_STATUS
PCIeId2Str (
  IN     UINT16    VendorId,
  IN     UINT16    DeviceId,     OPTIONAL
  IN OUT CHAR16    **VendorName, OPTIONAL
  IN OUT CHAR16    **DeviceName, OPTIONAL
  IN     CHAR8     *Languages    OPTIONAL
  )
{
  EFI_STATUS  Status;

  Status = EFI_UNSUPPORTED;

  //
  // Invalid Vendor Id
  //
  if ((VendorId == 0) || (VendorId == 0xffff)) {
    return EFI_INVALID_PARAMETER;
  }

  if (VendorName != NULL) {
    Status = LibGetVendorIdShortName (VendorId, VendorName, Languages);
  }

  if ((DeviceId != 0xffff) && (DeviceName != NULL)) {
    Status = LibGetDeviceIdName (VendorId, DeviceId, DeviceName, Languages);
  }

  return Status;
}

/**
  With giving Vendor ID, this function will return a short vendor name of the Vendor ID,
  if find the Vendor Id but the Vendor name isn's exist. Call GetVerdorIdFullName to get full name.

  @param[in]      VendorId        - Vendor ID for the PCI device.
  @param[in]      DeviceId        - Device ID for the PCI device.
  @param[in, out] **VendorName    - Vendor name for the Vendor ID.
  @param[in, out] **DeviceName    - Device name for the Device ID.
  @param[in]      *Languages      - String Language.


  @retval EFI_SUCCESS             - Find the String Success.
  @retval EFI_INVALID_PARAMETER   - Invalid PCI Vendor Id.
  @retval EFI_UNSUPPORTED         - All String pointer are NULL pointer or Feature closed.
  @retval EFI_NOT_FOUND           - Vendor Id not found or Device Id valued but device name not found
**/
EFI_STATUS
PCIeId2StrFull (
  IN     UINT16    VendorId,
  IN     UINT16    DeviceId,     OPTIONAL
  IN OUT CHAR16    **VendorName, OPTIONAL
  IN OUT CHAR16    **DeviceName, OPTIONAL
  IN     CHAR8     *Languages    OPTIONAL
  )
{
  EFI_STATUS  Status;

  Status = EFI_UNSUPPORTED;

  if ((VendorId == 0) || (VendorId == 0xffff)) {
    return EFI_INVALID_PARAMETER;
  }

  if (VendorName != NULL) {
    Status = LibGetVendorIdLongName (VendorId, VendorName, Languages);
  }

  if ((DeviceId != 0xffff) && (DeviceName != NULL)) {
    Status = LibGetDeviceIdName (VendorId, DeviceId, DeviceName, Languages);
  }

  return Status;
}

/**
  Generates printable Unicode strings that represent PCI device class,
  subclass and programmed I/F based on a value passed to the function.

  @param[in]      ClassCode     Value representing the PCI "Class Code" register read from a
                                PCI device. The encodings are:
                                bits 23:16 - Base Class Code
                                bits 15:8  - Sub-Class Code
                                bits  7:0  - Programming Interface
  @param[in, out] ClassStrings  Pointer of PCI_CLASS_STRINGS structure, which contains
                                printable class strings corresponding to ClassCode. The
                                caller must not modify the strings that are pointed by
                                the fields in ClassStrings.
  @param[in]      *Languages    String Language.
**/
VOID
EFIAPI
PciGetClassStrings(
  IN      UINT32    ClassCode,
  IN OUT  CHAR16    **BaseClass, OPTIONAL
  IN OUT  CHAR16    **SubClass,  OPTIONAL
  IN OUT  CHAR16    **PifClass,  OPTIONAL
  IN      CHAR8     *Languages   OPTIONAL
) {
  //
  // Link to Library selection function
  //
  LibPciGetClassStrings (ClassCode, BaseClass, SubClass, PifClass, Languages);
}

/**
  With giving Linkspeed value, this function will return a String of the Linkspeed.

  @param[in]      LinkSpeed        - Linkspeed for the PCI device.
  @param[in, out] **LinkSpeedStr   - String for the Linkspeed.
  @param[in, out] **PcieRevision   - PCIe Revision String
  @param[in]      *Languages       - Hii Supported Language

  @retval EFI_SUCCESS              - Find the Linkspeed Str.
  @retval EFI_NOT_FOUND            - Undefined Linkspeed value.
**/
EFI_STATUS
GetLinkSpeedStr (
  IN     UINT32     LinkSpeed,
  IN OUT CHAR16     **LinkSpeedStr,
  IN OUT CHAR16     **PcieRevision,
  IN     CHAR8      *Languages      OPTIONAL
)
{
  UINT16      Index = 0;

  //
  // Search Lookup table
  //
  while (Index < (sizeof (mPcieLinkSpeed) / sizeof (CHAR16*))) {
    if (mPcieLinkSpeed[Index].LinkSpeed == LinkSpeed) {
      *LinkSpeedStr = HiiGetString (mStringPackHandle, mPcieLinkSpeed[Index].LinkSpeedStr, Languages);
      *PcieRevision = HiiGetString (mStringPackHandle, mPcieLinkSpeed[Index].PcieRevision, Languages);
      return EFI_SUCCESS;
    }
    Index++;
  }

  return EFI_NOT_FOUND;
}

/**
  With giving header type value, this function will return a String of the header type.

  @param[in]      HeaderType        - Header for the PCI device.
  @param[in, out] **HeaderTypeStr   - String for header type.
  @param[in, out] **MultiFuncStr    - String for Multi-Function.
  @param[in]      *Languages        - Hii Supported Language.

  @retval EFI_SUCCESS               - Find the header type str.
  @retval EFI_INVALID_PARAMETER     - HeaderTypeInfo is a NULL pointer.
**/
EFI_STATUS
GetHeaderTypeStr (
  IN     UINT8    HeaderType,
  IN OUT CHAR16   **HeaderTypeStr,
  IN OUT CHAR16   **MultiFuncStr,
  IN     CHAR8    *Languages   OPTIONAL
)
{
  UINT8 Type;
  UINT8 MultiFunction;

  //
  // MultiFunction
  //
  MultiFunction = HeaderType & BIT7;
  *MultiFuncStr = HiiGetString (mStringPackHandle, mPciMultiStr[MultiFunction >> 7], Languages);

  //
  // Header type
  //
  Type           = HeaderType & 0x7F;
  if (Type < (sizeof (mPciHeaderTypeStr) / sizeof (EFI_STRING_ID))) {
    *HeaderTypeStr = HiiGetString (mStringPackHandle, mPciHeaderTypeStr[Type], Languages);
  } else {
    *HeaderTypeStr = HiiGetString (mStringPackHandle, mPciHeaderTypeStr[HEADER_TYPE_UNDEFINE], Languages);
  }

  return EFI_SUCCESS;
}

/**
  With giving Capability ID, this function will return a String of the Capability name.

  @param[in]  CapabilityId        - Capability ID for the PCI device.
  @param[out] **CapabilityStr     - String for Capability.
  @param[in]  *Languages          - Hii Supported Language.

  @retval EFI_SUCCESS             - Find the Capability str.
  @retval EFI_INVALID_PARAMETER   - CapabilityStr is a NULL pointer.
**/
EFI_STATUS
EFIAPI
GetCapabilityStr(
  IN     UINT8    CapabilityId,
  IN OUT CHAR16   **CapabilityStr,
  IN     CHAR8    *Languages   OPTIONAL
) {
  //
  // Link to Library selection function
  //
  return LibGetCapabilityStr (CapabilityId, CapabilityStr, Languages);
}

/**
  With giving Capability ID, this function will return a String of the Capability name.

  @param[in]  ExtCapabilityId     - Capability ID for the PCI device.
  @param[out] **CapabilityStr     - String for Capability.
  @param[in]  *Languages          - Hii Supported Language.

  @retval EFI_SUCCESS             - Find the Capability str.
  @retval EFI_INVALID_PARAMETER   - CapabilityStr is a NULL pointer.
**/
EFI_STATUS
EFIAPI
GetExtCapabilityStr(
  IN     UINT16   ExtCapabilityId,
  IN OUT CHAR16   **CapabilityStr,
  IN     CHAR8    *Languages   OPTIONAL
) {
  //
  // Link to Library selection function
  //
  return LibGetExtCapabilityStr (ExtCapabilityId, CapabilityStr, Languages);
}

/**
  The entry point for the driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
PciLookupTableDriverEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
  EFI_STATUS                        Status;
  H2O_PCI_LOOKUP_TABLE_PROTOCOL     *PciLookupTableProtocol;
  UINTN                             StringInitIndex;

  PciLookupTableProtocol = NULL;
  Status = gBS->LocateProtocol (
                  &gH2OPciLookupTableProtocolGuid,
                  NULL,
                  (VOID **) &PciLookupTableProtocol
                  );
  if (EFI_ERROR (Status)) {

    PciLookupTableProtocol = AllocatePool (sizeof (H2O_PCI_LOOKUP_TABLE_PROTOCOL));
    if (PciLookupTableProtocol == NULL) {
      DEBUG ((EFI_D_ERROR, "%a() - Fail to allocate PciLookupTableProtocol buffer\n", __FUNCTION__));
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Init function pointer
    //
    PciLookupTableProtocol->PCIeId2Str             = PCIeId2Str;
    PciLookupTableProtocol->PCIeId2StrFull         = PCIeId2StrFull;
    PciLookupTableProtocol->PciGetClassStrings     = PciGetClassStrings;
    PciLookupTableProtocol->GetVendorLinkSpeedStr  = GetLinkSpeedStr;
    PciLookupTableProtocol->GetHeaderTypeStr       = GetHeaderTypeStr;
    PciLookupTableProtocol->GetCapabilityStr       = GetCapabilityStr;
    PciLookupTableProtocol->GetExtCapabilityStr    = GetExtCapabilityStr;

    //
    // Install interface
    //
    Status = gBS->InstallProtocolInterface (
                    &mDriverHandle,
                    &gH2OPciLookupTableProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    PciLookupTableProtocol
                    );
    if (EFI_ERROR (Status)) {
      FreePool(PciLookupTableProtocol);
    }

    //
    // Hii String Initialized
    //
    for (StringInitIndex = 0; StringInitIndex < sizeof(mPciStringInit)/sizeof(PCI_STRING_INIT_FUNCTION); StringInitIndex++) {
      Status = mPciStringInit[StringInitIndex]();
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR,"%a(), Init StringStruct[%d] Failed Status:r\n", __FUNCTION__, StringInitIndex));
      }
    }

  } else {
    return EFI_ALREADY_STARTED;
  }

  return Status;
}

/**
  Unloads this driver and its installed protocol.

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
  @retval other                 Some error occurs when executing driver unload.
**/
EFI_STATUS
EFIAPI
PciLookupTableDriverUnload (
  IN EFI_HANDLE        ImageHandle
)
{
  EFI_STATUS                         Status;
  H2O_PCI_LOOKUP_TABLE_PROTOCOL      *PciLookupTableProtocol;

  Status = gBS->LocateProtocol (
                  &gH2OPciLookupTableProtocolGuid,
                  NULL,
                  (VOID **) &PciLookupTableProtocol
                  );

  if (!EFI_ERROR (Status)) {
    Status = gBS->UninstallProtocolInterface (
                    mDriverHandle,
                    &gH2OPciLookupTableProtocolGuid,
                    (VOID *) PciLookupTableProtocol
                    );
    mDriverHandle = NULL;
  }
  return Status;
}
