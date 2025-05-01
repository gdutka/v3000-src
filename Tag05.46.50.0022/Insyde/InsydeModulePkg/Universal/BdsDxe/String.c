/** @file
  String support

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

/**
  String support

Copyright (c) 2004 - 2010, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "String.h"

EFI_HII_HANDLE gStringPackHandle;

EFI_GUID mBdsStringPackGuid = {
  0x7bac95d3, 0xddf, 0x42f3, {0x9e, 0x24, 0x7c, 0x64, 0x49, 0x40, 0x37, 0x9a}
};

/**
  Initialize HII global accessor for string support.

**/
VOID
InitializeStringSupport (
  VOID
  )
{
  gStringPackHandle = HiiAddPackages (
                        &mBdsStringPackGuid,
                        gImageHandle,
                        BdsDxeStrings,
                        NULL
                        );
  ASSERT (gStringPackHandle != NULL);
}

/**
  Get string by string id from HII Interface

  @param Id              String ID.

  @retval  CHAR16 *  String from ID.
  @retval  NULL      If error occurs.

**/
CHAR16 *
GetStringById (
  IN  EFI_STRING_ID   Id
  )
{
  return HiiGetString (gStringPackHandle, Id, NULL);
}


/**
  Update the content of STR_BOOT_DEVICE_ERROR_MESSAGE string to CSM16 for boot error message
  if this string isn't an empty string.

  @retval EFI_SUCCESS    Update boot error message to CSM16 successfully.
  @retval EFI_NOT_FOUND  Cannot find default boot error message in CSM16.
  @retval EFI_NOT_READY  STR_BOOT_DEVICE_ERROR_MESSAGE token is empty string.
**/
EFI_STATUS
UpdateBootMessage (
  VOID
  )
{
  EFI_LEGACY_REGION2_PROTOCOL       *LegacyRegion;
  EFI_LEGACY_BIOS_PROTOCOL          *LegacyBios;
  EFI_COMPATIBILITY16_TABLE         *Table;
  EFI_IA32_REGISTER_SET             Regs;
  EFI_STATUS                        Status;
  UINT8                             *Ptr;
  BOOT_MESSAGE                      *BootMsg;
  CHAR16                            *NewString;
  UINTN                             StringLen;
  UINT8                             *NewStringAddress;

  //
  // See if the Legacy Region Protocol is available
  //
  Status = gBS->LocateProtocol (&gEfiLegacyRegion2ProtocolGuid, NULL, (VOID **)&LegacyRegion);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, (VOID **)&LegacyBios);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  BootMsg = NULL;
  // CODE ANALYSIS (VS19.0.2) - Reports C6011. Added typecast to Ptr = Ptr + 0x10.
  for (Ptr = (UINT8 *) ((UINTN) 0xF0000);
       Ptr < (UINT8 *) ((UINTN) 0x100000);
       Ptr = (UINT8 *) ((UINTN) Ptr + 0x10)) {
    //
    // "boot device error string (BDES)"
    //
    if (*(UINT32 *) Ptr == SIGNATURE_32 ('B', 'D', 'E', 'S')) {
      BootMsg = (BOOT_MESSAGE *) Ptr;
      break;
    }
  }
  if (BootMsg == NULL) {
    return EFI_NOT_FOUND;
  }

  NewString = GetStringById (STRING_TOKEN (STR_BOOT_DEVICE_ERROR_MESSAGE));
  if (NewString == NULL) {
    return EFI_NOT_READY;
  }
  StringLen = StrLen (NewString);
  if (StringLen == 0) {
    return EFI_NOT_READY;
  }

  if (StringLen <= BOOT_FAIL_STRING_LENTH) {
    Status = LegacyRegion->UnLock (LegacyRegion, 0xF0000, 0x10000, NULL);
    ZeroMem (&BootMsg->String, BOOT_FAIL_STRING_LENTH);
    UnicodeStrToAsciiStrS (NewString, &BootMsg->String, BOOT_FAIL_STRING_LENTH);
    Status = LegacyRegion->Lock (LegacyRegion, 0xF0000, 0x10000, NULL);
    return EFI_SUCCESS;
  }

  Table = NULL;
  // CODE ANALYSIS (VS19.0.2) - Reports C6011. Added typecast to Ptr = Ptr + 0x10.
  for (Ptr = (UINT8 *) ((UINTN) 0xFE000);
       Ptr < (UINT8 *) ((UINTN) 0x100000);
       Ptr = (UINT8 *) ((UINTN) Ptr + 0x10)) {
    if (*(UINT32 *) Ptr == SIGNATURE_32 ('I', 'F', 'E', '$')) {
      Table   = (EFI_COMPATIBILITY16_TABLE *) Ptr;
      break;
    }
  }
  if (Table == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = LegacyRegion->UnLock (LegacyRegion, 0xE0000, 0x10000, NULL);
  ZeroMem (&Regs, sizeof (EFI_IA32_REGISTER_SET));
  Regs.X.AX = Legacy16GetTableAddress;
  Regs.X.BX = 0x0002;
  Regs.X.CX = (UINT16)StringLen;
  Regs.X.DX = 1;
  LegacyBios->FarCall86 (
                LegacyBios,
                Table->Compatibility16CallSegment,
                Table->Compatibility16CallOffset,
                &Regs,
                NULL,
                0);

  NewStringAddress = (UINT8 *)(UINTN)(Regs.X.DS*16 + Regs.X.BX);

  BootMsg->StrSegment = (UINT16)(((UINTN)NewStringAddress >> 4) & 0xF000);
  BootMsg->StrOffset  = (UINT16)(UINTN)NewStringAddress;

  ZeroMem (NewStringAddress, StringLen);
  UnicodeStrToAsciiStrS (NewString, (CHAR8 *) NewStringAddress, StringLen);
  Status = LegacyRegion->Lock (LegacyRegion, 0xE0000, 0x10000, NULL);

  return EFI_SUCCESS;
}

