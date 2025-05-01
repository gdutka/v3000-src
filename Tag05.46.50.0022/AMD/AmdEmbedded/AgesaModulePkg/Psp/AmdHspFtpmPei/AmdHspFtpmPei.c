/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiPei.h>
#include <Ppi/AmdFtpmPpi.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/AmdPspRecoveryDetectPpi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspCommonLib.h>
#include <Library/AmdCapsuleLib.h>
#include <Library/AmdFtpmLib.h>
#include <Library/FchBaseLib.h>
#include <Library/AmdHeapLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/FabricResourceManagerLib.h>
#include <Library/PciLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <IndustryStandard/Pci22.h>
#include <Gnb.h>
#include <Hsp.h>
#include <Library/HspLib.h>
#include <Filecode.h>
#include <Ppi/FabricResourceManagerServicesPpi.h>

#define FILECODE PSP_AMDHSPFTPMPEI_AMDHSPFTPMPEI_FILECODE

#define STRAP_APER0_EN_DEV2_F2      0x10136418
#define HSP_TEMP_BUS                0x3F

#define HSP_BASE_ADDR_LO            0x13B102D0
#define HSP_BASE_ADDR_HI            0x13B102D4
#define HSP_ACPI_DEVICE_MMIO_SIZE   0x100000


extern
VOID
HspDumpData (
  IN  VOID    *p,
  IN  UINT32  iSize
  );

EFI_STATUS
EFIAPI
HspFtpmEndOfPeiSignalPpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

/**
 * @brief   Query Tcg logs from PSP.
 *            1. Send command to PSP and get Tcg logs.
 *            2. Save Tcg logs to AGESA heap.
 *
 * @param   PeiServices     EFI_PEI_SERVICES.
 * @return  EFI_STATUS      EFI_SUCCESS             Success
 *                          EFI_OUT_OF_RESOURCES    No resource to save TCG log
 *                          EFI_NOT_FOUND           No TCG log found.
 */
STATIC
EFI_STATUS
QueryHspfTPMTcgLogs (
  IN CONST EFI_PEI_SERVICES         **PeiServices
  )
{
  EFI_STATUS            Status;
  ALLOCATE_HEAP_PARAMS  AllocParams;
  PTCGLOGS              pTcgLogs;
  UINT32                TcgsBufferPhyAddressLo;
  UINT32                TcgsBufferPhyAddressHi;
  UINT32                TcgsBufferSize;

  Status                  = EFI_SUCCESS;
  pTcgLogs                = NULL;
  TcgsBufferPhyAddressLo  = 0;
  TcgsBufferPhyAddressHi  = 0;
  TcgsBufferSize          = 0;
  ZeroMem (&AllocParams, sizeof (AllocParams));

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.QueryHspfTPMTcgLogs\n");

  // 1. Get Tcg logs from PSP
  Status = PspMboxGetTcgLogs (
             &TcgsBufferPhyAddressLo,
             &TcgsBufferPhyAddressHi,
             &TcgsBufferSize
             );

  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tQuery Tcg Logs from PSP. Status=%r, Size=0x%x, AddressLo=0x%x, AddressHi=0x%x\n",
    Status,
    TcgsBufferSize,
    TcgsBufferPhyAddressLo,
    TcgsBufferPhyAddressHi
    );

#if HSP_DEBUG_ENABLE
  if (TcgsBufferPhyAddressHi) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tQuery Tcg Logs failed\n");
    return EFI_NOT_FOUND;
  }
#else
  ASSERT (TcgsBufferPhyAddressHi==0);
#endif

  if ( EFI_ERROR (Status) || ( 0 == TcgsBufferSize ) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tQuery Tcg Logs failed\n");
    return EFI_NOT_FOUND;
  }

#if HSP_DEBUG_ENABLE
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tDump Tcg Log:\n");
  HspDumpData ((VOID*)(UINTN)(TcgsBufferPhyAddressLo), TcgsBufferSize);
#endif

  if ( *(volatile UINT32*)(UINTN)(TcgsBufferPhyAddressLo) == 0xFFFFFFFF ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tQuery Tcg Logs Data invalid\n");
    return EFI_NOT_FOUND;
  }

  // 2. Copy the Tcg Logs to AGESA heap
  AllocParams.RequestedBufferSize = sizeof (TCGLOGS)+TcgsBufferSize;
  AllocParams.BufferHandle = AMD_HSP_GET_TCG_LOGS;
  AllocParams.Persist = HEAP_BOOTTIME_SYSTEM_MEM;
  if (HeapAllocateBuffer (&AllocParams, NULL) == AGESA_SUCCESS) {
    pTcgLogs = (PTCGLOGS)AllocParams.BufferPtr;
    pTcgLogs->Status = EFI_SUCCESS;
    pTcgLogs->TcgLogsSize = TcgsBufferSize;

    CopyMem ((VOID*)(((UINT8*)pTcgLogs)+sizeof (TCGLOGS)), (VOID*)TcgsBufferPhyAddressLo, TcgsBufferSize);
    return pTcgLogs->Status;
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tAGESA heap failed.\n");
    return (EFI_OUT_OF_RESOURCES);
  }
}


