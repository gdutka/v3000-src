/*
*****************************************************************************
*
 * Copyright (C) 2018-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/RasRmbBaseLib.h>
#include <Library/PciLib.h>
#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_RASGNBASELIB_RASGNBASELIB_FILECODE

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

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
extern  ADDR_DATA               *gAddrData;

INT32 savedComponentIdMask = -1;
INT32 savedNodeIdShift = -1;
INT32 savedNodeIdMask = -1;
INT32 savedSocketIdShift = -1;
INT32 savedSocketIdMask = -1;
INT32 savedDieIdShift = -1;
INT32 savedDieIdMask = -1;

UINT8 DRAMTYPE=7;
UINT8 DRAMTYPE_1=7;
UINTN LOC =0 ;
UINTN LOC_1 =0 ;
UINTN LOC_2 =0 ;
UINT8 TOTAL_NUM_UMCCH_PER_UMC_ADDR_TRANS =1;
UINT8 three_way_cs;
UINT8 two_p_one;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
// Function prototypes of private internal functions
UINT32 getBits (UINT32 lowBit, UINT32 highBit, UINT32 data);
UINT64 getBits64 (UINT32 lowBit, UINT32 highBit, UINT64 data);
UINT64 getBit64 (UINT32 bit, UINT64 data);
UINT32 getBit (UINT32 bit, UINT32 data);
UINT64 removeBits64 (UINT32 lowBit, UINT32 highBit, UINT64 data);
UINT64 expandBits64 (UINT32 bitNumber, UINT32 numBits, UINT64 data);
UINT32 getDfReg (UINT32 dfType, UINT32 instance, UINT32 nodeId, UINT32 regAddr);
UINT32 getDfRegSystemFabricIdMask0 (UINT32 dfType);
UINT32 getDfRegSystemFabricIdMask1 (UINT32 dfType);
UINT32 getDfRegSystemFabricIdMask2 (UINT32 dfType);
UINT32 getComponentIdMask (UINT32 dfType);
UINT32 getNodeIdShift (UINT32 dfType);
UINT32 getNodeIdMask (UINT32 dfType);
UINT32 getSocketIdShift (UINT32 dfType);
UINT32 getSocketIdMask (UINT32 dfType);
UINT32 getDieIdShift (UINT32 dfType);
UINT32 getDieIdMask (UINT32 dfType);
UINT32 getNumAddressMaps (UINT32 dfType);
UINT32 getDfRegFabricBlkInstanceCnt (UINT32 nodeId);
UINT32 getDfRegFabricBlkInstInfo0 (UINT32 dfType, UINT32 instanceId, UINT32 nodeId);
UINT32 getDfRegFabricBlkInstInfo3 (UINT32 dfType, UINT32 instanceId, UINT32 nodeId);
UINT32 getIntlvHashCtlBits (UINT32 dfType, UINT32 *dramAddressMapRegs);
UINT32 determineDfType (VOID);
UINT32 getDfRegDramOffset (UINT32 dfType, UINT32 instanceId, UINT32 nodeId, UINT32 regNum);
UINT64 extractDramOffset (UINT32 dfType, UINT32 dramOffsetReg);
VOID getDramAddressMap (UINT32 dfType, INT32 instanceId, UINT32 nodeId, UINT32 mapNumber, UINT32 *dramAddressMapRegs);
UINT32 getDfRegDramHoleCtrl (UINT32 dfType);
UINT64 getDramHoleBase  (UINT32 dfType);
UINT32 decodeDramIntLvAddrBit (UINT32 dfType, UINT32 *dramAddressMapRegs);
INT32 getNumChannelFromDramIntLvMode (INT32 intLvMode);
UINT32 decodeDramIntLvNumChan (UINT32 dfType, UINT32 *dramAddressMapRegs);
UINT32 extractDramIntLvNumDies (UINT32 dfType, UINT32 *dramAddressMapRegs);
UINT32 decodeDramIntLvNumDies (UINT32 dfType, UINT32 *dramAddressMapRegs);
UINT32 extractDramIntLvNumSkts (UINT32 dfType, UINT32 *dramAddressMapRegs);
UINT32 extractDstFabricId (UINT32 dfType, UINT32 *dramAddressMapRegs);
UINT64 extractDramBaseAddr (UINT32 dfType, UINT32 *dramAddressMapRegs);
UINT64 extractDramLimitAddr (UINT32 dfType, UINT32 *dramAddressMapRegs);
UINT32 extractDramAddrRangeValid (UINT32 dfType, UINT32 *dramAddressMapRegs);
UINT32 extractLgcyMmioHoleEn (UINT32 dfType, UINT32 *dramAddressMapRegs);
UINT32 extractLog2Addr64KSpace (UINT32 dfType, UINT32 *dramAddressMapRegs);
UINT32 convertPhysicalCsFabricIdToLogicalCsFabricId (UINT32 dfType, UINT32 csFabricId, UINT32 *dramAddressMapRegs);
UINT32 convertLogicalFabricIdToPhysicalFabricId (UINT32 dfType, UINT32 logicalDstFabricId, UINT32 *dramAddressMapRegs);
UINT32 getCsLogicalComponentIdFromAddr (UINT32 dfType, UINT32 *dramAddressMapRegs, UINT64 addr);
UINT64 normalizeMod3 (UINT32 dfType, UINT32 *dramAddressMapRegs, UINT64 addr);
UINT64 normalizeDf4NP2 (UINT32 dfType, UINT32 *dramAddressMapRegs, UINT64 addr);
UINT64 normalizeAddr (UINT32 dfType, UINT32 *dramAddressMapRegs, UINT64 addr);
UINT64 deNormalizeAddrMod3 (UINT32 dfType, UINT32 logicalCsFabricId, UINT32 *dramAddressMapRegs, UINT64 normAddr);
UINT64 deNormalizeAddrDf4Np2 (UINT32 dfType, UINT32 logicalCsFabricId, UINT32 *dramAddressMapRegs, UINT64 normAddr);
UINT64 deNormalizeAddr (UINT32 dfType, UINT32 csFabricId, UINT32 *dramAddressMapRegs, UINT64 normAddr);
UINT64 deNormHashAddr (UINT32 dfType, UINT32 *dramAddressMapRegs, UINT64 deNormAddr);
BOOLEAN isSystemDiscreteGpu (UINT32 dfType, UINT32 nodeId);
UINT32 findModeratorInstanceId (UINT32 dfType, UINT32 nodeId);
UINT32 findMapRegBySysAddr (UINT32 dfType, UINT32 nodeId, UINT64 sysAddr);
UINT32 findMapRegByDstFabricId (UINT32 dfType, UINT32 nodeId, UINT32 dstFabricId);

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
UINT8 get_dramtype()
{
  return DRAMTYPE_1;
}
/**
 *
 *
 *
 */
UINT64
pow_ras (
  UINTN   input,
  UINTN   exp
  )
{
  UINT64  value;
  UINTN   i;

  value = 1;

  if (0 == exp) {
  } else {
    for (i = 0; i < exp; i++) {
      value *= input;
    }
  }
  return value;
}

/*----------------------------------------------------------------------------------------*/
/**
 *
 *
 */
UINT32
log2 (
  UINT32 input
  )
{
  switch (input)
  {
    case 1:
      return 0;
    case 2:
      return 1;
    case 4:
      return 2;
    case 8:
      return 3;
    case 16:
      return 4;
    case 32:
      return 5;
    case 64:
      return 6;
    case 128:
      return 7;
    case 256:
      return 8;
    case 512:
      return 9;
    case 1024:
      return 10;
    default:
      ASSERT (FALSE);
      return 0;
  }
}

/*------------------------------------------------------------------
 Function: getBits
 Purpose: A helper function to get a bit range from a uint32
 Inputs:
   low bit number
   high bit number
   the data
 Outputs:
   the requested bits, right justified
 *------------------------------------------------------------------*/
UINT32
getBits (
  UINT32 lowBit,
  UINT32 highBit,
  UINT32 data
  )
{
  UINT32 mask;
  ASSERT (highBit<32);
  ASSERT (lowBit<32);
  ASSERT (lowBit<=highBit);
  mask = (1 << (highBit - lowBit + 1)) - 1;
  return ((data >> lowBit) & mask);
}

/*------------------------------------------------------------------
 Function: getBits64
 Purpose: A helper function to get a bit range from a uint64
 Inputs:
   low bit number
   high bit number
   the data
 Outputs:
   the requested bits, right justified
 *------------------------------------------------------------------*/
UINT64
getBits64 (
  UINT32 lowBit,
  UINT32 highBit,
  UINT64 data
  )
{
  UINT64    mask;
  ASSERT (highBit<64);
  ASSERT (lowBit<64);
  ASSERT (lowBit<=highBit);
  mask = (((UINT64) 1) << (highBit - lowBit + ((UINT64) 1))) - ((UINT64) 1);
  return ((data >> lowBit) & mask);
}

/*------------------------------------------------------------------
 Function: getBit64
 Purpose: A helper function to get a specific bit from a uint64
 Inputs:
   bit number
   the data
 Outputs:
   the requested bits, right justified
 *------------------------------------------------------------------*/
UINT64
getBit64 (
  UINT32 bit,
  UINT64 data
  )
{
  ASSERT (bit<64);
  return ((data >> bit) & 0x1);
}

/*------------------------------------------------------------------
 Function: getBit
 Purpose: A helper function to get a specific bit from a uint32
 Inputs:
   bit number
   the data
 Outputs:
   the requested bit, right justified
 *------------------------------------------------------------------*/
UINT32
getBit (
  UINT32 bit,
  UINT32 data
  )
{
  ASSERT (bit<32);
  return ((data >> bit) & 0x1);
}

/*------------------------------------------------------------------
 Function: removeBits64
 Purpose: A helper function to remove bits from a 64-bit value
 Inputs:
   low bit number
   high bit number
   the data
 Outputs:
   data with bits [high:low] removed (shifted out)
   e.g. AAAAAAXXXBBBB with bits 6:4 removed is AAAAAABBBB
 *------------------------------------------------------------------*/
UINT64 removeBits64 (
  UINT32 lowBit,
  UINT32 highBit,
  UINT64 data
  )
{
  UINT64 temp1, temp2;
  ASSERT (highBit<64);
  ASSERT (lowBit<64);
  ASSERT (lowBit<=highBit);
  if (lowBit == 0)
  {
    data = data >> (highBit+1);
    return (data);
  }
  temp1 = getBits64(0, lowBit-1, data);
  temp2 = getBits64(highBit+1, 63, data);
  temp2 = temp2 << lowBit;
  data = temp1 | temp2;
  return (data);
}

/*------------------------------------------------------------------
 Function: expandBits64
 Purpose: A helper function to expand bits within 64-bit value
 Inputs:
   bit number to start at
   number of bits to push out
   the data
 Outputs:
   data with bits [63:n] << number of Bits
   ORd with bits [n:0]
   e.g. AAAAAABBBB with 3 bits added at bit location 4 is AAAAAA000BBBB
 *------------------------------------------------------------------*/
UINT64 expandBits64 (
  UINT32 bitNumber,
  UINT32 numBits,
  UINT64 data
  )
{
  UINT64 temp1, temp2;
  ASSERT (bitNumber<64);
  ASSERT (numBits<64);
  if (bitNumber == 0)
  {
    data = data << numBits;
    return (data);
  }
  temp1 = getBits64(0, bitNumber-1, data);
  temp2 = getBits64(bitNumber, 63, data);
  temp2 = temp2 << (bitNumber + numBits);
  data = temp1 | temp2;
  return (data);
}

/*------------------------------------------------------------------
 Function: getDfReg
 Purpose: Get a DF register
 Inputs:
   DF type
   An instanceID (-1 for a broadcast read),
   A nodeID (for the PCIe bus/device),
   A register address:
     function in bits 14:12
     offset in bits 11:0
     NOTE: This is DF4 compatible, DF3 must shift the function over 2 bits
 Outputs: The requested register
 *------------------------------------------------------------------*/
UINT32
getDfReg (
  UINT32 dfType,
  UINT32 instanceId,
  UINT32 nodeId,
  UINT32 regAddr
  )
{
  UINT32                               RegisterValue;
  PCI_ADDR                             PciAddr;
  FABRIC_IND_CFG_ACCESS_ADDR_REGISTER  FICAA3;
  UINT32                               Ficaa3Save;

  PciAddr.AddressValue = 0;
  PciAddr.Address.Device = nodeId + 0x18;   // 0x18 = PCIE device number of socket 0 die 0

  FICAA3.Value = 0;   // Variable initiate
  if ((instanceId & 0xFF) != FABRIC_REG_ACC_BC) {
    FICAA3.Field.CfgRegInstAccEn = 1;
  }

  PciAddr.Address.Function = FICAA3_FUNC;
  PciAddr.Address.Register = FICAA3_REG;

  //Save FICAA3 register value
  Ficaa3Save = PciRead32 (PciAddr.AddressValue);

  FICAA3.Field.IndCfgAccRegNum = ((UINT32) (regAddr & 0xfff)) >> 2;
  FICAA3.Field.IndCfgAccFuncNum = ((UINT32) (regAddr >> 12 )) & 7;
  FICAA3.Field.CfgRegInstID = (UINT32) instanceId;
  PciWrite32 (PciAddr.AddressValue, FICAA3.Value);

  PciAddr.Address.Function = FICAD3_LO_FUNC;
  PciAddr.Address.Register = FICAD3_LO_REG;
  RegisterValue = PciRead32 (PciAddr.AddressValue);

  //Restore FICAA3 register value
  PciAddr.Address.Function = FICAA3_FUNC;
  PciAddr.Address.Register = FICAA3_REG;
  PciWrite32 (PciAddr.AddressValue, Ficaa3Save);

  return RegisterValue;
}

/*------------------------------------------------------------------
 Function: getDfRegSystemFabricIdMask0
 Purpose: Get the DF::SystemFabricIdMask register
 Inputs: The DF type
 Outputs: The requested register from node 0 (all nodes will be the same)
 *------------------------------------------------------------------*/
UINT32
getDfRegSystemFabricIdMask0 (
  UINT32 dfType
  )
{
  UINT32 addr = 0;

  switch (dfType)
  {
    case DF_TYPE_DF2:
    case DF_TYPE_DF3:       addr = DF__SYSFABIDMASK0_REGADDR_DF2_AND_DF3;
                            break;
    case DF_TYPE_DF3POINT5: addr = DF__SYSFABIDMASK0_REGADDR_DF3POINT5;
                            break;
    case DF_TYPE_DF4:       addr = DF__SYSFABIDMASK0_REGADDR_DF4;
                            break;
    default:                ASSERT (FALSE);
  }
  return (getDfReg(dfType, BROADCAST_ACCESS, 0, addr));
}

/*------------------------------------------------------------------
 Function: getDfRegSystemFabricIdMask1
 Purpose: Get the DF::SystemFabricIdMask1register
 Inputs: The DF type
 Outputs: The requested register from node 0 (all nodes will be the same)
 *------------------------------------------------------------------*/
UINT32
getDfRegSystemFabricIdMask1 (
  UINT32 dfType
  )
{
  UINT32 addr = 0;

  switch (dfType)
  {
    case DF_TYPE_DF2:       ASSERT (FALSE);
                            break;
    case DF_TYPE_DF3:       addr = DF__SYSFABIDMASK1_REGADDR_DF3;
                            break;
    case DF_TYPE_DF3POINT5: addr = DF__SYSFABIDMASK1_REGADDR_DF3POINT5;
                            break;
    case DF_TYPE_DF4:       addr = DF__SYSFABIDMASK1_REGADDR_DF4;
                            break;
    default:                ASSERT (FALSE);
                            break;
  }
  return (getDfReg(dfType, BROADCAST_ACCESS, 0, addr));
}

/*------------------------------------------------------------------
 Function: getDfRegSystemFabricIdMask2
 Purpose: Get the DF::SystemFabricIdMask2register
 Inputs: The DF type
 Outputs: The requested register from node 0 (all nodes will be the same)
 *------------------------------------------------------------------*/
UINT32
getDfRegSystemFabricIdMask2 (
  UINT32 dfType
  )
{
  UINT32 addr = 0;

  switch (dfType)
  {
    case DF_TYPE_DF2:
    case DF_TYPE_DF3:       ASSERT (FALSE);
                            break;
    case DF_TYPE_DF3POINT5: addr = DF__SYSFABIDMASK2_REGADDR_DF3POINT5;
                            break;
    case DF_TYPE_DF4:       addr = DF__SYSFABIDMASK2_REGADDR_DF4;
                            break;
    default:                ASSERT (FALSE);
  }
  return (getDfReg(dfType, BROADCAST_ACCESS, 0, addr));
}

/*------------------------------------------------------------------
 Function: getComponentIdMask
 Purpose: Get the mask of the component ID **within a FabricID**
          Example, if bits 5:0 of a FabricID is componentID, this will return 0x3F
 Inputs: DF type
 Outputs: A mask to apply to the FabricID to get just the component ID bits
 *------------------------------------------------------------------*/
UINT32
getComponentIdMask (
  UINT32 dfType
  )
{
  if (savedComponentIdMask < 0)
  {
    switch (dfType)
    {
      case DF_TYPE_DF2:
        savedComponentIdMask = ((~(getSocketIdMask(dfType) | getDieIdMask(dfType))) & 0xFF);
        break;
      case DF_TYPE_DF3:
      case DF_TYPE_DF3POINT5:
      case DF_TYPE_DF4:
        savedComponentIdMask = getBits(DF__COMPONENT_ID_MASK_BITPOS_LO_DF3, DF__COMPONENT_ID_MASK_BITPOS_HI_DF3,
                                       getDfRegSystemFabricIdMask0(dfType));
        break;
      default:
        ASSERT (FALSE);
    }
  }
  return (savedComponentIdMask);
}

/*------------------------------------------------------------------
 Function: getNodeIdShift
 Purpose: Get the shift (LSB) of the node ID **within a FabricID**
          Example, if bits 7:6 of a FabricID is nodeID, this will return 0xC0
 Inputs: DF type
 Outputs: A mask to apply to the FabricID to get just the node ID bits
   Note: Within DF registers, these fields are based off of the LSB
         of the NodeID (varies per variant). This function will normalize
         these to FabricID[0]
 *------------------------------------------------------------------*/
UINT32
getNodeIdShift (
  UINT32 dfType
  )
{
  if (savedNodeIdShift < 0)
  {
    switch (dfType)
    {
      case DF_TYPE_DF2:
        // not directly available from registers, but it is the same as the die ID shift
        savedNodeIdShift = getDieIdShift(dfType);
        break;
      case DF_TYPE_DF3:
      case DF_TYPE_DF3POINT5:
      case DF_TYPE_DF4:
        savedNodeIdShift = getBits(DF__NODE_ID_SHIFT_BITPOS_LO_DF3, DF__NODE_ID_SHIFT_BITPOS_HI_DF3,
                                   getDfRegSystemFabricIdMask1(dfType));
        break;
      default:
        ASSERT (FALSE);
    }
  }
  return (savedNodeIdShift);
}

/*------------------------------------------------------------------
 Function: getNodeIdMask
 Purpose: Get the mask of the node ID **within a FabricID**
          Example, if bits 7:6 of a FabricID is nodeID, this will return 0xC0
 Inputs: DF type
 Outputs: A mask to apply to the FabricID to get just the node ID bits
   Note: Within DF registers, these fields are based off of the LSB
         of the NodeID (varies per variant). This function will normalize
         these to FabricID[0]
 *------------------------------------------------------------------*/
UINT32
getNodeIdMask (
  UINT32 dfType
  )
{
  if (savedNodeIdMask < 0)
  {
    switch (dfType)
    {
      case DF_TYPE_DF2:
        // not directly available from registers, but it is the same as the die ID shift
        savedNodeIdMask = getSocketIdMask(dfType) | getDieIdMask(dfType);
        break;
      case DF_TYPE_DF3:
      case DF_TYPE_DF3POINT5:
      case DF_TYPE_DF4:
        savedNodeIdMask = getBits(DF__NODE_ID_MASK_BITPOS_LO_DF3, DF__NODE_ID_MASK_BITPOS_HI_DF3,
                                  getDfRegSystemFabricIdMask0(dfType));
        break;
      default:
        ASSERT (FALSE);
    }
  }
  return (savedNodeIdMask);
}

/*------------------------------------------------------------------
 Function: getSocketIdShift
 Purpose: Get the shift (LSB) of the socket ID **within a FabricID**
          Example, if bits 7:6 of a FabricID is socketID, this will return 6
 Inputs: DF type
 Outputs: The LSB of socket ID
   Note: Within DF registers, these fields are based off of the LSB
         of the NodeID (varies per variant). This function will normalize
         these to FabricID[0]
 *------------------------------------------------------------------*/
UINT32
getSocketIdShift (
  UINT32 dfType
  )
{
  if (savedSocketIdShift < 0)
  {
    switch (dfType)
    {
      case DF_TYPE_DF2:
        savedSocketIdShift = getBits(DF__SOCKET_ID_SHIFT_BITPOS_LO_DF2, DF__SOCKET_ID_SHIFT_BITPOS_HI_DF2,
                                     getDfRegSystemFabricIdMask0(dfType));
        break;
      case DF_TYPE_DF3:
      case DF_TYPE_DF3POINT5:
      case DF_TYPE_DF4:
        savedSocketIdShift = getBits(DF__SOCKET_ID_SHIFT_BITPOS_LO_DF3, DF__SOCKET_ID_SHIFT_BITPOS_HI_DF3,
                                     getDfRegSystemFabricIdMask1(dfType));
        savedSocketIdShift += getNodeIdShift(dfType);
        break;
      default:
        ASSERT (FALSE);
    }
  }
  return (savedSocketIdShift);
}

/*------------------------------------------------------------------
 Function: getSocketIdMask
 Purpose: Get the mask of the socket ID **within a FabricID**
          Example, if bits 7:6 of a FabricID is socketID, this will return 0xC0
 Inputs: DF type
 Outputs: A mask to apply to the FabricID to get just the socket ID bits
   Note: Within DF registers, these fields are based off of the LSB
         of the NodeID (varies per variant). This function will normalize
         these to FabricID[0]
 *------------------------------------------------------------------*/
UINT32
getSocketIdMask (
  UINT32 dfType
  )
{
  if (savedSocketIdMask < 0)
  {
    switch (dfType)
    {
      case DF_TYPE_DF2:
        savedSocketIdMask = getBits(DF__SOCKET_ID_MASK_BITPOS_LO_DF2, DF__SOCKET_ID_MASK_BITPOS_HI_DF2,
                                    getDfRegSystemFabricIdMask0(dfType));
        break;
      case DF_TYPE_DF3:
        savedSocketIdMask = getBits(DF__SOCKET_ID_MASK_BITPOS_LO_DF3, DF__SOCKET_ID_MASK_BITPOS_HI_DF3,
                                    getDfRegSystemFabricIdMask1(dfType));
        savedSocketIdMask = savedSocketIdMask << getNodeIdShift(dfType);
        break;
      case DF_TYPE_DF3POINT5:
      case DF_TYPE_DF4:
        savedSocketIdMask = getBits(DF__SOCKET_ID_MASK_BITPOS_LO_DF4, DF__SOCKET_ID_MASK_BITPOS_HI_DF4,
                                    getDfRegSystemFabricIdMask2(dfType));
        savedSocketIdMask = savedSocketIdMask << getNodeIdShift(dfType);
        break;
      default:
        ASSERT (FALSE);
    }
  }
  return (savedSocketIdMask);
}

/*------------------------------------------------------------------
 Function: getDieIdShift
 Purpose: Get the shift (LSB) of the die ID **within a FabricID**
          Example, if bits 7:6 of a FabricID is dieID, this will return 0xC0
 Inputs: DF type
 Outputs: A mask to apply to the FabricID to get just the die ID bits
   Note: Within DF registers, these fields are based off of the LSB
         of the NodeID (varies per variant). This function will normalize
         these to FabricID[0]
 *------------------------------------------------------------------*/
UINT32
getDieIdShift (
  UINT32 dfType
  )
{
  if (savedDieIdShift < 0)
  {
    switch (dfType)
      {
      case DF_TYPE_DF2:
        savedDieIdShift = getBits(DF__DIE_ID_SHIFT_BITPOS_LO_DF2, DF__DIE_ID_SHIFT_BITPOS_HI_DF2,
                                  getDfRegSystemFabricIdMask0(dfType));
        break;
      case DF_TYPE_DF3:
      case DF_TYPE_DF3POINT5:
      case DF_TYPE_DF4:
        // not directly available from registers, but it is the same as the node ID shift
        savedDieIdShift = getNodeIdShift(dfType);
        break;
      default:
        ASSERT (FALSE);
    }
  }
  return (savedDieIdShift);
}

/*------------------------------------------------------------------
 Function: getDieIdMask
 Purpose: Get the mask of the die ID **within a FabricID**
          Example, if bits 7:6 of a FabricID is dieID, this will return 0xC0
 Inputs: DF type
 Outputs: A mask to apply to the FabricID to get just the die ID bits
   Note: Within DF registers, these fields are based off of the LSB
         of the NodeID (varies per variant). This function will normalize
         these to FabricID[0]
 *------------------------------------------------------------------*/
UINT32
getDieIdMask (
  UINT32 dfType
  )
{
  if (savedDieIdMask < 0)
  {
    switch (dfType)
    {
      case DF_TYPE_DF2:
        savedDieIdMask = getBits(DF__DIE_ID_MASK_BITPOS_LO_DF2, DF__DIE_ID_MASK_BITPOS_HI_DF2,
                                 getDfRegSystemFabricIdMask0(dfType));
        break;
      case DF_TYPE_DF3:
        savedDieIdMask = getBits(DF__DIE_ID_MASK_BITPOS_LO_DF3, DF__DIE_ID_MASK_BITPOS_HI_DF3,
                                 getDfRegSystemFabricIdMask1(dfType));
        savedDieIdMask = savedDieIdMask << getNodeIdShift(dfType);
        break;
      case DF_TYPE_DF3POINT5:
      case DF_TYPE_DF4:
        savedDieIdMask = getBits(DF__DIE_ID_MASK_BITPOS_LO_DF4, DF__DIE_ID_MASK_BITPOS_HI_DF4,
                                 getDfRegSystemFabricIdMask2(dfType));
        savedDieIdMask = savedDieIdMask << getNodeIdShift(dfType);
        break;
      default:
        ASSERT (FALSE);
    }
  }
  return (savedDieIdMask);
}

/*------------------------------------------------------------------
 Function: getNumAddressMaps
 Purpose: Returns the number of address maps in the hardware address space
 Inputs: DF type
 Outputs: The number of available address maps
          These do not have to be "instantiated" on a particular SOC
          As they can be reserved and will always return zero.
 *------------------------------------------------------------------*/
UINT32
getNumAddressMaps (
  UINT32 dfType
  )
{
  switch (dfType)
  {
    case DF_TYPE_DF2:
    case DF_TYPE_DF3:
      return (DF__NUM_DRAM_MAPS_AVAILABLE_PRE_DF4);
    case DF_TYPE_DF3POINT5:
      // Include an extra one for the "mega" map
      return (DF__NUM_DRAM_MAPS_AVAILABLE_PRE_DF4 + 1);
    case DF_TYPE_DF4:
      return (DF__NUM_DRAM_MAPS_AVAILABLE_DF4);
    default:
      ASSERT (FALSE);
  }
  return (0);
}

/*------------------------------------------------------------------
 Function: getDfRegFabricBlkInstanceCnt
 Purpose: Get the DF::FabricBlockInstanceCount register
 Inputs: An instanceID (CS) and a nodeID (for the PCIe bus/device)
 Outputs: The requested register
 *------------------------------------------------------------------*/
UINT32
getDfRegFabricBlkInstanceCnt (
  UINT32  nodeId
  )
{
  return (getDfReg(DF_TYPE_UNKNOWN, BROADCAST_ACCESS, nodeId, DF__FABBLKINSTCNT_REGADDR));
}

/*------------------------------------------------------------------
 Function: getDfRegFabricBlkInstInfo0
 Purpose: Get the DF::FabricBlockInstanceInformation0 register
 Inputs: DF type, an instanceID (CS) and a nodeID (for the PCIe bus/device)
 Outputs: The requested register
 *------------------------------------------------------------------*/
UINT32
getDfRegFabricBlkInstInfo0 (
  UINT32 dfType,
  UINT32 instanceId,
  UINT32 nodeId
  )
{
  return (getDfReg (dfType, instanceId, nodeId, DF__FABBLKINFO0_REGADDR));
}

/*------------------------------------------------------------------
 Function: getDfRegFabricBlkInstInfo3
 Purpose: Get the DF::FabricBlockInstanceInformation3 register
 Inputs: An instanceID (CS) and a nodeID (for the PCIe bus/device)
 Outputs: The requested register
 *------------------------------------------------------------------*/
UINT32
getDfRegFabricBlkInstInfo3 (
  UINT32 dfType,
  UINT32 instanceId,
  UINT32 nodeId
  )
{
  return (getDfReg (dfType, instanceId, nodeId, DF__FABBLKINFO3_REGADDR));
}

/*------------------------------------------------------------------
 Function: getIntlvHashCtlBits
 Purpose: Get the three bits to control interleave hashing, right justified
 Inputs: DF type and the address map
 Outputs:
   bit0: HASH_INTLV_CTL_64K
   bit1: HASH_INTLV_CTL_2M
   bit2: HASH_INTLV_CTL_1G
 *------------------------------------------------------------------*/
UINT32
getIntlvHashCtlBits (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs
  )
{
  UINT32 dfGlobalCtrlReg;

  // MI200 is an odd exception to this, as it has DramIntlvLocHashCtl0 and DramIntlvLocHashCtl1 registers
  // with individual enable bits for each one. But MI200 still has DfGlobalCtrl enabling them (for the CS),
  // and the only supported case is one where this is the same for all registers. So we do not have to
  // check DramIntlvLocHashCtl0/DramIntlvLocHashCtl1 at all.
  if (dfType < DF_TYPE_DF4)
  {
    dfGlobalCtrlReg = getDfReg(dfType, BROADCAST_ACCESS, 0, DF__DFGLOBALCTRL_REGADDR);
    dfGlobalCtrlReg = (dfGlobalCtrlReg >> DF__HASH_INTLV_CTL_64K_BITPOS_DF3);
  }
  else
  {
    dfGlobalCtrlReg = (dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET] >> DF__HASH_INTLV_CTL_64K_BITPOS_DF4);
  }
  dfGlobalCtrlReg &= 7;
  return (dfGlobalCtrlReg);
}

/*------------------------------------------------------------------
 Function: determineDfType
 Purpose: Determine if the DF type is DF2 or DF3, DF3.5 (Trento/MI200), or DF4
 Inputs: None
 Outputs: The DF type as an encoded type

 Algorithm:
  1) Read F0x40 (FabricBlockInstanceCount). Bits 27:24 specify the MajorRevision,
     but this was added in DF4. Prior to DF4, this field is always zero.
  2) If bits F0x40[27:24] != 0, then MajorRevision indicates DF4 (done)
  3) To determine pre-DF4...
  4) Read F1x150. SystemFabricIdMask0 was moved in Shoreline components (Trento and MI200)
     F1x150[15:0] is the ComponentIdMask and at least some LSB bits must be non-zero.
     Prior to Trento/MI200, F1x150 was reserved.
  5) If F1x150[7:0] != 0, then this is "DF3.5" (done)
  6) To determine DF3 vs DF2...
  7) Read F1x208. In DF3, this is the ComponentIdMask and that must be non-zero.
     In DF2, F1x208[7:0] was reserved.
  8) If F1x208[7:0] is non-zero, this is DF3 done)
  9) Else it is DF2 (done).
  We can do all this on node 0, since there will always be a node 0 and all
  nodes will be the same.

  Note that all of the accesses here must be done as "broadcast accesses" because
  even the format of the indirect access registers is variant specific.
 *------------------------------------------------------------------*/
UINT32
determineDfType (
  VOID
  )
{
  UINT32 rev;

  rev = getBits(DF__MAJOR_REVISION_BITPOS_LO, DF__MAJOR_REVISION_BITPOS_HI, getDfRegFabricBlkInstanceCnt(0));
  if (rev != 0)
  {
    ASSERT (rev == 4);
    return (DF_TYPE_DF4);
  }

  if ((getDfReg(DF_TYPE_UNKNOWN, BROADCAST_ACCESS, 0, DF__SYSFABIDMASK0_REGADDR_DF3POINT5) & 0xFF) != 0)
  {
    return (DF_TYPE_DF3POINT5);
  }

  if ((getDfReg(DF_TYPE_UNKNOWN, BROADCAST_ACCESS, 0, DF__SYSFABIDMASK0_REGADDR_DF2_AND_DF3) & 0xFF) != 0)
  {
    return (DF_TYPE_DF3);
  }

  return (DF_TYPE_DF2);
}

/*------------------------------------------------------------------
 Function: getDfRegDramOffset
 Purpose: Get the DF::DramOffset[n] register
 Inputs: The DF type, an instanceID (CS), a nodeID (for the PCIe bus/device)
   and a register number
 Outputs: The requested register
 *------------------------------------------------------------------*/
UINT32
getDfRegDramOffset (
  UINT32 dfType,
  UINT32 instanceId,
  UINT32 nodeId,
  UINT32 regNum
  )
{
  UINT32 addr = 0;

  // There is no actual DramOffset_n0. This is treated as if it is zero for code simplicity
  // (which is what the HW does as well).
  if (regNum == 0)
  {
    return (0);
  }
  if (dfType >= DF_TYPE_DF4)
  {
    ASSERT (regNum<=3); // DF4 supports only three DRAM offset registers (n0-n3, where n0 is always zero)
  }
  else
  {
    ASSERT (regNum<=1); // DF2+Df3 supports only one DRAM offset register (n0-n1, where n0 is always zero)
  }

  if (dfType != DF_TYPE_DF4)
  {
    addr = DF__DRAMOFFSET0_REGADDR_PRE_DF4;
  }
  else
  {
    addr = DF__DRAMOFFSET0_REGADDR_DF4;
  }
  addr += (4*regNum);
  return (getDfReg(dfType, instanceId, nodeId, addr));
}

/*------------------------------------------------------------------
 Function: extractDramOffset
 Purpose: Decodes the DRAM offset register into a normalized address
 Inputs: The DF type, and the register
   and a register number
 Outputs: The requested register
 *------------------------------------------------------------------*/
UINT64
extractDramOffset (
  UINT32 dfType,
  UINT32 dramOffsetReg
  )
{
  UINT64 hiAddrOffset = 0;

  if (dfType == DF_TYPE_DF2)
  {
    hiAddrOffset = getBits(DF__HI_ADDR_OFFSET_BITPOS_LO_DF2, DF__HI_ADDR_OFFSET_BITPOS_HI_DF2, dramOffsetReg);
  }
  else if ((dfType == DF_TYPE_DF3) || (dfType == DF_TYPE_DF3POINT5))
  {
    hiAddrOffset = getBits(DF__HI_ADDR_OFFSET_BITPOS_LO_DF3, DF__HI_ADDR_OFFSET_BITPOS_HI_DF3, dramOffsetReg);
  }
  else if (dfType == DF_TYPE_DF4)
  {
    hiAddrOffset = getBits(DF__HI_ADDR_OFFSET_BITPOS_LO_DF4, DF__HI_ADDR_OFFSET_BITPOS_HI_DF4, dramOffsetReg);
  }
  else
  {
    ASSERT (FALSE);
  }
  hiAddrOffset = hiAddrOffset << DF__LSB_ADDR_BIT_IN_DRAM_OFFSET;
  return (hiAddrOffset);
}

/*------------------------------------------------------------------
 Function: getDramAddressMap
 Purpose: Get the DF address map array to be handed around in these
          functions so that it is not continually refetched.
 Inputs: The DF type, an instanceID (CS), a nodeID (for the PCIe bus/device)
   and a map instance number (0 through 15)
 Outputs: This function will populate the "DRAM address map array"
          with the requested map. In DF3, the global interleave control
          bits will be fetched as well, allowing the code to treat the
          two similarly. In addition, the "CS target remap" registers
          will be brought in only when they are valid. When they are
          not valid or remapping is not enabled, the map registers are
          setup as "unity". This is mainly to simplify the logic and
          keep it common across all variants.
 *------------------------------------------------------------------*/
