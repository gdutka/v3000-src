/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Agesa structures and definitions
 *
 * Contains AMD AGESA core interface
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Include
 * @e \$Revision: 317069 $   @e \$Date: 2015-04-20 11:25:50 -0700 (Mon, 20 Apr 2015) $
 */


#ifndef _FCHCIO_H_
#define _FCHCIO_H_

#pragma pack (push, 1)

///
/// USB4 controller Structure
///
typedef struct {
  BOOLEAN               Usb4InitEnable;                  ///< USB4 overall Init enable
  BOOLEAN               Usb4HostEnable;                  ///< USB4 Host Router enable
  UINT8                 Usb3HCDisable;                   ///< USB3 HC (connected to USB4) disable
  UINT8                 Usb4PcieTunnelingDisable;        ///< PCIe Tunneling disable
  UINT8                 Usb4Usb3TunnelingDisable;        ///< USB3 Tunneling disable
  BOOLEAN               Usb4DPTunnelingDisable;          ///< DP Tunneling disable
  BOOLEAN               Usb4Tbt3NotSupport;              ///< Not Support TBT3 Compatibility
  UINT8                 Usb4PcieAdpHidden;               ///< USB4 PCIe Adapter Hidden
  UINT8                 Usb4Usb3AdpHidden;               ///< USB4 USB3 Adapter Hidden
  UINT8                 Usb4DPAdpHidden;                 ///< USB4 DP Adapter Hidden
  BOOLEAN               Usb4Gen3Disable;                 ///< USB4 Gen3 disable
  UINT8                 Usb4PhyEnable;                   ///< USB4 PHY enable
  UINT8                 Usb4Gen3Support;                 ///< USB4 Gen3 Suppport
  UINT8                 Usb4TxFFEMode;                   ///< TxFFE Mode
  UINT8                 TxFFEPreSetValue;                ///< TxFFE Pre-Set Value bit[3:0] - Lane0; bit[7:4] Lane1.
  UINT8                 Reserved[17];
} FCH_USB4;

#pragma pack (pop)

#endif // _FCHCIO_H_


