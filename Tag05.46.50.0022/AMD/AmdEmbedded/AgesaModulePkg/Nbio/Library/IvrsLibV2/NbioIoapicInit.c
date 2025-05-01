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


#include  <AGESA.h>
#include  <Gnb.h>
#include  <PiDxe.h>
#include  <GnbDxio.h>
#include  <GnbIommu.h>
#include  <Filecode.h>
#include  <Library/IdsLib.h>
#include  <Library/AmdBaseLib.h>
#include  <Library/GnbCommonLib.h>
#include  <Library/PcdLib.h>
#include  <Library/SmnAccessLib.h>
#include  <Library/PcieConfigLib.h>
#include  <Library/NbioHandleLib.h>

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#define FILECODE NBIO_LIBRARY_IVRSLIBV2_NBIOIOAPICINIT_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define DEFAULT_NBIO_APIC_ID    0x20
#define DEFAULT_FCH_APIC_ID     0x1F

#define FCH_IOAPIC_ID_ADDRESS   0xFEC00010

//
// IOAPIC_ID_REGISTER Register Bitfields:
//

/// Bitfield Description :
#define IOAPIC_ID_REGISTER_Reserved_23_0_OFFSET                0
#define IOAPIC_ID_REGISTER_Reserved_23_0_WIDTH                 24
#define IOAPIC_ID_REGISTER_Reserved_23_0_MASK                  0xffffff

/// Bitfield Description : IOAPIC device ID.
#define IOAPIC_ID_REGISTER_DEV_ID_OFFSET                       24
#define IOAPIC_ID_REGISTER_DEV_ID_WIDTH                        4
#define IOAPIC_ID_REGISTER_DEV_ID_MASK                         0xf000000

/// Bitfield Description : Extended device id, writable only if Ioapic_id_ext_en is set.
#define IOAPIC_ID_REGISTER_EXTEND_ID_OFFSET                    28
#define IOAPIC_ID_REGISTER_EXTEND_ID_WIDTH                     4
#define IOAPIC_ID_REGISTER_EXTEND_ID_MASK                      0xf0000000L

typedef union {
  struct {
    UINT32                                           Reserved_23_0:24; ///<
    UINT32                                                  DEV_ID:4 ; ///<
    UINT32                                               EXTEND_ID:4 ; ///<
  } Field;                                                             ///<
  UINT32 Value;                                                        ///<
} IOAPIC_ID_REGISTER_STRUCT;


//
// Register Name IOAPIC_ID_REGISTER
//
#define SMN_IOAPIC_ID_REGISTER_ADDRESS                         0x2801000UL

#ifndef NBIO_SPACE
  #define  NBIO_SPACE(HANDLE, ADDRESS)   (ADDRESS + (HANDLE->RBIndex << 20))
#endif

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
 * Set IOAPIC IDs for all IOAPICs (FCH and NBIO)
 *
 *
 * @param[in]  GnbHandle       GNB handle
 *
 */
VOID
NbioAssignIoapicIds (
  IN       GNB_HANDLE               *GnbHandle
  )
{
  IOAPIC_ID_REGISTER_STRUCT         ApicIdValue;
  GNB_HANDLE                        *NbioHandle;

  if ((PcdGet8 (PcdCfgFchIoapicId) == 00) || (PcdGet8 (PcdCfgFchIoapicId) == 0xFF)) {
    PcdSet8S (PcdCfgFchIoapicId, DEFAULT_FCH_APIC_ID);
  }

  ApicIdValue.Value = (UINT32) (PcdGet8 (PcdCfgFchIoapicId) << 24);

  LibAmdMemWrite (AccessWidth32, FCH_IOAPIC_ID_ADDRESS, &ApicIdValue, (AMD_CONFIG_PARAMS *)NULL);
  IDS_HDT_CONSOLE (GNB_TRACE, "FchIoapicValue Value 0x%x\n", ApicIdValue);

  if ((PcdGet8 (PcdCfgGnbIoapicId) == 00) || (PcdGet8 (PcdCfgGnbIoapicId) == 0xFF)) {
    PcdSet8S (PcdCfgGnbIoapicId, DEFAULT_NBIO_APIC_ID);
  }
  ApicIdValue.Value = (UINT32) (PcdGet8 (PcdCfgGnbIoapicId) << 24);

  NbioHandle = GnbHandle;
  while (NbioHandle != NULL) {
    SmnRegisterWriteS (NbioHandle->Address.Address.Segment,
                       NbioHandle->Address.Address.Bus,
                       NBIO_SPACE(NbioHandle, SMN_IOAPIC_ID_REGISTER_ADDRESS),
                       &ApicIdValue,
                       GNB_REG_ACC_FLAG_S3SAVE
                      );
    IDS_HDT_CONSOLE (MAIN_FLOW, "PcdCfgGnbIoapicId Value 0x%x\n", ApicIdValue);
    ApicIdValue.Field.DEV_ID++;
    NbioHandle = GnbGetNextHandle (NbioHandle);
  }
  NbioHandle = NbioGetAltHandle (PcieConfigGetPlatform(GnbHandle));
  while (NbioHandle != NULL) {
    SmnRegisterWriteS (NbioHandle->Address.Address.Segment,
                       NbioHandle->Address.Address.Bus,
                       SMN_IOAPIC_ID_REGISTER_ADDRESS,
                       &ApicIdValue,
                       GNB_REG_ACC_FLAG_S3SAVE
                      );
    IDS_HDT_CONSOLE (MAIN_FLOW, "PcdCfgGnbIoapicId Value 0x%x\n", ApicIdValue);
    ApicIdValue.Field.DEV_ID++;
    NbioHandle = GnbGetNextHandle (NbioHandle);
  }
}


