/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
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
#ifndef __AOD_V2_SETUP_H__
#define __AOD_V2_SETUP_H__

// {5ED15DC0-EDEF-4161-9151-6014C4CC630C}
#define AOD_SETUP_GUID \
  { 0x5ed15dc0, 0xedef, 0x4161, 0x91, 0x51, 0x60, 0x14, 0xc4, 0xcc, 0x63, 0xc }

extern EFI_GUID gAodSystemConfigurationGuid;

#define AOD_SETUP_VARIABLE_NAME  L"AodSetup"
// {E39AC631-48ED-445E-9042-55B34F405843}
#define AOD_SETUP_FORMSET_GUID \
  { 0xe39ac631, 0x48ed, 0x445e, 0x90, 0x42, 0x55, 0xb3, 0x4f, 0x40, 0x58, 0x43 }
#define  VARSTORE_ID_AODSETUP                       0xF000

#define AOD_STRINGNIZE(param) #param
#define AOD_SOC_SETUP_DEFINITION(param)  AOD_STRINGNIZE(Features/AODv2/Include/AodConfig##param.h)
#define AOD_SETUP_CLASS                   1
#define AOD_SETUP_SUB_CLASS               0
#define CPU_OVERCLOCKING_CAPABLE          0x00000001
#define MOC_CAPABLE                       0x00000002
#define PBO_ADVANCED_CAPABLE              0x00000004
#define GFX_OVERCLOCKING_CAPABLE          0x00000010
#define MEM_OVERCLOCKING_CAPABLE          0x00000020
#define GFX_PAD_OVERCLOCKING_CAPABLE      0x00020000
#define CPU_PAD_OVERCLOCKING_CAPABLE      0x00010000

#pragma pack(push,1)

typedef struct _AOD_VARIABLE_HEADER {
  UINT16  AodVariableSize;           ///< Will be different if AOD varialbe structure changes
  UINT8   AodComboFlag;              ///< Record the comboflag
  UINT8   CpuOverClocking;           ///< CPU overclocking capbility of SMC_MSG_GetOverclockCap SMU message BIT0
  UINT8   MocOverClocking;           ///< PBO overclocking capbility of SMC_MSG_GetOverclockCap SMU message BIT1
  UINT8   GfxOverClocking;           ///< GFX overclocking capbility of SMC_MSG_GetOverclockCap SMU message BIT4
  UINT8   MemOverClocking;           ///< Memory overclocking capability of SMC_MSG_GetOverclockCap SMU message BIT5
  UINT8   GfxPrecisedOverClocking;   ///< Gfx Precise and Direct overclocking capability of SMC_MSG_GetOverclockCap SMU message BIT17
  UINT8   CpuPrecisedOverClocking;   ///< CPU Precise and Direct overclocking capability of SMC_MSG_GetOverclockCap SMU message BIT16
  UINT8   PBOAdvancedOverClocking;   ///< PBO Advanced overclocking capbility of SMC_MSG_GetOverclockCap SMU message BIT2
  UINT8   Reserved[6];               ///< Reserved for future use
} AOD_VARIABLE_HEADER;


#pragma pack(pop)

#endif