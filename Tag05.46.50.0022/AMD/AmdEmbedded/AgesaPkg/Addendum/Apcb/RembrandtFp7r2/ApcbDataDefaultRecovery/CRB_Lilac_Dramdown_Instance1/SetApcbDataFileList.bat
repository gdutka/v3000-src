@echo on
REM -------------------------------------------------------------------
REM BOARD NAME is set by caller
REM -------------------------------------------------------------------
ECHO *********************************
ECHO BOARD NAME : %BOARD_NAME%
ECHO *********************************

SET APCB_DATA_TYPE_FILE_LIST=%APCB_DATA_TYPE_FILE_LIST% %BOARD_NAME%\ApcbData_FP7r2_GID_0x1704_Type_PsDramDqPinMapping
SET APCB_DATA_TYPE_FILE_LIST=%APCB_DATA_TYPE_FILE_LIST% %BOARD_NAME%\ApcbData_FP7r2_GID_0x1704_Type_SpdInfoDdr5

:END
EXIT /B 0
