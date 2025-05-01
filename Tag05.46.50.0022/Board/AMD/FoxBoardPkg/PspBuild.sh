#!/bin/bash

echo inside RMB pspbuild Emu

FD_NAME=${PLATFORM_NAME}_GLUED

#Default Value
if [ -z $KEY_MODE ]; then
  KEY_MODE=NOSIGN
fi

echo KEY_MODE $KEY_MODE
PSP_PLATFORM_PATH=$WORKSPACE/$PROJECT_REL_PATH/$PROJECT_PKG/PlatformConfig
KVM_PLATFORM_PATH=$WORKSPACE/$PROJECT_REL_PATH/$PROJECT_PKG/Binary/KVM

if [ $KEY_MODE = "PK" ]; then
  PSP_FW_PATH=$WORKSPACE/$CHIPSET_REL_PATH/AgesaModulePkg/Firmwares/RMB
  PSP_CONFIG_FILE_PATH=$PSP_PLATFORM_PATH
fi

if [ $KEY_MODE = "NOSIGN" ]; then
  PSP_FW_PATH=$WORKSPACE/$CHIPSET_REL_PATH/AgesaModulePkg/Firmwares/RMB
  PSP_CONFIG_FILE_PATH=$PSP_PLATFORM_PATH
fi

echo PSP_FW_PATH $PSP_FW_PATH

PSPKIT_PATH=$WORKSPACE/$CHIPSET_REL_PATH/AgesaModulePkg/AMDTools/NewPspKit
PSP_TEMP_PATH=$WORKSPACE/Build/$PROJECT_PKG/NewPspKit

APCB_BOARD_PKG_TEMP_PATH=$WORKSPACE/Build/$PROJECT_PKG/ApcbToolV3/External
CUSTOM_APCB_PATH=$APCB_BOARD_PKG_TEMP_PATH/Release
LPDDR5=$(grep 'LPDDR5' $WORKSPACE/$PROJECT_REL_PATH/$PROJECT_PKG/Project.env | awk '{print $4}')
if [[ $LPDDR5 == "YES"* ]]; then
  if [ ! -f $CUSTOM_APCB_PATH/APCB_FP7_DefaultRecovery.bin ]; then
    CUSTOM_APCB_PATH=
  fi
  if [ ! -f $CUSTOM_APCB_PATH/APCB_FP7_Updatable.bin ]; then
    CUSTOM_APCB_PATH=
  fi
elif [[ $LPDDR5 == "NO"* ]] ; then
  if [ ! -f $CUSTOM_APCB_PATH/APCB_FP7r2_DefaultRecovery.bin ]; then
    CUSTOM_APCB_PATH=
  fi
  if [ ! -f $CUSTOM_APCB_PATH/APCB_FP7r2_Updatable.bin ]; then
    CUSTOM_APCB_PATH=
  fi
fi
echo CUSTOM_APCB_PATH $CUSTOM_APCB_PATH

BIOS_FV_PATH=$WORKSPACE/Build/$PROJECT_PKG/${TARGET}_${TOOL_CHAIN_TAG}/FV

KVM_SUPPORT=$(grep 'KVM_SUPPORT' $WORKSPACE/$PROJECT_REL_PATH/$PROJECT_PKG/Project.env | awk '{print $4}')
echo KVM_SUPPORT $KVM_SUPPORT

if [ -d $PSP_TEMP_PATH ]; then
  rm -rf $PSP_TEMP_PATH
