@echo off
setlocal enabledelayedexpansion

:: Try to find git-bash.exe in PATH
where git-bash.exe >nul 2>nul
if %errorlevel%==0 (
    set "GIT_BASH=%%i"
    echo Git Bash found in PATH.
    goto run_bash
)

:: Check default install location
set "GIT_BASH=%ProgramFiles%\Git\git-bash.exe"
if exist "!GIT_BASH!" (
    echo Git Bash found at !GIT_BASH!
    goto run_bash
)

:: Try 32-bit path
set "GIT_BASH=%ProgramFiles(x86)%\Git\git-bash.exe"
if exist "!GIT_BASH!" (
    echo Git Bash found at !GIT_BASH_X86!
    goto run_bash
)

:: If not found
echo Git Bash not found.
set /p CHOICE=Do you want to download and install Git Bash? (y/n): 
if /i "%CHOICE%"=="y" (
    echo Git bash not found!
    echo Opening download page...
    start https://git-scm.com/download/win
    echo run the script again after installation...
    goto end
) else (
    echo Installation canceled.
    goto end
)

:run_bash
"%GIT_BASH%" -c "./setup-cmake.sh"

:end