/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
  "AmdCpmPmfBoard.aml", // Output file
  "SSDT",                    // Signature
  0x01,                      // SSDT Revision
  "AMD",                     // OEM ID
  "OEMPMF",                  // OEM Table ID
  0x1                        // OEM Revision
  )
{
  //
  //  PMF BOARD ACPI SAMPLE
  //  Should be custimized according to platform settings.
  //
  Scope (\_SB) {

    OperationRegion(TP80, SystemIO, 0x80, 0x4)
    Field(TP80, DWordAcc, NoLock, Preserve) {
      PMTP, 32
    }

    Name (DPX4, 0xA9F40000)
    // Name of APX4 can be changed by AMD_CPM_PMF_TABLE.NotifySbiosHeartbeatAcpiMethodObjName
    // CPM will call this method for sending PMF heartbeat periodically within AMD_CPM_PMF_TABLE.HeartbeatTimeInterval.
    // OEM should notify EC FW about PMF status in this function.
    Method (APX4, 0, Serialized)
    {
       Store (DPX4, PMTP)
       Add (DPX4, 1, DPX4)
    }

    // Name of APX7 can be changed by AMD_CPM_PMF_TABLE.SetFanTableIndexAcpiMethodObjName
    // CPM will call this method for Set Fan Table Index
    Method (APX7, 2, Serialized)
    {
       ///<         Arg0: FanControlMode
       ///<               0=Auto; 1=Manual
       ///<         Arg1: FanControlIndex
       ///<               Range 0-7
       Store ( Or(0xA9F70000, Arg0), PMTP)
       Sleep (1)
       Store ( Arg1, PMTP)
    }

    // Name of APX8 can be changed by AMD_CPM_PMF_TABLE.SliderEventNotificationAcpiMethodObjName
    // CPM will call this method for Slider Event Notification.
    Method (APX8, 1, Serialized)
    {
       ///<  Arg0: Slider Event Notification
       ///<    Bit 0: DC Best Performance position
       ///<    Bit 1: DC Better Performance position
       ///<    Bit 2: DC Better Battery position
       ///<    Bit 3: DC Battery Saver position
       ///<    Bit 4: AC Best Performance position
       ///<    Bit 5: AC Better Performance position
       ///<    Bit 6: AC Better Battery position
       Store ( Or(0xA9F80000, Arg0), PMTP)
    }

    // Name of APXA can be changed by AMD_CPM_PMF_TABLE.NotifyInBagPolicyUpdateAcpiMethodObjName
    // CPM will call this method for in Bag Policy Update.
    Method (APXA, 1, Serialized)
    {
       ///<  Arg0: Policy Mask
       ///<    Bit 0: In Bag Policy
       Store ( Or(0xA9FA0000, Arg0), PMTP)
    }

    // Name of APXE can be changed by AMD_CPM_PMF_TABLE.NotifySmartPcSolutoinUpdateAcpiMethodObjName
    // CPM will call this method for notifying Smart PC Solution Updates.
    Method (APXE, 3, Serialized)
    {
       ///<  Arg0: Pending requests Bits
       ///<    Bit 0: If bit 0 is set: Custom BIOS output1 is triggered.
       ///<    Bit 1: If bit 1 is set: Custom BIOS output2 is triggered..
       ///<  Arg1: Provide a customized output to BIOS from PMF smart policies.
       ///<  Arg2: Provide a customized output to BIOS from PMF smart policies.
       Store ( Or(0xA9FE0000, Arg0), PMTP)
       Sleep (1)
       Store ( Arg1, PMTP)
       Sleep (1)
       Store ( Arg2, PMTP)
    }
  } // End of Scope (\_SB)
}

