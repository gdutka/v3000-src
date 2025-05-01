/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include "MemTest.h"
#include <AMD.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/TimerLib.h>
#include <GnbDxio.h>
#include <Library/NbioSmuV13Lib.h>
#include <Library/SmnAccessLib.h>

//#pragma optimize ("", off)


#define AMD_MEM_TEST_VARIABLE_NAME  L"MEM_TEST_RESULT"
STATIC EFI_GUID mAmdMemTestResultVariableGuid = {
                  0xB07FE30F, 0xA2D3, 0x497C, {0xB6, 0xE4, 0x17, 0x32, 0x6B, 0x2F, 0x24, 0x5A}};

typedef struct MemTestResultVariable {
  UINT64 TestRunCount;
  UINT64 MemTestResult[2];
} MEMTEST_RESULT_VARIABLE;

MEMTEST_RESULT_VARIABLE gMemTestResultVar;

EFI_MP_SERVICES_PROTOCOL  *gMpServices;

UINTN gNumberOfProcessors;

UINT8 *gMonitorLineReservation;
volatile UINT8 *gAlignMonitorLine;

UINT64  gMemTestResult = 0;         // Bit map of logical processors that failed test.
UINTN   *gMemTestErrorCounter;      //Memory test error counter...

// This state machine requires APs to execute single threaded.
#define AP_STATE_BSP_EXECUTE_MEM_TEST 0
#define AP_STATE_AP_INITIAL_STATE     1
#define AP_START_AP_START_MEM_TEST    2
#define AP_START_AP_EXECUTE_MEM_TEST  3
volatile UINT8 gApState;

VOID StartTest (VOID);

VOID AsmDeadLoop (VOID);

// Allocate global variables and initialize.
VOID InitializeGlobalData ()
{
  EFI_STATUS Status;
  UINTN  i;
  UINTN  NumberOfEnabledProcessors;
  UINT32 RegEbx;
  UINT32 MonitorLineSize;

  MemTestSetup ();

  Status = gMpServices->GetNumberOfProcessors (gMpServices, &gNumberOfProcessors, &NumberOfEnabledProcessors);
  ASSERT_EFI_ERROR (Status);

  gMemTestErrorCounter = AllocatePool (gNumberOfProcessors * sizeof (UINTN));
  for (i = 0; i < gNumberOfProcessors; i++) {
    gMemTestErrorCounter[i] = 0;
  }

  // Align a large block for now.
  AsmCpuid (5, NULL, &RegEbx, NULL, NULL);
  MonitorLineSize = (UINT16)RegEbx;

  // Make sure monitored line doesn't overlap two cache lines, so double allocation.
  gMonitorLineReservation = AllocatePool (MonitorLineSize * 2);
  gAlignMonitorLine = (UINT8*)((UINTN)gMonitorLineReservation + MonitorLineSize);
}

// Free allocate memory
VOID CleanUp ()
{
  MemTestCleanup ();
  FreePool (gMonitorLineReservation);
}

VOID
WaitOnAps (VOID)
{
  BOOLEAN InterruptState;

 *gAlignMonitorLine = 0;

  InterruptState = SaveAndDisableInterrupts ();

  // Release APs.
  gApState = AP_START_AP_START_MEM_TEST;

  // Wait for AP signal to start.
  while (gApState != AP_START_AP_EXECUTE_MEM_TEST) {
    CpuPause ();
  }

  // Put BSP to sleep. Wait until APs finish.
  while (*gAlignMonitorLine == 0) {
    AsmMonitor ((UINTN)gAlignMonitorLine, 0, 0);
    if (*gAlignMonitorLine == 0) {
      AsmMwait (0, 0);
    }
  }

  if (InterruptState) {
    EnableInterrupts ();
  }
}

