/** @file
  TIS (TPM Interface Specification) functions to access discrete TPM module.

;******************************************************************************
;* Copyright (c) 2013, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiPei.h>

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PerformanceLib.h>
#include <Library/BaseLib.h>
//#include <Library/Tpm2TisCommLib.h>

#include <Ppi/Tpm2InterfacePei.h>
#include <Ppi/AmdFtpmPpi.h>
#include <Library/AmdFtpmLib.h>
#include <Library/ChipsetConfigLib.h>
#include <ChipsetSetupConfig.h>
#include <Library/PcdLib.h>
#include <TpmPolicy.h>
#include <Guid/TpmInstance.h>
#include <Guid/AmdTpmDeviceFtpm.h>

//
// Add for HSP
//
#include <Ppi/AmdFtpmPpi.h>
#include <Library/Tpm2CommandLib.h>
#include <Library/HobLib.h>
#include <IndustryStandard/UefiTcgPlatform.h>

#ifndef ALIGN
  #define ALIGN(v, a) (UINTN)(((((UINTN)v) - 1) | ((a) - 1)) + 1)
#endif

EFI_STATUS
EFIAPI
PspFtpmPpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );
#define CHANGE_STARTUP_MP                        ( 1 << 4  ) //BIT4  //Should define in TpmPolicy.h
EFI_PEI_NOTIFY_DESCRIPTOR   mPspFtpmPpiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdFtpmPpiGuid,
  PspFtpmPpiCallback
};

EFI_STATUS
AmdfTpmGetInfo (
  IN OUT   UINTN                *fTPMStatus
)
{
  EFI_STATUS      Status = EFI_SUCCESS;
  AMD_FTPM_PPI    *PspFtpmPpi = NULL;

  do
  {
    Status = PeiServicesLocatePpi (
      &gAmdFtpmPpiGuid,
      0,
      NULL,
      (VOID**)&PspFtpmPpi
    );
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  Locate gAmdFtpmPpiGuid - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }

    Status = PspFtpmPpi->CheckStatus (PspFtpmPpi, fTPMStatus);
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  PspFtpmPpi->CheckStatus(...) - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }
  } while (FALSE);

  return Status;
}

EFI_STATUS
AmdfTpmPeiSend (
  IN     EFI_PEI_SERVICES                 **PeiServices,
  IN     PEI_TPM2_INTERFACE_PPI           *This,
  IN     UINT8                            *BufferIn,
  IN     UINT32                           SizeIn
)
{
  EFI_STATUS      Status = EFI_SUCCESS;
  AMD_FTPM_PPI    *PspFtpmPpi = NULL;

  do
  {
    Status = PeiServicesLocatePpi (
      &gAmdFtpmPpiGuid,
      0,
      NULL,
      (VOID**)&PspFtpmPpi
    );
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  Locate gAmdFtpmPpiGuid - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }

    Status = PspFtpmPpi->SendCommand (PspFtpmPpi, BufferIn, SizeIn);
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  PspFtpmPpi->SendCommand(...) - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }
  } while (FALSE);

  return Status;
}

EFI_STATUS
AmdfTpmPeiReceive (
  IN     EFI_PEI_SERVICES                 **PeiServices,
  IN     PEI_TPM2_INTERFACE_PPI           *This,
  IN OUT UINT8                            *BufferOut,
  IN OUT UINT32                           *SizeOut
)
{
  EFI_STATUS      Status = EFI_SUCCESS;
  AMD_FTPM_PPI    *PspFtpmPpi = NULL;

  do
  {
    Status = PeiServicesLocatePpi (
      &gAmdFtpmPpiGuid,
      0,
      NULL,
      (VOID**)&PspFtpmPpi
    );
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  Locate gAmdFtpmPpiGuid - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }

    Status = PspFtpmPpi->GetResponse (PspFtpmPpi, BufferOut, SizeOut);
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  PspFtpmPpi->GetResponse(...) - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }
  } while (FALSE);

  return Status;
}

EFI_STATUS
AmdfTpmExecuteCommand (
  IN       VOID                 *CommandBuffer,
  IN       UINT32               CommandSize,
  IN OUT   VOID                 *ResponseBuffer,
  IN OUT   UINT32               *ResponseSize
)
{
  EFI_STATUS      Status = EFI_SUCCESS;
  AMD_FTPM_PPI    *PspFtpmPpi = NULL;

  do
  {
    Status = PeiServicesLocatePpi (
      &gAmdFtpmPpiGuid,
      0,
      NULL,
      (VOID**)&PspFtpmPpi
    );
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  Locate gAmdFtpmPpiGuid - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }

    Status = PspFtpmPpi->SendCommand (PspFtpmPpi, CommandBuffer, CommandSize);
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  PspFtpmPpi->SendCommand(...) - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }

    Status = PspFtpmPpi->GetResponse (PspFtpmPpi, ResponseBuffer, ResponseSize);
    if (EFI_ERROR (Status))
    {
      DEBUG ((DEBUG_INFO, "  PspFtpmPpi->GetResponse(...) - %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      break;
    }
  } while (FALSE);

  return Status;

}

EFI_STATUS
AmdfTpmPeiCommand (
  IN     EFI_PEI_SERVICES                 **PeiServices,
  IN     PEI_TPM2_INTERFACE_PPI           *This,
  IN     UINT8                            *BufferIn,
  IN     UINT32                           SizeIn,
  IN OUT UINT8                            *BufferOut,
  IN OUT UINT32                           *SizeOut
  )
{
  return AmdfTpmExecuteCommand (
    BufferIn,
    SizeIn,
    BufferOut,
    SizeOut
    );
}

EFI_STATUS
AmdfTpmPeiRequestUseTpm (
  IN     EFI_PEI_SERVICES                 **PeiServices,
  IN     PEI_TPM2_INTERFACE_PPI           *This
  )
{
  UINTN              iTPMStatus;
  return AmdfTpmGetInfo (&iTPMStatus);
}

PEI_TPM2_INTERFACE_PPI         mPeiTpm2InterfacePpi = {
  AmdfTpmPeiCommand,
  AmdfTpmPeiRequestUseTpm,
  AmdfTpmPeiSend,
  AmdfTpmPeiReceive
};

EFI_PEI_PPI_DESCRIPTOR   mInstallPeiTpm2InterfacePpi = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gPeiTpm2InterfacePpiGuid,
  &mPeiTpm2InterfacePpi
};

EFI_STATUS
TpmAutoDetectFtpm (
  IN EFI_PEI_SERVICES                  **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR         *NotifyDescriptor,
  IN VOID                              *Ppi
  )
{
  UINTN                                Size;
  EFI_STATUS                           Status;
  DEBUG ((EFI_D_ERROR, "TpmAutoDetectFtpm\n"));

  Size = sizeof (EFI_GUID);  
  Status = PcdSetPtrS (PcdTpmInstanceGuid, &Size, &gEfiTpmDeviceInstanceTpm20FtpmGuid);
  ASSERT_EFI_ERROR (Status);
  
  return EFI_SUCCESS;
}

EFI_PEI_NOTIFY_DESCRIPTOR mTpmDeviceSeleted[] = {
  {(EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiTpmDeviceSelectedGuid,
  TpmAutoDetectFtpm}
};

/*
 * Transfer the Tcg Event Log format to EDKII Base Hob.
 *
 * @param[in] ptrEventHdr       TPM 2.0 Crypto Agile Event Log Event Format.
 *
 * @return                      Size of TcgTpm20 Event Log Size
 * */
