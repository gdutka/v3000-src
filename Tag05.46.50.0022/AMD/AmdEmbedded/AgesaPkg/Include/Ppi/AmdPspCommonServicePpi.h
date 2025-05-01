/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file AmdPspCommonServicePpi.h
 *
 * @brief  PSP common service PPI prototype definition
 *
 * @details Provide generic PSP external interface accorss programs
 *
 */
#ifndef _AMD_PSP_COMMONSERVICE_PPI_H_
#define _AMD_PSP_COMMONSERVICE_PPI_H_

#include <AmdPspDirectory.h>
/**
 * @brief Function declaration used to get Ftpm control area address
 *
 * @param FtpmControlArea  Point to address used to hold FtpmControlArea address
 *
 * @retval BOOLEAN TRUE: Succeed FALSE: Failed
 */
typedef
BOOLEAN
(*FP_GET_FTPM_CONTROLAREA) (
  IN OUT   VOID **FtpmControlArea
  );


/**
 * @brief Function declaration used to turn on/restore PSP MMIO decode, deprecated from SOC15
 *
 */
typedef
VOID
(*FP_SWITCH_PSP_MMIO_DECODE) (
  IN       BOOLEAN SwitchFlag,
  IN OUT   UINT32 *RegisterCopy
  );

/**
 * @brief Function declaration used to check PSP Device present by checking fuse receipt copy, deprecated from SOC15
*/
typedef
BOOLEAN
(*FP_CHECK_PSP_DEVICE_PRESENT) (
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
typedef
EFI_STATUS
(*FP_CHECK_FTPM_CAPS) (
  UINT32 *Caps
  );

/**
  @brief This function is to get the PSP entry information for given PSP entry type.

  @param[in]   value of given PSP entry type
  @param[out]  pointer to PSP entry address
  @param[out]  pointer to PSP entry size

  @retval TRUE            The given entry type is found
  @retval FALSE           The given entry type is not found

**/
typedef
BOOLEAN
(*FP_PSPENTRYINFO) (
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
typedef
BOOLEAN
(*FP_PSP_LIB_TIMEOUT) (
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
typedef
VOID
(*FP_ACQUIRE_PSPSMIREGMUTEX) (
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
typedef
VOID
(*FP_RELEASE_PSPSMIREGMUTEX) (
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
typedef
EFI_STATUS
(*FP_READ_C2P_MESSAGE_REGISTER) (
  IN         UINTN        Socket,
  IN         UINTN        Die,
  IN         UINT32       C2pMsgOffset,
  IN OUT     UINT32       *RegValue
  );

/**
 * @brief Check PSP caps, e.g. NVRAM status which used for RPMC feature
 *
 * @param PspCaps Point to PSP caps
 * @return EFI_STATUS 0: Success, NonZero Error
 */
typedef
EFI_STATUS
(*FP_CHECK_PSP_CAPS) (
  IN OUT   UINT32 *PspCaps
  );


/**
 * @brief  AMD_PSP_COMMON_SERVICE_PPI prototype
 *
 * @details Defines AMD_PSP_COMMON_SERVICE_PPI, which publish the common PSP service across all programs
 */
typedef struct _AMD_PSP_COMMON_SERVICE_PPI {
  FP_GET_FTPM_CONTROLAREA       GetFtpmControlArea;     ///< Get FTPM control Area
  FP_SWITCH_PSP_MMIO_DECODE     SwitchPspMmioDecode;     ///< Turn on/off Psp MMIO
  FP_CHECK_PSP_DEVICE_PRESENT   CheckPspDevicePresent;     ///< Check if PSP Device Present
  FP_CHECK_FTPM_CAPS            CheckFtpmCaps;     ///< Check if Ftpm supported
  FP_PSPENTRYINFO               PSPEntryInfo;           ///< Get specific PSP Entry information
  FP_PSP_LIB_TIMEOUT            PspLibTimeOut;     ///< Delay function
  FP_READ_C2P_MESSAGE_REGISTER  ReadC2PMsgReg;     ///< Read C2P MsgReg
  FP_ACQUIRE_PSPSMIREGMUTEX     AcquirePspSmiRegMutex;  ///< Acquire Psp SmiReg Mutex
  FP_RELEASE_PSPSMIREGMUTEX     ReleasePspSmiRegMutex;  ///< Release Psp SmiReg Mutex
  FP_CHECK_PSP_CAPS             CheckPspCaps;     ///< Check PSP caps
} AMD_PSP_COMMON_SERVICE_PPI;

extern EFI_GUID gAmdPspCommonServicePpiGuid;

#endif //_AMD_PSP_COMMONSERVICE_PPI_H_