// Execute memory test. It will called on each each logical processor.
VOID
ExecuteMemoryTest (
  IN OUT VOID  *Buffer
)
{
  UINTN   ProcessorNumber;

  gMpServices->WhoAmI (gMpServices, &ProcessorNumber);

  //
  // Unless BSP is executing test, Syncronize AP with BSP then BSP will sleep.
  //
  if (gApState != AP_STATE_BSP_EXECUTE_MEM_TEST) {

    // Syncronize BSP and APs
    while (gApState != AP_START_AP_START_MEM_TEST) {
      CpuPause ();
    }
    gApState = AP_START_AP_EXECUTE_MEM_TEST;
  }

  //
  // Run memory Test
  //
  gMemTestErrorCounter[ProcessorNumber] = MemTestRun ();

  //
  // Report errors
  //
  if (gMemTestErrorCounter[ProcessorNumber] > 0) {
    gMemTestResult |= LShiftU64(1, ProcessorNumber);
  }

  if (gApState == AP_STATE_BSP_EXECUTE_MEM_TEST) {
    return;
  }

  //
  // Wake BSP
  //
  *gAlignMonitorLine = 1;
}

EFI_STATUS
EFIAPI
SetMemTestResultVariable (
  IN UINT64 MemTestResult
  )
{
  EFI_STATUS                          Status;

  if ((gMemTestResultVar.TestRunCount == 0) && (MemTestResult != 0)) {
    gMemTestResultVar.MemTestResult[gMemTestResultVar.TestRunCount] = 0xFFFF;
  } else {
    gMemTestResultVar.MemTestResult[gMemTestResultVar.TestRunCount] = MemTestResult;
  }
  gMemTestResultVar.TestRunCount++;

  DEBUG((DEBUG_INFO, "gMemTestResultVar.TestRunCount = %x\n", gMemTestResultVar.TestRunCount));
  DEBUG((DEBUG_INFO, "gMemTestResultVar.MemTestResult[0] = %x\n", gMemTestResultVar.MemTestResult[0]));
  DEBUG((DEBUG_INFO, "gMemTestResultVar.MemTestResult[1] = %x\n", gMemTestResultVar.MemTestResult[1]));

  Status = gRT->SetVariable (
                AMD_MEM_TEST_VARIABLE_NAME,
                &mAmdMemTestResultVariableGuid,
                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                sizeof (gMemTestResultVar),
                &gMemTestResultVar
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to set MemTestResult variable\n"));
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}

// Display messages on the screen.
VOID
ReadyToBootCallback (
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
  )
{
#if SHOW_ERROR_MESSAGE_ON_SCREEN
  CHAR16        OutputString[50]; //50 characters should be enough to hold pass/failed messaged for each CPU thread.
  STATIC CONST CHAR16  MemTestPassedString[] = L"CPU %d Memory Test passed.\n";
  STATIC CONST CHAR16  MemTestFailedString[] = L"CPU %d Memory Test failed.\n";
  STATIC CONST CHAR16  MemTestErrorCountString[] = L"Error Count = %d\n";
  EFI_INPUT_KEY Key;
  EFI_STATUS    Status;
  UINTN         i;
#endif

  StartTest ();

#if SHOW_ERROR_MESSAGE_ON_SCREEN == 1
  if (gST->ConOut == NULL) {
    return;
  }

  for (i = 0; i < gNumberOfProcessors; i++) {
    if (gMemTestResult & LShiftU64 (1, i)) {
      UnicodeSPrint (OutputString, sizeof (OutputString), MemTestFailedString, i);
    } else {
      UnicodeSPrint (OutputString, sizeof (OutputString), MemTestPassedString, i);
    }
    gST->ConOut->OutputString (gST->ConOut, OutputString);

    UnicodeSPrint (OutputString, sizeof (OutputString), MemTestErrorCountString, gMemTestErrorCounter[i]);
    gST->ConOut->OutputString (gST->ConOut, OutputString);
  }

  if (gST->ConIn == NULL) {
    return;
  }

  gST->ConOut->OutputString (gST->ConOut, L"\nPress any key to continue.\n");

  do {
    Status = gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
  } while (Status == EFI_NOT_READY);
#endif

  if (gMemTestResult) {
    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
  }

  gBS->CloseEvent (Event);
}


VOID StartTest (VOID)
{
  EFI_STATUS  Status;
  UINTN       ProcessorNumber;
  EFI_EVENT   WaitEvent;
  UINTN       i;

#if TEST_THREAD_2 == 0
  UINT32      RegEax;
  BOOLEAN     SmtEnabled;
#endif

  InitializeGlobalData ();

  Status = gMpServices->WhoAmI (gMpServices, &ProcessorNumber);
  ASSERT_EFI_ERROR (Status);

  //                                                                   //
  // Event is signaled by MP Services very late, so do not close this. //
  //                                                                   //
  Status = gBS->CreateEvent (
            0,
            TPL_CALLBACK,
            NULL,
            NULL,
            &WaitEvent
          );
  ASSERT_EFI_ERROR (Status);

#if TEST_THREAD_2 == 0
  AsmCpuidEx (0x8000001D, 0, &RegEax, NULL, NULL, NULL);
  SmtEnabled = ((RegEax >> 14) & 0xFFF) > 0;
#endif

  for (i = 0; i < gNumberOfProcessors; i++) {
#if TEST_THREAD_2 == 0
    if (SmtEnabled && (i % 2) == 1) {
      DEBUG ((DEBUG_INFO, "Skip CPU\n"));
      continue; // Skip SMT thread 1.
    }
#endif

    if (i == ProcessorNumber) {
      gApState = AP_STATE_BSP_EXECUTE_MEM_TEST;
      ExecuteMemoryTest (NULL);
    } else {
      gApState = AP_STATE_AP_INITIAL_STATE;

      DEBUG ((DEBUG_INFO, "ProcessorNumber = %d\n", i));

      gMpServices->StartupThisAP (
        gMpServices,
        ExecuteMemoryTest,
        i,
        WaitEvent,
        0,
        NULL,
        NULL
      );
      WaitOnAps ();
    }
  }

#if ADJUST_PSM == 1
  SetMemTestResultVariable (gMemTestResult);
#endif

  DEBUG ((DEBUG_ERROR, "Memory test results\n"));

  for (i = 0; i < gNumberOfProcessors; i++) {
    DEBUG ((DEBUG_ERROR, "processor = %d, result= %x\n", i, (gMemTestResult & LShiftU64(1, i)) == 0));
  }

  CleanUp (); // Frees allocated memory.
}



EFI_STATUS
EFIAPI
MemTestEntryPoint (
  IN  EFI_HANDLE           ImageHandle,
  IN  EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_EVENT   ReadyToBootEvent;
  EFI_STATUS  Status;
#if ADJUST_PSM == 1
  UINTN       BufferSize;
  UINT32      SmnData32;
  UINT32      Value;
#endif

  DEBUG ((DEBUG_INFO, "MemTestEntryPoint\n"));

  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **) &gMpServices
                  );
  ASSERT_EFI_ERROR (Status);