VOID
getDramAddressMap (
  UINT32 dfType,
  INT32  instanceId,
  UINT32 nodeId,
  UINT32 mapNumber,
  UINT32 *dramAddressMapRegs
  )
{
  UINT32 addr;
  UINT32 remapSel;
  UINT32 dstFabricId;
  UINT32 i;

  ASSERT (instanceId != BROADCAST_ACCESS);
  ASSERT (mapNumber < getNumAddressMaps(dfType));

  // initialize the values
  for (i=0; i<(ADDR_MAP_ARRAYSIZE-2); i++)
  {
    dramAddressMapRegs[i] = 0;
  }
  dramAddressMapRegs[ADDR_MAP_ARRAY_NP2_OFFSET] = 0xFFFFFFFF; // not used
  dramAddressMapRegs[ADDR_MAP_ARRAY_REMAPLO_OFFSET] = 0x76543210; // unity
  dramAddressMapRegs[ADDR_MAP_ARRAY_REMAPHI_OFFSET] = 0xFEDCBA98; // unity

  if ((dfType == DF_TYPE_DF3POINT5) && (mapNumber == DF__NUM_DRAM_MAPS_AVAILABLE_PRE_DF4))
  {
    // This is a request to fetch the "mega" address map. In DF3.5, this is only used for MI200 dGPU.
    // (it is used in the MI200 for heterogeneous systems too, but we only read Trento registers for heterogeneous)
    dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET] = getDfReg(dfType, instanceId, nodeId, DF__DRAMMEGABASE_REGADDR_PRE_DF4);
    dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET] = getDfReg(dfType, instanceId, nodeId,DF__DRAMMEGALIMIT_REGADDR_PRE_DF4);
    // Place the DF3.5 "Log2DieAddr64KSpzce" in the same place as it would be in DF4.
    dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET] = getDfReg(dfType, instanceId, nodeId,DF__DRAMMEGACONTROL_REGADDR_PRE_DF4) << 24;
    // Pretend to set the Mega bit in the (otherwise empty) DramAddressCtl register
    dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET] = 0x8;
  }
  if (dfType < DF_TYPE_DF4)
  {
    addr = DF__DRAMBASE0_REGADDR_PRE_DF4;
    addr += (mapNumber * 8);
    dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET] = getDfReg(dfType, instanceId, nodeId, addr);
    dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET] = getDfReg(dfType, instanceId, nodeId, addr + 4);
    // We treat the NP2_CHANNEL_CONFIG and REMAP in DF3 as an extension to the DF maps
    // to keep the code the same between DF3 and DF4
    // Warning: These registers do not exist prior to GN, so you cannot read them.
    //          To avoid this, we only read the registers if the interleave mode is 6-channel
    //          (which can't be enabled prior to GN either), and force the registers
    //          to look like unity.
    if (decodeDramIntLvNumChan(dfType, dramAddressMapRegs) == INTERLEAVE_MODE_DF3_6CHAN)
    {
      dstFabricId = extractDstFabricId(dfType, dramAddressMapRegs) & getNodeIdMask(dfType);
      // Read the correct CS target remap register based on DstFabricId
      if ((dstFabricId & getSocketIdMask(dfType)) == 0)
      {
        dramAddressMapRegs[ADDR_MAP_ARRAY_REMAPLO_OFFSET] = getDfReg(dfType, BROADCAST_ACCESS, nodeId, DF__SKT0CSTARGETREMAP0_REGADDR);
      }
      else
      {
        dramAddressMapRegs[ADDR_MAP_ARRAY_REMAPLO_OFFSET] = getDfReg(dfType, BROADCAST_ACCESS, nodeId, DF__SKT1CSTARGETREMAP0_REGADDR);
      }
      // Must read NP2_CHANNEL_CONFIG on the nodeID from the DstFabricID
      // (you may be reading the DRAM base registers from a CCM on node 0
      //  but then find the DstFabricID to be on node 1. Since the log2Addr64K
      //  is only available in CS, there is no guarantee that it is programmed on
      //  node 0 (or programmed to the same value)
      nodeId = dstFabricId & getNodeIdMask(dfType) >> getNodeIdShift(dfType);
      dramAddressMapRegs[ADDR_MAP_ARRAY_NP2_OFFSET] = getDfReg(dfType, BROADCAST_ACCESS, nodeId, DF__NP2CHANNELCONFIG_REGADDR);
    }
  }
  else if (dfType == DF_TYPE_DF4)
  {
    addr = DF__DRAMBASE0_REGADDR_DF4;
    addr += (mapNumber * 16);
    dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET] = getDfReg(dfType, instanceId, nodeId, addr);
    dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET] = getDfReg(dfType, instanceId, nodeId, addr + 4);
    dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET] = getDfReg(dfType, instanceId, nodeId, addr + 8);
    dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET] = getDfReg(dfType, instanceId, nodeId, addr + 12);
    if (getBit(DF__REMAP_EN_BITPOS_DF4, dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET]))
    {
      remapSel = getBits(DF__REMAP_SEL_BITPOS_LO_DF4, DF__REMAP_SEL_BITPOS_HI_DF4, dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET]);
      addr = DF__CSTARGETREMAP0A_REGADDR_DF4 + (remapSel * 8);
      dramAddressMapRegs[ADDR_MAP_ARRAY_REMAPLO_OFFSET] = dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET] = getDfReg(dfType, instanceId, nodeId, addr);
      dramAddressMapRegs[ADDR_MAP_ARRAY_REMAPHI_OFFSET] = dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET] = getDfReg(dfType, instanceId, nodeId, addr + 4);
    }
  }
  else
  {
    ASSERT (FALSE);
  }

  return;
}

/*------------------------------------------------------------------
 Function: getDfRegDramHoleCtrl
 Purpose: Get the DF::DramHoleControl register (always on node 0 via BROADCAST_ACCESS)
 Inputs: The DF type
 Outputs: The requested register
 *------------------------------------------------------------------*/
UINT32
getDfRegDramHoleCtrl (
  UINT32 dfType
  )
{
  UINT32 addr;
  if (dfType != DF_TYPE_DF4)
  {
    addr = DF__DRAMHOLECTL_REGADDR_PRE_DF4;
  }
  else
  {
    addr = DF__DRAMHOLECTL_REGADDR_DF4;
  }
  return (getDfReg(dfType, BROADCAST_ACCESS, 0, addr));
}

/*------------------------------------------------------------------
 Function: getDramHoleBase
 Purpose: Get the address of the DRAM hole
 Inputs: The DF type
 Outputs: The base address of the DRAM/MMIO hole (TOM)
 *------------------------------------------------------------------*/
UINT64
getDramHoleBase (
  UINT32 dfType
  )
{
  UINT64 dramHoleBase;

  dramHoleBase = getBits(DF__DRAM_HOLE_BASE_ADDR_BITPOS_LO, DF__DRAM_HOLE_BASE_ADDR_BITPOS_HI, getDfRegDramHoleCtrl(dfType));
  dramHoleBase = dramHoleBase << 24;

  return (dramHoleBase);
}

/*------------------------------------------------------------------
 Function: decodeDramIntLvAddrBit
 Purpose: Decode the least-significant interleave bit
 Inputs: DF2/DF3 mode, and the address map
 Outputs: The starting interleave bit (8, 9, 10, 11, or 12)
 *------------------------------------------------------------------*/
UINT32
decodeDramIntLvAddrBit (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs
  )
{
  UINT32 intLvAddrBit = 0;
  UINT32 intLvAddrSel = 0;

  if (dfType == DF_TYPE_DF2)
  {
    intLvAddrSel = getBits(DF__INTLV_ADDR_SEL_BITPOS_LO_DF2, DF__INTLV_ADDR_SEL_BITPOS_HI_DF2, dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET]);
  }
  else if ((dfType == DF_TYPE_DF3) || (dfType == DF_TYPE_DF3POINT5))
  {
    intLvAddrSel = getBits(DF__INTLV_ADDR_SEL_BITPOS_LO_DF3, DF__INTLV_ADDR_SEL_BITPOS_HI_DF3, dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET]);
  }
  else if (dfType == DF_TYPE_DF4)
  {
    intLvAddrSel = getBits(DF__INTLV_ADDR_SEL_BITPOS_LO_DF4, DF__INTLV_ADDR_SEL_BITPOS_HI_DF4, dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET]);
  }
  else
  {
    ASSERT (FALSE);
  }
  switch (intLvAddrSel)
  {
    case 0: intLvAddrBit = 8;
            break;
    case 1: intLvAddrBit = 9;
            break;
    case 2: intLvAddrBit = 10;
            break;
    case 3: intLvAddrBit = 11;
            break;
    case 4: intLvAddrBit = 12; // Milan only, but assume that if this is selected it is valid.
            break;
    default: ASSERT (FALSE); // Unsupported intLvAddrSel
  }

  return (intLvAddrBit);
}

/*------------------------------------------------------------------
 Function: getNumChannelFromDramIntLvMode
 Purpose: convert the IntLvMode (from decodeDramIntLvNumChan(intLvNumChan)
          into the nujmber of channels.
 Inputs: interleaving mode
 Outputs: The number of channels
 *------------------------------------------------------------------*/
INT32
getNumChannelFromDramIntLvMode (
  INT32 intLvMode
  )
{
  switch (intLvMode)
  {
    case INTERLEAVE_MODE_NONE:                 return (1);
    case INTERLEAVE_MODE_2CHAN_NOHASH:         return (2);
    case INTERLEAVE_MODE_ZP_2CHAN_HASH:        return (2);
    case INTERLEAVE_MODE_DF3_COD4_2CHAN_HASH:  return (2);
    case INTERLEAVE_MODE_DF4_NPS4_2CHAN_HASH:  return (2);
    case INTERLEAVE_MODE_DF4_NPS4_3CHAN_HASH:  return (3);
    case INTERLEAVE_MODE_4CHAN_NOHASH:         return (4);
    case INTERLEAVE_MODE_DF3_COD2_4CHAN_HASH:  return (4);
    case INTERLEAVE_MODE_DF4_NPS2_4CHAN_HASH:  return (4);
    case INTERLEAVE_MODE_DF4_NPS2_5CHAN_HASH:  return (5);
    case INTERLEAVE_MODE_DF3_6CHAN:            return (6);
    case INTERLEAVE_MODE_DF4_NPS2_6CHAN_HASH:  return (6);
    case INTERLEAVE_MODE_8CHAN_NOHASH:         return (8);
    case INTERLEAVE_MODE_DF3_COD1_8CHAN_HASH:  return (8);
    case INTERLEAVE_MODE_MI_HASH_8CHAN:        return (8);
    case INTERLEAVE_MODE_DF4_NPS1_8CHAN_HASH:  return (8);
    case INTERLEAVE_MODE_DF4_NPS1_10CHAN_HASH: return (10);
    case INTERLEAVE_MODE_DF4_NPS1_12CHAN_HASH: return (12);
    case INTERLEAVE_MODE_16CHAN_NOHASH:        return (16);
    case INTERLEAVE_MODE_MI_HASH_16CHAN:       return (16);
    case INTERLEAVE_MODE_32CHAN_NOHASH:        return (32);
    case INTERLEAVE_MODE_MI_HASH_32CHAN:       return (32);
  }
  ASSERT (FALSE);
  return 0;
}

/*------------------------------------------------------------------
 Function: decodeDramIntLvNumChan
 Purpose: Extract and decode IntLvNumChan from the address map
 Inputs: DF type, and the address map
 Outputs: The interleave mode (decoded)
 *------------------------------------------------------------------*/
UINT32
decodeDramIntLvNumChan (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs
  )
{
  UINT32 intLvNumChanField = 0;
  UINT32 intLvMode = 0;

  if (dfType == DF_TYPE_DF2)
  {
    intLvNumChanField = getBits(DF__INTLV_NUM_CHAN_BITPOS_LO_DF2, DF__INTLV_NUM_CHAN_BITPOS_HI_DF2, dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET]);
    // There was one reuse of an encoding. To hide this from the code, we convert to a different encoding.
    if (intLvNumChanField == 8)
    {
      intLvMode = INTERLEAVE_MODE_ZP_2CHAN_HASH;
    }
    else
    {
      intLvMode = intLvNumChanField;
    }
    // limited support in DF2...
    ASSERT ((intLvMode == INTERLEAVE_MODE_NONE) ||
            (intLvMode == INTERLEAVE_MODE_2CHAN_NOHASH) ||
            (intLvMode == INTERLEAVE_MODE_ZP_2CHAN_HASH));
  }
  else if (dfType == DF_TYPE_DF3)
  {
    intLvNumChanField = getBits(DF__INTLV_NUM_CHAN_BITPOS_LO_DF3, DF__INTLV_NUM_CHAN_BITPOS_HI_DF3, dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET]);
    intLvMode = intLvNumChanField;
  }
  else if (dfType == DF_TYPE_DF3POINT5)
  {
    intLvNumChanField = getBits(DF__INTLV_NUM_CHAN_BITPOS_LO_DF3, DF__INTLV_NUM_CHAN_BITPOS_HI_DF3POINT5, dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET]);
    intLvMode = intLvNumChanField;
    ASSERT (intLvMode != INTERLEAVE_MODE_DF3_6CHAN);
    // All the DF4 encodings are above the number of valid bits, so no need to check that they are excluded
  }
  else if (dfType == DF_TYPE_DF4)
  {
    intLvNumChanField = getBits(DF__INTLV_NUM_CHAN_BITPOS_LO_DF4, DF__INTLV_NUM_CHAN_BITPOS_HI_DF4, dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET]);
    intLvMode = intLvNumChanField;
    // All of the DF3 specific encodings are now reserved.
    ASSERT (intLvMode != INTERLEAVE_MODE_DF3_COD4_2CHAN_HASH);
    ASSERT (intLvMode != INTERLEAVE_MODE_DF3_COD2_4CHAN_HASH);
    ASSERT (intLvMode != INTERLEAVE_MODE_DF3_COD1_8CHAN_HASH);
    ASSERT (intLvMode != INTERLEAVE_MODE_DF3_6CHAN);
  }
  else
  {
    ASSERT (FALSE);
  }
  return (intLvMode);
}

/*------------------------------------------------------------------
 Function: extractDramIntLvNumDies
 Purpose: Extract extractDramIntLvNumDies from the address map
 Inputs: DF type, and the address map
 Outputs: The requested field (raw and not decoded)
 *------------------------------------------------------------------*/
UINT32
extractDramIntLvNumDies (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs
  )
{
  if (dfType == DF_TYPE_DF2)
  {
    return (getBits(DF__INTLV_NUM_DIES_BITPOS_LO_DF2, DF__INTLV_NUM_DIES_BITPOS_HI_DF2, dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET]));
  }
  if (dfType == DF_TYPE_DF3)
  {
    return (getBits(DF__INTLV_NUM_DIES_BITPOS_LO_DF3, DF__INTLV_NUM_DIES_BITPOS_HI_DF3, dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET]));
  }
  if (dfType == DF_TYPE_DF3POINT5)
  {
    return (getBit(DF__INTLV_NUM_DIES_BITPOS_DF3POINT5, dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET]));
  }
  if (dfType == DF_TYPE_DF4)
  {
    return (getBits(DF__INTLV_NUM_DIES_BITPOS_LO_DF4, DF__INTLV_NUM_DIES_BITPOS_HI_DF4, dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET]));
  }
  ASSERT (FALSE);
  return 0;
}

/*------------------------------------------------------------------
 Function: decodeDramIntLvNumDies
 Purpose: Decode IntLvNumDies from the address map
 Inputs: DF type, and the address map
 Outputs: The requested field, decoded (e.g. 1, 2, or 4 dies interleaved)
 *------------------------------------------------------------------*/
UINT32
decodeDramIntLvNumDies (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs
  )
{
  UINT32 intLvNumDies = 0;
  UINT32 numDieInterleaved = 0;

  intLvNumDies = extractDramIntLvNumDies(dfType, dramAddressMapRegs);

  switch (intLvNumDies)
  {
    case 0:
      numDieInterleaved = 1;
      break;
    case 1:
      numDieInterleaved = 2;
      break;
    case 2:
      numDieInterleaved = 4;
      break;
    default:
      ASSERT (FALSE);
  }
  return (numDieInterleaved);
}

/*------------------------------------------------------------------
 Function: extractDramIntLvNumSkts
 Purpose: Extract IntLvNumSkts from the address map
 Inputs: DF type, and the address map
 Outputs: The requested field (raw and not decoded)
 *------------------------------------------------------------------*/
UINT32
extractDramIntLvNumSkts (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs
  )
{
  if (dfType == DF_TYPE_DF2)
  {
    return (getBit(DF__INTLV_NUM_SOCKETS_BITPOS, dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET]));
  }
  else if ((dfType == DF_TYPE_DF3) || (dfType == DF_TYPE_DF3POINT5))
  {
    return (getBit(DF__INTLV_NUM_SOCKETS_BITPOS, dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET]));
  }
  else if (dfType == DF_TYPE_DF4)
  {
    return (getBit(DF__INTLV_NUM_SOCKETS_BITPOS_DF4, dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET]));
  }
  ASSERT (FALSE);
  return 0;
}

/*------------------------------------------------------------------
 Function: extractDstFabricId
 Purpose: Extract DstFabricId from from the address map
 Inputs: DF type, and the address map
 Outputs: The requested field
 *------------------------------------------------------------------*/
UINT32
extractDstFabricId (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs
  )
{
  if (dfType == DF_TYPE_DF2)
  {
    return (getBits(DF__DSTFABRICID_BITPOS_LO_DF2, DF__DSTFABRICID_BITPOS_HI_DF2, dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET]));
  }
  else if (dfType == DF_TYPE_DF3)
  {
    return (getBits(DF__DSTFABRICID_BITPOS_LO_DF3, DF__DSTFABRICID_BITPOS_HI_DF3, dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET]));
  }
  else if (dfType == DF_TYPE_DF3POINT5)
  {
    return (getBits(DF__DSTFABRICID_BITPOS_LO_DF3, DF__DSTFABRICID_BITPOS_HI_DF3POINT5, dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET]));
  }
  else if (dfType == DF_TYPE_DF4)
  {
    return (getBits(DF__DSTFABRICID_BITPOS_LO_DF4, DF__DSTFABRICID_BITPOS_HI_DF4, dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET]));
  }
  ASSERT (FALSE);
  return 0;
}

/*------------------------------------------------------------------
 Function: extractDramBaseAddr
 Purpose: Extract DramBaseAddr (64-bits) from from the address map
 Inputs: DF type, and the address map
 Outputs: The requested field
 *------------------------------------------------------------------*/
UINT64
extractDramBaseAddr (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs
  )
{
  UINT64 dramBaseAddr;

  if (dfType < DF_TYPE_DF4)
  {
    dramBaseAddr = getBits(DF__DRAM_BASE_ADDR_BITPOS_LO, DF__DRAM_BASE_ADDR_BITPOS_HI, dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET]);
  }
  else
  {
    dramBaseAddr = getBits(DF__DRAM_BASE_ADDR_BITPOS_LO_DF4, DF__DRAM_BASE_ADDR_BITPOS_HI_DF4, dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET]);
  }
  dramBaseAddr = dramBaseAddr << DF__LSB_ADDR_BIT_IN_DRAM_MAPS;

  return (dramBaseAddr);
}

/*------------------------------------------------------------------
 Function: extractDramLimitAddr
 Purpose: Extract DramLimitAddr (64-bits) from from the address map
 Inputs: DF type, and the address map
 Outputs: The requested field
 *------------------------------------------------------------------*/
UINT64
extractDramLimitAddr (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs
  )
{
  UINT64 dramLimitAddr;
  UINT64 lowBits;

  if (dfType < DF_TYPE_DF4)
  {
    dramLimitAddr = getBits(DF__DRAM_LIMIT_ADDR_BITPOS_LO, DF__DRAM_LIMIT_ADDR_BITPOS_HI, dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET]);
  }
  else
  {
    dramLimitAddr = getBits(DF__DRAM_LIMIT_ADDR_BITPOS_LO_DF4, DF__DRAM_LIMIT_ADDR_BITPOS_HI_DF4, dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET]);
  }
  dramLimitAddr = dramLimitAddr << DF__LSB_ADDR_BIT_IN_DRAM_MAPS;
  // Must extend the dramLimitAddr to the right (from LSB-1 : 0) with ones.
  lowBits = (1 << DF__LSB_ADDR_BIT_IN_DRAM_MAPS);
  lowBits = lowBits - 1;
  dramLimitAddr = dramLimitAddr | lowBits;

  return (dramLimitAddr);

}

/*------------------------------------------------------------------
 Function: extractDramAddrRangeValid
 Purpose: Extract AddrRngValid from from the address map
 Inputs: DF type, and the address map
 Outputs: The requested field
 *------------------------------------------------------------------*/
UINT32
extractDramAddrRangeValid (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs
  )
{
  if (dfType < DF_TYPE_DF4)
  {
    return (getBit(DF__ADDR_RANGE_VALID_BITPOS, dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET]));
  }
  return (getBit(DF__ADDR_RANGE_VALID_BITPOS, dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET]));
}

/*------------------------------------------------------------------
 Function: extractLgcyMmioHoleEn
 Purpose: Extract LgcyMmioHoleEn from from the address map
 Inputs: DF type, and the address map
 Outputs: The requested field
 *------------------------------------------------------------------*/
UINT32
extractLgcyMmioHoleEn (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs
  )
{
  if (dfType < DF_TYPE_DF4)
  {
    return (getBit(DF__LEGACY_MMIO_HOLE_EN_BITPOS, dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET]));
  }
  return (getBit(DF__LEGACY_MMIO_HOLE_EN_BITPOS, dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET]));
}

/*------------------------------------------------------------------
 Function: extractLog2Addr64KSpace
 Purpose: Extract Log2Addr64KSpace from from the address map
 Inputs: DF type, and the address map
 Outputs: The requested field
 *------------------------------------------------------------------*/
UINT32
extractLog2Addr64KSpace (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs
  )
{
  UINT32 log2Addr64KSpace = 0;

  ASSERT (dfType != DF_TYPE_DF2);
  ASSERT (dfType != DF_TYPE_DF3POINT5); // 6-channel is not supported

  if (dfType == DF_TYPE_DF3)
  {
    ASSERT (dramAddressMapRegs[ADDR_MAP_ARRAY_NP2_OFFSET] != 0xFFFFFFFF); // detect unsupported cases
    // 6-channel config is only supported on address space 0,
    // so only need to look at log2Addr64KSpace0
    log2Addr64KSpace = getBits(DF__LOG2_ADDR_SPACE_BITPOS_LO, DF__LOG2_ADDR_SPACE_BITPOS_HI, dramAddressMapRegs[ADDR_MAP_ARRAY_NP2_OFFSET]);
  }
  else if (dfType == DF_TYPE_DF4)
  {
    log2Addr64KSpace = getBits(DF__LOG2_ADDR_SPACE_BITPOS_LO_DF4, DF__LOG2_ADDR_SPACE_BITPOS_HI_DF4, dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET]);
  }
  else
  {
    ASSERT (FALSE);
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "log2Addr64KSpace=%d\n", log2Addr64KSpace);
  return (log2Addr64KSpace);
}


/*------------------------------------------------------------------
 Function: convertPhysicalCsFabricIdToLogicalCsFabricId
 Purpose: Remap a "actual FabricID" to a logical FabricID
 Inputs: The DF type, csFabricId (actual), and address map
 Outputs: The logical CS FabricID
 *------------------------------------------------------------------*/
UINT32
convertPhysicalCsFabricIdToLogicalCsFabricId (
  UINT32 dfType,
  UINT32 csFabricId,
  UINT32 *dramAddressMapRegs
  )
{
  INT32 i;
  UINT32 csComponentId;
  UINT32 physicalId;

  if (dfType == DF_TYPE_DF3POINT5)
  {
    return (csFabricId); // no remap supported on heterogeneous or MI200, avoid needing to read dGPU block fabric IDs
  }

  csComponentId = csFabricId & getComponentIdMask(dfType);

  // readDfAddressMap registers have already made this "unity map" if it is not enabled, so just use it

  // The index is logical->physical. We want physical->logical, so just search for the first match.
  for (i=0; i<8; i++)
  {
    physicalId = (dramAddressMapRegs[ADDR_MAP_ARRAY_REMAPLO_OFFSET] >> (i * 4)) & 0x0F;
    if (physicalId == csComponentId)
    {
      csFabricId = (csFabricId & getNodeIdMask(dfType)) | i;
      return (csFabricId);
    }
  }
  ASSERT (dfType >= DF_TYPE_DF4); // only 4-bit CS IDs supported prior to DF4
  for (i=0; i<8; i++)
  {
    physicalId = (dramAddressMapRegs[ADDR_MAP_ARRAY_REMAPHI_OFFSET] >> (i * 4)) & 0x0F;
    if (physicalId == csComponentId)
    {
      csFabricId = (csFabricId & getNodeIdMask(dfType)) | (i + 8);
      return (csFabricId);
    }
  }
  ASSERT (FALSE); // could not find it in the index register.
  return 0;
}

/*------------------------------------------------------------------
 Function: convertLogicalFabricIdToPhysicalFabricId
 Purpose: checkDramHit has found that a given address would map to
          a "logical" CS component on a given nodeID. However, this
          logical number may not map to a physical CS component.
          This determines the physical CS component.
          It is "packaged" in a FabricID format.
 Inputs: The DF type, logical CS component ID, its socket number and address map
 Outputs: The physical CS nodeID and instance ID packaged in a FabricID format

 Algorithm: There are two ways that a DstFabricID can be converted.
            1) The first is through a remap register in DF. This is
               indexed by logical CS number and gives a "physical CS"
            2) The second is by BIOS renumbering the IDs. So now what
               looks like DstFabricID=2 ends up going to physical CS 3
               for example. But we would find a CS instnace 3 which
               has a BlockFabricID=2.
            It is unlikely that both will be in play at the same time
            but this is not ruled out.
 *------------------------------------------------------------------*/
UINT32
convertLogicalFabricIdToPhysicalFabricId (
  UINT32 dfType,
  UINT32 logicalDstFabricId,
  UINT32 *dramAddressMapRegs
  )
{
  UINT32 remapReg;
  UINT32 fabricBlockInstanceInformation0Reg;
  UINT32 nodeId;
  UINT32 physicalCsComponentId;
  UINT32 physicalDstFabricId;
  UINT32 numDFInstances;
  UINT32 logicalCsId;
  UINT32 i;

  if (dfType == DF_TYPE_DF3POINT5)
  {
    return (logicalDstFabricId); // no remap supported on heterogeneous or MI200, avoid needing to read dGPU block fabric IDs
  }

  // First go through the CS remapper logic to convert the logical CS fabricID (nth offset in the map)
  // to an "actual" CS fabricID (the one that will go in the packet).
  // readDramAddressMapRegs have already made it "unity" if the remap is not enabled, so just read it.
  logicalCsId = logicalDstFabricId & getComponentIdMask(dfType);
  if (logicalCsId < 8)
  {
    remapReg = dramAddressMapRegs[ADDR_MAP_ARRAY_REMAPLO_OFFSET];
    physicalCsComponentId = (remapReg >> (logicalCsId * 4));
  }
  else
  {
    ASSERT (dfType == DF_TYPE_DF4);
    remapReg = dramAddressMapRegs[ADDR_MAP_ARRAY_REMAPHI_OFFSET];
    physicalCsComponentId = (remapReg >> ((logicalCsId - 8) * 4));
  }
  physicalCsComponentId &= 0x0F;
  physicalDstFabricId = (logicalDstFabricId & getNodeIdMask(dfType)) | physicalCsComponentId;

  // Now find which CS block will get this FabricID destination.
  // To allow for BIOS renumbering of fabricIDs, search for this instance ID.
  nodeId = (logicalDstFabricId & getNodeIdMask(dfType)) >> getNodeIdShift(dfType);
  numDFInstances = getBits(DF__BLOCK_INSTANCE_COUNT_BITPOS_LO, DF__BLOCK_INSTANCE_COUNT_BITPOS_HI, getDfRegFabricBlkInstanceCnt(nodeId));
  for (i=0; i<numDFInstances; i++)
  {
    fabricBlockInstanceInformation0Reg = getDfRegFabricBlkInstInfo0(dfType, i, nodeId);
    // Skip gated blocks (detected because at least one bit must be non-zero in non-gated blocks)
    if (fabricBlockInstanceInformation0Reg == 0)
    {
      continue;
    }
    if (getBits(DF__INSTANCE_TYPE_BITPOS_LO, DF__INSTANCE_TYPE_BITPOS_HI, fabricBlockInstanceInformation0Reg) != DF__CS_INSTANCE_TYPE_VALUE)
    {
      continue;
    }
    if (getBits(DF__BLOCK_FABRICID_BITPOS_LO, DF__BLOCK_FABRICID_BITPOS_HI, getDfRegFabricBlkInstInfo3(dfType, i, nodeId)) == physicalDstFabricId)
    {
      // Package the "CS instanceID" and the "CS nodeID" in a FabricID format.
      // Note that this is no longer the DstFabricID in any way. It is really now {nodeId, instanceID}
      physicalDstFabricId = (physicalDstFabricId & getNodeIdMask(dfType)) | i;
      return (physicalDstFabricId);
    }
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "Could not find logicalDstFabricId=0x%x (physicalDstFabricId=0x%x) in FabricBlockInstanceInformation3!\n",
          logicalDstFabricId, physicalDstFabricId);
  // CS not found!!!
  ASSERT (FALSE);
  return 0;
}

/*------------------------------------------------------------------
 Function: getCsLogicalComponentIdFromAddr
 Purpose: Determines the offset from DstFabricID based on the interleave modes
          and the given address.
 Inputs: DF type, address map, and address
 Outputs: The fabricID to add to DstFabricID.
 *------------------------------------------------------------------*/
