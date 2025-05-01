/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */


/**** This is a platform specific reference only driver, the implemenatation depends on OEM design***/

#include <AmdCpmDxe.h>
#include <Protocol/PspPlatformProtocol.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmdPspBaseLibV2.h>

STATIC EFI_HANDLE mPspPlatformHandle = NULL;

STATIC PSP_PLATFORM_PROTOCOL mPspPlatformProtocol = {
  TRUE,                   //Support CPU Context Resume
  0xDD,                   //SW SMI Command for Build Context
  0x40000,                //BSP Stack Size
  0x4000,                 //AP Stack Size
  NULL                    //Address of PEI Info Structure
};

EFI_STATUS
PspPlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS        Status;
  TYPE_ATTRIB       TypeAttrib;
  UINT64            EntryAddress;
  UINT32            EntrySize;
  UINT64            EntryDest;
  UINT64            RsmInfoLocation;
  UINT64            ResetVectorSeg;
  BOOLEAN           ApcbEntryExist;

  DEBUG((DEBUG_INFO, "FEA-DXE-PspPlatformEntryPoint-Start\n"));

/*Search for Resume HandOff Infomation structure*/

  mPspPlatformProtocol.SwSmiCmdtoBuildContext = PcdGet8 (PcdSwSmiCmdtoBuildContext);
  mPspPlatformProtocol.BspStackSize = PcdGet32 (PcdBspStackSize);
  mPspPlatformProtocol.ApStackSize = PcdGet32 (PcdApStackSize);
  // Resume HandOff Infomation structure
  // This should based on Resume SEC phase design
  mPspPlatformProtocol.RsmHandOffInfo = (RSM_HANDOFF_INFO *)PcdGet64(PcdRsmInfoLocation);
  // Platform does not set it, try AMD standard layout like below
  //    RSM_HANDOFF_INFO
  //    ModuleEntryPoint
  if (mPspPlatformProtocol.RsmHandOffInfo == 0) {
    ApcbEntryExist = BIOSEntryInfo (BIOS_FIRMWARE, INSTANCE_IGNORED, &TypeAttrib, &EntryAddress, &EntrySize, &EntryDest);
    if (!ApcbEntryExist) {
      DEBUG((DEBUG_INFO, "FEA-DXE-PspPlatformEntryPoint-End-APCB Entry not exist\n"));
      return EFI_NOT_FOUND;
    }
    ResetVectorSeg = EntryDest + EntrySize - 0x10000;
    // get ModuleEntryPoint, like: xxFFFF : 90 90 E9 EB E5 - nop nop jmp addr
    RsmInfoLocation = *((UINT16 *)(UINTN) (ResetVectorSeg + 0xFFF0 + 0x3));
    RsmInfoLocation += 0xFFF5;  //jmp
    RsmInfoLocation = (RsmInfoLocation & 0xFFFF) | ResetVectorSeg;
    // ModuleEntryPoint is E5E0, then - sizeof (RSM_HANDOFF_INFO)
    RsmInfoLocation -= sizeof (RSM_HANDOFF_INFO);
    mPspPlatformProtocol.RsmHandOffInfo = (RSM_HANDOFF_INFO *)RsmInfoLocation;
  }

  DEBUG((DEBUG_INFO, "  mPspPlatformProtocol.CpuContextResumeEnable = %x\n", mPspPlatformProtocol.CpuContextResumeEnable ? 1:0));
  DEBUG((DEBUG_INFO, "  mPspPlatformProtocol.SwSmiCmdtoBuildContext = 0x%x\n", mPspPlatformProtocol.SwSmiCmdtoBuildContext));
  DEBUG((DEBUG_INFO, "  mPspPlatformProtocol.BspStackSize =           0x%x\n", mPspPlatformProtocol.BspStackSize));
  DEBUG((DEBUG_INFO, "  mPspPlatformProtocol.ApStackSize =            0x%x\n", mPspPlatformProtocol.ApStackSize));
  DEBUG((DEBUG_INFO, "  mPspPlatformProtocol.RsmHandOffInfo =         0x%x\n", mPspPlatformProtocol.RsmHandOffInfo));
/*===================================================*/

  //Install Protocol
  DEBUG((DEBUG_INFO, "FEA-DXE-PspPlatformEntryPoint-InstallProtocolInterface-gPspPlatformProtocolGuid\n"));
  Status = gBS->InstallProtocolInterface (
                              &mPspPlatformHandle,
                              &gPspPlatformProtocolGuid,
                              EFI_NATIVE_INTERFACE,
                              &mPspPlatformProtocol
                              );

  DEBUG((DEBUG_INFO, "FEA-DXE-PspPlatformEntryPoint-End-EFI_SUCCESS\n"));
  return EFI_SUCCESS;
}