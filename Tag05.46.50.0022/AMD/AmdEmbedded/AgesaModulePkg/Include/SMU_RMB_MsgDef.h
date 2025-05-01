/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
#ifndef _SMU_RMB_MSG_DEF_H_
#define _SMU_RMB_MSG_DEF_H_

// Message Definitions:
#define BIOSSMC_MSG_TestMessage                         0x1
#define BIOSSMC_MSG_GetSmuVersion                       0x2
#define BIOSSMC_MSG_GetBiosIfVersion                    0x3
#define BIOSSMC_MSG_GetNameString                       0x4
#define BIOSSMC_MSG_EnableSmuFeatures                   0x5
#define BIOSSMC_MSG_GetEnabledSmuFeatures               0x6
#define BIOSSMC_MSG_DisableSmuFeatures                  0x7
#define BIOSSMC_MSG_DramLogSetDramAddr                  0x8
#define BIOSSMC_MSG_DxioTestMessage                     0x9
#define BIOSSMC_MSG_ReadCoreCacWeightRegister           0xA
#define BIOSSMC_MSG_SleepEntry                          0xB
#define BIOSSMC_MSG_SetGbeStatus                        0xC //Not applicable for VGM
#define BIOSSMC_MSG_SetBiosDramAddr                     0xD
#define BIOSSMC_MSG_SetToolsDramAddr                    0xE
#define BIOSSMC_MSG_TransferTableSmu2Dram               0xF
#define BIOSSMC_MSG_TransferTableDram2Smu               0x10
#define BIOSSMC_MSG_PowerSourceAC                       0x11
#define BIOSSMC_MSG_PowerSourceDC                       0x12
#define BIOSSMC_MSG_DcBtc                               0x13

//DPTC {
#define BIOSSMC_MSG_SetSustainedPowerLimit              0x14
#define BIOSSMC_MSG_SetFastPPTLimit                     0x15
#define BIOSSMC_MSG_SetSlowPPTLimit                     0x16
#define BIOSSMC_MSG_SetSlowPPTTimeConstant              0x17
#define BIOSSMC_MSG_SetStapmTimeConstant                0x18
#define BIOSSMC_MSG_SetTjMax                            0x19
#define BIOSSMC_MSG_SetVrmVddCurrentLimit               0x1A
#define BIOSSMC_MSG_SetVrmSocCurrentLimit               0x1B
#define BIOSSMC_MSG_SetVrmVddMaximumCurrentLimit        0x1C
#define BIOSSMC_MSG_SetVrmSocMaximumCurrentLimit        0x1D
#define BIOSSMC_MSG_SetPsi3VddCurrentLimit              0x1E
#define BIOSSMC_MSG_SetProchotDeassertionRampTime       0x1F //}
#define BIOSSMC_MSG_SetPeakPackagePowerLimit            0x20 //}

#define BIOSSMC_MSG_SetupNvmeTrap                       0x21 ///< Setup Nvme trap
#define BIOSSMC_MSG_GetNumNvmeTrap                      0x22 ///< Get number of Nvme traps

 //A+A {
#define BIOSSMC_MSG_SetSlowPPTLimitApuOnly              0x23
#define BIOSSMC_MSG_SetGpuDeviceId                      0x24
#define BIOSSMC_MSG_SetGpuApertureLow                   0x25
#define BIOSSMC_MSG_SetGpuApertureHigh                  0x26
#define BIOSSMC_MSG_StartGpuLink                        0x27
#define BIOSSMC_MSG_StopGpuLink                         0x28 //}

#define BIOSSMC_MSG_UsbSxEntry                          0x29
#define BIOSSMC_MSG_UsbSxExit                           0x2A
#define BIOSSMC_MSG_UsbInit                             0x2B
#define BIOSSMC_MSG_UsbConfigUpdate                     0x2C
#define BIOSSMC_MSG_UsbControllerDisabled               0x2D

#define BIOSSMC_MSG_PowerUpSata                         0x2E
#define BIOSSMC_MSG_PowerDownSata                       0x2F
#define BIOSSMC_MSG_DisableSataController               0x30

//PLAT-44535 {
#define BIOSSMC_MSG_Set_STT_ALPHA_APU                   0x31
#define BIOSSMC_MSG_Set_STT_ALPHA_HS2                   0x32
#define BIOSSMC_MSG_Set_STT_SKIN_TEMPERATURE_LIMIT_APU  0x33
#define BIOSSMC_MSG_Set_STT_SKIN_TEMPERATURE_LIMIT_HS2  0x34
#define BIOSSMC_MSG_Set_STT_ERROR_COEFF                 0x35
#define BIOSSMC_MSG_Set_STT_ERROR_RATE_COEFF            0x36
#define BIOSSMC_MSG_Set_STT_M1_COEFF                    0x37
#define BIOSSMC_MSG_Set_STT_M2_COEFF                    0x38
#define BIOSSMC_MSG_Set_STT_M3_COEFF                    0x39
#define BIOSSMC_MSG_Set_STT_M4_COEFF                    0x3A
#define BIOSSMC_MSG_Set_STT_M5_COEFF                    0x3B
#define BIOSSMC_MSG_Set_STT_M6_COEFF                    0x3C
#define BIOSSMC_MSG_Set_STT_C_APU_COEFF                 0x3D
#define BIOSSMC_MSG_Set_STT_C_HS2_COEFF                 0x3E
//}

