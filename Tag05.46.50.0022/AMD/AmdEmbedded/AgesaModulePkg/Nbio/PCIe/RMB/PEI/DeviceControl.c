/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * NBIF Device Control Functions
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 276746 $   @e \$Date: 2014-10-28 12:22:52 -0600 (Mon, 28 Oct 2013) $
 *
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <AMD.h>
#include <Gnb.h>
#include <PiPei.h>
#include <Filecode.h>
#include <GnbRegistersRMB.h>
#include <AmdPcieComplex.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/SmnAccessLib.h>

#include "AmdNbioPciePei.h"

#define FILECODE NBIO_PCIE_RMB_PEI_DEVICECONTROL_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

typedef struct {
  UINT8      NbifFunctionType;       ///< Function type, see RMB_NBIF_FUNCTION_TYPES
  UINT8      DeviceNumber;           ///< NBIF Device Number
  UINT8      FunctionNumber;         ///< NBIF Function Number
} NBIF_CONFIG_ENTRY;

#define NBIF_CONFIG_ENTRY_CONSTRUCTOR(FUNCTION_TYPE, STRAP_DEV, STRAP_FUN) \
        { FUNCTION_TYPE, STRAP_DEV, STRAP_FUN }

// Define PCIe device/functions to avoid using magic numbers where they are utilized
#define PCIE_DEVICE0    0
#define PCIE_DEVICE1    1
#define PCIE_DEVICE2    2

#define PCIE_FUNCTION0  0
#define PCIE_FUNCTION1  1
#define PCIE_FUNCTION2  2
#define PCIE_FUNCTION3  3
#define PCIE_FUNCTION4  4
#define PCIE_FUNCTION5  5
#define PCIE_FUNCTION6  6
#define PCIE_FUNCTION7  7

// This macro supplies a port strap address given a base address, device and function. They follow a pattern thus enabling this macro
#define  NBIF_PORT_STRAP_ADDRESS(BASE_ADDR, DEVICE)   (BASE_ADDR + ((DEVICE * 2) << 8))

// This macro supplies a device strap address given a base address, device and function. They follow a pattern thus enabling this macro
#define  NBIF_DEVICE_STRAPx_ADDRESS(BASE_ADDR, DEVICE, FUNCTION)   (BASE_ADDR + ((2 * FUNCTION) << 8) + (DEVICE << 12))

// This macro gives us the strap0 address.
#define  NBIF_DEVICE_STRAP0_ADDRESS(DEVICE, FUNCTION)   NBIF_DEVICE_STRAPx_ADDRESS(SMN_NBIF0INTERNAL_RCC_DEV0_EPF0_STRAP0_ADDRESS, DEVICE, FUNCTION)

// These offsets remain the same for all d/f
#define NBIF_STRAP0_FUNCTION_ENABLE_OFFSET RCC_DEV0_EPF0_STRAP0_STRAP_FUNC_EN_DEV0_F0_OFFSET
#define NBIF_STRAP0_DUMMY_F0_ENABLE_OFFSET RCC_DEV0_PORT_STRAP0_STRAP_EPF0_DUMMY_EN_DEV0_OFFSET

// This address remains the same for all d/f
#define NBIF_INTR_LINE_ENABLE_ADDRESS SMN_NBIF0_INTR_LINE_ENABLE_ADDRESS

// This macrfo gives us the interrupt enable bit offset
#define NBIF_INTR_LINE_ENABLE_OFFSET(DEVICE, FUNCTION)   (8 * DEVICE + FUNCTION)

// The below table comments should match PPR 13.6.9.1 ver .10. ,
// The array index must be the same as RMB_NBIF_FUNCTION_TYPES enum value. This could change later if we implement
// more advanced functionality but for now keep it a simple LUT
NBIF_CONFIG_ENTRY mNbifConfigurations [] =
{
  // NBIF0 Port0 F0 : GFX
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionGfx,
    PCIE_DEVICE0,
    PCIE_FUNCTION0
    ),
  // NBIF0 Port0 F1 : DCE.AZ
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionDceAz,
    PCIE_DEVICE0,
    PCIE_FUNCTION1
    ),
  // NBIF0 Port0 F2 : PSPPCCP
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionPspCcp,
    PCIE_DEVICE0,
    PCIE_FUNCTION2
    ),
  // NBIF0 Port0 F3 : USB3.1_0
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionUsb3Gen1_0,
    PCIE_DEVICE0,
    PCIE_FUNCTION3
    ),
  // NBIF0 Port0 F4 : USB3.1_1
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionUsb3Gen1_1,
    PCIE_DEVICE0,
    PCIE_FUNCTION4
    ),
  // NBIF0 Port0 F5 : ACP
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionAcp,
    PCIE_DEVICE0,
    PCIE_FUNCTION5
    ),
  // NBIF0 Port0 F6 : HDAudio
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionHdAudio,
    PCIE_DEVICE0,
    PCIE_FUNCTION6
    ),
  // NBIF0 Port0 F7 : MP2
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionMp2,
    PCIE_DEVICE0,
    PCIE_FUNCTION7
    ),
  // NBIF0 Port1 F0 : SATA 0  - 8
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionSata,
    PCIE_DEVICE1,
    PCIE_FUNCTION0
    ),
  // NBIF0 Port1 F1 : SATA 1  - 9
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionSata1,
    PCIE_DEVICE1,
    PCIE_FUNCTION1
    ),
  // NBIF0 Port1 F2 : XGBE0   - 10
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionXgbe0,
    PCIE_DEVICE1,
    PCIE_FUNCTION2
    ),
  // NBIF0 Port1 F3 : XGBE1   - 11
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionXgbe1,
    PCIE_DEVICE1,
    PCIE_FUNCTION3
    ),
  // NBIF0 Port2 F0 : WLAN
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionWlan,
    PCIE_DEVICE2,
    PCIE_FUNCTION0
    ),
  // NBIF0 Port2 F1 : BT
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionBt,
    PCIE_DEVICE2,
    PCIE_FUNCTION1
    ),
  // NBIF0 Port2 F2 : I2S/AC97
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionI2sAC97,
    PCIE_DEVICE2,
    PCIE_FUNCTION2
    ),
  // End Of List
  NBIF_CONFIG_ENTRY_CONSTRUCTOR (
    NbifFunctionUnused,
    0,
    0
    ),
};


