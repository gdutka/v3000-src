#!/bin/bash

if [ ! -f $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/StmApi_bak.h ]; then
  cp $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/StmApi.h $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/StmApi_bak.h
fi
cp $WORKSPACE/$CHIPSET_REL_PATH/$CHIPSET_PKG/Override/EDK2/MdePkg/Include/Register/Intel/StmApi.h $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/StmApi.h

if [ ! -f $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/SmramSaveStateMap_bak.h ]; then
  cp $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/SmramSaveStateMap.h $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/SmramSaveStateMap_bak.h
fi
cp $WORKSPACE/$CHIPSET_REL_PATH/$CHIPSET_PKG/Override/EDK2/MdePkg/Include/Register/Intel/SmramSaveStateMap.h $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/SmramSaveStateMap.h
