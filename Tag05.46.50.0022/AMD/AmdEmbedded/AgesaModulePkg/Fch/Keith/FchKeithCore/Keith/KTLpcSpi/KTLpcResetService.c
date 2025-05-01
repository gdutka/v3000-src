/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"

#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTLPCSPI_KTLPCRESETSERVICE_FILECODE
#define SPI_BASE 0xFEC10000ul

/**
 * FchInitKeithResetLpcPciTable - Lpc (Spi) device registers
 * initial during the power on stage.
 *
 *
 *
 *
 */
REG8_MASK FchInitKeithResetLpcPciTable[] =
{
  //
  // LPC Device (Bus 0, Dev 20, Func 3)
  //
  {0x00, LPC_BUS_DEV_FUN, 0},


  {0xFF, 0xFF, 0xFF},
};

/**
 * FchInitResetLpcProgram - Config Lpc controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetLpcProgram (
  IN       VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //HW default RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REG6C, AccessWidth32, 0xFFFFFF00, 0, StdHeader);

  ProgramPciByteTable (
    (REG8_MASK*) (&FchInitKeithResetLpcPciTable[0]),
    sizeof (FchInitKeithResetLpcPciTable) / sizeof (REG8_MASK),
    StdHeader
    );

  if ( LocalCfgPtr->Spi.LpcClk0 ) {
    RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGD0 + 1, AccessWidth8, 0xDF, 0x20, StdHeader);
  } else {
    RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGD0 + 1, AccessWidth8, 0xDF, 0, StdHeader);
  }
  if ( LocalCfgPtr->Spi.LpcClk1 ) {
    RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGD0 + 1, AccessWidth8, 0xBF, 0x40, StdHeader);
  } else {
    RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGD0 + 1, AccessWidth8, 0xBF, 0, StdHeader);
  }
}

/**
 * FchInitResetSpi - Config Spi controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetSpi (
  IN       VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  AMD_CONFIG_PARAMS         *StdHeader;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  AGESA_TESTPOINT (TpFchInitResetSpi, NULL);
  //
  // Set Spi ROM Base Address
  //
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGA0, AccessWidth32, 0x001F, SPI_BASE, StdHeader);

  RwMem (SPI_BASE + FCH_SPI_MMIO_REG00, AccessWidth32, 0xFFFFFFFF, (BIT19 + BIT24 + BIT25 + BIT26));
  RwMem (SPI_BASE + FCH_SPI_MMIO_REG0C, AccessWidth32, 0xFFC0FFFF, 0 );

  //Set SPI100 Enable
  RwMem (SPI_BASE + FCH_SPI_MMIO_REG20, AccessWidth8, 0xFE, (UINT8) ((LocalCfgPtr->SPI100_Enable) << 0));

  //
  //  Spi Pad Initial
  //UINT32                SPI100_RX_Timing_Config_Register_38;                 ///< SPI100_RX_Timing_Config_Register_38
  //UINT16                SPI100_RX_Timing_Config_Register_3C;                 ///< SPI100_RX_Timing_Config_Register_3C
  //UINT8                 SpiProtectEn0_1d_34;                                 ///

  //RwMem (SPI_BASE + FCH_SPI_MMIO_REG38, AccessWidth32, 0, LocalCfgPtr->SPI100_RX_Timing_Config_Register_38);
  //RwMem (SPI_BASE + FCH_SPI_MMIO_REG3C, AccessWidth16, 0, LocalCfgPtr->SPI100_RX_Timing_Config_Register_3C);
  //RwMem (SPI_BASE + FCH_SPI_MMIO_REG1D, AccessWidth8, 0xE7, (UINT8) ((LocalCfgPtr->SpiProtectEn0_1d_34) << 3));

  //
  //  Spi Mode Initial
  //
  if (LocalCfgPtr->SpiSpeed) {
    RwMem (
      SPI_BASE + FCH_SPI_MMIO_REG22,
      AccessWidth32,
      ~((UINT32) (0xF << 12)),
      ((LocalCfgPtr->SpiSpeed - 1 ) << 12)
    );
  }

  if (LocalCfgPtr->WriteSpeed) {
    RwMem (
      SPI_BASE + FCH_SPI_MMIO_REG20,
      AccessWidth32,
      ~((UINT32) (0xF << 20)),
      ((LocalCfgPtr->WriteSpeed - 1 ) << 20)
    );
  }

  if (LocalCfgPtr->SpiTpmSpeed) {
    RwMem (
      SPI_BASE + FCH_SPI_MMIO_REG20,
      AccessWidth32,
      ~((UINT32) (0xF << 16)),
      ((LocalCfgPtr->SpiTpmSpeed - 1 ) << 16)
    );
  }

  if (LocalCfgPtr->SpiClkEarlier) {
    RwMem (SPI_BASE + FCH_SPI_MMIO_REG00, AccessWidth32, ~(UINT32)(BIT13), BIT13);
  }

  RwMem (SPI_BASE + FCH_SPI_MMIO_REG1C, AccessWidth32, ~(UINT32) (BIT10), ((LocalCfgPtr->BurstWrite) << 10));

  //ENH433556:Enabling SPI Performance enhancement
  RwMem (SPI_BASE + FCH_SPI_MMIO_REG2C, AccessWidth32, ~(UINT32) (BIT14), (1 << 14));

  //ENH457313:  SPI Mem 0x2C[15] needs to be cleared in Carrizo
  RwMem (SPI_BASE + FCH_SPI_MMIO_REG2C, AccessWidth32, ~(UINT32) (BIT15), 0);

  // Enabling SPI ROM Prefetch
  // Set LPC cfg 0xBA bit 8
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGBA, AccessWidth16, 0xFFFF, BIT8, StdHeader);

  // Enable SPI Prefetch for USB, set LPC cfg 0xBA bit 7 to 1.
  //Keith RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGBA, AccessWidth16, 0xFFFF, BIT7, StdHeader);

  // PLAT-55140 PLAT-20087
  // PrefetchEnSPIFromHost (LPC PCI Cfg 0xB8[24]=1) --> leave it as H/W default, no need to touch
  // HostHitSoonEn (SPI Mem 0x2C[13]=0) --> leave it as H/W default, no need to touch
  // HostWillHitEn (SPI Mem 0x2C[12]=1) --> leave it as H/W default, no need to touch
  // LPC PCI Configuration Register 0xDC[0]  (default=0) is used as chicken bit.
  // Software need to set this bit to 1 before remove the software patch to verify the ECO.
  RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGDC, AccessWidth16, 0xFFFF, BIT0, StdHeader);
}
