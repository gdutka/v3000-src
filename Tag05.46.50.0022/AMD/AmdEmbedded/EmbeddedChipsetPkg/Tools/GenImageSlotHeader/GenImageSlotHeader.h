
UINT32
Fletcher32 (
  IN OUT   UINT16  *data,
  IN       UINTN   words
  );

#pragma pack (push, 1)

#define RMB_PSP_ID    0xbc0d0200

typedef struct {
    UINT32     Checksum;            // [0x0000] 32 bit CRC of header items below and the entire table
    UINT32     BootPriority;        // [0x0004] Pickup largest value from all slot_headers as preferred image slot, value 0x0 means unbootable
    UINT32     UpdateRetries;       // [0x0008] MSB is the additional boot attempts that are allowed before an uninitialized image is considered unbootable, 
                                    //          default filled with 1s and right shift 1 bit for each trial. E.g. 0x00000003 -> 0x00000001 (Maximum tries: 32)
    UINT8      GlitchRetries;       // [0x000C] MSB is the additional warm boot attempts when fuse BOOT_MAX_RETRIES=0, otherwise fuse value of BOOT_MAX_RETRIES is the apptempst 
                                    //          that are allowed in order to mitigate electrical glitch before an normal intialized image is considered unbootable,
                                    //          default filled with 1s and check against warm reset persistent register (1<<MP0_GLITCH_RETRY_COUNTER[n]). 
                                    //          MP0_GLITCH_RETRY_COUNTER[n] is increased for each trial of the image, and cleared by SW. (Maximum tries: 8)
    UINT8      Reserved[3];         // [0x000D] Reserved
    UINT32     Location;            // [0x0010] Absolute address of PSP directory table for current slot image
    UINT32     PspId;               // [0x0014] PSP_ID to support multiple SOCs
    UINT32     SlotMaxSize;         // [0x0018] Maximum image size allowed to program into the slot
    UINT32     Reserved_1;          // [0x001C] Reserved
} IMAGE_SLOT_HEADER;                // [0x0020] 

#pragma pack (pop)