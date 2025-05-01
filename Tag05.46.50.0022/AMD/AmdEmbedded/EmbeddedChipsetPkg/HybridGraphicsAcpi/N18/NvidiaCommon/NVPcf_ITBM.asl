/** @file

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

/*
    == Readme ==
    For NVPCF support, SBIOS has to adjust below code to meet platform:
    - Please refer to NVIDIA SW design guide (DG-08778-001) to generate the NVPCF buffer in sub-func#1
    - Remember to update the checksum byte if NVPCF buffer changed
    - NVPCF is a virture device under \_SB, do not change the device path
    - OEM has to create a EC IO address by "IOBS" for the PPAB v2.0 access
    - OEM should define ITBMT_IOTRAP for ITBM using SMI
    - OEM should check the GPE _Lxx event for ITBM using SCI event
    - OEM need store MSR 0x1AD value to EC IO when the AutoFL_State InActivate
    - OEM has to update the CPC2/CPTB method. Please consult NVIDIA SW AE for more detail
    - In [Set _CPC "Highest Performance" value] case, IBV/OEM need to notify all CPU core with 0x85
*/

#define ITBMT_IOTRAP    0x820

/* OEM need add "\_SB.PCI0.PEG0.PEGP.CAFL()" into ITBM SCI event handler */
/* Please refer ..\Intel\TigerLake\TigerLakeBoardPkg\Acpi\AcpiTables\Dsdt\Gpe.asl */
//Scope (\_GPE)
//{
//  //
//  // Software GPE caused the ITBM event.
//  //
//  Method (_L62)
//  {
//    ///
//    /// Handle Intel Turbo Boost Max Technology 3.0 SCI event
//    ///
//    If (LEqual (\_SB.ITBI, 1)) {
//      If (CondRefOf (\_GPE.ITBH)) {
//        \_SB.PCI0.PEG0.PEGP.CAFL ()     /* IBV_customize: NVIDIA added */
//        \_GPE.ITBH ()
//      }
//    ///
//    /// Clear interrupt status
//    ///
//    }
//  }
//}

Scope (\_SB.PR00)
{
  Name (TBRL, ResourceTemplate() {Register(FFixedHW, 8, 0, 0x1AD, 4)})    // MSR_TURBO_RATIO_LIMIT
}

