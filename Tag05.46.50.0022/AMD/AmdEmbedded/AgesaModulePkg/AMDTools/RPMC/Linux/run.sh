#;*******************************************************************************
#; Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
#;
#;****************************************************************************
#! /bin/sh
# run.sh
#usage sudo sh ./run.sh param1 param2   RPMC's params 

SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
if lsmod | grep msr
then
echo "msr.ko has been installed."
else
sudo modprobe msr
fi
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${SHELL_FOLDER}/lib
${SHELL_FOLDER}/main.out "$@"
