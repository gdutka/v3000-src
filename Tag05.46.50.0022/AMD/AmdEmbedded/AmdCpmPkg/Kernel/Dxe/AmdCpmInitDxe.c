/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>
#include <AmdCpmBaseIo.h>
#include <AmdCpmCpu.h>
#include <AmdCpmFch.h>
#include <AmdCpmTable.h>
#include <Guid/AmdMemoryInfoHob.h>
#include <FchRegistersCommon.h>
#include <Library/AmdStbLib.h>
#include <Library/AmdIdsDebugPrintLib.h>


AMD_CPM_TABLE_PROTOCOL    *AmdCpmTableProtocolPtr;
AMD_CPM_NV_DATA_PROTOCOL  *AmdCpmNvDataProtocolPtr;
EFI_EVENT                 ReadyToBootEvent;

extern UINT8              AmdCpmSsdtSupport;

EFI_GUID  COMMON_ACPI_DEVICE_GUID = {
  0x5b1e5b38, 0x8ebd, 0x4784, 0x9b, 0x52, 0x38, 0xb2, 0xaa, 0x9b, 0xff, 0x2f
};

VOID
EFIAPI
AmdCpmDxeRegisterKernel (
  IN       VOID                                *This
  );

VOID
EFIAPI
AmdCpmInitLate (
  IN       EFI_EVENT                           Event,
  IN       VOID                                *Context
  );

EFI_STATUS
EFIAPI
AmdCpmAddSsdtTable (
  IN       VOID                                *This,
  IN       VOID                                *EfiGuid,
  IN       UINT64                              *OemTableId,
  IN       AMD_CPM_ADDSSDTCALLBACK_FN          Function,
  IN       VOID                                *Context
  );

