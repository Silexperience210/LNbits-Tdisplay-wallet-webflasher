#!/bin/bash

# ========================================
# Setup Script - LNbits ESP32 Wallet
# Bitcoin Maxi Edition
# ========================================

echo ""
echo "========================================"
echo "  LNbits ESP32 Wallet - Setup Script"
echo "  Bitcoin Maxi Edition"
echo "========================================"
echo ""

# Check if Git is installed
if ! command -v git &> /dev/null; then
    echo "[ERROR] Git n'est pas installé !"
    echo ""
    echo "Installation:"
    echo "  Mac: brew install git"
    echo "  Ubuntu/Debian: sudo apt install git"
    echo "  Fedora: sudo dnf install git"
    echo ""
    exit 1
fi

echo "[OK] Git est installé"
echo ""

# Get user information
read -p "Entrez votre nom d'utilisateur GitHub: " USERNAME
read -p "Entrez votre email GitHub: " EMAIL
read -p "Entrez le nom du repository (défaut: lnbits-esp32-wallet): " REPONAME

REPONAME=${REPONAME:-lnbits-esp32-wallet}

echo ""
echo "Configuration:"
echo "- Username: $USERNAME"
echo "- Email: $EMAIL"
echo "- Repository: $REPONAME"
echo ""

read -p "Est-ce correct? (O/N): " CONFIRM
if [[ ! $CONFIRM =~ ^[Oo]$ ]]; then
    echo "Opération annulée."
    exit 0
fi

echo ""
echo "[1/8] Configuration de Git..."
git config --global user.name "$USERNAME"
git config --global user.email "$EMAIL"
echo "[OK] Git configuré"

echo ""
echo "[2/8] Création de la structure de dossiers..."
mkdir -p firmware src docs assets/screenshots
echo "[OK] Dossiers créés"

echo ""
echo "[3/8] Création du fichier .gitignore..."
cat > .gitignore << 'EOF'
# Build files
.pio/
.vscode/
build/
*.o
*.a

# Arduino
*.hex
*.eep
*.elf

# OS files
.DS_Store
Thumbs.db

# Temp files
*.tmp
*.bak
*~

# Secrets
secrets.h
config_local.h

# Binaries
*.bin
!firmware/*.bin
EOF
echo "[OK] .gitignore créé"

echo ""
echo "[4/8] Création du fichier LICENSE..."
cat > LICENSE << EOF
MIT License

Copyright (c) 2025 $USERNAME

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
EOF
echo "[OK] LICENSE créé"

echo ""
echo "[5/8] Initialisation du repository Git..."
git init
echo "[OK] Repository initialisé"

echo ""
echo "[6/8] Ajout de tous les fichiers..."
git add .
echo "[OK] Fichiers ajoutés"

echo ""
echo "[7/8] Création du commit initial..."
git commit -m "🎉 Initial commit - Bitcoin Maxi Edition

- Ultra-modern display with glassmorphism
- Web flasher with ESP Web Tools
- LNbits API integration
- Real-time stats and animations
- 100% Open Source

Stack Sats. HODL Hard. 🚀⚡"
echo "[OK] Commit créé"

echo ""
echo "[8/8] Configuration du remote..."
REPO_URL="https://github.com/$USERNAME/$REPONAME.git"
git remote add origin "$REPO_URL"
git branch -M main
echo "[OK] Remote configuré"

echo ""
echo "========================================"
echo "  SETUP TERMINÉ !"
echo "========================================"
echo ""
echo "Prochaines étapes:"
echo ""
echo "1. Créez le repository sur GitHub:"
echo "   https://github.com/new"
echo "   Nom: $REPONAME"
echo "   Public, sans README/License/.gitignore"
echo ""
echo "2. Puis exécutez:"
echo "   git push -u origin main"
echo ""
echo "3. Activez GitHub Pages:"
echo "   Settings > Pages > Source: main branch"
echo ""
echo "Votre site sera disponible sur:"
echo "https://$USERNAME.github.io/$REPONAME/"
echo ""
echo "========================================"
echo ""
