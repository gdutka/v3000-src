#!/bin/bash
#
# Copyright (c) 2008 - 2011, Apple Inc. All rights reserved.<BR>
# Copyright (c) 2010 - 2019, Intel Corporation. All rights reserved.<BR>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

TARGET_TOOLS=GCC48

gcc_version=$(gcc -v 2>&1 | tail -1 | awk '{print $3}')
case $gcc_version in
  [1-3].*|4.[0-7].*)
    TARGET_TOOLS=GccTooOld
    ;;
  4.8.*)
    TARGET_TOOLS=GCC48
    ;;
  4.9.*|6.[0-2].*)
    TARGET_TOOLS=GCC49
    ;;
  *)
    TARGET_TOOLS=GCC5
    ;;
esac

echo ${TARGET_TOOLS}
