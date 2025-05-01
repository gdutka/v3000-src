/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP Base Library
 *
 * Contains interface to the PSP library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include <Base.h>
#include "AGESA.h"
#include "Filecode.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/AmdPspCommonLib.h>
#include <Library/CcxRolesLib.h>
#include <Library/FabricResourceManagerLib.h>
#include <Library/PciLib.h>
#include <Library/AmdPspMmioLib.h>
#include <Library/AmdPspRegMuxLibV2.h>

#define FILECODE LIBRARY_AMDPSPBASELIBV2_AMDPSPBASELIBV2_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
///< Define the SMN Base address for NB MISC register
#define NBMSIC_SMN_BASE                     0x13B10000ul
///< Define the SMN Base address for MTS NB MISC register
#define MTS_NBMISC_SMN_BASE                 0x13E10000ul
///< Define the offset of PSP_BASE_ADDR_LO in NB MISC block
#define NBMSIC_PSP_BASE_ADDR_LO_OFFSET      0x2E0
///< Define the offset of PSP_BASE_ADDR_HI in NB MISC block
#define NBMSIC_PSP_BASE_ADDR_HI_OFFSET      0x2E4
#define PSP_BAR_SIZE                        0x100000ul             ///< Size of PSP BAR
#define C2P_REG_OFFSET                      0x10500ul              ///< BIOS to PSP mailbox Register SMN offset
#define BIOS_MAILBOX_BASE                   (28 * 4)               ///< BIOS to PSP Mailbox base offset on PCIe BAR

#define NB_SMN_INDEX_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0, 0, 0xB8)) ///< PCI Addr of NB_SMN_INDEX_2
#define NB_SMN_DATA_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0, 0, 0xBC))  ///< PCI Addr of NB_SMN_DATA_2
///< BIOS mutex: BIOS sets this to non-zero requesting the mutex; clears this register when releases the mutex;
#define BIOS_MUTEX_OFFSET    (23 * 4)
///< PSP sets this to non-zero requesting the mutex; clears this register when releases the mutex;
#define PSP_MUTEX_OFFSET     (24 * 4)
///< Turn registers as it is: BIOS sets this to 1 when requests the mutes; PSP sets it to 0 when PSP requests the mutex
#define MUTEX_TURN_OFFSET    (25 * 4)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */



/**
 * @brief Woker function, implemented in assembly
 *
 * @param Rand
 * @return BOOLEAN
 */
BOOLEAN
EFIAPI
InternalX86RdSeed32 (
  OUT     UINT32                    *Rand
  );

/**
  Check if PSP device is present

  @retval BOOLEAN  0: PSP Disabled, 1: PSP Enabled

**/
BOOLEAN
EFIAPI
CheckPspDevicePresentV2 (
  VOID
  )
{
  return (TRUE);
}

/**
  Check PSP Platform Seucre Enable State
  HVB & Secure S3 (Resume vector set to Dram, & core content will restore by uCode)
  will be applied if Psp Plaform Secure is enabled

  @retval BOOLEAN  0: PSP Platform Secure Disabled, 1: PSP Platform Secure  Enabled

**/
BOOLEAN
CheckPspPlatformSecureEnableV2 (
  VOID
  )
{
  ///@todo Add code to read SMU FUSE shadow register
  return (TRUE);
}

/**
  Return the PspMbox MMIO location


  @retval EFI_STATUS  0: Success, NonZero Error

**/
BOOLEAN
GetPspMboxLocation (
  IN OUT   PSP_MBOX_V2 **PspMbox
  )
{
  UINT32               PspMmioBase;

  if (GetPspMmioBase (&PspMmioBase) == FALSE) {
    return FALSE;
  }

  *PspMbox = (PSP_MBOX_V2 *) (UINTN) (PspMmioBase + C2P_REG_OFFSET + BIOS_MAILBOX_BASE);
  return TRUE;
}

/**
  Check PSP Recovery Flag
  Target will set Recovery flag if some PSP entry point by PSP directory has been corrupted.

  @retval BOOLEAN  0: Recovery Flag is cleared, 1: Recovery Flag has been set

**/
BOOLEAN
CheckPspRecoveryFlagV2 (
  VOID
  )
{
  PSP_MBOX_V2 *PspMbox;

  PspMbox = NULL;

  if (PcdGetBool (PcdAmdPspRecoveryFlagDetectEnable) == FALSE) {
    return FALSE;
  }
  //Init PSP MMIO
  PspBarInitEarlyV2 ();
  //NOTE, GetPspMboxLocation is reading the address from index will have the potential multiple core racinng issue
  GetPspMboxLocation (&PspMbox);

  ASSERT (PspMbox->Cmd.Value != 0xFFFFFFFF);
  return (BOOLEAN) (PspMbox->Cmd.Field.Recovery);
}

