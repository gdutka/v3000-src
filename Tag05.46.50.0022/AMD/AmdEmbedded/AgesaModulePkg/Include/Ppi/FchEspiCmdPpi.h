/*****************************************************************************
 *
 * Copyright (C) 2008-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef __FCH_ESPI_CMD_PPI_H__
#define __FCH_ESPI_CMD_PPI_H__

extern EFI_GUID gAmdFchEspiCmdPpiGuid;

typedef struct _EFI_PEI_ESPI_CMD_PPI EFI_PEI_ESPI_CMD_PPI;

/**
  * eSPI In Band Reset
  *
  * @param This                   Pointer to an EFI_PEI_ESPI_CMD_PPI structure.
  * @param[in]  EspiBase          Espi MMIO base
  *
  */
typedef
VOID
(EFIAPI *EFI_PEI_ESPI_CMD_PPI_IN_BAND_RST) (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase
  );

/**
  * eSPI Get Configuration
  *
  * @param This                   Pointer to an EFI_PEI_ESPI_CMD_PPI structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  RegAddr           Slave register address
  *
  * @retval    Register Value
  *
  */
typedef
UINT32
(EFIAPI *EFI_PEI_ESPI_CMD_PPI_GET_CONFIGURATION) (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr
 );

/**
  * eSPI Set Configuration
  *
  * @param This                   Pointer to an EFI_PEI_ESPI_CMD_PPI structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  RegAddr           Slave register address
  * @param[in]  Value             Slave register value
  *
  */
typedef
VOID
(EFIAPI *EFI_PEI_ESPI_CMD_PPI_SET_CONFIGURATION) (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr,
  IN  UINT32                      Value
  );

/**
  * eSPI MMIO Read
  *
  * @param This                   Pointer to an EFI_PEI_ESPI_CMD_PPI structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  MemData           Pointer to memory data and addr
  *
  */
typedef
VOID
(EFIAPI *EFI_PEI_ESPI_CMD_PPI_MEM_READ) (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW               *MemData
  );

/**
  * eSPI MMIO Write
  *
  * @param This                   Pointer to an EFI_PEI_ESPI_CMD_PPI structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  MemData           Pointer to memory data and addr
  *
  */
typedef
VOID
(EFIAPI *EFI_PEI_ESPI_CMD_PPI_MEM_WRITE) (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW                      *MemData
  );

/**
  * eSPI IO Read
  *
  * @param This                   Pointer to an EFI_PEI_ESPI_CMD_PPI structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  Message_IO        Pointer to IO data and addr
  *
  */
typedef
VOID
(EFIAPI *EFI_PEI_ESPI_CMD_PPI_IO_READ) (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                      *Message_IO
  ); 

/**
  * eSPI IO Write
  *
  * @param This                   Pointer to an EFI_PEI_ESPI_CMD_PPI structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  Message_IO        Pointer to IO data and addr
  *
  */
typedef
VOID
(EFIAPI *EFI_PEI_ESPI_CMD_PPI_IO_WRITE) (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                      *Value
  ); 

/**
  * @brief This PPI provides the functions for eSpi flash commands
  * @param InBandRst                eSPI In Band Reset
  * @param GetConfiguration         eSPI Get Configuration
  * @param SetConfiguration         eSPI Set Configuration
  * @param PeiEspiMemRead           eSPI MMIO read
  * @param PeiEspiMemWrite          eSPI MMIO Write
  * @param PeiEspiIORead            eSPI IO read
  * @param PeiEspiIOWrite           eSPI IO Write  
  */
typedef struct _EFI_PEI_ESPI_CMD_PPI {
  EFI_PEI_ESPI_CMD_PPI_IN_BAND_RST       InBandRst;
  EFI_PEI_ESPI_CMD_PPI_GET_CONFIGURATION GetConfiguration;
  EFI_PEI_ESPI_CMD_PPI_SET_CONFIGURATION SetConfiguration;
  EFI_PEI_ESPI_CMD_PPI_MEM_READ          PeiEspiMemRead;
  EFI_PEI_ESPI_CMD_PPI_MEM_WRITE         PeiEspiMemWrite;
  EFI_PEI_ESPI_CMD_PPI_IO_READ           PeiEspiIORead;
  EFI_PEI_ESPI_CMD_PPI_IO_WRITE          PeiEspiIOWrite;  
} EFI_PEI_ESPI_CMD_PPI;

#endif

