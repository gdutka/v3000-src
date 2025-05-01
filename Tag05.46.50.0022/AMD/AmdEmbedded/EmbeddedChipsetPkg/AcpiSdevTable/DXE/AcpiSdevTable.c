#include <Uefi.h>
#include <IndustryStandard/Acpi.h>    
#include "AcpiSdevTable.h"
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/ChipsetConfigLib.h>
#include <Library/MemoryAllocationLib.h>
#include <ChipsetSetupConfig.h>
#include <Library/PciLib.h>

#define ACPI_XHCI_SECURE_DEVICE_IR_CAM_PATH   "\\_SB.PCI0.GP19.XHC2.RHUB.PRT1.CAM0"
#define ACPI_XHCI_SECURE_DEVICE_FHD_CAM_PATH  "\\_SB.PCI0.GP19.XHC2.RHUB.PRT1.CAM1"
#define MAX_DEV_NUM 32
#define MAX_FUN_NUM 8
#define A0_USB2_CONTROLLER_DIDVID  0x161C1022
#define B0_USB2_CONTROLLER_DIDVID  0x161F1022

CHAR8 STATIC AcpiDeviceIrCamObj[] = ACPI_XHCI_SECURE_DEVICE_IR_CAM_PATH;
CHAR8 STATIC AcpiDeviceFhdCamObj[] = ACPI_XHCI_SECURE_DEVICE_FHD_CAM_PATH;

CONST ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR gXhciIrCamera = {
  0,
  0,       //Length: Updated after
  2,       //Revision
  1,       //Attributes
  1,       //RootPortNumber
  0x0408,  //PID
  0x5255,  //DID
  0x7,     //UsbRevision
  0x2,     //InterfaceNumber
  0xE,     //Class
  0x3,     //Subclass
  0,       //Protocol
  0x34,    //AcpiPathStringOffset
  0,       //AcpiPathStringLength
 {0x94, 0x61, 0x00, 0xDB, 0x53, 0x13, 0x92, 0x2D, 0x7E, 0x91, 0x82, 0x2F, 0xEB, 0xD5, 0x83, 0x96,
  0x82, 0xD0, 0x4F, 0x57, 0xC3, 0xAD, 0x76, 0x74, 0x05, 0x7F, 0x25, 0x9E, 0xC9, 0x9C, 0xCA, 0xAF},
};

CONST ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR gXhciFhdCamera = {
  0,
  0,       //Length: Updated after
  2,       //Revision
  1,       //Attributes
  1,       //RootPortNumber
  0x0408,  //PID
  0x5255,  //DID
  0x7,     //UsbRevision
  0x0,     //InterfaceNumber
  0xE,     //Class
  0x3,     //Subclass
  0,       //Protocol
  0x34,    //AcpiPathStringOffset
  0,       //AcpiPathStringLength
 {0x94, 0x61, 0x00, 0xDB, 0x53, 0x13, 0x92, 0x2D, 0x7E, 0x91, 0x82, 0x2F, 0xEB, 0xD5, 0x83, 0x96,
  0x82, 0xD0, 0x4F, 0x57, 0xC3, 0xAD, 0x76, 0x74, 0x05, 0x7F, 0x25, 0x9E, 0xC9, 0x9C, 0xCA, 0xAF},
};

CONST ACPI_SDEV_XHCI_VENDOR_SPECIFIC_INFO gXhciSpecificInfo = {
  0x80,
  sizeof(ACPI_SDEV_XHCI_VENDOR_SPECIFIC_INFO),
  {0, 0},
  0xFF,
  {0, 0, 0, 0},
};

VOID
DumpTable (
  UINT8 *Address,
  UINTN Size
)
{
  UINTN Index;

  for (Index = 0; Index < Size; Index++) {
    if((Index % 0x10 == 0) && (Index != 0) ) {
      DEBUG ((EFI_D_ERROR, "\n", Address[Index]));
    }
    DEBUG ((EFI_D_ERROR, "0x%02x  ", Address[Index]));
  }
  DEBUG ((EFI_D_ERROR, "\n"));
}

