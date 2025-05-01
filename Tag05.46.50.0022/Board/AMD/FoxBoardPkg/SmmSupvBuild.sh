#!/bin/bash
INPUT_FILE_NAME=SmmSupv_Empty
OUTPUT_FILE_NAME=SmmSupv
#Default Value

SMM_ISOLATION_SUPPORT=$1

if [ $SMM_ISOLATION_SUPPORT = "NO" ]; then
  exit
fi

#Build version: Debug or Release
TARGET=$2
echo $TARGET

if [ -z $PSPKIT_PATH ]; then
  PSPKIT_PATH=$WORKSPACE/$CHIPSET_REL_PATH/AgesaModulePkg/AMDTools/NewPspKit
fi
SMMSUPV_PKG_PATH=$WORKSPACE/$CHIPSET_REL_PATH/AmdSupervisorPkg
SMMSUPV_SIGNTOOL_PATH=$SMMSUPV_PKG_PATH/AmdTools/SmmSupervisorSigningTool
SMMSUPV_TEMP_PATH=$WORKSPACE/Build/$PROJECT_PKG/SmmSupv
BIN_PATH=$SMMSUPV_TEMP_PATH/Bin
#"PSP_FW_PATH" is required for BuildPspImage.bat
PSP_FW_PATH=$BIN_PATH

if [ -f $WORKSPACE/Conf/target.txt ]; then
  TOOL_CHAIN_TAG=$(grep 'TOOL_CHAIN_TAG' $WORKSPACE/Conf/target.txt | awk '{print $3}')
fi
SMMSUPV_BUILD_PATH=$WORKSPACE/Build/$PROJECT_PKG/${TARGET}_${TOOL_CHAIN_TAG}/X64

#deleted old build files
if [ -d $SMMSUPV_TEMP_PATH ]; then
  rm -rf $SMMSUPV_TEMP_PATH
fi
mkdir -p $SMMSUPV_TEMP_PATH
if [ -d $BIN_PATH ]; then
  rm -rf $BIN_PATH
fi
mkdir -p $BIN_PATH
#Copy PSP tool to build path
cp -R $PSPKIT_PATH/*                                $SMMSUPV_TEMP_PATH
#Copy SmmSupvDir required binaries
cp -R $SMMSUPV_PKG_PATH/Binary/*                    $BIN_PATH

# Todo add code to build OemPolicy.bin

cp $BIN_PATH/${INPUT_FILE_NAME}.bin                 $SMMSUPV_TEMP_PATH
PSPKIT_PATH=$SMMSUPV_TEMP_PATH

if [[ $TARGET == "DEBUG"* ]]; then
  cp -R $SMMSUPV_PKG_PATH/Binary/SmmSupervisor/Debug/SmmSupvSig.bin                 $BIN_PATH
else
  cp -R $SMMSUPV_PKG_PATH/Binary/SmmSupervisor/Release/SmmSupvSig.bin               $BIN_PATH
fi

BIOS_IMAGE_CONFIG_FILE=$SMMSUPV_PKG_PATH/Binary/SmmSupv.xml

OUTPUT_BIN=${OUTPUT_FILE_NAME}.bin
export PSPKIT_PATH
export PSP_FW_PATH
chmod -R 777 $SMMSUPV_TEMP_PATH/BuildPspImage.sh
$SMMSUPV_TEMP_PATH/BuildPspImage.sh ${INPUT_FILE_NAME}.bin NotFound $BIOS_IMAGE_CONFIG_FILE $OUTPUT_BIN NOSIGN
cp $SMMSUPV_TEMP_PATH/Temp/Output/$OUTPUT_BIN $BIN_PATH