/**
 * @brief   Return HSP TPM status.
 *
 * @param   This            Pointer to AMD_FTPM_PROTOCOL.
 * @param   FtpmStatus      fTPM status.
 * @return  EFI_STATUS      EFI_SUCCESS              HSP fTPM is ready.
 *                          EFI_UNSUPPORTED          HSP fTPM is not ready.
 */
EFI_STATUS
GetHspfTPMStatus (
  IN       AMD_FTPM_PPI         *This,
  IN OUT   UINTN                *FtpmStatus
  )
{
  return (FtpmGetInfo (FtpmStatus));
}


/**
 * @brief   Send TPM CRB command.
 *
 * @param   This            Pointer to AMD_FTPM_PROTOCOL.
 * @param   CommandBuffer   Command buffer address.
 * @param   CommandSize     Command buffer size.
 * @return  EFI_STATUS      EFI_SUCCESS                Command executed successfully
 *                          EFI_UNSUPPORTED            Device unsupported
 *                          EFI_TIMEOUT                Command fail due the time out
 *                          EFI_DEVICE_ERROR           Command fail due the error status set
 *                          EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 */
EFI_STATUS
SendHspfTPMCommand (
  IN     AMD_FTPM_PPI         *This,
  IN     VOID                 *CommandBuffer,
  IN     UINTN                 CommandSize
  )
{
  return FtpmSendCommand (
           CommandBuffer,
           CommandSize);
}


/**
 * @brief   Get TPM CRB command response data.
 *
 * @param   This            Pointer to AMD_FTPM_PROTOCOL.
 * @param   ResponseBuffer  buffer address.
 * @param   ResponseSize    buffer size.
 * @return  EFI_STATUS      EFI_SUCCESS                Command executed successfully
 *                          EFI_UNSUPPORTED            Device unsupported
 *                          EFI_TIMEOUT                Command fail due the time out
 *                          EFI_DEVICE_ERROR           Command fail due the error status set
 *                          EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 */
EFI_STATUS
GetHspfTPMResponse (
  IN       AMD_FTPM_PPI          *This,
  IN OUT   VOID                  *ResponseBuffer,
  IN OUT   UINTN                 *ResponseSize
  )
{
  EFI_STATUS Status;
  Status = FtpmGetResponse (
             ResponseBuffer,
             ResponseSize
             );
  return (Status);
}


/**
 * @brief   Get Tcg logs from PSP.
 *           1. Retrieve Tcg logs from AGESA heap.
 *           2. Return Tcg logs to the caller.
 *
 * @param   This            Pointer to AMD_FTPM_PROTOCOL.
 * @param   ResponseBuffer  buffer address.
 * @param   ResponseSize    buffer size.
 * @return  EFI_STATUS      EFI_SUCCESS                Success
 *                          EFI_DEVICE_ERROR           Get Tcg Log fail due the error status set
 *                          EFI_BUFFER_TOO_SMALL       Input Response Size is too small for Tcg Logs.
 *                          EFI_NOT_FOUND         -    Do not find AGESA heap for Tcg Logs.
 */
EFI_STATUS
GetHspfTPMTcgLogs (
  IN     AMD_FTPM_PPI          *This,
  IN OUT VOID                  *ResponseBuffer,
  IN OUT UINTN                 *ResponseSize
  )
{
  LOCATE_HEAP_PTR LocateHeapParams;
  PTCGLOGS        pTcgLogs;

  pTcgLogs  = NULL;
  ZeroMem (&LocateHeapParams, sizeof (LocateHeapParams));

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.PEI.GetHspfTPMTcgLogs\n");

  LocateHeapParams.BufferHandle = AMD_HSP_GET_TCG_LOGS;
  if (HeapLocateBuffer (&LocateHeapParams, NULL) == AGESA_SUCCESS) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tFind TCG log Heap.\n");

    pTcgLogs = (PTCGLOGS)LocateHeapParams.BufferPtr;

    if ( !EFI_ERROR (pTcgLogs->Status) ) {
      if ( 0 == *ResponseSize ) {
        // Caller need the Size, Just return the size
        *ResponseSize = pTcgLogs->TcgLogsSize;
        IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCaller need the Size, just return the size (%d).\n", *ResponseSize);
        return (EFI_SUCCESS);
      } else {
        //Validate Input parameters
        if ( ResponseBuffer == NULL ) {
          IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInvalid parameters. ResponseBuffer is NULL\n");
          return (EFI_INVALID_PARAMETER);
        }

        if ( pTcgLogs->TcgLogsSize > *ResponseSize ) {
          IDS_HDT_CONSOLE_PSP_TRACE (
            "\t\tResponse buffer size is too small for Tcg logs. Response Size = %d, TCG Logs Size = %d\n",
            *ResponseSize,
            pTcgLogs->TcgLogsSize
            );
          return (EFI_BUFFER_TOO_SMALL);
        } else {
          *ResponseSize = pTcgLogs->TcgLogsSize;
          if ( pTcgLogs->TcgLogsSize ) {
            CopyMem (ResponseBuffer, (VOID*)(((UINT8*)pTcgLogs)+sizeof (TCGLOGS)) , pTcgLogs->TcgLogsSize);
          }
          return (EFI_SUCCESS);
        }
      }
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tGet the TCG Logs heap, but the status is not EFI_SUCCESS.\n");
      return (EFI_DEVICE_ERROR);
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tNo TCG logs.\n");
    return (EFI_NOT_FOUND);
  }
}