#define BIOSSMC_MSG_GetPerSrcBistPF                     0x3F  //PMFW-1748
#define BIOSSMC_MSG_GetMp1AccessOnlyFuses               0x40  //PLAT-37425
#define BIOSSMC_MSG_SetupSataPciMsiTrap                 0x41 ///< To setup SATA PCI MSI trap

//PMFW-1726 {
#define BIOSSMC_MSG_GetThrottleReason                   0x42 ///< Get throttle reason
#define BIOSSMC_MSG_ClearThrottleReason                 0x43 ///< Clear recorded throttle reason in PMFW

#define BIOSSMC_MSG_EnableWalle                         0x44
#define BIOSSMC_MSG_SetupCfg0NbifTrap                   0x45
#define BIOSSMC_MSG_SetVddOffVid                        0x46  //CBS internal debug option }
#define BIOSSMC_MSG_GetCPPCSupportedRegisters           0x47
#define BIOSSMC_MSG_GetCPPCNominalFrequency             0x48
#define BIOSSMC_MSG_GetCPPCLowestFrequency              0x49

#define BIOSSMC_MSG_SetSttMinLimit                      0x4A

#define BIOSSMC_MSG_SetCorePsmMargin                    0x4B
#define BIOSSMC_MSG_SetAllCorePsmMargin                 0x4C

#define BIOSSMC_MSG_SetHardMinI2sMclk                   0x4D
#define BIOSSMC_MSG_PowerGateMmHub                      0x4E
#define BIOSSMC_MSG_FllBootTimeCalibration              0x4F
#define BIOSSMC_MSG_BootupSata                          0x50
#define BIOSSMC_MSG_SetExternalPcieBuses                0x51
#define BIOSSMC_MSG_SetRootPortBridgeSbdfo              0x52
#define BIOSSMC_MSG_SetStbDramAddr                      0x53
#define BIOSSMC_MSG_SetS0i3PmeTurnOffDelay              0x54
#define BIOSSMC_MSG_GetSwSciGpeTriggerInfo              0x55 //PMFW expects this message after software Sci event trigger
#define BIOSSMC_MSG_CoreCountControl                    0x56 //Send Core Control level [0 - 2] as argument
//PLAT-41060 {
#define BIOSSMC_MSG_EnableOverclocking                  0x57
#define BIOSSMC_MSG_DisableOverclocking                 0x58
#define BIOSSMC_MSG_SetOverclockCclkFreqAllCores        0x59
#define BIOSSMC_MSG_SetOverclockCclkFreqPerCore         0x5A
#define BIOSSMC_MSG_SetOverclockVid                     0x5B
#define BIOSSMC_MSG_SetGfxclkOverdriveByFreqVid         0x5C
#define BIOSSMC_MSG_DisableGfxclkOverdrive              0x5D
#define BIOSSMC_MSG_GetOverclockCap                     0x5E

#define BIOSSMC_MSG_GetSustainedPowerAndThmLimit        0x5F
#define BIOSSMC_MSG_Usb4Init                            0x60
#define BIOSSMC_MSG_Usb4PS3SleepEvent                   0x61 //USB4
#define BIOSSMC_MSG_S0i3ResumeStatusForNvmeUnlock       0x62 ///< Get S0i3 resume state, response this message one time in each of S0i3 resume.
#define BIOSSMC_MSG_SetFitLimitScalar                   0x63 ///< Set FIT Limit Scalar
#define BIOSSMC_MSG_ForceGfxVidMargin                   0x64 ///< Force GFX vid margine. Input: signed vid (-30 to +30)
#define BIOSSMC_MSG_Usb4RouterHostReset                 0x65 ///< USB4 router host reset request
#define BIOSSMC_MSG_SetHardMinAclkByFreq                0x66
#define BIOSSMC_MSG_SetupUsb4HiaInterruptMaskTrap       0x67 ///< Setup USB4 0/1 HIA Interrupt Mask Clear
#define BIOSSMC_MSG_EnableNVMSelfRefresh                0x68
#define BIOSSMC_MSG_SetResetCpuOnSyncFlood              0x69 ///< Based on PCD BIOS will set this bit indicating CPU to be reset on sync flood
#define BIOSSMC_MSG_SetSyncFloodToApml                  0x6A ///< Alert_L has to be raised or not
#define BIOSSMC_MSG_SetScratchRegister                  0x6B ///< Used to set the memory clear bit for subsequent ABL consumption
#define BIOSSMC_MSG_GetScratchRegister                  0x6C ///< Used to Get the Set Scratch Register values.
#define BIOSSMC_Message_Count                           0x6D ///< Total number of BIOS messages

//}
#endif
