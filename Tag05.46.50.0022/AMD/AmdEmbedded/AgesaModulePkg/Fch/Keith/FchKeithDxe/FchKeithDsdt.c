/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "FchKeithDsdt.h"

#define FILECODE FCH_KEITH_FCHKEITHDXE_FCHKEITHDSDT_FILECODE

ASL_NAME_OP_INFO IrqAslNameInfo[] = {
  {SIGNATURE_32 ('U', 'R', '0', 'I'), sizeof (UINT8), FixedPcdGet8 (PcdFchUart0Irq)},
  {SIGNATURE_32 ('U', 'R', '1', 'I'), sizeof (UINT8), FixedPcdGet8 (PcdFchUart1Irq)},
  {SIGNATURE_32 ('U', 'R', '2', 'I'), sizeof (UINT8), FixedPcdGet8 (PcdFchUart2Irq)},
  {SIGNATURE_32 ('U', 'R', '3', 'I'), sizeof (UINT8), FixedPcdGet8 (PcdFchUart3Irq)},
  {SIGNATURE_32 ('U', 'R', '4', 'I'), sizeof (UINT8), FixedPcdGet8 (PcdFchUart4Irq)},
  {SIGNATURE_32 ('I', 'C', '0', 'I'), sizeof (UINT8), FixedPcdGet8 (PcdFchI2c0Irq)},
  {SIGNATURE_32 ('I', 'C', '1', 'I'), sizeof (UINT8), FixedPcdGet8 (PcdFchI2c1Irq)},
  {SIGNATURE_32 ('I', 'C', '2', 'I'), sizeof (UINT8), FixedPcdGet8 (PcdFchI2c2Irq)},
  {SIGNATURE_32 ('I', 'C', '3', 'I'), sizeof (UINT8), FixedPcdGet8 (PcdFchI2c3Irq)},
  {SIGNATURE_32 ('I', 'C', '4', 'I'), sizeof (UINT8), FixedPcdGet8 (PcdFchI2c4Irq)},
};
UINTN  IrqAslNameInfoSize = (sizeof (IrqAslNameInfo) / sizeof (ASL_NAME_OP_INFO));

