/** @file

;******************************************************************************
;* Copyright (c) 2018 - 2019, Insyde Software Corp. All Rights Reserved.
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
    For NVIDIA general support, SBIOS has to set below GPIO config:
    - Please search keyword: IBV_customize and OEM_customize to match platform design
*/

Scope (\)
{
	Name(REST, 0x8C)
	Name(PWEN, 0x8D)
	Name(PWOK, 0x8F)
}

Scope (DGPU_BRIDGE_SCOPE)
{
    Method (SGPC, 1, NotSerialized)
    {
        If (LEqual (Arg0, One))
        {
            M000 (0x9D)
            Sleep (0x05)
            If (LEqual (M009 (PWEN), One))
            {
                Return ()
            }
            M010 (REST, Zero)
            Sleep (One)
            M010 (PWEN, One)
            Sleep (0x07)
            M010 (REST, One)
            Sleep (0x64)
            Store (Zero, LNKD)
            Store (0x7FFFFFFF, Local6)
            Or (Local6, 0x80000000, Local6)
            Store (M017 (Zero, One, One, 0x19, Zero, 0x08), Local2)
            Store (M019 (Zero, One, One, 0x54), Local1)
            M020 (Zero, One, One, 0x54, And (Local1, 0xFFFF7FFC))
            Store (One, Local4)
            Store (0x28, Local5)
            While(LAnd(Local4, Local5))
				{
					Store(M019(Local2, Zero, Zero, Zero), Local0)
					If(LNotEqual(Local0, Local6))
					{
						Store(Zero, Local4)
					}
					Else
					{
						Sleep(0x05)
						Decrement(Local5)
					}
				}
				Store(DGPU_SCOPE.LTRE, DGPU_BRIDGE_SCOPE.LREN)
				Store(One, DGPU_BRIDGE_SCOPE.CEDR)
				M020(Zero, One, One, 0x54, And(Local1, 0xFFFF7FFF))
				M000(0xA0)
		}
		Else
		{
			M000(0xA1)
			M010(PWEN, One)
			Store(DGPU_BRIDGE_SCOPE.LREN, DGPU_SCOPE.LTRE)
			Store(One, LNKD)
			M010(REST, Zero)
			Sleep(One)
			M010(PWEN, Zero)
			Sleep(0x64)
			M000(0xA3)
		}
	}

	PowerResource(PG00, 0, 0)
	{
		Name(M239, One)
		Method(_STA, 0, NotSerialized)
		{
			Return(0x0F)
		}
		Method(_ON, 0, NotSerialized)
		{
			If(LEqual(M239, Zero))
			{
				SGPC(One)
				Store(0x06, DGPU_BRIDGE_SCOPE.CMDR)
				Store(Zero, DGPU_BRIDGE_SCOPE.D0ST)
                Store (PCI_SCOPE.DSSV, DGPU_SCOPE.SSSV) // OEM_customize: fill SVID/SSID in DWORD
			}
			Store(One, M239)
		}
		Method(_OFF, 0, NotSerialized)
		{
			If(LEqual(M239, One))
			{
				SGPC(Zero)
			}
			Store(Zero, M239)
		}
	}
	
	Name(_PR0, Package(1) {PG00})
	Name(_PR2, Package(1) {PG00})
	Name(_PR3, Package(1) {PG00})
	Name(_S0W, 0x04)

    OperationRegion (RPCX, SystemMemory, PCI_SCOPE.DGBA, 0x1000) // GPP0 Root Port.
    Field (RPCX, DWordAcc, NoLock, Preserve)
    {
        Offset (0x04),
        CMDR,  8,       // Command Register
        Offset (0x19),
        PRBN,  8,       // Primary Bus Number
        Offset (0x54),  // Power Management Status/Control Register
        D0ST,  2,       // POWER_STATE
        Offset (0x62),  // Device Status Register
        CEDR,  1,       // CORR_ERR
        offset (0x68),  // Link Control Register
        ASPM,  2,       // PM_CONTROL
            ,  2,
        LNKD,  1,       // LINK_DIS
        Offset (0x80),  // Device Capabilities 2 Register
            ,  8,
            ,  2,   
        LREN,  1,       // LTR_EN
    }
}

