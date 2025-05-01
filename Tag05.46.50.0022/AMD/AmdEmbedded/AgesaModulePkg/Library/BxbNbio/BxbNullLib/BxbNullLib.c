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
#include  <PiPei.h>
#include  <AGESA.h>
#include  <AmdPcieComplex.h>
#include  <Library/NbioHandleLib.h>
#include  <Library/IdsLib.h>
#include  <Filecode.h>

#define FILECODE LIBRARY_BXBNBIO_BXBNULLLIB_BXBNULLLIB_FILECODE

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
 * Clean up programming from early BMC initialization
 *
 *
 *
 * @param[in]     GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]     DieNumber      InstanceId of the Die that contains the BMC link
 */
BOOLEAN
BixbyFindEarlyLink (
  IN       DXIO_COMPLEX_DESCRIPTOR   *ComplexDescriptor,
  IN       GNB_HANDLE                *GnbHandle
  )
{
  return (FALSE);
}
