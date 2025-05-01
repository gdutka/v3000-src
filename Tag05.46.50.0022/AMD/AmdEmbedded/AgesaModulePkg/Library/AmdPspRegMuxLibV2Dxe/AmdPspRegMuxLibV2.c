/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include <Library/PciLib.h>
#include <Library/AmdPspMmioLib.h>
#include <Library/AmdPspRegMuxLibV2.h>
#include <Library/AmdSocBaseLib.h>
#include <Filecode.h>
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_AMDPSPREGMUXLIBV2DXE_AMDPSPREGMUXLIBV2_FILECODE
#define NBMSIC_SMN_BASE                     0x13B10000ul                     ///< Define the SMN Base address for NB MISC register
#define MTS_NBMISC_SMN_BASE                 0x13E10000ul                     ///< Define the SMN Base address for MTS NB MISC register
#define NBMSIC_PSP_BASE_ADDR_LO_OFFSET      0x2E0                            ///< Define the offset of PSP_BASE_ADDR_LO in NB MISC block
#define NB_SMN_INDEX_2_PCI_ADDR             (MAKE_SBDFO (0, 0, 0, 0, 0xB8))  ///< PCI Addr of NB_SMN_INDEX_2
#define NB_SMN_DATA_2_PCI_ADDR              (MAKE_SBDFO (0, 0, 0, 0, 0xBC))  ///< PCI Addr of NB_SMN_DATA_2
#define PSP_BAR_SIZE                        0x100000ul                       ///< Size of PSP BAR
#define C2P_REG_OFFSET                      0x10500ul                        ///< BIOS to PSP mailbox Register SMN offset
#define BIOS_MUTEX_OFFSET                   (23 * 4)                         ///< BIOS mutex: BIOS sets this to non-zero requesting the mutex; clears this register when releases the mutex;
#define PSP_MUTEX_OFFSET                    (24 * 4)                         ///< PSP sets this to non-zero requesting the mutex; clears this register when releases the mutex;
#define MUTEX_TURN_OFFSET                   (25 * 4)                         ///< Turn registers as it is: BIOS sets this to 1 when requests the mutes; PSP sets it to 0 when PSP requests the mutex

BOOLEAN                     mRomArmor2Or3Enabled;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
  Return the PspMMIO MMIO location

  @param[in] PspMmioBase Pointer to Psp MMIO address

  @retval BOOLEAN  0: Error, 1 Success
**/
BOOLEAN
GetPspMmioBaseAddress (
  IN OUT   UINT32 *PspMmioBase
  )
{
  UINT32    Value32;
  UINTN     PciAddress;
  UINT32    SmnBase;

  *PspMmioBase = 0;

  SmnBase = NBMSIC_SMN_BASE;
  if (SocFamilyIdentificationCheck (F17_MTS_RAW_ID) || SocFamilyIdentificationCheck (F17_SSP_RAW_ID) || SocFamilyIdentificationCheck (F19_VMR_RAW_ID)) {
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

/**
  Library constructor for the AMD PSP Register Mutex Library V2 instance.

  @param ImageHandle The ImageHandle of the driver consuming the AmdPspRegMuxLibV2
  @param SystemTable Pointer to the EFI System Table

  @return EFI_SUCCESS The library constructor completed execution
**/
EFI_STATUS EFIAPI AmdPspRegMuxLibV2Constructor (
){
  UINT8 RomArmorSelection = PcdGet8 (PcdAmdPspRomArmorSelection);
  mRomArmor2Or3Enabled = (RomArmorSelection == 2 || RomArmorSelection == 3);
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 * Acquire the Mutex for access PSP,X86 co-accessed register
 * Call this routine before access certain registers, especially for SMI registers
 *
 */
VOID
AcquirePspAccRegMutex ()
{
  UINT32 PspMmioBase;


  //When RA2 enabled, PSP FW does not expect BIOS touch any PSP hardware mutex register
  if (mRomArmor2Or3Enabled) {
    return;
  }

  PspMmioBase = 0;
  if (GetPspMmioBaseAddress (&PspMmioBase)) {
    PspMmioWrite (C2P_REG_OFFSET + BIOS_MUTEX_OFFSET, 1);
    PspMmioWrite (C2P_REG_OFFSET + MUTEX_TURN_OFFSET, 1);
    //Wait till PSP FW release the mutex
    while ((PspMmioRead (C2P_REG_OFFSET + PSP_MUTEX_OFFSET) == 1) &&
           (PspMmioRead (C2P_REG_OFFSET + MUTEX_TURN_OFFSET) == 1)) {
      ;
    }
  }
}
/**
 * Release the Mutex for access PSP,X86 co-accessed register
 * Call this routine after access certain registers, especially for SMI registers
 *
 */
VOID
ReleasePspAccRegMutex ()
{
  UINT32 PspMmioBase;

  //When RA2 enabled, PSP FW does not expect BIOS touch any PSP hardware mutex register
  if (mRomArmor2Or3Enabled) {
    return;
  }

  PspMmioBase = 0;
  if (GetPspMmioBaseAddress (&PspMmioBase)) {
    PspMmioWrite (C2P_REG_OFFSET + BIOS_MUTEX_OFFSET, 0);
  }
}