Scope (DGPU_SCOPE)
{
    // Name (_ADR, Zero)
    Name (LTRE, Zero)
    Name (DGPS, Zero)
    Name (_PSC, Zero)
    Name (GPRF, Zero)
    Name (OPCE, 0x02)
    // NVIDIA NBCI related start
    Name (GSV1, Buffer(0x01)                 // Buffer for VK key#1
    {
        0x00
    })
    Name (GSV2, Buffer(0x01)                 // Buffer for VK key#2 (Optional)
    {
        0x00
    })
    Name (GSDR, Buffer(0x01)                 // Buffer for DR key
    {
        0x00
    })

    // Method (_INI, 0, NotSerialized)
    // {
    //     Store (0xFF, DGPU_BRIDGE_SCOPE.VGA._ADR)
    //     Store (0xFE, DGPU_BRIDGE_SCOPE.HDAU._ADR)
    // }

    // Method (_RMV, 0, NotSerialized)
    // {
    //     Return (Zero)
    // }

    OperationRegion (PCIM, SystemMemory, PCI_SCOPE.DGDA, 0x1000)
    Field (PCIM, DWordAcc, NoLock, Preserve)
    {
        NVID, 16,
        NDID, 16, 
        CMDR, 8, 
        VGAR, 2008, 
        Offset (0x48B),
        , 1, 
        HDAE, 1,
    }

    OperationRegion (DGPU, SystemMemory, PCI_SCOPE.DGDA, 0x100) // dGPU
    Field (DGPU, DWordAcc, NoLock, Preserve)
    {
        Offset (0x40), // Offset(64)
        SSSV, 32
    }

    OperationRegion (PCIS, PCI_Config, Zero, 0x100)
    Field (PCIS, AnyAcc, NoLock, Preserve)
    {
        PVID, 16, 
        PDID, 16,
    }

    Method (_PS0, 0, NotSerialized)
    {
        Store (Zero, _PSC)
        If (LNotEqual (DGPS, Zero))
        {
            DGPU_BRIDGE_SCOPE.PG00._ON()
            Store (Zero, DGPS)
        }  
    }

    Method (_PS3, 0, NotSerialized)
    {
        If (LEqual (OPCE, 0x03))
        {
            If (LEqual (DGPS, Zero))
            {
                DGPU_BRIDGE_SCOPE.PG00._OFF()
                Store (One, DGPS)
            }
            Store (0x02, OPCE)
        }
        Store (0x03, _PSC)
    }

    Method (SGST, 0, Serialized)
    {
        If (LNotEqual (PVID, 0xFFF))
        {
            Return (0x0F)
        }
        Return (Zero)
    }

    Method (CMPB, 2, NotSerialized)
    {
        Store (SizeOf(Arg0), Local1)
        If (LNotEqual (Local1, SizeOf(Arg1)))
        {
            Return (Zero)
        }
        Store (Zero, Local0)
        While (LLess (Local0, Local1))
        {
            If (LNotEqual (DerefOf (Index (Arg0, Local0)), DerefOf(Index(Arg1, Local0))))
            {
                Return (Zero)
            }
            Increment (Local0)
        }
        Return (One)
    }
    
    Method (NVOP, 4, Serialized)
    {
        If (LEqual (Arg2, Zero))
        {
            Return (Buffer (0x04)
            {
                0x01, 0x00, 0x00, 0x04
            })
        }
        Else
        {
            If (LEqual (Arg2, 0x1A))
            {
                CreateField (Arg3, 0x18, 0x02, OMPR)
                CreateField (Arg3, Zero, One, FLCH)
                CreateField (Arg3, One, One, DVSR)
                CreateField (Arg3, 0x02, One, DVSC)
                If (ToInteger (FLCH))
                {
                    Store (OMPR, DGPU_SCOPE.OPCE)
                }
                Store (Buffer (0x04)
                {
                    0x00, 0x00, 0x00, 0x00
                }, Local0)
                CreateField (Local0, Zero, One, OPEN)
                CreateField (Local0, 0x03, 0x02, CGCS)
                CreateField (Local0, 0x06, One, SHPC)
                CreateField (Local0, 0x08, One, SNSR)
                CreateField (Local0, 0x18, 0x03, DGPC)
                CreateField (Local0, 0x1B, 0x02, HDAC)
                Store (One, OPEN)
                Store (One, SHPC)
                Store (0x03, HDAC)
                Store (One, DGPC)
                If (ToInteger (DVSC))
                {
                    If (ToInteger (DVSR))
                    {
                        Store (One, DGPU_SCOPE.GPRF)
                    }
                    Else
                    {
                        Store (Zero, DGPU_SCOPE.GPRF)
                    }
                }
                Store (DGPU_SCOPE.GPRF, SNSR)
                If (LNotEqual (DGPU_SCOPE.SGST(), Zero))
                {
                    Store (0x03, CGCS)
                }
                Return (Local0)
            }
        }
        Return (0x80000002)
    }
    
    Method (NBCI, 4, Serialized)
    {
        If (LNotEqual (Arg1, 0x0102))
        {
            Return (STATUS_ERROR_UNSUPPORTED)
        }
        //
        // Sub-Function 0: NBCI_FUNC_SUPPORT
        //
        If (LEqual (Arg2, NBCI_FUNC_SUPPORT))
        {
            Return (Buffer (0x04)
            {
                0x01, 0x00, 0x01, 0x00                
            })
        }
        //
        // Sub-Function 1: NBCI_FUNC_PLATCAPS
        //
        If (LEqual (Arg2, NBCI_FUNC_PLATCAPS))
        {
            Name (TEMP, Buffer (0x04)
            {
                0x00, 0x00, 0x00, 0x00
            })
            CreateDWordField (TEMP, Zero, STS0)
            Or (STS0, Zero, STS0)
            Return (TEMP)
        }
        //
        // Sub-Function 16: NBCI_FUNC_GETOBJBYTYPE
        //
        If (LEqual (Arg2, NBCI_FUNC_GETOBJBYTYPE))
        {
            CreateWordField (Arg3, 0x02, BFF0)
            // If (LEqual (BFF0, 0x564B)) // Check DR object
            // {
            //     If (LEqual (DEID, 0x0D04))
            //     {
            //         Return (GSV1)
            //     }
            //     If (LEqual (DEID, 0x0C04))
            //     {
            //         Return (GSV2)
            //     }
            // }
            If (LEqual (BFF0, 0x4452)) {
                // OEM_customize: DR Key is an data Object which might vary on different eDP panel. OEM SBIOS need to check corresponding info then pick correct VK object to return.
                Return (GSDR)
            }
        }
        //
        // Sub-Function 20: NBCI_FUNC_GETBACKLIGHT
        //
        If (LEqual (Arg2, NBCI_FUNC_GETBACKLIGHT))
        {
            // OEM_customize: Package returned in this interface is panel related. OEM SBIOS need to follow definition in NBSI Spec to reflect real panel feature in this pacakge.
            Return (Package (32)
            {
                0x8001A450, 0x0200, Zero, Zero, 0x05, One, 0xC8, 0x32, 0x03E8, 0x0B,
                0x32, 0x64, 0x96, 0xC8, 0x012C, 0x0190, 0x01FE, 0x0276, 0x02F8, 0x0366,
                0x03E8, Zero, 0x64, 0xC8, 0x012C, 0x0190, 0x01F4, 0x0258, 0x02BC,
                0x0320, 0x0384, 0x03E8
            })
        }
        Return (Zero)
    }

    Method (_DSM, 4, Serialized)
    {
        CreateByteField (Arg0, 0x03, GUID)
        If (CMPB (Arg0, ToUUID("A486D8F8-0BDA-471B-A72B-6042A6B5BEE0")))
        {
            Return (DGPU_SCOPE.NVOP (Arg0, Arg1, Arg2, Arg3))
        }
        Else
        {
            If (LEqual (Arg0, ToUUID ("D4A50B75-65C7-46F7-BFB7-41514CEA0244")))
            {
                Return (DGPU_SCOPE.NBCI (Arg0, Arg1, Arg2, Arg3))
            }
            Else
            {
                Return (0x80000002)
            }
        }
        Return (0x80000001)
    }

    //  method  : _ROM
    //  Arguments:
    //          Arg0:   Integer     Offset of the graphics device ROM data
    //          Arg1:   Integer     Size of the buffer to fill in (up to 4K)
    //
    //  Return Value:
    //          Returns Buffer      Buffer of requested video ROM bytes
        Method (_ROM, 2)
    {
        Store (Arg0, Local0)
        Store (Arg1, Local1)

        Name (VROM, Buffer(Local1) {0x00}) // Create 4K buffer to return to DD

        If (LGreater (Local1, 0x1000))
        {
            Store (0x1000, Local1) // Return dummy buffer if asking for more than 4K
        }

        If (LGreater (Arg0, RVBS))
        {
            Return (VROM) // Return dummy buffer if asking beyond VBIOS image
        }

        Add (Arg0, Arg1, Local2)

        If (LGreater (Local2, RVBS)) // If requested BASE+LEN > VBIOS image size
        {
            Subtract (RVBS, Local0, Local1) // Limit length to the final chunk of VBIOS image
        }

        Divide (Local0, 0x8000, Local3, Local4)  // (Dividend, Divisor, Remainder, Result)

        Switch (Local4)
        {
            Case (0)
            {
                Store (DGPU_SCOPE.VBS1, Local5)
            }
            Case (1)
            {
                Store (DGPU_SCOPE.VBS2, Local5)
            }
            Case (2)
            {
                Store (DGPU_SCOPE.VBS3, Local5)
            }
            Case (3)
            {
                Store (DGPU_SCOPE.VBS4, Local5)
            }
            Case (4)
            {
        	      Store (DGPU_SCOPE.VBS5, Local5)
            }
            Case (5)
            {
        	      Store (DGPU_SCOPE.VBS6, Local5)
            }
            Case (6)
            {
        	      Store (DGPU_SCOPE.VBS7, Local5)
            }
            Case (7)
            {
        	      Store (DGPU_SCOPE.VBS8, Local5)
            }
        } 
        
        Multiply (0x8000, Local4, Local4)   // (Multiplicand, Multiplier, Result)
        Subtract (Local0, Local4, Local0) 
        
        Mid (Local5, Local0, Local1, VROM)

        Return (VROM)
    }
}
