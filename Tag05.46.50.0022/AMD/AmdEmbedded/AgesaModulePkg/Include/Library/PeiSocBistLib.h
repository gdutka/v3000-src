/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CCX BIST Library
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 */

#ifndef _PEI_SOC_BIST_LIB_H_
#define _PEI_SOC_BIST_LIB_H_


 AGESA_STATUS
 ReadCcxBistData (
   IN       UINTN            SocketNum,
   IN       UINTN            DieNum,
   IN       UINTN            CcdNum,
   IN       UINTN            CcxNum,
   IN       UINTN            CurrSysDieCount,
   IN       UINTN            NumberOfComplexes,
        OUT UINTN            *BistData,
   IN CONST EFI_PEI_SERVICES **PeiServices
  );

AGESA_STATUS
ReadNonCcxBistData (
  IN       UINTN            SocketNum,
  IN       UINTN            DieNum,
  IN       UINTN            CcdNum,
  IN       UINTN            CurrSysDieCount,
       OUT UINTN            *BistData,
  IN CONST EFI_PEI_SERVICES **PeiServices
  );

#endif

