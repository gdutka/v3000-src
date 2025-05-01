/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspCommonLib.h>
#include <Library/AmdFtpmLib.h>
#include <Protocol/AmdFtpmProtocol.h>

#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>

#include "HSPfTPMAcpi.h"

#include "HSPfTPMAmlData.h"
#include "MHSPSSDT.h"

STATIC TPM2_ACPI_TABLE  Tpm2AcpiTable = {
  {
    SIGNATURE_32 ('T','P','M','2'),
    sizeof (TPM2_ACPI_TABLE),
    0x05,                             // ACPI Revision.
    0x00,
    TPM2_ACPI_OEM_ID,                 // OEM ID (filled in below).
    TPM2_ACPI_TABLE_OEM_ID,           // OEM Table ID (filled in below).
    TPM2_ACPI_OEM_REVISION ,          // ACPI OEM Revision (filled in below).
    1,                                // OEM Creator ID (filled in below).
    1                                 // OEM Creator Revision (filled in below).
  },
  0,                                 // PlatformClass (0 for client)
  0,                                 // Reserved
  0,                                 // AddressofControlArea (Need update runtime)
  13,                                // StartMethod (use 13)
  0,                                 // TPMStartAddress (Need update runtime)
  0                                  // TPMReplyAddress (Need update runtime)
};


STATIC MHSP_ACPI_TABLE  MHSP2AcpiTable = {
  {
    SIGNATURE_32 ('M','H','S','P'),
    sizeof (MHSP_ACPI_TABLE),
    0x04,                             // ACPI Revision. Shall be 4.
    0x00,
    TPM2_ACPI_OEM_ID,                 // OEM ID (filled in below).
    TPM2_ACPI_TABLE_OEM_ID,           // OEM Table ID (filled in below).
    TPM2_ACPI_OEM_REVISION ,          // ACPI OEM Revision (filled in below).
    1,                                // OEM Creator ID (filled in below).
    1                                 // OEM Creator Revision (filled in below).
  },

  // ProtocolId
  1,                                  // Shall have a value of 1.

  // Channels
  {
    // Channels[0]
    {
      0,                                // ChannelBaseAddress
      0,                                // RequestDoorbellAddress
      0,                                // ReplyDoorbellAddress
      0x1000,                           // ChannelSize
      50,                               // IRQResource  26 (VTL0) + 24 (GSI Base)
      {0, 0}                            // ChannelParameters
    },

    // Channels[1]
    {
      0,                                // ChannelBaseAddress
      0,                                // RequestDoorbellAddress
      0,                                // ReplyDoorbellAddress
      0x1000,                           // ChannelSize
      51,                               // IRQResource  27 (VTL0) + 24 (GSI Base)
      {0, 0}                            // ChannelParameters
    },

    // Channels[2]
    {
      0,                                // ChannelBaseAddress
      0,                                // RequestDoorbellAddress
      0,                                // ReplyDoorbellAddress
      0x1000,                           // ChannelSize
      52,                               // IRQResource  28 (VTL0) + 24 (GSI Base)
      {0, 0}                            // ChannelParameters
    },

    // Channels[3]
    {
      0,                                // ChannelBaseAddress
      0,                                // RequestDoorbellAddress
      0,                                // ReplyDoorbellAddress
      0x1000,                           // ChannelSize
      53,                               // IRQResource  29 (VTL0) + 24 (GSI Base)
      {0, 0}                            // ChannelParameters
    }
  }
};


VOID
ChecksumAcpiTable (
  IN OUT   EFI_ACPI_SDT_HEADER *Table
  )
{
  UINT8   *BuffTempPtr;
  UINT8   Checksum;
  UINT32  BufferOffset;

  BuffTempPtr   = (UINT8 *) Table;
  Checksum      = 0;
  BufferOffset  = 0;

  Table->Checksum = 0;
  for (BufferOffset = 0; BufferOffset < Table->Length; BufferOffset++) {
    Checksum = Checksum - *(BuffTempPtr + BufferOffset);
  }

  Table->Checksum = Checksum;
}


