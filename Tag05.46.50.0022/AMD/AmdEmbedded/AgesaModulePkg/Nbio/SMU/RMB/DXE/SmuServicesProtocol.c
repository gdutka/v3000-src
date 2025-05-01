/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/* $NoKeywords:$ */
/**
 * @file
 *
 * SMU Services Protocol
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  SmuV13Dxe
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */

#include <PiDxe.h>
#include <Filecode.h>
#include <SmuV13Dxe.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <GnbDxio.h>
#include <GnbRegistersRMB.h>
#include <Library/NbioHandleLib.h>
#include <IdsHookId.h>
#include <Library/AmdIdsHookLib.h>
#include <SMU_RMB_MsgDef.h>
#include <Library/NbioSmuV13Lib.h>
#include <Library/SmnAccessLib.h>
#include "smu13_bios_if_RMB.h"
#include "smu13_RMB.h"
#include "Smu13FeaturesEnable.h"
#include <Addendum/Apcb/Inc/EnvironmentFlags.h>

#define FILECODE        NBIO_SMU_RMB_DXE_SMUSERVICESPROTOCOL_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define GNB_MB_TO_PAGE(a) ((a) * (SIZE_1MB / EFI_PAGE_SIZE))
#define MAX_CAC_WEIGHT_NUM  21

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL mDxeAmdNbioSmuServicesProtocol = {
  DXE_AMD_NBIO_SMU_SERVICES_DXE_REVISION,
  AmdSmuReadFuseByName,
  AmdSmuReadFuseByNumber,
  AmdSmuServiceRequest,
  AmdSmuRegisterRead,
  AmdSmuRegisterWrite,
  AmdSmuRegisterRMW,
  AmdSmuReadBrandString,
  AmdSmuReadCacWeights,
  AmdSmuReadThrottleThresholds
};


/*----------------------------------------------------------------------------------------*/
/**
 *   Reads a fuse value based on an enumerated list of fuse "names"
 *
 *
 * @param[in]   This               A pointer to the DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL instance.
 * @param[in]   FuseName           The "name" of the fuse from an enumerated list of fuses supported by this function
 * @param[out]  FuseValue          Pointer to the value of the fuse
 *
 * @retval      EFI_SUCCESS
 *
 */
EFI_STATUS
EFIAPI
AmdSmuReadFuseByName (
  IN       DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  *This,
  IN       UINT32                         InstanceId,
  IN       UINTN                          FuseName,
  IN OUT   UINTN                          *BufferSize,
     OUT   UINT8                          *FuseValue
  )
{
  return EFI_UNSUPPORTED;
}

/*----------------------------------------------------------------------------------------*/
/**
 *   Reads a fuse value based on chain, start bit, and size
 *
 *
 * @param[in]   This               A pointer to the DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL instance.
 * @param[in]   FuseChain          The chain number of the fuse to read
 * @param[in]   FuseStartBit       The starting bit within the specified fuse chain of the fuse to read
 * @param[in]   FuseSize           The bitfield width of the fuse to read
 * @param[out]  FuseValue          Pointer to the value of the fuse
 *
 * @retval      EFI_SUCCESS
 *
 */
EFI_STATUS
EFIAPI
AmdSmuReadFuseByNumber (
  IN       DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  *This,
  IN       UINT32                              FuseChain,
  IN       UINT32                              FuseStartBit,
  IN       UINT32                              FuseSize,
     OUT   UINT32                              *FuseValue
  )
{
  return EFI_UNSUPPORTED;
}

/*----------------------------------------------------------------------------------------*/
/**
 *   Executes a specific SMU service and returns the results
 *
 *
 * @param[in]   This               A pointer to the DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL instance.
 * @param[in]   InstanceId         The unique identifier of the SMU instance associated with this socket
                                   (for RMB, the socket number)
 * @param[in]   ServiceRequest     The service request identifier
 * @param[in]   InputParameters    Pointer to an array of service request arguments (for CZ, UINT32[6])
 * @param[out]  ReturnValues       Pointer to an array of service request response values (UINT32[6])
 *
 * @retval      EFI_SUCCESS
 *
 */
