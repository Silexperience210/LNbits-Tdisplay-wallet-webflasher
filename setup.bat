@echo off
REM ========================================
REM Setup Script - LNbits ESP32 Wallet
REM Bitcoin Maxi Edition
REM ========================================

echo.
echo ========================================
echo   LNbits ESP32 Wallet - Setup Script
echo   Bitcoin Maxi Edition
echo ========================================
echo.

REM Check if Git is installed
git --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Git n'est pas installe !
    echo.
    echo Telechargez Git sur: https://git-scm.com/download/win
    echo.
    pause
    exit /b 1
)

echo [OK] Git est installe
echo.

REM Get user information
set /p USERNAME="Entrez votre nom d'utilisateur GitHub: "
set /p EMAIL="Entrez votre email GitHub: "
set /p REPONAME="Entrez le nom du repository (defaut: lnbits-esp32-wallet): "

if "%REPONAME%"=="" set REPONAME=lnbits-esp32-wallet

echo.
echo Configuration:
echo - Username: %USERNAME%
echo - Email: %EMAIL%
echo - Repository: %REPONAME%
echo.

set /p CONFIRM="Est-ce correct? (O/N): "
if /i not "%CONFIRM%"=="O" (
    echo Operation annulee.
    pause
    exit /b 0
)

echo.
echo [1/8] Configuration de Git...
git config --global user.name "%USERNAME%"
git config --global user.email "%EMAIL%"
echo [OK] Git configure

echo.
echo [2/8] Creation de la structure de dossiers...
if not exist firmware mkdir firmware
if not exist src mkdir src
if not exist docs mkdir docs
if not exist assets mkdir assets
if not exist assets\screenshots mkdir assets\screenshots
echo [OK] Dossiers crees

echo.
echo [3/8] Creation du fichier .gitignore...
(
echo # Build files
echo .pio/
echo .vscode/
echo build/
echo *.o
echo *.a
echo.
echo # Arduino
echo *.hex
echo *.eep
echo *.elf
echo.
echo # OS files
echo .DS_Store
echo Thumbs.db
echo.
echo # Temp files
echo *.tmp
echo *.bak
echo *~
echo.
echo # Secrets
echo secrets.h
echo config_local.h
echo.
echo # Binaries
echo *.bin
echo !firmware/*.bin
) > .gitignore
echo [OK] .gitignore cree

echo.
echo [4/8] Creation du fichier LICENSE...
(
echo MIT License
echo.
echo Copyright ^(c^) 2025 %USERNAME%
echo.
echo Permission is hereby granted, free of charge, to any person obtaining a copy
echo of this software and associated documentation files ^(the "Software"^), to deal
echo in the Software without restriction, including without limitation the rights
echo to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
echo copies of the Software, and to permit persons to whom the Software is
echo furnished to do so, subject to the following conditions:
echo.
echo The above copyright notice and this permission notice shall be included in all
echo copies or substantial portions of the Software.
echo.
echo THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
echo IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
echo FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
echo AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
echo LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
echo OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
echo SOFTWARE.
) > LICENSE
echo [OK] LICENSE cree

echo.
echo [5/8] Initialisation du repository Git...
git init
echo [OK] Repository initialise

echo.
echo [6/8] Ajout de tous les fichiers...
git add .
echo [OK] Fichiers ajoutes

echo.
echo [7/8] Creation du commit initial...
git commit -m "🎉 Initial commit - Bitcoin Maxi Edition

- Ultra-modern display with glassmorphism
- Web flasher with ESP Web Tools
- LNbits API integration
- Real-time stats and animations
- 100%% Open Source

Stack Sats. HODL Hard. 🚀⚡"
echo [OK] Commit cree

echo.
echo [8/8] Configuration du remote...
set REPO_URL=https://github.com/%USERNAME%/%REPONAME%.git
git remote add origin %REPO_URL%
git branch -M main
echo [OK] Remote configure

echo.
echo ========================================
echo   SETUP TERMINE !
echo ========================================
echo.
echo Prochaines etapes:
echo.
echo 1. Creez le repository sur GitHub:
echo    https://github.com/new
echo    Nom: %REPONAME%
echo    Public, sans README/License/.gitignore
echo.
echo 2. Puis executez:
echo    git push -u origin main
echo.
echo 3. Activez GitHub Pages:
echo    Settings ^> Pages ^> Source: main branch
echo.
echo Votre site sera disponible sur:
echo https://%USERNAME%.github.io/%REPONAME%/
echo.
echo ========================================
echo.
pause