VOID
PspBarInitEarlyV2 (
  VOID
  )
{
  UINT32                 Value32;
  UINT64                 PspMmioBase;
  UINTN                  PciAddress;
  UINT64                 Length;
  FABRIC_TARGET          MmioTarget;
  EFI_STATUS             Status;
  FABRIC_MMIO_ATTRIBUTE  Attributes;
  UINT32                 SmnBase;
  UINT8                  RbNumber;

  SmnBase = 0;
  RbNumber = 0;
  if ((CcxIsBsp (NULL) == FALSE)) {
    return;
  }
  //Check if PSP BAR has been assigned, if not do the PSP BAR initialation
  if (GetPspMmioBase (&Value32) == FALSE) {
    SmnBase = NBMSIC_SMN_BASE;
    if (SocFamilyIdentificationCheck (F17_MTS_RAW_ID) || SocFamilyIdentificationCheck (F19_VMR_RAW_ID)) {
      SmnBase = MTS_NBMISC_SMN_BASE;
      RbNumber = 0;
    } else if (SocFamilyIdentificationCheck (F17_SSP_RAW_ID)) {
      SmnBase = MTS_NBMISC_SMN_BASE;  //SSP uses same address as MTS
      RbNumber = 3;
    }
    //Allocate MMIO Region from MMIO manager
    Length = PSP_BAR_SIZE;
    MmioTarget.TgtType = TARGET_RB;
    MmioTarget.SocketNum = 0;
    MmioTarget.RbNum = RbNumber;
    Attributes.ReadEnable = 1;
    Attributes.WriteEnable = 1;
    Attributes.NonPosted = 0;
    Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;
    PspMmioBase  = 0;
    Status = FabricAllocateMmio (&PspMmioBase, &Length, ALIGN_1M, MmioTarget, &Attributes);
    ASSERT (Status == EFI_SUCCESS );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PspBarInitEarlyV2 Allocate MMIO Fail\n");
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("PspBarInitEarlyV2 Allocate MMIO @0x%lx\n", PspMmioBase);
    }
    //Set PSP BASE Address in NBMISC, and enable lock the MMIO
    PciAddress = NB_SMN_INDEX_2_PCI_ADDR;
    Value32 = SmnBase + NBMSIC_PSP_BASE_ADDR_LO_OFFSET;
    PciWrite32 (PciAddress, Value32);
    PciAddress = NB_SMN_DATA_2_PCI_ADDR;
    Value32 = (UINT32) (PspMmioBase | (BIT0 + BIT8));
    PciWrite32 (PciAddress, Value32);

    PciAddress = NB_SMN_INDEX_2_PCI_ADDR;
    Value32 = SmnBase + NBMSIC_PSP_BASE_ADDR_HI_OFFSET;
    PciWrite32 (PciAddress, Value32);
    PciAddress = NB_SMN_DATA_2_PCI_ADDR;
    Value32 = (UINT32) RShiftU64 (PspMmioBase, 32);
    PciWrite32 (PciAddress, Value32);
  }
}

/**
  Return the PspMMIO MMIO location

  @param[in] PspMmioBase Pointer to Psp MMIO address

  @retval BOOLEAN  0: Error, 1 Success
**/
BOOLEAN
GetPspMmioBase (
  IN OUT   UINT32 *PspMmioBase
  )
{
  UINT32    Value32;
  UINTN     PciAddress;
  UINT32    SmnBase;

  *PspMmioBase = 0;

  SmnBase = NBMSIC_SMN_BASE;
  if (SocFamilyIdentificationCheck (F17_MTS_RAW_ID) ||
      SocFamilyIdentificationCheck (F17_SSP_RAW_ID) ||
      SocFamilyIdentificationCheck (F19_VMR_RAW_ID)) {
    SmnBase = MTS_NBMISC_SMN_BASE;
  }

  PciAddress = NB_SMN_INDEX_2_PCI_ADDR;
  Value32 = SmnBase + NBMSIC_PSP_BASE_ADDR_LO_OFFSET;
  PciWrite32 (PciAddress, Value32);
  PciAddress = NB_SMN_DATA_2_PCI_ADDR;
  Value32 = PciRead32 (PciAddress);
  //Mask out the lower bits
  Value32 &= 0xFFF00000;

  if (Value32 == 0) {
    return (FALSE);
  }

  *PspMmioBase = Value32;
  return (TRUE);
}

