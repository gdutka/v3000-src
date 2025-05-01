#
# This file is be exexcute PatchPspDir.py, Please follow Python language
# If using PatchPspDir.exe, it only support already used python library in PatchPspDir.py.
# Therefore, you can use os.environ to assign different table.
#
import os

if os.environ.get ('VN_16M_AB_CFG') == 'YES':
    #
    # 16M BIOS layout doesn't contain MPM firmware
    #
    PcdList.PcdList['PcdFlashMpmEntry85Base'] = '0x0'
    PcdList.PcdList['PcdFlashMpmEntry85Size'] = '0x0'
    PcdList.PcdList['PcdFlashMpmEntry86Base'] = '0x0'
    PcdList.PcdList['PcdFlashMpmEntry86Size'] = '0x0'
    PcdList.PcdList['PcdFlashMpmEntry89Base'] = '0x0'
    PcdList.PcdList['PcdFlashMpmEntry89Size'] = '0x0'
    PcdList.PcdList['PcdFlashWlanFwBase'] = '0x0'
    PcdList.PcdList['PcdFlashWlanFwSize'] = '0x0'

if os.environ.get ('BOOT_FROM_ROM') != 'YES':
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


        # PSP L1 A / B
        ('PSP_DIR[@Level==0x1][1]'                            ,  'Base'    , '0x%08X' % (PcdList['PcdFlashPSPL1HdrBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==0x1][1]/POINT_ENTRY[@Type==0x48]'   ,  'Address' , '0x%08X' % (PcdList['PcdFlashImageSlot1HdrBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==0x1][1]/POINT_ENTRY[@Type==0x4a]'   ,  'Address' , '0x%08X' % (PcdList['PcdFlashImageSlot2HdrBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==0x1][2]'                            ,  'Base'    , '0x%08X' % (PcdList['PcdFlashBackupPSPL1HdrBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==0x1][2]/POINT_ENTRY[@Type==0x48]'   ,  'Address' , '0x%08X' % (PcdList['PcdFlashImageSlot1HdrBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==0x1][2]/POINT_ENTRY[@Type==0x4a]'   ,  'Address' , '0x%08X' % (PcdList['PcdFlashImageSlot2HdrBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),

        # ISH A / B, backup A
        ('ISH_HEADER[1]'                                      ,  'Base'    , '0x%08X' % (PcdList['PcdFlashImageSlot1HdrBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('ISH_HEADER[1]'                                      ,  'Location', '0x%08X' % (PcdList['PcdFlashPspDirABase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('ISH_HEADER[2]'                                      ,  'Base'    , '0x%08X' % (PcdList['PcdFlashImageSlot2HdrBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('ISH_HEADER[2]'                                      ,  'Location', '0x%08X' % (PcdList['PcdFlashPspDirBBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('ISH_HEADER[3]'                                      ,  'Base'    , '0x%08X' % (PcdList['PcdFlashImageSlot2HdrBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'] + 0x100)),
        ('ISH_HEADER[3]'                                      ,  'Location', '0x%08X' % (PcdList['PcdFlashPspDirABase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),

        # PSP L2 A / B
        ('PSP_DIR[@Level==2][1]'                              ,  'Base'    , '0x%08X' % (PcdList['PcdFlashPspDirABase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]'                              ,  'Size'    , '0x%08X' % (PcdList['PcdFlashPspDirASize'])),
        ('PSP_DIR[@Level==2][2]'                              ,  'Base'    , '0x%08X' % (PcdList['PcdFlashPspDirBBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][2]'                              ,  'Size'    , '0x%08X' % (PcdList['PcdFlashPspDirBSize'])),

        # PSP L2 A / B - PSP NVram
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x04]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashPspNvRamBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x04]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashPspNvRamSize'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x04]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashPspNvRamBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x04]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashPspNvRamSize'])),

        # PSP L2 A / B - PSP RPMC reserved 0x54
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x54]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashPspRpmcNvRamBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x54]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashPspRpmcNvRamSize'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x54]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashPspRpmcNvRamBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x54]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashPspRpmcNvRamSize'])),

#[-start-231102-IB20840032-remove]#
        # PSP L2 A / B - MPM AMF 0x86 FW Part2
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x86 and @Instance==0x0]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashMpmEntry86Base'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x86 and @Instance==0x0]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashMpmEntry86Size'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x86 and @Instance==0x1]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashMpmEntry86BaseB'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x86 and @Instance==0x1]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashMpmEntry86Size'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x86 and @Instance==0x0]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashMpmEntry86Base'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x86 and @Instance==0x0]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashMpmEntry86Size'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x86 and @Instance==0x1]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashMpmEntry86BaseB'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x86 and @Instance==0x1]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashMpmEntry86Size'])),

        # PSP L2 A / B - MPM 0x88 Wlan Firmware
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x88 and @Instance==0x0]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashWlanFwBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x88 and @Instance==0x0]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashWlanFwSize'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x88 and @Instance==0x1]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashWlanFwBaseB'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x88 and @Instance==0x1]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashWlanFwSize'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x88 and @Instance==0x0]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashWlanFwBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x88 and @Instance==0x0]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashWlanFwSize'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x88 and @Instance==0x1]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashWlanFwBaseB'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x88 and @Instance==0x1]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashWlanFwSize'])),

        # PSP L2 A / B - MPM 0x87 Persisent Object
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x87]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashMpmEntry87Base'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x87]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashMpmEntry87Size'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x87]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashMpmEntry87Base'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x87]'     ,  'Size'    , '0x%08X' % (PcdList['PcdFlashMpmEntry87Size'])),
#[-end-231102-IB20840032-remove]#

        # PSP L2 A / B - Point to BIOS Hdr
        # DIRS.AddressMode is relative mode (2), so BIOS A will according to PSP L2 A / B size
        ('PSP_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x49]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashPspDirASize'])),
        ('PSP_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x49]'     ,  'Address' , '0x%08X' % (PcdList['PcdFlashPspDirBSize'])),

        # BIOS L2 A
        ('//BIOS_DIR[@Level==2][1]'                             ,  'Base'    , '0x%08X' % (PcdList['PcdFlashBiosDirABase']- PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('//BIOS_DIR[@Level==2][1]'                             ,  'Size'    , '0x%08X' % (PcdList['PcdFlashBiosDirASize'])),
        ('//BIOS_DIR[@Level==2][2]'                             ,  'Base'    , '0x%08X' % (PcdList['PcdFlashBiosDirBBase']- PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('//BIOS_DIR[@Level==2][2]'                             ,  'Size'    , '0x%08X' % (PcdList['PcdFlashBiosDirBSize'])),

        # BIOS L2 A PEI entry point
        ('BIOS_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x62 and @Instance==0x0]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashFvRecoveryBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('BIOS_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x62 and @Instance==0x1]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashPeiFlashBBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),

        ('BIOS_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x62 and @Instance==0x0]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashFvRecoveryBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('BIOS_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x62 and @Instance==0x1]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashPeiFlashBBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),

        # BIOS L2 A / B NV storage
        ('BIOS_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x6D][1]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashNvStorageVariableBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('BIOS_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x6D][2]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashNvStorageFtwWorkingBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('BIOS_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x6D][3]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashNvStorageFtwSpareBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('BIOS_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x6D][4]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashNvStorageDmiBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('BIOS_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x6D][5]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashNvStorageMsdmDataBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('BIOS_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x6D][1]'    ,  'Size'    , '0x%08X' % (PcdList['PcdFlashNvStorageVariableSize'])),
        ('BIOS_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x6D][2]'    ,  'Size'    , '0x%08X' % (PcdList['PcdFlashNvStorageFtwWorkingSize'])),
        ('BIOS_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x6D][3]'    ,  'Size'    , '0x%08X' % (PcdList['PcdFlashNvStorageFtwSpareSize'])),
        ('BIOS_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x6D][4]'    ,  'Size'    , '0x%08X' % (PcdList['PcdFlashNvStorageDmiSize'])),
        ('BIOS_DIR[@Level==2][1]/POINT_ENTRY[@Type==0x6D][5]'    ,  'Size'    , '0x%08X' % (PcdList['PcdFlashNvStorageMsdmDataSize'])),

        ('BIOS_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x6D][1]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashNvStorageVariableBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('BIOS_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x6D][2]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashNvStorageFtwWorkingBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('BIOS_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x6D][3]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashNvStorageFtwSpareBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('BIOS_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x6D][4]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashNvStorageDmiBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('BIOS_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x6D][5]'    ,  'Address' , '0x%08X' % (PcdList['PcdFlashNvStorageMsdmDataBase'] - PcdList['PcdH2OFlashAreaMainBaseAddress'])),
        ('BIOS_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x6D][1]'    ,  'Size'    , '0x%08X' % (PcdList['PcdFlashNvStorageVariableSize'])),
        ('BIOS_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x6D][2]'    ,  'Size'    , '0x%08X' % (PcdList['PcdFlashNvStorageFtwWorkingSize'])),
        ('BIOS_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x6D][3]'    ,  'Size'    , '0x%08X' % (PcdList['PcdFlashNvStorageFtwSpareSize'])),
        ('BIOS_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x6D][4]'    ,  'Size'    , '0x%08X' % (PcdList['PcdFlashNvStorageDmiSize'])),
        ('BIOS_DIR[@Level==2][2]/POINT_ENTRY[@Type==0x6D][5]'    ,  'Size'    , '0x%08X' % (PcdList['PcdFlashNvStorageMsdmDataSize'])),

    ]
else:
    MapTable = []

if os.environ.get ('VN_16M_AB_CFG') == 'YES':
    #
    # 16M BIOS layout doesn't contain MPM firmware
    #
    NewMapTable = []
    for Item in MapTable:
        if '@Type==0x85' in Item[0] or \
           '@Type==0x86' in Item[0] or \
           '@Type==0x88' in Item[0] or \
           '@Type==0x89' in Item[0]:
           continue
        NewMapTable.append (Item)
    MapTable = NewMapTable



#
# don't remove
#
gMapTable.extend(MapTable)