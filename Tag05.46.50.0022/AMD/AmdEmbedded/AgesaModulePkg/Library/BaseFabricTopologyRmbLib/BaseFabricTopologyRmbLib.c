/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Fabric Topology Base Lib implementation for RMB
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Fabric
 * @e \$Revision$   @e \$Date$
 *
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Library/BaseLib.h>
#include <Library/BaseFabricTopologyLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/CcxBaseX86Lib.h>
#include <Library/FabricRegisterAccLib.h>
#include <Library/AmdBaseLib.h>
#include <FabricRegistersDf3.h>
#include <FabricInfoRmb.h>
#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE  LIBRARY_BASEFABRICTOPOLOGYRMBLIB_BASEFABRICTOPOLOGYRMBLIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
CONST
COMPONENT_LOCATION *
FindComponentLocationMap (
  IN       UINTN *Count,
  IN       UINTN *PhysIoms0FabricId
  );

//
// RMB IOD
//
CONST UINTN ROMDATA RmbPhysIoms0FabricId = RMB_IOMS0_FABRIC_ID;

CONST COMPONENT_LOCATION ROMDATA RmbComponentLocation [] = {
  {0, 0, RMB_IOMS0_FABRIC_ID, PrimaryFch}, // Physical location, Socket 0, Die 0, Ioms0
  {0, 0, RMB_IOMS0_FABRIC_ID, PrimarySmu}, // Physical location, Socket 0, Die 0, Ioms0
};

CONST DEVICE_IDS ROMDATA  RmbCsMap [] = {
  {RMB_CS0_FABRIC_ID, RMB_CS0_INSTANCE_ID},
  {RMB_CS1_FABRIC_ID, RMB_CS1_INSTANCE_ID},
  {RMB_CS2_FABRIC_ID, RMB_CS2_INSTANCE_ID},
  {RMB_CS3_FABRIC_ID, RMB_CS3_INSTANCE_ID}
};

CONST DEVICE_IDS ROMDATA  RmbGcmMap [] = {
  {RMB_GCM0_FABRIC_ID, RMB_GCM0_INSTANCE_ID},
  {RMB_GCM1_FABRIC_ID, RMB_GCM1_INSTANCE_ID},
  {RMB_GCM2_FABRIC_ID, RMB_GCM2_INSTANCE_ID},
  {RMB_GCM3_FABRIC_ID, RMB_GCM3_INSTANCE_ID}
};

CONST DEVICE_IDS ROMDATA  RmbCcmMap [] = {
  {RMB_CCM0_FABRIC_ID, RMB_CCM0_INSTANCE_ID}
};

CONST DEVICE_IDS ROMDATA  RmbNcmMap [] = {
  {RMB_NCM0_FABRIC_ID, RMB_NCM0_INSTANCE_ID},
  {RMB_NCM1_FABRIC_ID, RMB_NCM1_INSTANCE_ID}
};

CONST DEVICE_IDS ROMDATA  RmbNcsMap [] = {
  {RMB_NCS0_FABRIC_ID, RMB_NCS0_INSTANCE_ID},
  {RMB_NCS1_FABRIC_ID, RMB_NCS1_INSTANCE_ID}
};

CONST DEVICE_IDS ROMDATA  RmbIomsMap [] = {
  {RMB_IOMS0_FABRIC_ID, RMB_IOMS0_INSTANCE_ID}
};

CONST DEVICE_IDS ROMDATA  RmbPieMap [] = {
  {RMB_PIE_FABRIC_ID, RMB_PIE_INSTANCE_ID}
};

CONST DEVICE_IDS ROMDATA  RmbTcdxMap [] = {
  {0xFFFFFFFF, RMB_TCDX0_INSTANCE_ID},
  {0xFFFFFFFF, RMB_TCDX1_INSTANCE_ID},
  {0xFFFFFFFF, RMB_TCDX2_INSTANCE_ID},
  {0xFFFFFFFF, RMB_TCDX3_INSTANCE_ID},
  {0xFFFFFFFF, RMB_TCDX4_INSTANCE_ID}
};

CONST AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP ROMDATA RmbDeviceMap[] = {
  {Cs,     (UINTN) (sizeof (RmbCsMap) / sizeof (RmbCsMap[0])),         &RmbCsMap[0]},
  {Gcm,    (UINTN) (sizeof (RmbGcmMap) / sizeof (RmbGcmMap[0])),       &RmbGcmMap[0]},
  {Ccm,    (UINTN) (sizeof (RmbCcmMap) / sizeof (RmbCcmMap[0])),       &RmbCcmMap[0]},
  {Ncm,    (UINTN) (sizeof (RmbNcmMap) / sizeof (RmbNcmMap[0])),       &RmbNcmMap[0]},
  {Ncs,    (UINTN) (sizeof (RmbNcsMap) / sizeof (RmbNcsMap[0])),       &RmbNcsMap[0]},
  {Ioms,   (UINTN) (sizeof (RmbIomsMap) / sizeof (RmbIomsMap[0])),     &RmbIomsMap[0]},
  {Pie,    (UINTN) (sizeof (RmbPieMap) / sizeof (RmbPieMap[0])),       &RmbPieMap[0]},
  {Tcdx,   (UINTN) (sizeof (RmbTcdxMap) / sizeof (RmbTcdxMap[0])),     &RmbTcdxMap[0]},
  {FabricDeviceTypeMax, 0, NULL}
};


