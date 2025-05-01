/** @file
  This protocol provides an interface to override the AHCI timeout value.

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _H2O_AHCI_TIMING_H_
#define _H2O_AHCI_TIMING_H_

#define H2O_AHCI_TIMING_PROTOCOL_GUID \
  {0x9170f23c, 0x0017, 0x4043, 0x05, 0xb9, 0x16, 0xb0, 0x0d, 0xde, 0x20, 0x7b}

typedef struct _H2O_AHCI_TIMING_PROTOCOL H2O_AHCI_TIMING_PROTOCOL;

struct _H2O_AHCI_TIMING_PROTOCOL {
  //
  // The size of H2O_AHCI_TIMING structure. Producers of this protocol must initialize this
  // field to sizeof (H2O_AHCI_TIMING_PROTOCOL). Consumers of this protocol must check size 
  // before accessing any structure members after PhyDetectTimeoutMs.
  //
  UINT64                                 Size;
  
  //
  // Unsigned integer that specifies the amount of time to wait for the PHY to detect the presence of a device. 
  // If the value is set to UINT8_MAX, it means it will use kernel default. 
  // NOTE: If the value is set too low, the device will not be detected by controller.
  // NOTE: If the value is set too high, it will increase the post time. 
  // Producers of this protocol must initialize this field to proper timeout value.
  //
  UINT8                                  PhyDetectTimeoutMs;
};

extern EFI_GUID gH2OAhciTimingProtocolGuid;

#endif