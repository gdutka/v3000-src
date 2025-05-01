/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#include <PiPei.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/I2cMaster.h>
#include "AmdXgbeWorkaround.h"
#include <AMD.h>
#include <Library/AmdBaseLib.h>
#include <Library/PcdLib.h>
#include <Library/PciLib.h>
#include <Ppi/FabricTopologyServices2Ppi.h>
#include <Ppi/PostBootScriptTable.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <FchPlatform.h>
#include "xGbEI2cLib.h"
#include <Ppi/Stall.h>


//I2C frequency
#define SS_SPEED           (100*1000)     //100kbps
#define PORT_I2C_MUTEX        0x1D080ul

EFI_PEI_STALL_PPI                 *gStallPpi=NULL;

EFI_STATUS
EFIAPI
S3ResumeXGBEWorkaround (
  IN EFI_PEI_SERVICES              **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDescriptor,
  IN VOID                          *Ppi
  );


EFI_PEI_NOTIFY_DESCRIPTOR  mNotifyOnPostScriptList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiPostScriptTablePpiGuid,
  S3ResumeXGBEWorkaround
};



/**
 *---------------------------------------------------------------------------------------
 *
 *  getBitfield
 *
 *  Description:
 *    Read value of bits.
 *
 *  Parameters:
 *    @param[in]      Value       In coming value
 *
 *    @param[in]      *Lsb        Least significant bit to start reading
 *
 *    @param[in]      *Width      How many bits to read
 *
 *    @return         Value       Value after reading bits
 *
 *------------------------------------------------------------------------------------
 **/
STATIC
UINT32
getBitfield (
  IN UINT32 Value,
  IN UINT32 Lsb,
  IN UINT32 Width
  )
{
  UINT32 Mask;
  UINT32 i;

  Mask = 0;
  for (i = 0; i < Width; i++) {
    Mask |= 1 << i;
  }

  Value &= Mask << Lsb;

  return (Value >> Lsb);
}
/**
 *---------------------------------------------------------------------------------------
 *
 *  getBitfieldReg32
 *
 *  Description:
 *    Read value of bits.
 *
 *  Parameters:
 *    @param[in]      Addr       In coming Address
 *
 *    @param[in]      *Lsb        Least significant bit to start reading
 *
 *    @param[in]      *Width      How many bits to read
 *
 *    @return         Value       Value after reading bits
 *
 *------------------------------------------------------------------------------------
 **/
STATIC
UINT32
getBitfieldReg32 (
  IN UINT32 Addr,
  IN UINT32 Lsb,
  IN UINT32 Width
  )
{
  UINT32 Value;

  Value = MmioRead32 (Addr);
  return getBitfield (Value, Lsb, Width);
}

/*********************************************************************************
 * Name: XgbeI2cInit
 *
 * Description
 *
 *
 * Input
 *   XgbeData : XGBE data to store base address for each XGBE port
 *
 * Output
 *
 *********************************************************************************/
VOID
XgbeI2cInit (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN  XGBE_DATA *XgbeData,
  AMD_XGBE_I2C_MASTER_PRIVATE *I2cPrivate
  )
{
  //
  // I2C registers for ethernet is mapped to BAR0 @offset 1E000
  //

  I2cPrivate->I2cMasterPpi[0].I2cSdpAddress = XgbeData->Xgmac0BaseAdd + 0x1E000;
  I2cPrivate->I2cMasterPpi[1].I2cSdpAddress = XgbeData->Xgmac1BaseAdd + 0x1E000;

  I2cInit (&I2cPrivate->I2cMasterPpi[0]);
  I2cInit (&I2cPrivate->I2cMasterPpi[1]);

}
VOID
GetI2cOwnership (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN       UINT32 XgmacBase,
  IN       UINT8 MutexId
  )
{
  UINT32 MutexVal = 0;
  UINT32 MutexSet1 = 0;
  MutexVal |= MutexId;
  MutexVal <<= 29;
  MutexVal |= 1;

  if (XgmacBase == 0xFFFFFFFF || XgmacBase == 0) {
    DEBUG((DEBUG_INFO, "In GetI2cOwnership, returning with XgmacBase=%x line(%d)\n", XgmacBase, __LINE__));
    return;
  }

  MmioWrite32 (XgmacBase + PORT_I2C_MUTEX, 0x80000000);
  gStallPpi->Stall (PeiServices, gStallPpi, 1000);

  while(MmioRead32 (XgmacBase + PORT_I2C_MUTEX)) {
    DEBUG((DEBUG_INFO, "In GetI2cOwnership, waiting for mutex. line(%d)\n", __LINE__));
  }

  do {
    MmioWrite32 (XgmacBase + PORT_I2C_MUTEX, MutexVal);
    gStallPpi->Stall (PeiServices, gStallPpi, 1000);
    MutexSet1 = MmioRead32 (XgmacBase + PORT_I2C_MUTEX);
  } while (MutexSet1 != MutexVal);
}


