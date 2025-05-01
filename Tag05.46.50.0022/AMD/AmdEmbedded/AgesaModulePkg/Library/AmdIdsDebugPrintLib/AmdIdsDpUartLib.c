/*****************************************************************************
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

 /*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AMD.h"
#include <Library/AmdBaseLib.h>
#include <Library/BaseLib.h>
#include <Library/FchBaseLib.h>
#include <Library/AmdIdsDebugPrintLib.h>
#include <Filecode.h>

#define FILECODE LIBRARY_AMDIDSDEBUGPRINTLIB_AMDIDSDPUARTLIB_FILECODE

#define IDS_SERIAL_PORT (PcdGet64(PcdAmdIdsDebugPrintSerialPort))
#define PCIE_BASE_ADDRESS (PcdGet64(PcdPciExpressBaseAddress))

#define IDS_SERIAL_PORT_MSR \
(IDS_SERIAL_PORT > 0xFFFF ? \
(IDS_SERIAL_PORT + 6*4) : \
(IDS_SERIAL_PORT + 6))

#define IDS_SERIAL_PORT_MSR_CTS      (BIT4)

#define IDS_SERIAL_PORT_MSR_READ(Value) \
(IDS_SERIAL_PORT > 0xFFFF ? \
LibAmdMemRead(AccessWidth8, (UINT64) IDS_SERIAL_PORT_MSR, &Value, NULL) : \
LibAmdIoRead(AccessWidth8, (UINT16) IDS_SERIAL_PORT_MSR, &Value, NULL))

#define NB_SMN_INDEX_2_PCI_ADDR     0x000000B8ul  ///< PCI Addr of NB_SMN_INDEX_2
#define NB_SMN_DATA_2_PCI_ADDR      0x000000BCul  ///< PCI Addr of NB_SMN_DATA_2

#define MP0_C2PMSG_97               0x03810A84ul
#define MP0_C2PMSG_98               0x03810A88ul

#define UART_IOMUX_0x135            0xFED80D87ul
#define UART_IOMUX_0x136            0xFED80D88ul
#define UART_IOMUX_0x137            0xFED80D89ul
#define UART_IOMUX_0x138            0xFED80D8Aul
#define UART_IOMUX_0x139            0xFED80D8Bul
#define UART_IOMUX_0x140            0xFED80D8Cul
#define UART_IOMUX_0x141            0xFED80D8Dul
#define UART_IOMUX_0x142            0xFED80D8Eul
#define UART_IOMUX_0x143            0xFED80D8Ful
#define UART_IOMUX_0x144            0xFED80D90ul
#define UART_IOMUX_0x153            0xFED80D99ul
#define UART_IOMUX_0x154            0xFED80D9Aul
#define UART_IOMUX_0x155            0xFED80D9Bul
#define UART_IOMUX_0x156            0xFED80D9Cul
#define UART_IOMUX_0x157            0xFED80D9Dul
#define UART_CVIP_IOMUX_0x350       0x02D029DEul
#define UART_CVIP_IOMUX_0x351       0x02D029DFul

#define FCH_AOACx40_D3_CONTROL      0xFED81E40ul
#define FCH_AOACx56_UART0           11 * 2        // UART0
#define FCH_AOACx58_UART1           12 * 2        // UART1
#define FCH_AOACx60_UART2           16 * 2        // UART2
#define FCH_AOACx60_UART3           26 * 2        // UART3
#define FCH_AOACx60_UART4           20 * 2        // UART4
#define FCH_AOACx62_AMBA            0x22          // AMBA
#define AOAC_PWR_ON_DEV             BIT3          // PwrOnDev

#define R_UART_RXBUF                0             // LCR_DLAB = 0
#define R_UART_TXBUF                0             // LCR_DLAB = 0
#define R_UART_BAUD_LOW             0             // LCR_DLAB = 1
#define R_UART_BAUD_HIGH            1             // LCR_DLAB = 1
#define R_UART_IER                  1             // LCR_DLAB = 0
#define R_UART_FCR                  2
#define   B_UART_FCR_FIFOE          BIT0
#define   B_UART_FCR_FIFO64         BIT5
#define R_UART_LCR                  3
#define   B_UART_LCR_DLAB           BIT7
#define R_UART_MCR                  4
#define   B_UART_MCR_DTRC           BIT0
#define   B_UART_MCR_RTS            BIT1
#define   B_UART_MCR_OUT1           BIT2
#define   B_UART_MCR_OUT2           BIT3
#define   B_UART_MCR_LB             BIT4
#define R_UART_LSR                  5
#define   B_UART_LSR_RXRDY          BIT0
#define   B_UART_LSR_TXRDY          BIT5
#define   B_UART_LSR_TEMT           BIT6
#define R_UART_MSR                  6
#define   B_UART_MSR_CTS            BIT4
#define   B_UART_MSR_DSR            BIT5
#define   B_UART_MSR_RI             BIT6
#define   B_UART_MSR_DCD            BIT7
#define FCH_UART_USR                0x7C          // UART Register 0x7C
#define   RSVD_BUSY                 BIT0          // 0 = DW_apb_uart is idle or inactive. 1 = DW_apb_uart is busy.

CHAR8   *InitUartString             = " --- Init Fch Uart --- ";
CHAR8   *UartIsReadyString          = " --- Fch Uart is ready --- ";

/**
 *      Light version SMN read with less depx
 *
 *
 *  @param[in]    Address   = SMN address
 *  @param[in]    OpFlag    = 1: byte, 2: word, 3:dword
 *  @param[out]   *ValuePtr = Value to be read from the SMN address
 *
 **/
