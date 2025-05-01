/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Nbio PCIe AER Protocol prototype definition
 *
 */

#ifndef _NBIO_PCIE_AER_PROTOCOL_H_
#define _NBIO_PCIE_AER_PROTOCOL_H_

#include <Library/GnbPciLib.h>

// Need to remove these below 3 structures for AGESA V9+ in favor of the ones in GnbPciLib.h based off of the actual PCIe spec
// These defines all have multiple duplicates. which is is confusing as well as do not list all bits available now.

/// To be deprecated
/// PCIe Correctable Error Mask
typedef union {
  struct {                                                            ///<
    UINT32                  BadTLPMask :1;                            ///<
    UINT32                  BadDLLPMask :1;                           ///<
    UINT32                  ReplayNumberRolloverMask :1;              ///<
    UINT32                  ReplayTimerTimeoutMask :1;                ///<
    UINT32                  AdvisoryNonFatalErrorMask :1;             ///<
  } Field;                                                            ///< field
  UINT32 Value;                                                       ///< value
} AER_PROTOCOL_PCIe_AER_CORRECTABLE_MASK;

/// To be deprecated
/// PCIe Uncorrectable Error Mask
typedef union {
  struct {                                                            ///<
    UINT32                  DataLinkProtocolErrorMask :1;             ///<
    UINT32                  PoisonedTLPMask :1;                       ///<
    UINT32                  CompletionTimeoutMask :1;                 ///<
    UINT32                  CompleterAbortMask :1;                    ///<
    UINT32                  UnexpectedCompletionMask :1;              ///<
    UINT32                  MalTlpMask:1 ;                            ///<
    UINT32                  ECRCErrorMask :1;                         ///<
    UINT32                  UnsupportedRequestErrorMask :1;           ///<
    UINT32                  AcsViolationMask:1 ;                      ///<
  } Field;                                                            ///<field
  UINT32 Value;                                                       ///< value
} AER_PROTOCOL_PCIe_AER_UNCORRECTABLE_MASK;

/// To be deprecated
/// PCIe Uncorrectable Error Severity
typedef union {
  struct {                                                            ///<
    UINT32                  DataLinkProtocolErrorSeverity :1;         ///<
    UINT32                  PoisonedTLPSeverity :1;                   ///<
    UINT32                  CompletionTimeoutSeverity :1;             ///<
    UINT32                  CompleterAbortSeverity :1;                ///<
    UINT32                  UnexpectedCompletionSeverity :1;          ///<
    UINT32                  MalTlpSeverity:1 ;                        ///<
    UINT32                  ECRCErrorSeverity :1;                     ///<
    UINT32                  UnsupportedRequestErrorSeverity :1;       ///<
    UINT32                  AcsViolationSeverity:1 ;                  ///<
  } Field;                                                            ///< field
  UINT32 Value;                                                       ///< value
} AER_PROTOCOL_PCIe_AER_UNCORRECTABLE_SEVERITY;

/// To be deprecated
/// PCIe AER Port Configuration
typedef struct {
  UINT8                                        AerEnable;                          ///< General per-port enable, 0=disable 1=enable
  UINT8                                        PciBus;                             ///< PCI Bus Number
  UINT8                                        PciDev;                             ///< PCI Device Number
  UINT8                                        PciFunc;                            ///< PCI Function Number
  AER_PROTOCOL_PCIe_AER_CORRECTABLE_MASK       CorrectableMask;                    ///< Per-port mask for correctable errors
  AER_PROTOCOL_PCIe_AER_UNCORRECTABLE_MASK     UncorrectableMask;                  ///< Per-port mask for uncorrectable errors
  AER_PROTOCOL_PCIe_AER_UNCORRECTABLE_SEVERITY UncorrectableSeverity;              ///< Per-port severity configuration for uncorrectable errors
} PCIe_PORT_AER_CONFIG;


/// PCIe AER Port Configuration Extended version. The structures used in this version are based off the actual structures used
/// in the PCIe spec (defined in GnbPciLib)
typedef struct {
  UINT8                                        AerEnable;                          ///< General per-port enable, 0=disable 1=enable
  UINT8                                        PciBus;                             ///< PCI Bus Number
  UINT8                                        PciDev;                             ///< PCI Device Number
  UINT8                                        PciFunc;                            ///< PCI Function Number
  PCIE_EXTCAP_AER_CORRECTABLE_ERROR_MASK       CorrectableMask;                    ///< Per-port mask for correctable errors
  PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_MASK     UncorrectableMask;                  ///< Per-port mask for uncorrectable errors
  PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY UncorrectableSeverity;              ///< Per-port severity configuration for uncorrectable errors
} PCIe_PORT_AER_CONFIG_EXTENDED;

///
/// Forward declaration for the AMD_NBIO_PCIE_AER_PROTOCOL
///
typedef struct _AMD_NBIO_PCIE_AER_PROTOCOL AMD_NBIO_PCIE_AER_PROTOCOL;


//
// Protocol Definitions
//
/**
  Enable or disable Advanced Error Reporting feature. To be deprecated for extended version at some point
  This
    A pointer to the AMD_NBIO_PCIE_AER_PROTOCOL instance.
  PcieAerSetting
    A pointer to store the address of the PCIe port AER config structure
**/
typedef
EFI_STATUS
(EFIAPI * AMD_NBIO_PCIE_AER_FEATURE) (
  IN  AMD_NBIO_PCIE_AER_PROTOCOL  *This,                  ///< this ptr
  IN  PCIe_PORT_AER_CONFIG        *PcieAerSetting         ///< pcieaersetting
);

/**
  Enable or disable Advanced Error Reporting feature, extended version.
  This
    A pointer to the AMD_NBIO_PCIE_AER_PROTOCOL instance.
  PcieAerSetting
    A pointer to store the address of the extended PCIe port AER config structure
**/
typedef
EFI_STATUS
(EFIAPI * AMD_NBIO_PCIE_AER_FEATURE_EXTENDED) (
  IN  PCIe_PORT_AER_CONFIG_EXTENDED   *PcieAerSetting     ///< pcie aer setting
);

///
/// The protocol implementation of NBIO PCIe AER Protocol
///
struct _AMD_NBIO_PCIE_AER_PROTOCOL {
  AMD_NBIO_PCIE_AER_FEATURE           SetPcieAerFeature;           ///< set features
  AMD_NBIO_PCIE_AER_FEATURE_EXTENDED  SetPcieAerFeatureExtended;   ///< set features extended
};

extern EFI_GUID gAmdNbioPcieAerProtocolGuid;      ///for protocol calling

#endif /* _NBIO_PCIE_AER_PROTOCOL_H_ */


