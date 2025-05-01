//ACDC Timer asl
DefinitionBlock (
    "AcDcTimer.aml",         // Output file
    "SSDT",                  // Signature
    0x01,                    // SSDT Revision
    "AMD",                   // OEM ID
    "ACDC",                  // OEM Table ID
    0x1                      // OEM Revision
    )
{
  Scope(\_SB)
  {
    Device(ACDC){
      Name(_HID, "ACPI000E") //device ID

      Name(_CRS, ResourceTemplate() {
        GpioInt(Edge, ActiveLow, SharedAndWake, PullUp, 3000, "\\_SB.GPIO", ,) {52}
      })

      OperationRegion(CMOP, SystemMemory, 0xFED81D00, 0x100)
      Field(CMOP, DWordAcc, NoLock, Preserve) {
        ATVE,  32, //AcTimerValue
        AETP,  32, //AcExpiredTimerPolicy
        ATED,  1,  //AcTimerExpired.
        ATWP,  1,  //AcTimerWakeup.
        Offset (0x10),
        DTVE,  32, //DcTimerValue
        DETP,  32, //DcExpiredTimerPolicy
        DTED,  1,  //DcTimerExpired.
        DTWP,  1,  //DcTimerWakeup.
        Offset (0x20),
        BUSY,  1,  //Busy
            ,  7,  //
        ATEE,  1,  //AcTimerEventEn.
        DTEE,  1,  //DcTimerEventEn.
      }

      Method(_INI) {
        Store(one, ATEE)  //Enable AC Timer to wake up system 
        Store(one, DTEE)  //Enable DC Timer to wake up system
      }

      Method(AINT) {
        Store(one, ATEE)
      }

      Method(DINT) {
        Store(one, DTEE)
      }

      Method(_GCP) {
        //AC wake implemented;
        //DC wake implemented;
        //Get/Set real time features implemented
        //Wake supported from S4 on AC
        //Wake supported from S4 on DC
        Return (0xA7)
      }

      Method(_STP, 2){
        If(LEqual(Arg0, 0)){
          Store(Arg1, AETP) //Set AC timer policy
        }Else{
          Store(Arg1, DETP) //Set DC timer policy
        }
        Return(0)
      }

      Method(_TIP, 1){
        If(LEqual(Arg0, 0)){
          Store(AETP, Local0) //Get AC timer policy
        }Else{
          Store(DETP, Local0) //Get DC timer policy
        }
        Return (Local0)
      }

      Method(_STV, 2){
        If(LEqual(Arg0, 0)){
          AINT()
          Store(Arg1, ATVE) //Set AC timer value
        }Else{
          DINT()
          Store(Arg1, DTVE) //Set DC timer value
        }
        Return(0)
      }

      Method(_TIV, 1){
        If(LEqual(Arg0, 0)){
          Store(ATVE, Local0) //Get AC timer value
        }Else{
          Store(DTVE, Local0) //Get DC timer value
        }
        Return (Local0)
      }

      Method(_GWS, 1){
        If(LEqual(Arg0, 0)){
          ShiftLeft (ATWP, 1, Local0) //Get AC timer wake status
          Or(Local0, ATED, Local0)    //Get AC timer expired status
        }Else{
          ShiftLeft (DTWP, 1, Local0) //Get DC timer wake status
          Or(Local0, DTED, Local0)    //Get DC timer expired status
        }
        Return (Local0)
      }

      Method(_CWS, 1){
        If(LEqual(Arg0, 0)){
          Store(1, ATWP) //Clear AC Wake status
        }Else{
          Store(1, DTWP) //Clear DC Wake status
        }
        Return(0)
      }

      OperationRegion(PM00, SystemMemory, 0xFED80300, 0x100)
      Field(PM00, AnyAcc, NoLock, Preserve)
      {
        Offset (0x56),
        RCTL, 16,    // RTC Control
      }

      OperationRegion(RRTC, SystemIo, 0x72, 0x02)
      Field(RRTC, ByteAcc, NoLock, Preserve)
      {
        RTCI, 8,
        RTCD, 8,
      }

      OperationRegion(CMOM, SystemMemory, 0xFED80700, 0x100)
      Field(CMOM, ByteAcc, NoLock, Preserve) {
        SECR, 8,  // RTC Seconds
        SECA, 8,  // RTC Seconds Alarm
        MINR, 8,  // RTC Minutes
        MINA, 8,  // RTC Minutes Alarm
        HONR, 8,  // RTC Hours
        HONA, 8,  // RTC Hours Alarm
        DYWR, 8,  // RTC Day of Week
        DYMR, 8,  // RTC Date of Month
        MONR, 8,  // RTC Month
        YARR, 8,  // RTC Year
        RTRA, 8,  // RTC Register A
        RTRB, 8,  // RTC Register B
        RTRC, 8,  // RTC Register C
        RTDA, 8,  // RTC Date Alarm
        Offset (0x32),
        RTAC, 8,  // RTC AltCentury
        Offset (0x48),
        RTCC, 8,  // RTC Century
        Offset (0x50),
        ERAP, 8,  // RTC Extended RAM Address Port
        Offset (0x53),
        ERDP, 8,  // RTC Extended RAM Data Port
        Offset (0x7E),
        RTTC, 8,  // RTC Timer Clear
        RTRE, 8,  // RTC RAM Enable
      }

      Name(TZLI, 0xFF)    // Flag to indicate Timezone/Daylight stored inside CMOS RAM
      Name(TZVL, 0xFF)    // Offset in CMOS RAM to store lower byte of Timezone
      Name(TZVH, 0xFF)    // Offset in CMOS RAM to store lower byte of Timezone
      Name(LIVR, 0xFF)    // Offset in CMOS RAM to store Datlight
      Name(RBUF, Buffer(16) {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
      Mutex(RTCL, 0)

      Method (_GRT, 0x0, Serialized)
      {
        CreateWordField(RBUF, 0x0, YEAW)    // Year 1900 - 9999
        CreateByteField(RBUF, 0x2, MONB)    // Month 1 - 12
        CreateByteField(RBUF, 0x3, DAYB)    // Day 1 - 31
        CreateByteField(RBUF, 0x4, HOUB)    // Hour 0 - 23
        CreateByteField(RBUF, 0x5, MINB)    // Minute 0 - 59
        CreateByteField(RBUF, 0x6, SECB)    // Second 0 - 59
        CreateByteField(RBUF, 0x7, VALB)    // Valid 0 - Time is not valid (request failed); 1 - Time is valid
        CreateWordField(RBUF, 0x8, MILW)    // milliseconds 1 - 1000
        CreateWordField(RBUF, 0xA, TIMW)    // TimeZone -1440 to 1440 or 2047 (unspecified)
        CreateByteField(RBUF, 0xC, LIGB)    // Daylight

        Acquire(RTCL, 0xFFFF)

        FromBCD (SECR, SECB)    // Get Second
        FromBCD (MINR, MINB)    // Get Minute
        FromBCD (HONR, HOUB)    // Get Hour
        FromBCD (DYMR, DAYB)    // Get Day
        FromBCD (MONR, MONB)    // Get Month
        FromBCD (YARR, YEAW)    // Get Year

        //Get Century
        If(LAnd(LNotEqual(And(RTRA, 0x10), 0x10), LEqual(And(RCTL, 0x1000), 0x1000))){
          FromBCD (RTAC, Local0)
        }ElseIf(LEqual(And(RTRA, 0x10), 0x10)){
          FromBCD (RTCC, Local0)
        }Else{
          FromBCD (0x20, Local0)    //return 0x20 by default
        }

        //Calculate Year
        Multiply (Local0, 100, Local0);
        Add (Local0, YEAW, YEAW)

        If(LNotEqual(TZLI, 0xFF)){
          Store(TZVL, RTCI)
          Store(RTCD, Local1)
          Store(TZVH, RTCI)
          ShiftLeft(RTCD, 8, Local2)
          Or(Local1, Local2, Local2)

          Store(LIVR, RTCI)
          Store(RTCD, Local3)

          Store (Local2, TIMW)
          Store (Local3, LIGB)
        }else{
          Store (2047, TIMW)    // Unspecified Time Zone
          Store (0x00, LIGB)    // Not affected by daylight and has not been adjusted for daylight savings
        }

        Store (0x01, MILW)    // return millisecond as 1
        Store (0x01, VALB)    // Data valid

        Release (RTCL)

        return (RBUF)
      }

      Method (_SRT, 0x1, Serialized)
      {
        CreateWordField(Arg0, 0x0, YEAW)    // Year 1900 - 9999
        CreateByteField(Arg0, 0x2, MONB)    // Month 1 - 12
        CreateByteField(Arg0, 0x3, DAYB)    // Day 1 - 31
        CreateByteField(Arg0, 0x4, HOUB)    // Hour 0 - 23
        CreateByteField(Arg0, 0x5, MINB)    // Minute 0 - 59
        CreateByteField(Arg0, 0x6, SECB)    // Second 0 - 59
        CreateWordField(Arg0, 0x8, MILW)    // milliseconds 1 - 1000
        CreateWordField(Arg0, 0xA, TIMW)    // Timezone
        CreateByteField(Arg0, 0xC, LIGB)    // Daylight

        Acquire(RTCL, 0xFFFF)

        Store (RTRB, Local2)
        Or (RTRB, 0x80, RTRB)

        ToBCD (SECB, SECR)   // Set second
        ToBCD (MINB, MINR)   // Set minute
        ToBCD (HOUB, HONR)   // Set hour
        ToBCD (DAYB, DYMR)   // Set day
        ToBCD (MONB, MONR)   // Set month

        Divide (YEAW, 100, Local0, Local1)
        ToBCD (Local0, YARR)    // Set Year

        //Set Century
        If(LAnd(LNotEqual(And(RTRA, 0x10), 0x10), LEqual(And(RCTL, 0x1000), 0x1000))){
          ToBCD (Local1, RTAC)
        }ElseIf(LEqual(And(RTRA, 0x10), 0x10)){
          ToBCD (Local1, RTCC)
        }

        Store (Local2, RTRB)

        If(LNotEqual(TZLI, 0xFF)){
          ShiftRight(TIMW, 8, Local3)
          Store(TZVH, RTCI)
          Store(Local3, RTCD)
          And(TIMW, 0xFFFF, Local3)
          Store(TZVL, RTCI)
          Store(Local3, RTCD)

          Store(LIVR, RTCI)
          Store(LIGB, RTCD)
        }

        Release (RTCL)

        return (0x0)
      }
    } // end of ACPI Wake Alarm device object
  }
}