/**
 * @brief   Execute TPM CRB command.
 *
 * @param   This            Pointer to AMD_FTPM_PROTOCOL.
 * @param   CommandBuffer   Command buffer address.
 * @param   CommandSize     Command buffer size.
 * @param   ResponseBuffer  Response buffer address.
 * @param   ResponseSize    Response buffer size.
 * @return  EFI_STATUS      EFI_SUCCESS                Command executed successfully
 *                          EFI_UNSUPPORTED            Device unsupported
 *                          EFI_TIMEOUT                Command fail due the time out
 *                          EFI_DEVICE_ERROR           Command fail due the error status set
 *                          EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 */
EFI_STATUS
ExecuteHspfTPMCommand (
  IN       AMD_FTPM_PPI         *This,
  IN       VOID                 *CommandBuffer,
  IN       UINTN                CommandSize,
  IN OUT   VOID                 *ResponseBuffer,
  IN OUT   UINTN                *ResponseSize
  )
{
  return FtpmExecuteCommand (
           CommandBuffer,
           CommandSize,
           ResponseBuffer,
           ResponseSize
           );
}


STATIC AMD_FTPM_PPI mAmdHspfTPMPpi = {
  ExecuteHspfTPMCommand,
  GetHspfTPMStatus,
  SendHspfTPMCommand,
  GetHspfTPMResponse,
  GetHspfTPMTcgLogs
};

STATIC EFI_PEI_PPI_DESCRIPTOR mAmdHspfTPMPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdFtpmPpiGuid,
  &mAmdHspfTPMPpi
};

STATIC AMD_FTPM_FACTORY_RESET_PPI mAmdPspFtpmFactoryResetPpi = {
  0x00000001ul
};

STATIC EFI_PEI_PPI_DESCRIPTOR mAmdPspFtpmFactoryResetPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdFtpmFactoryResetPpiGuid,
  &mAmdPspFtpmFactoryResetPpi
};

STATIC AMD_PSP_RECOVERY_DETECT_PPI mPspRecoveryDetectPpi = {
  0x01
};

STATIC EFI_PEI_PPI_DESCRIPTOR mPspRecoveryDetectedPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPspRecoveryDetectPpiGuid,
  &mPspRecoveryDetectPpi
};

EFI_PEI_NOTIFY_DESCRIPTOR   mHspFtpmEndOfPeiSignalPpiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) HspFtpmEndOfPeiSignalPpiCallback
};

/**
 * @brief   Allocate MMIO for HSP PCIE device.
 *
 * @param   PeiServices     EFI_PEI_SERVICES.
 * @return  EFI_STATUS      EFI_SUCCESS                Success
 *                          EFI_DEVICE_ERROR           Device Error
 *                          EFI_UNSUPPORTED            Function Unsupported
 */