UINT32
getCsLogicalComponentIdFromAddr (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs,
  UINT64 addr
  )
{
  UINT32 csId=0;
  UINT32 dieId=0;
  UINT32 socketId=0;
  UINT32 nodeId;
  UINT32 intLvMode;
  UINT32 numChanInterleaved;
  UINT32 intLvAddrBit;
  UINT32 numDieInterleaved;
  UINT32 numSocketsInterleaved;
  UINT32 hashPA, hashPA8, hashPA12, hashPA13, modValue;
  UINT32 addressMod;
  UINT32 intlvHashCtl, hashIntlvCtl64K, hashIntlvCtl2M, hashIntlvCtl1G;
  UINT64 tempAddr;
  UINT64 sizePerNode=0;

  // IDS_HDT_CONSOLE (MAIN_FLOW, "getCsLogicalComponentIdFromAddr(addr=0x%016lX, dfType=%d\n",
  //         addr, dfType);
  // IDS_HDT_CONSOLE (MAIN_FLOW, "dramBaseReg=0x%08X, dramLimitReg=0x%08X dramIntlvReg=0x%08X, dramCtlReg=0x%08X\n",
  //         dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET], dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET],
  //         dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET], dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET]);

  intLvMode = decodeDramIntLvNumChan(dfType, dramAddressMapRegs);
  intLvAddrBit = decodeDramIntLvAddrBit(dfType, dramAddressMapRegs);
  numChanInterleaved = getNumChannelFromDramIntLvMode(intLvMode);
  numDieInterleaved = decodeDramIntLvNumDies(dfType, dramAddressMapRegs);
  numSocketsInterleaved = extractDramIntLvNumSkts(dfType, dramAddressMapRegs) + 1;

  // non-interleaved should still have "1 socket/die interleaved"...
  ASSERT (numSocketsInterleaved != 0);
  ASSERT (numDieInterleaved != 0);
  ASSERT (numDieInterleaved <= 4);
  ASSERT (numSocketsInterleaved <= 2);

  intlvHashCtl = getIntlvHashCtlBits(dfType, dramAddressMapRegs);
  hashIntlvCtl64K = ((intlvHashCtl & 0x1) != 0) ? 1 : 0;
  hashIntlvCtl2M = ((intlvHashCtl & 0x2) != 0) ? 1 : 0;
  hashIntlvCtl1G = ((intlvHashCtl & 0x4) != 0) ? 1 : 0;

  switch (intLvMode)
  {
    case INTERLEAVE_MODE_NONE:
    case INTERLEAVE_MODE_2CHAN_NOHASH:
    case INTERLEAVE_MODE_4CHAN_NOHASH:
    case INTERLEAVE_MODE_8CHAN_NOHASH:
    case INTERLEAVE_MODE_16CHAN_NOHASH:
    case INTERLEAVE_MODE_32CHAN_NOHASH:
      if (intLvMode != INTERLEAVE_MODE_NONE)
      {
        csId = (UINT32)getBits64(intLvAddrBit,
                                 intLvAddrBit+((INT32)log2(numChanInterleaved)-1),
                                 addr);
      }
      if (numDieInterleaved > 1)
      {
        dieId = (UINT32)getBits64(intLvAddrBit+((INT32)log2(numChanInterleaved)),
                                  intLvAddrBit+((INT32)log2(numChanInterleaved))+((INT32)log2(numDieInterleaved)-1),
                                  addr);
      }
      if (numSocketsInterleaved > 1)
      {
        socketId = (UINT32)getBits64(intLvAddrBit+((INT32)log2(numChanInterleaved))+((INT32)log2(numDieInterleaved)),
                                     intLvAddrBit+((INT32)log2(numChanInterleaved))+((INT32)log2(numDieInterleaved))+((INT32)log2(numSocketsInterleaved)-1),
                                     addr);
      }
      break;
    case INTERLEAVE_MODE_DF3_COD4_2CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD2_4CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD1_8CHAN_HASH:
      // If we are using address hashing, the interleave address bit must be 8 or 9.
      ASSERT ((intLvAddrBit == 8) || (intLvAddrBit == 9));
      // Socket/die interleaving must be disabled.
      ASSERT (numDieInterleaved == 1);
      ASSERT (numSocketsInterleaved == 1);
      // CSSelect[0] = XOR of addr{intLvAddrBit, 14, 18, 23, 32}
      // CSSelect[1] = XOR of addr{12, 16, 21, 30}
      // CSSelect[2] = XOR of addr{13, 17, 22, 31}
      csId = 0;
      csId += (UINT32)((getBit64(intLvAddrBit, addr) ^
                        getBit64(14, addr) ^
                        (getBit64(18, addr) & hashIntlvCtl64K) ^
                        (getBit64(23, addr) & hashIntlvCtl2M) ^
                        (getBit64(32, addr) & hashIntlvCtl1G)
                      ) << 0);
      csId += (UINT32)((getBit64(12, addr) ^
                        (getBit64(16, addr) & hashIntlvCtl64K) ^
                        (getBit64(21, addr) & hashIntlvCtl2M) ^
                        (getBit64(30, addr) & hashIntlvCtl1G)
                      ) << 1);
      csId += (UINT32)((getBit64(13, addr) ^
                        (getBit64(17, addr) & hashIntlvCtl64K) ^
                        (getBit64(22, addr) & hashIntlvCtl2M) ^
                        (getBit64(31, addr) & hashIntlvCtl1G)
                      ) << 2);
      csId &= (numChanInterleaved - 1);
      break;
    case INTERLEAVE_MODE_DF3_6CHAN:
      // Interleave address bit must be 11 or 12.
      ASSERT ((intLvAddrBit == 11) || (intLvAddrBit == 12));
      // Socket/die interleaving must be disabled.
      ASSERT (numDieInterleaved == 1);
      ASSERT (numSocketsInterleaved == 1);
      // For intLvAddrBit==12:
      // HashPA[0] = {12 ^ 15 ^ 23 ^ 32}
      // HashPA[1] = {13 ^ 21 ^ 30}
      // HashPA[2] = {14 ^ 22 ^ 31}
      // ModId[1:0] = (HashPA[2:1] == 3) ? PA[n:15] % 3 : HashPA[2:1]
      // CsSelect[2:0] = {ModId[1:0], HashPA[0])
      // For intLvAddrBit==11:
      // HashPA[0] = {11 ^ 14 ^ 23 ^ 32}
      // HashPA[1] = {12 ^ 21 ^ 30}
      // HashPA[2] = {13 ^ 22 ^ 31}
      // ModId[1:0] = (HashPA[2:1] == 3) ? PA[n:14] % 3 : HashPA[2:1]
      // CsSelect[2:0] = {ModId[1:0], HashPA[0])
      hashPA = 0;
      hashPA += (UINT32)((getBit64(intLvAddrBit, addr) ^
                          getBit64(intLvAddrBit+3, addr) ^
                          (getBit64(23, addr) & hashIntlvCtl2M) ^
                          (getBit64(32, addr) & hashIntlvCtl1G)
                        ) << 0);
      hashPA += (UINT32)((getBit64(intLvAddrBit+1, addr) ^
                         (getBit64(21, addr) & hashIntlvCtl2M) ^
                         (getBit64(30, addr) & hashIntlvCtl1G)
                        ) << 1);
      hashPA += (UINT32)((getBit64(intLvAddrBit+2, addr) ^
                         (getBit64(22, addr) & hashIntlvCtl2M) ^
                         (getBit64(31, addr) & hashIntlvCtl1G)
                        ) << 2);
      // IDS_HDT_CONSOLE (MAIN_FLOW, "hashPA=%d\n", hashPA);
      if (getBits(1, 2, hashPA) == 3)
      {
        // keep bit 0 of hashPA
        hashPA = getBit(0, hashPA);
        // Find modulo 3 of bits n:15 or n:14 of system address
        tempAddr = getBits64(intLvAddrBit+3, 63, addr);
        addressMod = (UINT32) (tempAddr % 3);
        // IDS_HDT_CONSOLE (MAIN_FLOW, "tempAddr=0x%016lX, addressMod=%d\n",
        //         tempAddr, addressMod);
        // The modulo 3 is used as CsSelect[2:1]
        hashPA |= (addressMod << 1);
      }
      csId = hashPA;
      break;
    case INTERLEAVE_MODE_MI_HASH_8CHAN:
    case INTERLEAVE_MODE_MI_HASH_16CHAN:
    case INTERLEAVE_MODE_MI_HASH_32CHAN:
      // Socket/die interleaving must be disabled.
      ASSERT (numDieInterleaved == 1);
      ASSERT (numSocketsInterleaved == 1);
      // intLvAddrBit is actually a don't care. Assert that it must indicate bit 8.
      ASSERT (intLvAddrBit == 8);
      // CSSelect[0] = XOR of addr{8,  16, 21, 30};
      // CSSelect[1] = XOR of addr{9,  17, 22, 31};
      // CSSelect[2] = XOR of addr{10, 18, 23, 32};
      // CSSelect[3] = XOR of addr{11, 19, 24, 33};
      // CSSelect[4] = XOR of addr{12, 20, 25, 34};
      csId = 0;
      csId += (UINT32)((getBit64(8, addr) ^
                        (getBit64(16, addr) & hashIntlvCtl64K) ^
                        (getBit64(21, addr) & hashIntlvCtl2M) ^
                        (getBit64(30, addr) & hashIntlvCtl1G)
                       ) << 0);
      csId += (UINT32)((getBit64(9, addr) ^
                        (getBit64(17, addr) & hashIntlvCtl64K) ^
                        (getBit64(22, addr) & hashIntlvCtl2M) ^
                        (getBit64(31, addr) & hashIntlvCtl1G)
                       ) << 1);
      csId += (UINT32)((getBit64(10, addr) ^
                        (getBit64(18, addr) & hashIntlvCtl64K) ^
                        (getBit64(23, addr) & hashIntlvCtl2M) ^
                        (getBit64(32, addr) & hashIntlvCtl1G)
                       ) << 2);
      csId += (UINT32)((getBit64(11, addr) ^
                        (getBit64(19, addr) & hashIntlvCtl64K) ^
                        (getBit64(24, addr) & hashIntlvCtl2M) ^
                        (getBit64(33, addr) & hashIntlvCtl1G)
                       ) << 3);
      csId += (UINT32)((getBit64(12, addr) ^
                        (getBit64(20, addr) & hashIntlvCtl64K) ^
                        (getBit64(25, addr) & hashIntlvCtl2M) ^
                        (getBit64(34, addr) & hashIntlvCtl1G)
                       ) << 4);
      csId &= (numChanInterleaved - 1);
      break;
    case INTERLEAVE_MODE_ZP_2CHAN_HASH:
      // Iinterleave address bit must be 8 or 9.
      ASSERT ((intLvAddrBit == 8) || (intLvAddrBit == 9));
      // Socket/die interleaving must be disabled.
      ASSERT (numDieInterleaved == 1);
      ASSERT (numSocketsInterleaved == 1);
      // CSSelect[0] = XOR of addr{intLvAddrBit, 12, 18, 21, 30}
      csId = (UINT32)(getBit64(intLvAddrBit, addr) ^
                      getBit64(12, addr) ^
                      getBit64(18, addr) ^
                      getBit64(21, addr) ^
                      getBit64(30, addr));
      break;
    case INTERLEAVE_MODE_DF4_NPS4_2CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS2_4CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS1_8CHAN_HASH:
      // The interleave address bit must be 8.
      ASSERT (intLvAddrBit == 8);
      // Die interleaving must be disabled.
      ASSERT (numDieInterleaved == 1);
      ASSERT (numSocketsInterleaved <= 2);
      // CSSelect[0] = XOR of addr{8, 16, 21, 30}
      //               and add in an XOR of 14 if socket interleaving disabled
      // CSSelect[1] = XOR of addr{12, 17, 22, 31}
      // CSSelect[2] = XOR of addr{13, 18, 23, 32}
      // CSSelect[3] = XOR of addr{14, 19, 24, 33} // socket interleaving enabled only
      hashPA = (UINT32)(getBit64(8, addr) ^
                        (getBit64(16, addr) & hashIntlvCtl64K) ^
                        (getBit64(21, addr) & hashIntlvCtl2M) ^
                        (getBit64(30, addr) & hashIntlvCtl1G));
      if (numSocketsInterleaved == 1)
      {
        hashPA ^= getBit64(14, addr);
      }
      hashPA += (UINT32)((getBit64(12, addr) ^
                         (getBit64(17, addr) & hashIntlvCtl64K) ^
                         (getBit64(22, addr) & hashIntlvCtl2M) ^
                         (getBit64(31, addr) & hashIntlvCtl1G)
                        ) << 1);
      hashPA += (UINT32)((getBit64(13, addr) ^
                         (getBit64(17, addr) & hashIntlvCtl64K) ^
                         (getBit64(23, addr) & hashIntlvCtl2M) ^
                         (getBit64(32, addr) & hashIntlvCtl1G)
                        ) << 2);
      hashPA += (UINT32)((getBit64(14, addr) ^
                         (getBit64(19, addr) & hashIntlvCtl64K) ^
                         (getBit64(24, addr) & hashIntlvCtl2M) ^
                         (getBit64(33, addr) & hashIntlvCtl1G)
                        ) << 3);
      hashPA &= ((numChanInterleaved * numSocketsInterleaved) - 1);
      // When socket interleaving is enabled, CSSelect[0] is the socket bit
      if (numSocketsInterleaved == 1)
      {
        csId = hashPA;
      }
      else
      {
        socketId = hashPA & 1;
        csId = (hashPA >> 1);
      }
      break;
    case INTERLEAVE_MODE_DF4_NPS4_3CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS2_6CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS1_12CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS2_5CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS1_10CHAN_HASH:
      // The interleave address bit must be 8.
      ASSERT (intLvAddrBit == 8);
      // Die interleaving must be disabled.
      ASSERT (numDieInterleaved == 1);
      ASSERT (numSocketsInterleaved <= 2);
      switch (intLvMode)
      {
        case INTERLEAVE_MODE_DF4_NPS4_3CHAN_HASH:
        case INTERLEAVE_MODE_DF4_NPS2_6CHAN_HASH:
        case INTERLEAVE_MODE_DF4_NPS1_12CHAN_HASH:
          modValue = 3;
          break;
        case INTERLEAVE_MODE_DF4_NPS2_5CHAN_HASH:
        case INTERLEAVE_MODE_DF4_NPS1_10CHAN_HASH:
          modValue = 5;
          break;
      }
      // hashPA8
      hashPA8 = (UINT32)(getBit64(8, addr) ^
                         getBit64(14, addr) ^
                         (getBit64(16, addr) & hashIntlvCtl64K) ^
                         (getBit64(21, addr) & hashIntlvCtl2M) ^
                         (getBit64(30, addr) & hashIntlvCtl1G));
      // hashPA12
      hashPA12 = (UINT32)(getBit64(12, addr) ^
                          (getBit64(17, addr) & hashIntlvCtl64K) ^
                          (getBit64(22, addr) & hashIntlvCtl2M) ^
                          (getBit64(31, addr) & hashIntlvCtl1G));
      // hashPA13
      hashPA13 = (UINT32)(getBit64(13, addr) ^
                          (getBit64(18, addr) & hashIntlvCtl64K) ^
                          (getBit64(23, addr) & hashIntlvCtl2M) ^
                          (getBit64(32, addr) & hashIntlvCtl1G));
      tempAddr = getBits64(14, 63, addr);
      addressMod = tempAddr % modValue;

      // get csID from function{hashPA8/12/13, address % modValue}
      if (hashPA8)
      {
        csId = addressMod + 1;
        csId = csId % modValue;
      }
      else
      {
        csId = addressMod;
      }
      switch (intLvMode)
      {
        case INTERLEAVE_MODE_DF4_NPS2_6CHAN_HASH:
        case INTERLEAVE_MODE_DF4_NPS1_10CHAN_HASH:
          csId += hashPA13 * modValue;
          break;
        case INTERLEAVE_MODE_DF4_NPS1_12CHAN_HASH:
          csId += ((hashPA13 << 1) | hashPA12) * modValue;
          break;
      }
      if (numSocketsInterleaved > 1)
      {
        socketId = hashPA8;
      }
      break;
    default: ASSERT (FALSE);
  }

  // check for mega address map
  if ((dfType >= DF_TYPE_DF3POINT5) && (getBit(3, dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET]) != 0))
  {
    sizePerNode = (UINT32) getBits(24, 29, dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET]);
    sizePerNode = ((UINT64) 1 << sizePerNode); // 2 to the power of log2DieAddr64KSpace
    sizePerNode = sizePerNode << 16; // multiply by 64KB

    // Find the integer division which will give you the nodeID
    nodeId = (UINT32) (addr / sizePerNode);
    // and place the nodeID in the cs ID
    csId |= (nodeId << getNodeIdShift(dfType));
  }

  // IDS_HDT_CONSOLE (MAIN_FLOW, "intLvMode=%d, numChanInterleaved=%d, intLvAddrBit=%d, numDieInterleaved=%d, numSocketsInterleaved=%d\n",
  //         intLvMode, numChanInterleaved, intLvAddrBit, numDieInterleaved, numSocketsInterleaved);
  // IDS_HDT_CONSOLE (MAIN_FLOW, "csId=%d, dieId=%d, socketId=%d\n",
  //         csId, dieId, socketId);

  if (dieId != 0)
  {
    csId |= (dieId << getDieIdShift(dfType));
  }

  if (socketId != 0)
  {
    csId |= (socketId << getSocketIdShift(dfType));
  }

  return (csId);
}

/*------------------------------------------------------------------
 Function: normalizeMod3
 Purpose: Helper function to just do the mod3 algorithm
 Inputs: dfType, address map, and address
 Outputs: Normalized address
 *------------------------------------------------------------------*/
UINT64
normalizeMod3 (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs,
  UINT64 addr
  )
{
  UINT64 normAddr;
  UINT32 hashPA;
  UINT32 intlvHashCtl, hashIntlvCtl2M, hashIntlvCtl1G; //hashIntlvCtl64K
  UINT32 intLvMode;
  UINT32 intLvAddrBit;
  UINT32 numInterleaveBits;
  UINT32 log2Addr64KSpace;

  // Calculate hashPA
  //   For intLvAddrBit==12:
  //     HashPA[2] = {14 ^ 22 ^ 31}
  //     HashPA[1] = {13 ^ 21 ^ 30}
  //     HashPA[0] = {12 ^ 15 ^ 23 ^ 32}
  //     ModId[1:0] = (HashPA[2:1] == 3) ? PA[n:15] % 3 : HashPA[2:1]
  //     CsSelect[2:0] = {ModId[1:0], HashPA[0])
  //   For intLvAddrBit==11:
  //     HashPA[2] = {13 ^ 22 ^ 31}
  //     HashPA[1] = {12 ^ 21 ^ 30}
  //     HashPA[0] = {11 ^ 14 ^ 23 ^ 32}
  //     ModId[1:0] = (HashPA[2:1] == 3) ? PA[n:14] % 3 : HashPA[2:1]
  //     CsSelect[2:0] = {ModId[1:0], HashPA[0])

  intLvAddrBit = decodeDramIntLvAddrBit(dfType, dramAddressMapRegs);

  intlvHashCtl = getIntlvHashCtlBits(dfType, dramAddressMapRegs);
  // hashIntlvCtl64K = ((intlvHashCtl & 0x1) != 0) ? 1 : 0;
  hashIntlvCtl2M = ((intlvHashCtl & 0x2) != 0) ? 1 : 0;
  hashIntlvCtl1G = ((intlvHashCtl & 0x4) != 0) ? 1 : 0;

  hashPA = (UINT32)((getBit64(intLvAddrBit+2, addr) ^
                     (getBit64(22, addr) & hashIntlvCtl2M) ^
                     (getBit64(31, addr) & hashIntlvCtl1G))
                    << 2);
  hashPA += (UINT32)((getBit64(intLvAddrBit+1, addr) ^
                      (getBit64(21, addr) & hashIntlvCtl2M) ^
                      (getBit64(30, addr) & hashIntlvCtl1G))
                     << 1);
  hashPA += (UINT32)(getBit64(intLvAddrBit, addr) ^
                     getBit64(intLvAddrBit+3, addr) ^
                     (getBit64(23, addr) & hashIntlvCtl2M) ^
                     (getBit64(32, addr) & hashIntlvCtl1G));

  intLvMode = decodeDramIntLvNumChan(dfType, dramAddressMapRegs);
  // The number of interleave bits is as if we rounded up to the next power of 2
  // For example, 6-channel interleaving takes out 3 bits (6->8, log2(8)=3)
  // To do this, we simply truncate the log2 and add 1.
  numInterleaveBits = ((INT32) log2(getNumChannelFromDramIntLvMode(intLvMode))) +  1;

  // Pull out the three bits that we have as part of the interleave
  normAddr = removeBits64(intLvAddrBit, (intLvAddrBit + numInterleaveBits - 1), addr);

  // if HashPA is 11x, then force on the two MSBs of the address
  if (getBits(1, 2, hashPA) == 3)
  {
    log2Addr64KSpace = extractLog2Addr64KSpace(dfType, dramAddressMapRegs);
    // Force the two MSBs (as indicated by log2Addr64KSpace) to one.
    // Since log2Addr64KSpace is in units of 64KB (2^16) and in units of the entire
    // DRAM map, we first must "normalize" it (16-numInterleaveBits)
    // Then the shift is 2 less than this (to align to MSB-2)
    normAddr |= (((UINT64) 3) << (14 - numInterleaveBits + log2Addr64KSpace));
  }

  return (normAddr);
}

/*------------------------------------------------------------------
 Function: normalizeDf4NP2
 Purpose: Helper function to just do the DF4 anon-power-of-two lgorithm
 Inputs: dfType, address map, and address
 Outputs: Normalized address
 *------------------------------------------------------------------*/
UINT64 normalizeDf4NP2 (UINT32 dfType, UINT32 *dramAddressMapRegs, UINT64 addr)
{
  UINT32 hashPA8;
  UINT64 normalizedAddr;
  UINT64 tempAddrA;
  UINT64 tempAddrB;
  UINT32 intLvMode;
  UINT32 intlvHashCtl, hashIntlvCtl64K, hashIntlvCtl2M, hashIntlvCtl1G;
  UINT32 shiftValue= 0;
  UINT32 modValue = 0;

  intLvMode = decodeDramIntLvNumChan(dfType, dramAddressMapRegs);

  intlvHashCtl = getIntlvHashCtlBits(dfType, dramAddressMapRegs);
  hashIntlvCtl64K = ((intlvHashCtl & 0x1) != 0) ? 1 : 0;
  hashIntlvCtl2M = ((intlvHashCtl & 0x2) != 0) ? 1 : 0;
  hashIntlvCtl1G = ((intlvHashCtl & 0x4) != 0) ? 1 : 0;

  switch (intLvMode)
  {
    case INTERLEAVE_MODE_DF4_NPS4_3CHAN_HASH:
      modValue = 3;
      shiftValue = 13;
      break;
    case INTERLEAVE_MODE_DF4_NPS2_6CHAN_HASH:
      modValue = 3;
      shiftValue = 12;
      break;
    case INTERLEAVE_MODE_DF4_NPS1_12CHAN_HASH:
      modValue = 3;
      shiftValue = 11;
      break;
    case INTERLEAVE_MODE_DF4_NPS2_5CHAN_HASH:
      modValue = 5;
      shiftValue = 13;
      break;
    case INTERLEAVE_MODE_DF4_NPS1_10CHAN_HASH:
      modValue = 5;
      shiftValue = 12;
      break;
    default:  //Should not be here. Since normalizeDf4NP2 will only be invoked if intLvMode is one of the above cases.
      return 0;
  }

  // get PA[n:14] / 3 or 5
  tempAddrA = getBits64(14, 63, addr);
  tempAddrA = tempAddrA / modValue;

  // When socket interleaving is disabled, hashPA8 is stored as part of the NA
  // It will be just below the divided PA[n:14]
  // When socket interleaving is enabled, hashPA8 is "stored" in the CSID
  if (extractDramIntLvNumSkts(dfType, dramAddressMapRegs) == 0)
  {
    // Calculate hashPA8, as this is part of the normalized address
    // (unless it is socketID of the CSID)
    hashPA8 = (UINT32)(getBit64(8, addr) ^
                       getBit64(14, addr) ^
                       (getBit64(16, addr) & hashIntlvCtl64K) ^
                       (getBit64(21, addr) & hashIntlvCtl2M) ^
                       (getBit64(30, addr) & hashIntlvCtl1G));

    tempAddrA = tempAddrA << 1;
    tempAddrA |= ((UINT64) hashPA8);
  }

  // Align PA[n:14] / modValue and the hashPA[8] into bit[shiftValue]
  tempAddrA = tempAddrA << shiftValue;

  // Now remove the bits that are part of the CSID
  // bit8 (always), bit 12 and bit 13 (based on the interleaving mode)
  tempAddrB = removeBits64(8, 8, addr);

  // Keep around only the bits below the shiftValue
  //   This will remove bit 12 and bit 13 based on the interleaving mode.
  tempAddrB = getBits64(0, (shiftValue-1), tempAddrB);

  // Now combine:
  //   tempAddrA = PA[n:14] / modValue || hashPA8 (when necessary)
  //   tempAddrB = PA[13:9] (as appropriate) and [7:0]
  normalizedAddr = tempAddrA | tempAddrB;

  return (normalizedAddr);
}

/*------------------------------------------------------------------
 Function: normalizeAddr
 Purpose: Helper function to do the address normalization
 Inputs: dfType, address map, and address
 Outputs: Normalized address
 *------------------------------------------------------------------*/
UINT64
normalizeAddr (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs,
  UINT64 addr
  )
{
  UINT64 normalizedAddr = 0;;
  UINT32 intLvAddrBit = 0;;
  UINT32 intLvMode = 0;;
  UINT32 numInterleaveBits = 0;;
  BOOLEAN debugDataValid = FALSE;

  // Note that most sanity checks were already done in checkDramHit. There is no need to repeat them again.
  intLvMode = decodeDramIntLvNumChan(dfType, dramAddressMapRegs);

  IDS_HDT_CONSOLE (MAIN_FLOW, "normalizeAddr, dfType=%d, addr=0x%016lX\n", dfType, addr);
  IDS_HDT_CONSOLE (MAIN_FLOW, "dramBaseReg=0x%08X, dramLimitReg=0x%08X dramIntlvReg=0x%08X, dramCtlReg=0x%08X\n",
          dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET], dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET],
          dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET], dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET]);

  switch (intLvMode)
  {
    case INTERLEAVE_MODE_NONE:
      normalizedAddr = addr;
      break;
    case INTERLEAVE_MODE_2CHAN_NOHASH:
    case INTERLEAVE_MODE_4CHAN_NOHASH:
    case INTERLEAVE_MODE_8CHAN_NOHASH:
    case INTERLEAVE_MODE_16CHAN_NOHASH:
    case INTERLEAVE_MODE_32CHAN_NOHASH:
    case INTERLEAVE_MODE_MI_HASH_8CHAN:
    case INTERLEAVE_MODE_MI_HASH_16CHAN:
    case INTERLEAVE_MODE_MI_HASH_32CHAN:
    case INTERLEAVE_MODE_ZP_2CHAN_HASH:
      // The system address bit consists of:
      //      XXXXXXXXXIIIYYY
      // where III is the ID for this CS, and XXXXXX and YYYYY are the address bits used
      // in the normalized address.
      // Simply remove the number of channel bits (III)
      numInterleaveBits = log2(getNumChannelFromDramIntLvMode(intLvMode));
      numInterleaveBits += extractDramIntLvNumDies(dfType, dramAddressMapRegs);
      numInterleaveBits += extractDramIntLvNumSkts(dfType, dramAddressMapRegs);
      intLvAddrBit = decodeDramIntLvAddrBit(dfType, dramAddressMapRegs);
      // Pull out the III (from intLvAddrBit to intLvAddrBit + numInterleavedBits-1)
      normalizedAddr = removeBits64(intLvAddrBit, (intLvAddrBit + numInterleaveBits - 1), addr);
      debugDataValid = TRUE;
      break;
    case INTERLEAVE_MODE_DF3_COD4_2CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD2_4CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD1_8CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS4_2CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS2_4CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS1_8CHAN_HASH:
      // The system address bit consists of:
      //      XXXXXXIIZZZIYYY
      //   where III is the ID for this CS, and XXXXXXZZZYYYYY are the address bits used
      //   in the normalized address.
      //   log2(number of channels) tells us how many "I" bits there are.
      //   intLvAddrBit tells us how many "Y" bits there are (where the first "I" starts)
      //   The remaining III bits are higher up starting at bit 12.
      // Pull out the low-order I bit (intLvAddrBit)
      intLvAddrBit = decodeDramIntLvAddrBit(dfType, dramAddressMapRegs);
      normalizedAddr = removeBits64(intLvAddrBit, intLvAddrBit, addr);
      // Pull out the high-order I bits (intLvAddrBit)
      numInterleaveBits = log2(getNumChannelFromDramIntLvMode(intLvMode));
      numInterleaveBits += extractDramIntLvNumSkts(dfType, dramAddressMapRegs); // DF4 only
      if (numInterleaveBits > 1)
      {
        // Since we pulled out one bit already, the remaining III bits are now at bit 11.
        // So remove bits (11 to 11+numInterLeaveBits-1-1)
        //    first -1 because we have already removed one bit
        //    second -1 to get the "numInterLeaveBits" to get the top-most bit
        normalizedAddr = removeBits64(11, (9 + numInterleaveBits), normalizedAddr);
      }
      debugDataValid = TRUE;
      break;
    case INTERLEAVE_MODE_DF3_6CHAN:
      normalizedAddr = normalizeMod3(dfType, dramAddressMapRegs, addr);
      break;
    case INTERLEAVE_MODE_DF4_NPS4_3CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS2_6CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS1_12CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS2_5CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS1_10CHAN_HASH:
      normalizedAddr = normalizeDf4NP2(dfType, dramAddressMapRegs, addr);
      break;
    default: ASSERT (FALSE);
  }
  if (debugDataValid) IDS_HDT_CONSOLE (MAIN_FLOW, "intLvAddrBit=%d, numInterleaveBits=%d\n", intLvAddrBit, numInterleaveBits);
  IDS_HDT_CONSOLE (MAIN_FLOW, "addr 0x%016lX normalized to 0x%016lX\n", addr, normalizedAddr);

  return (normalizedAddr);
}

/*------------------------------------------------------------------
 Function: deNormalizeAddrMod3
 Purpose: Helper function to do the address denormalization
 Inputs: dfType, csFabricId (already converted to logical), address map, and address
 Outputs: Normalized address
 *------------------------------------------------------------------*/
UINT64
deNormalizeAddrMod3 (
  UINT32 dfType,
  UINT32 logicalCsFabricId,
  UINT32 *dramAddressMapRegs,
  UINT64 normAddr
  )
{
  UINT64 deNormAddr = 0;
  UINT64 tempAddrA, tempAddrB;

  UINT32 intLvMode;
  UINT32 intLvAddrBit;
  UINT32 numInterleaveBits;
  UINT32 csId;
  UINT32 msbNormAddrBits;
  UINT32 physicalAddressMSB;
  UINT32 addressMod;
  UINT32 log2Addr64KSpace;
  //UINT32 intlvHashCtl, hashIntlvCtl64K, hashIntlvCtl2M, hashIntlvCtl1G;

  intLvMode = decodeDramIntLvNumChan(dfType, dramAddressMapRegs);
  intLvAddrBit = decodeDramIntLvAddrBit(dfType, dramAddressMapRegs);

  //intlvHashCtl = getIntlvHashCtlBits(dfType, dramAddressMapRegs);
  //hashIntlvCtl64K = ((intlvHashCtl & 0x1) != 0) ? 1 : 0;
  //hashIntlvCtl2M = ((intlvHashCtl & 0x2) != 0) ? 1 : 0;
  //hashIntlvCtl1G = ((intlvHashCtl & 0x4) != 0) ? 1 : 0;

  // Calculate the CS ID and adjust the MSBs appropriately
  // Also make room in the address for where the CS ID goes at the same time.
  csId = logicalCsFabricId & getComponentIdMask(dfType);
  switch (intLvMode)
  {
    case INTERLEAVE_MODE_DF3_6CHAN:
      // Check if two MSBs (as indicated by log2Addr64KSpace) are one.
      // 2^(log2Addr64KSpace+16) is the size of the DRAM address space rounded up to the nearest power of 2.
      // In other words, if you have a 6*1GB address space, the total size is 6GB. Log2(8GB) is 33, so
      // the value of log2Addr64KSpace is 17 (33-16=17).
      // numInterleaveBits is log2(number of channels rounded up to the nearest power of 2)
      // Therefore, 2^(log2Addr64KSpace+16-numInterleaveBits) is the size of the normalized address space.
      // and the 2 MSB bits are (log2Addr64KSpace+15-numInterleaveBits and log2Addr64KSpace+14+numInterleaveBits)
      numInterleaveBits = ((INT32) log2(getNumChannelFromDramIntLvMode(intLvMode))) +  1;
      log2Addr64KSpace = extractLog2Addr64KSpace(dfType, dramAddressMapRegs);
      msbNormAddrBits = (UINT32)getBits64((log2Addr64KSpace + 14 - numInterleaveBits),
                                          (log2Addr64KSpace + 15 - numInterleaveBits),
                                          normAddr);
      // If the two MSB address bits are 11b, then the logical CSID is now 6 or 7,
      // and the normalized address is adjusted based on the mod3
      if (msbNormAddrBits == 3)
      {
        // Remove the 2 MSBs of 11b.
        tempAddrB = getBits64(intLvAddrBit, (log2Addr64KSpace + 13 - numInterleaveBits), normAddr);
        // Calculate the mod3
        addressMod = tempAddrB % 3;
        // Calculate the physical address MSB based on the mod 3, the logical CS number and shift amount
        physicalAddressMSB = ((1 + ((intLvAddrBit+log2Addr64KSpace+1) & 1)) *
                              (3 - addressMod + getBits(1, 2, csId))) % 3;
        // Remember that tempAddrB is already just bits n:(intLvAddrBit+numInterleaveBits)
        tempAddrB |= (((UINT64) physicalAddressMSB) << (log2Addr64KSpace + 14 - numInterleaveBits - intLvAddrBit));
        // csId = [1,1,csId[0]]
        csId = (3 << 1) + (csId & 1);
      }
      else
      {
        tempAddrB = getBits64(intLvAddrBit, 63, normAddr);
      }
      // tempAddrA is all the normalized bits below the interleave bit.
      tempAddrA = getBits64(0, (intLvAddrBit - 1), normAddr);
      // align tempAddrB
      tempAddrB = tempAddrB << (intLvAddrBit + numInterleaveBits);
      deNormAddr = tempAddrA | tempAddrB;
      break;
  }

  // Insert the unhashed CS ID into the appropriate bits of the address
  switch (intLvMode)
  {
    case INTERLEAVE_MODE_DF3_6CHAN:
      // csID is inserted at bits (intLvAddrBit+numIntLvBit-1) : intLvAddrBit
      deNormAddr |= (((UINT64) csId) << intLvAddrBit);
      break;
  }

  return (deNormAddr);
}

/*------------------------------------------------------------------
 Function: deNormalizeAddrDf4Np2
 Purpose: Helper function to do the address denormalization
 Inputs: dfType, csFabricId (already converted to logical), address map, and address
 Outputs: Normalized address
 *------------------------------------------------------------------*/
UINT64 deNormalizeAddrDf4Np2 (UINT32 dfType, UINT32 logicalCsFabricId, UINT32 *dramAddressMapRegs, UINT64 normAddr)
{
  UINT64 deNormAddr;
  UINT64 tempAddrA, tempAddrB;
  UINT32 intLvMode;
  UINT32 modValue = 0;
  UINT32 shiftValue = 0;
  UINT32 hashPA8;
  UINT32 hashedBit;
  UINT32 group;
  UINT32 groupOffset;
  UINT32 numSocketsInterleaved;
  UINT32 intlvHashCtl, hashIntlvCtl64K, hashIntlvCtl2M, hashIntlvCtl1G;

  intLvMode = decodeDramIntLvNumChan(dfType, dramAddressMapRegs);

  intlvHashCtl = getIntlvHashCtlBits(dfType, dramAddressMapRegs);
  hashIntlvCtl64K = ((intlvHashCtl & 0x1) != 0) ? 1 : 0;
  hashIntlvCtl2M = ((intlvHashCtl & 0x2) != 0) ? 1 : 0;
  hashIntlvCtl1G = ((intlvHashCtl & 0x4) != 0) ? 1 : 0;

  switch (intLvMode)
  {
    case INTERLEAVE_MODE_DF4_NPS4_3CHAN_HASH:
      modValue = 3;
      shiftValue = 13;
      break;
    case INTERLEAVE_MODE_DF4_NPS2_6CHAN_HASH:
      modValue = 3;
      shiftValue = 12;
      break;
    case INTERLEAVE_MODE_DF4_NPS1_12CHAN_HASH:
      modValue = 3;
      shiftValue = 11;
      break;
    case INTERLEAVE_MODE_DF4_NPS2_5CHAN_HASH:
      modValue = 5;
      shiftValue = 13;
      break;
    case INTERLEAVE_MODE_DF4_NPS1_10CHAN_HASH:
      modValue = 5;
      shiftValue = 12;
      break;
    default:  //Should not be here. Since deNormalizeAddrDf4Np2 will only be invoked if intLvMode is one of the above cases.
      return 0;
  }

  numSocketsInterleaved = extractDramIntLvNumSkts(dfType, dramAddressMapRegs) + 1;
  // Remember the hashPA bit, which was stored in NA[shiftValue] or CS[SocketId]
  // If it was stored in NA, shift it out.
  if (numSocketsInterleaved == 1)
  {
    hashPA8 = (UINT32)getBit64(shiftValue, normAddr);
    tempAddrA = removeBits64(shiftValue, shiftValue, normAddr);
  }
  else
  {
    hashPA8 = (logicalCsFabricId & getSocketIdMask(dfType)) >> getSocketIdShift(dfType);
    tempAddrA = normAddr;
  }
  // Now make room for the real PA[8] to be inserted
  tempAddrA = expandBits64(8, 1, tempAddrA);

  // Now make room in the address for other bits from the CS
  if ((intLvMode == INTERLEAVE_MODE_DF4_NPS2_6CHAN_HASH) ||
      (intLvMode == INTERLEAVE_MODE_DF4_NPS1_10CHAN_HASH))
  {
    tempAddrA = expandBits64(13, 1, tempAddrA);
  }
  if (intLvMode == INTERLEAVE_MODE_DF4_NPS1_12CHAN_HASH)
  {
    tempAddrA = expandBits64(12, 2, tempAddrA);
  }
  // Keep around these low order bits
  tempAddrA = getBits64(0, 13, tempAddrA);

  // Get NA[n:14] * 3 or NA[n:14] * 5 (NA[14] actually depends on the interleave mode)
  tempAddrB = getBits64((shiftValue + 1 - log2(numSocketsInterleaved)), 63, normAddr);
  tempAddrB *= modValue;

  // Calculate your group (quadrant in mod3 and side in mod5) and your offset within that group
  group = (logicalCsFabricId & getComponentIdMask(dfType)) / modValue;
  groupOffset = (logicalCsFabricId & getComponentIdMask(dfType)) % modValue;

  // Add in the remainder from the division (which was stored in CS ID groupOffset)
  if (hashPA8)
  {
    if (groupOffset == 0)
    {
      groupOffset = modValue - 1;
    }
    else
    {
      groupOffset--;
    }
  }
  tempAddrB += (UINT64) groupOffset;

  // Realign this to PA[n:14]
  tempAddrB = tempAddrB << 14;

  // Merge together the PA[n:14] (tempAddrB) and PA[13:0] (tempAddrB)
  deNormAddr = tempAddrA | tempAddrB;

  // Now calculate the hash value for PA[8, 12, and 13]
  // and place them in there. The bits will be zero, so you can just OR it in.
  // bit 8 = hashPA8 ^ 14 ^ 16 ^ 21 ^ 30
  hashedBit = (UINT32)(hashPA8 ^
                       getBit64(14, deNormAddr) ^
                       (getBit64(16, deNormAddr) & hashIntlvCtl64K) ^
                       (getBit64(21, deNormAddr) & hashIntlvCtl2M) ^
                       (getBit64(30, deNormAddr) & hashIntlvCtl1G)
                      );
  // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
  deNormAddr = deNormAddr | (((UINT64) hashedBit) << 8);
  if (intLvMode == INTERLEAVE_MODE_DF4_NPS1_12CHAN_HASH)
  {
    // bit 12 = group[0] ^ 17 ^ 22 ^ 31
    // bit 13 = group[1] ^ 18 ^ 23 ^ 32
    hashedBit = (UINT32)(getBit(0, group)^
                         (getBit64(17, deNormAddr) & hashIntlvCtl64K) ^
                         (getBit64(22, deNormAddr) & hashIntlvCtl2M) ^
                         (getBit64(31, deNormAddr) & hashIntlvCtl1G)
                        );
    hashedBit += (UINT32)((getBit(1, group) ^
                           (getBit64(18, deNormAddr) & hashIntlvCtl64K) ^
                           (getBit64(23, deNormAddr) & hashIntlvCtl2M) ^
                           (getBit64(32, deNormAddr) & hashIntlvCtl1G)
                          ) << 1);
    // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
    deNormAddr = deNormAddr | (((UINT64) hashedBit) << 12);
  }
  else if ((intLvMode == INTERLEAVE_MODE_DF4_NPS2_6CHAN_HASH) ||
           (intLvMode == INTERLEAVE_MODE_DF4_NPS1_10CHAN_HASH))
  {
    // bit 13 = group[0] ^ 18 ^ 23 ^ 32
    hashedBit = (UINT32)(getBit(0, group) ^
                         (getBit64(18, deNormAddr) & hashIntlvCtl64K) ^
                         (getBit64(23, deNormAddr) & hashIntlvCtl2M) ^
                         (getBit64(32, deNormAddr) & hashIntlvCtl1G)
                        );
    // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
    deNormAddr = deNormAddr | (((UINT64) hashedBit) << 13);
  }

  return (deNormAddr);
}

/*------------------------------------------------------------------
 Function: deNormalizeAddr
 Purpose: Helper function to do the address denormalization
 Inputs: dfType, csFabricId, address map, and address
 Outputs: Normalized address
 *------------------------------------------------------------------*/
