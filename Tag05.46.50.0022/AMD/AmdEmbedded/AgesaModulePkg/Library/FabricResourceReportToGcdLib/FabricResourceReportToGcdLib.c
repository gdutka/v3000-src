/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Report used MMIO region to GCD service
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Library
 * @e \$Revision$   @e \$Date$
 *
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "Porting.h"
#include "AMD.h"
#include "Filecode.h"
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/AmdHeapLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/FabricResourceManagerLib.h>

#define FILECODE LIBRARY_FABRICRESOURCEREPORTTOGCDLIB_FABRICRESOURCEREPORTTOGCDLIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define X86IO_LIMIT                         0x10000    // X86 IO Limit
#define X86_LEGACY_IO_SIZE                  0x1000     // IO size which is reserved for legacy devices
#define FABRIC_MMIO_ATTRIBUTES              (EFI_MEMORY_UC | EFI_MEMORY_RUNTIME)

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * FabricResourceReportToGcd
 *
 * Report MMIO regions to GCD
 *
 */
EFI_STATUS
FabricResourceReportToGcd (
  )
{
  UINT8                     i;
  UINT8                     j;
  UINT8                     TempSocket;
  UINT8                     TempRb;
  UINT64                    Base;
  UINT64                    Size;
  EFI_STATUS                Status;
  EFI_STATUS                CalledStatus;
  LOCATE_HEAP_PTR           LocateHeapParams;
  FABRIC_MMIO_MANAGER      *FabricMmioManager;

  Status = EFI_SUCCESS;
  FabricMmioManager = NULL;

  IDS_HDT_CONSOLE (CPU_TRACE, "Report MMIO region to GCD\n");

  // Find MMIO manager in heap
  LocateHeapParams.BufferHandle = AMD_MMIO_MANAGER;
  if (HeapLocateBuffer (&LocateHeapParams, NULL) == AGESA_SUCCESS) {
    FabricMmioManager = (FABRIC_MMIO_MANAGER *) LocateHeapParams.BufferPtr;
  }

  if (FabricMmioManager == NULL) {
    IDS_HDT_CONSOLE (CPU_TRACE, "FabricMmioManager locate wrong.\n");
    return EFI_NOT_FOUND;
  }

  // report reserved region (for primary FCH)
  Base = PcdGet32 (PcdAmdBottomMmioReservedForPrimaryRb);
  Size = 0x100000000 - Base;
  CalledStatus = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                                      Base,
                                      Size,
                                      FABRIC_MMIO_ATTRIBUTES);
  Status = (CalledStatus > Status) ? CalledStatus : Status;
  IDS_HDT_CONSOLE (CPU_TRACE, "  Add reserved MMIO region base 0x%lX size 0x%lX, Status: %x\n", Base, Size, Status);

  CalledStatus = gDS->AllocateMemorySpace (EfiGcdAllocateAddress,
                                           EfiGcdMemoryTypeMemoryMappedIo,
                                           0,
                                           Size,
                                           &Base,
                                           (EFI_HANDLE) FabricMmioManager->ImageHandle,
                                           NULL);
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  gDS->SetMemorySpaceAttributes (Base, Size, FABRIC_MMIO_ATTRIBUTES);
  Status = (CalledStatus > Status) ? CalledStatus : Status;
  IDS_HDT_CONSOLE (CPU_TRACE, "  Allocate reserved MMIO region base 0x%lX size 0x%lX, Status: %x\n", Base, Size, CalledStatus);

  // MMIO Resource to GCD
  for (i = 0; i < MAX_SOCKETS_SUPPORTED; i++) {
    for (j = 0; j < MAX_RBS_PER_SOCKET; j++) {
      if (FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci != 0) {
        CalledStatus = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                                            FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci,
                                            FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci,
                                            FABRIC_MMIO_ATTRIBUTES);
        Status = (CalledStatus > Status) ? CalledStatus : Status;

        CalledStatus = gDS->AllocateMemorySpace (EfiGcdAllocateAddress,
                                                 EfiGcdMemoryTypeMemoryMappedIo,
                                                 0,
                                                 FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci,
                                                 &FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci,
                                                 (EFI_HANDLE) FabricMmioManager->ImageHandle,
                                                 NULL);
        Status = (CalledStatus > Status) ? CalledStatus : Status;

        CalledStatus = gDS->SetMemorySpaceAttributes (FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci,
                                                      FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci,
                                                      FABRIC_MMIO_ATTRIBUTES);
        Status = (CalledStatus > Status) ? CalledStatus : Status;
        IDS_HDT_CONSOLE (CPU_TRACE, "  Allocate Non-Pci MMIO region base 0x%lX size 0x%lX, Status: %x\n", \
                         FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPci, FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPci, CalledStatus);
      }

      if (FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch != 0) {
        CalledStatus = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                                            FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch,
                                            FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch,
                                            FABRIC_MMIO_ATTRIBUTES);
        Status = (CalledStatus > Status) ? CalledStatus : Status;
        IDS_HDT_CONSOLE (CPU_TRACE, "  Allocate Non-Prefetch MMIO region base 0x%lX size 0x%lX, Status: %x\n", \
                         FabricMmioManager->MmioRegionBelow4G[i][j].BaseNonPrefetch, FabricMmioManager->MmioRegionBelow4G[i][j].SizeNonPrefetch, CalledStatus);
      }

      if (FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch != 0) {
        CalledStatus = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                                            FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch,
                                            FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch,
                                            FABRIC_MMIO_ATTRIBUTES);
        Status = (CalledStatus > Status) ? CalledStatus : Status;
        IDS_HDT_CONSOLE (CPU_TRACE, "  Allocate Prefetch MMIO region base 0x%lX size 0x%lX, Status: %x\n", \
                         FabricMmioManager->MmioRegionBelow4G[i][j].BasePrefetch, FabricMmioManager->MmioRegionBelow4G[i][j].SizePrefetch, CalledStatus);
      }

      if (FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPci != 0) {
        CalledStatus = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                                            FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPci,
                                            FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPci,
                                            FABRIC_MMIO_ATTRIBUTES);
        Status = (CalledStatus > Status) ? CalledStatus : Status;

        CalledStatus = gDS->AllocateMemorySpace (EfiGcdAllocateAddress,
                                                 EfiGcdMemoryTypeMemoryMappedIo,
                                                 0,
                                                 FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPci,
                                                 &FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPci,
                                                 (EFI_HANDLE) FabricMmioManager->ImageHandle,
                                                 NULL);
        Status = (CalledStatus > Status) ? CalledStatus : Status;

        CalledStatus = gDS->SetMemorySpaceAttributes (FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPci,
                                                        FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPci,
                                                        FABRIC_MMIO_ATTRIBUTES);
        Status = (CalledStatus > Status) ? CalledStatus : Status;
        IDS_HDT_CONSOLE (CPU_TRACE, "  Allocate Non-Pci MMIO region base 0x%lX size 0x%lX, Status: %x\n", \
                         FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPci, FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPci, CalledStatus);
      }

      if (FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPrefetch != 0) {
        CalledStatus = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                                            FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPrefetch,
                                            FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPrefetch,
                                            FABRIC_MMIO_ATTRIBUTES);
        Status = (CalledStatus > Status) ? CalledStatus : Status;
        IDS_HDT_CONSOLE (CPU_TRACE, "  Allocate Non-Prefetch MMIO region base 0x%lX size 0x%lX, Status: %x\n", \
                         FabricMmioManager->MmioRegionAbove4G[i][j].BaseNonPrefetch, FabricMmioManager->MmioRegionAbove4G[i][j].SizeNonPrefetch, CalledStatus);
      }

      if (FabricMmioManager->MmioRegionAbove4G[i][j].SizePrefetch != 0) {
        CalledStatus = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                                            FabricMmioManager->MmioRegionAbove4G[i][j].BasePrefetch,
                                            FabricMmioManager->MmioRegionAbove4G[i][j].SizePrefetch,
                                            FABRIC_MMIO_ATTRIBUTES);
        Status = (CalledStatus > Status) ? CalledStatus : Status;
        IDS_HDT_CONSOLE (CPU_TRACE, "  Allocate Prefetch MMIO region base 0x%lX size 0x%lX, Status: %x\n", \
                         FabricMmioManager->MmioRegionAbove4G[i][j].BasePrefetch, FabricMmioManager->MmioRegionAbove4G[i][j].SizePrefetch, CalledStatus);
      }
    }
  }

  // Check primary Rb's 2nd MMIO
  if (FabricMmioManager->PrimaryRbHas2ndMmioBelow4G) {
    TempSocket = (FabricMmioManager->PrimaryRb2ndMmioPairBelow4G >> 4) & 0xF;
    TempRb = FabricMmioManager->PrimaryRb2ndMmioPairBelow4G & 0xF;
    IDS_HDT_CONSOLE (CPU_TRACE, " Primary Rb's 2nd Non-Pci MMIO region at Socket:%d,RootBridge:%d\n", TempSocket, TempRb);

    if (FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].SizeNonPci != 0) {
      CalledStatus = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                                          FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].BaseNonPci,
                                          FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].SizeNonPci,
                                          FABRIC_MMIO_ATTRIBUTES);
      Status = (CalledStatus > Status) ? CalledStatus : Status;

      CalledStatus = gDS->AllocateMemorySpace (EfiGcdAllocateAddress,
                                               EfiGcdMemoryTypeMemoryMappedIo,
                                               0,
                                               FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].SizeNonPci,
                                               &FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].BaseNonPci,
                                               (EFI_HANDLE) FabricMmioManager->ImageHandle,
                                               NULL);
      Status = (CalledStatus > Status) ? CalledStatus : Status;

      CalledStatus = gDS->SetMemorySpaceAttributes (FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].BaseNonPci,
                                                    FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].SizeNonPci,
                                                    FABRIC_MMIO_ATTRIBUTES);
      Status = (CalledStatus > Status) ? CalledStatus : Status;
      IDS_HDT_CONSOLE (CPU_TRACE, "  Allocate primary Rb's 2nd Non-Pci MMIO region base 0x%lX size 0x%lX, Status: %x\n", \
                       FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].BaseNonPci, FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].SizeNonPci, CalledStatus);
    }

    if (FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].SizeNonPrefetch != 0) {
      CalledStatus = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                                          FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].BaseNonPrefetch,
                                          FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].SizeNonPrefetch,
                                          FABRIC_MMIO_ATTRIBUTES);
      Status = (CalledStatus > Status) ? CalledStatus : Status;
      IDS_HDT_CONSOLE (CPU_TRACE, "  Allocate primary Rb's 2nd Non-Prefetch MMIO region base 0x%lX size 0x%lX, Status: %x\n", \
                       FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].BaseNonPrefetch, FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].SizeNonPrefetch, CalledStatus);
    }

    if (FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].SizePrefetch != 0) {
      CalledStatus = gDS->AddMemorySpace (EfiGcdMemoryTypeMemoryMappedIo,
                                          FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].BasePrefetch,
                                          FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].SizePrefetch,
                                          FABRIC_MMIO_ATTRIBUTES);
      Status = (CalledStatus > Status) ? CalledStatus : Status;
      IDS_HDT_CONSOLE (CPU_TRACE, "  Allocate primary Rb's 2nd Prefetch MMIO region base 0x%lX size 0x%lX, Status: %x\n", \
                       FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].BasePrefetch, FabricMmioManager->MmioRegionBelow4G[TempSocket][TempRb].SizePrefetch, CalledStatus);
    }
  }

  //IO Resource to GCD
  CalledStatus = gDS->AddIoSpace (EFI_RESOURCE_IO,
                                      (0 + X86_LEGACY_IO_SIZE),
                                      (X86IO_LIMIT - X86_LEGACY_IO_SIZE));
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  FabricMmioManager->ReportToGcd = TRUE;

  return Status;
}


/*---------------------------------------------------------------------------------------*/
/**
 * ReportMmioToGcd
 *
 *
 */
EFI_STATUS
ReportMmioToGcd (
  IN       UINT64 *BaseAddress,
  IN       UINT64 Length,
  IN       FABRIC_MMIO_MANAGER *FabricMmioManager
  )
{
  EFI_STATUS Status;

  Status = gDS->AllocateMemorySpace (EfiGcdAllocateAddress,
                                     EfiGcdMemoryTypeMemoryMappedIo,
                                     0,
                                     Length,
                                     BaseAddress,
                                     (EFI_HANDLE) FabricMmioManager->ImageHandle,
                                     NULL);
  gDS->SetMemorySpaceAttributes (*BaseAddress, Length, FABRIC_MMIO_ATTRIBUTES);
  IDS_HDT_CONSOLE (CPU_TRACE, "  report to GCD, Status: %x\n", Status);

  return Status;
}