EFI_STATUS
AllocateHspPciTempMMIO (
  IN CONST EFI_PEI_SERVICES         **PeiServices
  )
{
  EFI_STATUS                       Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *ReadVariable2;
  UINTN                            VariableSize;
  HSP_SAVE_PCIE_CONFIG_SPACE       HspSavePcieConfigSpace;
  HSP_SAVE_PCIE_CONFIG_SPACE       *pHspPcieCfgSpace;
  UINT8                            SleepType;
  UINTN                            BridgeBus;
  UINTN                            BridgeDev;
  UINTN                            BridgeFun;
  UINTN                            HspBus;
  UINTN                            HspDev;
  UINTN                            HspFun;
  UINT64                           HspMmioBase;
  UINT64                           HspMmioLength;
  UINT16                           BridgeMemoryBase;
  UINT16                           BridgeMemoryLimit;
  UINT32                           Temp32;
  UINT32                           HspMmioBaseValue;
  UINT32                           HspH2cHspL0DoorBellValue;
  FABRIC_TARGET                    HspMmioTarget;
  FABRIC_MMIO_ATTRIBUTE            HspMmioAttributes;
  volatile UINT8                   *PtrMmioBase;
  FABRIC_RESOURCE_MANAGER_PPI      *FabricResourceManagerServices;

  Status                   = EFI_SUCCESS;
  ReadVariable2            = NULL;
  pHspPcieCfgSpace         = NULL;
  HspMmioBase              = 0;
  HspMmioLength            = 0;
  BridgeMemoryBase         = 0;
  BridgeMemoryLimit        = 0;
  Temp32                   = 0;
  HspMmioBaseValue         = 0;
  HspH2cHspL0DoorBellValue = 0;
  PtrMmioBase              = NULL;
  SleepType                = FchReadSleepType ();

  ZeroMem (&HspMmioAttributes, sizeof (HspMmioAttributes));
  ZeroMem (&HspMmioTarget, sizeof (HspMmioTarget) );

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.FtpmPei AllocateHspPciTempMMIO.\n");

  BridgeBus  = ( PcdGet16 (PcdAmdHspUpBridgeBDF) >> 8 ) & 0xFF;
  BridgeDev  = ( PcdGet16 (PcdAmdHspUpBridgeBDF) >> 3 ) & 0x1F;
  BridgeFun  = ( PcdGet16 (PcdAmdHspUpBridgeBDF) >> 0 ) & 0x07;

  HspBus     = HSP_TEMP_BUS;
  HspDev     = ( PcdGet16 (PcdAmdHspBDF) >> 3 ) & 0x1F;
  HspFun     = ( PcdGet16 (PcdAmdHspBDF) >> 0 ) & 0x07;

  //1. Check the Device is Pcie Bridge or Not.
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t1. Check the Device is Pcie Bridge or Not.\n");
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, PCI_REVISION_ID_OFFSET));
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- PCIe Device Class Code is 0x%x\n", Temp32);
  if ((Temp32 & 0xFFFFFF00) != ((PCI_CLASS_BRIDGE << 24) | (PCI_CLASS_BRIDGE_P2P << 16) | (PCI_IF_BRIDGE_P2P << 8))) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- ERROR: Bus: %d Dev: %d Fun: %d is not a PCI Bridge. Quit\n", BridgeBus, BridgeDev, BridgeFun);
    return EFI_DEVICE_ERROR;
  }

  //2. PCIe Bridge: Set SecondaryBus and SubordinateBus Value.
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t2. PCIe Bridge: Set SecondaryBus and SubordinateBus Value.\n");
  PciWrite8 (PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET), (UINT8)HspBus);
  PciWrite8 (PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET), (UINT8)HspBus);
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\t\t|- Set PCIe Bridge Sec/Sub Bus Number, and Read Back Value is 0x%x/0x%x\n",
    PciRead8 (PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET)),
    PciRead8 (PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET))
    );

  //3. Check Hsp Device is Existing or Not.
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t3. Check Hsp Device is Existing or Not.\n");
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (HspBus, HspDev, HspFun, PCI_VENDOR_ID_OFFSET));
  if (HSP_VIDDID != Temp32) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- ERROR: Hsp Device isn't Existing...\n");
    return EFI_DEVICE_ERROR;
  }

  //4. Get HSP Pcie Device BAR0 Size
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t4. Get HSP Pcie Device BAR0 Size.\n");
  PciWrite32 (PCI_LIB_ADDRESS (HspBus, HspDev, HspFun, PCI_BASE_ADDRESSREG_OFFSET), 0xFFFFFFFF);
  Temp32 = PciRead32 (PCI_LIB_ADDRESS (HspBus, HspDev, HspFun, PCI_BASE_ADDRESSREG_OFFSET));
  if ( Temp32 == 0xFFFFFFFF ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- Hsp Pcie BAR0 return all 0xFF\n");
    return EFI_DEVICE_ERROR;
  } else if ( Temp32 == 0 ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- Hsp Pcie BAR0 return 0, no MMIO needed.\n");
    return EFI_DEVICE_ERROR;
  } else {
    HspMmioLength = (UINT64)(~Temp32 + 1);
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- Get Hsp Pcie MMIO Size: 0x%lx. (Raw data: 0x%x)\n", HspMmioLength, Temp32);
  }

  //5. Allocate HSP Pcie Device BAR0 Mmio.
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t5. Allocate HSP Pcie Device BAR0 Mmio.\n");
  HspMmioTarget.TgtType   = TARGET_PCI_BUS;
  HspMmioTarget.SocketNum = 0;
  HspMmioTarget.RbNum     = 0;
  HspMmioTarget.PciBusNum = (UINT16)HspBus;

  HspMmioAttributes.ReadEnable  = 1;
  HspMmioAttributes.WriteEnable = 1;
  HspMmioAttributes.NonPosted   = 0;
  HspMmioAttributes.MmioType    = NON_PCI_DEVICE_BELOW_4G;

  // [NOTE] Fabric Mmio allocation is still required during S3 resume,
  // otherwise the system will hang at S3 Resume when VBS is enabled.
  Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gAmdFabricResourceManagerServicesPpiGuid,
                            0,
                            NULL,
                            (VOID **) &FabricResourceManagerServices
                            );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCan not locate FabricResourceManagerServices PPI:%r", Status);
    ASSERT (FALSE);
    return Status;
  }
  Status = FabricResourceManagerServices->FabricAllocateMmio (
             &HspMmioBase,
             &HspMmioLength,
             (UINT64)(~Temp32),
             HspMmioTarget,
             &HspMmioAttributes
             );
  if (EFI_ERROR(Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- Cannot Allocate Fabric Mmio\n");
    return Status;
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- Allocate Fabric Mmio: 0x%lx\n", HspMmioBase);
    if ( (HspMmioBase >> 32) != 0 ) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- Cannot Allocate Fabric Mmio that is not Under 4GB\n");
      return EFI_DEVICE_ERROR;
    }
  }

  if (SleepType != 3) {
    BridgeMemoryBase  = ((UINT32) HspMmioBase >> 16 ) & 0xFFF0;
    BridgeMemoryLimit = ((((UINT32) HspMmioBase + (UINT32) HspMmioLength) >> 16) & 0xFFF0);
  } else {
    pHspPcieCfgSpace = &HspSavePcieConfigSpace;

    Status = (**PeiServices).LocatePpi (
                               PeiServices,
                               &gEfiPeiReadOnlyVariable2PpiGuid,
                               0,
                               NULL,
                               (VOID**)&ReadVariable2
                               );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tLocatePpi: gEfiPeiReadOnlyVariable2PpiGuid Fail\n");
      return Status;
    }

    VariableSize = sizeof (HSP_SAVE_PCIE_CONFIG_SPACE);
    Status = ReadVariable2->GetVariable (
                              ReadVariable2,
                              HSP_PCICFG_S3_SAVE_UEFI_VARIABLE_NAME,
                              &gAmdHspPciCfgS3SaveUefiVariableGuid,
                              NULL,
                              &VariableSize,
                              (VOID *) pHspPcieCfgSpace
                              );

    if (EFI_ERROR(Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tGet Hsp Pcie Config S3 Save UEFI Variable Fail, Status: %r\n",   Status);
      return Status;
    }

    if (pHspPcieCfgSpace == NULL) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tpHspPcieCfgSpace is NULL\n");
      return EFI_UNSUPPORTED;
    }

    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHsp Pcie Config Space CheckFlag: 0x%08x\n", pHspPcieCfgSpace->CheckFlag);
    if (pHspPcieCfgSpace->CheckFlag != HSP_PCICFG_UEFI_VARIABLE_CHECK_FLAG) {
      return EFI_UNSUPPORTED;
    }

    // Set BAR0 Address (It will not use FabricAllocateMmio Space when S3 Resume)
    HspMmioBase        = pHspPcieCfgSpace->HspPcieDeviceReg.Bar0;
    BridgeMemoryBase   = pHspPcieCfgSpace->HspPcieBridgeReg.MemoryBase;
    BridgeMemoryLimit  = pHspPcieCfgSpace->HspPcieBridgeReg.MemoryLimit;
  }

  //6. Set HSP Pcie Device BAR0 Register.
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t6. Set HSP Pcie Device BAR0 Register.\n");
  PciWrite32 (
    PCI_LIB_ADDRESS (HspBus, HspDev, HspFun, PCI_BASE_ADDRESSREG_OFFSET),
    (UINT32) HspMmioBase
    );
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\t\t|- Read Back the HSP BAR0 Register Value: 0x%08x\n",
    PciRead32 (PCI_LIB_ADDRESS (HspBus, HspDev, HspFun, PCI_BASE_ADDRESSREG_OFFSET))
    );

  //7. Set HSP Device: Offset: 0x04 ~ 0x5, Command.
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t7. Set HSP Device: Offset: 0x04 ~ 0x5, Command.\n");
  PciWrite16 (
    PCI_LIB_ADDRESS (HspBus, HspDev, HspFun, PCI_COMMAND_OFFSET),
    PciRead16 (PCI_LIB_ADDRESS (HspBus, HspDev, HspFun, PCI_COMMAND_OFFSET)) | ( EFI_PCI_COMMAND_MEMORY_SPACE + EFI_PCI_COMMAND_BUS_MASTER )
    );
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\t\t|- Read Back the HSP Device CMD_STS value: 0x%08x, CMD_STS. Memory Space = %d, Bus Master = %d\n",
    PciRead32 (PCI_LIB_ADDRESS (HspBus, HspDev, HspFun, PCI_COMMAND_OFFSET)),
    ( PciRead32 (PCI_LIB_ADDRESS (HspBus, HspDev, HspFun, PCI_COMMAND_OFFSET)) & EFI_PCI_COMMAND_MEMORY_SPACE ) ? 1 : 0,
    ( PciRead32 (PCI_LIB_ADDRESS (HspBus, HspDev, HspFun, PCI_COMMAND_OFFSET)) & EFI_PCI_COMMAND_BUS_MASTER ) ? 1 : 0
    );

  //8. Set HSP Bridge: Offset 0x20 ~ 0x23, Memory Base & Memory Limit.
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t8. Set HSP Bridge: Offset 0x20 ~ 0x23, Memory Base & Memory Limit.\n");
  PciWrite16 (PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, 0x20), BridgeMemoryBase);
  PciWrite16 (PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, 0x22), BridgeMemoryLimit);
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\t\t|- Read Back Upstream Bridge Limit/Base 0x%08x\n",
    PciRead32 (PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, 0x20))
    );

  //9. Set HSP Bridge: Offset: 0x4 ~ 0x5, Command.
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t9. Set HSP Bridge: Offset: 0x4 ~ 0x5, Command.\n");
  PciWrite16 (
    PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, PCI_COMMAND_OFFSET),
    PciRead16 (PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, PCI_COMMAND_OFFSET)) | ( EFI_PCI_COMMAND_MEMORY_SPACE + EFI_PCI_COMMAND_BUS_MASTER )
    );
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\t\t|- Read Back Upstream Bridge CMD_STS: 0x%x, CMD_STS. Memory Space = %d, Bus Master = %d\n",
    PciRead16 (PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, PCI_COMMAND_OFFSET)),
    ( PciRead16 (PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, PCI_COMMAND_OFFSET)) & EFI_PCI_COMMAND_MEMORY_SPACE ) ? 1 : 0,
    ( PciRead16 (PCI_LIB_ADDRESS (BridgeBus, BridgeDev, BridgeFun, PCI_COMMAND_OFFSET)) & EFI_PCI_COMMAND_BUS_MASTER ) ? 1 : 0
    );

  //10. Make sure STRAP_APER0_EN_DEV2_F2 for Aperture is Enabled.
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t10. Make sure STRAP_APER0_EN_DEV2_F2 for Aperture is Enabled.\n");
  FtpmSetValueSMN (
    STRAP_APER0_EN_DEV2_F2,
    FtpmGetValueSMN (STRAP_APER0_EN_DEV2_F2) | BIT0,
    0
    );

  //11. Test HSP Pcie Mmio BAR0.
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t11. Test HSP Pcie Mmio BAR0.\n");
  PtrMmioBase              =   (UINT8  *)(UINTN) HspMmioBase;
  HspMmioBaseValue         = *((UINT32 *) PtrMmioBase);
  HspH2cHspL0DoorBellValue = *((UINT32 *)(PtrMmioBase + HSP_H2C_HSP_L0_DOORBELL_OFFSET));
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- Try to Read 4 Bytes of MMIO Address: 0x%08x and Get Value: 0x%08x\n", \
    PtrMmioBase, HspMmioBaseValue);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- Try to Read 4 Bytes of MMIO Address: 0x%08x and Get Value: 0x%08x\n", \
    (PtrMmioBase + HSP_H2C_HSP_L0_DOORBELL_OFFSET), HspH2cHspL0DoorBellValue );
  if ((0xFFFFFFFF == HspMmioBaseValue) && (0xFFFFFFFF == HspH2cHspL0DoorBellValue)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- The Mmio BAR0 Address is not Work.\n");
      return EFI_DEVICE_ERROR;
    }

  //12. Set PcdAmdHspMmioBase.
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t12. Set PcdAmdHspMmioBase.\n");
  PcdSet32S (PcdAmdHspMmioBase, (UINT32) HspMmioBase);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t|- PcdAmdHspMmioBase = 0x%08x\n", PcdGet32 (PcdAmdHspMmioBase));

  return EFI_SUCCESS;
}

