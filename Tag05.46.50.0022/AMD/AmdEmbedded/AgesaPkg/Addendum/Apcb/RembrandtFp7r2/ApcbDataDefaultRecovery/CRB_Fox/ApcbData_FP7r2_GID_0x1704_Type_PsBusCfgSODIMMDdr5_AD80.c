/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

#include <MyPorting.h>
#include <MiscMemDefines.h>
#include <APCB.h>
#include "ApcbCustomizedDefinitions.h"
#include "ApcbCustomizedBoardDefinitions.h"
#include <ApcbDefaults.h>

#define PT_DONT_CARE 0xFF
#define NP     1
#define V1_5   1
#define V1_35  2
#define V1_25  4
#define V1_2   1
#define V_TBD1 2
#define V_TBD2 4
#define V1_1   1
#define V0_6   2
//
// Rank
//
#define DIMM_NP 1
#define DIMM_SR 2
#define DIMM_DR 4
#define DIMM_QR 8
//
// Device Width
//
#define DEVWIDTH_4   0x01
#define DEVWIDTH_8   0x02
#define DEVWIDTH_16  0x04
#define DEVWIDTH_32  0x08
#define DEVWIDTH_ALL 0xFF
//
// MPstate
//
#define MP0    0x01
#define MP1    0x02
#define MP2    0x04
#define MP3    0x08
#define MP_ALL 0xFF
//
// Timing
//
#define TMG_1N    1
#define TMG_2N    0
//
// Defined values for CK_ODT, CS_ODT, and CK_ODT
//
#define D5_ODT_OFF        0
#define D5_ODT_480        1
#define D5_ODT_240        2
#define D5_ODT_120        3
#define D5_ODT_80         4
#define D5_ODT_60         5
#define D5_ODT_40         7
//
// Defined values for RTT_NOM_WR,RTT_NOM_RD
//
#define D5_RTT_OFF        0
#define D5_RTT_240        1
#define D5_RTT_120        2
#define D5_RTT_80         3
#define D5_RTT_60         4
#define D5_RTT_48         5
#define D5_RTT_40         6
#define D5_RTT_34         7
//
// Generic, un-encoded impedance values for various purposes. They can be
// translated to the appropriate value for how it is consumed.
//
#define D5_IMP_OFF        0
#define D5_IMP_480        480
#define D5_IMP_240        240
#define D5_IMP_160        160
#define D5_IMP_120        120
#define D5_IMP_96         96
#define D5_IMP_80         80
#define D5_IMP_68         68
#define D5_IMP_60         60
#define D5_IMP_53         53
#define D5_IMP_48         48
#define D5_IMP_43         43
#define D5_IMP_40         40
#define D5_IMP_36         36
#define D5_IMP_34         34
#define D5_IMP_32         32
#define D5_IMP_30         30
#define D5_IMP_28         28
#define D5_IMP_26         26
#define D5_IMP_25         25
//
// DFE
//
#define DFE_DIS           0
#define DFE_EN            1


//
// The entry header structure
//
typedef struct _PSCFG_DDR5_BUS_ENTRY_HEADER_S {
  UINT32 Length;          ///< The size of this structure
  UINT32 MemClk;          ///< The target MemClk Frequency
  UINT8  DimmPerCh;       ///< The number of DIMM slot per chanel
  UINT8  Dimm0;           ///< The bitmap of rank type of DIMM0
  UINT8  Dimm1;           ///< The bitmap of rank type of DIMM1
  UINT8  DevWidth;        ///< The bitmap of SDRAM IO width
} PSCFG_DDR5_BUS_ENTRY_HEADER_S;

//
// The entry payload structure
// The entry data is listed at the following.
//
typedef struct _PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S {
  UINT32 Length;          ///< The size of this structure
  UINT32 CaTimingMode;    ///< CaTimingMode
  UINT32 RttNomRd;        ///< RTT_NOM_RD
  UINT32 RttNomWr;        ///< RTT_NOM_WR
  UINT32 RttWr;           ///< RTT_WR
  UINT32 RttPark;         ///< RTT_PARK
  UINT32 DqsRttPark;      ///< DQS_RTT_PARK
  UINT32 DramDrv;         ///< DRAM_DRV
  UINT32 CkOdtA;          ///< CK_ODT Group A
  UINT32 CsOdtA;          ///< CS_ODT Group A
  UINT32 CaOdtA;          ///< CA_ODT Group A
  UINT32 CkOdtB;          ///< CK_ODT Group B
  UINT32 CsOdtB;          ///< CS_ODT Group B
  UINT32 CaOdtB;          ///< CA_ODT Group B
  UINT32 POdt;            ///< PODT
  UINT32 DqDrv;           ///< DQ Drv
  UINT32 AlertPu;         ///< Alert Pullup
  UINT32 CaDrv;           ///< CA Drv
  UINT32 PhyVref;         ///< PHY Vref
  UINT32 DqVref;          ///< DQ Vref
  UINT32 CaVref;          ///< CA Vref
  UINT32 CsVref;          ///< CS Vref
  UINT32 RxDfe;           ///< RXDFE
  UINT32 TxDfe;           ///< TXDFE
} PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S;