VOID
AddPcieEntry (
  UINT8  *CurrentPtr,
  UINT8  Bus,
  UINT8  Dev,
  UINT8  Func
)
{
  EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE   *SdevPcieEndPoint;
  ACPI_SDEV_PCIE_EP_OBJECT                           *AcpiPciPath;

  DEBUG ((EFI_D_ERROR, "[ACPI SDEV] AddPcieEntry PCIE_ENDPOINT_DEVICE Current:0x%x\n", (UINTN)CurrentPtr));

  SdevPcieEndPoint = (EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE *)CurrentPtr;
  SdevPcieEndPoint->Type = 1;
  SdevPcieEndPoint->Flags = 0;
  SdevPcieEndPoint->PciSegmentNumber = 0;
  SdevPcieEndPoint->StartBusNumber = Bus;
  SdevPcieEndPoint->PciPathOffset = sizeof(EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE);
  SdevPcieEndPoint->PciPathLength = sizeof(ACPI_SDEV_PCIE_EP_OBJECT);
  SdevPcieEndPoint->VendorSpecificDataOffset = SdevPcieEndPoint->PciPathOffset + SdevPcieEndPoint->PciPathLength;
  SdevPcieEndPoint->VendorSpecificDataLength = 0;
  AcpiPciPath = (ACPI_SDEV_PCIE_EP_OBJECT *)(CurrentPtr + SdevPcieEndPoint->PciPathOffset);
  AcpiPciPath->PciDevice = Dev;
  AcpiPciPath->PciFunction = Func;
}

/**
 To update the PCIe Endpoint device structure.

 @param [in]   CurrentPtr: The latest point of SDEV table to add PCIe ep.
 @param [in]   Bus: Bridge bus number
 @param [in]   Dev: Bridge device number
 @param [in]   Func: Bridge function number
 @param [in]   NextDev: Child device number
 @param [in]   NextFunc: Child function number

**/
VOID
AddPcieEntry2 (
  UINT8  *CurrentPtr,
  UINT8  Bus,
  UINT8  Dev,
  UINT8  Func,
  UINT8  NextDev,
  UINT8  NextFunc
)
{
  EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE   *SdevPcieEndPoint;
  ACPI_SDEV_PCIE_EP_OBJECT                           *AcpiPciPath;

  DEBUG ((EFI_D_ERROR, "[ACPI SDEV] AddPcieEntry2 PCIE_ENDPOINT_DEVICE Current:0x%x\n", (UINTN)CurrentPtr));
  //EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE
  SdevPcieEndPoint = (EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE *)CurrentPtr;
  SdevPcieEndPoint->Type = EFI_ACPI_6_2_SDEV_TYPE_PCIE_ENDPOINT_DEVICE;
  SdevPcieEndPoint->Flags = 1;
  SdevPcieEndPoint->PciSegmentNumber = 0;
  SdevPcieEndPoint->StartBusNumber = Bus;
  SdevPcieEndPoint->PciPathOffset = sizeof(EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE);
  SdevPcieEndPoint->PciPathLength = 2 * sizeof(ACPI_SDEV_PCIE_EP_OBJECT);
  SdevPcieEndPoint->VendorSpecificDataOffset = SdevPcieEndPoint->PciPathOffset + SdevPcieEndPoint->PciPathLength;
  SdevPcieEndPoint->VendorSpecificDataLength = 0;
  AcpiPciPath = (ACPI_SDEV_PCIE_EP_OBJECT *)(CurrentPtr + SdevPcieEndPoint->PciPathOffset);
  AcpiPciPath->PciDevice = Dev;
  AcpiPciPath->PciFunction = Func;
  AcpiPciPath++;
  AcpiPciPath->PciDevice = NextDev;
  AcpiPciPath->PciFunction = NextFunc;
}

/**

 Get USB2 Controller Bus/Dev/Fun number

 @return Status code

**/

