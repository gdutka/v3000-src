/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _ACP_AZALIAIP_H_
#define _ACP_AZALIAIP_H_


/***********************************************************
* Register Name : AUDIO_AZ_INPUT_SD0CTL_AND_STS
* Register Description :
* Input Stream Descriptor 0 Control and Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : Stream Reset (SRST):
// Writing a 1 causes the corresponding stream to be reset. The Stream Descriptor registers (except the SRST bit itself), FIFO's, and cadence generator for the corresponding stream are reset. After the stream hardware has completed sequencing into the reset state, it will report a 1 in this bit. Software must read a 1 from this bit to verify that the stream is in reset. Writing a 0 causes the corresponding stream to exit reset. When the stream hardware is ready to begin operation, it will report a 0 in this bit. Software must read a 0 from this bit before accessing any of the stream registers. The RUN bit must be cleared before SRST is asserted.
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_SRST_OFFSET      0
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_SRST_MASK        0x1

// Bitfield Description : Stream Run (RUN):
// When set to 1, the DMA engine associated with this input stream will be enabled to transfer data in the FIFO to main memory. When cleared to 0, the DMA engine associated with this input stream will be disabled. If the corresponding SSYNC bit is 0, input stream data will be taken from the link and moved to the FIFO and an over-run may occur
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_RUN_OFFSET      1
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_RUN_MASK        0x2

// Bitfield Description : Interrupt On Completion Enable (IOCE):
// This bit controls whether or not an interrupt occurs when a buffer completes with the IOC bit set in its descriptor. If this bit is not set, bit 3 in the Status register will be set, but the interrupt will not occur.
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_IOCE_OFFSET      2
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_IOCE_MASK        0x4

// Bitfield Description : FIFO Error Interrupt Enable (FEIE):
// This bit controls whether the occurrence of a FIFO error (overrun for input or under run for output) will cause an interrupt or not. If this bit is not set, bit 4 in the Status register will be set, but the interrupt will not occur. Either way, the samples will be dropped.
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_FEIE_OFFSET      3
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_FEIE_MASK        0x8

// Bitfield Description : Descriptor Error Interrupt Enable (DEIE):
// Controls whether an interrupt is generated when the Descriptor Error Status (DESE) bit is set.
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_DEIE_OFFSET      4
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_DEIE_MASK        0x10

// Bitfield Description : 
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_Reserved_15_5_OFFSET      5
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_Reserved_15_5_MASK        0xffe0

// Bitfield Description : Stripe Control (STRIPE):
// (Output and Bidirectional engines configured for output only. Read Only 0 for input streams.) If the NSDO field of the Global Capabilities register indicates that the controller supports multiple SDO lines and the codec has been determined to have compatible capabilities, STRIPE can be used to indicate how many of the SDO lines the stream should be striped across.
// 00: 1 SDO
// 01: 2 SDOs
// 10: 4 SDOs
// 11: Reserved
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_STRIPE_OFFSET      16
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_STRIPE_MASK        0x30000

// Bitfield Description : Traffic Priority (TP):
// If set to a 1, the stream will be treated as preferred traffic if the underlying bus supports it. If set to a 0, the traffic will be handled on a "best effort" basis. The actual meaning of this bit is specific to the hardware implementation. Depending on the hardware implementation, there may be additional restrictions on the traffic, and software should assume that the buffers associated with this stream will not be snooped or cached.
// On PCI Express*, for example, setting the TP bit to a 1 might cause the controller to generate non-snooped isochronous traffic, while a PCI implementation may ignore this bit.
// It is permitted to implement this bit as RO if there is no controllability available.
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_TP_OFFSET      18
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_TP_MASK        0x40000

// Bitfield Description : Bidirectional Direction Control (DIR): (Bidirectional engines only. Read-only 0 for engines which are not bidirectional.) For a bidirectional engine, this bit determines the direction in which the bidirectional engine should operate. This bit can only be changed after stream reset (SRST) has been asserted and cleared and before any other stream registers have been programmed. Because setting this bit changes the fundamental behavior of the stream and the meaning of some bits, changing this bit after any other register in the stream descriptor has been written to may lead to undetermined results.
// 0 = Bidirectional engine is configured as an Input Engine.
// 1 = Bidirectional engine is configured as an Output Engine.
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_DIR_OFFSET      19
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_DIR_MASK        0x80000

// Bitfield Description : Stream Number (STRM):
// This value reflects the Tag associated with the data being transferred on the link.
// When data controlled by this descriptor is sent out over the link, it will have this stream number encoded on the SYNC signal.
// When an input stream is detected on any of the SDATA_INx signals that match this value, the data samples are loaded into the FIFO associated with this descriptor. Note that while a single SDAT_INx input may contain data from more than one stream number, two different SDATA_INx inputs may not be configured with the same stream number.
// Although the controller hardware is capable of transmitting any stream number, by convention stream 0 is reserved as unused by software, so that converters whose stream numbers have been reset to 0 do not unintentionally decode data not intended for them.
// 0000 = Reserved (Indicates Unused)
// 0001 = Stream 1
// ...
// 1110 = Stream 14
// 1111 = Stream 15
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_STRM_OFFSET      20
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_STRM_MASK        0xf00000

// Bitfield Description : 
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_Reserved_25_24_OFFSET      24
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_Reserved_25_24_MASK        0x3000000

// Bitfield Description : Buffer Completion Interrupt Status (BCIS):
// For an outbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor has been fetched from memory and put into the DMA FIFO, and the current descriptor has the IOC bit set.
// For an inbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor with an IOC bit set has been removed from the DMA FIFO and the current descriptor has the IOC bit set.
// BCIS remains active until software clears it by writing a 1 to this bit position.
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_BCIS_OFFSET      26
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_BCIS_MASK        0x4000000

// Bitfield Description : FIFO Error (FIFOE):
// Set when a FIFO error occurs. Bit is cleared by writing a 1 to this bit position. This bit is set even if an interrupt is not enabled.
// For an input stream, this indicates a FIFO overrun occurring while the RUN bit is set. When this happens, the FIFO pointers do not increment and the incoming data is not written into the FIFO, thereby being lost.
// For an output stream, this indicates a FIFO under run when there are still buffers to send. The hardware should not transmit anything on the link for the associated stream if there is not valid data to send.
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_FIFOE_OFFSET      27
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_FIFOE_MASK        0x8000000

// Bitfield Description : Descriptor Error (DESE):
// During the fetch of a descriptor, an error has occurred. This could be a result of a Master Abort, a Parity or ECC error on the bus, or any other error that renders the current Buffer Descriptor or Buffer Descriptor List useless. This error is treated as a fatal stream error as the stream cannot continue running. The RUN bit will be cleared and the stream will stop. Software may attempt to restart the stream engine after addressing the cause of the error and writing a 1 to this bit to clear it.
// Note : It is not implemented and is tied to ZERO in hardware.
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_DESE_OFFSET      28
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_DESE_MASK        0x10000000

// Bitfield Description : FIFO Ready (FIFORDY):
// For an Output stream, the controller hardware will set this bit to a 1 while the output DMA FIFO contains enough data to maintain the stream on the link. This bit defaults to 0 on reset because the FIFO is cleared on a reset. The amount of data required to maintain the stream will depend on the controller implementation but, in general, for an output stream, it means that the FIFO is full.
// For an input stream, this bit indicates that a descriptor has been fetched, and the engine is ready for the RUN bit to be set.
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_FIFORDY_OFFSET      29
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_FIFORDY_MASK        0x20000000

// Bitfield Description : 
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_Reserved_31_30_OFFSET      30
#define AUDIO_AZ_INPUT_SD0CTL_AND_STS_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            SRST:1;
    UINT32                            RUN:1;
    UINT32                            IOCE:1;
    UINT32                            FEIE:1;
    UINT32                            DEIE:1;
    UINT32                            Reserved_15_5:11;
    UINT32                            STRIPE:2;
    UINT32                            TP:1;
    UINT32                            DIR:1;
    UINT32                            STRM:4;
    UINT32                            Reserved_25_24:2;
    UINT32                            BCIS:1;
    UINT32                            FIFOE:1;
    UINT32                            DESE:1;
    UINT32                            FIFORDY:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} AUDIO_AZ_INPUT_SD0CTL_AND_STS_STRUCT;

#define SMN_AUDIO_AZ_INPUT_SD0CTL_AND_STS_ADDRESS    0x1200080UL


/***********************************************************
* Register Name : AUDIO_AZ_INPUT_SD1CTL_AND_STS
* Register Description :
* Input Stream Descriptor 1 Control and Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : Stream Reset (SRST):
// Writing a 1 causes the corresponding stream to be reset. The Stream Descriptor registers (except the SRST bit itself), FIFO's, and cadence generator for the corresponding stream are reset. After the stream hardware has completed sequencing into the reset state, it will report a 1 in this bit. Software must read a 1 from this bit to verify that the stream is in reset. Writing a 0 causes the corresponding stream to exit reset. When the stream hardware is ready to begin operation, it will report a 0 in this bit. Software must read a 0 from this bit before accessing any of the stream registers. The RUN bit must be cleared before SRST is asserted.
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_SRST_OFFSET      0
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_SRST_MASK        0x1

// Bitfield Description : Stream Run (RUN):
// When set to 1, the DMA engine associated with this input stream will be enabled to transfer data in the FIFO to main memory. When cleared to 0, the DMA engine associated with this input stream will be disabled. If the corresponding SSYNC bit is 0, input stream data will be taken from the link and moved to the FIFO and an over-run may occur
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_RUN_OFFSET      1
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_RUN_MASK        0x2

// Bitfield Description : Interrupt On Completion Enable (IOCE):
// This bit controls whether or not an interrupt occurs when a buffer completes with the IOC bit set in its descriptor. If this bit is not set, bit 3 in the Status register will be set, but the interrupt will not occur.
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_IOCE_OFFSET      2
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_IOCE_MASK        0x4

// Bitfield Description : FIFO Error Interrupt Enable (FEIE):
// This bit controls whether the occurrence of a FIFO error (overrun for input or under run for output) will cause an interrupt or not. If this bit is not set, bit 4 in the Status register will be set, but the interrupt will not occur. Either way, the samples will be dropped.
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_FEIE_OFFSET      3
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_FEIE_MASK        0x8

// Bitfield Description : Descriptor Error Interrupt Enable (DEIE):
// Controls whether an interrupt is generated when the Descriptor Error Status (DESE) bit is set.
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_DEIE_OFFSET      4
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_DEIE_MASK        0x10

// Bitfield Description : 
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_Reserved_15_5_OFFSET      5
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_Reserved_15_5_MASK        0xffe0

// Bitfield Description : Stripe Control (STRIPE):
// (Output and Bidirectional engines configured for output only. Read Only 0 for input streams.) If the NSDO field of the Global Capabilities register indicates that the controller supports multiple SDO lines and the codec has been determined to have compatible capabilities, STRIPE can be used to indicate how many of the SDO lines the stream should be striped across.
// 00: 1 SDO
// 01: 2 SDOs
// 10: 4 SDOs
// 11: Reserved
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_STRIPE_OFFSET      16
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_STRIPE_MASK        0x30000

// Bitfield Description : Traffic Priority (TP):
// If set to a 1, the stream will be treated as preferred traffic if the underlying bus supports it. If set to a 0, the traffic will be handled on a "best effort" basis. The actual meaning of this bit is specific to the hardware implementation. Depending on the hardware implementation, there may be additional restrictions on the traffic, and software should assume that the buffers associated with this stream will not be snooped or cached.
// On PCI Express*, for example, setting the TP bit to a 1 might cause the controller to generate non-snooped isochronous traffic, while a PCI implementation may ignore this bit.
// It is permitted to implement this bit as RO if there is no controllability available.
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_TP_OFFSET      18
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_TP_MASK        0x40000

// Bitfield Description : Bidirectional Direction Control (DIR): (Bidirectional engines only. Read-only 0 for engines which are not bidirectional.) For a bidirectional engine, this bit determines the direction in which the bidirectional engine should operate. This bit can only be changed after stream reset (SRST) has been asserted and cleared and before any other stream registers have been programmed. Because setting this bit changes the fundamental behavior of the stream and the meaning of some bits, changing this bit after any other register in the stream descriptor has been written to may lead to undetermined results.
// 0 = Bidirectional engine is configured as an Input Engine.
// 1 = Bidirectional engine is configured as an Output Engine.
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_DIR_OFFSET      19
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_DIR_MASK        0x80000

// Bitfield Description : Stream Number (STRM):
// This value reflects the Tag associated with the data being transferred on the link.
// When data controlled by this descriptor is sent out over the link, it will have this stream number encoded on the SYNC signal.
// When an input stream is detected on any of the SDATA_INx signals that match this value, the data samples are loaded into the FIFO associated with this descriptor. Note that while a single SDAT_INx input may contain data from more than one stream number, two different SDATA_INx inputs may not be configured with the same stream number.
// Although the controller hardware is capable of transmitting any stream number, by convention stream 0 is reserved as unused by software, so that converters whose stream numbers have been reset to 0 do not unintentionally decode data not intended for them.
// 0000 = Reserved (Indicates Unused)
// 0001 = Stream 1
// ...
// 1110 = Stream 14
// 1111 = Stream 15
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_STRM_OFFSET      20
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_STRM_MASK        0xf00000

// Bitfield Description : 
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_Reserved_25_24_OFFSET      24
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_Reserved_25_24_MASK        0x3000000

// Bitfield Description : Buffer Completion Interrupt Status (BCIS):
// For an outbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor has been fetched from memory and put into the DMA FIFO, and the current descriptor has the IOC bit set.
// For an inbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor with an IOC bit set has been removed from the DMA FIFO and the current descriptor has the IOC bit set.
// BCIS remains active until software clears it by writing a 1 to this bit position.
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_BCIS_OFFSET      26
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_BCIS_MASK        0x4000000

// Bitfield Description : FIFO Error (FIFOE):
// Set when a FIFO error occurs. Bit is cleared by writing a 1 to this bit position. This bit is set even if an interrupt is not enabled.
// For an input stream, this indicates a FIFO overrun occurring while the RUN bit is set. When this happens, the FIFO pointers do not increment and the incoming data is not written into the FIFO, thereby being lost.
// For an output stream, this indicates a FIFO under run when there are still buffers to send. The hardware should not transmit anything on the link for the associated stream if there is not valid data to send.
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_FIFOE_OFFSET      27
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_FIFOE_MASK        0x8000000

// Bitfield Description : Descriptor Error (DESE):
// During the fetch of a descriptor, an error has occurred. This could be a result of a Master Abort, a Parity or ECC error on the bus, or any other error that renders the current Buffer Descriptor or Buffer Descriptor List useless. This error is treated as a fatal stream error as the stream cannot continue running. The RUN bit will be cleared and the stream will stop. Software may attempt to restart the stream engine after addressing the cause of the error and writing a 1 to this bit to clear it.
// Note : It is not implemented and is tied to ZERO in hardware.
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_DESE_OFFSET      28
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_DESE_MASK        0x10000000

// Bitfield Description : FIFO Ready (FIFORDY):
// For an Output stream, the controller hardware will set this bit to a 1 while the output DMA FIFO contains enough data to maintain the stream on the link. This bit defaults to 0 on reset because the FIFO is cleared on a reset. The amount of data required to maintain the stream will depend on the controller implementation but, in general, for an output stream, it means that the FIFO is full.
// For an input stream, this bit indicates that a descriptor has been fetched, and the engine is ready for the RUN bit to be set.
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_FIFORDY_OFFSET      29
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_FIFORDY_MASK        0x20000000

// Bitfield Description : 
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_Reserved_31_30_OFFSET      30
#define AUDIO_AZ_INPUT_SD1CTL_AND_STS_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            SRST:1;
    UINT32                            RUN:1;
    UINT32                            IOCE:1;
    UINT32                            FEIE:1;
    UINT32                            DEIE:1;
    UINT32                            Reserved_15_5:11;
    UINT32                            STRIPE:2;
    UINT32                            TP:1;
    UINT32                            DIR:1;
    UINT32                            STRM:4;
    UINT32                            Reserved_25_24:2;
    UINT32                            BCIS:1;
    UINT32                            FIFOE:1;
    UINT32                            DESE:1;
    UINT32                            FIFORDY:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} AUDIO_AZ_INPUT_SD1CTL_AND_STS_STRUCT;

#define SMN_AUDIO_AZ_INPUT_SD1CTL_AND_STS_ADDRESS    0x12000a0UL


/***********************************************************
* Register Name : AUDIO_AZ_INPUT_SD2CTL_AND_STS
* Register Description :
* Input Stream Descriptor 2 Control and Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : Stream Reset (SRST):
// Writing a 1 causes the corresponding stream to be reset. The Stream Descriptor registers (except the SRST bit itself), FIFO's, and cadence generator for the corresponding stream are reset. After the stream hardware has completed sequencing into the reset state, it will report a 1 in this bit. Software must read a 1 from this bit to verify that the stream is in reset. Writing a 0 causes the corresponding stream to exit reset. When the stream hardware is ready to begin operation, it will report a 0 in this bit. Software must read a 0 from this bit before accessing any of the stream registers. The RUN bit must be cleared before SRST is asserted.
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_SRST_OFFSET      0
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_SRST_MASK        0x1

// Bitfield Description : Stream Run (RUN):
// When set to 1, the DMA engine associated with this input stream will be enabled to transfer data in the FIFO to main memory. When cleared to 0, the DMA engine associated with this input stream will be disabled. If the corresponding SSYNC bit is 0, input stream data will be taken from the link and moved to the FIFO and an over-run may occur
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_RUN_OFFSET      1
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_RUN_MASK        0x2

// Bitfield Description : Interrupt On Completion Enable (IOCE):
// This bit controls whether or not an interrupt occurs when a buffer completes with the IOC bit set in its descriptor. If this bit is not set, bit 3 in the Status register will be set, but the interrupt will not occur.
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_IOCE_OFFSET      2
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_IOCE_MASK        0x4

// Bitfield Description : FIFO Error Interrupt Enable (FEIE):
// This bit controls whether the occurrence of a FIFO error (overrun for input or under run for output) will cause an interrupt or not. If this bit is not set, bit 4 in the Status register will be set, but the interrupt will not occur. Either way, the samples will be dropped.
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_FEIE_OFFSET      3
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_FEIE_MASK        0x8

// Bitfield Description : Descriptor Error Interrupt Enable (DEIE):
// Controls whether an interrupt is generated when the Descriptor Error Status (DESE) bit is set.
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_DEIE_OFFSET      4
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_DEIE_MASK        0x10

// Bitfield Description : 
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_Reserved_15_5_OFFSET      5
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_Reserved_15_5_MASK        0xffe0

// Bitfield Description : Stripe Control (STRIPE):
// (Output and Bidirectional engines configured for output only. Read Only 0 for input streams.) If the NSDO field of the Global Capabilities register indicates that the controller supports multiple SDO lines and the codec has been determined to have compatible capabilities, STRIPE can be used to indicate how many of the SDO lines the stream should be striped across.
// 00: 1 SDO
// 01: 2 SDOs
// 10: 4 SDOs
// 11: Reserved
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_STRIPE_OFFSET      16
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_STRIPE_MASK        0x30000

// Bitfield Description : Traffic Priority (TP):
// If set to a 1, the stream will be treated as preferred traffic if the underlying bus supports it. If set to a 0, the traffic will be handled on a "best effort" basis. The actual meaning of this bit is specific to the hardware implementation. Depending on the hardware implementation, there may be additional restrictions on the traffic, and software should assume that the buffers associated with this stream will not be snooped or cached.
// On PCI Express*, for example, setting the TP bit to a 1 might cause the controller to generate non-snooped isochronous traffic, while a PCI implementation may ignore this bit.
// It is permitted to implement this bit as RO if there is no controllability available.
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_TP_OFFSET      18
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_TP_MASK        0x40000

// Bitfield Description : Bidirectional Direction Control (DIR): (Bidirectional engines only. Read-only 0 for engines which are not bidirectional.) For a bidirectional engine, this bit determines the direction in which the bidirectional engine should operate. This bit can only be changed after stream reset (SRST) has been asserted and cleared and before any other stream registers have been programmed. Because setting this bit changes the fundamental behavior of the stream and the meaning of some bits, changing this bit after any other register in the stream descriptor has been written to may lead to undetermined results.
// 0 = Bidirectional engine is configured as an Input Engine.
// 1 = Bidirectional engine is configured as an Output Engine.
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_DIR_OFFSET      19
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_DIR_MASK        0x80000

// Bitfield Description : Stream Number (STRM):
// This value reflects the Tag associated with the data being transferred on the link.
// When data controlled by this descriptor is sent out over the link, it will have this stream number encoded on the SYNC signal.
// When an input stream is detected on any of the SDATA_INx signals that match this value, the data samples are loaded into the FIFO associated with this descriptor. Note that while a single SDAT_INx input may contain data from more than one stream number, two different SDATA_INx inputs may not be configured with the same stream number.
// Although the controller hardware is capable of transmitting any stream number, by convention stream 0 is reserved as unused by software, so that converters whose stream numbers have been reset to 0 do not unintentionally decode data not intended for them.
// 0000 = Reserved (Indicates Unused)
// 0001 = Stream 1
// ...
// 1110 = Stream 14
// 1111 = Stream 15
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_STRM_OFFSET      20
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_STRM_MASK        0xf00000

// Bitfield Description : 
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_Reserved_25_24_OFFSET      24
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_Reserved_25_24_MASK        0x3000000

// Bitfield Description : Buffer Completion Interrupt Status (BCIS):
// For an outbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor has been fetched from memory and put into the DMA FIFO, and the current descriptor has the IOC bit set.
// For an inbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor with an IOC bit set has been removed from the DMA FIFO and the current descriptor has the IOC bit set.
// BCIS remains active until software clears it by writing a 1 to this bit position.
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_BCIS_OFFSET      26
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_BCIS_MASK        0x4000000

// Bitfield Description : FIFO Error (FIFOE):
// Set when a FIFO error occurs. Bit is cleared by writing a 1 to this bit position. This bit is set even if an interrupt is not enabled.
// For an input stream, this indicates a FIFO overrun occurring while the RUN bit is set. When this happens, the FIFO pointers do not increment and the incoming data is not written into the FIFO, thereby being lost.
// For an output stream, this indicates a FIFO under run when there are still buffers to send. The hardware should not transmit anything on the link for the associated stream if there is not valid data to send.
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_FIFOE_OFFSET      27
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_FIFOE_MASK        0x8000000

// Bitfield Description : Descriptor Error (DESE):
// During the fetch of a descriptor, an error has occurred. This could be a result of a Master Abort, a Parity or ECC error on the bus, or any other error that renders the current Buffer Descriptor or Buffer Descriptor List useless. This error is treated as a fatal stream error as the stream cannot continue running. The RUN bit will be cleared and the stream will stop. Software may attempt to restart the stream engine after addressing the cause of the error and writing a 1 to this bit to clear it.
// Note : It is not implemented and is tied to ZERO in hardware.
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_DESE_OFFSET      28
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_DESE_MASK        0x10000000

// Bitfield Description : FIFO Ready (FIFORDY):
// For an Output stream, the controller hardware will set this bit to a 1 while the output DMA FIFO contains enough data to maintain the stream on the link. This bit defaults to 0 on reset because the FIFO is cleared on a reset. The amount of data required to maintain the stream will depend on the controller implementation but, in general, for an output stream, it means that the FIFO is full.
// For an input stream, this bit indicates that a descriptor has been fetched, and the engine is ready for the RUN bit to be set.
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_FIFORDY_OFFSET      29
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_FIFORDY_MASK        0x20000000

// Bitfield Description : 
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_Reserved_31_30_OFFSET      30
#define AUDIO_AZ_INPUT_SD2CTL_AND_STS_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            SRST:1;
    UINT32                            RUN:1;
    UINT32                            IOCE:1;
    UINT32                            FEIE:1;
    UINT32                            DEIE:1;
    UINT32                            Reserved_15_5:11;
    UINT32                            STRIPE:2;
    UINT32                            TP:1;
    UINT32                            DIR:1;
    UINT32                            STRM:4;
    UINT32                            Reserved_25_24:2;
    UINT32                            BCIS:1;
    UINT32                            FIFOE:1;
    UINT32                            DESE:1;
    UINT32                            FIFORDY:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} AUDIO_AZ_INPUT_SD2CTL_AND_STS_STRUCT;

#define SMN_AUDIO_AZ_INPUT_SD2CTL_AND_STS_ADDRESS    0x12000c0UL


/***********************************************************
* Register Name : AUDIO_AZ_INPUT_SD3CTL_AND_STS
* Register Description :
* Input Stream Descriptor 3 Control and Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : Stream Reset (SRST):
// Writing a 1 causes the corresponding stream to be reset. The Stream Descriptor registers (except the SRST bit itself), FIFO's, and cadence generator for the corresponding stream are reset. After the stream hardware has completed sequencing into the reset state, it will report a 1 in this bit. Software must read a 1 from this bit to verify that the stream is in reset. Writing a 0 causes the corresponding stream to exit reset. When the stream hardware is ready to begin operation, it will report a 0 in this bit. Software must read a 0 from this bit before accessing any of the stream registers. The RUN bit must be cleared before SRST is asserted.
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_SRST_OFFSET      0
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_SRST_MASK        0x1

// Bitfield Description : Stream Run (RUN):
// When set to 1, the DMA engine associated with this input stream will be enabled to transfer data in the FIFO to main memory. When cleared to 0, the DMA engine associated with this input stream will be disabled. If the corresponding SSYNC bit is 0, input stream data will be taken from the link and moved to the FIFO and an over-run may occur
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_RUN_OFFSET      1
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_RUN_MASK        0x2

// Bitfield Description : Interrupt On Completion Enable (IOCE):
// This bit controls whether or not an interrupt occurs when a buffer completes with the IOC bit set in its descriptor. If this bit is not set, bit 3 in the Status register will be set, but the interrupt will not occur.
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_IOCE_OFFSET      2
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_IOCE_MASK        0x4

// Bitfield Description : FIFO Error Interrupt Enable (FEIE):
// This bit controls whether the occurrence of a FIFO error (overrun for input or under run for output) will cause an interrupt or not. If this bit is not set, bit 4 in the Status register will be set, but the interrupt will not occur. Either way, the samples will be dropped.
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_FEIE_OFFSET      3
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_FEIE_MASK        0x8

// Bitfield Description : Descriptor Error Interrupt Enable (DEIE):
// Controls whether an interrupt is generated when the Descriptor Error Status (DESE) bit is set.
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_DEIE_OFFSET      4
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_DEIE_MASK        0x10

// Bitfield Description : 
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_Reserved_15_5_OFFSET      5
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_Reserved_15_5_MASK        0xffe0

// Bitfield Description : Stripe Control (STRIPE):
// (Output and Bidirectional engines configured for output only. Read Only 0 for input streams.) If the NSDO field of the Global Capabilities register indicates that the controller supports multiple SDO lines and the codec has been determined to have compatible capabilities, STRIPE can be used to indicate how many of the SDO lines the stream should be striped across.
// 00: 1 SDO
// 01: 2 SDOs
// 10: 4 SDOs
// 11: Reserved
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_STRIPE_OFFSET      16
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_STRIPE_MASK        0x30000

// Bitfield Description : Traffic Priority (TP):
// If set to a 1, the stream will be treated as preferred traffic if the underlying bus supports it. If set to a 0, the traffic will be handled on a "best effort" basis. The actual meaning of this bit is specific to the hardware implementation. Depending on the hardware implementation, there may be additional restrictions on the traffic, and software should assume that the buffers associated with this stream will not be snooped or cached.
// On PCI Express*, for example, setting the TP bit to a 1 might cause the controller to generate non-snooped isochronous traffic, while a PCI implementation may ignore this bit.
// It is permitted to implement this bit as RO if there is no controllability available.
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_TP_OFFSET      18
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_TP_MASK        0x40000

// Bitfield Description : Bidirectional Direction Control (DIR): (Bidirectional engines only. Read-only 0 for engines which are not bidirectional.) For a bidirectional engine, this bit determines the direction in which the bidirectional engine should operate. This bit can only be changed after stream reset (SRST) has been asserted and cleared and before any other stream registers have been programmed. Because setting this bit changes the fundamental behavior of the stream and the meaning of some bits, changing this bit after any other register in the stream descriptor has been written to may lead to undetermined results.
// 0 = Bidirectional engine is configured as an Input Engine.
// 1 = Bidirectional engine is configured as an Output Engine.
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_DIR_OFFSET      19
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_DIR_MASK        0x80000

// Bitfield Description : Stream Number (STRM):
// This value reflects the Tag associated with the data being transferred on the link.
// When data controlled by this descriptor is sent out over the link, it will have this stream number encoded on the SYNC signal.
// When an input stream is detected on any of the SDATA_INx signals that match this value, the data samples are loaded into the FIFO associated with this descriptor. Note that while a single SDAT_INx input may contain data from more than one stream number, two different SDATA_INx inputs may not be configured with the same stream number.
// Although the controller hardware is capable of transmitting any stream number, by convention stream 0 is reserved as unused by software, so that converters whose stream numbers have been reset to 0 do not unintentionally decode data not intended for them.
// 0000 = Reserved (Indicates Unused)
// 0001 = Stream 1
// ...
// 1110 = Stream 14
// 1111 = Stream 15
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_STRM_OFFSET      20
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_STRM_MASK        0xf00000

// Bitfield Description : 
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_Reserved_25_24_OFFSET      24
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_Reserved_25_24_MASK        0x3000000

// Bitfield Description : Buffer Completion Interrupt Status (BCIS):
// For an outbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor has been fetched from memory and put into the DMA FIFO, and the current descriptor has the IOC bit set.
// For an inbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor with an IOC bit set has been removed from the DMA FIFO and the current descriptor has the IOC bit set.
// BCIS remains active until software clears it by writing a 1 to this bit position.
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_BCIS_OFFSET      26
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_BCIS_MASK        0x4000000

// Bitfield Description : FIFO Error (FIFOE):
// Set when a FIFO error occurs. Bit is cleared by writing a 1 to this bit position. This bit is set even if an interrupt is not enabled.
// For an input stream, this indicates a FIFO overrun occurring while the RUN bit is set. When this happens, the FIFO pointers do not increment and the incoming data is not written into the FIFO, thereby being lost.
// For an output stream, this indicates a FIFO under run when there are still buffers to send. The hardware should not transmit anything on the link for the associated stream if there is not valid data to send.
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_FIFOE_OFFSET      27
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_FIFOE_MASK        0x8000000

// Bitfield Description : Descriptor Error (DESE):
// During the fetch of a descriptor, an error has occurred. This could be a result of a Master Abort, a Parity or ECC error on the bus, or any other error that renders the current Buffer Descriptor or Buffer Descriptor List useless. This error is treated as a fatal stream error as the stream cannot continue running. The RUN bit will be cleared and the stream will stop. Software may attempt to restart the stream engine after addressing the cause of the error and writing a 1 to this bit to clear it.
// Note : It is not implemented and is tied to ZERO in hardware.
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_DESE_OFFSET      28
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_DESE_MASK        0x10000000

// Bitfield Description : FIFO Ready (FIFORDY):
// For an Output stream, the controller hardware will set this bit to a 1 while the output DMA FIFO contains enough data to maintain the stream on the link. This bit defaults to 0 on reset because the FIFO is cleared on a reset. The amount of data required to maintain the stream will depend on the controller implementation but, in general, for an output stream, it means that the FIFO is full.
// For an input stream, this bit indicates that a descriptor has been fetched, and the engine is ready for the RUN bit to be set.
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_FIFORDY_OFFSET      29
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_FIFORDY_MASK        0x20000000

// Bitfield Description : 
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_Reserved_31_30_OFFSET      30
#define AUDIO_AZ_INPUT_SD3CTL_AND_STS_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            SRST:1;
    UINT32                            RUN:1;
    UINT32                            IOCE:1;
    UINT32                            FEIE:1;
    UINT32                            DEIE:1;
    UINT32                            Reserved_15_5:11;
    UINT32                            STRIPE:2;
    UINT32                            TP:1;
    UINT32                            DIR:1;
    UINT32                            STRM:4;
    UINT32                            Reserved_25_24:2;
    UINT32                            BCIS:1;
    UINT32                            FIFOE:1;
    UINT32                            DESE:1;
    UINT32                            FIFORDY:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} AUDIO_AZ_INPUT_SD3CTL_AND_STS_STRUCT;

#define SMN_AUDIO_AZ_INPUT_SD3CTL_AND_STS_ADDRESS    0x12000e0UL


/***********************************************************
* Register Name : AUDIO_AZ_IOC_CLKGATE_CONTROL
* Register Description :
* Control to clock gate the azalia
* Visibility : 0x2
************************************************************/