//
//  From PSP mail discuss, the Log format will follow TCG Spec
// https://trustedcomputinggroup.org/wp-content/uploads/PC-ClientSpecific_Platform_Profile_for_TPM_2p0_Systems_v51.pdf, Section 9.
// "TPM Crypto Agile Event Log Format."
//
UINTN
GetTcgEvent2LogSize (
  IN UINT8*     ptrEventHdr
)
{
  UINTN         Size;
  UINTN         DigestSize;
  Size = 4 + 4;                         // sizeof (PCRIndex) + sizeof (pcrType)
  ptrEventHdr += Size;
  DigestSize = GetDigestListSize ((TPML_DIGEST_VALUES*)ptrEventHdr);    // Digest_L Size
  Size += DigestSize;
  ptrEventHdr += DigestSize;
  Size += 4;                            // sizeof (EventSize)
  Size += *((UINT32*)ptrEventHdr);      // EventDataSize

  return Size;
}

UINTN
GetTcgEventHdrSize (
    UINT8*     ptrEventHdr
)
{
  TCG_PCR_EVENT_HDR     *pEventHdr = (TCG_PCR_EVENT_HDR*)ptrEventHdr;

  if (EV_NO_ACTION == pEventHdr->EventType && 0 == pEventHdr->PCRIndex)
  {
    return sizeof(TCG_PCR_EVENT_HDR) + pEventHdr->EventSize;
  }
  return 0;
}