EFI_STATUS
EFIAPI
GetUsb2ControllerBusDevFun (
  IN OUT UINT8 *Bus,
  IN OUT UINT8 *Device,
  IN OUT UINT8 *Func
)
{
  UINT8   SecBus;
  UINT8   SubBus;
  UINT32  DidVid;

  SecBus = PciRead8 (PCI_LIB_ADDRESS (*(UINT8*)Bus, *(UINT8*)Device, *(UINT8 *)Func, 0x19));
  SubBus = PciRead8 (PCI_LIB_ADDRESS (*(UINT8*)Bus, *(UINT8*)Device, *(UINT8*)Func, 0x1A));
  DEBUG ((DEBUG_INFO, "Get SecBus number = %d, SubBus = %d\n", SecBus, SubBus));
  for (UINT8 i  = SecBus; i <= SubBus; i++) {
    for (UINT8 j = 0; j < MAX_DEV_NUM; j ++) {
      for (UINT8 k = 0; k < MAX_FUN_NUM; k ++) {
        DidVid = PciRead32 (PCI_LIB_ADDRESS (i, j, k, 0x00));
        if ((DidVid == A0_USB2_CONTROLLER_DIDVID) || (DidVid == B0_USB2_CONTROLLER_DIDVID)) {
          *(UINT8*)Bus = i;
          *(UINT8*)Device = j;
          *(UINT8*)Func = k;
          DEBUG ((DEBUG_INFO, "Found SecureBIO device, Bus = %d, Dev = %d, Fun = %d, DidVid = 0x%x\n", *(UINT8*)Bus, *(UINT8*)Device, *(UINT8*)Func, DidVid));
          return EFI_SUCCESS;
        }
      }
    }
  }
  DEBUG ((DEBUG_INFO, "Not found SecureBIO device.\n"));
  return EFI_NOT_FOUND;
}

