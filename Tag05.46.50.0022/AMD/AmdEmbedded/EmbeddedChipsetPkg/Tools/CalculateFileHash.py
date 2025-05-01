from lxml import etree
import re
import sys
import os
import traceback
import argparse
import hashlib
from pathlib2 import Path

args = None
def main():
    global args 
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--FilePath",   type=lambda p: str(Path(p).absolute()), required=True,  help="intput file")
    parser.add_argument("-o", "--HashFilePath",  type=lambda p: str(Path(p).absolute()), required=True,  help="output hash file")

    args = parser.parse_args()
       
    BinaryFile = open(args.FilePath, 'r+b')
    HashFile = open(args.HashFilePath,'wb')

    BinaryFileBytes = BinaryFile.read()
    hash = hashlib.sha256(BinaryFileBytes);

    HashFile.write(hash.digest())

    BinaryFile.close()
    HashFile.close()

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        exc_type, exc_value, exc_traceback = sys.exc_info()
        ErrorMsg = ''.join(traceback.format_exception(exc_type, exc_value, exc_traceback))
        print (ErrorMsg)
        if args != None:
            print ("Please check %s and %s file" % (args.FilePath, args.HashFilePath))
        sys.exit (-1)
        
