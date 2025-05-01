/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include <Library/AmdPspRegMuxLibV2.h>
#include <Filecode.h>
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_AMDPSPREGMUXLIBV2NULL_AMDPSPREGMUXLIBV2_FILECODE
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 * Acquire the Mutex for access PSP,X86 co-accessed register
 * Call this routine before access certain registers, especially for SMI registers
 *
 */
VOID
AcquirePspAccRegMutex ()
{
}

/**
 * Release the Mutex for access PSP,X86 co-accessed register
 * Call this routine after access certain registers, especially for SMI registers
 *
 */
VOID
ReleasePspAccRegMutex ()
{
}
