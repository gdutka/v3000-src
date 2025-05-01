/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Library
 *
 * Contains implementation for the CCX CPPC library routines
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision:  $   @e \$Date:  $
 *
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Filecode.h>
#include <Library/IdsLib.h>
#include <Library/CcxCppcLib.h>
#include <Protocol/AmdNbioCppcServicesProtocol.h>
#include <Protocol/AmdAcpiCppcServicesProtocol.h>

#define FILECODE LIBRARY_DXECCXCPPCLIB_CCXCPPCLIB_FILECODE

STATIC CPPC_ENTRIES_COMMON_TABLE  ROMDATA CppcRegisterDescriptorsMailbox[] =
{
  //Highest Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000000}, NULL, SUPPORTED_BY_V2},
  //Nominal Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000004}, NULL, SUPPORTED_BY_V2},
  //Lowest Non-Linear Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000008}, NULL, SUPPORTED_BY_V2},
  //Lowest Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x000000000000000C}, NULL, SUPPORTED_BY_V2},
  //Guaranteed Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000010}, NULL, SUPPORTED_BY_V2},
  //Desired Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000014}, NULL, SUPPORTED_BY_V2},
  //Minimum Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000018}, NULL, SUPPORTED_BY_V2},
  //Maximum Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x000000000000001C}, NULL, SUPPORTED_BY_V2},
  //Performance Reduction Tolerance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000020}, NULL, SUPPORTED_BY_V2},
  //Time Window
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000024}, NULL, SUPPORTED_BY_V2},
  //Counter Wraparound Time
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000028}, NULL, SUPPORTED_BY_V2},
  //Reference Performance Counter
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 64, 0, GENERIC_REG_ADDR_SIZE_QWORD, 0x00000000000000E7}, NULL, SUPPORTED_BY_V2},
  //Delivered Performance Counter
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 64, 0, GENERIC_REG_ADDR_SIZE_QWORD, 0x00000000000000E8}, NULL, SUPPORTED_BY_V2},
  //Performance Limited
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000040}, NULL, SUPPORTED_BY_V2},
  //CPPC Enable
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000044}, NULL, SUPPORTED_BY_V2},
  //Autonomous Selection Enable
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000048}, NULL, SUPPORTED_BY_V2},
  //Autonomous Activity Window
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x000000000000004C}, NULL, SUPPORTED_BY_V2},
  //Energy Performance Preference
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000050}, NULL, SUPPORTED_BY_V2},
  //Reference Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_PCC, 32, 0, PCC_SUBSPACE_0, 0x0000000000000054}, NULL, SUPPORTED_BY_V2},
  //Lowest Frequency
  {CPC_INTEGER_DWORD, {0xFF, 0xFF, 0xFF, 0xFF, 0xFFFFFFFFFFFFFFFF}, AmdCppcGetLowestSpeed, SUPPORTED_BY_V3},
  //Nominal Frequency
  {CPC_INTEGER_DWORD, {0xFF, 0xFF, 0xFF, 0xFF, 0xFFFFFFFFFFFFFFFF}, AmdCppcGetNominalSpeed, SUPPORTED_BY_V3},
  //Terminator
  {CPC_END_OF_TABLE, {0xFF, 0xFF, 0xFF, 0xFF, 0xFFFFFFFFFFFFFFFF}, 0},
};


/* -----------------------------------------------------------------------------*/
/**
 *
 *  GetCppcReportingVersion
 *
 *    Returns the current version of CPPC
 *
 *    @param[in]      NbioCppcServices         Pointer to NBIO Cppc Services
 *
 *    @return         CPPC reporting version
 */
