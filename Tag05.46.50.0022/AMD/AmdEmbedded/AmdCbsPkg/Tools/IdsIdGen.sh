#call Ids Id headerfile script tool
if [ -d "$WORKSPACE/AGESA/AgesaModulePkg/Include" ]; then
  AGESA_INC_PATH=$WORKSPACE/AGESA/AgesaModulePkg/Include
else
  AGESA_INC_PATH=$WORKSPACE/$CHIPSET_REL_PATH/AgesaModulePkg/Include
fi

if [ -z $PERL_PATH ]; then
  PERL_PATH=$WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/Tools/Perl/bin
fi

PERL_LIB_PATH=$PERL_PATH/../lib

perl -I$WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/Tools -I$PERL_LIB_PATH $WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/Tools/IdsIdGen.pl -i $WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/Library -o $WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/Build
cp -Rf $WORKSPACE/$CHIPSET_REL_PATH/AmdCbsPkg/Build/*.*      $AGESA_INC_PATH
