/** @file
   To implement h2o owner draw panel protocol.
;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/H2OOwnerDrawPanelProtocol.h>
#include <Guid/PropertyDef.h>

STATIC
BOOLEAN
IsValidImageAndBorder (
  IN       EFI_IMAGE_INPUT                  *Image,
  IN CONST RECT                             *Border
  )
{
  if (Image->Bitmap == NULL || Image->Height == 0 || Image->Width == 0 || Image->Flags != 0) {
    return FALSE;
  }

  if (Border->left < 0 || Border->top < 0 || Border->right < 0 || Border->bottom < 0 ||
      Border->left >= Border->right || Border->top >= Border->bottom) {
    return FALSE;
  }

  if ((Border->right  - Border->left) > Image->Width ||
      (Border->bottom - Border->top ) > Image->Height) {
    return FALSE;
  }

  return TRUE;
}

STATIC
BOOLEAN
IsPanelIdSupported (
  IN CONST H2O_OWNER_DRAW_PANEL_PROTOCOL    *This,
  IN H2O_PANEL_ID                           PanelId
  )
{
  UINT32                                    Index;

  for (Index = 0; Index < This->PanelIdCount; Index++) {
    if (This->PanelIds[Index] == PanelId) {
      return TRUE;
    }
  }

  return FALSE;
}

EFI_STATUS
Draw (
  IN CONST H2O_OWNER_DRAW_PANEL_PROTOCOL    *This,
  IN H2O_PANEL_ID                           PanelId,
  IN OUT EFI_IMAGE_INPUT                    *Image,
  IN CONST RECT                             *Border,
  IN UINT32                                 Flags,
  IN OUT UINT32                             *RefreshTime
  )
{
  UINT32                                    X;
  UINT32                                    Y;
  UINT32                                    MaxY;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL             Color;
  STATIC BOOLEAN                            Flag = TRUE;

  if (This == NULL || Image == NULL || Border == NULL || RefreshTime == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (!IsValidImageAndBorder (Image, Border) ||
      !IsPanelIdSupported (This, PanelId)) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&Color, sizeof (Color));
  if (Flag) {
    Color.Red = 0xFF;
  } else {
    Color.Green = 0xFF;
  }
  Flag = !Flag;

  MaxY = (Image->Height < 3) ? Image->Height : 3;
  for (Y = 0; Y < MaxY; Y++) {
    for (X = 0; X < Image->Width; X++) {
      Image->Bitmap[X + Image->Width * Y] = Color;
    }
  }

  *RefreshTime = 300;
  return EFI_MEDIA_CHANGED;
}

EFI_STATUS
Draw2 (
  IN CONST H2O_OWNER_DRAW_PANEL_PROTOCOL    *This,
  IN H2O_PANEL_ID                           PanelId,
  IN OUT EFI_IMAGE_INPUT                    *Image,
  IN CONST RECT                             *Border,
  IN UINT32                                 Flags,
  IN OUT UINT32                             *RefreshTime
  )
{
  UINT32                                    X;
  UINT32                                    Y;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL             Color;
  STATIC BOOLEAN                            Flag = TRUE;
  STATIC UINT32                             StaticRefreshTime = 100;

  if (This == NULL || Image == NULL || Border == NULL || RefreshTime == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (!IsValidImageAndBorder (Image, Border) ||
      !IsPanelIdSupported (This, PanelId)) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&Color, sizeof (Color));
  if (Flag) {
    Color.Blue = 0xFF;
  }
  Flag = !Flag;

  for (Y = (Image->Height >= 3) ? Image->Height - 3 : 0; Y < Image->Height; Y++) {
    for (X = 0; X < Image->Width; X++) {
      Image->Bitmap[X + Image->Width * Y] = Color;
    }
  }

  *RefreshTime = StaticRefreshTime;
  StaticRefreshTime += 100;
  return EFI_MEDIA_CHANGED;
}

H2O_PANEL_ID                   mPanelIdList[]              = {OWNER_DRAW_PANEL_ID};
H2O_OWNER_DRAW_PANEL_PROTOCOL  mH2OOwnerDrawPanelProtocol  = {sizeof (H2O_OWNER_DRAW_PANEL_PROTOCOL), sizeof(mPanelIdList) / sizeof(H2O_PANEL_ID), mPanelIdList, Draw };
H2O_OWNER_DRAW_PANEL_PROTOCOL  mH2OOwnerDrawPanelProtocol2 = {sizeof (H2O_OWNER_DRAW_PANEL_PROTOCOL), sizeof(mPanelIdList) / sizeof(H2O_PANEL_ID), mPanelIdList, Draw2};

/**
  Initialize and install H2O owner draw panel protocol

  @param [in] ImageHandle        The image handle
  @param [in] SystemTable        The system table

  @retval EFI_SUCCESS            Success to install protocol
  @retval Other                  Fail install protocol
**/
EFI_STATUS
EFIAPI
H2OOwnerDrawPanelEntryPoint (
  IN EFI_HANDLE                             ImageHandle,
  IN EFI_SYSTEM_TABLE                       *SystemTable
  )
{
  EFI_HANDLE                                Handle;

  Handle = NULL;
  gBS->InstallProtocolInterface (&Handle, &gH2OOwnerDrawPanelProtocolGuid, EFI_NATIVE_INTERFACE, &mH2OOwnerDrawPanelProtocol);

  Handle = NULL;
  gBS->InstallProtocolInterface (&Handle, &gH2OOwnerDrawPanelProtocolGuid, EFI_NATIVE_INTERFACE, &mH2OOwnerDrawPanelProtocol2);

  return EFI_SUCCESS;
}
