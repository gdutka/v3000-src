/** @file
 Function definition for H2O Owner draw panel protocol

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
#ifndef _H2O_OWNER_DRAW_PANEL_PROTOCOL_H_
#define _H2O_OWNER_DRAW_PANEL_PROTOCOL_H_

#include <Library/RectLib.h>
#include <Library/UefiHiiServicesLib.h>
#include <Guid/InsydeModuleHii.h>

#define H2O_OWNER_DRAW_PANEL_PROTOCOL_GUID \
  {0x1f53f08c,0x1ae7,0x4259,{0xa9, 0xf4, 0x25, 0x59, 0xac, 0xda, 0xa2, 0xcb}};

typedef struct _H2O_OWNER_DRAW_PANEL_PROTOCOL  H2O_OWNER_DRAW_PANEL_PROTOCOL;

/**
  This function retrieves the image that will be used for the Owner Draw Panel specified
  by PanelId.
  On entry, Image points to an initialized image structure that includes the raw bitmap
  image and the size of the bitmap image. The bitmap may be filled with a background
  image and background color. The bitmap may also have a border.
  On entry, Border points to a rectangle that is less than or equal to the size of the
  Image. This rectangle indicates the 'content' area of the panel, which excludes any
  border area. If there is no border, then the Border rectangle is equal to the size of the
  Image.
  On entry, RefreshTime points to an unsigned integer that is initialized to zero. On exit,
  RefreshTime points to an updated unsigned integer that indicates the number of
  milliseconds that must expire before the panel's image should be updated. If zero,
  then no attempt to update will be made.

  @param [in] This               Points to an instance of this protocol interface structure.

  @param [in] PanelId            Panel identifier that specifies which panel is being drawn.

  @param [in out] Image          On entry, points to the image size and contents of any background image,
                                 background color and border. On exit, points to the updated image.
                                 The returned value of any transparency or alpha-blending flags
                                 will be ignored.

  @param [in] Border             Points to a rectangle that describes the content area of
                                 the image specified by Image, excluding any border.
                                 The rectangle must be valid and must not exceed
                                 the size of the image specified by Image.

  @param [in] Flags              Reserved. Must be zero.

  @param [in out] RefreshTime    On entry, initialized to 0. On exit, optionally updated to
                                 the number of milliseconds before the panel contents will be updated.
                                 If 0, then the panel will not be scheduled for refresh.

  @retval EFI_SUCCESS            Owner draw panel image was not updated.
  @retval EFI_MEDIA_CHANGED      Owner draw panel image was updated.
  @retval EFI_INVALID_PARAMETER  PanelId refers to a panel identifier not listed
                                 in the PanelsId array in the protocol interface structure.

  @retval EFI_INVALID_PARAMETER  Image is NULL or Image->Bitmap is NULL or
                                 Image->Width is zero or Image->Height is zero or
                                 Image->Flags is non-zero.

  @retval EFI_INVALID_PARAMETER  Border is NULL or Border is an invalid rectangle or
                                 Border contains coordinates that are less than zero or
                                 Border contains coordinates that are greater than the
                                 height or width of the Image.
**/
typedef
EFI_STATUS
(EFIAPI *H2O_OWNER_DRAW_PANEL_DRAW)(
  IN CONST H2O_OWNER_DRAW_PANEL_PROTOCOL   *This,
  IN H2O_PANEL_ID                          PanelId,
  IN OUT EFI_IMAGE_INPUT                   *Image,
  IN CONST RECT                            *Border,
  IN UINT32                                Flags,
  IN OUT UINT32                            *RefreshTime
);

typedef struct _H2O_OWNER_DRAW_PANEL_PROTOCOL {
  UINT32                      Size;
  UINT32                      PanelIdCount;
  H2O_PANEL_ID                *PanelIds;
  H2O_OWNER_DRAW_PANEL_DRAW   Draw;

} H2O_OWNER_DRAW_PANEL_PROTOCOL;

extern EFI_GUID gH2OOwnerDrawPanelProtocolGuid;

#endif

