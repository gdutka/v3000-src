/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Include/JedecNvdimm.h>
#include "JedecNvdimmDsm.h"
#include "IndustryStandard/Acpi.h"
#include "Protocol/AcpiSystemDescriptionTable.h"
#include <Protocol/SmmBase2.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/SmmMemLib.h>
#include "Filecode.h"
#include <Library/AmdPspDxeSmmBufLib.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE JEDECNVDIMM_SMM_JEDECNVDIMM_FILECODE

#define ACPI_MMIO_BASE   (0xFED80000ul)
#define PMIO_BASE        (0x300)         // DWORD
#define FCH_PMIOA_REG64  (0x64)          // FCH::PM::ACPIPMTMRBLK

//---------------------------------------------------------------------------

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
extern EFI_STATUS ArsNvdimmSmmInit();
extern EFI_STATUS GenerateJedecNvdimmAcpi();
extern EFI_STATUS InstallDsmSmiHandler();
extern EFI_STATUS SetEventNotifications (UINT16 TotalNvdimm);
extern EFI_STATUS JedecNvdimmSmbusInit();
extern EFI_STATUS ReadNvdimm (UINT8 Socket, UINT8 Channel, UINT8 Dimm, UINT8 Page, UINT8 Register, UINT8 *Data, BOOLEAN IsSpd);
extern EFI_STATUS WriteNvdimm (UINT8 Socket, UINT8 Channel, UINT8 Dimm, UINT8 Page, UINT8 Register, UINT8 *Data, BOOLEAN IsSpd);
extern UINT32 ResetToFactoryDefaults (UINT8 Socket, UINT8 Channel, UINT8 Dimm);
extern EFI_GUID gAmdJedecNvdimmSmmProtocolGuid;
extern EFI_GUID gJedecNvdimmClaimDsmRegionGuid;
extern EFI_GUID gAmdJedecNvdimmProtocolGuid;

DSM_ACPI_SMM_DATA       *pDsmAcpiSmmData;
ARS_ACPI_SMM_DATA       *pArsAcpiSmmData;
UINT32 gNvdimmBitMap;
EFI_HANDLE mHandle = NULL;
UINT16 gFchAcpiPmTmrBlk;

JEDEC_NVDIMM_SMM_PROTOCOL JedecNvdimmSmmProtocol = {
    ReadMemoryErrorCounters,
    SetMemoryErrorCounters,
    IncrementMemErrorCounters,
    PerformNvdimmPurge
};

JEDEC_NVDIMM_PROTOCOL JedecNvdimmProtocol = {
    NULL,
    NULL,
    0
};

EFI_GUID  JedecNvdimmAslGuid = {0xFC987C70, 0x6DAA, 0x46E7, 0xAF, 0x60, 0xB0, 0xB0, 0xF2, 0x40, 0x01, 0xB0};

/**
 * gNvdimmBitMap
 *
+---------+-----------------------------------------------+-----------------------------------------------+
| Socket  |                      S1                       |                      S0                       |
+---------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
| Channel |  C7 |  C6 |  C5 |  C4 |  C3 |  C2 |  C1 |  C0 |  C7 |  C6 |  C5 |  C4 |  C3 |  C2 |  C1 |  C0 |
+---------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| Dimm    |D1|D0|D1|D0|D1|D0|D1|D0|D1|D0|D1|D0|D1|D0|D1|D0|D1|D0|D1|D0|D1|D0|D1|D0|D1|D0|D1|D0|D1|D0|D1|D0|
+---------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| Bit     |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
+---------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
**/

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
ClaimNvdimmDsmRegionSmiHandler (
    IN EFI_HANDLE  DispatchHandle,
    IN CONST VOID  *Context,
    IN OUT VOID    *CommBuffer,
    IN OUT UINTN   *CommBufferSize
);

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
//======================================================
//  ACPI Tables header structure  for IA32
//======================================================
typedef struct _ACPI_HDR {
    UINT32      Signature;
    UINT32      Length;
    UINT8       Revision;
    UINT8       Checksum;
    UINT8       OemId[6];
    UINT8       OemTblId[8];
    UINT32      OemRev;
    UINT32      CreatorId;
    UINT32      CreatorRev;
} ACPI_HDR;

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          T A B L E    D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
  Initialize timeout for a specified amount of time in uS.

   @param Timeout - Pointer to structure, data modified by function.
   @param Time - Timeout value in uS.
   @param Status - Status returned by function.

   @return EFI_STATUS
   @retval  EFI_SUCCESS - Always return success.
