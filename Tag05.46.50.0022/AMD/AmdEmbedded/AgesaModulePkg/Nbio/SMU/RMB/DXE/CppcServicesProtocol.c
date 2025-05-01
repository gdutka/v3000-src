/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * CPPC Services
 *
 *
 */

#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/BaseLib.h>
#include <Library/NbioHandleLib.h>
#include <SMU_RMB_MsgDef.h>
#include <Library/NbioSmuV13Lib.h>
#include <Protocol/AmdNbioCppcServicesProtocol.h>
#include <Addendum/Apcb/Inc/EnvironmentFlags.h>
#include "smu13_RMB.h"
#include "SmuV13Dxe.h"
#include <GnbRegistersRMB.h>
#include <GnbDxio.h>
#include <Filecode.h>
#include <smu_cppc_if.h>


#define FILECODE        NBIO_SMU_RMB_DXE_CPPCSERVICESPROTOCOL_FILECODE

STATIC GNB_HANDLE                    *mGnbHandle = NULL;

///
/// CPPC Message Value Template, to be filled in with a later protocol call
///
CPPC_SMU_MESSAGE_REGISTER  CppcMessageRegisters [] = {
  {FALSE,  CPC_LOWEST_FREQ_INDEX,  CPPC_SMU_MESSAGE_REGISTER_UNINITIALIZED },
  {FALSE,  CPC_NOMINAL_FREQ_INDEX, CPPC_SMU_MESSAGE_REGISTER_UNINITIALIZED }
  };

/**
  Function to initialize module specific variables used in CPPC initialization.

**/
VOID
InitializeCppcServicesProtocol (
  VOID
  )
{
  PCIe_PLATFORM_CONFIG                 *PcieFromHob;
  PCIe_PLATFORM_CONFIG                 *Pcie;

  if (mGnbHandle != NULL) {
    return;
  }

  SmuDxeInitialize(&PcieFromHob);
  Pcie = PcieFromHob;

  mGnbHandle = NbioGetHandle (Pcie);
}