// Bitfield Description : This field is used for clcok gating azalia from the ioc. 1 is written into this register from the IOC to initiate the clock gate of azalia controller. The SW deassert after reading DONE bit set after the pending data transfer is completed. Before applying clock gating DMAs should be disabled.
#define AUDIO_AZ_IOC_CLKGATE_CONTROL_IOC_CLK_GATE_OFFSET      0
#define AUDIO_AZ_IOC_CLKGATE_CONTROL_IOC_CLK_GATE_MASK        0x1

// Bitfield Description : Azalia set this bit to 1 after internal clock gate is done, reset to 0 when software writes 1.
#define AUDIO_AZ_IOC_CLKGATE_CONTROL_IOC_CLK_GATE_DONE_OFFSET      1
#define AUDIO_AZ_IOC_CLKGATE_CONTROL_IOC_CLK_GATE_DONE_MASK        0x2

// Bitfield Description : 
#define AUDIO_AZ_IOC_CLKGATE_CONTROL_Reserved_31_2_OFFSET      2
#define AUDIO_AZ_IOC_CLKGATE_CONTROL_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            IOC_CLK_GATE:1;
    UINT32                            IOC_CLK_GATE_DONE:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} AUDIO_AZ_IOC_CLKGATE_CONTROL_STRUCT;

#define SMN_AUDIO_AZ_IOC_CLKGATE_CONTROL_ADDRESS    0x1204008UL


/***********************************************************
* Register Name : AUDIO_AZ_IOC_SOFTRST_CONTROL
* Register Description :
* Control to Soft reset the azalia
* Visibility : 0x2
************************************************************/

// Bitfield Description : This field is used for soft reset azalia from the ioc. 1 is written into this register from the IOC to initiate the soft reset of azalia controller. The SW deassert after reading DONE bit set after the pending data transfer is completed.  Before applying soft reset, Controller DMAs should be disabled and CRST should be applied as per HD-Audio Spec
#define AUDIO_AZ_IOC_SOFTRST_CONTROL_IOC_SFT_RST_OFFSET      0
#define AUDIO_AZ_IOC_SOFTRST_CONTROL_IOC_SFT_RST_MASK        0x1

// Bitfield Description : Azalia set this bit to 1 after internal soft reset done, reset to 0 when software writes 1.
#define AUDIO_AZ_IOC_SOFTRST_CONTROL_IOC_SFT_RST_DONE_OFFSET      1
#define AUDIO_AZ_IOC_SOFTRST_CONTROL_IOC_SFT_RST_DONE_MASK        0x2

// Bitfield Description : 
#define AUDIO_AZ_IOC_SOFTRST_CONTROL_Reserved_31_2_OFFSET      2
#define AUDIO_AZ_IOC_SOFTRST_CONTROL_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            IOC_SFT_RST:1;
    UINT32                            IOC_SFT_RST_DONE:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} AUDIO_AZ_IOC_SOFTRST_CONTROL_STRUCT;

#define SMN_AUDIO_AZ_IOC_SOFTRST_CONTROL_ADDRESS    0x1204004UL


/***********************************************************
* Register Name : AUDIO_AZ_OUTPUT_SD0CTL_AND_STS
* Register Description :
* Output Stream Descriptor 0 Control and Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : Stream Reset (SRST):
// Writing a 1 causes the corresponding stream to be reset. The Stream Descriptor registers (except the SRST bit itself), FIFO's, and cadence generator for the corresponding stream are reset. After the stream hardware has completed sequencing into the reset state, it will report a 1 in this bit. Software must read a 1 from this bit to verify that the stream is in reset. Writing a 0 causes the corresponding stream to exit reset. When the stream hardware is ready to begin operation, it will report a 0 in this bit. Software must read a 0 from this bit before accessing any of the stream registers. The RUN bit must be cleared before SRST is asserted.
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_SRST_OFFSET      0
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_SRST_MASK        0x1

// Bitfield Description : Stream Run (RUN):
// When set to 1, the DMA engine associated with this input stream will be enabled to transfer data in the FIFO to main memory. When cleared to 0, the DMA engine associated with this input stream will be disabled. If the corresponding SSYNC bit is 0, input stream data will be taken from the link and moved to the FIFO and an over-run may occur
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_RUN_OFFSET      1
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_RUN_MASK        0x2

// Bitfield Description : Interrupt On Completion Enable (IOCE):
// This bit controls whether or not an interrupt occurs when a buffer completes with the IOC bit set in its descriptor. If this bit is not set, bit 3 in the Status register will be set, but the interrupt will not occur.
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_IOCE_OFFSET      2
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_IOCE_MASK        0x4

// Bitfield Description : FIFO Error Interrupt Enable (FEIE):
// This bit controls whether the occurrence of a FIFO error (overrun for input or under run for output) will cause an interrupt or not. If this bit is not set, bit 4 in the Status register will be set, but the interrupt will not occur. Either way, the samples will be dropped.
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_FEIE_OFFSET      3
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_FEIE_MASK        0x8

// Bitfield Description : Descriptor Error Interrupt Enable (DEIE):
// Controls whether an interrupt is generated when the Descriptor Error Status (DESE) bit is set.
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_DEIE_OFFSET      4
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_DEIE_MASK        0x10

// Bitfield Description : 
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_Reserved_15_5_OFFSET      5
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_Reserved_15_5_MASK        0xffe0

// Bitfield Description : Stripe Control (STRIPE):
// (Output and Bidirectional engines configured for output only. Read Only 0 for input streams.) If the NSDO field of the Global Capabilities register indicates that the controller supports multiple SDO lines and the codec has been determined to have compatible capabilities, STRIPE can be used to indicate how many of the SDO lines the stream should be striped across.
// 00: 1 SDO
// 01: 2 SDOs
// 10: 4 SDOs
// 11: Reserved
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_STRIPE_OFFSET      16
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_STRIPE_MASK        0x30000

// Bitfield Description : Traffic Priority (TP):
// If set to a 1, the stream will be treated as preferred traffic if the underlying bus supports it. If set to a 0, the traffic will be handled on a "best effort" basis. The actual meaning of this bit is specific to the hardware implementation. Depending on the hardware implementation, there may be additional restrictions on the traffic, and software should assume that the buffers associated with this stream will not be snooped or cached.
// On PCI Express*, for example, setting the TP bit to a 1 might cause the controller to generate non-snooped isochronous traffic, while a PCI implementation may ignore this bit.
// It is permitted to implement this bit as RO if there is no controllability available.
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_TP_OFFSET      18
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_TP_MASK        0x40000

// Bitfield Description : Bidirectional Direction Control (DIR): (Bidirectional engines only. Read-only 0 for engines which are not bidirectional.) For a bidirectional engine, this bit determines the direction in which the bidirectional engine should operate. This bit can only be changed after stream reset (SRST) has been asserted and cleared and before any other stream registers have been programmed. Because setting this bit changes the fundamental behavior of the stream and the meaning of some bits, changing this bit after any other register in the stream descriptor has been written to may lead to undetermined results.
// 0 = Bidirectional engine is configured as an Input Engine.
// 1 = Bidirectional engine is configured as an Output Engine.
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_DIR_OFFSET      19
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_DIR_MASK        0x80000

// Bitfield Description : Stream Number (STRM):
// This value reflects the Tag associated with the data being transferred on the link.
// When data controlled by this descriptor is sent out over the link, it will have this stream number encoded on the SYNC signal.
// When an input stream is detected on any of the SDATA_INx signals that match this value, the data samples are loaded into the FIFO associated with this descriptor. Note that while a single SDAT_INx input may contain data from more than one stream number, two different SDATA_INx inputs may not be configured with the same stream number.
// Although the controller hardware is capable of transmitting any stream number, by convention stream 0 is reserved as unused by software, so that converters whose stream numbers have been reset to 0 do not unintentionally decode data not intended for them.
// 0000 = Reserved (Indicates Unused)
// 0001 = Stream 1
// ...
// 1110 = Stream 14
// 1111 = Stream 15
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_STRM_OFFSET      20
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_STRM_MASK        0xf00000

// Bitfield Description : 
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_Reserved_25_24_OFFSET      24
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_Reserved_25_24_MASK        0x3000000

// Bitfield Description : Buffer Completion Interrupt Status (BCIS):
// For an outbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor has been fetched from memory and put into the DMA FIFO, and the current descriptor has the IOC bit set.
// For an inbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor with an IOC bit set has been removed from the DMA FIFO and the current descriptor has the IOC bit set.
// BCIS remains active until software clears it by writing a 1 to this bit position.
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_BCIS_OFFSET      26
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_BCIS_MASK        0x4000000

// Bitfield Description : FIFO Error (FIFOE):
// Set when a FIFO error occurs. Bit is cleared by writing a 1 to this bit position. This bit is set even if an interrupt is not enabled.
// For an input stream, this indicates a FIFO overrun occurring while the RUN bit is set. When this happens, the FIFO pointers do not increment and the incoming data is not written into the FIFO, thereby being lost.
// For an output stream, this indicates a FIFO under run when there are still buffers to send. The hardware should not transmit anything on the link for the associated stream if there is not valid data to send.
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_FIFOE_OFFSET      27
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_FIFOE_MASK        0x8000000

// Bitfield Description : Descriptor Error (DESE):
// During the fetch of a descriptor, an error has occurred. This could be a result of a Master Abort, a Parity or ECC error on the bus, or any other error that renders the current Buffer Descriptor or Buffer Descriptor List useless. This error is treated as a fatal stream error as the stream cannot continue running. The RUN bit will be cleared and the stream will stop. Software may attempt to restart the stream engine after addressing the cause of the error and writing a 1 to this bit to clear it.
// Note : It is not implemented and is tied to ZERO in hardware.
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_DESE_OFFSET      28
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_DESE_MASK        0x10000000

// Bitfield Description : FIFO Ready (FIFORDY):
// For an Output stream, the controller hardware will set this bit to a 1 while the output DMA FIFO contains enough data to maintain the stream on the link. This bit defaults to 0 on reset because the FIFO is cleared on a reset. The amount of data required to maintain the stream will depend on the controller implementation but, in general, for an output stream, it means that the FIFO is full.
// For an input stream, this bit indicates that a descriptor has been fetched, and the engine is ready for the RUN bit to be set.
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_FIFORDY_OFFSET      29
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_FIFORDY_MASK        0x20000000

// Bitfield Description : 
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_Reserved_31_30_OFFSET      30
#define AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            SRST:1;
    UINT32                            RUN:1;
    UINT32                            IOCE:1;
    UINT32                            FEIE:1;
    UINT32                            DEIE:1;
    UINT32                            Reserved_15_5:11;
    UINT32                            STRIPE:2;
    UINT32                            TP:1;
    UINT32                            DIR:1;
    UINT32                            STRM:4;
    UINT32                            Reserved_25_24:2;
    UINT32                            BCIS:1;
    UINT32                            FIFOE:1;
    UINT32                            DESE:1;
    UINT32                            FIFORDY:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_STRUCT;

#define SMN_AUDIO_AZ_OUTPUT_SD0CTL_AND_STS_ADDRESS    0x1200100UL


/***********************************************************
* Register Name : AUDIO_AZ_OUTPUT_SD1CTL_AND_STS
* Register Description :
* Output Stream Descriptor 1 Control and Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : Stream Reset (SRST):
// Writing a 1 causes the corresponding stream to be reset. The Stream Descriptor registers (except the SRST bit itself), FIFO's, and cadence generator for the corresponding stream are reset. After the stream hardware has completed sequencing into the reset state, it will report a 1 in this bit. Software must read a 1 from this bit to verify that the stream is in reset. Writing a 0 causes the corresponding stream to exit reset. When the stream hardware is ready to begin operation, it will report a 0 in this bit. Software must read a 0 from this bit before accessing any of the stream registers. The RUN bit must be cleared before SRST is asserted.
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_SRST_OFFSET      0
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_SRST_MASK        0x1

// Bitfield Description : Stream Run (RUN):
// When set to 1, the DMA engine associated with this input stream will be enabled to transfer data in the FIFO to main memory. When cleared to 0, the DMA engine associated with this input stream will be disabled. If the corresponding SSYNC bit is 0, input stream data will be taken from the link and moved to the FIFO and an over-run may occur
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_RUN_OFFSET      1
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_RUN_MASK        0x2

// Bitfield Description : Interrupt On Completion Enable (IOCE):
// This bit controls whether or not an interrupt occurs when a buffer completes with the IOC bit set in its descriptor. If this bit is not set, bit 3 in the Status register will be set, but the interrupt will not occur.
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_IOCE_OFFSET      2
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_IOCE_MASK        0x4

// Bitfield Description : FIFO Error Interrupt Enable (FEIE):
// This bit controls whether the occurrence of a FIFO error (overrun for input or under run for output) will cause an interrupt or not. If this bit is not set, bit 4 in the Status register will be set, but the interrupt will not occur. Either way, the samples will be dropped.
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_FEIE_OFFSET      3
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_FEIE_MASK        0x8

// Bitfield Description : Descriptor Error Interrupt Enable (DEIE):
// Controls whether an interrupt is generated when the Descriptor Error Status (DESE) bit is set.
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_DEIE_OFFSET      4
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_DEIE_MASK        0x10

// Bitfield Description : 
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_Reserved_15_5_OFFSET      5
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_Reserved_15_5_MASK        0xffe0

// Bitfield Description : Stripe Control (STRIPE):
// (Output and Bidirectional engines configured for output only. Read Only 0 for input streams.) If the NSDO field of the Global Capabilities register indicates that the controller supports multiple SDO lines and the codec has been determined to have compatible capabilities, STRIPE can be used to indicate how many of the SDO lines the stream should be striped across.
// 00: 1 SDO
// 01: 2 SDOs
// 10: 4 SDOs
// 11: Reserved
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_STRIPE_OFFSET      16
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_STRIPE_MASK        0x30000

// Bitfield Description : Traffic Priority (TP):
// If set to a 1, the stream will be treated as preferred traffic if the underlying bus supports it. If set to a 0, the traffic will be handled on a "best effort" basis. The actual meaning of this bit is specific to the hardware implementation. Depending on the hardware implementation, there may be additional restrictions on the traffic, and software should assume that the buffers associated with this stream will not be snooped or cached.
// On PCI Express*, for example, setting the TP bit to a 1 might cause the controller to generate non-snooped isochronous traffic, while a PCI implementation may ignore this bit.
// It is permitted to implement this bit as RO if there is no controllability available.
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_TP_OFFSET      18
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_TP_MASK        0x40000

// Bitfield Description : Bidirectional Direction Control (DIR): (Bidirectional engines only. Read-only 0 for engines which are not bidirectional.) For a bidirectional engine, this bit determines the direction in which the bidirectional engine should operate. This bit can only be changed after stream reset (SRST) has been asserted and cleared and before any other stream registers have been programmed. Because setting this bit changes the fundamental behavior of the stream and the meaning of some bits, changing this bit after any other register in the stream descriptor has been written to may lead to undetermined results.
// 0 = Bidirectional engine is configured as an Input Engine.
// 1 = Bidirectional engine is configured as an Output Engine.
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_DIR_OFFSET      19
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_DIR_MASK        0x80000

// Bitfield Description : Stream Number (STRM):
// This value reflects the Tag associated with the data being transferred on the link.
// When data controlled by this descriptor is sent out over the link, it will have this stream number encoded on the SYNC signal.
// When an input stream is detected on any of the SDATA_INx signals that match this value, the data samples are loaded into the FIFO associated with this descriptor. Note that while a single SDAT_INx input may contain data from more than one stream number, two different SDATA_INx inputs may not be configured with the same stream number.
// Although the controller hardware is capable of transmitting any stream number, by convention stream 0 is reserved as unused by software, so that converters whose stream numbers have been reset to 0 do not unintentionally decode data not intended for them.
// 0000 = Reserved (Indicates Unused)
// 0001 = Stream 1
// ...
// 1110 = Stream 14
// 1111 = Stream 15
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_STRM_OFFSET      20
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_STRM_MASK        0xf00000

// Bitfield Description : 
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_Reserved_25_24_OFFSET      24
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_Reserved_25_24_MASK        0x3000000

// Bitfield Description : Buffer Completion Interrupt Status (BCIS):
// For an outbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor has been fetched from memory and put into the DMA FIFO, and the current descriptor has the IOC bit set.
// For an inbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor with an IOC bit set has been removed from the DMA FIFO and the current descriptor has the IOC bit set.
// BCIS remains active until software clears it by writing a 1 to this bit position.
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_BCIS_OFFSET      26
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_BCIS_MASK        0x4000000

// Bitfield Description : FIFO Error (FIFOE):
// Set when a FIFO error occurs. Bit is cleared by writing a 1 to this bit position. This bit is set even if an interrupt is not enabled.
// For an input stream, this indicates a FIFO overrun occurring while the RUN bit is set. When this happens, the FIFO pointers do not increment and the incoming data is not written into the FIFO, thereby being lost.
// For an output stream, this indicates a FIFO under run when there are still buffers to send. The hardware should not transmit anything on the link for the associated stream if there is not valid data to send.
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_FIFOE_OFFSET      27
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_FIFOE_MASK        0x8000000

// Bitfield Description : Descriptor Error (DESE):
// During the fetch of a descriptor, an error has occurred. This could be a result of a Master Abort, a Parity or ECC error on the bus, or any other error that renders the current Buffer Descriptor or Buffer Descriptor List useless. This error is treated as a fatal stream error as the stream cannot continue running. The RUN bit will be cleared and the stream will stop. Software may attempt to restart the stream engine after addressing the cause of the error and writing a 1 to this bit to clear it.
// Note : It is not implemented and is tied to ZERO in hardware.
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_DESE_OFFSET      28
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_DESE_MASK        0x10000000

// Bitfield Description : FIFO Ready (FIFORDY):
// For an Output stream, the controller hardware will set this bit to a 1 while the output DMA FIFO contains enough data to maintain the stream on the link. This bit defaults to 0 on reset because the FIFO is cleared on a reset. The amount of data required to maintain the stream will depend on the controller implementation but, in general, for an output stream, it means that the FIFO is full.
// For an input stream, this bit indicates that a descriptor has been fetched, and the engine is ready for the RUN bit to be set.
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_FIFORDY_OFFSET      29
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_FIFORDY_MASK        0x20000000

// Bitfield Description : 
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_Reserved_31_30_OFFSET      30
#define AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            SRST:1;
    UINT32                            RUN:1;
    UINT32                            IOCE:1;
    UINT32                            FEIE:1;
    UINT32                            DEIE:1;
    UINT32                            Reserved_15_5:11;
    UINT32                            STRIPE:2;
    UINT32                            TP:1;
    UINT32                            DIR:1;
    UINT32                            STRM:4;
    UINT32                            Reserved_25_24:2;
    UINT32                            BCIS:1;
    UINT32                            FIFOE:1;
    UINT32                            DESE:1;
    UINT32                            FIFORDY:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_STRUCT;

#define SMN_AUDIO_AZ_OUTPUT_SD1CTL_AND_STS_ADDRESS    0x1200120UL


/***********************************************************
* Register Name : AUDIO_AZ_OUTPUT_SD2CTL_AND_STS
* Register Description :
* Output Stream Descriptor 2 Control and Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : Stream Reset (SRST):
// Writing a 1 causes the corresponding stream to be reset. The Stream Descriptor registers (except the SRST bit itself), FIFO's, and cadence generator for the corresponding stream are reset. After the stream hardware has completed sequencing into the reset state, it will report a 1 in this bit. Software must read a 1 from this bit to verify that the stream is in reset. Writing a 0 causes the corresponding stream to exit reset. When the stream hardware is ready to begin operation, it will report a 0 in this bit. Software must read a 0 from this bit before accessing any of the stream registers. The RUN bit must be cleared before SRST is asserted.
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_SRST_OFFSET      0
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_SRST_MASK        0x1

// Bitfield Description : Stream Run (RUN):
// When set to 1, the DMA engine associated with this input stream will be enabled to transfer data in the FIFO to main memory. When cleared to 0, the DMA engine associated with this input stream will be disabled. If the corresponding SSYNC bit is 0, input stream data will be taken from the link and moved to the FIFO and an over-run may occur
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_RUN_OFFSET      1
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_RUN_MASK        0x2

// Bitfield Description : Interrupt On Completion Enable (IOCE):
// This bit controls whether or not an interrupt occurs when a buffer completes with the IOC bit set in its descriptor. If this bit is not set, bit 3 in the Status register will be set, but the interrupt will not occur.
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_IOCE_OFFSET      2
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_IOCE_MASK        0x4

// Bitfield Description : FIFO Error Interrupt Enable (FEIE):
// This bit controls whether the occurrence of a FIFO error (overrun for input or under run for output) will cause an interrupt or not. If this bit is not set, bit 4 in the Status register will be set, but the interrupt will not occur. Either way, the samples will be dropped.
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_FEIE_OFFSET      3
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_FEIE_MASK        0x8

// Bitfield Description : Descriptor Error Interrupt Enable (DEIE):
// Controls whether an interrupt is generated when the Descriptor Error Status (DESE) bit is set.
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_DEIE_OFFSET      4
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_DEIE_MASK        0x10

// Bitfield Description : 
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_Reserved_15_5_OFFSET      5
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_Reserved_15_5_MASK        0xffe0

// Bitfield Description : Stripe Control (STRIPE):
// (Output and Bidirectional engines configured for output only. Read Only 0 for input streams.) If the NSDO field of the Global Capabilities register indicates that the controller supports multiple SDO lines and the codec has been determined to have compatible capabilities, STRIPE can be used to indicate how many of the SDO lines the stream should be striped across.
// 00: 1 SDO
// 01: 2 SDOs
// 10: 4 SDOs
// 11: Reserved
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_STRIPE_OFFSET      16
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_STRIPE_MASK        0x30000

// Bitfield Description : Traffic Priority (TP):
// If set to a 1, the stream will be treated as preferred traffic if the underlying bus supports it. If set to a 0, the traffic will be handled on a "best effort" basis. The actual meaning of this bit is specific to the hardware implementation. Depending on the hardware implementation, there may be additional restrictions on the traffic, and software should assume that the buffers associated with this stream will not be snooped or cached.
// On PCI Express*, for example, setting the TP bit to a 1 might cause the controller to generate non-snooped isochronous traffic, while a PCI implementation may ignore this bit.
// It is permitted to implement this bit as RO if there is no controllability available.
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_TP_OFFSET      18
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_TP_MASK        0x40000

// Bitfield Description : Bidirectional Direction Control (DIR): (Bidirectional engines only. Read-only 0 for engines which are not bidirectional.) For a bidirectional engine, this bit determines the direction in which the bidirectional engine should operate. This bit can only be changed after stream reset (SRST) has been asserted and cleared and before any other stream registers have been programmed. Because setting this bit changes the fundamental behavior of the stream and the meaning of some bits, changing this bit after any other register in the stream descriptor has been written to may lead to undetermined results.
// 0 = Bidirectional engine is configured as an Input Engine.
// 1 = Bidirectional engine is configured as an Output Engine.
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_DIR_OFFSET      19
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_DIR_MASK        0x80000

// Bitfield Description : Stream Number (STRM):
// This value reflects the Tag associated with the data being transferred on the link.
// When data controlled by this descriptor is sent out over the link, it will have this stream number encoded on the SYNC signal.
// When an input stream is detected on any of the SDATA_INx signals that match this value, the data samples are loaded into the FIFO associated with this descriptor. Note that while a single SDAT_INx input may contain data from more than one stream number, two different SDATA_INx inputs may not be configured with the same stream number.
// Although the controller hardware is capable of transmitting any stream number, by convention stream 0 is reserved as unused by software, so that converters whose stream numbers have been reset to 0 do not unintentionally decode data not intended for them.
// 0000 = Reserved (Indicates Unused)
// 0001 = Stream 1
// ...
// 1110 = Stream 14
// 1111 = Stream 15
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_STRM_OFFSET      20
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_STRM_MASK        0xf00000

// Bitfield Description : 
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_Reserved_25_24_OFFSET      24
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_Reserved_25_24_MASK        0x3000000

// Bitfield Description : Buffer Completion Interrupt Status (BCIS):
// For an outbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor has been fetched from memory and put into the DMA FIFO, and the current descriptor has the IOC bit set.
// For an inbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor with an IOC bit set has been removed from the DMA FIFO and the current descriptor has the IOC bit set.
// BCIS remains active until software clears it by writing a 1 to this bit position.
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_BCIS_OFFSET      26
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_BCIS_MASK        0x4000000

// Bitfield Description : FIFO Error (FIFOE):
// Set when a FIFO error occurs. Bit is cleared by writing a 1 to this bit position. This bit is set even if an interrupt is not enabled.
// For an input stream, this indicates a FIFO overrun occurring while the RUN bit is set. When this happens, the FIFO pointers do not increment and the incoming data is not written into the FIFO, thereby being lost.
// For an output stream, this indicates a FIFO under run when there are still buffers to send. The hardware should not transmit anything on the link for the associated stream if there is not valid data to send.
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_FIFOE_OFFSET      27
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_FIFOE_MASK        0x8000000

// Bitfield Description : Descriptor Error (DESE):
// During the fetch of a descriptor, an error has occurred. This could be a result of a Master Abort, a Parity or ECC error on the bus, or any other error that renders the current Buffer Descriptor or Buffer Descriptor List useless. This error is treated as a fatal stream error as the stream cannot continue running. The RUN bit will be cleared and the stream will stop. Software may attempt to restart the stream engine after addressing the cause of the error and writing a 1 to this bit to clear it.
// Note : It is not implemented and is tied to ZERO in hardware.
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_DESE_OFFSET      28
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_DESE_MASK        0x10000000

// Bitfield Description : FIFO Ready (FIFORDY):
// For an Output stream, the controller hardware will set this bit to a 1 while the output DMA FIFO contains enough data to maintain the stream on the link. This bit defaults to 0 on reset because the FIFO is cleared on a reset. The amount of data required to maintain the stream will depend on the controller implementation but, in general, for an output stream, it means that the FIFO is full.
// For an input stream, this bit indicates that a descriptor has been fetched, and the engine is ready for the RUN bit to be set.
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_FIFORDY_OFFSET      29
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_FIFORDY_MASK        0x20000000

// Bitfield Description : 
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_Reserved_31_30_OFFSET      30
#define AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            SRST:1;
    UINT32                            RUN:1;
    UINT32                            IOCE:1;
    UINT32                            FEIE:1;
    UINT32                            DEIE:1;
    UINT32                            Reserved_15_5:11;
    UINT32                            STRIPE:2;
    UINT32                            TP:1;
    UINT32                            DIR:1;
    UINT32                            STRM:4;
    UINT32                            Reserved_25_24:2;
    UINT32                            BCIS:1;
    UINT32                            FIFOE:1;
    UINT32                            DESE:1;
    UINT32                            FIFORDY:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_STRUCT;

#define SMN_AUDIO_AZ_OUTPUT_SD2CTL_AND_STS_ADDRESS    0x1200140UL


/***********************************************************
* Register Name : AUDIO_AZ_OUTPUT_SD3CTL_AND_STS
* Register Description :
* Output Stream Descriptor 3 Control and Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : Stream Reset (SRST):
// Writing a 1 causes the corresponding stream to be reset. The Stream Descriptor registers (except the SRST bit itself), FIFO's, and cadence generator for the corresponding stream are reset. After the stream hardware has completed sequencing into the reset state, it will report a 1 in this bit. Software must read a 1 from this bit to verify that the stream is in reset. Writing a 0 causes the corresponding stream to exit reset. When the stream hardware is ready to begin operation, it will report a 0 in this bit. Software must read a 0 from this bit before accessing any of the stream registers. The RUN bit must be cleared before SRST is asserted.
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_SRST_OFFSET      0
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_SRST_MASK        0x1

// Bitfield Description : Stream Run (RUN):
// When set to 1, the DMA engine associated with this input stream will be enabled to transfer data in the FIFO to main memory. When cleared to 0, the DMA engine associated with this input stream will be disabled. If the corresponding SSYNC bit is 0, input stream data will be taken from the link and moved to the FIFO and an over-run may occur
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_RUN_OFFSET      1
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_RUN_MASK        0x2

// Bitfield Description : Interrupt On Completion Enable (IOCE):
// This bit controls whether or not an interrupt occurs when a buffer completes with the IOC bit set in its descriptor. If this bit is not set, bit 3 in the Status register will be set, but the interrupt will not occur.
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_IOCE_OFFSET      2
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_IOCE_MASK        0x4

// Bitfield Description : FIFO Error Interrupt Enable (FEIE):
// This bit controls whether the occurrence of a FIFO error (overrun for input or under run for output) will cause an interrupt or not. If this bit is not set, bit 4 in the Status register will be set, but the interrupt will not occur. Either way, the samples will be dropped.
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_FEIE_OFFSET      3
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_FEIE_MASK        0x8

// Bitfield Description : Descriptor Error Interrupt Enable (DEIE):
// Controls whether an interrupt is generated when the Descriptor Error Status (DESE) bit is set.
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_DEIE_OFFSET      4
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_DEIE_MASK        0x10

// Bitfield Description : 
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_Reserved_15_5_OFFSET      5
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_Reserved_15_5_MASK        0xffe0

// Bitfield Description : Stripe Control (STRIPE):
// (Output and Bidirectional engines configured for output only. Read Only 0 for input streams.) If the NSDO field of the Global Capabilities register indicates that the controller supports multiple SDO lines and the codec has been determined to have compatible capabilities, STRIPE can be used to indicate how many of the SDO lines the stream should be striped across.
// 00: 1 SDO
// 01: 2 SDOs
// 10: 4 SDOs
// 11: Reserved
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_STRIPE_OFFSET      16
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_STRIPE_MASK        0x30000

// Bitfield Description : Traffic Priority (TP):
// If set to a 1, the stream will be treated as preferred traffic if the underlying bus supports it. If set to a 0, the traffic will be handled on a "best effort" basis. The actual meaning of this bit is specific to the hardware implementation. Depending on the hardware implementation, there may be additional restrictions on the traffic, and software should assume that the buffers associated with this stream will not be snooped or cached.
// On PCI Express*, for example, setting the TP bit to a 1 might cause the controller to generate non-snooped isochronous traffic, while a PCI implementation may ignore this bit.
// It is permitted to implement this bit as RO if there is no controllability available.
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_TP_OFFSET      18
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_TP_MASK        0x40000

// Bitfield Description : Bidirectional Direction Control (DIR): (Bidirectional engines only. Read-only 0 for engines which are not bidirectional.) For a bidirectional engine, this bit determines the direction in which the bidirectional engine should operate. This bit can only be changed after stream reset (SRST) has been asserted and cleared and before any other stream registers have been programmed. Because setting this bit changes the fundamental behavior of the stream and the meaning of some bits, changing this bit after any other register in the stream descriptor has been written to may lead to undetermined results.
// 0 = Bidirectional engine is configured as an Input Engine.
// 1 = Bidirectional engine is configured as an Output Engine.
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_DIR_OFFSET      19
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_DIR_MASK        0x80000

// Bitfield Description : Stream Number (STRM):
// This value reflects the Tag associated with the data being transferred on the link.
// When data controlled by this descriptor is sent out over the link, it will have this stream number encoded on the SYNC signal.
// When an input stream is detected on any of the SDATA_INx signals that match this value, the data samples are loaded into the FIFO associated with this descriptor. Note that while a single SDAT_INx input may contain data from more than one stream number, two different SDATA_INx inputs may not be configured with the same stream number.
// Although the controller hardware is capable of transmitting any stream number, by convention stream 0 is reserved as unused by software, so that converters whose stream numbers have been reset to 0 do not unintentionally decode data not intended for them.
// 0000 = Reserved (Indicates Unused)
// 0001 = Stream 1
// ...
// 1110 = Stream 14
// 1111 = Stream 15
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_STRM_OFFSET      20
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_STRM_MASK        0xf00000

// Bitfield Description : 
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_Reserved_25_24_OFFSET      24
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_Reserved_25_24_MASK        0x3000000

// Bitfield Description : Buffer Completion Interrupt Status (BCIS):
// For an outbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor has been fetched from memory and put into the DMA FIFO, and the current descriptor has the IOC bit set.
// For an inbound engine, this bit is set to 1 by the hardware after the last byte of data for the current descriptor with an IOC bit set has been removed from the DMA FIFO and the current descriptor has the IOC bit set.
// BCIS remains active until software clears it by writing a 1 to this bit position.
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_BCIS_OFFSET      26
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_BCIS_MASK        0x4000000

// Bitfield Description : FIFO Error (FIFOE):
// Set when a FIFO error occurs. Bit is cleared by writing a 1 to this bit position. This bit is set even if an interrupt is not enabled.
// For an input stream, this indicates a FIFO overrun occurring while the RUN bit is set. When this happens, the FIFO pointers do not increment and the incoming data is not written into the FIFO, thereby being lost.
// For an output stream, this indicates a FIFO under run when there are still buffers to send. The hardware should not transmit anything on the link for the associated stream if there is not valid data to send.
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_FIFOE_OFFSET      27
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_FIFOE_MASK        0x8000000

// Bitfield Description : Descriptor Error (DESE):
// During the fetch of a descriptor, an error has occurred. This could be a result of a Master Abort, a Parity or ECC error on the bus, or any other error that renders the current Buffer Descriptor or Buffer Descriptor List useless. This error is treated as a fatal stream error as the stream cannot continue running. The RUN bit will be cleared and the stream will stop. Software may attempt to restart the stream engine after addressing the cause of the error and writing a 1 to this bit to clear it.
// Note : It is not implemented and is tied to ZERO in hardware.
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_DESE_OFFSET      28
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_DESE_MASK        0x10000000

// Bitfield Description : FIFO Ready (FIFORDY):
// For an Output stream, the controller hardware will set this bit to a 1 while the output DMA FIFO contains enough data to maintain the stream on the link. This bit defaults to 0 on reset because the FIFO is cleared on a reset. The amount of data required to maintain the stream will depend on the controller implementation but, in general, for an output stream, it means that the FIFO is full.
// For an input stream, this bit indicates that a descriptor has been fetched, and the engine is ready for the RUN bit to be set.
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_FIFORDY_OFFSET      29
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_FIFORDY_MASK        0x20000000

// Bitfield Description : 
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_Reserved_31_30_OFFSET      30
#define AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            SRST:1;
    UINT32                            RUN:1;
    UINT32                            IOCE:1;
    UINT32                            FEIE:1;
    UINT32                            DEIE:1;
    UINT32                            Reserved_15_5:11;
    UINT32                            STRIPE:2;
    UINT32                            TP:1;
    UINT32                            DIR:1;
    UINT32                            STRM:4;
    UINT32                            Reserved_25_24:2;
    UINT32                            BCIS:1;
    UINT32                            FIFOE:1;
    UINT32                            DESE:1;
    UINT32                            FIFORDY:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_STRUCT;

#define SMN_AUDIO_AZ_OUTPUT_SD3CTL_AND_STS_ADDRESS    0x1200160UL


/***********************************************************
* Register Name : AUDIO_AZ_POWER_MANAGEMENT_CONTROL
* Register Description :
* Power management control and status
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define AUDIO_AZ_POWER_MANAGEMENT_CONTROL_Reserved_7_0_OFFSET      0
#define AUDIO_AZ_POWER_MANAGEMENT_CONTROL_Reserved_7_0_MASK        0xff

// Bitfield Description : Enables the function to assert PME. This bit is in resume well and only cleared on power-on reset.
#define AUDIO_AZ_POWER_MANAGEMENT_CONTROL_PME_ENABLE_OFFSET      8
#define AUDIO_AZ_POWER_MANAGEMENT_CONTROL_PME_ENABLE_MASK        0x100

// Bitfield Description : 
#define AUDIO_AZ_POWER_MANAGEMENT_CONTROL_Reserved_14_9_OFFSET      9
#define AUDIO_AZ_POWER_MANAGEMENT_CONTROL_Reserved_14_9_MASK        0x7e00

// Bitfield Description : This bit set when when HD Audio controller asserts the PME signal, Writing a 1 clears this bit.
#define AUDIO_AZ_POWER_MANAGEMENT_CONTROL_PME_STATUS_OFFSET      15
#define AUDIO_AZ_POWER_MANAGEMENT_CONTROL_PME_STATUS_MASK        0x8000

// Bitfield Description : 
#define AUDIO_AZ_POWER_MANAGEMENT_CONTROL_Reserved_31_16_OFFSET      16
#define AUDIO_AZ_POWER_MANAGEMENT_CONTROL_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            Reserved_7_0:8;
    UINT32                            PME_ENABLE:1;
    UINT32                            Reserved_14_9:6;
    UINT32                            PME_STATUS:1;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} AUDIO_AZ_POWER_MANAGEMENT_CONTROL_STRUCT;

#define SMN_AUDIO_AZ_POWER_MANAGEMENT_CONTROL_ADDRESS    0x1204000UL


/***********************************************************
* Register Name : AudioAZ_Misc_Control_Register_1
* Register Description :
* AudioAZ Misc Control Register 1
* Visibility : 0x2
************************************************************/

