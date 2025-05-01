/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CCX APIC Library
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision: 312770 $   @e \$Date: 2015-02-11 13:24:42 +0800 (Wed, 11 Feb 2015) $
 *
 */

#ifndef _CCX_APIC_LIB_H_
#define _CCX_APIC_LIB_H_

#define XAPIC_ID_MAX 0xFF

/// Configuration modes for PcdAmdApicMode
typedef enum {
  ApicCompatibilityMode   = 0x00,     ///< threads below 255 run in xAPIC with xAPIC ACPI structures and threads 255 & above run in x2 mode with x2 ACPI structures
  xApicMode               = 0x01,     ///< force legacy xApic mode (downcores if enabled threads are >= 255)
  x2ApicMode              = 0x02,     ///< force x2Apic mode independent of thread count
  ApicAutoMode            = 0xFF      ///< Auto
} APIC_MODE;

 /*---------------------------------------------------------------------------------------*/
 /**
  *
  * Returns the APIC Id based on the provided inputs
  *
  * @param[in]      Socket              Socket # of APIC Id to calculate
  * @param[in]      Die                 Die # of APIC Id to calculate
  * @param[in]      Ccd                 CCD # of APIC Id to calculate
  * @param[in]      Complex             Complex # of APIC Id to calculate
  * @param[in]      Core                Core # of APIC Id to calculate
  * @param[in]      Thread              Thread # of APIC Id to calculate
  *
  * @return         UINT8               Calculate APIC Id
  */
UINT32
CcxCalcLocalApic (
  IN       UINTN    Socket,
  IN       UINTN    Die,
  IN       UINTN    Ccd,
  IN       UINTN    Complex,
  IN       UINTN    Core,
  IN       UINTN    Thread
);

#endif // _CCX_APIC_LIB_H_
