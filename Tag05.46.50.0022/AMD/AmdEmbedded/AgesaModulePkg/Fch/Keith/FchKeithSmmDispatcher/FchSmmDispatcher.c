/******************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
**/
#include "FchSmmDispatcher.h"
#include <Protocol/SmmCpuIo2.h>
#include <Protocol/FchInitProtocol.h>
#include "FchSmmSxDispatcher.h"
#include "FchSmmSwDispatcher.h"
#include "FchSmmPwrBtnDispatcher.h"
#include "FchSmmIoTrapDispatcher.h"
#include "FchSmmPeriodicalDispatcher.h"
#include "FchSmmGpiDispatcher.h"
#include "FchSmmUsbDispatcher.h"
#include "FchSmmMiscDispatcher.h"
#include <Library/SmmMemLib.h>

#define FILECODE FCH_KEITH_FCHKEITHSMMDISPATCHER_FCHSMMDISPATCHER_FILECODE

extern FCH_SMM_DISPATCHER_TABLE FchSmmDispatcherTable[];
extern UINT8 NumOfDispatcherTableEntry;
extern SAVE_B2B_IO B2bIoList[];
extern UINT8 NumOfB2bIoListEntry;
extern SAVE_PCI SavePciList[];
extern UINT8 NumOfSavePciListEntry;
extern UINT16 Cpl3BlockIoList[];
extern UINT8 NumOfCpl3BlockIoListEntry;

EFI_SMM_CPU_PROTOCOL         *mSmmCpuProtocol;

//
// Smi Handler Profile Protocol is optional.
//
SMI_HANDLER_PROFILE_PROTOCOL *mSmiHandlerProfileProtocol = NULL;

FCH_SMM_SW_NODE              *HeadFchSmmSwNodePtr;
FCH_SMM_SX_NODE              *HeadFchSmmSxNodePtr;
FCH_SMM_PWRBTN_NODE          *HeadFchSmmPwrBtnNodePtr;
FCH_SMM_PERIODICAL_NODE      *HeadFchSmmPeriodicalNodePtr;
FCH_SMM_GPI_NODE             *HeadFchSmmGpiNodePtr;
FCH_SMM_USB_NODE             *HeadFchSmmUsbNodePtr;
FCH_SMM_MISC_NODE            *HeadFchSmmMiscNodePtr;
FCH_SMM_COMMUNICATION_BUFFER *CommunicationBufferPtr;
FCH_SMM_SW_CONTEXT           *EfiSmmSwContext;

EFI_SMM_PERIODIC_TIMER_CONTEXT EfiSmmPeriodicTimerContext;

typedef struct {
  EFI_GUID  *Guid;
  VOID      *Interface;
} FCH_PROTOCOL_LIST;

UINT16 mAcpiPmCntBase;
UINT16 mSwSmiCmdAddress;

FCH_PROTOCOL_LIST FchProtocolList[] = {
                    &gFchSmmSwDispatch2ProtocolGuid,            &gFchSmmSwDispatch2Protocol,
                    &gEfiSmmSwDispatch2ProtocolGuid,            &gEfiSmmSwDispatch2Protocol,
                    &gFchSmmSxDispatch2ProtocolGuid,            &gFchSmmSxDispatch2Protocol,
                    &gEfiSmmSxDispatch2ProtocolGuid,            &gEfiSmmSxDispatch2Protocol,
                    &gFchSmmPwrBtnDispatch2ProtocolGuid,        &gFchSmmPwrBtnDispatch2Protocol,
                    &gEfiSmmPowerButtonDispatch2ProtocolGuid,   &gEfiSmmPwrBtnDispatch2Protocol,
                    &gFchSmmPeriodicalDispatch2ProtocolGuid,    &gFchSmmPeriodicalDispatch2Protocol,
                    &gEfiSmmPeriodicTimerDispatch2ProtocolGuid, &gEfiSmmPeriodicalDispatch2Protocol,
                    &gFchSmmUsbDispatch2ProtocolGuid,           &gFchSmmUsbDispatch2Protocol,
                    &gEfiSmmUsbDispatch2ProtocolGuid,           &gEfiSmmUsbDispatch2Protocol,
                    &gFchSmmGpiDispatch2ProtocolGuid,           &gFchSmmGpiDispatch2Protocol,
                    &gEfiSmmGpiDispatch2ProtocolGuid,           &gEfiSmmGpiDispatch2Protocol,
                    &gFchSmmIoTrapDispatch2ProtocolGuid,        &gFchSmmIoTrapDispatch2Protocol,
                    &gEfiSmmIoTrapDispatch2ProtocolGuid,        &gEfiSmmIoTrapDispatch2Protocol,
                    &gFchSmmMiscDispatchProtocolGuid,           &gFchSmmMiscDispatchProtocol,
                    };

