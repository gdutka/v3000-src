/** @file
  Compatibility macros for VFR (Visual Form Representation) files.

;******************************************************************************
;* Copyright 2021 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef H2O_VFR_COMPAT_MACROS_H_
#define H2O_VFR_COMPAT_MACROS_H_

//
// The macro OLD_VFR_PCD_GUID_EXPANSION enables compatibility with H2O Kernel
// 05.43.29 and earlier. Specifically, without IB19060011, the build tool
// (build.exe) expands PCDs in VFR files in the old way that requires syntax
// workaround. If the package or module (INF file) won't build, add this line
// to the [BuildOptions] section of the package's DSC file:
//   *_*_*_VFRPP_FLAGS = -D OLD_VFR_PCD_GUID_EXPANSION
//
// Usage: VFR_GUID_FROM_PCD (PcdGetPtr (TokenName))
//
#ifdef OLD_VFR_PCD_GUID_EXPANSION
#define VFR_GUID_FROM_PCD(A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P) \
  {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P}
#else
//
// Assume the compiler supports __VA_ARGS__; otherwise it's hard to implement
// the compat macro. Braces do not prevent commas from separating arguments, so
//   VFR_GUID_FROM_PCD({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})
// should have 16 arguments not 1. Visual C++ doesn't conform to this.
//
#define VFR_GUID_FROM_PCD(...)  __VA_ARGS__
#endif

#endif // H2O_VFR_COMPAT_MACROS_H_
