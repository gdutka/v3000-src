/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Guid/EventGroup.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/MpmLib.h>
#include <Library/MpmMboxLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Protocol/PciRootBridgeIo.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Pci.h>
#include <Library/MpmFuncLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/FchSpiAccessLib.h>
#include <Protocol/AcpiTable.h>
#include <Library/DxeServicesLib.h>
#include <Library/MpmDevLib.h>
#include <Protocol/PciIo.h>
#include <Library/ReportStatusCodeLib.h>
#include <AmdMpmAsf.h>

#include <Filecode.h>

#define FILECODE MPM_MPMDXE_MPMDXE_FILECODE
#define MMIO_MP0_C2PMSG_073_OFFSET 0x10A24UL
#define MMIO_MP0_C2PMSG_111_OFFSET 0x10ABCUL
#define AMD_MPM_SSDT_GUID \
  { 0x50502ff2, 0xfc9f, 0x407a, 0xb4, 0x08, 0xb6, 0x6d, 0x96, 0x2c, 0x4a, 0xde }

MPM_PRVIATE_HEAP_DATA_V1 *mMpmData = NULL;
/**
  This function gets the protocol interface from the given handle, and
  obtains its address space descriptors.

  @param[in] Handle          The PCI_ROOT_BRIDIGE_IO_PROTOCOL handle.
  @param[out] IoDev          Handle used to access configuration space of PCI device.
  @param[out] Descriptors    Points to the address space descriptors.

  @retval EFI_SUCCESS     The command completed successfully
**/
EFI_STATUS
PciGetProtocolAndResource (
  IN  EFI_HANDLE                            Handle,
  OUT EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL       **IoDev,
  OUT EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR     **Descriptors
  )
{
  EFI_STATUS  Status;

  //
  // Get inferface from protocol
  //
  Status = gBS->HandleProtocol (
                Handle,
                &gEfiPciRootBridgeIoProtocolGuid,
                (VOID**)IoDev
               );

  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Call Configuration() to get address space descriptors
  //
  Status = (*IoDev)->Configuration (*IoDev, (VOID**)Descriptors);
  if (Status == EFI_UNSUPPORTED) {
    *Descriptors = NULL;
    return EFI_SUCCESS;

  } else {
    return Status;
  }
}