fi
mkdir -p $PSP_TEMP_PATH
cp -R $PSPKIT_PATH/*                                $PSP_TEMP_PATH
cp $BIOS_FV_PATH/${FD_NAME^^}.fd                    $PSP_TEMP_PATH
PSPKIT_PATH=$PSP_TEMP_PATH

RTM_FILE=RECOVERYFV.Fv
cp $BIOS_FV_PATH/$RTM_FILE                          $PSP_TEMP_PATH

# Select BIOSImageDirectory
VN_16M_AB_CFG=$(grep 'VN_16M_AB_CFG' $WORKSPACE/$PROJECT_REL_PATH/$PROJECT_PKG/Project.env | awk '{print $4}')
L2A_ONLY=$(grep 'L2A_ONLY' $WORKSPACE/$PROJECT_REL_PATH/$PROJECT_PKG/Project.env | awk '{print $4}')
if [[ $VN_16M_AB_CFG == "YES"* ]]; then
  echo PSP 16M RMB build
  BIOS_IMAGE_CONFIG_FILE_NAME=BIOSImageDirectory16M.xml
else
  echo PSP 32M RMB build
  if [[ $LPDDR5 == "YES"* ]]; then
    if [[ $L2A_ONLY == "YES"* ]]; then
      BIOS_IMAGE_CONFIG_FILE_NAME=BIOSImageDirectory32M_LPDDR5_L2A.xml
    else
      BIOS_IMAGE_CONFIG_FILE_NAME=BIOSImageDirectory32M_LPDDR5.xml
    fi
  elif [[ $LPDDR5 == "NO"* ]]; then
    if [[ $L2A_ONLY == "YES"* ]]; then
      BIOS_IMAGE_CONFIG_FILE_NAME=BIOSImageDirectory32M_DDR5_L2A.xml
    else
      BIOS_IMAGE_CONFIG_FILE_NAME=BIOSImageDirectory32M_DDR5.xml
    fi
  fi
fi

if [[ $KVM_SUPPORT == "YES"* ]]; then
  #Copy file KvmEngine if exist
  if [ -f $KVM_PLATFORM_PATH/KvmEngine.csbin ]; then
    cp -R $KVM_PLATFORM_PATH/*                          $PSP_FW_PATH
  fi
fi

echo PathPspDir $PSP_PLATFORM_PATH/$BIOS_IMAGE_CONFIG_FILE_NAME
if [ $L2A_ONLY = "YES" ]; then
  python3 $PSP_PLATFORM_PATH/Tools/PatchPspDir.py -p $WORKSPACE/Build/$PROJECT_PKG/${TARGET}_${TOOL_CHAIN_TAG}/IA32/PcdList.txt \
                                                  -t $PSP_PLATFORM_PATH/PspPcdMapTableConfig_L2A.py \
                                                  -i $PSP_PLATFORM_PATH/$BIOS_IMAGE_CONFIG_FILE_NAME
else
  python3 $PSP_PLATFORM_PATH/Tools/PatchPspDir.py -p $WORKSPACE/Build/$PROJECT_PKG/${TARGET}_${TOOL_CHAIN_TAG}/IA32/PcdList.txt \
                                                  -t $PSP_PLATFORM_PATH/PspPcdMapTableConfig.py \
                                                  -i $PSP_PLATFORM_PATH/$BIOS_IMAGE_CONFIG_FILE_NAME
fi

cp $PSP_PLATFORM_PATH/$BIOS_IMAGE_CONFIG_FILE_NAME $BIOS_FV_PATH
BIOS_IMAGE_CONFIG_FILE=$BIOS_FV_PATH/$BIOS_IMAGE_CONFIG_FILE_NAME

if [ $KEY_MODE = "NOSIGN" ]; then
  #remove the line that conatins RTMSignature string for NOSIGN mode
  grep -v "RTMSignature" $BIOS_IMAGE_CONFIG_FILE > $BIOS_FV_PATH/BIOSImageDirectory-NOSIGN.xml
  BIOS_IMAGE_CONFIG_FILE=$BIOS_FV_PATH/BIOSImageDirectory-NOSIGN.xml
fi

echo BIOS_IMAGE_CONFIG_FILE $BIOS_IMAGE_CONFIG_FILE
OUTPUT_BIOS=$(echo ${PLATFORM_NAME,,} | sed 's/\b[a-z]/\U&/g').FD
chmod -R 777 $PSP_TEMP_PATH
export PSPKIT_PATH
export PSP_FW_PATH
export CUSTOM_APCB_PATH
echo $PSP_TEMP_PATH/BuildPspImage.sh ${FD_NAME^^}.fd $RTM_FILE $BIOS_IMAGE_CONFIG_FILE $OUTPUT_BIOS $KEY_MODE
$PSP_TEMP_PATH/BuildPspImage.sh ${FD_NAME^^}.fd $RTM_FILE $BIOS_IMAGE_CONFIG_FILE $OUTPUT_BIOS $KEY_MODE

cp $PSP_TEMP_PATH/$OUTPUT_BIOS $WORKSPACE/$PROJECT_REL_PATH/$PROJECT_PKG/BIOS

if [[ $KVM_SUPPORT == "YES"* ]]; then
  #Remove the copy file KvmEngine if exist
  if [ -f $KVM_PLATFORM_PATH/KvmEngine.csbin ]; then
    rm $PSP_FW_PATH/KvmEngine.csbin
  fi
fi