/**
 * @brief   Allocate MMIO for HSP ACPI device.
 *
 * @param   PeiServices     EFI_PEI_SERVICES.
 * @return  EFI_STATUS      EFI_SUCCESS                Success
 *                          EFI_DEVICE_ERROR           Device Error
 */
EFI_STATUS
AllocateHspAcpiDeviceMMIO (
  IN CONST EFI_PEI_SERVICES         **PeiServices
  )
{
  EFI_STATUS                    Status;
  UINT64                        HspMmioBase;
  UINT64                        HspMmioLength;
  UINT32                        Value32;
  FABRIC_TARGET                 HspMmioTarget;
  FABRIC_MMIO_ATTRIBUTE         HspMmioAttributes;
  FABRIC_RESOURCE_MANAGER_PPI   *FabricResourceManagerServices;

  Status        = EFI_SUCCESS;
  HspMmioBase   = 0;
  HspMmioLength = HSP_ACPI_DEVICE_MMIO_SIZE;

  ZeroMem (&HspMmioAttributes, sizeof (HspMmioAttributes));
  ZeroMem (&HspMmioTarget, sizeof (HspMmioTarget));

  HspMmioTarget.TgtType   = TARGET_RB;
  HspMmioTarget.SocketNum = 0;
  HspMmioTarget.RbNum     = 0;

  HspMmioAttributes.ReadEnable  = 1;
  HspMmioAttributes.WriteEnable = 1;
  HspMmioAttributes.NonPosted   = 0;
  HspMmioAttributes.MmioType    = NON_PCI_DEVICE_BELOW_4G;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.FtpmPei AllocateHspAcpiTempMMIO.\n");

  // Allocate memory for HSP ACPI device
  IDS_HDT_CONSOLE_PSP_TRACE ("Allocate memory for HSP\n");
  Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gAmdFabricResourceManagerServicesPpiGuid,
                            0,
                            NULL,
                            (VOID **) &FabricResourceManagerServices
                            );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCan not locate FabricResourceManagerServices PPI:%r", Status);
    ASSERT (FALSE);
    return Status;
  }
  Status = FabricResourceManagerServices->FabricAllocateMmio (
             &HspMmioBase,
             &HspMmioLength,
             ALIGN_1M,
             HspMmioTarget,
             &HspMmioAttributes
             );

  if (Status != EFI_SUCCESS) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHsp.FtpmPei Cannot allocate MMIO\n");
    ASSERT (Status == EFI_SUCCESS);
    return Status;
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tMMIO 0x%lx\n", HspMmioBase);
    if ( (HspMmioBase>>32) != 0 ) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tMMIO is not under 4GB\n");
      return EFI_DEVICE_ERROR;
    }
  }

  //set IOHC::HSP_BASE_ADDR_LO and IOHC::HSP_BASE_ADDR_HI
  Value32 = (UINT32)HspMmioBase;
  FtpmSetValueSMN (HSP_BASE_ADDR_LO, Value32 | (BIT0 + BIT8), 0);
  Value32 = (UINT32) RShiftU64(HspMmioBase, 32);
  FtpmSetValueSMN (HSP_BASE_ADDR_HI, Value32, 0);

  //Save MMIO address to PCD.
  PcdSet32S (PcdAmdHspMmioBase, (UINT32)HspMmioBase);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tPcdAmdHspMmioBase = 0x%x\n", PcdGet32 (PcdAmdHspMmioBase));

  return EFI_SUCCESS;
}

