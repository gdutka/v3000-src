/*
*****************************************************************************
*
 * Copyright (C) 2008-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include <Filecode.h>
#include <Library/IoLib.h>
#include <Library/IdsLib.h>
#include <Library/FchBaseLib.h>
#include <Library/FchEspiCmdLib.h>
#include <FchRegistersCommon.h>

#define FILECODE LIBRARY_FCHESPICMDLIB_FCHESPICMDLIB_FILECODE
#define ESPI_CMD_DEBUG_ON     0     // set to 1 if want to turn on debugging output in the library

/*----------------------------------------------------------------------------------------*/
/**
 * Wait4stClear - Wait for DNCMD status bit to be cleared
 *
 * @param[in]  EspiBase       Espi MMIO base
 *
 */
VOID
Wait4stClear (
  IN  UINT32     EspiBase
  )
{
  UINT32            Retry;
  ESPIx00_DN_TXHDR0 EspiReg00;

  for ( Retry = 0; Retry < MAX_ESPI_RETRY; Retry++ ) {
    EspiReg00.Value = MmioRead32 ((UINTN)(EspiBase + ESPI_DN_TXHDR_0));
    if ( EspiReg00.Field.CommandStatus == 0 ) {
      break;
    }
    FchStall (2, NULL);  //delay 2us
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * EspiGetIOMmioDecodeInfo - update MMIO decode info in conifg
 *
 * @param[in]  EspiBase       Espi MMIO base
 * @param[in]  config         config pointer that needs to be updated
 *
 */

VOID 
EspiGetIOMmioDecodeInfo (
  UINT32 EspiBase,
  IO_MMIO_DECODE_CONFIG *config
  )
{  
  config->io_mmio_dc_enable = ACPIMMIO32(EspiBase + 0x40);
	config->range0.Value = ACPIMMIO32(EspiBase + 0x44);
	config->range1.Value = ACPIMMIO32(EspiBase + 0x48);
	config->range2.Value = ACPIMMIO32(EspiBase + 0x4C);
	config->mmio_target_range0 = ACPIMMIO32(EspiBase + 0x50);
  config->mmio_target_range1 = ACPIMMIO32(EspiBase + 0x54);
  config->mmio_target_range2 = ACPIMMIO32(EspiBase + 0x58);
  config->mmio_target_range3 = ACPIMMIO32(EspiBase + 0x5C);
  config->mmio_range4.Value = ACPIMMIO32(EspiBase + 0x60);
  config->mmio_range5.Value= ACPIMMIO32(EspiBase + 0x64);
}

/*----------------------------------------------------------------------------------------*/
/**
 * FchEspiCmd_InBandRst - eSPI In Band Reset command
 *      1. Check "command status" of "eSPI downstream transmit Header Register 0"
 *         (FCH::ITF::ESPI::DN_TXHDR_0th[DNCMD_STATUS]) to see if the registers are ready to accept the
 *         software programming.
 *         If this bit is set, stop here. If clear, continue to step2.
 *      2. Program Slave0 configuration register (FCH::ITF::ESPI::SLAVE0_CONFIG[CLK_FREQ_SEL]) to set
 *         eSPI Bus clock to 16.7MHz.
 *      3. Program "eSPI downstream transmit Header Register 0" (FCH::ITF::ESPI::DN_TXHDR_0th[DNCMD_TYPE])
 *         to select "In-Band Reset command".
 *      4. Program "command status" (FCH::ITF::ESPI::DN_TXHDR_0th[DNCMD_STATUS]) to 1 to start sending
 *         down "In-Band Reset".
 *      5. Wait for interrupt line set according to Register Command interrupt mapping setting
 *      6. Software check if "command status" bit (FCH::ITF::ESPI::DN_TXHDR_0th[DNCMD_STATUS]) is cleared.
 *      7. Software can issue new Command (GET_CONFIGURATION/SET_CONFIGURATION) for Slave0.
 *
 * @param[in]  EspiBase       Espi MMIO base
 *
 */
VOID
FchEspiCmd_InBandRst  (
  IN  UINT32     EspiBase
  )
{
  ESPIx00_DN_TXHDR0     EspiReg00;
  ESPIx68_SLAVE0_CONFIG EspiReg68;

  IDS_HDT_CONSOLE (FCH_TRACE," %a Entry\n",__FUNCTION__);
  Wait4stClear (EspiBase);
  EspiReg00.Value = MmioRead32 ((UINTN)(EspiBase + ESPI_DN_TXHDR_0));
  EspiReg68.Value = MmioRead32 ((UINTN)(EspiBase + ESPI_SLAVE0_CONFIG));
  EspiReg68.Field.OperatingFreq = 0;                                     // [27:25] CLK_FREQ_SEL
  EspiReg00.Field.SWCommandType = IN_BAND_RESET;                         // [2:0] DNCMD_TYPE
  MmioWrite32 ((UINTN) (EspiBase + ESPI_SLAVE0_CONFIG), EspiReg68.Value);
  MmioWrite32 ((UINTN) (EspiBase + ESPI_DN_TXHDR_0), EspiReg00.Value);
  EspiReg00.Field.CommandStatus = 1;
  MmioWrite32 ((UINTN) (EspiBase + ESPI_DN_TXHDR_0), EspiReg00.Value);
  Wait4stClear (EspiBase);
}

/*----------------------------------------------------------------------------------------*/
/**
 * FchEspiCmd_GetConfiguration - eSPI Get Configuration command
 *      1. Check "command status" of "eSPI downstream transmit Header Register 0"
 *         (FCH::ITF::ESPI::DN_TXHDR_0th[DNCMD_STATUS]) to see if the registers are ready to accept the
 *         software programming.
 *         If this bit is set, stop here. If clear, continue to step2.
 *      2. Program "eSPI downstream transmit Header Register 0" (FCH::ITF::ESPI::DN_TXHDR_0th[DNCMD_TYPE])
 *         to select "GET_CONFIGURATION".
 *      3. Program register0 address field (FCH::ITF::ESPI::DN_TXHDR_0th [23:8]) for "GET_CONFIGURATION".
 *      4. Program "command status" (FCH::ITF::ESPI::DN_TXHDR_0th[DNCMD_STATUS]) to 1 to start sending down
 *         "GET_CONFIGURATION".
 *      5. Wait for interrupt line set according to Register Command interrupt mapping setting.
 *      6. Software check if "command status" bit (FCH::ITF::ESPI::DN_TXHDR_0th[DNCMD_STATUS]) is cleared.
 *      7. Software Read data from "eSPI downstream Header Register 1" Data register field
 *         (FCH::ITF::ESPI::DN_TXHDR_1 [31:0]) for "GET_CONFIGURATION" command.
 *
 * @param[in]  EspiBase       Espi MMIO base
 * @param[in]  RegAddr        Slave register address
 *
 * @retval    Register Value
 */
UINT32
FchEspiCmd_GetConfiguration  (
  IN  UINT32     EspiBase,
  IN  UINT32     RegAddr
  )
{
  ESPIx00_DN_TXHDR0     EspiReg00;
  ESPIx04_DN_TXHDR1     EspiReg04;

  IDS_HDT_CONSOLE (FCH_TRACE," %a Entry\n",__FUNCTION__);
  Wait4stClear (EspiBase);
  EspiReg00.Value = MmioRead32 ((UINTN)(EspiBase + ESPI_DN_TXHDR_0));
  EspiReg00.Field.SWCommandType = GET_CONFIGURATION;                     // [2:0] DNCMD_TYPE
  EspiReg00.Field.DnCmdHdata0   = (RegAddr & 0xFF00) >> 8;
  EspiReg00.Field.DnCmdHdata1   = RegAddr & 0xFF;
  EspiReg04.Value = 0;
  MmioWrite32 ((UINTN) (EspiBase + ESPI_DN_TXHDR_0), EspiReg00.Value);
  MmioWrite32 ((UINTN) (EspiBase + ESPI_DN_TXHDR_1), EspiReg04.Value);
  EspiReg00.Field.CommandStatus = 1;  
  MmioWrite32 ((UINTN) (EspiBase + ESPI_DN_TXHDR_0), EspiReg00.Value);
  Wait4stClear (EspiBase);

  EspiReg04.Value = MmioRead32 ((UINTN)(EspiBase + ESPI_DN_TXHDR_1));
  return EspiReg04.Value;
}

/*----------------------------------------------------------------------------------------*/
/**
 * FchEspiCmd_SetConfiguration - eSPI Set Configuration command
 *      1. Check "command status" of "eSPI downstream transmit Header Register 0"
 *         (FCH::ITF::ESPI::DN_TXHDR_0th[DNCMD_STATUS]) to see if the registers are ready to accept the
 *         software programming.
 *         If this bit is set, stop here. If clear, continue to step2.
 *      2. Program register0 (FCH::ITF::ESPI::DN_TXHDR_0th[DNCMD_TYPE]) to select "SET_CONFIGURATION".
 *      3. Program register0 address field (FCH::ITF::ESPI::DN_TXHDR_0th [23:8]) for "SET_CONFIGURATION".
 *      4. Software write data to "eSPI downstream transmit Header Register 1" (FCH::ITF::ESPI::DN_TXHDR_1 [31:0])
 *         for "SET_CONFIGURATION" command.
 *      5. Program "command status" (FCH::ITF::ESPI::DN_TXHDR_0th[DNCMD_STATUS]) to 1 to start sending down
 *        "SET_CONFIGURATION".
 *      6. Wait for interrupt line set according to Register Command interrupt mapping setting.
 *      7. Software check "command status" bit (FCH::ITF::ESPI::DN_TXHDR_0th[DNCMD_STATUS]) is cleared.
 *      8. Software program Slave0 configuration register (FCH::ITF::ESPI::SLAVE0_CONFIG [31:0]) and set eSPI bus
 *         same as slave device configuration. If slave0 needs to switch clock, software needs to program Slave0
 *         configuration register to select new Bus clock
 *
 * @param[in]  EspiBase       Espi MMIO base
 * @param[in]  RegAddr        Slave register address
 * @param[in]  Value          Slave register value
 *
 */
VOID
FchEspiCmd_SetConfiguration  (
  IN  UINT32     EspiBase,
  IN  UINT32     RegAddr,
  IN  UINT32     Value
  )
{
  ESPIx00_DN_TXHDR0     EspiReg00;
  ESPIx04_DN_TXHDR1     EspiReg04;

  IDS_HDT_CONSOLE (FCH_TRACE," %a Entry\n",__FUNCTION__);
  Wait4stClear (EspiBase);
  EspiReg00.Value = MmioRead32 ((UINTN)(EspiBase + ESPI_DN_TXHDR_0));
  EspiReg00.Field.SWCommandType = SET_CONFIGURATION;                     // [2:0] DNCMD_TYPE
  EspiReg00.Field.DnCmdHdata0   = (RegAddr & 0xFF00) >> 8;
  EspiReg00.Field.DnCmdHdata1   = RegAddr & 0xFF;
  EspiReg04.Value = Value;
  MmioWrite32 ((UINTN) (EspiBase + ESPI_DN_TXHDR_0), EspiReg00.Value);
  MmioWrite32 ((UINTN) (EspiBase + ESPI_DN_TXHDR_1), EspiReg04.Value);
  EspiReg00.Field.CommandStatus = 1;
  MmioWrite32 ((UINTN) (EspiBase + ESPI_DN_TXHDR_0), EspiReg00.Value);
  Wait4stClear (EspiBase);
}

EFI_STATUS
EFIAPI
FchEspiCmd_PeripheralIORead(
  IN  UINT32     EspiBase,
  IN OUT PERIPH_IO_RW *Message_IO
  )
{
  IO_MMIO_DECODE_CONFIG IO_Config;

  IDS_HDT_CONSOLE (FCH_TRACE," %a Entry\n",__FUNCTION__);
  EspiGetIOMmioDecodeInfo(EspiBase, &IO_Config);  

	/*Check if port address is valid and if the range is enabled*/
	if (Message_IO->port >= IO_Config.range0.Field.base_addr_range0 &&
	    ((UINT16)Message_IO->port + Message_IO->len-1) <=
	    ((UINT16)(IO_Config.range0.Field.base_addr_range0 + IO_Config.range2.Field.io_range0_size))) {
                if (!(IO_Config.io_mmio_dc_enable & IO_DECODE_RANGE0))
                {
                        IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: IO range0 not enabled for port address: 0x%x\n", Message_IO->port);
                        return 1;
                }
        }
        else if(Message_IO->port >= IO_Config.range0.Field.base_addr_range1 &&
		((UINT16)Message_IO->port + Message_IO->len-1) <=
		((UINT16)(IO_Config.range0.Field.base_addr_range1 + IO_Config.range2.Field.io_range1_size))) {
                if (!(IO_Config.io_mmio_dc_enable & IO_DECODE_RANGE1))
                {
                        IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: IO range1 not enabled for port address: 0x%x\n", Message_IO->port);
                        return 1;
                }
        }
        else if(Message_IO->port >= IO_Config.range1.Field.base_addr_range2 &&
		((UINT16)Message_IO->port + Message_IO->len-1) <=
		((UINT16)(IO_Config.range1.Field.base_addr_range2 + IO_Config.range2.Field.io_range2_size))) {
                if (!(IO_Config.io_mmio_dc_enable & IO_DECODE_RANGE2))
                {
                        IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: IO range2 not enabled for port address: 0x%x\n", Message_IO->port);
                        return 1;
                }
        }
        else if(Message_IO->port >= IO_Config.range1.Field.base_addr_range3 &&
		((UINT16)Message_IO->port + Message_IO->len-1) <=
		((UINT16)(IO_Config.range1.Field.base_addr_range3 + IO_Config.range2.Field.io_range3_size))) {
                if (!(IO_Config.io_mmio_dc_enable & IO_DECODE_RANGE3))
                {
                        IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: IO range1 not enabled for port address: 0x%x\n", Message_IO->port);
                        return 1;
                }
        }
        else {
                IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: Port address 0x%x is invalid\n", Message_IO->port);
                return 1;
        }
	switch (Message_IO->len) {
		case 1:
			Message_IO->data.data_b = IoRead8(Message_IO->port);
			break;
		case 2:
			Message_IO->data.data_w = IoRead16(Message_IO->port);
			break;
		case 4:
			Message_IO->data.data_l = IoRead32(Message_IO->port);
			break;
		default:
			IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: %s, Length of IO packet is not valid\n", __func__);
			return 1;
	}

	return 0;
}

EFI_STATUS
EFIAPI
FchEspiCmd_PeripheralIOWrite(
  IN UINT32     EspiBase,
  IN PERIPH_IO_RW *Message_IO
  )
{
  IO_MMIO_DECODE_CONFIG IO_Config;

  IDS_HDT_CONSOLE (FCH_TRACE," %a Entry\n",__FUNCTION__);
  EspiGetIOMmioDecodeInfo(EspiBase, &IO_Config);  

	/*Check if port address is valid and if the range is enabled*/
	if (Message_IO->port >= IO_Config.range0.Field.base_addr_range0 &&
	    ((UINT16)Message_IO->port + Message_IO->len-1) <=
	    ((UINT16)(IO_Config.range0.Field.base_addr_range0 + IO_Config.range2.Field.io_range0_size))) {
                if (!(IO_Config.io_mmio_dc_enable & IO_DECODE_RANGE0))
                {
                        IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: IO range0 not enabled for port address: 0x%x\n", Message_IO->port);
                        return 1;
                }
        }
        else if(Message_IO->port >= IO_Config.range0.Field.base_addr_range1 &&
		((UINT16)Message_IO->port + Message_IO->len-1) <=
		((UINT16)(IO_Config.range0.Field.base_addr_range1 + IO_Config.range2.Field.io_range1_size))) {
                if (!(IO_Config.io_mmio_dc_enable & IO_DECODE_RANGE1))
                {
                        IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: IO range1 not enabled for port address: 0x%x\n", Message_IO->port);
                        return 1;
                }
        }
        else if(Message_IO->port >= IO_Config.range1.Field.base_addr_range2 &&
		((UINT16)Message_IO->port + Message_IO->len-1) <=
		((UINT16)(IO_Config.range1.Field.base_addr_range2 + IO_Config.range2.Field.io_range2_size))) {
                if (!(IO_Config.io_mmio_dc_enable & IO_DECODE_RANGE2))
                {
                        IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: IO range2 not enabled for port address: 0x%x\n", Message_IO->port);
                        return 1;
                }
        }
        else if(Message_IO->port >= IO_Config.range1.Field.base_addr_range3 &&
		((UINT16)Message_IO->port + Message_IO->len-1) <=
		((UINT16)(IO_Config.range1.Field.base_addr_range3 + IO_Config.range2.Field.io_range3_size))) {
                if (!(IO_Config.io_mmio_dc_enable & IO_DECODE_RANGE3))
                {
                        IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: IO range1 not enabled for port address: 0x%x\n", Message_IO->port);
                        return 1;
                }
        }
        else {
                IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: Port address 0x%x is invalid\n", Message_IO->port);
                return 1;
        }

	switch (Message_IO->len) {
		case 1:
			IoWrite8(Message_IO->port, Message_IO->data.data_b);
			break;
		case 2:
			IoWrite16(Message_IO->port, Message_IO->data.data_w);
			break;
		case 4:
			IoWrite32(Message_IO->port, Message_IO->data.data_l);
			break;
		default:
			IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: %s, Length of IO packet is not valid\n", __func__);
			return 1;
	}
	return 0;  
}  

EFI_STATUS
EFIAPI
FchEspiCmd_PeripheralMemWrite(
  IN  UINT32     EspiBase,
  IN  PERIPH_MEM_RW     *MemData
  )
{
  IO_MMIO_DECODE_CONFIG IO_Config;
  
  IDS_HDT_CONSOLE (FCH_TRACE," %a Entry\n",__FUNCTION__);
  EspiGetIOMmioDecodeInfo(EspiBase, &IO_Config);
  
  //Check port address and range  
	if (MemData->addr >= IO_Config.mmio_target_range0 &&
	    ((UINT32)MemData->addr + 3) <=
	    ((UINT32)IO_Config.mmio_target_range0 + IO_Config.mmio_range4.Field.mmio_range0_size)) {
		if (!(IO_Config.io_mmio_dc_enable & MMIO_DECODE_RANGE0))
		{
			IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: MMIO range0 not enabled for address: 0x%x\n", MemData->addr);
			return 1;
		}  
  } else if (MemData->addr >= IO_Config.mmio_target_range1 &&
		 ((UINT32)MemData->addr + 3) <=
		 ((UINT32)IO_Config.mmio_target_range1 + IO_Config.mmio_range4.Field.mmio_range1_size)) {
		if (!(IO_Config.io_mmio_dc_enable & MMIO_DECODE_RANGE1))
		{
			IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: MMIO range1 not enabled for address: 0x%x\n", MemData->addr);
			return 1;
		}
	} else if (MemData->addr >= IO_Config.mmio_target_range2 &&
		 ((UINT32)MemData->addr + 3) <=
		 ((UINT32)IO_Config.mmio_target_range2 + IO_Config.mmio_range5.Field.mmio_range2_size)) {
		if (!(IO_Config.io_mmio_dc_enable & MMIO_DECODE_RANGE2))
		{
			IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: IO range2 not enabled for address: 0x%x\n", MemData->addr);
			return 1;
		}
	} else if (MemData->addr >= IO_Config.mmio_target_range3 &&
		 ((UINT32)MemData->addr + 3) <=
		 ((UINT32)IO_Config.mmio_target_range3 + IO_Config.mmio_range5.Field.mmio_range3_size)) {
		if (!(IO_Config.io_mmio_dc_enable & MMIO_DECODE_RANGE3))
		{
			IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: IO range1 not enabled for address: 0x%x\n", MemData->addr);
			return 1;
		}
	} else {
		IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: address 0x%x is invalid\n", MemData->addr);
		return 1;
	}

  MmioWrite32(MemData->addr,MemData->data);

	return 0;
}

EFI_STATUS
EFIAPI
FchEspiCmd_PeripheralMemRead(
  IN  UINT32     EspiBase,
  IN OUT  PERIPH_MEM_RW     *MemData
  )
{
  IO_MMIO_DECODE_CONFIG IO_Config;

  IDS_HDT_CONSOLE (FCH_TRACE," %a Entry\n",__FUNCTION__);
  EspiGetIOMmioDecodeInfo(EspiBase, &IO_Config);
  
  //Check port address and range  
	if (MemData->addr >= IO_Config.mmio_target_range0 &&
	    ((UINT32)MemData->addr + 3) <=
	    ((UINT32)IO_Config.mmio_target_range0 + IO_Config.mmio_range4.Field.mmio_range0_size)) {
		if (!(IO_Config.io_mmio_dc_enable & MMIO_DECODE_RANGE0))
		{
			IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: MMIO range0 not enabled for address: 0x%x\n", MemData->addr);
			return 1;
		}  
  } else if (MemData->addr >= IO_Config.mmio_target_range1 &&
		 ((UINT32)MemData->addr + 3) <=
		 ((UINT32)IO_Config.mmio_target_range1 + IO_Config.mmio_range4.Field.mmio_range1_size)) {
		if (!(IO_Config.io_mmio_dc_enable & MMIO_DECODE_RANGE1))
		{
			IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: MMIO range1 not enabled for address: 0x%x\n", MemData->addr);
			return 1;
		}
	} else if (MemData->addr >= IO_Config.mmio_target_range2 &&
		 ((UINT32)MemData->addr + 3) <=
		 ((UINT32)IO_Config.mmio_target_range2 + IO_Config.mmio_range5.Field.mmio_range2_size)) {
		if (!(IO_Config.io_mmio_dc_enable & MMIO_DECODE_RANGE2))
		{
			IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: IO range2 not enabled for address: 0x%x\n", MemData->addr);
			return 1;
		}
	} else if (MemData->addr >= IO_Config.mmio_target_range3 &&
		 ((UINT32)MemData->addr + 3) <=
		 ((UINT32)IO_Config.mmio_target_range3 + IO_Config.mmio_range5.Field.mmio_range3_size)) {
		if (!(IO_Config.io_mmio_dc_enable & MMIO_DECODE_RANGE3))
		{
			IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: IO range1 not enabled for address: 0x%x\n", MemData->addr);
			return 1;
		}
	} else {
		IDS_HDT_CONSOLE (FCH_TRACE,"AMD_ESPI: address 0x%x is invalid\n", MemData->addr);
		return 1;
	}

  MemData->data = MmioRead32(MemData->addr);

	return 0;
}