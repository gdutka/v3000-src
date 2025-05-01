/*****************************************************************************
 *
 * Copyright (C) 2008-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef __FCH_ESPI_CMD_PROTOCOL_H__
#define __FCH_ESPI_CMD_PROTOCOL_H__

#include <Library/FchEspiCmdLib.h>

typedef struct _EFI_ESPI_CMD_PROTOCOL EFI_ESPI_CMD_PROTOCOL;

/**
  * eSPI In Band Reset
  *
  * @param This                   Pointer to an EFI_ESPI_CMD_PROTOCOL structure.
  * @param[in]  EspiBase          Espi MMIO base
  *
  */
typedef
VOID
(EFIAPI *EFI_ESPI_CMD_PROTOCOL_IN_BAND_RST) (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase
  );

/**
  * eSPI Get Configuration
  *
  * @param This                   Pointer to an EFI_ESPI_CMD_PROTOCOL structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  RegAddr           Slave register address
  *
  * @retval    Register Value
  *
  */
typedef
UINT32
(EFIAPI *EFI_ESPI_CMD_PROTOCOL_GET_CONFIGURATION) (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr
 );

/**
  * eSPI Set Configuration
  *
  * @param This                   Pointer to an EFI_ESPI_CMD_PROTOCOL structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  RegAddr           Slave register address
  * @param[in]  Value             Slave register value
  *
  */
typedef
VOID
(EFIAPI *EFI_ESPI_CMD_PROTOCOL_SET_CONFIGURATION) (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr,
  IN  UINT32                      Value
);

/**
  * eSPI MMIO Read
  *
  * @param This                   Pointer to an EFI_ESPI_CMD_PROTOCOL structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  MemData           Pointer to memory data and addr
  *
  */
typedef
VOID
(EFIAPI *EFI_ESPI_CMD_PROTOCOL_MEM_READ) (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW               *MemData
  );

/**
  * eSPI MMIO Write
  *
  * @param This                   Pointer to an EFI_ESPI_CMD_PROTOCOL structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  MemData           Pointer to memory data and addr
  *
  */
typedef
VOID
(EFIAPI *EFI_ESPI_CMD_PROTOCOL_MEM_WRITE) (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW               *MemData
  );

/**
  * eSPI IO Read
  *
  * @param This                   Pointer to an EFI_ESPI_CMD_PROTOCOL structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  Message_IO        Pointer to IO data and addr
  *
  */
typedef
VOID
(EFIAPI *EFI_ESPI_CMD_PROTOCOL_IO_READ) (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                      *Message_IO
  ); 

/**
  * eSPI IO Write
  *
  * @param This                   Pointer to an EFI_ESPI_CMD_PROTOCOL structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  Message_IO        Pointer to IO data and addr
  *
  */
typedef
VOID
(EFIAPI *EFI_ESPI_CMD_PROTOCOL_IO_WRITE) (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                *Value
  ); 

/**
  * @brief This PROTOCOL provides the functions to send eSpi flash commands
  * @param InBandRst             eSPI In Band Reset
  * @param GetConfiguration      eSPI Get Configuration
  * @param SetConfiguration      eSPI Set Configuration
  * @param EspiMemRead           eSPI MMIO read
  * @param EspiMemWrite          eSPI MMIO Write
  * @param EspiIORead            eSPI IO read
  * @param EspiIOWrite           eSPI IO Write

  */
typedef struct _EFI_ESPI_CMD_PROTOCOL {
  EFI_ESPI_CMD_PROTOCOL_IN_BAND_RST       InBandRst;
  EFI_ESPI_CMD_PROTOCOL_GET_CONFIGURATION GetConfiguration;
  EFI_ESPI_CMD_PROTOCOL_SET_CONFIGURATION SetConfiguration;
  EFI_ESPI_CMD_PROTOCOL_MEM_READ          EspiMemRead;
  EFI_ESPI_CMD_PROTOCOL_MEM_WRITE         EspiMemWrite;
  EFI_ESPI_CMD_PROTOCOL_IO_READ           EspiIORead;
  EFI_ESPI_CMD_PROTOCOL_IO_WRITE          EspiIOWrite;
} EFI_ESPI_CMD_PROTOCOL;

#endif

