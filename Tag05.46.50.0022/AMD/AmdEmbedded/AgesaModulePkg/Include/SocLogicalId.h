/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _SOC_LOGICAL_ID_H_
#define _SOC_LOGICAL_ID_H_
#pragma pack (push, 1)

/*
 *--------------------------------------------------------------
 *
 *                      M O D U L E S    U S E D
 *
 *---------------------------------------------------------------
 */

/*
 *--------------------------------------------------------------
 *
 *                      D E F I N I T I O N S  /  M A C R O S
 *
 *---------------------------------------------------------------
 */
/// SOC logical ID structure
typedef struct _SOC_LOGICAL_ID {
  IN OUT   UINT32 Family;             ///< Indicates logical ID Family
  IN OUT   UINT16 Revision;           ///< Indicates logical ID Revision
} SOC_LOGICAL_ID;

/// Core logical ID structure
typedef struct _CORE_LOGICAL_ID {
  IN OUT   UINT32 CoreFamily;         ///< Indicates logical ID Core (silicon) Family
  IN OUT   UINT16 CoreRevision;       ///< Indicates logical ID Core (silicon) Revision
} CORE_LOGICAL_ID;

// UNKNOWN equates
// -------------------------------------
// This define should be equal to the total number of families
// in the cpuFamily enum.
#define MAX_CPU_FAMILIES    31
#define MAX_CPU_REVISIONS   15    // Max Cpu Revisions Per Family
// Family Unknown
#define AMD_FAMILY_UNKNOWN   0x80000000ul
#define AMD_REVISION_UNKNOWN 0x8000u

// --------------------------------------------------------------------------
//   S O C   F A M I L Y & R E V I S I O N S
// --------------------------------------------------------------------------

// SOC_LOGICAL_ID.Family equates
// -------------------------------------

// Family 15h CZ equates
#define AMD_FAMILY_15_CZ  0x00000001ul
#define AMD_FAMILY_CZ     (AMD_FAMILY_15_CZ)

#define AMD_FAMILY_15     (AMD_FAMILY_15_CZ)

// Family 17h ZP equates
#define AMD_FAMILY_17_ZP  0x00000002ul
#define AMD_FAMILY_ZP     (AMD_FAMILY_17_ZP)

// Family 17h RV equates
#define AMD_FAMILY_17_RV  0x00000004ul
#define AMD_FAMILY_RV     (AMD_FAMILY_17_RV)

// Family 17h RV2 equates
#define AMD_FAMILY_17_RV2 0x00000008ul
#define AMD_FAMILY_RV2    (AMD_FAMILY_17_RV2)

// Family 17h SSP equates
#define AMD_FAMILY_17_SSP 0x00000010ul
#define AMD_FAMILY_SSP    (AMD_FAMILY_17_SSP)

// Family 17h PIC equates
#define AMD_FAMILY_17_PIC 0x00000020ul
#define AMD_FAMILY_PIC    (AMD_FAMILY_17_PIC)
#define AMD_FAMILY_PCO    (AMD_FAMILY_17_PIC)

// Family 17h MTS equates
#define AMD_FAMILY_17_MTS 0x0000004ul
#define AMD_FAMILY_MTS    (AMD_FAMILY_17_MTS)

// Family 17h RN equates
#define AMD_FAMILY_17_RN  0x00000040ul
#define AMD_FAMILY_RN     (AMD_FAMILY_17_RN)

// Family 17h FF3 equates
#define AMD_FAMILY_17_FF3  0x00000080ul
#define AMD_FAMILY_FF3     (AMD_FAMILY_17_FF3)

#define AMD_FAMILY_17     (AMD_FAMILY_ZP | AMD_FAMILY_RV | AMD_FAMILY_RV2 | AMD_FAMILY_PIC | AMD_FAMILY_SSP | AMD_FAMILY_MTS | AMD_FAMILY_RN | AMD_FAMILY_FF3)

// Family 19h GN equates
#define AMD_FAMILY_19_GN  0x00000080ul
#define AMD_FAMILY_GN     (AMD_FAMILY_19_GN)

// Family 19h VMR equates
#define AMD_FAMILY_19_VMR 0x00000100ul
#define AMD_FAMILY_VMR    (AMD_FAMILY_19_VMR)

// Family 19h CZN equates
#define AMD_FAMILY_19_CZN 0x00000200ul
#define AMD_FAMILY_CZN    (AMD_FAMILY_19_CZN)

