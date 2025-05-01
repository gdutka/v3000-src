/** @file

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
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
            If (LEqual(RVID, 0xFFFFFFFF))
            {
                Return (Zero)
            }
            Return (GSTA ())
		}
		Method(_ON, 0, NotSerialized)
		{
            If (LNotEqual(DGPU_BRIDGE_SCOPE.GSTA(), Zero)) //check the dGPU OFF?
            {
              Return ()
            }            
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
            If (LNotEqual(DGPU_BRIDGE_SCOPE.GSTA(), One)) //check the dGPU ON?
            {
              Return ()
            }
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
        RVID, 32,
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
    /* GSTA: Get dGPU Power state
     * Arguments: (0)
     *   None
     *
     * Return: Zero: Current Power state is OFF
     *         One: Current Power state is ON
     *
     * Note: OEM_customize need to add PWR_OK pin GPIO number for "PWOK".
     *       Or figure out different approach to get dGPU power state,
     *       like PXE_RST pin (PWR_EN does not recommend due to GC6 no control it).
    */
    Method (GSTA, 0, NotSerialized)
    {
        If(LEqual(M009 (PWOK), Zero)) //OEM_customize
        {
            Return (Zero) // retrun dGPU Off
        }
        Else
        {
            Return (One) // retrun dGPU On
        }
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
    

    Method (_DSM, 4, Serialized)
    {
        //
        // Check for Nvidia Optimus _DSM UUID
        //
        // NVOP_DSM_GUID {A486D8F8-0BDA-471B-A72B-6042A6B5BEE0}
        If (LEqual (Arg0, ToUUID ("A486D8F8-0BDA-471B-A72B-6042A6B5BEE0")))
        {
            Return (DGPU_SCOPE.NVOP (Arg0, Arg1, Arg2, Arg3))
        }

        //
        // Check for Nvidia GPS _DSM UUID
        //
        // GPS_DSM_GUID {A3132D01-8CDA-49BA-A52E-BC9D46DF6B81}
        If (LEqual (Arg0, ToUUID ("A3132D01-8CDA-49BA-A52E-BC9D46DF6B81")))
        {
            If (LNotEqual (DGPU_SCOPE.GPSS, Zero))
            {
                Return (DGPU_SCOPE.GPS (Arg0, Arg1, Arg2, Arg3))
            }
        }

        //
        // Check for Nvidia GC6 _DSM UUID
        //
        // GC6_DSM_GUID {CBECA351-067B-4924-9CBD-B46B00B86F34}
        If (LEqual (Arg0, ToUUID ("CBECA351-067B-4924-9CBD-B46B00B86F34")))
        {
            If (LNotEqual (DGPU_SCOPE.GC6S, Zero))
            {
                Return (DGPU_SCOPE.NVJT (Arg0, Arg1, Arg2, Arg3))
            }
        }

        //
        // Check for Nvidia NBCI _DSM UUID
        //
        // NBCI_DSM_GUID {D4A50B75-65C7-46F7-BFB7-41514CEA0244}
        If (LEqual (Arg0, ToUUID ("D4A50B75-65C7-46F7-BFB7-41514CEA0244")))
        {
            If (LNotEqual (DGPU_SCOPE.NBCS, Zero))
            {
                Return (DGPU_SCOPE.NBCI (Arg0, Arg1, Arg2, Arg3))
            }
        }

        Return (STATUS_ERROR_UNSPECIFIED)
    }
}