VOID PrintBuf(
    UINTN   unBufSize,
    UINT8*  _buf
)
{
    UINTN   unIdx;
    for( unIdx = 0; unIdx<unBufSize; ++unIdx )
    {
        if( unIdx % 0x10 == 0 )
          DEBUG(( EFI_D_ERROR, "\n" ));

        DEBUG(( EFI_D_ERROR, " %02x", _buf[unIdx] ));
    }

    DEBUG(( EFI_D_ERROR, "\n" ));
}


/**
  Entry point of this module.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  Describes the list of possible PEI Services.

  @return Status.

**/
EFI_STATUS
EFIAPI
PspFtpmPpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  EFI_STATUS                       Status;
  UINTN                            iTPMStatus;
  AMD_FTPM_PPI                      *AmdfTpmPpi;
  UINTN                             preEvtLogSize;
  UINT8                             *preEvtLogBuf = NULL;

  DEBUG ((EFI_D_ERROR, "PspFtpmPpiCallback\n"));

  switch (PcdGet8 (PcdH2OTpmType)) {
    case 0:
    case 1:
      DEBUG((EFI_D_INFO, "  Not Select Amd fTPM\n"));
      return EFI_SUCCESS;
      break;
    default:
      break;
  }

  if (EFI_ERROR (AmdfTpmGetInfo (&iTPMStatus))) {
    DEBUG ((EFI_D_ERROR, "AMD fTPM not detected!\n"));
    return EFI_SUCCESS;
  }

