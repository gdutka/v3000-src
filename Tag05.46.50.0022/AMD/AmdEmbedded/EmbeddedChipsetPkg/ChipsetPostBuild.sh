#!/bin/bash

if [ -f $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/StmApi_bak.h ]; then
  cp $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/StmApi_bak.h $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/StmApi.h
  rm $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/StmApi_bak.h
else
  echo ERROR: Restore Fail: StmApi.h.
fi

if [ -f $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/SmramSaveStateMap_bak.h ]; then
  cp $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/SmramSaveStateMap_bak.h $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/SmramSaveStateMap.h
  rm $WORKSPACE/EDK2/MdePkg/Include/Register/Intel/SmramSaveStateMap_bak.h
else
  echo ERROR: Restore Fail: SmramSaveStateMap.h.
fi