*/

EFI_STATUS
StartTimeout(
    IN SMM_TIMEOUT *Timeout,
    IN UINTN        Time
    )
{
    UINT64  TicksNeeded;
    UINT64  EndValue;

    // There are 3.58 ticks per uS.
    TicksNeeded = (Time * 358) / 100;

    // Read ACPI Timer
    Timeout->OldTimerValue = IoRead32(gFchAcpiPmTmrBlk);
    EndValue = TicksNeeded + Timeout->OldTimerValue;

    // Calculate Overflow and EndValue from FullEndValue,
    // based on number of bits in ACPI Timer
    Timeout->OverFlow = (UINT32)(EndValue >> NUM_BITS_IN_ACPI_TIMER);
    Timeout->EndValue = ((UINT32)EndValue) & (UINT32)(((UINT64)1 << NUM_BITS_IN_ACPI_TIMER) - 1);

    return EFI_SUCCESS;
}

/**
  Checks if timeout has expired.

   @param Timeout - Pointer to structure, data modified by function.
   @param Status - Status returned by function.

   @return EFI_STATUS
   @retval  EFI_SUCCESS - Timer not expired.
   @retval  EFI_TIMEOUT - Timer expired.
*/

EFI_STATUS
HasItTimedOut(
    IN SMM_TIMEOUT *Timeout
    )
{
    UINTN TimerValue;

    // Read ACPI Timer
    TimerValue = IoRead32(gFchAcpiPmTmrBlk);

    if (Timeout->OverFlow > 0) {
        // See if the current timer value is less than the previous value.
        // If it is, then the timer had wrapped around.
        if (TimerValue < Timeout->OldTimerValue) {
            --Timeout->OverFlow;
        }

        // Update OldTimerValue
        Timeout->OldTimerValue = TimerValue;
        return EFI_SUCCESS;
    }

    // See if the current timer value is less than the previous value.
    // If it is, then we are done.
    if (TimerValue < Timeout->OldTimerValue) return EFI_TIMEOUT;

    // If we passed the EndValue, we are done.
    if (TimerValue >= Timeout->EndValue) return EFI_TIMEOUT;

    // Update OldTimerValue
    Timeout->OldTimerValue = TimerValue;

    return EFI_SUCCESS;
}

/**
    This routine delays for specified number of micro seconds

    @param Usec      Amount of delay (count in 1 microsec)

    @retval VOID

**/

VOID
FixedDelay(
    IN UINTN  Usec
    )
{
    UINTN   Counter, i;
    UINT32  Data32, PrevData;

    // There are 3.58 ticks per Usec
    Counter = (Usec * 358) / 100;

    //
    // Call WaitForTick for Counter + 1 ticks to try to guarantee Counter tick
    // periods, thus attempting to ensure Microseconds of stall time.
    //
    if (Counter != 0) {

        PrevData = IoRead32(gFchAcpiPmTmrBlk);
        for (i = 0; i < Counter; ) {
            Data32 = IoRead32(gFchAcpiPmTmrBlk);
            if (Data32 < PrevData) {        // Reset if there is a overlap
                PrevData = Data32;
                continue;
            }
            i += (Data32 - PrevData);
            PrevData = Data32;
        }
    }
    return;
}

