/** @file

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include "BhtHostPei.h"

int                     g_deviceId = 0;
UINT32                  mBar1;

void DbgNull(IN CONST CHAR16 * fmt, ...)
{
}

UINT8
mmio_read8 (
  IN        UINT32          offset
  )
{
  UINT8                    *Address;

  Address = (UINT8 *)(offset);
	return *Address;
}

void
mmio_write8 (
  IN  UINT32          offset,
  IN  UINT8          value
  )
{
  UINT8                    *Address;

  Address = (UINT8 *)(offset);
  *Address = value;
}


UINT32
bht_readl (
  IN        UINT32          offset
  )
{
  UINT32                    *Address;

  Address = (UINT32 *)(mBar1 + offset);
	return *Address;
}

void
bht_writel (
  UINT32 offset,
  UINT32 value
  )
{
  UINT32                    *Address;

  Address = (UINT32 *)(mBar1 + offset);
  *Address = value;
}


UINT32 
PciBhtRead32 (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN       UINT32                     offset
  )
{
  EFI_STATUS Status;
	UINT32 i = 0;
	UINT32 tmp[3] = {0};
  EFI_PEI_STALL_PPI       *StallPpi;

  Status = (**PeiServices).LocatePpi (
                             PeiServices,
                             &gEfiPeiStallPpiGuid,
                             0,
                             NULL,
                             (VOID **)&StallPpi
                             );

	if((g_deviceId == PCI_DEV_ID_SDS0) ||
			(g_deviceId == PCI_DEV_ID_SDS1) ||
			(g_deviceId == PCI_DEV_ID_FJ2) ||
			(g_deviceId == PCI_DEV_ID_SB0) ||
			(g_deviceId == PCI_DEV_ID_SB1))
	{
		// For Sandstorm, HW implement a mapping method by memory space reg to access PCI reg.
		// Enable mapping
	
		// Check function conflict
		if((g_deviceId == PCI_DEV_ID_SDS0) ||
				(g_deviceId == PCI_DEV_ID_FJ2) ||
				(g_deviceId == PCI_DEV_ID_SB0) ||
				(g_deviceId == PCI_DEV_ID_SB1))
		{
			i = 0;
			bht_writel(BHT_PCIRMappingEn, 0x40000000);
			while((bht_readl(BHT_PCIRMappingEn) & 0x40000000) == 0)
			{
				if(i == 5)
				{
					//DbgMsg((DRIVERNAME " - %s() function 0 can't lock!\n", __FUNCTION__));
					goto RD_DIS_MAPPING;
				}
						StallPpi->Stall (PeiServices, StallPpi, 1000);
				i++;
					bht_writel(BHT_PCIRMappingEn, 0x40000000);
	
			}
		}
		else if(g_deviceId == PCI_DEV_ID_SDS1)
		{
			i = 0;
			bht_writel(BHT_PCIRMappingEn, 0x20000000);
			while((bht_readl(BHT_PCIRMappingEn) & 0x20000000) == 0)
			{
				if(i == 5)
				{
					//DbgErr((DRIVERNAME " - %s() function 1 can't lock!\n", __FUNCTION__));
					goto RD_DIS_MAPPING;
				}
				StallPpi->Stall (PeiServices, StallPpi, 1000);
				i++;
				bht_writel(BHT_PCIRMappingEn, 0x20000000);
			}
		}
	
		// Check last operation is complete
		i = 0;
		while(bht_readl(BHT_PCIRMappingCtl) & 0xc0000000)
		{
			if(i == 5)
			{
				//DbgErr((DRIVERNAME " - [204] = 0x%x\n", RegisterRead32(ELN_dPCIRMappingCtl)));
				//DbgErr((DRIVERNAME " - [208] = 0x%x\n", RegisterRead32(ELN_dPCIRMappingEn)));
				//DbgErr((DRIVERNAME " - %s() check last operation complete timeout!!!\n", __FUNCTION__));
				goto RD_DIS_MAPPING;
			}
			StallPpi->Stall (PeiServices, StallPpi, 1000);
			i += 1;
		}
	
		// Set register address
		tmp[0] |= 0x40000000;
		tmp[0] |= offset;
		bht_writel(BHT_PCIRMappingCtl, tmp[0]);
	
		// Check read is complete
		i = 0;
		while(bht_readl(BHT_PCIRMappingCtl) & 0x40000000)
		{
			if(i == 5)
			{
				//DbgErr((DRIVERNAME " - %s() check read operation complete timeout!!!\n", __FUNCTION__));
				goto RD_DIS_MAPPING;
			}
			StallPpi->Stall (PeiServices, StallPpi, 1000);
			i += 1;
		}
	
		// Get PCIR value
		tmp[1] = bht_readl(BHT_PCIRMappingVal);
	
RD_DIS_MAPPING:
		// Disable mapping
		bht_writel(BHT_PCIRMappingEn, 0x80000000);
	
		//DbgDebug(L"%s offset=%x Value:%x\n", __FUNCTION__, offset, tmp[1]);
		return tmp[1];
	}
	
	//DbgDebug(L"%s offset=%x Value:%x\n", __FUNCTION__, offset, tmp[0]);
	return tmp[0];	
}

void 
PciBhtWrite32 (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN       UINT32                     offset,
  IN       UINT32                     value
  )
{
  EFI_STATUS Status;
	UINT32 tmp = 0;
    UINT32 i = 0;
  EFI_PEI_STALL_PPI       *StallPpi;

  //
  // Get StallPpi
  //
  Status = (**PeiServices).LocatePpi (
                             PeiServices,
                             &gEfiPeiStallPpiGuid,
                             0,
                             NULL,
                             (VOID **)&StallPpi
                             );

	if((g_deviceId == PCI_DEV_ID_SDS0) ||
			(g_deviceId == PCI_DEV_ID_SDS1) ||
			(g_deviceId == PCI_DEV_ID_FJ2) ||
			(g_deviceId == PCI_DEV_ID_SB0) ||
			(g_deviceId == PCI_DEV_ID_SB1))
    {
        // For Sandstorm, HW implement a mapping method by memory space reg to access PCI reg.
        // Upper caller doesn't need to set 0xD0.

        // Enable mapping

        // Check function conflict
		if((g_deviceId == PCI_DEV_ID_SDS0) ||
				(g_deviceId == PCI_DEV_ID_FJ2) ||
				(g_deviceId == PCI_DEV_ID_SB0) ||
				(g_deviceId == PCI_DEV_ID_SB1))
        {
            i = 0;
            bht_writel(BHT_PCIRMappingEn, 0x40000000);
            while((bht_readl(BHT_PCIRMappingEn) & 0x40000000) == 0)
            {
                if(i == 5)
                {
                    //DbgErr((DRIVERNAME " - %s() function 0 can't lock!\n", __FUNCTION__));
                    goto WR_DIS_MAPPING;
                }

                StallPpi->Stall (PeiServices, StallPpi, 1000);
                i++;
                bht_writel(BHT_PCIRMappingEn, 0x40000000);
            }
        }
        else if(g_deviceId == PCI_DEV_ID_SDS1)
        {
            i = 0;
            bht_writel(BHT_PCIRMappingEn, 0x20000000);

            while((bht_readl(BHT_PCIRMappingEn) & 0x20000000) == 0)
            {
                if(i == 5)
                {
                    //DbgErr((DRIVERNAME " - %s() function 0 can't lock!\n", __FUNCTION__));
                    goto WR_DIS_MAPPING;
                }

                StallPpi->Stall (PeiServices, StallPpi, 1000);
                i++;
                bht_writel(BHT_PCIRMappingEn, 0x20000000);
            }
        }

        // Enable MEM access
        bht_writel(BHT_PCIRMappingVal, 0x80000000);
        bht_writel(BHT_PCIRMappingCtl, 0x800000D0);

        // Check last operation is complete
        i = 0;
        while(bht_readl(BHT_PCIRMappingCtl) & 0xc0000000)
        {
            if(i == 5)
            {
                //DbgErr((DRIVERNAME " - %s() check last operation complete timeout!!!\n", __FUNCTION__));
                goto WR_DIS_MAPPING;
            }
            StallPpi->Stall (PeiServices, StallPpi, 1000);
            i += 1;
        }

        // Set write value
        bht_writel(BHT_PCIRMappingVal, value);
        // Set register address
        tmp |= 0x80000000;
        tmp |= offset;
        bht_writel(BHT_PCIRMappingCtl, tmp);

        // Check write is complete
        i = 0;
        while(bht_readl(BHT_PCIRMappingCtl) & 0x80000000)
        {
            if(i == 5)
            {
                //DbgErr((DRIVERNAME " - %s() check write operation complete timeout!!!\n", __FUNCTION__));
                goto WR_DIS_MAPPING;
            }
            StallPpi->Stall (PeiServices, StallPpi, 1000);
            i += 1;
        }

WR_DIS_MAPPING:
        // Disable MEM access
        bht_writel(BHT_PCIRMappingVal, 0x80000001);
        bht_writel(BHT_PCIRMappingCtl, 0x800000D0);

        // Check last operation is complete
        i = 0;
        while(bht_readl(BHT_PCIRMappingCtl) & 0xc0000000)
        {
            if(i == 5)
            {
                //DbgErr((DRIVERNAME " - %s() check last operation complete timeout!!!\n", __FUNCTION__));
                break;
            }
            StallPpi->Stall (PeiServices, StallPpi, 1000);
            i += 1;
        }

        // Disable function conflict

        // Disable mapping
        bht_writel(BHT_PCIRMappingEn, 0x80000000);
    }
}

void 
PciBhtOr32 (
 IN CONST EFI_PEI_SERVICES           **PeiServices,
 IN       UINT32                     offset,
 IN       UINT32                     value
  )
{
	UINT32 arg;
	arg = PciBhtRead32(PeiServices, offset);
	PciBhtWrite32(PeiServices, offset, value | arg);
}

void 
PciBhtAnd32 (
 IN CONST EFI_PEI_SERVICES           **PeiServices,
 IN       UINT32                     offset,
 IN       UINT32                     value
  )
{
	UINT32 arg;
	arg = PciBhtRead32(PeiServices, offset);
	PciBhtWrite32(PeiServices, offset, value & arg);
}

/**
  Configure SDHC and install PEI_SD_CONTROLLER_PPI.

  @param [in]   PeiServices              Pointer to the PEI Services Table
  @param [in]   Address                  Target SDHC PCI Cfg Adreess
  @param [in]   BaseAddress              Target SDHC Base Address
  @param [in]   BehindBridge             Whether enabling bridge is required
 
  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
EFI_STATUS
EnableBht (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN       UINT32                     Address
  )
{
  EFI_PEI_PCI_CFG2_PPI      *PciCfgPpi;
  UINT32                    Register;
  UINT8                     Value;
  EFI_PEI_STALL_PPI         *StallPpi;

  //
  // Get StallPpi
  //
  (**PeiServices).LocatePpi (
                    PeiServices,
                    &gEfiPeiStallPpiGuid,
                    0,
                    NULL,
                    (VOID **)&StallPpi
                    );

  PciCfgPpi = (**PeiServices).PciCfg;

  Register = 0;
  PciCfgPpi->Read (
               PeiServices,
               PciCfgPpi,
               EfiPeiPciCfgWidthUint32,
               Address | 0x10, // BAR0
               &Register
               );
  //
  // Assign base address 1
  //
  mBar1 = Register + 0x1000;
  PciCfgPpi->Write (
               PeiServices,
               PciCfgPpi,
               EfiPeiPciCfgWidthUint32,
               Address | 0x14,  // BAR1
               &mBar1
               );

  /* FET on */
  PciBhtOr32 (PeiServices, 0xEC, 0x3);
  /* Led on */
  PciBhtAnd32 (PeiServices, 0x334, (UINT32)~BIT13);
  PciBhtOr32 (PeiServices, 0xD4, BIT6);
  /* Set 1.8v emmc signaling flag */
  PciBhtOr32 (PeiServices, 0x308, BIT4);
  /* Set 200MBaseClock */
  PciBhtAnd32 (PeiServices, 0x304,0x0000FFFF);
  PciBhtOr32 (PeiServices, 0x304,0x25100000);	
  PciBhtOr32 (PeiServices, 0x3E4, BIT22);
  //enable internal clk
  Value = mmio_read8 (Register + 0x2C);
  Value |= BIT0;
  mmio_write8 (Register + 0x2C, Value);
  // Stall for stable
  StallPpi->Stall (PeiServices, StallPpi, 50000);
  //wait BaseClk stable 0x1CC bit14
  Value = mmio_read8 (Register + 0x1CD);
  while(!(Value & BIT6)) {		
    Value = mmio_read8 (Register + 0x1CD);
    DbgMsg (L"0x1CD=0x%x\n", Value);
  }