/**
 
 Intall ACPI SDEV table for secure devices

 @return Status code

**/
EFI_STATUS
EFIAPI
AcpiSdevInstall (
  EFI_ACPI_TABLE_PROTOCOL                            *AcpiTableProtocol
  )
{
  EFI_STATUS                                         Status;
  VOID                                               *Table;
  UINT8                                              *Ptr;
  EFI_ACPI_6_2_SECURE_DEVICES_TABLE_HEADER           *SdevTableHeader;
  UINT32                                             SdevTableSize;
  UINTN                                              TableList;
  ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR            *XhciSecureDeviceDesc;
  EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE   *SdevPcieEndPoint;
  EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE  *SdevAcpiObj;
  UINT8                                              Bus;
  UINT8                                              Device;
  UINT8                                              Func;

  DEBUG ((EFI_D_ERROR, "[ACPI SDEV] Entry...\n"));
  //----------------------------------------------------------------------------
  // EFI_ACPI_6_2_SECURE_DEVICES_TABLE_HEADER
  // EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE
  // ACPI_SDEV_PCIE_EP_OBJECT
  // ACPI_SDEV_PCIE_EP_OBJECT
  // ACPI_SDEV_XHCI_VENDOR_SPECIFIC_INFO
  // ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR - IR CAM
  // ACPI_XHCI_SECURE_DEVICE_STRING
  // EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE
  // ACPI_XHCI_SECURE_DEVICE_STRING
  // ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR - FHD CAM
  // ACPI_XHCI_SECURE_DEVICE_STRING
  // EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE
  // ACPI_XHCI_SECURE_DEVICE_STRING
  //----------------------------------------------------------------------------
  SdevTableSize = sizeof (EFI_ACPI_6_2_SECURE_DEVICES_TABLE_HEADER) +\
                  sizeof (EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE) + (2 * sizeof(ACPI_SDEV_PCIE_EP_OBJECT)) +\
                  sizeof (ACPI_SDEV_XHCI_VENDOR_SPECIFIC_INFO) +\
                  sizeof (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR) + sizeof (AcpiDeviceIrCamObj) + sizeof (EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE) + sizeof (AcpiDeviceIrCamObj) + \
                  sizeof (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR) + sizeof (AcpiDeviceFhdCamObj) + sizeof (EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE) + sizeof (AcpiDeviceFhdCamObj);

  Table = AllocateZeroPool (SdevTableSize);
  if (Table == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((EFI_D_ERROR, "[ACPI SDEV] AllocatePool SdevTable failed: (%r) \n", Status));
    return Status;
  }
  DEBUG ((EFI_D_ERROR, "[ACPI SDEV] SdevTableAdr:0x%x SdevTableSize: 0x%x, AcpiDeviceIrCamObj size: 0x%x\n", (UINTN)Table, SdevTableSize, sizeof (AcpiDeviceIrCamObj)));

  Ptr = (UINT8 *) Table;

  // EFI_ACPI_6_2_SECURE_DEVICES_TABLE_HEADER
  SdevTableHeader = (EFI_ACPI_6_2_SECURE_DEVICES_TABLE_HEADER *) Ptr;
  SdevTableHeader->Header.Signature = EFI_ACPI_6_2_SECURE_DEVICES_TABLE_SIGNATURE;
  SdevTableHeader->Header.Length = SdevTableSize;
  SdevTableHeader->Header.Revision = EFI_ACPI_6_2_SECURE_DEVICES_TABLE_REVISION;
  SdevTableHeader->Header.OemId[0] = 'A';
  SdevTableHeader->Header.OemId[1] = 'M';
  SdevTableHeader->Header.OemId[2] = 'D';
  SdevTableHeader->Header.OemTableId = SIGNATURE_64 ('S', 'd', 'e', 'v', 'T', 'b', 'l', 'e');
  SdevTableHeader->Header.OemRevision = 1;
  SdevTableHeader->Header.CreatorId = SIGNATURE_32('A','C','P','I');
  SdevTableHeader->Header.CreatorRevision = 0x00000002;
  Ptr = Ptr + sizeof(EFI_ACPI_6_2_SECURE_DEVICES_TABLE_HEADER);

  //EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE
  DEBUG ((EFI_D_ERROR, "[ACPI SDEV] PCIE_ENDPOINT_DEVICE Current:0x%x\n", (UINTN)Ptr));

  // Xhci card under GP19.
  Bus = 0x0;
  Device = 0x8;
  Func = 0x3;
  Status = GetUsb2ControllerBusDevFun (&Bus, &Device, &Func);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "GetUsb2ControllerBusDevFun failed\n"));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "SecureBIO Device Bus = %d, Dev = %d, Func = %d\n", Bus, Device, Func));
  AddPcieEntry2(Ptr, Bus, Device, Func, 0, 0);
  
  SdevPcieEndPoint = (EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE *)Ptr;
  SdevPcieEndPoint->Length = sizeof (EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE) + (2 * sizeof(ACPI_SDEV_PCIE_EP_OBJECT)) +\
                             sizeof (ACPI_SDEV_XHCI_VENDOR_SPECIFIC_INFO) +\
                             sizeof (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR) + sizeof (AcpiDeviceIrCamObj) +\
                             sizeof (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR) + sizeof (AcpiDeviceFhdCamObj);

  SdevPcieEndPoint->VendorSpecificDataOffset = SdevPcieEndPoint->PciPathOffset + SdevPcieEndPoint->PciPathLength;
  SdevPcieEndPoint->VendorSpecificDataLength = sizeof (ACPI_SDEV_XHCI_VENDOR_SPECIFIC_INFO) + sizeof (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR) + sizeof (AcpiDeviceIrCamObj) +\
                                               sizeof (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR) + sizeof (AcpiDeviceFhdCamObj);
  Ptr += (sizeof (EFI_ACPI_6_2_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE) + (2 * sizeof(ACPI_SDEV_PCIE_EP_OBJECT)));

  // ACPI_SDEV_XHCI_VENDOR_SPECIFIC_INFO
  gBS->CopyMem ((VOID *)Ptr, (VOID *)&gXhciSpecificInfo, sizeof (ACPI_SDEV_XHCI_VENDOR_SPECIFIC_INFO));
  Ptr += sizeof (ACPI_SDEV_XHCI_VENDOR_SPECIFIC_INFO);

  DEBUG ((EFI_D_ERROR, "[ACPI SDEV] Create for IR camera device descriptor: 0x%x\n", (UINTN)Ptr));
  // ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR
  XhciSecureDeviceDesc = (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR *)Ptr;
  gBS->CopyMem (XhciSecureDeviceDesc, (VOID *)&gXhciIrCamera, sizeof (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR));
  XhciSecureDeviceDesc->AcpiPathStringLength = sizeof (AcpiDeviceIrCamObj);
  XhciSecureDeviceDesc->Length = sizeof (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR) + sizeof (AcpiDeviceIrCamObj);

  // ACPI_XHCI_SECURE_DEVICE_STRING
  Ptr = Ptr + sizeof (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR);
  gBS->CopyMem ((VOID *)Ptr, (VOID *)AcpiDeviceIrCamObj, sizeof (AcpiDeviceIrCamObj));
  Ptr = Ptr + sizeof (AcpiDeviceIrCamObj);

  DEBUG ((EFI_D_ERROR, "[ACPI SDEV] Create for FHD camera device descriptor: 0x%x\n", (UINTN)Ptr));
  // ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR
  XhciSecureDeviceDesc = (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR *)Ptr;
  gBS->CopyMem (XhciSecureDeviceDesc, (VOID *)&gXhciFhdCamera, sizeof (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR));
  XhciSecureDeviceDesc->AcpiPathStringLength = sizeof (AcpiDeviceFhdCamObj);
  XhciSecureDeviceDesc->Length = sizeof (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR) + sizeof (AcpiDeviceFhdCamObj);

  // ACPI_XHCI_SECURE_DEVICE_STRING
  Ptr = Ptr + sizeof (ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR);
  gBS->CopyMem ((VOID *)Ptr, (VOID *)AcpiDeviceFhdCamObj, sizeof (AcpiDeviceFhdCamObj));
  Ptr = Ptr + sizeof (AcpiDeviceFhdCamObj);

  DEBUG ((EFI_D_ERROR, "[ACPI SDEV] Create for IR camera ACPI descriptor: 0x%x\n", (UINTN)Ptr));
  //EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE
  SdevAcpiObj = (EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE  *)Ptr;
  SdevAcpiObj->Type = EFI_ACPI_6_2_SDEV_TYPE_ACPI_NAMESPACE_DEVICE;
  SdevAcpiObj->Flags = 1;
  SdevAcpiObj->Length = sizeof (EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE) + sizeof (AcpiDeviceIrCamObj);
  SdevAcpiObj->DeviceIdentifierOffset = 12;
  SdevAcpiObj->DeviceIdentifierLength = sizeof (AcpiDeviceIrCamObj);
  SdevAcpiObj->VendorSpecificDataOffset = 12;
  SdevAcpiObj->VendorSpecificDataLength = 0;
  SdevAcpiObj++;
  gBS->CopyMem ((VOID *)SdevAcpiObj, (VOID *) AcpiDeviceIrCamObj, sizeof (AcpiDeviceIrCamObj));
  Ptr = Ptr + sizeof(EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE ) + sizeof (AcpiDeviceIrCamObj);

  DEBUG ((EFI_D_ERROR, "[ACPI SDEV] Create for FHD camera ACPI descriptor: 0x%x\n", (UINTN)Ptr));
  //EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE
  SdevAcpiObj = (EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE  *)Ptr;
  SdevAcpiObj->Type = EFI_ACPI_6_2_SDEV_TYPE_ACPI_NAMESPACE_DEVICE;
  SdevAcpiObj->Flags = 1;
  SdevAcpiObj->Length = sizeof (EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE) + sizeof (AcpiDeviceFhdCamObj);
  SdevAcpiObj->DeviceIdentifierOffset = 12;
  SdevAcpiObj->DeviceIdentifierLength = sizeof (AcpiDeviceFhdCamObj);
  SdevAcpiObj->VendorSpecificDataOffset = 12;
  SdevAcpiObj->VendorSpecificDataLength = 0;
  SdevAcpiObj++;
  gBS->CopyMem ((VOID *)SdevAcpiObj, (VOID *) AcpiDeviceFhdCamObj, sizeof (AcpiDeviceFhdCamObj));
  Ptr = Ptr + sizeof(EFI_ACPI_6_2_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE ) + sizeof (AcpiDeviceFhdCamObj);

  SdevTableHeader->Header.Checksum = CalculateCheckSum8(Table, SdevTableSize);
