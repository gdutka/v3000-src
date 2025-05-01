@echo off
set PY=c:\Python37

cd /d %~dp0
rmdir /s /q venv dist build
%PY%\Scripts\pip install virtualenv
%PY%\Scripts\virtualenv venv 
set PATH=%cd%\venv\Scripts;%PATH%

@REM
@REM For python 2.7, need install pyinstaller 3.6
@REM pyinstaller 4.0 - 4.2 will get false positives from Windows Defender, 
@REM so it still use pyinstaller 3.6 in py37
@REM 
@REM
for /f "delims=" %%a in ('python -c "import sys; print(str(sys.version)[0])" ^| findstr "2"') do set PY_VER=2
if "%PY_VER%" == "2" (
    pip install lxml pyinstaller==3.6 pathlib2
) else (
    pip install lxml pyinstaller==3.6 pathlib2
)

pyinstaller -F PatchPspDir.py 
copy dist\PatchPspDir.exe .
pause