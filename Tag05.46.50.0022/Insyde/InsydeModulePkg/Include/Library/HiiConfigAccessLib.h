/** @file
  Public include file for the HII Database Library

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

Copyright (c) 2007 - 2013, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under 
the terms and conditions of the BSD License that accompanies this distribution.  
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.                                            

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

**/

#ifndef __HII_CONFIG_ACCESS_LIB_H__
#define __HII_CONFIG_ACCESS_LIB_H__

#include <Protocol/HiiConfigAccess.h>

extern EFI_HII_CONFIG_ACCESS_PROTOCOL *gHiiConfigAccess;

/**
  Allocates and returns a Null-terminated Unicode <ConfigHdr> string using routing 
  information that includes a GUID, an optional Unicode string name, and a device
  path. The string returned is allocated with AllocatePool().  The caller is 
  responsible for freeing the allocated string with FreePool().
  
  The format of a <ConfigHdr> is as follows:

    GUID=<HexCh>32&NAME=<Char>NameLength&PATH=<HexChar>DevicePathSize<Null>

  @param[in]  Guid          The pointer to an EFI_GUID that is the routing information
                            GUID.  Each of the 16 bytes in Guid is converted to 
                            a 2 Unicode character hexadecimal string.  This is 
                            an optional parameter that may be NULL.
  @param[in]  Name          The pointer to a Null-terminated Unicode string that is 
                            the routing information NAME.  This is an optional 
                            parameter that may be NULL.  Each 16-bit Unicode 
                            character in Name is converted to a 4 character Unicode 
                            hexadecimal string.                        
  @param[in]  DriverHandle  The driver handle that supports a Device Path Protocol
                            that is the routing information PATH.  Each byte of
                            the Device Path associated with DriverHandle is converted
                            to a two (Unicode) character hexadecimal string.

  @retval NULL   DriverHandle does not support the Device Path Protocol.
  @retval NULL   DriverHandle does not support the Device Path Protocol.
  @retval Other  A pointer to the Null-terminate Unicode <ConfigHdr> string

**/
EFI_STRING
EFIAPI
HiiConstructConfigHdr (
  IN CONST EFI_GUID  *Guid,  OPTIONAL
  IN CONST CHAR16    *Name,  OPTIONAL
  IN EFI_HANDLE      DriverHandle
  );

/**
  Determines if the routing data specified by GUID and NAME match a <ConfigHdr>.

  If ConfigHdr is NULL, then ASSERT().

  @param[in] ConfigHdr  Either <ConfigRequest> or <ConfigResp>.
  @param[in] Guid       The GUID of the storage.
  @param[in] Name       The NAME of the storage.

  @retval TRUE   Routing information matches <ConfigHdr>.
  @retval FALSE  Routing information does not match <ConfigHdr>.

**/
BOOLEAN
EFIAPI
HiiIsConfigHdrMatch (
  IN CONST EFI_STRING  ConfigHdr,
  IN CONST EFI_GUID    *Guid,     OPTIONAL
  IN CONST CHAR16      *Name      OPTIONAL
  );
  
#endif
