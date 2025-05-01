/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
 The temporary Kms solution service functions.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#include "KmsService.h"

EFI_GUID gEfiKmsServiceSampleGuid        = EFI_KMS_SERVICE_SAMPLE_GUID;
EFI_GUID gEfiKmsFormatGenericDynamicGuid = EFI_KMS_FORMAT_GENERIC_DYNAMIC_GUID;

/**
  Init Kms private data.

  @param[in]      KmsPrivate         The global Kms device data.

**/
VOID
KmsInitPrivateData (
  EFI_KMS_PRIVATE_DATA    *KmsPrivate
  )
{
  KmsPrivate->Kms.GetServiceStatus    = (EFI_KMS_GET_SERVICE_STATUS) EfiKmsGetServiceStatus;
  KmsPrivate->Kms.RegisterClient      = (EFI_KMS_REGISTER_CLIENT) EfiKmsRegisterClient;
  KmsPrivate->Kms.CreateKey           = (EFI_KMS_CREATE_KEY) EfiKmsCreateKey;
  KmsPrivate->Kms.GetKey              = (EFI_KMS_GET_KEY) EfiKmsGetKey;
  KmsPrivate->Kms.AddKey              = (EFI_KMS_ADD_KEY) EfiKmsAddKey;
  KmsPrivate->Kms.DeleteKey           = (EFI_KMS_DELETE_KEY) EfiKmsDeleteKey;
  KmsPrivate->Kms.GetKeyAttributes    = (EFI_KMS_GET_KEY_ATTRIBUTES) EfiKmsGetKeyAttributes;
  KmsPrivate->Kms.AddKeyAttributes    = (EFI_KMS_ADD_KEY_ATTRIBUTES) EfiKmsAddKeyAttributes;
  KmsPrivate->Kms.DeleteKeyAttributes = (EFI_KMS_DELETE_KEY_ATTRIBUTES) EfiKmsDeleteKeyAttributes;
  KmsPrivate->Kms.GetKeyByAttributes  = (EFI_KMS_GET_KEY_BY_ATTRIBUTES) EfiKmsGetKeyByAttributes;

  KmsPrivate->Kms.ProtocolVersion = EFI_KMS_PROTOCOL_VERSION;
  CopyGuid (&KmsPrivate->Kms.ServiceId, &gEfiKmsServiceSampleGuid);

  KmsPrivate->Kms.ServiceName           = L"KmsServiceSample";
  KmsPrivate->Kms.ServiceVersion        = 1;
  KmsPrivate->Kms.ServiceAvailable      = TRUE;
  KmsPrivate->Kms.ClientIdSupported     = TRUE;
  KmsPrivate->Kms.ClientIdRequired      = TRUE;
  KmsPrivate->Kms.ClientIdMaxSize       = sizeof (EFI_GUID);
  KmsPrivate->Kms.ClientNameStringTypes = EFI_KMS_DATA_TYPE_UNICODE;
  KmsPrivate->Kms.ClientNameRequired    = FALSE;
  KmsPrivate->Kms.ClientNameMaxCount    = EFI_KMS_CLIENT_NAME_MAX_COUNT;
  KmsPrivate->Kms.ClientDataSupported   = TRUE;
  KmsPrivate->Kms.ClientDataMaxSize     = EFI_KMS_CLIENT_DATA_MAX_SIZE;

  KmsPrivate->Kms.KeyIdVariableLenSupported = TRUE;
  KmsPrivate->Kms.KeyIdMaxSize              = EFI_KMS_KEY_ID_MAX_LEN;

  KmsPrivate->Kms.KeyFormatsCount       = 1;
  KmsPrivate->Kms.KeyFormats = AllocatePool (sizeof (EFI_GUID));
  ASSERT (KmsPrivate->Kms.KeyFormats != NULL);
  if (KmsPrivate->Kms.KeyFormats != NULL) {
    CopyGuid (KmsPrivate->Kms.KeyFormats, &gEfiKmsFormatGenericDynamicGuid);
  }
  KmsPrivate->Kms.KeyAttributesSupported    = FALSE;
  KmsPrivate->Kms.KeyAttributeIdStringTypes = EFI_KMS_DATA_TYPE_NONE;
  KmsPrivate->Kms.KeyAttributeIdMaxCount    = 0;
  KmsPrivate->Kms.KeyAttributesCount        = 0;
  KmsPrivate->Kms.KeyAttributes             = NULL;

  KmsPrivate->EndOfDxe = FALSE;
  InitializeListHead (&KmsPrivate->RegisteredClientList);
}

/**
  The Kms registered event handle when end of Dxe.

  @param[in]      Event         The end of Dxe event.
  @param[in]      Context       The event context data.

**/
VOID
EFIAPI
KmsOnEndOfDxe (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_KMS_PRIVATE_DATA        *KmsPrivate;

  KmsPrivate = (EFI_KMS_PRIVATE_DATA*) Context;
  KmsPrivate->EndOfDxe = TRUE;

  gBS->CloseEvent (KmsPrivate->EndOfDxeEvent);
}

/**
  Init Kms environment.

  @param[in]      KmsPrivate         The global Kms device data.

  @retval EFI_SUCCESS                Kms service has been initiallized successfully.
  @retval EFI_ABORTED                Fail to init the random seed.
  @retval EFI_INVALID_PARAMETER      One or more parameters are invalid.

**/
EFI_STATUS
KmsServiceInit (
  IN EFI_KMS_PRIVATE_DATA     *KmsPrivate
  )
{
  EFI_STATUS                  Status;

  Status = EFI_SUCCESS;

  if (KmsPrivate == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  KmsPrivate->Signature   = KMS_SERVICE_SIGNATURE;
  KmsInitPrivateData (KmsPrivate);

  //
  // Init the ramdom seed.
  //
  if (!RandomSeed (NULL, 0)) {
    return EFI_ABORTED;
  }

  //
  // Register an end of DXE event.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  KmsOnEndOfDxe,
                  KmsPrivate,
                  &gEfiEndOfDxeEventGroupGuid,
                  &KmsPrivate->EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "Kms Service Started %r.\n", Status));

  return Status;
}

