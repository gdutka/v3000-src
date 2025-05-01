/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
/**
 * @file AmdRasServiceDxeProtocol.h
 *
 * @brief  RAS boot time service protocol prototype definition
 *
 * @details Provide RAS boot time service interface for Genoa or later programs.
 *
 */

#ifndef _AMD_RAS_SERVICE_DXE_PROTOCOL_H_
#define _AMD_RAS_SERVICE_DXE_PROTOCOL_H_

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include "AmdRas.h"

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

extern EFI_GUID gAmdRasServiceDxeProtocolGuid;

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

// current Protocol revision
#define AMD_RAS_SERVICE_REV               (0x01)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

typedef struct _AMD_RAS_SERVICE_DXE_PROTOCOL AMD_RAS_SERVICE_DXE_PROTOCOL;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 * @brief Adds an entry to the ACPI BERT table.
 *
 * @details Use this function to add an error into the ACPI BERT table.
 *
 * @param[in] ErrorRecord            Error Record buffer. This record is defined in ACPI reference specification
 *                                   as Generic Error Data Entry and in the UEFI reference pecification Appendix for
 *                                   Common Platform Error Record. *  
 * @param[in] RecordLen              Error Record buffer length.
 * @param[in] ErrorType              Error type of the record.
 * @param[in] SeverityType           Severity type of the record.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *AMD_ADD_BOOT_ERROR_RECORD_ENTRY) (
  IN    UINT8   *ErrorRecord,
  IN    UINT32  RecordLen,
  IN    UINT8   ErrorType,
  IN    UINT8   SeverityType
);

/**
 * @brief Adds an entry to the ACPI HEST error source table.
 *
 * @details This function provides the service to add an error source record entry into the ACPI HEST table.
 *
 * @param[in] ErrorRecord            Error Record buffer. This record is defined in ACPI reference specification
 *                                   in the Error Source section.  
 * @param[in] RecordLen              Error Record buffer length, in bytes.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *ADD_HEST_ERROR_SOURCE_ENTRY) (
  IN    UINT8   *pErrorRecord,
  IN    UINT32  RecordLen
);

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
 * @brief Adds multiple entries to the ACPI HEST error source table.
 *
 * @details This function provides the service to add multiple error source record entries into the ACPI HEST table.
 *
 * @param[in] ErrorRecord            Error Record buffer. This record is defined in ACPI reference specification
 *                                   in the Error Source section.  
 * @param[in] RecordLen              Error Record buffer length, in bytes.
 * @param[in] RecordCount            Error source enttry count.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *ADD_MULTIPLE_HEST_ERROR_SOURCE_ENTRY) (
  IN    UINT8   *pErrorRecord,
  IN    UINT32  RecordLen,
  IN    UINT32  RecordCount
);

/**
 * @brief Update DIMM fru text 
 *
 * @details This function provides the service to add fru text to UMC MCA bank
 *
 * @param[in] pAmdDimmFruTextTable   Pointer to AMD_DIMM_FRUTEXT_TABLE structure.
 * @retval EFI_STATUS                0: Success, NonZero: Standard EFI Error.
 */
typedef
EFI_STATUS
(EFIAPI *UPDATE_DIMM_FRU_TEXT_TO_MCA) (
  IN    AMD_DIMM_FRUTEXT_TABLE   *pAmdDimmFruTextTable
);

/**
 * @brief  AMD_RAS_SERVICE_SMM_PROTOCOL prototype
 *
 * @details This protocol provides configuration service and SMM runtime services for use by the platform to 
 *          support their RAS feature set. This protocol is introduced for F19M10 and will propagate to new processors.
 */
struct _AMD_RAS_SERVICE_DXE_PROTOCOL {
  MCA_ERROR_ADDR_TRANSLATE                  McaErrorAddrTranslate;            /**< Function to convert an MCA_ADDR_UMC address to
                                                                                *  system address and DIMM specific information.
                                                                                */
  TRANSLATE_SYSADDR_TO_CS                   TranslateSysAddrToCS;             /**< Function for converting a physical address to DIMM
                                                                                *  specific information.
                                                                                */
  AMD_ADD_BOOT_ERROR_RECORD_ENTRY           AddBootErrorRecordEntry;          ///< Function to add a boot error record entry into the ACPI BERT table.
  ADD_HEST_ERROR_SOURCE_ENTRY               AddHestErrorSourceEntry;          ///< Function to add an error source record entry into the ACPI HEST table.
  SEARCH_MCA_ERROR                          SearchMcaError;                   /**< Function for performing a search for MCA errors
                                                                                *  through all banks from a specific thread.
                                                                                */
  ADD_MULTIPLE_HEST_ERROR_SOURCE_ENTRY      AddMultipleHestErrorSourceEntry;  ///< Function to add multi error source entry into the ACPI HEST table.
  UPDATE_DIMM_FRU_TEXT_TO_MCA               UpdateDimmFruTextToMca;           /**< Function to update Dimm fru text to MCA UMC bank.
                                                                               * @li __New for Genoa__
                                                                               */
};

#endif //_AMD_RAS_SERVICE_DXE_PROTOCOL_H_


