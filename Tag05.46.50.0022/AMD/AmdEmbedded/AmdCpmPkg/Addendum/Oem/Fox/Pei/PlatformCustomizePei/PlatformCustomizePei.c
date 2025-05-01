/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/FabricResourceManagerLib.h>
#include <Ppi/AmdCpmTablePpi/AmdCpmTablePpi.h>
#include <AmdCpmBaseIo.h>
#include <Pei/AmdI2cMasterPei.h>
#include <Library/DebugLib.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>
#include <Ppi/Pca9535aPpi.h>
#include <Pei/Pca9535aPei.h>
#include <Ppi/Pca9555aPpi.h>
#include <Pei/Pca9555aPei.h>
#include<AMD.h>
#include <FchPlatform.h>

#define GPIO_PIN_113  0x71
#define GPIO_PIN_114  0x72
#define GPIO_PIN_06   0x6
#define GPIO_PIN_38  0x26
#define GPIO_PIN_39  0x27
#define GPIO_PIN_40  0x28
#define AGPIO3       0x0C

//[-start-240806-IB20840078-add]//
STATIC EFI_PEI_PPI_DESCRIPTOR mPlatformCustomizePeiInitReady = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPlatformCustomizePeiInitReadyPpiGuid,
  NULL
};
//[-end-240806-IB20840078-add]//

EFI_STATUS
ProgramAICPCA9555APWR (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN AMD_PBS_SETUP_OPTION   AmdPbsConfiguration
  );

EFI_STATUS HpiRegAccess(BOOLEAN IsRead, UINT16 Register, VOID *Buf, UINT32 Length, UINT8 ChipID)
{
  EFI_STATUS              Status, Status1;
  UINT8                   PacketBuffer[sizeof(EFI_I2C_REQUEST_PACKET) + sizeof(EFI_I2C_OPERATION)];
  EFI_I2C_REQUEST_PACKET  *RequestPacket;
  EFI_I2C_OPERATION       *Operation;
  UINT8                   Buffer[100];
  EFI_PEI_I2C_MASTER_PPI  *I2cMaster = NULL;

  Status1 = EFI_NOT_READY;

  if (ChipID > 0)
    return EFI_UNSUPPORTED;

  RequestPacket = (EFI_I2C_REQUEST_PACKET*)PacketBuffer;
  Operation   = RequestPacket->Operation;

  if (IsRead) {
    RequestPacket->OperationCount = 2;
    Operation[0].Flags            = 0;
    Operation[0].LengthInBytes    = sizeof(Register);
    Operation[0].Buffer           = (UINT8 *) &Register;
    Operation[1].Flags            = I2C_FLAG_READ;
    Operation[1].LengthInBytes    = Length;
    Operation[1].Buffer           = (UINT8 *)Buf;
  } else {
    if (Length > (sizeof (Buffer) - sizeof (Register))) {
      return EFI_UNSUPPORTED;
    }
    // Prepare write data buffer
    Buffer[0] = Register & 0x00FF; //Low byte
    Buffer[1] = Register >> 8;     //High byte
    CopyMem(Buffer + 2, Buf, Length);

    RequestPacket->OperationCount = 1;
    Operation[0].Flags            = 0;
    Operation[0].LengthInBytes    = sizeof(Register) + Length;
    Operation[0].Buffer           = Buffer;
  }

  Status = PeiServicesLocatePpi (
            &gEfiPeiI2cMasterPpiGuid,
            3, // Bus Number
            NULL,
            &I2cMaster
            );
  if (!EFI_ERROR(Status)) {
    Status1 = I2cMaster->StartRequest(I2cMaster, 0x8, RequestPacket);
  }
  return Status1;
}

VOID
EFIAPI
GetPdFwVersion (VOID)
{
  UINT64 FwVersion = 0;
  EFI_STATUS Status;

  Status = HpiRegAccess(1, 0x0020, &FwVersion, sizeof(FwVersion), 0);
  if (!EFI_ERROR(Status)) {
    PcdSet64S(PcdPdFwVersion, FwVersion);
  }
}

