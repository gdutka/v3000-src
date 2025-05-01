/*****************************************************************************
 *
 * Copyright (C) 2008-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "FchEspiCmdPei.h"
#include "FchPlatform.h"
#include <Library/HobLib.h>

#define FILECODE FCH_COMMON_FCHESPICMDPEI_FCHESPICMDPEI_FILECODE
/**
  * PpiInBandRst - eSPI In Band Reset
  *
  * @param This                   Pointer to an EFI_PEI_ESPI_CMD_PPI structure.
  * @param[in]  EspiBase          Espi MMIO base
  *
  */
VOID
EFIAPI
PeiInBandRst (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase
  )
{
  FchEspiCmd_InBandRst(EspiBase);
}

/**
  *  PeiGetConfiguration - eSPI Get Configuration
  *
  * @param This                   Pointer to an EFI_PEI_ESPI_CMD_PPI structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  RegAddr           Slave register address
  *
  * @retval    Register Value
  *
  */
UINT32
EFIAPI
PeiGetConfiguration (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr
 )
{
  return (FchEspiCmd_GetConfiguration(EspiBase, RegAddr));
}

/**
  * PeiSetConfiguration - eSPI Set Configuration
  *
  * @param This                   Pointer to an EFI_PEI_ESPI_CMD_PPI structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  RegAddr           Slave register address
  * @param[in]  Value             Slave register value
  *
  */
VOID
EFIAPI
PeiSetConfiguration (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr,
  IN  UINT32                      Value
)
{
  FchEspiCmd_SetConfiguration(EspiBase, RegAddr, Value);
}

VOID
EFIAPI
PeiEspiMemRead (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW                      *MemData
)
{
  FchEspiCmd_PeripheralMemRead(EspiBase, MemData);
}

VOID
EFIAPI
PeiEspiMemWrite (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW                      *MemData
)
{
  FchEspiCmd_PeripheralMemWrite(EspiBase, MemData);
}
VOID
EFIAPI
PeiEspiIORead (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                      *Message_IO
)
{
  FchEspiCmd_PeripheralIORead(EspiBase, Message_IO);
}
VOID
EFIAPI
PeiEspiIOWrite (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                      *Message_IO
)
{
  FchEspiCmd_PeripheralIOWrite(EspiBase, Message_IO);
}

EFI_PEI_ESPI_CMD_PPI PeiFchEspiCmdPpi = {
  PeiInBandRst,
  PeiGetConfiguration,
  PeiSetConfiguration,
  PeiEspiMemRead,
  PeiEspiMemWrite,
  PeiEspiIORead,
  PeiEspiIOWrite
};

EFI_PEI_PPI_DESCRIPTOR PeiFchEspiCmdPpiDescriptor =
{
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gAmdFchEspiCmdPpiGuid,
    &PeiFchEspiCmdPpi
};

/**
  *  @brief Entry point of the AMD FCH eSPI Command PEIM
  *
  */
EFI_STATUS
EFIAPI
AmdFchEspiCmdPeiInit (
  IN       EFI_PEI_FILE_HANDLE FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS                Status;
  EFI_HOB_GUID_TYPE         *FchHob;
  FCH_RESET_DATA_BLOCK      *FchResetParams;

  DEBUG ((DEBUG_INFO, "FchEspiCmdPei entry point: AmdFchEspiCmdPeiInit.\n"));

  FchHob = GetFirstGuidHob (&gFchResetDataHobGuid);
  FchHob++;
  FchResetParams = (FCH_RESET_DATA_BLOCK *) ((UINTN)FchHob - sizeof (UINT32));

  FchInitKTEspiDevice0(FchResetParams);

  Status = (*PeiServices)->InstallPpi (PeiServices, &PeiFchEspiCmdPpiDescriptor);

  if ( Status != EFI_SUCCESS ) {
    DEBUG ((DEBUG_ERROR, "[Warning]  InstallPpi failed (Status: 0x%x).\n", Status));
  }

  return (Status);
}