//  DumpTable((UINT8 *)Table, SdevTableSize);

  Status = AcpiTableProtocol->InstallAcpiTable (
            AcpiTableProtocol,
            Table,
            SdevTableSize,
            &TableList);
  DEBUG ((EFI_D_ERROR, "[ACPI SDEV] InstallAcpiTable: (%r) \n", Status));
  return Status;
}

/**

 Intall ACPI SSDT table for secure devices

 @return Status code

**/
EFI_STATUS
EFIAPI
AcpiIntallSecureCamSsdt (
  EFI_ACPI_TABLE_PROTOCOL     *AcpiTableProtocol
  )
{
  EFI_STATUS                                Status;
  UINTN                                     TableList;
  UINTN                                     Index;
  EFI_HANDLE                                *HandleBuffer;
  UINTN                                     NumberOfHandles;
  EFI_FIRMWARE_VOLUME2_PROTOCOL             *FwVol = NULL;
  UINTN                                     Size = 0;
  UINT32                                    FvStatus;
  EFI_FV_FILETYPE                           FileType;
  EFI_FV_FILE_ATTRIBUTES                    Attributes;
  EFI_ACPI_COMMON_HEADER                    *CurrentTable;
  INTN                                      Instance;
  EFI_GUID                                  SecureBioCamSsdtGuid = SECURE_BIO_CAM_SSDT_GUID;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);

  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID**) &FwVol
                    );
    ASSERT_EFI_ERROR (Status);

    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &SecureBioCamSsdtGuid,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );

    if (Status == EFI_SUCCESS) {
      break;
    }
  }
  gBS->FreePool (HandleBuffer);

  Instance     = 0;
  CurrentTable = NULL;
  while ((FwVol != NULL) && (Status == EFI_SUCCESS)) {
    Status = FwVol->ReadSection (
                    FwVol,
                    &SecureBioCamSsdtGuid,
                    EFI_SECTION_RAW,
                    Instance,
                    (VOID**)&CurrentTable,
                    &Size,
                    &FvStatus
                    );
    if (!EFI_ERROR (Status)) {
      if (((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId == SIGNATURE_64 ('S', 'E', 'C', 'B', 'I', 'O', 0, 0)) {
        DEBUG ((EFI_D_ERROR, "[ACPI SDEV] Secure BIO Camera SSDT found.\n"));
        break;
      }
      Instance++;
      CurrentTable = NULL;
    }
  }

  TableList = 0;
  Status = AcpiTableProtocol->InstallAcpiTable (
            AcpiTableProtocol,
            CurrentTable,
            Size,
            &TableList);
  return Status;
}

/**
 The module Entry Point of the Windows Platform Binary Table DXE driver.

 @param [in]   ImageHandle      The firmware allocated handle for the EFI image.
 @param [in]   SystemTable      A pointer to the EFI System Table.

 @return Status code

**/
EFI_STATUS
EFIAPI
AcpiSdevAcpiTableEntry (
  IN EFI_HANDLE          ImageHandle,
  IN EFI_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS                  Status;
  EFI_ACPI_TABLE_PROTOCOL     *AcpiTableProtocol;
  CHIPSET_CONFIGURATION       SystemConfiguration;

  Status = GetChipsetConfiguration (&SystemConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "GetKernelConfiguration failed Status = %x.\n", Status));
    return Status;
  }

  if (SystemConfiguration.SecureBioCamSupport == 0) {
    DEBUG ((EFI_D_INFO, "Secure Biometrics camera disasbled.\n"));
    return EFI_SUCCESS;
  }

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTableProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[ACPI SDEV] Locate AcpiTableProtocol failed: (%r) \n", Status));
    return Status;
  }

  Status = AcpiSdevInstall(AcpiTableProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[ACPI SDEV] Failed to install SDEV table...\n"));
    return Status;
  }

  Status = AcpiIntallSecureCamSsdt(AcpiTableProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[ACPI SDEV] Failed to install SSDT table...\n"));
    return Status;
  }

  return EFI_SUCCESS;
}