// Define IO Expander Registers
#define PCA9535PWR_U1_ADDR     0x23
#define PCA9535PWR_U117_ADDR   0x21
#define PCA9555_ADDR         0x20
#define CONFIG_REG_OFFSET      0x6
#define OUTPUT_PORT_REG_OFFSET 0x2

/*---------------------------------------------------------------------------------------------
/**
 * ProgramPCA9535PWR  Function to program IO Expander PCA9535PWR
 *
 * This function configures I/O0&1 related pins of I/O Expander and also changes the DP_SEL
 * pin values according to Setup Option change.
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Programmed  PCA9535PWR successfully
 * @retval        EFI_ERROR      Function failed (see error for more details)
 *
 */
EFI_STATUS
ProgramPCA9535PWR (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS             Status = EFI_SUCCESS;
  EFI_PEI_PCA9535A_PPI   *Pca9535aPpi = NULL;
  AMD_PBS_SETUP_OPTION   AmdPbsConfiguration;
  UINT16                 Data16 = 0;

  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gPca9535aPpiGuid,
                           0,
                           NULL,
                           &Pca9535aPpi
                           );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\n Failed to locate EFI_PEI_PCA9535A_PPI :%r \n", Status));
  return Status;
  }

  // Program PCA9535PWR_U1 IO Expander
  DEBUG ((DEBUG_INFO, "Program  PCA9535PWR U1 Configuration Registers \n"));

  // Program Input = 1 and Output = 0 into Configuration Register
  // I/O1_7 to I/O1_0 and I/O0_7 to I/O0_0
  Data16= 0x00FB; // EVAL_SLOT_PWREN, ETHPHY1_PCIE_SW,ETHPHY0_PCIE_SW,ISATA0_PCIE_SW,ISATA1_PCIE_SW,
                  // M2_DT_SW0,M2_DT_SW1,DP1_SEL,TPNL_PWR_EN  are set to Output
  DEBUG ((DEBUG_INFO, "Set PCA9535PWR U1 Config Register Data : %04x\n", Data16));
                           //Arg:PeiServices,BusSelect, SlaveAddress,Offset,Data
  Status = Pca9535aPpi->Set (PeiServices, 3,PCA9535PWR_U1_ADDR , CONFIG_REG_OFFSET, Data16);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\n Failed to Set Configuration Registers: %r \n", Status));
    return Status;
  }
  Data16 = 0;
  DEBUG((DEBUG_INFO, "Program PCA9535PWR U1 Output Port Registers \n"));
                         //Arg:PeiServices,BusSelect, SlaveAddress,Offset,*Data
  Status = Pca9535aPpi->Get (PeiServices, 3, PCA9535PWR_U1_ADDR, OUTPUT_PORT_REG_OFFSET, &Data16);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\n Failed to Get Ouput Port Registers: %r \n", Status));
    return Status;
  }
  DEBUG((DEBUG_INFO, "Data from Ouput Port Registers :%4x \n", Data16));

  Data16 |= 0xFF04; // All Output IO pins set to OUTPUT_HIGH.

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\n GetAmdPbsConfiguration fail : %r \n", Status));
    Status = Pca9535aPpi->Set (PeiServices, 3, PCA9535PWR_U1_ADDR, OUTPUT_PORT_REG_OFFSET, Data16);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "\n Failed to Set Ouput Port Registers: %r \n", Status));
    }
  }
  if (AmdPbsConfiguration.PCIE2X8 == 1) {
    AmdPbsConfiguration.M2PcieSlotSelect = 2;
  }
  if (AmdPbsConfiguration.EvalSlotPowerEnable == 0) {
    DEBUG((DEBUG_INFO, "Clear Output pin(IO0_2)[OUTPUT_LOW] for EVAL_SLOT_PWREN \n"));
    Data16 &= (~BIT2);
  }
  if (AmdPbsConfiguration.PcieSlot1 == 1) {
    DEBUG((DEBUG_INFO, "Clear Output pin(IO1_4 to IO1_7)[OUTPUT_LOW] for PCIE DT SLOT1 \n"));
    Data16 &= (~(BIT12+BIT13+BIT14+BIT15));
  }
  if (AmdPbsConfiguration.M2PcieSlotSelect == 2) {
    DEBUG((DEBUG_INFO, "Clear Output pin(IO1_2 to IO1_3)[OUTPUT_LOW] for PCIE DT SLOT3 \n"));
    Data16 &= (~(BIT10+BIT11));
  }
  if (AmdPbsConfiguration.DP[1] == 1) {
    DEBUG((DEBUG_INFO, "Clear Output pin(IO1_1)[OUTPUT_LOW] for eDP1 \n"));
    Data16 &= (~BIT9);
  }
  if (AmdPbsConfiguration.XgbePathSelect == 2) {
    DEBUG((DEBUG_INFO, "Clear Output pin(IO1_7 and IO1_6)[OUTPUT_LOW] for Add on card \n"));
    Data16 &= (~(BIT14+BIT15));
    ProgramAICPCA9555APWR(PeiServices,AmdPbsConfiguration);
  }
  Status = Pca9535aPpi->Set (PeiServices, 3, PCA9535PWR_U1_ADDR, OUTPUT_PORT_REG_OFFSET, Data16);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\n Failed to Set Ouput Port Registers: %r \n", Status));
  }

  // Program PCA9535PWR_U117 IO Expander
  DEBUG ((DEBUG_INFO, "Program  PCA9535PWR U117 Configuration Registers \n"));

  // Program Input = 1 and Output = 0 into Configuration Register
  // I/O1_7 to I/O1_0 and I/O0_7 to I/O0_0
  Data16= 0x0000; // Configure all IO0 & IO1 pins set to Output
  DEBUG ((DEBUG_INFO, "Set PCA9535PWR U117 Config Register Data : %04x\n", Data16));
                           //Arg:PeiServices,BusSelect, SlaveAddress,Offset,Data
  Status = Pca9535aPpi->Set (PeiServices, 3,PCA9535PWR_U117_ADDR , CONFIG_REG_OFFSET, Data16);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\n Failed to Set Configuration Registers: %r \n", Status));
    return Status;
  }
  Data16 = 0;
  DEBUG((DEBUG_INFO, "Program PCA9535PWR U117 Output Port Registers \n"));
                         //Arg:PeiServices,BusSelect, SlaveAddress,Offset,*Data
  Status = Pca9535aPpi->Get (PeiServices, 3, PCA9535PWR_U117_ADDR, OUTPUT_PORT_REG_OFFSET, &Data16);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\n Failed to Get Ouput Port Registers: %r \n", Status));
    return Status;
  }
  DEBUG((DEBUG_INFO, "Data from Ouput Port Registers :%4x \n", Data16));
  Data16 |= 0xFFFF; // All Output IO pins set to OUTPUT_HIGH.

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\n GetAmdPbsConfiguration fail : %r \n", Status));
    Status = Pca9535aPpi->Set (PeiServices, 3, PCA9535PWR_U117_ADDR, OUTPUT_PORT_REG_OFFSET, Data16);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "\n Failed to Set Ouput Port Registers: %r \n", Status));
    }
  }
  if (AmdPbsConfiguration.DP[4] == 3) {
    DEBUG((DEBUG_INFO, "Clear Output pin(IO0_2)[OUTPUT_LOW] for HDMI1 \n"));
    Data16 &= (~BIT2);
  }
  Status = Pca9535aPpi->Set (PeiServices, 3, PCA9535PWR_U117_ADDR, OUTPUT_PORT_REG_OFFSET, Data16);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\n Failed to Set Ouput Port Registers: %r \n", Status));
  }

  return Status;
}