#if ADJUST_PSM == 1
  BufferSize = sizeof (gMemTestResultVar);
  Status = gRT->GetVariable (
                  AMD_MEM_TEST_VARIABLE_NAME,
                  &mAmdMemTestResultVariableGuid,
                  NULL,
                  &BufferSize,
                  &gMemTestResultVar
                  );
  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
    gMemTestResultVar.TestRunCount = 0;
    gMemTestResultVar.MemTestResult[0] = 0;
    gMemTestResultVar.MemTestResult[1] = 0;

    SmnRegisterRead (0, 0x5d79c, &SmnData32);
    Value = (SmnData32 >> 19) & 0x1FF;
    DEBUG((DEBUG_INFO, "SmnData32 = %x, Value = %x\n", SmnData32, Value));
    if (Value >= 6) {
      // Skip memory test
      SetMemTestResultVariable(0);
      return EFI_SUCCESS;
    }
  } else {
    // Run test twice, if first run get pass, skip the second run
    if ((gMemTestResultVar.MemTestResult[0] == 0) || (gMemTestResultVar.TestRunCount == 2)) {
      return EFI_SUCCESS;
    }
  }
#endif

  // Callback for displaying on screen and wait for key.
  Status = EfiCreateEventReadyToBootEx (
    TPL_CALLBACK,
    ReadyToBootCallback,
    NULL,
    &ReadyToBootEvent
  );

  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