VOID
LightSmnRegRead (
  UINT32    Address,
  UINT8     OpFlag,
  VOID      *ValuePtr
  )
{
  UINT32    OriginalIndex;
  UINT32    WriteData;
  UINT64    MmioBaseAddr;

  MmioBaseAddr = PCIE_BASE_ADDRESS + NB_SMN_INDEX_2_PCI_ADDR;
  LibAmdMemRead (AccessWidth32, MmioBaseAddr, &OriginalIndex, NULL);
  WriteData = Address & 0xFFFFFFFC;
  LibAmdMemWrite (AccessWidth32, MmioBaseAddr, &WriteData, NULL);
  MmioBaseAddr = PCIE_BASE_ADDRESS + NB_SMN_DATA_2_PCI_ADDR;

  switch ( OpFlag ) {
    case AccessWidth8:
      LibAmdMemRead (AccessWidth8, MmioBaseAddr + (UINT64)(Address & 0x3), (UINT8*)ValuePtr, NULL);
      break;
    case AccessWidth16:
      LibAmdMemRead (AccessWidth16, MmioBaseAddr + (UINT64)(Address & 0x2), (UINT16*)ValuePtr, NULL);
      break;
    case AccessWidth32:
      LibAmdMemRead (AccessWidth32, MmioBaseAddr, (UINT32*)ValuePtr, NULL);
      break;
    default:
      //ASSERT (FALSE);
      break;
  }

  MmioBaseAddr = PCIE_BASE_ADDRESS + NB_SMN_INDEX_2_PCI_ADDR;
  LibAmdMemWrite (AccessWidth32, MmioBaseAddr, &OriginalIndex, NULL);
  return;
}

/**
 *      Light version SMN write with less depx
 *
 *
 *  @param[in]    Address   = SMN address
 *  @param[in]    OpFlag    = 1: byte, 2: word, 3:dword
 *  @param[in]    *ValuePtr = Value to be written to the SMN address
 *
 **/
VOID
LightSmnRegWrite (
  UINT32    Address,
  UINT8     OpFlag,
  VOID      *ValuePtr
  )
{
  UINT32    OriginalIndex;
  UINT32    WriteData;
  UINT64    MmioBaseAddr;

  MmioBaseAddr = PCIE_BASE_ADDRESS + NB_SMN_INDEX_2_PCI_ADDR;
  LibAmdMemRead (AccessWidth32, MmioBaseAddr, &OriginalIndex, NULL);
  WriteData = Address & 0xFFFFFFFC;
  LibAmdMemWrite (AccessWidth32, MmioBaseAddr, &WriteData, NULL);
  MmioBaseAddr = PCIE_BASE_ADDRESS + NB_SMN_DATA_2_PCI_ADDR;

  switch ( OpFlag ) {
    case AccessWidth8:
      LibAmdMemWrite (AccessWidth8, MmioBaseAddr + (UINT64)(Address & 0x3), (UINT8*)ValuePtr, NULL);
      break;
    case AccessWidth16:
      LibAmdMemWrite (AccessWidth16, MmioBaseAddr + (UINT64)(Address & 0x2), (UINT16*)ValuePtr, NULL);
      break;
    case AccessWidth32:
      LibAmdMemWrite (AccessWidth32, MmioBaseAddr, (UINT32*)ValuePtr, NULL);
      break;
    default:
      //ASSERT (FALSE);
      break;
  }

  MmioBaseAddr = PCIE_BASE_ADDRESS + NB_SMN_INDEX_2_PCI_ADDR;
  LibAmdMemWrite (AccessWidth32, MmioBaseAddr, &OriginalIndex, NULL);
  return;
}