/**
  Reads the DRAM_ECC_ERROR_COUNT (0x80) and DRAM_THRESHOLD_ECC_COUNT (0x81)
  NVDIMM registers

  @param  This                  JEDEC_NVDIMM_SMM_PROTOCOL instance
  @param  Socket                Socket # of NVDIMM
  @param  Channel               Channel # of NVDIMM
  @param  Dimm                  Dimm # of NVDIMM
  @param  DramEccErrorCount     Number of uncorrectable ECC errors detected
  @param  DramThresholdEccCount Number of correctable ECC threshold exceeded events

  @retval EFI_STATUS

**/

EFI_STATUS
EFIAPI
ReadMemoryErrorCounters (
    IN CONST JEDEC_NVDIMM_SMM_PROTOCOL  *This,
    IN UINT8                            Socket,
    IN UINT8                            Channel,
    IN UINT8                            Dimm,
    OUT UINT8                           *DramEccErrorCount,
    OUT UINT8                           *DramThresholdEccCount
)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINT8       Data;

    Status = ReadNvdimm(Socket, Channel, Dimm, 2, PAGE2_DRAM_ECC_ERROR_COUNT, &Data, FALSE);
    *DramEccErrorCount = Data;
    Status = ReadNvdimm(Socket, Channel, Dimm, 2, PAGE2_DRAM_THRESHOLD_ECC_COUNT, &Data, FALSE);
    *DramThresholdEccCount = Data;

    return Status;
}

/**
  Sets the DRAM_ECC_ERROR_COUNT (0x80) and DRAM_THRESHOLD_ECC_COUNT (0x81)
  NVDIMM registers

  @param  This                  JEDEC_NVDIMM_SMM_PROTOCOL instance
  @param  Socket                Socket # of NVDIMM
  @param  Channel               Channel # of NVDIMM
  @param  Dimm                  Dimm # of NVDIMM
  @param  DramEccErrorCount     Number of uncorrectable ECC errors
  @param  DramThresholdEccCount Number of correctable ECC threshold exceeded events

  @retval EFI_STATUS

**/

EFI_STATUS
EFIAPI
SetMemoryErrorCounters (
    IN CONST JEDEC_NVDIMM_SMM_PROTOCOL  *This,
    IN UINT8                            Socket,
    IN UINT8                            Channel,
    IN UINT8                            Dimm,
    IN UINT8                            DramEccErrorCount,
    IN UINT8                            DramThresholdEccCount
)
{
    EFI_STATUS  Status = EFI_SUCCESS;

    Status = WriteNvdimm(Socket, Channel, Dimm, 2, PAGE2_DRAM_ECC_ERROR_COUNT, &DramEccErrorCount, FALSE);
    Status = WriteNvdimm(Socket, Channel, Dimm, 2, PAGE2_DRAM_THRESHOLD_ECC_COUNT, &DramThresholdEccCount, FALSE);

    return Status;
}

/**
  Increments the DRAM_ECC_ERROR_COUNT (0x80) and DRAM_THRESHOLD_ECC_COUNT (0x81)
  NVDIMM registers

  @param  This                      JEDEC_NVDIMM_SMM_PROTOCOL instance
  @param  Socket                    Socket # of NVDIMM
  @param  Channel                   Channel # of NVDIMM
  @param  Dimm                      Dimm # of NVDIMM
  @param  IncDramEccErrorCount      Increment the number of uncorrectable ECC errors detected
  @param  IncDramThresholdEccCount  Increment the number of correctable ECC threshold exceeded events

  @retval EFI_SUCCESS or EFI_UNSUPPORTED

**/

