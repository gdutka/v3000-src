#
# This file is be exexcute PatchPspDir.py, Please follow Python language
# If using PatchPspDir.exe, it only support already used python library in PatchPspDir.py.
# Therefore, you can use os.environ to assign different table.
#
import os

if os.environ.get ('L2A_ONLY') != 'FALSE':
    MapTable = [
        #
        # format:
        #   (BIOS Image Directory xpath, node attribute, attribute value)
        #
        # python lxml support xpath 1.0 and we enhance to support [@attribute==integer] method (xpath 2.0)
        #
        # https://www.w3.org/TR/1999/REC-xpath-19991116/#NT-RelationalExpr
        # +------------+-------------------------------------------------------------------------------------------------------+
        # | Expression |                                              Description                                              |
        # +------------+-------------------------------------------------------------------------------------------------------+
        # | nodename   | Selects all nodes with the name "nodename"                                                            |
        # | /          | Selects from the root node                                                                            |
        # | //         | Selects nodes in the document from the current node that match the selection no matter where they are |
        # | .          | Selects the current node                                                                              |
        # | ..         | Selects the parent of the current node                                                                |
        # | @          | Selects attributes                                                                                    |
        # +------------+-------------------------------------------------------------------------------------------------------+

        # +------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+
        # |          Path Expression           |                                                                 Result                                                                 |
        # +------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+
        # | /bookstore/book[1]                 | Selects the first book element that is the child of the bookstore element.                                                             |
        # | /bookstore/book[last()]            | Selects the last book element that is the child of the bookstore element                                                               |
        # | /bookstore/book[last()-1]          | Selects the last but one book element that is the child of the bookstore element                                                       |
        # | /bookstore/book[position()<3]      | Selects the first two book elements that are children of the bookstore element                                                         |
        # | //title[@lang]                     | Selects all the title elements that have an attribute named lang                                                                       |
        # | //title[@lang='en']                | Selects all the title elements that have a "lang" attribute with a value of "en"                                                       |
        # | /bookstore/book[price>35.00]       | Selects all the book elements of the bookstore element that have a price element with a value greater than 35.00                       |
        # | /bookstore/book[price>35.00]/title | Selects all the title elements of the book elements of the bookstore element that have a price element with a value greater than 35.00 |
        # +------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------+


        # ISH A / B, backup A
        ('ISH_HEADER[1]'                                      ,  'Base'    , '0x%08X' % (PcdList['PcdFlashImageSlot1HdrBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('ISH_HEADER[1]'                                      ,  'Location', '0x%08X' % (PcdList['PcdFlashPspDirABase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),

        # PSP L2 A / B
        ('PSP_DIR[@Level==2][1]'                              ,  'Base'    , '0x%08X' % (PcdList['PcdFlashPspDirABase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]'                              ,  'Size'    , '0x%08X' % (PcdList['PcdFlashPspDirASize'])),

        # PSP L2 A / B - PSP NVram
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x04]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashPspNvRamBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x04]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashPspNvRamSize'])),

        # PSP L2 A / B - PSP RPMC reserved 0x54
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x54]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashPspRpmcNvRamBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x54]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashPspRpmcNvRamSize'])),

        # PSP L2 A / B - MPM AMF FW Part2
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x86]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashMpmEntry86Base'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x86]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashMpmEntry86Size'])),

        # PSP L2 A / B - Wlan Firmware
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x88]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashWlanFwBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x88]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashWlanFwSize'])),

        # PSP L2 A / B - Point to BIOS Hdr
        # DIRS.AddressMode is relative mode (2), so BIOS A will according to PSP L2 A / B size
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x49]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashPspDirASize'])),

        # BIOS L2 A
        ('//BIOS_DIR[@Level==2][1]'                             ,  'Base'    , '0x%08X' % (PcdList['PcdFlashBiosDirABase']- PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('//BIOS_DIR[@Level==2][1]'                             ,  'Size'    , '0x%08X' % (PcdList['PcdFlashBiosDirASize'])),
    ]
else:
    MapTable = []



#
# don't remove
#
gMapTable.extend(MapTable)
