/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include "AGESA.h"
#include "Filecode.h"
#include <Library/BaseLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/AmdHeapLib.h>
#include <Library/PciLib.h>
#include <Library/AmdEmulationFlagLib.h>
#include <Library/PresiliconControlRmbLib.h>

#define FILECODE LIBRARY_PRESILICONCONTROLRMBLIB_PRESILICONCONTROLRMBLIB_FILECODE

 /*----------------------------------------------------------------------------------------
  *                   D E F I N I T I O N S    A N D    M A C R O S
  *----------------------------------------------------------------------------------------
  */
#define NB_SMN_INDEX_2_PCI_ADDR     0x000000B8ul  ///< PCI Addr of NB_SMN_INDEX_2
#define NB_SMN_DATA_2_PCI_ADDR      0x000000BCul  ///< PCI Addr of NB_SMN_DATA_2
 /*----------------------------------------------------------------------------------------
  *                  T Y P E D E F S     A N D     S T R U C T U R E S
  *----------------------------------------------------------------------------------------
  */


 /*----------------------------------------------------------------------------------------
  *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
  *----------------------------------------------------------------------------------------
  */
/**

  Light version SMN Read with less depx
  @param[in]  Address, SMN address
  @return  Value, Value read from the address
**/
UINT32
SmnRegRead (
  UINT32 Address
  )
{
  UINTN                  PciAddress;

  PciAddress = MAKE_SBDFO (0, 0, 0, FUNC_0, NB_SMN_INDEX_2_PCI_ADDR);
  PciWrite32 (PciAddress, Address);
  PciAddress = MAKE_SBDFO (0, 0, 0, FUNC_0, NB_SMN_DATA_2_PCI_ADDR);
  return PciRead32 (PciAddress);
}

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
InitEmulationFlagRmb (
  VOID
)
{
  ALLOCATE_HEAP_PARAMS        AllocHeapParams;
  LOCATE_HEAP_PTR             LocateHeapParams;
  UINT8                       *EnvFlagesHeapBuffer = NULL;
  PRESILICON_CONTROL_STRUCT   *PresilCtrlPtr = NULL;
  PRESIL_CTRL0_RMB            PresilCtrl0;
  PRESIL_CTRL1_RMB            PresilCtrl1;
  PRESILICON_ENV_TYPE         EnvType;
  UINT8                       EnvRaw;
  AGESA_STATUS                AgesaStatus;

  LocateHeapParams.BufferHandle = AMD_EMULATION_FLAG;
  if (HeapLocateBuffer (&LocateHeapParams, NULL) == AGESA_SUCCESS) {
    EnvFlagesHeapBuffer = LocateHeapParams.BufferPtr;
  } else {
    //Allocate Heap buffer for PSP directory
    AllocHeapParams.RequestedBufferSize = MaxEnvFlagesSize;
    AllocHeapParams.BufferHandle        = AMD_EMULATION_FLAG;
    AllocHeapParams.Persist             = HEAP_BOOTTIME_SYSTEM_MEM;
    AgesaStatus = HeapAllocateBuffer (&AllocHeapParams, NULL);
    ASSERT (AgesaStatus == AGESA_SUCCESS);
    if (AgesaStatus != AGESA_SUCCESS) {
      return FALSE;
    }
    EnvFlagesHeapBuffer = AllocHeapParams.BufferPtr;
  }

  PresilCtrl0.Value = SmnRegRead (MP0_C2PMSG_97);
  PresilCtrl1.Value = SmnRegRead (MP0_C2PMSG_98);

  switch (PresilCtrl0.Field.Environment) {
    case PRESIL_CTRL_ENV_HW:
      EnvType = ENV_HW;
      break;
    case PRESIL_CTRL_ENV_SOC_EMULATION:
      EnvType = ENV_EMULATION;
      break;
    case PRESIL_CTRL_ENV_COBRA:
    case PRESIL_CTRL_ENV_GIO:
    case PRESIL_CTRL_ENV_GC_LITE:
      EnvType = ENV_SIMULATION;
      break;
    default:
      EnvType = ENV_HW;
      break;
  }

  EnvRaw = (UINT8)PresilCtrl0.Field.Environment;

  PresilCtrlPtr = (PRESILICON_CONTROL_STRUCT *)EnvFlagesHeapBuffer;

  //
  // Set the Environment Type
  //
  PresilCtrlPtr->EnvType= EnvType;
  //
  // Set the Environment Raw
  //
  PresilCtrlPtr->EnvRaw = EnvRaw;
  //
  // Set CoreClNum
  //
  PresilCtrlPtr->CoreClNum = (UINT8)PresilCtrl0.Field.CoreClNum;

  //
  // Look for the control struct or allocate a new one. Populate it with the
  //  type and flags from the external environment.
  //
  PresilCtrlPtr->ControlFlags[NO_FLAGS] = TRUE;
  PresilCtrlPtr->ControlFlags[PRESIL_SKIP_ALL_USBCONTROLLER_ACCESS] = (PresilCtrl0.Field.SkipAllUSBControllerAccess == 1) ? TRUE : FALSE;
  PresilCtrlPtr->ControlFlags[PRESIL_SKIP_ALL_USBPHY_ACCESS] = (PresilCtrl0.Field.SkipAllUSBPhyAccess == 1) ? TRUE : FALSE;
  PresilCtrlPtr->ControlFlags[PRESIL_SKIP_DXIO_INITIALIZAION] = (PresilCtrl0.Field.SkipDxioInitializaion == 1) ? TRUE : FALSE;
  PresilCtrlPtr->ControlFlags[PRESIL_FASTSIM_GIO] = (PresilCtrl1.Field.FASTSIM_GIO == 1) ? TRUE : FALSE;
  PresilCtrlPtr->ControlFlags[PRESIL_FASTSIM_DXIO] = (PresilCtrl1.Field.FASTSIM_DXIO == 1) ? TRUE : FALSE;
  PresilCtrlPtr->ControlFlags[PRESIL_FASTSIM_DFLT_TBL] = (PresilCtrl1.Field.FASTSIM_DFLT_TBL == 1) ? TRUE : FALSE;
  PresilCtrlPtr->ControlFlags[PRESIL_FASTSIM_SMU_MSGS] = (PresilCtrl1.Field.FASTSIM_SMU_MSGS == 1) ? TRUE : FALSE;
  PresilCtrlPtr->ControlFlags[PRESIL_DISABLE_EC] = (PresilCtrl1.Field.DISABLE_EC == 1) ? TRUE : FALSE;
  PresilCtrlPtr->ControlFlags[PRESIL_FASTSIM_PEI_LOG] = (PresilCtrl1.Field.FASTSIM_PEI_LOG == 1) ? TRUE : FALSE;
  PresilCtrlPtr->ControlFlags[PRESIL_FASTSIM_DXE_LOG] = (PresilCtrl1.Field.FASTSIM_DXE_LOG == 1) ? TRUE : FALSE;


  return EFI_SUCCESS;
}
