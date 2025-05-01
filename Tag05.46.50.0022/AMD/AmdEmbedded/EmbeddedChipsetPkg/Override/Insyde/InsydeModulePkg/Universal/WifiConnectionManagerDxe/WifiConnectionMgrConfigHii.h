/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
  GUIDs used as HII FormSet and HII Package list GUID in WifiMgrDxe driver.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#ifndef __WIFI_CONFIG_HII_GUID_H__
#define __WIFI_CONFIG_HII_GUID_H__

// {3441803E-5A88-4941-82F0-858A1085276C}
#define WIFI_CONFIG_FORM_SET_GUID \
  { \
    0x3441803e, 0x5a88, 0x4941, { 0x82, 0xf0, 0x85, 0x8a, 0x10, 0x85, 0x27, 0x6c } \
  }

// {36AF7790-0C2F-4055-9D3D-DB72A44953CB}
#define WIFI_CONFIG_NETWORK_LIST_REFRESH_GUID \
  { \
    0xc5f3c7f9, 0xfb9d, 0x49f1, { 0xbe, 0x67, 0x8b, 0xad, 0x20, 0xa7, 0xc6, 0xac } \
  }

#define WIFI_CONFIG_CONNECT_FORM_REFRESH_GUID \
  { \
    0xe5faf2b2, 0x5ecc, 0x44ac, { 0x91, 0x75, 0xfb, 0x78, 0xb2, 0x8a, 0x59, 0x6c } \
  }

#define WIFI_CONFIG_MAIN_FORM_REFRESH_GUID \
  { \
    0xde609972, 0xcbcc, 0x4e82, { 0x8b, 0x3e, 0x6a, 0xc5, 0xcf, 0x56, 0x73, 0x8d } \
  }

#endif
