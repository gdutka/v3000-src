/** @file
;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "CpuDeviceInfoDxe.h"
#include "X64/CpuDeviceInfo.h"

#include <Register/Amd/Cpuid.h>
#include <Register/Intel/Cpuid.h>

#include <Library/PciLib.h>

/**
  Smn register read.

  @param[in]      BusNumber                Bus number of D0F0 of the target die.
  @param[in]      Address                  Register offset, but PortDevice.
  @param[out]     Value                    Return value.
**/
STATIC
VOID
SmnRegisterRead (
  IN     UINT32               BusNumber,
  IN     UINT32               Address,
     OUT UINT32               *Value
  )
{
  UINT32        HostPciAddress;

  HostPciAddress = PCI_LIB_ADDRESS (BusNumber, 0, 0, IOHC_NB_SMN_INDEX2);
  PciWrite32 (HostPciAddress, Address);

  HostPciAddress = PCI_LIB_ADDRESS (BusNumber, 0, 0, IOHC_NB_SMN_DATA2);
  *Value = PciRead32 (HostPciAddress);

  return;
}

/**
  Smn register Write.

  @param[in]      BusNumber                Bus number of D0F0 of the target die.
  @param[in]      Address                  Register offset, but PortDevice.
  @param[in]      Value                    The value to write.
**/
STATIC
VOID
SmnRegisterWrite (
  IN     UINT32               BusNumber,
  IN     UINT32               Address,
  IN     UINT32               Value
  )
{
  UINT32        HostPciAddress;

  HostPciAddress = PCI_LIB_ADDRESS (BusNumber, 0, 0, IOHC_NB_SMN_INDEX2);
  PciWrite32 (HostPciAddress, Address);

  HostPciAddress = PCI_LIB_ADDRESS (BusNumber, 0, 0, IOHC_NB_SMN_DATA2);
  PciWrite32 (HostPciAddress, Value);

  return;
}

/**
  Smu service request.

  @param[in]      BusNumber                Bus number of D0F0 of the target die.
  @param[in]      RequestId                Request id.
  @param[in, out] RequestArgument          On input, empty,
                                           On output, respone the value.
**/
STATIC
UINT32
SmuServiceRequest (
  IN     UINT32               BusNumber,
  IN     UINT32               RequestId,
  IN OUT UINT32               *RequestArgument
  )
{
  UINT32                      RxSmuMessageResponse;
  UINT32                      DataMask;

  DataMask = 0xFFFFFFFF;

  if (RequestId == INVALID_SMU_MSG) {
    return BIOSSMC_RESULT_OK;
  }

  SmnRegisterRead (BusNumber, MP1_C2PMSG_RESPONSE_ADDRESS, &RxSmuMessageResponse);

  // 1 Clear Response
  RxSmuMessageResponse = 0;
  SmnRegisterWrite (BusNumber, MP1_C2PMSG_RESPONSE_ADDRESS, RxSmuMessageResponse);

  // 2 Write message arguments
  SmnRegisterWrite (BusNumber, MP1_C2PMSG_ARGUMENT_0_ADDRESS, RequestArgument[0]);
  SmnRegisterWrite (BusNumber, MP1_C2PMSG_ARGUMENT_1_ADDRESS, RequestArgument[1]);
  SmnRegisterWrite (BusNumber, MP1_C2PMSG_ARGUMENT_2_ADDRESS, RequestArgument[2]);
  SmnRegisterWrite (BusNumber, MP1_C2PMSG_ARGUMENT_3_ADDRESS, RequestArgument[3]);
  SmnRegisterWrite (BusNumber, MP1_C2PMSG_ARGUMENT_4_ADDRESS, RequestArgument[4]);
  SmnRegisterWrite (BusNumber, MP1_C2PMSG_ARGUMENT_5_ADDRESS, RequestArgument[5]);

  // 3 Write message ID
  SmnRegisterWrite (BusNumber, MP1_C2PMSG_MESSAGE_ADDRESS, RequestId);

  // 4 Poll Response until non-zero
  do {
    SmnRegisterRead (BusNumber, MP1_C2PMSG_RESPONSE_ADDRESS, &RxSmuMessageResponse);
  } while (RxSmuMessageResponse == 0x0);

  if (RxSmuMessageResponse != BIOSSMC_RESULT_OK) {
    return (RxSmuMessageResponse);
  }

  // 5 Read updated SMU message arguments
  SmnRegisterRead (BusNumber, MP1_C2PMSG_ARGUMENT_0_ADDRESS, &RequestArgument[0]);
  SmnRegisterRead (BusNumber, MP1_C2PMSG_ARGUMENT_1_ADDRESS, &RequestArgument[1]);
  SmnRegisterRead (BusNumber, MP1_C2PMSG_ARGUMENT_2_ADDRESS, &RequestArgument[2]);
  SmnRegisterRead (BusNumber, MP1_C2PMSG_ARGUMENT_3_ADDRESS, &RequestArgument[3]);
  SmnRegisterRead (BusNumber, MP1_C2PMSG_ARGUMENT_4_ADDRESS, &RequestArgument[4]);
  SmnRegisterRead (BusNumber, MP1_C2PMSG_ARGUMENT_5_ADDRESS, &RequestArgument[5]);

  return (RxSmuMessageResponse);
}