// Family 19h BA equates
#define AMD_FAMILY_19_BA  0x00000400ul
#define AMD_FAMILY_BA     (AMD_FAMILY_19_BA)

// Family 19h RMB equates
#define AMD_FAMILY_19_RMB  0x00000800ul
#define AMD_FAMILY_RMB    (AMD_FAMILY_19_RMB)

// Family 19h RS equates
#define AMD_FAMILY_19_RS  0x00001000ul
#define AMD_FAMILY_RS     (AMD_FAMILY_19_RS)

// Family 19h PHX equates
#define AMD_FAMILY_19_PHX 0x00002000ul
#define AMD_FAMILY_PHX    (AMD_FAMILY_19_PHX)

// Family 19h RPL equates
#define AMD_FAMILY_19_RPL 0x00004000ul
#define AMD_FAMILY_RPL    (AMD_FAMILY_19_RPL)

// Family 19h RSDN equates
#define AMD_FAMILY_19_RSDN 0x00008000ul
#define AMD_FAMILY_RSDN   (AMD_FAMILY_19_RSDN)

// Family 19h RSDN equates
#define AMD_FAMILY_19_MI3 0x00010000ul
#define AMD_FAMILY_MI3    (AMD_FAMILY_19_MI3)

#define AMD_FAMILY_19     ( AMD_FAMILY_GN     | AMD_FAMILY_19_BA  | AMD_FAMILY_VMR      | \
                            AMD_FAMILY_CZN    | AMD_FAMILY_RMB    | AMD_FAMILY_19_RS    | \
                            AMD_FAMILY_PHX    | AMD_FAMILY_RPL    | AMD_FAMILY_19_RSDN  | \
                            AMD_FAMILY_19_MI3 )

// Family 15h SOC_LOGICAL_ID.Revision equates
// -------------------------------------
  // Family 15h CZ steppings
#define AMD_REV_F15_CZ_A0    0x0001u
#define AMD_REV_F15_CZ_A1    0x0002u
#define AMD_REV_F15_BR_A1    0x0004u

#define AMD_REV_F15_BR_Ax    (AMD_REV_F15_BR_A1)
#define AMD_REV_F15_BR_ALL   (AMD_REV_F15_BR_Ax)

#define AMD_REV_F15_CZ_Ax    (AMD_REV_F15_CZ_A0 | AMD_REV_F15_CZ_A1 | AMD_REV_F15_BR_Ax)
#define AMD_REV_F15_CZ_ALL   (AMD_REV_F15_CZ_Ax | AMD_REV_F15_BR_ALL)

#define AMD_REV_F15_ALL      (AMD_REV_F15_CZ_ALL)

// Family 17h SOC_LOGICAL_ID.Revision equates
// -------------------------------------
  // Family 17h ZP core steppings
#define AMD_REV_F17_ZP_A0    0x0001u
#define AMD_REV_F17_ZP_B0    0x0002u
#define AMD_REV_F17_ZP_B1    0x0004u
#define AMD_REV_F17_ZP_B2    0x0008u
#define AMD_REV_F17_PR_B2    0x0010u
#define AMD_REV_F17_PR_B3    0x0020u
#define AMD_REV_F17_ZP_B3    0x0040u

#define AMD_REV_F17_ZP_Ax    (AMD_REV_F17_ZP_A0)
#define AMD_REV_F17_ZP_Bx    (AMD_REV_F17_ZP_B0 | AMD_REV_F17_ZP_B1 | AMD_REV_F17_ZP_B2 | AMD_REV_F17_ZP_B3)
#define AMD_REV_F17_PR_Bx    (AMD_REV_F17_PR_B2 | AMD_REV_F17_PR_B3)
#define AMD_REV_F17_ZP_PR_Bx (AMD_REV_F17_ZP_Bx | AMD_REV_F17_PR_Bx)

#define AMD_REV_F17_ZP_ALL   (AMD_REV_F17_ZP_Ax | AMD_REV_F17_ZP_Bx)
#define AMD_REV_F17_PR_ALL   (AMD_REV_F17_PR_Bx)
#define AMD_REV_F17_ZP_PR_ALL (AMD_REV_F17_ZP_ALL | AMD_REV_F17_PR_ALL)

  // Family 17h ZP derived steppings