/**
 Check whether there is only one physical GOP on system.

 @param[in] SinglePhyGop          Pointer to GOP instance

 @retval TRUE   There is only one physical GOP on system
 @retval FALSE  There is zero or only one physical GOP on system
**/
STATIC
BOOLEAN
IsSinglePhysicalGop (
  OUT EFI_HANDLE            *SinglePhyGop
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *GopDevicePath;
  EFI_STATUS                Status;
  UINTN                     HandleCount;
  EFI_HANDLE                *HandleBuffer;
  UINTN                     Index;
  UINTN                     NumberOfPhysicalGop;

  NumberOfPhysicalGop = 0;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiGraphicsOutputProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID*)&GopDevicePath
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }
    *SinglePhyGop = HandleBuffer[Index];
    NumberOfPhysicalGop++;
  }
  FreePool (HandleBuffer);

  if (NumberOfPhysicalGop == 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
 Get the text mode number by input resolution.

 @param[in]  RequestedColumns      Resolution column
 @param[in]  RequestedRows         Resolution row
 @param[out] TextModeNum           Pointer to text mode number

 @retval EFI_SUCCESS      There is matched text mode number
 @retval EFI_UNSUPPORTED  There is no matched text mode number
**/
STATIC
EFI_STATUS
ChkTextModeNum (
  IN UINTN                  RequestedColumns,
  IN UINTN                  RequestedRows,
  OUT UINTN                 *TextModeNum
  )
{
  UINTN                     ModeNum;
  UINTN                     Columns;
  UINTN                     Rows;
  EFI_STATUS                Status;

  for (ModeNum = 0; ModeNum < (UINTN)(gST->ConOut->Mode->MaxMode); ModeNum++) {
    gST->ConOut->QueryMode (gST->ConOut, ModeNum, &Columns, &Rows);
    if ((RequestedColumns == Columns) && (RequestedRows == Rows)) {
      *TextModeNum = ModeNum;
      break;
    }
  }

  if (ModeNum == (UINTN)(gST->ConOut->Mode->MaxMode)) {
    *TextModeNum = ModeNum;
    Status = EFI_UNSUPPORTED;
  } else {
    Status = EFI_SUCCESS;
  }

  return Status;
}

/**
 Get the device path which contain all console output device path instances.

 @retval The device path which contain all console output device path instances or NULL if not found.
**/
STATIC
VOID *
GetAllActiveConOutDevPath (
  VOID
  )
{
  UINTN                      NoHandles;
  EFI_HANDLE                 *HandleBuf;
  EFI_DEVICE_PATH_PROTOCOL   *DevPath;
  EFI_DEVICE_PATH_PROTOCOL   *ConsoleDevPath;
  EFI_DEVICE_PATH_PROTOCOL   *TempDevPath;
  UINT8                      Index;
  EFI_STATUS                 Status;

  //
  //init local
  //
  NoHandles = 0;
  HandleBuf = NULL;
  DevPath = NULL;
  ConsoleDevPath = NULL;
  TempDevPath = NULL;

  Status =  gBS->LocateHandleBuffer (
                   ByProtocol,
                   &gEfiConsoleOutDeviceGuid,
                   NULL,
                   &NoHandles,
                   &HandleBuf
                   );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  for (Index = 0; Index < NoHandles; Index++) {
    Status =  gBS->HandleProtocol (
                HandleBuf [Index],
                &gEfiDevicePathProtocolGuid,
                (VOID **)&DevPath
                );
    if (EFI_ERROR (Status) || (DevPath == NULL)) {
      continue;
    }

    TempDevPath = ConsoleDevPath;
    ConsoleDevPath = AppendDevicePathInstance (ConsoleDevPath, DevPath);
    if (TempDevPath != NULL) {
      FreePool (TempDevPath);
    }
  }

  FreePool (HandleBuf);
  return ConsoleDevPath;
}

/**
 Check whether there is a text-based console output device on system.

 @retval TRUE   There is a text-based console output device on system.
 @retval FALSE  There is no text-based console output device on system.
**/
STATIC
BOOLEAN
FoundTextBasedConsole (
  VOID
  )
{
  EFI_DEVICE_PATH_PROTOCOL         *RemainingDevicePath;
  EFI_DEVICE_PATH_PROTOCOL         *OrgRemainingDevicePath;
  EFI_DEVICE_PATH_PROTOCOL         *NextDevPathInstance;
  EFI_STATUS                       Status;
  EFI_HANDLE                       Handle;
  EFI_GRAPHICS_OUTPUT_PROTOCOL     *Gop;
  BOOLEAN                          FoundTextBasedCon;
  UINTN                            Size;
  EFI_DEVICE_PATH_PROTOCOL         *ConOutDevPathInstance;

  //
  //init local
  //
  RemainingDevicePath = NULL;
  Gop = NULL;
  FoundTextBasedCon = FALSE;
  NextDevPathInstance = NULL;
  ConOutDevPathInstance = NULL;

  RemainingDevicePath = GetAllActiveConOutDevPath ();
  if (RemainingDevicePath == NULL) {
    return FALSE;
  }
  OrgRemainingDevicePath = RemainingDevicePath;
  do {
    //
    //find all output console handles
    //
    NextDevPathInstance = GetNextDevicePathInstance (&RemainingDevicePath, &Size);
    if (NextDevPathInstance != NULL) {
      ConOutDevPathInstance = NextDevPathInstance;
      gBS->LocateDevicePath (&gEfiDevicePathProtocolGuid, &ConOutDevPathInstance, &Handle);
      Status = gBS->HandleProtocol (
                      Handle,
                      &gEfiGraphicsOutputProtocolGuid,
                      (VOID**)&Gop
                      );
       if (EFI_ERROR (Status)) {
         //
         //found text-based console
         //
         FoundTextBasedCon = TRUE;
         FreePool (NextDevPathInstance);
         break;
       }
       FreePool (NextDevPathInstance);
    }
  } while (RemainingDevicePath != NULL);
  FreePool (OrgRemainingDevicePath);
  return FoundTextBasedCon;
}

/**
 Get the optimal resolution.

 @param[in]  GraphicsOutput        Pointer to GOP instance
 @param[out] XResolution           Pointer to resolution X
 @param[out] YResoulution          Pointer to resolution Y
**/
STATIC
VOID
GetComboVideoOptimalResolution (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL     *GraphicsOutput,
  OUT UINT32                           *XResolution,
  OUT UINT32                           *YResoulution
  )
{
  UINT32                               ModeNumber;
  EFI_STATUS                           Status;
  UINTN                                SizeOfInfo;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
  UINT32                               MaxMode;
  UINTN                                MaxResolution;
  UINTN                                TempResolution;

  Status  = EFI_SUCCESS;
  MaxMode = GraphicsOutput->Mode->MaxMode;
  MaxResolution = 0;
  TempResolution = 0;

  for (ModeNumber = 0; ModeNumber < MaxMode; ModeNumber++) {
    Status = GraphicsOutput->QueryMode (
                               GraphicsOutput,
                               ModeNumber,
                               &SizeOfInfo,
                               &Info
                               );
    if (!EFI_ERROR (Status)) {
      TempResolution = (UINTN) Info->HorizontalResolution * Info->VerticalResolution;
      if (TempResolution > MaxResolution) {
        MaxResolution = TempResolution;
        *XResolution = Info->HorizontalResolution;
        *YResoulution = Info->VerticalResolution;
      }
      FreePool (Info);
      Info = NULL;
    }
  }
}

/**
 Check whether there is at least one physical GOP on system.

 @retval TRUE   There is at least one physical GOP on system.
 @retval FALSE  There is no one physical GOP on system.
**/
STATIC
BOOLEAN
HavePhysicalGop (
  VOID
  )
{
  EFI_STATUS                Status;
  UINTN                     HandleCount;
  EFI_HANDLE                *HandleBuffer;
  UINTN                     Index;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiGraphicsOutputProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiDevicePathProtocolGuid, (VOID*)&DevicePath);
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  FreePool (HandleBuffer);
  return (BOOLEAN) (Index < HandleCount);
}

