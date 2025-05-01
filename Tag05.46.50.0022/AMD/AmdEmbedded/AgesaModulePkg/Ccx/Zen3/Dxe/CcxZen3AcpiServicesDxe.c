/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <Library/BaseLib.h>
#include <Library/CcxBaseX86Lib.h>
#include <AGESA.h>
#include <Filecode.h>
#include <cpuRegisters.h>
#include <CcxRegistersZen3.h>
#include <CcxZen3AcpiServicesDxe.h>
#include <Library/AmdBaseLib.h>
#include <Library/AmdHeapLib.h>
#include <Library/CcxPstatesLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/FabricRegisterAccLib.h>
#include <Library/CcxApicLib.h>
#include <Library/CcxCppcLib.h>
#include <Protocol/MpService.h>
#include <Protocol/AmdAcpiCpuSsdtServicesProtocol.h>
#include <Protocol/AmdAcpiCratServicesProtocol.h>
#include <Protocol/AmdAcpiSratServicesProtocol.h>
#include <Protocol/AmdCoreTopologyV2Protocol.h>
#include <Protocol/FabricNumaServices2.h>
#include <Protocol/FabricTopologyServices2.h>
#include <Protocol/AmdNbioSmuServicesProtocol.h>
#include <Protocol/AmdNbioCppcServicesProtocol.h>
#include <Protocol/AmdAcpiPcctServicesProtocol.h>
#include <Protocol/AmdAcpiCppcServicesProtocol.h>
#include <Protocol/FabricResourceManagerServicesProtocol.h>


#define FILECODE CCX_ZEN3_DXE_CCXZEN3ACPISERVICESDXE_FILECODE
STATIC GEN_COMM_SUBSPACE_STRUCT  ROMDATA GenCommSubspaceStruct =
{
  GEN_COMM_TYPE,        // Type
  GEN_COMM_LENGTH,      // Length
  {0},                  // Reserved
  0,                    // Base Address
  0,                    // Length
  {0},                  // Doorbell Register
  0,                    // Doorbell Preserve
  0,                    // Doorbell Write
  4,                    // Nominal Latency
  0,                    // Maximum Periodic Access Rate
  0                     // Minimum Request Turnaround Time
};

/// TLB type
typedef enum {
  TLB_2M = 0,       ///< 0 - TLB 2M4M associativity
  TLB_4K,           ///< 1 - TLB 4K associativity
  TLB_1G,           ///< 2 - TLB 1G associativity
  TLB_TYPE_MAX,     ///< MaxValue
} TLB_TYPE;

#define SHARED_MAX_SIZE             ((ZEN3_MAX_NUMBER_OF_APS + 1) * 256)
#define  BITS_PER_BYTE   8
#define  KB_SHIFT       10

STATIC CPPC_ENTRIES_COMMON_TABLE  ROMDATA Zen3CppcRegisterDescriptorsMsr[] =
{
  //Highest Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 8, 24, GENERIC_REG_ADDR_SIZE_QWORD, 0xC00102B0}, NULL, SUPPORTED_BY_V2},
  //Nominal Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 8, 16, GENERIC_REG_ADDR_SIZE_QWORD, 0xC00102B0}, NULL, SUPPORTED_BY_V2},
  //Lowest Non-Linear Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 8,  8, GENERIC_REG_ADDR_SIZE_QWORD, 0xC00102B0}, NULL, SUPPORTED_BY_V2},
  //Lowest Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 8,  0, GENERIC_REG_ADDR_SIZE_QWORD, 0xC00102B0}, NULL, SUPPORTED_BY_V2},
  //Guaranteed Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 8,  8, GENERIC_REG_ADDR_SIZE_QWORD, 0xC00102B2}, NULL, SUPPORTED_BY_V2}, // Not implemented, the register filed pointed are all 0s.
  //Desired Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 8, 16, GENERIC_REG_ADDR_SIZE_QWORD, 0xC00102B3}, NULL, SUPPORTED_BY_V2},
  //Minimum Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 8, 0x08, GENERIC_REG_ADDR_SIZE_QWORD, 0xC00102B3}, NULL, SUPPORTED_BY_V2},
  //Maximum Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 8, 0x00, GENERIC_REG_ADDR_SIZE_QWORD, 0xC00102B3}, NULL, SUPPORTED_BY_V2},
  //Performance Reduction Tolerance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_MEM, 0, 0, GENERIC_REG_ADDR_SIZE_UNDEFINED, 0}, NULL, SUPPORTED_BY_V2}, // Unsupported
  //Time Window
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_MEM, 0, 0, GENERIC_REG_ADDR_SIZE_UNDEFINED, 0}, NULL, SUPPORTED_BY_V2}, // Unsupported
  //Counter Wraparound Time
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_MEM, 0, 0, GENERIC_REG_ADDR_SIZE_UNDEFINED, 0}, NULL, SUPPORTED_BY_V2}, // Unsupported
  //Reference Performance Counter
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 64, 0, GENERIC_REG_ADDR_SIZE_QWORD, 0x00000000000000E7}, NULL, SUPPORTED_BY_V2}, // MPERF
  //Delivered Performance Counter
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 64, 0, GENERIC_REG_ADDR_SIZE_QWORD, 0x00000000000000E8}, NULL, SUPPORTED_BY_V2}, // APERF
  //Performance Limited
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 2,  0, GENERIC_REG_ADDR_SIZE_QWORD, 0xC00102B4}, NULL, SUPPORTED_BY_V2},
  //CPPC Enable
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 1,  0, GENERIC_REG_ADDR_SIZE_QWORD, 0xC00102B1}, NULL, SUPPORTED_BY_V2},
  //Autonomous Selection Enable
  {CPC_INTEGER_DWORD, {0xFF, 0xFF, 0xFF, 0xFF, 0xFFFFFFFFFFFFFFFF}, AmdCppcAutonomousSelectionEnable, SUPPORTED_BY_V2}, // fixed value, set to 1
  //Autonomous Activity Window
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_MEM, 0, 0, GENERIC_REG_ADDR_SIZE_UNDEFINED, 0}, NULL, SUPPORTED_BY_V2},
  //Energy Performance Preference
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_FFH, 8, 24, GENERIC_REG_ADDR_SIZE_QWORD, 0xC00102B3}, NULL, SUPPORTED_BY_V2},
  //Reference Performance
  {CPC_BUFFER, {GENERIC_REG_SPACE_ID_MEM, 0, 0, GENERIC_REG_ADDR_SIZE_UNDEFINED, 0}, NULL, SUPPORTED_BY_V2},
  //Lowest Frequency
  {CPC_INTEGER_DWORD, {0xFF, 0xFF, 0xFF, 0xFF, 0xFFFFFFFFFFFFFFFF}, AmdCppcGetLowestSpeed, SUPPORTED_BY_V3},
  //Nominal Frequency
  {CPC_INTEGER_DWORD, {0xFF, 0xFF, 0xFF, 0xFF, 0xFFFFFFFFFFFFFFFF}, AmdCppcGetNominalSpeed, SUPPORTED_BY_V3},
  //Terminator
  {CPC_END_OF_TABLE, {0xFF, 0xFF, 0xFF, 0xFF, 0xFFFFFFFFFFFFFFFF}, 0},
};

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
CcxZen3PStateGatherData (
  IN       AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL    *This,
     OUT   AMD_PSTATE_SYS_INFO                   **PstateSysInfoPtr
  );

AGESA_STATUS
CcxZen3GetPstateTransLatency (
     OUT   UINT32                                 *TransitionLatency,
  IN       AMD_CONFIG_PARAMS                      *StdHeader
  );

EFI_STATUS
EFIAPI
CcxZen3GetCStateInfo (
  IN       AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL    *This,
     OUT   AMD_CSTATE_INFO                       **CstateInfo
  );

UINT32
EFIAPI
CcxZen3GetPsdDomain (
  IN       AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL    *This,
  IN       UINT32                                  LocalApicId
  );

EFI_STATUS
EFIAPI
CcxZen3GetPStatePower (
  IN       AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL    *This,
  IN       CCX_PSTATE                             Pstate,
     OUT   UINTN                                  *PowerInmW
  );

EFI_STATUS
EFIAPI
CcxZen3GetCratHsaProcEntry (
  IN       AMD_CCX_ACPI_CRAT_SERVICES_PROTOCOL    *This,
  IN       CRAT_HEADER                            *CratHeaderStructPtr,
  IN OUT   UINT8                                 **TableEnd
  );

EFI_STATUS
EFIAPI
CcxZen3GetCratCacheEntry (
  IN       AMD_CCX_ACPI_CRAT_SERVICES_PROTOCOL    *This,
  IN       CRAT_HEADER                            *CratHeaderStructPtr,
  IN OUT   UINT8                                 **TableEnd
  );