// Bitfield Description : Use Azalia Output fifo static size only on Enable bit set 00-one eigth, 01-one fourth, 10-half of, 11- full of Max fifo size/n POSSIBLE VALUES:/n 00 - AZ_STATIC_OP_FIFO_SIZE_1by8MAX: Use 1/8 the of Max Op FIFO size /n 01 - AZ_STATIC_OP_FIFO_SIZE_1by4MAX: Use 1/4 the of Max Op FIFO sizes /n 02 - AZ_STATIC_OP_FIFO_SIZE_1by2MAX: Use 1/2 the of Max Op FIFO sizees /n 03 - AZ_STATIC_OP_FIFO_SIZE_MAX: Use Max Op FIFO size
#define AudioAZ_Misc_Control_Register_1_STATIC_OP_FIFO_SIZE_SEL_OFFSET      0
#define AudioAZ_Misc_Control_Register_1_STATIC_OP_FIFO_SIZE_SEL_MASK        0x3

// Bitfield Description : 
#define AudioAZ_Misc_Control_Register_1_Reserved_7_2_OFFSET      2
#define AudioAZ_Misc_Control_Register_1_Reserved_7_2_MASK        0xfc

// Bitfield Description : Azalia Output fifo static size Enable
// POSSIBLE VALUES:
// 00 - Disable
// 01 - Enable
#define AudioAZ_Misc_Control_Register_1_STATIC_OP_FIFO_SIZE_EN_OFFSET      8
#define AudioAZ_Misc_Control_Register_1_STATIC_OP_FIFO_SIZE_EN_MASK        0x100

// Bitfield Description : 
#define AudioAZ_Misc_Control_Register_1_Reserved_15_9_OFFSET      9
#define AudioAZ_Misc_Control_Register_1_Reserved_15_9_MASK        0xfe00

typedef union {
  struct {
    UINT16                            STATIC_OP_FIFO_SIZE_SEL:2;
    UINT16                            Reserved_7_2:6;
    UINT16                            STATIC_OP_FIFO_SIZE_EN:1;
    UINT16                            Reserved_15_9:7;
  } Field;
  UINT16 Value;
} AudioAZ_Misc_Control_Register_1_STRUCT;

#define SMN_AudioAZ_Misc_Control_Register_1_ADDRESS    0x1200180UL


/***********************************************************
* Register Name : AudioAZ_Misc_Control_Register_2
* Register Description :
* AudioAZ Misc Control Register 2
* Visibility : 0x2
************************************************************/

// Bitfield Description : 1: No Snoop attribute is disabled on Buffer Descriptor and Data Buffer DMA. 0: Set the No Snoop attribute on Buffer Descriptor and Data Buffer DMA when the Traffic Priority bit is set in the Stream Descriptor.
// POSSIBLE VALUES:
// 00 - Enable
// 01 - Disable
#define AudioAZ_Misc_Control_Register_2_DISABLE_NO_SNOOP_OFFSET      0
#define AudioAZ_Misc_Control_Register_2_DISABLE_NO_SNOOP_MASK        0x1

// Bitfield Description : 1: bit[0] of this register controls the No Snoop attribute 0: override the bit[0] setting meaning always generate No Snoop attribute on Buffer Descriptor and Data Buffer DMA
// POSSIBLE VALUES:
// 00 - Enable
// 01 - Disable
#define AudioAZ_Misc_Control_Register_2_DISABLE_NO_SNOOP_OVERRIDE_OFFSET      1
#define AudioAZ_Misc_Control_Register_2_DISABLE_NO_SNOOP_OVERRIDE_MASK        0x2

// Bitfield Description : 1: enable No Snoop request to ACPI 0: disable No snoop request to ACPI
// POSSIBLE VALUES:
// 00 - Disable
// 01 - Enable
#define AudioAZ_Misc_Control_Register_2_ENABLE_NO_SNOOP_OVERRIDE_OFFSET      2
#define AudioAZ_Misc_Control_Register_2_ENABLE_NO_SNOOP_OVERRIDE_MASK        0x4

// Bitfield Description : 1: Disable minimum retry on ALink Bus 0: Enable
// POSSIBLE VALUES:
// 00 - Enable
// 01 - Disable
#define AudioAZ_Misc_Control_Register_2_DISABLE_MIN_RETRY_OFFSET      3
#define AudioAZ_Misc_Control_Register_2_DISABLE_MIN_RETRY_MASK        0x8

// Bitfield Description : 
#define AudioAZ_Misc_Control_Register_2_Reserved_7_4_OFFSET      4
#define AudioAZ_Misc_Control_Register_2_Reserved_7_4_MASK        0xf0

typedef union {
  struct {
    UINT8                            DISABLE_NO_SNOOP:1;
    UINT8                            DISABLE_NO_SNOOP_OVERRIDE:1;
    UINT8                            ENABLE_NO_SNOOP_OVERRIDE:1;
    UINT8                            DISABLE_MIN_RETRY:1;
    UINT8                            Reserved_7_4:4;
  } Field;
  UINT8 Value;
} AudioAZ_Misc_Control_Register_2_STRUCT;

#define SMN_AudioAZ_Misc_Control_Register_2_ADDRESS    0x1200182UL


/***********************************************************
* Register Name : Audio_AZ_Link0_Capabilities
* Register Description :
* Link 0 Capabilities
* Visibility : 0x2
************************************************************/

// Bitfield Description : BCLK Capabilities (BCAP): Each bit indicates xxx MHz BCLK is supported (A value of 1 means that the corresponding frequency is supported).
// The bit assignments are as follows:-
// Bit 0: 6 MHz
// Bit 1: 12 MHz
// Bit 2: 24 MHz
// Bits 3-15 Reserved. The controller must return a value of 0 for any reserved bits.
#define Audio_AZ_Link0_Capabilities_BCLK_Capabilities_OFFSET      0
#define Audio_AZ_Link0_Capabilities_BCLK_Capabilities_MASK        0xffff

// Bitfield Description : 
#define Audio_AZ_Link0_Capabilities_Reserved_23_16_OFFSET      16
#define Audio_AZ_Link0_Capabilities_Reserved_23_16_MASK        0xff0000

// Bitfield Description : Link Number of Serial Data Out Signals (LNSDO): 00b indicates that one SDO line is supported; 01b indicates that two SDO lines are supported on this Link. Software can enable the use of striping by setting the appropriate bit in the Stream Buffer Descriptor.
// 00: 1 SDO
// 01: 2 SDOs
// 10: 4 SDOs
// 11: Reserved
#define Audio_AZ_Link0_Capabilities_Link_Number_of_Serial_Data_Out_Signals_OFFSET      24
#define Audio_AZ_Link0_Capabilities_Link_Number_of_Serial_Data_Out_Signals_MASK        0x3000000

// Bitfield Description : 
#define Audio_AZ_Link0_Capabilities_Reserved_27_26_OFFSET      26
#define Audio_AZ_Link0_Capabilities_Reserved_27_26_MASK        0xc000000

// Bitfield Description : Audio Link Type (ALT): Indicates which Link Type this link belongs to.
// 1100-1111 = Vendor-Defined Audio Link
// 0001-1011 = Reserved
// 0000 = HD Audio Link
#define Audio_AZ_Link0_Capabilities_Audio_Link_Type_OFFSET      28
#define Audio_AZ_Link0_Capabilities_Audio_Link_Type_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            BCLK_Capabilities:16;
    UINT32                            Reserved_23_16:8;
    UINT32                            Link_Number_of_Serial_Data_Out_Signals:2;
    UINT32                            Reserved_27_26:2;
    UINT32                            Audio_Link_Type:4;
  } Field;
  UINT32 Value;
} Audio_AZ_Link0_Capabilities_STRUCT;

#define SMN_Audio_AZ_Link0_Capabilities_ADDRESS    0x1200240UL


/***********************************************************
* Register Name : Audio_AZ_Link0_Control
* Register Description :
* Link 0 Control
* Visibility : 0x2
************************************************************/

// Bitfield Description : Set Clock Frequency (SCF): Indicates the frequency that software wishes the link to run at. Changing this value to a value not supported by Link Capabilities shall result in indeterminate results. The possible encodings are:
// 0000 - 6Mhz
// 0001 - 12Mhz
// 0010 - 24Mhz
// 0011-1111 -- Reserved
// The value of SCF must persist across a controller reset.
#define Audio_AZ_Link0_Control_Sec_Clock_Frequency_OFFSET      0
#define Audio_AZ_Link0_Control_Sec_Clock_Frequency_MASK        0xf

// Bitfield Description : 
#define Audio_AZ_Link0_Control_Reserved_15_4_OFFSET      4
#define Audio_AZ_Link0_Control_Reserved_15_4_MASK        0xfff0

// Bitfield Description : Set Power Active (SPA): Software sets this bit to '1' to turn the link on, and clears it to '0' when it wishes to turn the link off. When CPA matches the value of this bit, the achieved power state has been reached. Software is expected to wait for CPA to match SPA before it can program SPA again. Any deviation may result in undefined behavior.
#define Audio_AZ_Link0_Control_Set_Power_Active_OFFSET      16
#define Audio_AZ_Link0_Control_Set_Power_Active_MASK        0x10000

// Bitfield Description : 
#define Audio_AZ_Link0_Control_Reserved_22_17_OFFSET      17
#define Audio_AZ_Link0_Control_Reserved_22_17_MASK        0x7e0000

// Bitfield Description : Current Power Active (CPA): This value changes to the value set by SPA when the power of the link has reached that state. Software sets SPA, then monitors CPA to know when the link has changed state.
#define Audio_AZ_Link0_Control_Current_Power_Active_OFFSET      23
#define Audio_AZ_Link0_Control_Current_Power_Active_MASK        0x800000

// Bitfield Description : 
#define Audio_AZ_Link0_Control_Reserved_31_24_OFFSET      24
#define Audio_AZ_Link0_Control_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            Sec_Clock_Frequency:4;
    UINT32                            Reserved_15_4:12;
    UINT32                            Set_Power_Active:1;
    UINT32                            Reserved_22_17:6;
    UINT32                            Current_Power_Active:1;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} Audio_AZ_Link0_Control_STRUCT;

#define SMN_Audio_AZ_Link0_Control_ADDRESS    0x1200244UL