VOID
PutI2cOwnership (
  IN       UINT32 XgmacBase,
  IN       UINT8 MutexId
  )
{
  if (XgmacBase == 0xFFFFFFFF || XgmacBase == 0) {
    DEBUG((DEBUG_INFO, "In PutI2cOwnership, returning with XgmacBase=%x line(%d)\n", XgmacBase, __LINE__));
    return;
  }

  if (getBitfieldReg32 (XgmacBase + PORT_I2C_MUTEX, 29, 2) == MutexId) {
    MmioWrite32 (XgmacBase + PORT_I2C_MUTEX, 0x80000000);
  }
}
/*********************************************************************************
 * Name: XgbeGetBaseAddr
 *
 * Description
 *   Get Base address of XGBE
 *
 * Input
 *   XgbeData : XGBE data to store base address for each XGBE port
 *
 * Output
 *
 *********************************************************************************/
EFI_STATUS
XgbeGetBaseAddr (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  OUT  XGBE_DATA *XgbeData
  )
{
  UINT32 SecBusDie0, port;
  UINT8 BusMasterEn;
  UINT32 Xgmac0_Base, Xgmac1_Base, Xpcs0_Base, Xpcs1_Base;
  UINTN BusNumberBase, SocketNum, DieNum;
  PCI_ADDR PciAddress;
  EFI_STATUS Status = 0;
  AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI *FabricTopologyServices;
  UINT16 CommandValue;
  UINT32 AddressValue;

  Xgmac0_Base = 0xFFFFFFFF;
  Xgmac1_Base = 0xFFFFFFFF;
  Xpcs0_Base = 0xFFFFFFFF;
  Xpcs1_Base = 0xFFFFFFFF;
  BusNumberBase = 0;
  SocketNum = 0;
  DieNum = 0;
  BusMasterEn = 0;
  SecBusDie0 = 0;

  DEBUG((DEBUG_INFO, "In XgbeGetBaseAddr\n"));

  Status = (*PeiServices)->LocatePpi (
                                     PeiServices,
                                     &gAmdFabricTopologyServices2PpiGuid,
                                     0,
                                     NULL,
                                     &FabricTopologyServices
                                     );

  if (FabricTopologyServices->GetRootBridgeInfo (SocketNum, DieNum, 0, NULL, &BusNumberBase, NULL, NULL, NULL, NULL) != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "GetRootBridgeInfo returned Status = %r\n", Status));
    Status = EFI_DEVICE_ERROR;
  }

  AddressValue = (MAKE_SBDFO (0, BusNumberBase, 0x8, FUNC_2, 0x4));
  CommandValue = PciRead16 (AddressValue);

  //Check for first 3-bits of command register for IO/Memory/Bus Master enable
  if( (CommandValue & 0x7) != 0x7)
  {
    CommandValue = (CommandValue | 0x7);
  }

   PciWrite16 (AddressValue, CommandValue);
  DEBUG((DEBUG_INFO, "BusNumberBase = %x  CommandValue:%d\n", BusNumberBase,CommandValue));

  //
  // Ethernet functions are under the Bridge@ Device 0, function2
  //
  PciAddress.AddressValue = (MAKE_SBDFO (0, BusNumberBase, 0x8, FUNC_2, 0x19));
  SecBusDie0 = PciRead8 (PciAddress.AddressValue);

  DEBUG((DEBUG_INFO, "BusNumberBase = %x, SecBusDie0 = %x\n", BusNumberBase, SecBusDie0));

  //
  // for FOX,
  // Port0 => Bus 6, Device 0, Function 2
  // Port1 => Bus 6, Device 0, Function 3
  //
  // Port0, Enable Bus master enable bit
  //
  PciAddress.AddressValue = (MAKE_SBDFO (0, SecBusDie0, 0x0, FUNC_2, 0x4));
  BusMasterEn = PciRead8 (PciAddress.AddressValue);
  BusMasterEn |= 0x6;
  PciWrite8 (PciAddress.AddressValue, BusMasterEn);

  //
  // Get the MAC and the PCS base addresses from the config space
  //
  if(PcdGetBool (PcdXgbeSfp0)){
    PciAddress.AddressValue = (MAKE_SBDFO (0, SecBusDie0, 0x0, FUNC_2, 0x10));
    Xgmac0_Base = PciRead32 (PciAddress.AddressValue);
    PciAddress.AddressValue = (MAKE_SBDFO (0, SecBusDie0, 0x0, FUNC_2, 0x14));
    Xpcs0_Base = PciRead32 (PciAddress.AddressValue);

	DEBUG((DEBUG_INFO, "Xgmac0_Base = %x, Xpcs0_Base = %x\n", Xgmac0_Base, Xpcs0_Base));
  }

  //
  // Port1, Enable the Bus master enable bit
  //
  PciAddress.AddressValue = (MAKE_SBDFO (0, SecBusDie0, 0x0, FUNC_3, 0x4));
  BusMasterEn = PciRead8 (PciAddress.AddressValue);
  BusMasterEn |= 0x6;
  PciWrite8 (PciAddress.AddressValue, BusMasterEn);

  //
  // Get the MAC and PCS base addresses from the config space
  //
  if(PcdGetBool (PcdXgbeSfp1)){
    PciAddress.AddressValue = (MAKE_SBDFO (0, SecBusDie0, 0x0, FUNC_3, 0x10));
    Xgmac1_Base = PciRead32 (PciAddress.AddressValue);
    PciAddress.AddressValue = (MAKE_SBDFO (0, SecBusDie0, 0x0, FUNC_3, 0x14));
    Xpcs1_Base = PciRead32 (PciAddress.AddressValue);

	DEBUG((DEBUG_INFO, "Xgmac1_Base = %x, Xpcs1_Base = %x\n", Xgmac1_Base, Xpcs1_Base));
  }

  XgbeData->Xgmac0BaseAdd = 0;
  XgbeData->Xgmac1BaseAdd = 0;

  //Program for XGBE ECC Interrupt.
  for (port = 0; port < NUMBER_OF_ETH_PORTS; port++){
    if (port == 0) {
      if(Xgmac0_Base == 0xFFFFFFFF)
        continue;
      XgbeData->Xgmac0BaseAdd = Xgmac0_Base;
    }
    if (port == 1) {
      if(Xgmac1_Base == 0xFFFFFFFF)
        break;
      XgbeData->Xgmac1BaseAdd = Xgmac1_Base;
    }
  }
  return (Status);
}