EFI_STATUS
EFIAPI
CcxZen3GetCratTlbEntry (
  IN       AMD_CCX_ACPI_CRAT_SERVICES_PROTOCOL    *This,
  IN       CRAT_HEADER                            *CratHeaderStructPtr,
  IN OUT   UINT8                                 **TableEnd
  );

UINT8 *
AddOneCratEntry (
  IN       CRAT_ENTRY_TYPE    CratEntryType,
  IN       CRAT_HEADER       *CratHeaderStructPtr,
  IN OUT   UINT8            **TableEnd
  );

UINT8
GetCacheAssoc (
  IN       UINT16   RawAssoc
  );

UINT8
GetTlbSize (
  IN       TLB_TYPE   TLB_TYPE,
  IN       CRAT_TLB  *CratTlbEntry,
  IN       UINT16     RawAssocSize
  );

EFI_STATUS
EFIAPI
CcxZen3CreateSratApicEntry (
  IN       AMD_CCX_ACPI_SRAT_SERVICES_PROTOCOL    *This,
  IN       SRAT_HEADER                            *SratHeaderStructPtr,
  IN OUT   UINT8                                 **TableEnd
  );

EFI_STATUS
EFIAPI
CcxZen3AddGenCommSubspaceStruct (
  IN       AMD_CCX_ACPI_PCCT_SERVICES_PROTOCOL    *This,
  IN       PCCT_HEADER                            *PcctHeaderStructPtr,
  IN       AMD_CPPC_INFO                          *AmdCppcInfo,
  IN OUT   UINT8                                  **EndOfTable
  );

EFI_STATUS
EFIAPI
CcxZen3GetRegisterDescriptorsMsr (
  IN       AMD_CCX_ACPI_CPPC_SERVICES_PROTOCOL    *This,
     OUT   CPPC_ENTRIES_COMMON_TABLE              **CppcRegDescMsr
  );

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
STATIC AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL   mZen3SsdtServicesProtocol = {
  0,
  CcxZen3PStateGatherData,
  CcxZen3GetCStateInfo,
  CcxZen3GetPsdDomain,
  CcxZen3GetPStatePower
};

STATIC AMD_CCX_ACPI_CRAT_SERVICES_PROTOCOL   mZen3CratServicesProtocol = {
  0,
  CcxZen3GetCratHsaProcEntry,
  CcxZen3GetCratCacheEntry,
  CcxZen3GetCratTlbEntry
};

STATIC AMD_CCX_ACPI_SRAT_SERVICES_PROTOCOL   mZen3SratServicesProtocol = {
  2,
  CcxZen3CreateSratApicEntry
};

STATIC AMD_CCX_ACPI_PCCT_SERVICES_PROTOCOL  mZen3PcctServicesProtocol = {
  1,
  CcxZen3AddGenCommSubspaceStruct
};

STATIC AMD_CCX_ACPI_CPPC_SERVICES_PROTOCOL  mZen3CppcServicesProtocol = {
  1,
  CcxZen3GetRegisterDescriptorsMsr
};

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

EFI_STATUS
EFIAPI
CcxZen3AcpiCpuSsdtServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  // Install ACPI CPU SSDT services protocol
  return gBS->InstallProtocolInterface (
                &ImageHandle,
                &gAmdAcpiCpuSsdtServicesProtocolGuid,
                EFI_NATIVE_INTERFACE,
                &mZen3SsdtServicesProtocol
                );
}

EFI_STATUS
EFIAPI
CcxZen3CratServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  // Install ACPI CPU CRAT services protocol
  return gBS->InstallProtocolInterface (
                &ImageHandle,
                &gAmdCcxAcpiCratServicesProtocolGuid,
                EFI_NATIVE_INTERFACE,
                &mZen3CratServicesProtocol
                );
}

EFI_STATUS
EFIAPI
CcxZen3SratServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  // Install ACPI CPU SRAT services protocol
  return gBS->InstallProtocolInterface (
                &ImageHandle,
                &gAmdCcxAcpiSratServicesProtocolGuid,
                EFI_NATIVE_INTERFACE,
                &mZen3SratServicesProtocol
                );
}

EFI_STATUS
EFIAPI
CcxZen3PcctServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return gBS->InstallProtocolInterface (
                &ImageHandle,
                &gAmdCcxAcpiPcctServicesProtocolGuid,
                EFI_NATIVE_INTERFACE,
                &mZen3PcctServicesProtocol
                );
}

EFI_STATUS
EFIAPI
CcxZen3CppcServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return gBS->InstallProtocolInterface (
                &ImageHandle,
                &gAmdCcxAcpiCppcServicesProtocolGuid,
                EFI_NATIVE_INTERFACE,
                &mZen3CppcServicesProtocol
                );
}
/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL
 *----------------------------------------------------------------------------------------
 */

