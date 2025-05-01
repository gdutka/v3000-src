/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/FchBaseLib.h>
#include <Library/BaseFabricTopologyLib.h>
#include <Library/PciSegmentLib.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_FCHBASELIB_FCHSMNLIB_FILECODE
#define IOHC_NB_SMN_INDEX_2_BIOS        0x00B8
#define IOHC_NB_SMN_DATA_2_BIOS         0x00BC


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchSmnRead - Read SMN register
 *
 * @param[in] IohcBus     - IOHC (Node) bus number
 * @param[in] SmnAddress  - Register SMN address
 * @param[in] Value       - Pointer to register value
 * @param[in] StdHeader   -
 *
 */
VOID
FchSmnRead (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32    RegIndex;
  UINT64    PciAddress;

  RegIndex = SmnAddress;
  PciAddress = PCI_SEGMENT_LIB_ADDRESS ((IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT), (IohcBus % MAX_PCI_BUS_NUMBER_PER_SEGMENT), 0, 0, IOHC_NB_SMN_INDEX_2_BIOS);
  PciSegmentWrite32 (PciAddress, RegIndex);
  PciAddress = PCI_SEGMENT_LIB_ADDRESS ((IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT), (IohcBus % MAX_PCI_BUS_NUMBER_PER_SEGMENT), 0, 0, IOHC_NB_SMN_DATA_2_BIOS);
  *Value = PciSegmentRead32 (PciAddress);
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchSmnWrite - Write SMN register
 *
 * @param[in] IohcBus     - IOHC (Node) bus number
 * @param[in] SmnAddress  - Register SMN address
 * @param[in] Value       - Pointer to register value
 * @param[in] StdHeader   -
 *
 */
VOID
FchSmnWrite (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32    RegIndex;
  UINT64    PciAddress;

  RegIndex = SmnAddress;
  PciAddress = PCI_SEGMENT_LIB_ADDRESS ((IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT), (IohcBus % MAX_PCI_BUS_NUMBER_PER_SEGMENT), 0, 0, IOHC_NB_SMN_INDEX_2_BIOS);
  PciSegmentWrite32 (PciAddress, RegIndex);
  PciAddress = PCI_SEGMENT_LIB_ADDRESS ((IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT), (IohcBus % MAX_PCI_BUS_NUMBER_PER_SEGMENT), 0, 0, IOHC_NB_SMN_DATA_2_BIOS);
  PciSegmentWrite32 (PciAddress, *Value);
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchSmnRW - Modify SMN register
 *
 * @param[in] IohcBus     - IOHC (Node) bus number
 * @param[in] SmnAddress  - Register SMN address
 * @param[in] AndMask  - Data And Mask 32 bits
 * @param[in] OrMask  - Data OR Mask 32 bits
 * @param[in] StdHeader   -
 *
 */
VOID
FchSmnRW (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              AndMask,
  IN       UINT32              OrMask,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32    RegValue;

  FchSmnRead (IohcBus, SmnAddress, &RegValue, StdHeader);
  RegValue &= AndMask;
  RegValue |= OrMask;
  FchSmnWrite (IohcBus, SmnAddress, &RegValue, StdHeader);
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchSmnRead8 - Read SMN register in Byte
 *
 * @param[in] IohcBus     - IOHC (Node) bus number
 * @param[in] SmnAddress  - Register SMN address
 * @param[in] Value8      - Pointer to register value
 * @param[in] StdHeader   -
 *
 */
VOID
FchSmnRead8 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT8               *Value8,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32    RegIndex;
  UINT64    PciAddress;

  RegIndex = SmnAddress & 0xFFFFFFFC;
  PciAddress = PCI_SEGMENT_LIB_ADDRESS ((IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT), (IohcBus % MAX_PCI_BUS_NUMBER_PER_SEGMENT), 0, 0, IOHC_NB_SMN_INDEX_2_BIOS);
  PciSegmentWrite32 (PciAddress, RegIndex);
  PciAddress = PCI_SEGMENT_LIB_ADDRESS ((IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT), (IohcBus % MAX_PCI_BUS_NUMBER_PER_SEGMENT), 0, 0, IOHC_NB_SMN_DATA_2_BIOS);
  *Value8 = PciSegmentRead8 (PciAddress + (SmnAddress & 0x3));
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchSmnWrite8 - Write SMN register in Byte
 *
 * @param[in] IohcBus     - IOHC (Node) bus number
 * @param[in] SmnAddress  - Register SMN address
 * @param[in] Value8      - Pointer to register value
 * @param[in] StdHeader   -
 *
 */
VOID
FchSmnWrite8 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT8               *Value8,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32    RegIndex;
  UINT64    PciAddress;

  RegIndex = SmnAddress & 0xFFFFFFFC;
  PciAddress = PCI_SEGMENT_LIB_ADDRESS ((IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT), (IohcBus % MAX_PCI_BUS_NUMBER_PER_SEGMENT), 0, 0, IOHC_NB_SMN_INDEX_2_BIOS);
  PciSegmentWrite32 (PciAddress, RegIndex);
  PciAddress = PCI_SEGMENT_LIB_ADDRESS ((IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT), (IohcBus % MAX_PCI_BUS_NUMBER_PER_SEGMENT), 0, 0, IOHC_NB_SMN_DATA_2_BIOS);
  PciSegmentWrite8 (PciAddress + (UINTN) (SmnAddress & 0x3), *Value8);
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchSmnRW8 - Modify SMN register
 *
 * @param[in] IohcBus     - IOHC (Node) bus number
 * @param[in] SmnAddress  - Register SMN address
 * @param[in] AndMask  - Data And Mask 8 bits
 * @param[in] OrMask  - Data OR Mask 8 bits
 * @param[in] StdHeader   -
 *
 */
VOID
FchSmnRW8 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT8               AndMask,
  IN       UINT8               OrMask,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8    RegValue;

  FchSmnRead8 (IohcBus, SmnAddress, &RegValue, StdHeader);
  RegValue &= AndMask;
  RegValue |= OrMask;
  FchSmnWrite8 (IohcBus, SmnAddress, &RegValue, StdHeader);
}


