/** @file
  This driver is for Hybrid Graphics Feature SMM initialize.

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <HybridGraphicsSmm.h>
#include <Protocol/FchSmmIoTrapDispatch2.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
//
// Function Prototypes
//
VOID
MxmInt15Callback (
  IN OUT EFI_IA32_REGISTER_SET          *CpuRegs,
  IN VOID                               *Context
  );

STATIC
VOID
NvidiaOptimusHandler (
  IN EFI_HANDLE                         DispatchHandle,
  IN CONST VOID                         *DispatchContext,
  IN OUT VOID                           *CommBuffer,
  IN OUT UINTN                          *CommBufferSize
  );

EFI_STATUS
EFIAPI
InitialNvidiaOptimusIOTrap (
  VOID
  );

//
// Module globals
//
EFI_SMM_SYSTEM_TABLE2                         *mSmst;
H2O_HG_INFO_PROTOCOL                          *mHgInfoProtocol;
// EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL            *mSmmIoTrapDispatch;
// EFI_SMM_IO_TRAP_REGISTER_CONTEXT              mIchIoTrapContext64;
// EFI_HANDLE                                    mIchIoTrapHandle64;

/**

  Hybrid Graphics feature SMM driver entry point.
  This driver will install HG and MXM int 15 callback function.

  @param[in] ImageHandle   Image handle of this driver.
  @param[in] SystemTable   Pointer to standard EFI system table.

  @retval EFI_SUCCESS    Hybrid Graphics feature DXE initialized successfully.
  @retval !EFI_SUCCESS   Hybrid Graphics feature doesn't be supported.

**/
EFI_STATUS
EFIAPI
HybridGraphicsSmmInitEntry (
  IN EFI_HANDLE                               ImageHandle,
  IN EFI_SYSTEM_TABLE                         *SystemTable
  )
{
  BOOLEAN                                     InSmm;
  EFI_SMM_BASE2_PROTOCOL                      *SmmBase2;
  EFI_SMM_INT15_SERVICE_PROTOCOL              *SmmInt15Service;
  EFI_STATUS                                  Status;

  InSmm = FALSE;
  SmmInt15Service = NULL;
  mHgInfoProtocol = NULL;

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID**)&SmmBase2);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = SmmBase2->InSmm (SmmBase2, &InSmm);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!InSmm) {
    return EFI_SUCCESS;
  }

  //
  // Great!  We're now in SMM!
  //
  Status = SmmBase2->GetSmstLocation (SmmBase2, &mSmst);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Locate the HG Info Protocol
  //
  Status = gBS->LocateProtocol (&gH2OHybridGraphicsInfoProtocolGuid, NULL, (VOID **)&mHgInfoProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate gH2OHybridGraphicsInfoProtocolGuid Protocol%r\n", Status));
  }
  if (FeaturePcdGet (PcdNvidiaOptimusSupported)) {
    //
    //  Register SMM handler 
    //
    Status = InitialNvidiaOptimusIOTrap();
  }
  Status = mSmst->SmmLocateProtocol (&gEfiSmmInt15ServiceProtocolGuid, NULL, (VOID **)&SmmInt15Service);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Status = SmmInt15Service->InstallInt15ProtocolInterface (
  //                             SmmInt15Service,
  //                             HYBRID_GRAPHICS_INT15_FUN_NUM,
  //                             IntelHgCallback,
  //                             NULL
  //                             );
  // if (Status == EFI_ALREADY_STARTED) {
  //   //
  //   // use new callback function to replace original one
  //   //
  //   Status = SmmInt15Service->ReinstallInt15ProtocolInterface (
  //                               SmmInt15Service,
  //                               HYBRID_GRAPHICS_INT15_FUN_NUM,
  //                               IntelHgCallback,
  //                               NULL
  //                               );
  //   if (EFI_ERROR(Status)) {
  //     return Status;
  //   }
  // }

  if (FeaturePcdGet (PcdNvidiaOptimusSupported)) {
    Status = SmmInt15Service->InstallInt15ProtocolInterface (
                                SmmInt15Service,
                                MXM_INT15_FUN_NUM,
                                MxmInt15Callback,
                                NULL
                                );
    if (Status == EFI_ALREADY_STARTED) {
      //
      // use new callback function to replace original one
      //
      Status = SmmInt15Service->ReinstallInt15ProtocolInterface (
                                  SmmInt15Service,
                                  MXM_INT15_FUN_NUM,
                                  MxmInt15Callback,
                                  NULL
                                  );
      if (EFI_ERROR(Status)) {
        return Status;
      }
    }
  }

  return EFI_SUCCESS;
}