EFI_STATUS
EFIAPI
FchSmmDispatchHandler (
  IN       EFI_HANDLE                   SmmImageHandle,
  IN       CONST EFI_SMM_ENTRY_CONTEXT  *SmmEntryContext,
  IN OUT   VOID                         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN                        *SourceSize OPTIONAL
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD FCH SMM dispatcher driver
 * Example of dispatcher driver that handled IO TRAP requests only
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmDispatcherEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    DispatchHandle;
  EFI_HANDLE                    FchSmmDispatcherHandle;
  UINTN                         i;
  FCH_INIT_PROTOCOL             *pFchCimxInitProtocol;

  AGESA_TESTPOINT (TpFchSmmDispatcherEntry, NULL);

  Status = gBS->LocateProtocol (
                  &gFchInitProtocolGuid,
                  NULL,
                  &pFchCimxInitProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mAcpiPmCntBase =
    ((FCH_DATA_BLOCK *) (pFchCimxInitProtocol->FchPolicy))->FchResetDataBlock.FchBldCfg.CfgAcpiPm1CntBlkAddr;
  mSwSmiCmdAddress =
    ((FCH_DATA_BLOCK *) (pFchCimxInitProtocol->FchPolicy))->FchResetDataBlock.FchBldCfg.CfgSmiCmdPortAddr;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmCpuProtocolGuid,
                    NULL,
                    &mSmmCpuProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (FCH_SMM_SW_NODE),
                    &HeadFchSmmSwNodePtr
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (HeadFchSmmSwNodePtr, sizeof (FCH_SMM_SW_NODE));

  Status = gSmst->SmmAllocatePool (
                          EfiRuntimeServicesData,
                          sizeof (FCH_SMM_SX_NODE),
                          &HeadFchSmmSxNodePtr
                          );

  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (HeadFchSmmSxNodePtr, sizeof (FCH_SMM_SX_NODE));

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (FCH_SMM_PWRBTN_NODE),
                    &HeadFchSmmPwrBtnNodePtr
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (HeadFchSmmPwrBtnNodePtr, sizeof (FCH_SMM_PWRBTN_NODE));

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (FCH_SMM_PERIODICAL_NODE),
                    &HeadFchSmmPeriodicalNodePtr
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (HeadFchSmmPeriodicalNodePtr, sizeof (FCH_SMM_PERIODICAL_NODE));

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (FCH_SMM_GPI_NODE),
                    &HeadFchSmmGpiNodePtr
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (HeadFchSmmGpiNodePtr, sizeof (FCH_SMM_GPI_NODE));
  HeadFchSmmGpiNodePtr->Context.GpiNum = 0xffff;

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (FCH_SMM_USB_NODE),
                    &HeadFchSmmUsbNodePtr
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (HeadFchSmmUsbNodePtr, sizeof (FCH_SMM_USB_NODE));
  HeadFchSmmUsbNodePtr->Context.Order = 0xFF;

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (FCH_SMM_MISC_NODE),
                    &HeadFchSmmMiscNodePtr
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (HeadFchSmmMiscNodePtr, sizeof (FCH_SMM_MISC_NODE));

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (FCH_SMM_SW_CONTEXT),
                    &EfiSmmSwContext
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (EfiSmmSwContext, sizeof (FCH_SMM_SW_CONTEXT));

  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (FCH_SMM_COMMUNICATION_BUFFER),
                    &CommunicationBufferPtr
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Smi Handler Profile Protocol is optional.
  //
  gSmst->SmmLocateProtocol (&gSmiHandlerProfileGuid, NULL, &mSmiHandlerProfileProtocol);

  for (i = 0 ; i < sizeof (FchProtocolList) / sizeof (FCH_PROTOCOL_LIST); i++ ) {
    FchSmmDispatcherHandle =  NULL;
    Status = gSmst->SmmInstallProtocolInterface (
               &FchSmmDispatcherHandle,
               FchProtocolList[i].Guid,
               EFI_NATIVE_INTERFACE,
               FchProtocolList[i].Interface);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  Status = gSmst->SmiHandlerRegister (
                    FchSmmDispatchHandler,
                    NULL,
                    &DispatchHandle
                    );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  {
    UINT32  SmmDispatcherData32;
    UINT32  SmmDispatcherIndex;

  //
  // Clear all handled SMI status bit
  //
    for (SmmDispatcherIndex = 0; SmmDispatcherIndex < NumOfDispatcherTableEntry; SmmDispatcherIndex++ ) {
      SmmDispatcherData32 =
        ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FchSmmDispatcherTable[SmmDispatcherIndex].StatusReg);
      SmmDispatcherData32 &= FchSmmDispatcherTable[SmmDispatcherIndex].SmiStatusBit;
      ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FchSmmDispatcherTable[SmmDispatcherIndex].StatusReg) =
        SmmDispatcherData32;
    }
  //
  // Clear SmiEnB and Set EOS
  //
    SmmDispatcherData32 = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98);
    SmmDispatcherData32 &= ~(BIT31);
    SmmDispatcherData32 |= BIT28;
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98) = SmmDispatcherData32;
  }

  AGESA_TESTPOINT (TpFchSmmDispatcherExit, NULL);
  return Status;
}