/***********************************************************
* Register Name : Audio_AZ_Link0_Input_Payload_Capability_L
* Register Description :
* Link0 Wall Frame Counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Input Payload Capability (LINPAY): Indicates the total input payload available on the link. This does not include bandwidth used for response. This measurement is in 16-bit word quantities per 48 kHz frame.
// 00h: 0 words
// 01h: 1 word payload
// ...
// FFh: 255h word payload
// [15:0] - 6Mhz Capability
// [31:16] - 12Mhz Capability
#define Audio_AZ_Link0_Input_Payload_Capability_L_LINPAY_Lower_OFFSET      0
#define Audio_AZ_Link0_Input_Payload_Capability_L_LINPAY_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LINPAY_Lower:32;
  } Field;
  UINT32 Value;
} Audio_AZ_Link0_Input_Payload_Capability_L_STRUCT;

#define SMN_Audio_AZ_Link0_Input_Payload_Capability_L_ADDRESS    0x1200270UL


/***********************************************************
* Register Name : Audio_AZ_Link0_Input_Payload_Capability_U
* Register Description :
* Link0 Wall Frame Counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Input Payload Capability (LINPAY): Indicates the total input payload available on the link. This does not include bandwidth used for response. This measurement is in 16-bit word quantities per 48 kHz frame.
// 00h: 0 words
// 01h: 1 word payload
// ...
// FFh: 255h word payload
// [15:0] - 24Mhz Capability
#define Audio_AZ_Link0_Input_Payload_Capability_U_LINPAY_Upper_OFFSET      0
#define Audio_AZ_Link0_Input_Payload_Capability_U_LINPAY_Upper_MASK        0xffff

// Bitfield Description : 
#define Audio_AZ_Link0_Input_Payload_Capability_U_Reserved_31_16_OFFSET      16
#define Audio_AZ_Link0_Input_Payload_Capability_U_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            LINPAY_Upper:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} Audio_AZ_Link0_Input_Payload_Capability_U_STRUCT;

#define SMN_Audio_AZ_Link0_Input_Payload_Capability_U_ADDRESS    0x1200274UL


/***********************************************************
* Register Name : Audio_AZ_Link0_Output_Payload_Capability_L
* Register Description :
* Link0 Wall Frame Counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Output Payload Capability (LOUTPAY): Indicates the total output payload available on the link at a given frequency. This does not include bandwidth used for command and control. This measurement is in 16-bit word quantities per 48 kHz frame.
// Note that this value does not reflect any bandwidth increase due to support for multiple SDO lines.
// 00h: 0 words
// 01h: 1 word payload
// ...
// FFh: 255h word payload
// [15:0] - 6Mhz Capability
// [31:16] - 12Mhz Capability
#define Audio_AZ_Link0_Output_Payload_Capability_L_LOUTPAY_Lower_OFFSET      0
#define Audio_AZ_Link0_Output_Payload_Capability_L_LOUTPAY_Lower_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LOUTPAY_Lower:32;
  } Field;
  UINT32 Value;
} Audio_AZ_Link0_Output_Payload_Capability_L_STRUCT;

#define SMN_Audio_AZ_Link0_Output_Payload_Capability_L_ADDRESS    0x1200260UL


/***********************************************************
* Register Name : Audio_AZ_Link0_Output_Payload_Capability_U
* Register Description :
* Link0 Wall Frame Counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Output Payload Capability (LOUTPAY): Indicates the total output payload available on the link at a given frequency. This does not include bandwidth used for command and control. This measurement is in 16-bit word quantities per 48 kHz frame.
// Note that this value does not reflect any bandwidth increase due to support for multiple SDO lines.
// 00h: 0 words
// 01h: 1 word payload
// ...
// FFh: 255h word payload
// [15:0] - 24Mhz Capability
#define Audio_AZ_Link0_Output_Payload_Capability_U_LOUTPAY_Upper_OFFSET      0
#define Audio_AZ_Link0_Output_Payload_Capability_U_LOUTPAY_Upper_MASK        0xffff

// Bitfield Description : 
#define Audio_AZ_Link0_Output_Payload_Capability_U_Reserved_31_16_OFFSET      16
#define Audio_AZ_Link0_Output_Payload_Capability_U_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            LOUTPAY_Upper:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} Audio_AZ_Link0_Output_Payload_Capability_U_STRUCT;

#define SMN_Audio_AZ_Link0_Output_Payload_Capability_U_ADDRESS    0x1200264UL


/***********************************************************
* Register Name : Audio_AZ_Link0_Output_Stream_ID
* Register Description :
* Link0 Output Stream ID
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define Audio_AZ_Link0_Output_Stream_ID_Reserved_0_0_OFFSET      0
#define Audio_AZ_Link0_Output_Stream_ID_Reserved_0_0_MASK        0x1

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 0001b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV1_OFFSET      1
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV1_MASK        0x2

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 0010b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV2_OFFSET      2
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV2_MASK        0x4

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 0011b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV3_OFFSET      3
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV3_MASK        0x8

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 0100b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV4_OFFSET      4
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV4_MASK        0x10

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 0101b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV5_OFFSET      5
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV5_MASK        0x20

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 0110b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV6_OFFSET      6
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV6_MASK        0x40

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 0111b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV7_OFFSET      7
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV7_MASK        0x80

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 1000b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV8_OFFSET      8
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV8_MASK        0x100

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 1001b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV9_OFFSET      9
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV9_MASK        0x200

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 1010b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV10_OFFSET      10
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV10_MASK        0x400

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 1011b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV11_OFFSET      11
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV11_MASK        0x800

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 1100b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV12_OFFSET      12
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV12_MASK        0x1000

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 1101b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV13_OFFSET      13
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV13_MASK        0x2000

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 1110b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV14_OFFSET      14
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV14_MASK        0x4000

// Bitfield Description : This link will claim / forward output cycles with Stream ID = 1111b.
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV15_OFFSET      15
#define Audio_AZ_Link0_Output_Stream_ID_L0OSIDV15_MASK        0x8000

// Bitfield Description : 
#define Audio_AZ_Link0_Output_Stream_ID_Reserved_31_16_OFFSET      16
#define Audio_AZ_Link0_Output_Stream_ID_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            Reserved_0_0:1;
    UINT32                            L0OSIDV1:1;
    UINT32                            L0OSIDV2:1;
    UINT32                            L0OSIDV3:1;
    UINT32                            L0OSIDV4:1;
    UINT32                            L0OSIDV5:1;
    UINT32                            L0OSIDV6:1;
    UINT32                            L0OSIDV7:1;
    UINT32                            L0OSIDV8:1;
    UINT32                            L0OSIDV9:1;
    UINT32                            L0OSIDV10:1;
    UINT32                            L0OSIDV11:1;
    UINT32                            L0OSIDV12:1;
    UINT32                            L0OSIDV13:1;
    UINT32                            L0OSIDV14:1;
    UINT32                            L0OSIDV15:1;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} Audio_AZ_Link0_Output_Stream_ID_STRUCT;

#define SMN_Audio_AZ_Link0_Output_Stream_ID_ADDRESS    0x1200248UL


/***********************************************************
* Register Name : Audio_AZ_Link0_Per_Stream_Overhead
* Register Description :
* Link0 Per Stream Overhead
* Visibility : 0x2
************************************************************/

// Bitfield Description : Per Stream Output Overhead (PSOO): Indicates the SDO Output Overhead on a per stream basis. This does not include bandwidth used for command and control. This measurement is in byte quantities per 48 kHz frame.
// Software calculates available link bandwidth in bytes per 48kHz frame with the following formula:
// (2 * LOUTPAY) * (NumOfStreams * LPSOO).
#define Audio_AZ_Link0_Per_Stream_Overhead_PSOO_OFFSET      0
#define Audio_AZ_Link0_Per_Stream_Overhead_PSOO_MASK        0xff

// Bitfield Description : 
#define Audio_AZ_Link0_Per_Stream_Overhead_Reserved_15_8_OFFSET      8
#define Audio_AZ_Link0_Per_Stream_Overhead_Reserved_15_8_MASK        0xff00

// Bitfield Description : Per Stream Input Overhead (PSIO): Indicates the SDI Input Overhead on a per stream basis. This does not include bandwidth used for response. This measurement is in byte quantities per 48 kHz frame.
// Software calculates available link bandwidth in bytes with the following formula:
// (2 * LINPAY) * (NumOfStreams * LPSIO).
#define Audio_AZ_Link0_Per_Stream_Overhead_PSIO_OFFSET      16
#define Audio_AZ_Link0_Per_Stream_Overhead_PSIO_MASK        0xff0000

// Bitfield Description : 
#define Audio_AZ_Link0_Per_Stream_Overhead_Reserved_31_24_OFFSET      24
#define Audio_AZ_Link0_Per_Stream_Overhead_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            PSOO:8;
    UINT32                            Reserved_15_8:8;
    UINT32                            PSIO:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} Audio_AZ_Link0_Per_Stream_Overhead_STRUCT;

#define SMN_Audio_AZ_Link0_Per_Stream_Overhead_ADDRESS    0x1200250UL


/***********************************************************
* Register Name : Audio_AZ_Link0_SDI_Identifier
* Register Description :
* Link0 SDI Identifier
* Visibility : 0x2
************************************************************/

// Bitfield Description : SDI 0 (SDIID0): This link uses SDI 0.
#define Audio_AZ_Link0_SDI_Identifier_SDIID0_OFFSET      0
#define Audio_AZ_Link0_SDI_Identifier_SDIID0_MASK        0x1

// Bitfield Description : SDI 1 (SDIID1): This link uses SDI 1.
#define Audio_AZ_Link0_SDI_Identifier_SDIID1_OFFSET      1
#define Audio_AZ_Link0_SDI_Identifier_SDIID1_MASK        0x2

// Bitfield Description : SDI 2 (SDIID2): This link uses SDI 2.
#define Audio_AZ_Link0_SDI_Identifier_SDIID2_OFFSET      2
#define Audio_AZ_Link0_SDI_Identifier_SDIID2_MASK        0x4

// Bitfield Description : SDI 3 (SDIID3): This link uses SDI 3.
#define Audio_AZ_Link0_SDI_Identifier_SDIID3_OFFSET      3
#define Audio_AZ_Link0_SDI_Identifier_SDIID3_MASK        0x8

// Bitfield Description : SDI 4 (SDIID4): This link uses SDI 4.
#define Audio_AZ_Link0_SDI_Identifier_SDIID4_OFFSET      4
#define Audio_AZ_Link0_SDI_Identifier_SDIID4_MASK        0x10

// Bitfield Description : SDI 5 (SDIID5): This link uses SDI 5.
#define Audio_AZ_Link0_SDI_Identifier_SDIID5_OFFSET      5
#define Audio_AZ_Link0_SDI_Identifier_SDIID5_MASK        0x20

// Bitfield Description : SDI 6 (SDIID6): This link uses SDI 6.
#define Audio_AZ_Link0_SDI_Identifier_SDIID6_OFFSET      6
#define Audio_AZ_Link0_SDI_Identifier_SDIID6_MASK        0x40

// Bitfield Description : SDI 7 (SDIID7): This link uses SDI 7.
#define Audio_AZ_Link0_SDI_Identifier_SDIID7_OFFSET      7
#define Audio_AZ_Link0_SDI_Identifier_SDIID7_MASK        0x80

// Bitfield Description : SDI 8 (SDIID8): This link uses SDI 8.
#define Audio_AZ_Link0_SDI_Identifier_SDIID8_OFFSET      8
#define Audio_AZ_Link0_SDI_Identifier_SDIID8_MASK        0x100

// Bitfield Description : SDI 9 (SDIID9): This link uses SDI 9.
#define Audio_AZ_Link0_SDI_Identifier_SDIID9_OFFSET      9
#define Audio_AZ_Link0_SDI_Identifier_SDIID9_MASK        0x200

// Bitfield Description : SDI 10 (SDIID10): This link uses SDI 10.
#define Audio_AZ_Link0_SDI_Identifier_SDIID10_OFFSET      10
#define Audio_AZ_Link0_SDI_Identifier_SDIID10_MASK        0x400

// Bitfield Description : SDI 11 (SDIID11): This link uses SDI 11.
#define Audio_AZ_Link0_SDI_Identifier_SDIID11_OFFSET      11
#define Audio_AZ_Link0_SDI_Identifier_SDIID11_MASK        0x800

// Bitfield Description : SDI 12 (SDIID12): This link uses SDI 12.
#define Audio_AZ_Link0_SDI_Identifier_SDIID12_OFFSET      12
#define Audio_AZ_Link0_SDI_Identifier_SDIID12_MASK        0x1000

// Bitfield Description : SDI 13 (SDIID13): This link uses SDI 13.
#define Audio_AZ_Link0_SDI_Identifier_SDIID13_OFFSET      13
#define Audio_AZ_Link0_SDI_Identifier_SDIID13_MASK        0x2000

// Bitfield Description : SDI 14 (SDIID14): This link uses SDI 14.
#define Audio_AZ_Link0_SDI_Identifier_SDIID14_OFFSET      14
#define Audio_AZ_Link0_SDI_Identifier_SDIID14_MASK        0x4000

// Bitfield Description : 
#define Audio_AZ_Link0_SDI_Identifier_Reserved_31_15_OFFSET      15
#define Audio_AZ_Link0_SDI_Identifier_Reserved_31_15_MASK        0xffff8000

typedef union {
  struct {
    UINT32                            SDIID0:1;
    UINT32                            SDIID1:1;
    UINT32                            SDIID2:1;
    UINT32                            SDIID3:1;
    UINT32                            SDIID4:1;
    UINT32                            SDIID5:1;
    UINT32                            SDIID6:1;
    UINT32                            SDIID7:1;
    UINT32                            SDIID8:1;
    UINT32                            SDIID9:1;
    UINT32                            SDIID10:1;
    UINT32                            SDIID11:1;
    UINT32                            SDIID12:1;
    UINT32                            SDIID13:1;
    UINT32                            SDIID14:1;
    UINT32                            Reserved_31_15:17;
  } Field;
  UINT32 Value;
} Audio_AZ_Link0_SDI_Identifier_STRUCT;

#define SMN_Audio_AZ_Link0_SDI_Identifier_ADDRESS    0x120024cUL


/***********************************************************
* Register Name : Audio_AZ_Link0_Wall_Frame_Counter
* Register Description :
* Link0 Wall Frame Counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : Clock in Frame (CIF): 9 bit counter that is incremented on each link BCLK period and rolls over from 499 to 0. This counter will roll over to zero with a period of 48 KHz HD Audio frame.
// With the introduction of multiple link segments for the HD Audio controller, and the capability of running each link segment at different and asynchronous clock speeds, the BCLK definition is fixed at 24 MHz equivalent rate always, independent of the physical link clock speed.
#define Audio_AZ_Link0_Wall_Frame_Counter_Clock_In_Frame_OFFSET      0
#define Audio_AZ_Link0_Wall_Frame_Counter_Clock_In_Frame_MASK        0x1ff

// Bitfield Description : 
#define Audio_AZ_Link0_Wall_Frame_Counter_Reserved_31_9_OFFSET      9
#define Audio_AZ_Link0_Wall_Frame_Counter_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            Clock_In_Frame:9;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} Audio_AZ_Link0_Wall_Frame_Counter_STRUCT;

#define SMN_Audio_AZ_Link0_Wall_Frame_Counter_ADDRESS    0x1200258UL


/***********************************************************
* Register Name : Audio_AZ_Multiple_Links_Capability_Declaration
* Register Description :
* Multiple Links Capability Declaration
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Count (LCOUNT): Indicates the number of links. Up to 15 links can be supported. A '0' indicates 1 link, and '1110' indicates 15 links. Note: '1111' is reserved. Note that this Link Count is the cumulative total number of links where the links can be heterogeneous.
#define Audio_AZ_Multiple_Links_Capability_Declaration_Link_Count_OFFSET      0
#define Audio_AZ_Multiple_Links_Capability_Declaration_Link_Count_MASK        0xf

// Bitfield Description : 
#define Audio_AZ_Multiple_Links_Capability_Declaration_Reserved_31_4_OFFSET      4
#define Audio_AZ_Multiple_Links_Capability_Declaration_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            Link_Count:4;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} Audio_AZ_Multiple_Links_Capability_Declaration_STRUCT;

#define SMN_Audio_AZ_Multiple_Links_Capability_Declaration_ADDRESS    0x1200204UL


/***********************************************************
* Register Name : Audio_AZ_Multiple_Links_Capability_Header
* Register Description :
* Multiple Links Capability Header
* Visibility : 0x2
************************************************************/

// Bitfield Description : Next Capability Pointer (PTR): This field contains the address to the next capability structure or 0000h if no other items exist in the linked list of capabilities.
#define Audio_AZ_Multiple_Links_Capability_Header_Next_Capability_Pointer_OFFSET      0
#define Audio_AZ_Multiple_Links_Capability_Header_Next_Capability_Pointer_MASK        0xffff

// Bitfield Description : Capability Identifier (ID): This field is a HD Audio Specfication defined ID number that indicates the nature and format of the capability.
#define Audio_AZ_Multiple_Links_Capability_Header_Capability_Identifier_OFFSET      16
#define Audio_AZ_Multiple_Links_Capability_Header_Capability_Identifier_MASK        0xfff0000

// Bitfield Description : Capability Version (VER): This field is a HD Audio Specfication defined version number that indicates the version of the capability structure present.
#define Audio_AZ_Multiple_Links_Capability_Header_Capability_Version_OFFSET      28
#define Audio_AZ_Multiple_Links_Capability_Header_Capability_Version_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            Next_Capability_Pointer:16;
    UINT32                            Capability_Identifier:12;
    UINT32                            Capability_Version:4;
  } Field;
  UINT32 Value;
} Audio_AZ_Multiple_Links_Capability_Header_STRUCT;

#define SMN_Audio_AZ_Multiple_Links_Capability_Header_ADDRESS    0x1200200UL


/***********************************************************
* Register Name : Audio_Az_CORB_Control
* Register Description :
* CORB Control
* Visibility : 0x2
************************************************************/

// Bitfield Description : CORB Memory Error Interrupt Enable (CMEIE):
// If this bit is set, the controller will generate and interrupt if the MEI status bit is set.
#define Audio_Az_CORB_Control_CMEIE_OFFSET      0
#define Audio_Az_CORB_Control_CMEIE_MASK        0x1

// Bitfield Description : Enable CORB DMA Engine (CORBRUN):
// 0 = DMA Stop
// 1 = DMA Run (when Read Pointer lags Write Pointer)
// Must read the value back
#define Audio_Az_CORB_Control_CORBRUN_OFFSET      1
#define Audio_Az_CORB_Control_CORBRUN_MASK        0x2

// Bitfield Description : 
#define Audio_Az_CORB_Control_Reserved_7_2_OFFSET      2
#define Audio_Az_CORB_Control_Reserved_7_2_MASK        0xfc

typedef union {
  struct {
    UINT8                            CMEIE:1;
    UINT8                            CORBRUN:1;
    UINT8                            Reserved_7_2:6;
  } Field;
  UINT8 Value;
} Audio_Az_CORB_Control_STRUCT;

#define SMN_Audio_Az_CORB_Control_ADDRESS    0x120004cUL


/***********************************************************
* Register Name : Audio_Az_CORB_Lower_Base_Address
* Register Description :
* CORB_Lower_Base_Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : CORB Lower Base Unimplemented Bits:
// Hardwired to 0. This requires the CORB to be allocated with 128-byte granularity to allow for cache line fetch optimizations.
#define Audio_Az_CORB_Lower_Base_Address_CORB_Lower_Base_Unimplemented_Bits_OFFSET      0
#define Audio_Az_CORB_Lower_Base_Address_CORB_Lower_Base_Unimplemented_Bits_MASK        0x7f

// Bitfield Description : CORB Lower Base Address (CORBLBASE):
// Lower address of the Command Output Ring Buffer, allowing the CORB Base Address to be assigned on any 1 KB boundary. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted.
#define Audio_Az_CORB_Lower_Base_Address_CORBLBASE_OFFSET      7
#define Audio_Az_CORB_Lower_Base_Address_CORBLBASE_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            CORB_Lower_Base_Unimplemented_Bits:7;
    UINT32                            CORBLBASE:25;
  } Field;
  UINT32 Value;
} Audio_Az_CORB_Lower_Base_Address_STRUCT;

#define SMN_Audio_Az_CORB_Lower_Base_Address_ADDRESS    0x1200040UL


/***********************************************************
* Register Name : Audio_Az_CORB_Read_Pointer
* Register Description :
* CORB Read Pointer
* Visibility : 0x2
************************************************************/

// Bitfield Description : CORB Read Pointer (CORBRP):
// Software reads this field to determine how many commands it can write to the CORB without over-running. The value read indicates the CORB Read Pointer offset in Dword granularity. The offset entry read from this field has been successfully fetched by the DMA controller and may be over-written by software. Supports up to 256 CORB entries (256 x 4 B = 1 KB) in the cyclic CORB buffer. This field may be read while the DMA engine is running.
#define Audio_Az_CORB_Read_Pointer_CORBRP_OFFSET      0
#define Audio_Az_CORB_Read_Pointer_CORBRP_MASK        0xff

// Bitfield Description : 
#define Audio_Az_CORB_Read_Pointer_Reserved_14_8_OFFSET      8
#define Audio_Az_CORB_Read_Pointer_Reserved_14_8_MASK        0x7f00

// Bitfield Description : CORB Read Pointer Reset (CORBRPRST):
// Software writes a 1 to this bit to reset the CORB Read Pointer to 0 and clear any residual pre-fetched commands in the CORB hardware buffer within the controller. The hardware will physically update this bit to 1 when the CORB pointer reset is complete. Software must read a 1 to verify that the reset completed correctly. Software must clear this bit back to 0, by writing a 0, and then read back the 0 to verify that the clear completed correctly. The CORB DMA engine must be stopped prior to resetting the Read Pointer or else DMA transfer may be corrupted.
#define Audio_Az_CORB_Read_Pointer_CORBRPRST_OFFSET      15
#define Audio_Az_CORB_Read_Pointer_CORBRPRST_MASK        0x8000

typedef union {
  struct {
    UINT16                            CORBRP:8;
    UINT16                            Reserved_14_8:7;
    UINT16                            CORBRPRST:1;
  } Field;
  UINT16 Value;
} Audio_Az_CORB_Read_Pointer_STRUCT;

#define SMN_Audio_Az_CORB_Read_Pointer_ADDRESS    0x120004aUL


/***********************************************************
* Register Name : Audio_Az_CORB_Size
* Register Description :
* CORB Size
* Visibility : 0x2
************************************************************/

// Bitfield Description : CORB Size (CORBSIZE): The setting of the register determines when the address counter in the DMA controller will wrap around.
// 00b ---- 8B = 2 entries
// 01b ---- 64B = 16 entries
// 10b ---- 1KB = 256 entries
// 11b ---- Reserved
// Setting this field to an unsupported size will produce unspecified results. When only one CORB Size is supported it is permissible to make this field Read Only (RO).
#define Audio_Az_CORB_Size_CORBSIZE_OFFSET      0
#define Audio_Az_CORB_Size_CORBSIZE_MASK        0x3

// Bitfield Description : 
#define Audio_Az_CORB_Size_Reserved_3_2_OFFSET      2
#define Audio_Az_CORB_Size_Reserved_3_2_MASK        0xc

// Bitfield Description : CORB Size Capability (CORBSZCAP): A bit mask indicating the sizes of the CORB supported by the controller.
// 0001b ---- 8B = 2 entries
// 0010b ---- 64B = 16 entries
// 0100b ---- 1024B = 256 Entries
// 1000b ---- Reserved
// This is implemented as a bit mask; for example, if the controller supported two entries and 256 entries, this register would have a value of 0101b.
// There is no requirement to support more than one CORB Size.
#define Audio_Az_CORB_Size_CORBSZCAP_OFFSET      4
#define Audio_Az_CORB_Size_CORBSZCAP_MASK        0xf0

typedef union {
  struct {
    UINT8                            CORBSIZE:2;
    UINT8                            Reserved_3_2:2;
    UINT8                            CORBSZCAP:4;
  } Field;
  UINT8 Value;
} Audio_Az_CORB_Size_STRUCT;

#define SMN_Audio_Az_CORB_Size_ADDRESS    0x120004eUL


/***********************************************************
* Register Name : Audio_Az_CORB_Status
* Register Description :
* CORB Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : CORB Memory Error Indication (CMEI):
// If this status bit is set, the controller has detected an error in the pathway between the controller and memory. This may be an ECC bit error or any other type of detectable data error which renders the command data fetched invalid. Writing a 1 to this bit will clear the bit, but a CRST must be performed before operation continues as this indicates a severe machine error has occurred and the current state is not trustable.
#define Audio_Az_CORB_Status_CMEI_OFFSET      0
#define Audio_Az_CORB_Status_CMEI_MASK        0x1

// Bitfield Description : 
#define Audio_Az_CORB_Status_Reserved_7_1_OFFSET      1
#define Audio_Az_CORB_Status_Reserved_7_1_MASK        0xfe

typedef union {
  struct {
    UINT8                            CMEI:1;
    UINT8                            Reserved_7_1:7;
  } Field;
  UINT8 Value;
} Audio_Az_CORB_Status_STRUCT;

#define SMN_Audio_Az_CORB_Status_ADDRESS    0x120004dUL


/***********************************************************
* Register Name : Audio_Az_CORB_Upper_Base_Address
* Register Description :
* CORB_Upper_Base_Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : CORB Upper Base Address (CORBUBASE):
// Upper 32 address bits of the Command Output Ring Buffer. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This register is Reserved, Read-only 0 if the 64OK (64-bit OK) bit indicates that the controller does not support 64-bit addressing.
#define Audio_Az_CORB_Upper_Base_Address_CORBLBASE_OFFSET      0
#define Audio_Az_CORB_Upper_Base_Address_CORBLBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CORBLBASE:32;
  } Field;
  UINT32 Value;
} Audio_Az_CORB_Upper_Base_Address_STRUCT;

#define SMN_Audio_Az_CORB_Upper_Base_Address_ADDRESS    0x1200044UL


/***********************************************************
* Register Name : Audio_Az_CORB_Write_Pointer
* Register Description :
* CORB Write Pointer
* Visibility : 0x2
************************************************************/

// Bitfield Description : CORB Write Pointer (CORBWP):
// Software writes the last valid CORB entry offset into this field in Dword granularity. The DMA engine fetches commands from the CORB until the Read Pointer matches the Write Pointer. This supports up to 256 CORB entries (256 x 4 B = 1 KB). This field may be written while the DMA engine is running.
#define Audio_Az_CORB_Write_Pointer_CORBWP_OFFSET      0
#define Audio_Az_CORB_Write_Pointer_CORBWP_MASK        0xff

// Bitfield Description : 
#define Audio_Az_CORB_Write_Pointer_Reserved_15_8_OFFSET      8
#define Audio_Az_CORB_Write_Pointer_Reserved_15_8_MASK        0xff00

typedef union {
  struct {
    UINT16                            CORBWP:8;
    UINT16                            Reserved_15_8:8;
  } Field;
  UINT16 Value;
} Audio_Az_CORB_Write_Pointer_STRUCT;

#define SMN_Audio_Az_CORB_Write_Pointer_ADDRESS    0x1200048UL


