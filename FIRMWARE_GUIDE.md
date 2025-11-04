# 📦 Generating Firmware Files for Web Flasher

This guide explains how to generate the `.bin` files needed for the ESP Web Tools flasher.

## 🎯 What Files Do You Need?

The web flasher needs these binary files:

```
firmware/
├── bootloader.bin          # ESP32 bootloader
├── partitions.bin          # Partition table
├── boot_app0.bin          # Boot app
└── lnbits_wallet_ultra_modern.bin  # Your main firmware
```

## 🛠️ Method 1: Arduino IDE (Easiest)

### Step 1: Prepare Arduino IDE

1. **Install ESP32 board support**
   - File → Preferences → Additional Boards Manager URLs:
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
   - Tools → Board → Boards Manager → Search "ESP32" → Install

2. **Install required libraries**
   - TFT_eSPI (v2.5.0+)
   - ArduinoJson (v7.0+)

3. **Select your board**
   - Tools → Board → ESP32 Arduino → **LILYGO T-Display S3**
   - Or: **ESP32S3 Dev Module** (for generic ESP32-S3)

### Step 2: Configure Upload Settings

```
Tools → Board: "LILYGO T-Display S3"
Tools → Upload Speed: "921600"
Tools → USB CDC On Boot: "Enabled"
Tools → CPU Frequency: "240MHz"
Tools → Flash Size: "16MB (128Mb)"
Tools → Partition Scheme: "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
Tools → PSRAM: "OCI PSRAM"
```

### Step 3: Export Compiled Binary

1. **Open your .ino file**
   - `lnbits_wallet_ULTRA_MODERN_v2.ino`

2. **Verify (compile) the sketch**
   - Sketch → Verify/Compile
   - Wait for compilation to finish

3. **Export binary**
   - Sketch → **Export Compiled Binary**
   - Arduino IDE will generate binaries in your sketch folder

4. **Find the files**
   
   Location (Windows):
   ```
   C:\Users\YourName\Documents\Arduino\lnbits_wallet_ULTRA_MODERN_v2\
   ```
   
   Location (Mac):
   ```
   ~/Documents/Arduino/lnbits_wallet_ULTRA_MODERN_v2/
   ```
   
   Location (Linux):
   ```
   ~/Arduino/lnbits_wallet_ULTRA_MODERN_v2/
   ```

5. **Generated files:**
   - `lnbits_wallet_ULTRA_MODERN_v2.ino.bin` → Rename to `lnbits_wallet_ultra_modern.bin`
   - `lnbits_wallet_ULTRA_MODERN_v2.ino.bootloader.bin` → Rename to `bootloader.bin`
   - `lnbits_wallet_ULTRA_MODERN_v2.ino.partitions.bin` → Keep as `partitions.bin`

### Step 4: Get boot_app0.bin

The `boot_app0.bin` file is located in the ESP32 Arduino core:

**Windows:**
```
C:\Users\YourName\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.14\tools\partitions\boot_app0.bin
```

**Mac:**
```
~/Library/Arduino15/packages/esp32/hardware/esp32/2.0.14/tools/partitions/boot_app0.bin
```

**Linux:**
```
~/.arduino15/packages/esp32/hardware/esp32/2.0.14/tools/partitions/boot_app0.bin
```

Copy this file to your firmware folder.

---

## 🛠️ Method 2: Arduino CLI (For Automation)

### Step 1: Install Arduino CLI

**Windows (with chocolatey):**
```bash
choco install arduino-cli
```

**Mac:**
```bash
brew install arduino-cli
```

**Linux:**
```bash
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
```

### Step 2: Configure & Compile

```bash
# Initialize config
arduino-cli config init

# Add ESP32 board support
arduino-cli config add board_manager.additional_urls https://espressif.github.io/arduino-esp32/package_esp32_index.json

# Update index
arduino-cli core update-index

# Install ESP32 core
arduino-cli core install esp32:esp32

# Install libraries
arduino-cli lib install "TFT_eSPI"
arduino-cli lib install "ArduinoJson"

# Compile
arduino-cli compile --fqbn esp32:esp32:lilygo_t_display_s3 lnbits_wallet_ULTRA_MODERN_v2/

# Find binaries in:
# build/esp32.esp32.lilygo_t_display_s3/
```

---

## 🛠️ Method 3: PlatformIO (Advanced)

### Step 1: Install PlatformIO

```bash
# Using pip
pip install platformio

# Or download PlatformIO IDE
# https://platformio.org/install/ide?install=vscode
```

### Step 2: Create platformio.ini