#define AMD_REV_F17_ZP_LT_B1  (AMD_REV_F17_ZP_Ax | AMD_REV_F17_ZP_B0)
#define AMD_REV_F17_ZP_GTE_B1 (AMD_REV_F17_ZP_ALL & ~AMD_REV_F17_ZP_LT_B1)
#define AMD_REV_F17_ZP_LTE_B1 (AMD_REV_F17_ZP_Ax | AMD_REV_F17_ZP_B0 | AMD_REV_F17_ZP_B1)
#define AMD_REV_F17_ZP_PR_GTE_B1 ((AMD_REV_F17_ZP_ALL & ~AMD_REV_F17_ZP_LT_B1) | AMD_REV_F17_PR_ALL)

#define AMD_REV_F17_ZP_LT_B2  (AMD_REV_F17_ZP_Ax | AMD_REV_F17_ZP_B0 | AMD_REV_F17_ZP_B1)
#define AMD_REV_F17_ZP_GTE_B2 (AMD_REV_F17_ZP_ALL & ~AMD_REV_F17_ZP_LT_B2)
#define AMD_REV_F17_ZP_PR_GTE_B2 ((AMD_REV_F17_ZP_ALL & ~AMD_REV_F17_ZP_LT_B2) | AMD_REV_F17_PR_ALL)

  // Family 17h RV steppings
#define AMD_REV_F17_RV_A0    0x0001u
#define AMD_REV_F17_RV_B0    0x0002u
#define AMD_REV_F17_RV_B1    0x0004u

#define AMD_REV_F17_RV_Ax    (AMD_REV_F17_RV_A0)
#define AMD_REV_F17_RV_Bx    (AMD_REV_F17_RV_B0 | AMD_REV_F17_RV_B1)

#define AMD_REV_F17_RV_LT_B0  (AMD_REV_F17_RV_Ax)
#define AMD_REV_F17_RV_GTE_B0 (AMD_REV_F17_RV_ALL & ~AMD_REV_F17_RV_LT_B0)

#define AMD_REV_F17_RV_ALL   (AMD_REV_F17_RV_Ax | AMD_REV_F17_RV_Bx)

  // Family 17h RV2 steppings
#define AMD_REV_F17_RV2_A0   0x0001u
#define AMD_REV_F17_RV2_A1   0x0002u

#define AMD_REV_F17_RV2_Ax   (AMD_REV_F17_RV2_A0 | AMD_REV_F17_RV2_A1)

#define AMD_REV_F17_RV2_ALL  (AMD_REV_F17_RV2_Ax)

  // Family 17h SSP steppings
#define AMD_REV_F17_SSP_A0   0x0001u
#define AMD_REV_F17_SSP_B0   0x0010u

#define AMD_REV_F17_SSP_Ax   (AMD_REV_F17_SSP_A0)
#define AMD_REV_F17_SSP_Bx   (AMD_REV_F17_SSP_B0)

#define AMD_REV_F17_SSP_ALL  (AMD_REV_F17_SSP_Ax | AMD_REV_F17_SSP_Bx)

#define AMD_REV_F17_SSP_GTE_B0 (AMD_REV_F17_SSP_ALL & ~AMD_REV_F17_SSP_Ax)

  // Family 17h PIC steppings
#define AMD_REV_F17_PIC_B0   0x0001u
#define AMD_REV_F17_PIC_B1   0x0002u

#define AMD_REV_F17_PIC_Bx   (AMD_REV_F17_PIC_B0 | AMD_REV_F17_PIC_B1)

#define AMD_REV_F17_PIC_GTE_B0 (AMD_REV_F17_PIC_Bx)

#define AMD_REV_F17_PIC_ALL  (AMD_REV_F17_PIC_Bx)

  // Family 17h RN steppings
#define AMD_REV_F17_RN_A0    0x0001u

#define AMD_REV_F17_RN_Ax    (AMD_REV_F17_RN_A0)

#define AMD_REV_F17_RN_ALL   (AMD_REV_F17_RN_Ax)

  // Family 17h FF3 steppings
#define AMD_REV_F17_FF3_A0    0x0001u
#define AMD_REV_F17_FF3_A1    0x0002u
#define AMD_REV_F17_FF3_A2    0x0004u

#define AMD_REV_F17_FF3_Ax    (AMD_REV_F17_FF3_A0 | AMD_REV_F17_FF3_A1 | AMD_REV_F17_FF3_A2)

#define AMD_REV_F17_FF3_ALL   (AMD_REV_F17_FF3_Ax)

#define AMD_REV_F17_MR_A0    0x0081u
#define AMD_REV_F17_MR_A1    0x0082u
#define AMD_REV_F17_MR_A2    0x0084u