/***********************************************************
* Register Name : Audio_Az_DPLBASE
* Register Description :
* DMA Position Lower Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : DMA Position Buffer Enable: When this bit is set to a 1, the controller will write the DMA positions of each of the DMA engines to the buffer in main memory periodically. Software can use this value to know what data in memory is valid data.
// The controller must ensure that the values in the DMA Position Buffer that the software can read represent positions in the stream for which valid data exists in the Stream.s DMA buffer. This has particular relevance in systems which support isochronous transfer; the stream positions in the software-visible memory buffer must represent stream data which has reached the Global Observation point
#define Audio_Az_DPLBASE_DMA_Position_Buffer_Enable_OFFSET      0
#define Audio_Az_DPLBASE_DMA_Position_Buffer_Enable_MASK        0x1

// Bitfield Description : 
#define Audio_Az_DPLBASE_Reserved_6_1_OFFSET      1
#define Audio_Az_DPLBASE_Reserved_6_1_MASK        0x7e

// Bitfield Description : DMA Position Lower Base Address (DPLBASE): Contains the upper 25 bits of the lower 32 bits of the DMA Position Buffer Base Address. The lower 7 bits of the DMA Position Buffer Base Address are always zero and not programmable to allow for 128 byte alignment and cache line write optimizations.
// This register field must not be written when any DMA engine is running or the DMA transfer may be corrupted. This same address is used by the Flush Control, and must be programmed with a valid value before the FLCNRTL bit is set.
#define Audio_Az_DPLBASE_DPLBASE_OFFSET      7
#define Audio_Az_DPLBASE_DPLBASE_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            DMA_Position_Buffer_Enable:1;
    UINT32                            Reserved_6_1:6;
    UINT32                            DPLBASE:25;
  } Field;
  UINT32 Value;
} Audio_Az_DPLBASE_STRUCT;

#define SMN_Audio_Az_DPLBASE_ADDRESS    0x1200070UL


/***********************************************************
* Register Name : Audio_Az_DPUBASE
* Register Description :
* DMA_Position_Upper_Base_Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : DMA Position Upper Base Address (RIRBUBASE):
// Upper 32 address bits of the DMA Position Buffer Base Address. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This register is Reserved, Read-only 0 if the 64OK (64-bit OK) bit indicates that the controller does not support 64-bit addressing.
#define Audio_Az_DPUBASE_RIRBUBASE_OFFSET      0
#define Audio_Az_DPUBASE_RIRBUBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RIRBUBASE:32;
  } Field;
  UINT32 Value;
} Audio_Az_DPUBASE_STRUCT;

#define SMN_Audio_Az_DPUBASE_ADDRESS    0x1200074UL


/***********************************************************
* Register Name : Audio_Az_Global_Capabilities
* Register Description :
* Global Capabilities.
There are a maximum of 30 Streams that can be supported, of which 15 may be configured as output and 15 may be configured as input streams at any one point in time.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 64Bit Address Supported;
// A 1 indicates that 64 bit addressing is supported by the controller for BDL addresses, data buffer addresses, and command buffer addresses. A 0 indicates that only 32-bit addressing is available
#define Audio_Az_Global_Capabilities_A_640K_OFFSET      0
#define Audio_Az_Global_Capabilities_A_640K_MASK        0x1

// Bitfield Description : Number of Serial Data Out Signals;
// 00: 1 SDO
// 01: 2 SDOs
// 10: 4 SDOs
// 11:
#define Audio_Az_Global_Capabilities_NSDO_OFFSET      1
#define Audio_Az_Global_Capabilities_NSDO_MASK        0x6

// Bitfield Description : Number of Biderectional Streams Supported;
// A value of 00000b indicates that there are no Bidirectional Streams supported. A maximum of 30 bidirectional streams are supported.
// 00000b: No bidirectional streams supported
// 00001b: 1 bidirectional stream supported
// ...
// 11110b: 30 bidirectional streams supported
#define Audio_Az_Global_Capabilities_BSS_OFFSET      3
#define Audio_Az_Global_Capabilities_BSS_MASK        0xf8

// Bitfield Description : Number of Input Streams Supported;
// A value of 0000b indicates that there are no Input Streams supported.  A maximum of 15 input streams are supported.
// 0000b: No input streams supported
// 0001b: 1 input stream supported
// ...
// 1111b: 15 input streams supported
#define Audio_Az_Global_Capabilities_ISS_OFFSET      8
#define Audio_Az_Global_Capabilities_ISS_MASK        0xf00

// Bitfield Description : Number of Output Streams Supported;
// A value of 0000b indicates that there are no Output Streams supported.  A maximum of 15 output streams are supported.
// 0000b: No output streams supported
// 0001b: 1 output stream supported
// ...
// 1111b: 15 output streams supported
#define Audio_Az_Global_Capabilities_OSS_OFFSET      12
#define Audio_Az_Global_Capabilities_OSS_MASK        0xf000

typedef union {
  struct {
    UINT16                            A_640K:1;
    UINT16                            NSDO:2;
    UINT16                            BSS:5;
    UINT16                            ISS:4;
    UINT16                            OSS:4;
  } Field;
  UINT16 Value;
} Audio_Az_Global_Capabilities_STRUCT;

#define SMN_Audio_Az_Global_Capabilities_ADDRESS    0x1200000UL


/***********************************************************
* Register Name : Audio_Az_Global_Control
* Register Description :
* Global Control
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controller Reset;
// Writing a 0 to this bit causes the High Definition Audio controller to transition to the Reset state. With the exception of certain registers such as those required for Wake support, all state machines, FIFO's, and memory mapped configuration registers (not PCI Configuration Registers) in the controller will be reset. The link RESET# signal will be asserted and all other link signals will be driven to their "reset" values. After the hardware has completed sequencing into the reset state, it will report a 0 in this bit. Software must read a 0 from this bit to verify that the controller is in reset.
// Writing a 1 to this bit causes the controller to exit its Reset state and de-assert the link RESET# signal. Software is responsible for setting/clearing this bit such that the minimum link RESET# signal assertion pulse width specification is met.
// When the controller hardware is ready to begin operation, it will report a 1 in this bit. Software must read a 1 from this bit before accessing any controller registers.  The CRST# bit defaults to a 0 after hardware reset, therefore software needs to write a 1 to this bit to begin operation.
// Note that the CORB/RIRB RUN bits and all Stream RUN bits must be verified cleared to 0 before CRST# is written to 0 (asserted) in order to assure a clean re - start.
// When CRST is 0 indicating that the controller is in reset, most registers will return their default values on reads, and writes will have no effect. The exceptions are the WAKEEN and STATESTS registers, which are only cleared on power -on reset, and the CRST bit itself, which will cause the controller to leave the reset state when a 1 is written to it.
#define Audio_Az_Global_Control_CRST_OFFSET      0
#define Audio_Az_Global_Control_CRST_MASK        0x1

// Bitfield Description : Flush Control;
// Writing a 1 to this bit initiates a flush. The flush is complete when Flush Status is set. Before a flush cycle is initiated, the DMA Position Buffer must be programmed with a valid memory address by software, but the DMA Position Buffer bit 0 need not be set to enable the position reporting mechanism. Also, all streams must be stopped (the associated RUN bit must be 0).
// When the flush is initiated, the controller will flush pipelines to memory to ensure that the hardware is ready to transition to a D3 state. Setting this bit is not a critical step in the power state transition if the content of the FIFOs is not critical.
#define Audio_Az_Global_Control_FCNTRL_OFFSET      1
#define Audio_Az_Global_Control_FCNTRL_MASK        0x2

// Bitfield Description : 
#define Audio_Az_Global_Control_Reserved_7_2_OFFSET      2
#define Audio_Az_Global_Control_Reserved_7_2_MASK        0xfc

// Bitfield Description : Accept Unsolicited Response Enable;
// If UNSOL is a 1, Unsolicited Responses from the codecs are accepted by the controller and placed into the Response Input Ring Buffer. If UNSOL is a 0, unsolicited responses are not accepted and dropped on the floor.
#define Audio_Az_Global_Control_UNSOL_OFFSET      8
#define Audio_Az_Global_Control_UNSOL_MASK        0x100

// Bitfield Description : 
#define Audio_Az_Global_Control_Reserved_31_9_OFFSET      9
#define Audio_Az_Global_Control_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            CRST:1;
    UINT32                            FCNTRL:1;
    UINT32                            Reserved_7_2:6;
    UINT32                            UNSOL:1;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} Audio_Az_Global_Control_STRUCT;

#define SMN_Audio_Az_Global_Control_ADDRESS    0x1200008UL


/***********************************************************
* Register Name : Audio_Az_Global_Status
* Register Description :
* Global Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define Audio_Az_Global_Status_Reserved_0_0_OFFSET      0
#define Audio_Az_Global_Status_Reserved_0_0_MASK        0x1

// Bitfield Description : Flush Status;
// This bit is set to a 1 by the hardware to indicate that the flush cycle initiated when the FCNTRL bit was set has completed. Software must write a 1 to clear this bit before the next time FCNTRL is set to clear the bit.
#define Audio_Az_Global_Status_FSTS_OFFSET      1
#define Audio_Az_Global_Status_FSTS_MASK        0x2

// Bitfield Description : 
#define Audio_Az_Global_Status_Reserved_15_2_OFFSET      2
#define Audio_Az_Global_Status_Reserved_15_2_MASK        0xfffc

typedef union {
  struct {
    UINT16                            Reserved_0_0:1;
    UINT16                            FSTS:1;
    UINT16                            Reserved_15_2:14;
  } Field;
  UINT16 Value;
} Audio_Az_Global_Status_STRUCT;

#define SMN_Audio_Az_Global_Status_ADDRESS    0x1200010UL


/***********************************************************
* Register Name : Audio_Az_Immediate_Command_Output_Interface
* Register Description :
* Immediate Command Output Interface
* Visibility : 0x2
************************************************************/

// Bitfield Description : Immediate Command Write (ICW):
// The value written into this register is used as the verb to be sent out over the link when the ICB (ICS bit 0) is set to one (1). Software must ensure that the ICB bit in the Immediate Command Status register is clear before writing a value into this register or undefined behavior will result
#define Audio_Az_Immediate_Command_Output_Interface_ICW_OFFSET      0
#define Audio_Az_Immediate_Command_Output_Interface_ICW_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            ICW:32;
  } Field;
  UINT32 Value;
} Audio_Az_Immediate_Command_Output_Interface_STRUCT;

#define SMN_Audio_Az_Immediate_Command_Output_Interface_ADDRESS    0x1200060UL


/***********************************************************
* Register Name : Audio_Az_Immediate_Command_Status
* Register Description :
* Immediate Response Input Interface
* Visibility : 0x2
************************************************************/

// Bitfield Description : Immediate Command Busy (ICB):
// This bit is a 0 when the controller can accept an immediate command. Software must wait for this bit to be 0 before writing a value in the ICW register and may write this bit to a 0 if the bit fails to return to 0 after a reasonable timeout period. Writing to 0 is not permissible if the CORB is active.
// This bit will be clear (indicating "ready") when the following conditions are met: (1) the link is running, (2) the CORB is not active (CORBRP = CORBWP or CORBEN is not set), and (3) there is not an immediate command already in the queue waiting to be sent.
// Writing this bit to 1 will cause the contents of the ICW register to be sent as a verb in the next frame. Once a response is received the IRV bit will be set and this bit will be cleared indicating ready to transmit another verb.
#define Audio_Az_Immediate_Command_Status_ICB_OFFSET      0
#define Audio_Az_Immediate_Command_Status_ICB_MASK        0x1

// Bitfield Description : Immediate Result Valid (IRV):
// This bit is set to a 1 by hardware when a new response is latched into the IRR register. Software must clear this bit before issuing a new command by writing a one to it so that the software may determine when a new response has arrived.
#define Audio_Az_Immediate_Command_Status_IRV_OFFSET      1
#define Audio_Az_Immediate_Command_Status_IRV_MASK        0x2

// Bitfield Description : Immediate Command Version:
// Indicates if the IRRADD field and IRRUNSOL bit are implemented. If ICVER is 0 then the IRRADD and IRRUNSOL are reserved. If ICVER is 1 then both IRRADD and IRRUNSOL are implemented.
#define Audio_Az_Immediate_Command_Status_Immediate_Command_Version_OFFSET      2
#define Audio_Az_Immediate_Command_Status_Immediate_Command_Version_MASK        0x4

// Bitfield Description : Immediate Response Result Unsolicited (IRRUNSOL):
// Indicates whether the response latched in the Immediate Response Input register is a solicited or unsolicited response. This bit is optional.
#define Audio_Az_Immediate_Command_Status_IRRUNSOL_OFFSET      3
#define Audio_Az_Immediate_Command_Status_IRRUNSOL_MASK        0x8

// Bitfield Description : Immediate Response Result Address (IRRADD):
// The address of the codec which sent the response currently latched into the Immediate Response Input register. This field is optional.
#define Audio_Az_Immediate_Command_Status_IRRADD_OFFSET      4
#define Audio_Az_Immediate_Command_Status_IRRADD_MASK        0xf0

// Bitfield Description : 
#define Audio_Az_Immediate_Command_Status_Reserved_15_8_OFFSET      8
#define Audio_Az_Immediate_Command_Status_Reserved_15_8_MASK        0xff00

typedef union {
  struct {
    UINT16                            ICB:1;
    UINT16                            IRV:1;
    UINT16                            Immediate_Command_Version:1;
    UINT16                            IRRUNSOL:1;
    UINT16                            IRRADD:4;
    UINT16                            Reserved_15_8:8;
  } Field;
  UINT16 Value;
} Audio_Az_Immediate_Command_Status_STRUCT;

#define SMN_Audio_Az_Immediate_Command_Status_ADDRESS    0x1200068UL


/***********************************************************
* Register Name : Audio_Az_Immediate_Response_Input_Interface
* Register Description :
* Immediate Response Input Interface
* Visibility : 0x2
************************************************************/

// Bitfield Description : Immediate Response Read (IRR):
// The value in this register latches the last response to come in over the link.
// If multiple codecs responded in the same frame, which one of the responses that will be saved is indeterminate. The codec.s address for the response that was latched is indicated in the ICRADD field of the Immediate Command Status register if the ICRADD field is implemented.
// Note that there is no defined usage for SW to write to this register, and therefore it is recommended to be implemented as RO attribute. RW attribute is kept as an option for compatible with earlier specification definition.
#define Audio_Az_Immediate_Response_Input_Interface_IRR_OFFSET      0
#define Audio_Az_Immediate_Response_Input_Interface_IRR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IRR:32;
  } Field;
  UINT32 Value;
} Audio_Az_Immediate_Response_Input_Interface_STRUCT;

#define SMN_Audio_Az_Immediate_Response_Input_Interface_ADDRESS    0x1200064UL


/***********************************************************
* Register Name : Audio_Az_Input_Payload_Capability
* Register Description :
* Input Payload Capability
* Visibility : 0x2
************************************************************/

// Bitfield Description : Input Payload Capability;
// Indicates the total input payload available on the link. This does not include bandwidth used for command and control.  This measurement is in 16-bit Word quantities per 48-kHz frame. The default link clock speed of 24.000 MHz provides 500 bits per frame, or 31.25 Words. 36 bits (2.25 Words) are used for command and control, leaving 29 Words for payload.  This measurement is on a per-codec basis.
// 00h: 0 Words
// 01h: 1 Word payload
// ...
// FFh: 255h Word payload
#define Audio_Az_Input_Payload_Capability_INPAY_OFFSET      0
#define Audio_Az_Input_Payload_Capability_INPAY_MASK        0xffff

typedef union {
  struct {
    UINT16                            INPAY:16;
  } Field;
  UINT16 Value;
} Audio_Az_Input_Payload_Capability_STRUCT;

#define SMN_Audio_Az_Input_Payload_Capability_ADDRESS    0x1200006UL


/***********************************************************
* Register Name : Audio_Az_Input_SD0BDPL
* Register Description :
* Input Stream Descriptor 0 BDL Pointer Lower Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define Audio_Az_Input_SD0BDPL_Reserved_6_0_OFFSET      0
#define Audio_Az_Input_SD0BDPL_Reserved_6_0_MASK        0x7f

// Bitfield Description : Buffer Descriptor List Lower Base Address (BDLLBASE):
// Lower address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Input_SD0BDPL_BDLLBASE_OFFSET      7
#define Audio_Az_Input_SD0BDPL_BDLLBASE_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            BDLLBASE:25;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD0BDPL_STRUCT;

#define SMN_Audio_Az_Input_SD0BDPL_ADDRESS    0x1200098UL


/***********************************************************
* Register Name : Audio_Az_Input_SD0BDPU
* Register Description :
* Input Stream Descriptor 0 BDL Pointer Upper Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : Buffer Descriptor List Upper Base Address (BDLUBASE):
// Upper 32-bit address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0. This register is Reserved, Read-only 0 if the 64OK (64-bit OK) bit indicates that the controller does not support 64-bit addressing.
#define Audio_Az_Input_SD0BDPU_BDLUBASE_OFFSET      0
#define Audio_Az_Input_SD0BDPU_BDLUBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BDLUBASE:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD0BDPU_STRUCT;

#define SMN_Audio_Az_Input_SD0BDPU_ADDRESS    0x120009cUL


/***********************************************************
* Register Name : Audio_Az_Input_SD0CBL
* Register Description :
* Input Stream Descriptor 0 Cyclic Buffer Length
* Visibility : 0x2
************************************************************/

// Bitfield Description : Cyclic Buffer Length (CBL):
// Indicates the number of bytes in the complete cyclic buffer. Link Position in Buffer (SDnLPIB) will be reset when it reaches this value.
// Software may only write to this register after Global Reset, Controller Reset, or Stream Reset has occurred. Once the RUN bit has been set to enable the engine, software must not write to this register until after the next reset is asserted, or undefined events will occur.
// CBL must represent an integer number of samples. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Input_SD0CBL_CBL_OFFSET      0
#define Audio_Az_Input_SD0CBL_CBL_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CBL:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD0CBL_STRUCT;

#define SMN_Audio_Az_Input_SD0CBL_ADDRESS    0x1200088UL


/***********************************************************
* Register Name : Audio_Az_Input_SD0FIFOS
* Register Description :
* Input Stream Descriptor 0 FIFO Size
* Visibility : 0x2
************************************************************/

// Bitfield Description : FIFO Size (FIFOS):
// Indicates the maximum number of bytes that could be fetched by the controller at one time. This is the maximum number of bytes that may have been DMA.d into memory but not yet transmitted on the link, and is also the maximum possible value that the LPIB count will increase by at one time. This number may be static to indicate a static buffer size, or may change after the data format has been programmed if the controller is able to vary its FIFO size based on the stream format. If it is able to change value after the data format has been programmed, the value update must happen immediately before the next read of the FIFOS register, and remain static until the next programming of data format.
#define Audio_Az_Input_SD0FIFOS_FIFOS_OFFSET      0
#define Audio_Az_Input_SD0FIFOS_FIFOS_MASK        0xffff

typedef union {
  struct {
    UINT16                            FIFOS:16;
  } Field;
  UINT16 Value;
} Audio_Az_Input_SD0FIFOS_STRUCT;

#define SMN_Audio_Az_Input_SD0FIFOS_ADDRESS    0x1200090UL


/***********************************************************
* Register Name : Audio_Az_Input_SD0FMT
* Register Description :
* Input Stream Descriptor 0 Format
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of Channels (CHAN):
// Number of channels for this stream in each "sample block" of the "packets" in each "frame" on the link.
// 0000 = 1
// 0001 = 2
// ...
// 1111 = 16
#define Audio_Az_Input_SD0FMT_CHAN_OFFSET      0
#define Audio_Az_Input_SD0FMT_CHAN_MASK        0xf

// Bitfield Description : Bits per Sample (BITS):
// 000 = 8 bits. The data will be packed in memory in 8-bit containers on 16-bit boundaries.
// 001 = 16 bits. The data will be packed in memory in 16-bit containers on 16-bit boundaries.
// 010 = 20 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 011 = 24 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 100 = 32 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 101-111 = Reserved
#define Audio_Az_Input_SD0FMT_BITS_OFFSET      4
#define Audio_Az_Input_SD0FMT_BITS_MASK        0x70

// Bitfield Description : 
#define Audio_Az_Input_SD0FMT_Reserved_7_7_OFFSET      7
#define Audio_Az_Input_SD0FMT_Reserved_7_7_MASK        0x80

// Bitfield Description : Sample Base Rate Divisor (DIV):
// 000 = Divide by 1 (48 kHz, 44.1 kHz)
// 001 = Divide by 2 (24 kHz, 22.05 kHz)
// 010 = Divide by 3 (16 kHz, 32 kHz)
// 011 = Divide by 4 (11.025 kHz)
// 100 = Divide by 5 (9.6 kHz)
// 101 = Divide by 6 (8 kHz)
// 110 = Divide by 7
// 111 = Divide by 8 (6 kHz)
#define Audio_Az_Input_SD0FMT_DIV_OFFSET      8
#define Audio_Az_Input_SD0FMT_DIV_MASK        0x700

// Bitfield Description : 
#define Audio_Az_Input_SD0FMT_Reserved_13_11_OFFSET      11
#define Audio_Az_Input_SD0FMT_Reserved_13_11_MASK        0x3800

// Bitfield Description : Sample Base Rate (BASE):
// 0 = 48 kHz
// 1 = 44.1 kHz
#define Audio_Az_Input_SD0FMT_MULT_OFFSET      14
#define Audio_Az_Input_SD0FMT_MULT_MASK        0x4000

// Bitfield Description : 
#define Audio_Az_Input_SD0FMT_Reserved_15_15_OFFSET      15
#define Audio_Az_Input_SD0FMT_Reserved_15_15_MASK        0x8000

typedef union {
  struct {
    UINT16                            CHAN:4;
    UINT16                            Reserved_BITS:3;
    UINT16                            Reserved_7_7:1;
    UINT16                            DIV:3;
    UINT16                            Reserved_13_11:3;
    UINT16                            MULT:1;
    UINT16                            Reserved_15_15:1;
  } Field;
  UINT16 Value;
} Audio_Az_Input_SD0FMT_STRUCT;

#define SMN_Audio_Az_Input_SD0FMT_ADDRESS    0x1200092UL


/***********************************************************
* Register Name : Audio_Az_Input_SD0LICBA
* Register Description :
* Input Stream Descriptor 0 Link Position in Buffer Alias
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer n Alias (LPIBA):
// An alias of the Link Position In Buffer register for each Stream Descriptor. This is an alias of the counter register and behaves exactly the same as if the Link Position register were being read directly.
#define Audio_Az_Input_SD0LICBA_LPIBA_OFFSET      0
#define Audio_Az_Input_SD0LICBA_LPIBA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIBA:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD0LICBA_STRUCT;

#define SMN_Audio_Az_Input_SD0LICBA_ADDRESS    0x1202084UL


/***********************************************************
* Register Name : Audio_Az_Input_SD0LPIB
* Register Description :
* Input Stream Descriptor 0 Link Position in Buffer
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer (LPIB):
// Indicates the number of bytes that have been received off the link. Since this register reflects the number of bytes that have been received into the current buffer, for the first buffer SDnLPIB will count from 0 to the value in the Cyclic Buffer Length (SDnCBL) register, inclusive. For subsequent buffers, SNnLPIB will count from a value of 1 to the value in the Cyclic Buffer Length register, inclusive
#define Audio_Az_Input_SD0LPIB_LPIB_OFFSET      0
#define Audio_Az_Input_SD0LPIB_LPIB_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIB:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD0LPIB_STRUCT;

#define SMN_Audio_Az_Input_SD0LPIB_ADDRESS    0x1200084UL


/***********************************************************
* Register Name : Audio_Az_Input_SD0LVI
* Register Description :
* Input Stream Descriptor 0 Last Valid Index
* Visibility : 0x2
************************************************************/

// Bitfield Description : Last Valid Index (LVI): The value written to this register indicates the index for the last valid Buffer Descriptor in the BDL. After the controller has processed this descriptor, it will wrap back to the first descriptor in the list on continue processing.
// LVI must be at least 1; i.e., there must be at least two valid entries in the buffer descriptor list before DMA operations can begin.
// This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Input_SD0LVI_LVI_OFFSET      0
#define Audio_Az_Input_SD0LVI_LVI_MASK        0xff

// Bitfield Description : 
#define Audio_Az_Input_SD0LVI_Reserved_15_8_OFFSET      8
#define Audio_Az_Input_SD0LVI_Reserved_15_8_MASK        0xff00

typedef union {
  struct {
    UINT16                            LVI:8;
    UINT16                            Reserved_15_8:8;
  } Field;
  UINT16 Value;
} Audio_Az_Input_SD0LVI_STRUCT;

#define SMN_Audio_Az_Input_SD0LVI_ADDRESS    0x120008cUL


/***********************************************************
* Register Name : Audio_Az_Input_SD1BDPL
* Register Description :
* Input Stream Descriptor 1 BDL Pointer Lower Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define Audio_Az_Input_SD1BDPL_Reserved_6_0_OFFSET      0
#define Audio_Az_Input_SD1BDPL_Reserved_6_0_MASK        0x7f

// Bitfield Description : Buffer Descriptor List Lower Base Address (BDLLBASE):
// Lower address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Input_SD1BDPL_BDLLBASE_OFFSET      7
#define Audio_Az_Input_SD1BDPL_BDLLBASE_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            BDLLBASE:25;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD1BDPL_STRUCT;

#define SMN_Audio_Az_Input_SD1BDPL_ADDRESS    0x12000b8UL


/***********************************************************
* Register Name : Audio_Az_Input_SD1BDPU
* Register Description :
* Input Stream Descriptor 1 BDL Pointer Upper Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : Buffer Descriptor List Upper Base Address (BDLUBASE):
// Upper 32-bit address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0. This register is Reserved, Read-only 0 if the 64OK (64-bit OK) bit indicates that the controller does not support 64-bit addressing.
#define Audio_Az_Input_SD1BDPU_BDLUBASE_OFFSET      0
#define Audio_Az_Input_SD1BDPU_BDLUBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BDLUBASE:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD1BDPU_STRUCT;

#define SMN_Audio_Az_Input_SD1BDPU_ADDRESS    0x12000bcUL


/***********************************************************
* Register Name : Audio_Az_Input_SD1CBL
* Register Description :
* Input Stream Descriptor 1 Cyclic Buffer Length
* Visibility : 0x2
************************************************************/

// Bitfield Description : Cyclic Buffer Length (CBL):
// Indicates the number of bytes in the complete cyclic buffer. Link Position in Buffer (SDnLPIB) will be reset when it reaches this value.
// Software may only write to this register after Global Reset, Controller Reset, or Stream Reset has occurred. Once the RUN bit has been set to enable the engine, software must not write to this register until after the next reset is asserted, or undefined events will occur.
// CBL must represent an integer number of samples. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Input_SD1CBL_CBL_OFFSET      0
#define Audio_Az_Input_SD1CBL_CBL_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CBL:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD1CBL_STRUCT;

#define SMN_Audio_Az_Input_SD1CBL_ADDRESS    0x12000a8UL


/***********************************************************
* Register Name : Audio_Az_Input_SD1FIFOS
* Register Description :
* Input Stream Descriptor 1 FIFO Size
* Visibility : 0x2
************************************************************/

// Bitfield Description : FIFO Size (FIFOS):
// Indicates the maximum number of bytes that could be fetched by the controller at one time. This is the maximum number of bytes that may have been DMA.d into memory but not yet transmitted on the link, and is also the maximum possible value that the LPIB count will increase by at one time. This number may be static to indicate a static buffer size, or may change after the data format has been programmed if the controller is able to vary its FIFO size based on the stream format. If it is able to change value after the data format has been programmed, the value update must happen immediately before the next read of the FIFOS register, and remain static until the next programming of data format.
#define Audio_Az_Input_SD1FIFOS_FIFOS_OFFSET      0
#define Audio_Az_Input_SD1FIFOS_FIFOS_MASK        0xffff

typedef union {
  struct {
    UINT16                            FIFOS:16;
  } Field;
  UINT16 Value;
} Audio_Az_Input_SD1FIFOS_STRUCT;

#define SMN_Audio_Az_Input_SD1FIFOS_ADDRESS    0x12000b0UL


/***********************************************************
* Register Name : Audio_Az_Input_SD1FMT
* Register Description :
* Input Stream Descriptor 1 Format
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of Channels (CHAN):
// Number of channels for this stream in each "sample block" of the "packets" in each "frame" on the link.
// 0000 = 1
// 0001 = 2
// ...
// 1111 = 16
#define Audio_Az_Input_SD1FMT_CHAN_OFFSET      0
#define Audio_Az_Input_SD1FMT_CHAN_MASK        0xf

// Bitfield Description : Bits per Sample (BITS):
// 000 = 8 bits. The data will be packed in memory in 8-bit containers on 16-bit boundaries.
// 001 = 16 bits. The data will be packed in memory in 16-bit containers on 16-bit boundaries.
// 010 = 20 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 011 = 24 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 100 = 32 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 101-111 = Reserved
#define Audio_Az_Input_SD1FMT_BITS_OFFSET      4
#define Audio_Az_Input_SD1FMT_BITS_MASK        0x70

// Bitfield Description : 
#define Audio_Az_Input_SD1FMT_Reserved_7_7_OFFSET      7
#define Audio_Az_Input_SD1FMT_Reserved_7_7_MASK        0x80

// Bitfield Description : Sample Base Rate Divisor (DIV):
// 000 = Divide by 1 (48 kHz, 44.1 kHz)
// 001 = Divide by 2 (24 kHz, 22.05 kHz)
// 010 = Divide by 3 (16 kHz, 32 kHz)
// 011 = Divide by 4 (11.025 kHz)
// 100 = Divide by 5 (9.6 kHz)
// 101 = Divide by 6 (8 kHz)
// 110 = Divide by 7
// 111 = Divide by 8 (6 kHz)
#define Audio_Az_Input_SD1FMT_DIV_OFFSET      8
#define Audio_Az_Input_SD1FMT_DIV_MASK        0x700

// Bitfield Description : 
#define Audio_Az_Input_SD1FMT_Reserved_13_11_OFFSET      11
#define Audio_Az_Input_SD1FMT_Reserved_13_11_MASK        0x3800

// Bitfield Description : Sample Base Rate (BASE):
// 0 = 48 kHz
// 1 = 44.1 kHz
#define Audio_Az_Input_SD1FMT_MULT_OFFSET      14
#define Audio_Az_Input_SD1FMT_MULT_MASK        0x4000

// Bitfield Description : 
#define Audio_Az_Input_SD1FMT_Reserved_15_15_OFFSET      15
#define Audio_Az_Input_SD1FMT_Reserved_15_15_MASK        0x8000

typedef union {
  struct {
    UINT16                            CHAN:4;
    UINT16                            Reserved_BITS:3;
    UINT16                            Reserved_7_7:1;
    UINT16                            DIV:3;
    UINT16                            Reserved_13_11:3;
    UINT16                            MULT:1;
    UINT16                            Reserved_15_15:1;
  } Field;
  UINT16 Value;
} Audio_Az_Input_SD1FMT_STRUCT;

#define SMN_Audio_Az_Input_SD1FMT_ADDRESS    0x12000b2UL


/***********************************************************
* Register Name : Audio_Az_Input_SD1LICBA
* Register Description :
* Input Stream Descriptor 1 Link Position in Buffer Alias
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer n Alias (LPIBA):
// An alias of the Link Position In Buffer register for each Stream Descriptor. This is an alias of the counter register and behaves exactly the same as if the Link Position register were being read directly.
#define Audio_Az_Input_SD1LICBA_LPIBA_OFFSET      0
#define Audio_Az_Input_SD1LICBA_LPIBA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIBA:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD1LICBA_STRUCT;

#define SMN_Audio_Az_Input_SD1LICBA_ADDRESS    0x12020a4UL


/***********************************************************
* Register Name : Audio_Az_Input_SD1LPIB
* Register Description :
* Input Stream Descriptor 1 Link Position in Buffer
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer (LPIB):
// Indicates the number of bytes that have been received off the link. Since this register reflects the number of bytes that have been received into the current buffer, for the first buffer SDnLPIB will count from 0 to the value in the Cyclic Buffer Length (SDnCBL) register, inclusive. For subsequent buffers, SNnLPIB will count from a value of 1 to the value in the Cyclic Buffer Length register, inclusive
#define Audio_Az_Input_SD1LPIB_LPIB_OFFSET      0
#define Audio_Az_Input_SD1LPIB_LPIB_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIB:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD1LPIB_STRUCT;

#define SMN_Audio_Az_Input_SD1LPIB_ADDRESS    0x12000a4UL


/***********************************************************
* Register Name : Audio_Az_Input_SD1LVI
* Register Description :
* Input Stream Descriptor 1 Last Valid Index
* Visibility : 0x2
************************************************************/