/// DDR5 Bus configuration entry for DRAMDOWN, SODIMM, or UDIMM
typedef struct _PSCFG_BUS_ENTRY_S {
  PSCFG_DDR5_BUS_ENTRY_HEADER_S   Header;
  PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S  Payload;
} PSCFG_BUS_ENTRY_S;

//SODIMM BUSCFG DDR5
APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_MEMORY,                          // GroupId
  APCB_MEM_TYPE_PS_SODIMM_DDR5_BUSCFG,        // TypeId
  sizeof(ApcbTypeHeader),                     // SizeOfType, will be fixed up by tool
  0xAD80,                                     // InstanceId
  {
    APCB_TYPE_ATTR_CONTEXT_TYPE_STRUCT,
    APCB_TYPE_ATTR_CONTEXT_FORMAT_NATIVE_RAW,
    0,
    APCB_PRIORITY_TYPE_MASK_DEFAULT,          // Priority mask
    0,
    0,
    BLDCFG_APCB_DATA_BOARD_MASK               // Board specific APCB instance mask
  }
};  // SizeOfType will be fixed up by tool

PSCFG_BUS_ENTRY_S PlatformCfgBus[] = {
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR2000_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_SR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_ALL & (~DEVWIDTH_16),
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_1N,
      .RttNomRd     = D5_IMP_OFF,
      .RttNomWr     = D5_IMP_OFF,
      .RttWr        = D5_IMP_60,
      .RttPark      = D5_IMP_60,
      .DqsRttPark   = D5_IMP_60,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_480,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_40,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x63,
      .DqVref       = 0x3F,
      .CaVref       = 0x36,
      .CsVref       = 0x35,
      .RxDfe        = DFE_DIS,
      .TxDfe        = DFE_DIS,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR2000_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_SR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_16,
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_1N,
      .RttNomRd     = D5_IMP_OFF,
      .RttNomWr     = D5_IMP_OFF,
      .RttWr        = D5_IMP_60,
      .RttPark      = D5_IMP_60,
      .DqsRttPark   = D5_IMP_60,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_OFF,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_40,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x63,
      .DqVref       = 0x3F,
      .CaVref       = 0x44,
      .CsVref       = 0x35,
      .RxDfe        = DFE_DIS,
      .TxDfe        = DFE_DIS,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR2000_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_DR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_ALL & (~DEVWIDTH_8),
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_2N,
      .RttNomRd     = D5_IMP_60,
      .RttNomWr     = D5_IMP_80,
      .RttWr        = D5_IMP_120,
      .RttPark      = D5_IMP_120,
      .DqsRttPark   = D5_IMP_80,
      .DramDrv      = D5_IMP_34,
      .CkOdtA       = D5_IMP_OFF,
      .CsOdtA       = D5_IMP_OFF,
      .CaOdtA       = D5_IMP_OFF,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_40,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x69,
      .DqVref       = 0x3F,
      .CaVref       = 0x38,
      .CsVref       = 0x35,
      .RxDfe        = DFE_DIS,
      .TxDfe        = DFE_DIS,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR2000_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_DR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_8,
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_2N,
      .RttNomRd     = D5_IMP_60,
      .RttNomWr     = D5_IMP_80,
      .RttWr        = D5_IMP_120,
      .RttPark      = D5_IMP_120,
      .DqsRttPark   = D5_IMP_80,
      .DramDrv      = D5_IMP_34,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_480,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_80,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_40,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x69,
      .DqVref       = 0x3F,
      .CaVref       = 0x27,
      .CsVref       = 0x35,
      .RxDfe        = DFE_DIS,
      .TxDfe        = DFE_DIS,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR3200_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_SR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_ALL & (~DEVWIDTH_16),
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_1N,
      .RttNomRd     = D5_IMP_OFF,
      .RttNomWr     = D5_IMP_OFF,
      .RttWr        = D5_IMP_60,
      .RttPark      = D5_IMP_60,
      .DqsRttPark   = D5_IMP_60,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_480,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x63,
      .DqVref       = 0x3F,
      .CaVref       = 0x36,
      .CsVref       = 0x35,
      .RxDfe        = DFE_DIS,
      .TxDfe        = DFE_DIS,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR3200_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_SR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_16,
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_1N,
      .RttNomRd     = D5_IMP_OFF,
      .RttNomWr     = D5_IMP_OFF,
      .RttWr        = D5_IMP_60,
      .RttPark      = D5_IMP_60,
      .DqsRttPark   = D5_IMP_60,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_OFF,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x63,
      .DqVref       = 0x3F,
      .CaVref       = 0x44,
      .CsVref       = 0x35,
      .RxDfe        = DFE_DIS,
      .TxDfe        = DFE_DIS,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR3200_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_DR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_ALL & (~DEVWIDTH_8),
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_2N,
      .RttNomRd     = D5_IMP_60,
      .RttNomWr     = D5_IMP_80,
      .RttWr        = D5_IMP_120,
      .RttPark      = D5_IMP_120,
      .DqsRttPark   = D5_IMP_80,
      .DramDrv      = D5_IMP_34,
      .CkOdtA       = D5_IMP_OFF,
      .CsOdtA       = D5_IMP_OFF,
      .CaOdtA       = D5_IMP_OFF,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x69,
      .DqVref       = 0x3F,
      .CaVref       = 0x38,
      .CsVref       = 0x35,
      .RxDfe        = DFE_DIS,
      .TxDfe        = DFE_DIS,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR3200_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_DR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_8,
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_2N,
      .RttNomRd     = D5_IMP_60,
      .RttNomWr     = D5_IMP_80,
      .RttWr        = D5_IMP_120,
      .RttPark      = D5_IMP_120,
      .DqsRttPark   = D5_IMP_80,
      .DramDrv      = D5_IMP_34,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_480,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_80,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x69,
      .DqVref       = 0x3F,
      .CaVref       = 0x27,
      .CsVref       = 0x35,
      .RxDfe        = DFE_DIS,
      .TxDfe        = DFE_DIS,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR4000_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_SR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_ALL & (~DEVWIDTH_16),
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_1N,
      .RttNomRd     = D5_IMP_OFF,
      .RttNomWr     = D5_IMP_OFF,
      .RttWr        = D5_IMP_60,
      .RttPark      = D5_IMP_60,
      .DqsRttPark   = D5_IMP_60,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_480,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x63,
      .DqVref       = 0x3F,
      .CaVref       = 0x36,
      .CsVref       = 0x35,
      .RxDfe        = DFE_EN,
      .TxDfe        = DFE_EN,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR4000_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_SR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_16,
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_1N,
      .RttNomRd     = D5_IMP_OFF,
      .RttNomWr     = D5_IMP_OFF,
      .RttWr        = D5_IMP_60,
      .RttPark      = D5_IMP_60,
      .DqsRttPark   = D5_IMP_60,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_OFF,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x63,
      .DqVref       = 0x3F,
      .CaVref       = 0x44,
      .CsVref       = 0x35,
      .RxDfe        = DFE_EN,
      .TxDfe        = DFE_EN,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR4000_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_DR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_ALL & (~DEVWIDTH_8),
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_2N,
      .RttNomRd     = D5_IMP_80,
      .RttNomWr     = D5_IMP_80,
      .RttWr        = D5_IMP_120,
      .RttPark      = D5_IMP_120,
      .DqsRttPark   = D5_IMP_80,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_OFF,
      .CsOdtA       = D5_IMP_OFF,
      .CaOdtA       = D5_IMP_OFF,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x69,
      .DqVref       = 0x3F,
      .CaVref       = 0x38,
      .CsVref       = 0x35,
      .RxDfe        = DFE_EN,
      .TxDfe        = DFE_EN,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR4000_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_DR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_8,
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_2N,
      .RttNomRd     = D5_IMP_80,
      .RttNomWr     = D5_IMP_80,
      .RttWr        = D5_IMP_120,
      .RttPark      = D5_IMP_120,
      .DqsRttPark   = D5_IMP_80,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_480,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_80,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x69,
      .DqVref       = 0x3F,
      .CaVref       = 0x27,
      .CsVref       = 0x35,
      .RxDfe        = DFE_EN,
      .TxDfe        = DFE_EN,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR4400_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_SR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_ALL & (~DEVWIDTH_16),
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_1N,
      .RttNomRd     = D5_IMP_OFF,
      .RttNomWr     = D5_IMP_OFF,
      .RttWr        = D5_IMP_60,
      .RttPark      = D5_IMP_60,
      .DqsRttPark   = D5_IMP_60,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_480,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x63,
      .DqVref       = 0x3F,
      .CaVref       = 0x36,
      .CsVref       = 0x35,
      .RxDfe        = DFE_EN,
      .TxDfe        = DFE_EN,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR4400_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_SR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_16,
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_1N,
      .RttNomRd     = D5_IMP_OFF,
      .RttNomWr     = D5_IMP_OFF,
      .RttWr        = D5_IMP_60,
      .RttPark      = D5_IMP_60,
      .DqsRttPark   = D5_IMP_60,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_OFF,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x63,
      .DqVref       = 0x3F,
      .CaVref       = 0x44,
      .CsVref       = 0x35,
      .RxDfe        = DFE_EN,
      .TxDfe        = DFE_EN,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR4400_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_DR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_ALL & (~DEVWIDTH_8),
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_2N,
      .RttNomRd     = D5_IMP_80,
      .RttNomWr     = D5_IMP_80,
      .RttWr        = D5_IMP_120,
      .RttPark      = D5_IMP_120,
      .DqsRttPark   = D5_IMP_80,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_OFF,
      .CsOdtA       = D5_IMP_OFF,
      .CaOdtA       = D5_IMP_OFF,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x69,
      .DqVref       = 0x3F,
      .CaVref       = 0x38,
      .CsVref       = 0x35,
      .RxDfe        = DFE_EN,
      .TxDfe        = DFE_EN,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR4400_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_DR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_8,
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_2N,
      .RttNomRd     = D5_IMP_80,
      .RttNomWr     = D5_IMP_80,
      .RttWr        = D5_IMP_120,
      .RttPark      = D5_IMP_120,
      .DqsRttPark   = D5_IMP_80,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_480,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_80,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x69,
      .DqVref       = 0x3F,
      .CaVref       = 0x27,
      .CsVref       = 0x35,
      .RxDfe        = DFE_EN,
      .TxDfe        = DFE_EN,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR4800_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_SR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_ALL & (~DEVWIDTH_16),
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_1N,
      .RttNomRd     = D5_IMP_OFF,
      .RttNomWr     = D5_IMP_OFF,
      .RttWr        = D5_IMP_60,
      .RttPark      = D5_IMP_60,
      .DqsRttPark   = D5_IMP_60,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_480,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x63,
      .DqVref       = 0x3F,
      .CaVref       = 0x36,
      .CsVref       = 0x35,
      .RxDfe        = DFE_EN,
      .TxDfe        = DFE_EN,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR4800_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_SR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_16,
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_1N,
      .RttNomRd     = D5_IMP_OFF,
      .RttNomWr     = D5_IMP_OFF,
      .RttWr        = D5_IMP_60,
      .RttPark      = D5_IMP_60,
      .DqsRttPark   = D5_IMP_60,
      .DramDrv      = D5_IMP_40,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_OFF,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x63,
      .DqVref       = 0x3F,
      .CaVref       = 0x44,
      .CsVref       = 0x35,
      .RxDfe        = DFE_EN,
      .TxDfe        = DFE_EN,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR4800_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_DR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_ALL & (~DEVWIDTH_8),
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_2N,
      .RttNomRd     = D5_IMP_60,
      .RttNomWr     = D5_IMP_80,
      .RttWr        = D5_IMP_120,
      .RttPark      = D5_IMP_120,
      .DqsRttPark   = D5_IMP_80,
      .DramDrv      = D5_IMP_34,
      .CkOdtA       = D5_IMP_OFF,
      .CsOdtA       = D5_IMP_OFF,
      .CaOdtA       = D5_IMP_OFF,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_60,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x69,
      .DqVref       = 0x3F,
      .CaVref       = 0x38,
      .CsVref       = 0x35,
      .RxDfe        = DFE_EN,
      .TxDfe        = DFE_EN,
    },
  },
  {
    .Header = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_HEADER_S),
      .MemClk       = DDR4800_FREQUENCY,
      .DimmPerCh    = 1,
      .Dimm0        = DIMM_DR,
      .Dimm1        = DIMM_NP,
      .DevWidth     = DEVWIDTH_8,
    },
    .Payload = {
      .Length       = sizeof(PSCFG_DDR5_BUS_ENTRY_PAYLOAD_S),
      .CaTimingMode = TMG_2N,
      .RttNomRd     = D5_IMP_60,
      .RttNomWr     = D5_IMP_80,
      .RttWr        = D5_IMP_120,
      .RttPark      = D5_IMP_120,
      .DqsRttPark   = D5_IMP_80,
      .DramDrv      = D5_IMP_34,
      .CkOdtA       = D5_IMP_480,
      .CsOdtA       = D5_IMP_480,
      .CaOdtA       = D5_IMP_480,
      .CkOdtB       = D5_IMP_60,
      .CsOdtB       = D5_IMP_60,
      .CaOdtB       = D5_IMP_80,
      .POdt         = D5_IMP_60,
      .DqDrv        = D5_IMP_34,
      .AlertPu      = D5_IMP_60,
      .CaDrv        = D5_IMP_30,
      .PhyVref      = 0x69,
      .DqVref       = 0x3F,
      .CaVref       = 0x27,
      .CsVref       = 0x35,
      .RxDfe        = DFE_EN,
      .TxDfe        = DFE_EN,
    },
  },
};

APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

