/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "AodSsdt.aml",     // Output file
    "SSDT",                         // Signature
    0x01,                           // SSDT Revision
    "AMD",                          // OEM ID
    "AOD     ",                          // OEM Table ID
    0x1                             // OEM Revision
    )
{
  External (\OBID, PkgObj)
  External (\OBIE, PkgObj)
  External (\OBIT, PkgObj)
  Scope (\) {
    Name (GF01, 0)
    Name (ASMI, 0xA5A5)
    Name (DSMI, 0x9D)
    Name (AODV, 6) // AOD VERSION
    //
    //Global NVS
    //
    OperationRegion(AODE, SystemMemory, 0x55AA55AA, 0x55AA)   //AOD transfer buffer
    Field(AODE, AnyAcc, NoLock, Preserve) {
      OUTB, 1600,   // AOD Run Command Out Put
      SCMI, 32,     // AOD SMI CMD
      SCMD, 32,     // AOD SMI Data
      DSPD, 68128,  // AodMemDimmSpdData: DimmSmbusInfo[AOD_MAX_DIMMS_PER_CHANNEL * AOD_MAX_CHANNELS_PER_SOCKET]
      RESV, 96,
      RMPD, 1120,   // AodRmpPackData:
      WCNS, 4096,   // AOD WMI current NVS
    }

    Device(AOD_) {
      // pnp0c14 is pnp id assigned to WMI mapper
      Name(_HID, EISAID("PNP0C14"))
      Name(_UID, "AOD")
      //////////////////////////////////////////////////////////////////////////////////////////////
      //MethodName:  AM01                                                                         //
      //Description: This method is used to get the version of command list.                      //
      //             If the version number is different, the command list has different content.  //
      //Input:       None                                                                         //
      //Output:      UINT32                                                                       //
      //////////////////////////////////////////////////////////////////////////////////////////////
      Method(AM01,0) {
        return (\AODV)
      }

      //////////////////////////////////////////////////////////////////////////////////////////////
      //MethodName: AM02                                                                          //
      //Description: Init before the Module.                                                      //
      //Input: None                                                                               //
      //Output: Status(No zero may have some error in it)                                         //
      //Note: Use Goble value GF01                                                                //
      //////////////////////////////////////////////////////////////////////////////////////////////
      Method(AM02,0) {
        If (LEqual (GF01, 0)) {
          Store(1,GF01)
        }
        Return (0x0)
      }

      //////////////////////////////////////////////////////////////////////////////////////////////
      //MethodName: AM03                                                                          //
      //Description: Get Object ID & Name	(AP can get ID, Then it can use these ID to Get detail  //
      //Output: Object ID table                                                                   //
      //Note:                                                                                     //
      //////////////////////////////////////////////////////////////////////////////////////////////
      Method(AM03,0) {
        Return(\OBID)
      }

      ////////////////////////////////////////////////////////////////////////////////////////////////
      //MethodName: AM04                                                                            //
      //Description: The method is used for AOD to get the discrete buffer of a buffer,             //
      //             the object 's linear or discrete property is report in method AM02             //
      //Input: object ID                                                                            //
      //Output:                                                                                     //
      //Note:                                                                                       //
      ////////////////////////////////////////////////////////////////////////////////////////////////
      Method(AM04,1) {
        Name(LODT,Buffer(412) {0})
        Name(TEMP,Buffer(512) {0})
        Store (Arg0, Local0)
        Store (DeRefOf(Index (\OBIT, 0)), Local1) //count
        Store (1, Local2)
        while (LLessEqual (Local2, Local1)) {
          Store (DeRefOf(Index (DeRefOf(Index (\OBIT, Local2)), 0)), Local3)
          Store (DeRefOf(Index (DeRefOf(Index (\OBIT, Local2)), 1)), Local4)
          if (LAnd(LEqual (Local3, Local0), LEqual (Local4, 0))) {
            Store (DeRefOf(Index (DeRefOf(Index (\OBIT, Local2)), 3)), LODT)
            Store (WCNS, TEMP)
            Store (DeRefOf(Index (DeRefOf(Index (\OBIT, Local2)), 2)), Local5)
            Multiply (Local5, 4, Local5)
            CreateDWordField (TEMP, Local5, TEM1)
            CreateDWordField (LODT, 4, CRUT)
            Store (TEM1, CRUT)
            Break
          }
          Increment (Local2)
        }
        Return (LODT)
      }

      OperationRegion(PSMI, SystemIO, ASMI, 0x2)   //SMI APM SYSTEM IO
      Field(PSMI, AnyAcc, NoLock, Preserve) {
        ASMO, 8
      }

      ////////////////////////////////////////////////////////////////////////////////////////////////
      //MethodName: AM05                                                                            //
      //Description: Run Command, dispatch task via command id                                      //
      //Input: 	arg0	CMD ID                                                                        //
      //		arg1	byte inputbuffer[256]                                                             //
      //Output: dword cmd status + byte outputbuffer[256]                                           //
      ////////////////////////////////////////////////////////////////////////////////////////////////
      Mutex (SMLO, 0)
      Method(AM05,1) {
        Name (LODT, Buffer(200){0})
       // Store (Acquire (SMLO, 1000), Local0) // save Acquire result so we can check for Mutex acquired
       // If (LEqual (Local0, Zero)) {// check for Mutex acquired
          CreateDwordField(Arg0,0,DCMI)
          CreateDwordField(Arg0,4,DCMD)
          Store (DCMI, SCMI)
          Store (DCMD, SCMD)
          Store (DSMI, ASMO)
          Store (OUTB, LODT)
       //   Release(SMLO)
       //  }
        Return (LODT)
      }

      ////////////////////////////////////////////////////////////////////////////////////////////////
      //MethodName: AM06                                                                            //
      //Description:  Get the linear object information in details(ID,MIN,MAX,STP)                  //
      //Input: Object ID                                                                            //
      //Output: LODT                                                                                //
      //Note:                                                                                       //
      ////////////////////////////////////////////////////////////////////////////////////////////////
      Method(AM06,1)
      {
        Name(LODT,Buffer(20){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0})
        Name(TEMP,Buffer(512) {0})
        Store (Arg0, Local0)
        Store (DeRefOf(Index (\OBIT, 0)), Local1) //count
        Store (1, Local2)
        while (LLessEqual (Local2, Local1)) {
          Store (DeRefOf(Index (DeRefOf(Index (\OBIT, Local2)), 0)), Local3)
          Store (DeRefOf(Index (DeRefOf(Index (\OBIT, Local2)), 1)), Local4)
          if (LAnd(LEqual (Local3, Local0), LEqual (Local4, 1))) {
            Store (DeRefOf(Index (DeRefOf(Index (\OBIT, Local2)), 3)), LODT)
            Store (WCNS, TEMP)
            Store (DeRefOf(Index (DeRefOf(Index (\OBIT, Local2)), 2)), Local5)
            Multiply (Local5, 4, Local5)
            CreateDWordField (TEMP, Local5, TEM1)
            CreateDWordField (LODT, 4, CRUT)
            Store (TEM1, CRUT)
            Break
          }
          Increment (Local2)
        }
        Return (LODT)
      }

      ////////////////////////////////////////////////////////////////////////////////////////////////
      //MethodName: AM07                                                                            //
      //Description:  Get the SPD data for one DIMM                                                 //
      //Input: Dimm index                                                                           //
      //Output: SPD buffer for given dimm index                                                     //
      //Note: The SPD data structure is 532 bytes                                                   //
      ////////////////////////////////////////////////////////////////////////////////////////////////
      Method(AM07,1)
      {
        Name(BSPD,buffer(532) {})  //DIMM SPD buffer

        // Can not exceed the max allowed dimm index (0-15)
        if (LGreater(arg0, 15))
        {
          return(BSPD)
        }

        if(LEqual(arg0, 0))
        {
          store (DSPD, BSPD)
          return (BSPD)
        }

        store(0,Local0)
        Add(Local0, Multiply(arg0, 532), Local0) // Start offset for dimm #index
        store(532, Local1)  // loops for number of bytes
        store(0, local2)    // offset for BSPD buffer to store the spd data

        while(Local1)
        {
            store(derefof(index(DSPD,Local0)),index(BSPD,local2))
            decrement(Local1)
            increment(Local0)
            increment(Local2)
        }

        return(BSPD)
      }

      ////////////////////////////////////////////////////////////////////////////////////////////////
      //MethodName: AM08                                                                            //
      //Description: Get Object ID 2 & Name (AP can get ID, Then it can use these ID to Get detail  //
      //Output: Object ID table 2                                                                   //
      //Note:                                                                                       //
      ////////////////////////////////////////////////////////////////////////////////////////////////
      Method(AM08,0)
      {
          Return(\OBIE)
      }

      ////////////////////////////////////////////////////////////////////////////////////////////////
      //MethodName: AM09                                                                            //
      //Description: Get RMP Data (AP can get ID, Then it can use these ID to Get detail            //
      //Output:                                                                                     //
      //Note:                                                                                       //
      ////////////////////////////////////////////////////////////////////////////////////////////////
      Method(AM09,0)
      {
        Name (TEMP, Buffer(140) {})  //RMP Data buffer
        Store (RMPD, TEMP)
        return (TEMP)
      }
      //The Data Block GUID Mapping control method named _WDG evaluates to a buffer
      //that has the GUID mapping information for data blocks, events, and WMI methods.
      Name(_WDG, Buffer() {

        //
        // Method Execute Guids.
        //
        // {ABBC0F6a-8EA1-11d1-00A0-C90629100000}
        // Method Execute Guid
        0x6a, 0x0f, 0xBC, 0xAB, 0xa1, 0x8e, 0xd1, 0x11, 0x00, 0xa0, 0xc9, 0x06, 0x29, 0x10, 0, 0,
        65, 65,          // Object Id (AA)
        1,               // Instance Count

        // Set this flag if the GUID represents a set of WMI method calls and not a data block
        //#define WMIACPI_REGFLAG_METHOD      0x2
        0x02,           // Flags
                        //
        // This GUID for returning the MOF data
        0x21, 0x12, 0x90, 0x05, 0x66, 0xd5, 0xd1, 0x11, 0xb2, 0xf0, 0x00, 0xa0, 0xc9, 0x06, 0x29, 0x10,
        66, 65,        // Object ID (BA)
        1,             // Instance Count
        0x00,          // Flags
      })//_WDG

      ////////////////////////////////////////////////////////////////////////////////////////////////
      //MethodName:WMAA                                                                             //
      //Description: The Method Used to dispatch all the ASU function call.                         //
      //Input: 	Arg0 The instance being queried, In this Module only one instance been defined      //
      //  Arg1 Method id ,also written in MOF file.                                                 //
      //  Arg2 The input package according to Method id also defined in MOF file.                   //
      //Output:	The Output Package according to Method id also defined in MOF file.                 //
      //Note: Method ID                                                                             //
      //1	Init before the Module.                                                                   //
      //2	Get Object ID & Name	(AP can get ID, Then it can use these ID to Get detail information) //
      //3	Get The Object Value                                                                      //
      //4 	Set the Object Value                                                                    //
      //5	Get the object information in details(ID,MIN,MAX,CUR,STP,PRO)                             //
      //6	Uninit after the AP complete                                                              //
      //7	The Module Request, for the ASL can't get every register value such as MSR.(Called first) //
      //The Method Sequence:                                                                        //
      // 7 1 2 5 (3 4 5) 6                                                                          //
      ////////////////////////////////////////////////////////////////////////////////////////////////
      Method(WMAA,3)
      {
        if(Lequal(Arg0,0))
        {
            if(LOR(LOR(LNOTequal(arg1,1),lnotequal(arg1,2)),Lnotequal(arg1,6)))
            {
                CreateDwordfield(Arg2,0,WIID)
            }

            if (lequal(arg1,1))
            { //Method ID 1
                Return(AM01())
            } elseif (lequal(arg1,2))
            { //Method ID 2
                Return(AM02())
            } elseif (lequal(arg1,3))
            { //Method ID 3
                Return(AM03())
            } elseif (lequal(arg1,4))
            { //Method ID 4
                Return(AM04(WIID))
            } elseif (lequal(arg1,5))
            { //Method ID 5
                Return(AM05(arg2))
            } elseif (lequal(arg1,6))
            { //Method ID 6
                Return(AM06(WIID))
            } elseif (lequal(arg1,7))
            { //Method ID 7
                Return(AM07(WIID))
            } elseif (lequal(arg1,8))
            { //Method ID 8
                Return(AM08())
            } elseif (lequal(arg1, 9)) {
              //Method ID 9
                Return(AM09())
            } else {
                return(Zero)
            }
        } //IF(Lequal(arg0,0)
      } //Method(WMAA,3)
        // AOD MOF file
        #include "AodMof.asi"
    }
  }

}

