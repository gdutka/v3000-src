/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_EMULATION_FLAG_LIB_H_
#define _AMD_EMULATION_FLAG_LIB_H_

#pragma pack (push, 1)

#define MaxEnvFlagesSize      sizeof(PRESILICON_CONTROL_STRUCT)

typedef enum _PRESILICON_ENV_TYPE {
  ENV_HW,                 // No external environment or has not been checked yet.
  ENV_EMULATION,          // Emulation Environment
  ENV_SIMULATION,         // Hybrid Simulation environment
  ENV_ALL                 // Both Emulation and Simulation Used as parameter to PRESILICON Macros
} PRESILICON_ENV_TYPE;

/**
* PRE-SILICON CONTROL FLAGS
*   These flags are mapped to hardware scratch registers  by the family specific code
*   and may be mapped to different hardware bits depending on the environment.
**/
typedef enum _PRESILICON_CONTROL_FLAG {
  NO_FLAGS,
  PRESIL_SKIP_ALL_USBCONTROLLER_ACCESS,
  PRESIL_SKIP_ALL_USBPHY_ACCESS,
  PRESIL_SKIP_DXIO_INITIALIZAION,
  PRESIL_FASTSIM_GIO,
  PRESIL_FASTSIM_DXIO,
  PRESIL_FASTSIM_DFLT_TBL,
  PRESIL_FASTSIM_SMU_MSGS,
  PRESIL_DISABLE_EC,
  PRESIL_FASTSIM_PEI_LOG,
  PRESIL_FASTSIM_DXE_LOG,
  NUM_CONTROL_FLAGS,
}PRESILICON_CONTROL_FLAG;

///< Pre-Silicon Control Structure
typedef struct _PRESILICON_CONTROL_STRUCT {
  PRESILICON_ENV_TYPE EnvType;
  UINT8               EnvRaw;
  UINT8               CoreClNum;
  BOOLEAN             ControlFlags[NUM_CONTROL_FLAGS];
} PRESILICON_CONTROL_STRUCT;


 /**
  *      initialize Emulation Switch Flag
  *
  *
  *  @param[in]    VOID
  *
  *  @return EFI_SUCCESS   The function always returns EFI_SUCCESS.
  *
  **/
EFI_STATUS
InitEmulationFlag (
  VOID
);

 /**
  *      Get Emulation switch flag
  *
  *
  *  @param[in, out]    PresilCtrlPtr   = Printer to Amd emulation flag heap buffer
  *  @return BOOLEAN    Returns TRUE if in emulatoin run. False if run in HW
  *
  **/
BOOLEAN
GetEmulationFlag (
    IN OUT PRESILICON_CONTROL_STRUCT **PresilCtrlPtr
);

/**
 *
 *  EmulationFlagCheck
 *
 *  Check the external environment to see if it matches the
 *  value passed in. Also, check the enabled control flags to
 *  see if the flag that is passed in is true. If these
 *  conditions are met, this functions will return true,
 *  enabling the woerkaround.
 * 
 *
 * */
BOOLEAN
EmulationFlagCheck (
  UINT8 Env,
  UINT8 CtrlFlag
);

VOID
DisplayEmulationFlag (
  VOID
);

#if (IDSOPT_PRESILICON_ENABLED == TRUE)

  /*
   * Pre-Silicon Skip Macro
   *
   */
  #define PRE_SILICON_SKIP(env, ctrlflag)\
    if (!EmulationFlagCheck(env, ctrlflag))

  /*
   * Pre-Silicon Insert Macro
   */
  #define PRE_SILICON_INSERT(env, ctrlflag, function, params)\
    if (EmulationFlagCheck(env, ctrlflag)) {\
      function params;\
    }\

  #define DISPLAY_EMU_FLAG()\
    DisplayEmulationFlag();

  #define PRE_SILICON_INIT(Initfunction)\
    Initfunction ();

#else // IDSOPT_PRESILICON_ENABLED == TRUE

  #define PRE_SILICON_SKIP(env, ctrlflag)
  #define PRE_SILICON_INSERT(function, params)
  #define DISPLAY_EMU_FLAG()
  #define PRE_SILICON_INIT(Initfunction)

#endif // IDSOPT_PRESILICON_ENABLED == TRUE

#pragma pack (pop)
#endif // _AMD_EMULATION_FLAG_LIB_H_