#define AMD_REV_F17_MR_Ax    (AMD_REV_F17_MR_A0 | AMD_REV_F17_MR_A1 | AMD_REV_F17_MR_A2)

#define AMD_REV_F17_MR_ALL   (AMD_REV_F17_MR_Ax)

  // Family 17h MTS steppings
#define AMD_REV_F17_MTS_A0   0x0001u
#define AMD_REV_F17_MTS_B0   0x0010u

#define AMD_REV_F17_MTS_Ax   (AMD_REV_F17_MTS_A0)
#define AMD_REV_F17_MTS_Bx   (AMD_REV_F17_MTS_B0)

#define AMD_REV_F17_MTS_ALL  (AMD_REV_F17_MTS_Ax | AMD_REV_F17_MTS_Bx)

  // Family 19h GN steppings
#define AMD_REV_F19_GN_A0    0x0001u
#define AMD_REV_F19_GN_B0    0x0010u
#define AMD_REV_F19_GN_B1    0x0020u

#define AMD_REV_F19_GN_Ax    (AMD_REV_F19_GN_A0)
#define AMD_REV_F19_GN_Bx    (AMD_REV_F19_GN_B0 | AMD_REV_F19_GN_B1)

#define AMD_REV_F19_GN_ALL   (AMD_REV_F19_GN_Ax | AMD_REV_F19_GN_Bx)

  // Family 19h CGL steppings
#define AMD_REV_F19_CGL_B0    0x0100u
#define AMD_REV_F19_CGL_B2    0x0200u

#define AMD_REV_F19_CGL_Bx   (AMD_REV_F19_CGL_B0 | AMD_REV_F19_CGL_B2)

  // Family 19h RS steppings
#define AMD_REV_F19_RS_A0    0x0001u
#define AMD_REV_F19_RS_A1    0x0002u

#define AMD_REV_F19_RS_Ax    (AMD_REV_F19_RS_A0 | AMD_REV_F19_RS_A1)

#define AMD_REV_F19_RS_ALL   (AMD_REV_F19_RS_Ax)

  // Family 19h STP steppings
#define AMD_REV_F19_STP_A0    0x0100u

#define AMD_REV_F19_STP_Ax   (AMD_REV_F19_STP_A0)

#define AMD_REV_F19_STP_ALL  (AMD_REV_F19_STP_Ax)

  // Family 19h BA steppings
#define AMD_REV_F19_BA_A0_B0CCD   0x0001u
#define AMD_REV_F19_BA_A0_B1CCD   0x0002u
#define AMD_REV_F19_BA_A0         (AMD_REV_F19_BA_A0_B0CCD | AMD_REV_F19_BA_A0_B1CCD)

#define AMD_REV_F19_BA_Ax    (AMD_REV_F19_BA_A0)

#define AMD_REV_F19_BA_ALL   (AMD_REV_F19_BA_Ax)

  // Family 19h VMR steppings
#define AMD_REV_F19_VMR_A0   0x0001u
#define AMD_REV_F19_VMR_B0   0x0010u
#define AMD_REV_F19_VMR_B2   0x0020u

#define AMD_REV_F19_VMR_Ax   (AMD_REV_F19_VMR_A0)
#define AMD_REV_F19_VMR_Bx   (AMD_REV_F19_VMR_B0 | AMD_REV_F19_VMR_B2)

#define AMD_REV_F19_VMR_ALL  (AMD_REV_F19_VMR_Ax | AMD_REV_F19_VMR_Bx)

  // Family 19h CZN steppings
#define AMD_REV_F19_CZN_A0   0x0001u

#define AMD_REV_F19_CZN_Ax   (AMD_REV_F19_CZN_A0)

#define AMD_REV_F19_CZN_ALL  (AMD_REV_F19_CZN_Ax)

  // Family 19h RMB steppings
#define AMD_REV_F19_RMB_A0   0x0001u
#define AMD_REV_F19_RMB_B0   0x0010u
#define AMD_REV_F19_RMB_B1   0x0020u

#define AMD_REV_F19_RMB_Ax   (AMD_REV_F19_RMB_A0)
#define AMD_REV_F19_RMB_Bx   (AMD_REV_F19_RMB_B0 | AMD_REV_F19_RMB_B1)

#define AMD_REV_F19_RMB_ALL  (AMD_REV_F19_RMB_Ax | AMD_REV_F19_RMB_Bx)

  // Family 19h PHX steppings
