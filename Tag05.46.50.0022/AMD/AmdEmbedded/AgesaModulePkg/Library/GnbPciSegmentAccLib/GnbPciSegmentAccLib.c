/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "Porting.h"
#include  "AMD.h"
#include  "Filecode.h"
#include  <Library/BaseLib.h>
#include  <Library/PciSegmentLib.h>
#include  <Library/AmdBaseLib.h>
#include  <Library/IdsLib.h>
#include  <Library/GnbPciAccLib.h>
#include  <Library/AmdS3SaveLib.h>

#define FILECODE LIBRARY_GNBPCISEGMENTACCLIB_GNBPCISEGMENTACCLIB_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Write PCI registers
 *
 *
 *
 * @param[in] Address         PCI address (as presented in PCI_ADDR.AddressValue)
 * @param[in] Width           Access width
 * @param[in] Value           Pointer to value
 * @param[in] StdHeader       Pointer to standard header
 */
VOID
GnbLibPciWrite (
  IN      UINT32              Address,
  IN      ACCESS_WIDTH        Width,
  IN      VOID                *Value,
  IN      AMD_CONFIG_PARAMS   *StdHeader
  )
{
  PCI_ADDR PciAddr;
  UINT64   PciSegAddr;

  if (Width >= AccessS3SaveWidth8) {
    AmdS3SaveScriptPciWrite (Width - AccessS3SaveWidth8 + 1, Address, Value);
    // IDS_HDT_CONSOLE (NB_MISC, "  R S3 SAVE Script: Address 0x%08x, Width 0x%08x \n", Address, Width);
  }

  PciAddr.AddressValue = Address;
  PciSegAddr = PCI_SEGMENT_LIB_ADDRESS ((UINT64) PciAddr.Address.Segment,
                                        (UINT64) PciAddr.Address.Bus,
                                        (UINT64) PciAddr.Address.Device,
                                        (UINT64) PciAddr.Address.Function,
                                        (UINT64) PciAddr.Address.Register);

  switch (Width) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    PciSegmentWrite8 (PciSegAddr, *((UINT8 *) Value));
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    PciSegmentWrite16 (PciSegAddr, *((UINT16 *) Value));
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    PciSegmentWrite32 (PciSegAddr, *((UINT32 *) Value));
    break;
  default:
    IDS_ERROR_TRAP;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Read PCI registers
 *
 *
 *
 * @param[in]   Address         PCI address (as presented in PCI_ADDR.AddressValue)
 * @param[in]   Width           Access width
 * @param[out]  Value           Pointer to value
 * @param[in]   StdHeader       Pointer to standard header
 */

VOID
GnbLibPciRead (
  IN       UINT32             Address,
  IN       ACCESS_WIDTH       Width,
     OUT   VOID               *Value,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  PCI_ADDR PciAddr;
  UINT64   PciSegAddr;

  PciAddr.AddressValue = Address;
  PciSegAddr = PCI_SEGMENT_LIB_ADDRESS ((UINT64) PciAddr.Address.Segment,
                                        (UINT64) PciAddr.Address.Bus,
                                        (UINT64) PciAddr.Address.Device,
                                        (UINT64) PciAddr.Address.Function,
                                        (UINT64) PciAddr.Address.Register);

  switch (Width) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    *((UINT8 *) Value) = PciSegmentRead8 (PciSegAddr);
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    *((UINT16 *) Value) = PciSegmentRead16 (PciSegAddr);
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    *((UINT32 *) Value) = PciSegmentRead32 (PciSegAddr);
    break;
  default:
    IDS_ERROR_TRAP;
  }
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to Read/Modify/Write all register spaces.
 *
 *
 *
 * TODO
 *
 */
VOID
GnbLibPciRmw (
  IN       UINT32              Address,
  IN       ACCESS_WIDTH        Width,
  IN       UINT32              Mask,
  IN       UINT32              OrValue,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32  Data;
  GnbLibPciRead (Address, Width, &Data, StdHeader);
  Data = (Data & Mask) | OrValue;
  GnbLibPciWrite (Address, Width, &Data, StdHeader);
}