//SetTpmPolicy = PcdGet32(PcdPeiTpmPolicy) | TPM2_STARTUP_IN_MP;
//PcdSet32(PcdPeiTpmPolicy, SetTpmPolicy);

  //
  // Install PPI
  //
  Status = PeiServicesInstallPpi (&mInstallPeiTpm2InterfacePpi);
  ASSERT_EFI_ERROR (Status);

  //
  // Notify PPI for TrEEConfigPeim of Tpm Auto Detect feature
  //
  Status = PeiServicesNotifyPpi (mTpmDeviceSeleted);
  ASSERT_EFI_ERROR (Status);

  //
  // AMD HSP f-TPM for EDK2 Core Base, Get the HSP PSP TcgEvetLog before BIOS
  //
  Status = (**PeiServices).LocatePpi ((CONST EFI_PEI_SERVICES**)PeiServices, &gAmdFtpmPpiGuid, 0, NULL, (VOID**)&AmdfTpmPpi);
  ASSERT_EFI_ERROR (Status);

  do {
      UINTN     IndexSize;
      UINTN     TempVal;
      VOID      *HobData;

      IndexSize = 0;
      TempVal   = 0;
      HobData   = NULL;
      
    if (EFI_ERROR (Status)) {
      // Did not locate the PPI.
      break;
      }

    if (SYSTEM_TPM_CONFIG_HSP_FTPM != PcdGet8 (PcdAmdPspSystemTpmConfig)) {
          DEBUG ((EFI_D_INFO, "Platform Did not support HSP\n"));
          break;
      }

      // Get for Event Log Actual Size
      preEvtLogSize = 0;
      Status = AmdfTpmPpi->GetTcgLogs (AmdfTpmPpi, NULL, &preEvtLogSize);
      DEBUG ((EFI_D_INFO, "  AmdfTpmPpi->GetTcgLogs (...) - %r, Size [0x%x].\n", Status, preEvtLogSize));
    if (0 == preEvtLogSize || EFI_ERROR (Status)) {
      // Size is Zero or Failed get Logs.
      break;
    }

      Status = (**PeiServices).AllocatePool (
                                 (CONST EFI_PEI_SERVICES**)PeiServices,
                                 ALIGN (preEvtLogSize + 16, 16),            // allocate pool align.
                                 (VOID**)&preEvtLogBuf
                                 );
    if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "Failed to allocate to HSP pre boot memory\n"));
          ASSERT_EFI_ERROR (Status);
          break;
      }
      SetMem (preEvtLogBuf, ALIGN (preEvtLogSize + 16, 16), 0);

      // Start buffer point align
      preEvtLogBuf = (UINT8*)ALIGN ((UINTN)preEvtLogBuf, 16);

      Status = AmdfTpmPpi->GetTcgLogs (AmdfTpmPpi, (VOID*)preEvtLogBuf, &preEvtLogSize);
    if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "Failed to allocate to HSP GetTcgLogs()\n"));
          break;
      }

      // First 1, We need to check the Event Log Format is expected.
      // Dummy run for check the In/Out Size is expect
      for (IndexSize = 0, TempVal = 0; IndexSize < preEvtLogSize; IndexSize += TempVal) {
        // Check the first header Event format
        if (0 == IndexSize) {
          TempVal = GetTcgEventHdrSize (preEvtLogBuf + IndexSize);
          if (TempVal)
            continue;
        }
        TempVal = GetTcgEvent2LogSize (preEvtLogBuf + IndexSize);
      }

    if (IndexSize != preEvtLogSize) {
          Status = EFI_INVALID_PARAMETER;
          DEBUG ((EFI_D_ERROR, "Un-expect Size fromat of HSP Get\n"));
          break;
      }

      // Create the Pre-x86 TCG Event Log Hob
      for (IndexSize = 0, TempVal = 0; IndexSize < preEvtLogSize; IndexSize += TempVal) {
        if (0 == IndexSize) {
          TempVal = GetTcgEventHdrSize (preEvtLogBuf + IndexSize);
          DEBUG ((DEBUG_INFO, "[%d]: PrintEvent:", __LINE__));
          PrintBuf (TempVal, preEvtLogBuf + IndexSize);
          if (TempVal)
            continue;
        }
        TempVal = GetTcgEvent2LogSize (preEvtLogBuf + IndexSize);
        HobData = BuildGuidHob (
                   &gTcgEvent2EntryHobGuid,
                   TempVal
                   );
        if (HobData) {
          (**PeiServices).CopyMem (HobData, (VOID*)(preEvtLogBuf + IndexSize), TempVal);
          DEBUG ((DEBUG_INFO, "[%d]: PrintEvent:", __LINE__));
          PrintBuf (TempVal, (UINT8* )HobData);
        }

      }
  } while (FALSE);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeimEntry (
  IN       EFI_PEI_FILE_HANDLE            FileHandle,
  IN CONST EFI_PEI_SERVICES               **PeiServices
  )
{
  EFI_STATUS                       Status;

  Status = PeiServicesRegisterForShadow (FileHandle);

  if (Status == EFI_ALREADY_STARTED) {
    Status = (**PeiServices).NotifyPpi (PeiServices, &mPspFtpmPpiCallback);
  }
  return Status;
}
