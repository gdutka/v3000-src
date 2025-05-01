/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "PiPei.h"

#include <Library/FchPeiLib.h>
#include "FchRegistersCommon.h"
#define FILECODE LIBRARY_FCHPEILIB_FCHPEILIB_FILECODE

/**
 * @brief Read 8-bit data from memory
 *
 * @param[in]       PeiServices     EFI_PEI_SERVICES
 * @param[in]       Address         Memory Address
 *
 * @return  Read Data
 *
 */
UINT8
LibFchMemRead8 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT64            Address
  )
{
  EFI_PEI_CPU_IO_PPI            *CpuIo;
  UINT8                Data;

  CpuIo = (*PeiServices)->CpuIo;
  Data = CpuIo->MemRead8 (
                  PeiServices,
                  CpuIo,
                  Address
                  );
  return (Data);
}

/**
 * @brief Read 8-bit data from memory
 *
 * @param PeiServices   EFI_PEI_SERVICES
 * @param Address       Memory Address
 * @param Value         Pointer to Data buffer
 * @return VOID
 */
VOID
LibFchMemWrite8 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT64            Address,
  IN       UINT8             *Value
  )
{
  EFI_PEI_CPU_IO_PPI            *CpuIo;

  CpuIo = (*PeiServices)->CpuIo;
  CpuIo->MemWrite8 (
           PeiServices,
           CpuIo,
           Address,
           *Value
           );
}

/**
 * @brief Read-Modify-Write 8-bit data from memory
 *
 * @param PeiServices   EFI_PEI_SERVICES
 * @param Address       Memory Address
 * @param AndMask       And Mask
 * @param OrMask        Or Mask
 * @return VOID
 */
VOID
LibFchMemRMW8 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT64            Address,
  IN       UINT8             AndMask,
  IN       UINT8             OrMask
  )
{
  EFI_PEI_CPU_IO_PPI   *CpuIo;
  UINT8                Data;

  CpuIo = (*PeiServices)->CpuIo;
  Data = CpuIo->MemRead8 (
                  PeiServices,
                  CpuIo,
                  Address
                  );
  Data &= AndMask;
  Data |= OrMask;
  CpuIo->MemWrite8 (
           PeiServices,
           CpuIo,
           Address,
           Data
           );
}


/**
 * @brief Read 8 bits from the PMIO space using CD6/CD7
 *
 * @param PeiServices  EFI_PEI_SERVICES
 * @param RegIndex     PMIO Register Index
 * @return UINT8
 */
UINT8
LibFchPmIoRead8 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT8             RegIndex
  )
{
  EFI_PEI_CPU_IO_PPI   *CpuIo;
  UINT8                Data;

  // get the CpuIo pointer
  CpuIo = (*PeiServices)->CpuIo;

  // perform the read
  CpuIo->IoWrite8 (
           PeiServices,
           CpuIo,
           FCH_IOMAP_REGCD6,
           RegIndex
           );

  Data = CpuIo->IoRead8 (
                  PeiServices,
                  CpuIo,
                  FCH_IOMAP_REGCD7
                  );

  return (Data);
}


/**
 * @brief  Read 8 bits from PMIO space using Acpi Mmio
 *
 * @param PeiServices  EFI_PEI_SERVICES
 * @param RegIndex     PMIO Register Index
 * @return UINT8
 */
UINT8
LibFchPmIoRead8V2 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT8             RegIndex
  )
{
  EFI_PEI_CPU_IO_PPI   *CpuIo;
  UINT8                Data;

  // get the CpuIo pointer
  CpuIo = (*PeiServices)->CpuIo;

  // perform the read
  Data = CpuIo->MemRead8 (
           PeiServices,
           CpuIo,
           ACPI_MMIO_BASE + PMIO_BASE + (UINT64)RegIndex
           );

  return (Data);
}

/**
 * @brief Read 16 bits from the PMIO space using CD6/CD7
 *
 * @param PeiServices  EFI_PEI_SERVICES
 * @param RegIndex     PMIO Register Index
 * @return UINT16
 */
UINT16
LibFchPmIoRead16 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT8             RegIndex
  )
{
  UINT16               Data;

  // get the data using two 1 byte read
  Data  = LibFchPmIoRead8 (PeiServices, RegIndex);
  Data |= (LibFchPmIoRead8 (PeiServices, RegIndex + 1) << 8);

  return (Data);
}