/**
 *---------------------------------------------------------------------------------------
 *
 *  CcxZen3PStateGatherData
 *
 *  Description:
 *    This function will gather PState information from the MSRs and fill up the
 *    pStateBuf. This buffer will be used by the PState table generation code later.
 *
 *  Parameters:
 *    @param[in]  This                                 A pointer to the AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL instance.
 *    @param[out] PstateSysInfoPtr                     Contains Pstate information for whole system
 *
 *    @returns    EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EFIAPI
CcxZen3PStateGatherData (
  IN       AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL    *This,
     OUT   AMD_PSTATE_SYS_INFO                   **PstateSysInfoPtr
  )
{
  UINT32                                    LApicIdLoop;
  UINT32                                    TableSize;
  UINT32                                    PstateLoop;
  UINT32                                    TotalEnabledPStates;
  UINTN                                     NumberOfCcds;
  UINTN                                     NumberOfComplexes;
  UINTN                                     NumberOfCores;
  UINTN                                     NumberOfThreads;
  UINTN                                     CcdLoop;
  UINTN                                     ComplexLoop;
  UINTN                                     CoreLoop;
  UINTN                                     ThreadLoop;
  UINTN                                     LogicalCoreNum;
  UINTN                                     MaxSwState;
  UINTN                                     NumberOfBoostPstate;
  UINTN                                     Frequency;
  UINTN                                     VoltageInuV;
  UINTN                                     PowerInmW;
  UINTN                                     NumberOfSockets;
  UINTN                                     NumberOfDies;
  UINTN                                     SocketLoop;
  UINTN                                     DieLoop;
  AMD_PSTATE_SOCKET_INFO                   *PstateSocketInfo;
  S_PSTATE_VALUES                          *PstateStructure;
  ALLOCATE_HEAP_PARAMS                      AllocHeapParams;
  EFI_STATUS                                Status;
  AGESA_STATUS                              AgesaStatus;
  AMD_CONFIG_PARAMS                         StdHeader;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL   *FabricTopology;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL   *CoreTopology;

  CcxGetPstateNumber (0, &NumberOfBoostPstate, &MaxSwState, &StdHeader);
  MaxSwState = MaxSwState - NumberOfBoostPstate;

  Status = gBS->LocateProtocol (&gAmdFabricTopologyServices2ProtocolGuid, NULL, &FabricTopology);
  ASSERT (!EFI_ERROR (Status));

  Status = gBS->LocateProtocol (&gAmdCoreTopologyServicesV2ProtocolGuid, NULL, &CoreTopology);
  ASSERT (!EFI_ERROR (Status));

  if (FabricTopology->GetSystemInfo (FabricTopology, &NumberOfSockets, NULL, NULL, NULL, NULL) != EFI_SUCCESS) {
    return EFI_DEVICE_ERROR;
  }

  // Create Heap and store p-state data for ACPI table
  TableSize = (UINT32) (sizeof (AMD_PSTATE_SYS_INFO) + MultU64x64 ((MultU64x64 (MaxSwState, sizeof (S_PSTATE_VALUES)) + sizeof (AMD_PSTATE_SOCKET_INFO)), NumberOfSockets));

  AllocHeapParams.RequestedBufferSize = TableSize;
  AllocHeapParams.BufferHandle        = AMD_PSTATE_DATA_BUFFER_HANDLE;
  AllocHeapParams.Persist             = HEAP_SYSTEM_MEM;
  AgesaStatus = HeapAllocateBuffer (&AllocHeapParams, &StdHeader);
  ASSERT (AgesaStatus == AGESA_SUCCESS);
  if (AgesaStatus == AGESA_SUCCESS) {
    *PstateSysInfoPtr = (AMD_PSTATE_SYS_INFO *) AllocHeapParams.BufferPtr;
  } else {
    return EFI_OUT_OF_RESOURCES;
  }

  (*PstateSysInfoPtr)->TotalSocketInSystem = (UINT8) NumberOfSockets;
  (*PstateSysInfoPtr)->SizeOfBytes    = TableSize;
  PstateSocketInfo = (*PstateSysInfoPtr)->PStateSocketStruct;

  for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
    // Calculate number of logical cores
    LogicalCoreNum = 0;
    LApicIdLoop = 0;
    if (FabricTopology->GetProcessorInfo (FabricTopology, SocketLoop, &NumberOfDies, NULL) == EFI_SUCCESS) {
      for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
        if (CoreTopology->GetCoreTopologyOnDie (CoreTopology, SocketLoop, DieLoop, &NumberOfCcds, &NumberOfComplexes, &NumberOfCores, &NumberOfThreads) == EFI_SUCCESS) {
          LogicalCoreNum += NumberOfCcds * NumberOfComplexes * NumberOfCores * NumberOfThreads;
          for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
            for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
              for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
                for (ThreadLoop = 0; ThreadLoop < NumberOfThreads; ThreadLoop++) {
                  PstateSocketInfo->LocalApicId[LApicIdLoop] = (UINT8) CcxCalcLocalApic (SocketLoop, DieLoop, CcdLoop, ComplexLoop, CoreLoop, ThreadLoop);
                  LApicIdLoop++;
                }
              }
            }
          }
        }
      }
    }

    PstateSocketInfo->SocketNumber        = (UINT8) SocketLoop;
    PstateSocketInfo->TotalLogicalCores   = (UINT8) LogicalCoreNum;
    PstateSocketInfo->CreateAcpiTables    = TRUE;
    PstateSocketInfo->PStateMaxValue      = (UINT8) MaxSwState;

    // Get transition latency
    CcxZen3GetPstateTransLatency (&(PstateSocketInfo->TransitionLatency), &StdHeader);

    // Get IsPsdDependent
    // Family 19h defaults to dependent PSD
    switch (PcdGet8 (PcdAmdAgesaPstatePolicy)) {
    case 0:
      PstateSocketInfo->IsPsdDependent = FALSE;
      break;
    case 1:
      PstateSocketInfo->IsPsdDependent = TRUE;
      break;
    case 2:
      PstateSocketInfo->IsPsdDependent = FALSE;
      break;
    default:
      ASSERT (FALSE);
      break;
    }

    PstateStructure = PstateSocketInfo->PStateStruct;
    TotalEnabledPStates = 0;

    for (PstateLoop = 0; PstateLoop <= MaxSwState; PstateLoop++) {

      LibAmdMemFill (PstateStructure, 0, sizeof (S_PSTATE_VALUES), &StdHeader);

      if (CcxGetPstateInfo (0, (SwPstate0 + PstateLoop), &Frequency, &VoltageInuV, &PowerInmW, &StdHeader)) {
        PstateStructure->CoreFreq       = (UINT32) Frequency;
        PstateStructure->Power          = (UINT32) PowerInmW;
        PstateStructure->SwPstateNumber = PstateLoop;
        PstateStructure->PStateEnable   = 1;
        PstateStructure++;
        TotalEnabledPStates++;
      }
    } // for (PstateLoop = 0; PstateLoop < MaxState; PstateLoop++)

    // Don't create ACPI Tables if there is one or less than one PState is enabled
    if (TotalEnabledPStates <= 1) {
      PstateSocketInfo->CreateAcpiTables = FALSE;
    }

    PstateSocketInfo = (AMD_PSTATE_SOCKET_INFO *) ((UINT8 *) PstateSocketInfo + sizeof (AMD_PSTATE_SOCKET_INFO) + sizeof (S_PSTATE_VALUES) * MaxSwState);
  }

  return EFI_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to get Pstate Transition Latency.
 *
 *  Calculate TransitionLatency by Gaters On/Off Time value and pll value.
 *
 *  @param[out]    TransitionLatency                 The transition latency.
 *  @param[in]     StdHeader                         Header for library and services
 *
 *  @retval        AGESA_SUCCESS Always succeeds.
 */