/*++

Routine Description:

  Check PSP NVRAM status which used for RPMC feature

Arguments:

Returns:

  EFI_STATUS

--*/

EFI_STATUS
PspCapabilitiesCheck (
  )
{
  EFI_STATUS                  Status;
  UINT32                      PspCapabilities;
  UINT64                      PspNvEntryAddr;
  UINT32                      PspNvEntrySize;


  PspNvEntryAddr = 0;
  PspNvEntrySize = 0;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspCapabilitiesCheck start\n");

  PspCapabilities = 0;
  Status = CheckPspCaps (&PspCapabilities);

  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Get PSP Capabilities fail, status = 0x%x\n", Status);
    return Status;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("PSP Capabilities is 0x%x\n", PspCapabilities);

  //if PSP NVRAM Exists but not healthy, set PcdAmdPspNvramClearRequest to TRUE
  if ((PspCapabilities & PSP_NVRAM_HEALTH) == 0) {
    if (PSPEntryInfoV2 (PSP_NVRAM, &PspNvEntryAddr, &PspNvEntrySize) == TRUE) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PSP NVRAM clear is required\n");
      //Set PCD for PSP NVRAM clear
      PcdSetBoolS(PcdAmdPspNvramClearRequest, TRUE);
    }
  } else {
    PcdSetBoolS(PcdAmdPspNvramClearRequest, FALSE);
  }

  if ((PspCapabilities & PSP_NVRAM_USING_RPMC_PROTECTION) == 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PSP RPMC is fail or NOT enabled\n");
    PcdSetBoolS(PcdAmdPspNvramUsingRpmc, FALSE);
  } else {
    PcdSetBoolS(PcdAmdPspNvramUsingRpmc, TRUE);
  }

  return Status;
}

