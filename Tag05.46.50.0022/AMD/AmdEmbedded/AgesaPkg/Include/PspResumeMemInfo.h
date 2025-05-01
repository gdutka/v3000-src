/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file PspResumeMemInfo.h
 *
 * @brief  Declare structure of resume memory info
 *
 * @details This structure will be used for S3 Exit entry code to initialize the Stack, heap
 *          and restore CCX related registers and etc
 *
 */

#ifndef _PSP_RESUME_MEM_INFO_H_
#define _PSP_RESUME_MEM_INFO_H_

#include <Guid/AcpiS3Context.h>


typedef VOID (*CCX_EARLY_INIT)(VOID);


#pragma pack(push, 1)
///
/// Used to keep SMM Resume Memory informtion
///
typedef struct _PSP_SMM_RSM_MEM_INFO {
  UINT32                  BspStackSize;           ///< BSP Stack Size for resume
  UINT32                  ApStackSize;            ///< AP Stack Size for resume
  EFI_PHYSICAL_ADDRESS    StackPtr;               ///< Point to the base of Stack
  EFI_PHYSICAL_ADDRESS    RsmHeapPtr;             ///< Point to the base of Resume Heap
  UINT32                  HeapSize;               ///< Reserved Heap Size, Change the Order to avoid Warning 4366
  UINT32                  TempRegionSize;         ///< Reserved Temp Region Size
  EFI_PHYSICAL_ADDRESS    TempRegionPtr;          ///< Point to the base of Temporary Region (used for store the dynamic value during SMM SEC Phase)
} PSP_SMM_RSM_MEM_INFO;

typedef struct _PSP_SMM_HDR_DATA {
  SMM_S3_RESUME_STATE   SmmS3ResumeState;         ///< SmmS3ResumeState built by EDKII core
  PSP_SMM_RSM_MEM_INFO  PspSmmRsmMemInfo;         ///< SMM Resume Memory informtion specific for AMD
  EFI_PHYSICAL_ADDRESS  CcxDataRegion;            ///< Data for CcxEarlyInit
  EFI_PHYSICAL_ADDRESS  CcxEarlyInit;             ///< Point to CcxEarlyInit function
  EFI_PHYSICAL_ADDRESS  ApStackTop;               ///< Pointer to AP Stack. This is updated when releaseing APs.
  UINT32                ApSyncFlag;               ///< Address to hold ApSyncFlag
  UINT32                CapsuleUpdateSignature;   ///< Flag specify if it is on the capsule update S3 path, CAPSULE_UPDATE_SIGNATURE
} PSP_SMM_HDR_DATA;

#pragma pack(pop)


#endif  //_PSP_RESUME_MEM_INFO_H_