/**
 This is the declaration of platform customize PEIM entry point.

 @param[in]         FileHandle          Handle of the file being invoked.
 @param[in]         PeiServices         Describes the list of possible PEI Services.

 @retval            EFI_SUCCESS         The operation completed successfully.
 @retval            Others              An unexpected error occurred.
*/
EFI_STATUS
EFIAPI
PlatformCustomizePeiEntryPoint (
  IN EFI_PEI_FILE_HANDLE             FileHandle,
  IN CONST EFI_PEI_SERVICES        **PeiServices
  )
{
  EFI_STATUS                          Status;
  FABRIC_RESOURCE_FOR_EACH_RB        *pFabricResource;
  UINT32                              CpuidFamilyModelStepping;
  UINT32                              Data;
  AMD_PBS_SETUP_OPTION               AmdPbsConfiguration;

  CpuidFamilyModelStepping = 0;

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "PlatformCustomizePeiEntryPoint: Failed to get AmdPbsConfiguration!\n"));
    return Status;
  }

  if (AmdPbsConfiguration.AmdCpmSrisModeDebug == 1) {
    PcdSet8S (PcdSrisCfgType, (PcdGet8 (PcdSrisCfgType) | BIT2));
  } else {
    PcdSet8S (PcdSrisCfgType, (PcdGet8 (PcdSrisCfgType) & ~(BIT2)));
  }

  //
  // Platfrom RAS
  //
  PcdSet32S   (PcdCpmPcieRpCorrectedErrorMask,        (UINT32)  AmdPbsConfiguration.AmdCpmPcieRpCorrErrMask);
  PcdSet32S   (PcdCpmPcieRpUnCorrectedErrorMask,      (UINT32)  AmdPbsConfiguration.AmdCpmPcieRpUnCorrErrMask);
  PcdSet32S   (PcdCpmPcieRpUnCorrectedErrorSeverity,  (UINT32)  AmdPbsConfiguration.AmdCpmPcieRpUnCorrectedErrorSev);
  PcdSet32S   (PcdCpmPcieDevCorrectedErrorMask,       (UINT32)  AmdPbsConfiguration.AmdCpmPcieDevCorrErrMask);
  PcdSet32S   (PcdCpmPcieDevUnCorrectedErrorMask,     (UINT32)  AmdPbsConfiguration.AmdCpmPcieDevUnCorrErrMask);
  PcdSet32S   (PcdCpmPcieDevUnCorrectedErrorSeverity, (UINT32)  AmdPbsConfiguration.AmdCpmPcieDevUnCorrErrorSev);

  GetPdFwVersion();

  //
  // Program PD Interrupt Pin
  //
  ReadMem (ACPI_MMIO_BASE + GPIO_BANK0_BASE + AGPIO3, AccessWidth32, &Data);
  Data = Data | BIT11 | BIT12 | BIT9;
  RwMem (ACPI_MMIO_BASE + GPIO_BANK0_BASE + AGPIO3, AccessWidth32, 0, Data);

  // Program IO Expander PCA9535PWR
  Status = ProgramPCA9535PWR(PeiServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\n ProgramPCA9535PWR Fail : %r \n", Status));
  }

  if (PcdGet8(PcdXgbeDisable) ){
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + GPIO_PIN_06, AccessWidth8, 0, 0x0);
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + GPIO_PIN_40, AccessWidth8, 0, 0x0);
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + GPIO_PIN_38, AccessWidth8, 0, 0x0);
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + GPIO_PIN_39, AccessWidth8, 0, 0x0);
  }

  Status = (*PeiServices)->AllocatePool (
                              PeiServices,
                              sizeof(FABRIC_RESOURCE_FOR_EACH_RB),
                              (VOID **) &pFabricResource
                            );
    if (!EFI_ERROR (Status)) {
        // Above 4G, NonP
        pFabricResource->NonPrefetchableMmioSizeAbove4G[0][0].Size = 0x0;
        pFabricResource->NonPrefetchableMmioSizeAbove4G[0][0].Alignment = 0;

        // Above 4G, PMem
#ifdef LPDDR5
        pFabricResource->PrefetchableMmioSizeAbove4G[0][0].Size = 0x7A9FE00000;
        pFabricResource->PrefetchableMmioSizeAbove4G[0][0].Alignment = 0;
#else
        pFabricResource->PrefetchableMmioSizeAbove4G[0][0].Size = 0x789FE00000;
        pFabricResource->PrefetchableMmioSizeAbove4G[0][0].Alignment = 0;
#endif

        // Below 4G, NonP
        CpmCpuidRead (NULL, &CpuidFamilyModelStepping, NULL);
        if ((CpuidFamilyModelStepping & 0xF0) == 0x00) {
          pFabricResource->NonPrefetchableMmioSizeBelow4G[0][0].Size = 0x20000000;
        } else {
          pFabricResource->NonPrefetchableMmioSizeBelow4G[0][0].Size = 0x60000000;
        }
        pFabricResource->NonPrefetchableMmioSizeBelow4G[0][0].Alignment = 0;

        // Below 4G, PMem
        pFabricResource->PrefetchableMmioSizeBelow4G[0][0].Size = 0;
        pFabricResource->PrefetchableMmioSizeBelow4G[0][0].Alignment = 0;

        // Primayr RootBridge 2nd MMIO
        // if NonP is 0 and PMem is non-zero, all available size would be assigned to PMem
        pFabricResource->PrimaryRbSecondNonPrefetchableMmioSizeBelow4G.Size = 0;
        pFabricResource->PrimaryRbSecondNonPrefetchableMmioSizeBelow4G.Alignment = 1;
        pFabricResource->PrimaryRbSecondPrefetchableMmioSizeBelow4G.Size = 1;
        pFabricResource->PrimaryRbSecondPrefetchableMmioSizeBelow4G.Alignment = 1;

        // IO
        pFabricResource->IO[0][0].Size = 0xF000;
        PcdSet64S (PcdAmdFabricResourceDefaultSizePtr, (UINT64) pFabricResource);
    }
  DEBUG((DEBUG_INFO, "FEA-PEI-%a-InstallPpi-PlatformCustomizePeiInitReady\n", __FUNCTION__));
  //[-start-240806-IB20840078-add]//
  Status = (*PeiServices)->InstallPpi (
                           PeiServices,
                           &mPlatformCustomizePeiInitReady
                           );
  //[-end-240806-IB20840078-add]//
  
  return EFI_SUCCESS;
}

