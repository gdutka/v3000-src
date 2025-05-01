Device(BTH0) {
  Name(_HID, "QCOM6390")
//   Name(_PRW, Package()
//   {
//       Zero,
//       Zero
//   })
  Name(_S4W, 0x2)
  Name(_S0W, 0x2)

  Method(_STA, 0x0, NotSerialized)
  {
    If(LEqual(BLTH, 0))
    {
      Return (0)
    }
    Else
    {
      Return (0xF)
    }
  }

  Method(_CRS, 0x0, NotSerialized)
  {
      Name(UBUF, ResourceTemplate()
      {
        UARTSerialBus(115200, DataBitsEight, StopBitsOne, 0xc0, LittleEndian, ParityTypeNone, FlowControlHardware, 0x20, 0x20, "\\_SB.FUR0", 0, ResourceConsumer, , )
        GpioInt(Edge, ActiveLow, ExclusiveAndWake, PullUp, 0, "\\_SB.GPIO", 0, ResourceConsumer, ,) {4}
      })
      Return(UBUF)
  }
} // Device BTH0