// Bitfield Description : Last Valid Index (LVI): The value written to this register indicates the index for the last valid Buffer Descriptor in the BDL. After the controller has processed this descriptor, it will wrap back to the first descriptor in the list on continue processing.
// LVI must be at least 1; i.e., there must be at least two valid entries in the buffer descriptor list before DMA operations can begin.
// This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Input_SD1LVI_LVI_OFFSET      0
#define Audio_Az_Input_SD1LVI_LVI_MASK        0xff

// Bitfield Description : 
#define Audio_Az_Input_SD1LVI_Reserved_15_8_OFFSET      8
#define Audio_Az_Input_SD1LVI_Reserved_15_8_MASK        0xff00

typedef union {
  struct {
    UINT16                            LVI:8;
    UINT16                            Reserved_15_8:8;
  } Field;
  UINT16 Value;
} Audio_Az_Input_SD1LVI_STRUCT;

#define SMN_Audio_Az_Input_SD1LVI_ADDRESS    0x12000acUL


/***********************************************************
* Register Name : Audio_Az_Input_SD2BDPL
* Register Description :
* Input Stream Descriptor 2 BDL Pointer Lower Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define Audio_Az_Input_SD2BDPL_Reserved_6_0_OFFSET      0
#define Audio_Az_Input_SD2BDPL_Reserved_6_0_MASK        0x7f

// Bitfield Description : Buffer Descriptor List Lower Base Address (BDLLBASE):
// Lower address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Input_SD2BDPL_BDLLBASE_OFFSET      7
#define Audio_Az_Input_SD2BDPL_BDLLBASE_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            BDLLBASE:25;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD2BDPL_STRUCT;

#define SMN_Audio_Az_Input_SD2BDPL_ADDRESS    0x12000d8UL


/***********************************************************
* Register Name : Audio_Az_Input_SD2BDPU
* Register Description :
* Input Stream Descriptor 2 BDL Pointer Upper Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : Buffer Descriptor List Upper Base Address (BDLUBASE):
// Upper 32-bit address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0. This register is Reserved, Read-only 0 if the 64OK (64-bit OK) bit indicates that the controller does not support 64-bit addressing.
#define Audio_Az_Input_SD2BDPU_BDLUBASE_OFFSET      0
#define Audio_Az_Input_SD2BDPU_BDLUBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BDLUBASE:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD2BDPU_STRUCT;

#define SMN_Audio_Az_Input_SD2BDPU_ADDRESS    0x12000dcUL


/***********************************************************
* Register Name : Audio_Az_Input_SD2CBL
* Register Description :
* Input Stream Descriptor 2 Cyclic Buffer Length
* Visibility : 0x2
************************************************************/

// Bitfield Description : Cyclic Buffer Length (CBL):
// Indicates the number of bytes in the complete cyclic buffer. Link Position in Buffer (SDnLPIB) will be reset when it reaches this value.
// Software may only write to this register after Global Reset, Controller Reset, or Stream Reset has occurred. Once the RUN bit has been set to enable the engine, software must not write to this register until after the next reset is asserted, or undefined events will occur.
// CBL must represent an integer number of samples. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Input_SD2CBL_CBL_OFFSET      0
#define Audio_Az_Input_SD2CBL_CBL_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CBL:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD2CBL_STRUCT;

#define SMN_Audio_Az_Input_SD2CBL_ADDRESS    0x12000c8UL


/***********************************************************
* Register Name : Audio_Az_Input_SD2FIFOS
* Register Description :
* Input Stream Descriptor 2 FIFO Size
* Visibility : 0x2
************************************************************/

// Bitfield Description : FIFO Size (FIFOS):
// Indicates the maximum number of bytes that could be fetched by the controller at one time. This is the maximum number of bytes that may have been DMA.d into memory but not yet transmitted on the link, and is also the maximum possible value that the LPIB count will increase by at one time. This number may be static to indicate a static buffer size, or may change after the data format has been programmed if the controller is able to vary its FIFO size based on the stream format. If it is able to change value after the data format has been programmed, the value update must happen immediately before the next read of the FIFOS register, and remain static until the next programming of data format.
#define Audio_Az_Input_SD2FIFOS_FIFOS_OFFSET      0
#define Audio_Az_Input_SD2FIFOS_FIFOS_MASK        0xffff

typedef union {
  struct {
    UINT16                            FIFOS:16;
  } Field;
  UINT16 Value;
} Audio_Az_Input_SD2FIFOS_STRUCT;

#define SMN_Audio_Az_Input_SD2FIFOS_ADDRESS    0x12000d0UL


/***********************************************************
* Register Name : Audio_Az_Input_SD2FMT
* Register Description :
* Input Stream Descriptor 2 Format
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of Channels (CHAN):
// Number of channels for this stream in each "sample block" of the "packets" in each "frame" on the link.
// 0000 = 1
// 0001 = 2
// ...
// 1111 = 16
#define Audio_Az_Input_SD2FMT_CHAN_OFFSET      0
#define Audio_Az_Input_SD2FMT_CHAN_MASK        0xf

// Bitfield Description : Bits per Sample (BITS):
// 000 = 8 bits. The data will be packed in memory in 8-bit containers on 16-bit boundaries.
// 001 = 16 bits. The data will be packed in memory in 16-bit containers on 16-bit boundaries.
// 010 = 20 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 011 = 24 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 100 = 32 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 101-111 = Reserved
#define Audio_Az_Input_SD2FMT_BITS_OFFSET      4
#define Audio_Az_Input_SD2FMT_BITS_MASK        0x70

// Bitfield Description : 
#define Audio_Az_Input_SD2FMT_Reserved_7_7_OFFSET      7
#define Audio_Az_Input_SD2FMT_Reserved_7_7_MASK        0x80

// Bitfield Description : Sample Base Rate Divisor (DIV):
// 000 = Divide by 1 (48 kHz, 44.1 kHz)
// 001 = Divide by 2 (24 kHz, 22.05 kHz)
// 010 = Divide by 3 (16 kHz, 32 kHz)
// 011 = Divide by 4 (11.025 kHz)
// 100 = Divide by 5 (9.6 kHz)
// 101 = Divide by 6 (8 kHz)
// 110 = Divide by 7
// 111 = Divide by 8 (6 kHz)
#define Audio_Az_Input_SD2FMT_DIV_OFFSET      8
#define Audio_Az_Input_SD2FMT_DIV_MASK        0x700

// Bitfield Description : 
#define Audio_Az_Input_SD2FMT_Reserved_13_11_OFFSET      11
#define Audio_Az_Input_SD2FMT_Reserved_13_11_MASK        0x3800

// Bitfield Description : Sample Base Rate (BASE):
// 0 = 48 kHz
// 1 = 44.1 kHz
#define Audio_Az_Input_SD2FMT_MULT_OFFSET      14
#define Audio_Az_Input_SD2FMT_MULT_MASK        0x4000

// Bitfield Description : 
#define Audio_Az_Input_SD2FMT_Reserved_15_15_OFFSET      15
#define Audio_Az_Input_SD2FMT_Reserved_15_15_MASK        0x8000

typedef union {
  struct {
    UINT16                            CHAN:4;
    UINT16                            Reserved_BITS:3;
    UINT16                            Reserved_7_7:1;
    UINT16                            DIV:3;
    UINT16                            Reserved_13_11:3;
    UINT16                            MULT:1;
    UINT16                            Reserved_15_15:1;
  } Field;
  UINT16 Value;
} Audio_Az_Input_SD2FMT_STRUCT;

#define SMN_Audio_Az_Input_SD2FMT_ADDRESS    0x12000d2UL


/***********************************************************
* Register Name : Audio_Az_Input_SD2LICBA
* Register Description :
* Input Stream Descriptor 2 Link Position in Buffer Alias
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer n Alias (LPIBA):
// An alias of the Link Position In Buffer register for each Stream Descriptor. This is an alias of the counter register and behaves exactly the same as if the Link Position register were being read directly.
#define Audio_Az_Input_SD2LICBA_LPIBA_OFFSET      0
#define Audio_Az_Input_SD2LICBA_LPIBA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIBA:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD2LICBA_STRUCT;

#define SMN_Audio_Az_Input_SD2LICBA_ADDRESS    0x12020c4UL


/***********************************************************
* Register Name : Audio_Az_Input_SD2LPIB
* Register Description :
* Input Stream Descriptor 2 Link Position in Buffer
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer (LPIB):
// Indicates the number of bytes that have been received off the link. Since this register reflects the number of bytes that have been received into the current buffer, for the first buffer SDnLPIB will count from 0 to the value in the Cyclic Buffer Length (SDnCBL) register, inclusive. For subsequent buffers, SNnLPIB will count from a value of 1 to the value in the Cyclic Buffer Length register, inclusive
#define Audio_Az_Input_SD2LPIB_LPIB_OFFSET      0
#define Audio_Az_Input_SD2LPIB_LPIB_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIB:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD2LPIB_STRUCT;

#define SMN_Audio_Az_Input_SD2LPIB_ADDRESS    0x12000c4UL


/***********************************************************
* Register Name : Audio_Az_Input_SD2LVI
* Register Description :
* Input Stream Descriptor 2 Last Valid Index
* Visibility : 0x2
************************************************************/

// Bitfield Description : Last Valid Index (LVI): The value written to this register indicates the index for the last valid Buffer Descriptor in the BDL. After the controller has processed this descriptor, it will wrap back to the first descriptor in the list on continue processing.
// LVI must be at least 1; i.e., there must be at least two valid entries in the buffer descriptor list before DMA operations can begin.
// This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Input_SD2LVI_LVI_OFFSET      0
#define Audio_Az_Input_SD2LVI_LVI_MASK        0xff

// Bitfield Description : 
#define Audio_Az_Input_SD2LVI_Reserved_15_8_OFFSET      8
#define Audio_Az_Input_SD2LVI_Reserved_15_8_MASK        0xff00

typedef union {
  struct {
    UINT16                            LVI:8;
    UINT16                            Reserved_15_8:8;
  } Field;
  UINT16 Value;
} Audio_Az_Input_SD2LVI_STRUCT;

#define SMN_Audio_Az_Input_SD2LVI_ADDRESS    0x12000ccUL


/***********************************************************
* Register Name : Audio_Az_Input_SD3BDPL
* Register Description :
* Input Stream Descriptor 3 BDL Pointer Lower Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define Audio_Az_Input_SD3BDPL_Reserved_6_0_OFFSET      0
#define Audio_Az_Input_SD3BDPL_Reserved_6_0_MASK        0x7f

// Bitfield Description : Buffer Descriptor List Lower Base Address (BDLLBASE):
// Lower address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Input_SD3BDPL_BDLLBASE_OFFSET      7
#define Audio_Az_Input_SD3BDPL_BDLLBASE_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            BDLLBASE:25;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD3BDPL_STRUCT;

#define SMN_Audio_Az_Input_SD3BDPL_ADDRESS    0x12000f8UL


/***********************************************************
* Register Name : Audio_Az_Input_SD3BDPU
* Register Description :
* Input Stream Descriptor 3 BDL Pointer Upper Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : Buffer Descriptor List Upper Base Address (BDLUBASE):
// Upper 32-bit address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0. This register is Reserved, Read-only 0 if the 64OK (64-bit OK) bit indicates that the controller does not support 64-bit addressing.
#define Audio_Az_Input_SD3BDPU_BDLUBASE_OFFSET      0
#define Audio_Az_Input_SD3BDPU_BDLUBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BDLUBASE:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD3BDPU_STRUCT;

#define SMN_Audio_Az_Input_SD3BDPU_ADDRESS    0x12000fcUL


/***********************************************************
* Register Name : Audio_Az_Input_SD3CBL
* Register Description :
* Input Stream Descriptor 3 Cyclic Buffer Length
* Visibility : 0x2
************************************************************/

// Bitfield Description : Cyclic Buffer Length (CBL):
// Indicates the number of bytes in the complete cyclic buffer. Link Position in Buffer (SDnLPIB) will be reset when it reaches this value.
// Software may only write to this register after Global Reset, Controller Reset, or Stream Reset has occurred. Once the RUN bit has been set to enable the engine, software must not write to this register until after the next reset is asserted, or undefined events will occur.
// CBL must represent an integer number of samples. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Input_SD3CBL_CBL_OFFSET      0
#define Audio_Az_Input_SD3CBL_CBL_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CBL:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD3CBL_STRUCT;

#define SMN_Audio_Az_Input_SD3CBL_ADDRESS    0x12000e8UL


/***********************************************************
* Register Name : Audio_Az_Input_SD3FIFOS
* Register Description :
* Input Stream Descriptor 3 FIFO Size
* Visibility : 0x2
************************************************************/

// Bitfield Description : FIFO Size (FIFOS):
// Indicates the maximum number of bytes that could be fetched by the controller at one time. This is the maximum number of bytes that may have been DMA.d into memory but not yet transmitted on the link, and is also the maximum possible value that the LPIB count will increase by at one time. This number may be static to indicate a static buffer size, or may change after the data format has been programmed if the controller is able to vary its FIFO size based on the stream format. If it is able to change value after the data format has been programmed, the value update must happen immediately before the next read of the FIFOS register, and remain static until the next programming of data format.
#define Audio_Az_Input_SD3FIFOS_FIFOS_OFFSET      0
#define Audio_Az_Input_SD3FIFOS_FIFOS_MASK        0xffff

typedef union {
  struct {
    UINT16                            FIFOS:16;
  } Field;
  UINT16 Value;
} Audio_Az_Input_SD3FIFOS_STRUCT;

#define SMN_Audio_Az_Input_SD3FIFOS_ADDRESS    0x12000f0UL


/***********************************************************
* Register Name : Audio_Az_Input_SD3FMT
* Register Description :
* Input Stream Descriptor 3 Format
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of Channels (CHAN):
// Number of channels for this stream in each "sample block" of the "packets" in each "frame" on the link.
// 0000 = 1
// 0001 = 2
// ...
// 1111 = 16
#define Audio_Az_Input_SD3FMT_CHAN_OFFSET      0
#define Audio_Az_Input_SD3FMT_CHAN_MASK        0xf

// Bitfield Description : Bits per Sample (BITS):
// 000 = 8 bits. The data will be packed in memory in 8-bit containers on 16-bit boundaries.
// 001 = 16 bits. The data will be packed in memory in 16-bit containers on 16-bit boundaries.
// 010 = 20 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 011 = 24 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 100 = 32 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 101-111 = Reserved
#define Audio_Az_Input_SD3FMT_BITS_OFFSET      4
#define Audio_Az_Input_SD3FMT_BITS_MASK        0x70

// Bitfield Description : 
#define Audio_Az_Input_SD3FMT_Reserved_7_7_OFFSET      7
#define Audio_Az_Input_SD3FMT_Reserved_7_7_MASK        0x80

// Bitfield Description : Sample Base Rate Divisor (DIV):
// 000 = Divide by 1 (48 kHz, 44.1 kHz)
// 001 = Divide by 2 (24 kHz, 22.05 kHz)
// 010 = Divide by 3 (16 kHz, 32 kHz)
// 011 = Divide by 4 (11.025 kHz)
// 100 = Divide by 5 (9.6 kHz)
// 101 = Divide by 6 (8 kHz)
// 110 = Divide by 7
// 111 = Divide by 8 (6 kHz)
#define Audio_Az_Input_SD3FMT_DIV_OFFSET      8
#define Audio_Az_Input_SD3FMT_DIV_MASK        0x700

// Bitfield Description : 
#define Audio_Az_Input_SD3FMT_Reserved_13_11_OFFSET      11
#define Audio_Az_Input_SD3FMT_Reserved_13_11_MASK        0x3800

// Bitfield Description : Sample Base Rate (BASE):
// 0 = 48 kHz
// 1 = 44.1 kHz
#define Audio_Az_Input_SD3FMT_MULT_OFFSET      14
#define Audio_Az_Input_SD3FMT_MULT_MASK        0x4000

// Bitfield Description : 
#define Audio_Az_Input_SD3FMT_Reserved_15_15_OFFSET      15
#define Audio_Az_Input_SD3FMT_Reserved_15_15_MASK        0x8000

typedef union {
  struct {
    UINT16                            CHAN:4;
    UINT16                            Reserved_BITS:3;
    UINT16                            Reserved_7_7:1;
    UINT16                            DIV:3;
    UINT16                            Reserved_13_11:3;
    UINT16                            MULT:1;
    UINT16                            Reserved_15_15:1;
  } Field;
  UINT16 Value;
} Audio_Az_Input_SD3FMT_STRUCT;

#define SMN_Audio_Az_Input_SD3FMT_ADDRESS    0x12000f2UL


/***********************************************************
* Register Name : Audio_Az_Input_SD3LICBA
* Register Description :
* Input Stream Descriptor 3 Link Position in Buffer Alias
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer n Alias (LPIBA):
// An alias of the Link Position In Buffer register for each Stream Descriptor. This is an alias of the counter register and behaves exactly the same as if the Link Position register were being read directly.
#define Audio_Az_Input_SD3LICBA_LPIBA_OFFSET      0
#define Audio_Az_Input_SD3LICBA_LPIBA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIBA:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD3LICBA_STRUCT;

#define SMN_Audio_Az_Input_SD3LICBA_ADDRESS    0x12020e4UL


/***********************************************************
* Register Name : Audio_Az_Input_SD3LPIB
* Register Description :
* Input Stream Descriptor 3 Link Position in Buffer
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer (LPIB):
// Indicates the number of bytes that have been received off the link. Since this register reflects the number of bytes that have been received into the current buffer, for the first buffer SDnLPIB will count from 0 to the value in the Cyclic Buffer Length (SDnCBL) register, inclusive. For subsequent buffers, SNnLPIB will count from a value of 1 to the value in the Cyclic Buffer Length register, inclusive
#define Audio_Az_Input_SD3LPIB_LPIB_OFFSET      0
#define Audio_Az_Input_SD3LPIB_LPIB_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIB:32;
  } Field;
  UINT32 Value;
} Audio_Az_Input_SD3LPIB_STRUCT;

#define SMN_Audio_Az_Input_SD3LPIB_ADDRESS    0x12000e4UL


/***********************************************************
* Register Name : Audio_Az_Input_SD3LVI
* Register Description :
* Input Stream Descriptor 3 Last Valid Index
* Visibility : 0x2
************************************************************/

// Bitfield Description : Last Valid Index (LVI): The value written to this register indicates the index for the last valid Buffer Descriptor in the BDL. After the controller has processed this descriptor, it will wrap back to the first descriptor in the list on continue processing.
// LVI must be at least 1; i.e., there must be at least two valid entries in the buffer descriptor list before DMA operations can begin.
// This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Input_SD3LVI_LVI_OFFSET      0
#define Audio_Az_Input_SD3LVI_LVI_MASK        0xff

// Bitfield Description : 
#define Audio_Az_Input_SD3LVI_Reserved_15_8_OFFSET      8
#define Audio_Az_Input_SD3LVI_Reserved_15_8_MASK        0xff00

typedef union {
  struct {
    UINT16                            LVI:8;
    UINT16                            Reserved_15_8:8;
  } Field;
  UINT16 Value;
} Audio_Az_Input_SD3LVI_STRUCT;

#define SMN_Audio_Az_Input_SD3LVI_ADDRESS    0x12000ecUL


/***********************************************************
* Register Name : Audio_Az_Input_Stream_Payload_Capability
* Register Description :
* Input Stream Payload Capability
* Visibility : 0x2
************************************************************/

// Bitfield Description : Input Stream Payload Capability (INSTRMPAY) Indicates the maximum number of Words per frame for any single input stream. This measurement is in 16-bit Word quantities per 48-kHz frame. The value must not be larger than the INPAY register value. Software must ensure that a format which would cause more Words per frame than indicated is not programmed into the Input Stream Descriptor Register.
// 00h: No Limit (Stream size is limited only by INPAY)
// 01h: 1 Word payload
// ...
// FFh: 255h Word payload
#define Audio_Az_Input_Stream_Payload_Capability_INSTRMPAY_OFFSET      0
#define Audio_Az_Input_Stream_Payload_Capability_INSTRMPAY_MASK        0xffff

typedef union {
  struct {
    UINT16                            INSTRMPAY:16;
  } Field;
  UINT16 Value;
} Audio_Az_Input_Stream_Payload_Capability_STRUCT;

#define SMN_Audio_Az_Input_Stream_Payload_Capability_ADDRESS    0x120001aUL


/***********************************************************
* Register Name : Audio_Az_Interrupt_Control
* Register Description :
* Interrupt Control
* Visibility : 0x2
************************************************************/

// Bitfield Description : Stream Interrupt Enable (SIE): When set to 1, the individual Streams are enabled to generate an interrupt when the corresponding stream status bits get set.
// A stream interrupt will be caused as a result of a buffer with IOC = 1 in the BDL entry being completed or as a result of a FIFO error (underrun or overrun) occurring. Control over the generation of each of these sources is in the associated Stream Descriptor.
// The streams are numbered and the SIE bits assigned sequentially, based on their order in the register set.
// For instance, if there are two input streams, three output streams, and one bidirectional stream (ISS = 2, OSS = 3, BSS = 1), the bit assignments would be as follows:
// Bit 0: Input Stream 1
// Bit 1: Input Stream 2
// Bit 2: Output Stream 1
// Bit 3: Output Stream 2
// Bit 4: Output Stream 3
// Bit 5: Bidirectional Stream 1
// Bits 6-28: Reserved
// All bits not assigned to a supported DMA engine are RsvdZ.
#define Audio_Az_Interrupt_Control_SIE_OFFSET      0
#define Audio_Az_Interrupt_Control_SIE_MASK        0x3fffffff

// Bitfield Description : Controller Interrupt Enable (CIE):
// Enables the general interrupt for controller functions. When set to 1 the controller generates an interrupt when the corresponding status bit get sets due to a Response Interrupt, a Response Buffer Overrun, and wake events
#define Audio_Az_Interrupt_Control_CIE_OFFSET      30
#define Audio_Az_Interrupt_Control_CIE_MASK        0x40000000

// Bitfield Description : Global Interrupt Enable (GIE):
// Global bit to enable device interrupt generation. When set to 1, the High Definition Audio device is enabled to generate an interrupt. This control is in addition to any bits in the bus specific address space, such as the Interrupt Enable bit in the PCI Configuration Space.
#define Audio_Az_Interrupt_Control_GIE_OFFSET      31
#define Audio_Az_Interrupt_Control_GIE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            SIE:30;
    UINT32                            CIE:1;
    UINT32                            GIE:1;
  } Field;
  UINT32 Value;
} Audio_Az_Interrupt_Control_STRUCT;

#define SMN_Audio_Az_Interrupt_Control_ADDRESS    0x1200020UL


/***********************************************************
* Register Name : Audio_Az_Interrupt_Status
* Register Description :
* Interrupt Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : Stream Interrupt Status (SIS):
// A 1 indicates that an interrupt condition occurred on the corresponding Stream. Note that these status bits are set regardless of the state of the corresponding interrupt enable bits.
// The streams are numbered and the SIS bits assigned sequentially based on their order in the register set in the same way the SIE bits are set.
#define Audio_Az_Interrupt_Status_SIS_OFFSET      0
#define Audio_Az_Interrupt_Status_SIS_MASK        0x3fffffff

// Bitfield Description : Controller Interrupt Status (CIS):
// Status of general controller interrupt. This bit may be set regardless of the corresponding enable bit, but a hardware interrupt will not be generated unless the corresponding enable bit is set.
// A 1 indicates that an interrupt condition occurred due to a Response Interrupt, a Response Overrun, or a Codec State Change request. The exact cause can be determined by interrogating the RIRB Status register and the State Change Status register. Note that this bit is set regardless of the state of the corresponding interrupt enable bit.
#define Audio_Az_Interrupt_Status_CIS_OFFSET      30
#define Audio_Az_Interrupt_Status_CIS_MASK        0x40000000

// Bitfield Description : Global Interrupt Status (GIS):
// This bit is an "OR" of all of the interrupt status bits in this register.
#define Audio_Az_Interrupt_Status_GIS_OFFSET      31
#define Audio_Az_Interrupt_Status_GIS_MASK        0x80000000

typedef union {
  struct {
    UINT32                            SIS:30;
    UINT32                            CIS:1;
    UINT32                            GIS:1;
  } Field;
  UINT32 Value;
} Audio_Az_Interrupt_Status_STRUCT;

#define SMN_Audio_Az_Interrupt_Status_ADDRESS    0x1200024UL


/***********************************************************
* Register Name : Audio_Az_Linked_List_Capability_Header
* Register Description :
* Linked List Capability Header
* Visibility : 0x2
************************************************************/

// Bitfield Description : First Capability Pointer (PTR): This field contains the offset to the first capability structure of the linked list of capabilities, or 0000h if no linked list of capabilities exists.
#define Audio_Az_Linked_List_Capability_Header_LLCH_OFFSET      0
#define Audio_Az_Linked_List_Capability_Header_LLCH_MASK        0xffff

typedef union {
  struct {
    UINT16                            LLCH:16;
  } Field;
  UINT16 Value;
} Audio_Az_Linked_List_Capability_Header_STRUCT;

#define SMN_Audio_Az_Linked_List_Capability_Header_ADDRESS    0x1200014UL


/***********************************************************
* Register Name : Audio_Az_Major_Version
* Register Description :
* Major Version
* Visibility : 0x2
************************************************************/

// Bitfield Description : Major Version;
// indicates major revision number 1 of the High Definition Audio specification, for specification version "1.0."
#define Audio_Az_Major_Version_VMAJ_OFFSET      0
#define Audio_Az_Major_Version_VMAJ_MASK        0xff

typedef union {
  struct {
    UINT8                            VMAJ:8;
  } Field;
  UINT8 Value;
} Audio_Az_Major_Version_STRUCT;

#define SMN_Audio_Az_Major_Version_ADDRESS    0x1200003UL


/***********************************************************
* Register Name : Audio_Az_Minor_Version
* Register Description :
* Minor Version
* Visibility : 0x2
************************************************************/

// Bitfield Description : Minor Version;
// Indicates minor revision number 00h of the High Definition Audio specification, for specification version "1.0."
#define Audio_Az_Minor_Version_VMIN_OFFSET      0
#define Audio_Az_Minor_Version_VMIN_MASK        0xff

