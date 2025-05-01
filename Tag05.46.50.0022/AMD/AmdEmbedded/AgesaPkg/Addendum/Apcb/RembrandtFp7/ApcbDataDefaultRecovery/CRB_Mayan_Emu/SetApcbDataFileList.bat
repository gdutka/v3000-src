@echo on
REM -------------------------------------------------------------------
REM BOARD NAME is set by caller
REM -------------------------------------------------------------------
ECHO *********************************
ECHO BOARD NAME : %BOARD_NAME%
ECHO *********************************

SET APCB_DATA_TYPE_FILE_LIST=%APCB_DATA_TYPE_FILE_LIST% %BOARD_NAME%\ApcbData_FP7_GID_0x1704_Type_SpdInfo

:END
EXIT /B 0