/**
  Get CPU cache.

  @param[in]      CacheLevel               Cache level.
  @param[in]      CacheType                Cache type.
  @param[out]     CacheSize                Cache size.
  @param[out]     CacheAssociativity       Cache associativity.

  @retval         EFI_SUCCESS              Success.
  @retval         EFI_NOT_FOUND
**/
EFI_STATUS
GetCpuCache (
  IN     UINT8                CacheLevel,
  IN     UINT8                CacheType,
     OUT UINT32               *CacheSize,
     OUT UINT32               *CacheAssociativity
  )
{
  EFI_STATUS                 Status;
  UINT8                      CacheIndex;
  CPUID_CACHE_PARAMS_EAX     Eax;
  CPUID_CACHE_PARAMS_EBX     Ebx;
  UINT32                     RegEcx;
  UINT32                     Ways;
  UINT32                     Partitions;
  UINT32                     LineSets;
  UINT32                     Sets;

  Status = EFI_NOT_FOUND;
  *CacheSize = 0;
  *CacheAssociativity = 0;

  for (CacheIndex = 0; CacheIndex < MAX_NUM_OF_CACHE_PARAMS_LEAF; CacheIndex++) {
    AsmCpuidEx (CPUID_CACHE_PARAMS, CacheIndex, &Eax.Uint32, &Ebx.Uint32, &RegEcx, NULL);
    if (Eax.Bits.CacheType == 0) {
      break;
    }

    if ((UINT8) Eax.Bits.CacheLevel == CacheLevel && (UINT8) Eax.Bits.CacheType == CacheType) {
      Ways                    = Ebx.Bits.Ways + 1;
      Partitions              = Ebx.Bits.LinePartitions + 1;
      LineSets                = Ebx.Bits.LineSize + 1;
      Sets                    = RegEcx + 1;
      *CacheSize = (Ways * Partitions * LineSets * Sets) / 1024;

      if (Eax.Bits.FullyAssociativeCache) {
        *CacheAssociativity = 0xFF;
      } else {
        *CacheAssociativity = Ways;
      }

      Status = EFI_SUCCESS;
      break;
    }
  }

  return Status;
}