AGESA_STATUS
CcxZen3GetPstateTransLatency (
     OUT   UINT32                                 *TransitionLatency,
  IN       AMD_CONFIG_PARAMS                      *StdHeader
  )
{
  *TransitionLatency = 0;

  return (AGESA_SUCCESS);
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  CcxZen3GetCStateInfo
 *
 *  Description:
 *    This function will gather CState information
 *
 *  Parameters:
 *    @param[in]  This                                 A pointer to the AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL instance.
 *    @param[out] CstateInfo                           Contains Cstate information
 *
 *    @returns    EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EFIAPI
CcxZen3GetCStateInfo (
  IN       AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL    *This,
     OUT   AMD_CSTATE_INFO                       **CstateInfo
  )
{
  AGESA_STATUS          AgesaStatus;
  ALLOCATE_HEAP_PARAMS  AllocHeapParams;

  AllocHeapParams.RequestedBufferSize = sizeof (AMD_CSTATE_INFO);
  AllocHeapParams.BufferHandle        = AMD_CSTATE_DATA_BUFFER_HANDLE;
  AllocHeapParams.Persist             = HEAP_SYSTEM_MEM;
  AgesaStatus = HeapAllocateBuffer (&AllocHeapParams, NULL);
  ASSERT (AgesaStatus == AGESA_SUCCESS);
  if (AgesaStatus == AGESA_SUCCESS) {
    *CstateInfo = (AMD_CSTATE_INFO *) AllocHeapParams.BufferPtr;
  } else {
    return EFI_OUT_OF_RESOURCES;
  }

  // Is Cstate enabled
  if ((PcdGet8 (PcdAmdCStateMode) != 1) ||
      (PcdGet16 (PcdAmdCStateIoBaseAddress) == 0)) {
    (*CstateInfo)->IsCstateEnabled = FALSE;
  } else {
    (*CstateInfo)->IsCstateEnabled = TRUE;
  }

  // Io Cstate address
  (*CstateInfo)->IoCstateAddr = (UINT32) PcdGet16 (PcdAmdCStateIoBaseAddress);

  (*CstateInfo)->IsCsdGenerated = (BOOLEAN) (CcxGetThreadsPerCore () > 1);

  (*CstateInfo)->IsMonitorMwaitSupported = TRUE;

  (*CstateInfo)->C1Latency = (UINT16) PcdGet16 (PcdAmdAcpiCpuCstC1Latency);

  (*CstateInfo)->C2Latency = (UINT16) PcdGet16 (PcdAmdAcpiCpuCstC2Latency);

  (*CstateInfo)->C3Latency = (UINT16) PcdGet16 (PcdAmdAcpiCpuCstC3Latency);

  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  CcxZen3GetPsdDomain
 *
 *  Description:
 *    This function will return PSD domain for independency
 *
 *  Parameters:
 *    @param[in]  This                                 A pointer to the AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL instance.
 *    @param[in]  LocalApicId                          Local APIC ID
 *
 *    @returns    _PSD domain for the given core
 *
 *---------------------------------------------------------------------------------------
 **/
UINT32
EFIAPI
CcxZen3GetPsdDomain (
  IN       AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL    *This,
  IN       UINT32                                  LocalApicId
  )
{
  UINT8  ThreadsPerCore;

  ThreadsPerCore = CcxGetThreadsPerCore ();
  ASSERT (ThreadsPerCore != 0);

  return ((UINT32) (LocalApicId / ThreadsPerCore));
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  CcxZen3GetPStatePower
 *
 *  Description:
 *    This function will return the Power in mW for specified PState
 *
 *  Parameters:
 *    @param[in]  This                    Pointer to the AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL instance.
 *    @param[in]  Pstate                  PState to read
 *    @param[out] PowerInmW               Power in mW of the specified PState
 *
 *    @retval     EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EFIAPI
CcxZen3GetPStatePower (
  IN       AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL    *This,
  IN       CCX_PSTATE                             Pstate,
     OUT   UINTN                                  *PowerInmW
  )
{
  UINT32    CpuVid;
  UINT32    IddValue;
  UINT32    IddDiv;
  UINT32    PstateNumber;
  UINTN     VoltageInuV;
  PSTATE_MSR  PstateMsr;

  PstateNumber = Pstate < HwPstate0 ? (UINT32) Pstate : (UINT32) Pstate - HwPstate0;
  PstateMsr.Value = AsmReadMsr64 (MSR_PSTATE_0 + PstateNumber);

  if (PstateMsr.Field.PstateEn == 0) {
    return FALSE;
  }

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3GetPStatePower - P%d\n", PstateNumber);

  CpuVid   = (UINT32) PstateMsr.Field.CpuVid;
  IddValue = (UINT32) PstateMsr.Field.IddValue;
  IddDiv   = (UINT32) PstateMsr.Field.IddDiv;


  // Voltage
  if ((CpuVid >= 0xF8) && (CpuVid <= 0xFF)) {
    VoltageInuV = 0;
  } else {
    VoltageInuV = 1550000L - (6250 * CpuVid);
  }

  // PowerInmW
  *PowerInmW = (VoltageInuV) / 10 * IddValue;

  switch (IddDiv) {
  case 0:
    *PowerInmW = *PowerInmW / 100L;
    break;
  case 1:
    *PowerInmW = *PowerInmW / 1000L;
    break;
  case 2:
    *PowerInmW = *PowerInmW / 10000L;
    break;
  default:
    // IddDiv is set to an undefined value.  This is due to either a misfused CPU, or
    // an invalid P-state MSR write.
    ASSERT (FALSE);
    *PowerInmW = 0;
    break;
  }
  return TRUE;
}


/*----------------------------------------------------------------------------------------
 *                          AMD_CCX_ACPI_CRAT_SERVICES_PROTOCOL
 *----------------------------------------------------------------------------------------
 */

/**
 * This service retrieves CRAT information about the HSA.
 *
 * @param[in]      This                             A pointer to the
 *                                                  AMD_CCX_ACPI_CRAT_SERVICES_PROTOCOL instance.
 * @param[in]      CratHeaderStructPtr              CRAT table structure pointer
 * @param[in, out] TableEnd                         Point to the end of this table
 *
 * @retval EFI_SUCCESS                              The HSA processor information was successfully retrieved.
 * @retval EFI_INVALID_PARAMETER                    CratHsaProcInfo is NULL.
 *
 **/
EFI_STATUS
EFIAPI
CcxZen3GetCratHsaProcEntry (
  IN       AMD_CCX_ACPI_CRAT_SERVICES_PROTOCOL    *This,
  IN       CRAT_HEADER                            *CratHeaderStructPtr,
  IN OUT   UINT8                                 **TableEnd
  )
{
  UINTN                                    SocketLoop;
  UINTN                                    DieLoop;
  UINTN                                    NumberOfSockets;
  UINTN                                    NumberOfDies;
  UINTN                                    NumberOfCcds;
  UINTN                                    NumberOfComplexes;
  UINTN                                    NumberOfCores;
  UINTN                                    NumberOfThreads;
  UINT32                                   Domain;
  UINT32                                   PreDomain;
  EFI_STATUS                               CalledStatus;
  CRAT_HSA_PROCESSING_UNIT                *CratHsaEntry;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL  *FabricTopology;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL  *CoreTopologyServices;
  FABRIC_NUMA_SERVICES2_PROTOCOL          *FabricNuma;

  // Locate Fabric CRAT Services Protocol
  if (gBS->LocateProtocol (&gAmdFabricNumaServices2ProtocolGuid, NULL, (VOID **) &FabricNuma) != EFI_SUCCESS) {
    return EFI_ABORTED;
  }

  // Locate FabricTopologyServices2Protocol
  CalledStatus = gBS->LocateProtocol (&gAmdFabricTopologyServices2ProtocolGuid, NULL, (VOID **) &FabricTopology);
  ASSERT (!EFI_ERROR (CalledStatus));

  CalledStatus = gBS->LocateProtocol (&gAmdCoreTopologyServicesV2ProtocolGuid, NULL, &CoreTopologyServices);
  ASSERT (!EFI_ERROR (CalledStatus));

  PreDomain = 0;
  CratHsaEntry = NULL;
  if (FabricTopology->GetSystemInfo (FabricTopology, &NumberOfSockets, NULL, NULL, NULL, NULL) == EFI_SUCCESS) {
    for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
      if (FabricTopology->GetProcessorInfo (FabricTopology, SocketLoop, &NumberOfDies, NULL) == EFI_SUCCESS) {
        for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
          if (FabricNuma->DomainXlat (FabricNuma, SocketLoop, DieLoop, 0, 0, &Domain) ==  EFI_SUCCESS) {
            if ((Domain != PreDomain) || (Domain == 0)) {
              CratHsaEntry = (CRAT_HSA_PROCESSING_UNIT *) AddOneCratEntry (CRAT_HSA_PROC_UNIT_TYPE, CratHeaderStructPtr, TableEnd);
              CratHsaEntry->Flags.Enabled    = 1;
              CratHsaEntry->Flags.CpuPresent = 1;
              CratHsaEntry->ProximityNode    = Domain;
              CratHsaEntry->ProcessorIdLow   = (UINT8) CcxCalcLocalApic (SocketLoop, DieLoop, 0, 0, 0, 0);
              CratHsaEntry->NumCPUCores      = 0;
              CratHsaEntry->WaveFrontSize    = 4;
              PreDomain = Domain;
            }
            CoreTopologyServices->GetCoreTopologyOnDie (CoreTopologyServices, SocketLoop, DieLoop, &NumberOfCcds, &NumberOfComplexes, &NumberOfCores, &NumberOfThreads);
            CratHsaEntry->NumCPUCores += (UINT16) (NumberOfCcds * NumberOfComplexes * NumberOfCores * NumberOfThreads);
          }
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**
 * This service retrieves information about the cache.
 *
 * @param[in]      This                             A pointer to the
 *                                                  AMD_CCX_ACPI_CRAT_SERVICES_PROTOCOL instance.
 * @param[in]      CratHeaderStructPtr              CRAT table structure pointer
 * @param[in, out] TableEnd                         Point to the end of this table
 *
 * @retval EFI_SUCCESS                              The cache information was successfully retrieved.
 * @retval EFI_INVALID_PARAMETER                    CratCacheInfo is NULL.
 *
 **/
EFI_STATUS
EFIAPI
CcxZen3GetCratCacheEntry (
  IN       AMD_CCX_ACPI_CRAT_SERVICES_PROTOCOL    *This,
  IN       CRAT_HEADER                            *CratHeaderStructPtr,
  IN OUT   UINT8                                 **TableEnd
  )
{
  UINT8                                    i;
  UINT8                                    ApicId;
  UINT8                                    SiblingMapMask;
  UINT32                                   NumOfThreadsSharing;
  UINT32                                   TotalThreads;
  UINTN                                    SocketLoop;
  UINTN                                    DieLoop;
  UINTN                                    CcdLoop;
  UINTN                                    ComplexLoop;
  UINTN                                    CoreLoop;
  UINTN                                    ThreadsLoop;
  UINTN                                    NumberOfSockets;
  UINTN                                    NumberOfDies;
  UINTN                                    NumberOfCcds;
  UINTN                                    NumberOfComplexes;
  UINTN                                    NumberOfCores;
  UINTN                                    NumberOfThreads;
  UINT32                                   CacheProp;
  EFI_STATUS                               CalledStatus;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL  *FabricTopology;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL  *CoreTopologyServices;
  CRAT_CACHE                              *CratCacheEntry;
  CPUID_CACHE_PROP                         CacheProperties;
  CPUID_EXT_FEAT_MSR                       ExtFeat;
  BOOLEAN                                  DisTopoExt;

  // Locate FabricTopologyServices2Protocol
  CalledStatus = gBS->LocateProtocol (&gAmdFabricTopologyServices2ProtocolGuid, NULL, (VOID **) &FabricTopology);
  ASSERT (!EFI_ERROR (CalledStatus));

  CalledStatus = gBS->LocateProtocol (&gAmdCoreTopologyServicesV2ProtocolGuid, NULL, &CoreTopologyServices);
  ASSERT (!EFI_ERROR (CalledStatus));

  ExtFeat.Value = AsmReadMsr64 (MSR_CPUID_EXT_FEAT);
  if (ExtFeat.Field.TopologyExtensions == 0) {
    DisTopoExt = TRUE;
    ExtFeat.Field.TopologyExtensions = 1;
    AsmWriteMsr64 (MSR_CPUID_EXT_FEAT, ExtFeat.Value);
  } else {
    DisTopoExt = FALSE;
  }

  TotalThreads = 0;
  if (FabricTopology->GetSystemInfo (FabricTopology, &NumberOfSockets, NULL, NULL, NULL, NULL) == EFI_SUCCESS) {
    for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
      if (FabricTopology->GetProcessorInfo (FabricTopology, SocketLoop, &NumberOfDies, NULL) == EFI_SUCCESS) {
        for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
          if (CoreTopologyServices->GetCoreTopologyOnDie (CoreTopologyServices, SocketLoop, DieLoop, &NumberOfCcds, &NumberOfComplexes, &NumberOfCores, &NumberOfThreads) == EFI_SUCCESS) {
            for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
              for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
                for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
                  for (ThreadsLoop = 0; ThreadsLoop < NumberOfThreads; ThreadsLoop++) {
                    ApicId = (UINT8) CcxCalcLocalApic (SocketLoop, DieLoop, CcdLoop, ComplexLoop, CoreLoop, ThreadsLoop);
                    CacheProp = 0;
                    do {
                      AsmCpuidEx (CPUID_CACHE_PROPERTIES,
                                  CacheProp++,
                                  &(CacheProperties.EAX.Value),
                                  &(CacheProperties.EBX.Value),
                                  &(CacheProperties.ECX.Value),
                                  &(CacheProperties.EDX.Value));
                      if (CacheProperties.EAX.Field.CacheType != 0) {
                        NumOfThreadsSharing = CacheProperties.EAX.Field.NumSharingCache + 1;
                        if ((TotalThreads % NumOfThreadsSharing) == 0) {
                          CratCacheEntry = (CRAT_CACHE *) AddOneCratEntry (CRAT_CACHE_TYPE, CratHeaderStructPtr, TableEnd);
                          CratCacheEntry->Flags.Enabled   = 1;
                          CratCacheEntry->Flags.CpuCache  = 1;
                          CratCacheEntry->Flags.DataCache = (CacheProperties.EAX.Field.CacheType != 2) ? 1 : 0;
                          CratCacheEntry->Flags.InstructionCache = (CacheProperties.EAX.Field.CacheType != 1) ? 1 : 0;
                          CratCacheEntry->ProcessorIdLow  = ApicId;
                          SiblingMapMask = 1;
                          for (i = 1; i < NumOfThreadsSharing; i++) {
                            SiblingMapMask = (SiblingMapMask << 1) + 1;
                          }
                          i = (UINT8) (TotalThreads / BITS_PER_BYTE);
                          CratCacheEntry->SiblingMap[i]   = SiblingMapMask << (TotalThreads % BITS_PER_BYTE);
                          CratCacheEntry->CacheProperties = (CacheProperties.EDX.Field.CacheInclusive == 0) ? 0 : 2;
                          CratCacheEntry->CacheSize       = (((CacheProperties.ECX.Field.CacheNumSets + 1) * (CacheProperties.EBX.Field.CacheNumWays + 1) * (CacheProperties.EBX.Field.CacheLineSize + 1)) >> KB_SHIFT);
                          CratCacheEntry->CacheLevel      = (UINT8) CacheProperties.EAX.Field.CacheLevel;
                          CratCacheEntry->CacheLineSize   = (UINT16) (CacheProperties.EBX.Field.CacheLineSize + 1);
                          CratCacheEntry->Associativity   = (UINT8) ((CacheProperties.EAX.Field.FullyAssociative == 1) ? 0xFF : (CacheProperties.EBX.Field.CacheNumWays + 1));
                          CratCacheEntry->CacheLatency    = 1;
                          CratCacheEntry->LinesPerTag     = 1;
                        }
                      }
                    } while (CacheProperties.EAX.Field.CacheType != 0);
                    TotalThreads++;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  if (DisTopoExt) {
    ExtFeat.Field.TopologyExtensions = 0;
    AsmWriteMsr64 (MSR_CPUID_EXT_FEAT, ExtFeat.Value);
  }

  return EFI_SUCCESS;
}

/**
 * This service retrieves information about the TLB.
 *
 * @param[in]      This                             A pointer to the
 *                                                  AMD_CCX_ACPI_CRAT_SERVICES_PROTOCOL instance.
 * @param[in]      CratHeaderStructPtr              CRAT table structure pointer
 * @param[in, out] TableEnd                         Point to the end of this table
 *
 * @retval EFI_SUCCESS                              The TLB information was successfully retrieved.
 * @retval EFI_INVALID_PARAMETER                    CratTlbInfo is NULL.
 *
 **/
EFI_STATUS
EFIAPI
CcxZen3GetCratTlbEntry (
  IN       AMD_CCX_ACPI_CRAT_SERVICES_PROTOCOL    *This,
  IN       CRAT_HEADER                            *CratHeaderStructPtr,
  IN OUT   UINT8                                 **TableEnd
  )
{
  UINT8                                    i;
  UINT8                                    ApicId;
  UINT8                                    SiblingMapMask;
  UINT32                                   NumOfThreadsSharing;
  UINT32                                   TotalThreads;
  UINTN                                    SocketLoop;
  UINTN                                    DieLoop;
  UINTN                                    CcdLoop;
  UINTN                                    ComplexLoop;
  UINTN                                    CoreLoop;
  UINTN                                    ThreadsLoop;
  UINTN                                    NumberOfSockets;
  UINTN                                    NumberOfDies;
  UINTN                                    NumberOfCcds;
  UINTN                                    NumberOfComplexes;
  UINTN                                    NumberOfCores;
  UINTN                                    NumberOfThreads;
  CPUID_DATA                               CpuId;
  EFI_STATUS                               CalledStatus;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL  *FabricTopology;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL  *CoreTopologyServices;
  CRAT_TLB                                *CratTlbEntry;

  // Locate FabricTopologyServices2Protocol
  CalledStatus = gBS->LocateProtocol (&gAmdFabricTopologyServices2ProtocolGuid, NULL, (VOID **) &FabricTopology);
  ASSERT (!EFI_ERROR (CalledStatus));

  CalledStatus = gBS->LocateProtocol (&gAmdCoreTopologyServicesV2ProtocolGuid, NULL, &CoreTopologyServices);
  ASSERT (!EFI_ERROR (CalledStatus));

  TotalThreads = 0;
  if (FabricTopology->GetSystemInfo (FabricTopology, &NumberOfSockets, NULL, NULL, NULL, NULL) == EFI_SUCCESS) {
    for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
      if (FabricTopology->GetProcessorInfo (FabricTopology, SocketLoop, &NumberOfDies, NULL) == EFI_SUCCESS) {
        for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
          if (CoreTopologyServices->GetCoreTopologyOnDie (CoreTopologyServices, SocketLoop, DieLoop, &NumberOfCcds, &NumberOfComplexes, &NumberOfCores, &NumberOfThreads) == EFI_SUCCESS) {
            for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
              for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
                for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
                  for (ThreadsLoop = 0; ThreadsLoop < NumberOfThreads; ThreadsLoop++) {
                    ApicId = (UINT8) CcxCalcLocalApic (SocketLoop, DieLoop, CcdLoop, ComplexLoop, CoreLoop, ThreadsLoop);

                    // L1 Data TLB
                    AsmCpuidEx (0x8000001D, 0, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
                    NumOfThreadsSharing = (((CpuId.EAX_Reg >> 14) & 0xFFF) + 1);
                    if ((TotalThreads % NumOfThreadsSharing) == 0) {
                      CratTlbEntry = (CRAT_TLB *) AddOneCratEntry (CRAT_TLB_TYPE, CratHeaderStructPtr, TableEnd);
                      CratTlbEntry->Flags.Enabled     = 1;
                      CratTlbEntry->Flags.DataTLB     = 1;
                      CratTlbEntry->Flags.CpuTLB      = 1;
                      CratTlbEntry->ProcessorIdLow    = ApicId;
                      SiblingMapMask = 1;
                      for (i = 1; i < NumOfThreadsSharing; i++) {
                        SiblingMapMask = (SiblingMapMask << 1) + 1;
                      }
                      i = (UINT8) (TotalThreads / 8);
                      CratTlbEntry->SiblingMap[i]     = SiblingMapMask << (TotalThreads % 8);
                      CratTlbEntry->TlbLevel          = L1_CACHE;
                      AsmCpuid (AMD_CPUID_TLB_L1Cache, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
                      CratTlbEntry->DataTlbAssociativity2MB = CpuId.EAX_Reg >> 24;
                      CratTlbEntry->DataTlbSize2MB    = GetTlbSize (TLB_2M, CratTlbEntry, ((CpuId.EAX_Reg >> 16) & 0xFF));
                      CratTlbEntry->DTLB4KAssoc       = CpuId.EBX_Reg >> 24;
                      CratTlbEntry->DTLB4KSize        = GetTlbSize (TLB_4K, CratTlbEntry, ((CpuId.EBX_Reg >> 16) & 0xFF));
                      AsmCpuid (AMD_CPUID_L1L2Tlb1GIdentifiers, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
                      CratTlbEntry->DTLB1GAssoc       = CpuId.EAX_Reg >> 28;
                      CratTlbEntry->DTLB1GSize        = GetTlbSize (TLB_1G, CratTlbEntry, ((CpuId.EAX_Reg >> 16) & 0xFFF));
                    }

                    // L1 Instruction TLB
                    AsmCpuidEx (0x8000001D, 1, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
                    NumOfThreadsSharing = (((CpuId.EAX_Reg >> 14) & 0xFFF) + 1);
                    if ((TotalThreads % NumOfThreadsSharing) == 0) {
                      CratTlbEntry = (CRAT_TLB *) AddOneCratEntry (CRAT_TLB_TYPE, CratHeaderStructPtr, TableEnd);
                      CratTlbEntry->Flags.Enabled     = 1;
                      CratTlbEntry->Flags.InstructionTLB = 1;
                      CratTlbEntry->Flags.CpuTLB      = 1;
                      CratTlbEntry->ProcessorIdLow    = ApicId;
                      SiblingMapMask = 1;
                      for (i = 1; i < NumOfThreadsSharing; i++) {
                        SiblingMapMask = (SiblingMapMask << 1) + 1;
                      }
                      i = (UINT8) (TotalThreads / 8);
                      CratTlbEntry->SiblingMap[i]     = SiblingMapMask << (TotalThreads % 8);
                      CratTlbEntry->TlbLevel          = L1_CACHE;
                      AsmCpuid (AMD_CPUID_TLB_L1Cache, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
                      CratTlbEntry->InstructionTlbAssociativity2MB = (CpuId.EAX_Reg >> 8) & 0xFF;
                      CratTlbEntry->InstructionTlbSize2MB = GetTlbSize (TLB_2M, CratTlbEntry, (CpuId.EAX_Reg & 0xFF));
                      CratTlbEntry->ITLB4KAssoc       = (CpuId.EBX_Reg >> 8) & 0xFF;
                      CratTlbEntry->ITLB4KSize        = GetTlbSize (TLB_4K, CratTlbEntry, (CpuId.EBX_Reg & 0xFF));
                      AsmCpuid (AMD_CPUID_L1L2Tlb1GIdentifiers, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
                      CratTlbEntry->ITLB1GAssoc       = (CpuId.EAX_Reg >> 12) & 0xF;
                      CratTlbEntry->ITLB1GSize        = GetTlbSize (TLB_1G, CratTlbEntry, (CpuId.EAX_Reg & 0xFFF));
                    }

                    // L2 Data TLB
                    AsmCpuidEx (0x8000001D, 2, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
                    NumOfThreadsSharing = (((CpuId.EAX_Reg >> 14) & 0xFFF) + 1);
                    if ((TotalThreads % NumOfThreadsSharing) == 0) {
                      CratTlbEntry = (CRAT_TLB *) AddOneCratEntry (CRAT_TLB_TYPE, CratHeaderStructPtr, TableEnd);
                      CratTlbEntry->Flags.Enabled     = 1;
                      CratTlbEntry->Flags.DataTLB     = 1;
                      CratTlbEntry->Flags.CpuTLB      = 1;
                      CratTlbEntry->ProcessorIdLow    = ApicId;
                      SiblingMapMask = 1;
                      for (i = 1; i < NumOfThreadsSharing; i++) {
                        SiblingMapMask = (SiblingMapMask << 1) + 1;
                      }
                      i = (UINT8) (TotalThreads / 8);
                      CratTlbEntry->SiblingMap[i]     = SiblingMapMask << (TotalThreads % 8);
                      CratTlbEntry->TlbLevel          = L2_CACHE;
                      AsmCpuid (AMD_CPUID_L2L3Cache_L2TLB, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
                      CratTlbEntry->DataTlbAssociativity2MB = CpuId.EAX_Reg >> 28;
                      CratTlbEntry->DataTlbSize2MB    = GetTlbSize (TLB_2M, CratTlbEntry, ((CpuId.EAX_Reg >> 16) & 0xFFF));
                      CratTlbEntry->DTLB4KAssoc       = GetCacheAssoc (CpuId.EBX_Reg >> 28);
                      CratTlbEntry->DTLB4KSize        = GetTlbSize (TLB_4K, CratTlbEntry, ((CpuId.EBX_Reg >> 16) & 0xFFF));
                      AsmCpuid (AMD_CPUID_L1L2Tlb1GIdentifiers, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
                      CratTlbEntry->DTLB1GAssoc       = GetCacheAssoc (CpuId.EBX_Reg >> 28);
                      CratTlbEntry->DTLB1GSize        = GetTlbSize (TLB_1G, CratTlbEntry, ((CpuId.EAX_Reg >> 16) & 0xFFF));
                    }

                    // L2 Instruction TLB
                    AsmCpuidEx (0x8000001D, 2, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
                    NumOfThreadsSharing = (((CpuId.EAX_Reg >> 14) & 0xFFF) + 1);
                    if ((TotalThreads % NumOfThreadsSharing) == 0) {
                      CratTlbEntry = (CRAT_TLB *) AddOneCratEntry (CRAT_TLB_TYPE, CratHeaderStructPtr, TableEnd);
                      CratTlbEntry->Flags.Enabled     = 1;
                      CratTlbEntry->Flags.InstructionTLB = 1;
                      CratTlbEntry->Flags.CpuTLB      = 1;
                      CratTlbEntry->ProcessorIdLow    = ApicId;
                      SiblingMapMask = 1;
                      for (i = 1; i < NumOfThreadsSharing; i++) {
                        SiblingMapMask = (SiblingMapMask << 1) + 1;
                      }
                      i = (UINT8) (TotalThreads / 8);
                      CratTlbEntry->SiblingMap[i]     = SiblingMapMask << (TotalThreads % 8);
                      CratTlbEntry->TlbLevel          = L2_CACHE;
                      AsmCpuid (AMD_CPUID_L2L3Cache_L2TLB, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
                      CratTlbEntry->InstructionTlbAssociativity2MB = GetCacheAssoc ((CpuId.EAX_Reg >> 12) & 0xF);
                      CratTlbEntry->InstructionTlbSize2MB = GetTlbSize (TLB_2M, CratTlbEntry, (CpuId.EAX_Reg & 0xFFF));
                      CratTlbEntry->ITLB4KAssoc       = GetCacheAssoc ((CpuId.EBX_Reg >> 12) & 0xF);
                      CratTlbEntry->ITLB4KSize        = GetTlbSize (TLB_4K, CratTlbEntry, (CpuId.EBX_Reg & 0xFFF));
                      AsmCpuid (AMD_CPUID_L1L2Tlb1GIdentifiers, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
                      CratTlbEntry->ITLB1GAssoc       = GetCacheAssoc ((CpuId.EBX_Reg >> 12) & 0xF);
                      CratTlbEntry->ITLB1GSize        = GetTlbSize (TLB_1G, CratTlbEntry, (CpuId.EBX_Reg & 0xFFF));
                    }

                    // No L3 TLB

                    TotalThreads++;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return EFI_SUCCESS;
}


/*----------------------------------------------------------------------------------------
 *                          AMD_CCX_ACPI_SRAT_SERVICES_PROTOCOL
 *----------------------------------------------------------------------------------------
 */

/**
 * This service create SRAT Local APIC structure
 *
 * @param[in]      This                             A pointer to the
 *                                                  AMD_CCX_ACPI_SRAT_SERVICES_PROTOCOL instance.
 * @param[in]      SratHeaderStructPtr              SRAT table structure pointer
 * @param[in, out] TableEnd                         Point to the end of this table
 *
 * @retval EFI_SUCCESS                              The LAPIC was successfully created.
 *
 **/
EFI_STATUS
EFIAPI
CcxZen3CreateSratApicEntry (
  IN       AMD_CCX_ACPI_SRAT_SERVICES_PROTOCOL    *This,
  IN       SRAT_HEADER                            *SratHeaderStructPtr,
  IN OUT   UINT8                                 **TableEnd
  )
{
  UINT8                                    ApicId;
  UINTN                                    SocketLoop;
  UINTN                                    DieLoop;
  UINTN                                    CcdLoop;
  UINTN                                    ComplexLoop;
  UINTN                                    CoreLoop;
  UINTN                                    ThreadsLoop;
  UINTN                                    NumberOfSockets;
  UINTN                                    NumberOfDies;
  UINTN                                    NumberOfCcds;
  UINTN                                    NumberOfComplexes;
  UINTN                                    NumberOfCores;
  UINTN                                    NumberOfThreads;
  UINT32                                   Domain;
  SRAT_APIC                               *ApicEntry;
  SRAT_x2APIC                             *X2ApicEntry;
  EFI_STATUS                               CalledStatus;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL  *FabricTopology;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL  *CoreTopologyServices;
  FABRIC_NUMA_SERVICES2_PROTOCOL          *FabricNuma;
  UINT8                                   ApicMode;

  // Locate Fabric SRAT Services Protocol
  if (gBS->LocateProtocol (&gAmdFabricNumaServices2ProtocolGuid, NULL, (VOID **) &FabricNuma) != EFI_SUCCESS) {
    return EFI_ABORTED;
  }

  // Locate FabricTopologyServices2Protocol
  CalledStatus = gBS->LocateProtocol (&gAmdFabricTopologyServices2ProtocolGuid, NULL, (VOID **) &FabricTopology);
  ASSERT (!EFI_ERROR (CalledStatus));

  CalledStatus = gBS->LocateProtocol (&gAmdCoreTopologyServicesV2ProtocolGuid, NULL, &CoreTopologyServices);
  ASSERT (!EFI_ERROR (CalledStatus));

  // get Apic Mode
  ApicMode = PcdGet8 (PcdAmdApicMode);

  if (FabricTopology->GetSystemInfo (FabricTopology, &NumberOfSockets, NULL, NULL, NULL, NULL) == EFI_SUCCESS) {
    for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
      if (FabricTopology->GetProcessorInfo (FabricTopology, SocketLoop, &NumberOfDies, NULL) == EFI_SUCCESS) {
        for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
          if (CoreTopologyServices->GetCoreTopologyOnDie (CoreTopologyServices, SocketLoop, DieLoop, &NumberOfCcds, &NumberOfComplexes, &NumberOfCores, &NumberOfThreads) == EFI_SUCCESS) {
            for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
              for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
                if (FabricNuma->DomainXlat (FabricNuma, SocketLoop, DieLoop, CcdLoop, ComplexLoop, &Domain) ==  EFI_SUCCESS) {
                  for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
                    for (ThreadsLoop = 0; ThreadsLoop < NumberOfThreads; ThreadsLoop++) {
                      ApicId = (UINT8) CcxCalcLocalApic (SocketLoop, DieLoop, CcdLoop, ComplexLoop, CoreLoop, ThreadsLoop);
                      if ((ApicMode == x2ApicMode) || ((ApicMode == ApicCompatibilityMode) && (ApicId >= XAPIC_ID_MAX))) {
                        X2ApicEntry = (SRAT_x2APIC *) *TableEnd;
                        *TableEnd += sizeof (SRAT_x2APIC);

                        X2ApicEntry->Type = SRAT_LOCAL_X2_APIC_TYPE;
                        X2ApicEntry->Length = sizeof (SRAT_x2APIC);
                        X2ApicEntry->ProximityDomain = Domain;
                        X2ApicEntry->x2ApicId = ApicId;
                        X2ApicEntry->Flags.Enabled = 1;
                        X2ApicEntry->ClockDomain = 0;
                      } else {
                        ApicEntry = (SRAT_APIC *) *TableEnd;
                        *TableEnd += sizeof (SRAT_APIC);

                        ApicEntry->Type = SRAT_LOCAL_APIC_TYPE;
                        ApicEntry->Length = sizeof (SRAT_APIC);
                        ApicEntry->ProximityDomain_7_0 = (UINT8) Domain & 0xFF;
                        ApicEntry->ProximityDomain_31_8[0] = (UINT8) ((Domain >> 8) & 0xFF);
                        ApicEntry->ProximityDomain_31_8[1] = (UINT8) ((Domain >> 16) & 0xFF);
                        ApicEntry->ProximityDomain_31_8[2] = (UINT8) ((Domain >> 24) & 0xFF);
                        ApicEntry->ApicId = ApicId;
                        ApicEntry->Flags.Enabled = 1;
                        ApicEntry->LocalSapicEid = 0;
                        ApicEntry->ClockDomain = 0;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**
 * This service creates a PCC Subspace structure
 *
 * @param[in]      This                             Pointer to AMD_CCX_ACPI_PCCT_SERVICES_PROTCOL instance
 * @param[in]      PcctHeaderStructPtr              Pointer to the PCCT header struct
 * @param[in]      EndOfTable                       Pointer to the end of the PCCT table
 *
 * @retval EFI_SUCCESS                              The structure is successfully created
 *
 **/
EFI_STATUS
EFIAPI
CcxZen3AddGenCommSubspaceStruct (
  IN       AMD_CCX_ACPI_PCCT_SERVICES_PROTOCOL    *This,
  IN       PCCT_HEADER                            *PcctHeaderStructPtr,
  IN       AMD_CPPC_INFO                          *AmdCppcInfo,
  IN OUT   UINT8                                  **EndOfTable
  )
{
  UINT32                               *Pcc;
  UINT32                               MtrrIndex;
  UINT64                               MmioBase;
  UINT64                               MmioLength;
  MTRR_INFO                            MtrrInfo;
  FABRIC_TARGET                        MmioTarget;
  FABRIC_MMIO_ATTRIBUTE                Attrib;
  EFI_STATUS                           Status;
  EFI_PHYSICAL_ADDRESS                 SharedRegion;
  EFI_MP_SERVICES_PROTOCOL             *MpServices;
  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL   *NbioSmuServices;
  FABRIC_RESOURCE_MANAGER_PROTOCOL     *FabricResourceMgr;
  GEN_COMM_SUBSPACE_STRUCT             *GenCommSubspacePtr;
  DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL  *NbioCppcServices;
  ALLOCATE_HEAP_PARAMS                 AllocParams;
  BOOLEAN                              IsAnyProtocolNotFound;

  IDS_HDT_CONSOLE (MAIN_FLOW, "  CcxZen3AddGenCommSubspaceStruct Entry\n");
  Status = gBS->LocateProtocol (&gAmdNbioSmuServicesProtocolGuid, NULL, (VOID **) &NbioSmuServices);
  ASSERT (Status == EFI_SUCCESS);
  IsAnyProtocolNotFound = (Status != EFI_SUCCESS) ? TRUE : FALSE;
  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);
  ASSERT (Status == EFI_SUCCESS);
  IsAnyProtocolNotFound = (Status != EFI_SUCCESS) ? TRUE : IsAnyProtocolNotFound;
  Status = gBS->LocateProtocol (&gAmdFabricResourceManagerServicesProtocolGuid, NULL, &FabricResourceMgr);
  ASSERT (Status == EFI_SUCCESS);
  IsAnyProtocolNotFound = (Status != EFI_SUCCESS) ? TRUE : IsAnyProtocolNotFound;
  Status = gBS->LocateProtocol (&gAmdNbioCppcServicesProtocolGuid, NULL, &NbioCppcServices);
  ASSERT (Status == EFI_SUCCESS);
  IsAnyProtocolNotFound = (Status != EFI_SUCCESS) ? TRUE : IsAnyProtocolNotFound;
  if (IsAnyProtocolNotFound) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "  Returning error, protocols not found\n");
    return EFI_NOT_FOUND;
  }
  GenCommSubspacePtr = (GEN_COMM_SUBSPACE_STRUCT *) *EndOfTable;
  LibAmdMemCopy ((VOID *) GenCommSubspacePtr, (VOID *) &GenCommSubspaceStruct, (UINTN) (sizeof (GEN_COMM_SUBSPACE_STRUCT)), NULL);

  // Allocate block of physical memory to store CPPC shared memory
  if (gBS->AllocatePages (AllocateAnyPages, EfiACPIMemoryNVS, EFI_SIZE_TO_PAGES (SHARED_MAX_SIZE * 2), &SharedRegion) != EFI_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "  Returning error, allocate page failed\n");
    return EFI_UNSUPPORTED;
  }

  SharedRegion = ((SharedRegion + (SHARED_MAX_SIZE - 1)) & ~((SHARED_MAX_SIZE) - 1));
  AllocParams.RequestedBufferSize = sizeof (UINT64);
  AllocParams.BufferHandle = AMD_PCCT_SHARED_REGION_ADDRESS_HANDLE;
  AllocParams.Persist = HEAP_SYSTEM_MEM;
  if (HeapAllocateBuffer (&AllocParams, NULL) == AGESA_SUCCESS) {
    *((UINT64 *) AllocParams.BufferPtr) = SharedRegion;
  }

  // Mark shared memory region as UC
  for (MtrrIndex = 0; MtrrIndex < 8; MtrrIndex++) {
    if (((AsmReadMsr64 (0x201 + (MtrrIndex << 1))) & BIT11) == 0) {
      break;
    }
  }
  if (MtrrIndex >= 8) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "  Returning unsupported, no var mtrr found\n");
    return EFI_UNSUPPORTED;
  }

  MtrrInfo.MtrrIndex = MtrrIndex;
  MtrrInfo.MtrrBase = SharedRegion;
  MtrrInfo.MtrrMask = 0xFFFFFFFF0800;
  MpServices->StartupAllAPs (
      MpServices,
      SetSharedMemoryToUC,
      FALSE,
      NULL,
      0,
      (VOID *) &MtrrInfo,
      NULL
  );
  SetSharedMemoryToUC (&MtrrInfo);

  // Set up doorbell for mailbox as MMIO
  MmioTarget.PciBusNum = 0;
  MmioTarget.SocketNum = 0;
  MmioTarget.RbNum = 0;
  MmioTarget.TgtType = 1;
  MmioTarget.PciSegNum = 0;

  Attrib.ReadEnable = 1;
  Attrib.WriteEnable = 1;
  Attrib.NonPosted = 0;
  Attrib.CpuDis = 0;
  Attrib.MmioType = NON_PCI_DEVICE_BELOW_4G;

  MmioLength = ALIGN_1M + 1;

  Status = FabricResourceMgr->FabricAllocateMmio (FabricResourceMgr, &MmioBase, &MmioLength, ALIGN_1M, MmioTarget, &Attrib);
  ASSERT (!EFI_ERROR (Status));
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "  Returning error, allocate mmio\n");
    return EFI_UNSUPPORTED;
  }

  // Populate signature of Shared Region with 'PCCNull'
  Pcc = (UINT32 *) SharedRegion;
  LibAmdMemFill ((VOID *) Pcc, 0x00, SHARED_MAX_SIZE, NULL);
  *Pcc = 0x50434300;

  // Pass shared region address to SMU FW
  Status = NbioCppcServices->SetTable (SharedRegion);

  // Populate the entries in the GAS
  GenCommSubspacePtr->RangeBaseAddress = SharedRegion;
  GenCommSubspacePtr->RangeLength = SHARED_MAX_SIZE;
  GenCommSubspacePtr->DoorbellRegister.AddrSpaceId = GAS_SPACE_ID_MEM;
  GenCommSubspacePtr->DoorbellRegister.RegisterBitWidth = 64;
  GenCommSubspacePtr->DoorbellRegister.RegisterBitOffset = 0;
  GenCommSubspacePtr->DoorbellRegister.AccessSize = GAS_ADDR_SIZE_QWORD;
  GenCommSubspacePtr->DoorbellRegister.RegisterAddress = AmdCppcInfo->DoorbellRegister;
  GenCommSubspacePtr->DoorbellPreserve = 0xFFFFFFFF00000000;
  GenCommSubspacePtr->DoorbellWrite = 0x1;
  GenCommSubspacePtr->NominalLatency = AmdCppcInfo->NominalLatency;
  GenCommSubspacePtr->MaxPeriodicAccessRate = AmdCppcInfo->MaxPeriodicAccessRate;
  GenCommSubspacePtr->MinReqTurnaroundTime = (UINT16) AmdCppcInfo->MinReqTurnaroundTime;
  *EndOfTable += sizeof (GEN_COMM_SUBSPACE_STRUCT);

  return EFI_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
* This function will add one CRAT entry.
*
*    @param[in]      CratEntryType        CRAT entry type
*    @param[in]      CratHeaderStructPtr  CRAT header pointer
*    @param[in, out] TableEnd             The end of CRAT
*
*    @returns        Pointer to the added entry
*/
UINT8 *
AddOneCratEntry (
  IN       CRAT_ENTRY_TYPE    CratEntryType,
  IN       CRAT_HEADER       *CratHeaderStructPtr,
  IN OUT   UINT8            **TableEnd
  )
{
  UINT8 *CurrentEntry;

  ASSERT (CratEntryType < CRAT_MAX_TYPE);

  CurrentEntry = *TableEnd;
  CratHeaderStructPtr->TotalEntries++;
  switch (CratEntryType) {
  case CRAT_HSA_PROC_UNIT_TYPE:
    *TableEnd += sizeof (CRAT_HSA_PROCESSING_UNIT);
    ((CRAT_HSA_PROCESSING_UNIT *) CurrentEntry)->Type = (UINT8) CratEntryType;
    ((CRAT_HSA_PROCESSING_UNIT *) CurrentEntry)->Length = sizeof (CRAT_HSA_PROCESSING_UNIT);
    CratHeaderStructPtr->NumNodes++;
    break;
  case CRAT_CACHE_TYPE:
    *TableEnd += sizeof (CRAT_CACHE);
    ((CRAT_CACHE *) CurrentEntry)->Type = (UINT8) CratEntryType;
    ((CRAT_CACHE *) CurrentEntry)->Length = sizeof (CRAT_CACHE);
    break;
  case CRAT_TLB_TYPE:
    *TableEnd += sizeof (CRAT_TLB);
    ((CRAT_TLB *) CurrentEntry)->Type = (UINT8) CratEntryType;
    ((CRAT_TLB *) CurrentEntry)->Length = sizeof (CRAT_TLB);
    break;
  case CRAT_FPU_TYPE:
    *TableEnd += sizeof (CRAT_FPU);
    ((CRAT_FPU *) CurrentEntry)->Type = (UINT8) CratEntryType;
    ((CRAT_FPU *) CurrentEntry)->Length = sizeof (CRAT_FPU);
    break;
  default:
    ASSERT (FALSE);
    break;
  }
  return CurrentEntry;
}

/*---------------------------------------------------------------------------------------*/
/**
* Return associativity
*
*    @param[in]      RawAssoc          Data which is got from CPUID
*
*    @returns        The actual associativity based on the encoded input
*
*/
UINT8
GetCacheAssoc (
  IN       UINT16   RawAssoc
  )
{
  UINT8 Associativity;

  Associativity = 0;

  switch (RawAssoc) {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
    Associativity = (UINT8) RawAssoc;
    break;
  case 5:
    Associativity = 6;
    break;
  case 6:
    Associativity = 8;
    break;
  case 8:
    Associativity = 16;
    break;
  case 0xA:
    Associativity = 32;
    break;
  case 0xB:
    Associativity = 48;
    break;
  case 0xC:
    Associativity = 64;
    break;
  case 0xD:
    Associativity = 96;
    break;
  case 0xE:
    Associativity = 128;
    break;
  case 0xF:
    Associativity = 0xFF;
    break;
  default:
    ASSERT (FALSE);
    break;
  }

  return Associativity;
}

/*---------------------------------------------------------------------------------------*/
/**
* Return associativity
*
*    @param[in]      TLB_TYPE          2M4M, 4K or 1G
*    @param[in]      CratTlbEntry      Crat TLB entry
*    @param[in]      RawAssocSize      Value which is got from CPUID
*
*    @returns        TLB size
*
*/
UINT8
GetTlbSize (
  IN       TLB_TYPE   TLB_TYPE,
  IN       CRAT_TLB  *CratTlbEntry,
  IN       UINT16     RawAssocSize
  )
{
  UINT8 TlbSize;

  if (RawAssocSize >= 256) {
    TlbSize = (UINT8) (RawAssocSize / 256);
    if (TLB_TYPE == TLB_2M) {
      CratTlbEntry->Flags.TLB2MBase256 = 1;
    }

    if (TLB_TYPE == TLB_4K) {
      CratTlbEntry->Flags.TLB4KBase256 = 1;
    }

    if (TLB_TYPE == TLB_1G) {
      CratTlbEntry->Flags.TLB1GBase256 = 1;
    }
  } else {
    TlbSize = (UINT8) (RawAssocSize);
  }

  return TlbSize;
}

VOID
SetSharedMemoryToUC (
  IN       MTRR_INFO  *MtrrInfo
  )
{
  AsmWriteMsr64 (((MtrrInfo->MtrrIndex << 1) + 0x200), MtrrInfo->MtrrBase);
  AsmWriteMsr64 (((MtrrInfo->MtrrIndex << 1) + 0x201), MtrrInfo->MtrrMask);
  AsmWbinvd ();
}

/**
 * This service returns information on the CPPC Register Descriptors MSR
 *
 * @param[in]      This                 Pointer to AMD_CCX_ACPI_CPPC_SERVICES_PROTCOL instance
 * @param[out]     CppcRegDescMsr       CPPC Register Descriptors MSR
 *
 * @retval EFI_SUCCESS                  Always succeeds
 *
 **/
EFI_STATUS
EFIAPI
CcxZen3GetRegisterDescriptorsMsr (
  IN       AMD_CCX_ACPI_CPPC_SERVICES_PROTOCOL    *This,
     OUT   CPPC_ENTRIES_COMMON_TABLE              **CppcRegDescMsr
  )
{
    *CppcRegDescMsr = (CPPC_ENTRIES_COMMON_TABLE *) &Zen3CppcRegisterDescriptorsMsr;

    return EFI_SUCCESS;
}
