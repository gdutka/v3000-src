/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
DefinitionBlock (
    "MpmSensor.aml",            // Output file
    "SSDT",                     // Signature
    0x01,                       // SSDT Revision
    "AMD",                      // OEM ID
    "MPMS",                     // OEM Table ID
    0x1                         // OEM Revision
    )
{
    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External(\_SB.PCI0.LPC0.EC0.DIEL, IntObj)
    External(\_SB.PCI0.LPC0.EC0.DIEH, IntObj)
    External(\_SB.PCI0.LPC0.EC0.TSRL, IntObj)
    External(\_SB.PCI0.LPC0.EC0.TSRH, IntObj)
    External(\_SB.PCI0.LPC0.EC0.TS1L, IntObj)
    External(\_SB.PCI0.LPC0.EC0.TS1H, IntObj)
    External(\_SB.PCI0.LPC0.EC0.TS2L, IntObj)
    External(\_SB.PCI0.LPC0.EC0.TS2H, IntObj)
    External(\_SB.PCI0.LPC0.EC0.GPI8, IntObj)
    External(\_SB.MPM0, DeviceObj)

    Scope(\_SB.MPM0)     // Start \_SB scope
    {
        Name(SENS, Package() {
          //SENSOR_T
            0,                               //Sensor Id
            "0",                             //Sensor name
            0,                               //Sensor Data Length
            2,                               //Sensor Type
            "TBD",                           //OtherSensorTypeDescription
            0x41,                            //PossibleStates
            1,                               //CurrentState
            10,                              //PollingInterval
            "TBD",                           //SensorContext
          //NUMERICSENSOR_T
            1,                               //BaseUnits
            1,                               //UnitModifier
            1,                               //RateUnits
            0,                               //CurrentReading
            0,                               //NominalReading
            0,                               //NormalMax
            0,                               //NormalMin
            0,                               //MaxReadable
            0,                               //MinReadable
            0,                               //Resolution
            1,                               //Tolerance
            1,                               //Accuracy
            1,                               //IsLinear
            0,                               //Hysteresis
            40,                              //LowerThresholdNonCritical
            0,                               //UpperThresholdNonCritical
            80,                              //LowerThresholdCritical
            0,                               //UpperThresholdCritical
            120,                             //LowerThresholdFatal
            0,                               //UpperThresholdFatal
            0x3F,                            //SupportedThresholds
            0,                               //EnabledThresholds
            0,                               //SettableThresholds
          //FANINFO_T
            "TBD",                           //SystemName
            "TBD",                           //SystemModel
            "TBD",                           //SerialNumber
            0,                               //OperationalStatus
            0,                               //VariableSpeed
            3000,                            //DesiredSpeed
            0,                               //ControlMode
            0,                               //ControlModesSupported
        })

        //
        // _DSM - Device Specific Method
        //
        // Arg0:  UUID    Unique function identifier
        // Arg1:   Integer   Revision Level
        // Arg2:   Integer   Function Index (0 = Return Supported Functions)
        // Arg3:   Package   Parameters
        Method (_DSM, 4, Serialized, 0, {IntObj,BuffObj},{BuffObj, IntObj, IntObj, PkgObj})
        {
          // Optional ACPI Start Method
          If (LEqual (Arg0, ToUUID("eaf0f0c0-3286-432c-b74d-da0a7b98eb8f"))) {
            switch(ToInteger(Arg2)) {
              // Function 0: Query support functions
              case(0) {
                return (Buffer() {0x03}) //function 1 support
                }
                //Read sensor information
                case(1) {
                  switch (ToInteger(Arg3)) {
                    case(0) {
                      // how many sensors
                      Return (6)
                    }
                    case(1) {
                      //sensor index 0
                      Store(0, Index(SENS, 0))                          //sensor id
                      Store("Internel APU temperature", Index(SENS, 1)) //sensor name
                      Store(2, Index(SENS, 3))                          //sensor type 2=temperature sensor

                      Store (\_SB.PCI0.LPC0.EC0.DIEL, Local0)
                      Store (\_SB.PCI0.LPC0.EC0.DIEH, Local1)
                      Or(ShiftLeft(Local1, 8), Local0, Local2)
                      Store (Local2, Index(SENS, 12))

                      CpmDebugPrint ("sensor 0 temperature data  = 0x%X\n", Local2, 0, 0, 0, 0, 0)
                      Store(SizeOf(SENS), Index(SENS, 2))
                      CpmDebugPrint ("SENS length = 0x%X\n", SizeOf(SENS), 0, 0, 0, 0, 0)
                      Return (SENS)
                    }
                    case(2) {
                      //sensor index 1
                      Store(1, Index(SENS, 0))                          //sensor id
                      Store("Charger temperature", Index(SENS, 1))      //sensor name
                      Store(2, Index(SENS, 3))                          //sensor type 2=temperature sensor

                      Store (\_SB.PCI0.LPC0.EC0.TSRL, Local0)
                      Store (\_SB.PCI0.LPC0.EC0.TSRH, Local1)
                      Or(ShiftLeft(Local1, 8), Local0, Local2)
                      Store (Local2, Index(SENS, 12))

                      CpmDebugPrint ("sensor 1 temperature data  = 0x%X\n", Local2, 0, 0, 0, 0, 0)
                      Store(SizeOf(SENS), Index(SENS, 2))
                      CpmDebugPrint ("SENS length = 0x%X\n", SizeOf(SENS), 0, 0, 0, 0, 0)
                      Return (SENS)
                    }
                    case(3){
                      //sensor index 2
                      Store(2, Index(SENS, 0))                          //sensor id
                      Store("APU External temperature", Index(SENS, 1)) //sensor name
                      Store(2, Index(SENS, 3))                          //sensor type 2=temperature sensor

                      Store (\_SB.PCI0.LPC0.EC0.TS1L, Local0)
                      Store (\_SB.PCI0.LPC0.EC0.TS1H, Local1)
                      Or(ShiftLeft(Local1, 8), Local0, Local2)
                      Store (Local2, Index(SENS, 12))

                      CpmDebugPrint ("sensor 2 temperature data  = 0x%X\n", Local2, 0, 0, 0, 0, 0)
                      Store(SizeOf(SENS), Index(SENS, 2))
                      CpmDebugPrint ("SENS length = 0x%X\n", SizeOf(SENS), 0, 0, 0, 0, 0)
                      Return (SENS)
                    }
                    case(4) {
                      //sensor index 3
                      Store(3, Index(SENS, 0))                          //sensor id
                      Store("IO temperature", Index(SENS, 1))           //sensor name
                      Store(2, Index(SENS, 3))                          //sensor type 2=temperature sensor

                      Store (\_SB.PCI0.LPC0.EC0.TS2L, Local0)
                      Store (\_SB.PCI0.LPC0.EC0.TS2H, Local1)
                      Or(ShiftLeft(Local1, 8), Local0, Local2)
                      Store (Local2, Index(SENS, 12))

                      CpmDebugPrint ("sensor 3 temperature data  = 0x%X\n", Local2, 0, 0, 0, 0, 0)
                      Store(SizeOf(SENS), Index(SENS, 2))
                      CpmDebugPrint ("SENS length = 0x%X\n", SizeOf(SENS), 0, 0, 0, 0, 0)
                      Return (SENS)
                    }
                    case(5) {
                      //sensor index 4
                      Store(4, Index(SENS, 0))                          //sensor id
                      Store("DDR external temperature", Index(SENS, 1)) //sensor name
                      Store(2, Index(SENS, 3))                          //sensor type 2=temperature sensor

                      Store(\_SB.PCI0.LPC0.EC0.GPI8, Local0)            //enable SMBUS0_BUFF_EN
                      Or(Local0, 0x1, Local1)
                      Store(Local1, \_SB.PCI0.LPC0.EC0.GPI8)

                      Store(CpmReadSmbusByte(0x0, 0x4D, 0x29), Local2)
                      Store(CpmReadSmbusByte(0x0, 0x4D, 0x00), Local3)
                      Or(ShiftLeft(Local3, 8), Local2, Local4)
                      Store (Local4, Index(SENS, 12))

                      CpmDebugPrint ("sensor 4 temperature data low byte = 0x%X\n", Local4, 0, 0, 0, 0, 0)
                      Store(Local0, \_SB.PCI0.LPC0.EC0.GPI8)
                      Store(SizeOf(SENS), Index(SENS, 2))
                      CpmDebugPrint ("SENS length = 0x%X\n", SizeOf(SENS), 0, 0, 0, 0, 0)
                      Store(CpmReadSmbusByte(0x0, 0x4D, 0x21), Local5) //Hysteresis
                      Store (Local5, Index(SENS, 22))
                      CpmDebugPrint ("sensor 4 Hysteresis data = 0x%X\n", Local5, 0, 0, 0, 0, 0)
                      return(SENS)
                    }
                    case(6) {
                      //sensor index 5
                      Store(5, Index(SENS, 0))                          //sensor id
                      Store("Chassis Fan information", Index(SENS, 1))  //sensor name
                      Store(5, Index(SENS, 3))                          //sensor type 5=fan speed sensor

                      Store(CpmReadMem8 (0xFED80400, 0x66, 0, 8), Local0)
                      Store(0, Local1)
                      Or(Local0, 0x1, Local1)
                      CpmWriteMem8(0xFED80400, 0x66, 0, 8, Local1) //enable fan dectector
                      //Sleep(150) //delay 150ms for hw preparing the data
                      Store(CpmReadMem8 (0xFED80400, 0x69, 0, 8), Local2) //fanspeed low
                      ShiftRight(Local2, 1, Local2) //bit1-7: fanspeed low byte, bit 0: reserved
                      Store(CpmReadMem8 (0xFED80400, 0x6A, 0, 8), Local3) //fanspeed high
                      Or(ShiftLeft(Local3, 8, Local3), Local2, Local2)
                      Store(Local2, Index(SENS, 36)) //VariableSpeed
                      CpmDebugPrint ("sensor 5 fanspeed = 0x%X\n", Local2, 0, 0, 0, 0, 0)
                      CpmWriteMem8(0xFED80400, 0x66, 0, 8,Local0) //restore the default value
                      Store(SizeOf(SENS), Index(SENS, 2))
                      CpmDebugPrint ("SENS length = 0x%X\n", SizeOf(SENS), 0, 0, 0, 0, 0)
                      Store(2, Index(SENS, 38)) //control mode
                      return (SENS)
                    }
                  }
                }

              default {
                Return(Buffer(One) { 0x00 })
              }
            }
          }
          //Default return
          return (Buffer () {0})
        }
    }

}

