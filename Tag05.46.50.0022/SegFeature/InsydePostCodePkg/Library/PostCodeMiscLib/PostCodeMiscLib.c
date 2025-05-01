/** @file
  The instance of PostCodeConfigLib Library

;******************************************************************************
;* Copyright (c) 2017, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/PostCodeMiscLib.h>
#include "InternalPostCodeMiscLib.h"

INSTRUCTION_MACHINE_CODE mInInstructionList [] = {
  {0x01, 0x02, 0xE4, (UINT8) FixedPcdGet16 (PcdH2OPostCodeDebugPortAddress), 0x00},  // in  al,0x80
  {0x02, 0x03, 0x66, 0xE5, (UINT8) FixedPcdGet16 (PcdH2OPostCodeDebugPortAddress)},  // in  ax,0x80
  {0x04, 0x02, 0xE5, (UINT8) FixedPcdGet16 (PcdH2OPostCodeDebugPortAddress), 0x00},  // in  eax,0x80
  {0x01, 0x01, 0xEC, 0x00, 0x00},                                                    // in  al.dx
  {0x02, 0x02, 0x66, 0xED, 0x00},                                                    // in  ax,dx
  {0x04, 0x01, 0xED, 0x00, 0x00}                                                     // in  eax,dx
};

INSTRUCTION_MACHINE_CODE mOutInstructionList [] = {
  {0x01, 0x02, 0xE6, (UINT8) FixedPcdGet16 (PcdH2OPostCodeDebugPortAddress), 0x00},  // out  0x80,al
  {0x02, 0x03, 0x66, 0xE7, (UINT8) FixedPcdGet16 (PcdH2OPostCodeDebugPortAddress)},  // out  0x80,ax
  {0x04, 0x02, 0xE7, (UINT8) FixedPcdGet16 (PcdH2OPostCodeDebugPortAddress), 0x00},  // out  0x80,eax
  {0x01, 0x01, 0xEE, 0x00, 0x00},                                                    // out  dx,al
  {0x02, 0x02, 0x66, 0xEF, 0x00},                                                    // out  dx,ax
  {0x04, 0x01, 0xEF, 0x00, 0x00}                                                     // out  dx,eax
};

BOOLEAN
EFIAPI 
IsTrappedByReadPort (
  IN  UINTN              SystemContextIp
  )
{
  UINT8    Index;
  
  for (Index = 0; Index < sizeof (mInInstructionList) / sizeof (INSTRUCTION_MACHINE_CODE); Index++) {
    //
    // SystemContextIp is the next instrucstion address
    // We need to check the previous instrucstion whcih triggered this trap 
    // Check if the previous instrucstion is "in" instrucstion
    //
    if (CompareMem ((VOID*)(SystemContextIp - mInInstructionList[Index].MachineCodeSize), mInInstructionList[Index].MachineCode, mInInstructionList[Index].MachineCodeSize) == 0) {   
      return TRUE;
    }
  }

  return FALSE;
}

UINT8
EFIAPI 
GetPostCodeLength (
  IN  UINTN              SystemContextIp
  )
{
  UINT8    Index;

  for (Index = 0; Index < sizeof (mOutInstructionList) / sizeof (INSTRUCTION_MACHINE_CODE); Index++) {
    //
    // SystemContextIp is the next instrucstion address
    // We need to check the previous instrucstion whcih triggered this trap 
    // The previous instrucstion must be "out" instruction to output the Post Code
    // Parse the "out" instruction to get the Post Code length
    //
    if (CompareMem ((VOID*)(SystemContextIp - mOutInstructionList[Index].MachineCodeSize), mOutInstructionList[Index].MachineCode, mOutInstructionList[Index].MachineCodeSize) == 0) {
      return mOutInstructionList[Index].OperandLength;
    }
  }

  //
  // If we cannot parse the instruction, return the default length
  //
  ASSERT (FALSE);
  return 0x01;
}

UINTN
EFIAPI 
GetScreenPostCodeStr (
  IN  UINT8              DisplayLength,
  IN  UINTN              PostCode,
  IN  UINTN              BufferSize,
  OUT CHAR8              *Buffer
  )
{
  UINTN NumofChar;
  
  switch (DisplayLength) {
  case 2:
    NumofChar = AsciiSPrint (Buffer, BufferSize, SCREEN_DISPLAY_2_BYTE, (UINT16) PostCode);
    break;
      
  case 4:
    NumofChar = AsciiSPrint (Buffer, BufferSize, SCREEN_DISPLAY_4_BYTE, (UINT32) PostCode);
    break;
      
  case 1: 
  default:
    NumofChar = AsciiSPrint (Buffer, BufferSize, SCREEN_DISPLAY_1_BYTE, (UINT8) PostCode);
  }
  
  return NumofChar;
}

UINTN
EFIAPI 
GetScreenClearStr (
  IN  UINT8              DisplayLength,
  IN  UINTN              BufferSize,
  OUT CHAR8              *Buffer
  )
{
  UINTN NumofChar;

  switch (DisplayLength) {
  case 2:
    NumofChar = AsciiSPrint (Buffer, BufferSize, SCREEN_CLEAR_2_BYTE);
    break;
      
  case 4:
    NumofChar = AsciiSPrint (Buffer, BufferSize, SCREEN_CLEAR_4_BYTE);
    break;
      
  case 1: 
  default:
    NumofChar = AsciiSPrint (Buffer, BufferSize, SCREEN_CLEAR_1_BYTE);
  } 

  return NumofChar;
}

UINTN
EFIAPI 
GetComPortPostCodeStr (
  IN  UINT8              DisplayLength,
  IN  UINTN              PostCode,
  IN  UINTN              BufferSize,
  OUT CHAR8              *Buffer
  )
{
  UINTN NumofChar;

  switch (DisplayLength) {
  case 2:
    NumofChar = AsciiSPrint (Buffer, BufferSize, COM_PORT_2_BYTE, (UINT16) PostCode);
    break;
      
  case 4:
    NumofChar = AsciiSPrint (Buffer, BufferSize, COM_PORT_4_BYTE, (UINT32) PostCode);
    break;
      
  case 1: 
  default:
    NumofChar = AsciiSPrint (Buffer, BufferSize, COM_PORT_1_BYTE, (UINT8) PostCode);
  }

  return NumofChar;
}

UINTN
EFIAPI 
GetPostCodeString (
  IN  POST_CODE_STR_TYPE StringType,
  IN  UINTN              PostCode,     OPTIONAL
  IN  UINTN              BufferSize,
  OUT CHAR8              *Buffer
  )
{
  UINT8 DisplayLength;
  UINTN NumofChar;

  if (Buffer == NULL) {
    return 0;
  }

  if (BufferSize < MAX_STRING_SIZE) {
    return 0;
  }
  
  DisplayLength = FixedPcdGet8 (PcdH2OPostCodeDisplayLength);

  switch (StringType) {
  case ScreenDisplayStr:
    NumofChar = GetScreenPostCodeStr (DisplayLength, PostCode, BufferSize, Buffer);
    break;
      
  case ScreenClearStr:
    NumofChar = GetScreenClearStr (DisplayLength, BufferSize, Buffer);
    break;
    
  case ComPortStr:
    NumofChar = GetComPortPostCodeStr (DisplayLength, PostCode, BufferSize, Buffer);
    break;

  default:
    NumofChar = 0;
  }

  return NumofChar;
}

