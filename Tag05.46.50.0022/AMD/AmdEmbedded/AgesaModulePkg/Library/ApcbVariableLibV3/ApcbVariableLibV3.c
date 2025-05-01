/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Library
 *
 * Contains interface to the AMD AGESA library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 */

/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "Porting.h"
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ApcbVariableLibV3.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE        LIBRARY_APCBVARIABLELIBV3_APCBVARIABLELIBV3_FILECODE

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
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

/*----------------------------------------------------------------------------------------
 *                          G L O B A L        V A L U E S
 *----------------------------------------------------------------------------------------
 */
STATIC APCB_VARIABLE_STRUCT *mApcbVariableStructAddress = NULL;

/**
 * @brief Get Apcb Variable Struct Address
 *
 * @param   VOID
 *
 * @return  APCB_VARIABLE_STRUCT* Pointer
 */
APCB_VARIABLE_STRUCT*
GetApcbVariableStruct  (
  VOID
  )
{

  if (mApcbVariableStructAddress == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[%a] mApcbVariableStructAddress is NULL\n", \
      __FUNCTION__);
  }

  return mApcbVariableStructAddress;
}


/**
 * @brief ApcbVariableLibV3Constructor
 *
 * @param[in] EFI_HANDLE        ImageHandle
 * @param[in] EFI_SYSTEM_TABLE  *SystemTable

 * @return  EFI_STATUS     EFI_SUCCESS       Success
 *                         Other Status      Fail
 */
EFI_STATUS
EFIAPI
ApcbVariableLibV3Constructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS            Status;
  APCB_VARIABLE_STRUCT  *pApcbVariableStruct;

  Status = EFI_SUCCESS;

  // After this point mApcbVariableStructAddress should already setup,
  // it can be used to store the real buffer address.
  // Check if address exist, if none allocate buffer, set buffer address.
  if (mApcbVariableStructAddress == NULL) {
    // In order to make all mApcbVariableStructAddress store the same run-time address,
    // use PCD to avoid allocating different run-time buffers.
    if (PcdGet64 (PcdApcbVariableStructAddress) == 0) {
      pApcbVariableStruct = NULL;
      Status = gBS->AllocatePool (
                      EfiRuntimeServicesData,
                      sizeof(APCB_VARIABLE_STRUCT),
                      (VOID **) &pApcbVariableStruct
                      );
      if (EFI_ERROR(Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("[%a] AllocatePool Fail, Status: %r\n", \
          __FUNCTION__, Status);
        ASSERT (FALSE);
        return EFI_SUCCESS;
      }

      if (pApcbVariableStruct == NULL) {
        IDS_HDT_CONSOLE_PSP_TRACE ("[%a] AllocatePool Address is NULL\n", \
          __FUNCTION__);
        ASSERT (FALSE);
        return EFI_SUCCESS;
      }

      IDS_HDT_CONSOLE_PSP_TRACE ("[%a] AllocatePool Address:       0x%x\n", \
        __FUNCTION__, pApcbVariableStruct);

      ZeroMem (pApcbVariableStruct, sizeof(APCB_VARIABLE_STRUCT));
      PcdSet64S (PcdApcbVariableStructAddress, (UINT64) (UINTN) pApcbVariableStruct);
    }

    mApcbVariableStructAddress = (APCB_VARIABLE_STRUCT *) (UINTN) PcdGet64 (PcdApcbVariableStructAddress);

    IDS_HDT_CONSOLE_PSP_TRACE ("[%a] mApcbVariableStructAddress: 0x%x\n", \
      __FUNCTION__, mApcbVariableStructAddress);
  }

  return EFI_SUCCESS;
}
