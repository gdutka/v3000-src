/** @file
  Implementation of H2ONvmeInfo.

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>

#include <IndustryStandard/Nvme.h>
#include <IndustryStandard/Pci22.h>

#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/NvmExpressPassthru.h>
#include <Protocol/PciIo.h>
#include <Protocol/H2ONvmeInfo.h>             // InsydePlatformInfoPkg

#include <Guid/MdeModuleHii.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>
#include <Library/H2OCpLib.h>                 // InsydeModulePkg
#include <Library/NvmeInfoLib.h>              // InsydePlatformInfoPkg

#include "NvmeInfoDxe.h"

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

STATIC NVME_INFO_DATA  mNvmeInfoPrivateData = {
  NVME_INFO_SIGNATURE,
  {         // NvmeInfo
    CollectControllers,
    IdentifyController,
    IdentifyNamespace,
    GetSmartHealthInfo,
  }
};

VOID
DebugHexDumpBuffer (
  IN UINTN       ErrorLevel,
  IN CONST VOID  *Buffer,
  IN UINTN       Size
  )
{
  UINTN        ByteOffset;
  UINTN        PrintOffsetStart;
  UINTN        CharOffset;
  CHAR8        HexString[49] = { '\0' };
  UINT8        Byte;
  CONST CHAR8  Digits[] = "0123456789abcdef";
  CHAR8        AsciiString[17] = { '\0' };

  if ((!DebugPrintEnabled ()) || (!DebugPrintLevelEnabled (ErrorLevel))) {
    return;
  }

  if (Buffer == NULL) {
    return;
  }

  ByteOffset = 0;
  while (ByteOffset < Size) {
    PrintOffsetStart = ByteOffset;
    //
    // We're not going to depend on SetMem() from BaseMemoryLib.
    //
    for (CharOffset = 0; CharOffset < 48; CharOffset++) {
      HexString[CharOffset] = ' ';
    }
    while (TRUE) {
      CharOffset = (ByteOffset & 0xF) * 3 +
                   (((ByteOffset & 0x8) != 0) ? 1 : 0);
      Byte = ((UINT8 *) Buffer)[ByteOffset];

      HexString[CharOffset] = Digits[Byte >> 4];
      HexString[CharOffset + 1] = Digits[Byte & 0xF];

      AsciiString[ByteOffset & 0xF] = ((Byte >= 0x20 && Byte <= 0x7E) ?
                                  (     CHAR8) Byte : '.');
      AsciiString[(ByteOffset & 0xF) + 1] = '\0';

      ByteOffset++;
      if ((ByteOffset & 0xF) == 0) {
        break;
      }
      if (ByteOffset >= Size) {
        break;
      }
    }
    DEBUG ((
      ErrorLevel,
      "%08x: %a  |%a|\n",
      (unsigned int) PrintOffsetStart,
      HexString,
      AsciiString
      ));
  }
}

EFI_STATUS
EFIAPI
IdentifyController (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN UINT32                              NamespaceId,
  OUT NVME_ADMIN_CONTROLLER_DATA         **IdentifyControllerData
  )
{
  UINT8                                     *TransferBuffer;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;
  EFI_STATUS                                Status;

  if ((NvmePassThru == NULL) || (IdentifyControllerData == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The NvmePassThru/IdentifyControllerData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *IdentifyControllerData = NULL;
  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command,       sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion,    sizeof (EFI_NVM_EXPRESS_COMPLETION));

  //
  // TODO: TransferBuffer needs to be aligned by the IoAlign field according to
  // UEFI spec of the pass thru protocol.
  //
  TransferBuffer = AllocateZeroPool (sizeof (NVME_ADMIN_CONTROLLER_DATA));
  if (TransferBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;
  CommandPacket.TransferBuffer = TransferBuffer;
  CommandPacket.TransferLength = sizeof (NVME_ADMIN_CONTROLLER_DATA);
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;

  Command.Cdw0.Opcode = NVME_ADMIN_IDENTIFY_CMD;
  //
  // According to Nvm Express 1.1 spec Figure 38, When not used, the field shall be cleared to 0h.
  // For the Identify command, the Namespace Identifier is only used for the Namespace data structure.
  //
  Command.Nsid        = NamespaceId;

  //
  // Set bit 0 (Cns bit) to 1 to identify a controller
  //
  Command.Cdw10       = (IdentifyControllerCns & 0xFF);
  Command.Flags       = CDW10_VALID;

  Status = NvmePassThru->PassThru (
                           NvmePassThru,
                           Command.Nsid,
                           &CommandPacket,
                           NULL
                           );
  if (EFI_ERROR (Status)) {
    FreePool (TransferBuffer);
    return Status;
  }

  *IdentifyControllerData = (NVME_ADMIN_CONTROLLER_DATA *) TransferBuffer;

  return Status;
}

/**
  Collect NVMe controller information

**/
EFI_STATUS
EFIAPI
CollectControllers (
  OUT UINTN                           *NumOfControllers,
  OUT H2O_NVME_INFO_CONTROLLER_ENTRY  **ControllerEntries
  )
{
  EFI_STATUS                          ReturnStatus;
  UINTN                               NumOfHandles;
  EFI_HANDLE                          *HandleBuffer;
  UINTN                               EntryCount;
  UINTN                               HandleIndex;
  EFI_STATUS                          Status;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru;
  UINT32                              NamespaceId;
  UINTN                               AllocateSize;
  UINTN                               EntryIndex;
  H2O_NVME_INFO_CONTROLLER_ENTRY      *RaidControllerEntry;

  if (NumOfControllers == NULL || ControllerEntries == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *NumOfControllers  = 0;
  *ControllerEntries = NULL;

  ReturnStatus = gBS->LocateHandleBuffer (
                        ByProtocol,
                        &gEfiNvmExpressPassThruProtocolGuid,
                        NULL, // SearchKey
                        &NumOfHandles,
                        &HandleBuffer
                        );
  if (EFI_ERROR (ReturnStatus)) {
    DEBUG ((
      ((ReturnStatus == EFI_NOT_FOUND) ? EFI_D_INFO : EFI_D_ERROR),
      "%a: Locate EfiNvmExpressPassThru: %r\n",
      gEfiCallerBaseName,
      ReturnStatus
      ));
    goto LocateHandleFail;
  }
  DEBUG ((
    EFI_D_VERBOSE,
    "%a: Number of handles = %lu\n",
    gEfiCallerBaseName,
    (UINT64) NumOfHandles
    ));

  EntryCount = NumOfHandles;
  //
  // Find NVMe controllers that are hidden under a Intel VMD (Volume
  // Management Device). These NVMe controllers won't be accessible through
  // PCI I/O (and thus they have no EFI handles) and they are identified
  // through the "Identify Controller" command with a valid (non-zero)
  // Namespace ID.
  //
  for (HandleIndex = 0; HandleIndex < NumOfHandles; HandleIndex++) {
    if (!IsNvmeRaidController (HandleBuffer[HandleIndex])) {
      continue;
    }

    Status = gBS->OpenProtocol (
                    HandleBuffer[HandleIndex],
                    &gEfiNvmExpressPassThruProtocolGuid,
                    (VOID **) &NvmePassThru,
                    gImageHandle, // AgentHandle
                    NULL,         // ControllerHandle
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );
    if (EFI_ERROR (Status)) {
      //
      // Hotplug is not supported.
      //
      DEBUG ((
        EFI_D_ERROR,
        "Handle %p: Open EfiNvmExpressPassThru: %r\n",
        HandleBuffer[HandleIndex],
        Status
        ));
      continue;
    }

    NamespaceId = 0xFFFFFFFF;
    while (TRUE) {
      Status = NvmePassThru->GetNextNamespace (NvmePassThru, &NamespaceId);
      if (EFI_ERROR (Status)) {
        break;
      }
      EntryCount++;
      if (EntryCount > MAX_UINTN / sizeof (H2O_NVME_INFO_CONTROLLER_ENTRY)) {
        //
        // Guard against UINTN arithmetic overflow
        //
        ReturnStatus = EFI_OUT_OF_RESOURCES;
        goto AllocateFail;
      }
    }
  }

  DEBUG ((
    EFI_D_VERBOSE,
    "%a: Number of entries to allocate = %lu\n",
    gEfiCallerBaseName,
    (UINT64) EntryCount
    ));
  AllocateSize = (EntryCount * sizeof (H2O_NVME_INFO_CONTROLLER_ENTRY));
  *ControllerEntries = AllocateZeroPool (AllocateSize);
  if (*ControllerEntries == NULL) {
    ReturnStatus = EFI_OUT_OF_RESOURCES;
    goto AllocateFail;
  }

  *NumOfControllers = EntryCount;

  for (HandleIndex = 0, EntryIndex = 0;
      HandleIndex < NumOfHandles && EntryIndex < EntryCount;
      HandleIndex++)
  {
    (*ControllerEntries)[EntryIndex].Handle = HandleBuffer[HandleIndex];
    ASSERT ((*ControllerEntries)[EntryIndex].NamespaceId == 0);

    RaidControllerEntry = NULL;
    if (IsNvmeRaidController (HandleBuffer[HandleIndex])) {
      (*ControllerEntries)[EntryIndex].IsRaidController = TRUE;
      RaidControllerEntry = &(*ControllerEntries)[EntryIndex];
      EntryIndex++;
    }

    Status = gBS->OpenProtocol (
                    HandleBuffer[HandleIndex],
                    &gEfiNvmExpressPassThruProtocolGuid,
                    (VOID **) &NvmePassThru,
                    gImageHandle, // AgentHandle
                    NULL,         // ControllerHandle
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );
    if (EFI_ERROR (Status)) {
      //
      // Hotplug is not supported.
      //
      DEBUG ((
        EFI_D_ERROR,
        "Handle %p: Open EfiNvmExpressPassThru: %r\n",
        HandleBuffer[HandleIndex],
        Status
        ));
      continue;
    }

    //
    // Count the number of valid namespaces for each controller.
    //
    NamespaceId = 0xFFFFFFFF;
    while (TRUE) {
      Status = NvmePassThru->GetNextNamespace (NvmePassThru, &NamespaceId);
      if (EFI_ERROR (Status)) {
        break;
      }

      if (RaidControllerEntry == NULL) {
        (*ControllerEntries)[EntryIndex].NumOfValidNamespaces++;
        continue;
      }

      RaidControllerEntry->NumOfValidNamespaces++;
      if (EntryIndex >= EntryCount) {
        continue;
      }
      (*ControllerEntries)[EntryIndex].Handle = HandleBuffer[HandleIndex];
      (*ControllerEntries)[EntryIndex].NamespaceId = NamespaceId;
      (*ControllerEntries)[EntryIndex].NumOfValidNamespaces = 1;
      EntryIndex++;
    }

    if (RaidControllerEntry != NULL) {
      DEBUG ((
        EFI_D_VERBOSE,
        "Handle %p: RAID controller; number of valid namespaces = %u\n",
        RaidControllerEntry->Handle,
        (unsigned int) RaidControllerEntry->NumOfValidNamespaces
        ));
    } else {
      DEBUG ((
        EFI_D_VERBOSE,
        "Handle %p: Number of valid namespaces = %u\n",
        (*ControllerEntries)[EntryIndex].Handle,
        (unsigned int) (*ControllerEntries)[EntryIndex].NumOfValidNamespaces
        ));
    }

    if (RaidControllerEntry == NULL) {
      EntryIndex++;
    }
  }

  if (EntryIndex > EntryCount) {
    //
    // No assert here since this could happen with external factors.
    //
    DEBUG ((
      EFI_D_WARN,
      "%a: Controller list returned is incomplete.\n",
      gEfiCallerBaseName
      ));
  }

AllocateFail:
  FreePool (HandleBuffer);
LocateHandleFail:
  return ReturnStatus;
}

EFI_STATUS
EFIAPI
IdentifyNamespace (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN UINT32                              NamespaceId,
  OUT NVME_ADMIN_NAMESPACE_DATA          **IdentifyNamespaceData
  )
{
  UINT8                                     *TransferBuffer;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;
  EFI_STATUS                                Status;

  if ((NvmePassThru == NULL) || (IdentifyNamespaceData == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The NvmePassThru/IdentifyNamespaceData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *IdentifyNamespaceData = NULL;
  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command,       sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion,    sizeof (EFI_NVM_EXPRESS_COMPLETION));

  //
  // TODO: TransferBuffer needs to be aligned by the IoAlign field according to
  // UEFI spec of the pass thru protocol.
  //
  TransferBuffer = AllocateZeroPool (sizeof (NVME_ADMIN_NAMESPACE_DATA));
  if (TransferBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;
  CommandPacket.TransferBuffer = TransferBuffer;
  CommandPacket.TransferLength = sizeof (NVME_ADMIN_NAMESPACE_DATA);
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;

  Command.Cdw0.Opcode = NVME_ADMIN_IDENTIFY_CMD;

  Command.Nsid        = NamespaceId;

  Command.Cdw10       = (IdentifyNamespaceCns & 0xFF);
  Command.Flags       = CDW10_VALID;

  Status = NvmePassThru->PassThru (
                           NvmePassThru,
                           Command.Nsid,
                           &CommandPacket,
                           NULL
                           );
  if (EFI_ERROR (Status)) {
    FreePool (TransferBuffer);
    return Status;
  }

  *IdentifyNamespaceData = (NVME_ADMIN_NAMESPACE_DATA *) TransferBuffer;

  return Status;
}

/**
  Check the information of S.M.A.R.T from driver handle

  @param[in]  This            Points to the H2O_NVME_INFO_PROTOCOL.
  @param[in]  HandleIndex     The Number of Handle.
  @param[out] SmartData       Data of S.M.A.R.T.

  @retval EFI_SUCCESS                 Get the controller name success
  @retval EFI_INVALID_PARAMETER       The arguments are invalid
  @retval EFI_UNSUPPORTED             This is not a pci controller handle

**/
EFI_STATUS
EFIAPI
GetSmartHealthInfo (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN UINT32                              NamespaceId,
  OUT NVME_SMART_HEALTH_INFO_LOG         **SmartHealthInfo
  )
{
  UINT8                                     *TransferBuffer;
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                   Command;
  EFI_NVM_EXPRESS_COMPLETION                Completion;
  EFI_STATUS                                Status;

  if ((NvmePassThru == NULL) || (SmartHealthInfo == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The NvmePassThru/SmartHealthInfo is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *SmartHealthInfo = NULL;
  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command,       sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion,    sizeof (EFI_NVM_EXPRESS_COMPLETION));

  if (NamespaceId == 0x0) {
    //
    // For requesting the controller log page, NVMe specification recommends
    // namespace ID of FFFFFFFFh rather than 0h, "for compatibility with
    // implementations compliant with NVM Express Base Specification revision
    // 1.4 and earlier".
    //
    NamespaceId = 0xFFFFFFFF;
  }

  //
  // TODO: TransferBuffer needs to be aligned by the IoAlign field according to
  // UEFI spec of the pass thru protocol.
  //
  TransferBuffer = AllocateZeroPool (sizeof (NVME_SMART_HEALTH_INFO_LOG));
  if (TransferBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;
  CommandPacket.TransferBuffer = TransferBuffer;
  CommandPacket.TransferLength = sizeof (NVME_SMART_HEALTH_INFO_LOG);
  CommandPacket.CommandTimeout = NVME_GENERIC_TIMEOUT;
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;

  Command.Cdw0.Opcode = NVME_ADMIN_GET_LOG_PAGE_CMD;
  //
  // Quote from NVMe specification revision 2.0
  // "There is no namespace specific information defined in the SMART / Health
  // log page in this revision of the specification, thus the controller log
  // page and namespaces specific log page contain identical information."
  //
  Command.Nsid        = NamespaceId;

  Command.Cdw10       = ((((sizeof (NVME_SMART_HEALTH_INFO_LOG) /
                          sizeof (UINT32)) - 1) << 16) | LID_SMART_INFO);
  Command.Flags       = CDW10_VALID;

  Status = NvmePassThru->PassThru (
                           NvmePassThru,
                           Command.Nsid,
                           &CommandPacket,
                           NULL
                           );
  if (EFI_ERROR (Status)) {
    FreePool (TransferBuffer);
    return Status;
  }

  *SmartHealthInfo = (NVME_SMART_HEALTH_INFO_LOG *) TransferBuffer;

  return Status;
}

/**
  Callback function for check point gH2OBdsCpConnectAllAfterGuid.

  This function will build private data for H2O Nvme Drive Info Protocol and install it.

  @param[in]  Event   A pointer to the Event that triggered the callback.
  @param[in]  Handle  Checkpoint handle.

**/
VOID
EFIAPI
H2OBdsCpConnectAllAfterCallback (
  IN  EFI_EVENT       Event,
  IN  H2O_CP_HANDLE   Handle
  )
{
  EFI_HANDLE  ImageHandle;
  EFI_STATUS  Status;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  H2OCpUnregisterHandler (Handle);

  ImageHandle = gImageHandle;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gH2ONvmeInfoProtocolGuid,
                  &mNvmeInfoPrivateData.NvmeInfo,
                  (EFI_GUID *) NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Fail to InstallMultipleProtocolInterfaces for Handle(%08x), Status = %r\n",
      __FUNCTION__,
      ImageHandle,
      Status
      ));
    return;
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}

/**
  Entry point of this driver.

  @param[in] ImageHandle  Image handle this driver.
  @param[in] SystemTable  Pointer to SystemTable.

**/
EFI_STATUS
EFIAPI
NvmeInfoDxeEntry (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                    Status;
  H2O_NVME_INFO_PROTOCOL        *NvmeInfo;
  H2O_CP_HANDLE                 H2OBdsCpConnectAllAfterHandle;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  Status = gBS->LocateProtocol (
                  &gH2ONvmeInfoProtocolGuid,
                  NULL,
                  (VOID **) &NvmeInfo
                  );
  if (!EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }

  //
  // Collect storage devices information after connect all
  //
  if (FeaturePcdGet (PcdH2OBdsCpConnectAllAfterSupported)) {
    Status = H2OCpRegisterHandler (
                &gH2OBdsCpConnectAllAfterGuid,
                H2OBdsCpConnectAllAfterCallback,
                H2O_CP_MEDIUM,
                &H2OBdsCpConnectAllAfterHandle
                );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a() - Fail to register checkpoint: %g, Status: %r\n", __FUNCTION__, &gH2OBdsCpConnectAllAfterGuid, Status));
      return Status;
    }
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));

  return EFI_SUCCESS;
}
