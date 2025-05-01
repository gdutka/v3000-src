/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmPei.h>
#include <AmdCpmBaseIo.h>
#include <AmdCpmBase.h>
#include "AmdCpmOemInitPeim.h"
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <FchXgbe.h>

#include <FchRegistersCommon.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <NbioGfxV4.h>
#include <Ppi/AmdFchInitPpi.h>
#include <GnbDxio.h>
#include <Library/DxioLibV2.h>
#include <AmdCpmBaseIo.h>
#include <Library/PciLib.h>
#include <Library/SmnAccessLib.h>

#include <smu13_bios_if_RMB.h>
#include <Library/IoLib.h>

extern  ATOM_COMMON_TABLE_HEADER       table_header_a0;
extern  ATOM_N6_DISPLAY_PHY_TUNING_SET display_phy_tuning_info_a0[];
extern  ATOM_COMMON_TABLE_HEADER       table_header_b0;
extern  ATOM_N6_DISPLAY_PHY_TUNING_SET display_phy_tuning_info_b0[];

typedef union {
  struct {
    UINT16    VendorId;
    UINT16    DeviceId;
  } Field;
  UINT32 Value;
} VEN_DEV_ID;


VEN_DEV_ID ApprovedListDeviceList[] = {
  {0x8086, 0x2526}, // I n t e l 9260
  {0x8086, 0x2723}, // I n t e l AX200
  {0x1912, 0x0015}, // R e n e s a s XHCI controller
  {0x14C3, 0x7961}, //RZ608 EVT
  {0x14C3, 0x0608}, //RZ608 PVT
  {0x14C3, 0x7922}, //RZ616 EVT
  {0x14C3, 0x0616}, //RZ616 PVT
  {0x17CB, 0x1103}, //QCOM NFA765
  {0x14C3, 0x7902}, //MTK MT7902
  {0x14C3, 0x6639}, //MTK MT7927
  {0x14C3, 0x0732}, //AMD RZ732
  {0xFFFF, 0xFFFF}
};

UINT8 OEMFchPmioEPostInitTableB0[] =
{
   0x02, 0x7D, 0x00, 0x1A, //XHCI3
   0x02, 0x7E, 0x00, 0x1A, //XHCI4
   0x02, 0x65, 0x00, 0x1A, //CIO0
   0x02, 0x66, 0x00, 0x1A, //CIO1
};

UINT32 NoTrainLinkDevFunc[] = {
  // Device << 3 | Function
  {(UINT32)((0x01 << 3) | 0x02)},  // NVME SSD1 (D:1, F:2)
  {(UINT32)((0x02 << 3) | 0x04)},  // NVME SSD0 (D:2, F:4)
  {(UINT32)((0x01 << 3) | 0x03)},  // WWAN (D:1, F:3)
  {0xFFFFFFFF}                     // END SIGN
};

//
// OEM CPM Table Definition
//

//
// Platform Id Table: Get Board Id from ECRAM
//
AMD_CPM_PLATFORM_ID_TABLE3          gCpmPlatformIdTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_GET_PLATFORM_ID3, sizeof (gCpmPlatformIdTable) / sizeof (UINT8), 0, 0, 0, 1},
  {
//  {RegisterOffset, StartBit, BitLength, PlatformIdBitOffset}
    {0x93,           4,        4,         0},                   // Board ID
    {0xFF,           0xFF,     0xFF,      0xFF}                 // End of Table
  }
};

//
// Convert Table from Board Id to Platform Id
//
AMD_CPM_PLATFORM_ID_CONVERT_TABLE   gCpmPlatformIdConvertTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table contect
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_GET_PLATFORM_ID_CONVERT, sizeof (gCpmPlatformIdConvertTable) / sizeof (UINT8), 0, 0, 0, 1},
  {
//  {UINT8  CpuRevisionId;  // CPU Revision ID
//   UINT16 OriginalIdMask; // Platform Id mask from platform Id table
//   UINT16 OriginalId;     // Platform Id from platform Id table
//   UINT16 ConvertedId;    // Platform Id which is used to check whether // CPM table supports current platform},
    {CPM_CPU_REVISION_ID_RMB, 0x000F, 0x0000, 0x0000}, //    Lilac FP7r2
    {CPM_CPU_REVISION_ID_RMB, 0x000F, 0x0001, 0x0001}, //    Mayan FP7
    {CPM_CPU_REVISION_ID_RMB, 0x000F, 0x0002, 0x0002}, //    Lilac FP7r2 DAP
    {CPM_CPU_REVISION_ID_RMB, 0x000F, 0x0003, 0x0003}, //    Mayan FP7 DAP
     0xFFFF,
  }
};

//
// Pre-Init Table
//
AMD_CPM_PRE_INIT_TABLE              gCpmPreInitTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_PRE_INIT, sizeof (gCpmPreInitTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
//  {UINT8  Type;     // Register type. 0: FCH MMIO. 1: PCI
//   UINT8  Select;   // Register sub-type
//   UINT8  Offset;   // Register offset
//   UINT8  AndMask;  // AND mask
//   UINT8  OrMask;   // OR mask
//   UINT8  Stage;    // Stage number},
//   UINT8  Purpose;  // Purpose. 0: For Normal Purpose. 1: For Modern Standby Purpose.
    {0x00, 0x0D, 0x09, 0xFC, 0x00, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE},
    {0x00, 0x0D, 0x0B, 0xFC, 0x00, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE},
    {0x00, 0x0D, 0x17, 0xFC, 0x00, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE},
    {0x00, 0x0D, 0x20, 0xFC, 0x00, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE},
    {0x00, 0x0D, 0x1A, 0xFC, 0x00, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE}, // IOMUXx0001A[1:0]: PCIE_RST_L = 0
    {0x00, 0x0D, 0x1B, 0xFC, 0x01, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE}, // IOMUXx0001B[1:0]: PCIE_RST1_L = 1
    {0x00, 0x03, 0x75, 0xFC, 0x01, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE},
    {0x00, 0x03, 0x8F, 0xFD, 0x02, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    {0x00, 0x15, 0xD9, 0x12, 0xF8, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE}, // Enable MP2 Wake Event
    {0x00, 0x15, 0xDB, 0xFF, 0x30, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE},
    {0x00, 0x15, 0x1D, 0xF8, 0x02, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE}, // AGPIO7  EC_SCI#
    {0x00, 0x15, 0x59, 0xF8, 0x03, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE}, // AGPIO22 EC ESPI_ALERT#
    {0x00, 0x15, 0x7D, 0xF8, 0x02, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE}, // AGPIO31 WWAN/GBE CLKREQ#
    {0x00, 0x15, 0x99, 0xF8, 0x02, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE}, // AGPIO38 CLKREQ5#
    {0x00, 0x15, 0x9D, 0xF8, 0x02, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE}, // AGPIO39 CLKREQ6#
    //for modern standby
    // For hardware reduced only, Power button; Clear WakeSts & InterruptSts; Enable wake in S0I3 state; set to Level trigger; Active Low; Enable interrupt delivery
    {0x00, 0x15, 0x01, 0x00, 0xE3, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    {0x00, 0x15, 0x03, 0xF7, 0x38, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    // for hardware reduced only, PwrBtnEn = 0, AcpiReducedHWEn = 0
    {0x00, 0x03, 0x75, 0xFE, 0x00, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    // need to confirm, Enable power button to wait the system after 4s shutdown
//    {0x00, 0x0E, 0x51, 0xEF, 0x10, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    // AC/DC timer
    {0x00, 0x15, 0xD1, 0x00, 0xE1, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    {0x00, 0x15, 0xD3, 0xFF, 0x30, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    // XHCI0 PME
    {0x00, 0x15, 0xE9, 0x00, 0xE1, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE},
    {0x00, 0x15, 0xEB, 0xC7, 0x28, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE},
    // XHCI1 PME
    {0x00, 0x15, 0xED, 0x00, 0xE1, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE},
    {0x00, 0x15, 0xEF, 0xC7, 0x28, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE},
    // NbGPP PME 172
    {0x00, 0x17, 0xB1, 0x00, 0x21, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    {0x00, 0x17, 0xB3, 0xFF, 0x30, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    // SD_CD# for onboard SD card reader
//    {0x00, 0x15, 0x0D, 0x00, 0x20, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
//    {0x00, 0x15, 0x0F, 0xFF, 0x30, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    // PCIE_WAKE_UP# => AGPIO2 (0xFED81508)
    {0x00, 0x15, 0x09, 0x00, 0xE3, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    {0x00, 0x15, 0x0B, 0xFF, 0x30, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    //WLAN_WAKE ->GPIO18
    {0x00, 0x15, 0x49, 0x00, 0xE3, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    {0x00, 0x15, 0x4B, 0xFF, 0x30, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    //WWAN_WAK ->GPIO17
    {0x00, 0x15, 0x45, 0x00, 0xE3, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    {0x00, 0x15, 0x47, 0xFF, 0x30, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    //LAN_WAK ->GPIO24
    {0x00, 0x15, 0x61, 0x00, 0xE3, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    {0x00, 0x15, 0x63, 0xFF, 0x30, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    // TPAD_ATTN# AGPIO8 (0x20)
    {0x00, 0x15, 0x21, 0x00, 0xE3, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE},
    {0x00, 0x15, 0x23, 0xF7, 0x38, CPM_PRE_INIT_STAGE_2, NORMAL_PURPOSE},
    //GPIO32 for touch Pannel
    {0x00, 0x15, 0x81, 0x00, 0x20, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    {0x00, 0x15, 0x83, 0xF7, 0x38, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    // WoV Int_ACP_FCH_AZ_Wake => AGPIO61
    {0x00, 0x15, 0xF5, 0x00, 0x20, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    {0x00, 0x15, 0xF7, 0xFF, 0x30, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    // WoV Int_ACP_FCH_AZ_Wake => AGPIO62
    {0x00, 0x15, 0xF9, 0x00, 0x20, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    {0x00, 0x15, 0xFB, 0xFF, 0x30, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    //GPIO44 for timer alarm
    {0x00, 0x15, 0xB1, 0x00, 0x28, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    {0x00, 0x15, 0xB3, 0xFF, 0x30, CPM_PRE_INIT_STAGE_2, MODERN_STANDBY},
    0xFF,
  }
};


//
// GPIO Init Table for Mayan/Lilac Combine support
//
AMD_CPM_GPIO_INIT_TABLE             gCpmGpioInitTable =
{
    {CPM_SIGNATURE_GPIO_INIT, sizeof (gCpmGpioInitTable) / sizeof (UINT8), 0, 0, 0x000000000, 0x0000000F},
    {
        0xFF,   // End of Table
    },
};

//
// MayanTV_RevA/LilacTv_RevB
//
AMD_CPM_GPIO_INIT_TABLE             gRmbGpioInitTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_GPIO_INIT, sizeof (gRmbGpioInitTable) / sizeof (UINT8), 0, 0, 0x000000000, 0x0000000F},
  {
    //       (socket,die,gpio,function,        output,           pullup)
        GPIO_DEF_V2 (0, 0, 0,   GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // PWR_BTN#_EC
        GPIO_DEF_V2 (0, 0, 1,   GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // SYS_RST#
        GPIO_DEF_V2 (0, 0, 2,   GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // PCIE_WAKE#
        GPIO_DEF_V2 (0, 0, 3,   GPIO_FUNCTION_0,  GPIO_INPUT,       GPIO_PU_EN),      // USBC_INT#_PD
        GPIO_DEF_V2 (0, 0, 4,   GPIO_FUNCTION_0,  GPIO_OUTPUT_HIGH, GPIO_PD_EN),      // EVAL_RST#_AUX
        GPIO_DEF_V2 (0, 0, 5,   GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PD_EN),      // M2_SSD_DEVSLP
        GPIO_DEF_V2 (0, 0, 6,   GPIO_FUNCTION_2,  GPIO_OUTPUT_HIGH, GPIO_PD_EN),      // MDIO0_SCL/PCIe_SLOT3_RST#_AUX
        GPIO_DEF_V2 (0, 0, 7,   GPIO_FUNCTION_0,  GPIO_OUTPUT_HIGH, GPIO_PU_EN),      // PCIe_SLOT1_RST#_AUX
        GPIO_DEF_V2 (0, 0, 8,   GPIO_FUNCTION_0,  GPIO_OUTPUT_HIGH, GPIO_PU_EN),      // PCIe_SLOT2_RST#_AUX
        GPIO_DEF_V2 (0, 0, 9,   GPIO_FUNCTION_0,  GPIO_OUTPUT_HIGH, GPIO_PU_EN),      // AGPIO9/PPS2_1V8
        GPIO_DEF_V2 (0, 0, 10,  GPIO_FUNCTION_0,  GPIO_OUTPUT_LOW,  GPIO_PU_EN),      // XGBE LED - EMBDEV-16906
        GPIO_DEF_V2 (0, 0, 11,  GPIO_FUNCTION_0,  GPIO_INPUT,       GPIO_PU_EN),      // AGPIO11/MDIO3_SDA_PPS3_1V8
        GPIO_DEF_V2 (0, 0, 12,  GPIO_FUNCTION_1,  GPIO_OUTPUT_HIGH, GPIO_PU_EN),      // M2_SSD1_PWREN
        GPIO_DEF_V2 (0, 0, 16,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // USB_OCP_ALL#
        GPIO_DEF_V2 (0, 0, 17,  GPIO_FUNCTION_1,  GPIO_INPUT,       GPIO_PU_EN),      // M2_SSD_PEDET_1V8
        GPIO_DEF_V2 (0, 0, 18,  GPIO_FUNCTION_1,  GPIO_INPUT,       GPIO_PU_EN),      // ALERT#_M2_SSD_1V8
        GPIO_DEF_V2 (0, 0, 19,  GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PU_EN),      // APU_SCLK1_1V8
        GPIO_DEF_V2 (0, 0, 20,  GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PU_EN),      // APU_SDATA1_1V8
        GPIO_DEF_V2 (0, 0, 21,  GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PU_EN),      // KBRST#
//  PLAT-79448 [RMB-FP7][eSPI] RMB FP7 eSPI_ALERT# still toggles after ALERT# event. "Request for Remove the eSPI default setting"
//      GPIO_DEF_V2 (0, 0, 22,  GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PU_EN),      // ESPI_ALERT#
        GPIO_DEF_V2 (0, 0, 23,  GPIO_FUNCTION_1,  GPIO_OUTPUT_HIGH, GPIO_PU_EN),      // AC_PRS/PPS1_1V8
        GPIO_DEF_V2 (0, 0, 24,  GPIO_FUNCTION_1,  GPIO_INPUT,       GPIO_PU_EN),      // KR10G_PHY0_INTR#_1V8
//      GPIO_DEF_V2 (0, 0, 26,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_PD_DIS),  // PCIE_RST#_R
//      GPIO_DEF_V2 (0, 0, 26,  GPIO_FUNCTION_1,  GPIO_OUTPUT_LOW,  GPIO_PD_EN),      // PCIE_RST#_R, Override case by GpioReset, it will alert the PIN been GPIO.
//      GPIO_DEF_V2 (0, 0, 27,  GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PD_EN),      // PCIE_RST1#_R
        GPIO_DEF_V2 (0, 0, 27,  GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PD_EN),      // PCIE_RST1#_R
        GPIO_DEF_V2 (0, 0, 29,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // TPM_CS
        GPIO_DEF_V2 (0, 0, 30,  GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PU_EN),      // ESPI_EC_CS#
        GPIO_DEF_V2 (0, 0, 31,  GPIO_FUNCTION_2,  GPIO_INPUT,       GPIO_PU_EN),      // INT_CLKREQ#_S5
        GPIO_DEF_V2 (0, 0, 32,  GPIO_FUNCTION_0,  GPIO_OUTPUT_HIGH, GPIO_PU_EN),      // AGPIO32/MDIO3_SCL_1V8
        GPIO_DEF_V2 (0, 0, 38,  GPIO_FUNCTION_2,  GPIO_NA,          GPIO_PU_EN),      // CLK_REQ5_L/AGPIO38/MDIO1_SDA
        GPIO_DEF_V2 (0, 0, 39,  GPIO_FUNCTION_2,  GPIO_NA,          GPIO_PU_EN),      // CLK_REQ6_L/AGPIO39/MDIO1_SCL
        GPIO_DEF_V2 (0, 0, 40,  GPIO_FUNCTION_2,  GPIO_NA,          GPIO_PD_EN),      // MDIO_SDA for PHY0
        GPIO_DEF_V2 (0, 0, 42,  GPIO_FUNCTION_0,  GPIO_INPUT,       GPIO_PU_EN),      // KR10G_PHY1_INTR#_1V8
        GPIO_DEF_V2 (0, 0, 67,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PD_EN),      // SPI_ROM_REQ
        GPIO_DEF_V2 (0, 0, 68,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // ESPI1_DAT2
        GPIO_DEF_V2 (0, 0, 69,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // ESPI1_DAT3
        GPIO_DEF_V2 (0, 0, 70,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PD_EN),      // SPI2_CLK
        GPIO_DEF_V2 (0, 0, 74,  GPIO_FUNCTION_1,  GPIO_OUTPUT_HIGH, GPIO_PU_EN),      // CLK_MUX_SELA/B_1V8
        GPIO_DEF_V2 (0, 0, 75,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // SPI2_CS#
        GPIO_DEF_V2 (0, 0, 76,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PD_EN),      // SPI_ROM_GNT#
        GPIO_DEF_V2 (0, 0, 77,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PD_EN),      // "ESPI1_CLK", ESPI1 Bus clock to EC
        GPIO_DEF_V2 (0, 0, 78,  GPIO_FUNCTION_1,  GPIO_OUTPUT_HIGH, GPIO_PD_EN),      // EC_RST_L
        GPIO_DEF_V2 (0, 0, 79,  GPIO_FUNCTION_1,  GPIO_OUTPUT_LOW,  GPIO_PD_EN),      // M2_SSD1_RST#_AUX
    	GPIO_DEF_V2 (0, 0, 80,  GPIO_FUNCTION_0, GPIO_NA,          GPIO_PU_EN),    // ESPI1_DAT1 <"FP6"/FP7>
    	GPIO_DEF_V2 (0, 0, 81,  GPIO_FUNCTION_0, GPIO_NA,          GPIO_PU_EN),    // ESPI1_DAT0 <"FP6"/FP7>
        GPIO_DEF_V2 (0, 0, 84,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // FANTACH0_APU
        GPIO_DEF_V2 (0, 0, 85,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // FANOUT0_APU
        GPIO_DEF_V2 (0, 0, 89,  GPIO_FUNCTION_2,  GPIO_INPUT,       GPIO_PU_EN),      // DP_HPD_DIG#_1V8
        GPIO_DEF_V2 (0, 0, 90,  GPIO_FUNCTION_2,  GPIO_INPUT,       GPIO_PU_EN),      // TPM_PIRQ#
//        GPIO_DEF_V2 (0, 0, 91,  GPIO_FUNCTION_1,  GPIO_OUTPUT_LOW,  GPIO_PU_EN),      // XGBE LED
        GPIO_DEF_V2 (0, 0, 92,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // INT_EVAL_CLKREQ#
        GPIO_DEF_V2 (0, 0, 104, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PD_EN),      // "SPI2_DAT0", SPI2 Bus DAT0
        GPIO_DEF_V2 (0, 0, 105, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PD_EN),      // "SPI2_DAT1", SPI2 Bus DAT0
        GPIO_DEF_V2 (0, 0, 106, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // "SPI2_DAT2", SPI2 Bus DAT0
        GPIO_DEF_V2 (0, 0, 107, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // "SPI2_DAT3", SPI2 Bus DAT0
        GPIO_DEF_V2 (0, 0, 113, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_PD_DIS),  // SCL0, APU_SCLK0_1v8
        GPIO_DEF_V2 (0, 0, 114, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_PD_DIS),  // SDA0, APU_SDATA0_1V8
        GPIO_DEF_V2 (0, 0, 115, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // INT_SLOT1_CLKREQ#
        GPIO_DEF_V2 (0, 0, 116, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // INT_SLOT2_CLKREQ#
        GPIO_DEF_V2 (0, 0, 130, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // SATA_ACT_L, To control SATA LED
        GPIO_DEF_V2 (0, 0, 131, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // INT_SLOT3_CLKREQ#
        GPIO_DEF_V2 (0, 0, 132, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_EN),      // INT_M2_SSD_CLKREQ#
        GPIO_DEF_V2 (0, 0, 135, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PD_EN),      // UART2_CTS#
        GPIO_DEF_V2 (0, 0, 136, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PD_EN),      // UART2_RXD
        GPIO_DEF_V2 (0, 0, 137, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PU_EN),      // UART2_RTS#
        GPIO_DEF_V2 (0, 0, 138, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PU_EN),      // UART2_TXD
        GPIO_DEF_V2 (0, 0, 139, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PU_EN),      // UART2_INTR
        GPIO_DEF_V2 (0, 0, 140, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PD_EN),      // UART0_CTS#
        GPIO_DEF_V2 (0, 0, 141, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PD_EN),      // UART0_RXD
        GPIO_DEF_V2 (0, 0, 142, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PU_EN),      // UART0_RTS#
        GPIO_DEF_V2 (0, 0, 143, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PU_EN),      // UART0_TXD
//        GPIO_DEF_V2 (0, 0, 144, GPIO_FUNCTION_0,  GPIO_OUTPUT_LOW, GPIO_PU_EN),      // XGBE LED
        GPIO_DEF_V2 (0, 0, 145, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_PD_DIS),  // APU_I2C0_SCL_1V8
        GPIO_DEF_V2 (0, 0, 146, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_PD_DIS),  // APU_I2C0_SDA_1V8
        GPIO_DEF_V2 (0, 0, 147, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_PD_DIS),  // APU_I2C1_SCL_1V8
        GPIO_DEF_V2 (0, 0, 148, GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_PD_DIS),  // APU_I2C1_SDA_1V8
        GPIO_DEF_V2 (0, 0, 153, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PD_EN),      // UART4_CTS#
        GPIO_DEF_V2 (0, 0, 154, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PU_EN),      // UART4_RTS#
        GPIO_DEF_V2 (0, 0, 155, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PD_EN),      // UART4_RXD
        GPIO_DEF_V2 (0, 0, 156, GPIO_FUNCTION_1,  GPIO_NA,          GPIO_PU_EN),      // UART4_TXD
//        GPIO_DEF_V2 (0, 0, 157, GPIO_FUNCTION_0,  GPIO_OUTPUT_LOW, GPIO_PU_EN),      // XGBE LED
    // End of GPIO Init Table
        0xFF,
  }
};


//
// GEVENT Init Table for Fox support
//
AMD_CPM_GEVENT_INIT_TABLE           gCpmGeventInitTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_GEVENT_INIT, sizeof (gCpmGeventInitTable) / sizeof (UINT8), 0, 0, 0x00000000, 0x00000001},
  {//                  GEvent EventEnable   SciTrigE      SciLevl         SmiSciEn        SciS0En         SciMap      SmiTrig       SmiControl
    GEVENT_DEFINITION (0x01,  EVENT_ENABLE, SCITRIG_LOW,  SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_01,  SMITRIG_HI,   SMICONTROL_DISABLE), // GEVENT01: TPM_PIRQ#
    GEVENT_DEFINITION (0x02,  EVENT_ENABLE, SCITRIG_LOW,  SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_02,  SMITRIG_HI,   SMICONTROL_DISABLE), // GEVENT02: PCIE_SD_WAKE#
    GEVENT_DEFINITION (0x04,  EVENT_ENABLE, SCITRIG_LOW,  SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_04,  SMITRIG_HI,   SMICONTROL_DISABLE), // GEVENT04: UART_WAKE#_M2_APU, Wake from M.2 BT
    GEVENT_DEFINITION (0x08,  EVENT_ENABLE, SCITRIG_LOW,  SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_08,  SMITRIG_HI,   SMICONTROL_DISABLE), // GEVENT8: PCIE_WAKE#
//    GEVENT_DEFINITION (0x0B,  EVENT_ENABLE, SCITRIG_LOW,  SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_11,  SMITRIG_HI,   SMICONTROL_DISABLE), // GEVENT11: EC_SCI#
//  GEVENT_DEFINITION (0x0D,  EVENT_ENABLE, SCITRIG_LOW,  SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_13,  SMITRIG_HI,   SMICONTROL_DISABLE), // GEVENT13: WAKE_ON_WAN#
//  GEVENT_DEFINITION (0x0E,  EVENT_ENABLE, SCITRIG_LOW,  SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_14,  SMITRIG_HI,   SMICONTROL_DISABLE), // GEVENT14: PCIE_WLAN_WAKE#
//  GEVENT_DEFINITION (0x0F,  EVENT_ENABLE, SCITRIG_LOW,  SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_15,  SMITRIG_HI,   SMICONTROL_DISABLE), // GEVENT15: PCIE_LOM_WAKE#
    GEVENT_DEFINITION (0x11,  EVENT_ENABLE, SCITRIG_LOW,  SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_17,  SMITRIG_LOW,  SMICONTROL_DISABLE), // GEVENT17: TPNL_INT#
    GEVENT_DEFINITION (0x17,  EVENT_ENABLE, SCITRIG_LOW,  SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_23,  SMITRIG_LOW,  SMICONTROL_DISABLE), // GEVENT23: TPAD_INT#
    0xFF,
  }
};


//
// CPM GPIO Module
//
AMD_CPM_GPIO_DEVICE_CONFIG_TABLE    gCpmGpioDeviceConfigTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_GPIO_DEVICE_CONFIG, sizeof (gCpmGpioDeviceConfigTable) / sizeof (UINT8), 0, 0, 0, 0x0000000F},
  {
//  GPIO_DEVICE_DEFINITION (deviceid,               enable,           assert,   deassert, hotplug)      // assert: ResetPinAssert (Pin will be low#)
    GPIO_DEVICE_DEFINITION (DEVICE_ID_MXM,          CPM_DEVICE_AUTO,  0,        1,        0),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_DT,           CPM_DEVICE_ON,    0,        0,        0),
//  GPIO_DEVICE_DEFINITION (DEVICE_ID_WLAN,         CPM_DEVICE_ON,    0,        0,        0),
//  GPIO_DEVICE_DEFINITION (DEVICE_ID_LOM,          CPM_DEVICE_ON,    0,        0,        0),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_DTx1,         CPM_DEVICE_ON,    0,        0,        0),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_SSD,          CPM_DEVICE_AUTO,  1,        0,        0),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_SSD1,         CPM_DEVICE_AUTO,  1,        0,        0),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_USB_TYPE_C0,  CPM_DEVICE_OFF,   0,        0,        0), // HDMI over USB Type C Port 0 control
    GPIO_DEVICE_DEFINITION (DEVICE_ID_USB_TYPE_C1,  CPM_DEVICE_OFF,   0,        0,        0), // HDMI over USB Type C Port 1 control
//  GPIO_DEVICE_DEFINITION (DEVICE_ID_SD,           CPM_DEVICE_ON,    0,        0,        1),
    GPIO_DEVICE_CONFIG (DEVICE_ID_VGAMUXVCC,    CPM_DEVICE_ON,    0,        0), // Smart Mux VCC/Reset
    GPIO_DEVICE_CONFIG (DEVICE_ID_VGAMUXSEL,    CPM_DEVICE_OFF,   0,        0), // Smart Mux Switch: CPM_DEVICE_OFF = iGPU, CPM_DEVICE_ON = dGPU
    0xFF,
  }
};

//
// CPM Device Detection Table for Lilac
//
AMD_CPM_GPIO_DEVICE_DETECTION_TABLE gCpmGpioDeviceDetectionTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_GPIO_DEVICE_DETECTION, sizeof (gCpmGpioDeviceDetectionTable) / sizeof (UINT8), 0, 0, 0, 0x0000000F},
  {//                     deviceid,      pin,              value,              misc
    GPIO_DETECTION_TYPE0 (DEVICE_ID_MXM, MXM_PRESENT,      MXM_DETECT_VALUE,   0),
//  GPIO_DETECTION_TYPE0 (DEVICE_ID_SSD, SSD_PRESENT,      1,                  0),  // Lilac Remove temporary for always enable.
    // TODO: Odd might be supported.

    // End of Table
    0xFF,
  }
};

// Check the Porting Guide Page PCIe&SATA HW signal
//
// CPM Device Reset Table
//
//
//  For Lilac/Mayan, The Reset Pin, some will depend on PCIE_RST#, it will move to OemGpio Lib
//
AMD_CPM_GPIO_DEVICE_RESET_TABLE gCpmGpioDeviceResetTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_GPIO_DEVICE_RESET, sizeof (gCpmGpioDeviceResetTable) / sizeof (UINT8), 0, 0, 0, 0x0000000F},
  {
//  {UINT8  DeviceId;       // Device Id
//   UINT8  Mode;           // Reset mode     // 0: Reset Assert. // 1: Reset De-assert             // 2: Delay between Assert and Deassert
//   UINT8  Type;           // Register type  // 0: GPIO.         // 1: Set GPIO and read back to check it's set. Max. time out is 100ms. when Mode = 0 or 1.
//   UINT32 ((UINT16)Pin + ((UINT8)Value << 16));                 // GPIO pin value
//   UINT8  InitFlag;       // Init flag in post},
//    { DEVICE_ID_SD,   CPM_RESET_ASSERT,   CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (SD_RESET, 0),          GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xA3[7] (BIT31)
//    { DEVICE_ID_WLAN, CPM_RESET_ASSERT,   CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (WLAN_RESET, 0),        GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xA3[5] (BIT29)
//    { DEVICE_ID_DTx1, CPM_RESET_ASSERT,   CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (X1_SLOT_RST_AUX, 0),   GPIO_DEVICE_INIT_STAGE_DUMMY}, //
//    { DEVICE_ID_WWAN, CPM_RESET_ASSERT,   CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (WWAN_RESET, 0),        GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xA3[4] (BIT28)
//    { DEVICE_ID_DT,   CPM_RESET_ASSERT,   CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (DT_RESET, 0),          GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xA3[2] (BIT26)
//    { DEVICE_ID_LOM,  CPM_RESET_ASSERT,   CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (LAN_RESET, 0),         GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xA3[1]
//    { DEVICE_ID_MXM,  CPM_RESET_ASSERT,   CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (MXM_RESET, 0),         GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xA3[0] (BIT24)
//    { DEVICE_ID_WWAN, CPM_RESET_ASSERT,   CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (WWAN_MODULE_RESET, 0), GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xAB[0] (BIT88)
    { DEVICE_ID_SSD,  CPM_RESET_ASSERT,   CPM_RESET_GPIO,       CPM_GPIO_PIN (SSD_RESET, 0),               GPIO_DEVICE_INIT_STAGE_DUMMY}, // FCH GPIO78
//    { DEVICE_ID_SSD1, CPM_RESET_ASSERT,   CPM_RESET_GPIO,       CPM_GPIO_PIN (SSD1_RESET, 0),              GPIO_DEVICE_INIT_STAGE_DUMMY}, // FCH GPIO79
//    { DEVICE_ID_BT,   CPM_RESET_ASSERT,   CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (BT_RADIO_EN, 0),       GPIO_DEVICE_INIT_STAGE_DUMMY},
//    { DEVICE_ID_SD,   CPM_RESET_DEASSERT, CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (SD_RESET, 1),          GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xA3[7] (BIT31)
//    { DEVICE_ID_WLAN, CPM_RESET_DEASSERT, CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (WLAN_RESET, 1),        GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xA3[5] (BIT29)
//    { DEVICE_ID_DTx1, CPM_RESET_DEASSERT, CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (X1_SLOT_RST_AUX, 1),   GPIO_DEVICE_INIT_STAGE_DUMMY}, //
//    { DEVICE_ID_WWAN, CPM_RESET_DEASSERT, CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (WWAN_RESET, 1),        GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xA3[4] (BIT28)
//    { DEVICE_ID_DT,   CPM_RESET_DEASSERT, CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (DT_RESET, 1),          GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xA3[2] (BIT26)
//    { DEVICE_ID_LOM,  CPM_RESET_DEASSERT, CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (LAN_RESET, 1),         GPIO_DEVICE_INIT_STAGE_DUMMY}, //
//    { DEVICE_ID_MXM,  CPM_RESET_DEASSERT, CPM_RESET_GPIO_CHECK, CPM_ECRAM_GPIO_PIN (MXM_RESET, 1),         GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xA3[0] (BIT24)
//    { DEVICE_ID_WWAN, CPM_RESET_DEASSERT, CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (WWAN_MODULE_RESET, 1), GPIO_DEVICE_INIT_STAGE_DUMMY}, // ECRAM xAB[0] (BIT88)
 //   { DEVICE_ID_SSD,  CPM_RESET_DEASSERT, CPM_RESET_GPIO,       CPM_GPIO_PIN (SSD_RESET, 1),               GPIO_DEVICE_INIT_STAGE_DUMMY}, // FCH GPIO40
//    { DEVICE_ID_SSD1, CPM_RESET_DEASSERT, CPM_RESET_GPIO,       CPM_GPIO_PIN (SSD1_RESET, 1),              GPIO_DEVICE_INIT_STAGE_DUMMY}, // FCH GPIO79
//    { DEVICE_ID_BT,   CPM_RESET_DEASSERT, CPM_RESET_GPIO,       CPM_ECRAM_GPIO_PIN (BT_RADIO_EN, 1),       GPIO_DEVICE_INIT_STAGE_DUMMY},
    0xFF,
  }
};


//
// CPM GPIO Device Init Table (Power On/Off) - Lilac/Mayan support
//
/*
 * For the device power, current it is override by function "AmdCpmTableOverride",
 * all "GPIO_DEVICE_INIT_STAGE_1" will be replace "GPIO_DEVICE_INIT_STAGE_DUMMY"
 *
 * Later will be restore on GpioResetControlLib.c.
 *
 * */
AMD_CPM_GPIO_DEVICE_POWER_TABLE gCpmGpioDevicePowerTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_GPIO_DEVICE_POWER, sizeof (gCpmGpioDevicePowerTable) / sizeof (UINT8), 0, 0, 0x00000000, 0x0000000F},
  {
//  {UINT8  DeviceId;       // Device Id
//   UINT8  Mode;           // Device Power Mode. 1: Power On. 0: Power Off
//   UINT8  Type;           // Device Power Item. 0: Set GPIO. 1: Wait GPIO. 2: Add Delay
//   UINT32 ((UINT16)Pin + ((UINT8)Value << 16));                 // GPIO pin value or delay timer
//   UINT8  InitFlag;       // Init flag in post},
  // {DEVICE_ID_MXM,         CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (MXM_POWER_ENABLE, 0),       GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_MXM,         CPM_POWER_OFF,  CPM_POWER_DELAY,  1000,                                           GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_MXM,         CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (MXM_POWER_ON, 0),           GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_SSD1,        CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SSD1_POWER_EN, 0),          GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_SSD,         CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SSD0_POWER_EN, 0),          GPIO_DEVICE_INIT_STAGE_1},
//  {DEVICE_ID_ODD,         CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (ODD_PWREN, 0),              GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_SD,          CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SD_PWR_EN, 0),              GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_DT,          CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (DT_POWER_EN, 0),            GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_DTx1,        CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (DT_X1_POWER_EN, 0),         GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_WLAN,        CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (WLAN_POWER_EN, 0),          GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_WWAN,        CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (WWAN_POWER_EN, 0),          GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_LOM,         CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (LAN_POWER_EN, 0),           GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_WWAN,        CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (WWAN_CARD_POWER_OFF_L, 0),  GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_WWAN,        CPM_POWER_OFF,  CPM_POWER_DELAY,  100,                                            GPIO_DEVICE_INIT_STAGE_1},
  //{DEVICE_ID_VGAMUXVCC,   CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SMART_MUX_PWM_EN, 0),       GPIO_DEVICE_INIT_STAGE_1},
  //{DEVICE_ID_VGAMUXVCC,   CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SMART_MUX_PANEL_BKEN, 0),   GPIO_DEVICE_INIT_STAGE_1},
  //{DEVICE_ID_VGAMUXVCC,   CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SMART_MUX_RESET, 0),        GPIO_DEVICE_INIT_STAGE_1},
  //{DEVICE_ID_VGAMUXSEL,   CPM_POWER_OFF,  CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SMART_MUX_SWITCH, 0),       GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_MXM,         CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (MXM_POWER_ON, 1),           GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_MXM,         CPM_POWER_ON,   CPM_POWER_DELAY,  30000,                                          GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_MXM,         CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (MXM_POWER_ENABLE, 1),       GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_MXM,         CPM_POWER_ON,   CPM_POWER_WAIT,   CPM_ECRAM_GPIO_PIN (MXM_POWER_GOOD, 1),         GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_MXM,         CPM_POWER_ON,   CPM_POWER_DELAY,  32000,                                          GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_SSD1,        CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SSD1_POWER_EN, 1),          GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_SSD,         CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SSD0_POWER_EN, 1),          GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_SD,          CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SD_PWR_EN, 1),              GPIO_DEVICE_INIT_STAGE_1},
//  {DEVICE_ID_ODD,         CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (ODD_PWREN, 1),              GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_DT,          CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (DT_POWER_EN, 1),            GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_DTx1,        CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (DT_X1_POWER_EN, 1),         GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_WLAN,        CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (WLAN_POWER_EN, 1),          GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_WWAN,        CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (WWAN_POWER_EN, 1),          GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_LOM,         CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (LAN_POWER_EN, 1),           GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_WWAN,        CPM_POWER_ON,   CPM_POWER_DELAY,  100,                                            GPIO_DEVICE_INIT_STAGE_1},
  // {DEVICE_ID_WWAN,        CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (WWAN_CARD_POWER_OFF_L, 1),  GPIO_DEVICE_INIT_STAGE_1},
  //{DEVICE_ID_VGAMUXVCC,   CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SMART_MUX_PWM_EN, 0),       GPIO_DEVICE_INIT_STAGE_1},
  //{DEVICE_ID_VGAMUXVCC,   CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SMART_MUX_PANEL_BKEN, 1),   GPIO_DEVICE_INIT_STAGE_1},
  //{DEVICE_ID_VGAMUXVCC,   CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SMART_MUX_RESET, 1),        GPIO_DEVICE_INIT_STAGE_1},
  //{DEVICE_ID_VGAMUXSEL,   CPM_POWER_ON,   CPM_POWER_SET,    CPM_ECRAM_GPIO_PIN (SMART_MUX_SWITCH, 1),       GPIO_DEVICE_INIT_STAGE_1},
  0xFF,
  }
};

//
// PCIE Clock Table
//
AMD_CPM_PCIE_CLOCK_TABLE    gCpmPcieClockTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_PCIE_CLOCK, sizeof (gCpmPcieClockTable) / sizeof (UINT8), 0, 0, 0x00000000, 0x0F},
  {
//  {UINT8  ClkId;              // FCH PCIe Clock
//   UINT8  ClkReq;             // FCH PCIe ClkReq
//   UINT8  ClkIdExt;           // External Clock Source
//   UINT8  ClkReqExt;          // External ClkReq
//   UINT8  DeviceId;           // Device Id. No Device Id if 0xFF
//   UINT8  Device;             // Device Number of PCIe bridge
//   UINT8  Function;           // Function Number of PCIe bridge
//   UINT8  SlotCheck;          // Slot Check Flag: // BIT0: Check PCI Space // BIT1: Check GPIO pin // BIT2: Check Clock Power Management Enable // BIT3~6: Reserved // BIT7: Change PCIe Clock in ACPI method
//   UINT32 SpecialFunctionId;  // Id of Special Function}
//        ( socket,   die,  ClkId,     ClkReq,        DeviceId,         Device, Function, SlotCheck)
    PCIE_INCLK (  0,  0,    GPP_CLK0,  CLK_REQ0,      DEVICE_ID_MXM,    1,      1,        NON_SLOT_CHECK), // EVAL
    PCIE_INCLK (  0,  0,    GPP_CLK1,  CLK_REQ1,      DEVICE_ID_DT,     1,      2,        NON_SLOT_CHECK), // PCIe x4 Slot-1
    PCIE_INCLK (  0,  0,    GPP_CLK2,  CLK_REQ2,      DEVICE_ID_DT,     2,      1,        NON_SLOT_CHECK), // PCIe x4 Slot-2
    PCIE_INCLK (  0,  0,    GPP_CLK3,  CLK_REQ3,      DEVICE_ID_DT,     2,      3,        NON_SLOT_CHECK), // PCIe x4 Slot-3
    PCIE_INCLK (  0,  0,    GPP_CLK4,  CLK_REQ4,      DEVICE_ID_SSD,    2,      2,        NON_SLOT_CHECK), // M.2 NVME
    0xFF,
  }
};

//
// CPM DXIO Topology Table
//
/// Note: DO NOT change the seqeunce of the PCIe slot, M.2 and SATA Express engines
AMD_CPM_DXIO_TOPOLOGY_TABLE gCpmDxioTopologyTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_DXIO_TOPOLOGY, sizeof (gCpmDxioTopologyTable) / sizeof (UINT8), 0, 0, 0x00000000, 0x0000000F},
  0,  // SocketId
  {   // DXIO_PORT_DESCRIPTOR
//    UINT32  Flags;                    /**< Descriptor flags
//    #define DXIO_ENGINE_DATA_INITIALIZER(mType, mStartLane, mEndLane, mHotplug, mGpioGroupId) {mType, mHotplug, 0, mStartLane, mEndLane, mGpioGroupId, 0, 0, 0}
//    #define DXIO_PORT_DATA_INITIALIZER_PCIE(mPortPresent, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, mMaxLinkCap, mAspm, mAspmL1_1, mAspmL1_2,  mClkPmSupport) \
//            {mPortPresent, mDevAddress, mDevFunction, mMaxLinkSpeed, mAspm, mAspmL1_1, mAspmL1_2, 0, mHotplug, {0, mMaxLinkCap, 0, mClkPmSupport}, {0, 0, 0}, DxioEndpointDetect}

      0
  },  // End of DXIO_PORT_DESCRIPTOR
  // DDI_DESCRIPTOR,
  {
    //
    // Default for Lilac/Mayan, follow DDI part might need VBIOS Help ...
    // Refer DxioDdiConnectorTypeList[] for final default setting.
    //
    { // DDI0 - DP
      0,
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux1, Hdp1)
    },
    { // DDI1 - DP
      0,
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux2, Hdp2)
    },
    { // DDI2 - DP (USB TypeC)
      0,
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDpWithTypeC, Aux3, Hdp3)
    },
    { // DDI3 - DP
      0,
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDpWithoutTypeC, Aux4, Hdp4)
    },
    { // DDI4 - DP
      DESCRIPTOR_TERMINATE_LIST,
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDpWithoutTypeC, Aux5, Hdp5)
    }
  }   // End of PCIe_DDI_DESCRIPTOR
};

AMD_CPM_NVME_RTD3_TABLE gCpmNvmeRtd3Table = {
  {CPM_SIGNATURE_NVME_STD3, sizeof (gCpmNvmeRtd3Table) / sizeof (UINT8), 0, 0, 0x00000000, 0x0000000F},
  0x00,                   // NvmeD3ColdSupported - It's auto-updated by PBS option
  0x00,                   // NvmeD3cConfigSize - It's auto-updated in the PEI phase.
  0x00,                   // IsSetNvmePassword
  {
    { // M.2 NVME SSD0
      0x02,               // Root port Device
      0x04,               // Roop port Function
      0x00,               // T-pvperl - 0x00 indicate the value will auto-sync with the PBS option
      0x00,               // Trst-cfg - 0x00 indicate the value will auto-sync with the PBS option
      78,                 // M.2 SSD0 Reset aux pin
      SSD0_POWER_EN,      // M.2 SSD0 Power (EC Ram)
      0xFFFFFFCF,         // Gpp clock request mask (CLK_REQ#4 BIT[4:5])
    },
    { // M.2 NVME SSD1
      0x01,               // Root port Device
      0x02,               // Roop port Function
      0x00,               // T-pvperl - 0x00 indicate the value will auto-sync with the PBS option
      0x00,               // Trst-cfg - 0x00 indicate the value will auto-sync with the PBS option
      79,                 // M.2 SSD1 Reset aux pin
      SSD1_POWER_EN,      // M.2 SSD1 Power (EC Ram)
      0xFFFFFFF3,         // Gpp clock request mask (CLK_REQ#1 BIT[2:3])
    },
    0xFF
  }
};

//
//+-------------+----------------------+-----+-----------+------------+------------+------------+
//| Platform Id |        Board         | DP0 |    DP1    |     DP2    |     DP3    |    DP4     |
//+-------------+----------------------+-----+-----------+------------+------------+------------+
//| 0x00        | PLATFORM_SelLilacTv  | eDP | DP NOVA   | DP + USB-C | DP + USB-C | DP + USB-C |
//| 0x01        | PLATFORM_SelMayaTv   | eDP | DP NOVA   | DP + USB-C | DP + USB-C | DP + USB-C |
//| 0x02        | PLATFORM_SelLilacDAP | eDP | HDMI NOVA | DP + USB-C | DP + USB-C | HDMI       |
//| 0x03        | PLATFORM_SelMayanDAP | eDP | HDMI NOVA | DP + USB-C | DP + USB-C | DP + USB-C |
//+-------------+----------------------+-----+-----------+------------+------------+------------+
//
//
typedef struct _PLATFORM_DXIO_DDI {
  UINT32                            PlaformId;
  PCIE_CONNECTOR_TYPE               ConnectorType[DXIO_DDI_COUNT];
} PLATFORM_DXIO_DDI;

PLATFORM_DXIO_DDI gCpmPlatformDxioDdiTable[] = {
  { 0x00, {ConnectorTypeDP,  ConnectorTypeDP,   ConnectorTypeDpWithTypeC, ConnectorTypeDpWithoutTypeC, ConnectorTypeDpWithoutTypeC }},
  { 0x01, {ConnectorTypeEDP,   ConnectorTypeDP, ConnectorTypeDpWithTypeC, ConnectorTypeDpWithTypeC,   ConnectorTypeDpWithTypeC }},
  { 0x02, {ConnectorTypeEDP, ConnectorTypeHDMI, ConnectorTypeDpWithTypeC, ConnectorTypeDpWithTypeC,   ConnectorTypeHDMI }},
  { 0x03, {ConnectorTypeEDP, ConnectorTypeHDMI, ConnectorTypeDpWithTypeC, ConnectorTypeDpWithTypeC,   ConnectorTypeDpWithTypeC }},
  { 0x05, {ConnectorTypeEDP, ConnectorTypeDP,   ConnectorTypeDpWithTypeC, ConnectorTypeDpWithTypeC,   ConnectorTypeDpWithTypeC }},
  { 0x06, {ConnectorTypeEDP, ConnectorTypeDP,   ConnectorTypeDpWithTypeC, ConnectorTypeDpWithTypeC,   ConnectorTypeDpWithTypeC }},
};

DXIO_PORT_DESCRIPTOR DxioFixDevicePort[] =
{
    // EVAL/dGPU                       // Entry 0
  {
    0,
    DXIO_ENGINE_DATA_INITIALIZER (DxioPcieEngine, 0, 7, HotPluggableDisabled, DEVICE_ID_MXM),
    DXIO_PORT_DATA_INITIALIZER_PCIE_V7 (
      DxioPortEnabled,                      // Port Present
      1,                                    // Requested Device
      1,                                    // Requested Function
      DxioHotplugDisabled,                  // Hotplug
      DxioGenMaxSupported,                  // Max Link Speed
      DxioGenMaxSupported,                  // Max Link Capability
      DxioAspmL1,                           // DxioAspmL0sL1, // ASPM
      DxioAspmL11Enabled,                   // ASPM L1.1  // 0:disabled, 1:Enabled,   <BU02>: Set to Disable, OrigSet: DxioAspmL11Enabled
      DxioAspmL12Enabled,                   // ASPM L1.2  // 0:disabled, 1:Enabled    <BU02>: Set to Disable, OrigSet: DxioAspmL12Enabled
      DxioClkPmSupportEnabled,              // Clock PM
      CLK_REQ0,                             // CLKREQ#
      3                                     // EQ Preset, set to 0x3 by default
    )
    NO_PHY_PARAMS_DATA
    PORT_PARAMS_START
    PORT_PARAM (PP_PSPP_AC, 0x144),         //AC_DirectIndirect[11:8], AC_MaxPreferredSpeed[7:4], AC_MinPreferredSpeed[3:0]
    //PORT_PARAM (PP_PSPP_DC, 0x133)          //DC_DirectIndirect[11:8], DC_MaxPreferredSpeed[7:4], DC_MinPreferredSpeed[3:0]
    PORT_PARAMS_END
  },

  // PCIe x4 Slot-2
  {
    0,                                      // End Flag will be DESCRIPTOR_TERMINATE_LIST
    DXIO_ENGINE_DATA_INITIALIZER (DxioPcieEngine, 12, 15, DxioHotplugDisabled, DEVICE_ID_DT),
    DXIO_PORT_DATA_INITIALIZER_PCIE_V7 (
      DxioPortEnabled,                      // Port Present
      2,                                    // Requested Device
      1,                                    // Requested Function
      DxioHotplugDisabled,                  // Hotplug
      DxioGenMaxSupported,                  // Max Link Speed
      DxioGenMaxSupported,                  // Max Link Capability
      DxioAspmL1,                           // DxioAspmL0sL1, // ASPM
      DxioAspmL11Enabled,                   // ASPM L1.1  // 0:disabled, 1:Enabled,   <BU02>: Set to Disable, OrigSet: DxioAspmL11Enabled
      DxioAspmL12Enabled,                   // ASPM L1.2  // 0:disabled, 1:Enabled    <BU02>: Set to Disable, OrigSet: DxioAspmL12Enabled
      DxioClkPmSupportEnabled,              // Clock PM
      CLK_REQ2,                             // CLKREQ#
  	  3                                     // EQ Preset, set to 0x3 by default
    )
    NO_PHY_PARAMS_DATA
    PORT_PARAMS_START
    PORT_PARAM (PP_PSPP_AC, 0x144),         //AC_DirectIndirect[11:8], AC_MaxPreferredSpeed[7:4], AC_MinPreferredSpeed[3:0]
    //PORT_PARAM (PP_PSPP_DC, 0x133)          //DC_DirectIndirect[11:8], DC_MaxPreferredSpeed[7:4], DC_MinPreferredSpeed[3:0]
    PORT_PARAMS_END
  },
};

DXIO_PORT_DESCRIPTOR PromSupportedData =
{
  // DEVICE_ID_PROMONTORY - PROMONTORY CHIP
      0,
      DXIO_ENGINE_DATA_INITIALIZER (DxioPcieEngine, 12, 15, HotPluggableDisabled, DEVICE_ID_DT), //DEVICE_ID_PROMONTORY
      DXIO_PORT_DATA_INITIALIZER_CHIP (
        DxioPortEnabled,                      // Port Present
        2,                                    // Requested Device
        1,                                    // Requested Function
        DxioHotplugDisabled,                  // Hotplug
        DxioGenMaxSupported,                  // Max Link Speed
        DxioGenMaxSupported,                  // Max Link Capability
        DxioAspmL1,                           // ASPM
        DxioAspmL11Disabled,                  // ASPM L1.1  // 0:disabled, 1:Enabled
        DxioAspmL12Disabled,                  // ASPM L1.2  // 0:disabled, 1:Enabled
        DxioClkPmSupportDisabled              // Clock PM
      )
};

DXIO_PORT_DESCRIPTOR DxioXGBEDevicePort[] =
{
  {// P0 - Ethernet Port0       //entry 0
       0,
       DXIO_ENGINE_DATA_INITIALIZER (DxioEthernetEngine, 8, 8, HotPluggableDisabled, 1),
       DXIO_PORT_DATA_INITIALIZER_ENET_V1 (
        DxioPortEnabled,            // Port Present
        0,    /// mPortNum
        0x8,  /// mPlatConf         (8 = SFP+ default , 1 = 10G/1G BackPlane , 6 = 10GBase-T)
        0,    /// mMdioId           (If MDIO is not used by this port, set this to 0)
        0x1F, /// mSuppSpeed        (For SFP+ connectors, 10G, 1G and 100M support should be indicated)
        0x1,  /// mConnType         ( 1 = SFP+ Connection I2C interface, 2 = MDIO PHY, 4= Backplane)
        0,    /// mMdioReset        (MDIO Reset Type, Integrated GPIO)
        0,    /// mMdioGpioResetNum (Integrated GPIO number for reset)
        1,    /// mSfpGpioAdd       (Lower I2C address of GPIO Expander PCA9535)
        0xE,  /// mTxFault,         (I/O1_6 connected SFPP_XGB00_0_TX_FAULT 0_0 to 0_7 = 8 , 1_0 to 1_6 = 7 , 8+7= 15-1 = E ( -1 for excluding for 0)
        0,    /// mRs,
        0xC,  /// mModAbs,
        0xD,  /// mRxLoss,
        0x2,  /// mSfpGpioMask,     (xx1xb = RS not supported)
        0x0,  /// mSfpMux,          (Lower address of Mux PCA 9545 or 111b if SFP+ directly connected to I2C) //SFP+ TWI Multiplexer
        0x0,  /// mSfpBusSeg,       (SFP BUS Segment. Downstream channels of PCA9545) //SFP+ TWI Bus Segment
        0x1C, /// mSfpMuxUpAdd,     (Upper address of Mux PCA 9545)
        0x0,  /// mRedriverAddress,
        1,    /// mRedriverInterface,  (1 => I2C, 0 => MDIO)
        0,    /// mRedriverLane,
        0,    /// mRedriverModel,
        0,    /// mRedriverPresent,
        0x1,  /// mPadMux0,         (Pad Mux Setting)
        0,    /// mPadMux1,         (Pad Mux Setting)
        0x0,  /// mMacAddressLo,    (TO BE FILLED IN BY CODE)
        0x0,  /// mMacAddressHi,    (TO BE FILLED IN BY CODE)
        0,    /// mTxEqPre,
        34,   /// mTxEqMain,
        24,   /// mTxEqPost
        1,    //TxEqApplicable  -   Always 1
        1,    //TxEqOvrRide     -   VR_XS_PMA_MP_12G_16G_25G_TX_EQ_CTRL1[6]
        0,    //TxEqDefCtrl     -   VR_XS_PMA_MP_12G_16G_25G_TX_EQ_CTRL1[7]
  //ETH_PORT_RXEQ
        1,    //RxEqApplicable  -   Always 1
        0x0E, //CtleBoost0     -   VR_XS_PMA_MP_16G_25G_RX_EQ_CTRL0[4:0]
        0x2,  //CtlePole0       -   VR_XS_PMA_MP_16G_25G_RX_EQ_CTRL0[6:5]
        0x6,//Vga2Gain0       -   VR_XS_PMA_MP_16G_25G_RX_EQ_CTRL0[10:8]
        0x6,//Vga1Gain0       -   VR_XS_PMA_MP_16G_25G_RX_EQ_CTRL0[14:12]
        0,//RxAdReq         -   VR_XS_PMA_MP_12G_16G_25G_RX_EQ_CTRL4[12]
        0,//RxDfeByp0       -   VR_XS_PMA_MP_16G_25G_RX_GENCTRL4[8]
        0x0F,//DfeTap1_0     -   VR_XS_PMA_MP_12G_16G_25G_DFE_TAP_CTRL0[7:0]
        0x0,//Rx0EqAttLvl     -   VR_XS_PMA_MP_12G_16G_25G_RX_ATTN_CTRL[2:0]
  //ETH_PORT_BOOST
        1,//BoostApplicable -   1
        1,//VboostEn0       -   VR_XS_PMA_MP_12G_16G_25G_TX_GENCTRL1[4]
        0xF,//Tx0Iboost       -   VR_XS_PMA_MP_12G_16G_25G_TX_BOOST_CTRL[3:0]
        5//TxVboostLvl     -   PCS_PMACONFIG_CONTROL10[2:0]
      )
  },
  {
       0,
       DXIO_ENGINE_DATA_INITIALIZER (DxioEthernetEngine, 9, 9, HotPluggableDisabled, 1),
       DXIO_PORT_DATA_INITIALIZER_ENET_V1 (
        DxioPortEnabled,            // Port Present
        1,    /// mPortNum
        0x6,  /// mPlatConf         (default , 1 = 10G/1G BackPlane , 6 = 10GBase-T , 8 = SFP+ )
        0,    /// mMdioId           (If MDIO is not used by this port, set this to 0)
        0x1F, /// mSuppSpeed        (For SFP+ connectors, 10G, 1G and 100M support should be indicated)
        2,    /// mConnType         (1 = SFP+ Connection I2C interface, 2 = MDIO PHY ,  4 = BackPlane)
        0,    /// mMdioReset        (MDIO Reset Type, Integrated GPIO)
        0,    /// mMdioGpioResetNum (Integrated GPIO number for reset)
        1,    /// mSfpGpioAdd       (Lower I2C address of GPIO Expander PCA9535)
        0xA,  /// mTxFault,         (I/O1_2 connected KR10G_PHY1_RST# 0_0 to 0_7 = 8 , 1_0 to 1_2 = 3 , 8+3= 11-1 = 0xA ( -1 for excluding for 0)
        0,    /// mRs,
        0x8,  /// mModAbs,          ( For MDIO config , mModAbs is not needed)
        0x9,  /// mRxLoss,          ( For MDIO config , mRxLoss is not needed)
        0x2,  /// mSfpGpioMask,     (xx1xb = RS not supported)
        0x3,  /// mSfpMux,          (Lower address of Mux PCA 9545 or 111b if SFP+ directly connected to I2C) //SFP+ TWI Multiplexer
        0x0,  /// mSfpBusSeg,       (SFP BUS Segment. Downstream channels of PCA9545) //SFP+ TWI Bus Segment
        0x1C, /// mSfpMuxUpAdd,     (Upper address of Mux PCA 9545)
        0x0,  /// mRedriverAddress,
        0,    /// mRedriverInterface,  (1 => I2C, 0 => MDIO)
        0,    /// mRedriverLane,
        0,    /// mRedriverModel,
        0,    /// mRedriverPresent,
        0x20, /// mPadMux0,          (Pad Mux Setting)
        0,    /// mPadMux1,          (Pad Mux Setting)
        0x0,  /// mMacAddressLo,     (TO BE FILLED IN BY CODE)
        0x0,  /// mMacAddressHi,     (TO BE FILLED IN BY CODE)
        0,    /// mTxEqPre,
        34,   /// mTxEqMain,
        24,   /// mTxEqPost
        1,    //TxEqApplicable  -   Always 1
        1,    //TxEqOvrRide     -   VR_XS_PMA_MP_12G_16G_25G_TX_EQ_CTRL1[6]
        0,    //TxEqDefCtrl     -   VR_XS_PMA_MP_12G_16G_25G_TX_EQ_CTRL1[7]
  //ETH_PORT_RXEQ
        1,    //RxEqApplicable  -   Always 1
        0x0E, //CtleBoost0     -   VR_XS_PMA_MP_16G_25G_RX_EQ_CTRL0[4:0]
        0x2,  //CtlePole0       -   VR_XS_PMA_MP_16G_25G_RX_EQ_CTRL0[6:5]
        0x6,//Vga2Gain0       -   VR_XS_PMA_MP_16G_25G_RX_EQ_CTRL0[10:8]
        0x6,//Vga1Gain0       -   VR_XS_PMA_MP_16G_25G_RX_EQ_CTRL0[14:12]
        0,//RxAdReq         -   VR_XS_PMA_MP_12G_16G_25G_RX_EQ_CTRL4[12]
        0,//RxDfeByp0       -   VR_XS_PMA_MP_16G_25G_RX_GENCTRL4[8]
        0x0F,//DfeTap1_0     -   VR_XS_PMA_MP_12G_16G_25G_DFE_TAP_CTRL0[7:0]
        0x0,//Rx0EqAttLvl     -   VR_XS_PMA_MP_12G_16G_25G_RX_ATTN_CTRL[2:0]
  //ETH_PORT_BOOST
        1,//BoostApplicable -   1
        1,//VboostEn0       -   VR_XS_PMA_MP_12G_16G_25G_TX_GENCTRL1[4]
        0xF,//Tx0Iboost       -   VR_XS_PMA_MP_12G_16G_25G_TX_BOOST_CTRL[3:0]
        0x5//TxVboostLvl     -   PCS_PMACONFIG_CONTROL10[2:0]
  )
  },
};
DXIO_PORT_DESCRIPTOR DxioSataPort =
{
    0,
    DXIO_ENGINE_DATA_INITIALIZER (DxioSATAEngine, 10, 11, HotPluggableDisabled, 1),
    DXIO_PORT_DATA_INITIALIZER_SATA_V2 (DxioPortEnabled, DxioSataChannelLong)  // Port Present
};
DXIO_PORT_DESCRIPTOR DxioPciePort =
{
    0,
    DXIO_ENGINE_DATA_INITIALIZER (DxioPcieEngine, 16, 19, HotPluggableDisabled, DEVICE_ID_SSD),
    DXIO_PORT_DATA_INITIALIZER_PCIE_V7 (
      DxioPortEnabled,                      // Port Present
      2,                                    // Requested Device
      2,                                    // Requested Function
      DxioHotplugDisabled,                  // Hotplug
      DxioGenMaxSupported,                  // Max Link Speed
      DxioGenMaxSupported,                  // Max Link Capability
      DxioAspmDisabled,                           // DxioAspmL0sL1, // ASPM
      DxioAspmL11Disabled,                   // ASPM L1.1  // 0:disabled, 1:Enabled,   <BU02>: Set to Disable, OrigSet: DxioAspmL11Enabled
      DxioAspmL12Disabled,                   // ASPM L1.2  // 0:disabled, 1:Enabled    <BU02>: Set to Disable, OrigSet: DxioAspmL12Enabled
      DxioClkPmSupportEnabled,              // Clock PM
      CLK_REQ4,                             // CLKREQ#
      3                                     // EQ Preset, set to 0x3 by default
    )
    NO_PHY_PARAMS_DATA
    PORT_PARAMS_START
    PORT_PARAM (PP_PSPP_AC, 0x144),         //AC_DirectIndirect[11:8], AC_MaxPreferredSpeed[7:4], AC_MinPreferredSpeed[3:0]
//    PORT_PARAM (PP_PSPP_DC, 0x133)          //DC_DirectIndirect[11:8], DC_MaxPreferredSpeed[7:4], DC_MinPreferredSpeed[3:0]
    PORT_PARAMS_END
  };
  /*
  // DEVICE_ID_WLAN                         // Entry 2
  {
    0,
    DXIO_ENGINE_DATA_INITIALIZER (DxioPcieEngine, 15, 15, DxioHotplugDisabled, DEVICE_ID_WLAN),
    DXIO_PORT_DATA_INITIALIZER_PCIE_V7 (
      DxioPortEnabled,                      // Port Present
      2,                                    // Requested Device
      2,                                    // Requested Function
      DxioHotplugDisabled,                  // Hotplug
      DxioGenMaxSupported,                  // Max Link Speed
      DxioGenMaxSupported,                  // Max Link Capability
      DxioAspmL1,                           // DxioAspmL0sL1, // ASPM
      DxioAspmL11Enabled,                   // ASPM L1.1  // 0:disabled, 1:Enabled,   <BU02>: Set to Disable, OrigSet: DxioAspmL11Enabled
      DxioAspmL12Enabled,                   // ASPM L1.2  // 0:disabled, 1:Enabled    <BU02>: Set to Disable, OrigSet: DxioAspmL12Enabled
      DxioClkPmSupportEnabled,              // Clock PM
      CLK_REQ6,                             // CLKREQ#
      3                                     // EQ Preset, set to 0x3 by default
    )
    NO_PHY_PARAMS_DATA
    PORT_PARAMS_START
    PORT_PARAM (PP_PSPP_AC, 0x144),         //AC_DirectIndirect[11:8], AC_MaxPreferredSpeed[7:4], AC_MinPreferredSpeed[3:0]
    PORT_PARAM (PP_PSPP_DC, 0x133)          //DC_DirectIndirect[11:8], DC_MaxPreferredSpeed[7:4], DC_MinPreferredSpeed[3:0]
    PORT_PARAMS_END
  },
};

DXIO_PORT_DESCRIPTOR DxioSataOddPort =
{
    0,
    DXIO_ENGINE_DATA_INITIALIZER (DxioSATAEngine, 10, 11, HotPluggableDisabled, 1),
    DXIO_PORT_DATA_INITIALIZER_SATA_V2 (DxioPortEnabled, DxioSataChannelLong)  // Port Present
};


DXIO_PORT_DESCRIPTOR DxioSdCardPort =
{
    0,
    DXIO_ENGINE_DATA_INITIALIZER (DxioPcieEngine, 14, 14, DxioHotplugDisabled, DEVICE_ID_SD),
    DXIO_PORT_DATA_INITIALIZER_PCIE_V7 (
      DxioPortEnabled,                      // Port Present
      2,                                    // Requested Device
      3,                                    // Requested Function
      DxioHotplugBasic,                     // Hotplug
      DxioGenMaxSupported,                  // Max Link Speed
      DxioGenMaxSupported,                  // Max Link Capability
      DxioAspmL1,                           // DxioAspmL0sL1, // ASPM
      DxioAspmL11Enabled,                   // ASPM L1.1  // 0:disabled, 1:Enabled,   <BU02>: Set to Disable, OrigSet: DxioAspmL11Enabled
      DxioAspmL12Enabled,                   // ASPM L1.2  // 0:disabled, 1:Enabled    <BU02>: Set to Disable, OrigSet: DxioAspmL12Enabled
      DxioClkPmSupportEnabled,              // Clock PM
      CLK_REQ5,                             // CLKREQ#
      3                                     // EQ Preset, set to 0x3 by default
    )
    NO_PHY_PARAMS_DATA
    PORT_PARAMS_START
    PORT_PARAM (PP_PSPP_AC, 0x144),         //AC_DirectIndirect[11:8], AC_MaxPreferredSpeed[7:4], AC_MinPreferredSpeed[3:0]
    PORT_PARAM (PP_PSPP_DC, 0x133)          //DC_DirectIndirect[11:8], DC_MaxPreferredSpeed[7:4], DC_MinPreferredSpeed[3:0]
    PORT_PARAMS_END
};

DXIO_PORT_DESCRIPTOR DxioDtSsd1Port =
{
    0,
    DXIO_ENGINE_DATA_INITIALIZER (DxioPcieEngine, 8, 9, HotPluggableDisabled, DEVICE_ID_DT),
    DXIO_PORT_DATA_INITIALIZER_PCIE_V7 (
      DxioPortEnabled,                      // Port Present
      1,                                    // Requested Device
      2,                                    // Requested Function
      DxioHotplugDisabled,                  // Hotplug
      DxioGenMaxSupported,                  // Max Link Speed
      DxioGenMaxSupported,                  // Max Link Capability
      DxioAspmL1,                           // DxioAspmL0sL1, // ASPM
      DxioAspmL11Enabled,                   // ASPM L1.1  // 0:disabled, 1:Enabled,   <BU02>: Set to Disable, OrigSet: DxioAspmL11Enabled
      DxioAspmL12Enabled,                   // ASPM L1.2  // 0:disabled, 1:Enabled    <BU02>: Set to Disable, OrigSet: DxioAspmL12Enabled
      DxioClkPmSupportEnabled,              // Clock PM
      CLK_REQ1,                             // CLKREQ#
      3                                     // EQ Preset, set to 0x3 by default
    )
    NO_PHY_PARAMS_DATA
    PORT_PARAMS_START
    PORT_PARAM (PP_PSPP_AC, 0x144),         //AC_DirectIndirect[11:8], AC_MaxPreferredSpeed[7:4], AC_MinPreferredSpeed[3:0]
    PORT_PARAM (PP_PSPP_DC, 0x133)          //DC_DirectIndirect[11:8], DC_MaxPreferredSpeed[7:4], DC_MinPreferredSpeed[3:0]
    PORT_PARAMS_END
};

DXIO_PORT_DESCRIPTOR DxioGbePort =
{
    0,
    DXIO_ENGINE_DATA_INITIALIZER (DxioPcieEngine, 13, 13, DxioHotplugDisabled, DEVICE_ID_LOM),
    DXIO_PORT_DATA_INITIALIZER_PCIE_V7 (
      DxioPortEnabled,                      // Port Present
      2,                                    // Requested Device
      1,                                    // Requested Function
      DxioHotplugDisabled,                  // Hotplug
      DxioGenMaxSupported,                  // Max Link Speed
      DxioGenMaxSupported,                  // Max Link Capability
      DxioAspmL1,                           // DxioAspmL0sL1, // ASPM
      DxioAspmL11Enabled,                   // ASPM L1.1  // 0:disabled, 1:Enabled,   <BU02>: Set to Disable, OrigSet: DxioAspmL11Enabled
      DxioAspmL12Enabled,                   // ASPM L1.2  // 0:disabled, 1:Enabled    <BU02>: Set to Disable, OrigSet: DxioAspmL12Enabled
      DxioClkPmSupportEnabled,              // Clock PM
      CLK_REQ3,                             // CLKREQ#
      3                                     // EQ Preset, set to 0x3 by default
    )
    NO_PHY_PARAMS_DATA
    PORT_PARAMS_START
    PORT_PARAM (PP_PSPP_AC, 0x144),         //AC_DirectIndirect[11:8], AC_MaxPreferredSpeed[7:4], AC_MinPreferredSpeed[3:0]
    PORT_PARAM (PP_PSPP_DC, 0x133)          //DC_DirectIndirect[11:8], DC_MaxPreferredSpeed[7:4], DC_MinPreferredSpeed[3:0]
    PORT_PARAMS_END
};

DXIO_PORT_DESCRIPTOR DxioWwanPciex1Port =
{
    0,
    DXIO_ENGINE_DATA_INITIALIZER (DxioPcieEngine, 12, 12, DxioHotplugDisabled, DEVICE_ID_DTx1),
    DXIO_PORT_DATA_INITIALIZER_PCIE_V7 (
      DxioPortEnabled,                      // Port Present
      1,                                    // Requested Device
      3,                                    // Requested Function
      DxioHotplugDisabled,                  // Hotplug
      DxioGenMaxSupported,                  // Max Link Speed
      DxioGenMaxSupported,                  // Max Link Capability
      DxioAspmL1,                           // DxioAspmL0sL1, // ASPM
      DxioAspmL11Enabled,                   // ASPM L1.1  // 0:disabled, 1:Enabled,   <BU02>: Set to Disable, OrigSet: DxioAspmL11Enabled
      DxioAspmL12Enabled,                   // ASPM L1.2  // 0:disabled, 1:Enabled    <BU02>: Set to Disable, OrigSet: DxioAspmL12Enabled
      DxioClkPmSupportEnabled,              // Clock PM
      CLK_REQ2,                             // CLKREQ#
      3                                     // EQ Preset, set to 0x3 by default
    )
    NO_PHY_PARAMS_DATA
    PORT_PARAMS_START
    PORT_PARAM (PP_PSPP_AC, 0x144),         //AC_DirectIndirect[11:8], AC_MaxPreferredSpeed[7:4], AC_MinPreferredSpeed[3:0]
    PORT_PARAM (PP_PSPP_DC, 0x133)          //DC_DirectIndirect[11:8], DC_MaxPreferredSpeed[7:4], DC_MinPreferredSpeed[3:0]
    PORT_PARAMS_END
};
*/

//
// CPM Device Path Table
//
AMD_CPM_DEVICE_PATH_TABLE gCpmDevicePathTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_DEVICE_PATH, sizeof (gCpmDevicePathTable) / sizeof (UINT8), 0, 0, 0, 0x0000000F},
  {
//  {AMD_CPM_DISPLAY_FEATURE_SUPPORT FeatureMask; // See AMD_CPM_DISPLAY_FEATURE_SUPPORT
//   UINT8                           IsDgpu;      // Is Igpu or Dgpu. 0: iGpu; 1: dGpu
//   AMD_CPM_PCI_DEVICE_FUNCTION     Bridge;      // Device and function number of PCIe Bridge
//   AMD_CPM_PCI_DEVICE_FUNCTION     Device;      // Device and function number of Display Device
//   UINT8                           DeviceId;    // Device Id to control GPIO for selecting Smart Mux Switch
//   UINT8                           Mode;        // Power mode for setting Smart Mux Switch
//   UINT8                           DeviceIdVcc; // Device Id for Vcc when D3Cold is supported}
    {0x80000003, 0x00, {8, 1}, {0, 0}},                                                    // PowerXpress|HCF, iGPU, (0,8,1)/(0,0)
    {0x8000000B, 0x01, {1, 1}, {0, 0}, DEVICE_ID_VGAMUXSEL, CPM_POWER_ON, DEVICE_ID_MXM},  // PowerXpress|HCF, dGPU, (0,1,1)/(0,0)
    {0x00000000, 0x00, {0, 0}, {0, 0}},
  }
};

//
// CPM Display Feature Table
//
AMD_CPM_DISPLAY_FEATURE_TABLE gCpmDisplayFeatureTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_DISPLAY_FEATURE, sizeof (gCpmDisplayFeatureTable) / sizeof (UINT8), 0, 0, 0, 0x0000000F},
  0,                            // UINT8  FunctionDisableMask;  // BIT0: Disable SetSsid. BIT1: Disable AddSsdt
  DEVICE_ID_MXM,                // UINT8  MxmDeviceId;          // Device Id for MXM
  0xFF,                         // UINT8  MxmOverTempEvent;     // GEVENT pin number for MXM_OVERT#
  2,                            // UINT8  MxmOverTempStateId;   // Forced Power State Id if MXM_OVERT# is low
  0x00,                         // UINT8  DisplayConnectEvent;  // GEVENT pin number for Discrete GPU display connect/disconnect event
  0,                            // UINT8  DockingDeviceId;      // Bit[0-6]: Device Id for docking detection if Bit7 = 0  // Forced docking status if Bit7 = 1
  0,                            // UINT8  MuxFlag;              // Smart Mux Flag, Bit[2:0] = ATPX Function 1 Bit[20:18], Bit[4] = Acpi Device present, Bit[5] = VGA _DEP
  DEVICE_ID_VGAMUXVCC,          // UINT8  DisplayMuxDeviceId;   // Device Id for Display Smart Mux VCC/Reset
  0,                            // UINT8  I2xMuxDeviceId;       //
  10,                           // UINT8  AtpxConnector8Number; // Number of reported display connectors in ATPX sub-function 8
  {                             // AMD_CPM_DISPLAY_CONNECTOR_8 AtpxConnector8[AMD_ATPX_CONNECTOR_8_SIZE]; // The Connector information for ATPX sub-function 8
    {0x05, 0x00, 0x00, 0x0110}, // Connector #0: LCD1 on iGPU
    {0x07, 0x03, 0x00, 0x0210}, // Connector #1: DFP1 on iGPU
    {0x07, 0x07, 0x00, 0x0220}, // Connector #2: DFP2 on iGPU
    {0x07, 0x09, 0x00, 0x0230}, // Connector #3: DFP3 on iGPU
    {0x07, 0x0A, 0x00, 0x0240}, // Connector #4: DFP4 on iGPU
    {0x01, 0x00, 0x01, 0x0110}, // Connector #5: LCD1 on dGPU
    {0x03, 0x03, 0x01, 0x0210}, // Connector #6: DFP1 on dGPU
    {0x03, 0x07, 0x01, 0x0220}, // Connector #7: DFP2 on dGPU
    {0x03, 0x09, 0x01, 0x0230}, // Connector #8: DFP3 on dGPU
    {0x03, 0x0A, 0x01, 0x0240}, // Connector #9: DFP4 on dGPU
  },
  0,                            // UINT8 AtpxConnector9Number;  // Number of reported display connectors in ATPX sub-function 9
  {                             // AMD_CPM_DISPLAY_CONNECTOR_9 AtpxConnector9[AMD_ATPX_CONNECTOR_9_SIZE]; // The Connector information for ATPX sub-function 9
    {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00},
  },
  0x59,                         // UINT32 AtifSupportedNotificationMask; // Supported Notifications Mask in ATIF sub-function 0
  7,                            // UINT8  AtifDeviceCombinationNumber;   // The number of Display Device Combination
  {                             // UINT8  AtifDeviceCombinationBuffer[AMD_ATIF_DEVICE_COMBINATION_BUFFER_SIZE]; // The data of Display Device Combination
    0x01, 0x02, 0x08, 0x80, 0x03, 0x09, 0x81 // AtifDeviceCombinationBuffer[20];
  },
  {                             // UINT8  Atif16Buffer[0x100];           // The data for Query Brightness Transfer Characteristics
    0x6C, 0x00,                 // WORD Structure Size : 0x6C
    0x00, 0x00,                 // WORD Flags          : Reserved
    0x00,                       // BYTE        Error Code     : 0x00
    0x64,                       // BYTE        AC Level       : 100%
    0x20,                       // BYTE        DC Level       : 32%
    0x0C,                       // BYTE        Minimum signal : 12
    0xFF,                       // BYTE        Maximum signal : 255
    0x31,                       // BYTE        Count          : 49
    0x02, 0x0E,                 // BYTE-BYTE    First Lumi/Signal: 2% - 14
    0x04, 0x10,                 // BYTE-BYTE
    0x06, 0x12,                 // BYTE-BYTE
    0x08, 0x15,                 // BYTE-BYTE
    0x0A, 0x17,                 // BYTE-BYTE
    0x0C, 0x1A,                 // BYTE-BYTE
    0x0E, 0x1D,                 // BYTE-BYTE
    0x10, 0x20,                 // BYTE-BYTE
    0x12, 0x23,                 // BYTE-BYTE
    0x14, 0x26,                 // BYTE-BYTE
    0x16, 0x29,                 // BYTE-BYTE
    0x18, 0x2C,                 // BYTE-BYTE
    0x1A, 0x30,                 // BYTE-BYTE
    0x1C, 0x34,                 // BYTE-BYTE
    0x1E, 0x37,                 // BYTE-BYTE
    0x20, 0x3B,                 // BYTE-BYTE
    0x22, 0x3E,                 // BYTE-BYTE
    0x24, 0x43,                 // BYTE-BYTE
    0x26, 0x47,                 // BYTE-BYTE
    0x28, 0x4B,                 // BYTE-BYTE
    0x2A, 0x50,                 // BYTE-BYTE
    0x2C, 0x54,                 // BYTE-BYTE
    0x2E, 0x58,                 // BYTE-BYTE
    0x30, 0x5D,                 // BYTE-BYTE
    0x32, 0x62,                 // BYTE-BYTE
    0x34, 0x67,                 // BYTE-BYTE
    0x36, 0x6C,                 // BYTE-BYTE
    0x38, 0x71,                 // BYTE-BYTE
    0x3A, 0x76,                 // BYTE-BYTE
    0x3C, 0x7B,                 // BYTE-BYTE
    0x3E, 0x81,                 // BYTE-BYTE
    0x40, 0x87,                 // BYTE-BYTE
    0x42, 0x8C,                 // BYTE-BYTE
    0x44, 0x92,                 // BYTE-BYTE
    0x46, 0x98,                 // BYTE-BYTE
    0x48, 0x9E,                 // BYTE-BYTE
    0x4A, 0xA4,                 // BYTE-BYTE
    0x4C, 0xAB,                 // BYTE-BYTE
    0x4E, 0xB1,                 // BYTE-BYTE
    0x50, 0xB7,                 // BYTE-BYTE
    0x52, 0xBE,                 // BYTE-BYTE
    0x54, 0xC5,                 // BYTE-BYTE
    0x56, 0xCC,                 // BYTE-BYTE
    0x58, 0xD3,                 // BYTE-BYTE
    0x5A, 0xDA,                 // BYTE-BYTE
    0x5C, 0xE1,                 // BYTE-BYTE
    0x5E, 0xE8,                 // BYTE-BYTE
    0x60, 0xF0,                 // BYTE-BYTE
    0x62, 0xF7                  // BYTE-BYTE   Last Lumi/Signal: 98% - 250
  },
  BOMACO_EN,                    // UINT32 MacoGpio;         // BOMACO_EN GPIO pin number for Discrete GPU BxMACO Support        //
  MXM_POWER_ENABLE,             // UINT32 MxmPwrEnGpio;     // MXM_POWER_ENABLE GPIO pin number for Discrete GPU BOMACO Support // ECRAM BIT33
  0,                            // UINT8  TimeMacoUpPwrDn;  // MACO Entry (us): MACO_EN assert to PWR_EN deassert     // Min: 10us
  0,                            // UINT8  TimePwrDnRstDn;   // MACO Entry (ms): PWR_EN deassert to PERSTb assert      // Min: 20ms
  0,                            // UINT8  TimePwrUpRstUp;   // MACO Exit  (ms): PWR_EN assert to PERSTb deassert      // Min: 20ms
  0,                            // UINT8  TimeRstUpMacoDn;  // MACO Exit  (ms): PERSTb deassert to MACO_EN deassert   // Min: 1ms
  0,                            // UINT32 PcdSetS0i3PmeTurnOffDelay;  // S0i3 PmeTurnOff Delay Time: Copy from gEfiAmdAgesaPkgTokenSpaceGuid.PcdSetS0i3PmeTurnOffDelay
  1,                            // UINT8  DgpuSsidSvid;     // Discrete GPU's SSID/SVID: Program Discrete GPU's SSID/SVID depends on HybridGraphics setting
  0,                            // UINT8  AtcsFunction9Support;   // ATCS Function 9 Support
  { 0, 0, 0, 0, 0, 0, 0 },      // UINT8  AtcsFunction9Buffer[7]; // ATCS Function 9 Buffer
  0,                            // UINT8  AtifFunction23Support;  // ATIF Function 23 Support
  { 0, 0, 0, 0, 0, 0, 0 },      // UINT8  AtifFunction23Buffer[7];// ATIF Function 23 Buffer
  10,                           // UINT8  TimePeRstToWakeL23;     // EVAL_RST to LC_WAKE_FROM_L23 Time 1 ~ 30(ms)
  100,                          // UINT8  TimeDlActToCfgAcc;      // DL_ACTIVE to Config Access Delay Time 1 ~ 100(ms)
  0,                            // UINT8  AtifNotifyCommandCode;  // Atif Notify Command Code - 0 = 0x81 or 0xD0 ~ 0xD9
  0,                            // UINT8  AtifFun21Support;       // ATIF Function 21 Support - 0: Disabled, 1: Enabled
  0,                            // UINT8  AtifFun22Support;       // ATIF Function 22 Support - 0: Disabled, 1: Target = Undefined, 2: Target = iGPU, 3: Target = dGPU
  0,                            // UINT32 AtifFun22Value;         // ATIF Function 22 Value
  0,                            // UINT8  DgpuStartStopSupport;   // CpmDgpuStartStopSupport
  CPM_SIGNATURE_64 ('S', 'M', 'U', 'X', '1', '2', '3', '4'),  // UINT64 SmartMux_HID; // Smart Mux ACPI _HID String
  0,                            // UINT8  SmartMuxMDMSupportLevel;    // Smart Mux MDM Support Level
  0,                            // UINT8  SmartMuxFirstConnectedGPU;  // Smart Mux First Connected GPU
  3,                            // UINT8  SmartMuxAcpiMethodLocation; // Smart Mux ACPI Method Location
};

//
// CPM Zero Power Odd Table RevB
//
AMD_CPM_ZERO_POWER_ODD_TABLE      gCpmZeroPowerOddTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_ZERO_POWER_ODD, sizeof (gCpmZeroPowerOddTable) / sizeof (UINT8), 0, 0, 0x0, 0x0000000F},
  DEVICE_ID_ODD,        // UINT8 DeviceId;            // Device Id
  ZPODD_DA_GPIO,        // UINT8 EventPin1;           // Event Pin Number for ODD_DA#
  ZPODD_PLUGIN_GPIO,    // UINT8 EventPin2;           // Event Pin Number for ODD_PLUGIN#
  ZPODD_DUMMY_EVENT,    // UINT8 EventPin3;           // Dummy Event
  ZPODD_SATA_MASK,      // UINT8 SataModeSupportMask; // Mask of SATA mode to support  ///< BIT0: IDE Mode ///< BIT1: AHCI Mode  ///< BIT2: RAID Mode  ///< BIT3: AMD AHCI Mode
  ZPODD_SATA_PORT,      // UINT8 SataPortId;          // Sata Port Number for Odd
  ZPODD_DA_SOURCE,      // UINT8 EventSource1;        // Source of ODD_DA#     - 0: FCH GPIO Pin, 1: KBC GPIO Pin, 2: ECRAM GPIO Pin
  ZPODD_PLUGIN_SOURCE,  // UINT8 EventSource2;        // Source of ODD_PLUGIN# - 0: FCH GPIO Pin, 1: KBC GPIO Pin, 2: ECRAM GPIO Pin
  ZPODD_DA_FALLING,     // UINT8 QEventFalling1;      // QEvent Number for ODD_DA#     FALLING  if EventSource1 = 1 or 2
  ZPODD_DA_RISING,      // UINT8 QEventRising1;       // QEvent Number for ODD_DA#     RISING   if EventSource1 = 1 or 2
  ZPODD_PLUGIN_FALLING, // UINT8 QEventFalling2;      // QEvent Number for ODD_PLUGIN# FALLING if EventSource2 = 1 or 2
  ZPODD_PLUGIN_RISING,  // UINT8 QEventRising2;       // QEvent Number for ODD_PLUGIN# RISING  if EventSource2 = 1 or 2
  0,                    // UINT8 Bus;                 // Bus number of SATA Bridge
  {8, 2},               // AMD_CPM_PCI_DEVICE_FUNCTION Bridge;  // Device and function number of SATA Bridge
  {0, 0},               // AMD_CPM_PCI_DEVICE_FUNCTION Device;  // Device and function number of SATA Controller
  0,                    // UINT32 PciBusAslName;      // ASL name of SATA PCI Bus     // 0 is treated as PCI0 // Example: PCI0 (50 43 49 30) = 0x30494350
  0,                    // UINT32 BridgeAslName;      // ASL name of SATA PCI Bridge  // 0 is treated as GP18 // Example: GP18 (47 50 31 38) = 0x38315047
  0                     // UINT32 DeviceAslName;      // ASL name of SATA PCI Device  // 0 is treated as SATA // Example: SATA (53 41 54 41) = 0x41544153
};


//
// Save Context Table
//
AMD_CPM_SAVE_CONTEXT_TABLE        gCpmSaveContextTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_SAVE_CONTEXT, sizeof (gCpmSaveContextTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  0x05, // UINT8 BufferType;   // Buffer Type. 5: BIOS RAM. Other: Reserved.
  0x50, // UINT8 BufferOffset; // Buffer Offset
  0x10, // UINT8 BufferSize;   // Buffer Size
};

//
// UCSI Table
//
AMD_CPM_UCSI_TABLE         gCpmUcsiTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_UCSI, sizeof (gCpmUcsiTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  0x00, // UINT32 EcUcsiLocation;   // EC UCSI Location. // 0: EmbeddedControl. others: System Memory Address
  0x60, // UINT8  EcVersionLocation;// EC UCSI USB Type-C Interface Version Number Offset
  0x64, // UINT8  EcCciLocation;    // EC UCSI USB Type-C Command Status & Connector Change Indication Offset
  0x68, // UINT8  EcCtlLocation;    // EC UCSI USB Type-C Control Offset
  0x70, // UINT8  EcMgiLocation;    // EC UCSI USB Type-C Message In Offset
  0x80, // UINT8  EcMgoLocation;    // EC UCSI USB Type-C Message Out Offset
  0xE0, // UINT8  EcUcsiCommand;    // EC UCSI Command
  0x668,// UINT16 EcUcsiCommandIo;  // EC UCSI Command IO Port
  0x50, // UINT8  EcUcsiQEvent;     // EC UCSI QEvent Number
  {     // UINT8  Reserved[48];     // Reserve for UCSI OPM ACPI OperationRegion CpmUcsiOperationRegion
    0x00
  },
  0x3,  // UINT8  EcUcsiPortNumber; // EC UCSI USB Type-C port number (2 or 3)
  {     // AMD_UCSI_PLD_INFO Port[UCSI_PORT_NUMBER];              // UCSI Port _PLD information
    {0x00000082, 0x00000000, 0x02001D51, 0x00000001, 0xFFFFFFFF}, // UCSI Port 0 _PLD information
    {0x00000082, 0x00000000, 0x02801D51, 0x00000001, 0xFFFFFFFF}, // UCSI Port 1 _PLD information
    {0x00000082, 0x00000000, 0x03001D51, 0x00000001, 0xFFFFFFFF}, // UCSI Port 2 _PLD information
  }
};

#if 0
// Lilac <TODO: Later review>
//
// CPM DT Card Table
//
AMD_CPM_OTHER_HOTPLUG_CARD_TABLE  gCpmDesktopSlotTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table contect
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_PCIE_OTHER_HOTPLUG_CARD, sizeof (gCpmDesktopSlotTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  1,                    // UINT8 Number;        // Card Number: 0 ~ 2
  DTDEVICE_BRIDGE_DEV,  // UINT8 Device0;       // Device Number of PCIe Bridge for Card 0
  DTDEVICE_BRIDGE_FUN,  // UINT8 Function0;     // Function Number of PCIe Bridge for Card 0
  2,                    // UINT8 EventSource0;  // Event Source for Card 0 - 0: FCH GPIO Pin, 1: KBC GPIO Pin, 2: ECRAM GPIO Pin
  0,                    // UINT8 EventPin0;     // Event Number for Card 0 if EventSource0 = 0
  0x24,                 // UINT8 QEventFalling0;// QEvent Number for Card 0 - FALLING if EventSource0 = 1 or 2
  0x25,                 // UINT8 QEventRising0; // QEvent Number for Card 0 - RISING  if EventSource0 = 1 or 2
  1,                    // UINT8 HotPlugMode0;  // Pcie HotPlug Mode of for Card 0 - 0: Basic Mode, 1: Enhanced Mode
  DEVICE_ID_DT,         // UINT8 DeviceId0;     // Device Id for Card 0
  0xFF,                 // UINT8 Device1;       // Device Number of PCIe Bridge for Card 1
  0xFF,                 // UINT8 Function1;     // Function Number of PCIe Bridge for Card 1
  0xFF,                 // UINT8 EventSource1;  // Event Source for Card 1 - 0: FCH GPIO Pin, 1: KBC GPIO Pin, 2: ECRAM GPIO Pin
  0xFF,                 // UINT8 EventPin1;     // Event Number for Card 1 if EventSource1 = 0
  0xFF,                 // UINT8 QEventFalling1;// QEvent Number for Card 1 - FALLING if EventSource1 = 1 or 2
  0xFF,                 // UINT8 QEventRising1; // QEvent Number for Card 1 - RISING  if EventSource1 = 1 or 2
  0xFF,                 // UINT8 HotPlugMode1;  // Pcie HotPlug Mode of for Card 1 - 0: Basic Mode, 1: Enhanced Mode
  0xFF                  // UINT8 DeviceId1;     // Device Id  for Card 1
};
#endif
/*
//
// Low Power Ethernet Table
//
AMD_CPM_LOW_POWER_ETHERNET_TABLE  gCpmLowPowerEthernetTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_PCIE_LOW_POWER_ETHERNET, sizeof (gCpmLowPowerEthernetTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  1,                    // UINT8  Device;                     ///< Device Number of PCIe Bridge for Low Power Ethernet
  3,                    // UINT8  Function;                   ///< Function Number of PCIe Bridge for Low Power Ethernet
  0x46,                 // UINT8  QEventCablePlugOut;         ///< QEvent Number for Ethernet Cable Plug Out
  0x47,                 // UINT8  QEventCablePlugIn;          ///< QEvent Number for Ethernet Cable Plug In
  LAN_LOW_PWR,          // UINT32 LanLowPowerPin;             ///< GPIO Pin Number for LAN_LOW_PWR
  DEVICE_ID_LAN         // UINT8  DeviceId;                   ///< Device Id for Low Power Ethernet
};

*/

//
// Thunderbolt Table
//
AMD_CPM_THUNDERBOLT_TABLE         gCpmThunderboltTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_THUNDERBOLT, sizeof (gCpmThunderboltTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  0,            // UINT8  TBSupport;        ///< Enable/Disable Thunderbolt - 0: Disabled, 1: Enabled
  DEVICE_ID_MXM,// UINT8  DeviceId;         ///< CPM Device Id for TB
  0,            // UINT8  SocketDieBridge;  ///< SocketDieBridge Number of PCIe Bridge for TB
  1,            // UINT8  Device;           ///< Device Number of PCIe Bridge for TB
  1,            // UINT8  Function;         ///< Function Number of PCIe Bridge for TB
  0x214,        // UINT32 ForcePwrGpio;     ///< The GPIO number for Force PWR - Follow CpmSetGpio paramater Pin format.
  2,            // UINT8  PlugEventSource;  ///< Plug Event Source - 0: FCH GEvent, 2: EC QEvent, 6: SMBUS GPIO Event
  0x51,         // UINT8  PlugEventNumber;  ///< The number of QEvent(Falling)/QEvent+1(Rising) or GEvent for Plug Event in ACPI ASL code
  0,            // UINT8  NativeHotPlug;    ///< NativeHotPlug - 0: Disabled, 1: Enabled
  TBT_RTD3_PWR, // UINT32 Rtd3PwrGpio; ///< Thunderbolt HR RTD3 Power GPIO Pin number - Follow CpmSetGpio paramater Pin format.
  0,            // UINT8  BootMode;         ///< BootMode - 0: S0, 1: S1, 3: S3, 4: S4, 5: S5
  0,            // UINT8  RootPortBus;      ///< RootPortBus - TB Root Port Bus number
  0x68,         // UINT8  ThunderboltSwSmi; ///< ThunderboltSwSmi - Thunderbolt Sw Smi
  0,            // UINT8  SecurityLevel;    ///< SecurityLevel - 0: SL0, 1: SL1, 2: SL2, 3: SL3
  0,            // UINT8  ForcePwrEnable;   ///< ForcePwrEnable - 0: Disabled, 1: Enabled
  0,            // UINT8  BootFromTBEnable; ///< BootFromTBEnable - 0: Disabled, 1: Enabled
  0,            // UINT8  BootFromUSBEnable;///< BootFromUSBEnable - 0: Disabled, 1: Enabled
  0,            // UINT8  AssignResource;   ///< AssignResource - 0: Disabled, 1: Enabled
  0,            // UINT8  ResourceSize;     ///< ResourceSize - Thunderbolt MMIO Resource Size
  0,            // UINT8  TBL1SSSupport;    ///< TBL1SSSupport - 0: Disable Thunderbolt L1SS, 1: Enable Thunderbolt L1SS
  0,            // UINT8  HotPlugCodeBase;  ///< HotPlugCodeBase - 0: PF6 code base, 1: AM4 code base
};

//
// Wireless LAN Recovery Table
//
AMD_CPM_WIRELESS_LAN_RECOVERY_TABLE  gCpmWirelessLanRecoveryTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_WIRELESS_LAN_RECOVERY, sizeof (gCpmWirelessLanRecoveryTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  2,                                             // UINT8  Device;               ///< Device Number of SOC GPPx PCIe Bridge for Wireless LAN
  2,                                             // UINT8  Function;             ///< Function Number of SOC GPPx PCIe Bridge for Wireless LAN
  200,                                           // UINT32 WlanResetDelay;       ///< Delay time for assert the WLAN RESET         // Delay time in ms
  DEVICE_ID_WLAN,                                // UINT8  DeviceId;             ///< Device Id for Wireless LAN
  CPM_SIGNATURE_32 ('P', 'C', 'I', '0'),         // UINT32 PciBusAslName;        ///< ASL name of PCI Bus                          // Example: PCI0 (50 43 49 30) = 0x30494350
  CPM_SIGNATURE_32 ('W', 'L', 'A', 'N'),         // UINT32 WlanDeviceAslName;    ///< ASL name of Wireless LAN                     // Example: WLAN (57 4C 41 4E) = 0x4E414C57
  0,                                             // UINT8  WlanLocation;         ///< Wireless LAN Location                        // 0 = SOC GPPx PCIe Bridge, 1 = PT, 2 = BXB PCIe Bridge
  0,                                             // UINT32 PtBxbUspAslName;      ///< ASL name of PT or BXB Upstream Switch Port   // Example: PT02 (50 54 30 32) = 0x32305450
  0,                                             // UINT32 PtBxbDspAslName;      ///< ASL name of PT or BXB Downstream Switch Port // Example: PT29 (50 54 32 39) = 0x39325450
  0,                                             // UINT32 WlanPldrSupport       ///< Control for the WLAN PLDR support            // 0 = Disable, 1 = Enable, 2 = Dummy Reset
  CPM_SIGNATURE_32 ('G', 'P', '1', '7'),         // UINT32 BtXhciUspAslName;     ///< ASL name of BT's XHCI Upstream Port          // Example: GP17 (47 50 31 37) = 0x37315047
  CPM_SIGNATURE_32 ('X', 'H', 'C', '1'),         // UINT32 BtXhciAslName;        ///< ASL name of BT's XHCI                        // Example: XHC1 (58 48 43 31) = 0x31434858
  CPM_SIGNATURE_32 ('R', 'H', 'U', 'B'),         // UINT32 BtXhciRootHubAslName; ///< ASL name of BT's root hub                    // Example: RHUB (52 48 55 42) = 0x42554852
  0,                                             // UINT32 BtXhciExtHubAslName;  //< ASL name of BT's external usb hub             // 0 for non-external hub design
  CPM_SIGNATURE_32 ('P', 'R', 'T', '3'),         // UINT32 BtPortAslName;        ///< ASL name of BT's USB port                    // Example: PRT4 (50 52 54 34) = 0x34545250
  DEVICE_ID_BT,                                  // UINT8  BtDeviceId;           ///< Device Id for BT
  200,                                           // UINT32 BtResetDelay;         ///< Delay time for assert the BT RESET           // Delay time in ms
  0,                                             // UINT32 BtPldrSupport;        ///< Control for the BT PLDR support              // 0 = Disable, 1 = Enable, 2 = Dummy Reset
};

//
// Wireless Botton Table
//
AMD_CPM_WIRELESS_BUTTON_TABLE  gCpmWirelessButtonTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_WIRELESS_BUTTON, sizeof (gCpmWirelessButtonTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  {0, 0, 0, 0},                     ///< AMD_CPM_PCI_DEVICE_FUNCTION  Bridge[4];         ///< Device & Function Number of PCIe Bridge
  0,                                ///< UINT8                        EventPin;          ///< GEVENT Pin
  0,                                ///< UINT8                        DeviceIdRadio;     ///< Device Id to control radio
  0,                                ///< UINT8                        DeviceIdPower;     ///< Device Id to control power
  0,                                ///< UINT8                        DeviceIdOther;     ///< Device Id to control other device
  0x28,                             ///< UINT8                        HidNotifyQEvent;   ///< Q event number for notify Airplane mode on/off
};

//
// PCI Reserved Resource Table - for PciHotPlugInit
//
AMD_CPM_PCI_RES_RES_TABLE         gCpmPciResResTable = {
//{  UINT32 TableSignature;   // Signature of CPM table
//   UINT16 TableSize;        // Table size
//   UINT8  FormatRevision;   // Revision of table format
//   UINT8  ContentRevision;  // Revision of table content
//   UINT32 PlatformMask;     // The mask of platform table supports
//   UINT32 Attribute;        // Table attribute},
  {CPM_SIGNATURE_PCI_RESERVED_RESOURCE, sizeof (gCpmPciResResTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  {
    {
      IUSB4_GPP0,     // UINT8  DevType;                      // Device Support List is in AMD_CPM_DEVICE_TYPE
      0,              // UINT8  SocketDieBridge;              // SocketDieBridge Number of GPP/SWUS/SWDS
      {3, 1},         // AMD_CPM_PCI_DEVICE_FUNCTION  Gpp;    // Device/Function Number of GPP
      {0, 0},         // AMD_CPM_PCI_DEVICE_FUNCTION  SwUs;   // Device/Function Number of SWUS
      {0, 0},         // AMD_CPM_PCI_DEVICE_FUNCTION  SwDs;   // Device/Function Number of SWDS
      48,             // UINT8  ResBus;                       // Reserved PCI bus number
      KB008,          // UINT16 ResIo;                        // Reserved IO ranges in bytes
      MB256 + MB128,  // UINT64 ResNonPrefetchableMmio;       // Reserved Non-Prefetchable MMIO base address
      MB064,          // UINT64 AlignResNonPrefetch;          // Alignment for Reserved Non-Prefetchable MMIO base address
      MB512 + MB128,  // UINT64 ResPrefetchableMmio;          // Reserved Prefetchable MMIO base address
      MB256           // UINT64 AlignResPrefetch;             // Alignment for Reserved Prefetchable MMIO base address
    },
    {
      IUSB4_GPP1,     // UINT8  DevType;                      // Device Support List is in AMD_CPM_DEVICE_TYPE
      0,              // UINT8  SocketDieBridge;              // SocketDieBridge Number of GPP/SWUS/SWDS
      {4, 1},         // AMD_CPM_PCI_DEVICE_FUNCTION  Gpp;    // Device/Function Number of GPP
      {0, 0},         // AMD_CPM_PCI_DEVICE_FUNCTION  SwUs;   // Device/Function Number of SWUS
      {0, 0},         // AMD_CPM_PCI_DEVICE_FUNCTION  SwDs;   // Device/Function Number of SWDS
      48,             // UINT8  ResBus;                       // Reserved PCI bus number
      KB008,          // UINT16 ResIo;                        // Reserved IO ranges in bytes
      MB256 + MB128,  // UINT64 ResNonPrefetchableMmio;       // Reserved Non-Prefetchable MMIO base address
      MB064,          // UINT64 AlignResNonPrefetch;          // Alignment for Reserved Non-Prefetchable MMIO base address
      MB512 + MB128,  // UINT64 ResPrefetchableMmio;          // Reserved Prefetchable MMIO base address
      MB256           // UINT64 AlignResPrefetch;             // Alignment for Reserved Prefetchable MMIO base address
    },
    CPM_END_OF_TABLE
  }
};

//
// Lilac Verb Table
//
UINT32 AzaliaCodecVerbTable[] = {
  0x10EC0245,                                       // VID: 0x10EC, DevId: 0x0245
  0x00172095, 0x001721D5, 0x00172222, 0x00172310,   // HDA Codec Subsystem ID  : 0x1022D595
  0x0017FF00, 0x0017FF00, 0x0017FF00, 0x0017FF00,   // Widget node 0x01 :
  0x0205001A, 0x0204C003, 0x0205001A, 0x02048003,   // 1bit reset
  0x01271C40, 0x01271D01, 0x01271EA6, 0x01271FB7,   // Pin widget 0x12 - DMIC
  0x01371C00, 0x01371D00, 0x01371E00, 0x01371F40,   // Pin widget 0x13 - DMIC
  0x01471C10, 0x01471D01, 0x01471E17, 0x01471F90,   // Pin widget 0x14 - FRONT (Port-D)
  0x01671CF0, 0x01671D11, 0x01671E11, 0x01671F41,   // Pin widget 0x16 - NPC
  0x01771CF0, 0x01771D11, 0x01771E11, 0x01771F41,   // Pin widget 0x17 - I2S-OUT
  0x01871CF0, 0x01871D11, 0x01871E11, 0x01871F41,   // Pin widget 0x18 - I2S-IN
  0x01971CF0, 0x01971D11, 0x01971E11, 0x01971F41,   // Pin widget 0x19 - MIC2 (Port-F)
  0x01A71CF0, 0x01A71D11, 0x01A71E11, 0x01A71F41,   // Pin widget 0x1A - NPC
  0x01B71C50, 0x01B71D10, 0x01B71EA1, 0x01B71F03,   // Pin widget 0x1B - LINE2 (Port-E)
  0x01D71C01, 0x01D71D00, 0x01D71E60, 0x01D71F40,   // Pin widget 0x1D - BEEP-IN
  0x01E71C20, 0x01E71D11, 0x01E71E45, 0x01E71F03,   // Pin widget 0x1E - S/PDIF-OUT
  0x02171C30, 0x02171D10, 0x02171E21, 0x02171F03,   // Pin widget 0x21 - HP1-OUT (Port-I)
  0x02050045, 0x0204B089, 0x02050063, 0x02040020,   // additional microphone setup
  0x02050066, 0x02047C00, 0x02050035, 0x02048D6A,   // additional SPDIF OUT setup
  0x0205004A, 0x02042020, 0x02050038, 0x02047909,   // Enable HP-JD & Class-d to 2W/4ohm
  0x0205003C, 0x0204F175, 0x0205003C, 0x0204F135,   //===== Class-D POR
  0xFFFFFFFF,                                       // Vender Table End.
  0x10EC0256,                                       // VID: 0x10EC, DevId: 0x0256
  0x10172008, 0x10172112, 0x101722EC, 0x10172310,   // HDA Codec Subsystem ID  : 0x10EC1208
  0x1017FF00, 0x1017FF00, 0x1017FF00, 0x1017FF00,   // Widget node 0x01
  0x1205001A, 0x1204C003, 0x1205001A, 0x12048003,   // 1bit reset
  0x11271C30, 0x11271D01, 0x11271EA6, 0x11271FB7,   // Pin widget 0x12 - DMIC
  0x11371C00, 0x11371D00, 0x11371E00, 0x11371F40,   // Pin widget 0x13 - DMIC
  0x11471C10, 0x11471D01, 0x11471E17, 0x11471F90,   // Pin widget 0x14 - Front (Port-D)
  0x11871CF0, 0x11871D11, 0x11871E11, 0x11871F41,   // Pin widget 0x18 - I2S IN
  0x11971C20, 0x11971D10, 0x11971EA1, 0x11971F03,   // Pin widget 0x19 - MIC2 (Port-F)
  0x11A71CF0, 0x11A71D11, 0x11A71E11, 0x11A71F41,   // Pin widget 0x1A - LINE1 (Port-C)
  0x11B71CF0, 0x11B71D11, 0x11B71E11, 0x11B71F41,   // Pin widget 0x1B - LINE2 (Port-E)
  0x11D71C01, 0x11D71D00, 0x11D71E60, 0x11D71F40,   // Pin widget 0x1D - BEEP-IN
  0x11E71CF0, 0x11E71D11, 0x11E71E11, 0x11E71F41,   // Pin widget 0x1E - S/PDIF-OUT
  0x12171C40, 0x12171D10, 0x12171E21, 0x12171F03,   // Pin widget 0x21 - HP1-OUT (Port-I)
  0x12050009, 0x1204E003, 0x12050004, 0x1204AA09,   // Widget node 0x20 :
  0x12050038, 0x12047981, 0x12050010, 0x12040020,   // Widget node 0x20 - 1 :
  0x1205003C, 0x12040354, 0x1205003C, 0x12040314,   // Widget node 0x20 - 2 :
  0x1205001B, 0x12040A4B, 0x12050008, 0x12046A6C,   // Widget node 0x20 - 3 :
  0x12050046, 0x12040004, 0x15750003, 0x157409A3,   // Widget node 0x20 - 4 :
  0x11470C00, 0x12050036, 0x12047151, 0x11470740,   // Dos beep path - 1 :
  0x1143b000, 0x11470C02, 0x11470C02, 0x11470C02,   // Dos beep path - 2 :
  0xFFFFFFFF,                                       // Vender Table End.
  0x14F120D0,                                       // VID: 0x14F1, DevId: 0x20D0
  0x0017ff00, 0x000f0000, 0x000f0000, 0x000f0000,   // ;Boot up soft reset
  0x000f0000, 0x000f0000,
  0x00172022, 0x00172110, 0x00172295, 0x001723D5,   // Node 01: SVID/SSID => D595/1022
  0x01671C40, 0x01671D10, 0x01671E21, 0x01671F03,   // Node 16(Port A): 03211040(HP out)
  0x01771C10, 0x01771D01, 0x01771E17, 0x01771F91,   // Node 17(port G): 91170110(SPK/Class_D)
  0x01871C2E, 0x01871D30, 0x01871E81, 0x01871F04,   // Node 18(Port B): 0481302E(Mic/Line In)
  0x01971C20, 0x01971D10, 0x01971EA1, 0x01971F03,   // Node 19(Port D): 03A11020(Headset Mic)
  0x01A71CF0, 0x01A71D01, 0x01A71EA6, 0x01A71F95,   // Node 1A(port C): 95A601F0(1st Dig-Mic I/F)
  0x01D71CF0, 0x01D71D01, 0x01D71EF0, 0x01D71F40,   // Node 1D(port E):40F001F0(I2S/TDM Out)-Disable
  0x01E71CF0, 0x01E71D01, 0x01E71EF0, 0x001723D5,   // Node 1E(port F): 40F001F0 (I2S/TDM In)-Disable
  0x01F71CF0, 0x01F71D01, 0x01F71EF0, 0x01F71F40,   // Node 1F(port H): 40F001F0 (2nd D-Mic I/F)-Disable
  0x02171CF0, 0x02171D01, 0x02171EF0, 0x02171F40,   // Node 21(port I): 40F001F0(SPDI/F)-Disable
  0x02671CF0, 0x02671D01, 0x02671EF0, 0x02671F40,   // Node 26(port J): 40F001F0(I2S/TDM Out)-Disable
  0x02771CF0, 0x02771D01, 0x02771EF0, 0x02771F40,   // Node 27(port K): 40F001F0(I2S/TDM In)-Disable
  0xFFFFFFFF,                                       // Vender Table End.
  0xFFFFFFFF, 0xFFFFFFFF                            // End of AzaliaCodecVerbTable
};

//
// Lilac <TODO: Don't know how to porting>
//
/*
UINT8 HdmiRetimerRedriver[] = {
    0xBA, // HdmiSlvAddr
    0x03, // HdmiRegNum
    0x03, // Hdmi6GRegNum
    0x0A, 0x13,  // HdmiRegSetting[0].ucI2cRegIndex , HdmiRegSetting[0].ucI2cRegVal
    0x0B, 0xC8,  // HdmiRegSetting[1].ucI2cRegIndex , HdmiRegSetting[1].ucI2cRegVal
    0x0C, 0x91,  // HdmiRegSetting[2].ucI2cRegIndex , HdmiRegSetting[2].ucI2cRegVal
    0x00, 0x00,  // HdmiRegSetting[3].ucI2cRegIndex , HdmiRegSetting[3].ucI2cRegVal
    0x00, 0x00,  // HdmiRegSetting[4].ucI2cRegIndex , HdmiRegSetting[4].ucI2cRegVal
    0x00, 0x00,  // HdmiRegSetting[5].ucI2cRegIndex , HdmiRegSetting[5].ucI2cRegVal
    0x00, 0x00,  // HdmiRegSetting[6].ucI2cRegIndex , HdmiRegSetting[6].ucI2cRegVal
    0x00, 0x00,  // HdmiRegSetting[7].ucI2cRegIndex , HdmiRegSetting[7].ucI2cRegVal
    0x00, 0x00,  // HdmiRegSetting[8].ucI2cRegIndex , HdmiRegSetting[8].ucI2cRegVal
    0x0A, 0x13,  // Hdmi6GhzRegSetting[0].ucI2cRegIndex , Hdmi6GhzRegSetting[0].ucI2cRegVal
    0x0B, 0xDA,  // Hdmi6GhzRegSetting[1].ucI2cRegIndex , Hdmi6GhzRegSetting[1].ucI2cRegVal
    0x0C, 0x1D   // Hdmi6GhzRegSetting[2].ucI2cRegIndex , Hdmi6GhzRegSetting[2].ucI2cRegVal
};
*/

EFI_STATUS
SpecificQCommWlanPreInitOverride (
  IN  EFI_PEI_SERVICES          **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN  VOID                      *Ppi
  )
{
  UINT32                        VidDid;
  UINT32                        Temp32;
  AMD_CPM_TABLE_PPI             *CpmTablePtr;
  EFI_STATUS                    Status;
  AMD_CPM_PRE_INIT_TABLE        *PreInitTablePtr;

  DEBUG((EFI_D_INFO, "%a-Start\n", __FUNCTION__));

  if (PcdGetBool (PcdMpmEnable)) {
    return EFI_SUCCESS;
  }

  Status = (*PeiServices)->LocatePpi (
                              (CPM_PEI_SERVICES**)PeiServices,
                              &gAmdCpmTablePpiGuid,
                              0,
                              NULL,
                              (VOID**)&CpmTablePtr
                              );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Locate-AmdCpmTablePpiGuid-fail-Status=%r\n", Status));
    return Status;
  }

  Temp32 = PciRead32(PCI_LIB_ADDRESS (0, 2, 2, 00));
  if (Temp32 == 0xFFFFFFFF) {
    DEBUG ((EFI_D_ERROR, "Wlan RP is hidden\n"));
    return EFI_NOT_FOUND;
  }

  PciWrite32(PCI_LIB_ADDRESS (0, 2, 2, 0x18), 0x20200); //Assign temp bus
  VidDid = PciRead32(PCI_LIB_ADDRESS (2, 0, 0, 00));

  if (VidDid != 0x110317CB) { // Specific Qcomm Wlan VID/DID
    PciWrite32(PCI_LIB_ADDRESS (0, 2, 2, 0x18), 0x00); //clear temp bus
    DEBUG ((EFI_D_ERROR, "%a-EP: 0x%x is Not Specific QComm Wlan .\n", __FUNCTION__, VidDid));
    return EFI_NOT_FOUND;
  }

  PreInitTablePtr           = CpmTablePtr->CommonFunction.GetTablePtr (CpmTablePtr, CPM_SIGNATURE_PRE_INIT);
  if (PreInitTablePtr) {
    AMD_CPM_PRE_SETTING_ITEM        *TablePtr;

    TablePtr = &PreInitTablePtr->Item[0];
    while (TablePtr->Type != 0xFF ) {
      if (TablePtr->Type == 0x00 && TablePtr->Select == 0x15 && TablePtr->Offset == 0x49) {
        // GPIOx0000048 is wlan wake gpio pin offset and we start access from bit 8 to follow pre-init table
        MmioWrite8 (ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x49, (UINT8) (MmioRead8 (ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x49) & 0x1F));
        DEBUG((DEBUG_INFO, "Disable Wlan Wake Capability\n"));
        break;
      }
      TablePtr ++;
    }
  }

  PciWrite32(PCI_LIB_ADDRESS (0, 2, 2, 0x18), 0x00); //clear temp bus

  DEBUG((EFI_D_INFO, "%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mPreinitNotify =
{
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEfiPeiMemoryDiscoveredPpiGuid,
  SpecificQCommWlanPreInitOverride
};

//
// OEM CPM Table definition end
//

//
// PLAT-78461, Notify EC USB ready in ECRAM offset 0xFE bit0
//
EFI_STATUS
EFIAPI
AmdFchInitRdy_EcWork (
  EFI_PEI_SERVICES                    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR        *NotifyDescriptor,
  IN VOID                             *Ppi
)
{
  EFI_STATUS                          Status;
  AMD_CPM_TABLE_PPI                   *AmdCpmTablePpi;
  UINT8                               Value8;
  UINT8                               Offset;

  DEBUG((DEBUG_INFO, "[%d]: Lilac CPM, Enter AmdFchInitReady(...)\n", __LINE__));

  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gAmdCpmTablePpiGuid,
                           0,
                           NULL,
                           (VOID**)&AmdCpmTablePpi
                           );
  if (EFI_ERROR (Status))
  {
    DEBUG((DEBUG_INFO, "  [%d]: OEM-PEI-AmdCpmTablePpi-Status=%r\n", __LINE__, Status));
    return Status;
  }

  Offset = ECRAMxFE;
  AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);

  Value8 |= FCH_INIT_PEI;
  AmdCpmTablePpi->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &Offset, &Value8);
  return Status;

}

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mFchInitRdyNotifyEcList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdFchInitPpiGuid,
  AmdFchInitRdy_EcWork
};

//
// Lilac EC Initial Table
//

#define U8_CLEAR_BIT(ArgBit)            ((UINT8)~(ArgBit))
#define MAX_EC_INDEX                (sizeof(PorgEcConfig)/sizeof(PorgEcConfig[0]))
#define EcTblSet(_unIdx,_ClearBit,_SetBit) InnerEcTblSet(PorgEcConfig,MAX_EC_INDEX,_unIdx,_ClearBit,_SetBit)

#pragma pack (push, 1)
typedef struct{
  UINT8   IdxEcRam;
  UINT8   AndEcRamData;
  UINT8   OrEcRamData;
  UINT8   Rev[1];
} struEcTbl;
#pragma pack (pop)

/*
 * Lilac EC Table Setting/Clear function
 * */
EFI_STATUS InnerEcTblSet (
    struEcTbl   _PorgEcConfig[],
    UINTN       MaxEcTblSize,
    UINT8       unIdx,
    UINT8       ClearBit,
    UINT8       SetBit
)
{
  UINTN     unIndex;

  for (unIndex = 0; unIndex < MaxEcTblSize; ++unIndex)
  {
    if (_PorgEcConfig[unIndex].IdxEcRam == unIdx)
      break;
  }
  if (unIndex == MaxEcTblSize)
  {
    DEBUG ((DEBUG_INFO, "[%d]: Failed Index to Set EC Table - %r\n", __LINE__, EFI_INVALID_PARAMETER));
    return EFI_INVALID_PARAMETER;
  }

  _PorgEcConfig[unIndex].AndEcRamData &= U8_CLEAR_BIT((UINTN)ClearBit);
  _PorgEcConfig[unIndex].OrEcRamData &= U8_CLEAR_BIT((UINTN)ClearBit);
  _PorgEcConfig[unIndex].OrEcRamData |= SetBit;

  return EFI_SUCCESS;
}

UINT8 InnerEcIdxRead (
    AMD_CPM_TABLE_PPI   *AmdCpmTablePpi,
    UINT8               u8Ofst
)
{
  EFI_STATUS    Status;
  UINT8         u8Val = 0xFF;

  Status = AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &u8Ofst, &u8Val);
  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_INFO, "[%d]: Fail to Read CPM EC function - %r\n", __LINE__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  return u8Val;
}

EFI_STATUS ImmEcIdxWrite (
    AMD_CPM_TABLE_PPI   *AmdCpmTablePpi,
    UINT8               u8Ofst,
    UINT8               u8Val
)
{
  EFI_STATUS    Status;

  Status = AmdCpmTablePpi->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &u8Ofst, &u8Val);
  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_INFO, "[%d]: Fail to Write CPM EC function - %r\n", __LINE__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  return Status;

}

EFI_STATUS ImmEcRegClearOr (
    AMD_CPM_TABLE_PPI   *AmdCpmTablePpi,
    UINT8               u8Ofst,
    UINT8               u8ClearBit,
    UINT8               u8OrVal
)
{
  EFI_STATUS    Status;
  UINT8         Value;

  Value = InnerEcIdxRead (AmdCpmTablePpi, u8Ofst);

  Value &= U8_CLEAR_BIT (u8ClearBit);
  Value |= u8OrVal;

  Status = ImmEcIdxWrite (AmdCpmTablePpi, u8Ofst, Value);

  return Status;

}

EFI_STATUS  EcI2cDevByteAccess (
    AMD_CPM_TABLE_PPI   *AmdCpmTablePpi,
    BOOLEAN             boolRead,
    UINT8               I2c7BitSlaveAddr,
    UINT8               SlaveIdx,
    UINT8               *SlaveData
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT8                 Offset;
  UINT8                 Value8;
  UINTN                 Index8;

  do
  {
    //
    // EcName.asl define the EC Access Rule
    //
//    Offset(0x90), // PMIC/VR
//    PMC0,8,       // register index
//    PMC1,8,       // value write to/read from PMIC/VR
//    PMC2,8,       // on write, this field indicates the 8-bit PMIC/VR slave device address.
                    //           LSB is 1 to trigger read operation, 0 to trigger write operation.
                    // on read,  returns the status of last operation.
                    //           0xAC - success, 0xE2 - error, 0xCC - ongoing
    // I2C Slave Index
    Status = ImmEcIdxWrite (AmdCpmTablePpi, 0x90, SlaveIdx);
    if (EFI_ERROR (Status))
    {
      ASSERT_EFI_ERROR (Status);
      break;
    }

    if (FALSE == boolRead)
    {
      // I2C Slave Data to Write
      Status = ImmEcIdxWrite (AmdCpmTablePpi, 0x91, *SlaveData);
      if (EFI_ERROR (Status))
      {
        ASSERT_EFI_ERROR (Status);
        break;
      }
    }

    // I2C Slave Addr and Execute write command.
    Value8 = (I2c7BitSlaveAddr << 1) | ( TRUE == boolRead ? 1 : 0);
    Status = ImmEcIdxWrite (AmdCpmTablePpi, 0x92, Value8);
    if (EFI_ERROR (Status))
    {
      ASSERT_EFI_ERROR (Status);
      break;
    }

    // Wait device response
    do
    {
      for (Index8 = 0; Index8 < 50; Index8++)
      {
        AmdCpmTablePpi->CommonFunction.IoRead8 (0x80);
      }
      Offset = 0x92;
      Status = AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);
      ASSERT_EFI_ERROR (Status);
      if (0xAC == Value8)
      {
        Status = EFI_SUCCESS;
        break;
      }
      else if (0xE2 == Value8)
      {
        Status = EFI_DEVICE_ERROR;
        ASSERT_EFI_ERROR (Status);
        break;
      }
    } while (Value8 == 0xCC);

    if (EFI_ERROR (Status))
    {
      ASSERT_EFI_ERROR (Status);
      break;
    }

    if (TRUE == boolRead)
    {
      Offset = 0x91;
      Status = AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);
      if (EFI_ERROR (Status))
      {
        ASSERT_EFI_ERROR (Status);
        break;
      }
      *SlaveData = Value8;
    }
    // Success Operation.
    DEBUG ((DEBUG_INFO, "  EC I2C %s - %r, Addr[0x%x],Offset[0x%x]:[0x%x]\n", (boolRead ? "Read":"Write"), Status, I2c7BitSlaveAddr, SlaveIdx, *SlaveData));
  } while (FALSE);

  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_INFO, "  EC I2C %s - %r\n", (boolRead ? "Read":"Write"), Status));
  }

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Detect MXM card presence
 *
 * This function returns the presence of MXM card
 *
 * @param[in]     This           Pointer to AMD CPM Table PPI
 *
 * @retval        TURE  - MXM card is present
 * @retval        FALSE - MXM card is not present
 *
 */
BOOLEAN
MxmPresent (
  IN       AMD_CPM_TABLE_PPI   *This
  )
{
  AMD_CPM_TABLE_PPI       *CpmTablePpiPtr;
//  AMD_CPM_GPIO_INIT_TABLE *GpioTablePtr;
//  AMD_CPM_GPIO_ITEM       *TablePtr;

  CpmTablePpiPtr = This;
//  if (CpmTablePpiPtr) {
//    GpioTablePtr = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_GPIO_INIT);
//    if (GpioTablePtr) {
//      TablePtr = &GpioTablePtr->GpioList[0];
//      while (TablePtr->Pin != 0xFF ) {
//        if(TablePtr->Pin == 0x45) {
//          CpmTablePpiPtr->CommonFunction.SetGpio (CpmTablePpiPtr, TablePtr->Pin, TablePtr->Setting.Raw);
//          break;
//        }
//        TablePtr++;
//      }
//    }
//  }
  return (CpmTablePpiPtr->CommonFunction.DetectDevice (CpmTablePpiPtr, DEVICE_ID_MXM, NULL));
}

/*----------------------------------------------------------------------------------------*/
/**
 * Update SMBUS port
 *
 * @param[in]     AmdCpmTablePpi            Pointer to AMD CPM Table PPI
 *
 */
VOID
SetSmBusPort (
  IN       AMD_CPM_TABLE_PPI            *AmdCpmTablePpi
  )
{
  UINT16                  LoopCount;
  AMD_CPM_COMMON_FUNCTION *CommonFunctionPtr;
  AMD_CPM_MAIN_TABLE      *MainTablePtr;

  MainTablePtr        = AmdCpmTablePpi->MainTablePtr;
  CommonFunctionPtr   = &AmdCpmTablePpi->CommonFunction;

  // Hold the SMBUS Semaphore first. (SMBUSx08[4, HostSemaphore])
  LoopCount = 100;
  do {
    // Set HostSemaphore
    CommonFunctionPtr->MmioOr8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + 0x0A00 + 0x08), BIT4);
    CommonFunctionPtr->IoRead8(0xed);    // 0xed IO Delay
    CommonFunctionPtr->IoRead8(0xed);    // 0xed IO Delay
    LoopCount--;
  } while (LoopCount > 0 &&
           0 == (CommonFunctionPtr->MmioRead8 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + 0x0A00 + 0x08)) & BIT4));

  // Make sure SMBUS port PMx00[20:19, SmBus0Sel] is 0.
  LoopCount = 100;
  while (LoopCount > 0 &&
         0 != (CommonFunctionPtr->MmioRead32 ((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + 0x0300 + 0x00)) & (BIT19 + BIT20))) {
    CommonFunctionPtr->MmioAnd32((UINT32)(MainTablePtr->AcpiMemIoBaseAddr + 0x0300 + 0x00), ~(UINT32) (BIT19 + BIT20));
    CommonFunctionPtr->IoRead8(0xed);    // 0xed IO Delay
    LoopCount--;
  }
}


CHAR16 *
GetConnectorTypeStr (
  IN UINT32             ConnectorType
  )
{
#define QUOTE(name) L ## #name
#define STR(type) if (type == ConnectorType) return QUOTE(type)

  STR(ConnectorTypeDP);
  STR(ConnectorTypeEDP);
  STR(ConnectorTypeSingleLinkDVI);
  STR(ConnectorTypeDualLinkDVI);
  STR(ConnectorTypeHDMI);
  STR(ConnectorTypeDpToVga);
  STR(ConnectorTypeDpToLvds);
  STR(ConnectorTypeNutmegDpToVga);
  STR(ConnectorTypeSingleLinkDviI);
  STR(ConnectorTypeDpWithTypeC);
  STR(ConnectorTypeDpWithoutTypeC);
  STR(ConnectorTypeEDPToLvds);
  STR(ConnectorTypeEDPToLvdsSwInit);
  STR(ConnectorTypeAutoDetect);
  STR(UnusedType);
  STR(MaxConnectorType);

  return L"Unknown";
}

PCIE_CONNECTOR_TYPE
GetConnectorTypeFromPBS (
  IN       CHAR8                        *FunctionName,
  IN       AMD_PBS_SETUP_OPTION         *AmdPbsConfiguration,
  IN       PLATFORM_DXIO_DDI            *PlatformDxioDDI,
  IN       UINTN                        Index
  )
{
  UINT8                     ConfigDpValue;
  UINT32                    DpConnectorType;

  ConfigDpValue = AmdPbsConfiguration->DP[Index];
  DpConnectorType = MaxConnectorType;

  //
  // DP Switch
  //    1: EDP
  //    2: DP (no/with/without TypeC will according to AgesaModulePkg\Nbio\GFX\RMB\PEI\GfxEnumConnectors.c)
  //    3: HDMI
  //    4: DP with TypeC
  //    5: DP without TypeC
  //    0 and Others: using defult by board ID
  //
  switch (ConfigDpValue) {
  case 1:
    DpConnectorType = ConnectorTypeEDP;
    break;
  case 2:
    DpConnectorType = ConnectorTypeDP;
    break;
  case 3:
    DpConnectorType = ConnectorTypeHDMI;
    break;
  case 4:
    DpConnectorType = ConnectorTypeDpWithTypeC;
    break;
  case 5:
    DpConnectorType = ConnectorTypeDpWithoutTypeC;
    break;
  case 0:
  default:
    DpConnectorType = MaxConnectorType;
  }

  DEBUG((DEBUG_INFO, "-->  AmdPbsConfiguration.DP[%d] = %x (%s)\n",
    Index, ConfigDpValue, GetConnectorTypeStr(DpConnectorType)));

  if (PlatformDxioDDI == NULL) {
    return DpConnectorType;
  }

  DEBUG((DEBUG_INFO, "OEM-PEI-%a->DP%d, default value: %d (%s)\n",
    FunctionName,
    Index,
    PlatformDxioDDI->ConnectorType[Index],
    GetConnectorTypeStr(PlatformDxioDDI->ConnectorType[Index])
    ));

  if (DpConnectorType != MaxConnectorType) {
    DEBUG((DEBUG_INFO, "OEM-PEI-%a->DP%d, override value by config: %d (%s)\n",
      FunctionName, Index, DpConnectorType, GetConnectorTypeStr(DpConnectorType)));
  }

  return DpConnectorType;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Detect PCIe Devices Automatically
 *
 * This function patches PCIe Topology Override Table by detecting PCIe devices
 * automatically.
 *
 * @param[in]     AmdCpmTablePpi                Pointer to AMD CPM Table PPI
 * @param[in]     DxioTopologyTablePtr          Pointer to Dxio Topology Table
 * @param[in]     AmdPbsConfiguration           Pointer to the structure of AMD_PBS_SETUP_OPTION
 * @param[out]    TravisStatus                  Current Travis Status
 * @param[in]     CurrentBoardId                Current Board ID
 *
 */
VOID
DetectPcieDdiDevices (
  IN       AMD_CPM_TABLE_PPI            *AmdCpmTablePpi,
  IN       AMD_CPM_DXIO_TOPOLOGY_TABLE  *DxioTopologyTablePtr,
  IN       AMD_PBS_SETUP_OPTION         *AmdPbsConfiguration,
  IN OUT   UINT8                        *TravisStatus,
  IN       UINT8                        PlatformSel
  )
{
  // ATOM_ENCODER_CAP_RECORD_DP_FIXED_VS_EN Copy from AgesaModulePkg\Nbio\GFX\RMB\PEI\GfxEnumConnectors.c
#define ATOM_ENCODER_CAP_RECORD_DP_FIXED_VS_EN             0x0002
  DDI_DESCRIPTOR          *DdiPort;
  UINTN                   Index;
  UINT32                  DpConnectorType;
  PLATFORM_DXIO_DDI       *PlatformDxioDDI;
  UINT32                  Dp_Cap;

  DdiPort = &(DxioTopologyTablePtr->Ddi[0]);

  DEBUG((DEBUG_INFO, "OEM-PEI-DetectDdiDevices-Start\n"));

  //
  // search Platform DXIO DDI setting from table
  //
  PlatformDxioDDI = NULL;
/*  for (Index = 0; Index < ARRAY_SIZE (gCpmPlatformDxioDdiTable); Index++) {
    if (PlatformSel == gCpmPlatformDxioDdiTable[Index].PlaformId) {*/
     PlatformSel = 0x00;
      PlatformDxioDDI = &gCpmPlatformDxioDdiTable[0];
/*      break;
    }
  }*/

  ASSERT (PlatformDxioDDI != NULL);
  if (PlatformDxioDDI == NULL) {
    DEBUG((DEBUG_ERROR, "OEM-PEI-DetectDdiDevices " \
      "Cannot found platform DXIO DDI default settings by platform id: %x\n", PlatformSel));
  }

  for (Index = 0; Index < DXIO_DDI_COUNT; Index++) {
    DpConnectorType = GetConnectorTypeFromPBS (
                        __FUNCTION__,
                        AmdPbsConfiguration,
                        PlatformDxioDDI,
                        Index
                        );
    if (DpConnectorType == MaxConnectorType) {
      continue;
    }
    DdiPort[Index].Ddi.ConnectorType = (UINT8)DpConnectorType;

    //
    // final item
    //
    if ((DdiPort[Index].Flags & DESCRIPTOR_TERMINATE_LIST) == DESCRIPTOR_TERMINATE_LIST) {
      break;
    }
  }

  //
  // gEfiAmdAgesaPkgTokenSpaceGuid.PcdDisplayCapDdi1
  // gEfiAmdAgesaPkgTokenSpaceGuid.PcdDisplayCapDdi2
  //
  DEBUG((DEBUG_ERROR, "OEM-PEI-DetectDdiDevices " \
    "PcdDisplayCapDdi1: %x\n", PcdGet32(PcdDisplayCapDdi1)));
  if (AmdPbsConfiguration->DP1DisplayCaps != 0xFFFFFFFF) {
    PcdSet32S(PcdDisplayCapDdi1, AmdPbsConfiguration->DP1DisplayCaps);
    DEBUG((DEBUG_ERROR, "OEM-PEI-DetectDdiDevices " \
      "PcdDisplayCapDdi1 override value: %x\n", PcdGet32(PcdDisplayCapDdi1)));
  }
  // Override for Default Ddi
  else
  {
    if (DdiPort[1].Ddi.ConnectorType == ConnectorTypeDP)
    {
      PcdSet32S(PcdDisplayCapDdi1, EXT_DISPLAY_PATH_CAPS__DP2  | EXT_DISPLAY_PATH_CAPS__UHBR10_EN);
    }
    else if (DdiPort[1].Ddi.ConnectorType == ConnectorTypeHDMI)
    {
      PcdSet32S(PcdDisplayCapDdi1,
          (EXT_DISPLAY_PATH_CAPS__HDMI_FRL_12GbEn |
          EXT_DISPLAY_PATH_CAPS__HDMI_FRL_10GbEn |
          EXT_DISPLAY_PATH_CAPS__HDMI_FRL_8GbEn |
          EXT_DISPLAY_PATH_CAPS__HDMI_FRL));
    }
  }

  DEBUG((DEBUG_ERROR, "OEM-PEI-DetectDdiDevices " \
    "PcdDisplayCapDdi2: %x\n", PcdGet32(PcdDisplayCapDdi2)));
  if (AmdPbsConfiguration->DP2DisplayCaps != 0xFFFFFFFF) {
    PcdSet32S(PcdDisplayCapDdi2, AmdPbsConfiguration->DP2DisplayCaps);
    DEBUG((DEBUG_ERROR, "OEM-PEI-DetectDdiDevices " \
      "PcdDisplayCapDdi2 override value: %x\n", PcdGet32(PcdDisplayCapDdi2)));
  }
  // Override for Default Ddi
  else
  {
    if (DdiPort[2].Ddi.ConnectorType == ConnectorTypeDP)
    {
      Dp_Cap = EXT_DISPLAY_PATH_CAPS__DP2  | EXT_DISPLAY_PATH_CAPS__UHBR10_EN;
      PcdSet32S(PcdDisplayCapDdi2, Dp_Cap);
    }
    else if (DdiPort[2].Ddi.ConnectorType == ConnectorTypeHDMI)
    {
      Dp_Cap = EXT_DISPLAY_PATH_CAPS__HDMI_FRL_12GbEn | EXT_DISPLAY_PATH_CAPS__HDMI_FRL_10GbEn |
          EXT_DISPLAY_PATH_CAPS__HDMI_FRL_8GbEn | EXT_DISPLAY_PATH_CAPS__HDMI_FRL;

      PcdSet32S(PcdDisplayCapDdi2, Dp_Cap);
    }
    else if (DdiPort[2].Ddi.ConnectorType == ConnectorTypeDpWithTypeC)
    {
      Dp_Cap = 0;
      if ((PlatformSel == PLATFORM_SelMayanKD) || (PlatformSel == PLATFORM_SelLilacKD))
      {
        Dp_Cap = EXT_DISPLAY_PATH_CAPS__USB4_DPIA_ENABLE | ATOM_ENCODER_CAP_RECORD_DP_FIXED_VS_EN;
      }
      Dp_Cap |= ATOM_ENCODER_CAP_RECORD_USB_C_TYPE | EXT_DISPLAY_PATH_CAPS__DP2 | EXT_DISPLAY_PATH_CAPS__UHBR10_EN;

      PcdSet32S(PcdDisplayCapDdi2, Dp_Cap);
    }
    else if (DdiPort[2].Ddi.ConnectorType == ConnectorTypeDpWithoutTypeC)
    {
      Dp_Cap = EXT_DISPLAY_PATH_CAPS__DP2 | ATOM_ENCODER_CAP_RECORD_DP_FIXED_VS_EN | EXT_DISPLAY_PATH_CAPS__UHBR10_EN;

      PcdSet32S(PcdDisplayCapDdi2, Dp_Cap);
    }
  }

  /*
   * The follow code will be defined on NBIO Code, Put here for override reference.
  // Set default for DP3
  if (DdiPort[3].Ddi.ConnectorType == ConnectorTypeDpWithTypeC)
  {
    Dp_Cap = 0;
    if ((PlatformSel == PLATFORM_SelMayanKD) || (PlatformSel == PLATFORM_SelLilacKD))
    {
      Dp_Cap = EXT_DISPLAY_PATH_CAPS__USB4_DPIA_ENABLE | ATOM_ENCODER_CAP_RECORD_DP_FIXED_VS_EN;
    }
    Dp_Cap |= ATOM_ENCODER_CAP_RECORD_USB_C_TYPE | EXT_DISPLAY_PATH_CAPS__DP2 | EXT_DISPLAY_PATH_CAPS__UHBR10_EN;

    PcdSet32S(PcdDisplayCapDdi3, Dp_Cap);
  }
  */


  DEBUG((DEBUG_INFO, "OEM-PEI-DetectPcieDdiDevices-End\n"));
}

#pragma pack (push, 1)
typedef union {
  struct {
    // Bit 0~7
    UINT8   iSata_Gpp11   : 1;      //  GPP11 reworked to iSATA
    UINT8   iSata_Gpp10   : 1;      //  GPP10 reworked to iSATA
    UINT8   reDp1         : 1;      //  DP1 reworked to DP CONN
    UINT8   reUsbC0_nova  : 1;      //  USBC0 reworked to NOVA CONN
    UINT8   reUsbC1_10G   : 1;      //  USBC1 reworked to NOVA CONN
    UINT8   DisUsb2Hs_Fp    : 1;    //  USB2 HS disconnected to FP
    UINT8   DisUsb3Hs_Wwan  : 1;    //  USB3 HS disconnected to WWAN
    UINT8   DisUsb6_Bt      : 1;    //  USB6 HS disconnected to BT
    // Bit 8~15
    UINT8   DisUsb7_Cam     : 1;    //  USB7 HS disconnected to camera
    UINT8   Ac25w_54w       : 1;    //  25W reworked to 54W
    UINT8   Ac54W_25W       : 1;    //  54W reworked to 25W
    UINT8   Adapter150WOnly : 1;    //  Adapter 150W only
    UINT8   Rev0            : 4;
    // Bit 16~63
    UINT8   Rev1;   //  16-23
    UINT8   Rev2;   //  24-31
    UINT8   Rev3;   //  32-39
    UINT8   Rev4;   //  40-47
    UINT8   Rev5;   //  48-55
    UINT8   Rev6;   //  56-63
    // Bit 64~71 (0x4E)
    UINT8   ApuSku          : 2;      // 00:Not initialized, 01:TV, 02:A0 APU silcon, 03: B0 APU silcon
    UINT8   PDInfo          : 2;      // 00:Not initialized, 01:PS8828A, 02:PS8830, 03:Not defined
    UINT8   Rev7            : 4;
    // 0x4F Memory type
    UINT8   Rev8            : 4;
    UINT8   MemType         : 4;      // (0x01 or 0x0F): Micron DRx32 16GB, 0x04: LPDDR5 Socket, 0x05: Micron SRx32 8GB, 0x06: Micron DRx32 32GB (x8 die)

  } Desc;
  UINT8      Raw[10];    // Up to 80 bit defined.
} T_BOARD_ID_REWORK_FIELD;

/*
 *  // Copy From EcName.asl
 * Offset(0x93),  // ECRAMx93
 * PCBR,4,        // PCB revision, 0 - '00A', 1 - '00B' ...
 * BRID,4,        // Board ID
 *                // 0 - D595 Lilac
 *                // 1 - D597 Mayan
 *                // 2 - D596 Lilac DAP
 *                // 3 - D598 Mayan DAP
 *                // E - In case Board ID EEPROM 0x40 ~ 0x43 are empty
 * SKU0,8,        // SKU
 * HWLE,8,        // HW Level (or REV)
 * SRK0,8,        // Special Rework 0
 * SRK1,8,        // Special Rework 1
 * SRK2,8,        // Special Rework 2
 * SRK3,8,        // Special Rework 3
 * SRK4,8,        // Special Rework 4
 * SRK5,8,        // Special Rework 5
 * MMID,8,        // 8-bit Memory ID
 **/
#define BOARD_ID_EXPORTED_FIELD_LENGTH 10
  typedef union {
    UINT8   arr[BOARD_ID_EXPORTED_FIELD_LENGTH];
    struct {
      UINT8   pcbr  : 4; // byte0: pcb revision, 0 - '00A', 1 - '00B', 2 - '00C' ...
      UINT8   bid   : 4; // byte0: board id, 0:Lilac 1: Mayan  2: MayanDAP 3: Moonglow 4: Lilac_KDKD 5: Mayan_KD
      UINT8   sku;       // byte1:
      UINT8   hwLevel;   // byte2:
      UINT8   specRework[BOARD_ID_EXPORTED_FIELD_LENGTH - 4]; // byte3-8
      UINT8   memType;   // byte9: memory id
    } f;
  } T_BOARD_ID_EXPORTED_FIELD;
#pragma pack (pop)


/*----------------------------------------------------------------------------------------*/
/**
 * Update PcdAmdBitMapDisaplyOnlyController Automatically
 *
 * For ComboPhy feature, FchInit need early decide whether load USB firmware
 * So it need earily set PcdAmdBitMapDisaplyOnlyController PCD,
 * however this function only support platform id from EC RAM,
 * if platform id from EEROM, please update PcdAmdBitMapDisaplyOnlyController in Project.dsc
 *
 * @param[in]     MainTablePtr                  Pointer to AMD CPM MAIN TABLE
 * @param[in]     AmdPbsConfiguration           Pointer to the structure of AMD_PBS_SETUP_OPTION
 */
VOID
UpdateDisplayPcdOnOemInitPeiEntry (
  IN       AMD_CPM_MAIN_TABLE           *MainTablePtr,
  IN       AMD_PBS_SETUP_OPTION         *AmdPbsConfiguration
  )
{
  UINT8                     u8Ofst;
  T_BOARD_ID_EXPORTED_FIELD RMB_EcBoard_Info;
  UINT8                     PlatformSel;
  UINT32                    DdiBitMap;
  UINT32                    DisableUsbBitOffset;
  UINTN                     Index;
  UINT32                    DpConnectorType;
  PLATFORM_DXIO_DDI         *PlatformDxioDDI;
  UINT8                     *TmpBufPtr = NULL;
  UINTN                     UsbTableSize;

  DEBUG((DEBUG_INFO, "OEM-PEI-UpdateDisplayPcdOnOemInitPeiEntry-Start\n"));

  //
  // Read EC from 0x93, read BOARD_ID_EXPORTED_FIELD_LENGTH (10) byte.
  // Current Boot on SimNow, Default be Mayan.
  //
  RMB_EcBoard_Info.f.bid = 1;
  RMB_EcBoard_Info.f.pcbr = 0;

  //
  //Environment: 0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO, 4 - GC Lite
  //
  if (PcdGet32 (PcdAmdEnvironmentFlag) == 0 && ((PcdGet32 (PcdAmdEnvironment98) & BIT16) == 0) )
  {
    if ((MainTablePtr->EcSupport) || (MainTablePtr->KbcSupport)) {
      DEBUG((DEBUG_INFO, "OEM-PEI-UpdateDisplayPcdOnEntry read EC for platform id\n"));
      for (Index = 0; Index < BOARD_ID_EXPORTED_FIELD_LENGTH; ++Index)
      {
        u8Ofst = (UINT8)(0x93 + Index);
        CpmKbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &u8Ofst, &RMB_EcBoard_Info.arr[Index]);
      }
    }
  }

  PlatformSel = (UINT8) RMB_EcBoard_Info.f.bid;
  DEBUG((DEBUG_INFO, "OEM-PEI-UpdateDisplayPcdOnEntry " \
    "EC Report : CurrentBoardId=%04X\n", RMB_EcBoard_Info.f.bid));


  // Lilac DAP has different PD interrupt mode.
  if (PLATFORM_SelLilacDAP== PlatformSel) {
    PcdSet8S (PcdPdInterruptModeEn, 0x0C);
  }

  UsbTableSize = PcdGetSize (PcdUsbKTOemConfigurationTable);
  DEBUG((DEBUG_INFO, "OEM-PEI-UpdateDisplayPcdOnEntry PcdUsbKTOemConfigurationTable size=[%d]\n", UsbTableSize));
  if (UsbTableSize >= 3) {
    TmpBufPtr = (UINT8*)PcdGetPtr (PcdUsbKTOemConfigurationTable);
    TmpBufPtr[2] = (UINT8) UsbTableSize;
    PcdSetPtrS (PcdUsbKTOemConfigurationTable, &UsbTableSize, TmpBufPtr);
  }


  //
  // search Platform DXIO DDI setting from table
  //
/*  PlatformDxioDDI = NULL;
  for (Index = 0; Index < ARRAY_SIZE (gCpmPlatformDxioDdiTable); Index++) {
    if (PlatformSel == gCpmPlatformDxioDdiTable[Index].PlaformId) {*/
	PlatformSel = 0x00;
      PlatformDxioDDI = &gCpmPlatformDxioDdiTable[0];
      /*break;
    }
  }*/

  ASSERT (PlatformDxioDDI != NULL);
  if (PlatformDxioDDI == NULL) {
    DEBUG((DEBUG_ERROR, "OEM-PEI-UpdateDisplayPcdOnEntry " \
      "Cannot found platform DXIO DDI default settings by platform id: %x\n", PlatformSel));
    return ;
  }

  //
  // Update PcdAmdBitMapDisaplyOnlyController
  // USB/Disaply Combo PHY info - 0: Enable USB capability; 1:Disable USB capability
  //   USBC0[0:3]  -- Combo-PHY 0
  //   USBC1[4:7]  -- Combo-PHY 1
  //   USBC2[8:11] -- Combo-PHY 2
  //   Reserved [12:31]
  //
  DdiBitMap = PcdGet32 (PcdAmdBitMapDisaplyOnlyController);
  for (Index = 2; Index < DXIO_DDI_COUNT; Index++) {
    DpConnectorType = GetConnectorTypeFromPBS (
                        __FUNCTION__,
                        AmdPbsConfiguration,
                        PlatformDxioDDI,
                        Index
                        );
    if (DpConnectorType == MaxConnectorType) {
      continue;
    }

    DisableUsbBitOffset = 1 << ((Index - 2) * 4);
    if (DpConnectorType == ConnectorTypeHDMI || DpConnectorType == ConnectorTypeDpWithoutTypeC) {
      DdiBitMap |= DisableUsbBitOffset;
    } else {
      DdiBitMap &= (~DisableUsbBitOffset);
    }
  }

  PcdSet32S (PcdAmdBitMapDisaplyOnlyController, DdiBitMap);
  DEBUG((DEBUG_INFO, "OEM-PEI-UpdateDisplayPcdOnOemInitPeiEntry " \
    "PcdAmdBitMapDisaplyOnlyController = %x \n", DdiBitMap));
}



/*----------------------------------------------------------------------------------------*/
/**
 * ConfigBoard ID Initial value
 *
 * @param[in]     AmdCpmTablePpi          Pointer to AMD CPM Table PPI
 *
 */
EFI_STATUS  ConfigBoardID (
    AMD_CPM_TABLE_PPI             *AmdCpmTablePpi
)
{
  // Declaration
  EFI_STATUS                Status = EFI_SUCCESS;
  UINT8                     u8Ofst;
  UINT8                     u8Val;
  UINTN                     unIdx;
  T_BOARD_ID_EXPORTED_FIELD RMB_EcBoard_Info;
  UINT8                     u8PlatSel;
  UINT32                    ECFwVersion = 0;

  u8Val = 0;

  // Read EC from 0x93, read BOARD_ID_EXPORTED_FIELD_LENGTH (10) byte.
  // Current Boot on SimNow, Default be Mayan.
  RMB_EcBoard_Info.f.bid = 1;
  RMB_EcBoard_Info.f.pcbr = 0;
  if (PcdGet32 (PcdAmdEnvironmentFlag) == 0 && ((PcdGet32 (PcdAmdEnvironment98) & BIT16) == 0) ) //Environment: 0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO, 4 - GC Lite
  {
    for (unIdx = 0; unIdx < BOARD_ID_EXPORTED_FIELD_LENGTH; ++unIdx)
    {
      u8Ofst = (UINT8)(0x93 + unIdx);
      RMB_EcBoard_Info.arr[unIdx] = InnerEcIdxRead (AmdCpmTablePpi, u8Ofst);
    }

    do
    {
      u8PlatSel = (UINT8) RMB_EcBoard_Info.f.bid;

      // Set the Platform Selection PCD
      PcdSet8S (PcdRmbBoardSelect, u8PlatSel);

      // Set the Current Platform Id.
      PcdSet16S (CurrentPlatformId, (UINT16)u8PlatSel);
      DEBUG((DEBUG_INFO, "  EC Report : CurrentBoardId=%04X\n", u8PlatSel));

      // Set the Platform Reversion
      u8PlatSel = (UINT8)RMB_EcBoard_Info.f.pcbr;
      PcdSet8S (PcdRmbPcbVersion, u8PlatSel);
      DEBUG((DEBUG_INFO, "  EC Report : CurrentBoardPcb Rev=%04X\n", u8PlatSel));

      // Set the Platform Sku
      u8PlatSel = (UINT8)RMB_EcBoard_Info.f.sku;
      PcdSet8S (PcdRmbSku, u8PlatSel);
      DEBUG((DEBUG_INFO, "  EC Report : CurrentBoardSku Rev=%04X\n", u8PlatSel));

      // Get and EC FW version
      /*
       * Offset (0xB8),  // 5 + 4 + 6
       * ECRV, 40,       // EC firmware revision  "0.x.x"
       * ECDT, 32,       // EC firmware timestamp "9B02"
       * ECTS, 48,       // EC firmware timestamp "230653"
       * */
      u8Val = InnerEcIdxRead (AmdCpmTablePpi, 0xB8);
      ECFwVersion |= u8Val;
      ECFwVersion <<= 8;

      u8Val = InnerEcIdxRead (AmdCpmTablePpi, 0xBA);
      ECFwVersion |= u8Val;
      ECFwVersion <<= 8;

      u8Val = InnerEcIdxRead (AmdCpmTablePpi, 0xBC);
      ECFwVersion |= u8Val;
      DEBUG((DEBUG_INFO, "ECFwVersion = 0x%x\n", ECFwVersion));
      PcdSet32S (PcdECFwVersion, ECFwVersion);

    } while (FALSE);
  }

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Config SLT Board Initial value
 *
 * @param[in]     AmdCpmTablePpi          Pointer to AMD CPM Table PPI
 * @param[in]     AmdPbsConfiguration     Value. 0: Disable. 1: Enable
 *
 */
EFI_STATUS  ConfigBoardSLTSku (
    AMD_CPM_TABLE_PPI             *AmdCpmTablePpi,
    AMD_PBS_SETUP_OPTION          *AmdPbsConfiguration
)
{
  EFI_STATUS          Status = EFI_SUCCESS;

  //  If ((0x93 == 0x0X) && (0x94 == 0x02))   this is Lilac SLT
  //  If ((0x93 == 0x1X) && (0x94 == 0x03))   this is Mayan SLT
  //  If ((0x93 == 0x5X) && (0x94 == 0x04))   this is Lilac KD SLT
  //  If ((0x93 == 0x6X) && (0x94 == 0x04))   this is Mayan KD SLT
  if ((PcdGet8 (PcdRmbBoardSelect) == 1 && PcdGet8 (PcdRmbSku) == 3) || (PcdGet8 (PcdRmbBoardSelect) == 6 && PcdGet8 (PcdRmbSku) == 4)) {   // Mayan or Mayan KD SLT
    AmdPbsConfiguration->Pciex1PowerEn = 1;
    AmdPbsConfiguration->WwanPowerEn = 0;
    AmdPbsConfiguration->ClockRequest0 = 0;
    PcdSetBoolS (PcdSpreadSpectrum, 0);
    PcdSetBoolS (PcdCfgAEREnable, 0);
  }
  if ((PcdGet8 (PcdRmbBoardSelect) == 0 && PcdGet8 (PcdRmbSku) == 2) || (PcdGet8 (PcdRmbBoardSelect) == 5 && PcdGet8 (PcdRmbSku) == 4)) {   // Lilac or Lilac KD SLT
    AmdPbsConfiguration->Pciex1PowerEn = 1;
    AmdPbsConfiguration->WwanPowerEn = 0;
    AmdPbsConfiguration->ClockRequest0 = 0;

    AmdPbsConfiguration->OddPwrEn = 0;
    AmdPbsConfiguration->HDDPwrEn = 0;
    AmdPbsConfiguration->DtSlotPowerControl = 0;
    AmdPbsConfiguration->Ssd1PwrEn = 1;
    PcdSetBoolS (PcdSpreadSpectrum, 0);
    PcdSetBoolS (PcdCfgAEREnable, 0);
  }

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Update GPIO Init Config Table
 *
 * @param[in]     AmdCpmTablePpi          Pointer to AMD CPM Table PPI
 * @param[in]     pInitGpioTbl            GPIO Init config table
 * @param[in]     AmdPbsConfiguration                      Value. 0: Disable. 1: Enable
 *
 */
EFI_STATUS  ConfigPlatformGpioTable (
    AMD_CPM_TABLE_PPI             *AmdCpmTablePpi,
    AMD_CPM_GPIO_INIT_TABLE       *pInitGpioTbl,
    AMD_PBS_SETUP_OPTION          *AmdPbsConfiguration,
    UINT8                         PlatformSel
)
{
  UINTN                     unIdx = 0;
  EFI_STATUS                Status = EFI_SUCCESS;
  AMD_CPM_GPIO_INIT_TABLE   *pSeleGpioInitTbl = NULL;
  do
  {
    pSeleGpioInitTbl = &gRmbGpioInitTable;
    //
    // Copy the Selection GPIO Table
    //
    CopyMem (&pInitGpioTbl->Header, &pSeleGpioInitTbl->Header, sizeof (pInitGpioTbl->Header));
    for (unIdx = 0; ;++unIdx)
    {
      if (unIdx == sizeof(pSeleGpioInitTbl->GpioList)/sizeof(pSeleGpioInitTbl->GpioList[0]) )
        break;

      CopyMem (&pInitGpioTbl->GpioList[unIdx], &pSeleGpioInitTbl->GpioList[unIdx], sizeof (pSeleGpioInitTbl->GpioList[0]));

      // Check, it is the End GPIO entry.
      if (pSeleGpioInitTbl->GpioList[unIdx].Pin == 0xFF)
        break;
    }

  } while (FALSE);

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Update GPIO Device Config Table
 *
 * @param[in]     GpioDeviceConfigTablePtr  Pointer to GPIO Device Config Table
 * @param[in]     DeviceId                  Device Id to update
 * @param[in]     Mode                      Value. 0: Disable. 1: Enable
 *
 */
VOID
SetDevice (
  IN       AMD_CPM_GPIO_DEVICE_CONFIG_TABLE     *GpioDeviceConfigTablePtr,
  IN       UINT8                                DeviceId,
  IN       UINT8                                Mode
  )
{
  UINT8   Index;
  UINT8   GpioDeviceId;
  for (Index = 0; Index < AMD_GPIO_DEVICE_SIZE; Index ++) {
    GpioDeviceId = GpioDeviceConfigTablePtr->DeviceList[Index].DeviceId;
    if (GpioDeviceId == 0xFF || GpioDeviceId == 0x00) {
      break;
    }
    if (GpioDeviceId == DeviceId) {
      GpioDeviceConfigTablePtr->DeviceList[Index].Config.Setting.Enable = Mode;
      break;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enable EC Burst (0x82)
 *
 * This command byte allows OSPM to request dedicated attention from the EC and
 * (except for critical events) prevents the EC from doing tasks other than
 * receiving command and data from the host processor.
 *
 * @param[in]     AmdCpmTablePpi            Pointer to AmdCpmTablePpi
 *
 */
/*UINT8 EcBurstEnable (AMD_CPM_TABLE_PPI * AmdCpmTablePpi)
{
#define _READ__IO8_(p)      AmdCpmTablePpi->CommonFunction.IoRead8(p)
#define _WRITE_IO8_(p, d)   AmdCpmTablePpi->CommonFunction.IoWrite8((p), (d))
#define _WAIT_IBE_(x)       {                                                           \
                               UINT32 waitCnt = (x);                                    \
                               UINT8  data8;                                            \
                               do {                                                     \
                                 data8 = _READ__IO8_ (0xCD6);                           \
                                 data8 = _READ__IO8_ (0xCD6);                           \
                                 data8 = _READ__IO8_ (CFG_CPM_ECRAM_BASE_ADDR + 0x04);  \
                               } while (waitCnt-- && (data8 & 0x02));                   \
                            }
#define _WAIT_OBF_(x)       {                                                           \
                               UINT32 waitCnt = (x);                                    \
                               UINT8  data8;                                            \
                               do {                                                     \
                                 data8 = _READ__IO8_ (0xCD6);                           \
                                 data8 = _READ__IO8_ (0xCD6);                           \
                                 data8 = _READ__IO8_ (CFG_CPM_ECRAM_BASE_ADDR + 0x04);  \
                               } while (waitCnt-- && !(data8 & 0x01));                  \
                             }
  _WAIT_IBE_(5000);
  _WRITE_IO8_(CFG_CPM_ECRAM_BASE_ADDR + 0x04, 0x82);
  _WAIT_OBF_(5000);
  return _READ__IO8_(CFG_CPM_ECRAM_BASE_ADDR);

#undef _READ__IO8_
#undef _WRITE_IO8_
#undef _WAIT_IBE_
#undef _WAIT_OBF_
}*/


typedef struct MemTestResultVariable {
  UINT64 TestRunCount;
  UINT64 MemTestResult[2];
} MEMTEST_RESULT_VARIABLE;


VOID
UpdatePerCoreDldoPsmMarginCtrl()
{
  EFI_STATUS                       Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariablePpi;
  UINTN                            Size;
  EFI_GUID                         AmdMemTestResultVariableGuid = { 0xB07FE30F, 0xA2D3, 0x497C, {0xB6, 0xE4, 0x17, 0x32, 0x6B, 0x2F, 0x24, 0x5A }};
  MEMTEST_RESULT_VARIABLE          MemTestResultVar;
  UINT32                           PcdValue;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariablePpi
             );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Size = sizeof (MEMTEST_RESULT_VARIABLE);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          L"MEM_TEST_RESULT",
                          &AmdMemTestResultVariableGuid,
                          NULL,
                          &Size,
                          &MemTestResultVar
                          );
  if (Status == EFI_SUCCESS) {
    PcdValue = (UINT32)(((MemTestResultVar.MemTestResult[1] & 0xFFFF) << 16) | (MemTestResultVar.MemTestResult[0] & 0xFFFF));
    PcdSet32S(PerCoreDldoPsmMarginCtrl, PcdValue);

    DEBUG((DEBUG_INFO, "PerCoreDldoPsmMarginCtrl = %x\n", PcdGet32(PerCoreDldoPsmMarginCtrl)));
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * CPM Override Function After AMD CPM Table PPI
 *
 * This function updates CPM OEM Tables according to setup options or the value to be detected
 * on run time after AMD CPM Table PPI is installed.
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Function initialized successfully
 * @retval        EFI_ERROR      Function failed (see error for more details)
 *
 */
EFI_STATUS
EFIAPI
AmdCpmTableOverride (
  IN       EFI_PEI_SERVICES           **PeiServices
  )
{
  EFI_STATUS                          Status;
  AMD_CPM_TABLE_PPI                   *AmdCpmTablePpi;
  AMD_CPM_MAIN_TABLE                  *MainTablePtr;
  AMD_CPM_PCIE_CLOCK_TABLE            *PcieClockTablePtr;
  AMD_CPM_DXIO_TOPOLOGY_TABLE         *DxioTopologyTablePtr;
  AMD_CPM_GPIO_DEVICE_CONFIG_TABLE    *GpioDeviceConfigTablePtr;
  AMD_CPM_OTHER_HOTPLUG_CARD_TABLE    *DtHotplugTablePtr;
  AMD_CPM_GPIO_DEVICE_POWER_TABLE     *PowerTablePtr;
  AMD_PBS_SETUP_OPTION                AmdPbsConfiguration;
  AMD_CPM_GPIO_INIT_TABLE             *GpioInitTablePtr;
  AMD_CPM_DISPLAY_FEATURE_TABLE       *DisplayFeatureTablePtr;
//  AMD_CPM_LOW_POWER_ETHERNET_TABLE    *LowPowerEthernetTablePtr;
  AMD_CPM_GPIO_DEVICE_RESET_TABLE     *GpioDeviceResetTablePtr;
//  AMD_CPM_THUNDERBOLT_TABLE           *ThunderboltTablePtr;
  AMD_CPM_PCI_RES_RES_TABLE           *PciResResTablePtr;
  AMD_CPM_GPIO_DEVICE_DETECTION_TABLE *GpioDeviceDetectionTablePtr;
  AMD_CPM_DEVICE_PATH_TABLE           *DevicePathTablePtr;
  AMD_CPM_GEVENT_INIT_TABLE           *GeventTablePtr;
/*  AMD_CPM_WIRELESS_LAN_RECOVERY_TABLE *WirelessLanRecoveryTablePtr;
  AMD_CPM_WIRELESS_BUTTON_TABLE       *WirelessButtonTablePtr;*/
  AMD_CPM_UCSI_TABLE                  *UcsiTablePtr;
  AMD_CPM_PRE_INIT_TABLE              *PreInitTablePtr;
  UINTN                               Index;
  UINT8                               CurPlatformSelect;
  DXIO_PORT_DESCRIPTOR                *DxioPort;
  DXIO_PORT_DESCRIPTOR                TmpDxioPort;
  UINT8                               Value8;
//  UINT8                               OfstU8;
//  UINT32                              DeviceEnableMap;
  UINTN                               *TableAddress;
//  CONST BOOLEAN                       CPM_SETUP_OVERRIDE = TRUE;
//  BOOLEAN                             AllowEcAccess = TRUE;
//  T_BOARD_ID_REWORK_FIELD             HwReworkMap = {0};        // Initial for Zero.
  UINT8                               *TmpBufPtr = NULL;
  ATOM_COMMON_TABLE_HEADER            *AtomCommonTableHeader;
  ATOM_N6_DISPLAY_PHY_TUNING_SET      *DisplayPhyTuningInfo;
#ifdef MODERN_STANDBY_SUPPORT
  UINT32                              GpioData = 0;
  S0I3_GPIO_RST_EP                    *S0I3_GPIO_RST_EP1;
  S0I3_GPIO_RST_EP                    *S0I3_GPIO_RST_EP3;
#endif
  UINT32                              CpuidFamilyModelStepping = 0;
  GpioTable_t                         *GpioSmuConfigTable = NULL;
  UINT8                               GpioSmuConfigCount = 0;
  UINT8                               Offset;
//  UINT32                              *NoTrainLinkTable = NULL;

  //GPIO CONFIG table to SMU for S0I3 and Z-state
  GpioConfig_t GpioSmuConfigZ10Table[] = {
    {0x02D0251C, 1, 3, 4, 0x00},  //GPIO7 - ec EVENT
    {0x02D02558, 1, 3, 4, 0x00},  //GPIO22 - ESPI ALERT
    {0x02D0257C, 1, 3, 4, 0x00},  //GPIO31 CLK_REQ#
    {0x02D02598, 1, 3, 4, 0x00},  //GPIO38 CLK_REQ#
    {0x02D0259c, 1, 3, 4, 0x00},  //GPIO39 CLK_REQ#
  };
/*
  GpioConfig_t GpioSmuConfigS0i3Table[] = {
    {0x02D02638, 0, 2, 2, 0x00},  //GPIO78 SSD0 AUX_RST#
    {0x02D0263C, 0, 2, 2, 0x00},  //GPIO79 SSD1 AUX_RST#
  };
*/

  // Follow EC Config Table, move to function innver variable, is fixed for PLAT-68985 System Hange when S3 resume
  /*
   * Here for record some Tips,
   * 1. For SMBUS0_BUFF_EN the two pin, so the rule will be:
   *   When use the SMBUS0, enable it, than after finished SMBUS communication, it should be closed immediate.
   *   The reason is avoid the extra loading for I2C logic.
   * 2. PLAT-83090: Need to keep SMBUS1 opened for DASH OOB operations.
   * *//*
  struEcTbl PorgEcConfig[] =
  {
      // Index      AndMask                                       OrMask
      {ECRAMxA0,    0xFF,                                         0x00 },
      {ECRAMxA1,    0xFF,                                         0x00 },
      {ECRAMxA2,    0xFF,                                         EVAL_19V_EN | EVAL_SLOT_PWR},
      {ECRAMxA3,    0xFF,                                         0x00 },
      {ECRAMxA7,    0xFF,                                         TPM_PWR_EN},
      {ECRAMxA8,    U8_CLEAR_BIT (SMBUS0_BUFF_EN),                SMBUS1_BUFF_EN},
      {ECRAMxA9,    0xFF,                                         0x00 },
      {ECRAMxAA,    0xFF,                                         0x00 },
      {ECRAMxAB,    0xFF,                                         0x00 },
      {ECRAMxAC,    0xFF,                                         0x00 },
      {ECRAMxAD,    0xFF,                                         0x00 },
      {ECRAMxAE,    0xFF,                                         0x00 },
      {ECRAMxB3,    0xFF,                                         0x00 },
      {ECRAMxB4,    0xFF,                                         0x00 },
      {ECRAMxB6,    0xFF,                                         0x00 },
      {ECRAMxB7,    0xFF,                                         0x00 },
      {ECRAMxC7,    0xFF,                                         0x00 },
      {ECRAMxC8,    0xFF,                                         0x00 },
      {ECRAMxCF,    U8_CLEAR_BIT (AC_DC_SWITCH_EN),               0x00 },
      {ECRAMxFE,    0xFF,                                         0x00 },
  };*/
#define EcTblRead(_unIdx) InnerEcIdxRead (AmdCpmTablePpi, _unIdx)
#define EcImmWrite(_unIdx,_val) ImmEcIdxWrite (AmdCpmTablePpi, _unIdx, _val)
#define EcImmRegClearOr(_unIdx,_u8AndVal,_u8OrVal) ImmEcRegClearOr (AmdCpmTablePpi, _unIdx, _u8AndVal, _u8OrVal)

  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Start\n"));

  GpioSmuConfigTable = AllocateZeroPool(sizeof(GpioTable_t));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverrideGpio-SmuConfigTable address: 0x%x\n", GpioSmuConfigTable));
  if (GpioSmuConfigTable) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverrideGpio-Copy GpioSmuConfigZ10Table\n"));
    GpioSmuConfigCount = sizeof(GpioSmuConfigZ10Table)/sizeof(GpioConfig_t);
    CopyMem (GpioSmuConfigTable, &GpioSmuConfigZ10Table, (UINTN)sizeof(GpioSmuConfigZ10Table));
  }

  Index = 0;
  Value8 = 0;

  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-LocatePpi-gAmdCpmTablePpiGuid\n"));
  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gAmdCpmTablePpiGuid,
                           0,
                           NULL,
                           (VOID**)&AmdCpmTablePpi
                           );
  if (EFI_ERROR (Status))
  {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-End-1-Status=%r\n", Status));
    return Status;
  }

  MainTablePtr = AmdCpmTablePpi->MainTablePtr;

  //
  // Get/Config Board ID, it will be initialed priority.
  //
//  ConfigBoardID (AmdCpmTablePpi);

  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-GetAmdPbsConfiguration\n"));
  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-End-2-Status=%r\n", Status));
    return Status;
  }

//  ConfigBoardSLTSku (AmdCpmTablePpi, &AmdPbsConfiguration);

  CurPlatformSelect = PcdGet8 (PcdRmbBoardSelect);
  DEBUG ((DEBUG_INFO, "[%d]: Current Platform Selection: %x\n", __LINE__, CurPlatformSelect));

  DxioTopologyTablePtr        = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_DXIO_TOPOLOGY);
  PcieClockTablePtr           = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_PCIE_CLOCK);
  GpioDeviceConfigTablePtr    = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_GPIO_DEVICE_CONFIG);
  DtHotplugTablePtr           = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_PCIE_OTHER_HOTPLUG_CARD);
  PowerTablePtr               = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_GPIO_DEVICE_POWER);
  GpioInitTablePtr            = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_GPIO_INIT);
  DisplayFeatureTablePtr      = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_DISPLAY_FEATURE);
//  LowPowerEthernetTablePtr    = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_PCIE_LOW_POWER_ETHERNET);
//  WirelessLanRecoveryTablePtr = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_WIRELESS_LAN_RECOVERY);
  GpioDeviceResetTablePtr     = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_GPIO_DEVICE_RESET);
//  ThunderboltTablePtr         = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_THUNDERBOLT);
  PciResResTablePtr           = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_PCI_RESERVED_RESOURCE);
  GpioDeviceDetectionTablePtr = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_GPIO_DEVICE_DETECTION);
  DevicePathTablePtr          = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_DEVICE_PATH);
  GeventTablePtr              = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_GEVENT_INIT);
  UcsiTablePtr                = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_UCSI);
//  WirelessButtonTablePtr      = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_WIRELESS_BUTTON);
  PreInitTablePtr             = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_PRE_INIT);

/*  DEBUG((DEBUG_INFO, "OEM-PEI-%a-NotifyPpi-gAmdNbioPcieTrainingDonePpiGuid-For-SpecificQCommWlanPreInitOverride\n", __FUNCTION__));
  Status = (**PeiServices).NotifyPpi (PeiServices, &mPreinitNotify);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "OEM-PEI-%a-Notify-Fail-Status=%r\n", __FUNCTION__, Status));
    return Status;
  }*/

  do
  {
    // For CPM Coverity Error check.

    if (NULL == DxioTopologyTablePtr)
    {
      DEBUG ((DEBUG_INFO, "[CPM Fox]: Fatal Error, Did not Get the DxioTopologyTablePtr Table."));
      Status = EFI_INVALID_PARAMETER;
      ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
      return Status;
    }

    if (NULL == PcieClockTablePtr)
    {
      DEBUG ((DEBUG_INFO, "[CPM Fox]: Fatal Error, Did not Get the PcieClockTablePtr Table."));
      Status = EFI_INVALID_PARAMETER;
      ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
      return Status;
    }

    if (NULL == PowerTablePtr)
    {
      DEBUG ((DEBUG_INFO, "[CPM Fox]: Fatal Error, Did not Get the PowerTablePtr Table."));
      Status = EFI_INVALID_PARAMETER;
      ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
      return Status;
    }

    if (NULL == DisplayFeatureTablePtr)
    {
      DEBUG ((DEBUG_INFO, "[CPM Fox]: Fatal Error, Did not Get the DisplayFeatureTablePtr Table."));
      Status = EFI_INVALID_PARAMETER;
      ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
      return Status;
    }
  } while (FALSE);

  // WLAN PLDR and BT PLDR support.
/*  if (WirelessLanRecoveryTablePtr) {
    WirelessLanRecoveryTablePtr->WlanPldrSupport = AmdPbsConfiguration.WlanRecovery;
    WirelessLanRecoveryTablePtr->BtPldrSupport = AmdPbsConfiguration.BtPldr;
    if ((AmdPbsConfiguration.WlanRecovery == 0) && (AmdPbsConfiguration.BtPldr == 0)) {
      AmdCpmTablePpi->CommonFunction.RemoveTable (AmdCpmTablePpi, WirelessLanRecoveryTablePtr);
    }
  }
*/
  // Initial Platform Gpio Table
  ConfigPlatformGpioTable (AmdCpmTablePpi, GpioInitTablePtr, &AmdPbsConfiguration, CurPlatformSelect);

  // disable EC support if _98[16] = 1 (disable EC support), this is used to support simnow running
  //Environment: 0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO, 4 - GC Lite
/*  if (PcdGet32 (PcdAmdEnvironmentFlag) == 0  && ((PcdGet32 (PcdAmdEnvironment98) & BIT16) == 0) )
  {
    AllowEcAccess = TRUE;
  }
  else
  {
    AllowEcAccess = FALSE;
  }

  //
  //  Reading the Rework setting for Mayan/Lialc common field
  //
  if (TRUE == AllowEcAccess)
  {
    // For the Special Rework, it will read from EC ram. it can get from T_BOARD_ID_EXPORTED_FIELD::specRework (Bit00~Bit48)
    for (Index = 0; Index < 6; ++Index)
    {
      OfstU8 = (UINT8)((0x93+3) + Index);   // "specRework" Start from ECRAMx96
      HwReworkMap.Raw[Index] = EcTblRead (OfstU8);
    }
    // 0x4E need to do special command access.
    // 1. ECRAMx31 = 0
    // 2. ECRAMx30 = 1
    // 3. then ECRAMx1E is board id rom offset 0x4E
    // 4. Close, ECRAMx31 = 0xFF;
    EcImmWrite (0x31, 0);
    EcImmWrite (0x30, 1);
    Index = 8;
    HwReworkMap.Raw[Index] = EcTblRead (0x1E);
    DEBUG((DEBUG_INFO, "[%d]: HwReworkMap.4E[%02x]\n", __LINE__, HwReworkMap.Raw[Index]));
    EcImmWrite (0x31, 0xFF);
    // 0x4F can get from T_BOARD_ID_EXPORTED_FIELD::"memType" byte 9.
    Index = 9;
    OfstU8 = 0x93+9;
    HwReworkMap.Raw[Index] = EcTblRead (OfstU8);

    //
    // PLAT-78461, Notify EC USB ready in ECRAM offset 0xFE bit0
    //
    Status = (**PeiServices).NotifyPpi (PeiServices, &mFchInitRdyNotifyEcList);
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "OEM-PEI-NotifyPpi-gAmdFchInitPpiGuid-Status=%r\n", Status));
    }
  }
*/

  //
  // Telemetry Slope/Offset (BIOS to SMU)
  //
  // TODO: Need to review the Vdd setting later
  DEBUG((DEBUG_INFO, "Set Telemetry for Fox board \n"));
  PcdSet32S(PcdTelemetry_VddcrVddfull_Scale_Current, 88755);  // mA
  PcdSet32S(PcdTelemetry_VddcrVddOffset, (UINT32) (0 - 75));
  PcdSet32S(PcdTelemetry_VddcrSocfull_Scale_Current, 29577);   // mA
  PcdSet32S(PcdTelemetry_VddcrSocOffset, 296);

  /*
   * PLAT-87633 [RMB-FP7] Add PCD settings in Mayan BIOS for PBO function
   * Check with design, Those value should be keep as default.
   * This value will for Lilac/Mayan Platform.
   * */
  PcdSet32S(PcdSlowPptLimit, 100000);                 //SLOW_PPT_LIMIT
  PcdSet32S(PcdFastPptLimit, 125000);                 //FAST_PPT_LIMIT
  PcdSet32S(PcdVrmCurrentLimit, 70000);               //VRM_VDD_CURRENT_LIMIT
  PcdSet32S(PcdVrmMaximumCurrentLimit, 150000);       //VRM_VDD_MAXIMUM_CURRENT_LIMIT
  PcdSet32S(PcdVrmSocCurrentLimit, 20000);            //VRM_SOC_CURRENT_LIMIT
  PcdSet32S(PcdVrmSocMaximumCurrentLimit, 30000);     //VRM_SOC_MAXIMUM_CURRENT_LIMIT

  if ((PLATFORM_SelLilac == CurPlatformSelect) || (PLATFORM_SelLilacDAP== CurPlatformSelect) || (PLATFORM_SelLilacKD== CurPlatformSelect))
  {
    // Only Lilac support NvmeRaid
    if (AmdPbsConfiguration.NvmeRaid == 1)
    {
      PcdSet8S (PcdNvmeRaid, 0x01);
    }
  }
  else
  {
    // Mayan PLAT-77468 RMB FP7 DPTC ALIB interface function check
    do
    {
      DEBUG((DEBUG_INFO, "  AmdPbsConfiguration.AmdDptcControl [%x]\n", AmdPbsConfiguration.AmdDptcControl));
      if (0 == AmdPbsConfiguration.AmdDptcControl)
      {
        break;
      }

      Value8 = EcTblRead(ECRAMxAD);
      DEBUG((DEBUG_INFO, "  ECRAMxAD Value[%x]\n", Value8));
      if ( Value8 & DOCK_UNPLUG )
      {
        DEBUG((DEBUG_INFO, "  ECRAMxAD, BIT5 DOCK_IN# non detected \n"));
        PcdSet32S (PcdSustainedPowerLimit, 15000);          // SUSTAINED_POWER_LIMIT
        PcdSet32S (PcdFastPptLimit, 30000);                 // FAST_PPT_LIMIT
        PcdSet32S (PcdSlowPptLimit, 25000);                 // SLOW_PPT_LIMIT
        PcdSet32S (PcdSlowPptTimeConstant, 5);              // SLOW_PTT_TIME_CONSTANT
        PcdSet32S (PcdCfgThermCtlValue, 100);               // THERMCTL_LIMIT
        PcdSet32S (PcdVrmCurrentLimit, 40000);              // VRM_VDD_CURRENT_LIMIT
        PcdSet32S (PcdVrmMaximumCurrentLimit, 90000);       // VRM_VDD_MAXIMUM_CURRENT_LIMIT
        PcdSet32S (PcdVrmSocCurrentLimit, 13000);           // VRM_SOC_CURRENT_LIMIT
        PcdSet32S (PcdProchotlDeassertionRampTime, 20);     // PROCHOT_L_DEASSERTION_RAMP_TIME
      }
      else
      {
        // Dock IN
        DEBUG((DEBUG_INFO, "  ECRAMxAD, BIT5 DOCK_IN# detected \n"));
        PcdSet32S (PcdSustainedPowerLimit, 25000);          // SUSTAINED_POWER_LIMIT
        PcdSet32S (PcdFastPptLimit, 48000);                 // FAST_PPT_LIMIT
        PcdSet32S (PcdSlowPptLimit, 37000);                 // SLOW_PPT_LIMIT
        PcdSet32S (PcdSlowPptTimeConstant, 4);              // SLOW_PTT_TIME_CONSTANT
        PcdSet32S (PcdCfgThermCtlValue, 90);                // THERMCTL_LIMIT
        PcdSet32S (PcdVrmCurrentLimit, 48000);              // VRM_VDD_CURRENT_LIMIT
        PcdSet32S (PcdVrmMaximumCurrentLimit, 105000);      // VRM_VDD_MAXIMUM_CURRENT_LIMIT
        PcdSet32S (PcdVrmSocCurrentLimit, 14000);           // VRM_SOC_CURRENT_LIMIT
        PcdSet32S (PcdProchotlDeassertionRampTime, 10);     // PROCHOT_L_DEASSERTION_RAMP_TIME
      }

      // Did not have the follow setting value.
//      PcdSet32S (PcdVrmSocMaximumCurrentLimit, 30000);     //VRM_SOC_MAXIMUM_CURRENT_LIMIT
    } while (FALSE);

  }

/*
  DeviceEnableMap = PcdGet32 (FchRTDeviceEnableMap);
  if ((AmdPbsConfiguration.NfcSensor >= 1) && ((DeviceEnableMap & (UINT32) BIT11) == BIT11)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-if (NfcSensor>=1 & DeviceEnableMap BIT11=1)\n"));
    DeviceEnableMap &= ~(UINT32) BIT11;               // # BIT11 = 0 disable UART0
    PcdSet32S(FchRTDeviceEnableMap, DeviceEnableMap);
  }
*/
  if (GpioInitTablePtr) {
    Index = 0;
    while (GpioInitTablePtr->GpioList[Index].Pin != 0xFF) {
      if ((GpioInitTablePtr->GpioList[Index].Pin == 140 || GpioInitTablePtr->GpioList[Index].Pin == 142) && (PcdGet64 (PcdAmdIdsDebugPrintSerialPort) == 0xFEDCA000)) {
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Change GPIO140, GPIO142 IOMUX to 2 for UART1\n"));
        // GPIO_DEF_V2 (0, 0, 140, GPIO_FUNCTION_2, GPIO_NA, GPIO_PU_PD_DIS), // UART1_TXD
        // GPIO_DEF_V2 (0, 0, 142, GPIO_FUNCTION_2, GPIO_NA, GPIO_PU_PD_DIS), // UART1_RXD
        GpioInitTablePtr->GpioList[Index].Setting.Raw = 0xA88;
      } else if ((GpioInitTablePtr->GpioList[Index].Pin == 135 || GpioInitTablePtr->GpioList[Index].Pin == 137) && (PcdGet64 (PcdAmdIdsDebugPrintSerialPort) == 0xFEDCF000)) {
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Change GPIO135, GPIO137 IOMUX to 2 for UART3\n"));
        // GPIO_DEF_V2 (0, 0, 135, GPIO_FUNCTION_2, GPIO_NA, GPIO_PU_PD_DIS), // UART3_TXD
        // GPIO_DEF_V2 (0, 0, 137, GPIO_FUNCTION_2, GPIO_NA, GPIO_PU_PD_DIS), // UART3_RXD
        GpioInitTablePtr->GpioList[Index].Setting.Raw = 0xA88;
      } else if ((GpioInitTablePtr->GpioList[Index].Pin == 153 || GpioInitTablePtr->GpioList[Index].Pin == 154) && (PcdGet64 (PcdAmdIdsDebugPrintSerialPort) == 0xFEDD1000)) {
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Change GPIO153, GPIO154 IOMUX to 1 for UART4\n"));
        // GPIO_DEF_V2 (0, 0, 153, GPIO_FUNCTION_1, GPIO_NA, GPIO_PU_PD_DIS), // UART4_CTS#
        // GPIO_DEF_V2 (0, 0, 154, GPIO_FUNCTION_1, GPIO_NA, GPIO_PU_PD_DIS), // UART4_RTS#
        GpioInitTablePtr->GpioList[Index].Setting.Raw = 0x988;
      }

      if ((GpioInitTablePtr->GpioList[Index].Pin == 31) && (AmdPbsConfiguration.SpiCs3 == 1)) {
        // GPIO_DEF_V2 (0, 0, 31,  GPIO_FUNCTION_0,  GPIO_NA,          GPIO_PU_PD_DIS),      // SPI_CS3
        GpioInitTablePtr->GpioList[Index].Setting.Raw = 0x888;
      }
      Index++;
    }
  }

  //
  // PLAT-84279
  //
  if (0 == AmdPbsConfiguration.PdUsb4PlatfromControl)
  {
  //  EcTblSet (ECRAMxFE, 0, USB_EN_DIS);
  }
  else
  {
//    EcTblSet (ECRAMxFE, USB_EN_DIS, 0);
  }

  //
  // PLAT-88468
  //
  if (PciResResTablePtr) {
    for (Index = 0; Index < AMD_RES_RES_SIZE; Index++) {
      if (PciResResTablePtr->ResRes[Index].DevType == CPM_END_OF_TABLE) {
        break;
      }
      PciResResTablePtr->ResRes[Index].ResBus = AmdPbsConfiguration.Usb4BusReserved;
      PciResResTablePtr->ResRes[Index].ResNonPrefetchableMmio = MultU64x32 (MultU64x32 (AmdPbsConfiguration.Usb4NonPrefetchMemoryReserved,   (UINT32)1024), (UINT32)1024); // The unit is MB
      PciResResTablePtr->ResRes[Index].ResPrefetchableMmio    = MultU64x32 (MultU64x32 (AmdPbsConfiguration.Usb4PrefetchMemoryReserved,      (UINT32)1024), (UINT32)1024); // The unit is MB
      PciResResTablePtr->ResRes[Index].AlignResNonPrefetch    = MultU64x32 (MultU64x32 (AmdPbsConfiguration.Usb4NonPrefetchMemoryAlign,      (UINT32)1024), (UINT32)1024); // The unit is MB
      PciResResTablePtr->ResRes[Index].AlignResPrefetch       = MultU64x32 (MultU64x32 (AmdPbsConfiguration.Usb4PrefetchMemoryReservedAlign, (UINT32)1024), (UINT32)1024); // The unit is MB
    }
  } // End of if (PciResResTablePtr)

  //
  // PLAT-84325 / PLAT-84325 / PLAT-84325
  //
  /*
  if (0 == AmdPbsConfiguration.ApuProcHotSetting)
  {
    EcTblSet (ECRAMxB7, BIT6, 0); // Disable pure-DC case
    EcTblSet (ECRAMxB7, BIT3, 0); // Disable pure-AC case
  }
  else if (1 == AmdPbsConfiguration.ApuProcHotSetting)
  {
    EcTblSet (ECRAMxB7, 0, BIT6); // Enable  pure-DC case
    EcTblSet (ECRAMxB7, BIT3, 0); // Disable pure-AC case
  }
  else if (2 == AmdPbsConfiguration.ApuProcHotSetting)
  {
    EcTblSet (ECRAMxB7, BIT6, 0); // Disable pure-DC case
    EcTblSet (ECRAMxB7, 0, BIT3); // Enable  pure-AC case
  }
  else if (3 == AmdPbsConfiguration.ApuProcHotSetting)
  {
    EcTblSet (ECRAMxB7, 0, BIT6); // Enable  pure-DC case
    EcTblSet (ECRAMxB7, 0, BIT3); // Enable  pure-AC case
  }
*/
  //
  // Select for UnusedGppClkOff and ClockRequest_x option
  //
  if ((PLATFORM_SelLilacKD== CurPlatformSelect) || (PLATFORM_SelMayanKD== CurPlatformSelect)
      || (PLATFORM_SelLilac== CurPlatformSelect) || (PLATFORM_SelMayan== CurPlatformSelect) )
  {
    /*
     * Test Condition:
     * All slots populated with L1SS capable device except an intentional empty x8 slot (CLK_REQ0#)
     * */

    /*
     * Case A: ClockRequest_x==Enable and UnusedGppClkOff==Enable
     * */
    // ClockReqest_x := CLK_ID_request

    /*
     * Case B: ClockRequest_x==Enable and UnusedGppClkOff==Disable
     * */
    // ClockRequest0 := clock_eanble.
    // ClockReqest_x := CLK_ID_request
    if (0 == AmdPbsConfiguration.UnusedGppClkOff)
    {
      AmdPbsConfiguration.ClockRequest0 = 0;    // Always enable MXM Clk.
    }

    /*
     * Case C: ClockRequest_x==Disable and UnusedGppClkOff==Enable
     * */
    // ClockRequest0 := CLK_REQ0#.
    // ClockReqest_x == CLK_ENABLE
    if (0 == AmdPbsConfiguration.ClockRequest0 && 1 == AmdPbsConfiguration.UnusedGppClkOff)
    {
      AmdPbsConfiguration.ClockRequest0 = 1;
    }

    /*
     * Case D: ClockRequest_x==Disable and UnusedGppClkOff==Disable
     * */
    // ClockReqest_x == CLK_ENABLE
  }

  //
  // PLAT-82967, USE CPU stepping ID to determine the A0 or B0 and then inform to EC.
  //
  AmdCpmTablePpi->CommonFunction.CpuidRead (AmdCpmTablePpi, &CpuidFamilyModelStepping, NULL);
  if ((CpuidFamilyModelStepping & 0xF0) == 0x00) {
    DEBUG((DEBUG_INFO, "OEM-PEI-This is RMB A0 Part, CPUID=0x%x\n", CpuidFamilyModelStepping));
  //  EcTblSet (ECRAMxFE, FCH_RMB_B0_SKU, FCH_RMB_A0_SKU);
  } else {
    DEBUG((DEBUG_INFO, "OEM-PEI-This is RMB B0 Part, CPUID=0x%x\n", CpuidFamilyModelStepping));
    //EcTblSet (ECRAMxFE, FCH_RMB_A0_SKU, FCH_RMB_B0_SKU);

    // PLAT-89274 patch for B0 part only
    UINTN PMIOTableSize = PcdGetSize (PcdOemProgrammingTablePtr);
    TmpBufPtr = (UINT8*)PcdGetPtr (PcdOemProgrammingTablePtr);
    Index= 0;
    while  (TmpBufPtr[Index]!=0xFF) {Index += 4;}
    for (UINT8 i = 0; i < 16; i++, Index++) {
      TmpBufPtr[Index] = OEMFchPmioEPostInitTableB0 [i];
    }
    PcdSetPtrS (PcdOemProgrammingTablePtr, &PMIOTableSize, TmpBufPtr);
  }

  //
  // PLAT-85679: Load separated AmdCpmDisplayPhySettings.c for RMB A0 and B0 separately.
  //
  if ((CpuidFamilyModelStepping & 0xF0) == 0x00) {
    AtomCommonTableHeader = &table_header_a0;
    DisplayPhyTuningInfo = display_phy_tuning_info_a0;
  } else {
    AtomCommonTableHeader = &table_header_b0;
    DisplayPhyTuningInfo = display_phy_tuning_info_b0;
  }

  DEBUG((DEBUG_INFO, "Address of table_header=%08X\n", AtomCommonTableHeader));
  DEBUG((DEBUG_INFO, "Address of display_phy_tuning_info=%08X\n", DisplayPhyTuningInfo));
  PcdSet32S (PcdAmdDisplayPhyTuningSettingTableHeader, (UINT32)(UINTN)AtomCommonTableHeader);
  PcdSet32S (PcdAmdDisplayPhyTuningSettingTableContent, (UINT32)(UINTN)DisplayPhyTuningInfo);

  if ((CpuidFamilyModelStepping & 0xF0) == 0x00) {
    // RMB A0 part
    PcdSet8S(PcdUsb4PlatformOSCControl, 0xFF);
  } else {
    // RMB B0 part
    Value8 = 0xFF;
    if (PcdGetBool (PcdUsb4Rt0En) || PcdGetBool (PcdUsb4Rt1En)) {
      // Bit[0] USB Tunneling
      // Bit[1] DisplayPort Tunneling
      // Bit[2] PCI Express Tunneling
      // Bit[3] Inter-domain USB4
      Value8 = 0xB;
      if (PcdGetBool (PcdUsb4Rt0PcieTnlEn) || PcdGetBool (PcdUsb4Rt1PcieTnlEn)) {
        Value8 |= BIT2;
      }
    }
    PcdSet8S (PcdUsb4PlatformOSCControl, Value8);
  }

  DEBUG ((EFI_D_INFO, "-->  AmdPbsConfiguration.amdAcDcSwitch = 0x%x\n", AmdPbsConfiguration.amdAcDcSwitch));
  DEBUG ((EFI_D_INFO, "-->  AmdPbsConfiguration.amdDcTimer = 0x%x\n", AmdPbsConfiguration.amdDcTimer));
  DEBUG ((EFI_D_INFO, "-->  AmdPbsConfiguration.amdAcTimer = 0x%x\n", AmdPbsConfiguration.amdAcTimer));
 /* if (1 == AmdPbsConfiguration.amdAcDcSwitch)
  {
    // Set DC switch timer
    EcTblSet (ECRAMxC7, 0xFF, AmdPbsConfiguration.amdDcTimer);
    // Set AC switch timer
    EcTblSet (ECRAMxC8, 0xFF, AmdPbsConfiguration.amdAcTimer);
    // Enable AC/DC switch
    EcTblSet (ECRAMxCF, AC_DC_SWITCH_EN, AC_DC_SWITCH_EN);
    // fakeDcLevel
    EcTblSet (ECRAMxB3, 0x7F, AmdPbsConfiguration.fakeDcLevel);
  } else {
    // Disable AC/DC switch
    EcTblSet (ECRAMxCF, AC_DC_SWITCH_EN, 0);
  }

  // Dynamic P3T limit
  if (AmdPbsConfiguration.DynamicP3TLimitEnable == 0) {
    // Disable
    EcTblSet (ECRAMxB3, 0x80, 0);
    EcTblSet (ECRAMxB4, 0x80, 0);
  } else if (AmdPbsConfiguration.DynamicP3TLimitEnable == 1) {
    // Enable for DC-Only
    EcTblSet (ECRAMxB3, 0x80, 0);
    EcTblSet (ECRAMxB4, 0, 0x80);
  } else if (AmdPbsConfiguration.DynamicP3TLimitEnable == 2) {
    // Enable
    EcTblSet (ECRAMxB3, 0, 0x80);
    EcTblSet (ECRAMxB4, 0, 0x80);
  }

  if (0 == AmdPbsConfiguration.SttSensorsRpt)
  {
    // off -> clear both ECRAMxB6 bit[2] and bit[3]
    EcTblSet (ECRAMxB6, STT_ONBOARD_SENSOR_SW | STT_EVALCARD_SENSOR_SW, 0);
  }
  else if (1 == AmdPbsConfiguration.SttSensorsRpt)
  {
    // Report onboard sensors -> Set ECRAMxB6 bit[2] = 1, clear bit[3] to 0
    EcTblSet (ECRAMxB6, STT_EVALCARD_SENSOR_SW, STT_ONBOARD_SENSOR_SW);
  }
  else if (2 == AmdPbsConfiguration.SttSensorsRpt)
  {
    // Report onboard + eval card sensors -> Set both ECRAMxB6 bit[2] and bit[3]
    EcTblSet (ECRAMxB6, 0, STT_ONBOARD_SENSOR_SW | STT_EVALCARD_SENSOR_SW);
  }
*/
  // Display Features
  MainTablePtr->DisplayFeature.Config.D3ColdSupport = (AmdPbsConfiguration.D3ColdSupport >= 1) ? 1 : 0;
  MainTablePtr->DisplayFeature.Config.DummyD3ColdSupport = (AmdPbsConfiguration.D3ColdSupport == 2) ? 1 : 0;
  MainTablePtr->DisplayFeature.Config.DgpuDsmFunASupport = (AmdPbsConfiguration.DgpuDsmFunASupport == 1) ? 1 : 0;
  MainTablePtr->DisplayFeature.Config.DgpuDsmFunBSupport = (AmdPbsConfiguration.DgpuDsmFunBSupport == 1) ? 1 : 0;
  MainTablePtr->DisplayFeature.Config.HybridGraphicsSupport = (AmdPbsConfiguration.SpecialVgaFeature == 4) ? 1 : 0;
  if ((MainTablePtr->DisplayFeature.Config.D3ColdSupport == 0) && (MainTablePtr->DisplayFeature.Config.HybridGraphicsSupport == 0))
  {
    MainTablePtr->DisplayFeature.Config.DgpuHotPlugMode = 0;
    MainTablePtr->DisplayFeature.Config.DgpuNonHotPlugMode = 0;
    MainTablePtr->DisplayFeature.Config.PmeTurnOffSupport = 0;
    MainTablePtr->DisplayFeature.Config.D3ColdGen1Support = 0;
  } else
  {
    MainTablePtr->DisplayFeature.Config.DgpuHotPlugMode = ((AmdPbsConfiguration.DgpuHotPlugMode & 0x1) == 0x1) ? 1 : 0;
    MainTablePtr->DisplayFeature.Config.DgpuNonHotPlugMode = ((AmdPbsConfiguration.DgpuHotPlugMode & 0x2) == 0x2) ? 1 : 0;
    MainTablePtr->DisplayFeature.Config.PmeTurnOffSupport = (AmdPbsConfiguration.PmeTurnOffSupport == 1) ? 1 : 0;
    MainTablePtr->DisplayFeature.Config.D3ColdGen1Support = (AmdPbsConfiguration.D3ColdGen1Support == 1) ? 1 : 0;
  }
  MainTablePtr->DisplayFeature.Config.DgpuD3ColdHpdSupport = (AmdPbsConfiguration.DgpuD3ColdHpdSupport == 1) ? 1 : 0;
  MainTablePtr->DisplayFeature.Config.PulseGeneratorSupport = (AmdPbsConfiguration.DgpuHpdCircuitry == 1) ? 1 : 0;
  MainTablePtr->DisplayFeature.Config.DisableDgpuAudioInPX = (AmdPbsConfiguration.DgpuAudio == 0) ? 1 : 0;
  MainTablePtr->DisplayFeature.Config.DisableDgpuUsbPort = (AmdPbsConfiguration.DgpuUsbPort == 1) ? 1 : 0;
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-D3ColdSupport         = %X\n", MainTablePtr->DisplayFeature.Config.D3ColdSupport));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-DummyD3ColdSupport    = %X\n", MainTablePtr->DisplayFeature.Config.DummyD3ColdSupport));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-DgpuDsmFunASupport    = %X\n", MainTablePtr->DisplayFeature.Config.DgpuDsmFunASupport));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-DgpuDsmFunBSupport    = %X\n", MainTablePtr->DisplayFeature.Config.DgpuDsmFunBSupport));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-HybridGraphicsSupport = %X\n", MainTablePtr->DisplayFeature.Config.HybridGraphicsSupport));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-DgpuHotPlugMode       = %X\n", MainTablePtr->DisplayFeature.Config.DgpuHotPlugMode));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-DgpuNonHotPlugMode    = %X\n", MainTablePtr->DisplayFeature.Config.DgpuNonHotPlugMode));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-DgpuD3ColdHpdSupport  = %X\n", MainTablePtr->DisplayFeature.Config.DgpuD3ColdHpdSupport));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-PmeTurnOffSupport     = %X\n", MainTablePtr->DisplayFeature.Config.PmeTurnOffSupport));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-D3ColdGen1Support     = %X\n", MainTablePtr->DisplayFeature.Config.D3ColdGen1Support));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-PulseGeneratorSupport = %X\n", MainTablePtr->DisplayFeature.Config.PulseGeneratorSupport));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-DisableDgpuAudioInPX  = %X\n", MainTablePtr->DisplayFeature.Config.DisableDgpuAudioInPX));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-DisableDgpuUsbPort    = %X\n", MainTablePtr->DisplayFeature.Config.DisableDgpuUsbPort));


  if (AmdPbsConfiguration.DgpuMaco == 0) {
    DisplayFeatureTablePtr->MacoGpio        = 0;
    DisplayFeatureTablePtr->TimeMacoUpPwrDn = 0;
    DisplayFeatureTablePtr->TimePwrDnRstDn  = 0;
    DisplayFeatureTablePtr->TimePwrUpRstUp  = 0;
    DisplayFeatureTablePtr->TimeRstUpMacoDn = 0;
  } else {
    DisplayFeatureTablePtr->TimeMacoUpPwrDn = (AmdPbsConfiguration.TimeMacoUpPwrDn <= 9) ? 1 : (AmdPbsConfiguration.TimeMacoUpPwrDn - 8);
    DisplayFeatureTablePtr->TimePwrDnRstDn  = AmdPbsConfiguration.TimePwrDnRstDn;
    DisplayFeatureTablePtr->TimePwrUpRstUp  = AmdPbsConfiguration.TimePwrUpRstUp;
    DisplayFeatureTablePtr->TimeRstUpMacoDn = AmdPbsConfiguration.TimeRstUpMacoDn;
  }

  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-PBS.DgpuMaco    = %d\n",    AmdPbsConfiguration.DgpuMaco));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-MacoGpio        = 0x%x\n",  DisplayFeatureTablePtr->MacoGpio));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-MxmPwrEnGpio    = 0x%x\n",  DisplayFeatureTablePtr->MxmPwrEnGpio));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-TimeMacoUpPwrDn = %d us\n", DisplayFeatureTablePtr->TimeMacoUpPwrDn));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-TimePwrDnRstDn  = %d ms\n", DisplayFeatureTablePtr->TimePwrDnRstDn));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-TimePwrUpRstUp  = %d ms\n", DisplayFeatureTablePtr->TimePwrUpRstUp));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-TimeRstUpMacoDn = %d ms\n", DisplayFeatureTablePtr->TimeRstUpMacoDn));


  if (DisplayFeatureTablePtr) {
    DisplayFeatureTablePtr->DgpuSsidSvid = AmdPbsConfiguration.DgpuSsidSvid;
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-DgpuSsidSvid        = %x\n",  DisplayFeatureTablePtr->DgpuSsidSvid));
    if (DisplayFeatureTablePtr->DgpuSsidSvid) {
      if ((PcdGet32 (PcdCpmDgpuSsid) == 0) && (AmdPbsConfiguration.AmdPcdCpmDgpuSsid != 0)) {
        PcdSet32S (PcdCpmDgpuSsid, AmdPbsConfiguration.AmdPcdCpmDgpuSsid);
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-PcdCpmDgpuSsid      = 0x%08X\n",  PcdGet32 (PcdCpmDgpuSsid)));
      }
      if ((PcdGet32 (PcdCpmDgpuAudioSsid) == 0) && (AmdPbsConfiguration.AmdPcdCpmDgpuAudioSsid != 0)) {
        PcdSet32S (PcdCpmDgpuAudioSsid, AmdPbsConfiguration.AmdPcdCpmDgpuAudioSsid);
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-PcdCpmDgpuAudioSsid = 0x%08X\n",  PcdGet32 (PcdCpmDgpuAudioSsid)));
      }
    }

    DisplayFeatureTablePtr->TimePeRstToWakeL23 = AmdPbsConfiguration.TimePeRstToWakeL23;
    DisplayFeatureTablePtr->TimeDlActToCfgAcc = AmdPbsConfiguration.TimeDlActToCfgAcc;
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-TimePeRstToWakeL23  = %d ms\n", DisplayFeatureTablePtr->TimePeRstToWakeL23));
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-TimeDlActToCfgAcc   = %d ms\n", DisplayFeatureTablePtr->TimeDlActToCfgAcc));

    // For ATPX Function 1 Bit[20:18] Display Panel Multiplexer
    // Enable or disable Smart Mux ACPI device with or without _DEP
    DisplayFeatureTablePtr->MuxFlag = (AmdPbsConfiguration.SmartMuxAcpiControl << 4) + AmdPbsConfiguration.DisplayPanelMultiplexer;
    // if (AmdPbsConfiguration.EcSmartMuxMode <= 2) {
    //   // CRB EC GPIO pins are at Read Mode at Smart Mux 1.0, skip those CRB EC GPIO pins
    //   SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_VGAMUXSEL, CPM_DEVICE_SKIP);
    //   SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_VGAMUXVCC, CPM_DEVICE_SKIP);
    //   DisplayFeatureTablePtr->DisplayMuxDeviceId = 0;
    // } else if (AmdPbsConfiguration.EcSmartMuxMode == 3) { //Bit[3] - 0 = SAG1.5, 1 = SAG2.0
    //   DisplayFeatureTablePtr->MuxFlag &= (UINT8)(~BIT3);
    // } else if (AmdPbsConfiguration.EcSmartMuxMode == 4) {
    //   DisplayFeatureTablePtr->MuxFlag |= BIT3;
    // }
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-MuxFlag             = 0x%X\n",   DisplayFeatureTablePtr->MuxFlag));
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-DisplayMuxDeviceId  = 0x%X\n", DisplayFeatureTablePtr->DisplayMuxDeviceId));

    if ((AmdPbsConfiguration.AtifNotifyCommandCode >= 0xD0) && (AmdPbsConfiguration.AtifNotifyCommandCode <= 0xD9)) {
      DisplayFeatureTablePtr->AtifNotifyCommandCode = AmdPbsConfiguration.AtifNotifyCommandCode;
    }
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-AtifNotifyCommandCode = 0x%X\n", DisplayFeatureTablePtr->AtifNotifyCommandCode));

    if ((DevicePathTablePtr) && (AmdPbsConfiguration.AtifFun21Support)) {
      DisplayFeatureTablePtr->AtifFun21Support = AmdPbsConfiguration.AtifFun21Support;
      DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-AtifFun21Support    = %d\n",   DisplayFeatureTablePtr->AtifFun21Support));
      for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
        if (DevicePathTablePtr->Path[Index].FeatureMask.Raw == 0xFFFFFFFF || DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
          break;
        }
        if (DevicePathTablePtr->Path[Index].IsDgpu) {
          DevicePathTablePtr->Path[Index].FeatureMask.Mask.ExternalGfxPort = AmdPbsConfiguration.AtifFun21ExternalGfxPort;
          DevicePathTablePtr->Path[Index].FeatureMask.Mask.HideXConnectGui = AmdPbsConfiguration.AtifFun21HideXConnectGui;
          DevicePathTablePtr->Path[Index].FeatureMask.Mask.RunTimePMandD3  = AmdPbsConfiguration.AtifFun21RunTimePMandD3;
          DevicePathTablePtr->Path[Index].FeatureMask.Mask.SupportAtifAtpx = AmdPbsConfiguration.AtifFun21SupportAtifAtpx;
          DEBUG((DEBUG_INFO, "    DevicePathTable[%d]-ExternalGfxPort = %d\n", Index, DevicePathTablePtr->Path[Index].FeatureMask.Mask.ExternalGfxPort));
          DEBUG((DEBUG_INFO, "    DevicePathTable[%d]-HideXConnectGui = %d\n", Index, DevicePathTablePtr->Path[Index].FeatureMask.Mask.HideXConnectGui));
          DEBUG((DEBUG_INFO, "    DevicePathTable[%d]-RunTimePMandD3  = %d\n", Index, DevicePathTablePtr->Path[Index].FeatureMask.Mask.RunTimePMandD3));
          DEBUG((DEBUG_INFO, "    DevicePathTable[%d]-SupportAtifAtpx = %d\n", Index, DevicePathTablePtr->Path[Index].FeatureMask.Mask.SupportAtifAtpx));
        }
      }
    } // End of if ((DevicePathTablePtr) && (AmdPbsConfiguration.AtifFun21Support))

    if (AmdPbsConfiguration.AtifFun22Support) {
      DisplayFeatureTablePtr->AtifFun22Support = AmdPbsConfiguration.AtifFun22Support;
      DisplayFeatureTablePtr->AtifFun22Value = AmdPbsConfiguration.AtifFun22Value;
      DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-AtifFun22Support    = %d\n",   DisplayFeatureTablePtr->AtifFun22Support));
      DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-AtifFun22Value      = 0x%08X\n",   DisplayFeatureTablePtr->AtifFun22Value));
    } // End of if (AmdPbsConfiguration.AtifFun22Support)

    if (AmdPbsConfiguration.AtifFunction23Support) {
      DisplayFeatureTablePtr->AtifFunction23Support   = AmdPbsConfiguration.AtifFunction23Support;
      DisplayFeatureTablePtr->AtifFunction23Buffer[3] = AmdPbsConfiguration.VariBrightMaxPerformance;
      DisplayFeatureTablePtr->AtifFunction23Buffer[4] = AmdPbsConfiguration.VariBrightBetterPerformance;
      DisplayFeatureTablePtr->AtifFunction23Buffer[5] = AmdPbsConfiguration.VariBrightBetterBattery;
      DisplayFeatureTablePtr->AtifFunction23Buffer[6] = AmdPbsConfiguration.VariBrightBatterySaver;
      DEBUG ((DEBUG_INFO, "OEM-PEI-%a-AtifFunction23Support = %d\n", __FUNCTION__ , DisplayFeatureTablePtr->AtifFunction23Support));
      DEBUG ((DEBUG_INFO, "OEM-PEI-%a-AtifFunction23Buffer[ ", __FUNCTION__));
      for (Index = 0; Index < 7; Index ++) {
        DEBUG((DEBUG_INFO, "%d ", DisplayFeatureTablePtr->AtifFunction23Buffer[Index]));
      }
      DEBUG ((DEBUG_INFO, "]\n"));
    } // End of if (AmdPbsConfiguration.AtifFunction23Support)
  } // End of if (DisplayFeatureTablePtr)

  if (AmdPbsConfiguration.EvalSlotPowerEnable) {
     DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-if (EvalSlotPower : Enable)\n"));
     if (PowerTablePtr) {
       Index = 0;
       while (PowerTablePtr->DevicePowerList[Index].DeviceId != 0xFF) {
         if (PowerTablePtr->DevicePowerList[Index].DeviceId == DEVICE_ID_MXM && PowerTablePtr->DevicePowerList[Index].Mode == CPM_POWER_OFF) {
           DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-(Item[%X].DeviceId == DEVICE_ID_MXM & CPM_POWER_OFF)\n", Index));
           if ( Index == 3 ) {
             DEBUG((DEBUG_INFO, "PowerTablePtr->DevicePowerList[Index].InitFlag = GPIO_DEVICE_INIT_STAGE_DUMMY\n"));
             PowerTablePtr->DevicePowerList[Index].InitFlag = GPIO_DEVICE_INIT_STAGE_DUMMY;
           }
         }
         Index++;
       }
     }

     // Read EVAL_PRSNT#_EC
     Offset = ECRAMxAE;
     AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);
     Value8 &= EVAL_PRSNT_EC_DIS;
     if (Value8 == 0) {
       // EVAL_19V_EN=1
       Offset = ECRAMxA2;
       AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);
       Value8 |= (EVAL_19V_EN);
       AmdCpmTablePpi->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &Offset, &Value8);
     } else {
       // EVAL_19V_EN=0
       Offset = ECRAMxA2;
       AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);
       Value8 &= ~(EVAL_19V_EN);
       AmdCpmTablePpi->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &Offset, &Value8);
     }

     Offset = ECRAMxA2;
     AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);
     Value8 |= (EVAL_SLOT_PWR);
     AmdCpmTablePpi->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &Offset, &Value8);

     SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_MXM, CPM_DEVICE_AUTO);
   } else {
     DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-if (EvalSlotPower : Disable)\n"));
     if (PowerTablePtr) {
       Index = 0;
       while (PowerTablePtr->DevicePowerList[Index].DeviceId != 0xFF) {
         if (PowerTablePtr->DevicePowerList[Index].DeviceId == DEVICE_ID_MXM && PowerTablePtr->DevicePowerList[Index].Mode == CPM_POWER_OFF) {
           DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-(Item[%X].DeviceId == DEVICE_ID_MXM & CPM_POWER_OFF)\n", Index));
           if ( Index == 3 ) {
             DEBUG((DEBUG_INFO, "PowerTablePtr->DevicePowerList[Index].InitFlag = GPIO_DEVICE_INIT_STAGE_1\n"));
             PowerTablePtr->DevicePowerList[Index].InitFlag = GPIO_DEVICE_INIT_STAGE_1;
           }
         }
         Index++;
       }
     }
     // EVAL_19V_EN=0
     Offset = ECRAMxA2;
     AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);
     Value8 &= ~(EVAL_19V_EN + EVAL_SLOT_PWR);
     AmdCpmTablePpi->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &Offset, &Value8);
     SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_MXM, CPM_DEVICE_OFF);
   }

  if (AmdPbsConfiguration.NVDgpuPowerEnable) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-SetGpio-Pin = 0x%X to 1 for NVIDIA DGPU_SEL# B17 pin\n", BOMACO_EN));
    AmdCpmTablePpi->CommonFunction.SetGpio (AmdCpmTablePpi, BOMACO_EN, 1);         // Output NVIDIA DGPU_SEL# B17 pin to high
    PcdSet64S (PcdCpmIgnoreOemTableId, 0x444C4333444D5043);                        // Ignore override ACPI Oem Table Id = CPMD3CLD
  } else {
    PcdSet64S (PcdCpmIgnoreOemTableId, 0);                                         // Clear ACPI Oem Table Id = CPMD3CLD
  }

  if (AmdPbsConfiguration.NonEvalDgpuSupport) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-if (NonEvalDgpuSupport)\n"));
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Set NonEvalDgpuSupport to 1\n"));
    MainTablePtr->DisplayFeature.Config.NonEvalDgpuSupport = 1;
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_MXM, CPM_DEVICE_ON);
    if (PowerTablePtr) {
      Index = 0;
      while (PowerTablePtr->DevicePowerList[Index].DeviceId != 0xFF) {
        if (PowerTablePtr->DevicePowerList[Index].DeviceId == DEVICE_ID_MXM
          && PowerTablePtr->DevicePowerList[Index].Mode == CPM_POWER_ON
          && PowerTablePtr->DevicePowerList[Index].Type == CPM_POWER_WAIT) {
          DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-(Item[%X].DeviceId == DEVICE_ID_MXM & CPM_POWER_ON & CPM_POWER_WAIT)\n", Index));
          DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Use delay 100ms instead of wait EVAL_PWRGD to high\n"));
          PowerTablePtr->DevicePowerList[Index].Type = CPM_POWER_DELAY;
          PowerTablePtr->DevicePowerList[Index].Config.Stall = 100000;
        }
        Index++;
      }
    }
  }

  if (PowerTablePtr && (AmdPbsConfiguration.TimeSlotPwrToPwrEn != 30)) {
    Index = 0;
    while (PowerTablePtr->DevicePowerList[Index].DeviceId != 0xFF) {
      if ( PowerTablePtr->DevicePowerList[Index].DeviceId == DEVICE_ID_MXM
        && PowerTablePtr->DevicePowerList[Index].Mode == CPM_POWER_ON
        && PowerTablePtr->DevicePowerList[Index].Type == CPM_POWER_DELAY
        && PowerTablePtr->DevicePowerList[Index].Config.Stall == 30000
        && PowerTablePtr->DevicePowerList[Index].InitFlag == GPIO_DEVICE_INIT_STAGE_1) {
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-(Item[%X] DEVICE_ID_MXM CPM_POWER_ON CPM_POWER_DELAY 30000)\n", Index));
        DEBUG((DEBUG_INFO, "PowerTablePtr->DevicePowerList[Index].Config.Stall = %d\n", AmdPbsConfiguration.TimeSlotPwrToPwrEn * 1000));
        PowerTablePtr->DevicePowerList[Index].Config.Stall = AmdPbsConfiguration.TimeSlotPwrToPwrEn * 1000;
        break;
      }
      Index++;
    }
  }

  if (PowerTablePtr && (AmdPbsConfiguration.TimePwrEnToPeRst != 32)) {
    Index = 0;
    while (PowerTablePtr->DevicePowerList[Index].DeviceId != 0xFF) {
      if ( PowerTablePtr->DevicePowerList[Index].DeviceId == DEVICE_ID_MXM
        && PowerTablePtr->DevicePowerList[Index].Mode == CPM_POWER_ON
        && PowerTablePtr->DevicePowerList[Index].Type == CPM_POWER_DELAY
        && PowerTablePtr->DevicePowerList[Index].Config.Stall == 32000
        && PowerTablePtr->DevicePowerList[Index].InitFlag == GPIO_DEVICE_INIT_STAGE_1) {
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-(Item[%X] DEVICE_ID_MXM CPM_POWER_ON CPM_POWER_DELAY 32000)\n", Index));
        DEBUG((DEBUG_INFO, "PowerTablePtr->DevicePowerList[Index].Config.Stall = %d\n", AmdPbsConfiguration.TimePwrEnToPeRst * 1000));
        PowerTablePtr->DevicePowerList[Index].Config.Stall = AmdPbsConfiguration.TimePwrEnToPeRst * 1000;
        break;
      }
      Index++;
    }
  }

  if ((AmdPbsConfiguration.PrimaryVideoAdaptor == 2))
  {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-if (PrimaryVideoAdaptor == 2)\n"));
    MainTablePtr->DisplayFeature.Config.IsDgpuPrimary = 1;
  }
/*
  // ZP-ODD for Lilac only
  if ((PLATFORM_SelLilac == CurPlatformSelect) || (PLATFORM_SelLilacDAP== CurPlatformSelect) || (PLATFORM_SelLilacKD== CurPlatformSelect))
  {
    if (AmdPbsConfiguration.ZeroPowerOddEn) {
      DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-if (ZeroPowerOddEn)\n"));
      //ZeroPowerOddEn, OddHotplugEn & Enable _PRW
      MainTablePtr->ZeroPowerOddEn = BIT0 | BIT1 | BIT2 | BIT3;
    }
  }
*/
  // UCSI Support
  DEBUG ((EFI_D_INFO, "-->  AmdPbsConfiguration.UcsiEn = 0x%x\n", AmdPbsConfiguration.UcsiEn));
  MainTablePtr->UcsiEn            = AmdPbsConfiguration.UcsiEn;
  if (0 != AmdPbsConfiguration.UcsiLocation)
  {
    UcsiTablePtr->EcUcsiLocation = 0xFEEC2000;
  } else {
    UcsiTablePtr->EcUcsiLocation = 0;
  }

  if (AmdPbsConfiguration.NonZ10CapPcieZstateSupport) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride Pci0ZstateSupport\n"));
    PcdSetBoolS(NonZ10CapPcieZstateSupport, TRUE);
  }

#ifdef MODERN_STANDBY_SUPPORT
  // Modern Standby Support
  DEBUG ((EFI_D_INFO, "-->  AmdPbsConfiguration.ModernStandbyEn = 0x%x\n", AmdPbsConfiguration.ModernStandbyEn));
  MainTablePtr->ModernStandbyEn   = AmdPbsConfiguration.ModernStandbyEn;

  if (MainTablePtr->ModernStandbyEn) {
    //enable Global PCIE reset for DXIO training
    PcdSetBoolS(PcdCfgDxioPCIeRSTGenericReset, TRUE);

    S0I3_GPIO_RST_EP1 = NULL;
    Status = (**PeiServices).AllocatePool (
                                 PeiServices,
                                 sizeof(S0I3_GPIO_RST_EP),
                                 &S0I3_GPIO_RST_EP1
                                 );
    if (!EFI_ERROR(Status)) {
      GpioData = AmdCpmTablePpi->CommonFunction.MmioRead32(0xFED81568);
      DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-S0I3_GPIO_RST_EP1-GpioData : %x\n", GpioData));
      S0I3_GPIO_RST_EP1->AssertionSeq = 1;
      S0I3_GPIO_RST_EP1->Address = 0x02D02568;  //GPIO26
      S0I3_GPIO_RST_EP1->DeassertionValue = (GpioData & (~BIT21)) | BIT22 | BIT23;
      S0I3_GPIO_RST_EP1->AssertionValue = (GpioData & (~(BIT22 | BIT23))) | BIT21;
      PcdSet32S(PcdCfgDxioPCIeGPIOResetEP1, (UINT32)(UINTN)S0I3_GPIO_RST_EP1);
    }

    S0I3_GPIO_RST_EP3 = NULL;
    Status = (**PeiServices).AllocatePool (
                                 PeiServices,
                                 sizeof(S0I3_GPIO_RST_EP),
                                 &S0I3_GPIO_RST_EP3
                                 );
    if (!EFI_ERROR(Status)) {
      GpioData = AmdCpmTablePpi->CommonFunction.MmioRead32(0xFED8156C);
      DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-S0I3_GPIO_RST_EP3-GpioData : %x\n", GpioData));
      S0I3_GPIO_RST_EP3->AssertionSeq = 1;
      S0I3_GPIO_RST_EP3->Address = 0x02D0256C;   //GPIO27
      S0I3_GPIO_RST_EP3->DeassertionValue = (GpioData & (~BIT21)) | BIT22 | BIT23;
      S0I3_GPIO_RST_EP3->AssertionValue = (GpioData & (~(BIT22 | BIT23))) | BIT21;
      PcdSet32S(PcdCfgDxioPCIeGPIOResetEP3, (UINT32)(UINTN)S0I3_GPIO_RST_EP3);
    }
/*
    if (AmdPbsConfiguration.NvmeD3cold) {
       OfstU8 = ECRAMxB7;
       AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &OfstU8, &Value8);
       Value8 = Value8 | SSD0_D3_COLD_EN | SSD1_D3_COLD_EN;
       AmdCpmTablePpi->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &OfstU8, &Value8);
       DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-NvmeD3Cold enabled.\n"));

       NoTrainLinkTable = AllocateZeroPool(sizeof(NoTrainLinkDevFunc));
       (**PeiServices).SetMem ((VOID *)NoTrainLinkTable, sizeof(NoTrainLinkDevFunc), 0xFF);

       Offset = 0;
       for(Index = 0; Index < sizeof(NoTrainLinkDevFunc)/sizeof(UINT32); Index++) {
         if (AmdPbsConfiguration.DtSlotPowerControl) {
           if (NoTrainLinkDevFunc[Index] == (UINT32)((0x01 << 3) | 0x02)) {
             // Don't send the RP(B00:D01:F02) to DXIO if DtSlotPowerControl is enabled.
             // To avoid lost device on the x4 slot when the system wake from s0i3.
             continue;
           }
         }
         *(NoTrainLinkTable + Offset) = NoTrainLinkDevFunc[Index];
         Offset++;
       }

       // Notify DXIO skip NVME link training when the system wake from s0i3
       PcdSet32S(PcdCfgDxioNoTrainLinkDevFunc, (UINT32)(UINTN)NoTrainLinkTable);
       DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-PcdCfgDxioNoTrainLinkDevFunc : %x\n", PcdGet32(PcdCfgDxioNoTrainLinkDevFunc)));

       // Merge GpioSmuConfigS0i3Table to SMU GPIO config table
       if (GpioSmuConfigTable) {
         DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverrideGpio-Copy GpioSmuConfigS0i3Table\n"));
         CopyMem (((GpioConfig_t *)GpioSmuConfigTable) + GpioSmuConfigCount, &GpioSmuConfigS0i3Table, (UINTN)sizeof(GpioSmuConfigS0i3Table));
       }

    } else {
       OfstU8 = ECRAMxB7;
       AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &OfstU8, &Value8);
       Value8 = Value8 & (~(SSD0_D3_COLD_EN | SSD1_D3_COLD_EN));
       AmdCpmTablePpi->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &OfstU8, &Value8);
       DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-NvmeD3Cold disabled.\n"));
    }*/

    //Set S0i3 white list table
    PcdSet32S (PcdS0i3UnSafeShutdownApprovedListVidDid, (UINT32)(UINTN)ApprovedListDeviceList);

  /*} else {
    // Play for safety, clear SSD0_D3_COLD_EN and SSD1_D3_COLD_EN in S3 case.
    DEBUG((DEBUG_INFO, "OEM-PEI-%a-Clear ECRAM SSD0_D3_COLD_EN & SSD1_D3_COLD_EN.\n", __FUNCTION__));
    OfstU8 = ECRAMxB7;
    AmdCpmTablePpi->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &OfstU8, &Value8);
    Value8 = Value8 & (~(SSD0_D3_COLD_EN | SSD1_D3_COLD_EN));
    AmdCpmTablePpi->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &OfstU8, &Value8);
*/
  }
#endif

  //Set SMU GPIO config table
  PcdSet32S(PcdAmdSmuGpioConfigTable, (UINT32)GpioSmuConfigTable);

  // Sensor Fusion User Mode Driver
  DEBUG ((EFI_D_INFO, "-->  AmdPbsConfiguration.SensorFusionEn = 0x%x\n", AmdPbsConfiguration.SensorFusionEn));
  MainTablePtr->SensorFusionEn    = AmdPbsConfiguration.SensorFusionEn;

  // Clock control
  DEBUG ((EFI_D_INFO, "-->  AmdPbsConfiguration.UnusedGppClkOff = 0x%x\n", AmdPbsConfiguration.UnusedGppClkOff));
  MainTablePtr->UnusedGppClkOffEn = AmdPbsConfiguration.UnusedGppClkOff;

  // ACP power gating.
  DEBUG ((EFI_D_INFO, "-->  AmdPbsConfiguration.AcpPowerGating = 0x%x\n", AmdPbsConfiguration.AcpPowerGating));
  MainTablePtr->AcpPowerGatingEn  = AmdPbsConfiguration.AcpPowerGating;

  // ACP Clock gating.
  DEBUG ((EFI_D_INFO, "-->  AmdPbsConfiguration.AcpClockGating = 0x%x\n", AmdPbsConfiguration.AcpClockGating));
  MainTablePtr->AcpClockGatingEn  = AmdPbsConfiguration.AcpClockGating;

  // Device control
  DxioPort = &(DxioTopologyTablePtr->Port[0]);

  //
  // HW Detect PIN
  //

  // Check for EVAL 19 HW detected
  /*
  if (TRUE == AllowEcAccess)
  {
    EcTblSet (ECRAMxA2, 0, EVAL_19V_EN);
    if (EcTblRead(ECRAMxAE) & EVAL_PRSNT_EC_DIS)
    {
      EcTblSet (ECRAMxA2, EVAL_19V_EN, 0);
    }
  }

  //
  // EC I2C PMIC/VR Config
  //
  do
  {
    UINT8 VddioMemS3Map[21] = {
        0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C, 0x8E, 0x90, 0x92, 0x94
    };
    UINT8 VddMisc_S0[21] = {
        0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x85, 0x8A, 0x8F, 0x94, 0x99, 0x9E, 0xA3, 0xA8, 0xAD, 0xB2
    };
    UINT8 VddMisc_S5[21] = {
        0x00, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x85, 0x8A, 0x8F, 0x94, 0x99, 0x9E, 0xA3, 0xA8, 0xAD, 0xB2
    };
    UINT8 Vdd11Map[21] = {
        0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C, 0x8E, 0x90, 0x92, 0x94
    };
    UINT8 VddMemqMap[5] = {
        0x00, 0x19, 0x32, 0x99, 0xB2
    };
    UINT8 Alw18vMap[21] = {
        0x00, 10*1, 10*2, 10*3, 10*4, 10*5, 10*6, 10*7, 10*8, 10*9, 10*10, 10*11+28, 10*12+28, 10*13+28, 10*14+28, 10*15+28, 10*16+28, 10*17+28, 10*18+28, 10*19+28, 10*20+28
    };
    if (FALSE == AllowEcAccess)
      break;

    if (FALSE == AmdPbsConfiguration.VddIoAdjustEn)
    {
      DEBUG ((DEBUG_INFO," Skip for VddIo tunnel\n"));
      break;
    }
    // Progame the VDDIO_MEM_S3, Slave Addr 0x30, Offset 0xF8
    EcI2cDevByteAccess (AmdCpmTablePpi, FALSE, 0x30, 0xF8, &VddioMemS3Map[AmdPbsConfiguration.MemVddio]);
    // Program the VDD_MISC, Slave Addr 0x10, 0ffset 0xFB
    EcI2cDevByteAccess (AmdCpmTablePpi, FALSE, 0x10, 0xFB, &VddMisc_S0[AmdPbsConfiguration.VddpRun]);
    // Program the VDD_MISC_S5, Slave Addr 0x10, 0ffset 0xFA
    EcI2cDevByteAccess (AmdCpmTablePpi, FALSE, 0x10, 0xFA, &VddMisc_S5[AmdPbsConfiguration.VddMiscS5]);
    // Progame the VDD11, Slave Addr 0x70, Offset 0xF8
    EcI2cDevByteAccess (AmdCpmTablePpi, FALSE, 0x70, 0xF8, &Vdd11Map[AmdPbsConfiguration.VddIo11]);
    // Progame the VDD11, Slave Addr 0x70, Offset 0xFA
    EcI2cDevByteAccess (AmdCpmTablePpi, FALSE, 0x70, 0xFA, &VddMemqMap[AmdPbsConfiguration.MemVddq]);
    // Progame the Alw18v, Slave Addr 0x20, Offset 0xF9
    EcI2cDevByteAccess (AmdCpmTablePpi, FALSE, 0x20, 0xF9, &Alw18vMap[AmdPbsConfiguration.Alw18v]);
  } while (FALSE);

*/
  //
  //  PBS HW Manual Selection
  //

  // For Mayan
/*  if ((PLATFORM_SelMayan == CurPlatformSelect) || (PLATFORM_SelMayanDAP== CurPlatformSelect) || (PLATFORM_SelMayanKD== CurPlatformSelect))
  {
    if ((AmdPbsConfiguration.Ssd1PwrEn == 1) && (AmdPbsConfiguration.DtSlotPowerControl == 1))
    {
      DEBUG ((DEBUG_INFO, "!Error! [%d]: M.2 SSD1 and Pcie X4 can't be enabled at the same time\n", __LINE__));
    }

    //
    // HW Rework mapping for Mayan.
    //
    if ( (1 == HwReworkMap.Desc.iSata_Gpp10) || (1 == HwReworkMap.Desc.iSata_Gpp11))
    {
      AmdPbsConfiguration.Ssd1PwrEn = 0;
      AmdPbsConfiguration.DtM2ssd1Mux = 0;
      AmdPbsConfiguration.Ssd1HddMux = 1;
    }

    if (AmdPbsConfiguration.Ssd1PwrEn == 1) {                 // For M.2 SSD1  x4 case
      AmdPbsConfiguration.DtM2ssd1Mux = 1;
      AmdPbsConfiguration.Ssd1HddMux = 0;
    }
    if (AmdPbsConfiguration.DtSlotPowerControl == 1) {        // For Pcie x4 case
      AmdPbsConfiguration.DtM2ssd1Mux = 0;
      AmdPbsConfiguration.Ssd1HddMux = 1;
    }
  }

  // For Lilac
  if ((PLATFORM_SelLilac == CurPlatformSelect) || (PLATFORM_SelLilacDAP== CurPlatformSelect) || (PLATFORM_SelLilacKD== CurPlatformSelect))
  {
    if ((AmdPbsConfiguration.Ssd1PwrEn == 1) && (AmdPbsConfiguration.OddPwrEn == 1)) {
      DEBUG ((DEBUG_INFO, "!Error! [%d]: M.2 SSD1 and ODD can't be enabled at the same time\n", __LINE__));
    }
    if ((AmdPbsConfiguration.Ssd1PwrEn == 1) && (AmdPbsConfiguration.DtSlotPowerControl == 1)) {
      DEBUG ((DEBUG_INFO, "!Error! [%d]: M.2 SSD1 and Pcie X4 can't be enabled at the same time\n", __LINE__));
    }

    if (AmdPbsConfiguration.OddPwrEn == 1) {                  // This is for Lilac odd
      AmdPbsConfiguration.OddSsd1Mux = 0;
    } else {
      AmdPbsConfiguration.OddSsd1Mux = 1;
    }
    if (AmdPbsConfiguration.HDDPwrEn == 1) {                  // This is for Lilac hdd
      AmdPbsConfiguration.Ssd1HddMux = 1;
    } else {
      AmdPbsConfiguration.Ssd1HddMux = 0;
    }
    if (AmdPbsConfiguration.DtSlotPowerControl == 1) {        // For Pcie x2 case
      AmdPbsConfiguration.DtM2ssd1Mux = 0;
    } else {
      AmdPbsConfiguration.DtM2ssd1Mux = 1;
    }
    // We don't need enable any mux for Ssd1PwrEn due to above "else" already set it.
    // if (AmdPbsConfiguration.Ssd1PwrEn == 1) {                 // For M.2 SSD1 x2 case
    // }
  }


  *//*
   * Currently, GPP13 only used for GPE lan. for some WWAN card problem
   * //-  AmdPbsConfiguration.WwanLomMux = (AmdPbsConfiguration.LanPowerControl == 1)? 1 : 0;
   * *//*
  AmdPbsConfiguration.WwanLomMux = 1;
  AmdPbsConfiguration.WwanX1Mux = (AmdPbsConfiguration.WwanPowerEn == 1)? 0 : 1;

  if ((PLATFORM_SelMayan == CurPlatformSelect) || (PLATFORM_SelMayanDAP== CurPlatformSelect) || (PLATFORM_SelMayanKD== CurPlatformSelect)) {  // This is for Mayan
    AmdPbsConfiguration.OddSsd1Mux = 1;
  } else if (AmdPbsConfiguration.OddPwrEn == 1) { // This is for Lilac odd
    AmdPbsConfiguration.OddSsd1Mux = 0;
  } else if (AmdPbsConfiguration.HDDPwrEn == 1) { // This is for Lilac hdd
    AmdPbsConfiguration.Ssd1HddMux = 1;
  }

  //
  //  Setting Device Enable PIN and Power
  //
  do
  {
    if (FALSE == CPM_SETUP_OVERRIDE)
      break;

    TmpBufPtr = (UINT8*)PcdGetPtr (PcdRmbDevPwrTblStageRestore);
    // For control by override setup. disable the gCpmGpioDevicePowerTable
    DEBUG ((DEBUG_INFO, "Override DevicePowerList GPIO_DEVICE_INIT_STAGE_1 to Dummy\n", Index, PowerTablePtr->DevicePowerList[Index].InitFlag));
    for (Index= 0; 0xFF != PowerTablePtr->DevicePowerList[Index].DeviceId; ++Index)
    {
      // Don't skip DEVICE_ID_VGAMUXVCC and DEVICE_ID_VGAMUXSEL
      if ((PowerTablePtr->DevicePowerList[Index].DeviceId != DEVICE_ID_VGAMUXVCC) && \
          (PowerTablePtr->DevicePowerList[Index].DeviceId != DEVICE_ID_VGAMUXSEL)) {
        TmpBufPtr[Index] = 0x5F;
        if (GPIO_DEVICE_INIT_STAGE_1 == PowerTablePtr->DevicePowerList[Index].InitFlag)
        {
          TmpBufPtr[Index] = GPIO_DEVICE_INIT_STAGE_1;
//          DEBUG ((DEBUG_INFO, "  Orig DevicePowerList[%d].InitFlag[%x]\n", (UINTN)Index, (UINTN)PowerTablePtr->DevicePowerList[Index].InitFlag));
          PowerTablePtr->DevicePowerList[Index].InitFlag = GPIO_DEVICE_INIT_STAGE_DUMMY;
        }
      } // End of if ((PowerTablePtr->DevicePowerList[Index].DeviceId != DEVICE_ID_VGAMUXVCC) && (PowerTablePtr->DevicePowerList[Index].DeviceId != DEVICE_ID_VGAMUXSEL))
    }
    Index = AMD_GPIO_DEVICE_POWER_SIZE;
    PcdSetPtrS (PcdRmbDevPwrTblStageRestore, &Index, TmpBufPtr);

    // PCIE x8 GFX
    EcTblSet (ECRAMxA2, EVAL_SLOT_PWR, 0);    // Main Power Control Pin
    EcTblSet (ECRAMxA1, EVAL_PWREN, 0);
    if (AmdPbsConfiguration.EvalSlotPowerEnable)
    {
      EcTblSet (ECRAMxA2, 0, EVAL_SLOT_PWR);
      EcTblSet (ECRAMxA1, 0, EVAL_PWREN);

    }

    // TBT3 ATL MODE
    (AmdPbsConfiguration.PdTBT3AltMode) ? EcTblSet (ECRAMxFE, 0, TBT3_ALT_EN_DIS) : EcTblSet (ECRAMxFE, TBT3_ALT_EN_DIS, 0);

    // PCIE x1
    (AmdPbsConfiguration.Pciex1PowerEn) ? EcTblSet (ECRAMxA1, 0, X1_SLOT_PWREN) : EcTblSet (ECRAMxA1, X1_SLOT_PWREN, 0);

    // PCIE x4
    (AmdPbsConfiguration.DtSlotPowerControl) ? EcTblSet (ECRAMxA7, 0, DT_PWREN) : EcTblSet (ECRAMxA7, DT_PWREN, 0);

    // WLan
    (AmdPbsConfiguration.WlanPowerControl) ? EcTblSet (ECRAMxAA, 0, WLAN_PWR_EN) : EcTblSet (ECRAMxAA, WLAN_PWR_EN, 0);

    // Lan
    (AmdPbsConfiguration.LanPowerControl) ? EcTblSet (ECRAMxAA, 0, LOM_PWREN) : EcTblSet (ECRAMxAA, LOM_PWREN, 0);

    // Wwan
    (AmdPbsConfiguration.WwanPowerEn) ? EcTblSet (ECRAMxAA, 0, WWAN_PWR_EN) : EcTblSet (ECRAMxAA, WWAN_PWR_EN, 0);

    switch (AmdPbsConfiguration.PbsWWANDeviceSupport) {
    case 0:
        (AmdPbsConfiguration.WwanPowerEn) ? EcTblSet (ECRAMxAB, 0, WWAN_CARD_POWER_EN) : EcTblSet (ECRAMxAB, WWAN_CARD_POWER_EN, 0);
      break;
    case 1: // Fibocom L860 R+
      // Control WWAN sequence in GpioResetControlLib.
      break;
    default:
      break;
    }

    // SSD1
    (AmdPbsConfiguration.Ssd1PwrEn) ? EcTblSet (ECRAMxA9, 0, M2SSD1_PWREN) : EcTblSet (ECRAMxA9, M2SSD1_PWREN, 0);

    // SSD0
    (AmdPbsConfiguration.Ssd0PwrEn) ? EcTblSet (ECRAMxAA, 0, M2SSD0_PWREN) : EcTblSet (ECRAMxAA, M2SSD0_PWREN, 0);

    // ODD
    (AmdPbsConfiguration.OddPwrEn) ? EcTblSet (ECRAMxAA, 0, ODD_PWR_EN) : EcTblSet (ECRAMxAA, ODD_PWR_EN, 0);

    // HDD
    (AmdPbsConfiguration.HDDPwrEn) ? EcTblSet (ECRAMxAA, 0, HDD_PWR_EN) : EcTblSet (ECRAMxAA, HDD_PWR_EN, 0);

    // UsbCameraPowerEn
    (AmdPbsConfiguration.UsbCameraPowerEn) ? EcTblSet (ECRAMxAA, 0, USB_CAM_PWRON) : EcTblSet (ECRAMxAA, USB_CAM_PWRON, 0);

    // SD Card PowerEn
    (AmdPbsConfiguration.SdCardPwrEn) ? EcTblSet (ECRAMxAA, 0, SD_PWR_BIT_EN) : EcTblSet (ECRAMxAA, SD_PWR_BIT_EN, 0);

    // Wlan Power for S3/S4
    (AmdPbsConfiguration.KeepWlanPowerInS3S4) ? EcTblSet (ECRAMxB6, 0, (WLAN_PWR_S3 | WLAN_PWR_S4)) : EcTblSet (ECRAMxB6, (WLAN_PWR_S3 | WLAN_PWR_S4), 0);

    // USBC port disable
    (AmdPbsConfiguration.UsbPortHwDisable) ? EcTblSet (ECRAMxFE, 0, USBC_PORT_DIS) : EcTblSet (ECRAMxFE, USBC_PORT_DIS, 0);

    // EC Post LED Turn On/Off
    (AmdPbsConfiguration.EcPostLedTurnOn) ? EcTblSet (ECRAMxCD, 0, EC_POSTLED_ON) : EcTblSet (ECRAMxCD, EC_POSTLED_ON, 0);

    if (AmdPbsConfiguration.EcSmartMuxMode > 2) {
      // Smart Mux 1.5 and Smart Mux 2.0
      EcTblSet (ECRAMxCD, EC_SMART_MUX_MASK, EC_SMART_MUX_MASK);
    } else {
      // EC SmartMux Disable/HyGraph/dGPU
      EcTblSet (ECRAMxCD, EC_SMART_MUX_MASK, (AmdPbsConfiguration.EcSmartMuxMode << 2) & EC_SMART_MUX_MASK);
    }



    // PowerSensorRoutingSelect
    if (1 == AmdPbsConfiguration.PowerSensorsRoutingSelect)
    {
#if defined(INTERNAL_IDS)
      *//*
       * PLAT-81585, Walle Lite Intermittently Working on RMB.
       * It is a hardware workaround. need to have a toggle, make device work normal
       * *//*
      // Reset to Zero
      Value8 = EcTblRead(ECRAMxAC);
      Value8 &= (~PWRGRP_WALLE_MP2_SEL);
      EcImmWrite(ECRAMxAC, Value8);
      AmdCpmTablePpi->CommonFunction.Stall (AmdCpmTablePpi, 10000);   // Delay 10ms
#endif
      EcTblSet (ECRAMxAC, 0, PWRGRP_WALLE_MP2_SEL);
      // PLAT-75832 +>
      if (0 == AmdPbsConfiguration.Mp2FwSelection)
      {
        PcdSet8S (PcdPowerSensorsRoutingSelect, 0); // MP2_SFH
        PcdSetBoolS(PcdCfgNbifDev0Epf7En, TRUE);
      }
      else if (1 == AmdPbsConfiguration.Mp2FwSelection)
      {
        PcdSet8S (PcdPowerSensorsRoutingSelect, 1); // MP2_Walle
        PcdSetBoolS(PcdCfgNbifDev0Epf7En, FALSE);
        // Use for Smu PCD.
        PcdSet8S (PcdIsDAP, 0);
        if ( PLATFORM_SelMayanDAP == CurPlatformSelect || PLATFORM_SelLilacDAP == CurPlatformSelect)
        {
          PcdSet8S (PcdIsDAP, 1);
        }
      }
      // <+ // PLAT-75832
    }
    else
    {
      EcTblSet (ECRAMxAC, PWRGRP_WALLE_MP2_SEL, 0);
      PcdSet8S (PcdPowerSensorsRoutingSelect, 0);
    }

    // EvalCardTdiodeRoutingSelect
    (AmdPbsConfiguration.EvalCardTdiodeRoutingSelect) ? EcTblSet (ECRAMxAC, EVAL_APU_SMBUS_EC_SEL, 0) : EcTblSet (ECRAMxAC, 0, EVAL_APU_SMBUS_EC_SEL);

    // Finger Printer Power Enable
    (AmdPbsConfiguration.UsbFprPwrEn) ? EcTblSet (ECRAMxA9, 0, FPR_PWR_EN) : EcTblSet (ECRAMxA9, FPR_PWR_EN, 0);

  } while (FALSE);      // End of setting device Power PIN.


  //
  // Featrue function selection
  //

  // Touch Pad
  if (AmdPbsConfiguration.TouchPad < 5)
  {
    Value8 = AmdPbsConfiguration.TouchPad;
    // Need to switch the SO/S1 Bit. (Bit5/Bit4)
    if (1 == Value8)
      Value8 = 2;
    else if (2 == Value8)
      Value8 = 1;
    EcTblSet (ECRAMxA9, 0, TPAD_EN);
    // MUX 1 on ECRAMxA8, bit[4|5]
    EcTblSet (ECRAMxA8, MUX1_S1 | MUX1_S0, Value8 << 4);
  }
  else
    EcTblSet (ECRAMxA9, TPAD_EN, 0);

  // Touch Panel
  if ((AmdPbsConfiguration.TouchPanel&0x0F) < 5)
  {
    Value8 = AmdPbsConfiguration.TouchPanel & 0x0F;
    // Need to switch the SO/S1 Bit. (Bit5/Bit4)
    if (1 == Value8)
      Value8 = 2;
    else if (2 == Value8)
      Value8 = 1;
    else if (4 == Value8)
    {
      // For USB TouchPanel, just need to enable the PowerPin.
      // The default MUX select, select to I2C0.
      Value8 = 0;
    }
    EcTblSet (ECRAMxA9, 0, TPNL_EN | TPNL_PWR_EN);
    EcTblSet (ECRAMxA8, MUX0_S1 | MUX0_S0, Value8 << 2);
  }
  else
  {
    EcTblSet (ECRAMxA9, TPNL_EN | TPNL_PWR_EN, 0);
  }

  // TODO: For NFC, it might need to be porting something ....
  // For example, SMBUS1 or IC2 choice ... etc
  // NFC
  if (AmdPbsConfiguration.NfcSensor < 5)
  {
    Value8 = AmdPbsConfiguration.NfcSensor;
    // Need to switch the SO/S1 Bit. (Bit5/Bit4)
    if (1 == Value8)
      Value8 = 2;
    else if (2 == Value8)
      Value8 = 1;
    EcTblSet (ECRAMxA8, MUX2_S1 | MUX2_S0, Value8 << 6);
  }
  else
  {
    // NFC Disable
  }

  //
  // Setting the GPP Mux function selection
  //
  do
  {
    // Initialize the Mux Selection.

    // WwanX1 Mux
    (AmdPbsConfiguration.WwanX1Mux ? EcTblSet (ECRAMxAC, 0, WWAN_PCIEx1_SW) : EcTblSet (ECRAMxAC, WWAN_PCIEx1_SW, 0) );
    // DtM2ssd1 Mux
    (AmdPbsConfiguration.DtM2ssd1Mux ? EcTblSet (ECRAMxAC, 0, DT_M2SSD1_SW) : EcTblSet (ECRAMxAC, DT_M2SSD1_SW, 0) );
    // Only for LilacTv Platform, MayanTv ignore.
    (AmdPbsConfiguration.OddSsd1Mux ? EcTblSet (ECRAMxAC, 0, ODD_M2SSD1_SW) : EcTblSet (ECRAMxAC, ODD_M2SSD1_SW, 0) );
    // MayanTv: (Ssd1 and DT Mux switch.), Lilac: (Ssd1 and Hdd Mux switch)
    (AmdPbsConfiguration.Ssd1HddMux ? EcTblSet (ECRAMxAC, 0, M2SSD1_HDD_SW) : EcTblSet (ECRAMxAC, M2SSD1_HDD_SW, 0) );

    // -->
    *//*
     * Currently, GPP13 only used for GPE lan. for some WWAN card problem
     * *//*
    (AmdPbsConfiguration.WwanLomMux ? EcTblSet (ECRAMxAC, 0, WWAN_LOM_SW) : EcTblSet (ECRAMxAC, WWAN_LOM_SW, 0) );
//-    (AmdPbsConfiguration.WwanLomMux ? EcTblSet (ECRAMxAC, 0, WWAN_LOM_SW) : EcTblSet (ECRAMxAC, 0, WWAN_LOM_SW) );
    // <--

    //
    (AmdPbsConfiguration.ChgByPassEn ? EcTblSet (ECRAMxB6, 0, CHARGER_MODE_SW) : EcTblSet (ECRAMxB6, CHARGER_MODE_SW, 0) );
*/

  do
  {
    // Create the Dynamic DXIO PCIE PORT configuration.
    Index = 0;

    if ((PcdGetBool(PcdSataEnable) == TRUE) && (AmdPbsConfiguration.PcieSlot1 == 0)){
      CopyMem (&DxioTopologyTablePtr->Port[Index++], &DxioSataPort, sizeof(DxioSataPort));
    }
    CopyMem (&DxioTopologyTablePtr->Port[Index], &DxioFixDevicePort[0], sizeof(DxioFixDevicePort));

    if (AmdPbsConfiguration.dGpuHotPlugEnable == 1) {
      DxioTopologyTablePtr->Port[Index].Port.LinkHotplug = DxioHotplugEnhanced;
      DxioTopologyTablePtr->Port[Index].Port.ClkReq      = CLK_DISABLE;
    }
    DEBUG ((EFI_D_INFO, "AmdPbsConfiguration.PromSupportEnable %x \n",AmdPbsConfiguration.PromSupportEnable));  
    if(AmdPbsConfiguration.PromSupportEnable == 1){
      //Edit DxioTopologyTablePtr to support promontory
      CopyMem (&DxioTopologyTablePtr->Port[Index+1], &PromSupportedData, sizeof(PromSupportedData));
    }

    if ((AmdPbsConfiguration.GppHotPlugEnable == 1) && (AmdPbsConfiguration.PromSupportEnable == 0)) {
      DxioTopologyTablePtr->Port[Index+1].Port.LinkHotplug = DxioHotplugEnhanced;
      DxioTopologyTablePtr->Port[Index+1].Port.ClkReq      = CLK_DISABLE;
    }

    if ((AmdPbsConfiguration.PCIE2X8 == 1) && (AmdPbsConfiguration.PromSupportEnable == 0)) {
      DEBUG ((EFI_D_INFO, "Combine 2 X4 lanes to 1 X8 lanes[12:19] \n"));
      DxioTopologyTablePtr->Port[Index+1].EngineData.StartLane = 12;
      DxioTopologyTablePtr->Port[Index+1].EngineData.EndLane = 19;
    }

/*    if ((PcdGet8 (PcdRmbBoardSelect) == 0 && PcdGet8 (PcdRmbSku) == 2) ||
        (PcdGet8 (PcdRmbBoardSelect) == 1 && PcdGet8 (PcdRmbSku) == 3) ||
        (PcdGet8 (PcdRmbBoardSelect) == 6 && PcdGet8 (PcdRmbSku) == 4) ||
        (PcdGet8 (PcdRmbBoardSelect) == 5 && PcdGet8 (PcdRmbSku) == 4)) {
        DxioPort = &(DxioTopologyTablePtr->Port[0]);
        DxioPort[0].EngineData.EngineType           = DxioPcieEngine;
        DxioPort[0].Port.PortPresent                = DxioPortEnabled;
        DxioPort[0].EngineData.StartLane            = 0;
        DxioPort[0].EngineData.EndLane              = 3;
        DxioPort[0].Port.LinkAspmL1_1               = DxioAspmL11Disabled;
        DxioPort[0].Port.LinkAspmL1_2               = DxioAspmL12Disabled;
        DxioPort[0].Port.MiscControls.ClkPmSupport  = DxioClkPmSupportDisabled;
        DxioPort[0].Port.ClkReq                     = DxioClkPmSupportDisabled;

        DxioPort[1].EngineData.EngineType           = DxioPcieEngine;
        DxioPort[1].Port.PortPresent                = DxioPortEnabled;
        DxioPort[1].EngineData.StartLane            = 4;
        DxioPort[1].EngineData.EndLane              = 7;
        DxioPort[1].Port.LinkAspmL1_1               = DxioAspmL11Disabled;
        DxioPort[1].Port.LinkAspmL1_2               = DxioAspmL12Disabled;
        DxioPort[1].Port.MiscControls.ClkPmSupport  = DxioClkPmSupportDisabled;
        DxioPort[1].Port.ClkReq                     = DxioClkPmSupportDisabled;
        EcTblSet (ECRAMxB7, 0, BIT6); // Enable  pure-DC case
        EcTblSet (ECRAMxB7, 0, BIT3); // Enable  pure-AC case
    }
*/
    Index += sizeof(DxioFixDevicePort) / sizeof (DxioFixDevicePort[0]);

    /*
     * PLAT-85463 [RMB] A915 hang due to stuck ClkReq/ClkAck handshake between PCIE controllers and IOHUB
     * [Design Hit]:
     *   When "DxioPort.Port.PortPresent == DxioPortDisabled",
     *   It still need to config "DxioPort.Port.LinkHotplug := DxioHotplugDisabled"
     *   The reason is Port.PortPresent and Port.LinkHotplug is work on DXIO different function Part.
     * */

    //
    // Selection the 0:Dt or 1:SSD1
    //
/*
    if (0 == AmdPbsConfiguration.DtM2ssd1Mux)
    {
      if ((PLATFORM_SelLilac == CurPlatformSelect) || (PLATFORM_SelLilacDAP== CurPlatformSelect) || (PLATFORM_SelLilacKD== CurPlatformSelect))
      {
        //
        // DTx2 Selection.
        //
        CopyMem (&TmpDxioPort, &DxioDtSsd1Port, sizeof(TmpDxioPort));
        TmpDxioPort.EngineData.StartLane = 8;
        TmpDxioPort.EngineData.EndLane = 9;
        TmpDxioPort.Port.LinkHotplug = DxioHotplugBasic;
        TmpDxioPort.EngineData.GpioGroupId = DEVICE_ID_DT;
        CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
        ++Index;
        if (0 == AmdPbsConfiguration.OddSsd1Mux || 1 == AmdPbsConfiguration.Ssd1HddMux)
        {
          if (PcdGetBool(PcdSataEnable) == TRUE)
          {
            CopyMem (&TmpDxioPort, &DxioSataOddPort, sizeof(TmpDxioPort));
            CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
            ++Index;
          }
        }
      }
      else if ((PLATFORM_SelMayan == CurPlatformSelect) || (PLATFORM_SelMayanDAP== CurPlatformSelect) || (PLATFORM_SelMayanKD== CurPlatformSelect))
      {
        if ( (1 == HwReworkMap.Desc.iSata_Gpp10) || (1 == HwReworkMap.Desc.iSata_Gpp11))
        {
          DEBUG ((DEBUG_INFO, "[%d]: Detected HwReworkMap GPP10 or GPP11"));
          //
          // DTx2 Selection.
          //
          CopyMem (&TmpDxioPort, &DxioDtSsd1Port, sizeof(TmpDxioPort));
          TmpDxioPort.EngineData.StartLane = 8;
          TmpDxioPort.EngineData.EndLane = 9;
          TmpDxioPort.EngineData.GpioGroupId = DEVICE_ID_DT;
          CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
          ++Index;
          // SATA Engine configuration
          if (PcdGetBool(PcdSataEnable) == TRUE)
          {
            CopyMem (&TmpDxioPort, &DxioSataOddPort, sizeof(TmpDxioPort));
            CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
            ++Index;
          }
        }
        else
        {
          //
          // DTx4 Selection.
          //
          CopyMem (&TmpDxioPort, &DxioDtSsd1Port, sizeof(TmpDxioPort));
          TmpDxioPort.EngineData.StartLane = 8;
          TmpDxioPort.EngineData.EndLane = 11;
          TmpDxioPort.Port.LinkHotplug = DxioHotplugBasic;
          TmpDxioPort.EngineData.GpioGroupId = DEVICE_ID_DT;
          CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
          ++Index;
        }
      }
      else
      {
        // Un-recognize
      }
    }
    else
    {
      if ((PLATFORM_SelLilac == CurPlatformSelect) || (PLATFORM_SelLilacDAP== CurPlatformSelect) || (PLATFORM_SelLilacKD== CurPlatformSelect))
      {
        //
        // SSD1 Selection
        //
        if (0 == AmdPbsConfiguration.OddSsd1Mux || 1 == AmdPbsConfiguration.Ssd1HddMux)
        {
          if (PcdGetBool(PcdSataEnable) == TRUE)
          {
            // SATA Engine configuration
            CopyMem (&TmpDxioPort, &DxioSataOddPort, sizeof(TmpDxioPort));
            CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
            ++Index;
          }
        }
        else
        {
          if (EcTblRead(ECRAMxAE) & M2_SSD1_PEDET)
          {
            // SSD1 will be lane 4.
            CopyMem (&TmpDxioPort, &DxioDtSsd1Port, sizeof(TmpDxioPort));
            TmpDxioPort.EngineData.StartLane = 8;
            TmpDxioPort.EngineData.EndLane = 11;
            TmpDxioPort.EngineData.GpioGroupId = DEVICE_ID_SSD1;
            CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
            ++Index;
          }
          else
          {
            if (PcdGetBool(PcdSataEnable) == TRUE)
            {
              // Config for SATA Porting
              CopyMem (&TmpDxioPort, &DxioSataOddPort, sizeof(TmpDxioPort));
              CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
              ++Index;
            }
            // Disable other two Lan
            CopyMem (&TmpDxioPort, &DxioDtSsd1Port, sizeof(TmpDxioPort));
            TmpDxioPort.EngineData.StartLane = 8;
            TmpDxioPort.EngineData.EndLane = 9;
            TmpDxioPort.EngineData.GpioGroupId = DEVICE_ID_SSD1;
            TmpDxioPort.Port.PortPresent = DxioPortDisabled;
            TmpDxioPort.Port.LinkHotplug = DxioHotplugDisabled;
            CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
            ++Index;
          }
        }
      }
      else if ((PLATFORM_SelMayan == CurPlatformSelect) || (PLATFORM_SelMayanDAP== CurPlatformSelect) || (PLATFORM_SelMayanKD== CurPlatformSelect))
      {
        if (EcTblRead(ECRAMxAE) & M2_SSD1_PEDET)
        {
          // SSD1 will be lane 4.
          CopyMem (&TmpDxioPort, &DxioDtSsd1Port, sizeof(TmpDxioPort));
          TmpDxioPort.EngineData.StartLane = 8;
          TmpDxioPort.EngineData.EndLane = 11;
          TmpDxioPort.EngineData.GpioGroupId = DEVICE_ID_SSD1;
          CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
          ++Index;
        }
        else
        {
          if (PcdGetBool(PcdSataEnable) == TRUE)
          {
            // Config for SATA Porting
            CopyMem (&TmpDxioPort, &DxioSataOddPort, sizeof(TmpDxioPort));
            CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
            ++Index;
          }
          // Disable other two Lan
          CopyMem (&TmpDxioPort, &DxioDtSsd1Port, sizeof(TmpDxioPort));
          TmpDxioPort.EngineData.StartLane = 8;
          TmpDxioPort.EngineData.EndLane = 9;
          TmpDxioPort.EngineData.GpioGroupId = DEVICE_ID_SSD1;
          TmpDxioPort.Port.PortPresent = DxioPortDisabled;
          TmpDxioPort.Port.LinkHotplug = DxioHotplugDisabled;
          CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
          ++Index;
        }
      }
      else
      {
        // Un-recognize
      }
    }

    //
    // Selection 0:Wwan or 1:Pcie_x1
    //
    if ( (0 == AmdPbsConfiguration.WwanPowerEn) && (0 == AmdPbsConfiguration.Pciex1PowerEn) )
    {
      // Disable the PciePhy
      CopyMem (&TmpDxioPort, &DxioWwanPciex1Port, sizeof(TmpDxioPort));
      TmpDxioPort.Port.PortPresent = DxioPortDisabled;
      TmpDxioPort.Port.LinkHotplug = DxioHotplugDisabled;
      CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
      ++Index;
    }
    else if (0 == AmdPbsConfiguration.WwanX1Mux)
    {
      //  WWAN
//      if (0 == AmdPbsConfiguration.WwanLomMux)  // Disable the GBE, the GPP can be
//      {
//-        // WWAN have on Lane GPP12
//        CopyMem (&TmpDxioPort, &DxioWwanPciex1Port, sizeof(TmpDxioPort));
//        TmpDxioPort.EngineData.StartLane = 12;
//        TmpDxioPort.EngineData.EndLane = 13;
//        TmpDxioPort.EngineData.GpioGroupId = DEVICE_ID_WWAN;
//        CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
//        ++Index;
//      }
//      else
//      {
        // WWAN have on Lane GPP12
        CopyMem (&TmpDxioPort, &DxioWwanPciex1Port, sizeof(TmpDxioPort));
        TmpDxioPort.EngineData.GpioGroupId = DEVICE_ID_WWAN;
        CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
        ++Index;
//      }
    }
    else
    {
      //  Pcie_x1
      CopyMem (&TmpDxioPort, &DxioWwanPciex1Port, sizeof(TmpDxioPort));
      TmpDxioPort.EngineData.GpioGroupId = DEVICE_ID_DTx1;
      TmpDxioPort.Port.LinkHotplug = DxioHotplugBasic;
      CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
      ++Index;
    }

    //
    // Lan GPP Control
    //
    *//*
     * Currently, GPP13 only used for GPE lan. for some WWAN card problem
     *
     * *//*
//    if (0 == AmdPbsConfiguration.WwanLomMux)
    if (0 == AmdPbsConfiguration.LanPowerControl)
    {
//      if (0 == AmdPbsConfiguration.WwanPowerEn)
//      {
//-        // GBE did not be locate the correct lane, Also WWAN did not occupy the GPP13
        CopyMem (&TmpDxioPort, &DxioGbePort, sizeof(TmpDxioPort));
        TmpDxioPort.Port.PortPresent = DxioPortDisabled;
        TmpDxioPort.Port.LinkHotplug = DxioHotplugDisabled;
        CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
        ++Index;
//      }
//      else
//      {
//-        // The WWAN Enable, CPP13 will be occupy by WWAN.
//      }
    }
    else
    {
      CopyMem (&TmpDxioPort, &DxioGbePort, sizeof(TmpDxioPort));
      CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
      ++Index;
    }

    //
    // SD Card Device Enable/Disable
    //
    if (AmdPbsConfiguration.SdCardPwrEn)
    {
      CopyMem (&TmpDxioPort, &DxioSdCardPort, sizeof(TmpDxioPort));
      CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
      ++Index;
    }
    else
    {
      // SD Card did not be locate the correct lane, it still need to be disable.
      CopyMem (&TmpDxioPort, &DxioSdCardPort, sizeof(TmpDxioPort));
      TmpDxioPort.Port.PortPresent = DxioPortDisabled;
      TmpDxioPort.Port.LinkHotplug = DxioHotplugDisabled;
      CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
      ++Index;
    }*/
  if (1 == AmdPbsConfiguration.PcieSlot1) {
    //
    // PCIe x4 Slot-1
    //
    CopyMem (&TmpDxioPort, &DxioPciePort, sizeof(TmpDxioPort));
    TmpDxioPort.EngineData.StartLane = 8;
    TmpDxioPort.EngineData.EndLane = 11;
    TmpDxioPort.EngineData.GpioGroupId = DEVICE_ID_DT;
    TmpDxioPort.Port.DeviceNumber = 1;
    TmpDxioPort.Port.FunctionNumber = 2;
    TmpDxioPort.Port.ClkReq = CLK_REQ1;
    CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
    ++Index;
  } else {
    if (!PcdGet8(PcdXgbeDisable) && (AmdPbsConfiguration.XgbePathSelect)){
      if(PcdGetBool (PcdXgbePort0ConfigEn)){
        CopyMem (&DxioTopologyTablePtr->Port[Index], &DxioXGBEDevicePort[0], sizeof(DxioXGBEDevicePort[0]) );
        ++Index;
      }
      if(PcdGetBool (PcdXgbePort1ConfigEn)){
        CopyMem (&DxioTopologyTablePtr->Port[Index], &DxioXGBEDevicePort[1], sizeof(DxioXGBEDevicePort[1]) );
        ++Index;
      }
    }


  }

  if (AmdPbsConfiguration.PCIE2X8 == 0) {
    switch (AmdPbsConfiguration.M2PcieSlotSelect) {
      case 0: // M.2 NVMe (x4)
        CopyMem (&DxioTopologyTablePtr->Port[Index], &DxioPciePort, sizeof(TmpDxioPort));
        ++Index;
        break;

      case 1: // M.2 NVMe (x2)
        CopyMem (&TmpDxioPort, &DxioPciePort, sizeof(TmpDxioPort));
        TmpDxioPort.EngineData.StartLane = 16;
        TmpDxioPort.EngineData.EndLane = 17;
        CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
        ++Index;

        TmpDxioPort.EngineData.StartLane = 18;
        TmpDxioPort.EngineData.EndLane = 19;
        TmpDxioPort.Port.PortPresent = DxioPortDisabled;
        CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
        ++Index;
        break;

      case 2: // PCIe x4 Slot-3
        CopyMem (&TmpDxioPort, &DxioPciePort, sizeof(TmpDxioPort));
        TmpDxioPort.EngineData.GpioGroupId = DEVICE_ID_DT;
        TmpDxioPort.Port.FunctionNumber = 3;
        TmpDxioPort.Port.ClkReq = CLK_REQ3;
        CopyMem (&DxioTopologyTablePtr->Port[Index], &TmpDxioPort, sizeof(TmpDxioPort));
        ++Index;
        break;
    }
  }

    //
    // End of Dxio config. Setting the lastest Dxio device flag been Terminal
    //
    DxioTopologyTablePtr->Port[Index-1].Flags = DESCRIPTOR_TERMINATE_LIST;
  if (0 == AmdPbsConfiguration.PcieSlot1){
    ConfigureEthernetDxioPorts  (PeiServices, &DxioPort);
    UpdateXgbeFchPcds(PeiServices, &DxioPort);
  }
  } while (FALSE);
  // End of Setting GPP Mux device function.
  // TODO: Lilac/Mayan Review later
  /*
  if ((PcdGet8 (PcdNvmeRaid)) == 1)
  {
    DEBUG((DEBUG_INFO, "NVME RAID enabled , disable L11/L12 for NVME and PCIE slot \n"));
    DxioPort[1].Port.LinkAspmL1_1          = DxioAspmL11Disabled;
    DxioPort[1].Port.LinkAspmL1_2          = DxioAspmL12Disabled;
    DxioPort[2].Port.LinkAspmL1_1          = DxioAspmL11Disabled;
    DxioPort[2].Port.LinkAspmL1_2          = DxioAspmL12Disabled;
    DxioPort[6].Port.LinkAspmL1_1          = DxioAspmL11Disabled;
    DxioPort[6].Port.LinkAspmL1_2          = DxioAspmL12Disabled;
  }
  */

  //
  // GPP[x] Clock
  //
/*
  do
  {
    if (FALSE == CPM_SETUP_OVERRIDE)
      break;
*/
    /*
     * NOTE - CLKREQ# = Disabled will also mean that CPM and L1SS can no longer work, so those 2 features should be disabled additionally.
     * */
/*
    Index = 0;
    for (Index = 0; PcieClockTablePtr->Item[Index].ClkId != 0xFF; ++Index)
    {
      //
      //  GPP_CLK0
      //
      if (PcieClockTablePtr->Item[Index].ClkId == GPP_CLK0)
      {
        Value8 = AmdPbsConfiguration.ClockRequest0;
        if (1 == Value8)
        {
//          PcieClockTablePtr->Item[Index].SlotCheck = CLKPM_L1SS_CHECK;
        }
        else if (0 == Value8)
        {
          DxioPort = &(DxioTopologyTablePtr->Port[0]);
          for (; ; ++DxioPort)
          {
            if (DxioPort->Port.ClkReq == CLK_REQ0)
            {
              DxioPort->Port.MiscControls.ClkPmSupport  = DxioClkPmSupportDisabled;
              DEBUG ((DEBUG_INFO, "Disable CLK_REQ0 AspmL1/ClkPmSupport\n"));
            }
            if (DxioPort->Flags == DESCRIPTOR_TERMINATE_LIST)
              break;
          }

          PcieClockTablePtr->Item[Index].SlotCheck      = NON_SLOT_CHECK;
          PcieClockTablePtr->Item[Index].ClkReq         = CLK_ENABLE;
        }
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Item[%X].ClkId=GPP_CLK0, Set ClkReq=CLK_REQ0\n", PcieClockTablePtr->Item[Index].ClkId));
        continue;
      }

      //
      //  GPP_CLK1
      //
      if (PcieClockTablePtr->Item[Index].ClkId == GPP_CLK1)
      {
        Value8 = AmdPbsConfiguration.ClockRequest1;
        if (1 == Value8)
        {
//          PcieClockTablePtr->Item[Index].SlotCheck = CLKPM_L1SS_CHECK;
        }
        else if (0 == Value8)
        {
          DxioPort = &(DxioTopologyTablePtr->Port[0]);
          for (; ; ++DxioPort)
          {
            if (DxioPort->Port.ClkReq == CLK_REQ1)
            {
              DxioPort->Port.MiscControls.ClkPmSupport  = DxioClkPmSupportDisabled;
              DEBUG ((DEBUG_INFO, "Disable CLK_REQ1 AspmL1/ClkPmSupport\n"));
            }
            if (DxioPort->Flags == DESCRIPTOR_TERMINATE_LIST)
              break;
          }

          PcieClockTablePtr->Item[Index].SlotCheck      = NON_SLOT_CHECK;
          PcieClockTablePtr->Item[Index].ClkReq         = CLK_ENABLE;
        }
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Item[%X].ClkId=GPP_CLK1, Set ClkReq=CLK_REQ1\n", PcieClockTablePtr->Item[Index].ClkId));
        continue;
      }

      //
      // GPP_CLK2
      //
      if (PcieClockTablePtr->Item[Index].ClkId == GPP_CLK2)
      {
        Value8 = AmdPbsConfiguration.ClockRequest2;
        if (1 == Value8)
        {}
        else if (0 == Value8)
        {
          DxioPort = &(DxioTopologyTablePtr->Port[0]);
          for (; ; ++DxioPort)
          {
            if (DxioPort->Port.ClkReq == CLK_REQ2)
            {
              DxioPort->Port.MiscControls.ClkPmSupport  = DxioClkPmSupportDisabled;
              DEBUG ((DEBUG_INFO, "Disable CLK_REQ2 AspmL1/ClkPmSupport\n"));
            }
            if (DxioPort->Flags == DESCRIPTOR_TERMINATE_LIST)
              break;
          }

          PcieClockTablePtr->Item[Index].SlotCheck      = NON_SLOT_CHECK;
          PcieClockTablePtr->Item[Index].ClkReq         = CLK_ENABLE;
        }
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Item[%X].ClkId=GPP_CLK2, Set ClkReq=CLK_REQ2\n", PcieClockTablePtr->Item[Index].ClkId));
        continue;
      }

      //
      // GPP_CLK3
      //
      if (PcieClockTablePtr->Item[Index].ClkId == GPP_CLK3)
      {
        Value8 = AmdPbsConfiguration.ClockRequest3;
        if (1 == Value8)
        {}
        else if (0 == Value8)
        {
          DxioPort = &(DxioTopologyTablePtr->Port[0]);
          for (; ; ++DxioPort)
          {
            if (DxioPort->Port.ClkReq == CLK_REQ3)
            {
              DxioPort->Port.MiscControls.ClkPmSupport  = DxioClkPmSupportDisabled;
              DEBUG ((DEBUG_INFO, "Disable CLK_REQ3 AspmL1/ClkPmSupport\n"));
            }
            if (DxioPort->Flags == DESCRIPTOR_TERMINATE_LIST)
              break;
          }

          PcieClockTablePtr->Item[Index].SlotCheck      = NON_SLOT_CHECK;
          PcieClockTablePtr->Item[Index].ClkReq         = CLK_ENABLE;
        }
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Item[%X].ClkId=GPP_CLK3, Set ClkReq=CLK_REQ3\n", PcieClockTablePtr->Item[Index].ClkId));
        continue;
      }
*/
      //
      // GPP_CLK4
      //
/*
      if (PcieClockTablePtr->Item[Index].ClkId == GPP_CLK4)
      {
        Value8 = AmdPbsConfiguration.ClockRequest4;
        if (1 == Value8)
        {}
        else if (0 == Value8)
        {
          DxioPort = &(DxioTopologyTablePtr->Port[0]);
          for (; ; ++DxioPort)
          {
            if (DxioPort->Port.ClkReq == CLK_REQ4)
            {
              DxioPort->Port.MiscControls.ClkPmSupport  = DxioClkPmSupportDisabled;
              DEBUG ((DEBUG_INFO, "Disable CLK_REQ4 AspmL1/ClkPmSupport\n"));
            }
            if (DxioPort->Flags == DESCRIPTOR_TERMINATE_LIST)
              break;
          }

          PcieClockTablePtr->Item[Index].SlotCheck      = NON_SLOT_CHECK;
          PcieClockTablePtr->Item[Index].ClkReq         = CLK_ENABLE;
        }
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Item[%X].ClkId=GPP_CLK4, Set ClkReq=CLK_REQ4\n", PcieClockTablePtr->Item[Index].ClkId));
        continue;
      }

      //
      // GPP_CLK5
      //
      if (PcieClockTablePtr->Item[Index].ClkId == GPP_CLK5)
      {
        Value8 = AmdPbsConfiguration.ClockRequest5;
        if (1 == Value8)
        {}
        else if (0 == Value8)
        {
          DxioPort = &(DxioTopologyTablePtr->Port[0]);
          for (; ; ++DxioPort)
          {
            if (DxioPort->Port.ClkReq == CLK_REQ5)
            {
              DxioPort->Port.MiscControls.ClkPmSupport  = DxioClkPmSupportDisabled;
              DEBUG ((DEBUG_INFO, "Disable CLK_REQ5 AspmL1/ClkPmSupport\n"));
            }
            if (DxioPort->Flags == DESCRIPTOR_TERMINATE_LIST)
              break;
          }

          PcieClockTablePtr->Item[Index].SlotCheck      = NON_SLOT_CHECK;
          PcieClockTablePtr->Item[Index].ClkReq         = CLK_ENABLE;
        }
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Item[%X].ClkId=GPP_CLK5, Set ClkReq=CLK_REQ5\n", PcieClockTablePtr->Item[Index].ClkId));
        continue;
      }

      //
      // GPP_CLK6
      //
      if (PcieClockTablePtr->Item[Index].ClkId == GPP_CLK6)
      {
        Value8 = AmdPbsConfiguration.ClockRequest6;
        if (1 == Value8)
        {}
        else if (0 == Value8)
        {
          DxioPort = &(DxioTopologyTablePtr->Port[0]);
          for (; ; ++DxioPort)
          {
            if (DxioPort->Port.ClkReq == CLK_REQ6)
            {
             DxioPort->Port.MiscControls.ClkPmSupport  = DxioClkPmSupportDisabled;
              DEBUG ((DEBUG_INFO, "Disable CLK_REQ6 AspmL1/ClkPmSupport\n"));
            }
            if (DxioPort->Flags == DESCRIPTOR_TERMINATE_LIST)
              break;
          }

          PcieClockTablePtr->Item[Index].SlotCheck      = NON_SLOT_CHECK;
          PcieClockTablePtr->Item[Index].ClkReq         = CLK_ENABLE;
        }
        DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-Item[%X].ClkId=GPP_CLK6, Set ClkReq=CLK_REQ6\n", PcieClockTablePtr->Item[Index].ClkId));
        continue;
      }
      // End ...
    }
  } while (FALSE);
*/
  // End of Set CPP Clock table override.

  //
  // TODO: something might need consider on SimNow environment
  //
  /*
   *    //Environment: 0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO, 4 - GC Lite
   *    if (! PcdGet32 (PcdAmdEnvironmentFlag) ) {
   * */

  DetectPcieDdiDevices (AmdCpmTablePpi, DxioTopologyTablePtr, &AmdPbsConfiguration, NULL, CurPlatformSelect);
/*
  // PLAT-92428
  if ((CurPlatformSelect == PLATFORM_SelMayanKD) || (CurPlatformSelect == PLATFORM_SelLilacKD)) {
    PcdSet8S(PcdDisplayFixVoltageSwing, 2);
    PcdSet32S(PcdDisplayCapDdi2, PcdGet32(PcdDisplayCapDdi2) | EXT_DISPLAY_PATH_CAPS__DP_FIXED_VS_EN);
//    PcdSet32S(PcdDisplayCapDdi3, PcdGet32(PcdDisplayCapDdi3) | EXT_DISPLAY_PATH_CAPS__DP_FIXED_VS_EN);
  }

  MainTablePtr->WirelessButtonEn = AmdPbsConfiguration.WirelessBotton;
  if (AmdPbsConfiguration.WirelessBotton == 0) {
    AmdCpmTablePpi->CommonFunction.RemoveTable (AmdCpmTablePpi, WirelessButtonTablePtr);
  }
*/
  // PCI Reserved Resource Table - for PciHotPlugInit
  if (PciResResTablePtr) {
    DEBUG ((DEBUG_INFO, "\nOEM-PEI-%a-PCI Reserved Resource Table\n", __FUNCTION__));
    for (Index = 0; Index < AMD_RES_RES_SIZE; Index++) {
      if (PciResResTablePtr->ResRes[Index].DevType == CPM_END_OF_TABLE) {
        break;
      }
      DEBUG ((DEBUG_INFO, "  { 0x%02X, 0x%02X,", PciResResTablePtr->ResRes[Index].DevType, PciResResTablePtr->ResRes[Index].SocketDieBridge));
      DEBUG ((DEBUG_INFO, " {%d, %d},", PciResResTablePtr->ResRes[Index].Gpp.Device, PciResResTablePtr->ResRes[Index].Gpp.Function));
      DEBUG ((DEBUG_INFO, " {%d, %d},", PciResResTablePtr->ResRes[Index].SwUs.Device, PciResResTablePtr->ResRes[Index].SwUs.Function));
      DEBUG ((DEBUG_INFO, " {%d, %d},", PciResResTablePtr->ResRes[Index].SwDs.Device, PciResResTablePtr->ResRes[Index].SwDs.Function));
      DEBUG ((DEBUG_INFO, " 0x%02X, 0x%04X,", PciResResTablePtr->ResRes[Index].ResBus, PciResResTablePtr->ResRes[Index].ResIo));
      DEBUG ((DEBUG_INFO, " 0x%016LX, 0x%016LX,", PciResResTablePtr->ResRes[Index].ResNonPrefetchableMmio, PciResResTablePtr->ResRes[Index].AlignResNonPrefetch));
      DEBUG ((DEBUG_INFO, " 0x%016LX, 0x%016LX }\n", PciResResTablePtr->ResRes[Index].ResPrefetchableMmio, PciResResTablePtr->ResRes[Index].AlignResPrefetch));
    }
    DEBUG ((DEBUG_INFO, "\n"));
  } // End of if (PciResResTablePtr)

 /* if (PreInitTablePtr)
  {
    switch (AmdPbsConfiguration.PbsWWANDeviceSupport)
    {
      case 0:
        break;
      case 1: // Fibocom L860 R+
        Index = 0;
        while (PreInitTablePtr->Item[Index].Type != 0xFF)
        {
          if ( PreInitTablePtr->Item[Index].Select == 0x15 && PreInitTablePtr->Item[Index].Offset == 0x45)
          {
            // WAKE# will be pulled down as a pulse signal by WWAN device when S4/shutdown, so disable the wake function as workaround.
            PreInitTablePtr->Item[Index].OrMask &= ((UINT8)~(BIT7));
            break;
          }
          Index++;
        }
        break;
      default:
        break;
    }
  }
*/
//  if (TRUE == AllowEcAccess )
//  {
    TableAddress = &AzaliaCodecVerbTable[0];
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-PcdCfgAzaliaCodecVerbTable-TableAddress = %x\n", TableAddress));
    PcdSet32S (PcdCfgAzaliaCodecVerbTable, (UINT32)TableAddress);
//  }

  if ((AmdPbsConfiguration.ProcessorAggregatorDevice == 0) || (AmdPbsConfiguration.CoreCountControl == 0)) {
    PcdSet8S (PcdCoreCountControlEnable, 0x0);
    PcdSet8S (PcdSwSciGpeID, 0xFF);
  } else {
    PcdSet8S (PcdCoreCountControlEnable, AmdPbsConfiguration.CoreCountControl);
    PcdSet8S (PcdSwSciGpeID, AmdPbsConfiguration.CoreCountSwSciGpeId);
  }
  MainTablePtr->ProcessorAggregatorDevice = (UINT16) AmdPbsConfiguration.ProcessorAggregatorDevice << 7;
  MainTablePtr->ProcessorAggregatorDevice |= (UINT16) PcdGet8 (PcdCoreCountControlEnable) << 6;
  MainTablePtr->ProcessorAggregatorDevice |= (UINT16) PcdGet8 (PcdSwSciGpeID) & 0x1F;
  MainTablePtr->ProcessorAggregatorDevice |= (UINT16) 0xFF << 8;
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-PcdCoreCountControlEnable               = 0x%X\n", PcdGet8 (PcdCoreCountControlEnable)));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-PcdSwSciGpeID                           = 0x%X\n", PcdGet8 (PcdSwSciGpeID)));
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-MainTablePtr->ProcessorAggregatorDevice = 0x%04X\n", MainTablePtr->ProcessorAggregatorDevice));

  if (AmdPbsConfiguration.DbgPrintInAsl != 0) {
    PcdSetBoolS (PcdCpmDebugPrintInAsl, TRUE);
    DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-PcdCpmDebugPrintInAsl      = TRUE\n"));
  }
/*
  // I2S TDM2_DIN Pull High Config
  SmnRegisterRead(0, 0x01241448, &DeviceEnableMap);
  DeviceEnableMap &= 0xFFFFF7FF;
  SmnRegisterWrite(0, 0x01241448, &DeviceEnableMap, AccessWidth32);
  if (AmdPbsConfiguration.Tdm2_din_PullUp)
  {
    SmnRegisterRead(0, 0x01241444, &DeviceEnableMap);
    DeviceEnableMap |= 0x800;
    SmnRegisterWrite(0, 0x01241444, &DeviceEnableMap, AccessWidth32);
  }

  PcdSet8S(PcdSoundWireSsdtSupport, AmdPbsConfiguration.SoundWireSsdtSupport);

  UpdatePerCoreDldoPsmMarginCtrl();

  if (TRUE == AllowEcAccess)
  {
    //
    // Porting or Initialize the EC RAM.
    //
    UINT8     TempData;
    DEBUG ((DEBUG_INFO, "[%d]: Program EC Config Value\n", __LINE__));
    for (Index=0; Index < MAX_EC_INDEX; ++Index)
    {
      if ((Index % 3) == 0) {
        EcBurstEnable(AmdCpmTablePpi);
      }

      OfstU8 = PorgEcConfig[Index].IdxEcRam;
      Value8 = EcTblRead (OfstU8);
      TempData = Value8;
      DEBUG ((DEBUG_INFO, "  Read   ECRAM0x%2x[%x]\n", OfstU8, Value8));
      Value8 &= PorgEcConfig[Index].AndEcRamData;
      Value8 |= PorgEcConfig[Index].OrEcRamData;
      if (TempData != Value8) {
        DEBUG ((DEBUG_INFO, "  Set    ECRAM0x%2x[%x]\n", OfstU8, Value8));
        EcImmWrite (OfstU8, Value8);
      }
    }

    DEBUG_CODE_BEGIN ();
    //
    // Read EC Validation
    //
    {
      UINT8     TempEcBuf[0x100];
      for (Index=0x00; Index < 0x100; ++Index)
      {
        Value8 = EcTblRead ((UINT8)Index);
        TempEcBuf [Index] = Value8;
      }

      DEBUG ((DEBUG_INFO, "ECRAM Info dump: \n"));
      DEBUG ((DEBUG_INFO, "    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"));
      DEBUG ((DEBUG_INFO, "---------------------------------------------------"));
      for (Index=0x00; Index < 0x100; ++Index)
      {
        if (0 == Index%0x10)
        {
          DEBUG ((DEBUG_INFO, "\n"));
          DEBUG ((DEBUG_INFO, "%02x|",(Index/0x10) ));
        }

        DEBUG ((DEBUG_INFO, " %02x", TempEcBuf [Index]));
      }
      DEBUG ((DEBUG_INFO, "\n"));
    }

    //
    // Dump HwReworkMap
    //
    DEBUG ((DEBUG_INFO, "HwReworkMap Info dump: \n"));
    DEBUG ((DEBUG_INFO, "    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"));
    DEBUG ((DEBUG_INFO, "---------------------------------------------------"));
    for (Index=0x00; Index < sizeof(HwReworkMap); ++Index)
    {
      if (0 == Index%0x10)
      {
        DEBUG ((DEBUG_INFO, "\n"));
        DEBUG ((DEBUG_INFO, "%02x|",(Index/0x10) ));
      }

      DEBUG ((DEBUG_INFO, " %02x", HwReworkMap.Raw[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));

    //
    DEBUG_CODE_END ();
  }
*/
  DEBUG((DEBUG_INFO, "OEM-PEI-AmdCpmTableOverride-End-3-Status=%r\n", Status));
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Function to check Back plane mode selction
 *
 * @param[in]     PortMode  Plat config selected by User
 * @retval       TRUE         BackPlane Mode selected
 * @retval        FALSE        Back Plane mode not selected
 *
 */
BOOLEAN IsPlatconfBackplane(
  UINTN PortMode
)
{
  if((PortMode == XGBE_10G_1G_BACKPLANE)||(PortMode == XGBE_2_5G_BACKPLANE) ||(PortMode == XGBE_SGMII_BACKPLANE))
    return TRUE;
  else
    return FALSE;
}
/*----------------------------------------------------------------------------------------*/
/**
 * Function to Dump Ethenet configurations
 *
 * @param[in]     DxioPortList  Pointer to DXIO_PORT_DESCRIPTOR
 *
 */

VOID DumpEthernetEngine
(
  DXIO_PORT_DESCRIPTOR* DxioPortList
)
{
  DEBUG((DEBUG_INFO, "XGBE Port : %d configuration \n", DxioPortList->EtherNet.EthPortProp0.PortNum));
  DEBUG((DEBUG_INFO, "PlatConf     = 0x%x\n", DxioPortList->EtherNet.EthPortProp0.PlatConf));
  DEBUG((DEBUG_INFO, "ConnType     = 0x%x\n", DxioPortList->EtherNet.EthPortProp0.ConnType));
  DEBUG((DEBUG_INFO, "SuppSpeed    = 0x%x\n", DxioPortList->EtherNet.EthPortProp0.SuppSpeed));
  DEBUG((DEBUG_INFO, "PadMux0      = 0x%x\n", DxioPortList->EtherNet.PadMux0));
  DEBUG((DEBUG_INFO, "PadMux1      = 0x%x\n", DxioPortList->EtherNet.PadMux1));
  DEBUG((DEBUG_INFO, "Rs           = 0x%x\n", DxioPortList->EtherNet.EthPortProp3.Rs));
  DEBUG((DEBUG_INFO, "SfpMux       = 0x%x\n", DxioPortList->EtherNet.EthPortProp4.SfpMux));
  DEBUG((DEBUG_INFO, "TxEqPre      = 0x%x\n", DxioPortList->EtherNet.EthPortTxEq.TxEqPre));
  DEBUG((DEBUG_INFO, "TxEqMain     = 0x%x\n", DxioPortList->EtherNet.EthPortTxEq.TxEqMain));
  DEBUG((DEBUG_INFO, "TxEqPost     = 0x%x\n", DxioPortList->EtherNet.EthPortTxEq.TxEqPost));
  DEBUG((DEBUG_INFO, "SfpGpioMask  = 0x%x\n", DxioPortList->EtherNet.EthPortProp3.SfpGpioMask));
  DEBUG((DEBUG_INFO, "SfpGpioAdd   = 0x%x\n", DxioPortList->EtherNet.EthPortProp3.SfpGpioAdd));
  DEBUG((DEBUG_INFO, "SfpMuxUpAdd  = 0x%x\n", DxioPortList->EtherNet.EthPortProp4.SfpMuxUpAdd));
  DEBUG((DEBUG_INFO, "TxFault      = 0x%x\n", DxioPortList->EtherNet.EthPortProp3.TxFault));
  DEBUG((DEBUG_INFO, "ModAbs       = 0x%x\n", DxioPortList->EtherNet.EthPortProp3.ModAbs));
  DEBUG((DEBUG_INFO, "RxLoss       = 0x%x\n", DxioPortList->EtherNet.EthPortProp3.RxLoss));
  DEBUG((DEBUG_INFO, "SfpBusSeg    = 0x%x\n", DxioPortList->EtherNet.EthPortProp4.SfpBusSeg));
}

/*----------------------------------------------------------------------------------------*/
/**
 * Function to Configure Ethernet Dxio ports
 *
 * @param[in]    DxioPort   Pointer to DXIO_PORT_DESCRIPTOR
 * @retval       EFI_STATUS

 */
EFI_STATUS
EFIAPI
ConfigureEthernetDxioPorts (
   IN EFI_PEI_SERVICES** PeiServices,
  IN DXIO_PORT_DESCRIPTOR** DxioPort
  )
{
  DXIO_PORT_DESCRIPTOR* DxioPortList = (*DxioPort);
  AMD_PBS_SETUP_OPTION              AmdPbsConfiguration;
  AMD_CPM_TABLE_PPI                 *AmdCpmTablePpi;
  AMD_CPM_PRE_INIT_TABLE            *PreInitTablePtr;
  AMD_CPM_PRE_SETTING_ITEM          *TablePtr;
  EFI_STATUS                        Status;
  UINT32 PortNum =0, PadmuxValue =0;  
  DEBUG((DEBUG_INFO, "In ConfigureEthernetDxioPorts\n"));

  if (DxioPortList == NULL) {
    DEBUG((DEBUG_INFO, "ConfigureEthernetDxioPorts, Invalid Port List\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-ConfigureEthernetDxioPorts-Status=%r\n", Status));
    return Status;
  }

  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gAmdCpmTablePpiGuid,
                           0,
                           NULL,
                           (VOID**)&AmdCpmTablePpi
                           );

  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "  [%d]: OEM-PEI-ConfigureEthernetDxioPorts-Status=%r\n", __LINE__, Status));
    return Status;
  }
  PreInitTablePtr = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_PRE_INIT);

  if (AmdPbsConfiguration.XgbePssOutputPort0 == 1) {
    if (PreInitTablePtr) {
      TablePtr = &PreInitTablePtr->Item[0];
      while (TablePtr->Type != 0xFF) {
        if (TablePtr->Offset == 0x9 || TablePtr->Offset == 0xB) {
          TablePtr->OrMask |= (UINT8)2;
        }
        TablePtr++;
      }
    }

    PadmuxValue = AmdCpmTablePpi->CommonFunction.SmnRead32(AmdCpmTablePpi, 0, 0, 0, 0x1631F000, 0);
    PadmuxValue |= 0x200000;
    AmdCpmTablePpi->CommonFunction.SmnWrite32(AmdCpmTablePpi, 0, 0, 0, 0x1631F000, 0,PadmuxValue);
    PadmuxValue = AmdCpmTablePpi->CommonFunction.SmnRead32(AmdCpmTablePpi, 0, 0, 0, 0x1631F004, 0);
    PadmuxValue |= 0x4000040;
    AmdCpmTablePpi->CommonFunction.SmnWrite32(AmdCpmTablePpi, 0, 0, 0, 0x1631F004, 0,PadmuxValue);
  }

  // Agpio32ResetL and XgbePssOutputPort1 uses AGPIO32, Only one can activly use AGPIO32
  if ((AmdPbsConfiguration.Agpio32ResetL == 1) && (AmdPbsConfiguration.XgbePssOutputPort1 == 0)) {
    if (PreInitTablePtr) {
      TablePtr = &PreInitTablePtr->Item[0];
      while (TablePtr->Type != 0xFF) {
        if (TablePtr->Offset == 0x20) {
          TablePtr->OrMask |= (UINT8)1;
        }
        TablePtr++;
      }
    }
  }

  if ((AmdPbsConfiguration.XgbePssOutputPort1 == 1) && (AmdPbsConfiguration.Agpio32ResetL == 0)){
    if (PreInitTablePtr) {
      TablePtr = &PreInitTablePtr->Item[0];
      while (TablePtr->Type != 0xFF) {
        if (TablePtr->Offset == 0x17 || TablePtr->Offset == 0x20) {
          TablePtr->OrMask |= (UINT8)2;
        }
        TablePtr++;
      }
    }

    PadmuxValue = AmdCpmTablePpi->CommonFunction.SmnRead32(AmdCpmTablePpi, 0, 0, 0, 0x1631F000, 0);
    PadmuxValue |= 0x800000;
    AmdCpmTablePpi->CommonFunction.SmnWrite32(AmdCpmTablePpi, 0, 0, 0, 0x1631F000, 0,PadmuxValue);
    PadmuxValue = AmdCpmTablePpi->CommonFunction.SmnRead32(AmdCpmTablePpi, 0, 0, 0, 0x1631F004, 0);
    PadmuxValue |= 0xE0008000;
    AmdCpmTablePpi->CommonFunction.SmnWrite32(AmdCpmTablePpi, 0, 0, 0, 0x1631F004, 0,PadmuxValue);
  }

 while(((DxioPortList->Flags & DESCRIPTOR_TERMINATE_LIST) != DESCRIPTOR_TERMINATE_LIST)||
   ((DxioPortList->Flags & DESCRIPTOR_TERMINATE_LIST) == DESCRIPTOR_TERMINATE_LIST) && (DxioPortList->EngineData.EngineType == DxioEthernetEngine)){

    if(DxioPortList->EngineData.EngineType == DxioEthernetEngine){
      PortNum = DxioPortList->EtherNet.EthPortProp0.PortNum;

      if(AmdPbsConfiguration.XgbePathSelect == XGBE_ON_BOARD_POARTS){ // CPM table override for On board ports
        DxioPortList->EtherNetExt.EthPortRxEq.RxEqApplicable = 0;
        DxioPortList->EtherNetExt.EthPortRxEq.RxDfeByp0 = 0;

        if(((PortNum ==ETH_PORT_0) && (AmdPbsConfiguration.Port0TypeSwitch == XGBE_PHY_MODE_RJ45))
           ||((PortNum ==ETH_PORT_1)&&(AmdPbsConfiguration.Port1TypeSwitch == XGBE_PHY_MODE_RJ45))){  // RJ45 Configuration
          DxioPortList->EtherNet.EthPortProp0.MdioId   = 0;
          DxioPortList->EtherNet.EthPortProp0.PlatConf  = (PortNum == ETH_PORT_0)?AmdPbsConfiguration.Port0ModeSel:(AmdPbsConfiguration.Port1ModeSel);
          DxioPortList->EtherNet.EthPortProp0.MdioId   = 0;
          DxioPortList->EtherNet.EthPortProp0.SuppSpeed = (PortNum == ETH_PORT_0)?AmdPbsConfiguration.Port0SpeedSel:AmdPbsConfiguration.Port1SpeedSel;
          DxioPortList->EtherNet.EthPortProp0.ConnType  = XGBE_CONNECTION_MDIO_PHY;
          DxioPortList->EtherNet.PadMux0 = (PortNum == ETH_PORT_0)? 0x10 : 0x20;
          DxioPortList->EtherNet.PadMux1 = 0;
          DxioPortList->EtherNet.EthPortProp3.SfpGpioMask = 0;
          DxioPortList->EtherNet.EthPortProp3.SfpGpioAdd  = 1;
          DxioPortList->EtherNet.EthPortProp3.Rs          = 0;
          DxioPortList->EtherNet.EthPortProp3.TxFault   = 0xA;
          DxioPortList->EtherNet.EthPortProp3.ModAbs    = 0;
          DxioPortList->EtherNet.EthPortProp3.RxLoss    = 0;
          DxioPortList->EtherNet.EthPortProp4.SfpMux      = 0;
          DxioPortList->EtherNet.EthPortProp4.SfpMuxUpAdd = 0;
          DxioPortList->EtherNet.EthPortProp4.RedriverAddress = 0;
          DxioPortList->EtherNet.EthPortProp4.RedriverInterface = 0;
          DxioPortList->EtherNet.EthPortProp4.RedriverLane = 0;
          DxioPortList->EtherNet.EthPortProp4.RedriverModel = 0;
          DxioPortList->EtherNet.EthPortProp4.RedriverPresent = 0;
          DxioPortList->EtherNet.EthPortProp4.SfpBusSeg = 0;
          DxioPortList->EtherNet.EthPortTxEq.TxEqPre      = 0;
          DxioPortList->EtherNet.EthPortTxEq.TxEqMain     = 34;
          DxioPortList->EtherNet.EthPortTxEq.TxEqPost     = 24;
          UpdateXgbeTxPolarity(PeiServices,PortNum, FALSE);

          if(PortNum == ETH_PORT_0){
            PcdSetBoolS(PcdXgbeMdio0, TRUE);
            PcdSetBoolS(PcdXgbeSfp0,   FALSE);
          }else{
            PcdSetBoolS(PcdXgbeMdio1, TRUE);
            PcdSetBoolS(PcdXgbeSfp1,   FALSE);
          }
        }else{   // SFP Configuration
          DxioPortList->EtherNet.EthPortProp0.MdioId   = 0;
          DxioPortList->EtherNet.EthPortProp0.PlatConf  = (PortNum == ETH_PORT_0)?AmdPbsConfiguration.Port0ModeSel:(AmdPbsConfiguration.Port1ModeSel);
          DxioPortList->EtherNet.EthPortProp0.MdioId   = 0;
          DxioPortList->EtherNet.EthPortProp0.SuppSpeed = (PortNum == ETH_PORT_0)?AmdPbsConfiguration.Port0SpeedSel:AmdPbsConfiguration.Port1SpeedSel;
          if(IsPlatconfBackplane(DxioPortList->EtherNet.EthPortProp0.PlatConf))
            DxioPortList->EtherNet.EthPortProp0.ConnType  = XGBE_BACKPLANE_CONNECTION;
          else{
             DxioPortList->EtherNet.EthPortProp0.ConnType  = XGBE_SFP_PLUS_CONNECTION;
              DxioPortList->EtherNetExt.EthPortRxEq.RxDfeByp0 = 1;
          }
          DxioPortList->EtherNet.PadMux0 = (PortNum == ETH_PORT_0)? 0x01 : 0x01; // One I2C MUXED for both ports>
          DxioPortList->EtherNet.PadMux1 = 0;
          DxioPortList->EtherNet.EthPortProp3.SfpGpioMask = 2;
          DxioPortList->EtherNet.EthPortProp3.SfpGpioAdd  = 1;
          DxioPortList->EtherNet.EthPortProp3.Rs          = 0;
          DxioPortList->EtherNet.EthPortProp3.TxFault   = (PortNum == ETH_PORT_0)?0xE:0xA;
          DxioPortList->EtherNet.EthPortProp3.ModAbs    = (PortNum == ETH_PORT_0)?0xC:0x8;;
          DxioPortList->EtherNet.EthPortProp3.RxLoss    = (PortNum == ETH_PORT_0) ?0xD:0x9;
          DxioPortList->EtherNet.EthPortProp4.SfpMux      = 0;
          DxioPortList->EtherNet.EthPortProp4.SfpMuxUpAdd = 0x1C;
          DxioPortList->EtherNet.EthPortProp4.RedriverAddress = 0;
          DxioPortList->EtherNet.EthPortProp4.RedriverInterface = 1;
          DxioPortList->EtherNet.EthPortProp4.RedriverLane = 0;
          DxioPortList->EtherNet.EthPortProp4.RedriverModel = 0;
          DxioPortList->EtherNet.EthPortProp4.RedriverPresent = 0;
          DxioPortList->EtherNet.EthPortProp4.SfpBusSeg = (PortNum == ETH_PORT_0) ? 0x1 : 0x2;
          DxioPortList->EtherNet.EthPortTxEq.TxEqPre      = 0;
          DxioPortList->EtherNet.EthPortTxEq.TxEqMain     = 34;
          DxioPortList->EtherNet.EthPortTxEq.TxEqPost     = 24;
          UpdateXgbeTxPolarity(PeiServices,PortNum, TRUE);

          if(IsPlatconfBackplane(DxioPortList->EtherNet.EthPortProp0.PlatConf)){
            if(PortNum == ETH_PORT_0){
            PcdSetBoolS(PcdXgbeMdio0, FALSE);
            PcdSetBoolS(PcdXgbeSfp0,  FALSE);
            }else{
            PcdSetBoolS(PcdXgbeMdio1, FALSE);
            PcdSetBoolS(PcdXgbeSfp1,  FALSE);
            }
           }else{
            if(PortNum == ETH_PORT_0){
              PcdSetBoolS(PcdXgbeMdio0, FALSE);
              PcdSetBoolS(PcdXgbeSfp0,  TRUE);
            }
            else{
              PcdSetBoolS(PcdXgbeMdio1, FALSE);
              PcdSetBoolS(PcdXgbeSfp1,  TRUE);
            }
          }
        }
        DumpEthernetEngine(DxioPortList);
      }
      else{   // CPM table override for Add In card
        DxioPortList->EtherNetExt.EthPortRxEq.RxDfeByp0 = 0;
        if(AmdPbsConfiguration.AicTypeSwitch == XGBE_AIC_H551_SFP){ // H551 Configurations
          DxioPortList->EtherNet.EthPortProp0.MdioId   = 0;
          DxioPortList->EtherNet.EthPortProp0.PlatConf  = AmdPbsConfiguration.AICModeSel;
          DxioPortList->EtherNet.EthPortProp0.MdioId   = 0;
          DxioPortList->EtherNet.EthPortProp0.SuppSpeed = AmdPbsConfiguration.AICSpeedSel;
          DxioPortList->EtherNet.EthPortProp0.ConnType  = XGBE_SFP_PLUS_CONNECTION;
          DxioPortList->EtherNet.PadMux0 =  0x01;
          DxioPortList->EtherNet.PadMux1 = 0;
          DxioPortList->EtherNet.EthPortProp3.SfpGpioMask = 2;

          DxioPortList->EtherNet.EthPortProp4.RedriverPresent = 1;
          if(AmdPbsConfiguration.Aic1TypeSel == 0){  //Inpi PHY
            DxioPortList->EtherNet.EthPortProp3.SfpGpioAdd  = 0;
            DxioPortList->EtherNet.EthPortProp4.SfpBusSeg = (PortNum == ETH_PORT_0) ?IOMUX_CHANNEL_2:IOMUX_CHANNEL_3;
             DxioPortList->EtherNet.EthPortProp4.RedriverAddress = 0x40;
             DxioPortList->EtherNet.EthPortProp4.RedriverInterface = 1;
             DxioPortList->EtherNet.EthPortProp4.RedriverPresent = 1;
               DxioPortList->EtherNetExt.EthPortRxEq.RxEqApplicable = 0;

          }else{
            DxioPortList->EtherNet.EthPortProp3.SfpGpioAdd  = 1;
            DxioPortList->EtherNet.EthPortProp4.SfpBusSeg = (PortNum == ETH_PORT_0) ?IOMUX_CHANNEL_0:IOMUX_CHANNEL_1;
            DxioPortList->EtherNet.EthPortProp4.RedriverAddress = 0;
            DxioPortList->EtherNet.EthPortProp4.RedriverInterface = 1;
            DxioPortList->EtherNet.EthPortProp4.RedriverPresent = 0;
             DxioPortList->EtherNetExt.EthPortRxEq.RxEqApplicable = 1;
             if(PortNum == ETH_PORT_1){
                UpdateXgbeTxPolarity(PeiServices,PortNum, TRUE);
             }
          }
          DxioPortList->EtherNet.EthPortProp3.Rs          = 0;
          DxioPortList->EtherNet.EthPortProp3.TxFault   = (PortNum == ETH_PORT_0)?0xE:0xA;
          DxioPortList->EtherNet.EthPortProp3.ModAbs    = (PortNum == ETH_PORT_0)?0xC:0x8;;
          DxioPortList->EtherNet.EthPortProp3.RxLoss    =(PortNum == ETH_PORT_0) ?0xD:0x9;
          DxioPortList->EtherNet.EthPortProp4.SfpMux      = 0;
          DxioPortList->EtherNet.EthPortProp4.SfpMuxUpAdd = 0x1C;
          DxioPortList->EtherNet.EthPortProp4.RedriverLane = (PortNum == ETH_PORT_0) ?0:1;
          DxioPortList->EtherNet.EthPortProp4.RedriverModel = 0;
          DxioPortList->EtherNet.EthPortTxEq.TxEqPre      = 0;
          DxioPortList->EtherNet.EthPortTxEq.TxEqMain     = 40;
          DxioPortList->EtherNet.EthPortTxEq.TxEqPost     = 0;

          PcdSetBoolS(PcdXgbeSfp0,  TRUE);
          PcdSetBoolS(PcdXgbeSfp1,  TRUE);
          PcdSetBoolS(PcdXgbeMdio0, FALSE);
          PcdSetBoolS(PcdXgbeMdio1, FALSE);

        }else {
           DxioPortList->EtherNet.EthPortProp0.MdioId   = 0;
          DxioPortList->EtherNet.EthPortProp0.PlatConf  = AmdPbsConfiguration.AICModeSel;
          DxioPortList->EtherNet.EthPortProp0.MdioId   = 0;
          DxioPortList->EtherNet.EthPortProp0.SuppSpeed = AmdPbsConfiguration.AICSpeedSel;
          DxioPortList->EtherNet.EthPortProp0.ConnType  = XGBE_CONNECTION_MDIO_PHY;
          DxioPortList->EtherNet.PadMux0 = (PortNum == ETH_PORT_0)? 0x10 : 0x20;
          DxioPortList->EtherNet.PadMux1 = 0;
          DxioPortList->EtherNet.EthPortProp3.SfpGpioMask = 2;
          DxioPortList->EtherNet.EthPortProp3.SfpGpioAdd  = 1;

          DxioPortList->EtherNet.EthPortProp3.Rs          = 0;
          if(AmdPbsConfiguration.AicTypeSwitch == XGBE_AIC_H552_MVL){
            DxioPortList->EtherNet.PadMux0 = (PortNum == ETH_PORT_0)? 0x10 : 0x20;
          if(AmdPbsConfiguration.Aic2TypeSel ==0){ //PHY_RST for 10G
            DxioPortList->EtherNet.PadMux0 = (PortNum == ETH_PORT_0)? 0x20 : 0x10;
            DxioPortList->EtherNet.EthPortProp3.TxFault   = (PortNum == ETH_PORT_0)?0xF:0xE;
              DxioPortList->EtherNetExt.EthPortRxEq.RxEqApplicable = 1;
          }
            else      //PHY_RST for 1G
            DxioPortList->EtherNet.EthPortProp3.TxFault   = (PortNum == ETH_PORT_0)?0xB:0xA;
          }else{
              DxioPortList->EtherNet.EthPortProp0.MdioId   =  (PortNum == ETH_PORT_0)?0:1;
              DxioPortList->EtherNet.PadMux0 = 0x10;  //H553 uses single MDIO lane for both ports
          }
          if (AmdPbsConfiguration.AicTypeSwitch == XGBE_ADD_H553_BCM && AmdPbsConfiguration.Aic3TypeSel == AIC3_10G) //MDIO_ID change for AIC3 10G PHY
            DxioPortList->EtherNet.EthPortProp0.MdioId   =  (PortNum == ETH_PORT_0)?1:0;

          DxioPortList->EtherNet.EthPortProp3.ModAbs    = (PortNum == ETH_PORT_0)?0xC:0x8;;
          DxioPortList->EtherNet.EthPortProp3.RxLoss    =(PortNum == ETH_PORT_0) ?0xD:0x9;
          DxioPortList->EtherNet.EthPortProp4.SfpMux      = 0;
          DxioPortList->EtherNet.EthPortProp4.SfpMuxUpAdd = 0x1C;
          DxioPortList->EtherNet.EthPortProp4.RedriverAddress = 0;
          DxioPortList->EtherNet.EthPortProp4.RedriverInterface = 1;
          DxioPortList->EtherNet.EthPortProp4.RedriverLane = 0;
          DxioPortList->EtherNet.EthPortProp4.RedriverModel = 0;
          DxioPortList->EtherNet.EthPortProp4.RedriverPresent = 0;
          DxioPortList->EtherNet.EthPortProp4.SfpBusSeg = (PortNum == ETH_PORT_0) ?0:1;
          DxioPortList->EtherNet.EthPortTxEq.TxEqPre      = 0;
          DxioPortList->EtherNet.EthPortTxEq.TxEqMain     = 40;
          DxioPortList->EtherNet.EthPortTxEq.TxEqPost     = 0;
          if (AmdPbsConfiguration.Aic3RevSel == XGBE_AIC3_REVA) {
            if (AmdPbsConfiguration.AicTypeSwitch == XGBE_ADD_H553_BCM && AmdPbsConfiguration.Aic3TypeSel == AIC3_10G) // RX and TX Polarity reversal for AIC3 10G PHY
            UpdateXgbeTxPolarity(PeiServices,PortNum, TRUE);
          }
          PcdSetBoolS(PcdXgbeMdio0, TRUE);
          PcdSetBoolS(PcdXgbeMdio1, TRUE);
          PcdSetBoolS(PcdXgbeSfp0,  FALSE);
          PcdSetBoolS(PcdXgbeSfp1,  FALSE);

        }
        DumpEthernetEngine(DxioPortList);
      }
     if ((DxioPortList->Flags & DESCRIPTOR_TERMINATE_LIST) == DESCRIPTOR_TERMINATE_LIST)
       break;
    }//End of Ethernet Engine


    DxioPortList++;
  }  //End of DXIO descriptor search loop

  return Status;

}

EFI_STATUS
EFIAPI
UpdateXgbeFchPcds (
  IN EFI_PEI_SERVICES** PeiServices,
  IN DXIO_PORT_DESCRIPTOR** DxioPort
  )
{
  UINTN PcdBufSize = sizeof(FCH_XGBE_PORT);
  EFI_STATUS Status = EFI_SUCCESS;
  VOID* Buffer  = NULL;
  FCH_XGBE_PORT* Port0 = NULL;
  FCH_XGBE_PORT *Port1 = NULL;
  FCH_XGBE_PORT *PortPtr = NULL;
  DXIO_PORT_DESCRIPTOR* DxioPortList = (*DxioPort);


  DEBUG((DEBUG_INFO, "In UpdateXgbeFchPcds\n"));

  if (DxioPortList == NULL) {
    DEBUG((DEBUG_INFO, "UpdateXgbeFchPcds, Invalid Port List\n"));
    return EFI_INVALID_PARAMETER;
  }



  Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               sizeof(FCH_XGBE_PORT),
                               (VOID**)&Port0
                               );
    if (EFI_ERROR (Status)) {
      DEBUG((DEBUG_INFO, "UpdateXgbeFchPcds AllocatePool failed, Status = %r\n", Status));
      return Status;
   }
   (*PeiServices)->SetMem(Port0, sizeof(FCH_XGBE_PORT), 0x0);

    Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               sizeof(FCH_XGBE_PORT),
                               (VOID**)&Port1
                               );
    if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "UpdateXgbeFchPcds AllocatePool failed, Status = %r\n", Status));
    return Status;
   }
   (*PeiServices)->SetMem(Port1, sizeof(FCH_XGBE_PORT), 0x0);

   DEBUG((DEBUG_INFO, "UpdateXgbeFchPcds Port0 = 0x%08X, Port1 = 0x%08X, Len = 0x%x\n", Port0, Port1, sizeof(FCH_XGBE_PORT)));
   while(((DxioPortList->Flags & DESCRIPTOR_TERMINATE_LIST) != DESCRIPTOR_TERMINATE_LIST)||
   ((DxioPortList->Flags & DESCRIPTOR_TERMINATE_LIST) == DESCRIPTOR_TERMINATE_LIST) && (DxioPortList->EngineData.EngineType == DxioEthernetEngine)){
    if(DxioPortList->EngineData.EngineType == DxioEthernetEngine) {
      DEBUG((DEBUG_INFO, "UpdateXgbeFchPcds, found DxioEthernetEngine\n"));

        PortPtr = (DxioPortList->EtherNet.EthPortProp0.PortNum == ETH_PORT_0) ? Port0 : Port1;

        PortPtr->XgbePortConfig  = 0x1;
        PortPtr->XgbePortPlatformConfig = (UINT8) DxioPortList->EtherNet.EthPortProp0.PlatConf;
        PortPtr->XgbePortSupportedSpeed  = (UINT8) DxioPortList->EtherNet.EthPortProp0.SuppSpeed;
        PortPtr->XgbePortConnectedType  = (UINT8) DxioPortList->EtherNet.EthPortProp0.ConnType;
        PortPtr->XgbePortMdioId         = (UINT8) DxioPortList->EtherNet.EthPortProp0.MdioId;

        DEBUG((DEBUG_INFO, "PortPtr->XgbePortConfig         = 0x%x\n", PortPtr->XgbePortConfig));
        DEBUG((DEBUG_INFO, "PortPtr->XgbePortPlatformConfig = 0x%x\n", PortPtr->XgbePortPlatformConfig));
        DEBUG((DEBUG_INFO, "PortPtr->XgbePortSupportedSpeed = 0x%x\n", PortPtr->XgbePortSupportedSpeed));
        DEBUG((DEBUG_INFO, "PortPtr->XgbePortConnectedType  = 0x%x\n", PortPtr->XgbePortConnectedType));

        PortPtr->XgbePortMdioResetType        = (UINT8) DxioPortList->EtherNet.EthPortProp3.MdioReset;
        PortPtr->XgbePortResetGpioNum         = (UINT8) DxioPortList->EtherNet.EthPortProp3.MdioGpioResetNum;
        PortPtr->XgbePortMdioResetI2cAddress  = (UINT8) 0x0;
        PortPtr->XgbePortSfpI2cAddress        = (UINT8) DxioPortList->EtherNet.EthPortProp3.SfpGpioAdd;
        PortPtr->XgbePortSfpTxFaultGpio       = (UINT8) DxioPortList->EtherNet.EthPortProp3.TxFault;
        PortPtr->XgbePortSfpRsGpio            = (UINT8) DxioPortList->EtherNet.EthPortProp3.Rs;
        PortPtr->XgbePortSfpModAbsGpio        = (UINT8) DxioPortList->EtherNet.EthPortProp3.ModAbs;
        PortPtr->XgbePortSfpRxLosGpio         = (UINT8) DxioPortList->EtherNet.EthPortProp3.RxLoss;
        PortPtr->XgbePortSfpGpioMask          = (UINT8) DxioPortList->EtherNet.EthPortProp3.SfpGpioMask;

        DEBUG((DEBUG_INFO, "PortPtr->XgbePortMdioResetType       = 0x%x\n", PortPtr->XgbePortMdioResetType));
        DEBUG((DEBUG_INFO, "PortPtr->XgbePortResetGpioNum        = 0x%x\n", PortPtr->XgbePortResetGpioNum));
        DEBUG((DEBUG_INFO, "PortPtr->XgbePortMdioResetI2cAddress = 0x%x\n", PortPtr->XgbePortMdioResetI2cAddress));
        DEBUG((DEBUG_INFO, "PortPtr->XgbePortSfpI2cAddress       = 0x%x\n", PortPtr->XgbePortSfpI2cAddress));
        DEBUG((DEBUG_INFO, "PortPtr->XgbePortSfpTxFaultGpio      = 0x%x\n", PortPtr->XgbePortSfpTxFaultGpio));
        DEBUG((DEBUG_INFO, "PortPtr->XgbePortSfpRxLosGpio        = 0x%x\n", PortPtr->XgbePortSfpRxLosGpio));
        DEBUG((DEBUG_INFO, "PortPtr->XgbePortSfpGpioMask         = 0x%x\n", PortPtr->XgbePortSfpGpioMask));
        DEBUG((DEBUG_INFO, "PortPtr->XgbePortSfpRsGpio           = 0x%x\n", PortPtr->XgbePortSfpRsGpio));
        DEBUG((DEBUG_INFO, "PortPtr->XgbePortSfpModAbsGpio       = 0x%x\n", PortPtr->XgbePortSfpModAbsGpio));

        PortPtr->XgbePortSfpTwiAddress     = (UINT8) (( DxioPortList->EtherNet.EthPortProp4.SfpMuxUpAdd << 3 ) | ( DxioPortList->EtherNet.EthPortProp4.SfpMux));
        PortPtr->XgbePortSfpTwiBus         = (UINT8)DxioPortList->EtherNet.EthPortProp4.SfpBusSeg;
        PortPtr->XgbaPortRedriverPresent   = (UINT8) DxioPortList->EtherNet.EthPortProp4.RedriverPresent;
        PortPtr->XgbaPortRedriverModel     = (UINT8) DxioPortList->EtherNet.EthPortProp4.RedriverModel;
        PortPtr->XgbaPortRedriverInterface = (UINT8) DxioPortList->EtherNet.EthPortProp4.RedriverInterface;
        PortPtr->XgbaPortRedriverAddress   = (UINT8) DxioPortList->EtherNet.EthPortProp4.RedriverAddress;
        PortPtr->XgbaPortRedriverLane      = (UINT8) DxioPortList->EtherNet.EthPortProp4.RedriverLane;
        PortPtr->XgbaPortPadGpio           = 0x0;
        // change PadMux based on the sideband connection MDIO/SFP+
        if (PortPtr->XgbePortConnectedType == XGBE_CONNECTION_MDIO_PHY) {
          PortPtr->XgbaPortPadMdio           = (UINT8) ((DxioPortList->EtherNet.PadMux0 >> 4) & 0xFF);
        } else {
          PortPtr->XgbaPortPadI2C            = (UINT8) DxioPortList->EtherNet.PadMux0;
        }
        PortPtr->Reserve1                  = 0x0;
        DEBUG((DEBUG_INFO, "PortPtr->XgbaPortRedriverModel     = 0x%x\n", PortPtr->XgbaPortRedriverModel));
        DEBUG((DEBUG_INFO, "PortPtr->XgbaPortRedriverInterface = 0x%x\n", PortPtr->XgbaPortRedriverInterface));
        DEBUG((DEBUG_INFO, "PortPtr->XgbaPortRedriverAddress   = 0x%x\n", PortPtr->XgbaPortRedriverAddress));
        DEBUG((DEBUG_INFO, "PortPtr->XgbaPortRedriverLane      = 0x%x\n", PortPtr->XgbaPortRedriverLane));
        DEBUG((DEBUG_INFO, "PortPtr->XgbaPortPadGpio           = 0x%x\n", PortPtr->XgbaPortPadGpio));
        DEBUG((DEBUG_INFO, "PortPtr->XgbaPortPadMdio           = 0x%x\n", PortPtr->XgbaPortPadMdio));
        DEBUG((DEBUG_INFO, "PortPtr->XgbaPortPadI2C            = 0x%x\n", PortPtr->XgbaPortPadI2C));

        if (DxioPortList->EtherNet.EthPortProp0.PortNum == ETH_PORT_0) {
          Buffer = (VOID*)PcdSetPtrS(PcdXgbePort0Table, &PcdBufSize, (VOID*) Port0);
        } else {
          Buffer = (VOID*)PcdSetPtrS(PcdXgbePort1Table, &PcdBufSize, (VOID*) Port1);
        }
        Status = (Buffer != NULL) ? EFI_SUCCESS : EFI_BUFFER_TOO_SMALL;
        DEBUG((DEBUG_INFO, "PcdSetPtrS  , Status = %r\n", Status));
        DEBUG((DEBUG_INFO, "PcdBufSzie = 0x%x\n", PcdBufSize));
       if ((DxioPortList->Flags & DESCRIPTOR_TERMINATE_LIST) == DESCRIPTOR_TERMINATE_LIST)
       break;
    }
    DxioPortList++;
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Function to Set/clear Tx polarity
 *
 *@param[in]     PeiServices    Pointer to Pei Services
 *@param[in]     PortNum        Port which Tx polarity needs be updated.
 *@param[in]     TxPol          TxPol value to be set
 * @retval       EFI_STATUS

 */
EFI_STATUS UpdateXgbeTxPolarity(
  IN EFI_PEI_SERVICES** PeiServices,
  IN UINT32 PortNum ,
  IN BOOLEAN TxPol
  )
{
  EFI_STATUS           Status = EFI_SUCCESS;
  AMD_CPM_TABLE_PPI    *AmdCpmTablePpi;
  AMD_PBS_SETUP_OPTION AmdPbsConfiguration;
  UINT16               Data = 0;

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);

  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-UpdateXgbeTxPolarity-Status=%r\n", Status));
    return Status;
  }

  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gAmdCpmTablePpiGuid,
                           0,
                           NULL,
                           (VOID**)&AmdCpmTablePpi
                           );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "  [%d]: OEM-PEI-AmdCpmTablePpi-Status=%r\n", __LINE__, Status));
    return Status;
  }
  AmdCpmTablePpi->CommonFunction.SmnWrite32(AmdCpmTablePpi, 0, 0, 0, 0x11dF806c + (PortNum * 0x100000), 0,0x70000); // 11DF_806C=11D0_0000 + F806C ( Aperture Index Base + Aperture Index Offset )
  Data= AmdCpmTablePpi->CommonFunction.SmnRead16(AmdCpmTablePpi, 0, 0, 0, 0x11DF1002 + (PortNum * 0x100000), 0);  // 0x11DF1002 = 11D0_0000 + F1000+(70002&FFFF)
                                                                                                                  //(Aperture Index Base + Appr window select+ (offset & 0xFFFF))
  DEBUG((DEBUG_INFO, "before SMN read Data of VR_XS_PCS_DIG_CTRL2: 0x%X  \n",Data));
  if (TxPol){
    Data |= BIT4;   //Set the polarity inversion bit for TX
    if (AmdPbsConfiguration.Aic3RevSel == XGBE_AIC3_REVA) {
      if (AmdPbsConfiguration.AicTypeSwitch == XGBE_ADD_H553_BCM && AmdPbsConfiguration.Aic3TypeSel == AIC3_10G)
        Data |= BIT0;   //Set the polarity inversion bit for RX only for 10G
	}
    } else{
    Data &=~BIT4;
  }
  AmdCpmTablePpi->CommonFunction.SmnWrite16(AmdCpmTablePpi, 0, 0, 0, 0x11DF1002 + (PortNum * 0x100000), 0,Data);
  Data= AmdCpmTablePpi->CommonFunction.SmnRead16(AmdCpmTablePpi, 0, 0, 0,0x11DF1002 + (PortNum * 0x100000), 0);
  DEBUG((DEBUG_INFO, "After SMN read Data of VR_XS_PCS_DIG_CTRL2: 0x%X  \n",Data));

  return Status;
}
