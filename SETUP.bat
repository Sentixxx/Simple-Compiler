@echo off
python --version 2>NUL
IF ERRORLEVEL 1 (
    echo Python is not installed.
    echo Setting up the python...
    start /wait "" "%~dp0res\python312.exe" /quiet InstallAllUsers=1 PrependPath=1 Include_test=0
    echo Setting up Complete
) ELSE (
    echo Python is already installed.
)

python -m venv %~dp0venv
echo "venv" folder created
%~dp0venv\bin\python.exe -m pip install -r %~dp0requirements.txt
echo "venv" setup complete
echo Setting up Graphviz...
start /wait "" "%~dp0res\graphviz.exe"
echo Setting up Complete
pause