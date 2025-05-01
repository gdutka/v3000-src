/** @file
//
//Brief description of file's purpose. Detailed description of file's purpose.
//
//;******************************************************************************
//;* Copyright (c) 2014-2014, Insyde Software Corporation. All Rights Reserved.
//;*
//;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
//;* transmit, broadcast, present, recite, release, license or otherwise exploit
//;* any part of this publication in any form, by any means, without the prior
//;* written permission of Insyde Software Corporation.
//;*
//;******************************************************************************
//
//
//other copy right statement
*/
/** @file
  TODO: Brief Description of UEFI Driver DriverArpServiceDxe
  
  TODO: Detailed Description of UEFI Driver DriverArpServiceDxe

  TODO: Copyright for UEFI Driver DriverArpServiceDxe
  
  TODO: License for UEFI Driver DriverArpServiceDxe

**/

#ifndef __EFI_DRIVER_ARP_SERVICE_DXE_H__
#define __EFI_DRIVER_ARP_SERVICE_DXE_H__

//#include <Uefi.h>
#include <PiDxe.h>

//
// Libraries
//
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeServicesLib.h>

//
// UEFI Driver Model Protocols
//

//
// Consumed Protocols
//
#include <Protocol/LoadedImage.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/Ebc.h>
#include <Protocol/Runtime.h>

//
// Produced Protocols
//
#include <Protocol/DriverArpService.h>

//
// Guids
//

//
// Driver Version
//

//
// Protocol instances
//

//
// Include files with function prototypes
//
#include "Tree.h"

#pragma pack(1)

//
//
//
typedef struct _DRIVER_IMAGE_INFO_NODE_ {
  //
  UINT64                                Signature;
  //
  DRIVER_ARP_SERVICE_IMAGE_INFO         Info;
  EFI_LOADED_IMAGE_PROTOCOL             *Img;
  //
  TREE_NODE                             Node;
} DRIVER_IMAGE_INFO_NODE;

#define DRIVER_IMG_INFO_NODE_SIGNATURE  SIGNATURE_64 ('D','R','V','I','M','G','N','D')
#define DRIVER_IMG_INFO_NODE_FROM_TREE_NODE(a) \
  CR (a, DRIVER_IMAGE_INFO_NODE, Node, DRIVER_IMG_INFO_NODE_SIGNATURE)

#define EFI_FILE_NAME_SIZE              256

#pragma pack()

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
EFI_STATUS
EFIAPI
DriverArpDestroyContents (
  IN  TREE_NODE                         *Ref
  );

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
RETRIEVE_DIRECTION
EFIAPI
DriverArpCompareNode (
  IN  TREE_NODE                         *Node1,
  IN  TREE_NODE                         *Node2
  );

//
// ----------------------------------------------------------------------------
//

/**

  Notify function for Loaded Image protocol  installed

  @param  Event    The Event that is being processed
  @param  Context  The Event Context

**/
VOID
EFIAPI
EfiLoadedImageNotifyFunction (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

/**
  Unloads an image.

  @param  ImageHandle           Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
  @retval EFI_INVALID_PARAMETER ImageHandle is not a valid image handle.

**/
EFI_STATUS 
EFIAPI
DriverArpServiceDxeUnload (
  IN EFI_HANDLE  ImageHandle
  );


/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
VOID
EFIAPI
DriverArpSwapImageNode (
  IN DRIVER_IMAGE_INFO_NODE              *ImgNode,
  IN DRIVER_IMAGE_INFO_NODE              *ImgNode2
  );

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
DRIVER_IMAGE_INFO_NODE *
EFIAPI
DriverArpNewImageNode (
  IN EFI_LOADED_IMAGE_PROTOCOL          *LoadedImage
  );

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
DriverArpRecordImage (
  IN EFI_LOADED_IMAGE_PROTOCOL          *LoadedImage
  );

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
DriverArpServiceDxeDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

//
//
//

/**

Brief-description of function.
Extended description of function.

@param[in]             para1             description
@param[in, out]        para2             description

@retval                EFI_SUCCESS       Function complete successfully.
@retval                EFI_SUCCESS       Function error.
@retval                NONE

**/
EFI_STATUS
EFIAPI
DriverArpRetrieveImageByAddress (
  IN  EFI_DRIVER_ARP_SERVICE_PROTOCOL   *This,
  IN  EFI_PHYSICAL_ADDRESS              Address,
  OUT DRIVER_ARP_SERVICE_IMAGE_INFO     *Info,
  IN OUT UINTN                          *NumberOfInfo
  );

#endif // end of #define __EFI_DRIVER_ARP_SERVICE_DXE_H__

