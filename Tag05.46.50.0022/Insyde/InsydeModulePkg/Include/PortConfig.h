/** @file

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _PORT_CONFIG_H_
#define _PORT_CONFIG_H_

#define PORT_DISABLE          0         //Disable port
#define PORT_ENABLE           1         //Enable port
#define PCIE_PORT_AUTO        2         //For PCIE root port configuration only, AUTO means follow reference code implementation.

typedef struct _PORT_CONFIG_1 {
  UINT64                Port00En  :1;
  UINT64                Port01En  :1;
  UINT64                Port02En  :1;
  UINT64                Port03En  :1;
  UINT64                Port04En  :1;
  UINT64                Port05En  :1;
  UINT64                Port06En  :1;
  UINT64                Port07En  :1;
  UINT64                Port08En  :1;
  UINT64                Port09En  :1;
  UINT64                Port10En  :1;
  UINT64                Port11En  :1;
  UINT64                Port12En  :1;
  UINT64                Port13En  :1;
  UINT64                Port14En  :1;
  UINT64                Port15En  :1;
  UINT64                Port16En  :1;
  UINT64                Port17En  :1;
  UINT64                Port18En  :1;
  UINT64                Port19En  :1;
  UINT64                Port20En  :1;
  UINT64                Port21En  :1;
  UINT64                Port22En  :1;
  UINT64                Port23En  :1;
  UINT64                Port24En  :1;
  UINT64                Port25En  :1;
  UINT64                Port26En  :1;
  UINT64                Port27En  :1;
  UINT64                Port28En  :1;
  UINT64                Port29En  :1;
  UINT64                Port30En  :1;
  UINT64                Port31En  :1;
  UINT64                Port32En  :1;
  UINT64                Port33En  :1;
  UINT64                Port34En  :1;
  UINT64                Port35En  :1;
  UINT64                Port36En  :1;
  UINT64                Port37En  :1;
  UINT64                Port38En  :1;
  UINT64                Port39En  :1;
  UINT64                Port40En  :1;
  UINT64                Port41En  :1;
  UINT64                Port42En  :1;
  UINT64                Port43En  :1;
  UINT64                Port44En  :1;
  UINT64                Port45En  :1;
  UINT64                Port46En  :1;
  UINT64                Port47En  :1;
  UINT64                Port48En  :1;
  UINT64                Port49En  :1;
  UINT64                Port50En  :1;
  UINT64                Port51En  :1;
  UINT64                Port52En  :1;
  UINT64                Port53En  :1;
  UINT64                Port54En  :1;
  UINT64                Port55En  :1;
  UINT64                Port56En  :1;
  UINT64                Port57En  :1;
  UINT64                Port58En  :1;
  UINT64                Port59En  :1;
  UINT64                Port60En  :1;
  UINT64                Port61En  :1;
  UINT64                Port62En  :1;
  UINT64                Port63En  :1;
} PORT_CONFIG_1;

typedef struct _PORT_CONFIG_2 {
  UINT64                Port00En  :2;
  UINT64                Port01En  :2;
  UINT64                Port02En  :2;
  UINT64                Port03En  :2;
  UINT64                Port04En  :2;
  UINT64                Port05En  :2;
  UINT64                Port06En  :2;
  UINT64                Port07En  :2;
  UINT64                Port08En  :2;
  UINT64                Port09En  :2;
  UINT64                Port10En  :2;
  UINT64                Port11En  :2;
  UINT64                Port12En  :2;
  UINT64                Port13En  :2;
  UINT64                Port14En  :2;
  UINT64                Port15En  :2;
  UINT64                Port16En  :2;
  UINT64                Port17En  :2;
  UINT64                Port18En  :2;
  UINT64                Port19En  :2;
  UINT64                Port20En  :2;
  UINT64                Port21En  :2;
  UINT64                Port22En  :2;
  UINT64                Port23En  :2;
  UINT64                Port24En  :2;
  UINT64                Port25En  :2;
  UINT64                Port26En  :2;
  UINT64                Port27En  :2;
  UINT64                Port28En  :2;
  UINT64                Port29En  :2;
  UINT64                Port30En  :2;
  UINT64                Port31En  :2;
} PORT_CONFIG_2;

typedef union _PORT_CONFIG {
  UINT64                  Value;
  PORT_CONFIG_1           Config1;
  PORT_CONFIG_2           Config2;
} PORT_CONFIG;

#endif
