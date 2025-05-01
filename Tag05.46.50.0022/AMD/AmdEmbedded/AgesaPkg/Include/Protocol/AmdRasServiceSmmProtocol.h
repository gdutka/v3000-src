/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
/**
 * @file AmdRasServiceSmmProtocol.h
 *
 * @brief  RAS runtime service protocol prototype definition
 *
 * @details Provide RAS runtime service interface for Genoa or later programs.
 *
 */

#ifndef _AMD_RAS_SERVICE_SMM_PROTOCOL_H_
#define _AMD_RAS_SERVICE_SMM_PROTOCOL_H_

#include "AmdRas.h"

extern EFI_GUID gAmdRasServiceSmmProtocolGuid;

// current PPI revision
#define AMD_RAS_SMM_REV  0x03

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
typedef struct _AMD_RAS_SERVICE_SMM_PROTOCOL AMD_RAS_SERVICE_SMM_PROTOCOL;

/**
 * @brief Converts an MCA address MSR value into a chip/device location.
 *
 * @details This function converts the given valid error address from an MCA ADDR MSR (UMC) register to system
 *          address with DIMM specific information.
 *
 * @param[in]  NormalizedAddress     This is the 'normalized' or UMC local relative address.
 * @param[out] SystemMemoryAddress   This return value indicates the physical system address related to this localized address.
 * @param[out] DimmInfo              Returned structure that contains the DIMM information related to the given address.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *MCA_ERROR_ADDR_TRANSLATE) (
  IN       NORMALIZED_ADDRESS *NormalizedAddress,
  OUT      UINT64             *SystemMemoryAddress,
  OUT      DIMM_INFO          *DimmInfo
);

/**
 * @brief Converts a system physical address into a chip/device location.
 *
 * @details This function converts the given system memory address into a normalized address indicating the SocketId,DieId,
 *          channelId for the memory controller that is responsible for the specified system address and also includes
 *          information to indicate the DIMM device.
 *
 * @param[in] SystemMemoryAddress    This return value indicates the physical system address related to this localized address.
 * @param[out] NormalizedAddress     This is the 'normalized' or UMC local relative address.
 * @param[out] DimmInfo              Returned structure that contains the DIMM information related to the given address.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *TRANSLATE_SYSADDR_TO_CS) (
  IN       UINT64             *SystemMemoryAddress,
  OUT      NORMALIZED_ADDRESS *NormalizedAddress,
  OUT      DIMM_INFO          *DimmInfo
);

/**
 * @brief This function sets an IO cycle trap/trigger to generate an SMI upon an access to the specified address.
 *
 * @details This function sets a given value into MSR register SmiTrigIoCycle. Please reference the PPR MSRC001_0056
 *          [SMI Trigger IO Cycle] register description for detail.
 *
 * @param[in] SmiTrigIoCycleData     The IO address for which the trap should be set.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *SET_SMI_TRIG_IOCYCLE) (
  IN       UINT64 SmiTrigIoCycleData
);


/**
 * @brief Collect the SMI status from all threads.
 *
 * @details Gather the SMI status flags for all threads. The local buffer will be filled with the SMI status from each core
 *          and/or thread.
 *
 * @param[in] pLocalSmiStatusList    A pointer to a local structure buffer to hold the status records for the threads.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *GET_ALL_LOCAL_SMI_STATUS) (
  IN       LOCAL_SMI_STATUS* pLocalSmiStatusList
);

/**
 * @brief Search through all MCA error banks for the errors.
 *
 * @details Searches the MCA error banks for the specified thread to locate the all reported MCA errors.
 *          Each search starts with MCA bank 0 and proceeds to the last bank.
 *
 * @param[in, out] RasMcaErrorInfo   A pointer to a local structure buffer that will hold the errors found.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *SEARCH_MCA_ERROR) (
  IN OUT   RAS_MCA_ERROR_INFO_V2* RasMcaErrorInfo
);

/**
 * @brief This is function selects the type of event generated to the specified processor upon SMM exit.
 *
 * @details When Platform First Error Handling (PFEH) is active, all MCA errors will go into SMM mode. After local
 *          processing, the SMI handler must set the event type that will be returned to the system. This function
 *          will set the type of event for the specific processor that will trigger the OS error handler for further
 *          processing.
 *
 * @param[in] ProcessorNumber        A pointer to the cpu index number used for the MP services during start up.
 *                                   See the UEFI reference specification for MP_SERVICES.The function will setup
 *                                   through all threads when input pointer = NULL.
 * @param[in] SmiExitType            Select interrupt type to be generate. Please refer to the PPR section for 
 *                                   "Event Percolation" for further detail.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *RAS_SMM_EXIT_TYPE) (
  IN       UINTN ProcessorNumber,
  IN       UINTN SmiExitType
);

/**
 * @brief Retrieve the SMM Base address for the indicated CPU core.
 *
 * @details This function will locate and return the SMM Save State base address for the indicated processor. 
 *          For further information about the Save State buffer, see the PPR.
 *
 * @param[in] ProcessorNumber        A pointer to the cpu index number used for the MP services during start up.
 *                                   See the UEFI reference specification for MP_SERVICES.The function will setup
 *                                   through all threads when input pointer = NULL.
 * @param[in] SmmSaveStateBase       A Pointer to a local buffer where to place the SMM base pointer.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *GET_SMM_SAVE_STATE_BASE) (
  IN       UINTN ProcessorNumber,
  OUT      UINT64* SmmSaveStateBase
);

/**
 * @brief Set the error cloaking mask for a specific processor.
 *
 * @details When Platform First Error Handling (PFEH) is active, the MCA registers are 'cloaked' from the OS -
 *          meaning the OS will not see them and cannot access them. This routine will set the cloaking mask value
 *          for the specified processor. If the firmware wished for the OS to handle an event/error. then the
 *          firmware will set the cloaking value so that the OS is permitted to see the error register. For further
 *          information on cloaking, please see the PPR.
 *
 * @param[in] ProcessorNumber        A pointer to the cpu index number used for the MP services during start up.
 *                                   See the UEFI reference specification for MP_SERVICES.The function will setup
 *                                   through all threads when input pointer = NULL.
 * @param[in] CloakValue             Value to write to the cloaking MSR for the indicated processor.
 * @param[in] UnCloakValue           Value to write to the uncloaking MSR for the indicated processor.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *SET_MCA_CLOAK_CFG) (
   IN       UINTN ProcessorNumber,
   IN       UINT64 CloakValue,
   IN       UINT64 UnCloakValue
);

/**
 * @brief Clears the indicated MCA status register.
 *
 * @details When Platform First Error Handling (PFEH) is active access to MCA registers will cause SMI; so, these
 *          accessed need to be filtered by routines such as this to avoid extra SMI occurrences.
 *
 * @param[in] ProcessorNumber        A pointer to the cpu index number used for the MP services during start up.
 *                                   See the UEFI reference specification for MP_SERVICES.The function will setup
 *                                   through all threads when input pointer = NULL.
 * @param[in] McaBankNumber          Indicates which MCA bank number for which the status should be cleared.
 * @param[in] IsWrMsr                This is a flag indicate the clear MCA_STATUS request is from OS or Firmware.
 *                                   @li FALSE - write is from firmware
 *                                   @li TRUE - write from OS via WRMSR instruction trap.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *CLR_MCA_STATUS) (
  IN       UINTN    ProcessorNumber,
  IN       UINTN    McaBankNumber,
  IN       BOOLEAN  IsWrMsr
);

/**
 * @brief This function is used to covert an ECC symbol to a DRAM device range.
 *
 * @param[in]  This                  UEFI SMM standard pointer to the SMM services function block.
 * @param[in]  RasMcaErrorInfo       A pointer to a structure buffer that will hold found errors.
 * @param[in]  NormalizedAddress     This is the 'normalized' or UMC local relative address.
 * @param[in]  BankIndex             Offset of data in McaBankErrorInfo struct.
 * @param[out] DeviceStart           First device where the symbol indicates an error.
 * @param[out] DeviceEnd             Last device where the symbol indicates an error.
 * @param[out] DeviceType            DIMM Device Width from SPD.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error
 */
