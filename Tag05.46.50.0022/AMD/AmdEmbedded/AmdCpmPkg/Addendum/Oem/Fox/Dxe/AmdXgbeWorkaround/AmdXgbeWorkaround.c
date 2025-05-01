/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
//#include <Protocol/AmdBoardIdProtocol.h>
#include "AmdXgbeWorkaround.h"
#include <AMD.h>
#include <Library/AmdBaseLib.h>
#include <Library/PcdLib.h>
#include <Library/PciLib.h>
#include <Protocol/FabricTopologyServices2.h>
#include <Protocol/xGbEI2cMaster.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>

extern  EFI_BOOT_SERVICES       *gBS;
#define PORT_I2C_MUTEX        0x1D080ul

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

VOID
GetI2cOwnership (
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
    return;
  }

  MmioWrite32 (XgmacBase + PORT_I2C_MUTEX, 0x80000000);
  gBS->Stall (1000);

  while(MmioRead32 (XgmacBase + PORT_I2C_MUTEX)){};

  do {
    MmioWrite32 (XgmacBase + PORT_I2C_MUTEX, MutexVal);
    gBS->Stall (1000);
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
  OUT  XGBE_DATA *XgbeData
  )
{
  UINT8 SecBusDie0;
  UINT8  port;
  UINT8 BusMasterEn;
  UINT32 Xgmac0_Base, Xgmac1_Base, Xpcs0_Base, Xpcs1_Base;
  UINTN BusNumberBase, SocketNum, DieNum;
  PCI_ADDR  PciAddress;
  EFI_STATUS Status;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL *FabricTopology;

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
  //
  // Get Base address for XGMAC and  XPCS
  // Get the Base BUS# first.
  //
  Status = gBS->LocateProtocol (
                &gAmdFabricTopologyServices2ProtocolGuid,
                NULL,
                &FabricTopology
                );

  if (FabricTopology->GetRootBridgeInfo (FabricTopology, SocketNum, DieNum, 0, NULL, &BusNumberBase, NULL, NULL, NULL, NULL) != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "GetRootBridgeInfo returned Status = %r\n", Status));
    Status = EFI_DEVICE_ERROR;
  }

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
 *   XgbeBaseAdd:   XGBE Base address.\
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
  UINT32 XgbeBaseAdd,
  UINT8 SlaveAdd,
  UINT8 Offset,
  UINT16 DataLength
  )
{
  XGBE_I2C_MASTER_PROTOCOL  *XgbEI2cMaster;
  UINT8                     PacketBuffer[sizeof(EFI_I2C_REQUEST_PACKET) + sizeof(EFI_I2C_OPERATION)];
  EFI_I2C_REQUEST_PACKET    *RequestPacket;
  EFI_I2C_OPERATION         *Operation;
  EFI_STATUS                Status;
  UINT8                     Data[2];
  UINT16                    Pca9555Data;
  UINT32                    XgmacBaseI2c;

  Status =                  EFI_SUCCESS;
  Pca9555Data =             0;
  Data[0] =                 0;
  Data[1] =                 0;

  //
  // I2C registers for ethernet is mapped to BAR0 @offset 1E000
  //
  XgmacBaseI2c = XgbeBaseAdd + 0x1E000;

  DEBUG((DEBUG_INFO, "XgmacBaseI2c = %x\n", XgmacBaseI2c));
  Status = gBS->LocateProtocol (&gxGbEI2cMasterProtocolGuid, NULL, &XgbEI2cMaster);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\nXGMAC Workaround: Could not Locate I2C Protocol.\n"));
  }

  XgbEI2cMaster->BaseAddress = XgmacBaseI2c;
  XgbEI2cMaster->I2cInit(XgbEI2cMaster);
  RequestPacket = (EFI_I2C_REQUEST_PACKET*)PacketBuffer;
  Operation   = RequestPacket->Operation;
  RequestPacket->OperationCount = 2;
  Operation[0].Flags            = 0;
  Operation[0].LengthInBytes    = sizeof(UINT8);
  Operation[0].Buffer           = &Offset;
  Operation[1].Flags            = I2C_FLAG_READ;
  Operation[1].LengthInBytes    = DataLength;
  Operation[1].Buffer           = Data;

  Status = XgbEI2cMaster->StartRequest(XgbEI2cMaster, SlaveAdd, RequestPacket, NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\nXGMAC: Failed to StartRequest.\n"));
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
 *   XgbeBaseAdd : XGBE Base address.\
 *   SlaveAdd:     Slave Address of I2C
 *   Offset:       Offset to write to
 *   Data:         Data to be written on device.
 *
 * Output
 *
 *********************************************************************************/

EFI_STATUS
XgbeWriteIoExpander(
  UINT32 XgbeBaseAdd,
  UINT8 SlaveAdd,
  UINT8 Offset,
  UINT16 Data
  )
{
  XGBE_I2C_MASTER_PROTOCOL  *XgbEI2cMaster;
  EFI_I2C_REQUEST_PACKET    RequestPacket;
  EFI_I2C_OPERATION         *Operation;
  EFI_STATUS                Status;
  UINT8                     Buffer[3];
  UINT32                    XgmacBaseI2c;
  Status =                  EFI_SUCCESS;

  //
  // I2C registers for ethernet is mapped to BAR0 @offset 1E000
  //
  XgmacBaseI2c = XgbeBaseAdd + 0x1E000;
  Status = gBS->LocateProtocol (&gxGbEI2cMasterProtocolGuid, NULL, &XgbEI2cMaster);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\nXGMAC Workaround: Could not Locate I2C Protocol.\n"));
  }
  XgbEI2cMaster->BaseAddress = XgmacBaseI2c;
  XgbEI2cMaster->I2cInit(XgbEI2cMaster);

  //Prepare write data buffer
  Buffer[0] = Offset;
  Buffer[1] = (UINT8)(Data & 0x00FF); //Low byte
  Buffer[2] = (UINT8)(Data >> 8);     //High byte

  Operation   = RequestPacket.Operation;
  RequestPacket.OperationCount = 1;
  Operation[0].Flags            = 0;
  Operation[0].LengthInBytes    = 3;
  Operation[0].Buffer           = &Buffer[0];

  Status = XgbEI2cMaster->StartRequest(XgbEI2cMaster, SlaveAdd, &RequestPacket, NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\nXGMAC: Failed to StartRequest.\n"));
  }

  return Status;
}
/*********************************************************************************
 * Name: XgbeTxDisableLow
 *
 * Description
 *   Work around to set TX_DISABLE pin to low.
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
  XGBE_DATA *XgbeData
  )
{
  UINT16 Data;
  UINT8 SlaveAdd;
  UINT16 DataSize;
  UINT32 XgbeBaseAdd;
  UINT8 Offset;
  UINT16 Port0En = 0;
  UINT16 Port1En = 0;
  UINT16 TxDisableMask;
  AMD_PBS_SETUP_OPTION   AmdPbsConfiguration;
  UINT16  Timeout ;
  UINT8   PortNum;
  EFI_STATUS Status;

  PortNum = 0;
  Timeout = 500;

  DEBUG ((EFI_D_INFO, "In XgbeTxDisableLow Start \n"));

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
    TxDisableMask = TxDisableMask & (~XGB00_0_TX_DISABLE);
    GetI2cOwnership(XgbeBaseAdd, 0);
    if(XgbeData->Xgmac1BaseAdd){
      Port1En = 1;
    }
  }else if(XgbeData->Xgmac1BaseAdd){
    XgbeBaseAdd = XgbeData->Xgmac1BaseAdd;
    PortNum = 1;
    TxDisableMask = TxDisableMask & (~XGB01_1_TX_DISABLE);
    GetI2cOwnership(XgbeBaseAdd, 1);
    Port1En = 1;
  }

  SlaveAdd = (UINT8)(PCA9535_ADDRESS + getBitfieldReg32 (XgbeBaseAdd + ENET_PORT_PROPERTY_3, 8, 3));
  DEBUG ((EFI_D_ERROR, "PCA9535 SlaveAdd = 0x%x \n", SlaveAdd));
  DataSize = sizeof (UINT16);

  if(AmdPbsConfiguration.XgbePathSelect == 1){ //On board IO expander configuration
    Offset = 6;       // PCA9535 Config register offset
    Data = XgbeReadIoExpander(XgbeBaseAdd, SlaveAdd, Offset, DataSize);
    DEBUG ((EFI_D_ERROR, " PCA9535 Config register read data = %x \n", Data));

    // (  0 => output and 1 => input)
    // I/O 0_7 to I/O 0_0 (1,1,1,1,0,0,0,1) (1111 0001 = 0xF1)
    // I/O 1_7 to I/O 1_0 (0,1,1,1,0,1,1,1) (0111 0111 = 0x77)
    Data &= 0x77F1; // This statement make sure output set as 0 properly
    Data |= 0x77F1; // This statement make sure input set as 1 properly
    DEBUG ((EFI_D_ERROR, " PCA9535 Config register before write data = %x \n", Data));
    XgbeWriteIoExpander(XgbeBaseAdd, SlaveAdd, Offset, Data);

    Data = 0;
    Data = XgbeReadIoExpander(XgbeBaseAdd, SlaveAdd, Offset, DataSize);
    DEBUG ((EFI_D_ERROR, " PCA9535 Config register read data = %x \n", Data));

    Offset = 2;       // PCA9535 output port register.
    Data = XgbeReadIoExpander(XgbeBaseAdd, SlaveAdd, Offset, DataSize);

    // SFP+_XGB1_DISABLE (I/O 1_3) =>  High:Disable the transmitter  Low : Enable the Transmiter
    // SFP+_XGB0_DISABLE (I/O 1_7) =>  High:Disable the transmitter  Low : Enable the Transmiter
    // Input port value is ignored even if we configure any value.
    Data &= 0x0000;             // Enable  Both SFP's Tx   and Disbale both SFP Power and XGBE Clock
    Data |=  0x2 ;              // I/O0_1 = CLK_156MHZ_EN   Enable XGBE Clock
    if(Port0En){
      Data |= 0x4;             // I/O0_2 = SFP0_POWER_EN   Enable SFP0 Power
    }else {
      Data |= 0x8000;          // Disable SFP0 Transmitter
    }
    if(Port1En){
      Data |= 0x8;             // I/O0_3 = SFP1_POWER_EN  Enable SFP1 Power
    }else{
      Data |= 0x0800;         // Disable SFP1 Transmitter
    }

    XgbeWriteIoExpander(XgbeBaseAdd, SlaveAdd, Offset, Data);
  }
  else if(AmdPbsConfiguration.XgbePathSelect == 2){  //Add on Card Configuratiom
    if(AmdPbsConfiguration.AicTypeSwitch == 0){// H551 AIC IO expander Configuration
      if(AmdPbsConfiguration.Aic1TypeSel == 1){//Retimer Configuration
      Offset = 6;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      Data &=  (~(SFP_00_PWR_CTRL | SFP_01_PWR_CTRL)); // Set as output
      Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
       Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
       DEBUG ((EFI_D_INFO, "1. PCA9555 Configuration Registers Data = %x\n", Data));

      Offset = 2;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      Data |= (SFP_00_PWR_CTRL | SFP_01_PWR_CTRL );// Drive High
      Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "2. PCA9555 Output Registers Data = %x\n", Data));

      Offset = 6;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      Data = Data & (~(READ_EN | SMB_EN ));  // Set as output
      Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
      Data = 0;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "3. PCA9555 Configuration Registers Data = %x\n", Data));

      Offset = 2;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      Data = Data & (~(SMB_EN | READ_EN )); // Drive LOW
      Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "4. PCA9555 Output Registers Data = %x\n", Data));


      Offset = 6;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      Data = Data & (~(EN_2_5V )); // Set as output
      Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
      Data = 0;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "5. PCA9555 Configuration Registers Data = %x\n", Data));

      Offset = 2;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      Data = Data & (~(EN_2_5V)); // Drive LOW
      Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "6. PCA9555 Output Registers Data = %x\n", Data));

      gBS->Stall(100000);
      Offset = 2;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      Data = Data | EN_2_5V; // Drive High
      Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "7. PCA9555 Output Registers Data = %x\n", Data));

      Offset = 0;
      while (--Timeout) {
        Data = 0;
        Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
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
      gBS->Stall(50000);

      Offset = 2;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      Data |= SMB_EN; // Drive HIGH
      Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "10. PCA9555 Outport Port Registers Data = %x\n", Data));
       /** Add on card port End*/

      Offset = 6;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      Data &=  (~(XGB00_0_TX_DISABLE | XGB01_1_TX_DISABLE)); // Set as output
      Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
      Data = 0;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "PCA9555 Configuration Registers Data = %x\n", Data));

      Offset = 2;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
     Data &=  (~(XGB00_0_TX_DISABLE | XGB01_1_TX_DISABLE)); // Set outgoing logic level LOW
      Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
      Data = 0;
      Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
      DEBUG ((EFI_D_INFO, "PCA9555 Output Port Registers Data = %x\n", Data));

      }else{  //Inphi PHY configuration
        Data = 0;
        Offset = 6;
        Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
        // I/O 0_7 to I/O 0_0 (  0 => output and 1 => input)
        // (0100 00111 = 0x47)
        // I/O 1_7 to I/O 1_0 (0,0,1,1,0,0,1,1) ( 0011 0011 = 0x33)
        Data &= 0x3347; // This statement make sure output set as 0 properly
        Data |= 0x3347; // This statement make sure input set as 1 properly
        Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
        Data = 0;
        Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
        DEBUG ((EFI_D_INFO, "PCA9555 configuration Registers Data = %x\n", Data));
        Offset = 2;
        Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
        Data &= (TxDisableMask ); // Set outgoing logic level LOW
        Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
        Data = 0;
        Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
        DEBUG ((EFI_D_INFO, "PCA9555 Output Port Registers Data = %x\n", Data));
        if((XgbeData->Xgmac1BaseAdd != XgbeBaseAdd) && (XgbeData->Xgmac1BaseAdd)){
          PutI2cOwnership (XgbeBaseAdd, PortNum);
          XgbeBaseAdd = XgbeData->Xgmac1BaseAdd;
          PortNum = 1;
          TxDisableMask = TxDisableMask & (~XGB01_1_TX_DISABLE);
          GetI2cOwnership(XgbeBaseAdd, 1);
          Port1En = 1;

          Data = 0;
          Offset = 6;
          Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
          // I/O 0_7 to I/O 0_0 (  0 => output and 1 => input)
          // (0100 00111 = 0x47)
          // I/O 1_7 to I/O 1_0 (0,0,1,1,0,0,1,1) ( 0011 0011 = 0x33)
          Data &= 0x3347; // This statement make sure output set as 0 properly
          Data |= 0x3347; // This statement make sure input set as 1 properly
          Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
          Data = 0;
          Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
          DEBUG ((EFI_D_INFO, "PCA9555 configuration Registers Data = %x\n", Data));
          Offset = 2;
          Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
          Data &= (TxDisableMask ); // Set outgoing logic level LOW
          Status = XgbeWriteIoExpander (XgbeBaseAdd, SlaveAdd, Offset, Data);
          Data = 0;
          Data = XgbeReadIoExpander (XgbeBaseAdd, SlaveAdd, Offset, DataSize);
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

/*********************************************************************************
 * Name: AmdXgbeWorkaroundDxeInit
 *
 * Description
 *   Entry point of the AMD XGBE WORK AROUND DXE driver
 *   Perform the required work around needed for XGBE.
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/

EFI_STATUS
EFIAPI
AmdXgbeWorkaroundDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS   Status;
  XGBE_DATA   *XgbeData;

  DEBUG ((EFI_D_INFO, "In AmdXgbeWorkaroundDxeInit\n"));

  Status = EFI_SUCCESS;

  if(PcdGet8(PcdXgbeDisable))
    return (Status);

  if(PcdGetBool (PcdXgbeSfp0) || PcdGetBool (PcdXgbeSfp1)) {

    XgbeData = AllocateZeroPool (sizeof (XGBE_DATA));
    if (XgbeData == NULL) {
      DEBUG ((EFI_D_ERROR, "Could not allocate XGBE_DATA structures.\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    Status = XgbeGetBaseAddr (XgbeData);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "\n XgbeGetBaseAddr failed\n"));
    }
    Status = XgbeTxDisableLow (XgbeData);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "\n XgbeTxDisableLow failed\n"));
    }
  }
  return (Status);
}
