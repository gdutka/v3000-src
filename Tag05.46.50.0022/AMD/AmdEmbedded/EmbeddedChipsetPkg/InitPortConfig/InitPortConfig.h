/** @file

***************************************************************************
* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
*
* You may not reproduce, distribute, publish, display, perform, modify, adapt,
* transmit, broadcast, present, recite, release, license or otherwise exploit
* any part of this publication in any form, by any means, without the prior
* written permission of Insyde Software Corporation.
*
******************************************************************************
*/

#ifndef _INIT_PORT_CONFIG_H_
#define _INIT_PORT_CONFIG_H_

typedef struct _PORT_CONFIG_4 {
  UINT64                Port00En  :4;
  UINT64                Port01En  :4;
  UINT64                Port02En  :4;
  UINT64                Port03En  :4;
  UINT64                Port04En  :4;
  UINT64                Port05En  :4;
  UINT64                Port06En  :4;
  UINT64                Port07En  :4;
  UINT64                Port08En  :4;
  UINT64                Port09En  :4;
  UINT64                Port10En  :4;
  UINT64                Port11En  :4;
  UINT64                Port12En  :4;
  UINT64                Port13En  :4;
  UINT64                Port14En  :4;
  UINT64                Port15En  :4; 
} PORT_CONFIG_4;

typedef union _PORT_CONFIG4 {
  UINT64                  Value;
  PORT_CONFIG_4           Config4;
} PORT_CONFIG4;

#endif

