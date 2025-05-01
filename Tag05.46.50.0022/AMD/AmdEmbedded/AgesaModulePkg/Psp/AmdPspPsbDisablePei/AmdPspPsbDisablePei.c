/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "PiPei.h"
#include <Library/DebugLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/AmdPspCommonLib.h>
#include <Filecode.h>

#define FILECODE PSP_AMDPSPPSBDISABLEPEI_AMDPSPPSBDISABLEPEI_FILECODE

extern  EFI_GUID  gAmdErrorLogServicePpiGuid;
#define MP0_C2P_MSG_37_OFFSET                      (0x10994)              ///< MP::MP0CRU::MP0_C2PMSG_37 Register SMN offset
#define MP0_C2P_MSG_38_OFFSET                      (0x10998)              ///< MP::MP0CRU::MP0_C2PMSG_38 Register SMN offset

/*++

Routine Description:

  PSP Driver Entry. Initialize PSP device and report DRAM info to PSP once found

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
EFIAPI
AmdPspPsbDisableEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT32          Result;
  UINT32          Mp0C2pMsg37Reg;
  UINT32          Mp0C2pMsg38Reg;

  DEBUG ((EFI_D_ERROR, "***************************** AmdPspPsbDisable Enter *********************\n"));

  if (PcdGetBool (PcdAmdPspPsbDisable) == TRUE) {
    DEBUG ((EFI_D_ERROR, "Perform PSB Disabling\n"));
    Status = PspMboxBiosDisablePsb (&Result);
      //check PSP return status.
    switch (Status) {
    case  EFI_SUCCESS:
      DEBUG ((EFI_D_ERROR, "Command Status: EFI_SUCCESS\n"));
      break;
    case  EFI_INVALID_PARAMETER:
      DEBUG ((EFI_D_ERROR, "Command Status: EFI_INVALID_PARAMETER\n"));
      break;
    case  EFI_UNSUPPORTED:
      DEBUG ((EFI_D_ERROR, "Command Status: EFI_UNSUPPORTED\n"));
      break;
    default:
      Status = EFI_DEVICE_ERROR;
      DEBUG ((EFI_D_ERROR, "Command Status: EFI_DEVICE_ERROR\n"));
      break;
    }
    DEBUG ((EFI_D_ERROR, "Result: %x\n", Result));

    ReadC2PMsgReg(0, 0, MP0_C2P_MSG_37_OFFSET, &Mp0C2pMsg37Reg);
    DEBUG ((EFI_D_ERROR, "MP0_C2P_MSG_37: 0x%08x\n", Mp0C2pMsg37Reg));

    ReadC2PMsgReg (0, 0, MP0_C2P_MSG_38_OFFSET, &Mp0C2pMsg38Reg);
    DEBUG ((EFI_D_ERROR, "MP0_C2P_MSG_38: 0x%08x\n", Mp0C2pMsg38Reg));

  } else {
    DEBUG ((EFI_D_ERROR, "PcdAmdPspPsbDisable = FALSE\n"));
  }

  DEBUG ((EFI_D_ERROR, "***************************** AmdPspPsbDisable Exit *********************\n"));

  return (Status);
}