/**
  This function get the next bus range of given address space descriptors.
  It also moves the pointer backward a node, to get prepared to be called
  again.

  @param[in, out] Descriptors Points to current position of a serial of address space
                              descriptors.
  @param[out] MinBus          The lower range of bus number.
  @param[out] MaxBus          The upper range of bus number.
  @param[out] IsEnd           Meet end of the serial of descriptors.

  @retval EFI_SUCCESS     The command completed successfully.
**/
EFI_STATUS
PciGetNextBusRange (
  IN OUT EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  **Descriptors,
  OUT    UINT16                             *MinBus,
  OUT    UINT16                             *MaxBus,
  OUT    BOOLEAN                            *IsEnd
  )
{
  *IsEnd = FALSE;

  //
  // When *Descriptors is NULL, Configuration() is not implemented, so assume
  // range is 0~PCI_MAX_BUS
  //
  if ((*Descriptors) == NULL) {
    *MinBus = 0;
    *MaxBus = PCI_MAX_BUS;
    return EFI_SUCCESS;
  }
  //
  // *Descriptors points to one or more address space descriptors, which
  // ends with a end tagged descriptor. Examine each of the descriptors,
  // if a bus typed one is found and its bus range covers bus, this handle
  // is the handle we are looking for.
  //

  while ((*Descriptors)->Desc != ACPI_END_TAG_DESCRIPTOR) {
    if ((*Descriptors)->ResType == ACPI_ADDRESS_SPACE_TYPE_BUS) {
      *MinBus = (UINT16) (*Descriptors)->AddrRangeMin;
      *MaxBus = (UINT16) (*Descriptors)->AddrRangeMax;
      (*Descriptors)++;
      return (EFI_SUCCESS);
    }

    (*Descriptors)++;
  }

  if ((*Descriptors)->Desc == ACPI_END_TAG_DESCRIPTOR) {
    *IsEnd = TRUE;
  }

  return EFI_SUCCESS;
}
//Debug print for dump PciInfo going to send to MPM
VOID
DumpPciInfo (
  PCI_INFO                          *PciInfo
  )
{
  UINT32 i;
  IDS_HDT_CONSOLE_PSP_TRACE ("NumberOfDevice: %d\n", PciInfo->NumberOfDevice);

  for (i = 0; i < PciInfo->NumberOfDevice; i++) {
    //Only print VID DID
    IDS_HDT_CONSOLE_PSP_TRACE ("Addr:0x%08x VID:0x%04X DID:0x%04X\n",PciInfo->PciDev[i].PciAddr, PciInfo->PciDev[i].Cfg.Device.Hdr.VendorId, PciInfo->PciDev[i].Cfg.Device.Hdr.DeviceId);
  }
}
#define MAX_PCI_RB_IO_HANDLES         8
#define MAX_PCI_DEV_SUPPORT           256
/*++

Routine Description:

  This function is invoked by gEfiPciEnumerationCompleteProtocolGuid.
  Collect all PCI information and report to MPM.
  To make sure running prior to KVM and Text, use the gEfiPciRootBridgeIoProtocolGuid

Arguments:

  Event   - The triggered event.
  Context - Context for this event.

Returns:

  None

--*/
VOID
MpmPciEnumerationCompleteCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                        Status;
  VOID                              *Interface;
  UINTN                             HandleBufSize;
  UINTN                             HandleCount;
  UINTN                             Index;
  EFI_HANDLE                        HandleBuf[MAX_PCI_RB_IO_HANDLES];
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *Descriptors;
  UINT16                            MinBus;
  UINT16                            MaxBus;
  BOOLEAN                           IsEnd;
  PCI_DEVICE_INDEPENDENT_REGION     PciHeader;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *IoDev;
  PCI_INFO                          *PciInfo;
  UINT16                            Bus;
  UINT16                            Device;
  UINT16                            Func;
  UINT64                            Address;

  //Initalize PciHeader to 0
  gBS->SetMem (&PciHeader, sizeof (PCI_DEVICE_INDEPENDENT_REGION), 0);
  //
  // Try to locate it because EfiCreateProtocolNotifyEvent will trigger it once when registration.
  // Just return if it is not found.
  //
  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
  HandleBufSize = sizeof (HandleBuf);

  Status = gBS->LocateHandle (
                ByProtocol,
                &gEfiPciRootBridgeIoProtocolGuid,
                NULL,
                &HandleBufSize,
                &HandleBuf[0]
                );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Allocate gEfiPciRootBridgeIoProtocolGuid fail Exit\n");
    return;
  }
  HandleCount = HandleBufSize / sizeof (EFI_HANDLE);
  IDS_HDT_CONSOLE_PSP_TRACE ("Found %d handle of gEfiPciRootBridgeIoProtocolGuid\n", HandleCount);
  //Allocate Pool to hold PciInfo structure
  PciInfo = NULL;
  PciInfo = AllocateZeroPool (sizeof (PCI_INFO) - sizeof (PCI_DEV) + (MAX_PCI_DEV_SUPPORT * sizeof (PCI_DEV)));
  if (PciInfo == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Memory Allocation Fail\n");
    return;
  }
  //Loop all handles, and find all device on it
  //Reference the shell command PCI implementation
  for (Index = 0; Index < HandleCount; Index++) {
    Status = PciGetProtocolAndResource (
              HandleBuf[Index],
              &IoDev,
              &Descriptors
              );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("gEfiPciRootBridgeIoProtocolGuid.Configuration return error\n", HandleCount);
      goto Done;
    }
    //
    // No document say it's impossible for a RootBridgeIo protocol handle
    // to have more than one address space descriptors, so find out every
    // bus range and for each of them do device enumeration.
    //
    while (TRUE) {
      Status = PciGetNextBusRange (&Descriptors, &MinBus, &MaxBus, &IsEnd);

      if (EFI_ERROR (Status)) {
        goto Done;
      }

      if (IsEnd) {
        break;
      }

      for (Bus = MinBus; Bus <= MaxBus; Bus++) {
        //
        // For each devices, enumerate all functions it contains
        //
        for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
          //
          // For each function, read its configuration space and print summary
          //
          for (Func = 0; Func <= PCI_MAX_FUNC; Func++) {
            Address = EFI_PCI_ADDRESS (Bus, Device, Func, 0);
            IoDev->Pci.Read (
                        IoDev,
                        EfiPciWidthUint16,
                        Address,
                        1,
                        &PciHeader.VendorId
                        );

            //
            // If VendorId = 0xffff, there does not exist a device at this
            // location. For each device, if there is any function on it,
            // there must be 1 function at Function 0. So if Func = 0, there
            // will be no more functions in the same device, so we can break
            // loop to deal with the next device.
            //
            if (PciHeader.VendorId == 0xffff && Func == 0) {
              break;
            }

            if (PciHeader.VendorId != 0xffff) {
              //Now we found a valid device
              IoDev->Pci.Read (
                          IoDev,
                          EfiPciWidthUint32,
                          Address,
                          sizeof (PCI_TYPE_GENERIC) / sizeof (UINT32),
                          &PciInfo->PciDev[PciInfo->NumberOfDevice].Cfg
                          );
              PciInfo->PciDev[PciInfo->NumberOfDevice].PciAddr = Address;
              PciInfo->NumberOfDevice++;
              //
              // If this is not a multi-function device, we can leave the loop
              // to deal with the next device.
              //
              if (Func == 0 && ((PciHeader.HeaderType & HEADER_TYPE_MULTI_FUNCTION) == 0x00)) {
                break;
              }
            }
          }
        }
      }
      //
      // If Descriptor is NULL, Configuration() returns EFI_UNSUPPRORED,
      // we assume the bus range is 0~PCI_MAX_BUS. After enumerated all
      // devices on all bus, we can leave loop.
      //
      if (Descriptors == NULL) {
        break;
      }
    }
  }