EFI_STATUS
EFIAPI
IncrementMemErrorCounters (
    IN CONST JEDEC_NVDIMM_SMM_PROTOCOL  *This,
    IN UINT8                            Socket,
    IN UINT8                            Channel,
    IN UINT8                            Dimm,
    IN BOOLEAN                          IncDramEccErrorCount,
    IN BOOLEAN                          IncDramThresholdEccCount
)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINT8       Data;

    if (IncDramEccErrorCount) {
        Status = ReadNvdimm(Socket, Channel, Dimm, 2, PAGE2_DRAM_ECC_ERROR_COUNT, &Data, FALSE);
        if (Data < 255) {
            Data += 1;
            Status = WriteNvdimm(Socket, Channel, Dimm, 2, PAGE2_DRAM_ECC_ERROR_COUNT, &Data, FALSE);
            DEBUG ((EFI_D_ERROR, "IncrementMemErrorCounters: New DramEccErrorCount = %d\n", Data));
        }
        else {
            DEBUG ((EFI_D_ERROR, "IncrementMemErrorCounters ERROR: DramEccErrorCount at max 255\n"));
            return EFI_UNSUPPORTED;
        }
    }

    if (IncDramThresholdEccCount) {
        Status = ReadNvdimm(Socket, Channel, Dimm, 2, PAGE2_DRAM_THRESHOLD_ECC_COUNT, &Data, FALSE);
        if (Data < 255) {
            Data += 1;
            Status = WriteNvdimm(Socket, Channel, Dimm, 2, PAGE2_DRAM_THRESHOLD_ECC_COUNT, &Data, FALSE);
            DEBUG ((EFI_D_ERROR, "IncrementMemErrorCounters: New DramThresholdEccCount = %d\n", Data));
        }
        else {
            DEBUG ((EFI_D_ERROR, "IncrementMemErrorCounters ERROR: DramThresholdEccCount at max 255\n"));
            return EFI_UNSUPPORTED;
        }
    }

    return Status;
}

/**
  Executes the JEDEC Restore Factory Defaults function

  @param  This                  JEDEC_NVDIMM_SMM_PROTOCOL instance
  @param  Socket                Socket # of NVDIMM
  @param  Channel               Channel # of NVDIMM
  @param  Dimm                  Dimm # of NVDIMM

  @retval EFI_SUCCESS or EFI_TIMEOUT or EFI_DEVICE_ERROR

**/

EFI_STATUS
EFIAPI
PerformNvdimmPurge (
    IN CONST JEDEC_NVDIMM_SMM_PROTOCOL  *This,
    IN UINT8                            Socket,
    IN UINT8                            Channel,
    IN UINT8                            Dimm
)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINT32      DsmStatus;
    UINT8       Data;

    DsmStatus = ResetToFactoryDefaults(Socket, Channel, Dimm);
    Status = ReadNvdimm(Socket, Channel, Dimm, 0, PAGE0_FACTORY_DEFAULT_STATUS0, &Data, FALSE);

    if (DsmStatus == ((1 << 16) | DSM_FUNCTION_ERROR_CODE)) {
        DEBUG ((EFI_D_ERROR, "PerformNvdimmPurge ERROR: Factory Default operation timed out\n"));
        return EFI_TIMEOUT;
    }
    else if (DsmStatus == DSM_FUNCTION_ERROR_CODE) {
        DEBUG ((EFI_D_ERROR, "PerformNvdimmPurge ERROR: Unable to start Factory Default operation\n"));
        return EFI_DEVICE_ERROR;
    }
    else if (Data & BIT1) {
        DEBUG ((EFI_D_ERROR, "PerformNvdimmPurge ERROR: Factory Default operation failed\n"));
        return EFI_DEVICE_ERROR;
    }

    DEBUG ((EFI_D_ERROR, "PerformNvdimmPurge SUCCESS!\n"));
    return Status;
}

/**
    JedecNvdimm Driver Entry Point

    @param    ImageHandle
    @param    *SystemTable

    @retval   EFI_SUCCESS or EFI_NOT_FOUND

**/

