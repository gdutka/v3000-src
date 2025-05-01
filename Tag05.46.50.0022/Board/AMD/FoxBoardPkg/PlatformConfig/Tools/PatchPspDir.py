from lxml import etree
import re
import sys
import os
import traceback
import argparse
# Python 3.4 above support pathlib, so Python 2.7 will fail 
# use pathlib2 to compatiable 2.7/3.7
from pathlib2 import Path 

def StrToNum (Str):
  if (Str != None):
    if type(Str) is str:
      if (Str.endswith ('L')):
        Str = Str.rstrip ('L')
      num = int(eval(Str))
      return num
    elif type(Str) is int:
      return Str
    else:
      raise ValueError ("unexpected Data Type {}".format(type(Str)))

#
# Because lxml only support xpath 1.0 spec, it doesn't support [@attrib==hex-value] (2.0 spec)
# Useing lxml xpath extension functions tonormalize hex/decimal string to integer string 
# and then can compare integer string
# usgae: "[val(@attrib)=val(hex-value)]"
# 
# https://lxml.de/2.0/extensions.html
# 
def NormalizeStrValue(context, a):
    return str (StrToNum(str(a[0])) if (type(a) == list) else StrToNum(str(a)))

class PcdListClass:
    def __init__ (self, FilePath):
        self.PcdList = {}
        self.FilePath = FilePath
        #
        # PcdList format: 
        #   PcdTokenGuid.PcdTokenName: PcdValue
        with open (FilePath, 'r') as File:
            for Line in File.readlines():
                Line = Line.strip()
                if Line == "":
                    continue
                Pcd = Line.split(': ')
                self.PcdList[Pcd[0]] = Pcd[1]
                if Pcd[0].split('.')[1] in self.PcdList:
                    self.PcdList[Pcd[0].split('.')[1]] = "duplicate"
                else:
                    self.PcdList[Pcd[0].split('.')[1]] = Pcd[1] 
    
        
    def __getitem__(self, Key):
        if Key not in self.PcdList:
            raise ValueError ("%s isn't in %s" % (Key, self.FilePath))
        if self.PcdList[Key] == "duplicate":
            raise ValueError ("%s is duplicated PcdTokenName" % Key)
        return StrToNum(self.PcdList[Key])

args = None
def main():
    global args 
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--PcdListFilePath",   type=lambda p: str(Path(p).absolute()), required=True,  help="PcdList.txt path")
    parser.add_argument("-i", "--InPspDirFilePath",  type=lambda p: str(Path(p).absolute()), required=True,  help="intput BIOSImageDirectory.xml file")
    parser.add_argument("-t", "--PspPcdMapTable",    type=lambda p: Path(p).absolute(), required=True,  help="Psp PCD map table")
    parser.add_argument("-o", "--OutPspDirFilePath", type=lambda p: str(Path(p).absolute()), required=False, help="output BIOSImageDirectory.xml file")    

    args = parser.parse_args()
    if args.OutPspDirFilePath == None:
        args.OutPspDirFilePath = args.InPspDirFilePath
       
    PcdList = PcdListClass(args.PcdListFilePath)
    PspDirTree = etree.parse(args.InPspDirFilePath)
    root = PspDirTree.getroot()

    # format: (PspDir node xpath, PspDir node attribute, value(need str type))
    MapTable = []
    with open(str(args.PspPcdMapTable)) as f:
        exec(compile(f.read(), args.PspPcdMapTable.name, "exec"), globals(), {'PcdList': PcdList, 'gMapTable':MapTable})
    
    # add val() function to normalize hex/decimal string to integer string 
    etree.FunctionNamespace(None)['val'] = NormalizeStrValue

    gSubValueCompareRe = re.compile (r'(@\w+)==(\"?\w+\"?)')
    for Map in MapTable:
        # convert [@attrib==hex-value] to [val(@attrib)=val(hex-value)] 
        PspPath = gSubValueCompareRe.sub (r'val(\1)=val("\2")',  Map[0])

        Item = PspDirTree.xpath (PspPath)
        if Item is None or len(Item) == 0:
            raise ValueError ("Not found '%s' path in %s" % (Map[0]))
        if Map[1] not in Item[0].attrib:
            raise ValueError ("Not found '%s' attribute by path '%s'" % (Map[1], Map[0]))
        Item[0].attrib[Map[1]] = Map[2]

    open(args.OutPspDirFilePath, 'wb').write (etree.tostring(root, xml_declaration=True, encoding='ascii').replace(b'\n', b'\r\n'))

    
if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        exc_type, exc_value, exc_traceback = sys.exc_info()
        ErrorMsg = ''.join(traceback.format_exception(exc_type, exc_value, exc_traceback))
        print (ErrorMsg)
        if args != None:
            print ("Please check %s and %s file" % (args.PspPcdMapTable, args.InPspDirFilePath))
        sys.exit (-1)
        