/**
 *      Detect the system is emulation or real platform.
 *
 *
 *  @retval       TRUE    The system is emulation
 *  @retval       FALSE   The system is real platform
 *
 **/
BOOLEAN
AmdIdsEmulationAutoDetect (
  VOID
  )
{
  UINT32    Data;
  if (PcdGetBool (PcdAmdIdsDebugPrintEmulationAutoDetect)) {
    LightSmnRegRead (MP0_C2PMSG_97, AccessWidth32, &Data);
    if (Data & 0x0F) {
      // non-0 = Emulation system
      return TRUE;
    } else {
      LightSmnRegRead (MP0_C2PMSG_98, AccessWidth32, &Data);
      // 0: Real HW system
      if (Data & BIT16) {
        // Bit[16] = 1 = Emulation system
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 *      Power up SOC UART and setup UART signal IOMUX function.
 *
 **/
VOID
AmdIdsInitUartIomux (
  VOID
  )
{
  UINT32         RegEax;
  UINT32         Revision;
  UINT8          DataByte;

  //
  // Initialize for FCH UART
  //
  AsmCpuid (0x80000001, &RegEax, NULL, NULL, NULL);
  Revision = RegEax & 0xFF;
  RegEax = (RegEax >> 16 ) & 0x0FFF;

  if (PcdGet64 (PcdAmdIdsDebugPrintSerialPort) == 0xFEDC9000) {
    // AOAC 'PwrOnDev' bit of AMBA & UART0
    LibAmdMemRead (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx62_AMBA), &DataByte, NULL);
    if ((DataByte & 0x03) != 0x03) {
      DataByte |= AOAC_PWR_ON_DEV;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx62_AMBA), &DataByte, NULL);
    }
    LibAmdMemRead (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx56_UART0), &DataByte, NULL);
    if ((DataByte & 0x03) != 0x00) {
      DataByte &= 0xFC;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx56_UART0), &DataByte, NULL);
      DataByte |= AOAC_PWR_ON_DEV;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx56_UART0), &DataByte, NULL);
    }
    // IOMUX switch to CTS_L, RTS_L, INTR, TXD & RXD
    if ((RegEax < 0x86) || (RegEax == 0x87) || (RegEax == 0xA0) || (RegEax == 0xA2)) {
      // For ZP(80), RV/PCO(81), RV2(82), SSP(83), MTS(87), GN/CGL(A0), VMR(A2)
      DataByte = 0x00;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x135, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x136, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x137, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x138, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x139, &DataByte, NULL);
    } else if ((RegEax == 0x86) && (Revision == 0x00)) {
      // For RN(86) Rev:A0
      DataByte = 0x00;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x140, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x141, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x144, &DataByte, NULL);
      DataByte = 0x01;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x142, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x143, &DataByte, NULL);
    } else if (((RegEax == 0x86) && (Revision >= 0x01)) || (RegEax == 0xA4) || (RegEax == 0xA5)) {
      // For RN(86) Rev:A1, RMB(A4), CZN(A5)
      DataByte = 0x01;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x140, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x141, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x142, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x143, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x144, &DataByte, NULL);
    } else if (RegEax == 0x89) {
      // For VN/FF3(89)
      DataByte = 1;
      LightSmnRegWrite (UART_CVIP_IOMUX_0x350, 1, &DataByte);   // 1 = AccessWidth8
      LightSmnRegWrite (UART_CVIP_IOMUX_0x351, 1, &DataByte);   // 1 = AccessWidth8
      DataByte = 0x01;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x141, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x143, &DataByte, NULL);
      DataByte = 0x02;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x144, &DataByte, NULL);
    }
  } else if (PcdGet64 (PcdAmdIdsDebugPrintSerialPort) == 0xFEDCA000) {
    // AOAC 'PwrOnDev' bit of AMBA & UART1
    LibAmdMemRead (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx62_AMBA), &DataByte, NULL);
    if ((DataByte & 0x03) != 0x03) {
      DataByte |= AOAC_PWR_ON_DEV;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx62_AMBA), &DataByte, NULL);
    }
    LibAmdMemRead (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx58_UART1), &DataByte, NULL);
    if ((DataByte & 0x03) != 0x00) {
      DataByte &= 0xFC;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx58_UART1), &DataByte, NULL);
      DataByte |= AOAC_PWR_ON_DEV;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx58_UART1), &DataByte, NULL);
    }
    // IOMUX switch to CTS_L, RTS_L, INTR, TXD & RXD
    if ((RegEax < 0x86) || (RegEax == 0x87) || (RegEax == 0xA0) || (RegEax == 0xA2)) {
      // For ZP(80), RV/PCO(81), RV2(82), SSP(83), MTS(87), GN/CGL(A0), VMR(A2)
      DataByte = 0x00;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x140, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x141, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x142, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x143, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x144, &DataByte, NULL);
    } else if ((RegEax == 0x86) && (Revision == 0x00)) {
      // For RN(86) Rev:A0
      DataByte = 0x02;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x140, &DataByte, NULL);
      DataByte = 0x00;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x142, &DataByte, NULL);
    } else if (((RegEax == 0x86) && (Revision >= 0x01)) || (RegEax == 0xA4) || (RegEax == 0xA5)) {
      // For RN(86) Rev:A1, RMB(A4), CZN(A5)
      DataByte = 0x02;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x140, &DataByte, NULL);
      DataByte = 0x02;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x142, &DataByte, NULL);
    } else if (RegEax == 0x89) {
      // For VN/FF3(89)
      DataByte = 2;
      LightSmnRegWrite (UART_CVIP_IOMUX_0x350, 1, &DataByte);   // 1 = AccessWidth8
      LightSmnRegWrite (UART_CVIP_IOMUX_0x351, 1, &DataByte);   // 1 = AccessWidth8
    }
  } else if (PcdGet64 (PcdAmdIdsDebugPrintSerialPort) == 0xFEDCE000) {
    // AOAC 'PwrOnDev' bit of AMBA & UART2
    LibAmdMemRead (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx62_AMBA), &DataByte, NULL);
    if ((DataByte & 0x03) != 0x03) {
      DataByte |= AOAC_PWR_ON_DEV;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx62_AMBA), &DataByte, NULL);
    }
    LibAmdMemRead (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx60_UART2), &DataByte, NULL);
    if ((DataByte & 0x03) != 0x00) {
      DataByte &= 0xFC;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx60_UART2), &DataByte, NULL);
      DataByte |= AOAC_PWR_ON_DEV;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx60_UART2), &DataByte, NULL);
    }
    // IOMUX switch to CTS_L, RTS_L, INTR, TXD & RXD
    if ((RegEax < 0x86) || (RegEax == 0x87) || (RegEax == 0xA0) || (RegEax == 0xA2)) {
      // For ZP(80), RV/PCO(81), RV2(82), SSP(83), MTS(87), GN/CGL(A0), VMR(A2)
      DataByte = 0x01;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x135, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x137, &DataByte, NULL);
    } else if ((RegEax == 0x86) || (RegEax == 0xA5)) {
      // For RN(86), CZN(A5)
    } else if ((RegEax == 0x89) || (RegEax == 0xA4)) {
      // For VN/FF3(89), RMB(A4)
      DataByte = 0x01;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x135, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x136, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x137, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x138, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x139, &DataByte, NULL);
    }
  } else if (PcdGet64 (PcdAmdIdsDebugPrintSerialPort) == 0xFEDCF000) {
    // AOAC 'PwrOnDev' bit of AMBA & UART3
    LibAmdMemRead (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx62_AMBA), &DataByte, NULL);
    if ((DataByte & 0x03) != 0x03) {
      DataByte |= AOAC_PWR_ON_DEV;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx62_AMBA), &DataByte, NULL);
    }
    LibAmdMemRead (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx60_UART3), &DataByte, NULL);
    if ((DataByte & 0x03) != 0x00) {
      DataByte &= 0xFC;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx60_UART3), &DataByte, NULL);
      DataByte |= AOAC_PWR_ON_DEV;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx60_UART3), &DataByte, NULL);
    }
    // IOMUX switch to CTS_L, RTS_L, INTR, TXD & RXD
    if ((RegEax < 0x86) || (RegEax == 0x87) || (RegEax == 0xA0) || (RegEax == 0xA2)) {
      // For ZP(80), RV/PCO(81), RV2(82), SSP(83), MTS(87), GN/CGL(A0), VMR(A2)
      DataByte = 0x01;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x140, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x142, &DataByte, NULL);
    } else if ((RegEax == 0x86) || (RegEax == 0xA5)) {
      // For RN(86), CZN(A5)
    } else if ((RegEax == 0x89) || (RegEax == 0xA4)) {
      // For VN/FF3(89), RMB(A4)
      DataByte = 0x02;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x135, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x137, &DataByte, NULL);
    }
  } else if (PcdGet64 (PcdAmdIdsDebugPrintSerialPort) == 0xFEDD1000) {
    // AOAC 'PwrOnDev' bit of AMBA & UART4
    LibAmdMemRead (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx62_AMBA), &DataByte, NULL);
    if ((DataByte & 0x03) != 0x03) {
      DataByte |= AOAC_PWR_ON_DEV;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx62_AMBA), &DataByte, NULL);
    }
    LibAmdMemRead (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx60_UART4), &DataByte, NULL);
    if ((DataByte & 0x03) != 0x00) {
      DataByte &= 0xFC;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx60_UART4), &DataByte, NULL);
      DataByte |= AOAC_PWR_ON_DEV;
      LibAmdMemWrite (AccessWidth8, (UINT64) (FCH_AOACx40_D3_CONTROL + FCH_AOACx60_UART4), &DataByte, NULL);
    }
    // IOMUX switch to CTS_L, RTS_L, INTR, TXD & RXD
    if ((RegEax < 0x86) || (RegEax == 0x87) || (RegEax == 0xA0) || (RegEax == 0xA2)) {
      // For ZP(80), RV/PCO(81), RV2(82), SSP(83), MTS(87), GN/CGL(A0), VMR(A2)
    } else if ((RegEax == 0x86) || (RegEax == 0xA5)) {
      // For RN(86), CZN(A5)
    } else if ((RegEax == 0x89) || (RegEax == 0xA4)) {
      // For VN/FF3(89), RMB(A4)
      DataByte = 0x01;
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x153, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x154, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x155, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x156, &DataByte, NULL);
      LibAmdMemWrite (AccessWidth8, (UINT64) UART_IOMUX_0x157, &DataByte, NULL);
    }
  }

  //
  // Wait for the serial port to be ready.
  // Verify that both the transmit FIFO and the shift register are empty.
  //
  do {
    LibAmdMemRead (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + FCH_UART_USR, &DataByte, NULL);
  } while ((DataByte & RSVD_BUSY) == RSVD_BUSY);

  do {
    LibAmdMemRead (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_LSR * 4), &DataByte, NULL);
  } while ((DataByte & (B_UART_LSR_TEMT | B_UART_LSR_TXRDY)) != (B_UART_LSR_TEMT | B_UART_LSR_TXRDY));

}