VOID
EFIAPI
AmdCpmPciIoProtEarly (
  IN       EFI_EVENT                           Event,
  IN       VOID                                *Context
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Init DXE driver
 *
 * This function installs AmdCpmTableProtocol, AmdCpmNvDataProtocol and CPM Common SSDT Table
 * and registers CPM common functions at AmdCpmInitDxe.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmInitDxeEntryPoint (
  IN       EFI_HANDLE              ImageHandle,
  IN       EFI_SYSTEM_TABLE        *SystemTable
  )
{
  EFI_STATUS                      Status;
  AMD_CPM_MAIN_TABLE              *MainTablePtr;
  VOID                            *HobList;
  VOID                            *Buffer;
  UINTN                           BufferSize;
  UINT8                           *HobBuffer;
  EFI_HANDLE                      Handle;
  AMD_CPM_NV_DATA_STRUCT          *NvDataPtr;
  AMD_CPM_TABLE_LIST              *TableListPtr;
  EFI_EVENT                       Event;
  VOID                            *Registration;
  AMD_MEMORY_INFO_HOB             *MemInfoHob;
  AMD_MEMORY_RANGE_DESCRIPTOR     *MemRangDesc;
  UINT32                          NumberOfDescriptor;
  UINT32                          Index;


  DEBUG((DEBUG_INFO, "KER-DXE-%a-Start\n", __FUNCTION__));
  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  RECORD_TIME_DXE (BeginAmdCpmInitDxeDriver);

  DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmGetSystemConfigurationTable-gEfiHobListGuid\n", __FUNCTION__));
  Status = CpmGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "KER-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  Buffer = NULL;
  BufferSize = 0;
  DEBUG((DEBUG_INFO, "KER-DXE-%a-If_CpmGetHobBuffer-gAmdCpmTableHobGuid-HobList %08x\n", __FUNCTION__, (UINT32)(UINTN)HobList));
  If_CpmGetHobBuffer (HobList, gAmdCpmTableHobGuid, Buffer, BufferSize, GuidHob) {
    DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmRelocateTableList\n", __FUNCTION__));
    CpmRelocateTableList (Buffer);
    TableListPtr = CpmAdjustTableList (Buffer);

    DEBUG((DEBUG_INFO, "KER-DXE-%a-AllocatePool-EfiACPIMemoryNVS-HobBuffer\n", __FUNCTION__));
    Status = gBS->AllocatePool (
                    EfiACPIMemoryNVS,
                    TableListPtr->Size + AMD_TABLE_ADDITIONAL_BUFFER_SIZE,
                    (VOID**)&HobBuffer
                    );

    DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmCopyTableListToMemory\n", __FUNCTION__));
    CpmCopyTableListToMemory (TableListPtr, HobBuffer);
    ((AMD_CPM_HOB_HEADER*)HobBuffer)->Revision = ((AMD_CPM_HOB_HEADER*)Buffer)->Revision;

    DEBUG((DEBUG_INFO, "KER-DXE-%a-AllocatePool-EfiACPIMemoryNVS-AmdCpmTableProtocolPtr\n", __FUNCTION__));
    Status = gBS->AllocatePool (
                    EfiACPIMemoryNVS,
                    sizeof (AMD_CPM_TABLE_PROTOCOL),
                    (VOID**)&AmdCpmTableProtocolPtr
                    );

    MainTablePtr                          = ((AMD_CPM_HOB_HEADER*)HobBuffer)->MainTablePtr.Pointer;
    AmdCpmTableProtocolPtr->Revision      = ((AMD_CPM_HOB_HEADER*)HobBuffer)->Revision;
    AmdCpmTableProtocolPtr->MainTablePtr  = MainTablePtr;
    AmdCpmTableProtocolPtr->SmmMainTablePtr  = NULL;

    DEBUG((DEBUG_INFO, "KER-DXE-%a-AmdCpmDxeRegisterKernel\n", __FUNCTION__));
    AmdCpmDxeRegisterKernel (AmdCpmTableProtocolPtr);
    DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmRegisterBaseIo\n", __FUNCTION__));
    CpmRegisterBaseIo (AmdCpmTableProtocolPtr);
    DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmRegisterCpu\n", __FUNCTION__));
    CpmRegisterCpu (AmdCpmTableProtocolPtr);
    DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmRegisterFch\n", __FUNCTION__));
    CpmRegisterFch (AmdCpmTableProtocolPtr);

    Handle = NULL;
    DEBUG((DEBUG_INFO, "KER-DXE-%a-InstallProtocolInterface-gAmdCpmTableProtocolGuid\n", __FUNCTION__));
    gBS->InstallProtocolInterface (
                    &Handle,
                    &gAmdCpmTableProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    AmdCpmTableProtocolPtr
                    );

    DEBUG((DEBUG_INFO, "KER-DXE-%a-AllocatePool-EfiACPIMemoryNVS-AmdCpmNvDataProtocolPtr\n", __FUNCTION__));
    Status = gBS->AllocatePool (
                    EfiACPIMemoryNVS,
                    sizeof (AMD_CPM_NV_DATA_PROTOCOL),
                    (VOID**)&AmdCpmNvDataProtocolPtr
                    );
    Handle = ImageHandle;
    DEBUG((DEBUG_INFO, "KER-DXE-%a-InstallProtocolInterface-gAmdCpmNvDataProtocolGuid\n", __FUNCTION__));
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gAmdCpmNvDataProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    AmdCpmNvDataProtocolPtr
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "KER-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return Status;
    }

    BufferSize = sizeof (AMD_CPM_NV_DATA_STRUCT);

    DEBUG((DEBUG_INFO, "KER-DXE-%a-AllocatePool-EfiACPIMemoryNVS-NvDataPtr\n", __FUNCTION__));
    Status = gBS->AllocatePool (
                    EfiACPIMemoryNVS,
                    BufferSize,
                    (VOID**)&NvDataPtr
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "KER-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return Status;
    }

    DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmZeroMem\n", __FUNCTION__));
    CpmZeroMem (NvDataPtr, BufferSize);
    NvDataPtr->CpmVersion               = (UINT32)AmdCpmTableProtocolPtr->Revision;
    NvDataPtr->CpmPcieMmioBaseAddr      = MainTablePtr->PcieMemIoBaseAddr;
    NvDataPtr->CpmAcpiMmioBaseAddr      = MainTablePtr->AcpiMemIoBaseAddr;
    NvDataPtr->CpmSbChipId              = AmdCpmTableProtocolPtr->ChipId.Sb;
    NvDataPtr->CpmEcRamGpioBaseOffset   = CFG_CPM_ECRAM_GPIO_OFFSET;
    NvDataPtr->CpmSwSmiPort             = MainTablePtr->SwSmiPort;
    NvDataPtr->CpmSwSmiCmdSetAutoUmaMode  = CFG_CPM_SWSMICMD_SETAUTOUMAMODE;
    NvDataPtr->CpmUmaSize               = 0;
    NvDataPtr->CpmSbStrap               = (UINT8)AmdCpmTableProtocolPtr->CommonFunction.GetStrap (AmdCpmTableProtocolPtr);
    NvDataPtr->CpmMainTable             = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_MAIN_TABLE);
    NvDataPtr->CpmDeviceDetectionTable  = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_GPIO_DEVICE_DETECTION);
    NvDataPtr->CpmDeviceResetTable      = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_GPIO_DEVICE_RESET);
    NvDataPtr->CpmDevicePowerTable      = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_GPIO_DEVICE_POWER);
    NvDataPtr->CpmWirelessButtonTable   = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_WIRELESS_BUTTON);
    NvDataPtr->CpmSaveContextTable      = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_SAVE_CONTEXT);
    NvDataPtr->CpmExpressCardTable      = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_PCIE_EXPRESS_CARD);
    NvDataPtr->CpmDisplayFeatureTable   = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_DISPLAY_FEATURE);
    NvDataPtr->CpmZeroPowerOddTable     = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_ZERO_POWER_ODD);
    NvDataPtr->CpmAcpiThermalFanTable   = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_ACPI_THERMAL_FAN);
    NvDataPtr->CpmAdaptiveS4Table       = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_ADAPTIVE_S4);
    NvDataPtr->CpmPcieClockTable        = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_PCIE_CLOCK);
    NvDataPtr->CpmOtherHotplugCardTable = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_PCIE_OTHER_HOTPLUG_CARD);
    NvDataPtr->CpmCoreTopologyTable     = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_CORE_TOPOLOGY);
    NvDataPtr->CpmUcsiTable             = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_UCSI);
    NvDataPtr->CpmUcsiTunnelTable       = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_UCSI_TUNNEL);
    NvDataPtr->CpmLowPowerEthernetTable = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_PCIE_LOW_POWER_ETHERNET);
    NvDataPtr->CpmWirelessLanRecoveryTable = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_WIRELESS_LAN_RECOVERY);
    NvDataPtr->CpmIoExpanderAcpiEventTable = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_SMBUS_IO_EXPANDER);
    NvDataPtr->CpmThunderboltTable      = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_THUNDERBOLT);
    NvDataPtr->CpmNvmeRtd3Table         = (UINT32) (UINT64)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_NVME_STD3);
    NvDataPtr->CpmSwSmiCmdSetUmaSizeId  = CFG_CPM_SWSMICMD_SETUMASIZEID;
    NvDataPtr->CpmSmbusIoBaseAddr       = MainTablePtr->SmbusIoBaseAddr;
    if (PcdGetBool (PcdCpmDebugPrintInAsl)) {
      if (AmdIdsEmulationAutoDetect()) {
        // It is an emulation system.
        NvDataPtr->CpmDebugPrintSerialAddr = (UINT32) PcdGet16 (PcdIdsDebugPort);
      } else if (PcdGetBool (PcdAmdIdsDebugPrintRedirectIOEnable)) {
        NvDataPtr->CpmDebugPrintSerialAddr = (UINT32) PcdGet16 (PcdIdsDebugPort);
      } else if (PcdGetBool (PcdAmdIdsDebugPrintSerialPortEnable)) {
        NvDataPtr->CpmDebugPrintSerialAddr = (UINT32) PcdGet64 (PcdAmdIdsDebugPrintSerialPort);
      }
      NvDataPtr->CpmUartDivisor           = (UINT16) (PcdGet32 (PcdAmdIdsUartClock) / (PcdGet32 (PcdAmdIdsUartBaudRate) * 16));
      NvDataPtr->CpmUartLineControl       = PcdGet8 (PcdAmdIdsUartLineControl) & 0x3F;
      NvDataPtr->CpmUartFifoControl       = PcdGet8 (PcdAmdIdsUartFifoControl) & 0x21;
      NvDataPtr->CpmUartHwFlowControl     = (PcdGetBool (PcdAmdIdsDebugPrintSerialPortHardwareFlowControl)) ? 1 : 0;
      DEBUG((DEBUG_INFO, "KER-DXE-%a-PcdCpmDebugPrintInAsl    = TRUE\n", __FUNCTION__));
      DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmDebugPrintSerialAddr  = 0x%X\n", __FUNCTION__, NvDataPtr->CpmDebugPrintSerialAddr));
      DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmUartDivisor           = 0x%X\n", __FUNCTION__, NvDataPtr->CpmUartDivisor));
      DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmUartLineControl       = 0x%X\n", __FUNCTION__, NvDataPtr->CpmUartLineControl));
      DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmUartFifoControl       = 0x%X\n", __FUNCTION__, NvDataPtr->CpmUartFifoControl));
      DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmUartHwFlowControl     = 0x%X\n", __FUNCTION__, NvDataPtr->CpmUartHwFlowControl));
    }

    AmdCpmNvDataProtocolPtr->NvDataPtr = NvDataPtr;

    DEBUG((DEBUG_INFO, "KER-DXE-%a-If_CpmGetHobBuffer-gAmdMemoryInfoHobGuid\n", __FUNCTION__));
    MemInfoHob = NULL;
    BufferSize = 0;
    If_CpmGetHobBuffer (HobList, gAmdMemoryInfoHobGuid, MemInfoHob, BufferSize, GuidHob) {
      NumberOfDescriptor = MemInfoHob->NumberOfDescriptor;
      MemRangDesc = &MemInfoHob->Ranges[0];
      DEBUG((DEBUG_INFO, "KER-DXE-%a-Get-CpmUmaSize-From-AMD_MEMORY_INFO_HOB\n", __FUNCTION__));
      for (Index = 0; Index < NumberOfDescriptor; Index++) {
        DEBUG((DEBUG_INFO, "KER-DXE-%a-MemRangDesc[Index=0x%X].Attribute=0x%X\n", __FUNCTION__, Index, MemRangDesc[Index].Attribute));
        DEBUG((DEBUG_INFO, "KER-DXE-%a-MemRangDesc[Index=0x%X].Size=0x%X\n", __FUNCTION__, Index, MemRangDesc[Index].Size));
        if (MemRangDesc[Index].Attribute == AMD_MEMORY_ATTRIBUTE_UMA) {
          NvDataPtr->CpmUmaSize = (UINT32) (MemRangDesc[Index].Size >> 20);
          DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmUmaSize=0x%X\n", __FUNCTION__, NvDataPtr->CpmUmaSize));
          break;
        }
      }
    }

    DEBUG((DEBUG_INFO, "KER-DXE-%a-CreateEventEx-gEfiEventReadyToBootGuid-For-AmdCpmInitLate\n", __FUNCTION__));
    Status = gBS->CreateEventEx (
                    CPM_EVENT_NOTIFY_SIGNAL,
                    CPM_TPL_NOTIFY,
                    AmdCpmInitLate,
                    (VOID*)AmdCpmNvDataProtocolPtr,
                    &gEfiEventReadyToBootGuid,
                    &ReadyToBootEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "KER-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return Status;
    }

    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    AmdCpmPciIoProtEarly,
                    NULL,
                    &Event
                    );
    if (!EFI_ERROR(Status)) {
      Status = gBS->RegisterProtocolNotify(&gEfiPciIoProtocolGuid, Event, &Registration);
      DEBUG ((DEBUG_INFO, "KER-DXE-%a RegisterProtocolNotify - gEfiPciIoProtocolGuid for AmdCpmPciIoProtEarly() callback %r\n", __FUNCTION__, Status));
    }

  }

  RECORD_TIME_DXE (EndAmdCpmInitDxeDriver);

  DEBUG ((DEBUG_INFO, "KER-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Common kernel function to check ACPI object
 *
 * This function is used to check whether it is an AML Op Region Object in the table.
 *
 * @param[in]     TablePtr       Pointer to the AML table
 *
 * @retval        TRUE           Is AML Op Region Object
 * @retval        FALSE          Is not AML Op Region Object
 */
BOOLEAN
IsAmlOpRegionObject (
  IN       UINT8                               *TablePtr
  )
{
  UINT16 *Operation;
  Operation = (UINT16*) (TablePtr - 2);
  if (*Operation == AML_OPREGION_OP ) {
    return TRUE;
  }

  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Common SSDT table
 *
 * This function is used to update the base address and size of the region object: CPNV.
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
AmdCpmCommonSsdtCallBack (
  IN       VOID                                *This,
  IN       VOID                                *AmlObjPtr,
  IN       VOID                                *Context
  )
{
  UINT32                  *RegionAddress;
  UINT32                  *RegionSize;
  UINT32                  *Buffer;
  AMD_CPM_TABLE_PROTOCOL  *TableProtocolPtr;
  STATIC BOOLEAN          M285Invoked = FALSE;
  STATIC BOOLEAN          M450Invoked = FALSE;

  TableProtocolPtr = This;
  Buffer = (UINT32*) Context;
  switch (*((UINT32*)AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('C', 'P', 'N', 'V')):
    if (TableProtocolPtr->CommonFunction.IsAmlOpRegionObject (AmlObjPtr)) {
      DEBUG((DEBUG_INFO, "KER-DXE-%a-UpdateAcpiName-CPNV\n", __FUNCTION__));
      RegionAddress = (UINT32 *) ((UINT8*)AmlObjPtr + 6);
      (* RegionAddress) = *Buffer;
      RegionSize = (UINT32 *) ((UINT8*)AmlObjPtr + 11);
      * RegionSize = *(Buffer + 1);
    }
    break;
  case (CPM_SIGNATURE_32 ('M', 'O', 'E', 'M')):
    if (*(Buffer + 2)) {
      DEBUG((DEBUG_INFO, "KER-DXE-%a-UpdateAcpiName-MOEM\n", __FUNCTION__));
      *((UINT32*)AmlObjPtr) = *(Buffer + 2);
    }
    break;
  case (CPM_SIGNATURE_32 ('M', 'L', 'I', 'B')):
    if (*(Buffer + 3)) {
      DEBUG((DEBUG_INFO, "KER-DXE-%a-UpdateAcpiName-MLIB\n", __FUNCTION__));
      *((UINT32*)AmlObjPtr) = *(Buffer + 3);
    }
    break;
  case (CPM_SIGNATURE_32 ('P', 'C', 'X', 'X')):
    if (*(Buffer + 4)) {
      DEBUG((DEBUG_INFO, "KER-DXE-%a-UpdateAcpiName-PCXX\n", __FUNCTION__));
      *((UINT32*)AmlObjPtr) = *(Buffer + 4);
    }
    break;
  case (CPM_SIGNATURE_32 ('G', 'P', 'X', 'X')):
    if (*(Buffer + 10)) {
      DEBUG((DEBUG_INFO, "KER-DXE-%a-UpdateAcpiName-GPXX\n", __FUNCTION__));
      *((UINT32*)AmlObjPtr) = *(Buffer + 10);
    }
    break;
  case (CPM_SIGNATURE_32 ('S', 'A', 'X', 'X')):
    if (*(Buffer + 11)) {
      DEBUG((DEBUG_INFO, "KER-DXE-%a-UpdateAcpiName-SAXX\n", __FUNCTION__));
      *((UINT32*)AmlObjPtr) = *(Buffer + 11);
    }
    break;
  case CPM_SIGNATURE_32 ('M', '2', '8', '5'):                       // ASL code: CpmPostCodeBuffer: M285
    if (!M285Invoked && (*(Buffer + 5) == 1)) {
      DEBUG((DEBUG_INFO, "KER-DXE-%a-UpdateAcpiName-M285\n", __FUNCTION__));
//    In Function.asi
//      Name (CpmPostCodeBuffer, Buffer (4) {0, 3, 0x80, 0})
//      CreateByteField (CpmPostCodeBuffer, 0, CpmTestPointEnable)  // PcdAgesaTestPointEnable
//      CreateByteField (CpmPostCodeBuffer, 1, CpmTestPointWidth)   // PcdAgesaTestPointWidth
//      CreateWordField (CpmPostCodeBuffer, 2, CpmDebugPort)        // PcdIdsDebugPort
//    In AmdCpmInitDxe.c
//      Buffer[5] = PcdGetBool (PcdAgesaTestPointEnable);
//      Buffer[6] = (UINT32)PcdGet8 (PcdAgesaTestPointWidth);
//      Buffer[7] = (UINT32)PcdGet16 (PcdIdsDebugPort);
        *((UINT8*)AmlObjPtr + 8) = *(UINT8*)(Buffer + 5);
        *((UINT8*)AmlObjPtr + 9) = *(UINT8*)(Buffer + 6);
        *((UINT16*)AmlObjPtr + 5) = *(UINT16*)(Buffer + 7);
        M285Invoked = TRUE;
    }
    break;
  case CPM_SIGNATURE_32 ('M', '4', '5', '0'):                       // ASL code: CpmSmartTraceBuffer: M450
    if (!M450Invoked && (*(Buffer + 13) == 1)) {
      DEBUG((DEBUG_INFO, "KER-DXE-%a-UpdateAcpiName-M450\n", __FUNCTION__));
//    In Function.asi
//      Name (CpmSmartTraceBuffer, Buffer (5) {0, 0x96, 0x5, 0, 0})
//      CreateDwordField (CpmSmartTraceBuffer, 0, CpmSmartTraceSmnAddress)  // PcdStbSmnAddress
//      CreateByteField (CpmSmartTraceBuffer, 4, CpmSmartTraceEnable)       // PcdAgesaTestPointToStb & PcdStbEnable
//    In AmdCpmInitDxe.c
//      Buffer[13] = 1;
//      Buffer[14] = PcdGet32 (PcdStbSmnAddress);
        *((UINT8*)AmlObjPtr + 12) = *(UINT8*)(Buffer + 13);
        *((UINT32*)AmlObjPtr + 2) = *(Buffer + 14);
        M450Invoked = TRUE;
    }
    break;
  case CPM_SIGNATURE_32 ('P', 'T', 'B', 'R'):                       // ASL code: \_SB.PCI0.PTBR.PTST
    if (*(Buffer + 8) != 0xFFFFFFFF) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 8);
    }
    break;
  case CPM_SIGNATURE_32 ('P', 'T', 'S', 'T'):                       // ASL code: \_SB.PCI0.PTBR.PTST
    if (*(Buffer + 9) != 0xFFFFFFFF) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 9);
    }
    break;
  case CPM_SIGNATURE_32 ('X', 'L', '0', 'B'):                       // ASL code: XL0B
    if (*(Buffer + 12) != 0xFF) {
      *(UINT8*)AmlObjPtr = '_';
      *(UINT16 *) ((UINT8*)AmlObjPtr + 2) = *(UINT16*)(Buffer + 12);
    }
    break;

  case CPM_SIGNATURE_32 ('S', 'I', 'E', '0'):                       // ASL code: \_GPE.SIE0
    if ((*(Buffer + 15) != CPM_SIGNATURE_32 ('S', 'I', 'E', '0')) && (*(Buffer + 15) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 15);
    }
    break;
  case CPM_SIGNATURE_32 ('B', 'I', 'E', '0'):                       // ASL code: \_GPE.BIE0
    if ((*(Buffer + 16) != CPM_SIGNATURE_32 ('B', 'I', 'E', '0')) && (*(Buffer + 16) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 16);
    }
    break;
  case CPM_SIGNATURE_32 ('A', 'I', 'E', '0'):                       // ASL code: \_GPE.AIE0
    if ((*(Buffer + 17) != CPM_SIGNATURE_32 ('A', 'I', 'E', '0')) && (*(Buffer + 17) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 17);
    }
    break;
  case CPM_SIGNATURE_32 ('P', 'T', 'S', '0'):                       // ASL code: \_GPE.PTS0
    if ((*(Buffer + 18) != CPM_SIGNATURE_32 ('P', 'T', 'S', '0')) && (*(Buffer + 18) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 18);
    }
    break;
  case CPM_SIGNATURE_32 ('W', 'A', 'K', '0'):                       // ASL code: \_GPE.WAK0
    if ((*(Buffer + 19) != CPM_SIGNATURE_32 ('W', 'A', 'K', '0')) && (*(Buffer + 19) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 19);
    }
    break;
  case CPM_SIGNATURE_32 ('S', 'P', '0', '0'):                       // ASL code: \_GPE.SP00
  case CPM_SIGNATURE_32 ('S', 'P', '0', '1'):                       // ASL code: \_GPE.SP01
  case CPM_SIGNATURE_32 ('S', 'P', '0', '2'):                       // ASL code: \_GPE.SP02
  case CPM_SIGNATURE_32 ('S', 'P', '0', '3'):                       // ASL code: \_GPE.SP03
  case CPM_SIGNATURE_32 ('S', 'P', '0', '4'):                       // ASL code: \_GPE.SP04
  case CPM_SIGNATURE_32 ('S', 'P', '0', '5'):                       // ASL code: \_GPE.SP05
  case CPM_SIGNATURE_32 ('S', 'P', '0', '6'):                       // ASL code: \_GPE.SP06
  case CPM_SIGNATURE_32 ('S', 'P', '0', '7'):                       // ASL code: \_GPE.SP07
    if (((*(Buffer + 20) & 0xFFFFFF) != (CPM_SIGNATURE_32 ('S', 'P', '0', '0') & 0xFFFFFF)) && (*(Buffer + 20) != 0x0)) {
      *((UINT32*)AmlObjPtr) &= 0xFF000000;
      *((UINT32*)AmlObjPtr) |= *(Buffer + 20) & 0xFFFFFF;
    }
    break;
  case CPM_SIGNATURE_32 ('S', 'P', '1', '0'):                       // ASL code: \_GPE.SP10
  case CPM_SIGNATURE_32 ('S', 'P', '1', '1'):                       // ASL code: \_GPE.SP11
  case CPM_SIGNATURE_32 ('S', 'P', '1', '2'):                       // ASL code: \_GPE.SP12
  case CPM_SIGNATURE_32 ('S', 'P', '1', '3'):                       // ASL code: \_GPE.SP13
  case CPM_SIGNATURE_32 ('S', 'P', '1', '4'):                       // ASL code: \_GPE.SP14
  case CPM_SIGNATURE_32 ('S', 'P', '1', '5'):                       // ASL code: \_GPE.SP15
  case CPM_SIGNATURE_32 ('S', 'P', '1', '6'):                       // ASL code: \_GPE.SP16
  case CPM_SIGNATURE_32 ('S', 'P', '1', '7'):                       // ASL code: \_GPE.SP17
    if (((*(Buffer + 21) & 0xFFFFFF) != (CPM_SIGNATURE_32 ('S', 'P', '1', '0') & 0xFFFFFF)) && (*(Buffer + 21) != 0x0)) {
      *((UINT32*)AmlObjPtr) &= 0xFF000000;
      *((UINT32*)AmlObjPtr) |= *(Buffer + 21) & 0xFFFFFF;
    }
    break;

  case CPM_SIGNATURE_32 ('S', 'I', 'E', '1'):                       // ASL code: \_GPE.SIE1
    if ((*(Buffer + 22) != CPM_SIGNATURE_32 ('S', 'I', 'E', '1')) && (*(Buffer + 22) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 22);
    }
    break;
  case CPM_SIGNATURE_32 ('B', 'I', 'E', '1'):                       // ASL code: \_GPE.BIE1
    if ((*(Buffer + 23) != CPM_SIGNATURE_32 ('B', 'I', 'E', '1')) && (*(Buffer + 23) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 23);
    }
    break;
  case CPM_SIGNATURE_32 ('A', 'I', 'E', '1'):                       // ASL code: \_GPE.AIE1
    if ((*(Buffer + 24) != CPM_SIGNATURE_32 ('A', 'I', 'E', '1')) && (*(Buffer + 24) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 24);
    }
    break;
  case CPM_SIGNATURE_32 ('P', 'T', 'S', '1'):                       // ASL code: \_GPE.PTS1
    if ((*(Buffer + 25) != CPM_SIGNATURE_32 ('P', 'T', 'S', '1')) && (*(Buffer + 25) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 25);
    }
    break;
  case CPM_SIGNATURE_32 ('W', 'A', 'K', '1'):                       // ASL code: \_GPE.WAK1
    if ((*(Buffer + 26) != CPM_SIGNATURE_32 ('W', 'A', 'K', '1')) && (*(Buffer + 26) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 26);
    }
    break;
  case CPM_SIGNATURE_32 ('S', 'P', '2', '0'):                       // ASL code: \_GPE.SP20
  case CPM_SIGNATURE_32 ('S', 'P', '2', '1'):                       // ASL code: \_GPE.SP21
  case CPM_SIGNATURE_32 ('S', 'P', '2', '2'):                       // ASL code: \_GPE.SP22
  case CPM_SIGNATURE_32 ('S', 'P', '2', '3'):                       // ASL code: \_GPE.SP23
  case CPM_SIGNATURE_32 ('S', 'P', '2', '4'):                       // ASL code: \_GPE.SP24
  case CPM_SIGNATURE_32 ('S', 'P', '2', '5'):                       // ASL code: \_GPE.SP25
  case CPM_SIGNATURE_32 ('S', 'P', '2', '6'):                       // ASL code: \_GPE.SP26
  case CPM_SIGNATURE_32 ('S', 'P', '2', '7'):                       // ASL code: \_GPE.SP27
    if (((*(Buffer + 27) & 0xFFFFFF) != (CPM_SIGNATURE_32 ('S', 'P', '2', '0') & 0xFFFFFF)) && (*(Buffer + 27) != 0x0)) {
      *((UINT32*)AmlObjPtr) &= 0xFF000000;
      *((UINT32*)AmlObjPtr) |= *(Buffer + 27) & 0xFFFFFF;
    }
    break;
  case CPM_SIGNATURE_32 ('S', 'P', '3', '0'):                       // ASL code: \_GPE.SP30
  case CPM_SIGNATURE_32 ('S', 'P', '3', '1'):                       // ASL code: \_GPE.SP31
  case CPM_SIGNATURE_32 ('S', 'P', '3', '2'):                       // ASL code: \_GPE.SP32
  case CPM_SIGNATURE_32 ('S', 'P', '3', '3'):                       // ASL code: \_GPE.SP33
  case CPM_SIGNATURE_32 ('S', 'P', '3', '4'):                       // ASL code: \_GPE.SP34
  case CPM_SIGNATURE_32 ('S', 'P', '3', '5'):                       // ASL code: \_GPE.SP35
  case CPM_SIGNATURE_32 ('S', 'P', '3', '6'):                       // ASL code: \_GPE.SP36
  case CPM_SIGNATURE_32 ('S', 'P', '3', '7'):                       // ASL code: \_GPE.SP37
    if (((*(Buffer + 28) & 0xFFFFFF) != (CPM_SIGNATURE_32 ('S', 'P', '3', '0') & 0xFFFFFF)) && (*(Buffer + 28) != 0x0)) {
      *((UINT32*)AmlObjPtr) &= 0xFF000000;
      *((UINT32*)AmlObjPtr) |= *(Buffer + 28) & 0xFFFFFF;
    }
    break;

  case CPM_SIGNATURE_32 ('S', 'I', 'E', '2'):                       // ASL code: \_GPE.SIE2
    if ((*(Buffer + 29) != CPM_SIGNATURE_32 ('S', 'I', 'E', '2')) && (*(Buffer + 29) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 29);
    }
    break;
  case CPM_SIGNATURE_32 ('B', 'I', 'E', '2'):                       // ASL code: \_GPE.BIE2
    if ((*(Buffer + 30) != CPM_SIGNATURE_32 ('B', 'I', 'E', '2')) && (*(Buffer + 30) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 30);
    }
    break;
  case CPM_SIGNATURE_32 ('A', 'I', 'E', '2'):                       // ASL code: \_GPE.AIE2
    if ((*(Buffer + 31) != CPM_SIGNATURE_32 ('A', 'I', 'E', '2')) && (*(Buffer + 31) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 31);
    }
    break;
  case CPM_SIGNATURE_32 ('P', 'T', 'S', '2'):                       // ASL code: \_GPE.PTS2
    if ((*(Buffer + 32) != CPM_SIGNATURE_32 ('P', 'T', 'S', '2')) && (*(Buffer + 32) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 32);
    }
    break;
  case CPM_SIGNATURE_32 ('W', 'A', 'K', '2'):                       // ASL code: \_GPE.WAK2
    if ((*(Buffer + 33) != CPM_SIGNATURE_32 ('W', 'A', 'K', '2')) && (*(Buffer + 33) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 33);
    }
    break;
  case CPM_SIGNATURE_32 ('S', 'P', '4', '0'):                       // ASL code: \_GPE.SP40
  case CPM_SIGNATURE_32 ('S', 'P', '4', '1'):                       // ASL code: \_GPE.SP41
  case CPM_SIGNATURE_32 ('S', 'P', '4', '2'):                       // ASL code: \_GPE.SP42
  case CPM_SIGNATURE_32 ('S', 'P', '4', '3'):                       // ASL code: \_GPE.SP43
  case CPM_SIGNATURE_32 ('S', 'P', '4', '4'):                       // ASL code: \_GPE.SP44
  case CPM_SIGNATURE_32 ('S', 'P', '4', '5'):                       // ASL code: \_GPE.SP45
  case CPM_SIGNATURE_32 ('S', 'P', '4', '6'):                       // ASL code: \_GPE.SP46
  case CPM_SIGNATURE_32 ('S', 'P', '4', '7'):                       // ASL code: \_GPE.SP47
    if (((*(Buffer + 34) & 0xFFFFFF) != (CPM_SIGNATURE_32 ('S', 'P', '4', '0') & 0xFFFFFF)) && (*(Buffer + 34) != 0x0)) {
      *((UINT32*)AmlObjPtr) &= 0xFF000000;
      *((UINT32*)AmlObjPtr) |= *(Buffer + 34) & 0xFFFFFF;
    }
    break;
  case CPM_SIGNATURE_32 ('S', 'P', '5', '0'):                       // ASL code: \_GPE.SP50
  case CPM_SIGNATURE_32 ('S', 'P', '5', '1'):                       // ASL code: \_GPE.SP51
  case CPM_SIGNATURE_32 ('S', 'P', '5', '2'):                       // ASL code: \_GPE.SP52
  case CPM_SIGNATURE_32 ('S', 'P', '5', '3'):                       // ASL code: \_GPE.SP53
  case CPM_SIGNATURE_32 ('S', 'P', '5', '4'):                       // ASL code: \_GPE.SP54
  case CPM_SIGNATURE_32 ('S', 'P', '5', '5'):                       // ASL code: \_GPE.SP55
  case CPM_SIGNATURE_32 ('S', 'P', '5', '6'):                       // ASL code: \_GPE.SP56
  case CPM_SIGNATURE_32 ('S', 'P', '5', '7'):                       // ASL code: \_GPE.SP57
    if (((*(Buffer + 35) & 0xFFFFFF) != (CPM_SIGNATURE_32 ('S', 'P', '5', '0') & 0xFFFFFF)) && (*(Buffer + 35) != 0x0)) {
      *((UINT32*)AmlObjPtr) &= 0xFF000000;
      *((UINT32*)AmlObjPtr) |= *(Buffer + 35) & 0xFFFFFF;
    }
    break;

  case CPM_SIGNATURE_32 ('S', 'I', 'E', '3'):                       // ASL code: \_GPE.SIE3
    if ((*(Buffer + 36) != CPM_SIGNATURE_32 ('S', 'I', 'E', '3')) && (*(Buffer + 36) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 36);
    }
    break;
  case CPM_SIGNATURE_32 ('B', 'I', 'E', '3'):                       // ASL code: \_GPE.BIE3
    if ((*(Buffer + 37) != CPM_SIGNATURE_32 ('B', 'I', 'E', '3')) && (*(Buffer + 37) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 37);
    }
    break;
  case CPM_SIGNATURE_32 ('A', 'I', 'E', '3'):                       // ASL code: \_GPE.AIE3
    if ((*(Buffer + 38) != CPM_SIGNATURE_32 ('A', 'I', 'E', '3')) && (*(Buffer + 38) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 38);
    }
    break;
  case CPM_SIGNATURE_32 ('P', 'T', 'S', '3'):                       // ASL code: \_GPE.PTS3
    if ((*(Buffer + 39) != CPM_SIGNATURE_32 ('P', 'T', 'S', '3')) && (*(Buffer + 39) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 39);
    }
    break;
  case CPM_SIGNATURE_32 ('W', 'A', 'K', '3'):                       // ASL code: \_GPE.WAK3
    if ((*(Buffer + 40) != CPM_SIGNATURE_32 ('W', 'A', 'K', '3')) && (*(Buffer + 40) != 0x0)) {
      *((UINT32*)AmlObjPtr) = *(Buffer + 40);
    }
    break;
  case CPM_SIGNATURE_32 ('S', 'P', '6', '0'):                       // ASL code: \_GPE.SP60
  case CPM_SIGNATURE_32 ('S', 'P', '6', '1'):                       // ASL code: \_GPE.SP61
  case CPM_SIGNATURE_32 ('S', 'P', '6', '2'):                       // ASL code: \_GPE.SP62
  case CPM_SIGNATURE_32 ('S', 'P', '6', '3'):                       // ASL code: \_GPE.SP63
  case CPM_SIGNATURE_32 ('S', 'P', '6', '4'):                       // ASL code: \_GPE.SP64
  case CPM_SIGNATURE_32 ('S', 'P', '6', '5'):                       // ASL code: \_GPE.SP65
  case CPM_SIGNATURE_32 ('S', 'P', '6', '6'):                       // ASL code: \_GPE.SP66
  case CPM_SIGNATURE_32 ('S', 'P', '6', '7'):                       // ASL code: \_GPE.SP67
    if (((*(Buffer + 41) & 0xFFFFFF) != (CPM_SIGNATURE_32 ('S', 'P', '6', '0') & 0xFFFFFF)) && (*(Buffer + 41) != 0x0)) {
      *((UINT32*)AmlObjPtr) &= 0xFF000000;
      *((UINT32*)AmlObjPtr) |= *(Buffer + 41) & 0xFFFFFF;
    }
    break;
  case CPM_SIGNATURE_32 ('S', 'P', '7', '0'):                       // ASL code: \_GPE.SP70
  case CPM_SIGNATURE_32 ('S', 'P', '7', '1'):                       // ASL code: \_GPE.SP71
  case CPM_SIGNATURE_32 ('S', 'P', '7', '2'):                       // ASL code: \_GPE.SP72
  case CPM_SIGNATURE_32 ('S', 'P', '7', '3'):                       // ASL code: \_GPE.SP73
  case CPM_SIGNATURE_32 ('S', 'P', '7', '4'):                       // ASL code: \_GPE.SP74
  case CPM_SIGNATURE_32 ('S', 'P', '7', '5'):                       // ASL code: \_GPE.SP75
  case CPM_SIGNATURE_32 ('S', 'P', '7', '6'):                       // ASL code: \_GPE.SP76
  case CPM_SIGNATURE_32 ('S', 'P', '7', '7'):                       // ASL code: \_GPE.SP77
    if (((*(Buffer + 42) & 0xFFFFFF) != (CPM_SIGNATURE_32 ('S', 'P', '7', '0') & 0xFFFFFF)) && (*(Buffer + 42) != 0x0)) {
      *((UINT32*)AmlObjPtr) &= 0xFF000000;
      *((UINT32*)AmlObjPtr) |= *(Buffer + 42) & 0xFFFFFF;
    }
    break;
  case CPM_SIGNATURE_32 ('X', 'I', 'N', 'I'):                       // ASL code: XINI
    if (*(Buffer + 43) == 0x1) {
      *(UINT8*)AmlObjPtr = '_';
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load Common SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmInitLate (
  IN       EFI_EVENT               Event,
  IN       VOID                    *Context
  )
{
  AMD_CPM_NV_DATA_PROTOCOL        *NvDataProtocolPtr;
  EFI_STATUS                      Status;
  UINT32                          Buffer[44];
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  AMD_CPM_MAIN_TABLE              *CpmMainTablePtr;
  AMD_CPM_ZERO_POWER_ODD_TABLE    *CpmZeroPowerOddTablePtr;
  AMD_CPM_IO_EXPANDER_ACPI_EVENT_TABLE  *CpmIoExpanderAcpiEventTablePtr;
  UINT8                           Index;
  UINT32                          EventBit = 1;
  UINT8                           VerbosityCfg;

  DEBUG((DEBUG_INFO, "KER-DXE-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, "-From-AmdCpmInitDxeEntryPoint-CreateEventEx-gEfiEventReadyToBootGuid\n"));
  RECORD_TIME_DXE (BeginAmdCpmLoadCommonSsdt);

  if (!InitlateInvoked) {

    DEBUG((DEBUG_INFO, "KER-DXE-%a-if (!InitlateInvoked)\n", __FUNCTION__));
    NvDataProtocolPtr = (AMD_CPM_NV_DATA_PROTOCOL *)Context;
    CpmMainTablePtr         = AmdCpmTableProtocolPtr->MainTablePtr;
    CpmZeroPowerOddTablePtr = (AMD_CPM_ZERO_POWER_ODD_TABLE *)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_ZERO_POWER_ODD);
    CpmIoExpanderAcpiEventTablePtr = (AMD_CPM_IO_EXPANDER_ACPI_EVENT_TABLE *)AmdCpmTableProtocolPtr->CommonFunction.GetTablePtr (AmdCpmTableProtocolPtr, CPM_SIGNATURE_SMBUS_IO_EXPANDER);

    ZeroMem (Buffer, sizeof (Buffer));
    Buffer[0] = (UINT32) AmdCpmTableProtocolPtr->CommonFunction.GetBxbBusNumber (AmdCpmTableProtocolPtr);
    Buffer[0] = (UINT32) (UINT64) (NvDataProtocolPtr->NvDataPtr);
    Buffer[1] = (UINT32) (sizeof (AMD_CPM_NV_DATA_STRUCT));
    Buffer[2] = CFG_CPM_ASL_OEM_CALLBACK;
    Buffer[3] = CFG_CPM_ASL_OEM_MLIB;
    if ((CpmZeroPowerOddTablePtr) && (CpmMainTablePtr->ZeroPowerOddEn & BIT0)) {
      if (CpmZeroPowerOddTablePtr->PciBusAslName == 0) {
        Buffer[4]  = 0x30494350;    // PCI0 (50 43 49 30)
      } else {
        Buffer[4]  = CpmZeroPowerOddTablePtr->PciBusAslName;
      }
      if (CpmZeroPowerOddTablePtr->BridgeAslName == 0) {
        Buffer[10]  = 0x38315047;   // GP18 (47 50 31 38)
      } else {
        Buffer[10]  = CpmZeroPowerOddTablePtr->BridgeAslName;
      }
      if (CpmZeroPowerOddTablePtr->DeviceAslName == 0) {
        Buffer[11]  = 0x41544153;   // SATA (53 41 54 41)
      } else {
        Buffer[11]  = CpmZeroPowerOddTablePtr->DeviceAslName;
      }
    }
    if (PcdGetBool (PcdAgesaTestPointEnable)) {
      Buffer[5] = 1;
      Buffer[6] = (UINT32)PcdGet8 (PcdAgesaTestPointWidth);
      Buffer[7] = (UINT32)PcdGet16 (PcdIdsDebugPort);
    }
    if (PcdGetBool(PcdPtZpoddEnabled)) {
      Buffer[8] = PcdGet32 (PcdPtZpoddPtBridgeName);
      Buffer[9] = PcdGet32 (PcdPtZpoddPtSataName);
    }
    else {
      Buffer[8] = 0xFFFFFFFF;
      Buffer[9] = 0xFFFFFFFF;
    }

    Buffer[12] = 0xFF;
    DEBUG((DEBUG_INFO, "KER-DXE-%a-MainTablePtr->ProcessorAggregatorDevice = 0x%04X\n", __FUNCTION__, AmdCpmTableProtocolPtr->MainTablePtr->ProcessorAggregatorDevice));
    if (PcdGet8 (PcdSwSciGpeID) != 0xFF ) {
      Buffer[12] = AmdCpmTableProtocolPtr->CommonFunction.EventAslNameConverter (AmdCpmTableProtocolPtr->MainTablePtr->ProcessorAggregatorDevice & 0x1F);
      EventBit <<= (AmdCpmTableProtocolPtr->MainTablePtr->ProcessorAggregatorDevice & 0x1F);
      DEBUG((DEBUG_INFO, "KER-DXE-%a-SwSciGpeId = %d, ", __FUNCTION__, AmdCpmTableProtocolPtr->MainTablePtr->ProcessorAggregatorDevice & 0x1F));
      DEBUG((DEBUG_INFO, "EventBit = 0x%08X, ", EventBit));
      DEBUG((DEBUG_INFO, "Set Active high, Edge trigger and Clear status\n"));
      AmdCpmTableProtocolPtr->CommonFunction.MmioOr32 (AmdCpmTableProtocolPtr->MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + FCH_SMI_REG08, EventBit);     // 1: Active high
      AmdCpmTableProtocolPtr->CommonFunction.MmioAnd32 (AmdCpmTableProtocolPtr->MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + FCH_SMI_REG0C, ~EventBit);   // 0: Edge trigger
      AmdCpmTableProtocolPtr->CommonFunction.MmioWrite32 (AmdCpmTableProtocolPtr->MainTablePtr->AcpiMemIoBaseAddr + SMI_BASE + FCH_SMI_REG00, EventBit);  // 1: Write 1 to clear status
    }

    if (PcdGetBool (PcdAgesaTestPointToStb) && PcdGetBool (PcdStbEnable)) {
      Status = GetBiosVerbosity (&VerbosityCfg);
      if (Status == EFI_SUCCESS) {
        if (2 > VerbosityCfg) {     // 2 = PROD_LEVEL_VERBOSITY
          Buffer[13] = 1;
          Buffer[14] = PcdGet32 (PcdStbSmnAddress);
        }
      }
    }

    if (CpmIoExpanderAcpiEventTablePtr) {
      DEBUG((DEBUG_INFO, "KER-DXE-%a-if (CpmIoExpanderAcpiEventTablePtr)\n", __FUNCTION__));
      for (Index = 0; Index < EXPANDER_NUMBER; Index++) {
        if (CpmIoExpanderAcpiEventTablePtr->Expander[Index].EntryNumber == CPM_END_OF_TABLE) {
          break;
        }
        // From Buffer[15] to Buffer[42]
        Buffer[15 + (Index * 7)] = CpmIoExpanderAcpiEventTablePtr->Expander[Index].EventAslName;               ///< The Event Asl Name.          Method (\_GPE.SIEx)
        Buffer[16 + (Index * 7)] = CpmIoExpanderAcpiEventTablePtr->Expander[Index].EntryCalloutAslName;        ///< The Entry Callout Asl Name.  Method (\_GPE.BIEx)
        Buffer[17 + (Index * 7)] = CpmIoExpanderAcpiEventTablePtr->Expander[Index].ExitCalloutAslName;         ///< The Exit Callout Asl Name.   Method (\_GPE.AIEx)
        Buffer[18 + (Index * 7)] = CpmIoExpanderAcpiEventTablePtr->Expander[Index].PTSCalloutAslName;          ///< The _PTS Callout Asl Name.   Method (\_GPE.PTSx)
        Buffer[19 + (Index * 7)] = CpmIoExpanderAcpiEventTablePtr->Expander[Index].WAKCalloutAslName;          ///< The _WAK Callout Asl Name.   Method (\_GPE.WAKx)
        Buffer[20 + (Index * 7)] = CpmIoExpanderAcpiEventTablePtr->Expander[Index].PrefixGpioAslName;          ///< The Prefix of Gpio Input Pin Asl Name. Method (\_GPE.SP00)
        Buffer[21 + (Index * 7)] = CpmIoExpanderAcpiEventTablePtr->Expander[Index].Port1PrefixGpioAslName;     ///< The Prefix of Gpio Input Pin Asl Name for 16-bit IO Expander Port 1. Method (\_GPE.SP10)
      }
      Buffer[43] = 0x1;
    }

    DEBUG((DEBUG_INFO, "KER-DXE-%a-AmdCpmAddSsdtTable-AmdCpmCommonSsdtCallBack\n", __FUNCTION__));
    Status = AmdCpmAddSsdtTable (  (VOID*)AmdCpmTableProtocolPtr,
                                &COMMON_ACPI_DEVICE_GUID,
                                NULL,
                                AmdCpmCommonSsdtCallBack,
                                &Buffer[0]
                                );
  }

  InitlateInvoked = TRUE;

  RECORD_TIME_DXE (EndAmdCpmLoadCommonSsdt);

  DEBUG((DEBUG_INFO, "KER-DXE-%a-End\n", __FUNCTION__));
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Kernal Common function to load SSDT table.
 *
 * This function searchs ACPI storage file and finds the matched SSDT table. The table will
 * be updated and registered in ACPI area.
 *
 * @param[in]     This           Point to Protocol
 * @param[in]     EfiGuid        The GUID of ACPI storage file of SSDT table
 * @param[in]     OemTableId     OEM Table Id of SSDT table
 * @param[in]     Function       Callback Function
 * @param[in]     Context        The Parameters for callback function
 *
 * @retval        EFI_SUCCESS    SSDT table initialized successfully
 * @retval        EFI_ERROR      Initialization failed
 */
EFI_STATUS
EFIAPI
AmdCpmAddSsdtTable (
  IN       VOID                                *This,
  IN       VOID                                *EfiGuid,
  IN       UINT64                              *OemTableId,
  IN       AMD_CPM_ADDSSDTCALLBACK_FN          Function,
  IN       VOID                                *Context
  )
{
  CPM_ACPI_SUPPORT_PROTOCOL           *AcpiSupport;
  UINT8                               *AmlObjPtr;
  UINTN                               TableList;
  EFI_STATUS                          Status;
  EFI_GUID                            *EfiGuidPtr;
  EFI_HANDLE                          *HandleBuffer;
  UINTN                               NumberOfHandles;
  UINTN                               Index;
  CPM_FIRMWARE_VOLUME_PROTOCOL        *FwVol;
  UINTN                               Size;
  UINT32                              FvStatus;
  EFI_FV_FILETYPE                     FileType;
  EFI_FV_FILE_ATTRIBUTES              Attributes;
  INTN                                Instance;
  EFI_ACPI_COMMON_HEADER              *CurrentTable;
  UINT64                              TableId;
  UINT8                               IsAmdCpmSsdtSupport;

  DEBUG((DEBUG_INFO, "KER-DXE-%a-Start\n", __FUNCTION__));
  CurrentTable  = NULL;
  TableList     = 0;
  EfiGuidPtr    = (EFI_GUID*) EfiGuid;
  FwVol         = NULL;
  Size          = 0;
  IsAmdCpmSsdtSupport = AmdCpmSsdtSupport;

  switch (((AMD_CPM_TABLE_PROTOCOL*)This)->CommonFunction.GetCpuRevisionId (This)) {
  case CPM_CPU_REVISION_ID_AM:
    IsAmdCpmSsdtSupport = 0;
    break;
  }

  if (!IsAmdCpmSsdtSupport) {
    DEBUG ((DEBUG_INFO, "KER-DXE-%a(%d)-End-Status = EFI_UNSUPPORTED\n", __FUNCTION__, __LINE__));
    return EFI_UNSUPPORTED;
  }

  //
  // Locate protocol.
  // There is little chance we can't find an FV protocol
  //
  DEBUG((DEBUG_INFO, "KER-DXE-%a-LocateHandleBuffer-gCpmFirmwareVolumeProtocolGuid\n", __FUNCTION__));
  Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gCpmFirmwareVolumeProtocolGuid,
                    NULL,
                    &NumberOfHandles,
                    &HandleBuffer
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "KER-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  //
  // Looking for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    DEBUG((DEBUG_INFO, "KER-DXE-%a-HandleProtocol-HandleBuffer[Index=0x%X]\n", __FUNCTION__, Index));
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gCpmFirmwareVolumeProtocolGuid,
                    (VOID**)&FwVol
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "KER-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return Status;
    }

    //
    // See if it has the ACPI storage file
    //
    Size     = 0;
    FvStatus = 0;
    DEBUG((DEBUG_INFO, "KER-DXE-%a-HandleProtocol-HandleBuffer[Index=0x%X]-FwVol->ReadFile\n", __FUNCTION__, Index));
    Status = FwVol->ReadFile (
                    FwVol,
                    EfiGuidPtr,
                    NULL,
                    &Size,
                    &FileType,
                    &Attributes,
                    &FvStatus
                    );

    //
    // If we found it, then we are done
    //
    if (Status == EFI_SUCCESS) {
      DEBUG((DEBUG_INFO, "KER-DXE-%a-HandleProtocol-HandleBuffer[Index=0x%X]-if (Status == EFI_SUCCESS)\n", __FUNCTION__, Index));
      break;
    }
  }

  //
  // Our exit status is determined by the success of the previous operations
  // If the protocol was found, Instance already points to it.
  //
  // Free any allocated buffers
  //
  DEBUG((DEBUG_INFO, "KER-DXE-%a-gBS->FreePool (HandleBuffer)\n", __FUNCTION__));
  gBS->FreePool (HandleBuffer);

  //
  // Sanity check that we found our data file
  //
  DEBUG((DEBUG_INFO, "KER-DXE-%a-ASSERT (FwVol)\n", __FUNCTION__));
  ASSERT (FwVol);

  //
  // Read tables from the storage file.
  //

  Instance     = 0;
  CurrentTable = NULL;

  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                    FwVol,
                    EfiGuidPtr,
                    EFI_SECTION_RAW,
                    Instance,
                    (VOID**)&CurrentTable,
                    &Size,
                    &FvStatus
                    );

    if (!EFI_ERROR (Status)) {
      TableId = ((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId;
      if (*((UINT32 *) CurrentTable) == 'TDSS' && (OemTableId ? (TableId == *OemTableId) : TRUE)) {
        if (Function) {
          if (Size >= sizeof (UINT32)) {
            for (AmlObjPtr = (UINT8*) CurrentTable + sizeof (EFI_ACPI_COMMON_HEADER); AmlObjPtr <= ((UINT8*) CurrentTable) + Size - sizeof (UINT32); AmlObjPtr ++) {
              if ((*Function) (This, AmlObjPtr, Context)) {
                break;
              }
            }
          }
        }
        break;
      }

      //
      // Increment the instance
      //
      Instance++;
      CurrentTable = NULL;
    }
  }

  DEBUG((DEBUG_INFO, "KER-DXE-%a-LocateProtocol-gCpmAcpiSupportGuid\n", __FUNCTION__));
  Status = gBS->LocateProtocol (
                    &gCpmAcpiSupportGuid,
                    NULL,
                    (VOID**)&AcpiSupport
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "KER-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  TableList = 0;

  //
  // Update OemId & OemTableId fields.
  //
  if (CurrentTable) {
    if (((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId != PcdGet64 (PcdCpmIgnoreOemTableId)) {
      DEBUG((DEBUG_INFO, "KER-DXE-AmdCpmAddSsdtTable-Overwrite OemTableId\n", ((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId));
      ((AMD_CPM_TABLE_PROTOCOL*)This)->CommonFunction.CopyMem (
        (VOID *) &((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemId,
        (VOID *) PcdGetPtr (PcdAmdAcpiTableHeaderOemId),
        AsciiStrnLenS (PcdGetPtr (PcdAmdAcpiTableHeaderOemId), 6)
        );

      ((AMD_CPM_TABLE_PROTOCOL*)This)->CommonFunction.CopyMem (
        (VOID *) &((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId,
        (VOID *) PcdGetPtr (PcdAmdAcpiTableHeaderOemTableId),
        AsciiStrnLenS (PcdGetPtr (PcdAmdAcpiTableHeaderOemTableId), 8)
        );
    } else {
      DEBUG((DEBUG_INFO, "KER-DXE-AmdCpmAddSsdtTable-Don't overwrite OemTableId 0x%LX\n", ((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId));
    }

    DEBUG((DEBUG_INFO, "KER-DXE-%a-CpmSetAcpiTable\n", __FUNCTION__));
    CpmSetAcpiTable (AcpiSupport, CurrentTable, TRUE, AMD_CPM_ACPI_VERSION_SUPPORT, Size, TableList, Status);

    DEBUG((DEBUG_INFO, "KER-DXE-%a-gBS->FreePool (CurrentTable)\n", __FUNCTION__));
    Status = gBS->FreePool (CurrentTable);
  }

  DEBUG ((DEBUG_INFO, "KER-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
  return  Status;
}

AGESA_STATUS
AmdCpmGetPostedVbiosImageNull (
  IN      GFX_VBIOS_IMAGE_INFO         *VbiosImageInfo
  )
{
  DEBUG((DEBUG_INFO, "KER-DXE-%a-return-AGESA_UNSUPPORTED\n", __FUNCTION__));
  return AGESA_UNSUPPORTED;
}


/*---------------------------------------------------------------------------------------*/
/**
 * Register Common Kenerl functions at the AmdCpmInitDxe entry point.
 *
 * This function registers CPM common kernel functions in AmdCpmTableProtocol
 * at AmdCpmInitDxe.
 *
 *  @param[in]   This                Pointer to Protocol.
 *
 */
VOID
EFIAPI
AmdCpmDxeRegisterKernel (
  IN       VOID                        *This
  )
{
  AMD_CPM_TABLE_PROTOCOL      *TableProtocolPtr = This;

  DEBUG((DEBUG_INFO, "KER-DXE-%a-Start\n", __FUNCTION__));
  TableProtocolPtr->CommonFunction.GetTablePtr            = CpmGetTablePtr;
  TableProtocolPtr->CommonFunction.GetTablePtr2           = CpmGetTablePtr2;
  TableProtocolPtr->CommonFunction.AddTable               = CpmAddTable;
  TableProtocolPtr->CommonFunction.RemoveTable            = CpmRemoveTable;
  TableProtocolPtr->CommonFunction.AddSsdtTable           = AmdCpmAddSsdtTable;
  TableProtocolPtr->CommonFunction.IsAmlOpRegionObject    = IsAmlOpRegionObject;
  TableProtocolPtr->CommonFunction.RelocateTable          = CpmRelocateTableList;
  TableProtocolPtr->CommonFunction.CopyMem                = CpmCopyMem;
  TableProtocolPtr->DxePublicFunction.GetPostedVbiosImage = AmdCpmGetPostedVbiosImageNull;

  DEBUG((DEBUG_INFO, "KER-DXE-%a-End\n", __FUNCTION__));
  return;
}

VOID
EFIAPI
AmdCpmPciIoProtEarly (
  IN       EFI_EVENT               Event,
  IN       VOID                    *Context
  )
{
  EFI_STATUS                       Status;
  RAS_PCI_IO_PROT_EARLY_PROTOCOL   *RasPciIoProtEarlyProtocol;

  Status = gBS->LocateProtocol (
                    &gRasPciIoProtEarlyProtocolGuid,
                    NULL,
                    (VOID**)&RasPciIoProtEarlyProtocol
                    );
  DEBUG ((DEBUG_INFO, "KER-DXE-%a Locate gRasPciIoProtEarlyProtocolGuid: %r\n", __FUNCTION__, Status));
  if (!EFI_ERROR (Status)) {
    RasPciIoProtEarlyProtocol->BldPcieMapAndBootErrDet();
  }

  gBS->CloseEvent (Event);

  return;
}
