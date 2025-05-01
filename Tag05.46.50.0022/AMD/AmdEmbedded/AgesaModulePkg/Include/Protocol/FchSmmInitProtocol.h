/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH SMM
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 10:28:05 -0800 (Tue, 09 Dec 2014) $
 *
 */

#ifndef _FCH_SMM_INIT_PROTOCOL_H_
#define _FCH_SMM_INIT_PROTOCOL_H_


extern EFI_GUID gFchSmmInitProtocolGuid;
extern EFI_GUID gFchSmmOemSecureCompleteProtocolGuid;

#define AMD_FCH_OEM_SECURE_PROTOCOL_REVISION   0x00

typedef struct _FCH_SMM_INIT_PROTOCOL FCH_SMM_INIT_PROTOCOL;

typedef struct _FCH_SMM_OEM_SECURE_COMPLETE_PROTOCOL FCH_SMM_OEM_SECURE_COMPLETE_PROTOCOL;

//
// Protocol prototypes
//

//
// USB port disable function
//
typedef EFI_STATUS (EFIAPI *FP_FCH_SMM_SECURE_USB_PORT_DISABLE) (
  IN       CONST FCH_SMM_INIT_PROTOCOL   *This              ///< FCH SMM INIT Protocol
);


/// FCH INIT Protocol
typedef struct _FCH_SMM_INIT_PROTOCOL {
  UINTN                               Revision;                              ///< Protocol Revision
  UINTN                               FchRev;                                ///< FCH Revision
  VOID                                *FchSmmPolicy;                         ///< Fch Config Data Block
  FP_FCH_SMM_SECURE_USB_PORT_DISABLE  FchSmmSecureUsbPortDisable;            ///< Fch Smm Secure Usb Port Disable
} FCH_SMM_INIT_PROTOCOL;


// FchOemSecure complete protocol
typedef struct _FCH_SMM_OEM_SECURE_COMPLETE_PROTOCOL {
  UINTN    Revision;               ///< Revision Number
} FCH_SMM_OEM_SECURE_COMPLETE_PROTOCOL;

#endif // _FCH_INIT_PROTOCOL_H_