/**

  MXM (Mobile PCI Express Module) INT15 Callback Routine specific function Number 0x5F80
  get MXM SIS (System Information Structure) binary or VBIOS.

  @param[in]  CpuRegs   The structure containing CPU Registers (AX, BX, CX, DX etc.).
                        CX - For most Settings (Configurations) described in the VBIOS Spec.
                        BX - In most cases, it is a calling register. It also can be used
                             as a return (for Settings/Configurations) register like CX
                        AX - Indicate the Status of Function Supported Fail/Success
  @param[in]  Context   Pointer to the context data registerd to the Event.

  @return None.

**/
VOID
MxmInt15Callback (
  IN OUT EFI_IA32_REGISTER_SET          *CpuRegs,
  IN VOID                               *Context
  )
{
  UINT32                                *MxmBinSrc;
  UINT16                                MxmBinSize;

  MxmBinSrc  = NULL;
  MxmBinSize = 0;

  if ((CpuRegs->X.AX & 0xFFFF) != MXM_INT15_FUN_NUM) {
    return;
  }

  switch (CpuRegs->H.BL) {
    case 0:
      //
      // Function 0 -Return Specification Support Level
      //
      // Entry:
      //   AX = 5F80h
      //   BL = 00h
      //   BH = FFh
      //   EBX[16:31] = Adapter ID
      //   CX = Revision of the MXM software specification
      //        that is supported by the MXM Module
      //
      // Return:
      //   AX = 005Fh to indicate that the system bios supports this function
      //   BL = Revision of the MXM software specification that is supported
      //        by the System Format is binary coded decimal,
      //        for example: 30h = 3.0, etc.
      //   CX = MXM functions supported
      //        Bit 0 = 1
      //        Bit 1 = 1 if Function 1 is supported, 0 if not supported
      //        Bit 2 = 1 if Function 2 is supported, 0 if not supported
      //        Bit 3 = 1 if Function 3 is supported, 0 if not supported
      //        Bit 4 = 1 if Function 4 is supported, 0 if not supported
      //        Bit 7 = 1 if Function 7 is supported, 0 if not supported
      //        Bit 8 = 1 if Function 8 is supported, 0 if not supported
      //        Bit 9 = 1 if Function 9 is supported, 0 if not supported
      //
      CpuRegs->H.BL &= ~(0xFF);
      CpuRegs->H.BL |= 0x30; // Support 3.0

      if (mHgInfoProtocol->HgMode == DisplayModeHgDisabled) {
        //
        // Return Function supported but failed
        //
        CpuRegs->X.CX = 0x0000;
        CpuRegs->X.AX &= ~(0xFFFF);
        CpuRegs->X.AX |= 0x015F;
        break;
      }
      if (mHgInfoProtocol->HgMode == DisplayModeDgpu) {
        //
        // Support function 0, 1
        //
        CpuRegs->X.CX &= ~(0xFFFF);
        CpuRegs->X.CX |= 0x0003;
      }
      if (mHgInfoProtocol->HgMode == DisplayModeHg) {
        //
        // Support function 0, 1, 7
        //
        CpuRegs->X.CX &= ~(0xFFFF);
        CpuRegs->X.CX |= 0x0083;
      }
      //
      // Return Function supported and successful
      //
      CpuRegs->X.AX &= ~(0xFFFF);
      CpuRegs->X.AX |= 0x005F;
      break;

    case 1:
      //
      // Function 1 -Return a Pointer to the MXM Structure
      //
      // Entry:
      //   AX = 5F80h
      //   BL = 01h
      //   BH = FFh
      //   EBX[16:31] = Adapter ID
      //   CX = Identifier for the data block to return
      //
      // Return:
      //   AX = 005Fh to indicate that the system bios supports this function
      //   BX = Vendor ID of data block if CX = 0x80-0x8F, else 0
      //   ES:DI = Pointer to the MXM structure in real mode memory (< 1 MB)
      //
      if (mHgInfoProtocol == NULL) {
        //
        // Return Function supported but failed
        //
        CpuRegs->X.AX &= ~(0xFFFF);
        CpuRegs->X.AX |= 0x015F;
        break;
      }
      if ((CpuRegs->E.EBX & 0xFF000000) > 0x01000000) {
        // MxmBinSrc  = (UINT32 *)mHgInfoProtocol->Dgpu2MxmBinFile.Address;
        // MxmBinSize = (UINT16)mHgInfoProtocol->Dgpu2MxmBinFile.Size;
      } else {
        MxmBinSrc  = (UINT32 *)mHgInfoProtocol->DgpuMxmBinFile.Address;
        MxmBinSize = (UINT16)mHgInfoProtocol->DgpuMxmBinFile.Size;
      }

      if ((MxmBinSize > 0x1000) || (MxmBinSize == 0) || (MxmBinSrc == 0)) {
        //
        // Return Function supported but failed
        //
        CpuRegs->X.AX &= ~(0xFFFF);
        CpuRegs->X.AX |= 0x015F;
        break;
      }
      CpuRegs->X.ES = (UINT16)((((UINTN)MxmBinSrc) & ~(0xFFFF)) >> 4);
      CpuRegs->E.EDI &= ~(0xFFFF);
      CpuRegs->E.EDI |= (UINT32)(((UINTN)MxmBinSrc) & (0xFFFF));
      //
      // The system bios supports this function but function failed (reserve bit31 - bit16)
      //
      CpuRegs->X.AX &= ~(0xFFFF);
      CpuRegs->X.AX |= 0x005F;
      break;

    case 7:
      //
      // Function 7 -Return a Pointer to the VBIOS Image for ROM-Less Adapters
      //
      // Entry:
      //   AX = 5F80h
      //   BL = 07h
      //   BH = FFh
      //   EBX[16:31] = Adapter ID
      //
      // Return:
      //   AX = 005Fh to indicate that the system bios supports this function
      //   ESDI = Physical Memory offset to the 128K aperture
      //          containing the selected VBIOS image.
      //
      if (mHgInfoProtocol == NULL) {
        //
        // The system bios supports this function but function failed (reserve bit31 - bit16)
        //
        CpuRegs->X.AX &= ~(0xffff);
        CpuRegs->X.AX |= 0x015F;
        break;
      }
      CpuRegs->X.ES = 0;
      CpuRegs->E.EDI &= ~(0xFFFFFFFF);
      CpuRegs->E.EDI |= (UINT32)((mHgInfoProtocol->Vbios.Address) & (0xFFFFFFFF));
      //
      // The system bios supports this function but function failed (reserve bit31 - bit16)
      //
      CpuRegs->X.AX &= ~(0xffff);
      CpuRegs->X.AX |= 0x005F;
      break;

    default:
      break;
  }
}