/*********************************************************************************
 * Name: XgbeReadIoExpander
 *
 * Description
 *   Call to Read XGBE I2Cdevices
 *
 * Input
 *   SlaveAdd:      Slave Address of I2C
 *   Offset:        Offset to write to
 *   DataLength:    Size of Data to be Read from device.
 *
 * Output
 *   Data read from device
 *
 *********************************************************************************/
UINT16
XgbeReadIoExpander(
  XGBE_I2C_MASTER_PRIVATE *XgbEI2cMaster,
  UINT8 SlaveAdd,
  UINT8 Offset,
  UINT16 DataLength
  )
{
  UINT8                     PacketBuffer[sizeof(EFI_I2C_REQUEST_PACKET) + sizeof(EFI_I2C_OPERATION)];
  EFI_I2C_REQUEST_PACKET    *RequestPacket;
  EFI_I2C_OPERATION         *Operation;
  EFI_STATUS                Status;
  UINT8                     Data[2];
  UINT16                    Pca9555Data;

  Status =                  EFI_SUCCESS;
  Pca9555Data =             0;
  Data[0] =                 0;
  Data[1] =                 0;

  RequestPacket = (EFI_I2C_REQUEST_PACKET*)PacketBuffer;
  Operation   = RequestPacket->Operation;
  RequestPacket->OperationCount = 2;
  Operation[0].Flags            = 0;
  Operation[0].LengthInBytes    = sizeof(UINT8);
  Operation[0].Buffer           = &Offset;
  Operation[1].Flags            = I2C_FLAG_READ;
  Operation[1].LengthInBytes    = DataLength;
  Operation[1].Buffer           = Data;

  Status = XgbeStartRequest(XgbEI2cMaster, SlaveAdd, RequestPacket);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\nXGMAC: Failed to XgbeStartRequest.\n"));
  }
  Pca9555Data = (Data[1] << 8) | Data[0];
  return Pca9555Data;
}

