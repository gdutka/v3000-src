from lxml import etree
import re
import sys
import os
import traceback
import argparse
from pathlib2 import Path 

args = None
def main():
    global args 
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-b", "--BiosFilePath",   type=lambda p: str(Path(p).absolute()), required=True,  help="intput BIOS file")
    parser.add_argument("-p", "--PatchFilePath",  type=lambda p: str(Path(p).absolute()), required=True,  help="intput patch file")
    parser.add_argument("-a", "--PatchAddr",  type=lambda x: int(x,0), required=True,  help="patch address")   

    args = parser.parse_args()
       
    FdFile = open(args.BiosFilePath, 'r+b')
    PatchFile = open(args.PatchFilePath,'rb')

    PatchFile.seek(0,2)
    PatchFileSize = PatchFile.tell()
    PatchFile.seek(0)

    Patch = PatchFile.read(PatchFileSize)

    FdFile.seek(args.PatchAddr)
    FdFile.write(Patch)

    FdFile.close()
    PatchFile.close()

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        exc_type, exc_value, exc_traceback = sys.exc_info()
        ErrorMsg = ''.join(traceback.format_exception(exc_type, exc_value, exc_traceback))
        print (ErrorMsg)
        if args != None:
            print ("Please check %s and %s file" % (args.BiosFilePath, args.PatchFilePath))
        sys.exit (-1)
        
