/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

#include <Library/FchSmmLib.h>
#include "FchRegistersCommon.h"

/*
#define CFG_ADDR_PORT     0xcf8
#define CFG_DATA_PORT     0xcfc
#define FCH_IOMAP_REGCD6  0xcd6
#define FCH_IOMAP_REGCD7  0xcd7
*/

#define FILECODE LIBRARY_FCHSMMLIB_FCHSMMLIB_FILECODE



/*----------------------------------------------------------------------------------------*/
/**
 * @brief SMM Memory Read Access
 *
 * @param[in]       SmmIo           Pointer to SMM CPU IO interface
 * @param[in]       AccessWidth     Access width
 * @param[in]       Address         Memory address
 * @param[out]      Value           Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchSmmMemRead (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT64                      Address,
     OUT   VOID                        *Value
  )
{
  SmmIo->Mem.Read (
                   SmmIo,
                   AccessWidth,
                   Address,
                   1,
                   Value
                   );
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief SMM Memory Write Access
 *
 * @param[in]       SmmIo           Pointer to SMM CPU IO interface
 * @param[in]       AccessWidth     Access width
 * @param[in]       Address         Memory address
 * @param[out]      Value           Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchSmmMemWrite (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT64                      Address,
     OUT   VOID                        *Value
  )
{
  SmmIo->Mem.Write (
                   SmmIo,
                   AccessWidth,
                   Address,
                   1,
                   Value
                   );
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief SMM I/O Read Access
 *
 * @param[in]       SmmIo           Pointer to SMM CPU IO interface
 * @param[in]       AccessWidth     Access width
 * @param[in]       Address         IO address
 * @param[out]      Value           Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchSmmIoRead (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT16                      Address,
     OUT   VOID                        *Value
  )
{
  SmmIo->Io.Read (
                   SmmIo,
                   AccessWidth,
                   Address,
                   1,
                   Value
                   );
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief SMM I/O Write Access
 *
 * @param[in]       SmmIo           Pointer to SMM CPU IO interface
 * @param[in]       AccessWidth     Access width
 * @param[in]       Address         IO address
 * @param[out]      Value           Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchSmmIoWrite (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT64                      Address,
  IN       VOID                        *Value
  )
{
  SmmIo->Io.Write (
              SmmIo,
              AccessWidth,
              Address,
              1,
              Value
              );
}

/**
 * @brief SMM PCI Read
 *
 * @param SmmIo         Pointer to SMM CPU IO interface
 * @param AccessWidth   Access width
 * @param PciAddress    PCI Address
 * @param Value         Pointer to data buffer
 * @return VOID
 */
VOID
LibFchSmmPciRead (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT32                      PciAddress,
     OUT   VOID                        *Value
  )
{
  UINT32     Address32;

  Address32 = BIT31 + (UINT32) ((PciAddress >> 8) & 0xFFFFFF00) + (PciAddress & 0xFF);
  LibFchSmmIoWrite (SmmIo, SMM_IO_UINT32, CFG_ADDR_PORT, &Address32);
  LibFchSmmIoRead (SmmIo, AccessWidth, CFG_DATA_PORT, Value);
}

/**
 * @brief SMM 8-bit PMIO register read from IO 0xCD6/CD7
 *
 * @param SmmIo         Pointer to SMM CPU IO interface
 * @param Offset        PMIO Index
 * @param Value         Pointer to data buffer
 * @return VOID
 */
VOID
LibFchSmmPmioRead8 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  )
{
  LibFchSmmIoWrite (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD6, &Offset);
  LibFchSmmIoRead (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD7, Value);
}

/**
 * @brief SMM 8-bit PMIO register read from MMIO
 *
 * @param SmmIo         Pointer to SMM CPU IO interface
 * @param Offset        PMIO Index
 * @param Value         Pointer to data buffer
 * @return VOID
 */
VOID
LibFchSmmPmioRead8V2 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  )
{
  LibFchSmmMemRead (SmmIo, SMM_IO_UINT8, (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + (UINT64)Offset), Value);
}

/**
 * @brief SMM 16-bit PMIO register read from IO 0xCD6/CD7
 *
 * @param SmmIo         Pointer to SMM CPU IO interface
 * @param Offset        PMIO Index
 * @param Value         Pointer to data buffer
 * @return VOID
 */
VOID
LibFchSmmPmioRead16 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  )
{
  LibFchSmmIoWrite (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD6, &Offset);
  LibFchSmmIoRead (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD7, Value);
  Offset +=1;
  LibFchSmmIoWrite (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD6, &Offset);
  LibFchSmmIoRead (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD7, (UINT8*)(UINTN)(Value)+1);
}

/**
 * @brief SMM 16-bit PMIO register read from MMIO
 *
 * @param SmmIo         Pointer to SMM CPU IO interface
 * @param Offset        PMIO Index
 * @param Value         Pointer to data buffer
 * @return VOID
 */
VOID
LibFchSmmPmioRead16V2 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  )
{
  LibFchSmmMemRead (SmmIo, SMM_IO_UINT16, (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + (UINT64)Offset), Value);
}

/**
 * @brief  SMM 8-bit PMIO register write from IO 0xCD6/CD7
 *
 * @param SmmIo         Pointer to SMM CPU IO interface
 * @param Offset        PMIO Index
 * @param Value         Pointer to data buffer
 * @return VOID
 */
VOID
LibFchSmmPmioWrite8 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  )
{
  LibFchSmmIoWrite (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD6, &Offset);
  LibFchSmmIoWrite (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD7, Value);
}

/**
 * @brief  SMM 8-bit PMIO register write from MMIO
 *
 * @param SmmIo         Pointer to SMM CPU IO interface
 * @param Offset        PMIO Index
 * @param Value         Pointer to data buffer
 * @return VOID
 */
VOID
LibFchSmmPmioWrite8V2 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  )
{
  LibFchSmmMemWrite (SmmIo, SMM_IO_UINT8, (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + (UINT64)Offset), Value);
}

/**
 * @brief SMM 8-bit PMIO register read-modify-write from IO 0xCD6/CD7
 *
 * @param SmmIo         Pointer to SMM CPU IO interface
 * @param Offset        PMIO Index
 * @param mask          Mask Data
 * @param Value         Write Data
 * @return VOID
 */
VOID
LibFchSmmPmioReadWrite8 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
  IN       UINT8                       mask,
  IN       UINT8                       Value
  )
{
  UINT8 Value8;
  LibFchSmmIoWrite (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD6, &Offset);
  LibFchSmmIoRead (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD7, &Value8);
  Value  |= (mask & Value8);
  LibFchSmmIoWrite (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD7, &Value);
}

/**
 * @brief SMM 8-bit PMIO register read-modify-write from MMIO
 *
 * @param SmmIo         Pointer to SMM CPU IO interface
 * @param Offset        PMIO Index
 * @param mask          Mask Data
 * @param Value         Write Data
 * @return VOID
 */
VOID
LibFchSmmPmioReadWrite8V2 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
  IN       UINT8                       mask,
  IN       UINT8                       Value
  )
{
  UINT8 Value8;

  Value8 = 0;

  LibFchSmmPmioRead8V2 (SmmIo, Offset, &Value8);
  Value  |= (mask & Value8);
  LibFchSmmPmioWrite8V2 (SmmIo, Offset, &Value);
}