EFI_STATUS
EFIAPI
InitialNvidiaOptimusIOTrap (
  VOID
  )
{
  EFI_STATUS                                  Status;
  FCH_SMM_IO_TRAP_DISPATCH2_PROTOCOL          *AmdIoTrapDispatch;
  FCH_SMM_IO_TRAP_REGISTER_CONTEXT            IoTrapRegisterContext;
  EFI_HANDLE                                  IoTrapHandle;

  Status = mSmst->SmmLocateProtocol(&gFchSmmIoTrapDispatch2ProtocolGuid, NULL, &AmdIoTrapDispatch);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate IoTrap Protocol returned %r\n", Status));
    return Status;
  }
  DEBUG ((DEBUG_ERROR, "Installing EcCsIoTrapHandler Handler \n"));
  IoTrapRegisterContext.Type         = ReadTrap;
  IoTrapRegisterContext.Length       = 1;
  IoTrapRegisterContext.Address      = 0x3CA;

  Status = AmdIoTrapDispatch->Register (
                                AmdIoTrapDispatch,
                                (EFI_SMM_HANDLER_ENTRY_POINT2)NvidiaOptimusHandler,
                                &IoTrapRegisterContext,
                                &IoTrapHandle
                                );
  DEBUG ((DEBUG_ERROR, "IoTrap register Status: %x\n", Status));

  // //
  // // Disable IO Trap Handler
  // //
  // Status = mSmst->SmmLocateProtocol (&gPchSmmIoTrapControlGuid, NULL, &mPchSmmIoTrapControl);
  // if (EFI_ERROR (Status)) {
  //   ASSERT_EFI_ERROR (Status);
  //   return Status;
  // }
  // Status = mPchSmmIoTrapControl->Pause (
  //                       mPchSmmIoTrapControl,
  //                       mIchIoTrapHandle64
  //                       );
  // if (EFI_ERROR (Status)) {
  //   DEBUG ((DEBUG_ERROR, "mPchSmmIoTrapControl->Pause returned %r\n", Status));
  //   return Status;
  // }

  return EFI_SUCCESS;
}