UINT8
GetCppcReportingVersion (
  IN       DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL *NbioCppcServices
  )
{
  UINT8 ReportingVersion;

  ReportingVersion = CppcReportingVersionUnknown;
  NbioCppcServices->GetReportingVersion (&ReportingVersion);

  return ReportingVersion;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  GetCppcRegisterDescriptor
 *
 *    Returns the CPPC Register Descriptors based on the current CPPC version
 *
 *    @param[in]      NbioCppcServices         Pointer to NBIO Cppc Services
 *
 *    @return         Pointer to list of CPPC register descriptors
 */
CPPC_ENTRIES_COMMON_TABLE   *
GetCppcRegisterDescriptor (
  IN       DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL *NbioCppcServices
  )
{
  EFI_STATUS                           CalledStatus;
  CPPC_ENTRIES_COMMON_TABLE            *CppcRegisterDescriptors;
  AMD_CCX_ACPI_CPPC_SERVICES_PROTOCOL  *AcpiCppcServices;

  if (CppcReportingVersionMsr == GetCppcReportingVersion (NbioCppcServices)) {
    CalledStatus = gBS->LocateProtocol (&gAmdCcxAcpiCppcServicesProtocolGuid, NULL, (VOID **) &AcpiCppcServices);
    ASSERT (!EFI_ERROR (CalledStatus));

    CalledStatus = AcpiCppcServices->GetRegisterDescriptorsMsr (AcpiCppcServices, &CppcRegisterDescriptors);
    ASSERT (!EFI_ERROR (CalledStatus));
  } else {
    CppcRegisterDescriptors = CppcRegisterDescriptorsMailbox;
  }

  return CppcRegisterDescriptors;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  AmdCppcAutonomousSelectionEnable
 *
 *    Returns the value of CPPC autonomous selection
 *
 *    @param[in]      NbioCppcServices         Pointer to NBIO Cppc Services
 *
 *    @return         CPPC Autonomous Selection
 */
UINT32
AmdCppcAutonomousSelectionEnable (
  IN       DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL *NbioCppcServices
  )
{
  UINT8 ReportingVersion;

  ReportingVersion = CppcReportingVersionUnknown;
  NbioCppcServices->GetReportingVersion (&ReportingVersion);
  if (ReportingVersion == CppcReportingVersionMsr) {
    return 1;
  } else {
    return 0;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  AmdCppcGetLowestSpeed
 *
 *    Returns the value of the lowest speed reported by SMU FW
 *
 *    @param[in]      NbioCppcServices         Pointer to NBIO Cppc Services
 *
 *    @return         Lowest speed
 */
UINT32
AmdCppcGetLowestSpeed (
  IN       DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL *NbioCppcServices
  )
{
  UINT32                     i;
  UINT32                     NumberOfRegisters;
  UINT32                     LowestSpeed;
  CPPC_SMU_MESSAGE_REGISTER *MessageRegisters;

  NumberOfRegisters = 0;
  MessageRegisters  = NULL;
  LowestSpeed       = 0;

  NbioCppcServices->GetMessageRegisters (&MessageRegisters, &NumberOfRegisters);

  if (MessageRegisters != NULL) {
    for (i = 0; i < NumberOfRegisters; i++) {
      if ((MessageRegisters + i)->Valid) {
        if ((MessageRegisters + i)->Index == CPC_LOWEST_FREQ_INDEX) {
          LowestSpeed = (MessageRegisters + i)->Value;
          IDS_HDT_CONSOLE (MAIN_FLOW, "  AmdCppcGetLowestSpeed %d\n", LowestSpeed);
          break;
        }
      }
    }
  }

  return LowestSpeed;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  AmdCppcGetNominalSpeed
 *
 *    Returns the value of the nominal speed reported by SMU FW
 *
 *    @param[in]      NbioCppcServices         Pointer to NBIO Cppc Services
 *
 *    @return         Nominal speed
 */
UINT32
AmdCppcGetNominalSpeed (
  IN       DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL *NbioCppcServices
  )
{
  UINT32                     i;
  UINT32                     NumberOfRegisters;
  UINT32                     NominalSpeed;
  CPPC_SMU_MESSAGE_REGISTER *MessageRegisters;

  NumberOfRegisters = 0;
  MessageRegisters  = NULL;
  NominalSpeed      = 0;

  NbioCppcServices->GetMessageRegisters (&MessageRegisters, &NumberOfRegisters);

  if (MessageRegisters != NULL) {
    for (i = 0; i < NumberOfRegisters; i++) {
      if ((MessageRegisters + i)->Valid) {
        if ((MessageRegisters + i)->Index == CPC_NOMINAL_FREQ_INDEX) {
          NominalSpeed = (MessageRegisters + i)->Value;
          IDS_HDT_CONSOLE (MAIN_FLOW, "  AmdCppcGetNominalSpeed %d\n", NominalSpeed);
          break;
        }
      }
    }
  }

  return NominalSpeed;
}