typedef
EFI_STATUS
(EFIAPI *MAP_SYMBOL_TO_DRAM_DEVICE) (
  IN       AMD_RAS_SERVICE_SMM_PROTOCOL *This,
  IN       RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo,
  IN       NORMALIZED_ADDRESS    *NormalizedAddress,
  IN       UINT8                 BankIndex,
  OUT      UINT32                *DeviceStart,
  OUT      UINT32                *DeviceEnd,
  OUT      UINT8                 *DeviceType
);

/**
 * @brief Routine to set MCA correctable error threshold counter.
 *
 * @param[in] ProcessorNumber        A pointer to the cpu index number used for the MP services during start up.
 *                                   See the UEFI reference specification for MP_SERVICES.The function will setup
 *                                   through all threads when input pointer = NULL.
 * @param[in] McaBankNumber          A Pointer to the MCA bank number for which the threshold need to set.
 *                                   The MCA bank is as defined in the PPR. The function will setup through all
 *                                   available banks in the thread when input pointer = NULL.
 * @param[in] RasThresholdConfig     point to RAS_THRESHOLD_CONFIG structure.
 * @param[in] OvrflwChk              FALSE: Always reset ErrCnt. TRUE: Will check Ovrflw bit first before reset ErrCnt. 
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error
 */