VOID
NvidiaOptimusHandler (
  IN EFI_HANDLE                         DispatchHandle,
  IN CONST VOID                         *DispatchContext,
  IN OUT VOID                           *CommBuffer,
  IN OUT UINTN                          *CommBufferSize
  )
{
  UINTN                                 PciAddress;
  UINTN                                 Gpp0BaseAddress;

  PciAddress = PCI_EXPRESS_LIB_ADDRESS (GPP_BUS_NUM, GPP_DEV_NUM, GPP0_FUN_NUM, 0x00);
  Gpp0BaseAddress = (UINT32)(PcdGet64 (PcdPciExpressBaseAddress) + PciAddress);
  //
  //  a. Enable PEX Link between root port and GPU ( it should be Link Disable bit in Link Control register of root port )
  //
  MmioOr16 (Gpp0BaseAddress + GPP_LCTL_OFFSET, BIT4);
  // //
  // //  b. Toggle GPU_EVENT(ENVT) to LOW ( GPU_EVENT is a PCH GPIO assigned by H/W. it might be different from project to project
  // //
  // GpioSetOutputValue (FixedPcdGet32 (PcdNvidiaGPUEvent), V_GPIO_PCR_TX_STATE_LOW);
  // //
  // //  c. Wait 20ms
  // //
  // MicroSecondDelay (20);
  // //
  // //  d.       Toggle GPU_EVENT(ENVT) to HIGH
  // //
  // GpioSetOutputValue (FixedPcdGet32 (PcdNvidiaGPUEvent), V_GPIO_PCR_TX_STATE_HIGH);
  // //
  // //
  // //  e. Touch the following registers on root port to enable PEX Link Bundle power. This is required for SKL-H and KBL-H for GC6 exit.
  // //       Root port PCIE register 0xC20[5:4] = 00b
  // //       Root port PCIE Register 0xC38[3] = 00b
  // MmioAnd8(Peg0BaseAddress + R_SA_PEG_AFEOVR_OFFSET, (UINT8) ~(BIT5 | BIT4));
  // MmioAnd8(Peg0BaseAddress + R_SA_PEG_CMNSPARE_OFFSET, (UINT8) ~(BIT3));
}