#define AMD_REV_F19_PHX_A0   0x0001u
#define AMD_REV_F19_PHX_B0   0x0010u

#define AMD_REV_F19_PHX_Ax   (AMD_REV_F19_PHX_A0)
#define AMD_REV_F19_PHX_Bx   (AMD_REV_F19_PHX_B0)

#define AMD_REV_F19_PHX_ALL  (AMD_REV_F19_PHX_Ax | AMD_REV_F19_PHX_Bx)

  // Family 19h RPL steppings
#define AMD_REV_F19_RPL_A0   0x0001u
#define AMD_REV_F19_RPL_A1   0x0002u

#define AMD_REV_F19_RPL_Ax   (AMD_REV_F19_RPL_A0 | AMD_REV_F19_RPL_A1)

#define AMD_REV_F19_RPL_ALL  (AMD_REV_F19_RPL_Ax)

// Family 19h RSDN steppings
#define AMD_REV_F19_RSDN_A0  0x0001u

#define AMD_REV_F19_RSDN_Ax  (AMD_REV_F19_RSDN_A0)

#define AMD_REV_F19_RSDN_ALL (AMD_REV_F19_RSDN_Ax)

// --------------------------------------------------------------------------
//   C O R E   F A M I L Y & R E V I S I O N S
// --------------------------------------------------------------------------

// -------------------------------------
// CORE_LOGICAL_ID.CoreFamily equates
// -------------------------------------
  // Core Family 17h VH equates
#define AMD_CORE_FAMILY_17_VH  0x00000001ul
#define AMD_CORE_FAMILY_VH     (AMD_CORE_FAMILY_17_VH)

  // Core Family 19h Zen3 equates
#define AMD_CORE_FAMILY_19_ZEN3  0x00000002ul
#define AMD_CORE_FAMILY_ZEN3     (AMD_CORE_FAMILY_19_ZEN3)

  // Core Family 19h Zen4 equates
#define AMD_CORE_FAMILY_19_ZEN4  0x00000003ul
#define AMD_CORE_FAMILY_ZEN4     (AMD_CORE_FAMILY_19_ZEN4)

// -------------------------------------
// CORE_LOGICAL_ID.LogicalCoreRev equates
// -------------------------------------
  // Family 17h VH core steppings
#define AMD_CORE_F17_VH_A0   0x0001u
#define AMD_CORE_F17_VH_B0   0x0010u

#define AMD_CORE_F17_VH_Ax   (AMD_CORE_F17_VH_A0)
#define AMD_CORE_F17_VH_Bx   (AMD_CORE_F17_VH_B0)

#define AMD_CORE_F17_VH_ALL  (AMD_CORE_F17_VH_Ax | AMD_CORE_F17_VH_Bx)

#define AMD_CORE_F17_VH_GTE_B0 (AMD_CORE_F17_VH_ALL & ~AMD_CORE_F17_VH_Ax)

  // All
#define AMD_CORE_F17_ALL      (AMD_CORE_F17_VH_ALL)

  // Family 19h Zen3 core steppings
#define AMD_CORE_F19_ZEN3_A0   0x0001u
#define AMD_CORE_F19_ZEN3_B0   0x0010u
#define AMD_CORE_F19_ZEN3_B1   0x0020u
#define AMD_CORE_F19_ZEN3_B2   0x0040u

#define AMD_CORE_F19_ZEN3_Ax   (AMD_CORE_F19_ZEN3_A0)
#define AMD_CORE_F19_ZEN3_Bx   (AMD_CORE_F19_ZEN3_B0 | AMD_CORE_F19_ZEN3_B1 | AMD_CORE_F19_ZEN3_B2)

#define AMD_CORE_F19_ZEN3_ALL  (AMD_CORE_F19_ZEN3_Ax | AMD_CORE_F19_ZEN3_Bx)

  // Family 19h Zen4 core steppings
#define AMD_CORE_F19_ZEN4_A0   0x0100u
#define AMD_CORE_F19_ZEN4_A1   0x0200u

#define AMD_CORE_F19_ZEN4_Ax   (AMD_CORE_F19_ZEN4_A0 | AMD_CORE_F19_ZEN4_A1)

#define AMD_CORE_F19_ZEN4_ALL  (AMD_CORE_F19_ZEN4_Ax)

  // All
#define AMD_CORE_F19_ALL      (AMD_CORE_F19_ZEN3_ALL | AMD_CORE_F19_ZEN4_ALL)


#pragma pack (pop)
#endif      // _SOC_LOGICAL_ID_H_