/*---------------------------------------------------------------------------------------------
/**
 * ProgramAICPCA9555APWR  Function to program IO Expander PCA9555PWR For AIC
 *
 * This function configures I/O0&1 related pins of I/O Expander
 *
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 *  @param[in] AmdPbsConfiguration PBS configuration structure
 *
 * @retval        EFI_SUCCESS    Programmed  PCA9535PWR successfully
 * @retval        EFI_ERROR      Function failed (see error for more details)
 *
 */

EFI_STATUS
ProgramAICPCA9555APWR (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN AMD_PBS_SETUP_OPTION   AmdPbsConfiguration
  )
{
  EFI_STATUS             Status = EFI_SUCCESS ;
  EFI_PEI_PCA9555A_PPI   *Pca9555aPpi = (EFI_PEI_PCA9555A_PPI*)NULL ;
  UINT16 Data = 0 ;
  UINT8 Egpio113_value = 0 ;
  UINT8 Egpio114_value = 0;

  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gPca9555aPpiGuid,
                           0,
                           NULL,
                           &Pca9555aPpi
                           );

  ReadMem (ACPI_MMIO_BASE + IOMUX_BASE + GPIO_PIN_113, AccessWidth8, &Egpio113_value);
  ReadMem (ACPI_MMIO_BASE + IOMUX_BASE + GPIO_PIN_114, AccessWidth8, &Egpio114_value);
  DEBUG ((EFI_D_ERROR, "\n back up values of GPIO 113 is %d and GPIO 114 is :%d\n", Egpio113_value,Egpio114_value));
  RwMem (ACPI_MMIO_BASE + IOMUX_BASE + GPIO_PIN_113, AccessWidth8, 0, 0x1);
  RwMem (ACPI_MMIO_BASE + IOMUX_BASE + GPIO_PIN_114, AccessWidth8, 0, 0x1);

  if(AmdPbsConfiguration.AicTypeSwitch == 0){
    DEBUG ((EFI_D_ERROR, "\n H551 is executing ..... \n", Status));
    Status = Pca9555aPpi->Get (PeiServices, 2, PCA9555_ADDR, CONFIG_REG_OFFSET, &Data);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "\n Failed to Get config Port Registers for PCA 9555: %r \n", Status));
      goto End;
    }
    Data &= (~(BIT2+BIT3));
    Status = Pca9555aPpi->Set (PeiServices, 2, PCA9555_ADDR, CONFIG_REG_OFFSET, Data);
    Status = Pca9555aPpi->Get (PeiServices, 2, PCA9555_ADDR, CONFIG_REG_OFFSET, &Data);
    DEBUG ((EFI_D_ERROR, "\n After setting data to PCA9555 config register Data:%X \n", Data));

    Status = Pca9555aPpi->Get (PeiServices, 2, PCA9555_ADDR, OUTPUT_PORT_REG_OFFSET, &Data);
    if(AmdPbsConfiguration.Aic1TypeSel == 0){  //Inphi Phy
      Data |= ((BIT2+BIT3));
    }else{
      Data &= (~(BIT2+BIT3));   //Retimer
    }
    Status = Pca9555aPpi->Set (PeiServices, 2, PCA9555_ADDR, OUTPUT_PORT_REG_OFFSET, Data);
    Status = Pca9555aPpi->Get (PeiServices, 2, PCA9555_ADDR, OUTPUT_PORT_REG_OFFSET, &Data);
    DEBUG ((EFI_D_ERROR, "\n After seting data to PCA9555 o/p register Data :%X \n", Data));

  }else if(AmdPbsConfiguration.AicTypeSwitch != 0){
    Status = Pca9555aPpi->Get (PeiServices, 2, PCA9555_ADDR, CONFIG_REG_OFFSET, &Data);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "\n Failed to Get config Port Registers for PCA 9555: %r \n", Status));
      goto End;
    }
    // make I/O 0_7 , IO 0_6,  I/O 0_01,I/O 0_0 as input
    // I/O 0_2 to I/O 0_5 and I/O 1_7 to I/O 1_0 as ouput
    Data &=  ~0xFF3C ;
    Status = Pca9555aPpi->Set (PeiServices, 2, PCA9555_ADDR, CONFIG_REG_OFFSET, Data);
    Status = Pca9555aPpi->Get (PeiServices, 2, PCA9555_ADDR, CONFIG_REG_OFFSET, &Data);
    DEBUG ((EFI_D_ERROR, "\n After setting data to PCA9555 config register Data:%X \n", Data));

    Status = Pca9555aPpi->Get (PeiServices, 2, PCA9555_ADDR, OUTPUT_PORT_REG_OFFSET, &Data);
    if(AmdPbsConfiguration.Aic2TypeSel == 0 || AmdPbsConfiguration.Aic3TypeSel == 0 ){ //10G PHY enable
      Data &=~(BIT2 | BIT3);
    }else{
      Data |= (BIT2 | BIT3);   //1G PHY enable
    }
    Data |= (BIT4 | BIT5);

    Status = Pca9555aPpi->Set (PeiServices, 2, PCA9555_ADDR, OUTPUT_PORT_REG_OFFSET, Data);
    Status = Pca9555aPpi->Get (PeiServices, 2, PCA9555_ADDR, OUTPUT_PORT_REG_OFFSET, &Data);
    DEBUG ((EFI_D_ERROR, "\n After setting data to PCA9555 o/p register Data :%X \n", Data));

  }

  End:
  //Revert the EPIO 113 and EGPIO 114 values
  RwMem (ACPI_MMIO_BASE + IOMUX_BASE + GPIO_PIN_113, AccessWidth8, 0, Egpio113_value);
  RwMem (ACPI_MMIO_BASE + IOMUX_BASE + GPIO_PIN_114, AccessWidth8, 0, Egpio114_value);
  return Status;

}