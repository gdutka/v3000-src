/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * NBIO CPPC Implementation
 *
 *
 */

#ifndef _AMD_NBIO_CPPC_SERVICES_PROTOCOL_H_
#define _AMD_NBIO_CPPC_SERVICES_PROTOCOL_H_

#include <CppcCommon.h>

// Current PROTOCOL revision
#define DXE_AMD_NBIO_CPPC_SERVICES_DXE_REVISION   0x00

///
/// Forward declaration for the NBIO_CPPC_SERVICES_PROTOCOL
///
typedef struct _DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL;

///
/// CPPC Info Structure
///
typedef struct {
  BOOLEAN              IsCppcEnabled;          ///< Is CPPC enabled
  UINT32               CppcRegBitmap;          ///< CPPC register bitmap
  UINT32               CppcVersionInfo;        ///< CPPC version info
  UINT32               DoorbellRegister;       ///< Doorbell register address
  UINT32               NominalLatency;         ///< Nominal Latency Value
  UINT32               MaxPeriodicAccessRate;  ///< Max Periodic Access Rate
  UINT32               MinReqTurnaroundTime;   ///< Min req turnaround time
} AMD_CPPC_INFO;


/// CPPC reporting versions
typedef enum {
  CppcReportingVersionMailbox = 0,
  CppcReportingVersionMsr = 1,
  CppcReportingVersionUnknown = 2
} CPPC_REPORTING_VERSION;

///
/// This structure describes the SMU message specific registers of CPPC
///
typedef struct {
  BOOLEAN     Valid;        ///< Will be TRUE as long as the list is valid. Any entries with FALSE
  UINT32      Index;        ///< Index of register in ACPI Spec, and CppcCommon.h
  UINT32      Width;        ///< Width of register in bits
  UINT32      Value;        ///< Value of register, taken from SMU. if invalid it is equal to 0xFFFFFFFF
} CPPC_SMU_MESSAGE_REGISTER;

#define CPPC_SMU_MESSAGE_REGISTER_UNINITIALIZED  0xFFFFFFFF

//
// Protocol Definitions
//

/**
  Get CPPC info from SMU

  @param This       Instance of this protocol
  @param CppcInfo   CPPC info structure

  @return EFI_STATUS   Returns EFI_SUCCESS, unless NULL inputs (EFI_INVALID_PARAMETER) or CPPC broken/off (EFI_NOT_FOUND)
**/
typedef
EFI_STATUS
(EFIAPI * AMD_CPPC_SERVICES_GET_INFO) (
  IN   DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL   *This,
  OUT  AMD_CPPC_INFO                         *CppcInfo
  );

/**
  Send location of CPPC shared memory region to SMU

  @param SharedRegionStart  Location of first byte of table in memory

  @return EFI_STATUS        Always returns EFI_SUCCESS
**/
typedef
EFI_STATUS
(EFIAPI * AMD_CPPC_SERVICES_SET_TABLE) (
  IN  EFI_PHYSICAL_ADDRESS      SharedRegionStart
  );


/**
  Get the version of CPPC protocol

  @param Version      Pointer to variable that will store version on return

  @return EFI_STATUS  Returns EFI_SUCCESS in all cases except if a NULL pointer passed in
**/
typedef
EFI_STATUS
(EFIAPI * AMD_CPPC_SERVICES_GET_REPORTING_VERSION) (
  OUT UINT8       *Version
  );

/**
  Retrieve all SMU message registers. ONLY supported if CppcGetReportingVersion reports CppcReportingVersionMsr

  @param MessageRegisters   Double pointer that points to location of table containing registers
  @param NumberOfRegisters  The number of registers in the table

  @return EFI_STATUS       Returns EFI_SUCCESS unless NULL parameters are passed in which case EFI_INVALID_PARAMETER
**/

typedef
EFI_STATUS
(EFIAPI * AMD_CPPC_SERVICES_GET_MESSAGE_REGISTERS) (
  OUT CPPC_SMU_MESSAGE_REGISTER     **MessageRegisters,
  OUT UINT32                        *NumberOfRegisters
  );

///
/// The Protcool Prototype of CPPC Services
///
struct _DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL {
  UINT32                                    Revision;            ///< revision
  AMD_CPPC_SERVICES_GET_INFO                CppcGetInfo;         ///< Get CPPC Info
  AMD_CPPC_SERVICES_SET_TABLE               SetTable;            ///< Set location of shared memory
  AMD_CPPC_SERVICES_GET_REPORTING_VERSION   GetReportingVersion; ///< Query protocol for SMU CPPC reporting version
  AMD_CPPC_SERVICES_GET_MESSAGE_REGISTERS   GetMessageRegisters; ///< Get CPPC message registers.
};

extern EFI_GUID gAmdNbioCppcServicesProtocolGuid;

#endif /* _AMD_NBIO_CPPC_SERVICES_PROTOCOL_H_ */

