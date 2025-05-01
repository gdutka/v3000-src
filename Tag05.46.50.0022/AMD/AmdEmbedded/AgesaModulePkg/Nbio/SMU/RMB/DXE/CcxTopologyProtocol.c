/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Core Topology Protocol
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
#include "SmuV13Dxe.h"
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AmdNbioPcieServicesProtocol.h>
#include <GnbDxio.h>
#include <Guid/GnbPcieInfoHob.h>
#include <GnbRegistersRMB.h>
#include <Library/NbioHandleLib.h>
#include <Library/PcieConfigLib.h>
#include <IdsHookId.h>
#include <Library/AmdIdsHookLib.h>
#include <Guid/HobList.h>
#include <Library/UefiLib.h>
#include <Library/NbioSmuV13Lib.h>
#include <Library/SmnAccessLib.h>
#include <Library/BaseLib.h>
#include "smu13_bios_if_RMB.h"
#include "smu13_RMB.h"
#include "Smu13FeaturesEnable.h"
#include <Library/BaseMemoryLib.h>
#include <CcxRegistersVh.h>
#include <Addendum/Apcb/Inc/RMB/APOB.h>
#include <Library/AmdPspApobLib.h>


#define FILECODE        NBIO_SMU_RMB_DXE_CCXTOPOLOGYPROTOCOL_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL mAmdCoreTopologyServicesProtocol = {
  SmuV13GetCoreTopologyOnDie,
  SmuV13LaunchThread
};

/**
 * This service retrieves information about the layout of the cores on the given die.
 *
 * @param[in]  This                           A pointer to the
 *                                            AMD_CORE_TOPOLOGY_SERVICES_PROTOCOL instance.
 * @param[in]  Socket                         Zero-based socket number to check.
 * @param[in]  Die                            The target die's identifier within Socket.
 * @param[out] NumberOfComplexes              Pointer to the number of enabled complexes on
 *                                            the given socket / die.
 * @param[out] NumberOfCores                  Pointer to the number of enabled cores per
 *                                            complex on the given socket / die.
 * @param[out] NumberOfThreads                Pointer to the number of enabled threads per
 *                                            core on the given socket / die.
 *
 * @retval EFI_SUCCESS                        The core topology information was successfully retrieved.
 * @retval EFI_INVALID_PARAMETER              Socket is non-existent.
 * @retval EFI_INVALID_PARAMETER              Die is non-existent.
 * @retval EFI_INVALID_PARAMETER              All output parameter pointers are NULL.
 *
 **/
EFI_STATUS
SmuV13GetCoreTopologyOnDie (
  IN       AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL *This,
  IN       UINTN                                  Socket,
  IN       UINTN                                  Die,
     OUT   UINTN                                  *NumberOfCcds,
     OUT   UINTN                                  *NumberOfComplexes,
     OUT   UINTN                                  *NumberOfCores,
     OUT   UINTN                                  *NumberOfThreads
  )
{
  UINT32                                        ApobInstanceId;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;
  APOB_TYPE_HEADER                              *ApobEntry;
  UINT32                                        Index;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);
  ApobInstanceId = ((UINT32) Socket << 8) | (UINT32) Die;
  AmdPspGetApobEntryInstance (APOB_CCX, APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE, ApobInstanceId, FALSE, &ApobEntry);
  CopyMem (&ApobCcdLogToPhysMap, ApobEntry, sizeof (APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT));

  *NumberOfCcds = 1;
  *NumberOfComplexes = 0;
  *NumberOfCores = 0;
  *NumberOfThreads = 0;

  for (Index = 0;Index < CCX_MAX_COMPLEXES_PER_DIE; Index++) {
    if (ApobCcdLogToPhysMap.CcdMap[0].ComplexMap[Index].PhysComplexNumber != CCX_NOT_PRESENT) {
      (*NumberOfComplexes)++;
    } else {
      break;
    }
  }

  for (Index = 0;Index < CCX_MAX_CORES_PER_COMPLEX; Index++) {
    if (ApobCcdLogToPhysMap.CcdMap[0].ComplexMap[0].CoreInfo[Index].PhysCoreNumber != CCX_NOT_PRESENT) {
      (*NumberOfCores)++;
    } else {
      break;
    }
  }

  for (Index = 0;Index < CCX_MAX_THREADS_PER_CORE; Index++) {
    if (ApobCcdLogToPhysMap.CcdMap[0].ComplexMap[0].CoreInfo[0].IsThreadEnabled[Index] == TRUE) {
      (*NumberOfThreads)++;
    } else {
      break;
    }
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);
  return EFI_SUCCESS;
}

/**
 * This service will start a core to fetch its first instructions from the reset
 * vector.  This service may only be called from the BSP.
 *
 * @param[in]  This                           A pointer to the
 *                                            AMD_CORE_TOPOLOGY_SERVICES_PROTOCOL instance.
 * @param[in]  Socket                         Zero-based socket number of the target thread.
 * @param[in]  Die                            Zero-based die number within Socket of the target thread.
 * @param[in]  LogicalCcd                     Zero-based logical core complex die of the target thread.
 * @param[in]  LogicalComplex                 Zero-based logical complex number of the target thread.
 * @param[in]  LogicalCore                    Zero-based logical core number of the target thread.
 * @param[in]  LogicalThread                  Zero-based logical thread number of the target thread.
 *
 * @retval EFI_SUCCESS                        The thread was successfully launched.
 * @retval EFI_DEVICE_ERROR                   The thread has already been launched.
 * @retval EFI_INVALID_PARAMETER              Socket is non-existent.
 * @retval EFI_INVALID_PARAMETER              Die is non-existent.
 * @retval EFI_INVALID_PARAMETER              LogicalComplex is non-existent.
 * @retval EFI_INVALID_PARAMETER              LogicalThread is non-existent.
 *
 **/
