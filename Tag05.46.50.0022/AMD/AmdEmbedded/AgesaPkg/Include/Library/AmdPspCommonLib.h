/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file AmdPspCommonLib.h
 *
 * @brief  PSP common service prototype definition
 *
 * @details Provide generic PSP external interface accorss programs, it will call AmdPspCommonServicePpi/Protocol underneath
 *
 */

#ifndef _AMD_PSPCOMMONLIB_H_
#define _AMD_PSPCOMMONLIB_H_

#include <AmdPspDirectory.h>
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

// AMD reserved TCG Logs Event ID (0x8000~0x8FFF)
// see "TCG PC Client Platform Firmware Profile Specification" Version 1.04 section 9.4.1 for Event Types
// Category 1 (0x8000 to 0x81FF): DRTM Events, extended by PSP
// Category 2 (0x8200 to 0x83FF): SRTM Events, extended by PSP, used for PSP measurements in HSP
// Category 3 (0x8400 to 0x85FF): SRTM Events, extended by AMD BIOS
//
//
#define TCG_EVENT_BASE_AMD                                ((TCG_EVENTTYPE) 0x8000)
#define TCG_EVENT_BASE_AMD_BIOS                           (TCG_EVENT_BASE_AMD + 0x400)
#define TCG_EVENT_AMD_BIOS_TSME_MEASUREMENT               (TCG_EVENT_BASE_AMD_BIOS + 1)
/**
 * @brief Function declaration used to get Ftpm control area address
 *
 * @param FtpmControlArea  Point to address used to hold FtpmControlArea address
 *
 * @retval BOOLEAN TRUE: Succeed FALSE: Failed
 */
BOOLEAN
GetFtpmControlArea (
  IN OUT   VOID **FtpmControlArea
  );

/**
 * @brief Function declaration used to turn on/restore PSP MMIO decode, deprecated from SOC15
 *
 */
VOID
SwitchPspMmioDecode (
  IN       BOOLEAN SwitchFlag,
  IN OUT   UINT32 *RegisterCopy
  );

/**
 * @brief Function declaration used to check PSP Device present by checking fuse receipt copy, deprecated from SOC15
*/
BOOLEAN
CheckPspDevicePresent (
  VOID
  );

/**
 * @brief Function declaration used to check Ftpm related capabilities
 *
 * @details Send command to PSP FW, to get fTPM related caps, e.g. PSP_CAP_TPM_SUPPORTED
 * PSP_CAP_TPM_REQ_FACTORY_RESET PSP_CAP_FTPM_NEED_RECOVERY
 *
 * @param Caps Point to address used to hold cap
 *
 * @retval EFI_STATUS                0: Success, NonZero Error
 */
EFI_STATUS
CheckPspCaps (
  IN OUT   UINT32 *PspCaps
  );

/**
 * @brief Function declaration used to check Ftpm related capabilities
 *
 * @details Send command to PSP FW, to get fTPM related caps, e.g. PSP_CAP_TPM_SUPPORTED
 * PSP_CAP_TPM_REQ_FACTORY_RESET PSP_CAP_FTPM_NEED_RECOVERY
 *
 * @param Caps Point to address used to hold cap
 *
 * @retval EFI_STATUS                0: Success, NonZero Error
 */
EFI_STATUS
CheckFtpmCaps (
  IN OUT   UINT32 *Caps
  );

/**
  @brief This function is to get the PSP entry information for given PSP entry type.

  @param[in]   value of given PSP entry type
  @param[out]  pointer to PSP entry address
  @param[out]  pointer to PSP entry size

  @retval TRUE            The given entry type is found
  @retval FALSE           The given entry type is not found

**/
BOOLEAN
PSPEntryInfo (
  IN      UINT32                       EntryType,
  IN OUT  UINT64                      *EntryAddress,
  IN      UINT32                      *EntrySize
  );

/**
 * @brief Function declaration, the function used as exit condition check for some routine
 *
 */
typedef
BOOLEAN
(* FP_CONDITIONER) (
  IN       VOID        *Context
  );

/**
 * @brief PSP Time out function with conditioner
 * @details The routine will exit in two conditions:
 *           1. Time out of input uSec
 *           2. Conditioner function return TRUE
 *
 * @param[in] uSec         Timer in microseconds
 * @param[in] Conditioner  Function for check condition of exit the timeout routine
 * @param[in] Context      Conditioner function context
 *
 * @retval BOOLEAN  FALSE: Timer exceed, TRUE: condition match
 */
#define PSPLIB_WAIT_INFINITELY 0xFFFFFFFFL

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
 **/
BOOLEAN
PspLibTimeOut (
  IN       UINT64              uSec,
  IN       FP_CONDITIONER      Conditioner,
  IN       VOID                *Context
  );

/**
 * @brief Accuquire the ownship of some FCH SMI register
 *
 * @details In Psp to X86 SMI interface, PSP will access some FCH SMI register to
 *          check if X86 is ready to receive a new SMI or PSP SMI is enabled and etc.
 *          To avoid racing conditon, call this lib to accquire the mutex before access these SMI register
 *          Note, acquire and release should in pair
 *
 * @return VOID
 */
VOID
AcquirePspSmiRegMutex (
  VOID
  );

/**
 * @brief Release the ownship of some FCH SMI register
 *
 * @details In Psp to X86 SMI interface, PSP will access some FCH SMI register to
 *          check if X86 is ready to receive a new SMI or PSP SMI is enabled and etc.
 *          call this lib to release the mutex after complete accessing these SMI register
 *          Note, acquire and release should in pair
 *
 * @return VOID
 */
VOID
ReleasePspSmiRegMutex (
  VOID
  );

/**
 * @brief Routine to read a C2P register.
 *
 * @param[in] Socket             Processor socket to read from
 * @param[in] Die                Die number on Socket to read from
 * @param[in] C2pMsgOffset       SMN address offset, e.g. MP0_C2PMSG_37_SMN_OFFSET
 * @param[in] Instance           Instance ID of the target fabric device
 * @retval    AGESA_BOUNDS_CHK   Invalid Socket or Die
 * @retval    AGESA_SUCCESS      Value retrieved successfully
 */
EFI_STATUS
ReadC2PMsgReg (
  IN           UINTN       Socket,
  IN           UINTN       Die,
  IN           UINT32      C2pMsgOffset,
  IN OUT       UINT32      *RegValue
  );

#endif // _AMD_PSPCOMMONLIB_H_


