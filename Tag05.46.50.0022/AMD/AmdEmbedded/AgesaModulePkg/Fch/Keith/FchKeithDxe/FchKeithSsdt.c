/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "FchKeithSsdt.h"

#if USB4_ASL_SUPPORT
#include "AMD.h"
#endif

#define FILECODE FCH_KEITH_FCHKEITHDXE_FCHKEITHSSDT_FILECODE

extern EFI_GUID gFchSsdtInstalledProtocolGuid;
EFI_GUID gAmdFchKeithSsdtGuid   = AMD_FCH_KEITH_SSDT_GUID;
OEM_TABLE_ID OemTableIdTable[]={
#if USB4_ASL_SUPPORT
  {SIGNATURE_64 ('K', 'T', 'U', '4', 0, 0, 0, 0), TRUE},
#endif
//  {SIGNATURE_64 ('F', 'C', 'H', 'S', 'H', 0, 0, 0), FALSE},
  {SIGNATURE_64 ('A', 'C', 'D', 'C', 0, 0, 0, 0), FALSE},
  {SIGNATURE_64 ('S', 'T', 'D', '3', 0, 0, 0, 0), FALSE}
};
UINT8  OemTableIdTableSize = (sizeof(OemTableIdTable)/sizeof(OEM_TABLE_ID));