typedef union {
  struct {
    UINT8                            VMIN:8;
  } Field;
  UINT8 Value;
} Audio_Az_Minor_Version_STRUCT;

#define SMN_Audio_Az_Minor_Version_ADDRESS    0x1200002UL


/***********************************************************
* Register Name : Audio_Az_Output_Payload_Capability
* Register Description :
* Output Payload Capability
* Visibility : 0x2
************************************************************/

// Bitfield Description : Output Payload Capability;
// Indicates the total output payload available on the link. This does not include bandwidth used for command and control. This measurement is in 16-bit Word quantities per 48-kHz frame. The default link clock speed of 24.000 MHz (the data is double pumped) provides 1000 bits per frame, or 62.5 Words in total. Forty bits (2.5 Words) are used for command and control, leaving 60 Words available for data payload. Note that this value does not reflect any bandwidth increase due to support for multiple SDO lines.
// 00h: 0 Words
// 01h: 1 Word payload
// ...
// FFh: 255h Word payload
#define Audio_Az_Output_Payload_Capability_OUTPAY_OFFSET      0
#define Audio_Az_Output_Payload_Capability_OUTPAY_MASK        0xffff

typedef union {
  struct {
    UINT16                            OUTPAY:16;
  } Field;
  UINT16 Value;
} Audio_Az_Output_Payload_Capability_STRUCT;

#define SMN_Audio_Az_Output_Payload_Capability_ADDRESS    0x1200004UL


/***********************************************************
* Register Name : Audio_Az_Output_SD0BDPL
* Register Description :
* Output Stream Descriptor 0 BDL Pointer Lower Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define Audio_Az_Output_SD0BDPL_Reserved_6_0_OFFSET      0
#define Audio_Az_Output_SD0BDPL_Reserved_6_0_MASK        0x7f

// Bitfield Description : Buffer Descriptor List Lower Base Address (BDLLBASE):
// Lower address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Output_SD0BDPL_BDLLBASE_OFFSET      7
#define Audio_Az_Output_SD0BDPL_BDLLBASE_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            BDLLBASE:25;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD0BDPL_STRUCT;

#define SMN_Audio_Az_Output_SD0BDPL_ADDRESS    0x1200118UL


/***********************************************************
* Register Name : Audio_Az_Output_SD0BDPU
* Register Description :
* Output Stream Descriptor 0 BDL Pointer Upper Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : Buffer Descriptor List Upper Base Address (BDLUBASE):
// Upper 32-bit address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0. This register is Reserved, Read-only 0 if the 64OK (64-bit OK) bit indicates that the controller does not support 64-bit addressing.
#define Audio_Az_Output_SD0BDPU_BDLUBASE_OFFSET      0
#define Audio_Az_Output_SD0BDPU_BDLUBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BDLUBASE:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD0BDPU_STRUCT;

#define SMN_Audio_Az_Output_SD0BDPU_ADDRESS    0x120011cUL


/***********************************************************
* Register Name : Audio_Az_Output_SD0CBL
* Register Description :
* Output Stream Descriptor 0 Cyclic Buffer Length
* Visibility : 0x2
************************************************************/

// Bitfield Description : Cyclic Buffer Length (CBL):
// Indicates the number of bytes in the complete cyclic buffer. Link Position in Buffer (SDnLPIB) will be reset when it reaches this value.
// Software may only write to this register after Global Reset, Controller Reset, or Stream Reset has occurred. Once the RUN bit has been set to enable the engine, software must not write to this register until after the next reset is asserted, or undefined events will occur.
// CBL must represent an integer number of samples. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Output_SD0CBL_CBL_OFFSET      0
#define Audio_Az_Output_SD0CBL_CBL_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CBL:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD0CBL_STRUCT;

#define SMN_Audio_Az_Output_SD0CBL_ADDRESS    0x1200108UL


/***********************************************************
* Register Name : Audio_Az_Output_SD0FIFOS
* Register Description :
* Output Stream Descriptor 0 FIFO Size
* Visibility : 0x2
************************************************************/

// Bitfield Description : FIFO Size (FIFOS):
// Indicates the maximum number of bytes that could be fetched by the controller at one time. This is the maximum number of bytes that may have been DMA.d into memory but not yet transmitted on the link, and is also the maximum possible value that the LPIB count will increase by at one time. This number may be static to indicate a static buffer size, or may change after the data format has been programmed if the controller is able to vary its FIFO size based on the stream format. If it is able to change value after the data format has been programmed, the value update must happen immediately before the next read of the FIFOS register, and remain static until the next programming of data format.
#define Audio_Az_Output_SD0FIFOS_FIFOS_OFFSET      0
#define Audio_Az_Output_SD0FIFOS_FIFOS_MASK        0xffff

typedef union {
  struct {
    UINT16                            FIFOS:16;
  } Field;
  UINT16 Value;
} Audio_Az_Output_SD0FIFOS_STRUCT;

#define SMN_Audio_Az_Output_SD0FIFOS_ADDRESS    0x1200110UL


/***********************************************************
* Register Name : Audio_Az_Output_SD0FMT
* Register Description :
* Output Stream Descriptor 0 Format
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of Channels (CHAN):
// Number of channels for this stream in each "sample block" of the "packets" in each "frame" on the link.
// 0000 = 1
// 0001 = 2
// ...
// 1111 = 16
#define Audio_Az_Output_SD0FMT_CHAN_OFFSET      0
#define Audio_Az_Output_SD0FMT_CHAN_MASK        0xf

// Bitfield Description : Bits per Sample (BITS):
// 000 = 8 bits. The data will be packed in memory in 8-bit containers on 16-bit boundaries.
// 001 = 16 bits. The data will be packed in memory in 16-bit containers on 16-bit boundaries.
// 010 = 20 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 011 = 24 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 100 = 32 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 101-111 = Reserved
#define Audio_Az_Output_SD0FMT_BITS_OFFSET      4
#define Audio_Az_Output_SD0FMT_BITS_MASK        0x70

// Bitfield Description : 
#define Audio_Az_Output_SD0FMT_Reserved_7_7_OFFSET      7
#define Audio_Az_Output_SD0FMT_Reserved_7_7_MASK        0x80

// Bitfield Description : Sample Base Rate Divisor (DIV):
// 000 = Divide by 1 (48 kHz, 44.1 kHz)
// 001 = Divide by 2 (24 kHz, 22.05 kHz)
// 010 = Divide by 3 (16 kHz, 32 kHz)
// 011 = Divide by 4 (11.025 kHz)
// 100 = Divide by 5 (9.6 kHz)
// 101 = Divide by 6 (8 kHz)
// 110 = Divide by 7
// 111 = Divide by 8 (6 kHz)
#define Audio_Az_Output_SD0FMT_DIV_OFFSET      8
#define Audio_Az_Output_SD0FMT_DIV_MASK        0x700

// Bitfield Description : 
#define Audio_Az_Output_SD0FMT_Reserved_13_11_OFFSET      11
#define Audio_Az_Output_SD0FMT_Reserved_13_11_MASK        0x3800

// Bitfield Description : Sample Base Rate (BASE):
// 0 = 48 kHz
// 1 = 44.1 kHz
#define Audio_Az_Output_SD0FMT_MULT_OFFSET      14
#define Audio_Az_Output_SD0FMT_MULT_MASK        0x4000

// Bitfield Description : 
#define Audio_Az_Output_SD0FMT_Reserved_15_15_OFFSET      15
#define Audio_Az_Output_SD0FMT_Reserved_15_15_MASK        0x8000

typedef union {
  struct {
    UINT16                            CHAN:4;
    UINT16                            Reserved_BITS:3;
    UINT16                            Reserved_7_7:1;
    UINT16                            DIV:3;
    UINT16                            Reserved_13_11:3;
    UINT16                            MULT:1;
    UINT16                            Reserved_15_15:1;
  } Field;
  UINT16 Value;
} Audio_Az_Output_SD0FMT_STRUCT;

#define SMN_Audio_Az_Output_SD0FMT_ADDRESS    0x1200112UL


/***********************************************************
* Register Name : Audio_Az_Output_SD0LICBA
* Register Description :
* Output Stream Descriptor 0 Link Position in Buffer Alias
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer n Alias (LPIBA):
// An alias of the Link Position In Buffer register for each Stream Descriptor. This is an alias of the counter register and behaves exactly the same as if the Link Position register were being read directly.
#define Audio_Az_Output_SD0LICBA_LPIBA_OFFSET      0
#define Audio_Az_Output_SD0LICBA_LPIBA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIBA:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD0LICBA_STRUCT;

#define SMN_Audio_Az_Output_SD0LICBA_ADDRESS    0x1202104UL


/***********************************************************
* Register Name : Audio_Az_Output_SD0LPIB
* Register Description :
* Output Stream Descriptor 0 Link Position in Buffer
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer (LPIB):
// Indicates the number of bytes that have been received off the link. Since this register reflects the number of bytes that have been received into the current buffer, for the first buffer SDnLPIB will count from 0 to the value in the Cyclic Buffer Length (SDnCBL) register, inclusive. For subsequent buffers, SNnLPIB will count from a value of 1 to the value in the Cyclic Buffer Length register, inclusive
#define Audio_Az_Output_SD0LPIB_LPIB_OFFSET      0
#define Audio_Az_Output_SD0LPIB_LPIB_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIB:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD0LPIB_STRUCT;

#define SMN_Audio_Az_Output_SD0LPIB_ADDRESS    0x1200104UL


/***********************************************************
* Register Name : Audio_Az_Output_SD0LVI
* Register Description :
* Output Stream Descriptor 0 Last Valid Index
* Visibility : 0x2
************************************************************/

// Bitfield Description : Last Valid Index (LVI): The value written to this register indicates the index for the last valid Buffer Descriptor in the BDL. After the controller has processed this descriptor, it will wrap back to the first descriptor in the list on continue processing.
// LVI must be at least 1; i.e., there must be at least two valid entries in the buffer descriptor list before DMA operations can begin.
// This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Output_SD0LVI_LVI_OFFSET      0
#define Audio_Az_Output_SD0LVI_LVI_MASK        0xff

// Bitfield Description : 
#define Audio_Az_Output_SD0LVI_Reserved_15_8_OFFSET      8
#define Audio_Az_Output_SD0LVI_Reserved_15_8_MASK        0xff00

typedef union {
  struct {
    UINT16                            LVI:8;
    UINT16                            Reserved_15_8:8;
  } Field;
  UINT16 Value;
} Audio_Az_Output_SD0LVI_STRUCT;

#define SMN_Audio_Az_Output_SD0LVI_ADDRESS    0x120010cUL


/***********************************************************
* Register Name : Audio_Az_Output_SD1BDPL
* Register Description :
* Output Stream Descriptor 1 BDL Pointer Lower Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define Audio_Az_Output_SD1BDPL_Reserved_6_0_OFFSET      0
#define Audio_Az_Output_SD1BDPL_Reserved_6_0_MASK        0x7f

// Bitfield Description : Buffer Descriptor List Lower Base Address (BDLLBASE):
// Lower address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Output_SD1BDPL_BDLLBASE_OFFSET      7
#define Audio_Az_Output_SD1BDPL_BDLLBASE_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            BDLLBASE:25;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD1BDPL_STRUCT;

#define SMN_Audio_Az_Output_SD1BDPL_ADDRESS    0x1200138UL


/***********************************************************
* Register Name : Audio_Az_Output_SD1BDPU
* Register Description :
* Output Stream Descriptor 1 BDL Pointer Upper Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : Buffer Descriptor List Upper Base Address (BDLUBASE):
// Upper 32-bit address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0. This register is Reserved, Read-only 0 if the 64OK (64-bit OK) bit indicates that the controller does not support 64-bit addressing.
#define Audio_Az_Output_SD1BDPU_BDLUBASE_OFFSET      0
#define Audio_Az_Output_SD1BDPU_BDLUBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BDLUBASE:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD1BDPU_STRUCT;

#define SMN_Audio_Az_Output_SD1BDPU_ADDRESS    0x120013cUL


/***********************************************************
* Register Name : Audio_Az_Output_SD1CBL
* Register Description :
* Output Stream Descriptor 1 Cyclic Buffer Length
* Visibility : 0x2
************************************************************/

// Bitfield Description : Cyclic Buffer Length (CBL):
// Indicates the number of bytes in the complete cyclic buffer. Link Position in Buffer (SDnLPIB) will be reset when it reaches this value.
// Software may only write to this register after Global Reset, Controller Reset, or Stream Reset has occurred. Once the RUN bit has been set to enable the engine, software must not write to this register until after the next reset is asserted, or undefined events will occur.
// CBL must represent an integer number of samples. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Output_SD1CBL_CBL_OFFSET      0
#define Audio_Az_Output_SD1CBL_CBL_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CBL:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD1CBL_STRUCT;

#define SMN_Audio_Az_Output_SD1CBL_ADDRESS    0x1200128UL


/***********************************************************
* Register Name : Audio_Az_Output_SD1FIFOS
* Register Description :
* Output Stream Descriptor 1 FIFO Size
* Visibility : 0x2
************************************************************/

// Bitfield Description : FIFO Size (FIFOS):
// Indicates the maximum number of bytes that could be fetched by the controller at one time. This is the maximum number of bytes that may have been DMA.d into memory but not yet transmitted on the link, and is also the maximum possible value that the LPIB count will increase by at one time. This number may be static to indicate a static buffer size, or may change after the data format has been programmed if the controller is able to vary its FIFO size based on the stream format. If it is able to change value after the data format has been programmed, the value update must happen immediately before the next read of the FIFOS register, and remain static until the next programming of data format.
#define Audio_Az_Output_SD1FIFOS_FIFOS_OFFSET      0
#define Audio_Az_Output_SD1FIFOS_FIFOS_MASK        0xffff

typedef union {
  struct {
    UINT16                            FIFOS:16;
  } Field;
  UINT16 Value;
} Audio_Az_Output_SD1FIFOS_STRUCT;

#define SMN_Audio_Az_Output_SD1FIFOS_ADDRESS    0x1200130UL


/***********************************************************
* Register Name : Audio_Az_Output_SD1FMT
* Register Description :
* Output Stream Descriptor 1 Format
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of Channels (CHAN):
// Number of channels for this stream in each "sample block" of the "packets" in each "frame" on the link.
// 0000 = 1
// 0001 = 2
// ...
// 1111 = 16
#define Audio_Az_Output_SD1FMT_CHAN_OFFSET      0
#define Audio_Az_Output_SD1FMT_CHAN_MASK        0xf

// Bitfield Description : Bits per Sample (BITS):
// 000 = 8 bits. The data will be packed in memory in 8-bit containers on 16-bit boundaries.
// 001 = 16 bits. The data will be packed in memory in 16-bit containers on 16-bit boundaries.
// 010 = 20 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 011 = 24 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 100 = 32 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 101-111 = Reserved
#define Audio_Az_Output_SD1FMT_BITS_OFFSET      4
#define Audio_Az_Output_SD1FMT_BITS_MASK        0x70

// Bitfield Description : 
#define Audio_Az_Output_SD1FMT_Reserved_7_7_OFFSET      7
#define Audio_Az_Output_SD1FMT_Reserved_7_7_MASK        0x80

// Bitfield Description : Sample Base Rate Divisor (DIV):
// 000 = Divide by 1 (48 kHz, 44.1 kHz)
// 001 = Divide by 2 (24 kHz, 22.05 kHz)
// 010 = Divide by 3 (16 kHz, 32 kHz)
// 011 = Divide by 4 (11.025 kHz)
// 100 = Divide by 5 (9.6 kHz)
// 101 = Divide by 6 (8 kHz)
// 110 = Divide by 7
// 111 = Divide by 8 (6 kHz)
#define Audio_Az_Output_SD1FMT_DIV_OFFSET      8
#define Audio_Az_Output_SD1FMT_DIV_MASK        0x700

// Bitfield Description : 
#define Audio_Az_Output_SD1FMT_Reserved_13_11_OFFSET      11
#define Audio_Az_Output_SD1FMT_Reserved_13_11_MASK        0x3800

// Bitfield Description : Sample Base Rate (BASE):
// 0 = 48 kHz
// 1 = 44.1 kHz
#define Audio_Az_Output_SD1FMT_MULT_OFFSET      14
#define Audio_Az_Output_SD1FMT_MULT_MASK        0x4000

// Bitfield Description : 
#define Audio_Az_Output_SD1FMT_Reserved_15_15_OFFSET      15
#define Audio_Az_Output_SD1FMT_Reserved_15_15_MASK        0x8000

typedef union {
  struct {
    UINT16                            CHAN:4;
    UINT16                            Reserved_BITS:3;
    UINT16                            Reserved_7_7:1;
    UINT16                            DIV:3;
    UINT16                            Reserved_13_11:3;
    UINT16                            MULT:1;
    UINT16                            Reserved_15_15:1;
  } Field;
  UINT16 Value;
} Audio_Az_Output_SD1FMT_STRUCT;

#define SMN_Audio_Az_Output_SD1FMT_ADDRESS    0x1200132UL


/***********************************************************
* Register Name : Audio_Az_Output_SD1LICBA
* Register Description :
* Output Stream Descriptor 1 Link Position in Buffer Alias
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer n Alias (LPIBA):
// An alias of the Link Position In Buffer register for each Stream Descriptor. This is an alias of the counter register and behaves exactly the same as if the Link Position register were being read directly.
#define Audio_Az_Output_SD1LICBA_LPIBA_OFFSET      0
#define Audio_Az_Output_SD1LICBA_LPIBA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIBA:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD1LICBA_STRUCT;

#define SMN_Audio_Az_Output_SD1LICBA_ADDRESS    0x1202124UL


/***********************************************************
* Register Name : Audio_Az_Output_SD1LPIB
* Register Description :
* Output Stream Descriptor 1 Link Position in Buffer
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer (LPIB):
// Indicates the number of bytes that have been received off the link. Since this register reflects the number of bytes that have been received into the current buffer, for the first buffer SDnLPIB will count from 0 to the value in the Cyclic Buffer Length (SDnCBL) register, inclusive. For subsequent buffers, SNnLPIB will count from a value of 1 to the value in the Cyclic Buffer Length register, inclusive
#define Audio_Az_Output_SD1LPIB_LPIB_OFFSET      0
#define Audio_Az_Output_SD1LPIB_LPIB_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIB:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD1LPIB_STRUCT;

#define SMN_Audio_Az_Output_SD1LPIB_ADDRESS    0x1200124UL


/***********************************************************
* Register Name : Audio_Az_Output_SD1LVI
* Register Description :
* Output Stream Descriptor 1 Last Valid Index
* Visibility : 0x2
************************************************************/

// Bitfield Description : Last Valid Index (LVI): The value written to this register indicates the index for the last valid Buffer Descriptor in the BDL. After the controller has processed this descriptor, it will wrap back to the first descriptor in the list on continue processing.
// LVI must be at least 1; i.e., there must be at least two valid entries in the buffer descriptor list before DMA operations can begin.
// This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Output_SD1LVI_LVI_OFFSET      0
#define Audio_Az_Output_SD1LVI_LVI_MASK        0xff

// Bitfield Description : 
#define Audio_Az_Output_SD1LVI_Reserved_15_8_OFFSET      8
#define Audio_Az_Output_SD1LVI_Reserved_15_8_MASK        0xff00

typedef union {
  struct {
    UINT16                            LVI:8;
    UINT16                            Reserved_15_8:8;
  } Field;
  UINT16 Value;
} Audio_Az_Output_SD1LVI_STRUCT;

#define SMN_Audio_Az_Output_SD1LVI_ADDRESS    0x120012cUL


/***********************************************************
* Register Name : Audio_Az_Output_SD2BDPL
* Register Description :
* Output Stream Descriptor 2 BDL Pointer Lower Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define Audio_Az_Output_SD2BDPL_Reserved_6_0_OFFSET      0
#define Audio_Az_Output_SD2BDPL_Reserved_6_0_MASK        0x7f

// Bitfield Description : Buffer Descriptor List Lower Base Address (BDLLBASE):
// Lower address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Output_SD2BDPL_BDLLBASE_OFFSET      7
#define Audio_Az_Output_SD2BDPL_BDLLBASE_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            BDLLBASE:25;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD2BDPL_STRUCT;

#define SMN_Audio_Az_Output_SD2BDPL_ADDRESS    0x1200158UL


/***********************************************************
* Register Name : Audio_Az_Output_SD2BDPU
* Register Description :
* Output Stream Descriptor 2 BDL Pointer Upper Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : Buffer Descriptor List Upper Base Address (BDLUBASE):
// Upper 32-bit address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0. This register is Reserved, Read-only 0 if the 64OK (64-bit OK) bit indicates that the controller does not support 64-bit addressing.
#define Audio_Az_Output_SD2BDPU_BDLUBASE_OFFSET      0
#define Audio_Az_Output_SD2BDPU_BDLUBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BDLUBASE:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD2BDPU_STRUCT;

#define SMN_Audio_Az_Output_SD2BDPU_ADDRESS    0x120015cUL


/***********************************************************
* Register Name : Audio_Az_Output_SD2CBL
* Register Description :
* Output Stream Descriptor 2 Cyclic Buffer Length
* Visibility : 0x2
************************************************************/

// Bitfield Description : Cyclic Buffer Length (CBL):
// Indicates the number of bytes in the complete cyclic buffer. Link Position in Buffer (SDnLPIB) will be reset when it reaches this value.
// Software may only write to this register after Global Reset, Controller Reset, or Stream Reset has occurred. Once the RUN bit has been set to enable the engine, software must not write to this register until after the next reset is asserted, or undefined events will occur.
// CBL must represent an integer number of samples. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Output_SD2CBL_CBL_OFFSET      0
#define Audio_Az_Output_SD2CBL_CBL_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CBL:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD2CBL_STRUCT;

#define SMN_Audio_Az_Output_SD2CBL_ADDRESS    0x1200148UL


/***********************************************************
* Register Name : Audio_Az_Output_SD2FIFOS
* Register Description :
* Output Stream Descriptor 2 FIFO Size
* Visibility : 0x2
************************************************************/

// Bitfield Description : FIFO Size (FIFOS):
// Indicates the maximum number of bytes that could be fetched by the controller at one time. This is the maximum number of bytes that may have been DMA.d into memory but not yet transmitted on the link, and is also the maximum possible value that the LPIB count will increase by at one time. This number may be static to indicate a static buffer size, or may change after the data format has been programmed if the controller is able to vary its FIFO size based on the stream format. If it is able to change value after the data format has been programmed, the value update must happen immediately before the next read of the FIFOS register, and remain static until the next programming of data format.
#define Audio_Az_Output_SD2FIFOS_FIFOS_OFFSET      0
#define Audio_Az_Output_SD2FIFOS_FIFOS_MASK        0xffff

typedef union {
  struct {
    UINT16                            FIFOS:16;
  } Field;
  UINT16 Value;
} Audio_Az_Output_SD2FIFOS_STRUCT;

#define SMN_Audio_Az_Output_SD2FIFOS_ADDRESS    0x1200150UL


/***********************************************************
* Register Name : Audio_Az_Output_SD2FMT
* Register Description :
* Output Stream Descriptor 2 Format
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of Channels (CHAN):
// Number of channels for this stream in each "sample block" of the "packets" in each "frame" on the link.
// 0000 = 1
// 0001 = 2
// ...
// 1111 = 16
#define Audio_Az_Output_SD2FMT_CHAN_OFFSET      0
#define Audio_Az_Output_SD2FMT_CHAN_MASK        0xf

// Bitfield Description : Bits per Sample (BITS):
// 000 = 8 bits. The data will be packed in memory in 8-bit containers on 16-bit boundaries.
// 001 = 16 bits. The data will be packed in memory in 16-bit containers on 16-bit boundaries.
// 010 = 20 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 011 = 24 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 100 = 32 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 101-111 = Reserved
#define Audio_Az_Output_SD2FMT_BITS_OFFSET      4
#define Audio_Az_Output_SD2FMT_BITS_MASK        0x70

// Bitfield Description : 
#define Audio_Az_Output_SD2FMT_Reserved_7_7_OFFSET      7
#define Audio_Az_Output_SD2FMT_Reserved_7_7_MASK        0x80

// Bitfield Description : Sample Base Rate Divisor (DIV):
// 000 = Divide by 1 (48 kHz, 44.1 kHz)
// 001 = Divide by 2 (24 kHz, 22.05 kHz)
// 010 = Divide by 3 (16 kHz, 32 kHz)
// 011 = Divide by 4 (11.025 kHz)
// 100 = Divide by 5 (9.6 kHz)
// 101 = Divide by 6 (8 kHz)
// 110 = Divide by 7
// 111 = Divide by 8 (6 kHz)
#define Audio_Az_Output_SD2FMT_DIV_OFFSET      8
#define Audio_Az_Output_SD2FMT_DIV_MASK        0x700

// Bitfield Description : 
#define Audio_Az_Output_SD2FMT_Reserved_13_11_OFFSET      11
#define Audio_Az_Output_SD2FMT_Reserved_13_11_MASK        0x3800

// Bitfield Description : Sample Base Rate (BASE):
// 0 = 48 kHz
// 1 = 44.1 kHz
#define Audio_Az_Output_SD2FMT_MULT_OFFSET      14
#define Audio_Az_Output_SD2FMT_MULT_MASK        0x4000

// Bitfield Description : 
#define Audio_Az_Output_SD2FMT_Reserved_15_15_OFFSET      15
#define Audio_Az_Output_SD2FMT_Reserved_15_15_MASK        0x8000

typedef union {
  struct {
    UINT16                            CHAN:4;
    UINT16                            Reserved_BITS:3;
    UINT16                            Reserved_7_7:1;
    UINT16                            DIV:3;
    UINT16                            Reserved_13_11:3;
    UINT16                            MULT:1;
    UINT16                            Reserved_15_15:1;
  } Field;
  UINT16 Value;
} Audio_Az_Output_SD2FMT_STRUCT;

#define SMN_Audio_Az_Output_SD2FMT_ADDRESS    0x1200152UL


/***********************************************************
* Register Name : Audio_Az_Output_SD2LICBA
* Register Description :
* Output Stream Descriptor 2 Link Position in Buffer Alias
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer n Alias (LPIBA):
// An alias of the Link Position In Buffer register for each Stream Descriptor. This is an alias of the counter register and behaves exactly the same as if the Link Position register were being read directly.
#define Audio_Az_Output_SD2LICBA_LPIBA_OFFSET      0
#define Audio_Az_Output_SD2LICBA_LPIBA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIBA:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD2LICBA_STRUCT;

#define SMN_Audio_Az_Output_SD2LICBA_ADDRESS    0x1202144UL


/***********************************************************
* Register Name : Audio_Az_Output_SD2LPIB
* Register Description :
* Output Stream Descriptor 2 Link Position in Buffer
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer (LPIB):
// Indicates the number of bytes that have been received off the link. Since this register reflects the number of bytes that have been received into the current buffer, for the first buffer SDnLPIB will count from 0 to the value in the Cyclic Buffer Length (SDnCBL) register, inclusive. For subsequent buffers, SNnLPIB will count from a value of 1 to the value in the Cyclic Buffer Length register, inclusive
#define Audio_Az_Output_SD2LPIB_LPIB_OFFSET      0
#define Audio_Az_Output_SD2LPIB_LPIB_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIB:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD2LPIB_STRUCT;

#define SMN_Audio_Az_Output_SD2LPIB_ADDRESS    0x1200144UL


/***********************************************************
* Register Name : Audio_Az_Output_SD2LVI
* Register Description :
* Output Stream Descriptor 2 Last Valid Index
* Visibility : 0x2
************************************************************/

