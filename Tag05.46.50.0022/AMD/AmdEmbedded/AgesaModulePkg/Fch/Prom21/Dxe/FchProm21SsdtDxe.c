/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/FirmwareVolume2.h>

#include <Library/AmdBaseLib.h>
#include <Filecode.h>

#include <Library/FchProm21Common.h>
#include <Library/FchProm21BaseLib.h>
#include <Protocol/FchProm21InitProtocol.h>
#include "FchProm21SsdtDxe.h"

#define FILECODE FCH_PROM21_DXE_FCHPROM21SSDTDXE_FILECODE

#define AMD_FCH_PROM21_SSDT_ASL_INF_GUID \
  { 0xE4B6B9D6, 0x8A5D, 0x48A6, 0xBA, 0x00, 0x2C, 0x1D, 0x80, 0x3C, 0x27, 0x34 }

EFI_GUID gAmdFchProm21SsdtAslInfGuid   = AMD_FCH_PROM21_SSDT_ASL_INF_GUID;

VOID
EFIAPI
FchProm21SsdtDxeInit (
  IN      EFI_EVENT Event,
  IN      VOID      *Context
  )
{
  EFI_STATUS        Status;
  PT21_INIT_PROTOCOL  *pAmdFchInitProtocol;
  UINTN             Index;
  INTN              Instance;
  UINTN             Size;
  UINTN             NumberOfHandles;
  UINTN             TableHandle;
  UINTN             TableSize;
  UINT32            FvStatus;
  UINT8             PTBusNum;
  UINT8             Data8;
  UINT8             CmdReg;
  UINT32            MmioBase;
  UINT32            DidVid;
  UINT32            GppPcieAddress;
  UINT8             GppBus;
  UINT8             GppDev;
  UINT8             GppFun;
  UINT32            SecGppPcieAddress;
  UINT8             SecGppBus;
  UINT8             SecGppDev;
  UINT8             SecGppFun;
  EFI_HANDLE        *HandleBuffer;
  EFI_FV_FILETYPE   FileType;
  EFI_FV_FILE_ATTRIBUTES      Attributes;
  EFI_ACPI_COMMON_HEADER      *CurrentTable;
  EFI_ACPI_DESCRIPTION_HEADER *FchPromontoryAcpiBlockPtr  = NULL;
  EFI_ACPI_TABLE_PROTOCOL     *AcpiTableProtocol;
  EFI_ACPI_SDT_PROTOCOL       *AcpiSdtProtocol;
  EFI_FIRMWARE_VOLUME2_PROTOCOL         *FwVol = NULL;

  DEBUG ((DEBUG_INFO, "PT-SSDTDXE-%a-Start\n", __FUNCTION__));

  // Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  // Check if Promontory was exist
  DidVid = PcdGet32 (PcdPT21XhciSsid);
  if (DidVid == 0xFFFFFFFF) {
    DEBUG ((DEBUG_INFO, "  SSDT DXE: Promontory is NOT FOUND!\n"));
    return;
  }

  Status = gBS->LocateProtocol (
                  &gAmdFchProm21InitProtocolGuid,
                  NULL,
                  &pAmdFchInitProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "  SSDT DXE: gAmdFchProm21InitProtocolGuid is NOT FOUND!\n"));
    return;
  }

  GppPcieAddress = PcdGet32 (PcdPT21GppPcieAddress);
  GppBus = (UINT8) ((GppPcieAddress >> 20) & 0xFF);
  GppDev = (UINT8) ((GppPcieAddress >> 15) & 0x1F);
  GppFun = (UINT8) ((GppPcieAddress >> 12) & 0x07);

  SecGppPcieAddress = PcdGet32 (PcdSecPT21GppPcieAddress);
  SecGppBus = (UINT8) ((SecGppPcieAddress >> 20) & 0xFF);
  SecGppDev = (UINT8) ((SecGppPcieAddress >> 15) & 0x1F);
  SecGppFun = (UINT8) ((SecGppPcieAddress >> 12) & 0x07);

  DEBUG ((EFI_D_INFO, "  PcdPT21XhciLock = %d\n", PcdGet8 (PcdPT21XhciLock)));

  if (PcdGet8 (PcdPT21XhciLock)) {
    // Get Bus Number of PT USP
    PTBusNum = PtPciRead8 (GppBus, GppDev, GppFun, 0x19);
    if ((PTBusNum != 0) && (PTBusNum != 0xFF)) {
      // Get Bus Number of PT DSP
      PTBusNum = PtPciRead8 (PTBusNum, 0, 0, 0x19);
      if ((PTBusNum != 0) && (PTBusNum != 0xFF)) {
        if ((PcdGet8 (PcdSecPT21XhciLock) != 0) && (SecGppPcieAddress != 0)) {
          // Get Bus Number of Secondary PT USP
          SecGppBus = PtPciRead8 (PTBusNum, SecGppDev, SecGppFun, 0x19);
          if ((SecGppBus != 0) && (SecGppBus != 0xFF)) {
            // Get Bus Number of Secondary PT DSP
            SecGppBus = PtPciRead8 (SecGppBus, 0, 0, 0x19);
            if ((SecGppBus != 0) && (SecGppBus != 0xFF)) {
              // Get Bus Number of Secondary PT XHCI
              SecGppBus = PtPciRead8 (SecGppBus, 0xC, 0, 0x19);
              if ((SecGppBus != 0) && (SecGppBus != 0xFF)) {
                CmdReg = PtPciRead8 (SecGppBus, 0, 0, 4);
                if (CmdReg != 0xFF) {
                  PtPciWrite8 (SecGppBus, 0, 0, 4, (CmdReg | 6));

                  // Lock Secondary PT
                  MmioBase = PtPciRead32 (SecGppBus, 0, 0, 0x10);
                  MmioBase &= 0xFFFFFFF0;
                  if ((MmioBase != 0) && (MmioBase != 0xFFFFFFF0)) {
                    Data8 = FchProm21XhciReadByte (MmioBase, XHCI_REG_PCIE_HW_RW);
                    FchProm21XhciWriteByte (MmioBase, XHCI_REG_PCIE_HW_RW, Data8 & ~BIT4);

                    PtPciWrite8 (SecGppBus, 0, 0, 4, CmdReg);
                    DEBUG ((EFI_D_INFO, "  Lock Secondary PT MMIO in BDS!\n"));
                  }
                }
              }
            }
          } // End of if ((SecGppBus != 0) && (SecGppBus != 0xFF))
        } // End of if ((PcdGet8 (PcdSecPT21XhciLock) != 0) && (SecGppPcieAddress != 0))

        // Get Bus Number of PT XHCI
        PTBusNum = PtPciRead8 (PTBusNum, 0xC, 0, 0x19);
        if ((PTBusNum != 0) && (PTBusNum != 0xFF)) {
          CmdReg = PtPciRead8 (PTBusNum, 0, 0, 4);
          if (CmdReg != 0xFF) {
            PtPciWrite8 (PTBusNum, 0, 0, 4, (CmdReg | 6));

            // Lock PT
            MmioBase = PtPciRead32 (PTBusNum, 0, 0, 0x10);
            MmioBase &= 0xFFFFFFF0;
            if ((MmioBase != 0) && (MmioBase != 0xFFFFFFF0)) {
              Data8 = FchProm21XhciReadByte (MmioBase, XHCI_REG_PCIE_HW_RW);
              FchProm21XhciWriteByte (MmioBase, XHCI_REG_PCIE_HW_RW, Data8 & ~BIT4);

              PtPciWrite8 (PTBusNum, 0, 0, 4, CmdReg);
              DEBUG ((EFI_D_INFO, "  Lock PT MMIO in BDS!\n"));
            }
          }
        }
      }
    }
  } // End of if (PcdGet8 (PcdPT21XhciLock))

  Status = gBS->LocateProtocol (
                &gEfiAcpiTableProtocolGuid,
                NULL,
                &AcpiTableProtocol
                );

  Status = gBS->LocateProtocol (
                &gEfiAcpiSdtProtocolGuid,
                NULL,
                &AcpiSdtProtocol
                );

  FvStatus = 0;
  //
  // Locate protocol.
  //
  Status = gBS->LocateHandleBuffer (
                 ByProtocol,
                 &gEfiFirmwareVolume2ProtocolGuid,
                 NULL,
                 &NumberOfHandles,
                 &HandleBuffer
                 );

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
                    &gAmdFchProm21SsdtAslInfGuid,
                    NULL,
                    &Size,
                    &FileType,
                    &Attributes,
                    &FvStatus
                    );
    if (Status == EFI_SUCCESS) {
      break;
    }
  } // End of for (Index = 0; Index < NumberOfHandles; Index++)

  Instance = 0;
  CurrentTable = NULL;
  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                    FwVol,
                    &gAmdFchProm21SsdtAslInfGuid,
                    EFI_SECTION_RAW,
                    Instance,
                    &CurrentTable,
                    &Size,
                    &FvStatus
                    );
    if (!EFI_ERROR (Status)) {
      if (((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId == SIGNATURE_64 ('F', 'C', 'H', 'P', 'T', 0, 0, 0)) {
        // Update table OEM fields.
        LibAmdMemCopy (
          (VOID *) &((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemId,
          (VOID *) PcdGetPtr(PcdAmdAcpiTableHeaderOemId),
          6,
          (AMD_CONFIG_PARAMS*) NULL
        );
        LibAmdMemCopy (
          (VOID *) &((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId,
          (VOID *) PcdGetPtr(PcdAmdAcpiPtSsdtTableHeaderOemTableId),
          8,
          (AMD_CONFIG_PARAMS*) NULL
        );

        FchPromontoryAcpiBlockPtr = (EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable;
        TableHandle = 0;
        TableSize = FchPromontoryAcpiBlockPtr->Length;

        //
        // Install ACPI table
        //
        Status = AcpiTableProtocol->InstallAcpiTable (
                                    AcpiTableProtocol,
                                    FchPromontoryAcpiBlockPtr,
                                    TableSize,
                                    &TableHandle
                                    );

        gBS->FreePool (CurrentTable);
      }

      Instance++;
      CurrentTable = NULL;
    } // End of if (!EFI_ERROR (Status))
  } // End of while (Status == EFI_SUCCESS)

  DEBUG ((DEBUG_INFO, "PT-SSDTDXE-%a-End\n", __FUNCTION__));
}

EFI_STATUS
EFIAPI
FchProm21SsdtDxeInitEntry (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
    )
{
  EFI_STATUS  Status;
  EFI_EVENT   ReadyToBootEvent;

  DEBUG ((DEBUG_INFO, "PT-SSDTDXE-%a-Start\n", __FUNCTION__));
  Status = gBS->CreateEventEx (
                      EVT_NOTIFY_SIGNAL,
                      TPL_CALLBACK,
                      FchProm21SsdtDxeInit,
                      NULL,
                      &gEfiEventReadyToBootGuid,
                      &ReadyToBootEvent
                      );
  DEBUG ((DEBUG_INFO, "PT-SSDTDXE-%a-End\n", __FUNCTION__));
  return Status;
}

