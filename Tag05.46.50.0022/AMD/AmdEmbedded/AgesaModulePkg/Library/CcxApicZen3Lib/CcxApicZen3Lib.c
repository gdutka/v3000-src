/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CCX APIC Library
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision$   @e \$Date$
 *
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <AMD.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/CcxApicLib.h>
#include <Library/CcxBaseX86Lib.h>
#include <Library/SmnAccessLib.h>
#include <Library/FabricRegisterAccLib.h>
#include <FabricRegistersDf3.h>
#include <CcxRegistersZen3.h>

#define FILECODE LIBRARY_CCXAPICZEN3LIB_CCXAPICZEN3LIB_FILECODE

#define MAX_CORES_PER_CCD 8
#define MAX_CORES_PER_SKT 64
/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
UINT8
BitWidth (
  IN      UINTN   Num
  );

 /*----------------------------------------------------------------------------------------
  *                          E X P O R T E D    F U N C T I O N S
  *----------------------------------------------------------------------------------------
 */

 /*---------------------------------------------------------------------------------------*/
 /**
  *
  * Returns the APIC Id based on the provided inputs
  *
  * @param[in]      Socket              Socket # of APIC Id to calculate
  * @param[in]      Die                 Die # of APIC Id to calculate
  * @param[in]      Ccd                 CCD # of APIC Id to calculate
  * @param[in]      Complex             Complex # of APIC Id to calculate
  * @param[in]      Core                Core # of APIC Id to calculate
  * @param[in]      Thread              Thread # of APIC Id to calculate
  *
  * @return         UINT8               Calculate APIC Id
  */
UINT32
CcxCalcLocalApic (
  IN       UINTN    Socket,
  IN       UINTN    Die,
  IN       UINTN    Ccd,
  IN       UINTN    Complex,
  IN       UINTN    Core,
  IN       UINTN    Thread
  )
{
  PMREG_INITPKG7_STRUCT     InitPkg7;
  UINT64                    CoreEnable;
  UINT8                     NumberOfSockets;
  UINT8                     NumberOfCcds;
  UINT8                     NumberOfCores;
  UINT8                     NumberOfThreads;
  UINT8                     SocketShift;
  UINT8                     CcdShift;
  UINT8                     CoreShift;
  UINT8                     ApicId;
  UINT8                     CcdIdx;
  UINT8                     CoreIdx;

  ASSERT (Socket < 2);
  ASSERT (Die == 0);
  ASSERT (Ccd < 8);
  ASSERT (Complex == 0);
  ASSERT (Core < 8);
  ASSERT (Thread < 2);

  // some CCDs/Cores may be software/fuse disabled, so find enabled one to read from
  CoreEnable = FabricRegisterAccRead (0, 0, PHYCOREENABLE0_FUNC, PHYCOREENABLE0_REG, FABRIC_REG_ACC_BC);
  CoreEnable |= (UINT64) (FabricRegisterAccRead (0, 0, PHYCOREENABLE1_FUNC, PHYCOREENABLE1_REG, FABRIC_REG_ACC_BC)) << 32;
  for (CoreIdx = 0; (CoreIdx < MAX_CORES_PER_SKT) && ((CoreEnable & 0x1) == 0); CoreEnable >>= 1, CoreIdx++);
  ASSERT(CoreIdx < MAX_CORES_PER_SKT);

  CcdIdx  = CoreIdx / MAX_CORES_PER_CCD;
  CoreIdx = CoreIdx % MAX_CORES_PER_CCD;
  SmnRegisterRead (0, (SMN_PMREG_INITPKG7_ADDRESS + (SMN_PMREG_INITPKG7_CDD_OFFSET * CcdIdx) + (SMN_PMREG_INITPKG7_CORE_OFFSET * CoreIdx)), &InitPkg7);

  NumberOfSockets = (UINT8) InitPkg7.Field.NumOfSockets;
  NumberOfCcds    = (UINT8) InitPkg7.Field.NumOfCcds;
  NumberOfCores   = (UINT8) InitPkg7.Field.NumOfCores;
  NumberOfThreads = CcxGetThreadsPerCore () - 1;

  ASSERT (Socket <= NumberOfSockets);
  ASSERT (Ccd <= NumberOfCcds);
  ASSERT (Core <= NumberOfCores);
  ASSERT (Thread <= NumberOfThreads);

  CoreShift = NumberOfThreads;
  CcdShift = (BitWidth (NumberOfCores)) + CoreShift;
  SocketShift = (BitWidth (NumberOfCcds)) + CcdShift;

  ApicId = (UINT8) ((Socket << SocketShift) | (Ccd << CcdShift) | (Core << CoreShift) | Thread);

  return (UINT32) ApicId;
}

UINT8
BitWidth (
  IN      UINTN   Num
  )
{
  UINT8 Width;

  for (Width = 0; Num; Width++) {
    Num >>= 1;
  }
  return Width;
}