EFI_STATUS
EFIAPI
AmdSmuServiceRequest (
  IN       DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  *This,
  IN       UINT32                              InstanceId,
  IN       UINT32                              ServiceRequest,
  IN       UINT32                              *InputParameters,
     OUT   UINT32                              *ReturnValues
  )
{
  GNB_HANDLE                      *GnbHandle;
  EFI_STATUS                      Status;
  UINT32                          SmuArg[6];
  UINT32                          SmuResult;
  PRESIL_CTRL1                    PreSilCtrl1;

  PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
  if (PreSilCtrl1.Field.FASTSIM_SMU_MSGS == 1) {
    return EFI_SUCCESS;
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);
  Status = SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status == EFI_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "GnbHandle Found\n");
    LibAmdMemCopy (SmuArg, InputParameters, 24, (AMD_CONFIG_PARAMS *) NULL);
    SmuResult = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle),
                                                ServiceRequest, SmuArg, GNB_REG_ACC_FLAG_S3SAVE);
    if (SmuResult == 1) {
      LibAmdMemCopy (ReturnValues, SmuArg, 24, (AMD_CONFIG_PARAMS *) NULL);
    } else {
      Status = EFI_INVALID_PARAMETER;
      IDS_HDT_CONSOLE (MAIN_FLOW, "InstanceId not found!!\n");
      ASSERT (FALSE);       // Unsupported SMU service request
    }
  }
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit Status = %d\n", __FUNCTION__, Status);

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 *   Reads a 32-bit register from SMU register space
 *
 *
 * @param[in]  This               A pointer to the DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL instance.
 * @param[in]  InstanceId         The unique identifier of the SMU instance associated with this socket
                                  (for RMB, the socket number)
 * @param[in]  RegisterIndex      The index of the register to be read
 * @param[in]  RegisterValue      Pointer to a UINT32 holding the value to write to the register
 *
 */
EFI_STATUS
EFIAPI
AmdSmuRegisterRead (
  IN       DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  *This,
  IN       UINT32                              InstanceId,
  IN       UINT32                              RegisterIndex,
     OUT   UINT32                              *RegisterValue
  )
{
  GNB_HANDLE                      *GnbHandle;
  EFI_STATUS                      Status;

  Status = SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status == EFI_SUCCESS) {
    SmnRegisterRead (GnbHandle->Address.Address.Bus, RegisterIndex, RegisterValue);
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Writes a 32-bit register in SMU register space
 *
 *
 *
 * @param[in]  This               A pointer to the DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL instance.
 * @param[in]  InstanceId         The unique identifier of the SMU instance associated with this socket
                                 (for RMB, the socket number)
 * @param[in]  RegisterIndex      The index of the register to be read
 * @param[in]  RegisterValue      Pointer to a UINT32 holding the value to write to the register
 *
 */
EFI_STATUS
EFIAPI
AmdSmuRegisterWrite (
  IN       DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  *This,
  IN       UINT32                              InstanceId,
  IN       UINT32                              RegisterIndex,
  IN       UINT32                              *RegisterValue
  )
{
  GNB_HANDLE                      *GnbHandle;
  EFI_STATUS                      Status;

  Status = SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status == EFI_SUCCESS) {
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, RegisterIndex, RegisterValue, GNB_REG_ACC_FLAG_S3SAVE);
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Read/Modify/Write a 32-bit register from SMU register space
 *
 *
 *
 * @param[in]  This               A pointer to the DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL instance.
 * @param[in]  InstanceId         The unique identifier of the SMU instance associated with this socket
                                  (for RMB, the socket number)
 * @param[in]  RegisterIndex      The index of the register to be read
 * @param[in]  RegisterANDValue   UINT32 holding the value to be "ANDed" with the data read from the register
                                  (i.e. mask of data that will not be modified)
 * @param[in]  RegisterORValue    UINT32 holding the value to be "ORed" with the data read from the register
                                  (i.e. data bits to be "set" by this function)
 *
 */
EFI_STATUS
EFIAPI
AmdSmuRegisterRMW (
  IN       DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  *This,
  IN       UINT32                              InstanceId,
  IN       UINT32                              RegisterIndex,
  IN       UINT32                              RegisterANDValue,
  IN       UINT32                              RegisterORValue
  )
{
  GNB_HANDLE                      *GnbHandle;
  EFI_STATUS                      Status;
  UINT32                          RegisterValue;

  Status = SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status == EFI_SUCCESS) {
    SmnRegisterRead (GnbHandle->Address.Address.Bus, RegisterIndex, &RegisterValue);
    RegisterValue = (RegisterValue & RegisterANDValue) | RegisterORValue;
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, RegisterIndex, &RegisterValue, GNB_REG_ACC_FLAG_S3SAVE);
  }
  return Status;
}

