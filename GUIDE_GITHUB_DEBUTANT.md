# 🚀 Guide Étape par Étape - Push sur GitHub

Guide ultra-détaillé pour mettre votre projet LNbits Wallet sur GitHub.

---

## 📋 Prérequis

### ✅ Ce dont vous avez besoin :

1. **Compte GitHub** → [github.com](https://github.com)
2. **Git installé** sur votre PC
3. **Les fichiers** du projet (vous les avez déjà !)

### Vérifier si Git est installé :

**Windows :**
```cmd
git --version
```

**Si pas installé :**
- Téléchargez : https://git-scm.com/download/win
- Installez avec les options par défaut
- Redémarrez le terminal

**Mac :**
```bash
git --version
```

**Si pas installé :**
```bash
brew install git
# OU installer Xcode Command Line Tools
```

**Linux :**
```bash
git --version
```

**Si pas installé :**
```bash
sudo apt install git     # Ubuntu/Debian
sudo dnf install git     # Fedora
```

---

## 🎬 ÉTAPE 1 : Créer le Repository sur GitHub

### 1.1 Aller sur GitHub

Ouvrez votre navigateur : https://github.com

### 1.2 Créer un nouveau repo

1. Cliquez sur le **"+"** en haut à droite
2. Cliquez sur **"New repository"**

### 1.3 Remplir les informations

```
Repository name: lnbits-esp32-wallet
Description: ⚡ Ultra-modern LNbits Lightning wallet display for ESP32 - Bitcoin Maxi Edition
```

**Options :**
- ☑️ **Public** (pour GitHub Pages gratuit)
- ☐ **PAS de README** (on va le créer nous-mêmes)
- ☐ **PAS de .gitignore** (on va le créer)
- ☐ **PAS de License** (on va le créer)

### 1.4 Cliquer sur "Create repository"

✅ **Résultat :** Vous voyez une page avec des instructions

**⚠️ NE FERMEZ PAS CETTE PAGE !** On va copier l'URL du repo.

### 1.5 Copier l'URL de votre repo

Vous verrez quelque chose comme :
```
https://github.com/VOTRE-USERNAME/lnbits-esp32-wallet.git
```

**💾 COPIEZ cette URL !** (On en aura besoin)

---

## 🎬 ÉTAPE 2 : Organiser vos fichiers localement

### 2.1 Créer un dossier pour le projet

**Windows :**
```cmd
cd C:\Users\VOTRE-NOM\Documents
mkdir lnbits-esp32-wallet
cd lnbits-esp32-wallet
```

**Mac/Linux :**
```bash
cd ~/Documents
mkdir lnbits-esp32-wallet
cd lnbits-esp32-wallet
```

### 2.2 Copier TOUS les fichiers téléchargés

Copiez ces fichiers dans le dossier `lnbits-esp32-wallet` :

```
✅ index.html
✅ manifest.json
✅ README.md
✅ FIRMWARE_GUIDE.md
✅ DEPLOYMENT_GUIDE.md
✅ PACKAGE_SUMMARY.md
✅ README_ULTRA_MODERN.md
✅ display_preview.html
✅ lnbits_wallet_ULTRA_MODERN_v2.ino
```

**Comment copier ?**
- Faites **glisser-déposer** depuis le dossier de téléchargements
- OU utilisez Ctrl+C / Ctrl+V

### 2.3 Créer la structure de dossiers

**Windows (dans le dossier du projet) :**
```cmd
mkdir firmware
mkdir src
mkdir docs
mkdir assets
mkdir assets\screenshots
```

**Mac/Linux :**
```bash
mkdir -p firmware src docs assets/screenshots
```

### 2.4 Déplacer les fichiers dans les bons dossiers

**Déplacez :**
```
lnbits_wallet_ULTRA_MODERN_v2.ino → src/
README_ULTRA_MODERN.md → docs/
display_preview.html → assets/
```

**Gardez à la racine :**
```
index.html
manifest.json
README.md
FIRMWARE_GUIDE.md
DEPLOYMENT_GUIDE.md
PACKAGE_SUMMARY.md
```

**💡 Votre structure finale :**
```
lnbits-esp32-wallet/
├── index.html
├── manifest.json
├── README.md
├── FIRMWARE_GUIDE.md
├── DEPLOYMENT_GUIDE.md
├── PACKAGE_SUMMARY.md
├── firmware/          (vide pour l'instant)
├── src/
│   └── lnbits_wallet_ULTRA_MODERN_v2.ino
├── docs/
│   └── README_ULTRA_MODERN.md
└── assets/
    ├── display_preview.html
    └── screenshots/   (vide pour l'instant)
```

---

## 🎬 ÉTAPE 3 : Initialiser Git localement

### 3.1 Ouvrir le terminal dans le dossier du projet

**Windows :**
```cmd
cd C:\Users\VOTRE-NOM\Documents\lnbits-esp32-wallet
```

**Mac/Linux :**
```bash
cd ~/Documents/lnbits-esp32-wallet
```

**OU dans Windows :**
- Ouvrez le dossier dans l'explorateur
- Shift + Clic droit dans le dossier
- "Ouvrir PowerShell ici" ou "Ouvrir dans Terminal"

### 3.2 Configurer Git (PREMIÈRE FOIS SEULEMENT)

**Si c'est votre première fois avec Git :**

```bash
git config --global user.name "VotreNom"
git config --global user.email "votre.email@example.com"
```

**⚠️ Remplacez par vos vraies informations !**

**Exemple :**
```bash
git config --global user.name "SatoshiNakamoto"
git config --global user.email "satoshi@bitcoin.org"
```

### 3.3 Initialiser le repository Git

```bash
git init
```

**✅ Vous devriez voir :**
```
Initialized empty Git repository in .../lnbits-esp32-wallet/.git/
```

---

## 🎬 ÉTAPE 4 : Créer les fichiers nécessaires

### 4.1 Créer .gitignore

**Windows (PowerShell) :**
```powershell
@"
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

# Binaries (except firmware folder)
*.bin
!firmware/*.bin
"@ | Out-File -Encoding UTF8 .gitignore
```

**Mac/Linux :**
```bash
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

# Binaries (except firmware folder)
*.bin
!firmware/*.bin
EOF
```

### 4.2 Créer LICENSE

**Windows (PowerShell) :**
```powershell
@"
MIT License

Copyright (c) 2025 VOTRE-NOM

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
"@ | Out-File -Encoding UTF8 LICENSE
```

**Mac/Linux :**
```bash
cat > LICENSE << 'EOF'
MIT License

Copyright (c) 2025 VOTRE-NOM

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
```

**⚠️ N'oubliez pas de remplacer "VOTRE-NOM" !**

### 4.3 Créer un README pour le dossier firmware

**Windows (PowerShell) :**
```powershell
@"
# Firmware Binaries

Place your compiled firmware files here:

- bootloader.bin
- partitions.bin
- boot_app0.bin
- lnbits_wallet_ultra_modern.bin

See FIRMWARE_GUIDE.md for instructions on how to generate these files.
"@ | Out-File -Encoding UTF8 firmware\README.md
```

**Mac/Linux :**
```bash
cat > firmware/README.md << 'EOF'
# Firmware Binaries

Place your compiled firmware files here:

- bootloader.bin
- partitions.bin
- boot_app0.bin
- lnbits_wallet_ultra_modern.bin

See FIRMWARE_GUIDE.md for instructions on how to generate these files.
EOF
```

---

## 🎬 ÉTAPE 5 : Vérifier les fichiers

### 5.1 Lister tous les fichiers

```bash
git status
```

**✅ Vous devriez voir :**
```
On branch main
No commits yet

Untracked files:
  .gitignore
  LICENSE
  index.html
  manifest.json
  README.md
  ...
```

### 5.2 Vérifier la structure

**Windows :**
```cmd
dir
dir src
dir docs
dir firmware
```

**Mac/Linux :**
```bash
ls -la
ls src/
ls docs/
ls firmware/
```

---

## 🎬 ÉTAPE 6 : Faire le premier commit

### 6.1 Ajouter TOUS les fichiers

```bash
git add .
```

**💡 Le point "." signifie "tous les fichiers"**

### 6.2 Vérifier ce qui va être commit

```bash
git status
```

**✅ Vous devriez voir en VERT :**
```
Changes to be committed:
  new file: .gitignore
  new file: LICENSE
  new file: index.html
  ...
```

### 6.3 Faire le commit

```bash
git commit -m "🎉 Initial commit - Bitcoin Maxi Edition

- Ultra-modern display with glassmorphism
- Web flasher with ESP Web Tools
- LNbits API integration
- Real-time stats and animations
- 100% Open Source

Stack Sats. HODL Hard. 🚀⚡"
```

**✅ Vous devriez voir :**
```
[main (root-commit) abc1234] 🎉 Initial commit - Bitcoin Maxi Edition
 XX files changed, XXXX insertions(+)
 create mode 100644 .gitignore
 create mode 100644 LICENSE
 ...
```

---

## 🎬 ÉTAPE 7 : Connecter à GitHub

### 7.1 Ajouter le remote

**Copiez l'URL de VOTRE repo (étape 1.5) et collez :**

```bash
git remote add origin https://github.com/VOTRE-USERNAME/lnbits-esp32-wallet.git
```

**⚠️ REMPLACEZ "VOTRE-USERNAME" par votre vrai username GitHub !**

**Exemple :**
```bash
git remote add origin https://github.com/satoshi/lnbits-esp32-wallet.git
```

### 7.2 Vérifier le remote

```bash
git remote -v
```

**✅ Vous devriez voir :**
```
origin  https://github.com/VOTRE-USERNAME/lnbits-esp32-wallet.git (fetch)
origin  https://github.com/VOTRE-USERNAME/lnbits-esp32-wallet.git (push)
```

---

## 🎬 ÉTAPE 8 : Push vers GitHub ! 🚀

### 8.1 Renommer la branche en "main"

```bash
git branch -M main
```

### 8.2 PUSH !

```bash
git push -u origin main
```

**💡 Première fois :** GitHub va vous demander de vous authentifier

**Option 1 : Browser (plus facile)**
- Une fenêtre de navigateur s'ouvre
- Cliquez sur "Authorize"
- Revenez au terminal

**Option 2 : Personal Access Token**
- Si ça ne marche pas, créez un token :
  1. GitHub → Settings → Developer settings
  2. Personal access tokens → Tokens (classic)
  3. Generate new token (classic)
  4. Cochez : repo, workflow
  5. Copiez le token
  6. Utilisez-le comme mot de passe

**✅ Après quelques secondes, vous verrez :**
```
Enumerating objects: XX, done.
Counting objects: 100% (XX/XX), done.
Writing objects: 100% (XX/XX), XX KiB | XX MiB/s, done.
Total XX (delta 0), reused 0 (delta 0)
To https://github.com/VOTRE-USERNAME/lnbits-esp32-wallet.git
 * [new branch]      main -> main
Branch 'main' set up to track remote branch 'main' from 'origin'.
```

---

## 🎬 ÉTAPE 9 : Vérifier sur GitHub

### 9.1 Ouvrir votre repo

Allez sur : `https://github.com/VOTRE-USERNAME/lnbits-esp32-wallet`

**✅ Vous devriez voir :**
- Tous vos fichiers
- Le README.md affiché en bas
- Les dossiers src/, docs/, firmware/, assets/

### 9.2 Vérifier les fichiers

Cliquez sur différents fichiers pour vérifier qu'ils sont bien là :
- index.html
- manifest.json
- src/lnbits_wallet_ULTRA_MODERN_v2.ino
- etc.

---

## 🎬 ÉTAPE 10 : Activer GitHub Pages

### 10.1 Aller dans Settings

Sur votre repo GitHub :
1. Cliquez sur **"Settings"** (en haut)
2. Scroll dans la barre de gauche
3. Cliquez sur **"Pages"**

### 10.2 Configurer Pages

**Source :**
- Deploy from a branch
- Branch : **main**
- Folder : **/ (root)**
- Cliquez **Save**

### 10.3 Attendre le déploiement

**⏱️ Temps d'attente : 2-3 minutes**

Rafraîchissez la page, vous verrez :
```
✅ Your site is live at https://VOTRE-USERNAME.github.io/lnbits-esp32-wallet/
```

### 10.4 Tester votre site

Cliquez sur le lien ou allez sur :
```
https://VOTRE-USERNAME.github.io/lnbits-esp32-wallet/
```

**🎉 BOOM ! Votre web flasher est en ligne !**

---

## 🎬 ÉTAPE 11 : Ajouter des Topics (Tags)

### 11.1 Retour sur la page principale du repo

### 11.2 Cliquer sur l'icône ⚙️ à côté de "About"

### 11.3 Ajouter ces topics :

```
bitcoin
lightning-network
esp32
lnbits
arduino
iot
cryptocurrency
web-flasher
esp-web-tools
```

### 11.4 Sauvegarder

---

## 🎉 FÉLICITATIONS ! Vous avez réussi !

### ✅ Ce que vous avez maintenant :

1. ✅ **Repository GitHub** public avec tout votre code
2. ✅ **GitHub Pages** activé
3. ✅ **Web flasher** en ligne et fonctionnel
4. ✅ **Documentation** complète
5. ✅ **Prêt à partager** avec la communauté !

---

## 📝 Commandes pour les prochaines fois

### Quand vous modifiez des fichiers :

```bash
# 1. Voir ce qui a changé
git status

# 2. Ajouter les changements
git add .

# 3. Commit avec un message
git commit -m "✨ Add new feature"

# 4. Push vers GitHub
git push
```

### Commandes utiles :

```bash
# Voir l'historique
git log --oneline

# Voir les différences
git diff

# Annuler les changements (ATTENTION !)
git checkout -- fichier.txt

# Créer une branche
git checkout -b nouvelle-branche

# Changer de branche
git checkout main
```

---

## 🐛 Problèmes courants

### "Permission denied"
**Solution :**
```bash
# Vérifier le remote
git remote -v

# Re-configurer avec votre username
git remote set-url origin https://VOTRE-USERNAME@github.com/VOTRE-USERNAME/lnbits-esp32-wallet.git
```

### "Authentication failed"
**Solutions :**
1. Créer un Personal Access Token
2. Utiliser SSH au lieu de HTTPS
3. Vérifier vos identifiants GitHub

### "fatal: not a git repository"
**Solution :**
```bash
# Vérifier que vous êtes dans le bon dossier
pwd  # Mac/Linux
cd   # Windows

# Ré-initialiser si nécessaire
git init
```

### "Updates were rejected"
**Solution :**
```bash
# Récupérer les changements d'abord
git pull origin main

# Puis push
git push origin main
```

---

## 🎯 Prochaines étapes

### 1. Personnaliser votre page

Éditez `index.html` :
- Changez "YOURUSERNAME" par votre username
- Ajoutez vos liens Twitter, etc.

```bash
# Après modification
git add index.html
git commit -m "🎨 Update personal links"
git push
```

### 2. Générer les firmwares

Suivez **FIRMWARE_GUIDE.md** pour créer les .bin

Puis :
```bash
cp *.bin firmware/
git add firmware/
git commit -m "📦 Add firmware binaries"
git push
```

### 3. Ajouter des screenshots

Prenez des photos de votre ESP32 :
```bash
cp photo.jpg assets/screenshots/
git add assets/screenshots/
git commit -m "📸 Add screenshots"
git push
```

### 4. Créer votre première release

Sur GitHub :
- Releases → Create a new release
- Tag : v3.0.0
- Title : "v3.0.0 - Ultra Modern Edition"
- Publish !

---

## 📚 Ressources

- [Git Documentation](https://git-scm.com/doc)
- [GitHub Guides](https://guides.github.com/)
- [GitHub Pages Docs](https://docs.github.com/pages)
- [ESP Web Tools](https://esphome.github.io/esp-web-tools/)

---

<div align="center">

## 🎉 VOUS L'AVEZ FAIT ! 🎉

**Votre projet est maintenant en ligne !**

### Partagez-le avec le monde ! 🌍

**Twitter :** Tweetez votre lien !
**Reddit :** Postez sur r/Bitcoin, r/lightningnetwork
**Telegram :** Partagez dans les groupes Bitcoin

---

**Stack Sats. Stay Humble. HODL Hard.** 💎🙌⚡

*"Fix the money, fix the world"*

**WAGMI 🚀**

</div>
