/** @file
  Register the UFS Controller Driver name

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

#include "UfsPassThru.h"

//
// EFI Component Name Protocol
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME_PROTOCOL  gUfsPassThruComponentName = {
  UfsPassThruComponentNameGetDriverName,
  UfsPassThruComponentNameGetControllerName,
  "eng"
};

//
// EFI Component Name 2 Protocol
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME2_PROTOCOL gUfsPassThruComponentName2 = {
  (EFI_COMPONENT_NAME2_GET_DRIVER_NAME) UfsPassThruComponentNameGetDriverName,
  (EFI_COMPONENT_NAME2_GET_CONTROLLER_NAME) UfsPassThruComponentNameGetControllerName,
  "en"
};


GLOBAL_REMOVE_IF_UNREFERENCED EFI_UNICODE_STRING_TABLE mUfsPassThruDriverNameTable[] = {
  {
    "eng;en",
    L"Universal Flash Storage (UFS) Pass Thru Driver"
  },
  {
    NULL,
    NULL
  }
};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_UNICODE_STRING_TABLE mUfsPassThruControllerNameTable[] = {
  {
    "eng;en",
    L"Universal Flash Storage (UFS) Host Controller"
  },
  {
    NULL,
    NULL
  }
};

/**
  Retrieves a Unicode string that is the user readable name of the driver.

  This function retrieves the user readable name of a driver in the form of a
  Unicode string. If the driver specified by This has a user readable name in
  the language specified by Language, then a pointer to the driver name is
  returned in DriverName, and EFI_SUCCESS is returned. If the driver specified
  by This does not support the language specified by Language,
  then EFI_UNSUPPORTED is returned.

  @param  This[in]              A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param  Language[in]          A pointer to a Null-terminated ASCII string
                                array indicating the language. This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified
                                in RFC 4646 or ISO 639-2 language code format.

  @param  DriverName[out]       A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                driver specified by This in the language
                                specified by Language.

  @retval EFI_SUCCESS           The Unicode string for the Driver specified by
                                This and the language specified by Language was
                                returned in DriverName.

  @retval EFI_INVALID_PARAMETER Language is NULL.

  @retval EFI_INVALID_PARAMETER DriverName is NULL.

  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/
EFI_STATUS
EFIAPI
UfsPassThruComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  )
{
  return LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           mUfsPassThruDriverNameTable,
           DriverName,
           (BOOLEAN)(This == &gUfsPassThruComponentName)
           );
}

/**
  Retrieves a Unicode string that is the user readable name of the controller
  that is being managed by a driver.

  This function retrieves the user readable name of the controller specified by
  ControllerHandle and ChildHandle in the form of a Unicode string. If the
  driver specified by This has a user readable name in the language specified by
  Language, then a pointer to the controller name is returned in ControllerName,
  and EFI_SUCCESS is returned.  If the driver specified by This is not currently
  managing the controller specified by ControllerHandle and ChildHandle,
  then EFI_UNSUPPORTED is returned.  If the driver specified by This does not
  support the language specified by Language, then EFI_UNSUPPORTED is returned.

  @param  This[in]              A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param  ControllerHandle[in]  The handle of a controller that the driver
                                specified by This is managing.  This handle
                                specifies the controller whose name is to be
                                returned.

  @param  ChildHandle[in]       The handle of the child controller to retrieve
                                the name of.  This is an optional parameter that
                                may be NULL.  It will be NULL for device
                                drivers.  It will also be NULL for a bus drivers
                                that wish to retrieve the name of the bus
                                controller.  It will not be NULL for a bus
                                driver that wishes to retrieve the name of a
                                child controller.

  @param  Language[in]          A pointer to a Null-terminated ASCII string
                                array indicating the language.  This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified in
                                RFC 4646 or ISO 639-2 language code format.

  @param  ControllerName[out]   A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                controller specified by ControllerHandle and
                                ChildHandle in the language specified by
                                Language from the point of view of the driver
                                specified by This.

  @retval EFI_SUCCESS           The Unicode string for the user readable name in
                                the language specified by Language for the
                                driver specified by This was returned in
                                DriverName.

  @retval EFI_INVALID_PARAMETER ControllerHandle is NULL.

  @retval EFI_INVALID_PARAMETER ChildHandle is not NULL and it is not a valid
                                EFI_HANDLE.

  @retval EFI_INVALID_PARAMETER Language is NULL.

  @retval EFI_INVALID_PARAMETER ControllerName is NULL.

  @retval EFI_UNSUPPORTED       The driver specified by This is not currently
                                managing the controller specified by
                                ControllerHandle and ChildHandle.

  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/
EFI_STATUS
EFIAPI
UfsPassThruComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL                     *This,
  IN  EFI_HANDLE                                      ControllerHandle,
  IN  EFI_HANDLE                                      ChildHandle        OPTIONAL,
  IN  CHAR8                                           *Language,
  OUT CHAR16                                          **ControllerName
  )
{
  EFI_STATUS                                 Status;
  UFS_PASS_THRU_PRIVATE_DATA                 *Private;
  EFI_UNICODE_STRING_TABLE                   *ControllerNameTable;
  EFI_UFS_DEVICE_CONFIG_PROTOCOL             *UfsDevConfig;

  if (Language == NULL || ControllerName == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiUfsDeviceConfigProtocolGuid,
                  (VOID**) &UfsDevConfig,
                  gUfsPassThruDriverBinding.DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (Status != EFI_SUCCESS) {
    return EFI_UNSUPPORTED;
  }

  Private = UFS_PASS_THRU_PRIVATE_DATA_FROM_DEV_CONFIG (UfsDevConfig);
  if (Private->Signature != UFS_PASS_THRU_SIG) {
    return EFI_INVALID_PARAMETER;
  }
  ControllerNameTable = Private->ControllerNameTable;

  return LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           ControllerNameTable,
           ControllerName,
           (BOOLEAN)(This == &gUfsPassThruComponentName)
           );
}

/**
  Add the component name for the UFS device.
  
  @param[in] Private           The pointer to the UFS_PASS_THRU_PRIVATE_DATA data structure.
  @param[in] DeviceDescriptor  The pointer to the allocated UFS Device Descriptor.

**/
VOID
AddDeviceName (
  IN  UFS_PASS_THRU_PRIVATE_DATA  *Private,
  IN  UFS_DEV_DESC                *DeviceDescriptor
  )
{
  EFI_STATUS                             Status;
  UINT32                                 DeviceDescriptorSize;
  UFS_DESC_MN                            DescMn;
  UFS_DESC_SN                            DescSn;
  CHAR16                                 ModelName[80];
  UINT8                                  Mn[41];
  UINT8                                  Sn[21];
  UINT8                                  Index;
  UINTN                                  ArrIndex;

  //
  // Get device Mn and Sn
  //
  DeviceDescriptorSize = sizeof (UFS_DESC_MN);
  Status = UfsRwDeviceDesc (
              Private,
              TRUE,
              UfsStringDesc,
              DeviceDescriptor->ManufacturerName,
              0,
              &DescMn,
              &DeviceDescriptorSize
              );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read device manufacturerName, status = %r\n", Status));
  } else {
    ZeroMem (Mn, sizeof (Mn));
    for (Index = 0, ArrIndex = 0; Index < DescMn.Length - 2; Index++) {
      if (DescMn.Mn[Index] == 0) {
        continue;
      } else {
        Mn[ArrIndex] = DescMn.Mn[Index];
        ArrIndex++;
      }
    }
  }

  DeviceDescriptorSize = sizeof (UFS_DESC_SN);
  Status = UfsRwDeviceDesc (
             Private,
             TRUE,
             UfsStringDesc,
             DeviceDescriptor->SerialName,
             0,
             &DescSn,
             &DeviceDescriptorSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read device SerialName, status = %r\n", Status));
  } else {
    ZeroMem (Sn, sizeof (Sn));
    for (Index = 0, ArrIndex = 0; Index < DescSn.Length - 2; Index++) {
      if (DescSn.Sn[Index] == 0) {
        continue;
      } else {
        Sn[ArrIndex] = DescSn.Sn[Index];
        ArrIndex++;
      }
    }
  }
  
  //
  // Add Component Name for the UFS device that was discovered.
  //
  UnicodeSPrintAsciiFormat (ModelName, sizeof (ModelName), "%a-%a", Mn, Sn);

  Private->ControllerNameTable = NULL;
  AddUnicodeString2 (
    "eng",
    gUfsPassThruComponentName.SupportedLanguages,
    &Private->ControllerNameTable,
    ModelName,
    TRUE
    );

  AddUnicodeString2 (
    "en",
    gUfsPassThruComponentName2.SupportedLanguages,
    &Private->ControllerNameTable,
    ModelName,
    FALSE
    );
}