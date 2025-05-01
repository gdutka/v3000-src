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

#include "DriverArpServiceDxe.h"

VOID                                    *mRegistration = NULL;
TREE_NODE                               *mRoot = NULL;
TREE_NODE                               *mOverlapped = NULL;
EFI_EVENT                               mEvent = NULL;

EFI_DRIVER_ARP_SERVICE_PROTOCOL         mDriverArpServiceProtocol = {
  EFI_DRIVER_ARP_SERVICE_PROTOCOL_REVISION_1,
  DriverArpRetrieveImageByAddress
};

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
  )
{
  DRIVER_IMAGE_INFO_NODE                *ImgNode = NULL;

  if (Ref == NULL) {
    return RETURN_INVALID_PARAMETER;
  }

  //
  ImgNode = DRIVER_IMG_INFO_NODE_FROM_TREE_NODE (Ref);

  if (ImgNode->Info.FilePath != NULL)
    FreePool (ImgNode->Info.FilePath);
  if (ImgNode->Info.ImageFileName != NULL)
    FreePool (ImgNode->Info.ImageFileName);

  FreePool (ImgNode);

  return RETURN_SUCCESS;
}

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
  )
{
  DRIVER_ARP_SERVICE_IMAGE_INFO CONST   *Info1 = NULL, *Info2 = NULL;

  //
  if ((Node1 == NULL) || (Node2 == NULL)) {
    return RetrieveMax;
  }

  //
  Info1 = &(DRIVER_IMG_INFO_NODE_FROM_TREE_NODE (Node1))->Info;
  Info2 = &(DRIVER_IMG_INFO_NODE_FROM_TREE_NODE (Node2))->Info;

  //
  if (
    (Info2->ImageBase >= Info1->ImageBase) &&
    (Info2->ImageBase < (Info1->ImageBase + Info1->ImageSize))
    ) {
    return RetrieveEqual;
  }
  else if (Info2->ImageBase < Info1->ImageBase) {
    return RetrieveLeftNode;
  } else {
    return RetrieveRightNode;
  }
}

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
  )
{
  EFI_STATUS                            Status;
  EFI_LOADED_IMAGE_PROTOCOL             *Img;

  DRIVER_IMAGE_INFO_NODE                *ImgNode;

  //
  Status  = RETURN_SUCCESS;
  Img     = NULL;
  ImgNode = NULL;

  //
  do {
    Status = gBS->LocateProtocol (&gEfiLoadedImageProtocolGuid, mRegistration, (VOID *) &Img);
    if (!RETURN_ERROR (Status)) {

      //
      Status = DriverArpRecordImage (Img);
    }
  } while (Status == RETURN_SUCCESS);

  //
  if (Status == RETURN_NOT_FOUND) {
    Status = RETURN_SUCCESS;
  }

  // Invoked by EntryPoint
  if ((Event == NULL) && (Context != NULL)) {
    *(EFI_STATUS *) Context = Status;
  }

  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[DriverArpServiceDxe] There is an error occured @callback (%r)!\n", Status));
  }

  return;
}

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
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;
  //
  // Do any additional cleanup that is required for this driver
  //
  if (mRoot != NULL) {
    DestroyTree (mRoot);
    mRoot = NULL;
  }

  if (mOverlapped != NULL) {
    DestroyTree (mOverlapped);
    mOverlapped = NULL;
  }

  if (mEvent != NULL) {
    gBS->CloseEvent (mEvent);
    mEvent = NULL;
  }

  return EFI_SUCCESS;
}

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
  )
{
  DRIVER_IMAGE_INFO_NODE                temp;

  temp          = *ImgNode;
  *ImgNode      = *ImgNode2;

  // Keep the tree node structure
  ImgNode->Node = temp.Node;
  temp.Node     = ImgNode2->Node;

  *ImgNode2     = temp;

  return;
}

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
  )
{
  CHAR8                                 *EfiFileName;
  EFI_GUID                              *FvNameGuid;
  EFI_STATUS                            Status;

  UINTN                                 BufferSize;
  VOID                                  *Buffer;
  DRIVER_IMAGE_INFO_NODE                *ImgNode;
  DRIVER_ARP_SERVICE_IMAGE_INFO         *Info;

  //
  if (LoadedImage == NULL) return NULL;

  //
  //
  //
  EfiFileName       = NULL;
  FvNameGuid        = NULL;
  Status            = RETURN_SUCCESS;
  BufferSize        = 0;
  Buffer            = NULL;
  Info              = NULL;
  ImgNode           = NULL;

  //
  //
  //
  ImgNode = (DRIVER_IMAGE_INFO_NODE *) AllocateZeroPool (sizeof (DRIVER_IMAGE_INFO_NODE));
  if (ImgNode == NULL) {
    return NULL;
  }
  Info = &ImgNode->Info;

  ///
  /// BUGBUG:
  /// If a driver isn't loaded from UEFI Firmware Volume (ex. loaded from FAT file system),
  /// then we can't identify the driver by file GUID.
  ///

  ///
  /// Try to get image GUID from LoadedImageDevicePath protocol
  ///
  /// For FwVol File system there is only a single file name that is a GUID.
  if (LoadedImage->FilePath != NULL) {
    FvNameGuid = EfiGetNameGuidFromFwVolDevicePathNode ((CONST MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *) LoadedImage->FilePath);
    if (FvNameGuid == NULL)
      Status = RETURN_NOT_FOUND;
  } else {
    Status = RETURN_NOT_FOUND;
  }
  ///
  /// Method 1. Get the name string from FFS UI section
  /// Try to get the image's FFS UI section by image GUID
  ///
  if (!RETURN_ERROR (Status)) {
    Status = GetSectionFromFv (FvNameGuid, EFI_SECTION_USER_INTERFACE, 0, &Buffer, &BufferSize);
    if ((Status == RETURN_WARN_BUFFER_TOO_SMALL) || (Status == RETURN_BUFFER_TOO_SMALL)) {
      Buffer = AllocateZeroPool (BufferSize);
      Status = GetSectionFromFv (FvNameGuid, EFI_SECTION_USER_INTERFACE, 0, &Buffer, &BufferSize);
    }
  }
  ///
  /// Method 2: Get the name string from image DevicePath
  ///
  else {
    Buffer = (VOID *) ConvertDevicePathToText (LoadedImage->FilePath, TRUE, FALSE);
    if (Buffer != NULL) {
      BufferSize = StrSize ((CONST CHAR16 *) Buffer);
      Status = RETURN_SUCCESS;
    }
  }

  if (!RETURN_ERROR (Status) && (Buffer != NULL)){
    EfiFileName = (CHAR8 *) AllocateZeroPool ((BufferSize / 2));
    if (EfiFileName != NULL)
//[-start-190425-IB05820464-modify]//
      UnicodeStrToAsciiStrS ((CHAR16 *) Buffer, EfiFileName, (BufferSize / 2));
//[-end-190425-IB05820464-modify]//
    FreePool (Buffer);
  }

  //
  // Fill fields into driver image information node
  //
  ImgNode->Signature    = DRIVER_IMG_INFO_NODE_SIGNATURE;
  //
  Info->Revision        = DRIVER_ARP_SERVICE_IMAGE_INFO_REVISION_1;
  Info->Id              = (mRoot->NumberOfNodes + 1);
  Info->IsValid         = TRUE;
  //
  Info->FilePath        = DuplicateDevicePath (LoadedImage->FilePath);
  if (FvNameGuid != NULL)
    Info->ImageFileGuid = *FvNameGuid;
  if (EfiFileName != NULL)
    Info->ImageFileName   = EfiFileName;

  //
  Info->ImageBase       = (EFI_PHYSICAL_ADDRESS)(UINTN) LoadedImage->ImageBase;
  Info->ImageSize       = (EFI_PHYSICAL_ADDRESS) LoadedImage->ImageSize;
  Info->ImageCodeType   = LoadedImage->ImageCodeType;
  Info->ImageDataType   = LoadedImage->ImageDataType;

  //
  ImgNode->Img          = LoadedImage;

  //
  //
  //
#if 0
  DEBUG ((DEBUG_INFO, "----------------------------------------------------------------------------\n"));
  DEBUG ((DEBUG_INFO, "[DriverArpServiceDxe] Image information ...\n"));
  DEBUG ((DEBUG_INFO, "Image Protocol  @ 0x%0lx\n", (UINTN) LoadedImage));
  DEBUG ((DEBUG_INFO, "  |- Image's DevPath : %s\n", ConvertDevicePathToText (Info->FilePath, TRUE, TRUE)));
  DEBUG ((DEBUG_INFO, "  |- Image's Name : %a\n", (Info->ImageFileName == NULL ? "<Unknow>" : Info->ImageFileName)));
  DEBUG ((DEBUG_INFO, "  |- Image Base : 0x%0lx\n", Info->ImageBas);
  DEBUG ((DEBUG_INFO, "  |- Image Size : 0x%0lx\n", Info->ImageSize));
#endif

  return ImgNode;
}

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
  )
{
  EFI_STATUS                            Status;
  UINTN                                 Index;
  TREE_NODE                             *temp;
  DRIVER_IMAGE_INFO_NODE                *ImgNode;
  DRIVER_IMAGE_INFO_NODE                *ImgNode2;
  DRIVER_ARP_SERVICE_IMAGE_INFO         *Info;

  //
  Status    = EFI_SUCCESS;
  Index     = 0;
  temp      = NULL;
  ImgNode   = NULL;
  ImgNode2  = NULL;
  Info      = NULL;

  //
  ImgNode = DriverArpNewImageNode (LoadedImage);
  if (ImgNode == NULL) {
    Status = RETURN_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "Can not allocate memory for loaded image node (Index = %ld) (%r)!\n", Index, Status));
    return Status;
  }

  //
  temp = RetrieveTree (mRoot, &ImgNode->Node);

  // Add driver image info into database
  if (temp == NULL) {

    Index  = (mRoot == NULL ? 0 : mRoot->NumberOfNodes);
    Status = AddTree (&mRoot, &ImgNode->Node);
    if (RETURN_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Can not add a loaded image node (Index = %ld) into data base (%r)!\n", Index, Status));
      FreePool (ImgNode);
    }
  }
  //
  // Images's location is overlapped, swap it with old one.
  // And add the old one to overlapped tree.
  //
  else {

    Info = &ImgNode->Info;

    DEBUG ((DEBUG_INFO, "----------------------------------------------------------------------------\n"));
    DEBUG ((DEBUG_INFO, "[DriverArpServiceDxe] Warning, there are two images' location overlapped!!\n"));
    DEBUG ((DEBUG_INFO, "Image1 information ...\n"));
    DEBUG ((DEBUG_INFO, "Image Protocol  @ 0x%0lx\n", (UINTN) LoadedImage));
    DEBUG ((DEBUG_INFO, "  |- Image's DevPath : %s\n", ConvertDevicePathToText (Info->FilePath, TRUE, TRUE)));
    DEBUG ((DEBUG_INFO, "  |- Image's Name : %a\n", (Info->ImageFileName == NULL ? "<Unknow>" : Info->ImageFileName)));
    DEBUG ((DEBUG_INFO, "  |- Image Base : 0x%0lx\n", Info->ImageBase));
    DEBUG ((DEBUG_INFO, "  |- Image Size : 0x%0lx\n\n", Info->ImageSize));

    Index = (mOverlapped == NULL ? 0 : mOverlapped->NumberOfNodes);
    Info->Id      = (Index + 1);
    Info->IsValid = FALSE;

    Status = AddTree (&mOverlapped, &ImgNode->Node);
    if (RETURN_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Can not add a loaded image node (Index = %ld) into data base (%r)!\n", Index, Status));
      FreePool (ImgNode);
    }

    ImgNode2 = DRIVER_IMG_INFO_NODE_FROM_TREE_NODE (temp);
    Info = &ImgNode2->Info;

    DEBUG ((DEBUG_INFO, "Image2 information ...\n"));
    DEBUG ((DEBUG_INFO, "Image Protocol  @ 0x%0lx\n", (UINTN) ImgNode2->Img));
    DEBUG ((DEBUG_INFO, "  |- Image's DevPath : %s\n", ConvertDevicePathToText (Info->FilePath, TRUE, TRUE)));
    DEBUG ((DEBUG_INFO, "  |- Image's Name : %a\n", (Info->ImageFileName == NULL ? "<Unknow>" : Info->ImageFileName)));
    DEBUG ((DEBUG_INFO, "  |- Image Base : 0x%0lx\n", Info->ImageBase));
    DEBUG ((DEBUG_INFO, "  |- Image Size : 0x%0lx\n", Info->ImageSize));

    DriverArpSwapImageNode (ImgNode, ImgNode2);
  }

  return Status;
}

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
  )
{
  EFI_STATUS                            Status;
  EFI_LOADED_IMAGE_PROTOCOL             *Img;

  //
  Status          = EFI_SUCCESS;
  Img             = NULL;

  //
  Status = gBS->OpenProtocol (
                  ImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID **) &Img,
                  ImageHandle,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (!RETURN_ERROR (Status)) {
    Img->Unload = DriverArpServiceDxeUnload;
  }

  //
  InitializeTreeFunctions (&DriverArpCompareNode, &DriverArpDestroyContents);

  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  EfiLoadedImageNotifyFunction,
                  NULL,
                  &mEvent
                  );
  if (!RETURN_ERROR (Status)) {

    //
    Status = gBS->RegisterProtocolNotify (
                    &gEfiLoadedImageProtocolGuid,
                    mEvent,
                    &mRegistration
                    );
    if (RETURN_ERROR (Status)) {
      //
      gBS->CloseEvent (mEvent);
      mEvent = NULL;
    } else {
      //
      // Record images that have installed
      //
      EfiLoadedImageNotifyFunction (NULL, (VOID *) &Status);
    }
  }

  if (RETURN_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[DriverArpServiceDxe] There is an error occured within initial procedures (%r)!\n", Status));
  }