/**
  Get CPPC info

  @param This       Instance of this protocol
  @param CppcInfo   CPPC info structure

  @return EFI_STATUS   Returns EFI_SUCCESS always
**/
EFI_STATUS
CppcGetInfo (
  IN   DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL   *This,
  OUT  AMD_CPPC_INFO                         *CppcInfo
  )
{
  UINT32                              SmuArg[6];
  UINT32                              SmuFeatureControlBits;
  CPPC_Supported_Registers_t          *SmuCppcResponse;
  PRESIL_CTRL1                        PreSilCtrl1;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Start\n", __FUNCTION__);
  InitializeCppcServicesProtocol ();

  ASSERT ((This != NULL)  && (CppcInfo != NULL));
  if ((This == NULL)  || (CppcInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  ASSERT (mGnbHandle != NULL);

  ZeroMem (SmuArg, sizeof (UINT32)*6);
  // Note when this memory is zeroed out the CPPC enable return value is defaulted to FALSE
  ZeroMem (CppcInfo, sizeof (AMD_CPPC_INFO));

  SmuFeatureControlBits = PcdGet32 (PcdSmuFeatureControlDefinesExt);

  PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
  if (PreSilCtrl1.Field.FASTSIM_SMU_MSGS == 0) {
    if ((SmuFeatureControlBits & FEATURE_CPPC_MASK) > 0)  {
      // ASK SMU for this information
      NbioSmuServiceRequestV13 (
        NbioGetHostPciAddress (mGnbHandle),
        BIOSSMC_MSG_GetCPPCSupportedRegisters,
        SmuArg,
        0
        );

      if (SmuArg[0] != 0x00) {
        SmuCppcResponse = (CPPC_Supported_Registers_t *)&(SmuArg[0]);
        CppcInfo->IsCppcEnabled = (BOOLEAN)(SmuCppcResponse->CPPCEnable);
        CppcInfo->CppcRegBitmap = (UINT32) RShiftU64 (*((UINT32 *)SmuCppcResponse), 2);  // Shift out version. Version gets its own field below
        CppcInfo->CppcVersionInfo = SmuCppcResponse->Version;
        CppcInfo->DoorbellRegister = 0;
        CppcInfo->NominalLatency = CPPC_NOMINAL_LATENCY;
        CppcInfo->MaxPeriodicAccessRate = CPPC_MAX_PERIODIC_ACCESS_RATE;
        CppcInfo->MinReqTurnaroundTime = CPPC_MIN_REQ_TURNAROUND_TIME;
      }
      else {
        return EFI_NOT_FOUND;
      }
    }
    else {
      return EFI_NOT_FOUND;
    }
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);
  return EFI_SUCCESS;
}

/**
  Send location of CPPC shared memory region to SMU

  @param SharedRegionStart  Location of first byte of table in memory

  @return EFI_STATUS        Always returns EFI_SUCCESS
**/
EFI_STATUS
CppcSetTable (
  IN  EFI_PHYSICAL_ADDRESS      SharedRegionStart
  )
{

  // Only supported in MSR version
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Is not supported on this platform!\n", __FUNCTION__);
  ASSERT(FALSE);

  return EFI_NOT_FOUND;
}

/**
  Query protocol for SMU CPPC reporting version

  @param Version      Pointer to variable that will store version on return

  @return EFI_STATUS  Returns EFI_SUCCESS in all cases except if a NULL pointer passed in
**/
EFI_STATUS
CppcGetReportingVersion (
  OUT UINT8       *Version
  )
{
  if (Version == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Version = CppcReportingVersionMsr;

  return EFI_SUCCESS;
}

/**
  Retrieve all SMU message registers. ONLY supported if CppcGetReportingVersion reports CppcReportingVersionMsr

  @param MessageRegisters   Double pointer that points to location of table containing registers
  @param NumberOfRegisters  The number of registers in the table

  @return EFI_STATUS       Returns EFI_SUCCESS unless NULL parameters are passed in which case EFI_INVALID_PARAMETER
**/
EFI_STATUS
CppcGetMessageRegisters (
  OUT CPPC_SMU_MESSAGE_REGISTER     **MessageRegisters,
  OUT UINT32                        *NumberOfRegisters
  )
{
  UINT32                              SmuArg[6];

  // Invalid parameter check
  if ((MessageRegisters == NULL) || (NumberOfRegisters == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  InitializeCppcServicesProtocol ();

  ZeroMem (SmuArg, sizeof (UINT32)*6);
  NbioSmuServiceRequestV13 (
    NbioGetHostPciAddress (mGnbHandle),
    BIOSSMC_MSG_GetCPPCNominalFrequency,
    SmuArg,
    0
    );
  CppcMessageRegisters[0].Valid = TRUE;
  CppcMessageRegisters[0].Index = CPC_NOMINAL_FREQ_INDEX;
  CppcMessageRegisters[0].Width = 32;
  CppcMessageRegisters[0].Value = SmuArg[0];

  ZeroMem (SmuArg, sizeof (UINT32)*6);
  NbioSmuServiceRequestV13 (
    NbioGetHostPciAddress (mGnbHandle),
    BIOSSMC_MSG_GetCPPCLowestFrequency,
    SmuArg,
    0
    );
  CppcMessageRegisters[1].Valid = TRUE;
  CppcMessageRegisters[1].Index = CPC_LOWEST_FREQ_INDEX;
  CppcMessageRegisters[1].Width = 32;
  CppcMessageRegisters[1].Value = SmuArg[0];

  // Pointers to return variables
  *NumberOfRegisters = 2;
  *MessageRegisters = CppcMessageRegisters;

  return EFI_SUCCESS;

}

/**
  Protocol initialization for gAmdNbioCppcServicesProtocolGuid

**/
DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL  mNbioCppcServicesProtocol = {
  DXE_AMD_NBIO_CPPC_SERVICES_DXE_REVISION,
  CppcGetInfo,
  CppcSetTable,
  CppcGetReportingVersion,
  CppcGetMessageRegisters
};