/*********************************************************************************
 * Name: XgbeWriteIoExpander
 *
 * Description
 *   Call to write to XGBE I2C devices

 *
 * Input
 *   SlaveAdd:     Slave Address of I2C
 *   Offset:       Offset to write to
 *   Data:         Data to be written on device.
 *
 * Output
 *
 *********************************************************************************/

EFI_STATUS
XgbeWriteIoExpander(
  XGBE_I2C_MASTER_PRIVATE *XgbEI2cMaster,
  UINT8 SlaveAdd,
  UINT8 Offset,
  UINT16 Data
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_I2C_REQUEST_PACKET    RequestPacket;
  EFI_I2C_OPERATION         *Operation;
  UINT8                     Buffer[3];

  //Prepare write data buffer
  Buffer[0] = Offset;
  Buffer[1] = (UINT8)(Data & 0x00FF); //Low byte
  Buffer[2] = (UINT8)(Data >> 8);     //High byte

  //Write Byte Operation
  Operation   = RequestPacket.Operation;
  RequestPacket.OperationCount = 1;
  Operation[0].Flags           = 0;
  Operation[0].LengthInBytes   = 3;
  Operation[0].Buffer          = &Buffer[0];

  Status = XgbeStartRequest(XgbEI2cMaster, SlaveAdd, &RequestPacket);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\nXGMAC: Failed to XgbeStartRequest.\n"));
  }
  return Status;
}

