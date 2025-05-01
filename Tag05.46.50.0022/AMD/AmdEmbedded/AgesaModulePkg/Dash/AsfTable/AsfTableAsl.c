/*
;*****************************************************************************
;
; Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
;
;*****************************************************************************
*/

#include "AsfTableAsl.h"

ASF_TABLE_DESCRIPTION ASFT = {
  {
    ASF_TABLE_DESCRIPTION_SIGNATURE,  // Signature
    sizeof (ASF_TABLE_DESCRIPTION),   // Length
    ASF_TABLE_DESCRIPTION_REVISION,   // Revision
    0,                                // Checksum
    {'A', 'M', 'D', ' ', ' ', ' '},   // Oem ID
    0x20202063616C694C,               // Oem Table ID
    0x00000001,                       // Oem Revision
    0x20444D41,                       // Creator ID
    0x00000001                        // Creator Revision
  },
  {
    //
    // ASF INFO
    //
    {
      0x00,                           // "ASF_INFO" Type Value
      0x00,                           // Reserved
      sizeof (ACPI_TABLE_ASF_INFO)    // "ASF_INFO" Type Length
    },
    0x01,                             // Minimum Watchdog Reset Value
    0x02,                             // Minimum ASF Sensor Interpoll Wait Time
    0x0000,                           // System ID
    0x00000000,                       // IANA Manufacture ID
    0x00,                             // Feature Flags
    {0x00, 0x00, 0x00}                // Reserved
  },
  {
    //
    // ASF ALRT
    //
    {
      0x01,                           // "ASF_ALRT" Type Value
      0x00,                           // Reserved
      sizeof (ACPI_TABLE_ASF_ALRT)    // "ASF_ALRT" Type Length
    },
    0x00,                             // Assertion Event Bit Mask
    0x00,                             // De-assertion Event Bit Mask
    0x01,                             // Number Of Alerts
    0x0C,                             // Array Element Length
    //
    //  ASF ALERT DATA
    //
    {
      0x03,                           // Device Address
      0x02,                           // Command
      0xC1,                           // Data Mask
      0x03,                           // Compare Value
      0x01,                           // Event Sensor Type
      0x01,                           // Event Type
      0x00,                           // Event Offset
      0x18,                           // Event Source Type
      0x04,                           // Event Severity
      0x10,                           // Sensor Number
      0x07,                           // Entity
      0x02                            // Entity Instance
    }
  },
  {
    //
    // ASF RCTL
    //
    {
      0x02,                           // "ASF_RCTL" Type Value
      0x00,                           // Reserved
      sizeof (ACPI_TABLE_ASF_RCTL)    // "ASF_RCTL" Type Length
    },
    0x04,                             // Number Of Controls
    0x04,                             // Array Element Length
    0x0000,                           // Reserved
    //
    // ASF CONTROL DATA
    //
    {
    // Function, Device Adress, Command, Data Value
      0x00, 0x71, 0x50, 0x00,         // Control Function0: Reset system
      0x01, 0x71, 0x52, 0x00,         // Control Function1: Power Off system
      0x02, 0x71, 0x51, 0x00,         // Control Function2: Power On system
      0x03, 0x71, 0x53, 0x00          // Control Function3: Power Cycle Reset system
    }
  },
  {
    //
    // ASF RMCP
    //
    {
      0x03,                           // "ASF_RMCP" Type Value
      0x00,                           // Reserved
      sizeof (ACPI_TABLE_ASF_RMCP)    // "ASF_RMCP" Type Length
    },
    //
    // Remote Control Capabilities
    //
    {
      0x00,
      0x00,
      0x00,
      0x00,
      0x00,
      0x00,
      0x0F
    },
    0x01,                             // Boot Option Completion Code
    0x00000000,                       // IANA Enterprise ID
    0x00,                             // Special Command
    {
      0x00,                           // Special Command Parameter[0]
      0x00                            // Special Command Parameter[1]
    },
    {
      0x00,                           // Boot Options[0]
      0x00                            // Boot Options[1]
    },
    {
      0x00,                           // OEM Parameters[0]
      0x00                            // OEM Parameters[1]
    },
  },
  //
  // ASF ADDR
  //
  {
    {
      0x04,                           // "ASF_ADDR" Type Value
      0x00,                           // Reserved
      sizeof (ACPI_TABLE_ASF_ADDR)    // "ASF_ADDR" Type Length
    },
    0x21,                             // SEEPROM Address
    0x02,                             // Number Of Devices
    //
    // Fixed SMBus Address
    //
    {
      0x05,
      0x0D
    }
  }
};