BOOLEAN
IsCpl3BlockIo(
  UINT16    Io
  )
{
  UINT8  Index;

  for (Index = 0; Index < NumOfCpl3BlockIoListEntry; Index++){
    if(Io == Cpl3BlockIoList[Index]){
      return TRUE;
    }
  }
  return FALSE;
}

VOID
SaveB2BRegisters (
  VOID
  )
{
  EFI_SMM_CPU_IO2_PROTOCOL   *SmmCpuIo;
  UINT8                      i;
  UINT32                     PciAddress;
  UINT16                     CodeSegment;

  SmmCpuIo = &gSmst->SmmIo;

  CodeSegment = AsmReadCs();

  for (i = 0; i < NumOfB2bIoListEntry; i++) {
    if (CodeSegment & (BIT0|BIT1)){
      if (TRUE == IsCpl3BlockIo(B2bIoList[i].ioPort)){
        continue;
      }
    }
    SmmCpuIo->Io.Read (SmmCpuIo, B2bIoList[i].ioWidth, B2bIoList[i].ioPort, 1, &B2bIoList[i].ioValue);
  }
  for (i = 0; i < NumOfSavePciListEntry; i++) {
    PciAddress = MAKE_SBDFO (0, SavePciList[i].Bus, SavePciList[i].Dev, SavePciList[i].Func, SavePciList[i].Offset);
    switch (SavePciList[i].DataWidth){
      case SMM_IO_UINT32:
        SavePciList[i].DataValue = PciRead32 (PciAddress);
        break;
      case SMM_IO_UINT16:
        SavePciList[i].DataValue = PciRead16 (PciAddress);
        break;
      case SMM_IO_UINT8:
        SavePciList[i].DataValue = PciRead8 (PciAddress);
        break;
      default:
        break;
    }
  }
}