///@todo remove below FCH definition when it has been declared by FCH module
#define ACPI_MMIO_BASE  0xFED80000ul
#define PMIO_BASE       0x300   // DWORD
#define FCH_PMIOA_REG64          0x64         // AcpiPmTmrBlk
/**
 * PSP Time out function with conditioner
 * The routine will exit in two conditions:
 * 1. Time out of input uSec
 * 2. Conditioner function return TRUE
 *
 * @param[in] uSec         Timer in microseconds
 * @param[in] Conditioner  Function for check condition of exit the timeout routine
 * @param[in] Context      Conditioner function context
 *
 * @retval BOOLEAN  FALSE: Timer exceed, TRUE: condition match
 */
BOOLEAN
EFIAPI
PspLibTimeOutV2 (
  IN       UINT64              uSec,
  IN       FP_CONDITIONER      Conditioner,
  IN       VOID                *Context
  )
{
  UINT16 timerAddr;
  UINT32 startTime;
  UINT64 elapsedTime;

  // Return immediately if condition already match
  if (Conditioner (Context) == TRUE) {
    return TRUE;
  }

  //Check if infinitely flag set
  if (uSec == PSPLIB_WAIT_INFINITELY) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Wait infinitely ..\n");
    while (Conditioner (Context) == FALSE) {
      ;
    }
    return TRUE;
  }

  //Check availibitly of ACPI timer
  LibAmdMemRead (AccessWidth16, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG64), &timerAddr, NULL);
  //Using IO delay, if ACPI timer is not available, it is not accurate, since the conditioner function will take time
  if ( timerAddr == 0 ) {
    uSec = DivU64x32 (uSec, 2);
    while ( uSec != 0) {
      if (Conditioner (Context) == TRUE) {
        return TRUE;
      }
      LibAmdIoRead (AccessWidth8, 0xEB, (UINT8*)&elapsedTime, NULL);
      uSec--;
    }
  } else {
    LibAmdIoRead (AccessWidth32, timerAddr, &startTime, NULL);
    elapsedTime = 0;
    for ( ;; ) {
      LibAmdIoRead (AccessWidth32, timerAddr, &elapsedTime, NULL);
      if ( elapsedTime <= startTime ) {
        elapsedTime = elapsedTime + 0xFFFFFFFFul - startTime;
      } else {
        elapsedTime = elapsedTime - startTime;
      }

      if (Conditioner (Context) == TRUE) {
        return TRUE;
      }

      if (DivU64x32 ( MultU64x32 (elapsedTime, 28), 100 ) > uSec ) {
        break;
      }
    }
  }
  return FALSE;
}

BOOLEAN
EFIAPI
GetFtpmControlAreaV2 (
  IN OUT   VOID **FtpmControlArea
  )
{
  UINT32               PspMmioBase;

  if (GetPspMmioBase (&PspMmioBase) == FALSE) {
    return FALSE;
  }

  *FtpmControlArea = (VOID *) (UINTN) (PspMmioBase + C2P_REG_OFFSET);
  return TRUE;
}

VOID
EFIAPI
SwitchPspMmioDecodeV2 (
  IN       BOOLEAN SwitchFlag,
  IN OUT   UINT32 *RegisterCopy
  )
{
}


/**
 * Acquire the Mutex for access PSP,X86 co-accessed register
 * Call this routine before access SMIx98 & SMIxA8
 *
 */
VOID
AcquirePspSmiRegMutexV2 (
  VOID
  )
{
  AcquirePspAccRegMutex ();
}

/**
 * Release the Mutex for access PSP,X86 co-accessed register
 * Call this routine after access SMIx98 & SMIxA8
 *
 */
VOID
ReleasePspSmiRegMutexV2 (
  VOID
  )
{
  ReleasePspAccRegMutex ();
}

/**
 * Get PsbHstiStatus from C2P register
 *
 */
BOOLEAN
GetPsbStatus2 (
  PSB_STATUS_2 *PsbStatus2
  )
{
  UINT32            PspMmioBase;
  UINT32            RegValue;

  PspMmioBase = 0;
  if (GetPspMmioBase (&PspMmioBase) == FALSE) {
    return FALSE;
  }
  // MP0_C2PMSG_37 register address is different between family
  if (SocFamilyIdentificationCheck (F17_ZP_RAW_ID)) {
    RegValue = PspMmioRead (0x10594);
  } else {
    RegValue = PspMmioRead (0x10994);
  }
  ASSERT (sizeof (PSB_STATUS_2) == sizeof (UINT32));
  CopyMem (PsbStatus2, &RegValue, sizeof (PSB_STATUS_2));
  return TRUE;
}

/**
 * Get PsbHstiStatus from C2P register
 *
 */
