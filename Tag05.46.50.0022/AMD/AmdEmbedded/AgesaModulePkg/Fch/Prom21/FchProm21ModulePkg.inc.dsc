#;*****************************************************************************
#;
#; Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
#;
#;******************************************************************************

[Defines]

[LibraryClasses]
  FchProm21BaseLib|AgesaModulePkg/Fch/Prom21/Library/Base/FchProm21BaseLib.inf

[Components.IA32]
  AgesaModulePkg/Fch/Prom21/Pei/FchProm21Pei.inf
  AgesaModulePkg/Fch/Prom21/Pei/FchProm21GpioPei.inf
  AgesaModulePkg/Fch/Prom21/Pei/FchProm21CbsPei.inf

[Components.X64]
  AgesaModulePkg/Fch/Prom21/Asl/FchProm21Asl.inf
  AgesaModulePkg/Fch/Prom21/Dxe/FchProm21Dxe.inf
  AgesaModulePkg/Fch/Prom21/Dxe/FchProm21GpioDxe.inf
  AgesaModulePkg/Fch/Prom21/Dxe/FchProm21SsdtDxe.inf
  AgesaModulePkg/Fch/Prom21/Smm/FchProm21Smm.inf
  AgesaModulePkg/Fch/Prom21/Smm/FchProm21GpioSmm.inf

[PcdsFeatureFlag]

[PcdsFixedAtBuild]