/*----------------------------------------------------------------------------------------
/**
 * Returns the brand string read from SMN space.
 *
 * @param[in]  This               A pointer to the DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL instance.
 * @param[in]  InstanceId         The unique identifier of the SMU instance associated with this socket
                                  (for RMB, the socket number)
 * @param[in]  BrandStringLength  Number of characters to be read.  This value does NOT have to be a multiple of 4.
                                  NBIO driver code will be responsible for alignment, endianness, and string length.
 * @param[in]  BrandString        Pointer to a CHAR array to store the brand string.
 *
**/
EFI_STATUS
EFIAPI
AmdSmuReadBrandString (
  IN       DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  *This,
  IN       UINT32                              InstanceId,
  IN       UINT32                              BrandStringLength,
     OUT   UINT8                               *BrandString
  )
{
  GNB_HANDLE                      *GnbHandle;
  EFI_STATUS                      Status;
  UINT32                          SmuArg[6];
  UINT32                          SmuResult;
  UINT8                           *StringPtr;
  UINT32                          StringCount;
  UINT32                          ByteCount;
  UINT32                          Iteration;
  PRESIL_CTRL1                    PreSilCtrl1;

  PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
  if (PreSilCtrl1.Field.FASTSIM_SMU_MSGS == 1) {
    return EFI_SUCCESS;
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);
  Status = SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status == EFI_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "GnbHandle Found\n");
    StringCount = BrandStringLength;
    StringPtr = BrandString;
    Iteration = 0;
    while (StringCount != 0) {
      NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
      SmuArg[0] = Iteration++;
      SmuResult = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), BIOSSMC_MSG_GetNameString, SmuArg, 0);
      if (SmuResult == 1) {
        ByteCount = (StringCount < 4) ? StringCount : 4;
        LibAmdMemCopy (StringPtr, SmuArg, StringCount, (AMD_CONFIG_PARAMS *) NULL);
        StringPtr += ByteCount;
        StringCount -= ByteCount;
      } else {
        Status = EFI_INVALID_PARAMETER;
        IDS_HDT_CONSOLE (MAIN_FLOW, "SmuRequestFail!!\n");
        break;
      }
    }
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit Status = %d\n", __FUNCTION__, Status);
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Returns the CAC weights read from SMU
 *
 * @param[in]  This                    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
 * @param[in]  MaxNumWeights           The number of values to read from the SMU
                                       (defines the maximum size of ApmWeights return data buffer)
 * @param[Out] ApmWeights              Pointer to return data buffer
 *
 * @retval     EFI_SUCCESS             ApmWeights contains the CaC weights read from SMU
 * @retval     EFI_INVALID_PARAMETER   One of the input parameters was invalid
 *                                     InstanceId did not reference a valid NBIO instance
 * @retval     EFI_BUFFER_TOO_SMALL    The number of weights available exceeds MaxNumWeights
 * @retval     EFI_UNSUPPORTED         This function is not supported by this version of the driver
 */