EFI_STATUS
GetDsdtHeader (
  IN  EFI_ACPI_SDT_PROTOCOL          *AcpiSdtProtocol,
  OUT EFI_ACPI_DESCRIPTION_HEADER    **Header
  )
{
  EFI_STATUS                                Status = EFI_SUCCESS;
  UINTN                                     AcpiTblIndex;
  EFI_ACPI_TABLE_VERSION                    Version;
  EFI_ACPI_SDT_HEADER                       *AcpiTbl = NULL;
  UINTN                                     AcpiHandle;
  EFI_PHYSICAL_ADDRESS                      Dsdt = 0;
  EFI_PHYSICAL_ADDRESS                      XDsdt = 0;

  if (AcpiSdtProtocol == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (AcpiTblIndex = 0; ; AcpiTblIndex++) {
    Status = AcpiSdtProtocol->GetAcpiTable (AcpiTblIndex, &AcpiTbl, &Version, &AcpiHandle);
    if (!EFI_ERROR (Status)) {
      if (AcpiTbl->Signature == EFI_ACPI_1_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE) {    // ('F', 'A', 'C', 'P')
        if (Version == EFI_ACPI_TABLE_VERSION_1_0B) {
          Dsdt = (EFI_PHYSICAL_ADDRESS)(((EFI_ACPI_1_0_FIXED_ACPI_DESCRIPTION_TABLE *) AcpiTbl)->Dsdt);
        }
        if ((Version &
             (EFI_ACPI_TABLE_VERSION_2_0 |
              EFI_ACPI_TABLE_VERSION_3_0 |
              EFI_ACPI_TABLE_VERSION_4_0 |
              EFI_ACPI_TABLE_VERSION_5_0)) != 0)
        {
          XDsdt = (EFI_PHYSICAL_ADDRESS)(((EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE *) AcpiTbl)->XDsdt);
        }
      }
      if (Dsdt || XDsdt) {
        break;
      }
    }
  }

  if (XDsdt) {
    *Header = (EFI_ACPI_DESCRIPTION_HEADER *)XDsdt;
    DEBUG ((EFI_D_INFO, "X_DSDT is Valid\n"));
  } else if (Dsdt) {
    *Header = (EFI_ACPI_DESCRIPTION_HEADER *)Dsdt;
    DEBUG ((EFI_D_INFO, "DSDT is Valid\n"));
  }

  return Status;
}

BOOLEAN
CheckAmlNameObject (
  IN  VOID      *Name
  )
{
  UINT8       *OpCode;

  if (Name != NULL) {
    OpCode = (UINT8 *)Name - OPCODE_SIZE;
    if (*OpCode == AML_NAME_OP) {
      return TRUE;
    } else {
      return FALSE;
    }
  }
  return FALSE;
}

EFI_STATUS
GetAmlNameData (
  IN  VOID      *Name,
  IN  UINTN     Size,
  OUT VOID      *Data
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       *DataPrefix;

  if ((Data == NULL) || (Name == NULL) || (Size > MAX_DATA_BUFFER_SIZE)) {
    return EFI_INVALID_PARAMETER;
  }

  DataPrefix = (UINT8 *)Name + NAME_SIZE;
  switch (*(UINT8 *)DataPrefix) {
    case AML_BYTE_PREFIX:
      if (Size == sizeof (UINT8)) {
        *(UINT8 *)Data = *((UINT8 *)DataPrefix + PREFIX_SIZE);
      } else {
        Status = EFI_BAD_BUFFER_SIZE;
      }
      break;
    case AML_WORD_PREFIX:
      if (Size == sizeof (UINT16)) {
        *(UINT16 *)Data = *(UINT16 *)((UINT8 *)DataPrefix + PREFIX_SIZE);
      } else {
        Status = EFI_BAD_BUFFER_SIZE;
      }
      break;
    case AML_DWORD_PREFIX:
      if (Size == sizeof (UINT32)) {
        *(UINT32 *)Data = *(UINT32 *)((UINT8 *)DataPrefix + PREFIX_SIZE);
      } else {
        Status = EFI_BAD_BUFFER_SIZE;
      }
      break;
    case AML_QWORD_PREFIX:
      if (Size == sizeof (UINT64)) {
        *(UINT64 *)Data = *(UINT64 *)((UINT8 *)DataPrefix + PREFIX_SIZE);
      } else {
        Status = EFI_BAD_BUFFER_SIZE;
      }
      break;
    default:
      return Status = EFI_INVALID_PARAMETER;
  }
  return Status;
}

EFI_STATUS
SetAmlNameData (
  IN  VOID      *Name,
  IN  UINTN     Size,
  IN  VOID      *Data
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       *DataPrefix;

  if ((Data == NULL) || (Name == NULL) || (Size > MAX_DATA_BUFFER_SIZE)) {
    return EFI_INVALID_PARAMETER;
  }

  DataPrefix = (UINT8 *)Name + NAME_SIZE;
  switch (*(UINT8 *)DataPrefix) {
    case AML_BYTE_PREFIX:
      if (Size == sizeof (UINT8)) {
        *((UINT8 *)DataPrefix + PREFIX_SIZE) = *(UINT8 *)Data;
      } else {
        Status = EFI_INVALID_PARAMETER;
      }
      break;
    case AML_WORD_PREFIX:
      if (Size == sizeof (UINT16)) {
        *(UINT16 *)((UINT8 *)DataPrefix + PREFIX_SIZE) = *(UINT16 *)Data;
      } else {
        Status = EFI_INVALID_PARAMETER;
      }
      break;
    case AML_DWORD_PREFIX:
      if (Size == sizeof (UINT32)) {
        *(UINT32 *)((UINT8 *)DataPrefix + PREFIX_SIZE) = *(UINT32 *)Data;
      } else {
        Status = EFI_INVALID_PARAMETER;
      }
      break;
    case AML_QWORD_PREFIX:
      if (Size == sizeof (UINT64)) {
        *(UINT64 *)((UINT8 *)DataPrefix + PREFIX_SIZE) = *(UINT64 *)Data;
      } else {
        Status = EFI_INVALID_PARAMETER;
      }
      break;
    default:
      return Status = EFI_INVALID_PARAMETER;
  }
  return Status;
}

EFI_STATUS
ModifyNameObject (
  EFI_ACPI_DESCRIPTION_HEADER   *Header,
  UINT32                        Signature,
  UINTN                         Size,
  VOID                          *Data
  )
{
  EFI_STATUS                    Status = EFI_NOT_FOUND;
  UINT32                        DsdtSize;
  UINT8                         *CurrPtr = NULL;
  UINT32                        *CurrSig = NULL;
  BOOLEAN                       IsNameObj = FALSE;
  UINT8                         TempDataBuffer[MAX_DATA_BUFFER_SIZE];

  DsdtSize = Header->Length;
  CurrPtr = (UINT8 *)Header;

  while ((UINTN) CurrPtr < ((UINTN)Header + (UINTN)DsdtSize)) {
    CurrSig = (UINT32 *)CurrPtr;
    if (*CurrSig == Signature) {
      IsNameObj = CheckAmlNameObject (CurrSig);
      if (IsNameObj) {
        Status = GetAmlNameData (CurrSig, Size, TempDataBuffer);
        if (!EFI_ERROR (Status)) {
          Status = SetAmlNameData (CurrSig, Size, Data);
        }
        break;
      }
    }
    CurrPtr++;
  }
  return Status;
}

EFI_STATUS
UpdateAmlDevIrq (
  EFI_ACPI_DESCRIPTION_HEADER    *AcpiDescriptHeader,
  ASL_NAME_OP_INFO               *IrqInfo,
  UINTN                          Size
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  UINTN         Index;

  for (Index = 0; Index < Size; Index++) {
    Status = ModifyNameObject (
      AcpiDescriptHeader,
      (IrqInfo + Index)->Signature,
      (IrqInfo + Index)->Size,
      &((IrqInfo + Index)->Data)
      );
  }

  return Status;
}

/*********************************************************************************
 * Name: FchKeithDsdtInitEntry
 *
 * Description
 *   Entry point of the AMD FCH FchKeithDsdt support driver
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchKeithDsdtInitEntry (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                                Status;
  EFI_ACPI_SDT_PROTOCOL                     *AcpiSdtProtocol;
  EFI_ACPI_DESCRIPTION_HEADER               *AcpiDescriptHeader = NULL;
  Status = gBS->LocateProtocol (
                  &gEfiAcpiSdtProtocolGuid,
                  NULL,
                  &AcpiSdtProtocol
                  );
  if (!EFI_ERROR (Status)) {
    Status = GetDsdtHeader (AcpiSdtProtocol, &AcpiDescriptHeader);
    if (!EFI_ERROR (Status)) {
      UpdateAmlDevIrq (AcpiDescriptHeader, IrqAslNameInfo, IrqAslNameInfoSize);
    }
  }
  return Status;
}
