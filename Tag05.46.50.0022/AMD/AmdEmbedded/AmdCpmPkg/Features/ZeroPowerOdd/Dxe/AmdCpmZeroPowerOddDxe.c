/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>

EFI_GUID ZERO_POWER_ODD_FM15_ACPI_DEVICE_GUID = {
  0xb7951665, 0x0280, 0x487a, 0x83, 0xb6, 0x36, 0x27, 0x1d, 0x4c, 0x3a, 0x90
};

EFI_GUID ZERO_POWER_ODD_FM17_ACPI_DEVICE_GUID = {
  0xBB5576D8, 0xA414, 0x4A8A, 0xA0, 0xDC, 0x07, 0x09, 0x36, 0xFE, 0x2C, 0x02
};

VOID
EFIAPI
AmdCpmZeroPowerOddInitLate (
  IN      EFI_EVENT       Event,
  IN      VOID            *Context
  );

BOOLEAN
EFIAPI
AmdCpmZeroPowerOddCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Zero Power Oddd DXE driver
 *
 * This function stores the setting of Zero Power Odd in NV Data and loads, updates
 * and installs Zero Power Odd SSDT Table.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmZeroPowerOddDxeEntryPoint (
  IN      EFI_HANDLE              ImageHandle,
  IN      EFI_SYSTEM_TABLE        *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       ReadyToBootEvent;

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  Status = gBS->CreateEventEx (
                  CPM_EVENT_NOTIFY_SIGNAL,
                  CPM_TPL_CALLBACK,
                  AmdCpmZeroPowerOddInitLate,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * The function to load AMD CPM Zero Power Odd SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmZeroPowerOddInitLate (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  AMD_CPM_ZERO_POWER_ODD_TABLE    *CpmZeroPowerOddTablePtr;
  AMD_CPM_MAIN_TABLE              *CpmMainTablePtr;
  EFI_STATUS                      Status;
  UINT32                          Buffer[16];
  UINT8                           CpuRevisionId;
  UINT8                           BridgeBus;
  UINT8                           BridgeDev;
  UINT8                           BridgeFun;
  UINT8                           SataDev;
  UINT8                           SataFun;

  RECORD_TIME_DXE (BeginAmdCpmZeroPowerOddDxeDriver);

  if (!InitlateInvoked) {

    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
    if (EFI_ERROR (Status)) {
      return;
    }

    CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpZeroPowerOddDxeDriverBegin);

    CpmMainTablePtr         = CpmTableProtocolPtr->MainTablePtr;
    CpmZeroPowerOddTablePtr = (AMD_CPM_ZERO_POWER_ODD_TABLE *)CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_ZERO_POWER_ODD);
    if (CpmZeroPowerOddTablePtr) {
      if (CpmMainTablePtr->ZeroPowerOddEn & BIT0) {
        ZeroMem (Buffer, sizeof (Buffer));
        BridgeBus = CpmZeroPowerOddTablePtr->Bus;
        BridgeDev = CpmZeroPowerOddTablePtr->Bridge.Device;
        BridgeFun = CpmZeroPowerOddTablePtr->Bridge.Function;
        SataDev   = CpmZeroPowerOddTablePtr->Device.Device;
        SataFun   = CpmZeroPowerOddTablePtr->Device.Function;
        if (!(CpmZeroPowerOddTablePtr->SataModeSupportMask & CpmTableProtocolPtr->CommonFunction.GetSataMode (CpmTableProtocolPtr, BridgeBus, BridgeDev, BridgeFun, SataDev, SataFun))) {
          CpmMainTablePtr->ZeroPowerOddEn &= 0xF6;
        }
        if (CpmZeroPowerOddTablePtr->EventSource1 == 0) {
          Buffer[0] = CpmTableProtocolPtr->CommonFunction.EventAslNameConverter (CpmTableProtocolPtr->CommonFunction.GetSciMap (CpmTableProtocolPtr, CpmZeroPowerOddTablePtr->EventPin1));
        }
        if ((CpmMainTablePtr->ZeroPowerOddEn & BIT1) && (CpmZeroPowerOddTablePtr->EventSource2 == 0)) {
          Buffer[1] = CpmTableProtocolPtr->CommonFunction.EventAslNameConverter (CpmTableProtocolPtr->CommonFunction.GetSciMap (CpmTableProtocolPtr, CpmZeroPowerOddTablePtr->EventPin2));
        }
        Buffer[2] = (UINT32) CpmZeroPowerOddTablePtr->SataPortId;
        if (CpmZeroPowerOddTablePtr->EventSource1 == 0) {
          Buffer[3] = (UINT32) CpmTableProtocolPtr->CommonFunction.GetSciMap (CpmTableProtocolPtr, CpmZeroPowerOddTablePtr->EventPin1);
        } else {
          Buffer[3] = (UINT32) CpmTableProtocolPtr->CommonFunction.GetSciMap (CpmTableProtocolPtr, CpmZeroPowerOddTablePtr->EventPin3);
        }
        if (CpmMainTablePtr->ZeroPowerOddEn & BIT3) {
          Buffer[4] = 1;
        }
        if (CpmZeroPowerOddTablePtr->EventSource1 != 0) {
          Buffer[5] = CpmTableProtocolPtr->CommonFunction.EventAslNameConverter (CpmZeroPowerOddTablePtr->QEventFalling1);
          Buffer[6] = CpmTableProtocolPtr->CommonFunction.EventAslNameConverter (CpmZeroPowerOddTablePtr->QEventRising1);
        }
        if ((CpmMainTablePtr->ZeroPowerOddEn & BIT1) && (CpmZeroPowerOddTablePtr->EventSource2 != 0)) {
          Buffer[7] = CpmTableProtocolPtr->CommonFunction.EventAslNameConverter (CpmZeroPowerOddTablePtr->QEventFalling2);
          Buffer[8] = CpmTableProtocolPtr->CommonFunction.EventAslNameConverter (CpmZeroPowerOddTablePtr->QEventRising2);
        }
        Buffer[9] = CFG_AMD_CPM_ASL_EC_NAME;      // PcdGet32 (EcAslName)
        Buffer[10] = CFG_AMD_CPM_ASL_EC_MUTEX;    // PcdGet32 (EcMutexAslName)
        Buffer[11] = CFG_AMD_CPM_ASL_LPC_NAME;    // PcdGet32 (LpcAslName)
        Buffer[12] = CFG_AMD_CPM_ASL_OKEC_NAME;   // PcdGet32 (OkEcAslName)
        if (CpmZeroPowerOddTablePtr->PciBusAslName == 0) {
          Buffer[13]  = 0x30494350;    // PCI0 (50 43 49 30)
        } else {
          Buffer[13]  = CpmZeroPowerOddTablePtr->PciBusAslName;
        }
        if (CpmZeroPowerOddTablePtr->BridgeAslName == 0) {
          Buffer[14]  = 0x38315047;    // GP18 (47 50 31 38)
        } else {
          Buffer[14]  = CpmZeroPowerOddTablePtr->BridgeAslName;
        }
        if (CpmZeroPowerOddTablePtr->DeviceAslName == 0) {
          Buffer[15]  = 0x41544153;    // SATA (53 41 54 41)
        } else {
          Buffer[15]  = CpmZeroPowerOddTablePtr->DeviceAslName;
        }
        CpuRevisionId = CpmTableProtocolPtr->CommonFunction.GetCpuRevisionId (CpmTableProtocolPtr);
        if (CpuRevisionId < CPM_CPU_REVISION_ID_ZP) {
          CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID*)CpmTableProtocolPtr,
                                                              &ZERO_POWER_ODD_FM15_ACPI_DEVICE_GUID,
                                                              NULL,
                                                              AmdCpmZeroPowerOddCallBack,
                                                              &Buffer[0]
                                                           );
        } else {
          CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID*)CpmTableProtocolPtr,
                                                              &ZERO_POWER_ODD_FM17_ACPI_DEVICE_GUID,
                                                              NULL,
                                                              AmdCpmZeroPowerOddCallBack,
                                                              &Buffer[0]
                                                           );
          if (CpuRevisionId == CPM_CPU_REVISION_ID_ZP) {
            CpmZeroPowerOddTablePtr->PciBusAslName = 0x10242000;    // CpmZeroPowerOddSataPciCfgSmn = NBIF1EPF[7:1]CFG=1024_[7:1]000h
            CpmZeroPowerOddTablePtr->BridgeAslName = 0x03101000;    // CpmZeroPowerOddSataAhciSmn = SATAAHCIx0000; SATAAHCI=0310_1000h
          } else if ((CpuRevisionId == CPM_CPU_REVISION_ID_RV) || (CpuRevisionId == CPM_CPU_REVISION_ID_RV2) || \
                     (CpuRevisionId == CPM_CPU_REVISION_ID_VN) || (CpuRevisionId == CPM_CPU_REVISION_ID_RMB)) {
            CpmZeroPowerOddTablePtr->PciBusAslName = 0x10148000;    // CpmZeroPowerOddSataPciCfgSmn = NBIF0EPF8CFG=1014_8000h // NBIF0CFGx000[50,48]000; NBIF0CFG=1010_0000h
            CpmZeroPowerOddTablePtr->BridgeAslName = 0x03101000;    // CpmZeroPowerOddSataAhciSmn = SATAAHCIx0000; SATAAHCI=0310_1000h
          } else if ((CpuRevisionId == CPM_CPU_REVISION_ID_SSP) || (CpuRevisionId == CPM_CPU_REVISION_ID_MTS)) {
            if (CpmZeroPowerOddTablePtr->Bridge.Function == 3) {
              CpmZeroPowerOddTablePtr->PciBusAslName = 0x10850000;  // CpmZeroPowerOddSataPciCfgSmn = NBIO3NBIF1EPF0CFG=108[50,48,40]000h
              CpmZeroPowerOddTablePtr->BridgeAslName = 0x03201000;  // CpmZeroPowerOddSataAhciSmn =  SATA[1:0]AHCIx0000000; SATA[1:0]AHCI=03[2:1]0_1000h
            } else {
              CpmZeroPowerOddTablePtr->PciBusAslName = 0x10848000;  // CpmZeroPowerOddSataPciCfgSmn = NBIO3NBIF1EPF0CFG=108[50,48,40]000h
              CpmZeroPowerOddTablePtr->BridgeAslName = 0x03101000;  // CpmZeroPowerOddSataAhciSmn = SATAAHCIx0000; SATAAHCI=0310_1000h
            }
          } else if (CpuRevisionId == CPM_CPU_REVISION_ID_RN) {
            if (CpmZeroPowerOddTablePtr->Device.Function == 1) {
              CpmZeroPowerOddTablePtr->PciBusAslName = 0x10150000;  // CpmZeroPowerOddSataPciCfgSmn = NBIF0CFGx00[50,48]000; NBIF0CFG=1010_0000h
              CpmZeroPowerOddTablePtr->BridgeAslName = 0x03201000;  // CpmZeroPowerOddSataAhciSmn =  SATA[1:0]AHCIx0000000; SATA[1:0]AHCI=03[2:1]0_1000h
            } else {
              CpmZeroPowerOddTablePtr->PciBusAslName = 0x10148000;  // CpmZeroPowerOddSataPciCfgSmn = NBIF0CFGx00[50,48]000; NBIF0CFG=1010_0000h
              CpmZeroPowerOddTablePtr->BridgeAslName = 0x03101000;  // CpmZeroPowerOddSataAhciSmn = SATAAHCIx0000; SATAAHCI=0310_1000h
            }
          }
        }
      } else {
        CpmMainTablePtr->ZeroPowerOddEn &= 0xF6;
      }
    }
    CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpZeroPowerOddDxeDriverEnd);
  }

  InitlateInvoked = TRUE;

  RECORD_TIME_DXE (EndAmdCpmZeroPowerOddDxeDriver);

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to override Zero Power Odd SSDT Table
 *
 * This function is used to update GPE numbers and SATA port number.
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
AmdCpmZeroPowerOddCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT32  *BufferPtr;
  BufferPtr = (UINT32*)Context;

  switch (*((UINT32*)AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('X', 'L', '0', '6')):
    if (*BufferPtr) {
      *(UINT8*)AmlObjPtr = '_';
      *(UINT16 *) ((UINT8*)AmlObjPtr + 2) = *(UINT16*)BufferPtr;
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'L', '1', '0')):
    if (*(BufferPtr + 1)) {
      *(UINT8*)AmlObjPtr = '_';
      *(UINT16 *) ((UINT8*)AmlObjPtr + 2) = *(UINT16*)(BufferPtr + 1);
    }
    break;
  case (CPM_SIGNATURE_32 ('Z', 'P', 'R', 'W')):
    if (*(BufferPtr + 3)) {
      if (*((UINT32*)AmlObjPtr + 1) == 0x0A020612) {
        *((UINT8*)AmlObjPtr + 8) = *(UINT8*)(BufferPtr + 3);
      }
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'P', 'R', 'W')):
    if (*(BufferPtr + 4)) {
      *(UINT8*)AmlObjPtr = '_';
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'E', 'J', '0')):
    if (*(BufferPtr + 1)) {
      *(UINT8*)AmlObjPtr = '_';
    } else if (*(BufferPtr + 8)) {
      *(UINT8*)AmlObjPtr = '_';
    }
    break;
  case (CPM_SIGNATURE_32 ('P', 'R', 'T', '1')):
    *((UINT8 *)AmlObjPtr + 3) = '0' + *(UINT8*)(BufferPtr + 2);
    break;
  case (CPM_SIGNATURE_32 ('X', 'Q', '4', '2')):
    if (*(BufferPtr + 5)) {
      *(UINT8*)AmlObjPtr = '_';
      *(UINT16 *) ((UINT8*)AmlObjPtr + 2) = *(UINT16*)(BufferPtr + 5);
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'Q', '4', '3')):
    if (*(BufferPtr + 6)) {
      *(UINT8*)AmlObjPtr = '_';
      *(UINT16 *) ((UINT8*)AmlObjPtr + 2) = *(UINT16*)(BufferPtr + 6);
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'Q', '4', '0')):
    if (*(BufferPtr + 7)) {
      *(UINT8*)AmlObjPtr = '_';
      *(UINT16 *) ((UINT8*)AmlObjPtr + 2) = *(UINT16*)(BufferPtr + 7);
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'Q', '4', '1')):
    if (*(BufferPtr + 8)) {
      *(UINT8*)AmlObjPtr = '_';
      *(UINT16 *) ((UINT8*)AmlObjPtr + 2) = *(UINT16*)(BufferPtr + 8);
    }
    break;
  case (CPM_SIGNATURE_32 ('E', 'C', '0', '_')):
    if (*(BufferPtr + 9)) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr + 9);
    }
    break;
  case (CPM_SIGNATURE_32 ('M', '0', '4', '2')):     // CpmKbcMutex: M042
    if (*(BufferPtr + 10)) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr + 10);
    }
    break;
  case (CPM_SIGNATURE_32 ('L', 'P', 'C', '0')):
    if (*(BufferPtr + 11)) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr + 11);
    }
    break;
  case (CPM_SIGNATURE_32 ('O', 'K', 'E', 'C')):
    if (*(BufferPtr + 12)) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr + 12);
    }
    break;
  case (CPM_SIGNATURE_32 ('P', 'C', 'X', 'X')):
    if (*(BufferPtr + 13)) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr + 13);
    }
    break;
  case (CPM_SIGNATURE_32 ('G', 'P', 'X', 'X')):
    if (*(BufferPtr + 14)) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr + 14);
    }
    break;
  case (CPM_SIGNATURE_32 ('S', 'A', 'X', 'X')):
    if (*(BufferPtr + 15)) {
      *(UINT32*) AmlObjPtr = *(UINT32*)(BufferPtr + 15);
    }
    break;
  }

  return FALSE;
}