EFI_STATUS
EFIAPI
JedecNvdimmEntryPoint (
    IN  EFI_HANDLE              ImageHandle,
    IN  EFI_SYSTEM_TABLE        *SystemTable
)
{
    EFI_STATUS              Status;
    EFI_SMM_BASE2_PROTOCOL  *SmmBase2;
    EFI_PHYSICAL_ADDRESS    DsmAcpiSmmDataAddress;
    EFI_PHYSICAL_ADDRESS    ArsAcpiSmmDataAddress;
    UINT8                   *CurrentPtr;
    UINT32                  *CurrentSig;
    UINT16                  TotalNvdimm;
    EFI_HANDLE              Handle = NULL;
    EFI_ACPI_TABLE_PROTOCOL *AcpiTableProtocol;
    UINTN                   NumberOfHandles;
    UINTN                   TableHandle;
    UINTN                   TableSize;
    UINT32                  FvStatus;
    EFI_HANDLE              *HandleBuffer;
    UINTN                   Index;
    UINTN                   Size;
    EFI_FV_FILETYPE         FileType;
    EFI_FV_FILE_ATTRIBUTES  Attributes;
    EFI_ACPI_COMMON_HEADER  *CurrentTable;
    EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol = NULL;
    UINT32                  SsdtSize;
    INTN                    Instance;
    EFI_ACPI_DESCRIPTION_HEADER  *SsdtNvdimmNPtr = NULL;

    DEBUG ((EFI_D_ERROR, "JedecNvdimm Entry\n"));

    // Get PMx00000064 (FCH::PM::ACPIPMTMRBLK) IO register
    gFchAcpiPmTmrBlk = MmioRead16 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG64);

    Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID**)&SmmBase2);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = SmmBase2->GetSmstLocation (SmmBase2, &gSmst);
    if (EFI_ERROR (Status)) {
        return EFI_UNSUPPORTED;
    }

    // Initialize NVDIMM bitmap
    gNvdimmBitMap = 0;

    //
    // Initial Smbus variables
    //
    Status = JedecNvdimmSmbusInit ();
    ASSERT_EFI_ERROR (Status);

    //
    // Allocate ACPI to SMM data buffer
    //
    DsmAcpiSmmDataAddress = 0xFFFFFFFF;
    Status = gBS->AllocatePages (
                AllocateMaxAddress,
                EfiACPIMemoryNVS,
                EFI_SIZE_TO_PAGES (sizeof(DSM_ACPI_SMM_DATA)),
                &DsmAcpiSmmDataAddress);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Unable to allocate ACPI NVS memory!\n"));
        return Status;
    }

    pDsmAcpiSmmData = (DSM_ACPI_SMM_DATA *)DsmAcpiSmmDataAddress;
    DEBUG ((EFI_D_ERROR, "[AGESA NVDIMM] pDsmAcpiSmmData: 0x%08x\n", pDsmAcpiSmmData));
    gBS->SetMem (pDsmAcpiSmmData, sizeof(DSM_ACPI_SMM_DATA), 0);

    //
    // Allocate ACPI to SMM data buffer
    //
    ArsAcpiSmmDataAddress = 0xFFFFFFFF;
    Status = gBS->AllocatePages (
                AllocateMaxAddress,
                EfiACPIMemoryNVS,
                EFI_SIZE_TO_PAGES (sizeof(ARS_ACPI_SMM_DATA)),
                &ArsAcpiSmmDataAddress);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Root NVDIMM Device: Unable to allocate ACPI NVS memory!\n"));
        return Status;
    }

    pArsAcpiSmmData = (ARS_ACPI_SMM_DATA *)ArsAcpiSmmDataAddress;
    DEBUG ((EFI_D_ERROR, "[AGESA NVDIMM] pArsAcpiSmmData: 0x%08x\n", pArsAcpiSmmData));

    gBS->SetMem (pArsAcpiSmmData, sizeof(ARS_ACPI_SMM_DATA), 0);

    Status = gBS->LocateProtocol (
                    &gEfiAcpiTableProtocolGuid,
                    NULL,
                    &AcpiTableProtocol
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
                        &JedecNvdimmAslGuid,
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
      Status = FwVol->ReadSection(
                        FwVol,
                        &JedecNvdimmAslGuid,
                        EFI_SECTION_RAW,
                        Instance,
                        &CurrentTable,
                        &Size,
                        &FvStatus
                        );

      SsdtSize = ((ACPI_HDR *) CurrentTable)->Length;

      if (!EFI_ERROR (Status)) {
        if (((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId == SIGNATURE_64 ('N', 'V', 'D', 'I', 'M', 'M', '-', 'N')) {
          SsdtNvdimmNPtr = (EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable;
          CurrentPtr = (UINT8 *) CurrentTable;

          // Search for ('N', 'V', 'D', 'M')
          Status = EFI_NOT_FOUND;
          while ((UINTN) CurrentPtr < ((UINTN) CurrentPtr + (UINTN) SsdtSize)) {
            CurrentSig = (UINT32 *) CurrentPtr;
            if (*CurrentSig == SIGNATURE_32 ('N', 'V', 'D', 'M')) {
                //DEBUG ((EFI_D_ERROR, "SSDT NVDM Address = %08X\n", CurrentPtr));
                *(UINT32 *)CurrentPtr = (UINT32)(UINTN)pDsmAcpiSmmData;
                Status = EFI_SUCCESS;
                break;
            }
            CurrentPtr++;
          }
          ASSERT_EFI_ERROR (Status);

          // Search for ('A', 'R', 'S', 'D')
          CurrentPtr = (UINT8 *) CurrentTable;
          Status = EFI_NOT_FOUND;
          while ((UINTN) CurrentPtr < ((UINTN) CurrentPtr + (UINTN) SsdtSize)) {
              CurrentSig = (UINT32 *) CurrentPtr;
              if (*CurrentSig == SIGNATURE_32 ('A', 'R', 'S', 'D')) {
                  //DEBUG ((EFI_D_ERROR, "SSDT ARSD Address = %08X\n", CurrentPtr));
                  *(UINT32 *)CurrentPtr = (UINT32)(UINTN)pArsAcpiSmmData;
                  Status = EFI_SUCCESS;
                  break;
              }
              CurrentPtr++;
          }
          ASSERT_EFI_ERROR (Status);

          TableSize = SsdtNvdimmNPtr->Length;

          //
          // Install ACPI table
          //
          TableHandle = 0;
          Status = AcpiTableProtocol->InstallAcpiTable (
                                        AcpiTableProtocol,
                                        SsdtNvdimmNPtr,
                                        TableSize,
                                        &TableHandle
                                        );
          DEBUG ((EFI_D_ERROR, "NVDIMM-N SSDT Install Status = %r\n", Status));
        }

        Status = gBS->FreePool (CurrentTable);
        CurrentTable = NULL;
        Instance++;
      }
    }

    //
    // Create NFIT tables
    //
    Status = GenerateJedecNvdimmAcpi(&TotalNvdimm);
    pDsmAcpiSmmData->NvdimmBitMap = gNvdimmBitMap;
    pArsAcpiSmmData->NvdimmBitMap = gNvdimmBitMap;
    ASSERT_EFI_ERROR (Status);
    if (TotalNvdimm == 0) {
        DEBUG ((EFI_D_ERROR, "No NVDIMMs found, JedecNvdimm Exit\n"));
        return EFI_SUCCESS;
    }

    //
    // Install ARS NVDIMM SMM Protocol
    //
    Status = ArsNvdimmSmmInit();
    ASSERT_EFI_ERROR (Status);

    //
    // Install DSM SMI handler
    //
    Status = InstallDsmSmiHandler();
    ASSERT_EFI_ERROR (Status);

    //
    // Set NVDIMM event notifications
    //
    Status = SetEventNotifications(TotalNvdimm);
    ASSERT_EFI_ERROR (Status);

    //
    // Install JEDEC NVDIMM SMM Protocol
    //
    Status = gSmst->SmmInstallProtocolInterface (
        &Handle,
        &gAmdJedecNvdimmSmmProtocolGuid,
        EFI_NATIVE_INTERFACE,
        &JedecNvdimmSmmProtocol
        );
    ASSERT_EFI_ERROR (Status);


    //
    // Install JEDEC NVDIMM Protocol
    //
    Handle = NULL;
    JedecNvdimmProtocol.DsmAcpiSmmData = pDsmAcpiSmmData;
    JedecNvdimmProtocol.ArsAcpiSmmData = pArsAcpiSmmData;
    JedecNvdimmProtocol.NvdimmBitMap = gNvdimmBitMap;
    Status = gBS->InstallProtocolInterface (
        &Handle,
        &gAmdJedecNvdimmProtocolGuid,
        EFI_NATIVE_INTERFACE,
        &JedecNvdimmProtocol
        );
    ASSERT_EFI_ERROR (Status);

    //
    // Install ClaimNvdimmDsmRegionSmiHandler
    //
    Status = gSmst->SmiHandlerRegister (
                    ClaimNvdimmDsmRegionSmiHandler,
                    &gJedecNvdimmClaimDsmRegionGuid,
                    &mHandle
                    );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "Fail to register ClaimNvdimmDsmRegionSmiHandler\n"));
    }

    DEBUG ((EFI_D_ERROR, "JedecNvdimm Exit\n"));

    return Status;
}

