/******************************************************************************
    Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.

*****************************************************************************/

/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */
#ifndef __AOD_FORMSET_MANAGEME_PROTOCOL_H__
#define __AOD_FORMSET_MANAGEME_PROTOCOL_H__

typedef struct _AOD_FORMSET_MANAGE_PROTOCOL   AOD_FORMSET_MANAGE_PROTOCOL;

///
/// Global ID for the FORMSET MANAGE Protocol
///
#define AOD_FORMSET_MANAGE_PROTOCOL_GUID \
  { \
    0x1ffb2c8a, 0xe3a7, 0x4acc, { 0x97, 0x52, 0x50, 0x28, 0x8c, 0x74, 0x4b, 0x99} \
  }

/**
  This function allows a caller to set defulat for AOD varstore

  @param[in]   This              Points to the AOD_FORMSET_MANAGE_PROTOCOL.
  @param[in]   Name              TBD

  @retval EFI_SUCCESS            The Results is filled with the requested values.

**/
typedef
EFI_STATUS
(EFIAPI *AOD_FORMSET_LOAD_DEFAULT)(
  IN  CONST AOD_FORMSET_MANAGE_PROTOCOL      *This,
  IN  CHAR16                                 *Name OPTIONAL
  );

///
/// Interface structure for the AOD FORMSET Manage Protocol.
///
struct _AOD_FORMSET_MANAGE_PROTOCOL {
  AOD_FORMSET_LOAD_DEFAULT             LoadDefault;
};


#endif