UINT64
deNormalizeAddr (
  UINT32 dfType,
  UINT32 csFabricId,
  UINT32 *dramAddressMapRegs,
  UINT64 normAddr
  )
{
  UINT64 deNormAddr=0;

  UINT32 intLvMode;
  UINT32 intLvAddrBit;
  UINT32 numInterleaveBits;
  UINT32 mask;
  UINT32 csId;
  UINT32 numDies;
  UINT32 numSockets;

  intLvMode = decodeDramIntLvNumChan(dfType, dramAddressMapRegs);
  intLvAddrBit = decodeDramIntLvAddrBit(dfType, dramAddressMapRegs);

  IDS_HDT_CONSOLE (MAIN_FLOW, "deNormalizeAddr, dfType=%d, csFabricId=0x%x, normAddr=0x%016lX\n",
          dfType, csFabricId, normAddr);
  IDS_HDT_CONSOLE (MAIN_FLOW, "dramBaseReg=0x%08X, dramLimitReg=0x%08X dramIntlvReg=0x%08X, dramCtlReg=0x%08X\n",
          dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET], dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET],
          dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET], dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET]);
  IDS_HDT_CONSOLE (MAIN_FLOW, "intLvMode=%d, intLvAddrBit=%d\n",
          intLvMode, intLvAddrBit);

  // If there is no interleaving, just return here to avoid special casing it everywhere.
  if (intLvMode == INTERLEAVE_MODE_NONE)
  {
    IDS_HDT_CONSOLE (MAIN_FLOW, "deNormalizeAddr=0x%016lX (unchanged)\n", normAddr);
    return (normAddr);
  }

  // Go through the reverse CS remap to convert this fabricID to a "logical" fabricID.
  csFabricId = (csFabricId & getNodeIdMask(dfType)) |
                convertPhysicalCsFabricIdToLogicalCsFabricId(dfType, csFabricId, dramAddressMapRegs);

  // All modulo algorithms are done elsewhere
  if (intLvMode == INTERLEAVE_MODE_DF3_6CHAN)
  {
    deNormAddr = deNormalizeAddrMod3(dfType, csFabricId, dramAddressMapRegs, normAddr);
    IDS_HDT_CONSOLE (MAIN_FLOW, "deNormalizeAddr=0x%016lX (DF3-NP2)\n", deNormAddr);
    return (deNormAddr);
  }
  // All modulo algorithms are done elsewhere
  if ((intLvMode == INTERLEAVE_MODE_DF4_NPS4_3CHAN_HASH) ||
      (intLvMode == INTERLEAVE_MODE_DF4_NPS2_6CHAN_HASH) ||
      (intLvMode == INTERLEAVE_MODE_DF4_NPS1_12CHAN_HASH) ||
      (intLvMode == INTERLEAVE_MODE_DF4_NPS2_5CHAN_HASH) ||
      (intLvMode == INTERLEAVE_MODE_DF4_NPS1_10CHAN_HASH))
  {
    deNormAddr = deNormalizeAddrDf4Np2(dfType, csFabricId, dramAddressMapRegs, normAddr);
    IDS_HDT_CONSOLE (MAIN_FLOW, "deNormalizeAddr=0x%016lX (DF4-NP2)\n", deNormAddr);
    return (deNormAddr);
  }

  // Make room in the address for where the channel identifier goes
  switch (intLvMode)
  {
    case INTERLEAVE_MODE_2CHAN_NOHASH:
    case INTERLEAVE_MODE_4CHAN_NOHASH:
    case INTERLEAVE_MODE_8CHAN_NOHASH:
    case INTERLEAVE_MODE_16CHAN_NOHASH:
    case INTERLEAVE_MODE_32CHAN_NOHASH:
    case INTERLEAVE_MODE_MI_HASH_8CHAN:
    case INTERLEAVE_MODE_MI_HASH_16CHAN:
    case INTERLEAVE_MODE_MI_HASH_32CHAN:
    case INTERLEAVE_MODE_ZP_2CHAN_HASH:
      numInterleaveBits = log2(getNumChannelFromDramIntLvMode(intLvMode));
      numInterleaveBits += extractDramIntLvNumDies(dfType, dramAddressMapRegs);
      numInterleaveBits += extractDramIntLvNumSkts(dfType, dramAddressMapRegs);
      // Make room for the CS ID at bit "intLvAddrBit"
      deNormAddr = expandBits64(intLvAddrBit, numInterleaveBits, normAddr);
      break;
    case INTERLEAVE_MODE_DF3_COD4_2CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD2_4CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD1_8CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS4_2CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS2_4CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS1_8CHAN_HASH:
      // Make room for the CS ID at bit "intLvAddrBit" and then <n> bits starting at bit 12
      // <n> is the number of interleave bits - 1
      numInterleaveBits = log2(getNumChannelFromDramIntLvMode(intLvMode));
      numInterleaveBits += extractDramIntLvNumSkts(dfType, dramAddressMapRegs); // DF4 only
      deNormAddr = expandBits64(intLvAddrBit, 1, normAddr);
      if (numInterleaveBits > 1)
      {
        deNormAddr = expandBits64(12, (numInterleaveBits-1), deNormAddr);
      }
      break;
    default: ASSERT (FALSE);
  }

  // Calculate the CS ID
  switch (intLvMode)
  {
    case INTERLEAVE_MODE_2CHAN_NOHASH:
    case INTERLEAVE_MODE_4CHAN_NOHASH:
    case INTERLEAVE_MODE_8CHAN_NOHASH:
    case INTERLEAVE_MODE_16CHAN_NOHASH:
    case INTERLEAVE_MODE_32CHAN_NOHASH:
    case INTERLEAVE_MODE_ZP_2CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD4_2CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD2_4CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD1_8CHAN_HASH:
    case INTERLEAVE_MODE_MI_HASH_8CHAN:
    case INTERLEAVE_MODE_MI_HASH_16CHAN:
    case INTERLEAVE_MODE_MI_HASH_32CHAN:
      numInterleaveBits = log2(getNumChannelFromDramIntLvMode(intLvMode));
      // You subtract off the base dstFabricId here, but only as many bits in the
      // channel mask. We dont have any way for the dstFabricID to "spill" over
      // to the nodeID, so this offset can only be within the number of channels.
      // All this means is that socket and die interleaving must not have an offset
      mask = getComponentIdMask(dfType);
      csId = (csFabricId & mask) - (extractDstFabricId(dfType, dramAddressMapRegs) & mask);
      mask = (1 << numInterleaveBits) - 1;
      csId = csId & mask;
      numDies = decodeDramIntLvNumDies(dfType, dramAddressMapRegs);
      if (numDies > 1)
      {
        mask = (1 << ((INT32)(log2(numDies)))) - 1;
        csId |= (((csFabricId & getDieIdMask(dfType)) >> getDieIdShift(dfType)) & mask) << numInterleaveBits;
        numInterleaveBits += (INT32) (log2(numDies));
      }
      numSockets = extractDramIntLvNumSkts(dfType, dramAddressMapRegs) + 1;
      if (numSockets > 1)
      {
        mask = (1 << ((INT32)(log2(numSockets)))) - 1;
        csId |= (((csFabricId & getSocketIdMask(dfType)) >> getSocketIdShift(dfType)) & mask) << numInterleaveBits;
      }
      break;
    case INTERLEAVE_MODE_DF4_NPS4_2CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS2_4CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS1_8CHAN_HASH:
      numInterleaveBits = log2(getNumChannelFromDramIntLvMode(intLvMode));
      // You subtract off the base dstFabricId here, but only as many bits in the
      // channel mask. We dont have any way for the dstFabricID to "spill" over
      // to the nodeID, so this offset can only be within the number of channels.
      // All this means is that socket and die interleaving must not have an offset
      mask = getComponentIdMask(dfType);
      csId = (csFabricId & mask) - (extractDstFabricId(dfType, dramAddressMapRegs) & mask);
      mask = (1 << numInterleaveBits) - 1;
      csId = csId & mask;
      ASSERT (decodeDramIntLvNumDies(dfType, dramAddressMapRegs) == 1);
      numSockets = extractDramIntLvNumSkts(dfType, dramAddressMapRegs) + 1;
      if (numSockets > 1)
      {
        numInterleaveBits = log2(numSockets);
        csId = csId << numInterleaveBits;
        mask = (1 << numInterleaveBits) - 1;
        csId |= ((csFabricId & getSocketIdMask(dfType)) >> getSocketIdShift(dfType)) & mask;
      }
      break;
    default: ASSERT (FALSE);
  }

  // Insert the unhashed CS ID into the appropriate bits of the address
  switch (intLvMode)
  {
    case INTERLEAVE_MODE_2CHAN_NOHASH:
    case INTERLEAVE_MODE_4CHAN_NOHASH:
    case INTERLEAVE_MODE_8CHAN_NOHASH:
    case INTERLEAVE_MODE_16CHAN_NOHASH:
    case INTERLEAVE_MODE_32CHAN_NOHASH:
    case INTERLEAVE_MODE_MI_HASH_8CHAN:
    case INTERLEAVE_MODE_MI_HASH_16CHAN:
    case INTERLEAVE_MODE_MI_HASH_32CHAN:
    case INTERLEAVE_MODE_ZP_2CHAN_HASH:
      deNormAddr |= (((UINT64) csId) << intLvAddrBit);
      break;
    case INTERLEAVE_MODE_DF3_COD4_2CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD2_4CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD1_8CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS4_2CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS2_4CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS1_8CHAN_HASH:
      // csID[0] is inserted at intLvAddrBit
      deNormAddr |= (((UINT64) (csId & 0x1)) << intLvAddrBit);
      // csID[2:1] is inserted at bits (12+numIntLvBit-2) : 12
      deNormAddr |= (((UINT64) (csId & 0xe)) << 11);
      break;
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "deNormalizeAddr=0x%016lX\n", deNormAddr);

  return (deNormAddr);
}

/*------------------------------------------------------------------
 Function: deNormHashAddr
 Purpose: Helper function to do the address denormalization
 Inputs: dfType, address map, and address
 Outputs: Normalized address after correcting any hashing
 *------------------------------------------------------------------*/
UINT64
deNormHashAddr (
  UINT32 dfType,
  UINT32 *dramAddressMapRegs,
  UINT64 deNormAddr
  )
{
  UINT32 intLvMode;
  UINT32 intLvAddrBit;
  UINT32 numInterleaveBits;
  UINT32 i;
  UINT32 intlvHashCtl, hashIntlvCtl64K, hashIntlvCtl2M, hashIntlvCtl1G;
  UINT32 hashedBit;
  UINT32 numSocketsInterleaved;
  UINT32 totalChannelsInterleaved;

  intLvMode = decodeDramIntLvNumChan(dfType, dramAddressMapRegs);
  switch (intLvMode)
  {
    case INTERLEAVE_MODE_NONE:
    case INTERLEAVE_MODE_2CHAN_NOHASH:
    case INTERLEAVE_MODE_4CHAN_NOHASH:
    case INTERLEAVE_MODE_8CHAN_NOHASH:
    case INTERLEAVE_MODE_16CHAN_NOHASH:
    case INTERLEAVE_MODE_32CHAN_NOHASH:
      return (deNormAddr);
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "deNormHashAddr 0x%016lX\n", deNormAddr);

  intLvAddrBit = decodeDramIntLvAddrBit(dfType, dramAddressMapRegs);

  // Perform any hashing
  intlvHashCtl = getIntlvHashCtlBits(dfType, dramAddressMapRegs);
  hashIntlvCtl64K = ((intlvHashCtl & 0x1) != 0) ? 1 : 0;
  hashIntlvCtl2M = ((intlvHashCtl & 0x2) != 0) ? 1 : 0;
  hashIntlvCtl1G = ((intlvHashCtl & 0x4) != 0) ? 1 : 0;
  switch (intLvMode)
  {
    case INTERLEAVE_MODE_ZP_2CHAN_HASH:
      // Hashing must use interleave address bit 8 or 9
      ASSERT ((intLvAddrBit == 8) || (intLvAddrBit == 9));
      // Does not support socket and die interleaving
      ASSERT (decodeDramIntLvNumDies(dfType, dramAddressMapRegs) == 1);
      ASSERT (extractDramIntLvNumSkts(dfType, dramAddressMapRegs) == 0);
      // CSSelect[0] = XOR of deNormAddr{intLvAddrBit, 12, 18, 21, 30}
      // DF2 didnt have any hash controls so these are unnecessary here.
      hashedBit = (UINT32)(getBit64(intLvAddrBit, deNormAddr) ^
                           getBit64(12, deNormAddr) ^
                           getBit64(18, deNormAddr) ^
                           getBit64(21, deNormAddr) ^
                           getBit64(30, deNormAddr));
      // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
      if (getBit64(intLvAddrBit, deNormAddr) != hashedBit)
      {
        deNormAddr ^= (((UINT64) 1) << intLvAddrBit);
      }
      break;
    case INTERLEAVE_MODE_MI_HASH_8CHAN:
    case INTERLEAVE_MODE_MI_HASH_16CHAN:
    case INTERLEAVE_MODE_MI_HASH_32CHAN:
      // Does not support socket and die interleaving
      ASSERT (decodeDramIntLvNumDies(dfType, dramAddressMapRegs) == 1);
      ASSERT (extractDramIntLvNumSkts(dfType, dramAddressMapRegs) == 0);
      // CSSelect[0] = XOR of addr{8,  16, 21, 30};
      // CSSelect[1] = XOR of addr{9,  17, 22, 31};
      // CSSelect[2] = XOR of addr{10, 18, 23, 32};
      // CSSelect[3] = XOR of addr{11, 19, 24, 33}; - 16 and 32 channel only
      // CSSelect[4] = XOR of addr{12, 20, 25, 34}; - 32 channel only
      numInterleaveBits = log2(getNumChannelFromDramIntLvMode(intLvMode));
      for (i=0; i<numInterleaveBits; i++)
      {
        hashedBit = (UINT32)(getBit64(8+i, deNormAddr) ^
                             (getBit64(16+i, deNormAddr) & hashIntlvCtl64K) ^
                             (getBit64(21+i, deNormAddr) & hashIntlvCtl2M) ^
                             (getBit64(30+i, deNormAddr) & hashIntlvCtl1G));
          // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
        if (getBit64(8+i, deNormAddr) != hashedBit)
        {
          deNormAddr ^= (((UINT64) 1) << (8+i));
        }
      }
      break;
    case INTERLEAVE_MODE_DF3_COD4_2CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD2_4CHAN_HASH:
    case INTERLEAVE_MODE_DF3_COD1_8CHAN_HASH:
      // Hashing must use interleave address bit 8 or 9
      ASSERT ((intLvAddrBit == 8) || (intLvAddrBit == 9));
      // Does not support socket and die interleaving
      ASSERT (decodeDramIntLvNumDies(dfType, dramAddressMapRegs) == 1);
      ASSERT (extractDramIntLvNumSkts(dfType, dramAddressMapRegs) == 0);
      // CSSelect[0] = XOR of deNormAddr{intLvAddrBit, 14, 18, 23, 32}
      hashedBit = (UINT32)(getBit64(intLvAddrBit, deNormAddr) ^
                           getBit64(14, deNormAddr) ^
                           (getBit64(18, deNormAddr) & hashIntlvCtl64K) ^
                           (getBit64(23, deNormAddr) & hashIntlvCtl2M) ^
                           (getBit64(32, deNormAddr) & hashIntlvCtl1G));
        // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
      if (getBit64(intLvAddrBit, deNormAddr) != hashedBit)
      {
        deNormAddr ^= (((UINT64) 1) << intLvAddrBit);
      }
      if ((intLvMode == INTERLEAVE_MODE_DF3_COD2_4CHAN_HASH) ||
          (intLvMode == INTERLEAVE_MODE_DF3_COD1_8CHAN_HASH))
      {
        // CSSelect[1] = XOR of deNormAddr{12, 16, 21, 30}
        hashedBit = (UINT32)(getBit64(12, deNormAddr) ^
                             (getBit64(16, deNormAddr) & hashIntlvCtl64K) ^
                             (getBit64(21, deNormAddr) & hashIntlvCtl2M) ^
                             (getBit64(30, deNormAddr) & hashIntlvCtl1G));
        // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
        if (getBit64(12, deNormAddr) != hashedBit)
        {
          deNormAddr ^= (((UINT64) 1) << 12);
        }
      }
      if (intLvMode == INTERLEAVE_MODE_DF3_COD1_8CHAN_HASH)
      {
        // CSSelect[2] = XOR of deNormAddr{13, 17, 22, 31}
        hashedBit = (UINT32)(getBit64(13, deNormAddr) ^
                             (getBit64(17, deNormAddr) & hashIntlvCtl64K) ^
                             (getBit64(22, deNormAddr) & hashIntlvCtl2M) ^
                             (getBit64(31, deNormAddr) & hashIntlvCtl1G));
        // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
        if (getBit64(13, deNormAddr) != hashedBit)
        {
          deNormAddr ^= (((UINT64) 1) << 13);
        }
      }
      break;
    case INTERLEAVE_MODE_DF3_6CHAN:
      // The number of interleave bits is as if we rounded up to the next power of 2
      // For example, 6-channel interleaving takes out 3 bits (6->8, log2(8)=3)
      // To do this, we simply truncate the log2 and add 1.
      numInterleaveBits = ((INT32) log2(getNumChannelFromDramIntLvMode(intLvMode))) +  1;
      // The hashPA was inserted into Addr[14:12] or Addr[13:11] (depending on intLvAddrBit)
      // Either Addr[15] or Addr[14] is used.
      // interleaveBits[0] = HashPA[0] ^ PA[intlvBit+numIntLvBits] ^ PA[23] ^ PA[32];
      hashedBit = (UINT32)(getBit64((intLvAddrBit + 0), deNormAddr) ^
                           getBit64((intLvAddrBit + numInterleaveBits), deNormAddr) ^
                           (getBit64(23, deNormAddr) & hashIntlvCtl2M) ^
                           (getBit64(32, deNormAddr) & hashIntlvCtl1G));
      // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
      if (getBit64(intLvAddrBit, deNormAddr) != hashedBit)
      {
        deNormAddr ^= (((UINT64) 1) << intLvAddrBit);
      }
      // interleaveBits[1] = HashPA[1] ^ PA[21] ^ PA[30];
      hashedBit = (UINT32)(getBit64((intLvAddrBit + 1), deNormAddr) ^
                           (getBit64(21, deNormAddr) & hashIntlvCtl2M) ^
                           (getBit64(30, deNormAddr) & hashIntlvCtl1G));
      // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
      if (getBit64(intLvAddrBit+1, deNormAddr) != hashedBit)
      {
        deNormAddr ^= (((UINT64) 1) << (intLvAddrBit+1));
      }
      // interleaveBits[2] = HashPA[2] ^ PA[22] ^ PA[31];
      hashedBit = (UINT32)(getBit64((intLvAddrBit + 2), deNormAddr) ^
                           (getBit64(22, deNormAddr) & hashIntlvCtl2M) ^
                           (getBit64(31, deNormAddr) & hashIntlvCtl1G));
      // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
      if (getBit64(intLvAddrBit+2, deNormAddr) != hashedBit)
      {
        deNormAddr ^= (((UINT64) 1) << (intLvAddrBit+2));
      }
      break;
    case INTERLEAVE_MODE_DF4_NPS4_2CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS2_4CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS1_8CHAN_HASH:
      // Hashing must use interleave address bit 8
      ASSERT (intLvAddrBit == 8);
      // Does not support die interleaving
      ASSERT (decodeDramIntLvNumDies(dfType, dramAddressMapRegs) == 1);
      numSocketsInterleaved = extractDramIntLvNumSkts(dfType, dramAddressMapRegs) + 1;
      ASSERT (numSocketsInterleaved <= 2);
      totalChannelsInterleaved = getNumChannelFromDramIntLvMode(intLvMode) * numSocketsInterleaved;
      // CSSelect[0] = XOR of addr{8, 16, 21, 30}
      //               and add in an XOR of 14 if socket interleaving disabled
      // CSSelect[1] = XOR of addr{12, 17, 22, 31}
      // CSSelect[2] = XOR of addr{13, 18, 23, 32}
      // CSSelect[3] = XOR of addr{14, 19, 24, 33} // socket interleaving enabled only
      hashedBit = (UINT32)(getBit64(8, deNormAddr) ^
                           (getBit64(16, deNormAddr) & hashIntlvCtl64K) ^
                           (getBit64(21, deNormAddr) & hashIntlvCtl2M) ^
                           (getBit64(30, deNormAddr) & hashIntlvCtl1G));
      if (numSocketsInterleaved == 1)
      {
        hashedBit ^= getBit64(14, deNormAddr);
      }
      // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
      if (getBit64(8, deNormAddr) != hashedBit)
      {
        deNormAddr ^= (((UINT64) 1) << 8);
      }
      if (totalChannelsInterleaved > 2)
      {
        hashedBit = (UINT32)(getBit64(12, deNormAddr) ^
                             (getBit64(17, deNormAddr) & hashIntlvCtl64K) ^
                             (getBit64(22, deNormAddr) & hashIntlvCtl2M) ^
                             (getBit64(31, deNormAddr) & hashIntlvCtl1G));
        // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
        if (getBit64(12, deNormAddr) != hashedBit)
        {
          deNormAddr ^= (((UINT64) 1) << 12);
        }
      }
      if (totalChannelsInterleaved > 4)
      {
        hashedBit = (UINT32)(getBit64(13, deNormAddr) ^
                             (getBit64(17, deNormAddr) & hashIntlvCtl64K) ^
                             (getBit64(23, deNormAddr) & hashIntlvCtl2M) ^
                             (getBit64(32, deNormAddr) & hashIntlvCtl1G));
        // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
        if (getBit64(13, deNormAddr) != hashedBit)
        {
          deNormAddr ^= (((UINT64) 1) << 13);
        }
      }
      if (totalChannelsInterleaved > 8)
      {
        hashedBit = (UINT32)(getBit64(14, deNormAddr) ^
                             (getBit64(19, deNormAddr) & hashIntlvCtl64K) ^
                             (getBit64(24, deNormAddr) & hashIntlvCtl2M) ^
                             (getBit64(33, deNormAddr) & hashIntlvCtl1G));
        // IDS_HDT_CONSOLE (MAIN_FLOW, "hashedBit=%d\n", hashedBit);
        if (getBit64(14, deNormAddr) != hashedBit)
        {
          deNormAddr ^= (((UINT64) 1) << 14);
        }
      }
      break;
    case INTERLEAVE_MODE_DF4_NPS4_3CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS2_6CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS1_12CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS2_5CHAN_HASH:
    case INTERLEAVE_MODE_DF4_NPS1_10CHAN_HASH:
      // For these cases, the hashing was done as the CS ID was inserted
      // Since it depended on functions that were already calculated.
      break;
    default:
      ASSERT (FALSE);
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "deNormHashAddr completed - 0x%016lX\n", deNormAddr);

  return (deNormAddr);
}

/*------------------------------------------------------------------
 Function: isSystemDiscreteGpu
 Purpose: Helper function to find determine if this node is a dGPU node
 Inputs:  DF type and nodeID
 Outputs: true if a CCM is found, false otherwise
 *------------------------------------------------------------------*/
BOOLEAN isSystemDiscreteGpu (UINT32 dfType, UINT32 nodeId)
{
  UINT32 i;
  UINT32 numDFInstances;
  UINT32 fabricBlockInstanceInformation0Reg;

  // Find a CCM instance ID
  numDFInstances = getBits(DF__BLOCK_INSTANCE_COUNT_BITPOS_LO, DF__BLOCK_INSTANCE_COUNT_BITPOS_HI, getDfRegFabricBlkInstanceCnt(nodeId));
  for (i=0; i<numDFInstances; i++)
  {
    fabricBlockInstanceInformation0Reg = getDfRegFabricBlkInstInfo0(dfType, i, nodeId);
    // Skip gated blocks (detected because at least one bit must be non-zero in non-gated blocks)
    if (fabricBlockInstanceInformation0Reg == 0)
    {
      continue;
    }
    if (getBits(DF__INSTANCE_TYPE_BITPOS_LO, DF__INSTANCE_TYPE_BITPOS_HI, fabricBlockInstanceInformation0Reg) == DF__CCM_INSTANCE_TYPE_VALUE)
    {
      return (FALSE);
    }
  }

  return (TRUE);
}

/*------------------------------------------------------------------
 Function: findModeratorInstanceId
 Purpose: Helper function to find a CCM or GCM instance ID on the given node
 Inputs:  DF type and nodeID
 Outputs: The first CCM or GCM instance ID. One must be found!
          A CCM is returned if found.
 *------------------------------------------------------------------*/
UINT32
findModeratorInstanceId (
  UINT32 dfType,
  UINT32 nodeId
  )
{
  UINT32 i;
  UINT32 numDFInstances;
  UINT32 fabricBlockInstanceInformation0Reg;
  INT32 firstGcmFabricId = -1;

  // Find a CCM instance ID
  numDFInstances = getBits(DF__BLOCK_INSTANCE_COUNT_BITPOS_LO, DF__BLOCK_INSTANCE_COUNT_BITPOS_HI, getDfRegFabricBlkInstanceCnt(nodeId));
  for (i=0; i<numDFInstances; i++)
  {
    fabricBlockInstanceInformation0Reg = getDfRegFabricBlkInstInfo0(dfType, i, nodeId);
    // Skip gated blocks (detected because at least one bit must be non-zero in non-gated blocks)
    if (fabricBlockInstanceInformation0Reg == 0)
    {
      continue;
    }
    if (getBits(DF__INSTANCE_TYPE_BITPOS_LO, DF__INSTANCE_TYPE_BITPOS_HI, fabricBlockInstanceInformation0Reg) == DF__CCM_INSTANCE_TYPE_VALUE)
    {
      // IDS_HDT_CONSOLE (MAIN_FLOW, "ccmInstanceId=%d\n", i);
      return (i);
    }
    if ((getBits(DF__INSTANCE_TYPE_BITPOS_LO, DF__INSTANCE_TYPE_BITPOS_HI, fabricBlockInstanceInformation0Reg) == DF__GCM_INSTANCE_TYPE_VALUE)
         && firstGcmFabricId < 0)
    {
      firstGcmFabricId = i;
    }
  }

  // If a CCM wasn't found, then this must be a dGPU. Return the GCM found.
  // All parts have at least one CCM or one GCM.
  if (firstGcmFabricId >= 0)
  {
    // IDS_HDT_CONSOLE (MAIN_FLOW, "gcmInstanceId=%d\n", firstGcmFabricId);
    return (firstGcmFabricId);
  }

  // No moderator found!!!
  ASSERT (FALSE);
  return 0;
}

/*------------------------------------------------------------------
 Function: findMapRegBySysAddr
 Purpose: Helper function to find a map register for a given system address
          This should only be used for DF3.5 heterogeneous systems,
          it is a shortcut to trying to read the dGPU node.
 Inputs:  dfType, nodeID and address
 Outputs: The adddress map number to which this address assigned
          This address map will be on the first CCM (found by findModeratorInstanceId)
 *------------------------------------------------------------------*/
UINT32
findMapRegBySysAddr (
  UINT32 dfType,
  UINT32 nodeId,
  UINT64 sysAddr
  )
{
  UINT32 ccmInstanceId;
  UINT32 mapRegNumber;
  UINT32 dramAddressMapRegs[ADDR_MAP_ARRAYSIZE];
  UINT64 dramBaseAddr, dramLimitAddr;

  ASSERT (dfType == DF_TYPE_DF3POINT5); // only use for heterogeneous systems!

  // Find a CCM instance ID
  ccmInstanceId = findModeratorInstanceId(dfType, nodeId);

  for (mapRegNumber=0; mapRegNumber<getNumAddressMaps(dfType); mapRegNumber++)
  {
    getDramAddressMap(dfType, ccmInstanceId, nodeId, mapRegNumber, dramAddressMapRegs);
    if (extractDramAddrRangeValid(dfType, dramAddressMapRegs) == 0)
    {
      continue;
    }
    dramBaseAddr = extractDramBaseAddr(dfType, dramAddressMapRegs);
    dramLimitAddr = extractDramLimitAddr(dfType, dramAddressMapRegs);

    // Check if we hit in this address map (address within base and limit)
    if ((sysAddr >= dramBaseAddr) && (sysAddr <= dramLimitAddr))
    {
      return (mapRegNumber);
    }
  }

  // missed in address maps.
  IDS_HDT_CONSOLE (MAIN_FLOW, "findMapRegBySysAddr: SysAddr 0x%016lX missed DRAM maps\n",
          sysAddr);

  ASSERT (FALSE);
  return 0;
}

/*------------------------------------------------------------------
 Function: findMapRegByDstFabricId
 Purpose: Helper function to find a map register for a given DstFabricId
          This should only be used for DF3.5 heterogeneous systems,
          it is a shortcut to trying to read the dGPU node.
 Inputs:  dfType, nodeID and address
 Outputs: The adddress map number to which this address assigned
          This address map will be on the first CCM (found by findModeratorInstanceId)
 *------------------------------------------------------------------*/