EFI_STATUS
AmdSmuReadCacWeights (
  IN       DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  *This,
  IN       UINT32                              MaxNumWeights,
     OUT   UINT64                              *ApmWeights
  )
{
  EFI_STATUS                           Status;
  PCIe_PLATFORM_CONFIG                 *Pcie;
  PCIe_PLATFORM_CONFIG                 *PcieFromHob;
  GNB_HANDLE                           *GnbHandle;
  UINT8                                WeightIndex;
  UINT32                               SmuArg[6];
  UINT32                               RxSmuMessageResponse[6];
  UINT32                               SmuResult;
  UINT64                               CacWeights[MAX_CAC_WEIGHT_NUM];
  PRESIL_CTRL1                         PreSilCtrl1;

  PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
  if (PreSilCtrl1.Field.FASTSIM_SMU_MSGS == 1) {
    return EFI_SUCCESS;
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);

  LibAmdMemFill ((VOID*)CacWeights, 0x00, MAX_CAC_WEIGHT_NUM * (sizeof (UINT64)), NULL);
  SmuDxeInitialize (&PcieFromHob);
  Pcie = PcieFromHob;
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  GnbHandle = NbioGetHandle (Pcie);
  Status = SmuGetGnbHandle (0, &GnbHandle);

  MaxNumWeights = (MaxNumWeights <= MAX_CAC_WEIGHT_NUM) ? MaxNumWeights : MAX_CAC_WEIGHT_NUM;
  for (WeightIndex = 0; WeightIndex < MaxNumWeights; WeightIndex++) {

    NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
    SmuArg[0] = WeightIndex;

    SmuResult = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle),
                                               BIOSSMC_MSG_ReadCoreCacWeightRegister, SmuArg, 0);

    if (SmuResult == 1) {
      LibAmdMemCopy (RxSmuMessageResponse, SmuArg, 24, (AMD_CONFIG_PARAMS *) NULL);
      CacWeights[WeightIndex] = RxSmuMessageResponse[1];
      IDS_HDT_CONSOLE (MAIN_FLOW, "CacWeightsOrg[%d] = %x\n", WeightIndex, CacWeights[0]);
      IDS_HDT_CONSOLE (MAIN_FLOW, "RxSmuMessageResponse[0] = %x\n", RxSmuMessageResponse[0]);
      IDS_HDT_CONSOLE (MAIN_FLOW, "RxSmuMessageResponse[1] = %x\n", RxSmuMessageResponse[1]);
      CacWeights[WeightIndex] = (CacWeights[WeightIndex] << 32) | RxSmuMessageResponse[0];
      IDS_HDT_CONSOLE (MAIN_FLOW, "CacWeights[%d] = %llx\n", WeightIndex, CacWeights[WeightIndex]);
      LibAmdMemCopy (ApmWeights, CacWeights, MaxNumWeights * 8, (AMD_CONFIG_PARAMS *) NULL);
      IDS_HDT_CONSOLE (MAIN_FLOW, "ApmWeights[0] = %llx\n", ApmWeights[0]);
    } else {
      Status = EFI_INVALID_PARAMETER;
      IDS_HDT_CONSOLE (MAIN_FLOW, "InstanceId not found!!\n");
      ASSERT (FALSE);       // Unsupported SMU service request
    }
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "mCacWeightsMod[%d] = %llx\n", WeightIndex, CacWeights[WeightIndex]);

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit Status = %d\n", __FUNCTION__, Status);
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Returns the throttle thresholds read from SMU
 *
 * @param[in]  This              A pointer to the DXE_AMD_NBIO_SMU_SERVICES_PPI instance.
 * @param[Out] Thresholds        Pointer to return data buffer
 *
 * @retval     EFI_SUCCESS       ApmWeights contains the throttle thresholds read from SMU
 * @retval     EFI_UNSUPPORTED   This function is not supported by this version of the driver
 */
EFI_STATUS
AmdSmuReadThrottleThresholds (
  IN       DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  *This,
     OUT   UINT32                              *Thresholds
  )
{
  return EFI_UNSUPPORTED;
}




