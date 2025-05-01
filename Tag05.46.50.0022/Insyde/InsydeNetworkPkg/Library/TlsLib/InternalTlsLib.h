/** @file
  Internal include file for TlsLib.

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

Copyright (c) 2016 - 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __INTERNAL_TLS_LIB_H__
#define __INTERNAL_TLS_LIB_H__

#undef _WIN32
#undef _WIN64

#include <Library/BaseCryptLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SafeIntLib.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <x509/x509_local.h>

typedef struct {
  //
  // Main SSL Connection which is created by a server or a client
  // per established connection.
  //
  SSL                             *Ssl;
  //
  // Memory BIO for the TLS/SSL Reading operations.
  //
  BIO                             *InBio;
  //
  // Memory BIO for the TLS/SSL Writing operations.
  //
  BIO                             *OutBio;
} TLS_CONNECTION;

#endif

