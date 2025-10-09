
@REM SPDX-License-Identifier: MIT
@REM Copyright (c) 2025 Jayantkumar56


@echo off
setlocal enabledelayedexpansion

echo Generating build files for Quirk-Engine

if exist "Build" (
    echo -- 'Build' directory already exists.
    echo(
) else (
    set "target_dir=%cd%\Build"
    echo -- Creating 'Build' directory at !target_dir!
    echo(
    mkdir "!target_dir!"
)

cd Build
echo -- Running Cmake...
cmake .. -G "Visual Studio 17 2022"
PAUSE