/**
 * @brief   HSP Driver Initialize Function
 *            1. Check HSP fTPM support bit.
 *            2. Wait for HSP fTPM Lite ready.
 *            3. Retieve the TCG logs.
 *            4. Install HSP fTPM Ppi.
 *
 * @param   PeiServices
 * @return  EFI_STATUS      EFI_SUCCESS                Success
 */
EFI_STATUS
EFIAPI
HspfTpmInitialize (
  IN CONST EFI_PEI_SERVICES         **PeiServices
)
{
  EFI_STATUS                  Status;
  UINT32                      TpmCapabilities;

  Status          = EFI_SUCCESS;
  TpmCapabilities = 0;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.FtpmPei.HspfTpmInitialize\n");

  AGESA_TESTPOINT (TpHspfTpmPeiInitializeEntry, NULL);

  // Allocate MMIO for HSP device.
  if (PcdGetBool(PcdAmdHspAcpiFenceEnable) == FALSE) {
    Status = AllocateHspPciTempMMIO (PeiServices);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tAllocate MMIO for HSP PCIe agent failed.\n");
#if HSP_HIDE_PCIE_DEVICE
      ShowHspPciDevice (FALSE, 0);
#endif
      return Status;
    }
  } else {
    Status = AllocateHspAcpiDeviceMMIO (PeiServices);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tAllocate MMIO for HSP ACPI Device MMIO failed.\n");
      return Status;
    }
  }

  if ((FchReadSleepType () != 3) && (AmdCapsuleGetStatus () == FALSE)) {
    // Check PSP capabilities firstly
    Status = PspCapabilitiesCheck ();
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PspCapabilitiesCheck fail or not supported, PSP NVRAM is skipped\n");
    }

    //If HSP fTPM feature is NOT enabled, exit the callback.
    if (PcdGet8 (PcdAmdPspSystemTpmConfig) != SYSTEM_TPM_CONFIG_HSP_FTPM) {
      IDS_HDT_CONSOLE_PSP_TRACE ("HSP fTPM feature is NOT enabled, HSP ftpm is skipped\n");
      return EFI_SUCCESS;
    }

    //Send PSP mailbox command to ensure Ctrl Area have been initialed
    Status = CheckFtpmCaps (&TpmCapabilities);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Get fTPM cap fail\n");
      return Status;
    }
    //Supported & factory reset cannot be set at the same time
    ASSERT (TpmCapabilities  != (PSP_CAP_TPM_SUPPORTED | PSP_CAP_TPM_REQ_FACTORY_RESET));
    ASSERT (TpmCapabilities  != (PSP_CAP_TPM_SUPPORTED | PSP_CAP_FTPM_NEED_RECOVERY));

    if ((TpmCapabilities & PSP_CAP_TPM_SUPPORTED) == PSP_CAP_TPM_SUPPORTED) {
      // 1. Get TcgLog first.
      Status = QueryHspfTPMTcgLogs (PeiServices);
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHsp get TCG logs status = %r\n", Status);
      if ( EFI_ERROR (Status) ) {
        IDS_HDT_CONSOLE_PSP_TRACE ("\t\tWARNING: Get TCG logs failed.\n");
      }

      // 2. Install Ppi.
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall Hsp PPI.\n");
      Status = (**PeiServices).InstallPpi (PeiServices, &mAmdHspfTPMPpiList);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall Hsp PPI failed.\n");
      }
    }

    if ((TpmCapabilities & PSP_CAP_TPM_REQ_FACTORY_RESET) == PSP_CAP_TPM_REQ_FACTORY_RESET) {
      IDS_HDT_CONSOLE_PSP_TRACE ("FTPM FACTORY_RESET detected\n");
      PcdSetBoolS (PcdAmdFtpmNvCorrupted, TRUE);
      //Install a PPI to notify SBIOS
      IDS_HDT_CONSOLE_PSP_TRACE ("\tInstall fTPM factory reset PPI\n");
      Status = (**PeiServices).InstallPpi((CONST EFI_PEI_SERVICES **)PeiServices, &mAmdPspFtpmFactoryResetPpiList);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("InstallPpi fail\n");
      }
    }

    if ((TpmCapabilities & PSP_CAP_FTPM_NEED_RECOVERY) == PSP_CAP_FTPM_NEED_RECOVERY) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Ftpm Recovery Flag Detected\n");
      Status = (**PeiServices).InstallPpi((CONST EFI_PEI_SERVICES **)PeiServices, &mPspRecoveryDetectedPpiList);
      ASSERT (Status == EFI_SUCCESS);
    }
  }

  AGESA_TESTPOINT (TpHspfTpmPeiInitializeExit, NULL);

  return Status;
}