/**
 *      Check SOC UART Line Control and Divisor setting.
 *
 *
 *  @retval       TRUE    Line Control and Divisor setting is correct.
 *  @retval       FALSE   Line Control and Divisor setting is not correct.
 *
 **/
BOOLEAN
AmdIdsCheckUartDivisor (
  VOID
  )
{
  UINT32         Divisor;
  UINT32         CurrentDivisor;
  UINT8          DataByte;

  //
  // Calculate divisor for baud generator
  //    Ref_Clk_Rate / Baud_Rate / 16
  //
  LibAmdMemRead (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_LCR * 4), &DataByte, NULL);
  if ((DataByte & 0x3F) != (PcdGet8 (PcdAmdIdsUartLineControl) & 0x3F)) {
    return FALSE;
  }

  Divisor = PcdGet32 (PcdAmdIdsUartClock) / (PcdGet32 (PcdAmdIdsUartBaudRate) * 16);
  LibAmdMemRead (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_LCR * 4), &DataByte, NULL);
  DataByte |= B_UART_LCR_DLAB;
  LibAmdMemWrite (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_LCR * 4), &DataByte, NULL);
  LibAmdMemRead (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_BAUD_HIGH * 4), &DataByte, NULL);
  CurrentDivisor = (UINT32) DataByte << 8;
  LibAmdMemRead (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_BAUD_LOW * 4), &DataByte, NULL);
  CurrentDivisor |= (UINT32) DataByte;
  LibAmdMemRead (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_LCR * 4), &DataByte, NULL);
  DataByte &= ~B_UART_LCR_DLAB;
  LibAmdMemWrite (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_LCR * 4), &DataByte, NULL);

  if (CurrentDivisor != Divisor) {
    return FALSE;
  }

  AmdIdsDpSerialPrint (UartIsReadyString, 27);
  return TRUE;
}

