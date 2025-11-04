# ⚡₿ LNbits Lightning Wallet - Bitcoin Maxi Edition

> **Stack Sats. Lightning Fast. Ultra Degen.**

![Bitcoin](https://img.shields.io/badge/Bitcoin-F7931A?style=for-the-badge&logo=bitcoin&logoColor=white)
![Lightning Network](https://img.shields.io/badge/Lightning-792EE5?style=for-the-badge&logo=lightning&logoColor=white)
![ESP32](https://img.shields.io/badge/ESP32-000000?style=for-the-badge&logo=espressif&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-orange?style=for-the-badge)

## 🚀 What is This?

An **ultra-modern**, **Bitcoin Maxi approved** LNbits wallet display for ESP32 with TFT screens. Because displaying your sats should be as beautiful as the technology securing them.

### ✨ Why This Exists

- ✅ Monitor your Lightning wallet in **STYLE**
- ✅ Real-time balance tracking
- ✅ Glassmorphism design (because we're in 2025, anon)
- ✅ 100% Open Source (as Satoshi intended)
- ✅ One-click web flasher (no Arduino IDE needed)
- ✅ Made by degens, for degens

## 🎨 Screenshots

### Ultra-Modern Display
```
┌─────────────────────────────────────┐
│ ⚡ LIGHTNING WALLET      ⭕         │
├─────────────────────────────────────┤
│ ┌─────────────────────────────────┐ │
│ │ BALANCE                         │ │
│ │ 1,234,567 sats                  │ │
│ └─────────────────────────────────┘ │
├─────────────────────────────────────┤
│ ┌─────────────┐  ┌─────────────┐   │
│ │TRANSACTIONS │  │   VOLUME    │   │
│ │   125       │  │ ┌──┐  ┌──┐  │   │
│ │             │  │ │IN│  │OUT│  │   │
│ │ IN:  75     │  │ └──┘  └──┘  │   │
│ │ OUT: 50     │  │ AVG: 987    │   │
│ └─────────────┘  └─────────────┘   │
├─────────────────────────────────────┤
│ WiFi [====] API [====] Sync: 5s    │
└─────────────────────────────────────┘
```

## 💎 Features That Make You HODL

### 🎯 Core Features
- **Real-time Balance** - Watch your sats stack in real-time
- **Transaction History** - Last 100 transactions analyzed
- **IN/OUT Analysis** - See where your sats flow
- **Volume Charts** - Mini bar charts for quick insights
- **Average TX Amount** - Know your spending habits
- **Auto-Refresh** - Updates every 30 seconds
- **Animated UI** - Smooth transitions and pulsing indicators

### 🎨 Design Excellence
- **Glassmorphism** - Modern frosted glass effect cards
- **Neon Borders** - Cyberpunk-inspired glowing edges
- **Animated Balance** - Numbers count up smoothly
- **Pulsating Status** - Live connection indicator
- **Progress Bars** - WiFi and API status with shine effect
- **Lightning Logo** - Animated icon with pulse effect

### 🔧 Technical Specs
- **ESP32-S3** optimized (LILYGO T-Display S3 recommended)
- **TFT_eSPI** compatible (320x240 displays)
- **LNbits API** v1 integration
- **HTTPS** support for secure connections
- **Web Config Portal** - No code editing needed
- **EEPROM Storage** - Saves your config

## 🛠️ Installation (3 Ways)

### Method 1: Web Flasher (Recommended for Normies)

1. **Visit:** [https://yourusername.github.io/lnbits-esp32-wallet](https://yourusername.github.io/lnbits-esp32-wallet)
2. **Click:** The big orange "FLASH FIRMWARE" button
3. **Connect:** Your ESP32 via USB
4. **Wait:** ~2 minutes (check BTC price meanwhile)
5. **Configure:** Connect to "LNbits-Config" WiFi
6. **Done:** Stack sats in style! 🚀

**Browser Requirements:** Chrome, Edge, or Opera (Web Serial API needed)

### Method 2: Arduino IDE (For Devs)

```bash
# 1. Clone this repo
git clone https://github.com/yourusername/lnbits-esp32-wallet.git
cd lnbits-esp32-wallet

# 2. Install libraries
- TFT_eSPI (v2.5.0+)
- ArduinoJson (v7.0+)
- WiFi (built-in)
- HTTPClient (built-in)

# 3. Configure TFT_eSPI
# Edit: Arduino/libraries/TFT_eSPI/User_Setup.h
# For LILYGO T-Display S3, use their setup file

# 4. Open .ino file and upload
# File: lnbits_wallet_ULTRA_MODERN_v2.ino
```

### Method 3: PlatformIO (For Hardcore Devs)

```bash
# Coming soon™
```

## 📋 Configuration

### First Time Setup

1. **ESP32 creates WiFi AP:** `LNbits-Config`
2. **Password:** `12345678`
3. **Open browser:** `http://192.168.4.1`
4. **Enter details:**
   - WiFi SSID & Password
   - LNbits URL (e.g., `https://legend.lnbits.com`)
   - Wallet ID
   - Admin API Key
5. **Save & Reboot**
6. **HODL** 💎🙌

### Finding Your LNbits Credentials

1. Login to your LNbits instance
2. Select your wallet
3. Click **"API Keys"**
4. Copy:
   - `Wallet ID`
   - `Admin key` (starts with `lnbits_...`)

## 🖥️ Supported Hardware

### ✅ Tested & Working
- **LILYGO T-Display S3** (recommended)
- ESP32-WROOM with ILI9341 TFT
- ESP32-S3 with ST7789 TFT

### 📱 Display Requirements
- Resolution: 320x240 pixels
- Interface: SPI
- Library: TFT_eSPI compatible

## 🔒 Security Notes

> ⚠️ **NOT YOUR KEYS, NOT YOUR COINS**

- Uses **Admin API Key** (read wallet balance)
- **HTTPS** recommended for LNbits connection
- Config stored in **EEPROM** (device-only)
- No keys are transmitted to third parties
- Open source - verify the code yourself

### Best Practices
1. Use a dedicated monitoring wallet
2. Don't use your main stack wallet
3. Enable HTTPS on your LNbits
4. Use strong WiFi password
5. DYOR (Do Your Own Research)

## 📊 API Integration

### LNbits Endpoints Used
```javascript
GET /api/v1/wallet
// Returns: balance, name, id

GET /api/v1/payments?limit=100
// Returns: transaction history
```

### Data Analyzed
- Total balance (in sats)
- Transaction count
- Received transactions (count & amount)
- Sent transactions (count & amount)
- Average transaction size
- Last transaction timestamp

## 🎨 Customization

### Color Schemes

Want different colors? Edit these in the .ino file:

```cpp
#define COLOR_BG 0x0841        // Background
#define COLOR_ACCENT 0x07FF    // Main accent (cyan)
#define COLOR_SUCCESS 0x07E0   // Success (green)
#define COLOR_DANGER 0xF800    // Danger (red)
#define COLOR_PURPLE 0x781F    // Transactions
#define COLOR_GOLD 0xFEA0      // Volume
```

### Refresh Interval

Change update frequency (default 30s):

```cpp
if (millis() - lastUpdate > 30000) { // 30 seconds
  // Change to 60000 for 1 minute
  // Change to 10000 for 10 seconds
}
```

## 🐛 Troubleshooting

### Display Issues

**Screen stays black?**
- Check TFT_eSPI configuration
- Verify SPI pin connections
- Test with TFT_eSPI examples first

**Wrong colors?**
- Adjust RGB565 color definitions
- Check display driver (ST7789 vs ILI9341)

### Connection Issues

**WiFi not connecting?**
- Check SSID/password
- Verify WiFi signal strength
- Check Serial Monitor (115200 baud)

**API Error?**
- Verify LNbits URL (include https://)
- Check API key (admin key, not invoice)
- Test API with curl/Postman first

**"API Error" on display?**
```bash
# Test your API manually:
curl -X GET \
  https://your-instance.lnbits.com/api/v1/wallet \
  -H "X-Api-Key: your_admin_key"
```

## 🤝 Contributing

**All contributions welcome!** We're all Satoshi here.

### How to Contribute
1. Fork the repo
2. Create feature branch (`git checkout -b feature/StackMoreSats`)
3. Commit changes (`git commit -m 'Add some feature'`)
4. Push to branch (`git push origin feature/StackMoreSats`)
5. Open Pull Request

### Ideas for Contribution
- [ ] QR code generation for receiving
- [ ] Price conversion (BTC/USD)
- [ ] Historical chart (7 days)
- [ ] Multi-wallet support
- [ ] Sound notifications
- [ ] NFC payment support
- [ ] Your crazy idea here!

## 📜 Version History

### v3.0.0 - "Ultra Modern" (2025-11-04)
- ✨ Complete UI redesign with glassmorphism
- 📊 Enhanced statistics (100 TX analysis)
- 🎨 Animated charts and progress bars
- ⚡ Lightning icon with pulse effect
- 🔄 Smooth balance animation
- 📱 Responsive refresh (100ms animations)

### v2.0.0 - "Modern" (Previous)
- Basic web dashboard
- Simple display layout
- 10 TX limit
- Basic colors

### v1.0.0 - "Classic" (Ancient History)
- Text-only display
- No animations
- Monochrome vibes

## 🎯 Roadmap

- [ ] **v3.1** - QR code generation
- [ ] **v3.2** - BTC price ticker
- [ ] **v3.5** - Multiple wallet support
- [ ] **v4.0** - Complete UI themes
- [ ] **v5.0** - Hardware wallet integration (?)
- [ ] **v∞.0** - Time travel to check pre-halving prices

## 📄 License

**MIT License** - Do whatever you want with this code.

```
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
Not financial advice. DYOR. NFA. WAGMI.
```

## 🙏 Acknowledgments

- **Satoshi Nakamoto** - For fixing money
- **Lightning Network** - For making it fast
- **LNbits** - For making it accessible
- **ESP32** - For making it cheap
- **Coffee** - For making it possible
- **You** - For stacking sats

## 💬 Community

- 🐦 Twitter: [@yourhandle](https://twitter.com/yourhandle)
- 💬 Telegram: [t.me/yourchannel](https://t.me/yourchannel)
- 🎮 Discord: [discord.gg/yourserver](https://discord.gg/yourserver)

## 🍊 Bitcoin Maxi Badges

![HODL](https://img.shields.io/badge/HODL-💎🙌-orange?style=for-the-badge)
![FEW](https://img.shields.io/badge/FEW-🧠-orange?style=for-the-badge)
![WAGMI](https://img.shields.io/badge/WAGMI-🚀-orange?style=for-the-badge)
![21M](https://img.shields.io/badge/21M-🔢-orange?style=for-the-badge)

---

<div align="center">

### 🧡 Made with ₿itcoin Maximalism

**"Fix the money, fix the world"**

⚡ **Stack Sats. Stay Humble. HODL Hard.** ⚡

[⭐ Star this repo](https://github.com/yourusername/lnbits-esp32-wallet) | [🐛 Report Bug](https://github.com/yourusername/lnbits-esp32-wallet/issues) | [💡 Request Feature](https://github.com/yourusername/lnbits-esp32-wallet/issues)

</div>

---

<div align="center">
<sub>Not your keys, not your coins. Not financial advice. DYOR. NFA. WAGMI. GM. FEW.</sub>
</div>