BOOLEAN
CheckAmlNameObject (
  IN  VOID      *Name
)
{
  UINT8 *OpCode;

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
SetSstsAmlNameData(
  IN  VOID      *Name,
  IN  UINTN     Size,
  IN  VOID      *Data
)
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8       *DataPrefix;

  if ((Data == NULL) || (Name == NULL) || (Size > MAX_DATA_BUFFER_SIZE)){
    return EFI_INVALID_PARAMETER;
  }

  DataPrefix = (UINT8 *)Name + NAME_SIZE;
  switch (*(UINT8 *)DataPrefix){
    case AML_BYTE_PREFIX:
      if (Size == sizeof(UINT8)){
        *((UINT8 *)DataPrefix + PREFIX_SIZE) = *(UINT8 *)Data;
      }else{
        Status = EFI_INVALID_PARAMETER;
      }
      break;
    case AML_WORD_PREFIX:
      if (Size == sizeof(UINT16)){
        *(UINT16 *)((UINT8 *)DataPrefix + PREFIX_SIZE) = *(UINT16 *)Data;
      }else{
        Status = EFI_INVALID_PARAMETER;
      }
      break;
    case AML_DWORD_PREFIX:
      if (Size == sizeof(UINT32)){
        *(UINT32 *)((UINT8 *)DataPrefix + PREFIX_SIZE) = *(UINT32 *)Data;
      }else{
        Status = EFI_INVALID_PARAMETER;
      }
      break;
    case AML_QWORD_PREFIX:
      if (Size == sizeof(UINT64)){
        *(UINT64 *)((UINT8 *)DataPrefix + PREFIX_SIZE) = *(UINT64 *)Data;
      }else{
        Status = EFI_INVALID_PARAMETER;
      }
      break;
    case AML_ZERO_OP:
    case AML_ONE_OP:
      if ((*(UINT8 *)Data == 0x00) || (*(UINT8 *)Data == 0x01)){
        *(UINT8 *)DataPrefix = *(UINT8 *)Data;
      }else{
        Status = EFI_INVALID_PARAMETER;
      }
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
  UINT32                        SsdtSize;
  UINT8                         *CurrPtr = NULL;
  UINT32                        *CurrSig = NULL;
  BOOLEAN                       IsNameObj = FALSE;

  SsdtSize = Header->Length;
  CurrPtr = (UINT8 *)Header;

  while ((UINTN)CurrPtr < ((UINTN)Header + (UINTN)SsdtSize - (sizeof (UINT32) - 1))){

    CurrSig = (UINT32 *)CurrPtr;
    if (*CurrSig == Signature) {
      IsNameObj = CheckAmlNameObject (CurrSig);
      if (IsNameObj){
        Status = SetSstsAmlNameData (CurrSig, Size, Data);
        break;
      }
    }
    CurrPtr++;
  }
  return Status;
}


VOID
UpdateOemTableIdTable (VOID)
{
  BOOLEAN    AcdcEnable;
  UINT8      Index;

  AcdcEnable = PcdGetBool (PcdAcdcTimerEnable);

  for (Index = 0; Index < OemTableIdTableSize; Index++) {
    if (OemTableIdTable[Index].TableId == SIGNATURE_64 ('A', 'C', 'D', 'C', 0, 0, 0, 0))  {
      OemTableIdTable[Index].Enable = AcdcEnable;
    } else if (OemTableIdTable[Index].TableId == SIGNATURE_64 ('S', 'T', 'D', '3', 0, 0, 0, 0)) {
      OemTableIdTable[Index].Enable = PcdGet32 (FchRTD3ColdEnableMap) & BIT28 ? TRUE : FALSE;
    }
  }
}

VOID
UpdateSsdtDefinitionBlock (
  EFI_ACPI_DESCRIPTION_HEADER    *SstsTable
  )
{
  EFI_STATUS                          Status;
  UINT8                               StdsValue;
  BOOLEAN                             StddValue;
#if USB4_ASL_SUPPORT
  UINT8                               Usb4D3Enable;
#endif
  UINT8                               SataD3EntrySwSmi, SataD3ExitSwSmi;
  UINT8                               TimezoneDaylightValid, TimezoneHighByte, TimezoneLowByte, DaylightByte;
  UINT8                               *AmlObjPtr;
  UINT32                              RootBrdgeName, Sata0Name, Sata1Name;
  UINT32                              Checksum = 0x00;

  if (SstsTable->OemTableId == SIGNATURE_64 ('S', 'T', 'D', '3', 0, 0, 0, 0)){
    RootBrdgeName = *(UINT32 *)PcdGetPtr(PcdSataAcpiTableRootBridgeName);
    Sata0Name = *(UINT32 *)PcdGetPtr(PcdSata0AcpiTableDeviceName);
    Sata1Name = *(UINT32 *)PcdGetPtr(PcdSata1AcpiTableDeviceName);
    for (
      AmlObjPtr = (UINT8 *) SstsTable;
      AmlObjPtr < (UINT8 *) SstsTable + SstsTable->Length - (sizeof (UINT32) - 1);
      AmlObjPtr++)
    {
      if (*(UINT32 *)AmlObjPtr == SIGNATURE_32('R', 'T', 'X', 'X')) {
         *(UINT32 *)AmlObjPtr = RootBrdgeName;
      } else if (*(UINT32 *)AmlObjPtr == SIGNATURE_32('D', 'E', 'V', '0')) {
         *(UINT32 *)AmlObjPtr = Sata0Name;
      }
    }

    StdsValue = (PcdGet32(FchRTD3ColdEnableMap) & BIT28) >> 28;
    Status = ModifyNameObject(SstsTable, SIGNATURE_32('S', 'T', 'D', 'S'), sizeof (UINT8), &StdsValue);

    StddValue = (PcdGetBool(FchSataDummyD3ColdSupport)) ? 1 : 0;
    Status = ModifyNameObject(SstsTable, SIGNATURE_32('S', 'T', 'D', 'D'), sizeof (UINT8), &StddValue);

    SataD3EntrySwSmi = PcdGet8 (PcdFchSataD3EntrySwSmi);
    Status = ModifyNameObject(SstsTable, SIGNATURE_32('S', 'C', 'P', 'D'), sizeof (UINT8), &SataD3EntrySwSmi);

    SataD3ExitSwSmi = PcdGet8(PcdFchSataD3ExitSwSmi);
    Status = ModifyNameObject(SstsTable, SIGNATURE_32('S', 'C', 'P', 'U'), sizeof (UINT8), &SataD3ExitSwSmi);

    //Update checksum
    for (AmlObjPtr = (UINT8*) SstsTable; AmlObjPtr < ((UINT8*) SstsTable + SstsTable->Length); AmlObjPtr++) {
      Checksum += *AmlObjPtr;
    }
    Checksum -= SstsTable->Checksum;
    Checksum ^= 0xFFFFFFFF;
    Checksum += 1;
    SstsTable->Checksum = (UINT8)(Checksum & 0xFF);
  }

  if (SstsTable->OemTableId == SIGNATURE_64 ('A', 'C', 'D', 'C', 0, 0, 0, 0)){
    TimezoneDaylightValid = PcdGet8(PcdFchAcdcTimerTimezoneDaylightValid);
    Status = ModifyNameObject(SstsTable, SIGNATURE_32('T', 'Z', 'L', 'I'), sizeof (UINT8), &TimezoneDaylightValid);
    if ((Status == EFI_SUCCESS) && (TimezoneDaylightValid == 0x01)){
      TimezoneLowByte = PcdGet8(PcdFchAcdcTimerTimezoneCmosOffsetLow);
      Status = ModifyNameObject(SstsTable, SIGNATURE_32('T', 'Z', 'V', 'L'), sizeof (UINT8), &TimezoneLowByte);

      TimezoneHighByte = PcdGet8(PcdFchAcdcTimerTimezoneCmosOffsetHigh);
      Status = ModifyNameObject(SstsTable, SIGNATURE_32('T', 'Z', 'V', 'H'), sizeof (UINT8), &TimezoneHighByte);

      DaylightByte = PcdGet8(PcdFchAcdcTimerDaylightCmosOffset);
      Status = ModifyNameObject(SstsTable, SIGNATURE_32('L', 'I', 'V', 'R'), sizeof (UINT8), &DaylightByte);
    }

    //Update checksum
    Checksum = 0x00;
    for (AmlObjPtr = (UINT8*) SstsTable; AmlObjPtr < ((UINT8*) SstsTable + SstsTable->Length); AmlObjPtr++) {
      Checksum += *AmlObjPtr;
    }
    Checksum -= SstsTable->Checksum;
    Checksum ^= 0xFFFFFFFF;
    Checksum += 1;
    SstsTable->Checksum = (UINT8)(Checksum & 0xFF);
  }

#if USB4_ASL_SUPPORT
  if (SstsTable->OemTableId == SIGNATURE_64 ('K', 'T', 'U', '4', 0, 0, 0, 0)) {
    StddValue = (FchCheckRmbB0() == TRUE) ? 0xF : 0;
    Status = ModifyNameObject(SstsTable, SIGNATURE_32('C', 'D', 'W', 'V'), sizeof (UINT8), &StddValue);

    Usb4D3Enable = PcdGet32 (FchRTD3ColdEnableMap) & BIT30 ? 0x01 : 0x00;
    Status = ModifyNameObject(SstsTable, SIGNATURE_32('U', '4', 'D', '3'), sizeof (UINT8), &Usb4D3Enable);

    //Update checksum
    for (AmlObjPtr = (UINT8*) SstsTable; AmlObjPtr < ((UINT8*) SstsTable + SstsTable->Length); AmlObjPtr++) {
      Checksum += *AmlObjPtr;
    }
    Checksum -= SstsTable->Checksum;
    Checksum ^= 0xFFFFFFFF;
    Checksum += 1;
    SstsTable->Checksum = (UINT8)(Checksum & 0xFF);
  }
#endif
}

/*********************************************************************************
 * Name: FchKeithSsdtInitEntry
 *
 * Description
 *   Entry point of the AMD FCH FchKeithSsdtInit support driver
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
FchKeithSsdtInitEntry (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                                Status;
  UINTN                                     Index;
  INTN                                      Instance;
  UINTN                                     Size;
  UINTN                                     NumberOfHandles;
  UINTN                                     TableHandle;
  UINTN                                     TableSize;
  UINT32                                    FvStatus;
  EFI_HANDLE                                Handle;
  EFI_HANDLE                                *HandleBuffer;
  EFI_FV_FILETYPE                           FileType;
  EFI_FV_FILE_ATTRIBUTES                    Attributes;
  EFI_ACPI_COMMON_HEADER                    *CurrentTable;
  EFI_ACPI_DESCRIPTION_HEADER               *FchKeithAcpiBlockPtr  = NULL;

  EFI_ACPI_TABLE_PROTOCOL                   *AcpiTableProtocol;
  EFI_ACPI_SDT_PROTOCOL                     *AcpiSdtProtocol;
  EFI_FIRMWARE_VOLUME2_PROTOCOL             *FwVol = NULL;


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

  //Update OemTableIdTable
  UpdateOemTableIdTable ();

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

    Size = 0;
    FvStatus = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &gAmdFchKeithSsdtGuid,
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

  Instance = 0;
  CurrentTable = NULL;
  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gAmdFchKeithSsdtGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      &CurrentTable,
                      &Size,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      for (Index = 0; Index < OemTableIdTableSize; Index++) {
        if ((((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId == OemTableIdTable[Index].TableId)
            && (OemTableIdTable[Index].Enable))
        {
          // Update SSdt definition block
          UpdateSsdtDefinitionBlock((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable);

          FchKeithAcpiBlockPtr = (EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable;
          TableHandle = 0;
          TableSize = FchKeithAcpiBlockPtr->Length;
          Status = AcpiTableProtocol->InstallAcpiTable (
                                      AcpiTableProtocol,
                                      FchKeithAcpiBlockPtr,
                                      TableSize,
                                      &TableHandle
                                      );
          break;
        }
      }
      gBS->FreePool (CurrentTable);
      CurrentTable = NULL;
      Instance++;
    }
  }

  //
  // Install gFchSsdtInstalledProtocolGuid to signal Platform
  //
  Handle = ImageHandle;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gFchSsdtInstalledProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );

  return Status;
}