UINT32
findMapRegByDstFabricId (
  UINT32 dfType,
  UINT32 nodeId,
  UINT32 dstFabricId
  )
{
  UINT32 ccmInstanceId;
  UINT32 mapRegNumber;
  UINT32 nodeIdMask;
  UINT32 dramAddressMapRegs[ADDR_MAP_ARRAYSIZE];

  ASSERT (dfType == DF_TYPE_DF3POINT5); // only use for heterogeneous systems!

  // Find a CCM instance ID
  ccmInstanceId = findModeratorInstanceId(dfType, nodeId);
  nodeIdMask = getNodeIdMask(dfType);

  for (mapRegNumber=0; mapRegNumber<getNumAddressMaps(dfType); mapRegNumber++)
  {
    getDramAddressMap(dfType, ccmInstanceId, nodeId, mapRegNumber, dramAddressMapRegs);
    if (extractDramAddrRangeValid(dfType, dramAddressMapRegs) == 0)
    {
      continue;
    }

    //Compare just the nodeIDs of the dstFabricId
    if ((extractDstFabricId(dfType, dramAddressMapRegs) & nodeIdMask) == (dstFabricId & nodeIdMask))
    {
      return (mapRegNumber);
    }
  }

  // missed in address maps.
  IDS_HDT_CONSOLE (MAIN_FLOW, "findMapRegByDstFabricId: DstFabricId 0x%x missed DRAM maps\n",
          dstFabricId);

  ASSERT (FALSE);
  return 0;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Convert socket/die/UMC number to the Nth UMC in the system (the same as Nth channel in the system,
 * since one channel per UMC on ZP)
 * The input parameter umc_chan_num is fixed to 0 on ZP
 *
 * @param[in] pkg_no            Socket ID (0..1)
 * @param[in] mpu_no            Die ID (0..3)
 * @param[in] umc_inst_num      UMC ID (0..1)
 * @param[in] umc_chan_num      always = 0 in ZP
 * @retval                      Nth channel in the system
 *----------------------------------------------------------------------------------------*/
UINTN
convert_to_addr_trans_index (
  UINTN  pkg_no,
  UINTN  mpu_no,
  UINTN  umc_inst_num,
  UINTN  umc_chan_num
  )
{
  UINTN    U_CH;
  UINTN    M_U_CH;

  U_CH = ((CHANNEL_PER_UMC * umc_inst_num) + umc_chan_num);
  M_U_CH = ((CHANNEL_PER_UMC * UMC_PER_DIE) * mpu_no) + U_CH;

  return (((CHANNEL_PER_UMC * UMC_PER_DIE * DIE_PER_SOCKET) * pkg_no) + M_U_CH);
}

BOOLEAN
internal_bit_wise_xor (
  UINT32  inp
  )
{
  BOOLEAN   t;
  UINT32    i;

  t = 0;
  for (i = 0; i < 32; i++) {
    t = t ^ ((inp >> i) & 0x1);
  }

  return t;
}

UINT32
smnRegRead (
  UINTN   socket,
  UINTN   die,
  UINTN   umc,
  UINTN   ch,
  UINTN   offset,
  UINTN   BusNumberBase
  )
{
  UINTN   pciAddress;
  UINT32  smnIndex;
  UINT32  value;
  UINT32  umcSmnBase;

  pciAddress = ((UINTN)BusNumberBase << 20) + IOHC_NB_SMN_INDEX_2_BIOS;
  umcSmnBase = ((UINT32) umc) << 20;
  smnIndex = umcSmnBase + 0x50000 + (UINT32)offset;
  PciWrite32 (pciAddress, smnIndex);
  pciAddress = ((UINTN)BusNumberBase << 20) + IOHC_NB_SMN_DATA_2_BIOS;
  value = PciRead32 (pciAddress);
  return value;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Read related register and save to global variable
 *
 * @param[in] pkgnoe            Die ID
 * @param[in] mpuno             Channel ID
 * @param[in] umcno             UMC ID
 * @param[in] umcchno           always = 0 in ZP
 * @param[in] BusNumberBase     PCI bus number
 * @retval    VOID
 *----------------------------------------------------------------------------------------*/
VOID
retrieve_regs (
  UINTN   pkgno,
  UINTN   mpuno,
  UINTN   umcno,
  UINTN   umcchno,
  UINTN   BusNumberBase
  )
{

  LOC = convert_to_addr_trans_index (pkgno, mpuno, umcno, umcchno);

  // UMC0CHx00000000 [DRAM CS Base Address] (BaseAddr), ch0_cs[3:0]_aliasSMN; UMC0CHx0000_000[[C,8,4,0]];
  // [31:1]BaseAddr: Base Address [39:9]
  gAddrData->CSBASE[LOC][0] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x0, BusNumberBase) >> 1) << 1);
  gAddrData->CSBASE[LOC][1] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x4, BusNumberBase) >> 1) << 1);
  gAddrData->CSBASE[LOC][2] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x8, BusNumberBase) >> 1) << 1);
  gAddrData->CSBASE[LOC][3] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0xC, BusNumberBase) >> 1) << 1);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t\tBaseAddr-x00\t\t Channel %x cs0 = %08x cs1 = %08x cs2 = %08x cs3 = %08x\n",
         LOC, gAddrData->CSBASE[LOC][0], gAddrData->CSBASE[LOC][1], gAddrData->CSBASE[LOC][2], gAddrData->CSBASE[LOC][3]);

  gAddrData->EXT_CSBASE[LOC][0] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB00, BusNumberBase));
  gAddrData->EXT_CSBASE[LOC][1] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB04, BusNumberBase));
  gAddrData->EXT_CSBASE[LOC][2] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB08, BusNumberBase));
  gAddrData->EXT_CSBASE[LOC][3] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB0C, BusNumberBase));

  // UMC0CHx00000020 [DRAM CS Mask Address] (AddrMask), ch0_dimm[1:0]_aliasSMN; UMC0CHx0000_002[4,0];
  // [31:1]AddrMask: Address Mask [39:9]
  gAddrData->CSMASK[LOC][0] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x20, BusNumberBase)>>1)<<1) | 0x1;
  gAddrData->CSMASK[LOC][1] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x24, BusNumberBase)>>1)<<1) | 0x1;
  gAddrData->CSMASK[LOC][2] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x28, BusNumberBase)>>1)<<1) | 0x1;
  gAddrData->CSMASK[LOC][3] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x2c, BusNumberBase)>>1)<<1) | 0x1;
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t\tAddrMask-x20\t\tChannel %x Dimm0 = %08x Dimm1 = %08x Dimm2 = %08x Dimm3 = %08x\n",
                   LOC, gAddrData->CSMASK[LOC][0], gAddrData->CSMASK[LOC][1], gAddrData->CSMASK[LOC][2], gAddrData->CSMASK[LOC][3]);

  gAddrData->EXT_CSMASK[LOC][0] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB20, BusNumberBase));
  gAddrData->EXT_CSMASK[LOC][1] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB24, BusNumberBase));
  gAddrData->EXT_CSMASK[LOC][2] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB28, BusNumberBase));
  gAddrData->EXT_CSMASK[LOC][3] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB2c, BusNumberBase));

  // UMC0CHx00000010 [DRAM CS Base Secondary Address] (BaseAddrSec), ch0_cs[3:0]_aliasSMN; UMC0CHx0000_001[[C,8,4,0]];
  // [31:1]BaseAddr: Base Address [39:9]
  gAddrData->CSBASESEC[LOC][0] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x10, BusNumberBase) >> 1) << 1);
  gAddrData->CSBASESEC[LOC][1] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x14, BusNumberBase) >> 1) << 1);
  gAddrData->CSBASESEC[LOC][2] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x18, BusNumberBase) >> 1) << 1);
  gAddrData->CSBASESEC[LOC][3] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x1C, BusNumberBase) >> 1) << 1);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t\tBaseAddrSec-x10\t\tChannel %x cs0 = %08x cs1 = %08x cs2 = %08x cs3 = %08x\n",
                   LOC, gAddrData->CSBASESEC[LOC][0], gAddrData->CSBASESEC[LOC][1], gAddrData->CSBASESEC[LOC][2], gAddrData->CSBASESEC[LOC][3]);

  gAddrData->EXT_CSBASESEC[LOC][0] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB10, BusNumberBase));
  gAddrData->EXT_CSBASESEC[LOC][1] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB14, BusNumberBase));
  gAddrData->EXT_CSBASESEC[LOC][2] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB18, BusNumberBase));
  gAddrData->EXT_CSBASESEC[LOC][3] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB1C, BusNumberBase));

  //UMC00CHx00000030...UMC11CHx0000003C [DRAM CS Mask Secondary Address] (UMC::AddrMaskSec)
  gAddrData->CSMASKSEC[LOC][0] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x30, BusNumberBase)>>1)<<1)| 0x1;
  gAddrData->CSMASKSEC[LOC][1] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x34, BusNumberBase)>>1)<<1)| 0x1;
  gAddrData->CSMASKSEC[LOC][2] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x38, BusNumberBase)>>1)<<1)| 0x1;
  gAddrData->CSMASKSEC[LOC][3] = ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x3c, BusNumberBase)>>1)<<1)| 0x1;
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t\tAddrMaskSec-x30\t\tChannel %x Dimm0 = %08x Dimm1 = %08x Dimm2 = %08x Dimm3 = %08x\n",
                       LOC, gAddrData->CSMASKSEC[LOC][0], gAddrData->CSMASKSEC[LOC][1], gAddrData->CSMASKSEC[LOC][2], gAddrData->CSMASKSEC[LOC][3]);

  gAddrData->EXT_CSMASKSEC[LOC][0] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB30, BusNumberBase));
  gAddrData->EXT_CSMASKSEC[LOC][1] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB34, BusNumberBase));
  gAddrData->EXT_CSMASKSEC[LOC][2] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB38, BusNumberBase));
  gAddrData->EXT_CSMASKSEC[LOC][3] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB3c, BusNumberBase));

  //ctrlreg[0]=is_3wayintlv
  //ctrlreg[2]= 0->1+2 or 1->2+1
  //they need to be organically derived
  //if(addrmaskcs0=1=2=secmask0=1=2 || addrmaskcs0=2=3=sec0=2=3)ctrlreg[0]=1 if(csenable3==0 then [1]=1 else 0) else 0
  if(((gAddrData->CSMASK[LOC][0]==gAddrData->CSMASK[LOC][1]) && (gAddrData->CSMASK[LOC][1]==gAddrData->CSMASK[LOC][2]) &&
      (gAddrData->CSMASK[LOC][1]==gAddrData->CSMASKSEC[LOC][0]) && (gAddrData->CSMASKSEC[LOC][0]==gAddrData->CSMASKSEC[LOC][1]) &&
      (gAddrData->CSMASKSEC[LOC][1]==gAddrData->CSMASKSEC[LOC][2]) && (((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x0, BusNumberBase)) & 0x1)) &&
      ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x4, BusNumberBase)) & 0x1) && ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x8, BusNumberBase)) & 0x1) &&
      ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x10, BusNumberBase)) & 0x1) && ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x14, BusNumberBase)) & 0x1) &&
      ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x18, BusNumberBase)) & 0x1)) ||
     ((gAddrData->CSMASK[LOC][0]==gAddrData->CSMASK[LOC][3]) && (gAddrData->CSMASK[LOC][3]==gAddrData->CSMASK[LOC][2]) &&
     (gAddrData->CSMASK[LOC][3]==gAddrData->CSMASKSEC[LOC][0]) && (gAddrData->CSMASKSEC[LOC][0]==gAddrData->CSMASKSEC[LOC][3]) &&
     (gAddrData->CSMASKSEC[LOC][3]==gAddrData->CSMASKSEC[LOC][2]) && (((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x0, BusNumberBase)) & 0x1)) &&
     ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0xc, BusNumberBase)) & 0x1) && ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x8, BusNumberBase)) & 0x1) &&
     ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x10, BusNumberBase)) & 0x1) && ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x1c, BusNumberBase)) & 0x1) &&
     ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x18, BusNumberBase)) & 0x1) )
    ){
      three_way_cs=1;

      if(((smnRegRead(pkgno, mpuno, umcno, umcchno, 0xc, BusNumberBase)) & 0x1)==0)
        two_p_one=1;
      else
        two_p_one=0;
  }
  gAddrData->CTRLREG[LOC][0] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x11C, BusNumberBase)>>3) & 0x1)<<1) | ( three_way_cs & 0x1) | (((two_p_one) & 0x1)<<2);
  gAddrData->CTRLREG[LOC][1] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x11C, BusNumberBase)>>3) & 0x1)<<1) | ( three_way_cs & 0x1) | (((two_p_one) & 0x1)<<2);
  gAddrData->CTRLREG[LOC][2] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x11C, BusNumberBase)>>3) & 0x1)<<1) | ( three_way_cs & 0x1) | (((two_p_one) & 0x1)<<2);
  gAddrData->CTRLREG[LOC][3] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x11C, BusNumberBase)>>3) & 0x1)<<1) | ( three_way_cs & 0x1) | (((two_p_one) & 0x1)<<2);

  //UMC00CHx00000010...UMC11CHx0000001C [DRAM CS Base Secondary Address] (UMC::BaseAddrSec)
  //UMC00CHx00000040...UMC11CHx0000004C [DRAM Address Configuration] (UMC::AddrCfg)
  //UMC00CHx00000000...UMC11CHx0000000C [DRAM CS Base Address] (UMC::BaseAddr)
  gAddrData->CONFIGDIMM[LOC][0] =
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x10, BusNumberBase))>>0) & 0x1)<<24) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x40, BusNumberBase))>>2) & 0x3)<<20) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x40, BusNumberBase))>>16) & 0xf)<<16) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x40, BusNumberBase))>>6) & 0x1)<<12) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x40, BusNumberBase))>>8) & 0xf)<<8) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x40, BusNumberBase))>>4) & 0x3)<<6) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x40, BusNumberBase))>>20) & 0x3)<<4) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x40, BusNumberBase)>>30) & 0x3)<<1) |
    ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x0, BusNumberBase)) & 0x1);
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t\tAddrCfg-x40\t\t Channel %x Dimm0 = %08x ([25:24]BaseAddrSec CSEnable, [1:0]BaseAddr CSEnable)\n",
                   LOC, gAddrData->CONFIGDIMM[LOC][0]);

  gAddrData->CONFIGDIMM[LOC][1] =
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x14, BusNumberBase))>>0) & 0x1)<<24) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x44, BusNumberBase))>>2) & 0x3)<<20) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x44, BusNumberBase))>>16) & 0xf)<<16) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x44, BusNumberBase))>>6) & 0x1)<<12) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x44, BusNumberBase))>>8) & 0xf)<<8) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x44, BusNumberBase))>>4) & 0x3)<<6) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x44, BusNumberBase))>>20) & 0x3)<<4) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x44, BusNumberBase)>>30) & 0x3)<<1) |
    ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x4, BusNumberBase)) & 0x1);
  gAddrData->CONFIGDIMM[LOC][2] =
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x18, BusNumberBase))>>0) & 0x1)<<24) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x48, BusNumberBase))>>2) & 0x3)<<20) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x48, BusNumberBase))>>16) & 0xf)<<16) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x48, BusNumberBase))>>6) & 0x1)<<12) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x48, BusNumberBase))>>8) & 0xf)<<8) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x48, BusNumberBase))>>4) & 0x3)<<6) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x48, BusNumberBase))>>20) & 0x3)<<4) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x48, BusNumberBase)>>30) & 0x3)<<1) |
    ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x8, BusNumberBase)) & 0x1);
  gAddrData->CONFIGDIMM[LOC][3] =
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x1c, BusNumberBase))>>0) & 0x1)<<24) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x4c, BusNumberBase))>>2) & 0x3)<<20) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x4c, BusNumberBase))>>16) & 0xf)<<16) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x4c, BusNumberBase))>>6) & 0x1)<<12) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x4c, BusNumberBase))>>8) & 0xf)<<8) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x4c, BusNumberBase))>>4) & 0x3)<<6) |
    ((((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x4c, BusNumberBase))>>20) & 0x3)<<4) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x4c, BusNumberBase)>>30) & 0x3)<<1) |
    ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0xc, BusNumberBase)) & 0x1);

  //UMC00CHx00000050...UMC11CHx0000005C [DRAM Bank Address Select] (UMC::AddrSel)
  // [19:16]BankBit4
  // [15:12]BankBit3
  // [11:8]BankBit2
  // [7:4]BankBit1
  // [3:0]BankBit0
  gAddrData->BANKSELDIMM[LOC][0] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x50, BusNumberBase)>>16) & 0xf ) <<16) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x50, BusNumberBase)>>12) & 0xf ) <<12) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x50, BusNumberBase)>>8) & 0xf ) <<8) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x50, BusNumberBase)>>4) & 0xf ) <<4) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x50, BusNumberBase)) & 0xf ) >>0) ;
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t\tAddrSel-x50\t\tChannel %x BankBit Dimm0 = %08x Dimm1 = %08x Dimm2 = %08x Dimm3 = %08x\n",
                   LOC, gAddrData->BANKSELDIMM[LOC][0], gAddrData->BANKSELDIMM[LOC][1], gAddrData->BANKSELDIMM[LOC][2], gAddrData->BANKSELDIMM[LOC][3]);

  gAddrData->BANKSELDIMM[LOC][1] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x54, BusNumberBase)>>16) & 0xf ) <<16) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x54, BusNumberBase)>>12) & 0xf ) <<12) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x54, BusNumberBase)>>8) & 0xf ) <<8) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x54, BusNumberBase)>>4) & 0xf ) <<4) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x54, BusNumberBase)) & 0xf ) >>0) ;
  gAddrData->BANKSELDIMM[LOC][2] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x58, BusNumberBase)>>16) & 0xf ) <<16) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x58, BusNumberBase)>>12) & 0xf ) <<12) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x58, BusNumberBase)>>8) & 0xf ) <<8) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x58, BusNumberBase)>>4) & 0xf ) <<4) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x58, BusNumberBase)) & 0xf ) >>0) ;
  gAddrData->BANKSELDIMM[LOC][3] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x5c, BusNumberBase)>>16) & 0xf ) <<16) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x5c, BusNumberBase)>>12) & 0xf ) <<12) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x5c, BusNumberBase)>>8) & 0xf ) <<8) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x5c, BusNumberBase)>>4) & 0xf ) <<4) |
    (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x5c, BusNumberBase)) & 0xf ) >>0) ;

  //UMC00CHx00000050...UMC11CHx0000005C [DRAM Bank Address Select] (UMC::AddrSel)
  // [27:24]Row
  gAddrData->ROWSELDIMM[LOC][0] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x50, BusNumberBase)>>24) & 0xff ));
  gAddrData->ROWSELDIMM[LOC][1] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x54, BusNumberBase)>>24) & 0xff ));
  gAddrData->ROWSELDIMM[LOC][2] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x58, BusNumberBase)>>24) & 0xff ));
  gAddrData->ROWSELDIMM[LOC][3] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x5c, BusNumberBase)>>24) & 0xff ));
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t\tAddrSel-x50\t\t Channel %x RowHiLo Dimm0 = %08x Dimm1 = %08x Dimm2 = %08x Dimm3 = %08x\n",
                  LOC, gAddrData->ROWSELDIMM[LOC][0], gAddrData->ROWSELDIMM[LOC][1], gAddrData->ROWSELDIMM[LOC][2], gAddrData->ROWSELDIMM[LOC][3]);

  //UMC00CHx00000060...UMC11CHx00000078 [DRAM Column Address Select Low] (UMC::ColSelLo)
  gAddrData->COL0SELDIMM[LOC][0] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0x60, BusNumberBase));
  gAddrData->COL0SELDIMM[LOC][1] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0x68, BusNumberBase));
  gAddrData->COL0SELDIMM[LOC][2] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0x70, BusNumberBase));
  gAddrData->COL0SELDIMM[LOC][3] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0x78, BusNumberBase));
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t\tColSelLo-x60\t\t Channel %x Dimm0 = %08x Dimm1 = %08x Dimm2 = %08x Dimm3 = %08x\n",
                   LOC, gAddrData->COL0SELDIMM[LOC][0], gAddrData->COL0SELDIMM[LOC][1], gAddrData->COL0SELDIMM[LOC][2], gAddrData->COL0SELDIMM[LOC][3]);

  //UMC00CHx00000064...UMC11CHx0000007C [DRAM Column Address Select High] (UMC::ColSelHi)
  gAddrData->COL1SELDIMM[LOC][0] =  (smnRegRead (pkgno, mpuno, umcno, umcchno, 0x64, BusNumberBase));
  gAddrData->COL1SELDIMM[LOC][1] =  (smnRegRead (pkgno, mpuno, umcno, umcchno, 0x6C, BusNumberBase));
  gAddrData->COL1SELDIMM[LOC][2] =  (smnRegRead (pkgno, mpuno, umcno, umcchno, 0x74, BusNumberBase));
  gAddrData->COL1SELDIMM[LOC][3] =  (smnRegRead (pkgno, mpuno, umcno, umcchno, 0x7c, BusNumberBase));
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t\tColSelHi-x64\t\t Channel %x Dimm0 = %08x Dimm1 = %08x Dimm2 = %08x Dimm3 = %08x\n",
                   LOC, gAddrData->COL1SELDIMM[LOC][0], gAddrData->COL1SELDIMM[LOC][1], gAddrData->COL1SELDIMM[LOC][2], gAddrData->COL1SELDIMM[LOC][3]);

  //UMC00CHx00000080...UMC11CHx0000008C [DRAM Rank Multiply Address Select] (UMC::RmSel)
  gAddrData->RMSELDIMM[LOC][0] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0x80, BusNumberBase));
  gAddrData->RMSELDIMM[LOC][1] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0x84, BusNumberBase));
  gAddrData->RMSELDIMM[LOC][2] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0x88, BusNumberBase));
  gAddrData->RMSELDIMM[LOC][3] = (smnRegRead (pkgno, mpuno, umcno, umcchno, 0x8c, BusNumberBase));
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t\tRmSel-x80\t\tChannel %x Dimm0 = %08x Dimm1 = %08x Dimm2 = %08x Dimm3 = %08x\n",
                   LOC, gAddrData->RMSELDIMM[LOC][0], gAddrData->RMSELDIMM[LOC][1], gAddrData->RMSELDIMM[LOC][2], gAddrData->RMSELDIMM[LOC][3]);

  gAddrData->ADDRHASHBANK0[LOC] = smnRegRead (pkgno, mpuno, umcno, umcchno, 0x98, BusNumberBase);
  gAddrData->ADDRHASHBANK1[LOC] = smnRegRead (pkgno, mpuno, umcno, umcchno, 0x9C, BusNumberBase);
  gAddrData->ADDRHASHBANK2[LOC] = smnRegRead (pkgno, mpuno, umcno, umcchno, 0xA0, BusNumberBase);
  gAddrData->ADDRHASHBANK3[LOC] = smnRegRead (pkgno, mpuno, umcno, umcchno, 0xA4, BusNumberBase);
  gAddrData->ADDRHASHBANK4[LOC] = smnRegRead (pkgno, mpuno, umcno, umcchno, 0xA8, BusNumberBase);

  gAddrData->ADDRHASHNORMADDR[LOC][0] = smnRegRead (pkgno, mpuno, umcno, umcchno, 0xC8, BusNumberBase);
  gAddrData->ADDRHASHNORMADDR[LOC][1] = smnRegRead (pkgno, mpuno, umcno, umcchno, 0xCC, BusNumberBase);
  gAddrData->ADDRHASHNORMADDR[LOC][2] = 0;

  gAddrData->EXT_ADDRHASHNORMADDR[LOC][0] = (UINT8) smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB98, BusNumberBase);
  gAddrData->EXT_ADDRHASHNORMADDR[LOC][1] = (UINT8) smnRegRead (pkgno, mpuno, umcno, umcchno, 0xBCC, BusNumberBase);
  gAddrData->EXT_ADDRHASHNORMADDR[LOC][2] = 0;

  gAddrData->ADDRHASHRMADDR[LOC][0]= smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB0, BusNumberBase);
  gAddrData->ADDRHASHRMADDR[LOC][1]= smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB4, BusNumberBase);
  gAddrData->ADDRHASHRMADDR[LOC][2]= smnRegRead (pkgno, mpuno, umcno, umcchno, 0xB8, BusNumberBase);

  gAddrData->EXT_ADDRHASHRMADDR[LOC][0]= (UINT8) smnRegRead (pkgno, mpuno, umcno, umcchno, 0xBB4, BusNumberBase);
  gAddrData->EXT_ADDRHASHRMADDR[LOC][1]= (UINT8) smnRegRead (pkgno, mpuno, umcno, umcchno, 0xBB8, BusNumberBase);
  gAddrData->EXT_ADDRHASHRMADDR[LOC][2]= (UINT8) smnRegRead (pkgno, mpuno, umcno, umcchno, 0xBBC, BusNumberBase);

  if(gAddrData->CTRLREG[LOC][0]>>1 & 0x1){
    gAddrData->ADDRHASHPC[LOC]= smnRegRead (pkgno, mpuno, umcno, umcchno, 0xc0, BusNumberBase);
    gAddrData->ADDRHASHPC2[LOC]= smnRegRead (pkgno, mpuno, umcno, umcchno, 0xc4, BusNumberBase);
  }

  gAddrData->TOTAL_NUM_RANKS_PER_UMCCH_ADDR_TRANS[LOC] = (((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x4, BusNumberBase)) & 0x1)) +
                                                   ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x0, BusNumberBase)) & 0x1) +
                                                   ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x8, BusNumberBase)) & 0x1) +
                                                   ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0xC, BusNumberBase)) & 0x1) ;

  gAddrData->RANK_ENABLE_PER_UMCCH_ADDR_TRANS[LOC] =  ((((smnRegRead(pkgno, mpuno, umcno, umcchno, 0xC, BusNumberBase)) & 0x1))<<3)|
                                               ((((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x8, BusNumberBase)) & 0x1)) <<2)|
                                               ((((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x4, BusNumberBase)) & 0x1))<<1)|
                                                 ((smnRegRead(pkgno, mpuno, umcno, umcchno, 0x0, BusNumberBase)) & 0x1);


  //UMC00CHx00000090...UMC11CHx00000094 [DIMM Configuration] (UMC::DimmCfg)
  //[4] - This bit specifies if a RDIMM is populated on a given UMC channel
  gAddrData->DimmPresent[pkgno][umcno] = (((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x90, BusNumberBase) & BIT4) != 0) ||
                                          ((smnRegRead (pkgno, mpuno, umcno, umcchno, 0x94, BusNumberBase) & BIT4) != 0)) ? TRUE : FALSE;

}

/*----------------------------------------------------------------------------------------*/
/**
 * this function returns the (number of "1"s in inp)+8. mainly used to count it in CSMask
 *----------------------------------------------------------------------------------------*/
UINT32
popcnt (
  UINT32  inp
  )
{
  UINT32    ans=0, i=0;

  for (i = 0; i < 32; i++) {
    if ((inp >> i) & 0x1) {
      ans ++;
    }
  }
  return (ans + 8);
}

UINT32
popcnt64 (
  UINT64  inp
  )
{
  UINT32    ans=0, i=0;

  for (i = 0; i < 64; i++) {
    if ((inp >> i) & 0x1) {
      ans ++;
    }
  }
  return (ans + 8);
}

/*----------------------------------------------------------------------------------------*/
/**
 *  Function returns a Bank, Row, and Col as seen on the DRAM Command/Addr bus.
 *----------------------------------------------------------------------------------------*/
VOID
NormalizedToBankAddrMap(
  UINT64 ChannelAddr,
  UINT8 *Bank,
  UINT32 *Row,
  UINT16 *Col,
  UINT8 *Rankmul,
  UINT8 *Subchan,
  UINT8 numbankbits,
  UINT8 bank4,
  UINT8 bank3,
  UINT8 bank2,
  UINT8 bank1,
  UINT8 bank0,
  UINT8 numrowlobits,
  UINT8 numcolbits,
  UINT8 row_lo0,
  UINT32 COL0REG,
  UINT32 COL1REG,
  UINT8 numcsbits,
  UINT8 rm0,
  UINT8 rm1,
  UINT8 rm2,
  UINT8 chan,
  UINT8 invertmsbse,
  UINT8 invertmsbso,
  UINT8 SEC,
  UINT8 cs,
  UINT32 addrhashbank0,
  UINT32 addrhashbank1,
  UINT32 addrhashbank2,
  UINT32 addrhashbank3,
  UINT32 addrhashbank4,
  UINT32 addrhashpc,
  UINT32 addrhashpc2,
  UINT8 vcmen
  )
{

  if(SEC==3) {
    //IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: SEC value cannot be 3\n");
    ASSERT (FALSE);
  }

  //if addrhash is enabled, we can get bank after gettting row and col
  if(((addrhashbank0 & 0x1)==0) && ((addrhashbank1 & 0x1)==0) && ((addrhashbank2 & 0x1)==0) &&
      ((addrhashbank3 & 0x1)==0) && ((addrhashbank4 & 0x1)==0)) {
    *Bank = (numbankbits ==3) ? (((ChannelAddr >> (bank0 + 5)) &0x1) |
                                (((ChannelAddr >> (bank1 + 5)) &0x1)<<1) |
                                (((ChannelAddr >> (bank2 + 5)) &0x1)<<2)) :
            (numbankbits ==4) ? (((ChannelAddr >> (bank0 + 5)) &0x1) | (((ChannelAddr >> (bank1 + 5)) &0x1)<<1) |
                                (((ChannelAddr >> (bank2 + 5)) &0x1)<<2) | (((ChannelAddr >> (bank3 + 5)) &0x1)<<3) ) :
                                (((ChannelAddr >> (bank0 + 5)) &0x1) | (((ChannelAddr >> (bank1 + 5)) &0x1)<<1) |
                                (((ChannelAddr >> (bank2 + 5)) &0x1)<<2) | (((ChannelAddr >> (bank3 + 5)) &0x1)<<3) |
                                (((ChannelAddr >> (bank4 + 5)) &0x1)<<4)) ;

  }//IF addr hash is disabled

  if((numcolbits >= 5) && (numcolbits <= 8)) {
    *Col = ( (ChannelAddr >> (((COL0REG >> 0) & 0xF) + 2)) & 0x1 ) | (( (ChannelAddr >> (((COL0REG >> 4) & 0xF) + 2)) & 0x1 ) << 1) |
               (( (ChannelAddr >> (((COL0REG >> 8) & 0xF) + 2)) & 0x1 ) << 2) | (( (ChannelAddr >> (((COL0REG >> 12) & 0xF) + 2)) & 0x1 ) << 3) |
               (( (ChannelAddr >> (((COL0REG >> 16) & 0xF) + 2)) & 0x1 ) << 4) | (( (ChannelAddr >> (((COL0REG >> 20) & 0xF) + 2)) & 0x1 ) << 5) |
               (( (ChannelAddr >> (((COL0REG >> 24) & 0xF) + 2)) & 0x1 ) << 6) | (( (ChannelAddr >> (((COL0REG >> 28) & 0xF) + 2)) & 0x1 ) << 7);
   } else {
             *Col = ( (ChannelAddr >> (((COL0REG >> 0) & 0xF) + 2)) & 0x1 ) | (( (ChannelAddr >> (((COL0REG >> 4) & 0xF) + 2)) & 0x1 ) << 1) |
               (( (ChannelAddr >> (((COL0REG >> 8) & 0xF) + 2)) & 0x1 ) << 2) | (( (ChannelAddr >> (((COL0REG >> 12) & 0xF) + 2)) & 0x1 ) << 3) |
               (( (ChannelAddr >> (((COL0REG >> 16) & 0xF) + 2)) & 0x1 ) << 4) | (( (ChannelAddr >> (((COL0REG >> 20) & 0xF) + 2)) & 0x1 ) << 5) |
               (( (ChannelAddr >> (((COL0REG >> 24) & 0xF) + 2)) & 0x1 ) << 6) | (( (ChannelAddr >> (((COL0REG >> 28) & 0xF) + 2)) & 0x1 ) << 7) |

               ( ((ChannelAddr >> (((COL1REG >> 0) & 0xF) + 8)) & 0x1) << 8 ) | (( (ChannelAddr >> (((COL1REG >> 4) & 0xF) + 8)) & 0x1 ) << 9) |
               (( (ChannelAddr >> (((COL1REG >> 8) & 0xF) + 8)) & 0x1 ) << 10) | (( (ChannelAddr >> (((COL1REG >> 12) & 0xF) + 8)) & 0x1 ) << 11) |
               (( (ChannelAddr >> (((COL1REG >> 16) & 0xF) + 8)) & 0x1 ) << 12) | (( (ChannelAddr >> (((COL1REG >> 20) & 0xF) + 8)) & 0x1 ) << 13) |
               (( (ChannelAddr >> (((COL1REG >> 24) & 0xF) + 8)) & 0x1 ) << 14) | (( (ChannelAddr >> (((COL1REG >> 28) & 0xF) + 8)) & 0x1 ) << 15);

  }
  *Col = *Col & (((UINT16)pow_ras(2,(UINTN)numcolbits))-1);

  //will work for regular dimm, non power of 2 dimms are handled later
  *Row = (UINT32)(((ChannelAddr >> (row_lo0+12)) & ((UINT64)pow_ras(2,(UINTN)numrowlobits)-1)));

  if(SEC!=2) {
    *Row = ((((*Row>>(numrowlobits-1)) & 0x1 ) ^ (SEC ? (((invertmsbso>>1) & 0x1)) : (((invertmsbse>>1) & 0x1))))<<(numrowlobits-1)) |
             ((((*Row>>(numrowlobits-2)) & 0x1 ) ^ (SEC ? (((invertmsbso>>0) & 0x1)) : ( ((invertmsbse>>0) & 0x1))))<<(numrowlobits-2)) |
             (*Row & (((UINT64)pow_ras(2,(UINTN)numrowlobits-2))-1));
  }


  if (((addrhashbank0) & 0x1) || ((addrhashbank1) & 0x1) || ((addrhashbank2) & 0x1) || ((addrhashbank3) & 0x1) || ((addrhashbank4) & 0x1)) {
    *Bank = (numbankbits ==3) ? ((ChannelAddr >> (bank0 + 5) &0x1) |
                                ((ChannelAddr >> (bank1 + 5) &0x1)<<1) |
                                ((ChannelAddr >> (bank2 + 5) &0x1)<<2)) :
            (numbankbits ==4) ? ((ChannelAddr >> (bank0 + 5) &0x1) |
                                ((ChannelAddr >> (bank1 + 5) &0x1)<<1) |
                                ((ChannelAddr >> (bank2 + 5) &0x1)<<2) |
                                ((ChannelAddr >> (bank3 + 5) &0x1)<<3) ) :
                                ((ChannelAddr >> (bank0 + 5) &0x1) |
                                ((ChannelAddr >> (bank1 + 5) &0x1)<<1) |
                                ((ChannelAddr >> (bank2 + 5) &0x1)<<2) |
                                ((ChannelAddr >> (bank3 + 5) &0x1)<<3) |
                                ((ChannelAddr >> (bank4 + 5) &0x1)<<4)) ;


    gAddrData->addrhash[0] = ((internal_bit_wise_xor(*Col & ((addrhashbank0>>1)&0x1fff)))
                  ^ ( internal_bit_wise_xor(*Row & ((addrhashbank0>>14)&0x3ffff)))) & (addrhashbank0 & 1);
    gAddrData->addrhash[1] = ((internal_bit_wise_xor(*Col & ((addrhashbank1>>1)&0x1fff)))
                  ^ ( internal_bit_wise_xor(*Row & ((addrhashbank1>>14)&0x3ffff)))) & (addrhashbank1 & 1);
    gAddrData->addrhash[2] = ((internal_bit_wise_xor(*Col & ((addrhashbank2>>1)&0x1fff)))
                  ^ ( internal_bit_wise_xor(*Row & ((addrhashbank2>>14)&0x3ffff)))) & (addrhashbank2 & 1);
    gAddrData->addrhash[3] = ((internal_bit_wise_xor(*Col & ((addrhashbank3>>1)&0x1fff)))
                  ^ ( internal_bit_wise_xor(*Row & ((addrhashbank3>>14)&0x3ffff)))) & (addrhashbank3 & 1);
    gAddrData->addrhash[4] = ((internal_bit_wise_xor(*Col & ((addrhashbank4>>1)&0x1fff)))
                  ^ ( internal_bit_wise_xor(*Row & ((addrhashbank4>>14)&0x3ffff)))) & (addrhashbank4 & 1);


    *Bank = (numbankbits ==3) ? ((((addrhashbank0 & 0x1)==1) ? (gAddrData->addrhash[0] ^ (*Bank & 0x1)) : (*Bank & 0x1)) |
                                ((((addrhashbank1 & 0x1)==1) ? (gAddrData->addrhash[1] ^ ((*Bank>>1) & 0x1)) : ((*Bank>>1) & 0x1))<<1) |
                                ((((addrhashbank2 & 0x1)==1) ? (gAddrData->addrhash[2] ^ ((*Bank>>2) & 0x1)) : ((*Bank>>2) & 0x1))<<2)) :
            (numbankbits ==4) ? ((((addrhashbank0 & 0x1)==1) ? (gAddrData->addrhash[0] ^ (*Bank & 0x1)) : (*Bank & 0x1)) |
                                ((((addrhashbank1 & 0x1)==1) ? (gAddrData->addrhash[1] ^ ((*Bank>>1) & 0x1)) : ((*Bank>>1) & 0x1))<<1) |
                                ((((addrhashbank2 & 0x1)==1) ? (gAddrData->addrhash[2] ^ ((*Bank>>2) & 0x1)) : ((*Bank>>2) & 0x1))<<2) |
                                ((((addrhashbank3 & 0x1)==1) ? (gAddrData->addrhash[3] ^ ((*Bank>>3) & 0x1)) : ((*Bank>>3) & 0x1))<<3)) :
                                ((((addrhashbank0 & 0x1)==1) ? (gAddrData->addrhash[0] ^ (*Bank & 0x1)) : (*Bank & 0x1)) |
                                ((((addrhashbank1 & 0x1)==1) ? (gAddrData->addrhash[1] ^ ((*Bank>>1) & 0x1)) : ((*Bank>>1) & 0x1))<<1) |
                                ((((addrhashbank2 & 0x1)==1) ? (gAddrData->addrhash[2] ^ ((*Bank>>2) & 0x1)) : ((*Bank>>2) & 0x1))<<2) |
                                ((((addrhashbank3 & 0x1)==1) ? (gAddrData->addrhash[3] ^ ((*Bank>>3) & 0x1)) : ((*Bank>>3) & 0x1))<<3) |
                                ((((addrhashbank4 & 0x1)==1) ? (gAddrData->addrhash[4] ^ ((*Bank>>4) & 0x1)) : ((*Bank>>4) & 0x1))<<4));

  }

   *Rankmul = numcsbits == 0? 0 : (numcsbits == 1? ((ChannelAddr>>(rm0+12)) &0x1) :
                numcsbits ==2? (((ChannelAddr>>(rm0+12))&0x1) | (((ChannelAddr>>(rm1+12))&0x1)<<1)):(((ChannelAddr>>(rm0+12))&0x1) |
                               (((ChannelAddr>>(rm1+12))&0x1)<<1) | (((ChannelAddr>>(rm2+12))&0x1)<<2)));

  if(vcmen){
    if (((addrhashpc & 0x1)==0))
    {
      *Subchan = (((ChannelAddr >> (chan+5)) & 0x1 ));
    }
    else
    {
       gAddrData->addrhash[5] = ((internal_bit_wise_xor(*Col & ((addrhashpc>>1)&0x1fff))) ^ ( internal_bit_wise_xor(*Row & ((addrhashpc>>14)&0x3ffff))))  & (addrhashpc & 1);
       gAddrData->addrhash[5] = gAddrData->addrhash[5] ^ (internal_bit_wise_xor(((numbankbits ==3) ? ((ChannelAddr >> (bank0 + 5) &0x1) | ((ChannelAddr >> (bank1 + 5) &0x1)<<1) |
         ((ChannelAddr >> (bank2 + 5) &0x1)<<2)) : (numbankbits ==4) ? ((ChannelAddr >> (bank0 + 5) &0x1) | ((ChannelAddr >> (bank1 + 5) &0x1)<<1) | ((ChannelAddr >> (bank2 + 5) &0x1)<<2) |
         ((ChannelAddr >> (bank3 + 5) &0x1)<<3) ) : ((ChannelAddr >> (bank0 + 5) &0x1) | ((ChannelAddr >> (bank1 + 5) &0x1)<<1) | ((ChannelAddr >> (bank2 + 5) &0x1)<<2) |
         ((ChannelAddr >> (bank3 + 5) &0x1)<<3) | ((ChannelAddr >> (bank4 + 5) &0x1)<<4)))  & (addrhashpc2 & 0x1f))) ;
       *Subchan =  (addrhashpc & 0x1 ) ? ((((ChannelAddr >> (chan+5)) & 0x1 )) ^ (gAddrData->addrhash[5])) : (((ChannelAddr >> (chan+5)) & 0x1 ));
    }
  }
  else {
    *Subchan = 0;
  }
}