```ini
[env:lilygo-t-display-s3]
platform = espressif32
board = lilygo-t-display-s3
framework = arduino

; Upload settings
upload_speed = 921600
monitor_speed = 115200

; Build settings
build_flags = 
    -DBOARD_HAS_PSRAM
    -DARDUINO_USB_CDC_ON_BOOT=1
    -DARDUINO_USB_MODE=1

; Libraries
lib_deps = 
    bodmer/TFT_eSPI@^2.5.0
    bblanchon/ArduinoJson@^7.0.0
```

### Step 3: Build & Export

```bash
# Build project
pio run

# Export binaries
pio run --target buildfs

# Find files in:
# .pio/build/lilygo-t-display-s3/
```

---

## 📁 Organizing Your Firmware Folder

Create this structure:

```
firmware/
├── bootloader.bin           (from Arduino export)
├── partitions.bin           (from Arduino export)
├── boot_app0.bin           (from ESP32 core folder)
└── lnbits_wallet_ultra_modern.bin  (your main firmware)
```

For ESP32 (non-S3) boards, create a subfolder:

```
firmware/
├── bootloader.bin           (ESP32-S3 files)
├── partitions.bin
├── boot_app0.bin
├── lnbits_wallet_ultra_modern.bin
└── esp32/                   (ESP32 regular files)
    ├── bootloader.bin
    ├── partitions.bin
    ├── boot_app0.bin
    └── lnbits_wallet_ultra_modern.bin
```

---

## ✅ Verification

### Check File Sizes

Typical file sizes:
- `bootloader.bin`: ~20-30 KB
- `partitions.bin`: ~3 KB
- `boot_app0.bin`: ~4 KB
- `lnbits_wallet_ultra_modern.bin`: ~800 KB - 1.5 MB

### Verify Binary Integrity

```bash
# Check if binaries are valid (Linux/Mac)
file bootloader.bin
# Should output: "bootloader.bin: data"

# Windows: use HxD or similar hex editor
# File should start with: E9 (ESP32 magic byte)
```

---

## 🌐 Deploy to GitHub Pages

### Step 1: Create Repository Structure

```
your-repo/
├── index.html              (webflasher page)
├── manifest.json          (ESP Web Tools config)
├── README.md
└── firmware/
    ├── bootloader.bin
    ├── partitions.bin
    ├── boot_app0.bin
    └── lnbits_wallet_ultra_modern.bin
```

### Step 2: Enable GitHub Pages

1. Go to your repo on GitHub
2. Settings → Pages
3. Source: Deploy from branch
4. Branch: `main` → Root
5. Save

### Step 3: Access Your Flasher

Your page will be available at:
```
https://yourusername.github.io/lnbits-esp32-wallet/
```

---

## 🔄 Updating Firmware

To release a new version:

1. **Update version in code:**
   ```cpp
   // In your .ino file
   const char* VERSION = "v3.1.0";
   ```

2. **Recompile & export**
   - Sketch → Export Compiled Binary

3. **Replace firmware files**
   - Update `lnbits_wallet_ultra_modern.bin`
   - Keep bootloader/partitions same (unless updating ESP32 core)

4. **Update manifest.json**
   ```json
   {
     "version": "3.1.0-degen",
     ...
   }
   ```

5. **Commit & push to GitHub**
   ```bash
   git add firmware/lnbits_wallet_ultra_modern.bin manifest.json
   git commit -m "Release v3.1.0 - New features"
   git push
   ```

6. **Users can reflash**
   - Visit your GitHub Pages URL
   - Click flash button
   - New version installed!

---

## 🐛 Troubleshooting

### "Failed to open serial port"
- **Check:** ESP32 is connected via USB
- **Check:** No other program using serial port
- **Try:** Different USB cable (some are charge-only)

### "Failed to initialize"
- **Check:** Bootloader offset is correct (4096 for ESP32-S3)
- **Check:** Using correct chipFamily in manifest.json
- **Try:** Press and hold BOOT button during flash

### "Partition table invalid"
- **Check:** Partitions.bin is correct for your board
- **Check:** Flash size matches partition table
- **Try:** Use a fresh export from Arduino IDE

### Web flasher shows no devices
- **Check:** Using Chrome/Edge/Opera (Web Serial API)
- **Check:** Browser permissions granted
- **Try:** Enable "Experimental Web Platform features" in chrome://flags

---

## 📚 Additional Resources

- [ESP Web Tools Documentation](https://esphome.github.io/esp-web-tools/)
- [Arduino CLI Guide](https://arduino.github.io/arduino-cli/)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [ESP32 Partition Tables](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/partition-tables.html)

---

## 🎉 Success!

Once you have all the binary files:
1. Upload them to your GitHub repo
2. Commit and push
3. Wait for GitHub Pages to deploy (~2 minutes)
4. Share your flasher URL with fellow degens!

**Your web flasher is now live and ready to stack sats! 🚀⚡**

---

<div align="center">

### Questions?

Open an issue on GitHub or DM on Twitter

**WAGMI 💎🙌**

</div>
