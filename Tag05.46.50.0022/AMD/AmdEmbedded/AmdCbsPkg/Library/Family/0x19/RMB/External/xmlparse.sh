#call CBS XML parse tool
CBS_LIB_FAMILY_PATH=Library/Family/0x19/RMB/External
CBS_AGESA_PATH=$WORKSPACE/$CHIPSET_REL_PATH/AgesaModulePkg/Include
CBS_BUILD_PATH=$WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/Build
if [ ! -d "$CBS_AGESA_PATH" ]; then
  CBS_AGESA_PATH=$WORKSPACE/AGESA/AgesaModulePkg/Include
fi
FIRMWARE_PATH=$WORKSPACE/$CHIPSET_REL_PATH/AgesaPkg/Addendum/Apcb/Inc/RMB
if [ ! -d "$FIRMWARE_PATH" ]; then
  FIRMWARE_PATH=$WORKSPACE/AGESA/AgesaPkg/Addendum/Apcb/Inc/RMB
fi

if [ "$PERL_PATH" = "" ]; then
  PERL_PATH=$WORKSPACE/$CHIPSET_REL_PATH/AgesaCbsPkg/Tools/Perl/bin
fi

PERL_LIB_PATH=$PERL_PATH/../lib

if [ "$CBS_VENDER_STRING" = "" ]; then
  CBS_VENDER_STRING=NULL
fi

if [ "$CBS_PROMONTORY_21_SUPPORT" = "" ]; then
  CBS_PROMONTORY_21_SUPPORT=1
fi

if [ "$CBS_TURNER_SUPPORT" = "" ]; then
  CBS_TURNER_SUPPORT=0
fi

if [ "$1" = "" ]; then
  if [ -d "$CBS_BUILD_PATH/ResourceRMB/AmdCbsVariable.h" ]; then
    rmdir -p $CBS_BUILD_PATH/ResourceRMB
#    mkdir -p $CBS_BUILD_PATH/ResourceRMB
  fi
  perl -I$WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/Tools -I$PERL_LIB_PATH $WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/Tools/CBSgenerate.pl -i $WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/$CBS_LIB_FAMILY_PATH/Rembrandt/eSetupRembrandt.xml -o $CBS_BUILD_PATH/ResourceRMB --version external -b enable -t 0 -x 0 -r 0 -y $CBS_VENDER_STRING --cpt21Support $CBS_PROMONTORY_21_SUPPORT --cTnrSupport $CBS_TURNER_SUPPORT
  #Only copy files if they differ.  Prevents needless rebuild of files.
  if [ -f "$CBS_BUILD_PATH/ResourceRS/IdsNvDefRS.h" ]; then
    DIFF=$(diff $CBS_BUILD_PATH/ResourceRMB/IdsNvDefRMB.h $CBS_AGESA_PATH/IdsNvDefRMB.h)
    if [ "$DIFF" != "" ]; then
      cp $CBS_BUILD_PATH/ResourceRMB/IdsNvDefRMB.h $CBS_AGESA_PATH/IdsNvDefRMB.h
    fi
  else
    cp $CBS_BUILD_PATH/ResourceRMB/IdsNvDefRMB.h $CBS_AGESA_PATH/IdsNvDefRMB.h
  fi
fi
if [ "$1" = "EMBFP7" ]; then
  if [ -d "$CBS_BUILD_PATH/ResourceRMB/AmdCbsVariable.h" ]; then
    rmdir -p $CBS_BUILD_PATH/ResourceRMB
#    mkdir -p $CBS_BUILD_PATH/ResourceRMB
  fi
  perl -I$WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/Tools -I$PERL_LIB_PATH $WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/Tools/CBSgenerate.pl -i $WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/$CBS_LIB_FAMILY_PATH/Rembrandt/eSetupFox.xml -o $CBS_BUILD_PATH/ResourceRMB --version external -b enable -t 0 -x 0 -r 0 -y $CBS_VENDER_STRING --cpt21Support $CBS_PROMONTORY_21_SUPPORT --cTnrSupport $CBS_TURNER_SUPPORT
  #Only copy files if they differ.  Prevents needless rebuild of files.
  if [ -f "$CBS_BUILD_PATH/ResourceRS/IdsNvDefRS.h" ]; then
    DIFF=$(diff $CBS_BUILD_PATH/ResourceRMB/IdsNvDefRMB.h $CBS_AGESA_PATH/IdsNvDefRMB.h)
    if [ "$DIFF" != "" ]; then
      cp $CBS_BUILD_PATH/ResourceRMB/IdsNvDefRMB.h $CBS_AGESA_PATH/IdsNvDefRMB.h
    fi
  else
    cp $CBS_BUILD_PATH/ResourceRMB/IdsNvDefRMB.h $CBS_AGESA_PATH/IdsNvDefRMB.h
  fi
fi