/**
 * @brief Read 16 bits from the PMIO space using Acpi Mmio
 *
 * @param PeiServices  EFI_PEI_SERVICES
 * @param RegIndex     PMIO Register Index
 * @return UINT16
 */
UINT16
LibFchPmIoRead16V2 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT8             RegIndex
  )
{
  EFI_PEI_CPU_IO_PPI   *CpuIo;
  UINT16               Data16;

  // get the CpuIo pointer
  CpuIo = (*PeiServices)->CpuIo;

  // perform the read
  Data16 = CpuIo->MemRead16 (
             PeiServices,
             CpuIo,
             ACPI_MMIO_BASE + PMIO_BASE + (UINT64)RegIndex
             );

  return (Data16);
}


/*---------------------------------------------------------------------------------------*/
/**
 * @brief  Get AGESA FCH ACPIMMIO Address
 *
 * @param[in]      PeiServices   EFI_PEI_SERVICE
 *
 */
UINT32
LibFchGetAcpiMmio (
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  UINT32      AcpiMmioAddr;

  AcpiMmioAddr = 0xFED80000;
  return (AcpiMmioAddr);
}

/*---------------------------------------------------------------------------------------*/
/**
 * @brief LibFchPciCfgRead - FCH Read PCI Configuration space
 *
 * @param[in]      PeiServices   EFI_PEI_SERVICE
 * @param[in]      Width         PCI Cfg Read lenght
 * @param[in]      Address       PCI Cfg Read Address
 * @param[in]      Value         Return Read value buffer
 *
 */
VOID
LibFchPciCfgRead (
  IN       CONST EFI_PEI_SERVICES         **PeiServices,
  IN       EFI_PEI_PCI_CFG_PPI_WIDTH      Width,
  IN       UINT64                         Address,
  IN OUT   VOID                           *Value
  )
{
  EFI_PEI_PCI_CFG2_PPI            *PciCfg;

  PciCfg = ( EFI_PEI_PCI_CFG2_PPI* ) (*PeiServices)->PciCfg;
  PciCfg->Read (
            PeiServices,
            PciCfg,
            Width,
            Address,
            Value
            );
}

/*---------------------------------------------------------------------------------------*/
/**
 * @brief LibFchPciCfgWrite - FCH Write PCI Configuration space
 *
 * @param[in]      PeiServices   EFI_PEI_SERVICE
 * @param[in]      Width         PCI Cfg Write lenght
 * @param[in]      Address       PCI Cfg Write Address
 * @param[in]      Value         Return Write value buffer
 *
 */
VOID
LibFchPciCfgWrite (
  IN       CONST EFI_PEI_SERVICES   **PeiServices,
  IN       EFI_PEI_PCI_CFG_PPI_WIDTH Width,
  IN       UINT64                   Address,
  IN OUT   VOID                     *Value
  )
{
  EFI_PEI_PCI_CFG2_PPI            *PciCfg;

  PciCfg = ( EFI_PEI_PCI_CFG2_PPI* ) (*PeiServices)->PciCfg;
  PciCfg->Write (
            PeiServices,
            PciCfg,
            Width,
            Address,
            Value
            );
}

/*---------------------------------------------------------------------------------------*/
/**
 * @brief LibFchPciCfgModify - FCH Modify PCI Configuration space
 *
 * @param[in]      PeiServices   EFI_PEI_SERVICE
 * @param[in]      Width         PCI Cfg modify lenght
 * @param[in]      Address       PCI Cfg modify Address
 * @param[in]      SetBits       Set PCI Cfg Register bitmap
 * @param[in]      ClearBits     Clear PCI Cfg Register bitmap
 *
 */
VOID
LibFchPciCfgModify (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN       EFI_PEI_PCI_CFG_PPI_WIDTH Width,
  IN       UINT64                   Address,
  IN       UINTN                    SetBits,
  IN       UINTN                    ClearBits
  )
{
  EFI_PEI_PCI_CFG2_PPI            *PciCfg;

  PciCfg = ( EFI_PEI_PCI_CFG2_PPI* ) (*PeiServices)->PciCfg;
  PciCfg->Modify (
            PeiServices,
            PciCfg,
            Width,
            Address,
            &SetBits,
            &ClearBits
            );
}