EFI_STATUS
ClaimNvdimmDsmRegionSmiHandler (
   IN    EFI_HANDLE  DispatchHandle,
   IN    CONST VOID  *Context,
   IN OUT    VOID *CommBuffer,
   IN OUT    UINTN *CommBufferSize
  )
{
   EFI_STATUS Status = EFI_SUCCESS;
   UINTN TempCommBufferSize;

   DEBUG((EFI_D_INFO, "ClaimDsmRegion SMI handler Start\n"));

   //
   // If input is invalid, stop processing this SMI
   //
   if (CommBuffer == NULL || CommBufferSize == NULL) {
     return EFI_INVALID_PARAMETER;
   }

   TempCommBufferSize = *CommBufferSize;

   // too small buffer size may allow TOC/TOU attack, too big buffer size may cause stack overflow or heap overwritten
   if(TempCommBufferSize < sizeof (JEDEC_NVDIMM_PROTOCOL) || TempCommBufferSize > AMD_PSP_SMM_TMP_BUFFER_SIZE) {
     DEBUG((EFI_D_ERROR, "ClaimNvdimmDsmRegionSmiHandler Command Buffer Size invalid!\n"));
     return EFI_INVALID_PARAMETER;
   }

   if (!SmmIsBufferOutsideSmmValid ((UINTN)CommBuffer, TempCommBufferSize)) {
     DEBUG((EFI_D_ERROR, "ClaimNvdimmDsmRegionSmiHandler: SMM communication data buffer in SMRAM or overflow!\n"));
     return EFI_INVALID_PARAMETER;
   }

   ((JEDEC_NVDIMM_PROTOCOL *)CommBuffer)->DsmAcpiSmmData = pDsmAcpiSmmData;
   ((JEDEC_NVDIMM_PROTOCOL *)CommBuffer)->NvdimmBitMap = gNvdimmBitMap;

   DEBUG((EFI_D_INFO, "ClaimDsmRegion: DsmAcpiSmmData = 0x%x,  NvdimmBitMap = 0x%08x\n",
          ((JEDEC_NVDIMM_PROTOCOL *)CommBuffer)->DsmAcpiSmmData,
          ((JEDEC_NVDIMM_PROTOCOL *)CommBuffer)->NvdimmBitMap
          ));

   // Unregister SMI handler
   if (mHandle != NULL) {
     Status = gSmst->SmiHandlerUnRegister (mHandle);
   }

   return Status;
}