UINTN
FabricTopologyGetNumberOfProcessorsPresent (
  VOID
  )
{
  return 1;
}

UINTN
FabricTopologyGetNumberOfSystemDies (
  VOID
  )
{
  return 1;
}

UINTN
FabricTopologyGetNumberOfSystemRootBridges (
  VOID
  )
{
  return 1;
}

UINTN
FabricTopologyGetNumberOfDiesOnSocket (
  IN       UINTN Socket
  )
{
  return 1;
}

UINTN
FabricTopologyGetNumberOfRootBridgesOnSocket (
  IN       UINTN Socket
  )
{
  return 1;
}

UINTN
FabricTopologyGetNumberOfSocketNeighbors (
  IN       UINTN Socket
  )
{
  return 0;
}

AMD_FABRIC_TOPOLOGY_PROCESSOR_NEIGHBOR_INFO *
FabricTopologyGetNeighborInfoForSocket (
  IN       UINTN Socket
  )
{
  return NULL;
}

UINTN
FabricTopologyGetNumberOfRootBridgesOnDie (
  IN       UINTN Socket,
  IN       UINTN Die
  )
{
  return 1;
}

UINTN
FabricTopologyGetDieSystemOffset (
  IN       UINTN Socket,
  IN       UINTN Die
  )
{
  return 0;
}

CONST
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
FabricTopologyGetDeviceMapOnDie (
  IN       UINTN Socket,
  IN       UINTN Die
  )
{
  return &RmbDeviceMap[0];
}

UINTN
FabricTopologyGetHostBridgeSystemFabricID (
  IN       UINTN Socket,
  IN       UINTN Die,
  IN       UINTN Index
  )
{
  ASSERT (Index == 0);
  return RMB_IOMS0_FABRIC_ID;
}

UINTN
FabricTopologyGetHostBridgeBusBase (
  IN       UINTN Socket,
  IN       UINTN Die,
  IN       UINTN Index
  )
{
  return 0;
}

UINTN
FabricTopologyGetHostBridgeBusLimit (
  IN       UINTN Socket,
  IN       UINTN Die,
  IN       UINTN Index
  )
{
  UINTN                                      BusLimit;
  UINTN                                      CfgAddrMapIndex;
  UINTN                                      TargetFabricId;
  CFG_ADDRESS_MAP_REGISTER                   BusMap;

  TargetFabricId = FabricTopologyGetHostBridgeSystemFabricID (Socket, Die, Index);

  BusLimit = 0xFF;
  for (CfgAddrMapIndex = 0; CfgAddrMapIndex < RMB_NUMBER_OF_BUS_REGIONS; CfgAddrMapIndex++) {
    BusMap.Value = FabricRegisterAccRead (0, 0, CFGADDRESSMAP_FUNC, (CFGADDRESSMAP0_REG + (CfgAddrMapIndex * RMB_BUS_REGION_REGISTER_OFFSET)), FABRIC_REG_ACC_BC);
    if ((BusMap.Field.RE == 1) && (BusMap.Field.WE == 1) && (BusMap.Field.DstFabricID == (UINT32) TargetFabricId)) {
      BusLimit = (UINTN) BusMap.Field.BusNumLimit;
      break;
    }
  }

  return BusLimit;
}

BOOLEAN
FabricTopologyGetSystemComponentRootBridgeLocation (
  IN       COMPONENT_TYPE Component,
  IN OUT   ROOT_BRIDGE_LOCATION *Location
  )
{
  Location->Socket = 0;
  Location->Die = 0;
  Location->Index = 0;

  return TRUE;
}

BOOLEAN
FabricTopologyHasFch (
  IN       UINTN Socket,
  IN       UINTN Die,
  IN       UINTN Index
  )
{
  return TRUE;
}

BOOLEAN
FabricTopologyHasSmu (
  IN       UINTN Socket,
  IN       UINTN Die,
  IN       UINTN Index
  )
{
  return TRUE;
}

UINTN
FabricTopologyGetPhysRootBridgeNumber (
  IN       UINTN Socket,
  IN       UINTN Die,
  IN       UINTN Index
  )
{
  return 0;
}

CONST
AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP *
FindDeviceTypeEntryInMap (
  IN       UINTN               Socket,
  IN       UINTN               Die,
  IN       FABRIC_DEVICE_TYPE  Type
  )
{
  UINTN                                      i;
  CONST AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *DeviceMap;

  DeviceMap = FabricTopologyGetDeviceMapOnDie (Socket, Die);
  for (i = 0; DeviceMap[i].Type < FabricDeviceTypeMax; i++) {
    if (DeviceMap[i].Type == Type) {
      break;
    }
  }

  if (DeviceMap[i].Type == Type) {
    return &DeviceMap[i];
  } else {
    return NULL;
  }
}


CONST
COMPONENT_LOCATION *
FindComponentLocationMap (
  IN       UINTN *Count,
  IN       UINTN *PhysIoms0FabricId
  )
{
  if (Count != NULL) {
    *Count = sizeof (RmbComponentLocation) / sizeof (RmbComponentLocation[0]);
  }
  if (PhysIoms0FabricId != NULL) {
    *PhysIoms0FabricId = RmbPhysIoms0FabricId;
  }
  return &RmbComponentLocation[0];
}