Scope (\_SB)                     // DO NOT MODIFY THE PATH
{
  Device (NPCF)
  {
    Name (PABS, One)             // Platform Power Aware Boot Controller Switch
                                 // 0: Disable Platform Power Aware Boot Controller
                                 // 1: Enable Platform Power Aware Boot Controller

    Name (HPCT, 0x82)            // Get _CPC "HighestPerformance" configuration Control Type
                                 // [1:0]
                                 // 0: NVIDIA Default
                                 // 1: Reserved
                                 // 2: Embedded Controller IO Space
                                 // [6:2] Reserved
                                 // [7] ITBM support. 0 = ITBM not support, 1=ITBM support

    If (LEqual (\_SB.ITBM, 0))
    {
      Store (0x02, HPCT)
    }

    Name (IOBS, Zero)            // IBV_customize/OEM_customize // Example port: 0x25F

    Name (CDIS, 0)               // device disable state

    Name (_HID, "NVDA0820")      // ACPI HW ID for NVPCF
    Name (_UID, "NPCF")
    Method (_STA, 0, NotSerialized)
    {
        If (LEqual (CDIS, 1))
        {
            Return (0xD)            // Disabled from _DIS 1101b - Present/Disabled (via Device Manager)
        }
        Return (0xF)
    }
    Method (_SRS, 0x1, Serialized)
    {
        Store (0, CDIS)             // Clear Disabled bit
    }
    Method (_DIS, 0x0, NotSerialized)
    {
        Store (1, CDIS)             // Set Disabled bit
    }
    Method (_DSM, 4, Serialized)
    {
      // NVPCF_DSM_GUID = 36b49710-2483-11e7-9598-0800200c9a66
      If (LEqual (Arg0, ToUUID("36b49710-2483-11e7-9598-0800200c9a66")))
      {
        If (LNotEqual (DGPU_SCOPE.NPCS, Zero))
        {
          Return (NPCF (Arg0, Arg1, Arg2, Arg3))
        }
      }
    }
    Method (NPCF, 4, Serialized)
    {
      Store ("------- NVPCF DSM --------", Debug)
      If (LNotEqual (ToInteger(Arg1), 0x100))    // Revision check
      {
        Return (STATUS_ERROR_UNSPECIFIED)
      }
      Switch (ToInteger(Arg2))
      {
        //
        // Sub-Function 0: NVPCF_FUNC_GET_SUPPORTED
        //
        Case (NVPCF_FUNC_GET_SUPPORTED)
        {
          Store ("   NVPCF sub-func#0", Debug)
          Return (Buffer() {0x07, 0x00, 0x00, 0x00})    // sub-func#0,1,2 supported
        }

        //
        // Sub-Function 1: NVPCF_FUNC_GET_STATIC_CONFIG_TABLES
        //
        Case (NVPCF_FUNC_GET_STATIC_CONFIG_TABLES)
        {
          Store ("   NVPCF sub-func#1", Debug)
          // OEM_customize
          Return (Buffer() {
                   // Device header
                   0x10,0x04,0x09,0x02,
                   // Device object entry 0
                   0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                   // Device object entry 1
                   0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                   // Topology header
                   0x10,0x04,0x05,0x05,
                   // Topology object entry 0
                   0x01,0x00,0x00,0x00,0x00,
                   // Topology object entry 1
                   0x02,0x01,0x00,0x00,0x00,
                   // Topology object entry 2
                   0x02,0x01,0x01,0x00,0x00,
                   // Topology object entry 3
                   0x02,0x01,0x02,0x00,0x00,
                   // Topology object entry 4
                   0x02,0x01,0x03,0x00,0x00,
                   // Controller header
                   0x10,0x06,0x23,0x01,
                   0x64,0x00,
                   // Controller object entry 0
                   0x01,
                   0x01,0x00,
                   0x00,0xE8,0x00,0x00,
                   0x01,0x02,0x03,0x04,
                   0x00,0x00,0x00,0x00,
                   0x2D,0x00,0x23,0x00,
                   0x00,0xFF,0xFF,0xFF,
                   0xFF,0xFF,0xFF,0xFF,
                   0x66,0x0E,0x00,0x10,
                   0x00,0x00,0x00,0x00,
                   // Single byte checksum value
                   0x4E
                   })
        }

        //
        // Sub-Function 2: NVPCF_FUNC_UPDATE_DYNAMIC_PARAMS
        //
        Case (NVPCF_FUNC_UPDATE_DYNAMIC_PARAMS)
        {
          Store ("   NVPCF sub-func#2", Debug)

          Name (PBD2, Buffer(0x08) {})
          CreateByteField (PBD2, Zero, NDPT)    // Dynamic Params Table Version (0x10)
          CreateByteField (PBD2, 0x01, NSDP)    // Size of Dynamic Params Table Header in byte (0x4)
          CreateByteField (PBD2, 0x02, NNDT)    // Number of Dynamic Params Table Entries (0x1)
          CreateByteField (PBD2, 0x03, NRES)    // Reserved (0x0)
          Store (0x10, NDPT)
          Store (0x04, NSDP)
          Store (0x01, NNDT)
          Store (0x00, NRES)
          CreateByteField (PBD2, 0x04, NOIC)    // Output Entry0 [7:0]
          CreateByteField (PBD2, 0x05, NICC)    // Output Entry0 [15:8]: "HighestPerformance" Value
          CreateWordField (PBD2, 0x06, NICA)    // Output Entry0 [31:16]: IO Space Address

          CreateByteField (Arg3, 0x04, NICM)    // Input Command
          CreateByteField (Arg3, 0x06, NCA0)    // Command Arg0

          If (LEqual (ToInteger(NICM), 0x0))    // Get Platform Power Aware Boot Controller status
          {
            Store (PABS, NOIC)                  // Return PPAB status
          }
          If (LEqual (ToInteger(NICM), 0x1))    // Set Platform Power Aware Boot Controller status
          {
            If (LEqual (ToInteger(NCA0), 0x0))  // Disable PPAB
            {
              Store (Zero, PABS)
              Store (PABS, NOIC)
              // IBV_customize/OEM_customize
              // Reduce PL2 limit to avoid AC adapter OCP
              // SBIOS control here ...
            }
            If (LEqual (ToInteger(NCA0), 0x1))  // Enable PPAB
            {
              Store (One, PABS)
              Store (PABS, NOIC)
              // IBV_customize/OEM_customize
              // Adjust PL2 limit to normal
              // SBIOS control here ...
            }
          }
          If (LNotEqual (IOBS,0x0))
          {
            // Enable a IO port if HPCT[1:0] == 2
            OperationRegion (NVIO, SystemIO, IOBS, 0x10)    // IBV_customize/OEM_customize // Example port: 0x25F
            Field (NVIO, ByteAcc, NoLock, Preserve)
            {
              CPUC, 8
            }

            If (LEqual (ToInteger(NICM), 0x2))    // Get _CPC "Highest Performance" configuration
            {
              Store (HPCT, NOIC)                  // Return Control Type
              Store (CPUC, NICC)                  // Return Highest Performance value
              Store (IOBS, NICA)                  // Return IO Space Address
            }

            If (LEqual (ToInteger(NICM), 0x3))    // Set _CPC "Highest Performance" value
            {
              If (LNotEqual (ToInteger(NCA0), 0xFF))
              {
                Store (Zero, DGPU_SCOPE.AFST)     // Set Activated AutoFL_State
                Store (NCA0, CPUC)                // Store HighestPerformance value to Embedded IO Space
              }
              Else
              {
                Store (0xFF, DGPU_SCOPE.AFST)     // Set InActivate AutoFL_State
                /* IBV_customize/OEM_customize need store the MSR 0x1AD value to EC IO */
                Store (\_SB.PR00.TBRL, CPUC)
              }
              Store (HPCT, NOIC)                  // Return Control Type
              Store (NCA0, NICC)                  // Return Highest Performance value
              Store (IOBS, NICA)                  // Return IO Space Address

              /* IBV_customize/OEM_customize should add code to Notify All CPU core */
              /* Example:
               * Notify(\_SB.CPU0, 0x85)
               * Notify(\_SB.CPU1, 0x85)
               * Notify(\_SB.CPU2, 0x85)
               * Notify(\_SB.CPU3, 0x85)
               * ...
               * Notify(\_SB.CPUX, 0x85)
               */
              \_GPE.ITBH ()
            }
          }
          Else
          {
            Return (STATUS_ERROR_UNSPECIFIED)
          }
          Return (PBD2)
        }
      }
      Return (STATUS_ERROR_UNSUPPORTED)
    }
  }
}

/* Add the code for ITBM supported */
Scope (DGPU_SCOPE)
{
  Name (AFST, 0xFF)    // Set default InActivate for AutoFL_State

  /*
   * Check AutoFL state
   * Read ITBM CPU ratio value & store to EC IO if AutoFL_State InActivate
   */
  Method (CAFL, 0, Serialized)
  {
    /* AutoFL InActivate, Store the CPU Ratio to EC-IO */
    If (LEqual (AFST, 0xFF))
    {
      /* IBV_customize/OEM_customize need define ITBMT_IOTRAP */
      OperationRegion (SMIP, SystemIO, ITBMT_IOTRAP, 1)
      Field (SMIP, ByteAcc, NoLock, Preserve)
      {
        IOB2, 8
      }
      /* EC IO */
      OperationRegion (ECIO, SystemIO, \_SB.NPCF.IOBS, 1)
      Field (ECIO, ByteAcc, NoLock, Preserve)
      {
        ECTP, 8
      }
      /* Read CPU Ratio */
      Store (IOB2, Local0)
      /* Store CPU Ratio to EC IO */
      Store (Local0, ECTP)
    }
  }
}