BOOLEAN
GetPsbHstiStatus1 (
  PSB_HSTI_STATUS_1 *PsbHstiStatus1
  )
{
  UINT32            RegValue;
  UINT32            PspMmioBase;

  PspMmioBase = 0;
  if (GetPspMmioBase (&PspMmioBase) == FALSE) {
    return FALSE;
  }
  // MP0_C2PMSG_38 register address is different between family
  if (SocFamilyIdentificationCheck (F17_ZP_RAW_ID)) {
    RegValue = PspMmioRead (0x10598);
  } else {
    RegValue = PspMmioRead (0x10998);
  }

  ASSERT (sizeof (PSB_HSTI_STATUS_1) == sizeof (UINT32));
  CopyMem (PsbHstiStatus1, &RegValue, sizeof (PSB_HSTI_STATUS_1));
  return TRUE;
}


/**
 * @brief Loads the destination register with a hardware-generated random "seed" value
 *
 * @param Seed  buffer to store 32bits seed value
 * @return BOOLEAN TRUE: RDSEED succeed FALSE: error
 */
BOOLEAN
EFIAPI
AsmRdSeed32 (
  OUT     UINT32                    *Seed
  )
{
  ASSERT (Seed != NULL);
  return InternalX86RdSeed32 (Seed);
}

/**
 * @brief Dump RDRAND, RDSEED related information
 *
 * @return VOID
 */
VOID
DumpRdInstructionInfo (
  )
{
  UINT32 Value32;
  UINT64 Value64;
  UINT32 RdValue1;
  UINT32 RdValue2;
  BOOLEAN RdStatus1;

  //Print RdInstruction capability from CPUID
  // CPUID_Fn00000001_ECX [Feature Identifiers] (Core::X86::Cpuid::FeatureIdEcx)
  // BIT30 | RDRAND. Read-only. Reset: Fixed,1. RDRAND instruction support
  AsmCpuid (0x00000001, NULL, NULL, &Value32, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("CPUID_Fn00000001_ECX_RDRAND %a\n", (Value32 & BIT30) ? "Supported" : "Unsupported");
  // CPUID_Fn00000007_EBX_x00 [Structured Extended Feature Identifiers]
  // BIT18 | RDSEED. Read-only. Reset: Fixed,1. RDSEED is present.
  AsmCpuid (0x00000007, NULL, &Value32, NULL, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("CPUID_Fn00000007_EBX_RDSEED %a\n", (Value32 & BIT18)? "Supported" : "Unsupported");
  //Print RdInstruction enable status from MSR
  // MSRC001_1004 [CPUID Features for CPUID Fn00000001_E[C,D]X] (Core::X86::Msr::CPUID_Features)
  // BIT62 RDRAND
  Value64 = AsmReadMsr64 (0xC0011004);
  IDS_HDT_CONSOLE_PSP_TRACE ("MSRC001_1004_RDRAND %a\n", (Value64 & BIT62) ? "Enabled" : "Disabled");
  //MSRC001_1002 [CPUID Features for CPUID Fn00000007_E[A,B]X] (Core::X86::Msr::CPUID_7_Features)
  // BIT18 RDSEED
  Value64 = AsmReadMsr64 (0xC0011002);
  IDS_HDT_CONSOLE_PSP_TRACE ("MSRC001_1002_RDSEED %a\n", (Value64 & BIT18) ? "Enabled" : "Disabled");
  //Verify RdInstruction function by generate random value twice and check if identical


  RdValue1 = 0xFFFFFFFFul;
  RdValue2 = 0xFFFFFFFFul;
  RdStatus1 = AsmRdRand32 (&RdValue1);

  IDS_HDT_CONSOLE_PSP_TRACE ("RDRAND verify: RdValue1:0x%x RdValue2:0x%x\n", RdValue1, RdValue2);
  if (((RdStatus1 & RdStatus1) == FALSE) || (RdValue1 == 0xFFFFFFFFul) || (RdValue1 == RdValue2)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("%a\n", "FAIL");
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("%a\n", "PASS");
  }

  RdValue1 = 0xFFFFFFFFul;
  RdValue2 = 0xFFFFFFFFul;
  RdStatus1 = AsmRdSeed32 (&RdValue1);
  IDS_HDT_CONSOLE_PSP_TRACE ("RDSEED verify: RdValue1:0x%x RdValue2:0x%x\n", RdValue1, RdValue2);
  if (((RdStatus1 & RdStatus1) == FALSE) || (RdValue1 == 0xFFFFFFFFul) || (RdValue1 == RdValue2)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("%a\n", "FAIL");
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("%a\n", "PASS");
  }

}