VOID
RestoreB2BRegisters ( VOID )
{
  EFI_SMM_CPU_IO2_PROTOCOL   *SmmCpuIo;
  UINT8                      i;
  UINT32                     PciAddress;
  UINT16                     CodeSegment;

  SmmCpuIo = &gSmst->SmmIo;

  CodeSegment = AsmReadCs();

  for (i = 0; i < NumOfSavePciListEntry; i++) {
    PciAddress = MAKE_SBDFO (0, SavePciList[i].Bus, SavePciList[i].Dev, SavePciList[i].Func, SavePciList[i].Offset);
    switch (SavePciList[i].DataWidth){
      case SMM_IO_UINT32:
        PciWrite32 (PciAddress, (UINT32)(SavePciList[i].DataValue));
        break;
      case SMM_IO_UINT16:
        PciWrite16 (PciAddress, (UINT16)(SavePciList[i].DataValue));
        break;
      case SMM_IO_UINT8:
        PciWrite8 (PciAddress, (UINT8)(SavePciList[i].DataValue));
        break;
      default:
        break;
    }
  }
  for (i = 0; i < NumOfB2bIoListEntry; i++) {
    if (CodeSegment & (BIT0|BIT1)){
      if (TRUE == IsCpl3BlockIo(B2bIoList[i].ioPort)){
        continue;
      }
    }
    SmmCpuIo->Io.Write (SmmCpuIo, B2bIoList[i].ioWidth, B2bIoList[i].ioPort, 1, &B2bIoList[i].ioValue);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * FCH SMM dispatcher handler
 *
 *
 * @param[in]       SmmImageHandle        Image Handle
 * @param[in]       SmmEntryContext         (see PI 1.2 for more details)
 * @param[in, out]   OPTIONAL CommunicationBuffer   Communication Buffer (see PI 1.1 for more details)
 * @param[in, out]   OPTIONAL SourceSize            Buffer size (see PI 1.1 for more details)

 * @retval          EFI_SUCCESS           SMI handled by dispatcher
 * @retval          EFI_UNSUPPORTED       SMI not supported by dispcther
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchSmmDispatchHandler (
  IN       EFI_HANDLE                   SmmImageHandle,
  IN       CONST EFI_SMM_ENTRY_CONTEXT  *SmmEntryContext,
  IN OUT   VOID                         *CommunicationBuffer OPTIONAL,
  IN OUT   UINTN                        *SourceSize OPTIONAL
  )
{
  UINT8        SmmDispatcherIndex;
  UINT32       SmiStatusData;
  UINT32       SmiReg88StatusData;
  //UINT32       UsbSmiEnableRegister;
  //UINT32       UsbSmiEnableRegister1;
  //UINT32       UsbSmiEnableStatus;
  //UINT32       UsbSmiEnableStatus1;
  UINT32       EosStatus;
  EFI_STATUS   Status;

  if ( CommunicationBuffer != NULL
    && SourceSize != NULL
    && !SmmIsBufferOutsideSmmValid ((UINTN)CommunicationBuffer, (UINT64)(*SourceSize)) )
  {
    DEBUG ((EFI_D_ERROR, "[%a]SMM communication data buffer in SMRAM or overflow!\n", __FUNCTION__));
    return (EFI_INVALID_PARAMETER);
  }

  Status = EFI_WARN_INTERRUPT_SOURCE_PENDING; //Updated to be compliant with UDK2010.SR1.UP1
  SaveB2BRegisters ();
  SmiReg88StatusData = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG88) & SmiCmdPort;
  //FchSmnRead (0, 0x16D80118, &UsbSmiEnableRegister, NULL);
  //UsbSmiEnableStatus = UsbSmiEnableRegister & BIT8;
  //FchSmnRead (0, 0x16F80118, &UsbSmiEnableRegister1, NULL);
  //UsbSmiEnableStatus1 = UsbSmiEnableRegister1 & BIT8;
  do {
    //if (UsbSmiEnableStatus) {
    // FchSmnRW (0, 0x16D80118, 0xfffffeff, 0x000, NULL);
    //}
    //if (UsbSmiEnableStatus1) {
    // FchSmnRW (0, 0x16F80118, 0xfffffeff, 0x000, NULL);
    //}
    for (SmmDispatcherIndex = 0; SmmDispatcherIndex < NumOfDispatcherTableEntry; SmmDispatcherIndex++ ) {
      SmiStatusData = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FchSmmDispatcherTable[SmmDispatcherIndex].StatusReg);
      if (( SmiStatusData &= FchSmmDispatcherTable[SmmDispatcherIndex].SmiStatusBit) != 0) {
        CommunicationBufferPtr->SmiStatusReg = FchSmmDispatcherTable[SmmDispatcherIndex].StatusReg;
        CommunicationBufferPtr->SmiStatusBit = SmiStatusData;
        CommunicationBuffer = (VOID *) CommunicationBufferPtr;
        Status = FchSmmDispatcherTable[SmmDispatcherIndex].SmiDispatcher (SmmImageHandle,
                                                                            CommunicationBuffer,
                                                                            SourceSize
                                                                            );
        if (Status != EFI_SUCCESS) {
          Status = EFI_WARN_INTERRUPT_SOURCE_PENDING;
        }
        SmmDispatcherIndex = 0;
      }
    }
    //if (UsbSmiEnableStatus) {
    // FchSmnRW (0, 0x16D80118, 0xfffffeff, 0x100, NULL);
    //}
    //if (UsbSmiEnableStatus1) {
    // FchSmnRW (0, 0x16F80118, 0xfffffeff, 0x100, NULL);
    //}
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98) |= Eos;
    EosStatus = ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98) & Eos;
  } while ((EosStatus != Eos) || (ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG84) & UsbSmi));
  RestoreB2BRegisters ();
  return  Status;
}

//temp fix..
CONST VOID* IdsDebugPrint[] =
{
  NULL
};