EFI_STATUS
SmuV13LaunchThread (
  IN       AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL *This,
  IN       UINTN                                  Socket,
  IN       UINTN                                  Die,
  IN       UINTN                                  LogicalCcd,
  IN       UINTN                                  LogicalComplex,
  IN       UINTN                                  LogicalCore,
  IN       UINTN                                  LogicalThread
  )
{

  DXE_AMD_NBIO_PCIE_SERVICES_PROTOCOL           *PcieServicesProtocol;
  PCIe_PLATFORM_CONFIG                          *Pcie;
  EFI_STATUS                                    Status;
  GNB_HANDLE                                    *GnbHandle;
  GNB_PCIE_INFORMATION_DATA_HOB                 *PciePlatformConfigHobData;
  THREAD_ENABLE_STRUCT                          ThreadEnable;
  UINTN                                         NumberOfCcds;
  UINTN                                         NumberOfComplexes;
  UINTN                                         NumberOfCores;
  UINTN                                         NumberOfLogicalThreads;
  UINT32                                        SmuArg[6];
  UINT32                                        ApobInstanceId;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;
  APOB_TYPE_HEADER                              *ApobEntry;
  UINTN                                         WhichBit;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);

  Status = EFI_SUCCESS;
  GnbHandle = NULL;
  // Need topology structure
  Status = gBS->LocateProtocol (
                  &gAmdNbioPcieServicesProtocolGuid,
                  NULL,
                  &PcieServicesProtocol
                  );

  ASSERT (Status == EFI_SUCCESS);
  if (Status == EFI_SUCCESS) {
    PcieServicesProtocol->PcieGetTopology (PcieServicesProtocol, (UINT32 **) &PciePlatformConfigHobData);
    Pcie = &(PciePlatformConfigHobData->PciePlatformConfigHob);
    GnbHandle = NbioGetHandle (Pcie);
    while (GnbHandle != NULL) {
      //Find GnbHandle for this Socket
      if (Socket == GnbHandle->SocketId) {
        break;
      }
      GnbHandle = GnbGetNextHandle (GnbHandle);
    }
    ASSERT (GnbHandle != NULL);
  }
  if (GnbHandle != NULL) {
    ApobInstanceId = ((UINT32) Socket << 8) | (UINT32) Die;
    AmdPspGetApobEntryInstance (APOB_CCX, APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE, ApobInstanceId, FALSE, &ApobEntry);
    CopyMem (&ApobCcdLogToPhysMap, ApobEntry, sizeof (APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT));

    if (LogicalCcd != 0) {
      return EFI_INVALID_PARAMETER;
    }
    if ((LogicalComplex >= CCX_MAX_COMPLEXES_PER_DIE) ||
        (ApobCcdLogToPhysMap.CcdMap[0].ComplexMap[LogicalComplex].PhysComplexNumber == CCX_NOT_PRESENT)) {
      return EFI_INVALID_PARAMETER;
    }
    if ((LogicalCore >= CCX_MAX_CORES_PER_COMPLEX) ||
        (ApobCcdLogToPhysMap.CcdMap[0].ComplexMap[LogicalComplex].CoreInfo[LogicalCore].PhysCoreNumber == CCX_NOT_PRESENT)) {
      return EFI_INVALID_PARAMETER;
    }
    if ((LogicalThread >= CCX_MAX_THREADS_PER_CORE) ||
        (ApobCcdLogToPhysMap.CcdMap[0].ComplexMap[LogicalComplex].CoreInfo[LogicalCore].IsThreadEnabled[LogicalThread] == FALSE)) {
      return EFI_INVALID_PARAMETER;
    }

    Status = SmuV13GetCoreTopologyOnDie (This, Socket, Die, &NumberOfCcds, &NumberOfComplexes, &NumberOfCores, &NumberOfLogicalThreads);

    if (Status == EFI_SUCCESS) {
      WhichBit = LogicalComplex * 8;
      WhichBit += (LogicalCore * NumberOfLogicalThreads) + LogicalThread;

      SmnRegisterRead (GnbHandle->Address.Address.Bus,
                       SMN_THREAD_ENABLE_ADDRESS,
                       &ThreadEnable
                       );

      if ((ThreadEnable.Field.ThreadEn & (1 << WhichBit)) == 0) {
        ThreadEnable.Field.ThreadEn |= 1 << WhichBit;
        SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                          SMN_THREAD_ENABLE_ADDRESS,
                          &ThreadEnable,
                          0
                          );
        NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
        SmuArg[0] = 0xAA55AA55;
        Status = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), BIOSSMC_MSG_TestMessage, SmuArg, GNB_REG_ACC_FLAG_S3SAVE);
      } else {
        Status = EFI_DEVICE_ERROR;
      }
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);
  return Status;
}