/**
 *      Initialize SOC UART Line Control and Divisor setting.
 *
 **/
VOID
AmdIdsInitUartDivisor (
  VOID
  )
{
  UINT32         Divisor;
  UINT8          DataByte;

  //
  // Configure baud rate
  //
  Divisor = PcdGet32 (PcdAmdIdsUartClock) / (PcdGet32 (PcdAmdIdsUartBaudRate) * 16);
  DataByte = B_UART_LCR_DLAB;
  LibAmdMemWrite (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_LCR * 4), &DataByte, NULL);
  DataByte = (UINT8) (Divisor >> 8);
  LibAmdMemWrite (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_BAUD_HIGH * 4), &DataByte, NULL);
  DataByte = (UINT8) (Divisor & 0xFF);
  LibAmdMemWrite (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_BAUD_LOW * 4), &DataByte, NULL);

  //
  // Clear DLAB and configure Data Bits, Parity, and Stop Bits.
  // Strip reserved bits from PcdAmdIdsUartLineControl
  //
  DataByte = PcdGet8 (PcdAmdIdsUartLineControl) & 0x3F;
  LibAmdMemWrite (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_LCR * 4), &DataByte, NULL);

  //
  // Enable and reset FIFOs
  // Strip reserved bits from PcdAmdIdsUartFifoControl
  //
  DataByte = 0x00;
  LibAmdMemWrite (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_FCR * 4), &DataByte, NULL);
  DataByte = PcdGet8 (PcdAmdIdsUartFifoControl) & (B_UART_FCR_FIFOE | B_UART_FCR_FIFO64);
  LibAmdMemWrite (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_FCR * 4), &DataByte, NULL);

  //
  // Set FIFO Polled Mode by clearing IER after setting FCR
  //
  DataByte = 0x00;
  LibAmdMemWrite (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_IER * 4), &DataByte, NULL);

  //
  // Put Modem Control Register(MCR) into internal LoopBack mode, and output OUT2/OUT1/RTS/DTR to high.
  // Verify Modem Status Register(MSR), and make sure DCD/RI/DSR/CTS are high in internal LoopBack mode.
  //
  DataByte = B_UART_MCR_LB | B_UART_MCR_OUT2 | B_UART_MCR_OUT1 | B_UART_MCR_RTS | B_UART_MCR_DTRC;
  LibAmdMemWrite (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_MCR * 4), &DataByte, NULL);
  do {
    LibAmdMemRead (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_MSR * 4), &DataByte, NULL);
  } while ((DataByte & (B_UART_MSR_CTS | B_UART_MSR_DSR | B_UART_MSR_RI | B_UART_MSR_DCD)) != (B_UART_MSR_CTS | B_UART_MSR_DSR | B_UART_MSR_RI | B_UART_MSR_DCD));

  //
  // Put Modem Control Register(MCR) into its reset state of 0x00.
  //
  DataByte = 0x00;
  LibAmdMemWrite (AccessWidth8, PcdGet64 (PcdAmdIdsDebugPrintSerialPort) + (R_UART_MCR * 4), &DataByte, NULL);

  AmdIdsDpSerialPrint (InitUartString, 23);

}

/**
 *      Check Serial Port MSR register CTS bit. (Hardware Flow Control)
 *
 *
 *  @retval       TRUE    CTS bit is high.
 *  @retval       FALSE   CTS bit is low.
 *
 **/
BOOLEAN
AmdIdsDpSerialWritable (
  VOID
  )
{
  UINT8 Value;

  if (PcdGetBool (PcdAmdIdsDebugPrintSerialPortHardwareFlowControl)) {
    IDS_SERIAL_PORT_MSR_READ (Value);
    // Wait for CTS to be set.
    if ((Value & IDS_SERIAL_PORT_MSR_CTS) != IDS_SERIAL_PORT_MSR_CTS) {
      return FALSE;
    }
  }
  return TRUE;
}