// Bitfield Description : Last Valid Index (LVI): The value written to this register indicates the index for the last valid Buffer Descriptor in the BDL. After the controller has processed this descriptor, it will wrap back to the first descriptor in the list on continue processing.
// LVI must be at least 1; i.e., there must be at least two valid entries in the buffer descriptor list before DMA operations can begin.
// This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Output_SD2LVI_LVI_OFFSET      0
#define Audio_Az_Output_SD2LVI_LVI_MASK        0xff

// Bitfield Description : 
#define Audio_Az_Output_SD2LVI_Reserved_15_8_OFFSET      8
#define Audio_Az_Output_SD2LVI_Reserved_15_8_MASK        0xff00

typedef union {
  struct {
    UINT16                            LVI:8;
    UINT16                            Reserved_15_8:8;
  } Field;
  UINT16 Value;
} Audio_Az_Output_SD2LVI_STRUCT;

#define SMN_Audio_Az_Output_SD2LVI_ADDRESS    0x120014cUL


/***********************************************************
* Register Name : Audio_Az_Output_SD3BDPL
* Register Description :
* Output Stream Descriptor 3 BDL Pointer Lower Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define Audio_Az_Output_SD3BDPL_Reserved_6_0_OFFSET      0
#define Audio_Az_Output_SD3BDPL_Reserved_6_0_MASK        0x7f

// Bitfield Description : Buffer Descriptor List Lower Base Address (BDLLBASE):
// Lower address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Output_SD3BDPL_BDLLBASE_OFFSET      7
#define Audio_Az_Output_SD3BDPL_BDLLBASE_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            Reserved_6_0:7;
    UINT32                            BDLLBASE:25;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD3BDPL_STRUCT;

#define SMN_Audio_Az_Output_SD3BDPL_ADDRESS    0x1200178UL


/***********************************************************
* Register Name : Audio_Az_Output_SD3BDPU
* Register Description :
* Output Stream Descriptor 3 BDL Pointer Upper Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : Buffer Descriptor List Upper Base Address (BDLUBASE):
// Upper 32-bit address of the Buffer Descriptor List. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This value should not be modified except when the RUN bit is 0. This register is Reserved, Read-only 0 if the 64OK (64-bit OK) bit indicates that the controller does not support 64-bit addressing.
#define Audio_Az_Output_SD3BDPU_BDLUBASE_OFFSET      0
#define Audio_Az_Output_SD3BDPU_BDLUBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BDLUBASE:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD3BDPU_STRUCT;

#define SMN_Audio_Az_Output_SD3BDPU_ADDRESS    0x120017cUL


/***********************************************************
* Register Name : Audio_Az_Output_SD3CBL
* Register Description :
* Output Stream Descriptor 3 Cyclic Buffer Length
* Visibility : 0x2
************************************************************/

// Bitfield Description : Cyclic Buffer Length (CBL):
// Indicates the number of bytes in the complete cyclic buffer. Link Position in Buffer (SDnLPIB) will be reset when it reaches this value.
// Software may only write to this register after Global Reset, Controller Reset, or Stream Reset has occurred. Once the RUN bit has been set to enable the engine, software must not write to this register until after the next reset is asserted, or undefined events will occur.
// CBL must represent an integer number of samples. This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Output_SD3CBL_CBL_OFFSET      0
#define Audio_Az_Output_SD3CBL_CBL_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CBL:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD3CBL_STRUCT;

#define SMN_Audio_Az_Output_SD3CBL_ADDRESS    0x1200168UL


/***********************************************************
* Register Name : Audio_Az_Output_SD3FIFOS
* Register Description :
* Output Stream Descriptor 3 FIFO Size
* Visibility : 0x2
************************************************************/

// Bitfield Description : FIFO Size (FIFOS):
// Indicates the maximum number of bytes that could be fetched by the controller at one time. This is the maximum number of bytes that may have been DMA.d into memory but not yet transmitted on the link, and is also the maximum possible value that the LPIB count will increase by at one time. This number may be static to indicate a static buffer size, or may change after the data format has been programmed if the controller is able to vary its FIFO size based on the stream format. If it is able to change value after the data format has been programmed, the value update must happen immediately before the next read of the FIFOS register, and remain static until the next programming of data format.
#define Audio_Az_Output_SD3FIFOS_FIFOS_OFFSET      0
#define Audio_Az_Output_SD3FIFOS_FIFOS_MASK        0xffff

typedef union {
  struct {
    UINT16                            FIFOS:16;
  } Field;
  UINT16 Value;
} Audio_Az_Output_SD3FIFOS_STRUCT;

#define SMN_Audio_Az_Output_SD3FIFOS_ADDRESS    0x1200170UL


/***********************************************************
* Register Name : Audio_Az_Output_SD3FMT
* Register Description :
* Output Stream Descriptor 3 Format
* Visibility : 0x2
************************************************************/

// Bitfield Description : Number of Channels (CHAN):
// Number of channels for this stream in each "sample block" of the "packets" in each "frame" on the link.
// 0000 = 1
// 0001 = 2
// ...
// 1111 = 16
#define Audio_Az_Output_SD3FMT_CHAN_OFFSET      0
#define Audio_Az_Output_SD3FMT_CHAN_MASK        0xf

// Bitfield Description : Bits per Sample (BITS):
// 000 = 8 bits. The data will be packed in memory in 8-bit containers on 16-bit boundaries.
// 001 = 16 bits. The data will be packed in memory in 16-bit containers on 16-bit boundaries.
// 010 = 20 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 011 = 24 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 100 = 32 bits. The data will be packed in memory in 32-bit containers on 32-bit boundaries.
// 101-111 = Reserved
#define Audio_Az_Output_SD3FMT_BITS_OFFSET      4
#define Audio_Az_Output_SD3FMT_BITS_MASK        0x70

// Bitfield Description : 
#define Audio_Az_Output_SD3FMT_Reserved_7_7_OFFSET      7
#define Audio_Az_Output_SD3FMT_Reserved_7_7_MASK        0x80

// Bitfield Description : Sample Base Rate Divisor (DIV):
// 000 = Divide by 1 (48 kHz, 44.1 kHz)
// 001 = Divide by 2 (24 kHz, 22.05 kHz)
// 010 = Divide by 3 (16 kHz, 32 kHz)
// 011 = Divide by 4 (11.025 kHz)
// 100 = Divide by 5 (9.6 kHz)
// 101 = Divide by 6 (8 kHz)
// 110 = Divide by 7
// 111 = Divide by 8 (6 kHz)
#define Audio_Az_Output_SD3FMT_DIV_OFFSET      8
#define Audio_Az_Output_SD3FMT_DIV_MASK        0x700

// Bitfield Description : 
#define Audio_Az_Output_SD3FMT_Reserved_13_11_OFFSET      11
#define Audio_Az_Output_SD3FMT_Reserved_13_11_MASK        0x3800

// Bitfield Description : Sample Base Rate (BASE):
// 0 = 48 kHz
// 1 = 44.1 kHz
#define Audio_Az_Output_SD3FMT_MULT_OFFSET      14
#define Audio_Az_Output_SD3FMT_MULT_MASK        0x4000

// Bitfield Description : 
#define Audio_Az_Output_SD3FMT_Reserved_15_15_OFFSET      15
#define Audio_Az_Output_SD3FMT_Reserved_15_15_MASK        0x8000

typedef union {
  struct {
    UINT16                            CHAN:4;
    UINT16                            Reserved_BITS:3;
    UINT16                            Reserved_7_7:1;
    UINT16                            DIV:3;
    UINT16                            Reserved_13_11:3;
    UINT16                            MULT:1;
    UINT16                            Reserved_15_15:1;
  } Field;
  UINT16 Value;
} Audio_Az_Output_SD3FMT_STRUCT;

#define SMN_Audio_Az_Output_SD3FMT_ADDRESS    0x1200172UL


/***********************************************************
* Register Name : Audio_Az_Output_SD3LICBA
* Register Description :
* Output Stream Descriptor 3 Link Position in Buffer Alias
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer n Alias (LPIBA):
// An alias of the Link Position In Buffer register for each Stream Descriptor. This is an alias of the counter register and behaves exactly the same as if the Link Position register were being read directly.
#define Audio_Az_Output_SD3LICBA_LPIBA_OFFSET      0
#define Audio_Az_Output_SD3LICBA_LPIBA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIBA:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD3LICBA_STRUCT;

#define SMN_Audio_Az_Output_SD3LICBA_ADDRESS    0x1202164UL


/***********************************************************
* Register Name : Audio_Az_Output_SD3LPIB
* Register Description :
* Output Stream Descriptor 3 Link Position in Buffer
* Visibility : 0x2
************************************************************/

// Bitfield Description : Link Position in Buffer (LPIB):
// Indicates the number of bytes that have been received off the link. Since this register reflects the number of bytes that have been received into the current buffer, for the first buffer SDnLPIB will count from 0 to the value in the Cyclic Buffer Length (SDnCBL) register, inclusive. For subsequent buffers, SNnLPIB will count from a value of 1 to the value in the Cyclic Buffer Length register, inclusive
#define Audio_Az_Output_SD3LPIB_LPIB_OFFSET      0
#define Audio_Az_Output_SD3LPIB_LPIB_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            LPIB:32;
  } Field;
  UINT32 Value;
} Audio_Az_Output_SD3LPIB_STRUCT;

#define SMN_Audio_Az_Output_SD3LPIB_ADDRESS    0x1200164UL


/***********************************************************
* Register Name : Audio_Az_Output_SD3LVI
* Register Description :
* Output Stream Descriptor 3 Last Valid Index
* Visibility : 0x2
************************************************************/

// Bitfield Description : Last Valid Index (LVI): The value written to this register indicates the index for the last valid Buffer Descriptor in the BDL. After the controller has processed this descriptor, it will wrap back to the first descriptor in the list on continue processing.
// LVI must be at least 1; i.e., there must be at least two valid entries in the buffer descriptor list before DMA operations can begin.
// This value should not be modified except when the RUN bit is 0.
#define Audio_Az_Output_SD3LVI_LVI_OFFSET      0
#define Audio_Az_Output_SD3LVI_LVI_MASK        0xff

// Bitfield Description : 
#define Audio_Az_Output_SD3LVI_Reserved_15_8_OFFSET      8
#define Audio_Az_Output_SD3LVI_Reserved_15_8_MASK        0xff00

typedef union {
  struct {
    UINT16                            LVI:8;
    UINT16                            Reserved_15_8:8;
  } Field;
  UINT16 Value;
} Audio_Az_Output_SD3LVI_STRUCT;

#define SMN_Audio_Az_Output_SD3LVI_ADDRESS    0x120016cUL


/***********************************************************
* Register Name : Audio_Az_Output_Stream_Payload_Capability
* Register Description :
* Output Stream Payload Capability
* Visibility : 0x2
************************************************************/

// Bitfield Description : Output Stream Payload Capability;
// Indicates the maximum number of Words per frame for any single output stream. This measurement is in 16-bit Word quantities per 48-kHz frame. The value must not be larger than the OUTPAY register value. Software must ensure that a format which would cause more Words per frame than indicated is not programmed into the Output Stream Descriptor Register.
// 00h: No Limit (Stream size is limited only by OUTPAY)
// 01h: 1 Word payload
// ...
// FFh: 255h Word payload
#define Audio_Az_Output_Stream_Payload_Capability_OUTSTRMPAY_OFFSET      0
#define Audio_Az_Output_Stream_Payload_Capability_OUTSTRMPAY_MASK        0xffff

typedef union {
  struct {
    UINT16                            OUTSTRMPAY:16;
  } Field;
  UINT16 Value;
} Audio_Az_Output_Stream_Payload_Capability_STRUCT;

#define SMN_Audio_Az_Output_Stream_Payload_Capability_ADDRESS    0x1200018UL


/***********************************************************
* Register Name : Audio_Az_RIRB_Control
* Register Description :
* RIRB Control
* Visibility : 0x2
************************************************************/

// Bitfield Description : Response Interrupt Control (RINTCTL):
// 0 = Disable Interrupt
// 1 = Generate an interrupt after N number of Responses are sent to the RIRB buffer or when an empty Response slot is encountered on all SDATA_IN_x inputs after a frame which returned a response (whichever occurs first). The N counter is reset when the interrupt is generated.
#define Audio_Az_RIRB_Control_RINTCTL_OFFSET      0
#define Audio_Az_RIRB_Control_RINTCTL_MASK        0x1

// Bitfield Description : RIRB DMA Enable (RIRBDMAEN):
// 0 = DMA Stop
// 1 = DMA Run (when Response queue not empty)
#define Audio_Az_RIRB_Control_RIRBDMAEN_OFFSET      1
#define Audio_Az_RIRB_Control_RIRBDMAEN_MASK        0x2

// Bitfield Description : Response Overrun Interrupt Control (RIRBOIC): If this bit is set, the hardware will generate an interrupt when the Response Overrun Interrupt Status bit is set.
#define Audio_Az_RIRB_Control_RIRBOIC_OFFSET      2
#define Audio_Az_RIRB_Control_RIRBOIC_MASK        0x4

// Bitfield Description : 
#define Audio_Az_RIRB_Control_Reserved_7_3_OFFSET      3
#define Audio_Az_RIRB_Control_Reserved_7_3_MASK        0xf8

typedef union {
  struct {
    UINT8                            RINTCTL:1;
    UINT8                            RIRBDMAEN:1;
    UINT8                            RIRBOIC:1;
    UINT8                            Reserved_7_3:5;
  } Field;
  UINT8 Value;
} Audio_Az_RIRB_Control_STRUCT;

#define SMN_Audio_Az_RIRB_Control_ADDRESS    0x120005cUL


/***********************************************************
* Register Name : Audio_Az_RIRB_Lower_Base_Address
* Register Description :
* RIRB Lower Base Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : RIRB Lower Base Unimplemented Bits:
// Hardwired to 0 to force 128-byte buffer alignment for cache line fetch optimizations.
#define Audio_Az_RIRB_Lower_Base_Address_RIRB_Lower_Base_Unimplemented_Bits_OFFSET      0
#define Audio_Az_RIRB_Lower_Base_Address_RIRB_Lower_Base_Unimplemented_Bits_MASK        0x7f

// Bitfield Description : RIRB Lower Base Address (RIRBLBASE):
// Lower address of the Response Input Ring Buffer, allowing the RIRB Base Address to be assigned on any 2-KB boundary. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted.
#define Audio_Az_RIRB_Lower_Base_Address_RIRBLBASE_OFFSET      7
#define Audio_Az_RIRB_Lower_Base_Address_RIRBLBASE_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            RIRB_Lower_Base_Unimplemented_Bits:7;
    UINT32                            RIRBLBASE:25;
  } Field;
  UINT32 Value;
} Audio_Az_RIRB_Lower_Base_Address_STRUCT;

#define SMN_Audio_Az_RIRB_Lower_Base_Address_ADDRESS    0x1200050UL


/***********************************************************
* Register Name : Audio_Az_RIRB_Response_Interrupt_Count
* Register Description :
* RIRB Response Interrupt Count
* Visibility : 0x2
************************************************************/

// Bitfield Description : N Response Interrupt Count (RINTCNT):
// 0000_0001b = 1 Response sent to RIRB
// ...
// 1111_1111b = 255 Responses sent to RIRB
// 0000_0000b = 256 Responses sent to RIRB
// The DMA engine should be stopped when changing this field or else an interrupt may be lost.
// Note that each Response occupies two Dwords in the RIRB.
// This is compared to the total number of responses that have been returned, as opposed to the number of frames in which there were responses. If more than one codec responds in one frame, then the count is increased by the number of responses received in the frame.
#define Audio_Az_RIRB_Response_Interrupt_Count_RINTCNT_OFFSET      0
#define Audio_Az_RIRB_Response_Interrupt_Count_RINTCNT_MASK        0xff

// Bitfield Description : 
#define Audio_Az_RIRB_Response_Interrupt_Count_Reserved_15_8_OFFSET      8
#define Audio_Az_RIRB_Response_Interrupt_Count_Reserved_15_8_MASK        0xff00

typedef union {
  struct {
    UINT16                            RINTCNT:8;
    UINT16                            Reserved_15_8:8;
  } Field;
  UINT16 Value;
} Audio_Az_RIRB_Response_Interrupt_Count_STRUCT;

#define SMN_Audio_Az_RIRB_Response_Interrupt_Count_ADDRESS    0x120005aUL


/***********************************************************
* Register Name : Audio_Az_RIRB_Size
* Register Description :
* RIRB Size
* Visibility : 0x2
************************************************************/

// Bitfield Description : RIRB Size (RIRBSIZE):
// The setting of the register determines when the address counter in the DMA controller will wrap around.
// 00 --- 16B = 2 entries
// 01 --- 128B = 16 entries
// 10 --- 2KB = 256 entries
// 11 --- Reserved. This value must not be changed when the RIRB DMA engine is enabled.
// Setting this field to an unsupported size will produce unspecified results. When only one RIRB Size is supported it is permissible to make this field Read Only (RO)
#define Audio_Az_RIRB_Size_RIRBSIZE_OFFSET      0
#define Audio_Az_RIRB_Size_RIRBSIZE_MASK        0x3

// Bitfield Description : 
#define Audio_Az_RIRB_Size_Reserved_3_2_OFFSET      2
#define Audio_Az_RIRB_Size_Reserved_3_2_MASK        0xc

// Bitfield Description : RIRB Size Capability (RIRBSZCAP):
// A bit mask identifying the possible sizes of the RIRB.
// 0001b --- 16 B = 2 entries
// 0010b --- 128 B = 16 entries
// 0100b --- 2048 B = 256 Entries
// 1000b --- Reserved
// This implemented as a bit mask; for example, if the controller supported two entries and 256 entries, this register would be Read Only 0101b.
// There is no requirement to support more than one RIRB Size
#define Audio_Az_RIRB_Size_RIRBSZCAP_OFFSET      4
#define Audio_Az_RIRB_Size_RIRBSZCAP_MASK        0xf0

typedef union {
  struct {
    UINT8                            RIRBSIZE:2;
    UINT8                            Reserved_3_2:2;
    UINT8                            RIRBSZCAP:4;
  } Field;
  UINT8 Value;
} Audio_Az_RIRB_Size_STRUCT;

#define SMN_Audio_Az_RIRB_Size_ADDRESS    0x120005eUL


/***********************************************************
* Register Name : Audio_Az_RIRB_Status
* Register Description :
* RIRB Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : Response Overrun Interrupt Status (RIRBOIS):
// Hardware sets this bit to a 1 when an overrun occurs in the RIRB. An interrupt may be generated if the Response Overrun Interrupt Control bit is set.
// This bit will be set if the RIRB DMA engine is not able to write the incoming responses to memory before additional incoming responses overrun the internal FIFO.
// When hardware detects an overrun, it will drop the responses which overrun the buffer and set the RIRBOIS status bit to indicate the error condition. Optionally, if the RIRBOIC is set, the hardware will also generate an error to alert software to the problem.
// Software clears this bit by writing a 1 to it.
#define Audio_Az_RIRB_Status_RINTFL_OFFSET      0
#define Audio_Az_RIRB_Status_RINTFL_MASK        0x1

// Bitfield Description : 
#define Audio_Az_RIRB_Status_Reserved_1_1_OFFSET      1
#define Audio_Az_RIRB_Status_Reserved_1_1_MASK        0x2

// Bitfield Description : Response Interrupt (RINTFL):
// Hardware sets this bit to a 1 when an interrupt has been generated after N number of Responses are sent to the RIRB buffer or when an empty Response slot is encountered on all SDATA_IN[x] inputs (whichever occurs first). Software clears this flag by writing a 1 to this bit
#define Audio_Az_RIRB_Status_RIRBOIS_OFFSET      2
#define Audio_Az_RIRB_Status_RIRBOIS_MASK        0x4

// Bitfield Description : 
#define Audio_Az_RIRB_Status_Reserved_7_3_OFFSET      3
#define Audio_Az_RIRB_Status_Reserved_7_3_MASK        0xf8

typedef union {
  struct {
    UINT8                            RINTFL:1;
    UINT8                            Reserved_1_1:1;
    UINT8                            RIRBOIS:1;
    UINT8                            Reserved_7_3:5;
  } Field;
  UINT8 Value;
} Audio_Az_RIRB_Status_STRUCT;

#define SMN_Audio_Az_RIRB_Status_ADDRESS    0x120005dUL


/***********************************************************
* Register Name : Audio_Az_RIRB_Upper_Base_Address
* Register Description :
* RIRB_Upper_Base_Address
* Visibility : 0x2
************************************************************/

// Bitfield Description : RIRB Upper Base Address (RIRBUBASE):
// Upper 32 address bits of the Response Input Ring Buffer. This register field must not be written when the DMA engine is running or the DMA transfer may be corrupted. This register is Reserved, Read-only 0 if the 64OK (64-bit OK) bit indicates that the controller does not support 64-bit addressing.
#define Audio_Az_RIRB_Upper_Base_Address_RIRBUBASE_OFFSET      0
#define Audio_Az_RIRB_Upper_Base_Address_RIRBUBASE_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            RIRBUBASE:32;
  } Field;
  UINT32 Value;
} Audio_Az_RIRB_Upper_Base_Address_STRUCT;

#define SMN_Audio_Az_RIRB_Upper_Base_Address_ADDRESS    0x1200054UL


/***********************************************************
* Register Name : Audio_Az_RIRB_Write_Pointer
* Register Description :
* RIRB Write Pointer
* Visibility : 0x2
************************************************************/

// Bitfield Description : RIRB Write Pointer (RIRBWP):
// Indicates the last valid RIRB entry written by the DMA controller. Software reads this field to determine how many responses it can read from the RIRB. The value read indicates the RIRB Write Pointer offset in two Dword units (since each RIRB entry is two Dwords long). Supports up to 256 RIRB entries (256 x 8 B = 2 KB) in the cyclic RIRB buffer. This field may be read while the DMA engine is running
#define Audio_Az_RIRB_Write_Pointer_RIRBWP_OFFSET      0
#define Audio_Az_RIRB_Write_Pointer_RIRBWP_MASK        0xff

// Bitfield Description : 
#define Audio_Az_RIRB_Write_Pointer_Reserved_14_8_OFFSET      8
#define Audio_Az_RIRB_Write_Pointer_Reserved_14_8_MASK        0x7f00

// Bitfield Description : RIRB Write Pointer Reset (RIRBWPRST):
// Software writes a 1 to this bit to reset the RIRB Write Pointer and to 0.s. The DMA engine must be stopped prior to resetting the Write Pointer or else DMA transfer may be corrupted. This bit will always be read as 0.
#define Audio_Az_RIRB_Write_Pointer_RIRBWPRST_OFFSET      15
#define Audio_Az_RIRB_Write_Pointer_RIRBWPRST_MASK        0x8000

typedef union {
  struct {
    UINT16                            RIRBWP:8;
    UINT16                            Reserved_14_8:7;
    UINT16                            RIRBWPRST:1;
  } Field;
  UINT16 Value;
} Audio_Az_RIRB_Write_Pointer_STRUCT;

#define SMN_Audio_Az_RIRB_Write_Pointer_ADDRESS    0x1200058UL


/***********************************************************
* Register Name : Audio_Az_State_Change_Status
* Register Description :
* State Change Status
* Visibility : 0x2
************************************************************/

// Bitfield Description : SDIN State Change Status Flags;
// Flag bits that indicate which SDIN signal(s) received a "State Change" event. The bits are cleared by writing 1's to them. The SDATA_IN[0] line corresponds to bit 0, etc.
// These bits are only cleared by a power-on reset.
#define Audio_Az_State_Change_Status_SDIWEN_OFFSET      0
#define Audio_Az_State_Change_Status_SDIWEN_MASK        0x7fff

// Bitfield Description : 
#define Audio_Az_State_Change_Status_Reserved_15_15_OFFSET      15
#define Audio_Az_State_Change_Status_Reserved_15_15_MASK        0x8000

typedef union {
  struct {
    UINT16                            SDIWEN:15;
    UINT16                            Reserved_15_15:1;
  } Field;
  UINT16 Value;
} Audio_Az_State_Change_Status_STRUCT;

#define SMN_Audio_Az_State_Change_Status_ADDRESS    0x120000eUL


/***********************************************************
* Register Name : Audio_Az_Stream_Synchronization
* Register Description :
* Stream_Synchronization_Bits
* Visibility : 0x2
************************************************************/

// Bitfield Description : Stream Synchronization Bits (SSYNC):
// The Stream Synchronization bits, when set, block data from being sent on or received from the link. Each bit controls the associated Stream Descriptor; bit 0 corresponds to the first Stream Descriptor, etc.
// To synchronously start a set of DMA engines, the bits in the SSYNC register are set to a 1. The RUN bits for the associated Stream Descriptors can be set to a 1 to start the DMA engines. When all streams are ready, the associated SSYNC bits can all be set to 0 at the same time, and transmission or reception of bits to or from the link will begin together at the start of the next full link frame.
// To synchronously stop streams, the bits are set in the SSYNC register, and the RUN bits in the Stream Descriptors are cleared by software.
#define Audio_Az_Stream_Synchronization_SSYNC_OFFSET      0
#define Audio_Az_Stream_Synchronization_SSYNC_MASK        0x3fffffff

// Bitfield Description : 
#define Audio_Az_Stream_Synchronization_Reserved_31_30_OFFSET      30
#define Audio_Az_Stream_Synchronization_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            SSYNC:30;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} Audio_Az_Stream_Synchronization_STRUCT;

#define SMN_Audio_Az_Stream_Synchronization_ADDRESS    0x1200038UL


/***********************************************************
* Register Name : Audio_Az_Wake_Enable
* Register Description :
* Wake Enable
* Visibility : 0x2
************************************************************/

// Bitfield Description : SDIN Wake Enable Flags;
// Bits that control which SDIN signal(s) may generate a wake event or processor interrupt in response to a codec State Change request. A 1 bit in the bit mask indicates that the associated SDIN signal is enabled to generate a wake or processor interrupt. The SDATA_IN[0] signal corresponds to bit 0, etc.
// These bits are only cleared by a power-on reset. Software must make no assumptions about how these bits are set and set them appropriately.
#define Audio_Az_Wake_Enable_SDIWEN_OFFSET      0
#define Audio_Az_Wake_Enable_SDIWEN_MASK        0x7fff

// Bitfield Description : 
#define Audio_Az_Wake_Enable_Reserved_15_15_OFFSET      15
#define Audio_Az_Wake_Enable_Reserved_15_15_MASK        0x8000

typedef union {
  struct {
    UINT16                            SDIWEN:15;
    UINT16                            Reserved_15_15:1;
  } Field;
  UINT16 Value;
} Audio_Az_Wake_Enable_STRUCT;

#define SMN_Audio_Az_Wake_Enable_ADDRESS    0x120000cUL


/***********************************************************
* Register Name : Audio_Az_Wall_Clock_Counter
* Register Description :
* Wall Clock Counter
* Visibility : 0x2
************************************************************/

// Bitfield Description : Wall Clock Counter (Counter):
// 32 bit counter that is incremented at the link bitclock rate and rolls over from FFFF_FFFFh to 0000_0000h. This counter will roll over to 0 with a period of approximately 179 seconds with the nominal 24-MHz bitclock rate.
// This counter is enabled while the BCLK bit is set to 1. Software uses this counter to synchronize between multiple controllers. The counter will be reset on controller reset.
#define Audio_Az_Wall_Clock_Counter_Wall_Clock_Counter_OFFSET      0
#define Audio_Az_Wall_Clock_Counter_Wall_Clock_Counter_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Wall_Clock_Counter:32;
  } Field;
  UINT32 Value;
} Audio_Az_Wall_Clock_Counter_STRUCT;

#define SMN_Audio_Az_Wall_Clock_Counter_ADDRESS    0x1200030UL

#endif /* _ACP_AZALIAIP_H_ */