//  DEBUG_CODE ({
//  if (mRoot != NULL) {
//    DEBUG ((DEBUG_INFO, "[DriverArpServiceDxe] Is database a balance tree? %a.\n", (IsBalanceTree (mRoot) ? "TRUE" : "FALSE")));
//    DEBUG ((DEBUG_INFO, "Database left tree depth is %ld.\n", GetTreeDepth (mRoot->Left)));
//    DEBUG ((DEBUG_INFO, "Database right tree depth is %ld.\n", GetTreeDepth (mRoot->Right)));
//  }
//  });

  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gH2ODebugAssistDriverArpServiceProtocolGuid, &mDriverArpServiceProtocol,
                  NULL
                  );

  return Status;
}

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
  )
{
  EFI_STATUS                            Status = RETURN_SUCCESS;
  TREE_NODE                             *Start = NULL, *Result = NULL;
  DRIVER_IMAGE_INFO_NODE                FakeImgNode;
  UINTN                                 Index = 0;

  //
  if ((Info == NULL) || (NumberOfInfo == NULL)) {
    return RETURN_INVALID_PARAMETER;
  }
  if ((Address != (EFI_PHYSICAL_ADDRESS) (-1)) && (*NumberOfInfo == 0)) {
    return RETURN_INVALID_PARAMETER;
  }
  if (mRoot == NULL) {
    return RETURN_NOT_FOUND;
  }

  //
  ZeroMem ((VOID *) &FakeImgNode, sizeof (FakeImgNode));

  // Identify the address located with-in which images
  if (Address != (EFI_PHYSICAL_ADDRESS) (-1)) {
    //
    //
    FakeImgNode.Signature     = DRIVER_IMG_INFO_NODE_SIGNATURE;
    FakeImgNode.Info.Revision = DRIVER_ARP_SERVICE_IMAGE_INFO_REVISION_1;
    FakeImgNode.Info.ImageBase= Address;

    Start = mRoot;
    Result = RetrieveTree (Start, &FakeImgNode.Node);

    if (Result != NULL) {
      Info[Index] = (DRIVER_IMG_INFO_NODE_FROM_TREE_NODE (Result))->Info;
    } else {
      Status = RETURN_NOT_FOUND;
    }
  }
  // Return whole images info
  else if (*NumberOfInfo >= mRoot->NumberOfNodes){
    //
    // TODO:
    //
    
  }
  //
  else {
    (*NumberOfInfo) = mRoot->NumberOfNodes;
    Status = RETURN_BUFFER_TOO_SMALL;
  }

  return Status;
}

