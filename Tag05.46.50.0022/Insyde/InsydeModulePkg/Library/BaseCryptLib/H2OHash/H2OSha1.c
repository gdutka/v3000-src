/** @file
  MD5 Digest Wrapper Implementation

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "Uefi.h"

#include <Library/BaseLib.h>
#include <Library/PcdLib.h>

UINTN
EFIAPI
InternalSha1GetContextSize (
  VOID
  );


BOOLEAN
EFIAPI
InternalSha1Init (
  OUT  VOID  *Sha1Context
  );

BOOLEAN
EFIAPI
InternalSha1Duplicate (
  IN   CONST VOID  *Sha1Context,
  OUT  VOID        *NewSha1Context
  );

BOOLEAN
EFIAPI
InternalSha1Update (
  IN OUT  VOID        *Sha1Context,
  IN      CONST VOID  *Data,
  IN      UINTN       DataSize
  );

BOOLEAN
EFIAPI
InternalSha1Final (
  IN OUT  VOID   *Sha1Context,
  OUT     UINT8  *HashValue
  );

BOOLEAN
EFIAPI
InternalSha1HashAll (
  IN   CONST VOID  *Data,
  IN   UINTN       DataSize,
  OUT  UINT8       *HashValue
  );

/**
  Retrieves the size, in bytes, of the context buffer required for SHA-1 hash operations.

  @return  The size, in bytes, of the context buffer required for SHA-1 hash operations.

**/
UINTN
EFIAPI
Sha1GetContextSize (
  VOID
  )
{
  if (!FeaturePcdGet (PcdH2OCryptoSha1Supported)) {
    return 0;
  }
  return InternalSha1GetContextSize ();
}

/**
  Initializes user-supplied memory pointed by Sha1Context as SHA-1 hash context for
  subsequent use.

  If Sha1Context is NULL, then return FALSE.

  @param[out]  Sha1Context  Pointer to SHA-1 context being initialized.

  @retval TRUE   SHA-1 context initialization succeeded.
  @retval FALSE  SHA-1 context initialization failed.

**/
BOOLEAN
EFIAPI
Sha1Init (
  OUT  VOID  *Sha1Context
  )
{
  if (!FeaturePcdGet (PcdH2OCryptoSha1Supported)) {
    return FALSE;
  }
  return InternalSha1Init (Sha1Context);
}

/**
  Makes a copy of an existing SHA-1 context.

  If Sha1Context is NULL, then return FALSE.
  If NewSha1Context is NULL, then return FALSE.

  @param[in]  Sha1Context     Pointer to SHA-1 context being copied.
  @param[out] NewSha1Context  Pointer to new SHA-1 context.

  @retval TRUE   SHA-1 context copy succeeded.
  @retval FALSE  SHA-1 context copy failed.

**/
BOOLEAN
EFIAPI
Sha1Duplicate (
  IN   CONST VOID  *Sha1Context,
  OUT  VOID        *NewSha1Context
  )
{
  if (!FeaturePcdGet (PcdH2OCryptoSha1Supported)) {
    return FALSE;
  }
  return InternalSha1Duplicate (Sha1Context, NewSha1Context);
}

/**
  Digests the input data and updates SHA-1 context.

  This function performs SHA-1 digest on a data buffer of the specified size.
  It can be called multiple times to compute the digest of long or discontinuous data streams.
  SHA-1 context should be already correctly intialized by Sha1Init(), and should not be finalized
  by Sha1Final(). Behavior with invalid context is undefined.

  If Sha1Context is NULL, then return FALSE.

  @param[in, out]  Sha1Context  Pointer to the SHA-1 context.
  @param[in]       Data         Pointer to the buffer containing the data to be hashed.
  @param[in]       DataSize     Size of Data buffer in bytes.

  @retval TRUE   SHA-1 data digest succeeded.
  @retval FALSE  SHA-1 data digest failed.

**/
BOOLEAN
EFIAPI
Sha1Update (
  IN OUT  VOID        *Sha1Context,
  IN      CONST VOID  *Data,
  IN      UINTN       DataSize
  )
{
  if (!FeaturePcdGet (PcdH2OCryptoSha1Supported)) {
    return FALSE;
  }
  return InternalSha1Update (Sha1Context, Data, DataSize);
}

/**
  Completes computation of the SHA-1 digest value.

  This function completes SHA-1 hash computation and retrieves the digest value into
  the specified memory. After this function has been called, the SHA-1 context cannot
  be used again.
  SHA-1 context should be already correctly intialized by Sha1Init(), and should not be
  finalized by Sha1Final(). Behavior with invalid SHA-1 context is undefined.

  If Sha1Context is NULL, then return FALSE.
  If HashValue is NULL, then return FALSE.

  @param[in, out]  Sha1Context  Pointer to the SHA-1 context.
  @param[out]      HashValue    Pointer to a buffer that receives the SHA-1 digest
                                value (20 bytes).

  @retval TRUE   SHA-1 digest computation succeeded.
  @retval FALSE  SHA-1 digest computation failed.

**/
BOOLEAN
EFIAPI
Sha1Final (
  IN OUT  VOID   *Sha1Context,
  OUT     UINT8  *HashValue
  )
{
  if (!FeaturePcdGet (PcdH2OCryptoSha1Supported)) {
    return FALSE;
  }
  return InternalSha1Final (Sha1Context, HashValue);
}


/**
  Computes the SHA-1 message digest of a input data buffer.

  This function performs the SHA-1 message digest of a given data buffer, and places
  the digest value into the specified memory.

  If this interface is not supported, then return FALSE.

  @param[in]   Data        Pointer to the buffer containing the data to be hashed.
  @param[in]   DataSize    Size of Data buffer in bytes.
  @param[out]  HashValue   Pointer to a buffer that receives the SHA-1 digest
                           value (20 bytes).

  @retval TRUE   SHA-1 digest computation succeeded.
  @retval FALSE  SHA-1 digest computation failed.
  @retval FALSE  This interface is not supported.

**/
BOOLEAN
EFIAPI
Sha1HashAll (
  IN   CONST VOID  *Data,
  IN   UINTN       DataSize,
  OUT  UINT8       *HashValue
  )
{
  if (!FeaturePcdGet (PcdH2OCryptoSha1Supported)) {
    return FALSE;
  }
  return InternalSha1HashAll (Data, DataSize, HashValue);
}