/*********************************************************************************
 * Name: XgbeTxDisableLow
 *
 * Description
 *   Programming IO expander PCA9535PWR which is connected to STACKED_SFP+_CONNECTOR.
 *   Setting TX_DISABLE pin to low.
 *
 * Input
 *   XgbeData : XGBE DATA holds the base address of each XGBE port
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/

EFI_STATUS
XgbeTxDisableLow (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  XGBE_DATA *XgbeData,
  AMD_XGBE_I2C_MASTER_PRIVATE *I2cPrivate
  )
{
  UINT16 Data;
  UINT8 SlaveAdd;
  UINT16 DataSize;
  UINT32 XgbeBaseAdd;
  UINT8 Offset;
  UINT16 Port0En = 0;
  UINT16 Port1En = 0;
  XGBE_I2C_MASTER_PRIVATE *XgbeI2cMasterPrivate = NULL;
  EFI_STATUS Status;
  UINT16 TxDisableMask;
  AMD_PBS_SETUP_OPTION   AmdPbsConfiguration;
  UINT16  Timeout ;
  UINT8   PortNum;

  PortNum = 0;
  Timeout = 500;

  DEBUG ((EFI_D_INFO, "In XgbeTxDisableLow Start \n"));
  DEBUG ((EFI_D_INFO, "XgbeData->Xgmac0BaseAdd=%x, XgbeData->Xgmac1BaseAdd=%x line(%d)\n", XgbeData->Xgmac0BaseAdd, XgbeData->Xgmac1BaseAdd, __LINE__));

  Status = EFI_SUCCESS;

Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "XgbeTxDisableLow: GetAmdPbsConfiguration return with=%r\n", Status));
    return Status;
  }
  TxDisableMask = 0xFFFF;
  if(XgbeData->Xgmac0BaseAdd){
    XgbeBaseAdd = XgbeData->Xgmac0BaseAdd;
    Port0En = 1;
    PortNum = 0;
    GetI2cOwnership(PeiServices , XgbeBaseAdd, 0);
   XgbeI2cMasterPrivate = &I2cPrivate->I2cMasterPpi[0];
   if(XgbeData->Xgmac1BaseAdd){
      Port1En = 1;
    }
  }else if(XgbeData->Xgmac1BaseAdd){
    XgbeBaseAdd = XgbeData->Xgmac1BaseAdd;
    GetI2cOwnership(PeiServices , XgbeBaseAdd, 1);
    XgbeI2cMasterPrivate = &I2cPrivate->I2cMasterPpi[1];
    Port1En = 1;
    PortNum = 1;
  }

  SlaveAdd = (UINT8)(PCA9535_ADDRESS + getBitfieldReg32 (XgbeBaseAdd + ENET_PORT_PROPERTY_3, 8, 3));
  DEBUG ((EFI_D_ERROR, "PCA9535 SlaveAdd = 0x%x \n", SlaveAdd));
  DataSize = sizeof (UINT16);

if(AmdPbsConfiguration.XgbePathSelect == 1){ //On board IO expander configuration
  Offset = 6;       // PCA9535 Config register offset
  Data = XgbeReadIoExpander(  XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
  DEBUG ((EFI_D_ERROR, " PCA9535 Config register read data = %x \n", Data));

  // (  0 => output and 1 => input)
  // I/O 0_7 to I/O 0_0 (1,1,1,1,0,0,0,1) (1111 0001 = 0xF1)
  // I/O 1_7 to I/O 1_0 (0,1,1,1,0,1,1,1) (0111 0111 = 0x77)
  Data &= 0x77F1; // This statement make sure output set as 0 properly
  Data |= 0x77F1; // This statement make sure input set as 1 properly
  DEBUG ((EFI_D_ERROR, " PCA9535 Config register before write data = %x \n", Data));
  Status = XgbeWriteIoExpander(XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
  DEBUG ((EFI_D_ERROR, "XgbeWriteEeprom() Status = %r line(%d)\n", Status, __LINE__));

  Data = 0;
  Data = XgbeReadIoExpander(  XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
  DEBUG ((EFI_D_ERROR, " PCA9535 Config register read data = %x \n", Data));

  Offset = 2;       // PCA9535 output port register.
  Data = XgbeReadIoExpander(XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);

  // SFP+_XGB1_DISABLE (I/O 1_3) =>  High:Disable the transmitter  Low : Enable the Transmiter
  // SFP+_XGB0_DISABLE (I/O 1_7) =>  High:Disable the transmitter  Low : Enable the Transmiter
  // Input port value is ignored even if we configure any value.
  Data &= 0x0000;             // Enable  Both SFP's Tx   and Disbale both SFP Power and XGBE Clock
  Data |=  0x2 ;              // I/O0_1 = CLK_156MHZ_EN   Enable XGBE Clock
  if(Port0En){
  	Data |= 0x4; 			  // I/O0_2 = SFP0_POWER_EN   Enable SFP0 Power
  }else {
  	Data |= 0x8000;			  // Disable SFP0 Transmitter
  }
  if(Port1En){
    Data |= 0x8;              // I/O0_3 = SFP1_POWER_EN  Enable SFP1 Power
  }else{
  	Data |= 0x0800;			  // Disable SFP1 Transmitter
  }

  Status = XgbeWriteIoExpander(XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
  DEBUG ((EFI_D_ERROR, "XgbeWriteEeprom() Status = %r line(%d)\n", Status, __LINE__));

  Data = 0;
  Data = XgbeReadIoExpander(XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
  DEBUG ((EFI_D_ERROR, "Xgmac1 PCA9535 Output port register read data = %x \n", Data));
}else if(AmdPbsConfiguration.XgbePathSelect == 2){  //Add on Card Configuratiom
    if(AmdPbsConfiguration.AicTypeSwitch == 0){// H551 AIC IO expander Configuration
      if(AmdPbsConfiguration.Aic1TypeSel == 1){//Retimer Configuration
      Offset = 6;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      Data &=  (~(SFP_00_PWR_CTRL | SFP_01_PWR_CTRL)); // Set as output
      Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
       Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
       DEBUG ((EFI_D_INFO, "1. PCA9555 Configuration Registers Data = %x\n", Data));

      Offset = 2;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      Data |= (SFP_00_PWR_CTRL | SFP_01_PWR_CTRL );// Drive High
      Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "2. PCA9555 Output Registers Data = %x\n", Data));

      Offset = 6;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      Data = Data & (~(EEPROM_WP | SMB_EN )); // Set as output
      Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
      Data = 0;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "3. PCA9555 Configuration Registers Data = %x\n", Data));

      Offset = 2;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      Data = Data & (~(SMB_EN | EEPROM_WP )); // Drive LOW
      Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "4. PCA9555 Output Registers Data = %x\n", Data));


      Offset = 6;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      Data = Data & (~(EN_2_5V )); // Set as output
      Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
      Data = 0;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "5. PCA9555 Configuration Registers Data = %x\n", Data));

      Offset = 2;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      Data = Data & (~(EN_2_5V)); // Drive LOW
      Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "6. PCA9555 Output Registers Data = %x\n", Data));

      MicroSecondDelay(20000);
      Offset = 2;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      Data = Data | EN_2_5V; // Drive High
      Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "7. PCA9555 Output Registers Data = %x\n", Data));

      MicroSecondDelay(20000);
      Offset = 0;
      while (--Timeout) {
        Data = 0;
        Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
        DEBUG ((EFI_D_INFO, "8. PCA9555 Input Port Registers Data = %x\n", Data));
        if ((Data & ALL_DONE) == 0) { // Check for ALL_DONE to go LOW
         break;
        }
      }
      if (Timeout == 0) {
       DEBUG ((EFI_D_ERROR, "9.ALL_DONE to go LOW timed out\n"));
       PutI2cOwnership (XgbeBaseAdd, PortNum);
       return EFI_DEVICE_ERROR;
      }
      MicroSecondDelay(50000);

      Offset = 2;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      Data |= SMB_EN; // Drive HIGH
      Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "10. PCA9555 Outport Port Registers Data = %x\n", Data));
       /** Add on card port End*/

      Offset = 6;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      Data &= TxDisableMask; // Set as output
      Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
      Data = 0;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "PCA9555 Configuration Registers Data = %x\n", Data));

      Offset = 2;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      Data &= (TxDisableMask ); // Set outgoing logic level LOW
      Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
      Data = 0;
      Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "PCA9555 Output Port Registers Data = %x\n", Data));

      }else{  //Inphi PHY configuration
        Data = 0;
        Offset = 6;
        Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
        // I/O 0_7 to I/O 0_0 (  0 => output and 1 => input)
        // (0100 00111 = 0x47)
        // I/O 1_7 to I/O 1_0 (0,0,1,1,0,0,1,1) ( 0011 0011 = 0x33)
        Data &= 0x3347; // This statement make sure output set as 0 properly
        Data |= 0x3347; // This statement make sure input set as 1 properly
        Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
        Data = 0;
        Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
        DEBUG ((EFI_D_INFO, "PCA9555 configuration Registers Data = %x\n", Data));
        Offset = 2;
        Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
        Data &= (TxDisableMask ); // Set outgoing logic level LOW
        Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
        Data = 0;
        Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
        DEBUG ((EFI_D_INFO, "PCA9555 Output Port Registers Data = %x\n", Data));
        if((XgbeData->Xgmac1BaseAdd != XgbeBaseAdd) && (XgbeData->Xgmac1BaseAdd)){
          PutI2cOwnership (XgbeBaseAdd, PortNum);
          XgbeBaseAdd = XgbeData->Xgmac1BaseAdd;
          PortNum = 1;
          TxDisableMask = TxDisableMask & (~XGB01_1_TX_DISABLE);
           GetI2cOwnership(PeiServices , XgbeBaseAdd, 1);
          Port1En = 1;

          Data = 0;
          Offset = 6;
          Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
          // I/O 0_7 to I/O 0_0 (  0 => output and 1 => input)
          // (0100 00111 = 0x47)
          // I/O 1_7 to I/O 1_0 (0,0,1,1,0,0,1,1) ( 0011 0011 = 0x33)
          Data &= 0x3347; // This statement make sure output set as 0 properly
          Data |= 0x3347; // This statement make sure input set as 1 properly
          Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
          Data = 0;
          Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
          DEBUG ((EFI_D_INFO, "PCA9555 configuration Registers Data = %x\n", Data));
          Offset = 2;
          Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
          Data &= (TxDisableMask ); // Set outgoing logic level LOW
          Status = XgbeWriteIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, Data);
          Data = 0;
          Data = XgbeReadIoExpander (XgbeI2cMasterPrivate, SlaveAdd, Offset, DataSize);
          DEBUG ((EFI_D_INFO, "PCA9555 Output Port Registers Data = %x\n", Data));
        }
      }
    }
  }



  if (XgbeData->Xgmac1BaseAdd == XgbeBaseAdd) {
    PutI2cOwnership(XgbeBaseAdd, 1);
  }else{
  	PutI2cOwnership(XgbeBaseAdd, 0);
  }
  DEBUG ((EFI_D_ERROR, "In XgbeTxDisableLow End \n"));
  return Status;
}