/**
 * @brief Hsp Ftpm End of Pei Signal Ppi Callback Function
 * @details Ii will hide HSP PCIe Device at this Callback Function
 *
 * @param[in] PeiServices        Pointer to Pei Service
 * @param[in] PeiServices        Pointer to Pei Notify Deccriptor
 * @param[in] VOID               Pointer to Invokee Ppi
 *
 * @return    EFI_STATUS
 *              - EFI_SUCCESS      The Function Completed Successfully.
 */
EFI_STATUS
EFIAPI
HspFtpmEndOfPeiSignalPpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.FtpmPei.HspFtpmEndOfPeiSignalPpiCallback\n");

#if HSP_HIDE_PCIE_DEVICE
  ShowHspPciDevice (FALSE, 0);
#endif

  return Status;
}

/**
 * @brief   HSP PEI Driver Entry point.
 *
 * @param   FileHandle
 * @param   PeiServices
 * @return  EFI_STATUS      EFI_SUCCESS                Success
 */
EFI_STATUS
AmdHspFtpmPeiDriverEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;
  UINT32      Temp32;
  UINT32      Ignored;
  UINT64      EntryValue;

  Status     = EFI_SUCCESS;
  Temp32     = 0;
  Ignored    = 0;
  EntryValue = 0;

  //If PSP feature turn off, exit the driver
  if ((PcdGetBool (PcdAmdPspEnable) == 0) ||
      (PcdGetBool (PcdAmdHspCoreEnable) == FALSE)) {
    return EFI_SUCCESS;
  }

  // HSP Core Enable/Disable
  // EntryValue[36] = 0: Enable, HSP core is enabled.
  // EntryValue[36] = 1: Disable, HSP core is disabled then PSP will gate the HSP clock,
  // not further PSP to HSP commands. System will boot without HSP.
  PSPEntryInfo (AMD_SOFT_FUSE_CHAIN_01, &EntryValue, &Ignored);
  if (EntryValue & BIT36) {
    PcdSetBoolS (PcdAmdHspCoreEnable, FALSE);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tPcdGetBool (PcdAmdHspCoreEnable) = 0x%x\n", PcdGetBool (PcdAmdHspCoreEnable));
    return EFI_SUCCESS;
  }

  AGESA_TESTPOINT (TpHspfTpmPeiEntry, NULL);

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.Drv.FtpmPei Enter\n");

#if HSP_DEBUG_ENABLE
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tPcdAmdPspSystemTpmConfig=0x%x\n", PcdGet8 (PcdAmdPspSystemTpmConfig));
#endif

  // Set PcdAmdHspMmioBase to 0
  PcdSet32S (PcdAmdHspMmioBase, 0);



  // Set HSP PCI or ACPI device
  if (SocFamilyIdentificationCheckV2 (F19_RMB_A0_FAMILY_ID_BASE_MODEL) || SocFamilyIdentificationCheck (F17_MDN_RAW_ID)) {
    PcdSetBoolS (PcdAmdHspAcpiFenceEnable, FALSE);            // PCI device
  } else {
    // PcdAmdHspAcpiFenceEnable should have a default value as TRUE already
    // We set it to TRUE for non-RMB programs just to avoid wrong value set in SBIOS
    if (!SocFamilyIdentificationCheck (F19_RMB_RAW_ID)) {
      PcdSetBoolS (PcdAmdHspAcpiFenceEnable, TRUE);           // ACPI device
    }
  }

  if (PcdGetBool(PcdAmdHspAcpiFenceEnable) == TRUE) {
    ShowHspPciDevice (FALSE, 0);
  } else {
    ShowHspPciDevice (TRUE, 0);
  }

  if (FchReadSleepType () == 3) {
    if (PcdGetBool(PcdAmdHspAcpiFenceEnable) == FALSE) {
      Status = (**PeiServices).NotifyPpi (PeiServices, &mHspFtpmEndOfPeiSignalPpiCallback);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("\tError Installing mHspFtpmEndOfPeiSignalPpiCallback\n");
      }
    }
  }

  Status = HspfTpmInitialize (PeiServices);

  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHsp.Drv.FtpmPei Exit\n");
  AGESA_TESTPOINT (TpHspfTpmPeiExit, NULL);

  return Status;
}

