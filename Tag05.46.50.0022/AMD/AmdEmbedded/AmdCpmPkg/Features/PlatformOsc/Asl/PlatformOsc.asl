/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "PlatformOsc.aml",         // Output file
    "SSDT",                    // Signature
    0x01,                      // SSDT Revision
    "AMD",                     // OEM ID
    "CPMMSOSC",                // OEM Table ID
    0x1                        // OEM Revision
    )
{
    #include "CmnDef.asi"
    #include "CmnExt.asi"

    External (\DIMS, IntObj)
    External (\DDEP, IntObj)

    Scope (\_SB) {
        // Platform-wide capabilities check
        // Arg0 - A Buffer containing a UUID
        // Arg1 - An Integer containing a Revision ID of the buffer format
        // Arg2 - An Integer containing a count of entries in Arg3
        // Arg3 - A Buffer containing a list of DWORD capabilities
        Method (_OSC, 4, Serialized) {
            Name (MSDB, 0)      // ModernStandbyEn, the value wll update by C language during POST
            Name (U4PC, 0xFF)   // USB4 Platform Control, the value will update by C language during POST

            // Note: Since these DwordFields are accessed from the else
            // condition of UUID check, they cannot be created inside the
            // If(LEqual .... ) scope.  If put inside the LEqual, it will
            // cause problem if the code ever executes the Else (unrecognized UUID)
            // condition.

            // Create DWord-adressable fields from the Capabilities Buffer
            // The capability field of \_SB._OSC (i.e. Arg3) contains only 2 dwords
            CreateDWordField (Arg0, 0, UID0)
            CreateDWordField (Arg3, 0, CDW1)
            If (LGreaterEqual (Arg2, 2)) {
                CreateDWordField (Arg3, 4, CDW2)
            }
            If (LGreaterEqual (Arg2, 3)) {
                CreateDWordField (Arg3, 8, CDW3)
            }

            If (LEqual (Arg2, 1)) {
                CpmDebugPrint ("  FEA-ASL-\\_SB._OSC UUID 0x%X Start  CDW1 = 0x%X\n", UID0, CDW1, 0, 0, 0, 0)
            } ElseIf (LEqual (Arg2, 2)) {
                Store (CDW2, Local2)
                CpmDebugPrint ("  FEA-ASL-\\_SB._OSC UUID 0x%X Start  CDW1 = 0x%X CDW2 = 0x%X\n", UID0, CDW1, CDW2, 0, 0, 0)
            } Else {
                Store (CDW3, Local3)
                CpmDebugPrint ("  FEA-ASL-\\_SB._OSC UUID 0x%X Start  CDW1 = 0x%X CDW2 = 0x%X CDW3 = 0x%X\n", UID0, CDW1, CDW2, CDW3, 0, 0)
            }

            // Check for proper \_SB._OSC UUID
            If (LEqual (Arg0, ToUUID ("23A0D13A-26AB-486C-9C5F-0FFA525A575A"))) {
                If (LNotEqual (U4PC, 0xFF)) {
                    And (Local3, 0xFFFFFFF0, Local3)      // Clear Bit[3:0]
                    Or (Local3, U4PC, Local3)             // Bit[0] USB Tunneling
                                                          // Bit[1] DisplayPort Tunneling
                                                          // Bit[2] PCI Express Tunneling
                                                          // Bit[3] Inter-domain USB4
                }
                If (LNotEqual (Local3, CDW3)) {
                    Store (Local3, CDW3)
                    Or (CDW1, 0x10, CDW1)                 // Set Capabilities Masked
                }
            } ElseIf (LEqual (Arg0, ToUUID ("0811B06E-4A27-44F9-8D60-3CBBC22E7B48"))) {
                If (LEqual (Arg1, One)) {
                    If (LAnd (CondRefOf (\DIMS), CondRefOf (\DDEP))) {
                        If (LAnd (LEqual (\DDEP, 0x01), LEqual (And (CDW2, 0x40000), 0x40000))) {
                            // USB4 ACPI _DEP is supported
                            // Bit[18] Native USB4 is supported
                            // Issue a SW SMI to disable USB4 ACPI _DEP
                            CpmDebugPrint ("    Disable USB4 ACPI _DEP\n", 0, 0, 0, 0, 0, 0)
                            Store (\DIMS, Local1)
                            CpmTriggerSmi (Local1, 0x55, 0)
                            Store (0x00, \DDEP)
                        } // End of If (LEqual (And (CDW2, 0x40000), 0x40000))
                    } // End of If (LAnd (CondRefOf (\DIMS), CondRefOf (\DDEP)))
                    Or (Local2, 0x4, Local2)              // Enable Bit[2] _PR3 support
//                    If (LEqual(MSDB, 1)) {
                        And (Local2, 0xFFFFFF7F, Local2)  // Clear BIT7 Platform Coordinated Low Power Idle Support
//                    }
                    If (LNotEqual (U4PC, 0xFF)) {
                        Or (Local2, 0x40000, Local2)      // Set Bit[18] Native USB4 Support
                    }
                    If (LNotEqual (Local2, CDW2)) {
                        Store (Local2, CDW2)
                        Or (CDW1, 0x10, CDW1)             // Set Capabilities Masked
                    }
                } Else {
                    Or (CDW1, 0x08, CDW1)                 // Unrecognized revision
                }
            } Else {
                Or (CDW1, 0x04, CDW1)                     // Unrecognized UUID
            }

            If (LEqual (Arg2, 1)) {
                CpmDebugPrint ("  FEA-ASL-\\_SB._OSC UUID 0x%X Return CDW1 = 0x%X\n", UID0, CDW1, 0, 0, 0, 0)
            } ElseIf (LEqual (Arg2, 2)) {
                CpmDebugPrint ("  FEA-ASL-\\_SB._OSC UUID 0x%X Return CDW1 = 0x%X CDW2 = 0x%X\n", UID0, CDW1, CDW2, 0, 0, 0)
            } Else {
                CpmDebugPrint ("  FEA-ASL-\\_SB._OSC UUID 0x%X Return CDW1 = 0x%X CDW2 = 0x%X CDW3 = 0x%X\n", UID0, CDW1, CDW2, CDW3, 0, 0)
            }
            Return (Arg3)
        } // End _OSC
    }
}