/**
  @param  PeiServices                 Indirect reference to the PEI Services Table.
  @param  NotifyDescriptor            Address of the notification descriptor data structure. Type
          EFI_PEI_NOTIFY_DESCRIPTOR is defined above.
  @param  Ppi                         Address of the PPI that was installed.

  @retval   EFI_STATUS                Returns EFI_SUCCESS by default.

**/
EFI_STATUS
EFIAPI
S3ResumeXGBEWorkaround (
  IN EFI_PEI_SERVICES              **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDescriptor,
  IN VOID                          *Ppi
  )
{

  EFI_STATUS   Status = EFI_SUCCESS;
  XGBE_DATA    *XgbeData;
  AMD_XGBE_I2C_MASTER_PRIVATE *I2cPrivate;

  XgbeData = AllocateZeroPool (sizeof (XGBE_DATA));
  if (XgbeData == NULL) {
    DEBUG ((EFI_D_ERROR, "Could not allocate XGBE_DATA structures.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Create the XGBE_I2CMASTER Private Data
  //
  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (AMD_XGBE_I2C_MASTER_PRIVATE),
                             &I2cPrivate
                             );
  ASSERT_EFI_ERROR (Status);
  Status = (**PeiServices).LocatePpi (
                               PeiServices,
                               &gEfiPeiStallPpiGuid,
                               0,
                               NULL,
                               (VOID **)&gStallPpi
                               );
  Status = XgbeGetBaseAddr (PeiServices , XgbeData);
  XgbeI2cInit(PeiServices , XgbeData,I2cPrivate);
  Status = XgbeTxDisableLow (PeiServices , XgbeData,I2cPrivate);
  return Status;
}



/*********************************************************************************
 * Name: AmdXgbeWorkaroundPeiInit
 *
 * Description
 *   Entry point of the AMD XGBE WORK AROUND Pei driver
 *   Perform the required work around needed for XGBE.
 *
 *  @param[in]         FileHandle          Handle of the file being invoked.
 *  @param[in]         PeiServices         Describes the list of possible PEI Services.
 *
 *  @retval            EFI_SUCCESS         Module initialized successfully.
 *  @retval            EFI_ERROR           Initialization failed (see error for more details).
 *
 *********************************************************************************/

EFI_STATUS
EFIAPI
AmdXgbeWorkaroundPeiInit (
  IN EFI_PEI_FILE_HANDLE             FileHandle,
  IN CONST EFI_PEI_SERVICES        **PeiServices
  )
{
  EFI_STATUS   Status = EFI_SUCCESS;
  EFI_BOOT_MODE   BootMode;

  DEBUG ((EFI_D_INFO, "In AmdXgbeWorkaroundPeiInit\n"));

  if(PcdGet8(PcdXgbeDisable))
    return (Status);


  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);

  if (!EFI_ERROR (Status) && (BootMode != BOOT_ON_S3_RESUME)) {
    //
    // Do nothing except S3 Resume
    //
    return EFI_SUCCESS;
  }

  if (!EFI_ERROR (Status) && (BootMode == BOOT_ON_S3_RESUME)) {

    if (PcdGetBool (PcdXgbeSfp0) || PcdGetBool (PcdXgbeSfp1)) {
      Status = (*PeiServices)->NotifyPpi(PeiServices, &mNotifyOnPostScriptList);
      ASSERT_EFI_ERROR (Status);
	}
  }

  return (Status);
}
