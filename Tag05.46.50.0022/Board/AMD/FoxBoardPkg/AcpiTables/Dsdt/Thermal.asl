//
//  Thermal device
//
Scope(\_TZ)
{
  ThermalZone(TZ01)
  {

    Name (CRTT, 110)  // Default _CRT temperature.
    Name (PSVT, 50)  // Default _PSV temperature.
    Name (TSPS, 20)  // Default 2s for _TSP time.

    Method(_TMP,0,Serialized)
    {
      If(\_SB.PCI0.LPC0.ECOK())
      {
        If (LNot(Acquire(\_SB.PCI0.LPC0.EC0.Z009, 300))) {
          Store (\_SB.PCI0.LPC0.EC0.DIEH, Local0)
          Release(\_SB.PCI0.LPC0.EC0.Z009)
          Return(Add(2732,Multiply(Local0,10)))
        }
        Return(2932) // Default 20 degrees C
      }
      Else
      {
        Return(2932) // Default 20 degrees C
      }
    }

    Method(_PSL,0,Serialized)
    {
        Return(Package() {
            \_SB.PLTF.C000,
            \_SB.PLTF.C001,
            \_SB.PLTF.C002,
            \_SB.PLTF.C003,
            \_SB.PLTF.C004,
            \_SB.PLTF.C005,
            \_SB.PLTF.C006,
            \_SB.PLTF.C007,
            \_SB.PLTF.C008,
            \_SB.PLTF.C009,
            \_SB.PLTF.C00A,
            \_SB.PLTF.C00B,
            \_SB.PLTF.C00C,
            \_SB.PLTF.C00D,
            \_SB.PLTF.C00E,
            \_SB.PLTF.C00F
        })
    }

    Method(_CRT,0,Serialized)
    {
      Return(Add(2732,Multiply(CRTT,10)))
    }

    // Method(_PSV,0,Serialized) {
    //   Return(Add(2732,Multiply(PSVT,10)))
    // }

    //
    // DP [%] = _TC1 * ( Tn - Tn-1 ) + _TC2 * (Tn - Tt)
    //
    Method(_TC1,0,Serialized)
    {
      Return(1)
    }

    Method(_TC2,0,Serialized)
    {
      Return(2)
    }

    Method(_TSP,0,Serialized)
    {
      Return(TSPS)
    }
  }
}