// Function returns a rebuilt normalized address
UINT64 BankAddrToNormalizedMap (UINT32 CSBase, UINT64 CSBaseExt, UINT32 CSMask, UINT64 CSMaskExt, UINT8 Rank, UINT8 Bank, UINT32 Row, UINT16 Col, UINT8 Rankmul, UINT8 Subchan,
  UINT8 numbankbits, UINT8 bank4, UINT8 bank3, UINT8 bank2, UINT8 bank1, UINT8 bank0, UINT8 numrowlobits,
  UINT8 numcolbits, UINT8 row_lo0, UINT32 COL0REG, UINT32 COL1REG, UINT8 numcsbits,
  UINT8 rm0, UINT8 rm1, UINT8 rm2, UINT8 chan, UINT8 invertmsbse,
  UINT8 invertmsbso,
  UINT64 CSMasksec, UINT64 CSMasksecExt, UINT64 CSBasesec, UINT64 CSBasesecExt, UINT8 SEC, UINT32 addrhashbank0,UINT32 addrhashbank1,UINT32 addrhashbank2,
  UINT32 addrhashbank3,UINT32 addrhashbank4, UINT32 addrhashpc, UINT32 addrhashpc2, UINT8 vcmen ) {

  //Initialize the NA=0
  UINT64 ChannelAddr_G = 0 ;

  ChannelAddr_G = ChannelAddr_G | ((((Col >>0 ) & 0x1 ) << (((COL0REG>>0) &0xf)+2)) |
                                  (((Col >>1 ) & 0x1 ) << (((COL0REG>>4) &0xf)+2)) |
                                  (((Col >>2 ) & 0x1 ) << (((COL0REG>>8) &0xf)+2)) |
                                  (((Col >>3 ) & 0x1 ) << (((COL0REG>>12) &0xf)+2)) |
                                  (((Col >>4 ) & 0x1 ) << (((COL0REG>>16) &0xf)+2)));

  if(numcolbits >= 0x6)
  {
    ChannelAddr_G |= (((Col >>5 ) & 0x1 ) << (((COL0REG>>20) &0xf)+2));
  }
  if(numcolbits >= 0x7)
  {
    ChannelAddr_G |= (((Col >>6 ) & 0x1 ) << (((COL0REG>>24) &0xf)+2));
  }
  if(numcolbits >= 0x8)
  {
    ChannelAddr_G |= (((Col >>7 ) & 0x1 ) << (((COL0REG>>28) &0xf)+2));
  }
  if(numcolbits >= 0x9)
  {
    ChannelAddr_G |= (((Col >>8 ) & 0x1 ) << (((COL1REG>>0) &0xf)+8));
  }
  if(numcolbits >= 0xa)
  {
    ChannelAddr_G |= (((Col >>9 ) & 0x1 ) << (((COL1REG>>4) &0xf)+8));
  }
  if(numcolbits >= 0xb)
  {
    ChannelAddr_G |= (((Col >>10 ) & 0x1 ) << (((COL1REG>>8) &0xf)+8));
  }
  if(numcolbits >= 0xc)
  {
    ChannelAddr_G |= (((Col >>11 ) & 0x1 ) << (((COL1REG>>12) &0xf)+8));
  }
  if(numcolbits >= 0xd)
  {
    ChannelAddr_G |= (((Col >>12 ) & 0x1 ) << (((COL1REG>>16) &0xf)+8));
  }
  if(numcolbits >= 0xe)
  {
    ChannelAddr_G |= (((Col >>13 ) & 0x1 ) << (((COL1REG>>20) &0xf)+8));
  }
  if(numcolbits >= 0xf)
  {
    ChannelAddr_G |= (((Col >>14 ) & 0x1 ) << (((COL1REG>>24) &0xf)+8));
  }
  if(numcolbits >= 0x10)
  {
    ChannelAddr_G |= (((Col >>15 ) & 0x1 ) << (((COL1REG>>28) &0xf)+8));
  }


  //Place the row
  //manipulation
  if(SEC !=2)
  {
    Row = ((((Row>>(numrowlobits-1)) & 0x1 ) ^ (SEC ? (((invertmsbso>>1) & 0x1)) : (((invertmsbse>>1) & 0x1))))<<(numrowlobits-1)) |
            ((((Row>>(numrowlobits-2)) & 0x1 ) ^ (SEC ? (((invertmsbso>>0) & 0x1)) : (((invertmsbse>>0) & 0x1))))<<(numrowlobits-2)) |
            (Row & (((UINT64)pow_ras(2,numrowlobits-2))-1));

    ChannelAddr_G = ChannelAddr_G |  (((UINT64)(Row & ((UINT64)pow_ras(2,numrowlobits)-1)))<<(row_lo0+12)) ;

   //Row has been placed
  }


  //For HBM case, let us use Rankmul as the PC bit. its a one bit value. Let us consider Rankmul[0]
  //For HBM, if chan and bank0 are 1 or 2 apart, then we need to do fill bank first and then PC/VC bit else dont care

  //Place the rm


    ChannelAddr_G = ChannelAddr_G |  (numcsbits==0 ? 0 :
                   (numcsbits==1? (((UINT64)(Rankmul & 0x1))<<(rm0+12)):
                    numcsbits ==2? ((((UINT64)(Rankmul&0x1))<<(rm0+12)) |
                                   (((UINT64)((Rankmul>>1)&0x1))<<(rm1+12))) :
                                   ((((UINT64)(Rankmul&0x1))<<(rm0+12)) |
                                   (((UINT64)((Rankmul>>1)&0x1))<<(rm1+12)) |
                                   (((UINT64)((Rankmul>>2)&0x1))<<(rm2+12))))) ;

   //Let us compute Bank at the very end
   //Reason being, if we are in swizzle case, we want the right bits at other bit positions besides original bank bits
   if(((addrhashbank0 & 0x1)==0) && ((addrhashbank1 & 0x1)==0) &&
       ((addrhashbank2 & 0x1)==0) && ((addrhashbank3 & 0x1)==0) && ((addrhashbank4 & 0x1)==0))
   {

     ChannelAddr_G = ChannelAddr_G | ((numbankbits==3)? ((((UINT64)((Bank>>0) & 0x1))<<(bank0+5)) |
                                     (((UINT64)((Bank>>1) & 0x1))<<(bank1+5)) |
                                     (((UINT64)((Bank>>2) & 0x1))<<(bank2+5))) :
                    (numbankbits==4)? ((((UINT64)((Bank>>0) & 0x1))<<(bank0+5)) |
                                      (((UINT64)((Bank>>1) & 0x1))<<(bank1+5)) |
                                      (((UINT64)((Bank>>2) & 0x1))<<(bank2+5)) |
                                      (((UINT64)((Bank>>3) & 0x1))<<(bank3+5))) :
                                      ((((UINT64)((Bank>>0) & 0x1))<<(bank0+5)) |
                                      (((UINT64)((Bank>>1) & 0x1))<<(bank1+5)) |
                                      (((UINT64)((Bank>>2) & 0x1))<<(bank2+5)) |
                                      (((UINT64)((Bank>>3) & 0x1))<<(bank3+5)) |
                                      ((((UINT64)((Bank>>4) & 0x1))<<(bank4+5)))));
   }//addrhash was not on
   else
   {
     gAddrData->addrhash[0] = ((internal_bit_wise_xor(Col & ((addrhashbank0>>1)&0x1fff)))
                   ^ ( internal_bit_wise_xor(Row & ((addrhashbank0>>14)&0x3ffff)))) & (addrhashbank0 & 1);
     gAddrData->addrhash[1] = ((internal_bit_wise_xor(Col & ((addrhashbank1>>1)&0x1fff)))
                   ^ ( internal_bit_wise_xor(Row & ((addrhashbank1>>14)&0x3ffff)))) & (addrhashbank1 & 1);
     gAddrData->addrhash[2] = ((internal_bit_wise_xor(Col & ((addrhashbank2>>1)&0x1fff)))
                   ^ ( internal_bit_wise_xor(Row & ((addrhashbank2>>14)&0x3ffff)))) & (addrhashbank2 & 1);
     gAddrData->addrhash[3] = ((internal_bit_wise_xor(Col & ((addrhashbank3>>1)&0x1fff)))
                   ^ ( internal_bit_wise_xor(Row & ((addrhashbank3>>14)&0x3ffff)))) & (addrhashbank3 & 1);
     gAddrData->addrhash[4] = ((internal_bit_wise_xor(Col & ((addrhashbank4>>1)&0x1fff)))
                   ^ ( internal_bit_wise_xor(Row & ((addrhashbank4>>14)&0x3ffff)))) & (addrhashbank4 & 1);

     if(addrhashbank0 & 0x1)
       ChannelAddr_G = ChannelAddr_G | (((UINT64)((Bank & 0x1) ^ gAddrData->addrhash[0]))<<(bank0+5));
     else
       ChannelAddr_G = ChannelAddr_G | (((UINT64)(Bank & 0x1) )<<(bank0+5));

     if(addrhashbank1 & 0x1)
       ChannelAddr_G = ChannelAddr_G | (((UINT64)(((Bank>>1) & 0x1) ^ gAddrData->addrhash[1]))<<(bank1+5));
     else
       ChannelAddr_G = ChannelAddr_G | (((UINT64)((Bank>>1) & 0x1) )<<(bank1+5));

     if(addrhashbank2 & 0x1)
       ChannelAddr_G = ChannelAddr_G | (((UINT64)(((Bank>>2) & 0x1) ^ gAddrData->addrhash[2]))<<(bank2+5));
     else
       ChannelAddr_G = ChannelAddr_G | (((UINT64)((Bank>>2) & 0x1) )<<(bank2+5));

     if(numbankbits >= 4)
     {
       if(addrhashbank3 & 0x1)
         ChannelAddr_G = ChannelAddr_G | (((UINT64)(((Bank>>3) & 0x1) ^ gAddrData->addrhash[3]))<<(bank3+5));
       else
         ChannelAddr_G = ChannelAddr_G | (((UINT64)((Bank>>3) & 0x1) )<<(bank3+5));

       if(numbankbits == 5)
       {
         if(addrhashbank4 & 0x1)
           ChannelAddr_G = ChannelAddr_G | (((UINT64)(((Bank>>4) & 0x1) ^ gAddrData->addrhash[4]))<<(bank4+5));
         else
           ChannelAddr_G = ChannelAddr_G | (((UINT64)((Bank>>4) & 0x1) )<<(bank4+5));
       }
     }

   }


   if(vcmen){
     if((addrhashpc & 0x1)==0)
     {
         ChannelAddr_G = ChannelAddr_G | ((UINT64)((Subchan) & 0x1) << (chan+5));
     }
     else
     {
          gAddrData->addrhash[5] = ((internal_bit_wise_xor(Col & ((addrhashpc>>1)&0x1fff))) ^ ( internal_bit_wise_xor(Row & ((addrhashpc>>14)&0x3ffff))))  & (addrhashpc & 1);
          gAddrData->addrhash[5] = gAddrData->addrhash[5] ^ (internal_bit_wise_xor(Bank & (addrhashpc2 & 0x1f))) ;
          ChannelAddr_G  = ChannelAddr_G | ((UINT64)(Subchan ^ gAddrData->addrhash[5] ) << (chan+5));
     }
   }

   //Place the cs
   if((SEC==0) || ((SEC==2) && (Rank%2==0)))
   {
     ChannelAddr_G |= (UINT64)(((UINT64)(((((UINT64)(CSBaseExt))<<32)| (CSBase)) & ((((UINT64)(~CSMaskExt))<<32)| (~CSMask))))<<8);
   }
   else
   {
     ChannelAddr_G |= (UINT64)(((UINT64)(((((UINT64)(CSBasesecExt))<<32)| (CSBasesec)) & ((((UINT64)(~CSMasksecExt))<<32)| (~CSMasksec))))<<8);
   }

   return ChannelAddr_G;
}


VOID get_fake_bank_row_col_rm (UINT64 addrhashnormaddr0, UINT8 numbankbits, UINT8 bank0, UINT8 bank1,
    UINT8 bank2, UINT8 bank3, UINT8 bank4, UINT8 row_lo0, UINT8 numrowlobits,
    UINT8 numcsbits, UINT8 rm0, UINT8 rm1,  UINT8 rm2, UINT8 numcolbits,
    UINT32 COL0REG, UINT32 COL1REG, UINT8* fk_bank, UINT16 *fk_col, UINT32  *fk_row, UINT8 *fk_rm
    )
{
    *fk_bank = numbankbits == 3 ? ( ((( (UINT64)(addrhashnormaddr0>>1)<<9)>>(bank0+5)) & 0x1) |
                                    (((((UINT64)(addrhashnormaddr0>>1)<<9)>>(bank1+5)) & 0x1)<<1) |
                                    (((((UINT64)(addrhashnormaddr0>>1)<<9)>>(bank2+5)) & 0x1)<<2) ) :
             numbankbits == 4 ? (((((UINT64)(addrhashnormaddr0>>1)<<9)>>(bank0+5)) & 0x1) |
                                (((((UINT64)(addrhashnormaddr0>>1)<<9)>>(bank1+5)) & 0x1)<<1) |
                                (((((UINT64)(addrhashnormaddr0>>1)<<9)>>(bank2+5)) & 0x1)<<2) |
                                (((((UINT64)(addrhashnormaddr0>>1)<<9)>>(bank3+5)) & 0x1)<<3)) :
                                (((((UINT64)(addrhashnormaddr0>>1)<<9)>>(bank0+5)) & 0x1) |
                                (((((UINT64)(addrhashnormaddr0>>1)<<9)>>(bank1+5)) & 0x1)<<1) |
                                (((((UINT64)(addrhashnormaddr0>>1)<<9)>>(bank2+5)) & 0x1)<<2) |
                                (((((UINT64)(addrhashnormaddr0>>1)<<9)>>(bank3+5)) & 0x1)<<3) |
                                (((((UINT64)(addrhashnormaddr0>>1)<<9)>>(bank4+5)) & 0x1)<<4) ) ;

  *fk_row = (UINT32)((((UINT64)(addrhashnormaddr0>>1)<<9) >> (row_lo0+12)) & ((UINT64)pow_ras(2,numrowlobits)-1)) ;

  *fk_rm =  numcsbits == 0 ? 0 : numcsbits == 1 ? ((((UINT64)(addrhashnormaddr0>>1)<<9) >> (rm0+12)) & 0x1) :
            numcsbits == 2 ? (((((UINT64)(addrhashnormaddr0>>1)<<9) >> (rm0+12)) & 0x1) |
                             (((((UINT64)(addrhashnormaddr0>>1)<<9) >> (rm1+12)) & 0x1)<<1)) :
                             (((((UINT64)(addrhashnormaddr0>>1)<<9) >> (rm0+12)) & 0x1) |
                             (((((UINT64)(addrhashnormaddr0>>1)<<9) >> (rm1+12)) & 0x1)<<1) |
                             (((((UINT64)(addrhashnormaddr0>>1)<<9) >> (rm2+12)) & 0x1)<<2) ) ;

  //*fk_col

  *fk_col = ((((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL0REG >> 0) & 0xF) + 2)) & 0x1 ) |
            (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL0REG >> 4) & 0xF) + 2)) & 0x1 ) << 1) |
            (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL0REG >> 8) & 0xF) + 2)) & 0x1 ) << 2) |
            (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL0REG >> 12) & 0xF) + 2)) & 0x1 ) << 3) |
            (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL0REG >> 16) & 0xF) + 2)) & 0x1 ) << 4);

  if(numcolbits >= 0x6)
  {
    *fk_col |= (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL0REG >> 20) & 0xF) + 2)) & 0x1 ) << 5);
  }
  if(numcolbits >= 0x7)
  {
    *fk_col |= (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL0REG >> 24) & 0xF) + 2)) & 0x1 ) << 6);
  }
  if(numcolbits >= 0x8)
  {
    *fk_col |= (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL0REG >> 28) & 0xF) + 2)) & 0x1 ) << 7);
  }
  if(numcolbits >= 0x9)
  {
    *fk_col |= ( ((((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL1REG >> 0) & 0xF) + 8)) & 0x1) << 8 );
  }
  if(numcolbits >= 0xa)
  {
    *fk_col |= (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL1REG >> 4) & 0xF) + 8)) & 0x1 ) << 9);
  }
  if(numcolbits >= 0xb)
  {
    *fk_col |= (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL1REG >> 8) & 0xF) + 8)) & 0x1 ) << 10);
  }
  if(numcolbits >= 0xc)
  {
    *fk_col |= (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL1REG >> 12) & 0xF) + 8)) & 0x1 ) << 11);
  }
  if(numcolbits >= 0xd)
  {
    *fk_col |= (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL1REG >> 16) & 0xF) + 8)) & 0x1 ) << 12);
  }
  if(numcolbits >= 0xe)
  {
    *fk_col |= (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL1REG >> 20) & 0xF) + 8)) & 0x1 ) << 13);
  }
  if(numcolbits >= 0xf)
  {
    *fk_col |= (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL1REG >> 24) & 0xF) + 8)) & 0x1 ) << 14);
  }
  if(numcolbits >= 0x10)
  {
    *fk_col |= (( (((UINT64)(addrhashnormaddr0>>1)<<9) >> (((COL1REG >> 28) & 0xF) + 8)) & 0x1 ) << 15);
  }

}

VOID unhash_bank (UINT8 bank, UINT32 addrhashbank0, UINT32 addrhashbank1, UINT32 addrhashbank2,
    UINT32 addrhashbank3, UINT32 addrhashbank4, UINT8* outputbnk, UINT16 col, UINT32 row, UINT8 numbankbits)
{
  if (((addrhashbank0 & 1) ==0) && ((addrhashbank1 & 1) ==0) && ((addrhashbank2 & 1) ==0) &&
      ((addrhashbank3 & 1) ==0) && ((addrhashbank4 & 1) ==0))
  {
    *outputbnk = bank;
  }
  else
  {

    gAddrData->addrhash[0] = ((internal_bit_wise_xor(col & ((addrhashbank0>>1)&0x1fff)))
        ^ ( internal_bit_wise_xor(row & ((addrhashbank0>>14)&0x3ffff)))) & (addrhashbank0 & 1);
    gAddrData->addrhash[1] = ((internal_bit_wise_xor(col & ((addrhashbank1>>1)&0x1fff)))
        ^ ( internal_bit_wise_xor(row & ((addrhashbank1>>14)&0x3ffff)))) & (addrhashbank1 & 1);
    gAddrData->addrhash[2] = ((internal_bit_wise_xor(col & ((addrhashbank2>>1)&0x1fff)))
        ^ ( internal_bit_wise_xor(row & ((addrhashbank2>>14)&0x3ffff)))) & (addrhashbank2 & 1);
    gAddrData->addrhash[3] = ((internal_bit_wise_xor(col & ((addrhashbank3>>1)&0x1fff)))
        ^ ( internal_bit_wise_xor(row & ((addrhashbank3>>14)&0x3ffff)))) & (addrhashbank3 & 1);
    gAddrData->addrhash[4] = ((internal_bit_wise_xor(col & ((addrhashbank4>>1)&0x1fff)))
        ^ ( internal_bit_wise_xor(row & ((addrhashbank4>>14)&0x3ffff)))) & (addrhashbank4 & 1);


    *outputbnk = (numbankbits ==3) ? ((((addrhashbank0 & 0x1)==1) ? (gAddrData->addrhash[0] ^ (bank & 0x1)) : (bank & 0x1)) |
                                     ((((addrhashbank1 & 0x1)==1) ? (gAddrData->addrhash[1] ^ ((bank>>1) & 0x1)) : ((bank>>1) & 0x1))<<1) |
                                     ((((addrhashbank2 & 0x1)==1) ? (gAddrData->addrhash[2] ^ ((bank>>2) & 0x1)) : ((bank>>2) & 0x1))<<2)) :
                 (numbankbits ==4) ?  ((((addrhashbank0 & 0x1)==1) ? (gAddrData->addrhash[0] ^ (bank & 0x1)) : (bank & 0x1)) |
                                      ((((addrhashbank1 & 0x1)==1) ? (gAddrData->addrhash[1] ^ ((bank>>1) & 0x1)) : ((bank>>1) & 0x1))<<1) |
                                      ((((addrhashbank2 & 0x1)==1) ? (gAddrData->addrhash[2] ^ ((bank>>2) & 0x1)) : ((bank>>2) & 0x1))<<2) |
                                      ((((addrhashbank3 & 0x1)==1) ? (gAddrData->addrhash[3] ^ ((bank>>3) & 0x1)) : ((bank>>3) & 0x1))<<3)) :
                                      ((((addrhashbank0 & 0x1)==1) ? (gAddrData->addrhash[0] ^ (bank & 0x1)) : (bank & 0x1)) |
                                      ((((addrhashbank1 & 0x1)==1) ? (gAddrData->addrhash[1] ^ ((bank>>1) & 0x1)) : ((bank>>1) & 0x1))<<1) |
                                      ((((addrhashbank2 & 0x1)==1) ? (gAddrData->addrhash[2] ^ ((bank>>2) & 0x1)) : ((bank>>2) & 0x1))<<2) |
                                      ((((addrhashbank3 & 0x1)==1) ? (gAddrData->addrhash[3] ^ ((bank>>3) & 0x1)) : ((bank>>3) & 0x1))<<3) |
                                      ((((addrhashbank4 & 0x1)==1) ? (gAddrData->addrhash[4] ^ ((bank>>4) & 0x1)) : ((bank>>4) & 0x1))<<4));

  }
}
VOID uninvertmsbs_row (UINT32 row, UINT8 numrowlobits, UINT8 invertmsbse,
    UINT8 invertmsbso, UINT8 SEC, UINT32* output_row, UINT8 cs )
{
     *output_row = ((((row>>(numrowlobits-1)) & 0x1 ) ^ (cs%2 ? (((invertmsbso>>1) & 0x1)) : (((invertmsbse>>1) & 0x1))))<<(numrowlobits-1)) |
                      ((((row>>(numrowlobits-2)) & 0x1 ) ^ (cs%2 ? (((invertmsbso>>0) & 0x1)) : (((invertmsbse>>0) & 0x1))))<<(numrowlobits-2)) |
                      (row & (((UINT64)pow_ras(2,numrowlobits-2))-1));
}

VOID unhash_rm (UINT8 rankmul, UINT64 addrhashrm0, UINT64 addrhashrm1, UINT64 addrhashrm2, UINT8* outputrm, UINT8 noofrm, UINT8 numbankbits,
  UINT8 bank0, UINT8 bank1, UINT8 bank2, UINT8 bank3, UINT8 bank4, UINT8 row_lo0, UINT8 numrowlobits, UINT8 numcsbits, UINT8 rm0, UINT8 rm1,
  UINT8 rm2, UINT8 numcolbits, UINT32 COL0REG, UINT32 COL1REG, UINT32 row, UINT8 invertmsbse, UINT8 invertmsbso, UINT8 cs_num, UINT8 SEC, UINT16 col,
  UINT32 addrhashbank0, UINT32 addrhashbank1, UINT32 addrhashbank2, UINT32 addrhashbank3, UINT32 addrhashbank4, UINT8 bank  )
{
  UINT32 temp_row, fk_row;
  UINT8 temp_bank;
  UINT8 fk_rm, fk_bank;
  UINT16 fk_col;
  *outputrm=0;

  if(((addrhashrm0 & 0x1)==0) && ((addrhashrm1 & 0x1)==0) && ((addrhashrm2 & 0x1)==0))
  {
    *outputrm = rankmul;
  }
  else
  {
   uninvertmsbs_row(row, numrowlobits, invertmsbse, invertmsbso, SEC, &temp_row, cs_num  );
   unhash_bank (bank, addrhashbank0, addrhashbank1, addrhashbank2, addrhashbank3, addrhashbank4, &temp_bank, col, temp_row, numbankbits );
   if((addrhashrm0 & 1) && (noofrm >=1)){
     get_fake_bank_row_col_rm (addrhashrm0, numbankbits, bank0, bank1, bank2, bank3, bank4, row_lo0, numrowlobits, numcsbits, rm0, rm1, rm2,
                               numcolbits, COL0REG, COL1REG, &fk_bank, &fk_col, &fk_row, &fk_rm );
     *outputrm = (rankmul &0x1) ^ (internal_bit_wise_xor(fk_bank & temp_bank)) ^ (internal_bit_wise_xor(fk_row & temp_row)) ^ (internal_bit_wise_xor(fk_col & col)) ^ \
                   (internal_bit_wise_xor(fk_rm & rankmul));
   }
   else
   {
     *outputrm = rankmul & 1;
   }


   if((addrhashrm1 & 1) && (noofrm >=2) ){
     get_fake_bank_row_col_rm (addrhashrm1, numbankbits, bank0, bank1, bank2, bank3, bank4, row_lo0, numrowlobits, numcsbits, rm0, rm1, rm2,
                               numcolbits, COL0REG, COL1REG, &fk_bank, &fk_col, &fk_row, &fk_rm );
     *outputrm = (((rankmul>>1)&0x1) ^ (internal_bit_wise_xor(fk_bank & temp_bank)) ^ (internal_bit_wise_xor(fk_row & temp_row)) ^ (internal_bit_wise_xor(fk_col & col)) ^ \
                   (internal_bit_wise_xor(fk_rm & rankmul)))<<1 | *outputrm;
   }
   else
   {
     *outputrm = (((rankmul>>1)&0x1)<<1) | *outputrm;
   }

   if((addrhashrm2 & 1) && (noofrm >=3)){
     get_fake_bank_row_col_rm (addrhashrm2, numbankbits, bank0, bank1, bank2, bank3, bank4, row_lo0, numrowlobits, numcsbits, rm0, rm1, rm2,
                               numcolbits, COL0REG, COL1REG, &fk_bank, &fk_col, &fk_row, &fk_rm );
     *outputrm = (((rankmul>>2)&0x1) ^ (internal_bit_wise_xor(fk_bank & temp_bank)) ^ (internal_bit_wise_xor(fk_row & temp_row)) ^ (internal_bit_wise_xor(fk_col & col)) ^ \
                   (internal_bit_wise_xor(fk_rm & rankmul)))<<2 | *outputrm;
   }
   else
   {
     *outputrm = (((rankmul>>2)&0x1)<<2) | *outputrm;
   }
  }
}

//this function finds the cs position in a 40 bit NormAddr..depends on which rank we are looking at
VOID bit_pos_40b (UINT64 cs_pos, UINT8 *fpos, UINT8 *spos)
{
  UINT8 lp=0;
  UINT64 cs_pos_tmp=0;

  *fpos= 0; *spos=0;
  cs_pos_tmp = cs_pos;

  for(lp=0;lp<40;lp++)
  {
    if((cs_pos_tmp>>lp) & 0x1)
    {
       if(*fpos==0) *fpos = lp; else *spos = lp;
    }
  }
}

UINT32 get_row_bits (UINT8 pkg_no, UINT8 mpu_no, UINT8 umc_inst_num, UINT8 umc_chan_num, UINT8 cs_num)
{

  return (((((gAddrData->CONFIGDIMM[convert_to_addr_trans_index(pkg_no, mpu_no, umc_inst_num, umc_chan_num)][cs_num])>>8) & 0xf) + 10) +
         ((((gAddrData->CONFIGDIMM[convert_to_addr_trans_index(pkg_no, mpu_no, umc_inst_num, umc_chan_num)][cs_num])>>12) & 0xf) + 0));

}

/*----------------------------------------------------------------------------------------*/
/**
 * Convert normalized address to chip select, row, column, bank, rankmul
 *
 * ChannelAddr expected to be passed from the caller should be till lsb=0 and only msb=39 is considered
 * no fancy 39:4 version
 * pkg_no: socket number
 * mpu_no: die number
 *
 *----------------------------------------------------------------------------------------*/
VOID
translate_norm_to_dram_addr (
  UINT64  ChannelAddr,
  UINT8   pkg_no,
  UINT8   mpu_no,
  UINT8   umc_inst_num,
  UINT8   umc_chan_num,
  UINT8   *cs_num,
  UINT8   *bank,
  UINT32  *row,
  UINT16  *col,
  UINT8   *rankmul,
  UINT8   *subchan
  )
{
  //umc_inst_num is like nodeid.

  //Need to check the validity of the NA vs dct number vs node number
  UINT32 CSBase=0, CSMask=0,CSBasesec=0, CSMasksec=0;
  UINT64 CSBaseExt=0, CSMaskExt=0, CSBaseExtsec=0, CSMaskExtsec=0;
  //UINT64 NormAddr=0;
  UINT8 Bank=0,Rankmul=0, Subchan=0;
  UINT32 Row=0;
  UINT16 Col=0;
  UINT8 SEC = 0;  //this will be set to 1 if we are in the secondary rank of a non-power of 2 sized rank, or secondary of 3cs case

  UINT64 temp=0;
  UINT8 cs=0 ;

  BOOLEAN CSEn=0,CSEnsec=0;
  UINT8 Chipselect=0;
  UINT8 noofbank=0, noofrm=0, noofrowlo=0, noofcol=0,  bank0=0, bank1=0, bank2=0,bank3=0,bank4=0;
  UINT8 numrowlobits=0, numcolbits=0, numcsbits=0,  numbankbits=0;
  UINT8 row_lo0=0, rm0=0, rm1=0,rm2=0, chan=0,  invertmsbse=0, invertmsbso=0;
  UINT32 COL0REG=0, COL1REG=0;
  UINT32 addrhashbank0=0,addrhashbank1=0,addrhashbank2=0,addrhashbank3=0,addrhashbank4=0, addrhashpc=0, addrhashpc2=0;
  UINT64 InputAddr=0;
  UINT8 rankmul_temp=0,vcmen=0;

  LOC_1 = convert_to_addr_trans_index(pkg_no, mpu_no, umc_inst_num, umc_chan_num);

  //get_highest_possible_addr_bit(pkg_no, mpu_no, umc_inst_num, umc_chan_num);

  //read out the addrhash* registers here
  addrhashbank0  = gAddrData->ADDRHASHBANK0 [LOC_1];
  addrhashbank1  = gAddrData->ADDRHASHBANK1 [LOC_1];
  addrhashbank2  = gAddrData->ADDRHASHBANK2 [LOC_1];
  addrhashbank3  = gAddrData->ADDRHASHBANK3 [LOC_1];
  addrhashbank4  = gAddrData->ADDRHASHBANK4 [LOC_1];
  addrhashpc     = gAddrData->ADDRHASHPC    [LOC_1];
  addrhashpc2    = gAddrData->ADDRHASHPC2   [LOC_1];

  for (cs = 0; cs < 8; ++cs)
  {
    temp = gAddrData->CSBASE[LOC_1][cs];

    CSBase = temp & 0xffffffff;

    //Secondary decoder stuff for each primary
    temp = gAddrData->CSBASESEC[LOC_1][cs];
    CSBasesec = temp & 0xffffffff;

    temp = gAddrData->CSMASK[LOC_1][(cs)];

    CSMask = temp & 0xffffffff;

    //Secondary decoder stuff
    temp = gAddrData->CSMASKSEC[LOC_1][(cs)];
    CSMasksec = temp &  0xffffffff;

    temp = gAddrData->EXT_CSBASE[LOC_1][(cs)] & 0xff;
    CSBaseExt = temp;

    temp = gAddrData->EXT_CSBASESEC[LOC_1][(cs)] & 0xff;
    CSBaseExtsec = temp;

    temp = gAddrData->EXT_CSMASK[LOC_1][(cs)] & 0xff;
    CSMaskExt = temp;

    temp = gAddrData->EXT_CSMASKSEC[LOC_1][(cs)] & 0xff;
    CSMaskExtsec = temp;

    InputAddr = (ChannelAddr>>8) & 0x3ffffffff;

    //Read out *RAMCFG*
    temp = gAddrData->CONFIGDIMM[LOC_1][(cs)];
    CSEn = temp & 0x1;
    CSEnsec = (temp >> 24) & 0x1;

    if((CSEn && (InputAddr & ((((UINT64)(~CSMaskExt))<<32)| (~CSMask))) == ( ((((UINT64)(CSBaseExt))<<32)| (CSBase)) & ((((UINT64)(~CSMaskExt))<<32)| (~CSMask)))) || (CSEnsec &&
        (InputAddr & ((((UINT64)(~CSMaskExtsec))<<32)| (~CSMasksec))) == (((((UINT64)(CSBaseExtsec))<<32)| (CSBasesec)) & ((((UINT64)(~CSMaskExtsec))<<32)| (~CSMasksec))))) {

      ////hashing
      //Dealing with cshash..
      Chipselect = 0 ;
      if(gAddrData->ADDRHASHNORMADDR[LOC_1][0] & 0x1)  {
        Chipselect = ((internal_bit_wise_xor((((((UINT64)(gAddrData->EXT_ADDRHASHNORMADDR[LOC_1][0]))<<32) |gAddrData->ADDRHASHNORMADDR[LOC_1][0])>>1) & (ChannelAddr>>9))) ^ (cs & 0x1));
      } else {
        Chipselect  = (cs & 0x1) ;
      }

      if(gAddrData->ADDRHASHNORMADDR[LOC_1][1] & 0x1) {
        Chipselect =  Chipselect | (((internal_bit_wise_xor((((((UINT64)(gAddrData->EXT_ADDRHASHNORMADDR[LOC_1][1]))<<32)|
          gAddrData->ADDRHASHNORMADDR[LOC_1][1])>>1) & (ChannelAddr>>9))) ^ ((cs>>1) & 0x1))<<1);
      } else {
        Chipselect = Chipselect |(((cs>>1)& 0x1)<<1);
      }

      if(gAddrData->ADDRHASHNORMADDR[LOC_1][2] & 0x1) {
        Chipselect =  Chipselect | (((internal_bit_wise_xor((((((UINT64)(gAddrData->EXT_ADDRHASHNORMADDR[LOC_1][2]))<<32)|
          gAddrData->ADDRHASHNORMADDR[LOC_1][2])>>1) & (ChannelAddr>>9))) ^ ((cs>>2) & 0x1))<<2);
      } else {
        Chipselect = Chipselect |(((cs>>2)& 0x1)<<2);
      }

      //hashing end

      SEC = (CSEn && (InputAddr & ((((UINT64)(~CSMaskExt))<<32)| (~CSMask))) == (((((UINT64)(CSBaseExt))<<32)| (CSBase)) & ((((UINT64)(~CSMaskExt))<<32)| (~CSMask)))) ? 0 : (CSEnsec &&
        ((InputAddr & ((((UINT64)(~CSMaskExtsec))<<32) | (~CSMasksec))) == (((((UINT64)(CSBaseExtsec))<<32)| (CSBasesec)) & ((((UINT64)(~CSMaskExtsec))<<32)| (~CSMasksec)))) && CSEn) ? 1 : 3;
      InputAddr =0 ;

      noofbank  = (temp >> 4) & 0x3;
      noofrm    = (temp >> 6) & 0x3;
      noofrowlo = (temp >> 8) & 0xF;
      noofcol   = (temp >> 16) & 0xF;


      //Read out *BANK_SEL*

      temp = gAddrData->BANKSELDIMM[LOC_1][(cs)];
      bank0 = temp & 0xf;
      bank1 = (temp >> 4) & 0xf;
      bank2 = (temp >> 8) & 0xf;
      bank3 = (temp >> 12) & 0xf;
      bank4 = (temp >> 16) & 0xf;

      break;
    }//csen inputaddr loop
  }//cs loop

  if(((gAddrData->RANK_ENABLE_PER_UMCCH_ADDR_TRANS[LOC_1]>>((cs==2) && (((gAddrData->CONFIGDIMM[LOC_1][2]>>1) & 0x3)!=0) ? cs ^ ((gAddrData->CONFIGDIMM[LOC_1][2]>>1) & 0x3):cs)) & 0x1) == 0)
  {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Rank is out of bounds. What NormAddr did you pass into the library?? \n");
  }

  //Read out *ROW_SEL*
  temp = gAddrData->ROWSELDIMM[LOC_1][(cs)];
  row_lo0 = (temp >> 0) & 0xF ;

  //Read out *COL0_SEL*
  temp = gAddrData->COL0SELDIMM[LOC_1][(cs)];
  COL0REG = (UINT32)temp;

  //Read out *COL1_SEL*
  temp = gAddrData->COL1SELDIMM[LOC_1][(cs)];
  COL1REG = (UINT32)temp;

  //Read out *RM_SEL*
  temp = gAddrData->RMSELDIMM[LOC_1][(cs)];
  rm0 = (temp >> 0) & 0xF;
  rm1 = (temp >> 4) & 0xF;
  rm2 = (temp >> 8) & 0xF;
  chan = (temp >>16) & 0xF;
  invertmsbse = (temp >> 28) & 0x3;
  invertmsbso = (temp >> 30) & 0x3;

  numrowlobits = noofrowlo + 10;
  numcolbits = noofcol + 5;
  numcsbits = noofrm;
  numbankbits = noofbank == 2? 5: noofbank == 1? 4 : 3;

  //Let us see if we need to swizzle or not
  temp = gAddrData->CTRLREG[LOC_1][(cs>>1)];
  vcmen = (temp >>1) & 0x1;

  NormalizedToBankAddrMap(ChannelAddr, &Bank, &Row, &Col, &Rankmul, &Subchan, numbankbits, bank4, bank3, bank2, bank1, bank0,
         numrowlobits, numcolbits, row_lo0, COL0REG, COL1REG, numcsbits, rm0, rm1, rm2, chan, invertmsbse, invertmsbso,
         SEC, cs, addrhashbank0,addrhashbank1,addrhashbank2,addrhashbank3,addrhashbank4,addrhashpc,addrhashpc2, vcmen);

  rankmul_temp=0;

  if((gAddrData->ADDRHASHRMADDR[LOC_1][0] & 1) && (noofrm>=1))
  {
    rankmul_temp = ((internal_bit_wise_xor((((((UINT64)(gAddrData->EXT_ADDRHASHRMADDR[LOC_1][0]))<<32)|gAddrData->ADDRHASHRMADDR[LOC_1][0])>>1) & (ChannelAddr>>9))) ^ (Rankmul & 0x1)) ;
  } else {
    rankmul_temp = Rankmul & 0x1;
  }

  if((gAddrData->ADDRHASHRMADDR[LOC_1][1] & 0x1) && (noofrm>=2)) {
    rankmul_temp =  rankmul_temp | (((internal_bit_wise_xor((((((UINT64)(gAddrData->EXT_ADDRHASHRMADDR[LOC_1][1]))<<32)|gAddrData->ADDRHASHRMADDR[LOC_1][1])>>1) &
        (ChannelAddr>>9))) ^ ((Rankmul>>1) & 0x1))<<1);
  } else {
    rankmul_temp = rankmul_temp |(((Rankmul>>1)& 0x1)<<1);
  }

  if((gAddrData->ADDRHASHRMADDR[LOC_1][2] & 0x1)  && (noofrm>=3)) {
    rankmul_temp =  rankmul_temp | (((internal_bit_wise_xor((((((UINT64)(gAddrData->EXT_ADDRHASHRMADDR[LOC_1][2]))<<32)|gAddrData->ADDRHASHRMADDR[LOC_1][2])>>1) &
        (ChannelAddr>>9))) ^ ((Rankmul>>2) & 0x1))<<2);
  } else {
    rankmul_temp = rankmul_temp |(((Rankmul>>2)& 0x1)<<2);
  }

  //self checking
  //NormAddr = BankAddrToNormalizedMap(CSBase, CSBaseExt, CSMask, CSMaskExt, cs, Bank, Row, Col, Rankmul, Subchan, numbankbits, bank4, bank3, bank2, bank1, bank0,
  //       numrowlobits,  numcolbits, row_lo0, COL0REG, COL1REG, numcsbits, rm0, rm1, rm2, chan, invertmsbse, invertmsbso,
  //       CSMasksec, CSMaskExtsec, CSBasesec, CSBaseExtsec, SEC, addrhashbank0,addrhashbank1,addrhashbank2,addrhashbank3,addrhashbank4,addrhashpc,addrhashpc2);

  //if ((NormAddr & ((UINT64)pow_ras(2,gAddrData->VALIDHI[LOC_1])-1))>>3 != (ChannelAddr & ((UINT64)pow_ras(2,gAddrData->VALIDHI[LOC_1])-1))>>3) {
  //  IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Given NormAddr_39_3=%jx, Backward NormAddr_39_3=%jx\n\n",
  //    (ChannelAddr & ((UINT64)pow_ras(2,gAddrData->VALIDHI[LOC_1])-1) )>>3 ,(NormAddr & ((UINT64)pow_ras(2,gAddrData->VALIDHI[LOC_1])-1))>>3);
  //}

  *cs_num = Chipselect;
  //we need to change it to 0 if its 2 and we are in csxor case
  {
    if((Chipselect==2) && (((gAddrData->CONFIGDIMM[LOC_1][2]>>1) & 0x3)!=0)){
      *cs_num = *cs_num ^ ((gAddrData->CONFIGDIMM[LOC_1][2]>>1) & 0x3);
    }
  }

  *bank = Bank;
  *row = Row;
  *col = Col;
  *rankmul = rankmul_temp;
  *subchan = Subchan;

  //Need to check if the outputs respect the config
  if(Bank >= (noofbank==2? 32 : (noofbank==1? 16 : 8))) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "translate_norm_to_dram_addr: ERROR: Bank of the Given NormAddr is out of bounds\n");
  }

  if(Row >= (UINT32)(pow_ras(2,(noofrowlo+10)))) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "translate_norm_to_dram_addr: ERROR: Row of the Given NormAddr is out of bounds\n");
  }

  if(Col >= (UINT16)(pow_ras(2,(noofcol+5)))) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "translate_norm_to_dram_addr: ERROR: Col of the Given NormAddr is out of bounds\n");
  }
}