/**
 Set console output device to optimal resolution.
**/
VOID
EnableOptimalTextMode (
  VOID
  )
{
  EFI_STATUS                                Status;
  OEM_LOGO_RESOLUTION_DEFINITION            *OemLogoResolutionPtr;
  OEM_LOGO_RESOLUTION_DEFINITION            *OemLogoResolutionBuffer;
  EFI_GRAPHICS_OUTPUT_PROTOCOL              *GraphicsOutput;
  UINT32                                    SizeOfX;
  UINT32                                    SizeOfY;
  UINTN                                     RequestedRows;
  UINTN                                     RequestedColumns;
  UINTN                                     TextModeNum;
  UINT32                                    Index;
  EFI_HANDLE                                SinglePhyGopHandle;
  EFI_EDID_DISCOVERED_PROTOCOL              *EdidDiscovered;
  UINT32                                    NativeResolutionX;
  UINT32                                    NativeResolutionY;
  UINTN                                     BestResolution;
  UINTN                                     SizeOfInfo;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION      *Info;
  UINT32                                    GopModeNum;
  UINTN                                     ModeNum;
  UINTN                                     Columns;
  UINTN                                     Rows;
  UINTN                                     OptimalColumns;
  UINTN                                     OptimalRows;
  EFI_DEVICE_PATH_PROTOCOL                  *ConOutDevicePath;
  EFI_STATUS                                OemSvcStatus;

  //
  //init local vars
  //
  Index = 0;
  SinglePhyGopHandle = NULL;
  OemLogoResolutionPtr = NULL;
  SizeOfInfo = 0;
  Info = NULL;
  BestResolution = 0;
  RequestedColumns  = 0;
  RequestedRows = 0;
  SizeOfX = 0;
  SizeOfY = 0;
  GraphicsOutput = NULL;
  TextModeNum = gST->ConOut->Mode->MaxMode;
  ConOutDevicePath = NULL;
  OemLogoResolutionBuffer = NULL;

  ConOutDevicePath = GetAllActiveConOutDevPath ();
  if (ConOutDevicePath == NULL) {
    //
    // no monitor is attached
    //
    return ;
  }
  FreePool (ConOutDevicePath);

  OemLogoResolutionBuffer = AllocateCopyPool (PcdGetSize (PcdDefaultLogoResolution), PcdGetPtr (PcdDefaultLogoResolution));
  if (OemLogoResolutionBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }
  OemLogoResolutionPtr = OemLogoResolutionBuffer;

  if (FoundTextBasedConsole ()) {
    //
    //if any Console deveice is text-based display,
    //based on text mode intersection, find optimal text mode
    //
    OptimalRows    = 0;
    OptimalColumns = 0;
    for (ModeNum = 0; ModeNum < (UINTN)(gST->ConOut->Mode->MaxMode); ModeNum++) {
      Status =  gST->ConOut->QueryMode (gST->ConOut, ModeNum, &Columns, &Rows);
      if (!EFI_ERROR (Status)) {
        if ((Columns * Rows) > (OptimalColumns * OptimalRows)) {
          OptimalColumns = Columns;
          OptimalRows = Rows;
          TextModeNum = (UINTN)ModeNum;
        }
      }
    }

    if (!HavePhysicalGop ()) {
      OemLogoResolutionPtr->LogoResolutionX = (UINT32) (OptimalColumns * EFI_GLYPH_WIDTH);
      OemLogoResolutionPtr->LogoResolutionY = (UINT32) (OptimalRows * EFI_GLYPH_HEIGHT);
    } else {
      Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, (VOID**)&GraphicsOutput);
      if (!EFI_ERROR (Status)) {
        for (ModeNum = 0; ModeNum < GraphicsOutput->Mode->MaxMode; ModeNum++) {
          Status = GraphicsOutput->QueryMode (GraphicsOutput, (UINT32)ModeNum, &SizeOfInfo, &Info);
          if (EFI_ERROR (Status)) {
            continue;
          }

          if ((Info->HorizontalResolution / EFI_GLYPH_WIDTH) == OptimalColumns &&
              (Info->VerticalResolution / EFI_GLYPH_HEIGHT) == OptimalRows) {
            OemLogoResolutionPtr->LogoResolutionX = Info->HorizontalResolution;
            OemLogoResolutionPtr->LogoResolutionY = Info->VerticalResolution;

            FreePool (Info);
            Info = NULL;
            break;
          }
          FreePool (Info);
          Info = NULL;
        }
      }
    }
  } else if (IsSinglePhysicalGop (&SinglePhyGopHandle)) {
    gBS->HandleProtocol (
           SinglePhyGopHandle,
           &gEfiGraphicsOutputProtocolGuid,
           (VOID**) &GraphicsOutput
           );
    Status = gBS->HandleProtocol (
                    SinglePhyGopHandle,
                    &gEfiEdidDiscoveredProtocolGuid,
                    (VOID **) &EdidDiscovered
                    );
    if (!EFI_ERROR (Status)) {
      Status = GetResolutionByEdid (EdidDiscovered, &NativeResolutionX, &NativeResolutionY);
      if (!EFI_ERROR (Status)) {
        Status = CheckModeSupported (
                   GraphicsOutput,
                   NativeResolutionX,
                   NativeResolutionY,
                   &GopModeNum
                   );
        if (!EFI_ERROR (Status)) {
          OemLogoResolutionPtr->LogoResolutionX = NativeResolutionX;
          OemLogoResolutionPtr->LogoResolutionY = NativeResolutionY;
        }
      }
    } else {
      //
      // find optimal resoltion from gop modes to set for possible combination of two video output devices
      // attached to a gop device
      //
      GetComboVideoOptimalResolution (GraphicsOutput, &SizeOfX, &SizeOfY);
      OemLogoResolutionPtr->LogoResolutionX = SizeOfX;
      OemLogoResolutionPtr->LogoResolutionY = SizeOfY;
    }
  } else {
    Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, (VOID**)&GraphicsOutput);
    if (!EFI_ERROR (Status)) {
      for (Index = 0;Index < GraphicsOutput->Mode->MaxMode; Index++) {
        //
        //find best resolution from virtual gop
        //
        GraphicsOutput->QueryMode (GraphicsOutput, Index, &SizeOfInfo, &Info);
        if (((UINTN) Info->HorizontalResolution * Info->VerticalResolution) > BestResolution) {
          BestResolution =  (UINTN) Info->HorizontalResolution * Info->VerticalResolution;
          OemLogoResolutionPtr->LogoResolutionX = Info->HorizontalResolution;
          OemLogoResolutionPtr->LogoResolutionY = Info->VerticalResolution;
        }
        FreePool (Info);
        Info = NULL;
      }
    }
  }

  DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices Call: OemSvcLogoResolution \n"));
  OemSvcStatus = OemSvcLogoResolution (&OemLogoResolutionPtr);
  DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices OemSvcLogoResolution Status: %r\n", OemSvcStatus));

  SizeOfX = OemLogoResolutionPtr->LogoResolutionX;
  SizeOfY = OemLogoResolutionPtr->LogoResolutionY;

  RequestedColumns = SizeOfX / EFI_GLYPH_WIDTH;
  RequestedRows = SizeOfY / EFI_GLYPH_HEIGHT;
  Status = ChkTextModeNum (RequestedColumns, RequestedRows, &TextModeNum);

Done:
  if (!EFI_ERROR (Status)) {
    if ((UINTN)(gST->ConOut->Mode->Mode) != TextModeNum) {
      Status = gST->ConOut->SetMode (gST->ConOut, TextModeNum);
    } else {
      //
      // make sure text dimension's conrresponding resolution is set.
      //
      if (GraphicsOutput != NULL) {
        if ((SizeOfX != GraphicsOutput->Mode->Info->HorizontalResolution) ||
            (SizeOfY != GraphicsOutput->Mode->Info->VerticalResolution)) {
          CheckModeSupported (
            GraphicsOutput,
            SizeOfX,
            SizeOfY,
            &GopModeNum
            );
          GraphicsOutput->SetMode (GraphicsOutput, GopModeNum);
        }
      }
    }
  } else {
    Status = gST->ConOut->SetMode (gST->ConOut, 0);
  }
  if (OemLogoResolutionBuffer != NULL) {
    FreePool (OemLogoResolutionBuffer);
  }
}