//  //enable hardware tuning
//  Value = (UINT8)(~0x10);
//  mmio_write8 (Register + 0x110, Value);

  return EFI_SUCCESS;
}

BOOLEAN
BhtHostPciSupport (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN       UINT32                     Address
  )
{
  EFI_PEI_PCI_CFG2_PPI      *PciCfgPpi;
  UINT16                    VendorId;
  UINT16                    DeviceId;

  PciCfgPpi = (**PeiServices).PciCfg;

  VendorId = 0;
  PciCfgPpi->Read (
               PeiServices,
               PciCfgPpi,
               EfiPeiPciCfgWidthUint16,
               Address | VENDOR_ID_REGISTER,
               &VendorId
               );

  DeviceId = 0;
  PciCfgPpi->Read (
               PeiServices,
               PciCfgPpi,
               EfiPeiPciCfgWidthUint16,
               Address | DEVICE_ID_REGISTER,
               &DeviceId
               );

//	DEBUG ((DEBUG_INFO, "check device %04x:%04x\n", VendorId, DeviceId));

	if (VendorId != 0x1217)
		goto end;

	switch (DeviceId)
	{
		case 0x8420:	//PCI_DEV_ID_SDS0
		case 0x8421:	//PCI_DEV_ID_SDS1
		case 0x8520:	//PCI_DEV_ID_FJ2
		case 0x8620:	//PCI_DEV_ID_SB0
		case 0x8621:	//PCI_DEV_ID_SB1
			g_deviceId = DeviceId;
			return 1;
		default:
			break;
	}

	end:
	return 0;
}