typedef
EFI_STATUS
(EFIAPI *SET_MCA_THRESHOLD) (
  IN       UINTN                 *ProcessorNumber,
  IN       UINTN                 *McaBankNumber,
  IN       RAS_THRESHOLD_CONFIG  *RasThresholdConfig,
  IN       BOOLEAN               OvrflwChk
);

/**
 * @brief Routine to translate system address to NVDIMM DPA.
 *
 * @param[in] SystemMemoryAddress     System memory address
 * @param[out] Dpa                    NVDIMM Dpa
 * @retval EFI_STATUS                 0: Success, NonZero: Standard EFI Error
 */
typedef
EFI_STATUS
(EFIAPI *TRANSLATE_SYSADDR_TO_DPA) (
  IN       UINT64           *SystemMemoryAddress,
  OUT      UINT64           *Dpa
);

/**
  Enable DRAM Ecc Error Counter and Dram ECC Error Counter Threshold.

**/
typedef
EFI_STATUS
(EFIAPI *SET_DRAM_CORRECTED_ERROR_COUNTER) (
  VOID
);

/**
 * @brief  AMD_RAS_SERVICE_SMM_PROTOCOL prototype
 *
 * @details This protocol provides configuration service and SMM runtime services for use by the platform to
 *          support their RAS feature set. This protocol is introduced for F19M10 and will propagate to new processors.
 */
struct _AMD_RAS_SERVICE_SMM_PROTOCOL {
  MCA_ERROR_ADDR_TRANSLATE          McaErrorAddrTranslate;    /**< Function to convert an MCA_ADDR_UMC address to
                                                                *  system address and DIMM specific information.
                                                                */
  TRANSLATE_SYSADDR_TO_CS           TranslateSysAddrToCS;     /**< Function for converting a physical address to DIMM
                                                                *  specific information.
                                                                */
  SET_SMI_TRIG_IOCYCLE              SetSmiTrigIoCycle;        ///< Set SmiTrigIoCycle
  GET_ALL_LOCAL_SMI_STATUS          GetAllLocalSmiStatus;     ///< Get all Local Smi Status
  SEARCH_MCA_ERROR                  SearchMcaError;           /**< Function for performing a search for MCA errors
                                                                *  through all banks from a specific thread.
                                                                */
  RAS_SMM_EXIT_TYPE                 RasSmmExitType;           ///< Set SMM exit interrupt type
  GET_SMM_SAVE_STATE_BASE           GetSmmSaveStateBase;      ///< Get CPU SMM Save State Base Address
  SET_MCA_CLOAK_CFG                 SetMcaCloakCfg;           ///< Set PFEH_CLOAK_CFG
  CLR_MCA_STATUS                    ClrMcaStatus;             ///< Clear MCA_STATUS MSR
  MAP_SYMBOL_TO_DRAM_DEVICE         MapSymbolToDramDevice;    ///< Map the DRAM device to symbol
  SET_MCA_THRESHOLD                 SetMcaThreshold;          ///< Set MCA Thershold count
  TRANSLATE_SYSADDR_TO_DPA          TranslateSysAddrToDpa;    ///< System Address Translate to Dpa
  SET_DRAM_CORRECTED_ERROR_COUNTER  CorrectedErrorCounter;    ///< Config DRAM Corrected Error Counter
};

#endif //_AMD_RAS_SMMRS_PROTOCOL_H_


