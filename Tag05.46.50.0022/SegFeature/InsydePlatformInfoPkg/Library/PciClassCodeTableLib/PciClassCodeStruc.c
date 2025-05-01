/** @file
  PCI Class Code Lookup table

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
#include <Library/PciClassCodeTableLib.h>

//
// Declarations of entries which contain printable strings for class codes
// in PCI configuration space
//
PCI_CLASS_ENTRY PCIBlankEntry[];
PCI_CLASS_ENTRY PCISubClass_00[];
PCI_CLASS_ENTRY PCISubClass_01[];
PCI_CLASS_ENTRY PCISubClass_02[];
PCI_CLASS_ENTRY PCISubClass_03[];
PCI_CLASS_ENTRY PCISubClass_04[];
PCI_CLASS_ENTRY PCISubClass_05[];
PCI_CLASS_ENTRY PCISubClass_06[];
PCI_CLASS_ENTRY PCISubClass_07[];
PCI_CLASS_ENTRY PCISubClass_08[];
PCI_CLASS_ENTRY PCISubClass_09[];
PCI_CLASS_ENTRY PCISubClass_0a[];
PCI_CLASS_ENTRY PCISubClass_0b[];
PCI_CLASS_ENTRY PCISubClass_0c[];
PCI_CLASS_ENTRY PCISubClass_0d[];
PCI_CLASS_ENTRY PCISubClass_0e[];
PCI_CLASS_ENTRY PCISubClass_0f[];
PCI_CLASS_ENTRY PCISubClass_10[];
PCI_CLASS_ENTRY PCISubClass_11[];
PCI_CLASS_ENTRY PCISubClass_12[];
PCI_CLASS_ENTRY PCISubClass_13[];
PCI_CLASS_ENTRY PCISubClass_40[];
PCI_CLASS_ENTRY PCIPifClass_0100[];
PCI_CLASS_ENTRY PCIPifClass_0101[];
PCI_CLASS_ENTRY PCIPifClass_0105[];
PCI_CLASS_ENTRY PCIPifClass_0106[];
PCI_CLASS_ENTRY PCIPifClass_0107[];
PCI_CLASS_ENTRY PCIPifClass_0108[];
PCI_CLASS_ENTRY PCIPifClass_0109[];
PCI_CLASS_ENTRY PCIPifClass_0300[];
PCI_CLASS_ENTRY PCIPifClass_0403[];
PCI_CLASS_ENTRY PCIPifClass_0604[];
PCI_CLASS_ENTRY PCIPifClass_0608[];
PCI_CLASS_ENTRY PCIPifClass_0609[];
PCI_CLASS_ENTRY PCIPifClass_060b[];
PCI_CLASS_ENTRY PCIPifClass_0700[];
PCI_CLASS_ENTRY PCIPifClass_0701[];
PCI_CLASS_ENTRY PCIPifClass_0703[];
PCI_CLASS_ENTRY PCIPifClass_0800[];
PCI_CLASS_ENTRY PCIPifClass_0801[];
PCI_CLASS_ENTRY PCIPifClass_0802[];
PCI_CLASS_ENTRY PCIPifClass_0803[];
PCI_CLASS_ENTRY PCIPifClass_0904[];
PCI_CLASS_ENTRY PCIPifClass_0c00[];
PCI_CLASS_ENTRY PCIPifClass_0c03[];
PCI_CLASS_ENTRY PCIPifClass_0c07[];
PCI_CLASS_ENTRY PCIPifClass_0d01[];
PCI_CLASS_ENTRY PCIPifClass_0e00[];

//
// Base class strings entries
//
PCI_CLASS_ENTRY gClassStringList[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_BASE00_STRING),
    PCISubClass_00
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_BASE01_STRING),
    PCISubClass_01
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_BASE02_STRING),
    PCISubClass_02
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_BASE03_STRING),
    PCISubClass_03
  },
  {
    0x04,
    STRING_TOKEN (STR_PCI_CLASS_BASE04_STRING),
    PCISubClass_04
  },
  {
    0x05,
    STRING_TOKEN (STR_PCI_CLASS_BASE05_STRING),
    PCISubClass_05
  },
  {
    0x06,
    STRING_TOKEN (STR_PCI_CLASS_BASE06_STRING),
    PCISubClass_06
  },
  {
    0x07,
    STRING_TOKEN (STR_PCI_CLASS_BASE07_STRING),
    PCISubClass_07
  },
  {
    0x08,
    STRING_TOKEN (STR_PCI_CLASS_BASE08_STRING),
    PCISubClass_08
  },
  {
    0x09,
    STRING_TOKEN (STR_PCI_CLASS_BASE09_STRING),
    PCISubClass_09
  },
  {
    0x0a,
    STRING_TOKEN (STR_PCI_CLASS_BASE0A_STRING),
    PCISubClass_0a
  },
  {
    0x0b,
    STRING_TOKEN (STR_PCI_CLASS_BASE0B_STRING),
    PCISubClass_0b
  },
  {
    0x0c,
    STRING_TOKEN (STR_PCI_CLASS_BASE0C_STRING),
    PCISubClass_0c
  },
  {
    0x0d,
    STRING_TOKEN (STR_PCI_CLASS_BASE0D_STRING),
    PCISubClass_0d
  },
  {
    0x0e,
    STRING_TOKEN (STR_PCI_CLASS_BASE0E_STRING),
    PCISubClass_0e
  },
  {
    0x0f,
    STRING_TOKEN (STR_PCI_CLASS_BASE0F_STRING),
    PCISubClass_0f
  },
  {
    0x10,
    STRING_TOKEN (STR_PCI_CLASS_BASE10_STRING),
    PCISubClass_10
  },
  {
    0x11,
    STRING_TOKEN (STR_PCI_CLASS_BASE11_STRING),
    PCISubClass_11
  },
  {
    0x12,
    STRING_TOKEN (STR_PCI_CLASS_BASE12_STRING),
    PCISubClass_12
  },
  {
    0x13,
    STRING_TOKEN (STR_PCI_CLASS_BASE13_STRING),
    PCISubClass_13
  },
  {
    0x40,
    STRING_TOKEN (STR_PCI_CLASS_BASE40_STRING),
    PCISubClass_40
  },
  {
    0xff,
    STRING_TOKEN (STR_UNDEFINED_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

//
// Subclass strings entries
//
PCI_CLASS_ENTRY     PCIBlankEntry[] = {
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_00[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0000_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0001_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_01[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0100_STRING),
    PCIPifClass_0100
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0101_STRING),
    PCIPifClass_0101
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_SUB0102_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_SUB0103_STRING),
    PCIBlankEntry
  },
  {
    0x04,
    STRING_TOKEN (STR_PCI_CLASS_SUB0104_STRING),
    PCIBlankEntry
  },
  {
    0x05,
    STRING_TOKEN (STR_PCI_CLASS_SUB0105_STRING),
    PCIPifClass_0105
  },
  {
    0x06,
    STRING_TOKEN (STR_PCI_CLASS_SUB0106_STRING),
    PCIPifClass_0106
  },
  {
    0x07,
    STRING_TOKEN (STR_PCI_CLASS_SUB0107_STRING),
    PCIPifClass_0107
  },
  {
    0x08,
    STRING_TOKEN (STR_PCI_CLASS_SUB0108_STRING),
    PCIPifClass_0108
  },
  {
    0x09,
    STRING_TOKEN (STR_PCI_CLASS_SUB0109_STRING),
    PCIPifClass_0109
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0180_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_02[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0200_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0201_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_SUB0202_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_SUB0203_STRING),
    PCIBlankEntry
  },
  {
    0x04,
    STRING_TOKEN (STR_PCI_CLASS_SUB0204_STRING),
    PCIBlankEntry
  },
  {
    0x05,
    STRING_TOKEN (STR_PCI_CLASS_SUB0205_STRING),
    PCIBlankEntry
  },
  {
    0x06,
    STRING_TOKEN (STR_PCI_CLASS_SUB0206_STRING),
    PCIBlankEntry
  },
  {
    0x07,
    STRING_TOKEN (STR_PCI_CLASS_SUB0207_STRING),
    PCIBlankEntry
  },
  {
    0x08,
    STRING_TOKEN (STR_PCI_CLASS_SUB0208_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0280_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_03[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0300_STRING),
    PCIPifClass_0300
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0301_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_SUB0302_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0380_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    PCIBlankEntry
  }
};

PCI_CLASS_ENTRY PCISubClass_04[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0400_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0401_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_SUB0402_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_SUB0403_STRING),
    PCIPifClass_0403
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0480_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_05[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0500_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0501_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0580_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_06[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0600_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0601_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_SUB0602_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_SUB0603_STRING),
    PCIBlankEntry
  },
  {
    0x04,
    STRING_TOKEN (STR_PCI_CLASS_SUB0604_STRING),
    PCIPifClass_0604
  },
  {
    0x05,
    STRING_TOKEN (STR_PCI_CLASS_SUB0605_STRING),
    PCIBlankEntry
  },
  {
    0x06,
    STRING_TOKEN (STR_PCI_CLASS_SUB0606_STRING),
    PCIBlankEntry
  },
  {
    0x07,
    STRING_TOKEN (STR_PCI_CLASS_SUB0607_STRING),
    PCIBlankEntry
  },
  {
    0x08,
    STRING_TOKEN (STR_PCI_CLASS_SUB0608_STRING),
    PCIPifClass_0608
  },
  {
    0x09,
    STRING_TOKEN (STR_PCI_CLASS_SUB0609_STRING),
    PCIPifClass_0609
  },
  {
    0x0A,
    STRING_TOKEN (STR_PCI_CLASS_SUB060A_STRING),
    PCIBlankEntry
  },
  {
    0x0B,
    STRING_TOKEN (STR_PCI_CLASS_SUB060B_STRING),
    PCIPifClass_060b
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0680_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_07[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0700_STRING),
    PCIPifClass_0700
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0701_STRING),
    PCIPifClass_0701
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_SUB0702_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_SUB0703_STRING),
    PCIPifClass_0703
  },
  {
    0x04,
    STRING_TOKEN (STR_PCI_CLASS_SUB0704_STRING),
    PCIBlankEntry
  },
  {
    0x05,
    STRING_TOKEN (STR_PCI_CLASS_SUB0705_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0780_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_08[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0800_STRING),
    PCIPifClass_0800
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0801_STRING),
    PCIPifClass_0801
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_SUB0802_STRING),
    PCIPifClass_0802
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_SUB0803_STRING),
    PCIPifClass_0803
  },
  {
    0x04,
    STRING_TOKEN (STR_PCI_CLASS_SUB0804_STRING),
    PCIBlankEntry
  },
  {
    0x05,
    STRING_TOKEN (STR_PCI_CLASS_SUB0805_STRING),
    PCIBlankEntry
  },
  {
    0x06,
    STRING_TOKEN (STR_PCI_CLASS_SUB0806_STRING),
    PCIBlankEntry
  },
  {
    0x07,
    STRING_TOKEN (STR_PCI_CLASS_SUB0807_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0880_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_09[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0900_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0901_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_SUB0902_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_SUB0903_STRING),
    PCIBlankEntry
  },
  {
    0x04,
    STRING_TOKEN (STR_PCI_CLASS_SUB0904_STRING),
    PCIPifClass_0904
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0980_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_0a[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0A00_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0A80_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_0b[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0B00_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0B01_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_SUB0B02_STRING),
    PCIBlankEntry
  },
  {
    0x10,
    STRING_TOKEN (STR_PCI_CLASS_SUB0B10_STRING),
    PCIBlankEntry
  },
  {
    0x20,
    STRING_TOKEN (STR_PCI_CLASS_SUB0B20_STRING),
    PCIBlankEntry
  },
  {
    0x30,
    STRING_TOKEN (STR_PCI_CLASS_SUB0B30_STRING),
    PCIBlankEntry
  },
  {
    0x40,
    STRING_TOKEN (STR_PCI_CLASS_SUB0B40_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0B80_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_0c[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0C00_STRING),
    PCIPifClass_0c00
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0C01_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_SUB0C02_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_SUB0C03_STRING),
    PCIPifClass_0c03
  },
  {
    0x04,
    STRING_TOKEN (STR_PCI_CLASS_SUB0C04_STRING),
    PCIBlankEntry
  },
  {
    0x05,
    STRING_TOKEN (STR_PCI_CLASS_SUB0C05_STRING),
    PCIBlankEntry
  },
  {
    0x06,
    STRING_TOKEN (STR_PCI_CLASS_SUB0C06_STRING),
    PCIBlankEntry
  },
  {
    0x07,
    STRING_TOKEN (STR_PCI_CLASS_SUB0C07_STRING),
    PCIPifClass_0c07
  },
  {
    0x08,
    STRING_TOKEN (STR_PCI_CLASS_SUB0C08_STRING),
    PCIBlankEntry
  },
  {
    0x09,
    STRING_TOKEN (STR_PCI_CLASS_SUB0C09_STRING),
    PCIBlankEntry
  },
  {
    0x0A,
    STRING_TOKEN (STR_PCI_CLASS_SUB0C0A_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0C80_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_0d[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0D00_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0D01_STRING),
    PCIPifClass_0d01
  },
  {
    0x10,
    STRING_TOKEN (STR_PCI_CLASS_SUB0D10_STRING),
    PCIBlankEntry
  },
  {
    0x11,
    STRING_TOKEN (STR_PCI_CLASS_SUB0D11_STRING),
    PCIBlankEntry
  },
  {
    0x12,
    STRING_TOKEN (STR_PCI_CLASS_SUB0D12_STRING),
    PCIBlankEntry
  },
  {
    0x20,
    STRING_TOKEN (STR_PCI_CLASS_SUB0D20_STRING),
    PCIBlankEntry
  },
  {
    0x21,
    STRING_TOKEN (STR_PCI_CLASS_SUB0D21_STRING),
    PCIBlankEntry
  },
  {
    0x40,
    STRING_TOKEN (STR_PCI_CLASS_SUB0D40_STRING),
    PCIBlankEntry
  },
  {
    0x41,
    STRING_TOKEN (STR_PCI_CLASS_SUB0D41_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0D80_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_0e[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB0E00_STRING),
    PCIPifClass_0e00
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_0f[] = {
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB0F01_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_SUB0F02_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_SUB0F03_STRING),
    PCIBlankEntry
  },
  {
    0x04,
    STRING_TOKEN (STR_PCI_CLASS_SUB0F04_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB0F80_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_10[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB1000_STRING),
    PCIBlankEntry
  },
  {
    0x10,
    STRING_TOKEN (STR_PCI_CLASS_SUB1010_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB1080_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_11[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB1100_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_SUB1101_STRING),
    PCIBlankEntry
  },
  {
    0x10,
    STRING_TOKEN (STR_PCI_CLASS_SUB1110_STRING),
    PCIBlankEntry
  },
  {
    0x20,
    STRING_TOKEN (STR_PCI_CLASS_SUB1120_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_SUB1180_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_12[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB1200_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_13[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB1300_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCISubClass_40[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_SUB4000_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

//
// Programming Interface entries
//
PCI_CLASS_ENTRY PCIPifClass_0100[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF010000_STRING),
    PCIBlankEntry
  },
  {
    0x11,
    STRING_TOKEN (STR_PCI_CLASS_PIF010011_STRING),
    PCIBlankEntry
  },
  {
    0x12,
    STRING_TOKEN (STR_PCI_CLASS_PIF010012_STRING),
    PCIBlankEntry
  },
  {
    0x13,
    STRING_TOKEN (STR_PCI_CLASS_PIF010013_STRING),
    PCIBlankEntry
  },
  {
    0x21,
    STRING_TOKEN (STR_PCI_CLASS_PIF010021_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0101[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF010100_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF010101_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_PIF010102_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_PIF010103_STRING),
    PCIBlankEntry
  },
  {
    0x04,
    STRING_TOKEN (STR_PCI_CLASS_PIF010104_STRING),
    PCIBlankEntry
  },
  {
    0x05,
    STRING_TOKEN (STR_PCI_CLASS_PIF010105_STRING),
    PCIBlankEntry
  },
  {
    0x06,
    STRING_TOKEN (STR_PCI_CLASS_PIF010106_STRING),
    PCIBlankEntry
  },
  {
    0x07,
    STRING_TOKEN (STR_PCI_CLASS_PIF010107_STRING),
    PCIBlankEntry
  },
  {
    0x08,
    STRING_TOKEN (STR_PCI_CLASS_PIF010108_STRING),
    PCIBlankEntry
  },
  {
    0x09,
    STRING_TOKEN (STR_PCI_CLASS_PIF010109_STRING),
    PCIBlankEntry
  },
  {
    0x0a,
    STRING_TOKEN (STR_PCI_CLASS_PIF01010A_STRING),
    PCIBlankEntry
  },
  {
    0x0b,
    STRING_TOKEN (STR_PCI_CLASS_PIF01010B_STRING),
    PCIBlankEntry
  },
  {
    0x0c,
    STRING_TOKEN (STR_PCI_CLASS_PIF01010C_STRING),
    PCIBlankEntry
  },
  {
    0x0d,
    STRING_TOKEN (STR_PCI_CLASS_PIF01010D_STRING),
    PCIBlankEntry
  },
  {
    0x0e,
    STRING_TOKEN (STR_PCI_CLASS_PIF01010E_STRING),
    PCIBlankEntry
  },
  {
    0x0f,
    STRING_TOKEN (STR_PCI_CLASS_PIF01010F_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_PIF010180_STRING),
    PCIBlankEntry
  },
  {
    0x81,
    STRING_TOKEN (STR_PCI_CLASS_PIF010181_STRING),
    PCIBlankEntry
  },
  {
    0x82,
    STRING_TOKEN (STR_PCI_CLASS_PIF010182_STRING),
    PCIBlankEntry
  },
  {
    0x83,
    STRING_TOKEN (STR_PCI_CLASS_PIF010183_STRING),
    PCIBlankEntry
  },
  {
    0x84,
    STRING_TOKEN (STR_PCI_CLASS_PIF010184_STRING),
    PCIBlankEntry
  },
  {
    0x85,
    STRING_TOKEN (STR_PCI_CLASS_PIF010185_STRING),
    PCIBlankEntry
  },
  {
    0x86,
    STRING_TOKEN (STR_PCI_CLASS_PIF010186_STRING),
    PCIBlankEntry
  },
  {
    0x87,
    STRING_TOKEN (STR_PCI_CLASS_PIF010187_STRING),
    PCIBlankEntry
  },
  {
    0x88,
    STRING_TOKEN (STR_PCI_CLASS_PIF010188_STRING),
    PCIBlankEntry
  },
  {
    0x89,
    STRING_TOKEN (STR_PCI_CLASS_PIF010189_STRING),
    PCIBlankEntry
  },
  {
    0x8a,
    STRING_TOKEN (STR_PCI_CLASS_PIF01018A_STRING),
    PCIBlankEntry
  },
  {
    0x8b,
    STRING_TOKEN (STR_PCI_CLASS_PIF01018B_STRING),
    PCIBlankEntry
  },
  {
    0x8c,
    STRING_TOKEN (STR_PCI_CLASS_PIF01018C_STRING),
    PCIBlankEntry
  },
  {
    0x8d,
    STRING_TOKEN (STR_PCI_CLASS_PIF01018D_STRING),
    PCIBlankEntry
  },
  {
    0x8e,
    STRING_TOKEN (STR_PCI_CLASS_PIF01018E_STRING),
    PCIBlankEntry
  },
  {
    0x8f,
    STRING_TOKEN (STR_PCI_CLASS_PIF01018F_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0105[] = {
  {
    0x20,
    STRING_TOKEN (STR_PCI_CLASS_PIF010520_STRING),
    PCIBlankEntry
  },
  {
    0x30,
    STRING_TOKEN (STR_PCI_CLASS_PIF010530_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0106[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF010600_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF010601_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_PIF010602_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0107[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF010700_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF010701_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0108[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF010800_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF010801_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_PIF010802_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_PIF010803_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0109[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF010900_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF010901_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};


PCI_CLASS_ENTRY PCIPifClass_0300[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF030000_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF030001_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0403[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF040300_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_PIF040380_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0604[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF060400_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF060401_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0608[] = {
  {
    0x40,
    STRING_TOKEN (STR_PCI_CLASS_PIF060800_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_PIF060801_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0609[] = {
  {
    0x40,
    STRING_TOKEN (STR_PCI_CLASS_PIF060940_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_PIF060980_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_060b[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF060B00_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF060B01_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0700[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF070000_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF070001_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_PIF070002_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_PIF070003_STRING),
    PCIBlankEntry
  },
  {
    0x04,
    STRING_TOKEN (STR_PCI_CLASS_PIF070004_STRING),
    PCIBlankEntry
  },
  {
    0x05,
    STRING_TOKEN (STR_PCI_CLASS_PIF070005_STRING),
    PCIBlankEntry
  },
  {
    0x06,
    STRING_TOKEN (STR_PCI_CLASS_PIF070006_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0701[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF070100_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF070101_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_PIF070102_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_PIF070103_STRING),
    PCIBlankEntry
  },
  {
    0xfe,
    STRING_TOKEN (STR_PCI_CLASS_PIF0701FE_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0703[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF070300_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF070301_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_PIF070302_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_PIF070303_STRING),
    PCIBlankEntry
  },
  {
    0x04,
    STRING_TOKEN (STR_PCI_CLASS_PIF070304_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0800[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF080000_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF080001_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_PIF080002_STRING),
    PCIBlankEntry
  },
  {
    0x10,
    STRING_TOKEN (STR_PCI_CLASS_PIF080010_STRING),
    PCIBlankEntry
  },
  {
    0x20,
    STRING_TOKEN (STR_PCI_CLASS_PIF080020_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0801[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF080100_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF080101_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_PIF080102_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0802[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF080200_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF080201_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_PIF080202_STRING),
    PCIBlankEntry
  },
  {
    0x03,
    STRING_TOKEN (STR_PCI_CLASS_PIF080203_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0803[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF080300_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF080301_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_PIF080302_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0904[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF090400_STRING),
    PCIBlankEntry
  },
  {
    0x10,
    STRING_TOKEN (STR_PCI_CLASS_PIF090410_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0c00[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF0C0000_STRING),
    PCIBlankEntry
  },
  {
    0x10,
    STRING_TOKEN (STR_PCI_CLASS_PIF0C0010_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0c03[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF0C0300_STRING),
    PCIBlankEntry
  },
  {
    0x10,
    STRING_TOKEN (STR_PCI_CLASS_PIF0C0310_STRING),
    PCIBlankEntry
  },
  {
    0x20,
    STRING_TOKEN (STR_PCI_CLASS_PIF0C0320_STRING),
    PCIBlankEntry
  },
  {
    0x30,
    STRING_TOKEN (STR_PCI_CLASS_PIF0C0330_STRING),
    PCIBlankEntry
  },
  {
    0x40,
    STRING_TOKEN (STR_PCI_CLASS_PIF0C0340_STRING),
    PCIBlankEntry
  },
  {
    0x80,
    STRING_TOKEN (STR_PCI_CLASS_PIF0C0380_STRING),
    PCIBlankEntry
  },
  {
    0xfe,
    STRING_TOKEN (STR_PCI_CLASS_PIF0C03FE_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0c07[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF0C0700_STRING),
    PCIBlankEntry
  },
  {
    0x01,
    STRING_TOKEN (STR_PCI_CLASS_PIF0C0701_STRING),
    PCIBlankEntry
  },
  {
    0x02,
    STRING_TOKEN (STR_PCI_CLASS_PIF0C0702_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0d01[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF0D0100_STRING),
    PCIBlankEntry
  },
  {
    0x10,
    STRING_TOKEN (STR_PCI_CLASS_PIF0D0110_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING), /* null string ends the list */
    NULL
  }
};

PCI_CLASS_ENTRY PCIPifClass_0e00[] = {
  {
    0x00,
    STRING_TOKEN (STR_PCI_CLASS_PIF0E0000_STRING),
    PCIBlankEntry
  },
  {
    0x00,
    STRING_TOKEN (STR_NULL_STRING  ), /* null string ends the list */
    NULL
  }
};
