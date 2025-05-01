/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#define GPIO_IOAPIC_BAR       0xFEC30000

#define XHCI_REG_ADDR_L_OFFSET        0x3000
#define XHCI_REG_ADDR_H_OFFSET        0x3001
#define XHCI_REG_SEG_OFFSET           0x3002
#define XHCI_REG_WRITE_DATA_OFFSET    0x3004
#define XHCI_REG_READ_DATA_OFFSET     0x3008
#define XHCI_PCIE_WRITE_CYCLE_OFFSET  0x3009

#define XHCI_REG_PCIE_HW_RW   0x15087

#define IOHC_NB_SMN_INDEX_2_BIOS  0x00B8
#define IOHC_NB_SMN_DATA_2_BIOS   0x00BC

#define PCIE_CFG_RAM_ADDR0_REG          0xE2
#define PCIE_CFG_RESET_REG              0xEF
#define PCIE_MEM_RAM_WDATA_OFFSET       0x3010
#define PCIE_MEM_RAM_RDATA_OFFSET       0x3018

#define PROM21_MAX_GPIO_PIN_NUMBER      23

#define PROM21_DSP_DISABLE_REG     0x24734L
#define PROM21_MAX_DSP_NUMBER      13

#define PROM21_CTRL_WR_ENABLE_REG    0x24788L
#define PROM21_CLKREQ_MODE_REG       0x24720L
#define PROM21_CLKREQ_PORT_REG       0x24724L

#define PROM21_SATA_CLASS_CODE_REG   0x24714L
#define PROM21_DEVICE_ID_ENABLE_REG  0x24700L
#define PROM21_SATA_DEVICE_ID_REG    0x24706L

#define PROM21_MAX_CLKREQ_NUMBER     5
#define PROM21_MAX_CLKREQ_MODE       3
#define PROM21_MAX_PORT_NUMBER       11

#define PT21_SATA_ID          0x43F61022

#define PT21_XHCI_ID_L1        0x43F71022
#define PT21_XHCI_ID_L2        0x43F81022
#define PT21_XHCI_ID_L3        0x43F91022
#define PT21_XHCI_ID_L4        0x43FA1022

STATIC UINT8 XhciDspDev     = 12;
STATIC UINT8 SecXhciDspDev  = 12;
STATIC UINT8 SataDspDev     = 13;
STATIC UINT8 SecSataDspDev  = 13;

#define PT21_USP_ID         0x43F41022
#define PT21_DSP_ID         0x43F51022

typedef enum {
  Usb3Port0 = 0,
  Usb3Port1,
  Usb3Port2,
  Usb3Port3,
  Usb3Port4,
  Usb3Port5,
  Usb2Port0,
  Usb2Port1,
  Usb2Port2,
  Usb2Port3,
  Usb2Port4,
  Usb2Port5,
  Usb2Port6,
  Usb2Port7,
  Usb2Port8,
  Usb2Port9,
  Usb2Port10,
  Usb2Port11
} USB_PORTS;

typedef struct {
  UINT32   GpioPinDir;
  UINT32   GpioInputData;
  UINT32   GpioOutputData;
} PT_GPIO_REG;

#define ACPI_TIMER_MMIO_ADDRESS         0xFED80808ul
#define ACPI_MAX_TIMER                  0xFFFFFFFFul
#define ACPI_ELAPSED_TIME_UNIT          28
#define ACPI_ELAPSED_TIME_DIVIDER       100

#define IOAPICSHDW_NB_PROG_DEVICE_REMAP_PBr   0x14301000ul
#define PCIERCCFG_ADDRESS                     0x11100000ul
#define PCIERCCFG_RS_ADDRESS                  0x1A300000ul
#define PCIERCCFG                             0
#define PCIEPORT                              4
#define PCIECORE                              8
#define PCIE_LC_CNTL                          0x280