/*----------------------------------------------------------------------------------------*/
/**
 * NBIF Device Enable/Disable
 *
 *
 * @param[in]  GnbHandle          Handle to current NBIO instance
 * @param[in]  DeviceType         Device type as per RMB_NBIF_FUNCTION_TYPES
 * @param[in]  EnableOrDisable    Boolean indicating if device should be disabled or enabled
 *
 */
VOID
NbioDisableEnableNbifDevice (
  IN        GNB_HANDLE         *GnbHandle,
  IN        UINT8              DeviceType,
  IN        BOOLEAN            EnableOrDisable
  )
{
  UINT32 Mask;
  UINT8  Dev, Fun;

  IDS_HDT_CONSOLE(GNB_TRACE, "%a : (DeviceType=%x)\n", __FUNCTION__, DeviceType);
  //
  // Bounds check and make sure the entry is actually the one we think it is
  //
  ASSERT (DeviceType < NbifFunctionUnused);
  ASSERT (DeviceType == mNbifConfigurations[DeviceType].NbifFunctionType);

  Dev = mNbifConfigurations[DeviceType].DeviceNumber;
  Fun = mNbifConfigurations[DeviceType].FunctionNumber;

  //
  // Interrupt line disable/enable
  //
  Mask = (UINT32) ~(1 << NBIF_INTR_LINE_ENABLE_OFFSET(Dev, Fun));
  SmnRegisterRMW (
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE (GnbHandle, NBIF_INTR_LINE_ENABLE_ADDRESS),
    Mask,
    (EnableOrDisable << NBIF_INTR_LINE_ENABLE_OFFSET(Dev, Fun)),
    0
    );
  //
  // Hit the disable/enable bits in strap
  //
  SmnRegisterRMW (
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE (GnbHandle, NBIF_DEVICE_STRAP0_ADDRESS(Dev, Fun)),
    (UINT32) ~(1 << NBIF_STRAP0_FUNCTION_ENABLE_OFFSET), // This offset is the same for all d/f
    (EnableOrDisable << NBIF_STRAP0_FUNCTION_ENABLE_OFFSET),
    0
    );
  IDS_HDT_CONSOLE(GNB_TRACE, "%a : D %d F %d Int Offset %d Strap Addr %x \n", __FUNCTION__, Dev, Fun, NBIF_INTR_LINE_ENABLE_OFFSET(Dev, Fun), NBIO_SPACE (GnbHandle, NBIF_DEVICE_STRAP0_ADDRESS(Dev, Fun)));

  // PPR 13.6.9.5.4
  // If a function 0 device was requested to be disabled we must enable the dummy function to not lose the other devices
  // NOTE: no support for re enabling fn0 after disabling. We can add this later in the unlikely event it is ever needed
  if (Fun == 0 && EnableOrDisable == FALSE) {
    Mask = (UINT32) ~(1 << NBIF_STRAP0_DUMMY_F0_ENABLE_OFFSET); // This offset is the same for all d/f
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE (GnbHandle, NBIF_PORT_STRAP_ADDRESS (SMN_NBIF0INTERNAL_RCC_DEV0_PORT_STRAP0_ADDRESS, Dev)),
      ~RCC_DEV0_PORT_STRAP0_STRAP_EPF0_DUMMY_EN_DEV0_MASK,
      (UINT32)(1 << NBIF_STRAP0_DUMMY_F0_ENABLE_OFFSET),
      0
      );
    if (Dev == 0) {
      // Graphics disable only
      SmnRegisterRMW (
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SMN_NBIF0INTERNAL_RCC_DEV0_EPF1_STRAP0_ADDRESS),
        (UINT32) ~RCC_DEV0_EPF1_STRAP0_STRAP_FUNC_EN_DEV0_F1_MASK,
        (UINT32)(0 << RCC_DEV0_EPF1_STRAP0_STRAP_FUNC_EN_DEV0_F1_OFFSET),
        0
        );
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE (GnbHandle, NBIF_DEVICE_STRAPx_ADDRESS(SMN_NBIF0INTERNAL_RCC_DEV0_EPF0_STRAP0_ADDRESS, Dev, Fun)),
      (UINT32) ~RCC_DEV0_EPF0_STRAP0_STRAP_DEVICE_ID_DEV0_F0_MASK,
      (UINT32)(0x145A << RCC_DEV0_EPF0_STRAP0_STRAP_DEVICE_ID_DEV0_F0_OFFSET),
      0
      );
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE (GnbHandle, NBIF_DEVICE_STRAPx_ADDRESS(SMN_NBIF0INTERNAL_RCC_DEV0_EPF0_STRAP13_ADDRESS, Dev, Fun)),
      (UINT32) ~(RCC_DEV0_EPF0_STRAP13_STRAP_CLASS_CODE_PIF_DEV0_F0_MASK |
      RCC_DEV0_EPF0_STRAP13_STRAP_CLASS_CODE_SUB_DEV0_F0_MASK |
      RCC_DEV0_EPF0_STRAP13_STRAP_CLASS_CODE_BASE_DEV0_F0_MASK),
      (0x0 << RCC_DEV0_EPF0_STRAP13_STRAP_CLASS_CODE_PIF_DEV0_F0_OFFSET) |
      (0x0 << RCC_DEV0_EPF0_STRAP13_STRAP_CLASS_CODE_SUB_DEV0_F0_OFFSET) |
      (0x13 << RCC_DEV0_EPF0_STRAP13_STRAP_CLASS_CODE_BASE_DEV0_F0_OFFSET),
      0
      );
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE (GnbHandle, NBIF_DEVICE_STRAPx_ADDRESS(SMN_NBIF0INTERNAL_RCC_DEV0_EPF0_STRAP4_ADDRESS, Dev, Fun)),
      (UINT32) ~RCC_DEV0_EPF0_STRAP4_STRAP_INTERRUPT_PIN_DEV0_F0_MASK,
      (UINT32)(0x0 << RCC_DEV0_EPF0_STRAP4_STRAP_INTERRUPT_PIN_DEV0_F0_OFFSET),
      0
      );
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE (GnbHandle, NBIF_DEVICE_STRAPx_ADDRESS(SMN_NBIF0INTERNAL_RCC_DEV0_EPF0_STRAP3_ADDRESS, Dev, Fun)),
      (UINT32) ~(RCC_DEV0_EPF0_STRAP3_STRAP_MSI_EN_DEV0_F0_MASK |
      RCC_DEV0_EPF0_STRAP3_STRAP_MSIX_EN_DEV0_F0_MASK),
      (0x0 << RCC_DEV0_EPF0_STRAP3_STRAP_MSI_EN_DEV0_F0_OFFSET) |
      (0x0 << RCC_DEV0_EPF0_STRAP3_STRAP_MSIX_EN_DEV0_F0_OFFSET),
      0
      );
    SmnRegisterRMW (
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE (GnbHandle, NBIF_DEVICE_STRAPx_ADDRESS(SMN_NBIF0INTERNAL_RCC_DEV0_EPF0_STRAP4_ADDRESS, Dev, Fun)),
      (UINT32) ~RCC_DEV0_EPF0_STRAP4_STRAP_PME_SUPPORT_DEV0_F0_MASK,
      (UINT32)(0x0 << RCC_DEV0_EPF0_STRAP4_STRAP_PME_SUPPORT_DEV0_F0_OFFSET),
      0
      );
      // PPR states: NBIFMM::RCC_DEV0_EPF0_STRAP3[STRAP_VENDOR_ID_BIT_DEV0_F0] = 0x1 ( make vendor id to 1022)
      // But no such bitfield in that register
    }
    if (Dev == 1) {
      // Sata disable only
      SmnRegisterRMW (
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SMN_NBIF0_NBIF_SMN_VWR_VCHG_DIS_CTRL_ADDRESS),
        (UINT32) ~(NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET1_DIS_MASK |
        NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET0_DIS_MASK),
        (0x1 << NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET1_DIS_OFFSET) |
        (0x1 << NBIF_SMN_VWR_VCHG_DIS_CTRL_SMN_VWR_VCHG_SET0_DIS_OFFSET),
        0
        );
    }
  }

  return;
}


VOID
NbioDisableNbifDevice (
  IN        GNB_HANDLE         *GnbHandle,
  IN        UINT8              DeviceType,
  IN        UINT8              StartLane
  )
{
  NbioDisableEnableNbifDevice (GnbHandle, DeviceType, FALSE);
}
