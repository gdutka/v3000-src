/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#ifndef  _TS_FCH_TASK_LAUNCHER_H_
#define  _TS_FCH_TASK_LAUNCHER_H_


AGESA_STATUS
FchTaskLauncher (
  IN       FCH_TASK_ENTRY     **TaskPtr,
  IN       VOID               *FchCfg,
  IN       AGESA_POST_CODE           TestPoint
  );

#endif