Done:
  BiosMpmCmdPcieMsg (PciInfo);
  DumpPciInfo (PciInfo);
  IDS_HDT_CONSOLE_PSP_TRACE ("MpmPciEnumerationCompleteCallBack Exit\n");
}

/**
 * @brief Load WIFI FW if wireless KVM or TCR pending bit set
 *
 * @return EFI_STATUS
 */
EFI_STATUS
MpmLoadWifiFw (
  VOID
  )
{
  EFI_STATUS         Status = EFI_SUCCESS;
  VOID               *TempBuf;
  UINT64             EntryAddress;
  UINT32             EntrySize;
  WLAN_FW_BUF        WlanFwBuf;

  if (mMpmData == NULL) {
    return EFI_UNSUPPORTED;
  }
  //If wireless KVM or TCR pending request detected
  //  -Allocate temporary memory
  //  -Copy WLAN FW from SPI.PSPDIR to temp memory
  //  -Allocate 64M DMA memory
  //  -Send command to PSP for FW decompress,Auth, and Load
  if ((mMpmData->WirelessKvmPending && (PcdGetBool (PcdMpmWirelessKvmEnable) == TRUE)) ||
      (mMpmData->WirelessTxrPending && (PcdGetBool (PcdMpmWirelessTcrEnable) == TRUE))) {
    TempBuf = AllocateAlignedPages (EFI_SIZE_TO_PAGES (MPM_WLAN_TEMP_BUF_SIZE), BASE_64KB);
    IDS_HDT_CONSOLE_PSP_TRACE ("TempBuf address = 0x%x\n", TempBuf);
    //Locate WLAN FW
    if (PSPEntryInfoV2  (MPM_WLAN_FW, &EntryAddress, &EntrySize)) {
      if (EntrySize <= MPM_WLAN_TEMP_BUF_SIZE) {
        //Copy WLAN FW from SPI.PSPDIR to temp memory
        FchSpiRomReadEx ((UINTN) EntryAddress, TempBuf, EntrySize);
        WlanFwBuf.TempDramAddrLo = (UINT32) ((UINT64) (UINTN) TempBuf & 0xFFFFFFFF);
        WlanFwBuf.TempDramAddrHi = (UINT32) ((UINT64) (UINTN) TempBuf >> 32);
        WlanFwBuf.TempDramSize = MPM_WLAN_TEMP_BUF_SIZE;
        WlanFwBuf.AllocatedDramAddrLo = (UINT32) ((UINT64) (UINTN) mMpmData->WirelessDmaBufAddr & 0xFFFFFFFF);
        WlanFwBuf.AllocatedDramAddrHi = (UINT32) ((UINT64) (UINTN) mMpmData->WirelessDmaBufAddr >> 32);
        WlanFwBuf.AllocatedDramSize = MPM_DMA_HPA_BUF_SIZE;
        Status = PspMboxBiosCmdLoadWlanFw (&WlanFwBuf);
        if (EFI_ERROR (Status)) {
          ReportStatusCode (EFI_ERROR_CODE, WLAN_FW_LOAD_FAILED);
          PcdSetBoolS (PcdMpmWirelessKvmEnable, FALSE);
          PcdSetBoolS (PcdMpmWirelessTcrEnable, FALSE);
        }
      } else {
        IDS_HDT_CONSOLE_PSP_TRACE ("Buffer too small: EntrySize:0x%x > RsvdSize 0x%x\n", EntrySize, MPM_WLAN_TEMP_BUF_SIZE);
        return EFI_BUFFER_TOO_SMALL;
      }
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Locate MPM_WLAN_FW Failed\n");
      return EFI_NOT_FOUND;
    }
  }
  return Status;
}