VOID
EFIAPI
HSPfTPMAcpiPublish (
  IN  EFI_EVENT        Event,
  IN  VOID             *Context
  )
{
  EFI_STATUS                  Status;
  EFI_ACPI_TABLE_PROTOCOL     *AcpiTableProtocol;
  FTPM_INFO                   *pFtpmInfo;
  UINTN                       TableList;

  Status            = EFI_SUCCESS;
  AcpiTableProtocol = NULL;
  pFtpmInfo         = NULL;
  TableList         = 0;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.Drv.HSPfTPMAcpiPublish Enter\n");

  if (Event) {
    Status = gBS->CloseEvent(Event);
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tClose this event first. Status = %r\n", Status);
  }

  //
  // Locate ACPISupport table. Bail if absent
  //
  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  &AcpiTableProtocol
                  );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: Can't find ACPI protocol. Status = %r\n", Status);
    return;
  }

  // Update the TPM ACPI Table for ControlArea location
  Status = gBS->LocateProtocol (
                  &gAmdHspAcpiProtocolGuid,
                  NULL,
                  &pFtpmInfo);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCan't find gAmdHspAcpiProtocolGuid. Status = %r\n", Status);
    return;
  }

  if ( NULL == pFtpmInfo ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCan't get fTPM infomation.\n");
    return;
  }

  if (PcdGet8 (PcdAmdPspSystemTpmConfig) == SYSTEM_TPM_CONFIG_HSP_FTPM) {
    Tpm2AcpiTable.AddressofControlArea = pFtpmInfo->HSP_info.TPM_L0_Address;
    Tpm2AcpiTable.TPMStartAddress      = pFtpmInfo->HSP_info.TPM_L0_C2H_MSG_Address;
    Tpm2AcpiTable.TPMReplyAddress      = pFtpmInfo->HSP_info.TPM_L0_H2C_MSG_Address;
    ChecksumAcpiTable ((EFI_ACPI_SDT_HEADER*)&Tpm2AcpiTable);

    IDS_HDT_CONSOLE_PSP_TRACE (
      "\t\tTPM2.AddressofControlArea               = 0x%x\n"
      "\t\tTPM2.TPMStartAddress                    = 0x%x\n"
      "\t\tTPM2.TPMReplyAddress                    = 0x%x\n",
      Tpm2AcpiTable.AddressofControlArea,
      Tpm2AcpiTable.TPMStartAddress,
      Tpm2AcpiTable.TPMReplyAddress
      );

    //
    // 1. Install the TPM2 ACPI Table
    //
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall ACPI TPM2 Table\n");

    TableList = 0;
    Status = AcpiTableProtocol->InstallAcpiTable (
                                  AcpiTableProtocol,
                                  &Tpm2AcpiTable,
                                  sizeof (TPM2_ACPI_TABLE),
                                  &TableList
                                  );
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tStatus: %r\n", Status);
    ASSERT_EFI_ERROR (Status);
  }

  MHSP2AcpiTable.Channels[0].ChannelBaseAddress     = pFtpmInfo->HSP_info.VLT0_Address;
  MHSP2AcpiTable.Channels[0].RequestDoorbellAddress = pFtpmInfo->HSP_info.VLT0_C2H_MSG_Address;
  MHSP2AcpiTable.Channels[0].ReplyDoorbellAddress   = pFtpmInfo->HSP_info.VLT0_H2C_MSG_Address;
  MHSP2AcpiTable.Channels[1].ChannelBaseAddress     = pFtpmInfo->HSP_info.VLT1_Address;
  MHSP2AcpiTable.Channels[1].RequestDoorbellAddress = pFtpmInfo->HSP_info.VLT1_C2H_MSG_Address;
  MHSP2AcpiTable.Channels[1].ReplyDoorbellAddress   = pFtpmInfo->HSP_info.VLT1_HSC_MSG_Address;

  MHSP2AcpiTable.Channels[2].ChannelBaseAddress     = pFtpmInfo->HSP_info.VLT2_Address;
  MHSP2AcpiTable.Channels[2].RequestDoorbellAddress = pFtpmInfo->HSP_info.VLT2_C2H_MSG_Address;
  MHSP2AcpiTable.Channels[2].ReplyDoorbellAddress   = pFtpmInfo->HSP_info.VLT2_H2C_MSG_Address;
  MHSP2AcpiTable.Channels[3].ChannelBaseAddress     = pFtpmInfo->HSP_info.VLT3_Address;
  MHSP2AcpiTable.Channels[3].RequestDoorbellAddress = pFtpmInfo->HSP_info.VLT3_C2H_MSG_Address;
  MHSP2AcpiTable.Channels[3].ReplyDoorbellAddress   = pFtpmInfo->HSP_info.VLT3_H2C_MSG_Address;
  ChecksumAcpiTable ((EFI_ACPI_SDT_HEADER*)&MHSP2AcpiTable);

  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tMHSP2.Channel[0].ChannelBaseAddress     = 0x%x\n"
    "\t\tMHSP2.Channel[0].RequestDoorbellAddress = 0x%x\n"
    "\t\tMHSP2.Channel[0].ReplyDoorbellAddress   = 0x%x\n"
    "\t\tMHSP2.Channel[1].ChannelBaseAddress     = 0x%x\n"
    "\t\tMHSP2.Channel[1].RequestDoorbellAddress = 0x%x\n"
    "\t\tMHSP2.Channel[1].ReplyDoorbellAddress   = 0x%x\n"
    "\t\tMHSP2.Channel[2].ChannelBaseAddress     = 0x%x\n"
    "\t\tMHSP2.Channel[2].RequestDoorbellAddress = 0x%x\n"
    "\t\tMHSP2.Channel[2].ReplyDoorbellAddress   = 0x%x\n"
    "\t\tMHSP2.Channel[3].ChannelBaseAddress     = 0x%x\n"
    "\t\tMHSP2.Channel[3].RequestDoorbellAddress = 0x%x\n"
    "\t\tMHSP2.Channel[3].ReplyDoorbellAddress   = 0x%x\n",
    MHSP2AcpiTable.Channels[0].ChannelBaseAddress,
    MHSP2AcpiTable.Channels[0].RequestDoorbellAddress,
    MHSP2AcpiTable.Channels[0].ReplyDoorbellAddress,
    MHSP2AcpiTable.Channels[1].ChannelBaseAddress,
    MHSP2AcpiTable.Channels[1].RequestDoorbellAddress,
    MHSP2AcpiTable.Channels[1].ReplyDoorbellAddress,
    MHSP2AcpiTable.Channels[2].ChannelBaseAddress,
    MHSP2AcpiTable.Channels[2].RequestDoorbellAddress,
    MHSP2AcpiTable.Channels[2].ReplyDoorbellAddress,
    MHSP2AcpiTable.Channels[3].ChannelBaseAddress,
    MHSP2AcpiTable.Channels[3].RequestDoorbellAddress,
    MHSP2AcpiTable.Channels[3].ReplyDoorbellAddress
    );

  //
  // 2. Install the MHSP ACPI Table
  //
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall ACPI MHSP Table\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tMHSP2AcpiTable               : 0x%lx\n", &MHSP2AcpiTable);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tsizeof (MHSP_ACPI_TABLE)     : %d\n", sizeof (MHSP_ACPI_TABLE));
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tMHSP2AcpiTable.Header.Length : %d\n", MHSP2AcpiTable.Header.Length);

  TableList = 0;
  Status = AcpiTableProtocol->InstallAcpiTable (
                                AcpiTableProtocol,
                                &MHSP2AcpiTable,
                                sizeof (MHSP_ACPI_TABLE),
                                &TableList
                                );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tStatus: %r\n", Status);
  ASSERT_EFI_ERROR (Status);

  /*
   * Platform Override. it still need to add TPM PPI and MOR Spec supported.
   * */
