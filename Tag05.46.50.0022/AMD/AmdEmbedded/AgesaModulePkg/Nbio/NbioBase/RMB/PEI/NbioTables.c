/*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  <Library/NbioTableBlastLib.h>
#include  <Library/PcdLib.h>
#include  <GnbRegistersRMB.h>
#include  <NbioIoapicTbl.h>
#include  <NbioIohcTbl.h>
#include  <NbioNbifTbl.h>
#include  <NbioWorkaroundTbl.h>
#include  <Filecode.h>

#define FILECODE        NBIO_NBIOBASE_RMB_PEI_NBIOTABLES_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           T A B L E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 *
  */

// Version 1.52.0
GNB_TABLE ROMDATA GnbEarlyInitTable [] = {
  NBIO_IOHC_P2P_TBL                  // 14.1.4.4 Enabling Peer-to-Peer
  NBIO_IOHC_CRS_TBL                  // 14.1.4.6 PCI Express Configuration Cycle Hardware Retry on CRS
  NBIO_IOHC_CLOCK_GATING_TBL         // 14.1.4.9 IOHC Clock Gating
  NBIO_IOHC_POWER_GATING_TBL         // 14.1.4.10 IOHC Power Gating
  NBIO_IOHC_ARBITRATION_TBL          // 14.1.4.11 IOHC Arbitration Control
  NBIO_IOAPIC_CLOCK_GATING_TBL       // 14.2.3.1 IOAPIC Clock Gating and Power Management
  NBIO_IOAPIC_INTR_ROUTING_TBL       // 14.2.3.2 IOAPIC Initialization:  Table 168: Recommended Interrupt Routing and Swizzling
  NBIO_NBIF_DEVICE_CFG_TBL           // 14.6.9.1 nBIF Configurations
  NBIO_NBIF_ATC_ATS_TBL              // 14.6.9.4.3 ATC/ATS Support
  NBIO_NBIF_DEVICE_INTERRUPT_TBL     // 14.6.9.5.1 Endpoint function interrupt enable
  NBIO_NBIF_DMA_ARBITRATION_TBL      // 14.6.9.5.2 DMA Arbitration
  NBIO_NBIF_DISABLE_IGPU_TBL         // 14.6.9.5.3 Dummy F0
  NBIO_NBIF_ATOMIC_TBL               // 14.6.9.5.5 Atomic
  NBIO_NBIF_PERF_REG_TBL             // 14.6.9.5.6 PerfReg Final Setting
  NBIO_NBIF_DISABLE_LTR_TBL          // Disable LTR
  NBIO_NBIF_DISABLE_AER_TBL          // Disable AER
  NBIO_NBIF_DISABLE_FLR_ATOMIC_TBL   // Disable FLR & Atomic
  NBIO_NBIF_CLOCK_GATING_TBL         // 14.6.9.6.1 Clock Gating Feature
  NBIO_NBIF_POWER_GATING_TBL         // 14.6.9.6.2 Power Gating Feature
  NBIO_NBIF_SHUB_POWER_GATING_TBL    // 14.6.9.6.2 Syshub Power Gating Feature
  NBIO_WA_ACP_SUB_CLASS_TBL          // ACP Configuration Sub Class
  NBIO_WA_PCI_CV_TEST_TBL            // PCIe CV test
  NBIO_WA_CPL_TIMEOUT_TBL            // Enable Completion Timeout feature
  NBIO_WA_RIOMMU_DIS_TIMEOUT_TBL     // rIOMMU, disable timeout, 25MHz REFCLK, 31 (0x1F) invalidations limit
  NBIO_IOHC_POISON_ACTION_TBL        // IOHC::POISON_ACTION_CONTROL
  NBIO_WA_RIOMMU_TW_COHERENT_TBL     // rIOMMU TW Coherent bit
  NBIO_WA_PME_TURNOFF_MODE_TBL       // PME_TURNOFF_MODE
  NBIO_NBIF_FIRST_VF_OFFSET_TBL      // nBIF IOV First VF offset
  NBIO_WA_RSMU_MASTER_MESSAGE_SEND_ENABLE_TBL // RSMU_MASTER_MESSAGE_SEND_ENABLE
  NBIO_NBIF_HSP_CLOCK_DS_TBL         // nBIF Hsp Clock Deep Sleep
  NBIO_NBIF_OBFF_CTRL_TBL            // nBIF DAM/OBFF Control Table
  NBIO_SYSHUB_NGDC_MGCG_CTR_CLOCK_GATING_TBL // SYSHUB NGDC MGCG Clock Gating
  NBIO_SYSHUB_MGCG_CTRL_HSP_CLOCK_GATING_TBL // SYSHUB MGCG HSP Clock Gating
  NBIO_SION_CLIENT_TBL               // IOAGR::IOAGR_SION_Client, IOHC::IOHC_SION_CLIENT
  NBIO_ATHUB_MISC_CNTL_TBL           // ATHUB Misc Control
  NBIO_NBIF_ACS_CNTL_TBL             // ACS Control
  GNB_ENTRY_TERMINATE
};