/**
 * @brief Update MPM bar address and PSP MMIO base address to
 * MPM SSDT ACPI table
 *
 * @return EFI_STATUS
 */
EFI_STATUS
PublishMpmSsdt ()
{
  EFI_STATUS                                Status;
  UINTN                                     Size;
  UINTN                                     TableList;
  EFI_ACPI_COMMON_HEADER                    *CurrentTable;
  INTN                                      Instance;
  EFI_GUID                                  AmdMpmSsdtGuid = AMD_MPM_SSDT_GUID;
  EFI_ACPI_TABLE_PROTOCOL                   *AcpiTableProtocol;
  UINT8                                     *MpmAcpiDataPtr;
  UINT32                                    *Memory32Fixed;
  UINT32                                    PspMmioBase;
  UINT32                                    MpmMmioBase = 0;

  MpmMmioBase = GetMpmBarAddr();
  if (MpmMmioBase == 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("MPM MMIO not initialized\n");
    return EFI_UNSUPPORTED;
  }

  if (GetPspMmioBase (&PspMmioBase) == FALSE) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (
                    &gEfiAcpiTableProtocolGuid,
                    NULL,
                    &AcpiTableProtocol
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Instance     = 0;
  CurrentTable = NULL;
  Status = GetSectionFromAnyFv (
            &AmdMpmSsdtGuid,
            EFI_SECTION_RAW,
            Instance,
            (VOID**)&CurrentTable,
            &Size
          );
  if (!EFI_ERROR (Status)) {
    if (((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId != SIGNATURE_64 ('M', 'P', 'M', '0', 0, 0, 0, 0)) {
      return EFI_NOT_FOUND;
    }
  } else {
    return EFI_NOT_FOUND;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("MPM SSDT table length = 0x%x\n", CurrentTable->Length);
  //Update _CRS Object with actual value
  for (MpmAcpiDataPtr = ((UINT8 *)CurrentTable + sizeof (EFI_ACPI_COMMON_HEADER));
       MpmAcpiDataPtr <= ((UINT8 *)CurrentTable + ((EFI_ACPI_COMMON_HEADER *)CurrentTable)->Length);
       MpmAcpiDataPtr++
       ) {
    Memory32Fixed = (UINT32 *)MpmAcpiDataPtr;
    switch (*Memory32Fixed) {
    case 0xAAAAAAAA:
      *Memory32Fixed = MpmMmioBase;
      IDS_HDT_CONSOLE_PSP_TRACE ("GetMpmBarAddr = 0x%x\n", *(UINT32*)Memory32Fixed);
      MpmAcpiDataPtr += (sizeof (UINT32) - 1);
      break;
    case 0xBBBBBBBB:
      *Memory32Fixed = PspMmioBase + MMIO_MP0_C2PMSG_073_OFFSET;
      IDS_HDT_CONSOLE_PSP_TRACE ("PSPMmIoBase = 0x%x\n", PspMmioBase);
      MpmAcpiDataPtr += (sizeof (UINT32) - 1);
      break;
    case 0xCCCCCCCC:
      *Memory32Fixed = PspMmioBase + MMIO_MP0_C2PMSG_111_OFFSET;
      IDS_HDT_CONSOLE_PSP_TRACE ("PSPMmIoBase = 0x%x\n", PspMmioBase);
      MpmAcpiDataPtr += (sizeof (UINT32) - 1);
      break;
    default:
      break;
    }
  }

  TableList = 0;
  Status = AcpiTableProtocol->InstallAcpiTable (
              AcpiTableProtocol,
              CurrentTable,
              Size,
              &TableList);
  if (!EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Install MPM SSDT Success.\n");
    FreePool(CurrentTable);
  }
  return Status;
}

/**
 * @brief OEMs support more than the one AIM-T WLAN vendor and need a way to know if the installed
 * device is supported so BIOS can enable/disable AIMT options.
 * Provide AGESA PcdMpmWiredNicDetected and PcdMpmWirelessNicDetected to allow OEM BIOS to know if the
 * installed device is supported
 * @return EFI_STATUS
 */
VOID
DetectMpmNicDevice (
  IN OUT BOOLEAN              *IsMpmWirelessNicDetected,
  IN OUT BOOLEAN              *IsMpmWiredNicDetected
  )
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  EFI_HANDLE           *PciIoHandles;
  UINTN                NoPciIoHandles;
  UINTN                Index;
  EFI_STATUS           Status;
  UINT8                NicDevNum;


  PciIoHandles = NULL;
  PciIo = NULL;

  IDS_HDT_CONSOLE_PSP_TRACE ("DetectMpmNicDevice\n");
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &NoPciIoHandles,
                  &PciIoHandles
                  );
  if (EFI_ERROR(Status)) {
    return;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("%x PciIoHandles for Scan\n", NoPciIoHandles);

  //Loop All PCI handles
  for (Index = 0; Index < NoPciIoHandles; Index++) {
    Status = gBS->HandleProtocol (
        PciIoHandles[Index],
        &gEfiPciIoProtocolGuid,
        &PciIo
        );
    if (EFI_ERROR (Status)) {
      continue;
    }
    //Search if current handle is MPM wireless or wired Lan devices supported
    for (NicDevNum = 0; NicDevNum < GetMpmDevProtocolNum (); NicDevNum++) {
      if (mMpmDevProtocols [NicDevNum]->IsSupported (PciIoHandles[Index])) {
        if (mMpmDevProtocols [NicDevNum]->DeviceType == MPM_NIC_WIRELESS) {
        //Wireless WLAN devcie is found, set the pcd to TRUE
        PcdSetBoolS (PcdMpmWirelessNicDetected, TRUE);
        *IsMpmWirelessNicDetected = TRUE;
        IDS_HDT_CONSOLE_PSP_TRACE ("PcdMpmWirelessNicDetected is set to TRUE\n");
        } else if (mMpmDevProtocols [NicDevNum]->DeviceType == MPM_NIC_WIRED) {
          //Wired LAN devcie is found, set the pcd to TRUE
          PcdSetBoolS (PcdMpmWiredNicDetected, TRUE);
          *IsMpmWiredNicDetected = TRUE;
          IDS_HDT_CONSOLE_PSP_TRACE ("PcdMpmWiredNicDetected is set to TRUE\n");
        }
        break;
      }
    }
    //Exit main loop
    if ((*IsMpmWirelessNicDetected == TRUE) && (*IsMpmWiredNicDetected == TRUE)) {
      break;
    }
  }
  return;
}

EFI_STATUS
MpmDxeDriverEntry (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS            EfiStatus;
  EFI_EVENT             Event;
  VOID                  *Registration;
  CHAR8                 *AgesaVerStr;
  VOID                  *DmaBuf;
  EFI_HANDLE            WlanFWloadHandle = NULL;
  BOOLEAN               IsWirelessDevDetected = FALSE;
  BOOLEAN               IsWiredDevDetected = FALSE;


  EfiStatus = EFI_SUCCESS;
  AGESA_TESTPOINT (TpMpmDxeEntry, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("MpmDxe Enter\n");

  // Check MPM Enable
  if (!IsMpmEnabled()) {
    return EFI_UNSUPPORTED;
  }

  if (PcdGetBool (PcdMpmWirelessDeviceDetection) == TRUE) {
    //No matter the MPM NIC device is found or not, we need continue to do the next steps,
    //Because the emulation platform doesn't have the real hardware NIC device
    DetectMpmNicDevice (&IsWirelessDevDetected, &IsWiredDevDetected);
    if (IsWirelessDevDetected == FALSE) {
      PcdSetBoolS (PcdMpmWirelessManageability, FALSE);
      PcdSetBoolS (PcdMpmWirelessKvmEnable, FALSE);
      PcdSetBoolS (PcdMpmWirelessTcrEnable, FALSE);
    }
    if (IsWiredDevDetected == FALSE) {
       PcdSetBoolS (PcdMpmWiredKvmEnable, FALSE);
    }
  }

  EfiStatus = LocateMpmPrivateData (&mMpmData);
  if (EFI_ERROR (EfiStatus)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate mMpmData\n");
    return EfiStatus;
  }
  DmaBuf = AllocateAlignedPages (EFI_SIZE_TO_PAGES (MPM_DMA_HPA_BUF_SIZE), BASE_64MB);
  IDS_HDT_CONSOLE_PSP_TRACE ("DmaBuf = 0x%x\n", DmaBuf);
  ZeroMem (DmaBuf, MPM_DMA_HPA_BUF_SIZE);
  mMpmData->WirelessDmaBufAddr = (UINT64) (UINTN) DmaBuf;
  EfiStatus = MpmLoadWifiFw ();
  if (!EFI_ERROR (EfiStatus)) {
    //Wireless TCR driver need depend on this to send command start text
    EfiStatus = gBS->InstallProtocolInterface (
                    &WlanFWloadHandle,
                    &gAmdMpmWifiFwloadedProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL
                    );
    if (EFI_ERROR (EfiStatus)) {
      return EfiStatus;
    }
  }

  //Send AGESA version to MPM
  AgesaVerStr = (CHAR8 *)PcdGetPtr (PcdAmdPackageString);
  if (AsciiStrCmp (AgesaVerStr, "AGESA!V9") == 0) {
    //Strip the string "AGESA!V9\0"
    while (*AgesaVerStr) {
      AgesaVerStr++;
    }
    AgesaVerStr++;
  }
  BiosMpmCmdAgesaVersion (AgesaVerStr);

  //Create for event for report PCI Information
  IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate PciEnumerationCompleteProtocol callback\n");
  Event = NULL;
  Event = EfiCreateProtocolNotifyEvent (
                      &gEfiPciEnumerationCompleteProtocolGuid,
                      TPL_NOTIFY,
                      MpmPciEnumerationCompleteCallBack,
                      NULL,
                      &Registration
                      );
  EfiStatus = PublishMpmSsdt ();
  IDS_HDT_CONSOLE_PSP_TRACE ("PublishMpmSsdt: %x\n", EfiStatus);
  AGESA_TESTPOINT (TpMpmDxeExit, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("MpmDxe Exit: %x\n", EfiStatus);
  return EfiStatus;
}