#if 0
  //
  // 3. Install the SSDT ACPI Table for _SB.TPM2
  //
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall ACPI TPM SSDT Table\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSPfTPMAmlData              : 0x%lx\n", &HSPfTPMAmlData);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tsizeof (HSPfTPMAmlData)     : %d\n", sizeof (HSPfTPMAmlData));
  TableList = 0;
  Status = AcpiTableProtocol->InstallAcpiTable (
                                AcpiTableProtocol,
                                &HSPfTPMAmlData,
                                sizeof (HSPfTPMAmlData),
                                &TableList
                                );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tStatus: %r\n", Status);
  ASSERT_EFI_ERROR (Status);
#endif

  //
  // 4. Install the SSDT ACPI Table for _SB.MHSP
  //
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall ACPI MHSP SSDT Table\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tMHSPSSDT              : 0x%lx\n", &MHSPSSDT);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tsizeof (MHSPSSDT)     : %d\n", sizeof (MHSPSSDT));

  TableList = 0;
  Status = AcpiTableProtocol->InstallAcpiTable (
                                AcpiTableProtocol,
                                &MHSPSSDT,
                                sizeof (MHSPSSDT),
                                &TableList
                                );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tStatus: %r\n", Status);
  ASSERT_EFI_ERROR (Status);

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.Drv.HSPfTPMAcpiPublish Exit\n");
}


/**
 * @brief   Find HSP PCI device and get its BAR0 value.
 *
 * @param   pBaseAddress    HSP BAR0 value.
 * @return  EFI_STATUS      EFI_SUCCESS              Success
 *                          EFI_INVALID_PARAMETER    Invalid parameters
 *                          EFI_DEVICE_ERROR         HSP device error
 */
EFI_STATUS
HSPfTpmAcpiEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                    Status;
  AMD_FTPM_PROTOCOL             *mPspInstance;
  UINTN                         IsHspSts;
  EFI_EVENT                     Evt;
  VOID                          *Registration;

  Status        = EFI_SUCCESS;
  mPspInstance  = NULL;
  IsHspSts      = FALSE;
  Evt           = NULL;
  Registration  = NULL;

  if ((PcdGetBool (PcdAmdPspEnable) == FALSE) ||
      (PcdGetBool (PcdAmdHspCoreEnable) == FALSE)) {
    return EFI_SUCCESS;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.Drv.HSPfTpmAcpiEntryPoint Enter\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tPcdGet8 (PcdAmdPspSystemTpmConfig) = %d\n", PcdGet8 (PcdAmdPspSystemTpmConfig));

  //
  // Register notify function on ReadyToBoot Event.
  //
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall ReadyToBoot Event\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  HSPfTPMAcpiPublish,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &Evt
                  );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall ReadyToBoot Event Status %r\n", Status);
  ASSERT_EFI_ERROR (Status);

  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall Legacy Boot Event\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  HSPfTPMAcpiPublish,
                  NULL,
                  &gEfiEventLegacyBootGuid,
                  &Evt
                  );
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInstall Legacy Boot Event Status %r\n", Status);
  ASSERT_EFI_ERROR (Status);

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHsp.Drv.HSPfTpmAcpiEntryPoint End\n");

  return Status;
}
