/** @file
  Supports functions that saves and restores to the global register table
  information of PIC, KBC, PCI, CpuState, Edge Level, GPIO, and MTRR.

;*******************************************************************************
;* Copyright (c) 2019, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#include <SmmPlatform.h>
#include <Library/SynchronizationLib.h>

//
// Global variables.
//
BOOLEAN   mHaveAllocateMsrSaveTableBuffer = FALSE;
UINTN     mCoreIndex = 0;
volatile UINT32   mSemaphore;


//
// DeviceRegTable
//
UINT32
MSR_Table [] = {
  0xC0010113,
  0xC0010015,
  0xC001102E
  };
UINT64 *mMsrTableSave;
//UINTN MSR_Table_SIZE = (sizeof (MSR_Table) / sizeof (UINT32));
#define MSR_Table_SIZE  (sizeof (MSR_Table) / sizeof (UINT32))

/**
  Performs an atomic compare exchange operation to get semaphore.
  The compare exchange operation must be performed using
  MP safe mechanisms.

  @param      Sem        IN:  32-bit unsigned integer
                         OUT: original integer - 1
  @return     Original integer - 1

**/
UINT32
WaitForSemaphore (
  IN OUT  volatile UINT32           *Sem
  )
{
  UINT32                            Value;

  do {
    Value = *Sem;
  } while (Value == 0 ||
           InterlockedCompareExchange32 (
             (UINT32*)Sem,
             Value,
             Value - 1
             ) != Value);
  return Value - 1;
}


/**
  Performs an atomic compare exchange operation to release semaphore.
  The compare exchange operation must be performed using
  MP safe mechanisms.

  @param      Sem        IN:  32-bit unsigned integer
                         OUT: original integer + 1
  @return     Original integer + 1

**/
UINT32
ReleaseSemaphore (
  IN OUT  volatile UINT32           *Sem
  )
{
  UINT32                            Value;

  do {
    Value = *Sem;
  } while (Value + 1 != 0 &&
           InterlockedCompareExchange32 (
             (UINT32*)Sem,
             Value,
             Value + 1
             ) != Value);
  return Value + 1;
}

/**
  CpuSaveMsrByCore.

  @param None

  @retval void.

**/
VOID
CpuSaveMsrByCore (
  VOID
  )
{
  UINTN   Index;
  UINT64  MsrValue;

  MsrValue = AsmReadMsr64 (0xC0010010); //SYSCFGR
  MsrValue |= 0x00080000; //MtrrFixDramModEn
  AsmWriteMsr64 (0xC0010010, MsrValue);

  for (Index = 0; Index < MSR_Table_SIZE; Index++) {
    mMsrTableSave[(mCoreIndex * MSR_Table_SIZE) + Index] = AsmReadMsr64 (MSR_Table[Index]);
  }

  MsrValue = AsmReadMsr64 (0xC0010010); //SYSCFGR
  MsrValue &= ~(0x00080000); //MtrrFixDramModEn
  AsmWriteMsr64 (0xC0010010, MsrValue);

  if (mCoreIndex != gSmst->CurrentlyExecutingCpu) {
    ReleaseSemaphore (&mSemaphore);
  } 
  
  return;
}

/**
  CpuSaveRestoreMsr.

  @param None

  @retval void.

**/
VOID
CpuRestoreMsrByCore (
  VOID
  )
{
  UINTN   Index;
  UINT64  MsrValue;

  MsrValue = AsmReadMsr64 (0xC0010010); //SYSCFGR
  MsrValue |= 0x00080000; //MtrrFixDramModEn
  AsmWriteMsr64 (0xC0010010, MsrValue);

  for (Index = 0; Index < MSR_Table_SIZE; Index++) {
    AsmWriteMsr64 (MSR_Table[Index], mMsrTableSave[(mCoreIndex * MSR_Table_SIZE) + Index]);
  }

  MsrValue = AsmReadMsr64 (0xC0010010); //SYSCFGR
  MsrValue &= ~(0x00080000); //MtrrFixDramModEn
  AsmWriteMsr64 (0xC0010010, MsrValue);

  MsrValue = AsmReadMsr64 (0xC001001F);
  MsrValue &= ~(0x0000400000000000);
  AsmWriteMsr64 (0xC001001F, MsrValue);

  if (mCoreIndex != gSmst->CurrentlyExecutingCpu) {
    ReleaseSemaphore (&mSemaphore);  
  } 
  return;
}

/**
  CpuSaveRestoreMsr.

  @param [in]   SaveRestoreFlag  True: write data to Msr registers.
                                 False: read data from Msr registers to global registers.

  @retval EFI_SUCCESS            This routine was handled successfully.

**/
EFI_STATUS
CpuSaveRestoreMsr (
  IN  BOOLEAN                       SaveRestoreFlag
  )
{  
//  UINT64    TempMsrTable[8][3];
//  UINTN    CoreIndex;

  mSemaphore = 0;

  if (!SaveRestoreFlag) {
    if (!mHaveAllocateMsrSaveTableBuffer) {
      UINTN   BufferSize;

      BufferSize = gSmst->NumberOfCpus * MSR_Table_SIZE * sizeof (UINT64);
      gSmst->SmmAllocatePool (EfiRuntimeServicesData, BufferSize, (VOID**)&mMsrTableSave);
      mHaveAllocateMsrSaveTableBuffer = TRUE;
    }
    for (mCoreIndex = 0; mCoreIndex < gSmst->NumberOfCpus; mCoreIndex++) {
      if (mCoreIndex == gSmst->CurrentlyExecutingCpu) {
        CpuSaveMsrByCore ();
      } else {     
        gSmst->SmmStartupThisAp ((EFI_AP_PROCEDURE)CpuSaveMsrByCore, mCoreIndex, NULL);
        WaitForSemaphore (&mSemaphore);
      }
    }
  } else {
    for (mCoreIndex = 0; mCoreIndex < gSmst->NumberOfCpus; mCoreIndex++) {
      if (mCoreIndex == gSmst->CurrentlyExecutingCpu) {
        CpuRestoreMsrByCore ();
      } else {
        gSmst->SmmStartupThisAp ((EFI_AP_PROCEDURE)CpuRestoreMsrByCore, mCoreIndex, NULL);
        WaitForSemaphore (&mSemaphore);
      }
    }
  }
  
  // for (CoreIndex = 0; CoreIndex < gSmst->NumberOfCpus; CoreIndex++) {
  //   TempMsrTable [CoreIndex][0] = *(mMsrTableSave + (CoreIndex * MSR_Table_SIZE));
  //   TempMsrTable [CoreIndex][1] = *(mMsrTableSave + (CoreIndex * MSR_Table_SIZE) + 1);
  //   TempMsrTable [CoreIndex][2] = *(mMsrTableSave + (CoreIndex * MSR_Table_SIZE) + 2);
  // }
//   {
//   volatile UINTN  Index1;

  return EFI_SUCCESS;
}

/**
  SaveRestoreCpu.

  @param [in]   SaveRestoreFlag  True: write data to Cpu registers.
                                 False: read data from Cpu registers to global registers.

  @retval EFI_SUCCESS            This routine was handled successfully.

**/
EFI_STATUS
SaveRestoreCpu (
  IN  BOOLEAN                       SaveRestoreFlag
  )
{

  CpuSaveRestoreMsr (SaveRestoreFlag);

  return  EFI_SUCCESS;
}