UINT64 translate_dram_to_norm_addr (
  UINT8 pkg_no,
  UINT8 mpu_no,
  UINT8 umc_inst_num,
  UINT8 umc_chan_num,
  UINT8 cs_num,
  UINT8 bank,
  UINT32 row,
  UINT16 col,
  UINT8 rankmul,
  UINT8 subchan)
{

  UINT64 ChannelAddr_G=0;
  UINT64 InputAddr=0;

  //if(!OOR)
  {
      //Let us check the validity of nnode

      UINT8 TransCS=0;/*TransDCT=0,DctSelIntLvAddr=0,BankSwapAddr8En=0,DctSelBankSwap=0;*/

      UINT32 temp=0;
      UINT32 CSBase=0, CSMask=0,CSBasesec=0, CSMasksec=0,CSBase1sec=0,CSMask1sec=0;
      UINT64 CSBaseExt=0, CSMaskExt=0, CSBaseExtsec=0, CSMaskExtsec=0;
      UINT16 TransCol=0,fk_col=0;
      UINT32 TransRow=0,fk_row=0, temp_row=0;
      UINT8 TransBank=0, fk_bank=0, temp_bank=0;
      UINT8 TransRankmul=0,fk_rm=0, temp_rm;
      UINT8 TransSubchan;
      UINT8 SEC =0, Chipselect=0, Chipselect1=0;

      UINT8 noofbank=0, noofrm=0, noofrowlo=0, noofcol=0,  bank0=0, bank1=0, bank2=0,bank3=0,bank4=0;
      UINT8 numrowlobits=0, numcolbits=0, numcsbits=0, numbankbits=0;
      UINT8 row_lo0=0, rm0=0, rm1=0,rm2=0, chan=0, invertmsbse=0, invertmsbso=0;
      UINT32 addrhashbank0=0,addrhashbank1=0,addrhashbank2=0,addrhashbank3=0,addrhashbank4=0, addrhashpc=0, addrhashpc2=0;
      UINT64 addrhashnormaddr0=0, addrhashnormaddr1=0, addrhashnormaddr2=0, addrhashrmaddr0, addrhashrmaddr1, addrhashrmaddr2 ;
      UINT8 rankmul_temp,rankmul_temp1, vcmen;
      UINT8 cs1=0;
      BOOLEAN CSEn1=0, CSEn=0,CSEn1sec=0,CSEnsec=0;

      UINT32 COL0REG=0, COL1REG=0;

      UINT64 ADDR=0;
      UINT64 cs_pos=0;
      UINT8  fpos=0, spos=0;
      //UINT32 inputaddr_lhs=0, cs_rhs=0 ,inputaddr_lhs_sec=0, cs_rhssec=0;
      UINT32 CSMask1=0, CSBase1=0;
      UINT32 CSMask1Ext=0, CSBase1Ext=0, CSMask1Extsec=0, CSBase1Extsec=0;
      UINT64 CSMask1_64b=0,CSBase1_64b=0,CSMask1sec_64b=0,CSBase1sec_64b=0;

      DRAMTYPE = get_dramtype();

      LOC_2 = convert_to_addr_trans_index(pkg_no, mpu_no, umc_inst_num, umc_chan_num);

      //printf ("translate_dram_to_norm_addr %d %d %d %d rank%d %d 0x%x col%x %x %x\n",pkg_no,mpu_no,umc_inst_num,umc_chan_num, cs_num, bank, row, col, rankmul, subchan);
      //now cs hash has two requirements: no xoring in cs postion and the dimms have to ve identical on a channel.
      //so the incoming cs_num needs to unhashed. so that we can put the right csbasemask in.
      //to unhash, we need row bank col as wedont have normaddr yet to do the calculation.

      //read out the addrhash* registers here
      addrhashbank0 = gAddrData->ADDRHASHBANK0  [LOC_2];
      addrhashbank1 = gAddrData->ADDRHASHBANK1  [LOC_2];
      addrhashbank2 = gAddrData->ADDRHASHBANK2  [LOC_2];
      addrhashbank3 = gAddrData->ADDRHASHBANK3  [LOC_2];
      addrhashbank4 = gAddrData->ADDRHASHBANK4  [LOC_2];
      addrhashpc     = gAddrData->ADDRHASHPC    [LOC_2];
      addrhashpc2    = gAddrData->ADDRHASHPC2   [LOC_2];
      addrhashnormaddr0 = ((UINT64)(gAddrData->EXT_ADDRHASHNORMADDR[LOC_2][0])<<32) | gAddrData->ADDRHASHNORMADDR[LOC_2][0];
      addrhashnormaddr1 = ((UINT64)(gAddrData->EXT_ADDRHASHNORMADDR[LOC_2][1])<<32) | gAddrData->ADDRHASHNORMADDR[LOC_2][1];
      addrhashnormaddr2 = ((UINT64)(gAddrData->EXT_ADDRHASHNORMADDR[LOC_2][2])<<32) | gAddrData->ADDRHASHNORMADDR[LOC_2][2];
      addrhashrmaddr0= (((UINT64)(gAddrData->EXT_ADDRHASHRMADDR[LOC_2][0]))<<32)|gAddrData->ADDRHASHRMADDR[LOC_2][0];
      //addrhashrmaddr0= (((UINT64)(gAddrData->EXT_ADDRHASHRMADDR[LOC_2][0]))<<32)|gAddrData->ADDRHASHRMADDR[LOC_2][0];
      addrhashrmaddr1= (((UINT64)(gAddrData->EXT_ADDRHASHRMADDR[LOC_2][1]))<<32)|gAddrData->ADDRHASHRMADDR[LOC_2][1];
      addrhashrmaddr2= (((UINT64)(gAddrData->EXT_ADDRHASHRMADDR[LOC_2][2]))<<32)|gAddrData->ADDRHASHRMADDR[LOC_2][2];

      if (umc_chan_num >= TOTAL_NUM_UMCCH_PER_UMC_ADDR_TRANS)
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Invalid UMCCH was passed. it has to be between 0x0 and %d\n", TOTAL_NUM_UMCCH_PER_UMC_ADDR_TRANS-1);
      }

      if ((((gAddrData->RANK_ENABLE_PER_UMCCH_ADDR_TRANS[LOC_2])>>cs_num)& 0x1)==0)
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Invalid CS %d. Are you passing some rank greater than the number of ranks supported per channel\n",cs_num);
      }

       //now if csxor case is in play here, then we need to look at the #rows and csxor
       if((cs_num==0) && (row>(((UINT32)(pow_ras(2,(((gAddrData->CONFIGDIMM[LOC_2][0]>>8) & 0xf)+10)))-1)/2)) && (((gAddrData->CONFIGDIMM[LOC_2][2]>>1) & 0x3)!=0)){
         cs_num=cs_num ^ ((gAddrData->CONFIGDIMM[LOC_2][2]>>1) & 0x3);
       }
      temp = gAddrData->CONFIGDIMM[LOC_2][(cs_num)];

      CSEn = temp & 0x1;
      CSEnsec = (temp >>24) & 0x1;
      if(CSEn==0 && CSEnsec==0)
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: The rank you passed is not enabled!! Look at the inputs into routine\n");
      }
      else if ((CSEn==0) && (CSEnsec!=0) && (cs_num%2==0))
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "Error: UMC_ADDR_TRANS The rank you passed is even and why does it have secondary enabled with primary off?? \
            this is allowed for only odd ranks in asymm case of D4 for umc=%d, ch=%d\n",umc_inst_num, umc_chan_num);
      }

      noofbank = (temp >>4) & 0x3;
      noofrm = (temp>>6) & 0x3 ;
      noofrowlo = (temp >> 8) & 0xF;
      noofcol = (temp >> 16) & 0xF;

      //Asserts for Invalid bank
      if ((bank >= 0x8) && noofbank == 0)
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "Invalid bank. LOC_2:%0d,temp: %jx, cs_num:%0d. Bank has to be between 0x0 and 0x7.\
            Your bank=%0d, noofbank:%0d,\n",LOC_2, temp,cs_num,bank,noofbank);
        IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Invalid bank. It has to be 0x0-0x7\n");
      }else if (bank>=0x10 && noofbank == 1)
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "Invalid bank. LOC_2:%0d,temp: %jx, cs_num:%0d. Bank has to be between 0x0 and 0xf.\
            Your bank=%0d, noofbank:%0d,\n",LOC_2, temp,cs_num,bank,noofbank);
        IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Invalid bank. It has to be 0x0-0xf\n");
      }else if (bank>=0x20 && noofbank == 2)
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "Invalid bank. LOC_2:%0d,temp: %jx, cs_num:%0d. Bank has to be between 0x0 and 0x1f.\
            Your bank=%0d, noofbank:%0d,\n",LOC_2, temp,cs_num,bank,noofbank);
        IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Invalid bank. It has to be 0x0-0x1f\n");
      }

      //Assert for Invalid row
      if ((row >= (UINT32)(pow_ras(2,(noofrowlo+10)))))
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "Invalid row. LOC_2:%0d,temp: 0x%lx, cs_num:%0d. Row has to be between 0x0 and 0x%x.\
            Your row=0x%x, noofrowlo:%0d\n", LOC_2, temp, cs_num, (UINT32)(pow_ras(2,(noofrowlo+10))-1), row, noofrowlo);
        IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Invalid row. It has to be 0x0-0x1f\n");
      }

      //Assert for Invalid col
      if (col >= (UINT16)(pow_ras(2,(noofcol+5))))
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "Invalid col.LOC_2:%0d,temp: 0x%lx, cs_num:%0d.  Col has to be between 0x0 and 0x%x.\
            Your col=0x%x, noofcol:%0d\n",LOC_2, temp, cs_num, (UINT32)(pow_ras(2,(noofcol+5))-1), col, noofcol);
        IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Invalid col. It has to be 0x0-0x1f\n");
      }


      //Lets read swizzle
      temp = gAddrData->CTRLREG[LOC_2][(cs_num>>1)];
      vcmen = (temp >>1) & 0x1;


      //if( (DRAMTYPE==1) )
      //{
      //  //Normal case
      //  switch (noofrm)
      //  {
      //    case 0 : if ((rankmul>>0) & 0x1) {if(ASSERT_UMC_ADDR_TRANS){ printf
      //             ("File of coredump is %s and line of coredump is %d\n",__FILE__,__LINE__); exit(1);}
      //             else printf ("ERROR: rankmul should have been 0\n Check the inputs to the routine\n\n");} break;
      //    case 1 : if (((rankmul>>1) & 0x7f) != 0) {if(ASSERT_UMC_ADDR_TRANS){ printf
      //             ("File of coredump is %s and line of coredump is %d\n",__FILE__,__LINE__); exit(1);}
      //             else printf ("ERROR: Wrong rankmul is passed for noofrm=1\n");} break;//rankmul[7:1] should be 0
      //    case 2 : if (((rankmul>>2) & 0x3f) != 0) {if(ASSERT_UMC_ADDR_TRANS){ printf
      //             ("File of coredump is %s and line of coredump is %d\n",__FILE__,__LINE__); exit(1);}
      //             else printf ("ERROR: Wrong rankmul is passed for noofrm=2\n");} break;//rankmul[7:2] should be 0
      //    case 3 : if (((rankmul>>3) & 0x1f) != 0) {if(ASSERT_UMC_ADDR_TRANS){ printf
      //             ("File of coredump is %s and line of coredump is %d\n",__FILE__,__LINE__); exit(1);}
      //             else printf ("ERROR: Wrong rankmul is passed for noofrm=3\n");} break;//rankmul[7:3] should be 0
      //  }
      //}

      // Obtain the CS Base from D18F2x[4C:40]
      temp = gAddrData->CSBASE[LOC_2][cs_num];
      CSBase = temp & 0xffffffff;
      temp = gAddrData->CSBASESEC[LOC_2][cs_num];
      CSBasesec = temp & 0xffffffff;

      // Extract variables from D18F2x[64:60]
      temp = gAddrData->CSMASK[LOC_2][(cs_num)];
      CSMask = temp & 0xffffffff;
      temp = gAddrData->CSMASKSEC[LOC_2][(cs_num)];
      CSMasksec = temp & 0xffffffff;

      temp = gAddrData->EXT_CSBASE[LOC_2][(cs_num)] & 0xff;
      CSBaseExt = temp;

      temp = gAddrData->EXT_CSBASESEC[LOC_2][(cs_num)] & 0xff;
      CSBaseExtsec = temp;

      temp = gAddrData->EXT_CSMASK[LOC_2][(cs_num)] & 0xff;
      CSMaskExt = temp;

      temp = gAddrData->EXT_CSMASKSEC[LOC_2][(cs_num)] & 0xff;
      CSMaskExtsec = temp;

       if (((row > 0xbfff) && (noofrowlo+10 == 16) && (CSMasksec !=0) && (CSEnsec !=0) && (CSEn !=0)  ) ||
           ((row > 0x17fff) && (noofrowlo+10 == 17) && (CSMasksec !=0) && (CSEnsec !=0) && (CSEn !=0) ))
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: In an odd cs scenario..we ignore the last quarter of the rows in a setup...\
            your row falls in the ignored region...illegal row\n");
      }


      //Read out *BANK_SEL*

      temp = gAddrData->BANKSELDIMM[LOC_2][(cs_num)];
      bank0 = temp & 0xf;
      bank1 = (temp >>4)& 0xf;
      bank2 = (temp >>8)& 0xf;
      bank3 = (temp >>12)& 0xf;
      bank4 = (temp >>16)& 0xf;

      //REad out *ROW_SEL*
      temp = gAddrData->ROWSELDIMM[LOC_2][(cs_num)];
      row_lo0 = (temp >>0) & 0xF ;

      //Read out *COL0_SEL*
      temp = gAddrData->COL0SELDIMM[LOC_2][(cs_num)];
      COL0REG = temp;

      //Read out *COL1_SEL*

      temp = gAddrData->COL1SELDIMM[LOC_2][(cs_num)];
      COL1REG = temp;

      //Read out *RM_SEL*

      temp = gAddrData->RMSELDIMM[LOC_2][(cs_num)];
      rm0 = (temp >>0 ) & 0xF;
      rm1 = (temp >>4 ) & 0xF;
      rm2 = (temp >>8 ) & 0xF;
      chan = (temp >>16) & 0xF;
      invertmsbse = (temp>>28) & 0x3;
      invertmsbso = (temp>>30) & 0x3;


      numrowlobits = noofrowlo + 10;
      numcolbits = noofcol + 5;
      numcsbits = noofrm;
      numbankbits = noofbank==2 ? 5 : noofbank==1 ? 4 :3;

      //When NumRM=0, we ar dealing with 6GB/12GB/24GB..both 6 adn 12 have 16 row bits..and 24 has 17 row bits...
      //so half of 2^numrow/2 is the start of row in the sec..cos half is on primary and quarter is on sec...last quarter is left out.
      //6GB, rm=0 and 12GB, rm=0 and 24GB, rm=0

      //SEC is 1 for sec rank of non power of 2 when we have no3cs or 3cs intlv case (in 1+2 case, odd row on r0/3 and even row on r2...2+1 case, odd row on 0/2 and even row on 1)
      SEC = (((row >= ((UINT32)(pow_ras(2,((get_row_bits(pkg_no, mpu_no, umc_inst_num, umc_chan_num, cs_num))-1))))) && (((((UINT64)(CSBaseExtsec))<<32)|(CSBasesec)) !=0) && \
              (CSEnsec !=0) && (CSEn!=0) && !(gAddrData->CTRLREG[LOC_2][cs_num>>1] & 0x1)) \
              || ((gAddrData->CTRLREG[LOC_2][(cs_num>>1)] & 0x1) && (row & 0x1) && ((cs_num==0) || (cs_num==(((gAddrData->CTRLREG[LOC_2][(cs_num>>1)]>>2) & 0x1)? 2: 3)))) \
              || ((gAddrData->CTRLREG[LOC_2][(cs_num>>1)] & 0x1) && (!(row & 0x1)) && ((cs_num==(((gAddrData->CTRLREG[LOC_2][(cs_num>>1)]>>2) & 0x1)? 1: 2)))) \
              ) ? 1 : 0;

      Chipselect =cs_num;
      uninvertmsbs_row(row, numrowlobits, invertmsbse, invertmsbso, SEC, &temp_row, cs_num  );
      unhash_bank (bank, addrhashbank0, addrhashbank1, addrhashbank2, addrhashbank3, addrhashbank4,
          &temp_bank, col, temp_row, numbankbits );
      unhash_rm(rankmul, addrhashrmaddr0, addrhashrmaddr1, addrhashrmaddr2, &temp_rm, noofrm, numbankbits, bank0, bank1, bank2, bank3, bank4, row_lo0, numrowlobits, numcsbits, rm0, rm1, rm2,
                  numcolbits, COL0REG, COL1REG, row, invertmsbse, invertmsbso, cs_num, SEC, col, addrhashbank0, addrhashbank1, addrhashbank2, addrhashbank3, addrhashbank4, bank  );
      if (addrhashnormaddr0 &  1 ){
        get_fake_bank_row_col_rm (addrhashnormaddr0, numbankbits, bank0, bank1, bank2, bank3, bank4, row_lo0, numrowlobits, numcsbits, rm0, rm1, rm2,
                                  numcolbits, COL0REG, COL1REG, &fk_bank, &fk_col, &fk_row, &fk_rm );
        Chipselect = (cs_num & 0x1) ^ (internal_bit_wise_xor(fk_bank & temp_bank))
          ^ (internal_bit_wise_xor(fk_row & temp_row)) ^ (internal_bit_wise_xor(fk_col & col))
          ^  (internal_bit_wise_xor(fk_rm & rankmul));
      }
      else
      {
        Chipselect =  (cs_num & 0x1 );
      }

      if (addrhashnormaddr1 & 1){
        get_fake_bank_row_col_rm (addrhashnormaddr1, numbankbits, bank0, bank1, bank2, bank3, bank4, row_lo0, numrowlobits, numcsbits, rm0, rm1, rm2,
                                  numcolbits, COL0REG, COL1REG, &fk_bank, &fk_col, &fk_row, &fk_rm );
        Chipselect = (((cs_num >>1 ) & 0x1) ^ (internal_bit_wise_xor(fk_bank & temp_bank))
            ^ (internal_bit_wise_xor(fk_row & temp_row)) ^ (internal_bit_wise_xor(fk_col & col))
            ^  (internal_bit_wise_xor(fk_rm & rankmul))) <<1 | Chipselect;
      }
      else
      {
        Chipselect = (((cs_num>>1)&0x1)<<1) | Chipselect;
      }

      if (addrhashnormaddr2 & 1){
        get_fake_bank_row_col_rm (addrhashnormaddr2, numbankbits, bank0, bank1, bank2, bank3, bank4, row_lo0, numrowlobits, numcsbits, rm0, rm1, rm2,
                                  numcolbits, COL0REG, COL1REG, &fk_bank, &fk_col, &fk_row, &fk_rm );
        Chipselect = (((cs_num >>2 ) & 0x1) ^ (internal_bit_wise_xor(fk_bank & temp_bank)) \
            ^ (internal_bit_wise_xor(fk_row & temp_row)) ^ (internal_bit_wise_xor(fk_col & col)) \
            ^  (internal_bit_wise_xor(fk_rm & rankmul))) <<2 | Chipselect;
      }
      else
      {
        Chipselect = (((cs_num>>2)&0x1)<<2) | Chipselect;
      }

      CSBase = gAddrData->CSBASE[LOC_2][Chipselect];
      CSMask = gAddrData->CSMASK[LOC_2][Chipselect];
      CSBasesec = gAddrData->CSBASESEC[LOC_2][Chipselect];
      CSMasksec = gAddrData->CSMASKSEC[LOC_2][Chipselect];
      CSBaseExt = gAddrData->EXT_CSBASE[LOC_2][Chipselect];
      CSMaskExt = gAddrData->EXT_CSMASK[LOC_2][Chipselect];
      CSBaseExtsec = gAddrData->EXT_CSBASESEC[LOC_2][Chipselect];
      CSMaskExtsec = gAddrData->EXT_CSMASKSEC[LOC_2][Chipselect];

      cs_pos = (SEC==0) ? ((((UINT64)(CSBase!=0 ? (((((UINT64)(CSBaseExt))<<32)| (CSBase)) & ((((UINT64)(~CSMaskExt))<<32)| (~CSMask))): (((((UINT64)(~CSMaskExt))<<32)| (~CSMask)))))<<8)) : \
                                                  (((((UINT64)(CSBasesec!=0 ? (((((UINT64)(CSBaseExtsec))<<32)| (CSBasesec)) & ((((UINT64)(~CSMaskExtsec))<<32)| (~CSMasksec))) : \
                                                                              (((((UINT64)(~CSMaskExtsec))<<32)| (~CSMasksec))))))<<8)) ;//40b version where "1" is in cs position
      bit_pos_40b(cs_pos, &fpos, &spos);

      //before we send the components, we need to rmhash the rm parts. and rmhash considers cs
      rankmul_temp = 0;
      uninvertmsbs_row(row, numrowlobits, invertmsbse, invertmsbso, SEC, &temp_row, cs_num  );
      unhash_bank (bank, addrhashbank0, addrhashbank1, addrhashbank2, addrhashbank3, addrhashbank4, &temp_bank, col, temp_row, numbankbits );
      if((addrhashrmaddr0 & 1) && (noofrm >=1)){
        get_fake_bank_row_col_rm (addrhashrmaddr0, numbankbits, bank0, bank1, bank2, bank3, bank4, row_lo0, numrowlobits, numcsbits, rm0, rm1, rm2,
                                  numcolbits, COL0REG, COL1REG, &fk_bank, &fk_col, &fk_row, &fk_rm );

       if(spos==0)
         rankmul_temp = (rankmul &0x1) ^ (internal_bit_wise_xor(fk_bank & temp_bank)) ^ (internal_bit_wise_xor(fk_row & temp_row)) ^ (internal_bit_wise_xor(fk_col & col)) ^ \
                          (internal_bit_wise_xor(fk_rm & rankmul)) ^ (((((addrhashrmaddr0>>1)<<9)>>fpos) & 0x1) & (Chipselect & 0x1)) ;
       else
         rankmul_temp = (rankmul &0x1) ^ (internal_bit_wise_xor(fk_bank & temp_bank)) ^ (internal_bit_wise_xor(fk_row & temp_row)) ^ (internal_bit_wise_xor(fk_col & col)) ^ \
                          (internal_bit_wise_xor(fk_rm & rankmul)) ^ (((((addrhashrmaddr0>>1)<<9)>>fpos) & 0x1) & (Chipselect & 0x1)) ^ \
                          (((((addrhashrmaddr0>>1)<<9)>>spos) & 0x1) & (Chipselect>>1 & 0x1));
      }
      else
      {
        rankmul_temp = rankmul & 1;
      }


      if((addrhashrmaddr1 & 1) && (noofrm >=2) ){
        get_fake_bank_row_col_rm (addrhashrmaddr1, numbankbits, bank0, bank1, bank2, bank3, bank4, row_lo0, numrowlobits, numcsbits, rm0, rm1, rm2,
                                  numcolbits, COL0REG, COL1REG, &fk_bank, &fk_col, &fk_row, &fk_rm );
       if(spos==0)
         rankmul_temp = (((rankmul>>1)&0x1) ^ (internal_bit_wise_xor(fk_bank & temp_bank)) ^ (internal_bit_wise_xor(fk_row & temp_row)) ^ (internal_bit_wise_xor(fk_col & col)) ^ \
                          (internal_bit_wise_xor(fk_rm & rankmul)) ^ (((((addrhashrmaddr1>>1)<<9)>>fpos) & 0x1) & (Chipselect & 0x1))  )<<1 | rankmul_temp;
       else
         rankmul_temp = (((rankmul>>1)&0x1) ^ (internal_bit_wise_xor(fk_bank & temp_bank)) ^ (internal_bit_wise_xor(fk_row & temp_row)) ^ (internal_bit_wise_xor(fk_col & col)) ^ \
                          (internal_bit_wise_xor(fk_rm & rankmul)) ^ (((((addrhashrmaddr1>>1)<<9)>>fpos) & 0x1) & (Chipselect & 0x1)) ^ \
                          (((((addrhashrmaddr1>>1)<<9)>>spos) & 0x1) & (Chipselect>>1 & 0x1))  )<<1 | rankmul_temp;
      }
      else
      {
        rankmul_temp = (((rankmul>>1)&0x1)<<1) | rankmul_temp;
      }

      if((addrhashrmaddr2 & 1) && (noofrm >=3)){
        get_fake_bank_row_col_rm (addrhashrmaddr2, numbankbits, bank0, bank1, bank2, bank3, bank4, row_lo0, numrowlobits, numcsbits, rm0, rm1, rm2,
                                  numcolbits, COL0REG, COL1REG, &fk_bank, &fk_col, &fk_row, &fk_rm );
       if(spos==0)
         rankmul_temp = (((rankmul>>2)&0x1) ^ (internal_bit_wise_xor(fk_bank & temp_bank)) ^ (internal_bit_wise_xor(fk_row & temp_row)) ^ (internal_bit_wise_xor(fk_col & col)) ^ \
                          (internal_bit_wise_xor(fk_rm & rankmul)) ^ (((((addrhashrmaddr2>>1)<<9)>>fpos) & 0x1) & (Chipselect & 0x1)) )<<2 | rankmul_temp;
       else
         rankmul_temp = (((rankmul>>2)&0x1) ^ (internal_bit_wise_xor(fk_bank & temp_bank)) ^ (internal_bit_wise_xor(fk_row & temp_row)) ^ (internal_bit_wise_xor(fk_col & col)) ^ \
                          (internal_bit_wise_xor(fk_rm & rankmul)) ^ (((((addrhashrmaddr2>>1)<<9)>>fpos) & 0x1) & (Chipselect & 0x1)) ^ \
                          (((((addrhashrmaddr2>>1)<<9)>>spos) & 0x1) & (Chipselect>>1 & 0x1)) )<<2 | rankmul_temp;

      }
      else
      {
        rankmul_temp = (((rankmul>>2)&0x1)<<2) | rankmul_temp;
      }



      ChannelAddr_G = BankAddrToNormalizedMap(CSBase, CSBaseExt, CSMask, CSMaskExt, Chipselect, bank, row, col, rankmul_temp, subchan, numbankbits, bank4, bank3, bank2, bank1, bank0,
           numrowlobits, numcolbits, row_lo0, COL0REG, COL1REG, numcsbits, rm0, rm1, rm2, chan, invertmsbse, invertmsbso,
           CSMasksec, CSMaskExtsec, CSBasesec, CSBaseExtsec, SEC, addrhashbank0,addrhashbank1,addrhashbank2,addrhashbank3,addrhashbank4,addrhashpc,addrhashpc2, vcmen);

      //printf ("translate_dram_to_norm_addr %d %d %d %d rank%d %d %d col%x %x %x: addr=0x%jx\n",pkg_no,mpu_no,umc_inst_num,umc_chan_num, cs_num, bank, row, col, rankmul, subchan, ChannelAddr_G);

      //self checking
      NormalizedToBankAddrMap(ChannelAddr_G, &TransBank, &TransRow, &TransCol, &TransRankmul, &TransSubchan, numbankbits, bank4, bank3, bank2, bank1, bank0,
      numrowlobits,  numcolbits, row_lo0, COL0REG, COL1REG, numcsbits, rm0, rm1, rm2, chan, invertmsbse, invertmsbso,
      SEC, cs_num, addrhashbank0,addrhashbank1,addrhashbank2,addrhashbank3,addrhashbank4,addrhashpc,addrhashpc2,vcmen);
      //printf ("translate_dram_to_norm_addr self: %d %d %d %d rank is not displayed %d %d col%x %x %x: \n",
      //  pkg_no,mpu_no,umc_inst_num,umc_chan_num, TransBank, TransRow, TransCol, TransRankmul, TransSubchan);


      if ((row != TransRow) || (col != TransCol)||(bank != TransBank) )
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "Expected B/R/C/RM/SUBCH = 0x%x/0x%x/0x%x/0x%x/0x%x and \
            actual B/R/C/RM/SUBCH = 0x%x/0x%x/0x%x/0x%x/0x%x for cs=%d  for umc=%d\n", \
            bank, row, col, rankmul, subchan, TransBank, TransRow, TransCol, TransRankmul, TransSubchan, cs_num, umc_inst_num);
        IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Expected bank/row/col is differnt from actual bank/row/col\n");
      }else
      {
      //Lets check for node dct and cs_num
      //Changed for soc15
      for (cs1 = 0; cs1 < 8; ++cs1) {
      // Obtain the CS Base from D18F2x[1,0][4C:40]
        temp = gAddrData->CSBASE[LOC_2][cs1];
        CSBase1 = temp & 0xffffffff;
        temp = gAddrData->CSBASESEC[LOC_2][cs1];
        CSBase1sec = temp & 0xffffffff;
        temp = gAddrData->EXT_CSBASE[LOC_2][cs1];
        CSBase1Ext = temp & 0xffffffff;
        temp = gAddrData->EXT_CSBASESEC[LOC_2][cs1];
        CSBase1Extsec = temp & 0xffffffff;

        // Obtain the CS Mask from D18F2x[64:60]
        temp = gAddrData->CSMASK[LOC_2][(cs1)];
        CSMask1 = temp & 0xffffffff;
        temp = gAddrData->CSMASKSEC[LOC_2][(cs1)];
        CSMask1sec = temp & 0xffffffff;
        temp =  gAddrData->EXT_CSMASK[LOC_2][(cs1)];
        CSMask1Ext = temp & 0xffffffff;
        temp = gAddrData->EXT_CSMASKSEC[LOC_2][(cs1)];
        CSMask1Extsec = temp & 0xffffffff;

        // Adjust the Channel Addr for easy comparison
        InputAddr = (ChannelAddr_G >> 8) & 0x3ffffffff;
        //RAMCFG now has CSEn1
        temp = gAddrData->CONFIGDIMM[LOC_2][(cs1)];
        CSEn1 = temp & 0x1;
        CSEn1sec = (temp >>24) & 0x1;

        if((CSEn1 && (InputAddr & ((((UINT64)(~CSMask1Ext))<<32)| (~CSMask1))) == (((((UINT64)(CSBase1Ext))<<32)| (CSBase1)) & ((((UINT64)(~CSMask1Ext))<<32)| (~CSMask1)))) || \
           (CSEn1sec && (InputAddr & ((((UINT64)(~CSMask1Extsec))<<32)| (~CSMask1sec))) == (((((UINT64)(CSBase1Extsec))<<32)| (CSBase1sec)) & ((((UINT64)(~CSMask1Extsec))<<32)| (~CSMask1sec))))) {

           Chipselect1=0;
           ADDR=0;
           SEC = (CSEn1sec && ((InputAddr & ((((UINT64)(~CSMask1Extsec))<<32)| (~CSMask1sec))) == \
                   (((((UINT64)(CSBase1Extsec))<<32)| (CSBase1sec)) & ((((UINT64)(~CSMask1Extsec))<<32)| (~CSMask1sec)))) && CSEn1);
           CSMask1_64b = (((UINT64)(CSMask1))<<8)|0xff;
           CSBase1_64b = (((((UINT64)(CSBase1))>>1)<<1)<<8)|0x00;
           CSMask1sec_64b = (((UINT64)(CSMask1sec))<<8)|0xff;
           CSBase1sec_64b = (((((UINT64)(CSBase1sec))>>1)<<1)<<8)|0x00;
           CSMask1_64b = ((UINT64)(((UINT64)CSMask1Ext <<32)|(((UINT64)(CSMask1))))<<8)|0xff;
           CSBase1_64b = ((UINT64)(((UINT64)CSBase1Ext <<32)|((((UINT64)(CSBase1))>>1)<<1))<<8)|0x00;
           CSMask1sec_64b = ((UINT64)(((UINT64)CSMask1Extsec <<32)|(((UINT64)(CSMask1sec))))<<8)|0xff;
           CSBase1sec_64b = ((UINT64)(((UINT64)CSBase1Extsec <<32)|((((UINT64)(CSBase1sec))>>1)<<1))<<8)|0x00;
           ADDR = (SEC==0) ? (ChannelAddr_G & (~(CSBase1_64b & ~CSMask1_64b))) :
                             (ChannelAddr_G & (~(CSBase1sec_64b & ~CSMask1sec_64b)));

           if(gAddrData->ADDRHASHNORMADDR[LOC_2][0] & 0x1)
           {
             Chipselect1 = ((internal_bit_wise_xor((((((UINT64)(gAddrData->EXT_ADDRHASHNORMADDR[LOC_2][0]))<<32)|gAddrData->ADDRHASHNORMADDR[LOC_2][0])>>1) & (ADDR>>9))) ^ (cs1 & 0x1));
           }
           else
           {
             Chipselect1  = (cs1 & 0x1) ;
           }

           if(gAddrData->ADDRHASHNORMADDR[LOC_2][1] & 0x1)
           {
             Chipselect1 =  Chipselect1 | (((internal_bit_wise_xor((((((UINT64)(gAddrData->EXT_ADDRHASHNORMADDR[LOC_2][1]))<<32)|gAddrData->ADDRHASHNORMADDR[LOC_2][1])>>1) & (ADDR>>9))) ^ \
                                            ((cs1>>1) & 0x1))<<1);
           }
           else
           {
             Chipselect1 = Chipselect1 |(((cs1>>1)& 0x1)<<1);
           }
           if(gAddrData->ADDRHASHNORMADDR[LOC_2][2] & 0x1)
           {
             Chipselect1 =  Chipselect1 | ((((internal_bit_wise_xor(((((UINT64)(gAddrData->EXT_ADDRHASHNORMADDR[LOC_2][2]))<<32)|gAddrData->ADDRHASHNORMADDR[LOC_2][2])>>1) & (ADDR>>9))) ^ \
                                            ((cs1>>2) & 0x1))<<2);
           }
           else
           {
             Chipselect1 = Chipselect1 |(((cs1>>2)& 0x1)<<2);
           }

           TransCS = Chipselect1;
           InputAddr =0 ;
           CSMask1 =0;
           CSBase1 =0;
           //cs_rhs=0;
           //inputaddr_lhs=0;
           break;
        }//main eqn
      }//cs1 loop

      if (TransCS != cs_num)
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "Expected CS=0x%x Actual CS =0x%x. Failed!!!...bank=0x%x row=0x%x col=0x%x rankmul=0x%x\n", \
            cs_num, TransCS, bank, row, col, rankmul);
        IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Expected rank is differnt from actual rank \n");
      }

      if (!(addrhashrmaddr0 & 1) && !(addrhashrmaddr1 & 1) && !(addrhashrmaddr2 & 1))
      {
        if (TransRankmul != rankmul)
        {
          IDS_HDT_CONSOLE (MAIN_FLOW, "nohash Expected rankmul=0x%x Actual rankmul =0x%x. Failed!!!\n", rankmul, TransRankmul);
          IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Expected rankmul is differnt from actual rank \n");
        }
      }
      else
      {
        //need to  rmhash or rather un rmhash stuf
        //but before that, we dont want cs to come into play. so we have to 0 out the cs in the address we got since we never used cs in the inital rmhash cal

        rankmul_temp1=0;
        if((gAddrData->ADDRHASHRMADDR[LOC_2][0] & 0x1) && (noofrm >=1))
        {
          rankmul_temp1 = ((internal_bit_wise_xor((gAddrData->ADDRHASHRMADDR[LOC_2][0]>>1) & (ChannelAddr_G>>9))) ^ (TransRankmul & 0x1)) ;
        }
        else
        {
          rankmul_temp1 = TransRankmul & 0x1;
        }

        if((gAddrData->ADDRHASHRMADDR[LOC_2][1] & 0x1) &&  (noofrm >=2))
        {
          rankmul_temp1 =  rankmul_temp1 | (((internal_bit_wise_xor((gAddrData->ADDRHASHRMADDR[LOC_2][1]>>1) & (ChannelAddr_G>>9))) ^ ((TransRankmul>>1) & 0x1))<<1)   ;
        }
        else
        {
          rankmul_temp1 = rankmul_temp1 |(((TransRankmul>>1)& 0x1)<<1);
        }

        if((gAddrData->ADDRHASHRMADDR[LOC_2][2] & 0x1) && (noofrm >=3))
         {
           rankmul_temp1 =  rankmul_temp1 | (((internal_bit_wise_xor((gAddrData->ADDRHASHRMADDR[LOC_2][2]>>1) & (ChannelAddr_G>>9))) ^ ((TransRankmul>>2) & 0x1))<<2)   ;
         }
         else
         {
           rankmul_temp1 = rankmul_temp1 |(((TransRankmul>>2)& 0x1)<<2);
         }

         if(rankmul_temp1 != rankmul)
         {
          IDS_HDT_CONSOLE (MAIN_FLOW, "Expected rankmul=0x%x Actual rankmul =0x%x. Failed!!!\n", rankmul, rankmul_temp1);
          IDS_HDT_CONSOLE (MAIN_FLOW, "The umc=%x, ch=%x, rank=%x, bank=%x, row=%x col=%x and rankmul=%x \n", \
              umc_inst_num, umc_chan_num, cs_num, bank, row, col, rankmul);
         }
         else
         {
           TransRankmul = rankmul_temp1;
         }
      }

    }//else part after b/r/c checking

    return (ChannelAddr_G);
  }//NOT OOR
}

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
// END OF HELPER FUNCTIONS
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------