/**
  Get CPU TDP.

  Note: Since this part using Intel MSR/AMD SMU to get thermal information,
        It could be cause system hang or some other error.
        Please comfirm your MSR Support/SMU address before CPU TDP Information enable.

  @param[in]      This                     Points to the H2O_CPU_DEVICE_INFO_PROTOCOL.
  @param[out]     CpuTDP                   The contains CPU TDP pointer.

  @retval         EFI_INVALID_PARAMETER    The Param This or CpuTDP is NULL.
  @retval         EFI_SUCCESS              Get CPU TDP successfully.
**/
EFI_STATUS
EFIAPI
GetCpuTDP (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
     OUT UINT32                            *CpuTDP
  )
{
  UINT32                                   TDP;
  UINT32                                   PwrUnit;
  UINT64                                   PkgPwrLim1;
  UINT32                                   SmuArg[6] = {0};
  UINT32                                   SmuStatus;

  if ((This == NULL) || (CpuTDP == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/CpuTDP is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  TDP = 0;

  switch (IdentifyManufacture ()) {

  case ProcessorManufacturerIntel:
    // [3:0] PWR_UNIT:
    PwrUnit     = AsmReadMsr32 (INTEL_MSR_RAPL_POWER_UNIT_ADDRESS) & INTEL_MSR_POWER_UNIT_BITFIELD;
    // [14:0] PKG_PWR_LIM_1
    PkgPwrLim1  = AsmReadMsr64 (INTEL_MSR_PKG_POWER_LIMIT_ADDRESS) & INTEL_MSR_PKG_PWR_LIM_1_BITFIELD;

    TDP = ((UINT32) PkgPwrLim1 / (2 << (PwrUnit - 1)));
    break;

  case ProcessorManufacturerAmdHygon:
    // BusNum = FabricTopologyGetHostBridgeBusBase (0, 0, 0);
    SmuStatus = SmuServiceRequest (0, SMC_MSG_TDP_TJMAX, SmuArg);

    if (SmuStatus != 0x01) {
      DEBUG ((
      EFI_D_ERROR,
      "%a() - Smu service access failed\n",
      __FUNCTION__
      ));
      return EFI_NOT_FOUND;
    }
    if (SmuArg[0] != 0) {
      TDP = SmuArg[0] >> 16;
    }
    break;

  default:
    DEBUG ((EFI_D_INFO, "%a() - IdentifyManufacture fail\n", __FUNCTION__));
    break;
  }

  *CpuTDP = TDP;

  return EFI_SUCCESS;
}

/**
  Get core L1 data cache.

  @param[out]     CacheSize                The contains cache size pointer.
  @param[out]     CacheAssociativity       The contains cache associativity pointer.

  @retval         EFI_INVALID_PARAMETER    CacheSize or CacheAssociativity is NULL.
  @retval         EFI_SUCCESS              Get cache size and cache associativity successfully.
**/
EFI_STATUS
EFIAPI
GetCoreL1DataCache (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity
  )
{
  EFI_CPUID_REGISTER          CpuIdRegisters = {0};

  if (This == NULL || CacheSize == NULL || CacheAssociativity == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  switch (IdentifyManufacture ()) {

  case ProcessorManufacturerIntel:
    GetCpuCache (1, CPUID_CACHE_PARAMS_CACHE_TYPE_DATA, CacheSize, CacheAssociativity);
    break;

  case ProcessorManufacturerAmdHygon:
    AsmCpuid (
      0x80000005,
      &(CpuIdRegisters.RegEax),
      &(CpuIdRegisters.RegEbx),
      &(CpuIdRegisters.RegEcx),
      &(CpuIdRegisters.RegEdx)
      );
    *CacheSize          = (CpuIdRegisters.RegEcx >> 24);
    *CacheAssociativity = (CpuIdRegisters.RegEcx & (BIT23 | BIT22 | BIT21 |BIT20 | BIT19 | BIT18 | BIT17 | BIT16)) >> 16;
    break;

  default:
    DEBUG ((EFI_D_INFO, "%a() - IdentifyManufacture fail\n", __FUNCTION__));
    break;
  }

  return EFI_SUCCESS;
}

/**
  Get core L1 insruction cache.

  @param[out]     CacheSize                The contains cache size pointer.
  @param[out]     CacheAssociativity       The contains cache associativity pointer.

  @retval         EFI_INVALID_PARAMETER    CacheSize or CacheAssociativity is NULL.
  @retval         EFI_SUCCESS              Get cache size and cache associativity successfully.
**/
EFI_STATUS
EFIAPI
GetCoreL1InsructionCache (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity
  )
{
  EFI_CPUID_REGISTER          CpuIdRegisters = {0};

  if (This == NULL || CacheSize == NULL || CacheAssociativity == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  switch (IdentifyManufacture ()) {

  case ProcessorManufacturerIntel:
    GetCpuCache (1, CPUID_CACHE_PARAMS_CACHE_TYPE_INSTRUCTION, CacheSize, CacheAssociativity);
    break;

  case ProcessorManufacturerAmdHygon:
    AsmCpuid (
      0x80000005,
      &(CpuIdRegisters.RegEax),
      &(CpuIdRegisters.RegEbx),
      &(CpuIdRegisters.RegEcx),
      &(CpuIdRegisters.RegEdx)
      );
    *CacheSize          = (CpuIdRegisters.RegEdx >> 24);
    *CacheAssociativity = (CpuIdRegisters.RegEdx & (BIT23 | BIT22 | BIT21 |BIT20 | BIT19 | BIT18 | BIT17 | BIT16)) >> 16;
    break;

  default:
    DEBUG ((EFI_D_INFO, "%a() - IdentifyManufacture fail\n", __FUNCTION__));
    break;
  }

  return EFI_SUCCESS;
}

/**
  Get core L2 cache.

  @param[out]     CacheSize                The contains cache size pointer.
  @param[out]     CacheAssociativity       The contains cache associativity pointer.

  @retval         EFI_INVALID_PARAMETER    CacheSize or CacheAssociativity is NULL.
  @retval         EFI_SUCCESS              Get cache size and cache associativity successfully.
**/
EFI_STATUS
EFIAPI
GetCoreL2Cache (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity
  )
{

  EFI_CPUID_REGISTER          CpuIdRegisters = {0};

  if (This == NULL || CacheSize == NULL || CacheAssociativity == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  switch (IdentifyManufacture ()) {

  case ProcessorManufacturerIntel:
    GetCpuCache (2, CPUID_CACHE_PARAMS_CACHE_TYPE_UNIFIED, CacheSize, CacheAssociativity);
    break;

  case ProcessorManufacturerAmdHygon:
    AsmCpuid (
      0x80000006,
      &(CpuIdRegisters.RegEax),
      &(CpuIdRegisters.RegEbx),
      &(CpuIdRegisters.RegEcx),
      &(CpuIdRegisters.RegEdx)
      );
    *CacheSize          = (CpuIdRegisters.RegEcx >> 16);
    *CacheAssociativity = (CpuIdRegisters.RegEcx & (BIT15 | BIT14 | BIT13 |BIT12)) >> 12;
    break;

  default:
    DEBUG ((EFI_D_INFO, "%a() - IdentifyManufacture fail\n", __FUNCTION__));
    break;
  }

  return EFI_SUCCESS;
}

/**
  Get core L3 cache.

  @param[out]     CacheSize                The contains cache size pointer.
  @param[out]     CacheAssociativity       The contains cache associativity pointer.

  @retval         EFI_INVALID_PARAMETER    CacheSize or CacheAssociativity is NULL.
  @retval         EFI_SUCCESS              Get cache size and cache associativity successfully.
**/
EFI_STATUS
EFIAPI
GetCoreL3Cache (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity,
  OUT UINT32                          *CacheNumWays
  )
{
  EFI_CPUID_REGISTER          CpuIdRegisters = {0};

  if (This == NULL || CacheSize == NULL || CacheAssociativity == NULL || CacheNumWays == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  switch (IdentifyManufacture ()) {

  case ProcessorManufacturerIntel:
    GetCpuCache (3, CPUID_CACHE_PARAMS_CACHE_TYPE_UNIFIED, CacheSize, CacheAssociativity);
    break;

  case ProcessorManufacturerAmdHygon:
    AsmCpuid (
      0x80000006,
      &(CpuIdRegisters.RegEax),
      &(CpuIdRegisters.RegEbx),
      &(CpuIdRegisters.RegEcx),
      &(CpuIdRegisters.RegEdx)
      );
    *CacheSize          = (CpuIdRegisters.RegEdx >> 18) / 2;
    *CacheAssociativity = (CpuIdRegisters.RegEdx & (BIT15 | BIT14 | BIT13 |BIT12)) >> 12;
    if (*CacheAssociativity == 0x9) {
      AsmCpuidEx (
        0x8000001D,               // function 8000001D
        3,                        // x3  Cache Properties (L3)]
        &(CpuIdRegisters.RegEax),
        &(CpuIdRegisters.RegEbx),
        &(CpuIdRegisters.RegEcx),
        &(CpuIdRegisters.RegEdx)
        );
      *CacheNumWays = ((CpuIdRegisters.RegEbx & 0xFFC00000) >> 22) + 1;
    }
    break;

  default:
    DEBUG ((EFI_D_INFO, "%a() - IdentifyManufacture fail\n", __FUNCTION__));
    break;
  }

  return EFI_SUCCESS;
}

/**
  Get microcode version.

  @param[in]      This                     Points to the H2O_CPU_DEVICE_INFO_PROTOCOL.
  @param[out]     MicrocodeVersion         The contains microcode version pointer.

  @retval         EFI_INVALID_PARAMETER    The Param This or MicrocodeVersion is NULL.
  @retval         EFI_SUCCESS              Get microcode version successfully.
**/
EFI_STATUS
EFIAPI
GetMicrocodeVersion (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
     OUT UINT64                            *MicrocodeVersion
  )
{
  UINT64                                   MicroCodePatchLevel;

  if ((This == NULL) || (MicrocodeVersion == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/MicrocodeVersion is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  switch (IdentifyManufacture ()) {

  case ProcessorManufacturerIntel:
    AsmWriteMsr64 (0x8B, 0);
    AsmCpuid (0x01, NULL, NULL, NULL, NULL);
    MicroCodePatchLevel  = AsmReadMsr64 (0x8B);
    *MicrocodeVersion = RShiftU64 (MicroCodePatchLevel, 32);
    break;

  case ProcessorManufacturerAmdHygon:
    MicroCodePatchLevel  = AsmReadMsr64 (0x8B);
    *MicrocodeVersion = MicroCodePatchLevel;
    break;

  default:
    DEBUG ((EFI_D_INFO, "%a() - IdentifyManufacture fail\n", __FUNCTION__));
    *MicrocodeVersion = 0;
    break;
  }

  return EFI_SUCCESS;
}