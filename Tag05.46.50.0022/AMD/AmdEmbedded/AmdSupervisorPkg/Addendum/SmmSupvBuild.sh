export INPUT_FILE_NAME=SmmSupv_Empty
export OUTPUT_FILE_NAME=SmmSupv

export SMM_ISOLATION_SUPPORT=$1

if [ $SMM_ISOLATION_SUPPORT = "NO" ]; then
  exit 1
fi


#Build version: Debug or Release
export BUILD_VERSION=$2

if [ ! $PSPKIT_PATH ]; then
  export PSPKIT_PATH=$WORKSPACE/AGESA/AgesaModulePkg/AMDTools/NewPspKit
fi

export SMMSUPV_PKG_PATH=$WORKSPACE/AmdSupervisorPkg
export SMMSUPV_SIGNTOOL_PATH=$SMMSUPV_PKG_PATH/AmdTools/SmmSupervisorSigningTool
export SMMSUPV_TEMP_PATH=$WORKSPACE/Build/$PROJECT_PKG/SmmSupv
export BIN_PATH=$SMMSUPV_TEMP_PATH/Bin

#"PSP_FW_PATH" is required for BuildPspImage.bat
export PSP_FW_PATH=$BIN_PATH

#if [ ! $TOOL_CHAIN_TAG ]; then
#  export $TOOL_CHAIN_TAG=GCC5
#fi

#if [ ! $TARGET ]; then
#  export $TARGET=RELEASE
#fi

export SMMSUPV_BUILD_PATH=$WORKSPACE/Build/$PROJECT_PKG/$TARGET_$TOOL_CHAIN_TAG/X64

#deleted old build files
if [ $SMMSUPV_TEMP_PATH ]; then
  rm -rf $SMMSUPV_TEMP_PATH
fi

if [ $BIN_PATH  ]; then
  rm -rf $BIN_PATH
fi

#cp PSP tool to build path
mkdir $SMMSUPV_TEMP_PATH
cp -r $PSPKIT_PATH/*                               $SMMSUPV_TEMP_PATH/

#cp SmmSupvDir required binaries
mkdir $BIN_PATH
cp -r $SMMSUPV_PKG_PATH/Binary/*                   $BIN_PATH/

# Todo add code to build OemPolicy.bin
cp $BIN_PATH/$INPUT_FILE_NAME.bin                  $SMMSUPV_TEMP_PATH/

export PSPKIT_PATH=$SMMSUPV_TEMP_PATH

cp $SMMSUPV_PKG_PATH/Binary/SmmSupervisor/$BUILD_VERSION/SmmSupvSig.bin               $BIN_PATH/


export BIOS_IMAGE_CONFIG_FILE=$SMMSUPV_PKG_PATH/Binary/SmmSupv.xml

export OUTPUT_BIN=$OUTPUT_FILE_NAME.bin

chmod a+x $SMMSUPV_TEMP_PATH/BuildPspImage.sh
this_command="$SMMSUPV_TEMP_PATH/BuildPspImage.sh $INPUT_FILE_NAME.bin NotFound $BIOS_IMAGE_CONFIG_FILE $OUTPUT_BIN NOSIGN"
eval $this_command || exit 2

cp $SMMSUPV_TEMP_PATH/Temp/Output/$OUTPUT_BIN $BIN_PATH/