/*------------------------------------------------------------------
 Function: checkDramHit

 Purpose: Lookup a system address in the DRAM address map.

 Parameters (all are input only)
   sysAddr (ulong)
     The address to be converted.
     The user must remove all VMGuard key indices from the system address.
 Returns:
   The physical CS location for the address in a FabricID format:
      (FabricID & NodeIDMask) >> NodeIDShift: the DF NodeID
      (FabricID & ComponentIDMask): The physical location of the channel
   Note that this FabricID may not match the "dstFabricID" that
   the CS is known as in address maps - for example unused CS fabricIDs can
   be downshifted. The idea is that this function gives back a physical
   CS channel number in a "FabricID" format to make it possible to
   determine the nodeID as well.
   NodeIDMask, NodeIDShift, and ComponentIDMask can be read from DF
   SystemInformation registers.

 Side Effects:
   None:

 *------------------------------------------------------------------*/
UINT32
checkDramHit (
  UINT64  sysAddr
  )
{
  UINT64  dramBaseAddr;
  UINT64  dramLimitAddr;
  UINT64  adjSysAddr;
  UINT64  dramHoleBase;

  UINT32  dramAddressMapRegs[ADDR_MAP_ARRAYSIZE];

  UINT32  moderatorInstanceId;
  UINT32  mapRegNumber;
  UINT32  dfType;
  UINT32  holeEn;
  UINT32  nodeId;
  UINT32  logicalDstFabricId, physicalDstFabricId;

  IDS_HDT_CONSOLE (MAIN_FLOW, "checkDramHit (0x%016lX)\n", sysAddr);

  dfType = determineDfType();
  IDS_HDT_CONSOLE (MAIN_FLOW, "dfType = %d\n", dfType);

  nodeId = 0; // We use nodeID zero to find the map hit.

  // Find a CCM instance ID (if there is one, otherwise a GCM instance ID)
  moderatorInstanceId = findModeratorInstanceId(dfType, nodeId);

  for (mapRegNumber=0; mapRegNumber<getNumAddressMaps(dfType); mapRegNumber++)
  {
    getDramAddressMap(dfType, moderatorInstanceId, nodeId, mapRegNumber, dramAddressMapRegs);
    // IDS_HDT_CONSOLE (MAIN_FLOW, "dramBaseReg=0x%08X, dramLimitReg=0x%08X dramIntlvReg=0x%08X, dramCtlReg=0x%08X\n",
    //         dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET], dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET],
    //         dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET], dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET]);
    if (extractDramAddrRangeValid(dfType, dramAddressMapRegs) == 0)
    {
      continue;
    }
    dramBaseAddr = extractDramBaseAddr(dfType, dramAddressMapRegs);
    dramLimitAddr = extractDramLimitAddr(dfType, dramAddressMapRegs);
    // IDS_HDT_CONSOLE (MAIN_FLOW, "dramBaseAddr=0x%016lX, dramLimitAddr=0x%016lX\n",
    //         dramBaseAddr, dramLimitAddr);

    // Check if we hit in this address map (address within base and limit)
    if ((sysAddr >= dramBaseAddr) && (sysAddr <= dramLimitAddr))
    {
      // hit, now figure out the DstFabricId.

      // Check that the address is not within the DRAM hole
      holeEn = extractLgcyMmioHoleEn(dfType, dramAddressMapRegs);
      dramHoleBase = getDramHoleBase(dfType);
      // If the address is between DramHoleBase and 4GB (1<<32), then it is an MIMO address
      if (holeEn && (sysAddr < (((UINT64) 1) << 32)) && (sysAddr >= dramHoleBase))
      {
        IDS_HDT_CONSOLE (MAIN_FLOW, "CheckDramHit was given a system address (0x%016lX) that was an MMIO address\n",
                sysAddr);
        ASSERT (FALSE);
      }

      // The rules for which address was used in the hash algorithm changed.
      // It used to be just the raw system address, but is now the system address after any base address
      // and MMIO hole address correction is applied.
      //   DF2:                               The raw system address bits are used
      //   DF3 except 6-channel interleaving: The raw system address bits are used
      //   DF3 6-channel interleaving:        The bases are subtracted first
      //   DF3.5 and later:                   The bases are subtracted first
      adjSysAddr = sysAddr;
      if ((dfType >= DF_TYPE_DF3POINT5) ||
           (decodeDramIntLvNumChan(dfType, dramAddressMapRegs) == INTERLEAVE_MODE_DF3_6CHAN))
      {
        // Account for the DRAM hole - look for the address to be above 4GB
        if (holeEn && (adjSysAddr >= (((UINT64) 1) << 32)))
        {
          // IDS_HDT_CONSOLE (MAIN_FLOW, "dramHoleBase=0x%016lX, adjust=0x%016lX\n",
          //         dramHoleBase, ((((UINT64) 1) << 32) - dramHoleBase));
          adjSysAddr -= ((((UINT64) 1) << 32) - dramHoleBase);
          // IDS_HDT_CONSOLE (MAIN_FLOW, "adjSysAddr=0x%016lX\n", adjSysAddr);
        }

        adjSysAddr -= dramBaseAddr;
        // IDS_HDT_CONSOLE (MAIN_FLOW, "adjSysAddr=0x%016lX\n", adjSysAddr);
      }

      // Get the base FabricID from the address map
      logicalDstFabricId = extractDstFabricId(dfType, dramAddressMapRegs);

      // Translate the address to a (logical) FabricID offset from dstFabricID.
      logicalDstFabricId += getCsLogicalComponentIdFromAddr(dfType, dramAddressMapRegs, adjSysAddr);

      // Convert the component ID part into a physical component ({nodeId, instanceID})
      physicalDstFabricId = convertLogicalFabricIdToPhysicalFabricId(dfType, logicalDstFabricId, dramAddressMapRegs);

      IDS_HDT_CONSOLE (MAIN_FLOW, "CheckDramHit completed: System address 0x%016lX hits at physical CS FabricID=0x%x (logical 0x%x)\n",
              sysAddr, physicalDstFabricId, logicalDstFabricId);

      return (physicalDstFabricId);
    }
  }

  // missed in address maps.
  IDS_HDT_CONSOLE (MAIN_FLOW, "CheckDramHit completed: SysAddr 0x%016lX missed DRAM maps\n",
          sysAddr);
  ASSERT (FALSE);
  return (0);
}

/*------------------------------------------------------------------
 Function: calcNormAddr

 Purpose: Normalize a system address into a memory controller
   (normalized) system address.

 Parameters (all are input only)
   sysAddr (ulong)
     The address to be converted.
     The user must remove all VMGuard key indices from the system address.
 Returns:
   A normalized address (ulong)

 Side Effects:
   None:

 *------------------------------------------------------------------*/
NORMALIZED_ADDRESS
calcNormAddr (
  UINT64 sysAddr
  )
{
  UINT64  hiAddrOffset;
  UINT64  dramBaseAddr;
  UINT64 dramLimitAddr;
  UINT64  dramHoleBase;
  UINT64  normAddr = 0;

  UINT32  dramAddressMapRegs[ADDR_MAP_ARRAYSIZE];
  UINT32  dramOffsetReg;

  BOOLEAN performCsNormalization;

  UINT32  nodeId;
  UINT32  csInstanceId;
  UINT32  moderatorInstanceId;
  UINT32  csFabricId;
  INT32   mapRegNumber;
  UINT32  holeEn;
  //UINT32  hiAddrOffsetEn;
  UINT32  dfType;
  UINT32 fabricBlockInstanceInformation0Reg;
  UINT32 instanceSubType;
  BOOLEAN isDGpu, isCmp;

  NORMALIZED_ADDRESS    normalizedAddress;

  IDS_HDT_CONSOLE (MAIN_FLOW, "calcNormAddr (0x%016lX)\n", sysAddr);

  // Detect some information about the system
  dfType = determineDfType();
  isDGpu = isSystemDiscreteGpu(dfType, 0);

  // Find where this address maps
  csFabricId = checkDramHit(sysAddr);
  if (0xffff != csFabricId) {
    //IDS_HDT_CONSOLE (MAIN_FLOW, "calcNormAddr: csFabricId = %08x\n", csFabricId);
  } else {
    //IDS_HDT_CONSOLE (MAIN_FLOW, "calcNormAddr: checkDramHit return with failure --> system address does not locate in DRAM, fill normalizedAddr = 0xffffffff_ffffffff and return\n");
    normalizedAddress.normalizedAddr = 0xffffffffffffffff;
    normalizedAddress.normalizedSocketId = 0;
    normalizedAddress.normalizedDieId = 0;
    normalizedAddress.normalizedChannelId = 0;
    normalizedAddress.reserved = 0;

    return (normalizedAddress);
  }

  // Note that checkDramHit really returns {nodeID, channelNumber} in a FabricID format.
  // So the low order bits are really the physical channel number, and the physical channel number maps one to one to the instanceID
  // So grab the CS instance ID from the checkDramHit results
  csInstanceId = csFabricId & getComponentIdMask(dfType);

  // Grab the node ID from the checkDramHit results
  nodeId = (csFabricId & getNodeIdMask(dfType)) >> getNodeIdShift(dfType);

  // Check if it hits on DRAM mapping register 0 or 1 in the CS.
  // Special case heterogeneous systems where there is only one map used on the dGPU side (avoids having to find the off-chip CS)
  if ((dfType == DF_TYPE_DF3POINT5) && !isDGpu && (nodeId > 0))
  {
    // Here we just use the moderator maps on node 0 to determine it and know that the CS has a offset of zero.
    // All the other information is usable in the moderator maps for finding the normalized address.
    moderatorInstanceId = findModeratorInstanceId(dfType, 0);
    mapRegNumber = findMapRegBySysAddr(dfType, 0, sysAddr);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Using moderatorInstanceId=%d, mapRegNumber=%d for heterogeneous access\n", moderatorInstanceId, mapRegNumber);
    getDramAddressMap(dfType, moderatorInstanceId, 0, mapRegNumber, dramAddressMapRegs);
    mapRegNumber = -1;
    isCmp = FALSE; // not supported on heterogeneous systems
  }
  else
  {
    getDramAddressMap(dfType, csInstanceId, nodeId, 1, dramAddressMapRegs);
    dramBaseAddr = extractDramBaseAddr(dfType, dramAddressMapRegs);
    if (extractDramAddrRangeValid(dfType, dramAddressMapRegs) && (sysAddr >= dramBaseAddr))
    {
      mapRegNumber = 1;
    }
    else
    {
      mapRegNumber = 0;
      getDramAddressMap(dfType, csInstanceId, nodeId, mapRegNumber, dramAddressMapRegs);
    }
    fabricBlockInstanceInformation0Reg = getDfRegFabricBlkInstInfo0(dfType, csInstanceId, nodeId);
    instanceSubType = getBits(DF__INSTANCE_SUBTYPE_BITPOS_LO, DF__INSTANCE_SUBTYPE_BITPOS_HI, fabricBlockInstanceInformation0Reg);
    if (instanceSubType == DF__CSCMP_INSTANCE_SUBTYPE_VALUE)
    {
      isCmp = TRUE;
    }
    else
    {
      isCmp = FALSE;
    }
  }

  ASSERT (extractDramAddrRangeValid(dfType, dramAddressMapRegs) == 1);

  performCsNormalization = TRUE;

  if (performCsNormalization)
  {
    dramBaseAddr = extractDramBaseAddr(dfType, dramAddressMapRegs);
    dramLimitAddr = extractDramLimitAddr(dfType, dramAddressMapRegs);
    IDS_HDT_CONSOLE (MAIN_FLOW, "dramBaseAddr=0x%016lX, dramLimitAddr=0x%016lX\n",
                         dramBaseAddr, dramLimitAddr);

    holeEn = extractLgcyMmioHoleEn(dfType, dramAddressMapRegs);
    dramHoleBase = getDramHoleBase(dfType);
    //IDS_HDT_CONSOLE (MAIN_FLOW, "holeEn = %d, dramHoleBase = 0x%016lX\n", holeEn, dramHoleBase);

    // Account for the DRAM hole
    if (holeEn && (sysAddr >= dramHoleBase))
    {
      // IDS_HDT_CONSOLE (MAIN_FLOW, "dramHoleBase = 0x%016lX, adjust = 0x%016lX\n",
      //         dramHoleBase, ((((UINT64) 1) << 32) - dramHoleBase));
      normAddr = sysAddr - ((((UINT64) 1) << 32) - dramHoleBase);
      // IDS_HDT_CONSOLE (MAIN_FLOW, "post hole address=0x%016lX\n", normAddr);
    }
    else
    {
      normAddr = sysAddr;
    }

    // Subtract the base.
    normAddr = normAddr - dramBaseAddr;
    // IDS_HDT_CONSOLE (MAIN_FLOW, "post base address=0x%016lX\n", normAddr);

    // Now remove the bits and normalize...
    normAddr = normalizeAddr(dfType, dramAddressMapRegs, normAddr);

    // Add the offset, skip if it is known to be zero in heterogeneous systems
    if (mapRegNumber > 0)
    {
      dramOffsetReg = getDfRegDramOffset(dfType, csInstanceId, nodeId, mapRegNumber);
      //hiAddrOffsetEn = getBit(DF__HI_ADDR_OFFSET_EN_BITPOS, dramOffsetReg);
      hiAddrOffset = extractDramOffset(dfType, dramOffsetReg);
      // IDS_HDT_CONSOLE (MAIN_FLOW, "hiAddrOffset=0x%016lX\n", hiAddrOffset);
      normAddr = normAddr + hiAddrOffset;
    }
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "CalcNormAddr completed: System address 0x%016lX maps to normalized address=0x%016lX at CS FabricID=0x%x\n",
          sysAddr, normAddr, csFabricId);

  // Add the offset
  normalizedAddress.normalizedAddr = normAddr;
  normalizedAddress.normalizedSocketId = (UINT8) ((csFabricId & getSocketIdMask (dfType)) >> getSocketIdShift (dfType));
  normalizedAddress.normalizedDieId = (UINT8) ((csFabricId & getDieIdMask (dfType)) >> getDieIdShift (dfType));
  normalizedAddress.normalizedChannelId = (UINT8) (csFabricId & ~(getSocketIdMask (dfType) | getDieIdMask (dfType)));
  normalizedAddress.reserved = 0;

  return (normalizedAddress);
}

/*------------------------------------------------------------------
 Function: calcSysAddr

 Purpose: Denormalize an address from a memory controller into a
   system address.

   A normalized address comes from a memory controller that is on
   a particular socket and die, as well as a physical channel number
   (the UMC instance).

   This routine takes the location of the memory controller
   (socket+die+physical number) and converts the normalized
   address to a system adress.

 Parameters (all are input only)
   normAddr (ulong)
     The address to be converted
   addrSocketNum (uint):
     The socket number within the system where the memory channel is
     (i.e. where the normalized address was observed).
     Valid range:
      ZP/SSP/GN/RS/: 0-1
      BA: 0, 4-7
	  RMB: 0
   addrDieNum (uint):
     The die number within the socket where the memory channel is
     (i.e. where the normalized address was observed).
     Note: this is not related to any CCD die numbers.
     Valid range:
      ZP: 0-3
      SSP/GN/RS: 0
      BA: 0-1 (1 only when socketNum is 4-7).
	  RMB: 0
   umcPhysChannelNum (uint):
     This channel number within the die where this normalized
     address was found.
     Valid range:
      ZP: 0-1
      SSP/GN/BA: 0-7
     Note, that this is memory channel number from 0-n.
     It is both the physical UMC ID as well as the
     coherent station instanceID (UMCn is always attached to CSn).
     However, it may not be the CS fabricID as DF supports the
     ability to remap.
     Note that to convert an external pin name (e.g. channel A-H)
     to a UMC number, the table in the PPR must be consulted.
     It is not a consecutive (e.g. A=0, B=1, H=7) map!!!
 Returns:
   A system address (ulong)

 Side Effects:
   None:

 Limitations:
   - The system address returned will not have any VMGuard key information
   - When the UMC address is being used for system functions
     (e.g. PSP private area, CC6 save address), the algorithm will
     give you the system address - which is actually a UEFI reserved
     region in memory.

 *------------------------------------------------------------------*/
UINT64
calcSysAddr (
  UINT64  normAddr,
  UINT32  addrSocketNum,
  UINT32  addrDieNum,
  UINT32  umcPhysChannelNum
  )
{
  UINT64  hiAddrOffset;
  UINT64  dramBaseAddr;
  UINT64  dramLimitAddr;
  UINT64  dramHoleBase;
  UINT64  sysAddr;

  UINT32 dramAddressMapRegs[ADDR_MAP_ARRAYSIZE];
  UINT32 dramOffsetReg;

  UINT32  nodeIdShift;
  UINT32  socketIdShift;
  UINT32  socketIdMask;
  UINT32  dieIdMask;
  UINT32  dieIdShift;
  //UINT32  numDFInstances;
  UINT32  nodeId;
  UINT32  csInstanceId;
  UINT32  csFabricId;
  UINT32  ccmInstanceId;
  INT32   mapRegNumber;
  UINT32  intLvMode;
  UINT32  holeEn;
  UINT32  hiAddrOffsetEn;
  UINT32  dfType;
  BOOLEAN isDGpu;

  // Detect some information about the system
  dfType = determineDfType();
  isDGpu = isSystemDiscreteGpu(dfType, 0);

  //numDFInstances = getBits(DF__BLOCK_INSTANCE_COUNT_BITPOS_LO, DF__BLOCK_INSTANCE_COUNT_BITPOS_HI, getDfRegFabricBlkInstanceCnt(0));
  socketIdShift = getSocketIdShift(dfType);
  socketIdMask = getSocketIdMask(dfType);
  dieIdShift = getDieIdShift(dfType);
  dieIdMask = getDieIdMask(dfType);
  nodeIdShift = getNodeIdShift(dfType);

  nodeId = (((addrSocketNum << socketIdShift) & socketIdMask) | ((addrDieNum << dieIdShift) & dieIdMask)) >> nodeIdShift;

  IDS_HDT_CONSOLE (MAIN_FLOW, "calcSysAddr (0x%016lX, nodeId=%d, channel=%d)\n",
          normAddr, nodeId, umcPhysChannelNum);

  //IDS_HDT_CONSOLE (MAIN_FLOW, "socketIdShift=%d, socketIdMask=0x%x, dieIdShift=%d, dieIdMask=0x%x, nodeIdShift=%d,numDFInstances=%d\n",
  //                     socketIdShift, socketIdMask, dieIdShift, dieIdMask, nodeIdShift, numDFInstances);

  // Find the matching CS
  // If one was to do this programmatically, it would still require us to have some SOC-specific
  // defines to determine how the CS->UMC is attached). In addition, one would have to be careful
  // of coherent stations that are gated because they are unused (no memory, not valid on the package, etc).
  // However, on all CPU products, the relationship is one-to-one (CS0 maps to UMC0) and furthermore, CS0 is
  // always starting at instance ID 0 in the fabric.
  // For heterogeneous systems, the calling function must account for any swizzle in the non-CPU nodes.
  csInstanceId = umcPhysChannelNum;

  // Read the CS offset registers and determine whether this
  // address was part of base/limit register 0, or 1
  // Special case heterogeneous systems where there is only one map used on the dGPU side (avoids having to find the off-chip CS)
  if ((dfType == DF_TYPE_DF3POINT5) && !isDGpu && (nodeId > 0))
  {
    // No FabricID remapping in heterogeneous systems, CS fabricID is just instanceID and nodeID
    csFabricId = csInstanceId | (nodeId << nodeIdShift);

    // Here we just use the CCM maps on node 0 to determine it and know that the CS has a offset of zero.
    // All the other information is usable in the CCM for finding the normalized address.
    ccmInstanceId = findModeratorInstanceId(dfType, 0);
    mapRegNumber = findMapRegByDstFabricId(dfType, 0, csFabricId);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Using ccmInstanceId=%d, mapRegNumber=%d for heterogeneous access\n", ccmInstanceId, mapRegNumber);
    getDramAddressMap(dfType, ccmInstanceId, 0, mapRegNumber, dramAddressMapRegs);
    hiAddrOffset = 0;
    mapRegNumber = -1;
  }
  else
  {
    // Once you have the CS instance ID, now get the actual FabricID from the information block
    csFabricId = getBits(DF__BLOCK_FABRICID_BITPOS_LO, DF__BLOCK_FABRICID_BITPOS_HI, getDfRegFabricBlkInstInfo3(dfType, csInstanceId, nodeId));
    IDS_HDT_CONSOLE (MAIN_FLOW, "csInstanceId=%d, csFabricId=0x%x\n", csInstanceId, csFabricId);

    dramOffsetReg = getDfRegDramOffset(dfType, csInstanceId, nodeId, 1);
    hiAddrOffsetEn = getBit(DF__HI_ADDR_OFFSET_EN_BITPOS, dramOffsetReg);
    hiAddrOffset = extractDramOffset(dfType, dramOffsetReg);

    // IDS_HDT_CONSOLE (MAIN_FLOW, "hiAddrOffsetEn[1]=%d, hiAddrOffset[1]=0x%016lX\n", hiAddrOffsetEn, hiAddrOffset);
    if (hiAddrOffsetEn && (normAddr >= hiAddrOffset))
    {
      mapRegNumber = 1;
    }
    else
    {
      hiAddrOffset = 0;
      mapRegNumber = 0;
    }
    IDS_HDT_CONSOLE (MAIN_FLOW, "mapRegNumber=%d, hiAddrOffset=0x%016lX\n", mapRegNumber, hiAddrOffset);

    getDramAddressMap(dfType, csInstanceId, nodeId, mapRegNumber, dramAddressMapRegs);
  }
  // IDS_HDT_CONSOLE (MAIN_FLOW, "dramBaseReg=0x%08X, dramLimitReg=0x%08X dramIntlvReg=0x%08X, dramCtlReg=0x%08X\n",
  //         dramAddressMapRegs[ADDR_MAP_ARRAY_BASE_OFFSET], dramAddressMapRegs[ADDR_MAP_ARRAY_LIMIT_OFFSET],
  //         dramAddressMapRegs[ADDR_MAP_ARRAY_INTLV_OFFSET], dramAddressMapRegs[ADDR_MAP_ARRAY_CTL_OFFSET]);

  ASSERT (extractDramAddrRangeValid(dfType, dramAddressMapRegs) == 1);

  intLvMode = decodeDramIntLvNumChan(dfType, dramAddressMapRegs);

  dramBaseAddr = extractDramBaseAddr(dfType, dramAddressMapRegs);
  dramLimitAddr = extractDramLimitAddr(dfType, dramAddressMapRegs);
  IDS_HDT_CONSOLE (MAIN_FLOW, "dramBaseAddr=0x%016lX, dramLimitAddr=0x%016lX\n", dramBaseAddr, dramLimitAddr);

  holeEn = extractLgcyMmioHoleEn(dfType, dramAddressMapRegs);
  dramHoleBase = getDramHoleBase(dfType);
  IDS_HDT_CONSOLE (MAIN_FLOW, "holeEn=%d, dramHoleBase=0x%016lX\n", holeEn, dramHoleBase);

  // Subtract the normalized offset
  sysAddr = normAddr - hiAddrOffset;
  // IDS_HDT_CONSOLE (MAIN_FLOW, "after subtracting offset, sysAddr=0x%016lX\n", sysAddr);

  // Denormalize the address
  sysAddr = deNormalizeAddr(dfType, csFabricId, dramAddressMapRegs, sysAddr);
  // IDS_HDT_CONSOLE (MAIN_FLOW, "post normalization, sysAddr=0x%016lX\n", sysAddr);

  // The rules for which address was used in the hash algorithm changed.
  // It used to be just the raw system address, but is now the system address after any base address
  // and MMIO hole address correction is applied.
  //   DF2:                               The raw system address bits are used (case 1)
  //   DF3 except 6-channel interleaving: The raw system address bits are used (case 1)
  //   DF3 6-channel interleaving:        The bases are subtracted first (case 2)
  //   DF3.5 and later:                   The bases are subtracted first (case 2)
  // For denormalization, case 1 means the bases are added and then the hash is recalculated
  // And case 2 means that the hash is recalculated first and then the bases are added.
  // This test is for case 1
  if ((dfType < DF_TYPE_DF3POINT5) && (intLvMode != INTERLEAVE_MODE_DF3_6CHAN))
  {
    // Add in the base.
    sysAddr = sysAddr + dramBaseAddr;

    // Account for the DRAM hole
    if (holeEn && (sysAddr >= dramHoleBase))
    {
      IDS_HDT_CONSOLE (MAIN_FLOW, "dramHoleBase=0x%016lX, adjust=0x%016lX\n",
                       dramHoleBase, ((((UINT64) 1) << 32) - dramHoleBase));
      sysAddr = sysAddr + ((((UINT64) 1) << 32) - dramHoleBase);
    }
  }
  // IDS_HDT_CONSOLE (MAIN_FLOW, "prior to hashing, sysAddr=0x%016lX\n", sysAddr);

  // Now account for any hashing (corrects the CS ID that was placed in the address if the hash doesnt match)
  sysAddr = deNormHashAddr(dfType, dramAddressMapRegs, sysAddr);
  // IDS_HDT_CONSOLE (MAIN_FLOW, "post hashing, sysAddr=0x%016lX\n", sysAddr);

  // This test is for case 2
  if ((dfType >= DF_TYPE_DF3POINT5) || (intLvMode == INTERLEAVE_MODE_DF3_6CHAN))
  {
    // Add in the base.
    sysAddr = sysAddr + dramBaseAddr;

    // Account for the DRAM hole
    if (holeEn && (sysAddr >= dramHoleBase))
    {
      IDS_HDT_CONSOLE (MAIN_FLOW, "dramHoleBase=0x%016lX, adjust=0x%016lX\n",
                       dramHoleBase, ((((UINT64) 1) << 32) - dramHoleBase));
      sysAddr = sysAddr + ((((UINT64) 1) << 32) - dramHoleBase);
    }
  }
  // IDS_HDT_CONSOLE (MAIN_FLOW, "calculated sysAddr=0x%016lX\n", sysAddr);

  // Check that you didnt go over the limit
  ASSERT (sysAddr <= dramLimitAddr);

  IDS_HDT_CONSOLE (MAIN_FLOW, "CalcSysAddr completed: CS fabricID 0x%x normalized address 0x%016lX mapped to SysAddr 0x%016lX\n",
          csFabricId, normAddr, sysAddr);

  return (sysAddr);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Convert normalized address to chip select, row, column, bank, rankmul
 *
 * ChannelAddr expected to be passed from the caller should be till lsb=0 and only msb=39 is considered
 * no fancy 39:4 version
 * pkg_no: socket number
 * mpu_no: die number
 *
 *----------------------------------------------------------------------------------------*/
VOID
translate_norm_to_dpa (
  UINT64  ChannelAddr,
  UINT8   pkg_no,
  UINT8   mpu_no,
  UINT8   umc_inst_num,
  UINT8   umc_chan_num,
  UINT8   cs_num,
  UINT8   bank,
  UINT32  row,
  UINT16  col,
  UINT8   rankmul,
  UINT64  *Dpa
)
{
  UINTN   channelId;
  UINTN   configdimm;
  UINT8   dimm_num;
  UINTN   bankbits;
  UINTN   rowbits;
  UINTN   rankmulbits;

  // channelId: channel ID of system
  umc_chan_num = 0;
  channelId = convert_to_addr_trans_index(pkg_no, mpu_no, umc_inst_num, umc_chan_num);

  // read out the addrhash* registers here
  dimm_num = cs_num >> 1;
  configdimm = gAddrData->CONFIGDIMM [channelId][dimm_num];

  // bank + bankgroup bits = gAddrData->CONFIGDIMM[channel][0] bit [5:4], 0: 3 bits, 1: 4 bits, 2: 5 bits
  bankbits = ((configdimm >> 4) & 0x3) + 3;
  // row bits = gAddrData->CONFIGDIMM[channel][0] bit [11:8], then plus 10;
  rowbits = ((configdimm >> 8) & 0xF) + 10;
  // rankmul bits = gAddrData->CONFIGDIMM[channel][0] bit [7:6];
  rankmulbits = (configdimm >> 6) & 0x3;

  //*Dpa = (cs_num << (rowbits + rankmulbits + bankbits + 13)) | (row << (rankmulbits + bankbits + 13)) | (rankmul << (bankbits + 13)) | (bank << 13) | (col << 3) | (ChannelAddr & 0x7);
  *Dpa = (((((((((cs_num << rowbits) | row ) << (rankmulbits)) | rankmul) << bankbits) | bank) << 10) | col) << 3) | (ChannelAddr & 0x7);

}

/**
 * RasSmnRead - Read SMN register
 *
 * @param[in] IohcBus     - IOHC (Node) bus number
 * @param[in] SmnAddress  - Register SMN address
 * @param[in] Value       - Pointer to register value
 *
 */
VOID
RasSmnRead (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  )
{
  UINT32    RegIndex;
  UINTN     PciAddress;

  RegIndex = SmnAddress;
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_INDEX_2_BIOS;
  PciWrite32 (PciAddress, RegIndex);
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_DATA_2_BIOS;
  *Value = PciRead32 (PciAddress);
}

VOID
RasSmnWrite (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  )
{
  UINT32    RegIndex;
  UINTN     PciAddress;

  RegIndex = SmnAddress;
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_INDEX_2_BIOS;
  PciWrite32 (PciAddress, RegIndex);
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_DATA_2_BIOS;
  PciWrite32 (PciAddress, *Value);
}

VOID
RasSmnRW (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              AndMask,
  IN       UINT32              OrMask
  )
{
  UINT32    RegValue;

  RasSmnRead (IohcBus, SmnAddress, &RegValue);
  RegValue &= AndMask;
  RegValue |= OrMask;
  RasSmnWrite (IohcBus, SmnAddress, &RegValue);
}

/**
 * RasSmnRead8 - Read SMN register in Byte
 *
 * @param[in] IohcBus     - IOHC (Node) bus number
 * @param[in] SmnAddress  - Register SMN address
 * @param[in] Value8      - Pointer to register value
 *
 */
VOID
RasSmnRead8 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT8               *Value8
  )
{
  UINT32    RegIndex;
  UINTN     PciAddress;

  RegIndex = SmnAddress & 0xFFFFFFFC;
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_INDEX_2_BIOS;
  PciWrite32 (PciAddress, RegIndex);
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_DATA_2_BIOS;
  *Value8 = PciRead8 (PciAddress + SmnAddress & 0x3);
}

VOID
RasSmnWrite8 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT8               *Value8
  )
{
  UINT32    RegIndex;
  UINTN     PciAddress;

  RegIndex = SmnAddress & 0xFFFFFFFC;
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_INDEX_2_BIOS;
  PciWrite32 (PciAddress, RegIndex);
  PciAddress = ((UINTN)IohcBus << 20) + IOHC_NB_SMN_DATA_2_BIOS;
  PciWrite8 (PciAddress + (UINTN) (SmnAddress & 0x3), *Value8);
}

VOID
RasSmnRW8 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT8               AndMask,
  IN       UINT8               OrMask
  )
{
  UINT8    RegValue;

  RasSmnRead8 (IohcBus, SmnAddress, &RegValue);
  RegValue &= AndMask;
  RegValue |= OrMask;
  RasSmnWrite8 (IohcBus, SmnAddress, &RegValue);
}

EFI_STATUS
AcquireNbioSecBusNum (
  IN       UINT8 DfNode,
  IN       UINT8 NbioNumber,
  OUT      UINT8 *NbioSecBusNum
  )
{
  UINTN                             PciAddress;
  UINT16                            VendorID;

  if (MAX_NBIO_PER_DIE > NbioNumber)  {
    PciAddress = ((DfNode + 24) << 15) + 0x000;
    VendorID = PciRead16 (PciAddress);
    if (VendorID != AMD_VENDOR_ID) {
      return EFI_INVALID_PARAMETER;
    }
    *NbioSecBusNum = (UINT8) getDfReg(determineDfType(), RMB_IOMS0_INSTANCE_ID, DfNode, DF_CFGADDRESSCNTL);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Socket %d, NbioSecBusNum = %x\n", DfNode, *NbioSecBusNum);
    return EFI_SUCCESS;
  }

  return EFI_INVALID_PARAMETER;
}

