#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <SPI.h>

// Configuration EEPROM
#define EEPROM_SIZE 512
#define SSID_ADDR 0
#define PASS_ADDR 32
#define API_KEY_ADDR 64
#define API_URL_ADDR 128
#define WALLET_ID_ADDR 256

WebServer server(80);
TFT_eSPI tft = TFT_eSPI();

struct ConfigData {
  char ssid[32];
  char password[32];
  char apiKey[64];
  char apiUrl[128];
  char walletId[32];
};

struct WalletStats {
  float balance;
  int totalTransactions;
  int receivedCount;
  int sentCount;
  float totalReceived;
  float totalSent;
  long lastTxTime;
  float avgTxAmount;
};

ConfigData config;
WalletStats stats;
HTTPClient http;

// Variables d'animation
unsigned long animationTime = 0;
int animationPhase = 0;
float balanceAnimated = 0;

// Couleurs modernes (RGB565)
#define COLOR_BG 0x0841        // Bleu très foncé
#define COLOR_CARD_BG 0x1082   // Bleu foncé card
#define COLOR_ACCENT 0x07FF    // Cyan électrique
#define COLOR_SUCCESS 0x07E0   // Vert
#define COLOR_WARNING 0xFD20   // Orange
#define COLOR_DANGER 0xF800    // Rouge
#define COLOR_TEXT 0xFFFF      // Blanc
#define COLOR_TEXT_DIM 0x8410  // Gris
#define COLOR_PURPLE 0x781F    // Violet
#define COLOR_GOLD 0xFEA0      // Or

// ============= FONCTIONS GRAPHIQUES MODERNES =============

// Dessine une carte avec effet glassmorphism
void drawGlassCard(int x, int y, int w, int h, uint16_t color) {
  // Ombre
  tft.fillRoundRect(x + 3, y + 3, w, h, 8, 0x18C3);
  
  // Fond de la carte avec bordure
  tft.fillRoundRect(x, y, w, h, 8, COLOR_CARD_BG);
  tft.drawRoundRect(x, y, w, h, 8, color);
  tft.drawRoundRect(x + 1, y + 1, w - 2, h - 2, 7, color);
}

// Dessine une barre de progression moderne
void drawProgressBar(int x, int y, int w, int h, float progress, uint16_t color) {
  // Fond
  tft.fillRoundRect(x, y, w, h, h / 2, COLOR_BG);
  
  // Progression avec dégradé
  int fillWidth = (int)(w * progress);
  if (fillWidth > 0) {
    tft.fillRoundRect(x, y, fillWidth, h, h / 2, color);
    
    // Highlight brillant
    int highlightW = fillWidth / 3;
    for (int i = 0; i < h / 2; i++) {
      uint16_t highlightColor = tft.color565(
        min(255, ((color >> 11) & 0x1F) * 8 + 50 - i * 2),
        min(255, ((color >> 5) & 0x3F) * 4 + 50 - i * 2),
        min(255, (color & 0x1F) * 8 + 50 - i * 2)
      );
      tft.drawFastHLine(x + 2, y + i, highlightW, highlightColor);
    }
  }
  
  // Bordure
  tft.drawRoundRect(x, y, w, h, h / 2, color);
}

// Dessine un mini graphique en barres
void drawMiniBarChart(int x, int y, int w, int h, float sent, float received) {
  int barWidth = w / 3;
  int spacing = 5;
  
  // Normalisation
  float maxVal = max(sent, received);
  if (maxVal == 0) maxVal = 1;
  
  int sentHeight = (int)((sent / maxVal) * h);
  int receivedHeight = (int)((received / maxVal) * h);
  
  // Barre envoyé (rouge)
  if (sentHeight > 0) {
    tft.fillRoundRect(x, y + h - sentHeight, barWidth, sentHeight, 3, COLOR_DANGER);
    tft.drawRoundRect(x, y + h - sentHeight, barWidth, sentHeight, 3, 0xF800);
  }
  
  // Barre reçu (vert)
  if (receivedHeight > 0) {
    tft.fillRoundRect(x + barWidth + spacing, y + h - receivedHeight, barWidth, receivedHeight, 3, COLOR_SUCCESS);
    tft.drawRoundRect(x + barWidth + spacing, y + h - receivedHeight, barWidth, receivedHeight, 3, 0x07E0);
  }
  
  // Légendes
  tft.setTextColor(COLOR_DANGER, COLOR_CARD_BG);
  tft.setTextSize(1);
  tft.setCursor(x, y + h + 3);
  tft.print("OUT");
  
  tft.setTextColor(COLOR_SUCCESS, COLOR_CARD_BG);
  tft.setCursor(x + barWidth + spacing, y + h + 3);
  tft.print("IN");
}

// Icône Bitcoin/Lightning stylisée
void drawLightningIcon(int x, int y, int size, uint16_t color) {
  // Éclair stylisé
  tft.fillTriangle(x + size/2, y, x + size/4, y + size/2, x + size/2, y + size/2, color);
  tft.fillTriangle(x + size/2, y + size/2, x + size*3/4, y + size, x + size/2, y + size, color);
  
  // Cercle autour
  tft.drawCircle(x + size / 2, y + size / 2, size / 2 + 2, color);
  tft.drawCircle(x + size / 2, y + size / 2, size / 2 + 3, color);
}

// Animation de pulsation
void drawPulsingDot(int x, int y, int maxRadius, uint16_t color) {
  int phase = (millis() / 100) % 20;
  int radius = 2 + (phase * maxRadius) / 20;
  int alpha = 255 - (phase * 255) / 20;
  
  uint16_t fadeColor = tft.color565(
    ((color >> 11) & 0x1F) * alpha / 255 * 8,
    ((color >> 5) & 0x3F) * alpha / 255 * 4,
    (color & 0x1F) * alpha / 255 * 8
  );
  
  tft.fillCircle(x, y, radius, fadeColor);
}

// ============= AFFICHAGE ULTRA MODERNE =============

void displayInit() {
  tft.init();
  tft.setRotation(1);  // Paysage
  tft.fillScreen(COLOR_BG);
  tft.setTextColor(COLOR_TEXT, COLOR_BG);
}

void displayWalletInfoUltraModern() {
  tft.fillScreen(COLOR_BG);
  
  // ========== HEADER AVEC LOGO ==========
  drawLightningIcon(10, 5, 20, COLOR_ACCENT);
  
  tft.setTextColor(COLOR_ACCENT, COLOR_BG);
  tft.setTextSize(2);
  tft.setCursor(35, 8);
  tft.print("LIGHTNING");
  
  tft.setTextColor(COLOR_TEXT_DIM, COLOR_BG);
  tft.setTextSize(1);
  tft.setCursor(35, 24);
  tft.print("WALLET");
  
  // Point pulsant pour indiquer la connexion
  drawPulsingDot(290, 15, 8, COLOR_SUCCESS);
  
  // ========== CARTE PRINCIPALE: SOLDE ==========
  drawGlassCard(10, 40, 300, 75, COLOR_ACCENT);
  
  // Label
  tft.setTextColor(COLOR_TEXT_DIM, COLOR_CARD_BG);
  tft.setTextSize(1);
  tft.setCursor(20, 48);
  tft.print("BALANCE");
  
  // Animation du solde
  if (balanceAnimated < stats.balance) {
    balanceAnimated += (stats.balance - balanceAnimated) * 0.1;
    if (abs(balanceAnimated - stats.balance) < 0.1) {
      balanceAnimated = stats.balance;
    }
  }
  
  // Affichage du solde avec effet
  tft.setTextColor(COLOR_ACCENT, COLOR_CARD_BG);
  tft.setTextSize(4);
  tft.setCursor(20, 65);
  
  // Formattage du nombre
  if (balanceAnimated >= 1000000) {
    tft.printf("%.2fM", balanceAnimated / 1000000.0);
  } else if (balanceAnimated >= 1000) {
    tft.printf("%.1fK", balanceAnimated / 1000.0);
  } else {
    tft.printf("%.0f", balanceAnimated);
  }
  
  tft.setTextSize(2);
  tft.setTextColor(COLOR_TEXT_DIM, COLOR_CARD_BG);
  tft.print(" sats");
  
  // ========== STATISTIQUES EN 2 COLONNES ==========
  
  // COLONNE GAUCHE: Transactions
  drawGlassCard(10, 125, 145, 90, COLOR_PURPLE);
  
  tft.setTextColor(COLOR_PURPLE, COLOR_CARD_BG);
  tft.setTextSize(1);
  tft.setCursor(20, 133);
  tft.print("TRANSACTIONS");
  
  // Total
  tft.setTextColor(COLOR_TEXT, COLOR_CARD_BG);
  tft.setTextSize(3);
  tft.setCursor(20, 150);
  tft.print(stats.totalTransactions);
  
  tft.setTextSize(1);
  tft.setTextColor(COLOR_TEXT_DIM, COLOR_CARD_BG);
  tft.setCursor(20, 175);
  tft.print("TOTAL");
  
  // Détails IN/OUT
  tft.setTextColor(COLOR_SUCCESS, COLOR_CARD_BG);
  tft.setCursor(20, 188);
  tft.printf("IN:  %d", stats.receivedCount);
  
  tft.setTextColor(COLOR_DANGER, COLOR_CARD_BG);
  tft.setCursor(20, 198);
  tft.printf("OUT: %d", stats.sentCount);
  
  // COLONNE DROITE: Volume
  drawGlassCard(165, 125, 145, 90, COLOR_GOLD);
  
  tft.setTextColor(COLOR_GOLD, COLOR_CARD_BG);
  tft.setTextSize(1);
  tft.setCursor(175, 133);
  tft.print("VOLUME");
  
  // Mini graphique
  drawMiniBarChart(175, 148, 50, 40, stats.totalSent, stats.totalReceived);
  
  // Montants
  tft.setTextColor(COLOR_SUCCESS, COLOR_CARD_BG);
  tft.setTextSize(1);
  tft.setCursor(235, 153);
  tft.print("IN:");
  tft.setCursor(235, 163);
  if (stats.totalReceived >= 1000) {
    tft.printf("%.1fK", stats.totalReceived / 1000.0);
  } else {
    tft.printf("%.0f", stats.totalReceived);
  }
  
  tft.setTextColor(COLOR_DANGER, COLOR_CARD_BG);
  tft.setCursor(235, 178);
  tft.print("OUT:");
  tft.setCursor(235, 188);
  if (stats.totalSent >= 1000) {
    tft.printf("%.1fK", stats.totalSent / 1000.0);
  } else {
    tft.printf("%.0f", stats.totalSent);
  }
  
  // Moyenne par transaction
  tft.setTextColor(COLOR_TEXT_DIM, COLOR_CARD_BG);
  tft.setCursor(175, 198);
  tft.print("AVG: ");
  tft.setTextColor(COLOR_TEXT, COLOR_CARD_BG);
  tft.printf("%.0f", stats.avgTxAmount);
  
  // ========== BARRE DE STATUS EN BAS ==========
  int barY = 225;
  
  // WiFi
  tft.setTextColor(COLOR_SUCCESS, COLOR_BG);
  tft.setTextSize(1);
  tft.setCursor(10, barY);
  tft.print("WiFi");
  drawProgressBar(40, barY, 60, 8, 1.0, COLOR_SUCCESS);
  
  // API Status
  tft.setTextColor(COLOR_ACCENT, COLOR_BG);
  tft.setCursor(110, barY);
  tft.print("API");
  drawProgressBar(135, barY, 60, 8, 1.0, COLOR_ACCENT);
  
  // Sync indicator
  tft.setTextColor(COLOR_TEXT_DIM, COLOR_BG);
  tft.setCursor(205, barY);
  unsigned long secondsSinceUpdate = (millis() - animationTime) / 1000;
  tft.printf("Sync: %lus", min(secondsSinceUpdate, 30UL));
}

void displayMessage(const char* msg) {
  tft.fillScreen(COLOR_BG);
  
  // Carte centrée
  drawGlassCard(60, 90, 200, 60, COLOR_ACCENT);
  
  tft.setTextColor(COLOR_TEXT, COLOR_CARD_BG);
  tft.setTextSize(2);
  
  // Centrer le texte approximativement (6 pixels par char en size 1, x2 = 12)
  int textWidth = strlen(msg) * 12;
  int x = 160 - (textWidth / 2);
  if (x < 70) x = 70; // Limite gauche
  
  tft.setCursor(x, 115);
  tft.print(msg);
}

void displayLoadingAnimation() {
  static int loadFrame = 0;
  loadFrame = (loadFrame + 1) % 60;
  
  tft.fillScreen(COLOR_BG);
  
  // Logo central animé
  int centerX = 160;
  int centerY = 120;
  
  // Cercles animés
  for (int i = 0; i < 3; i++) {
    int offset = (loadFrame + i * 20) % 60;
    int radius = 20 + offset;
    int alpha = 255 - (offset * 4);
    
    uint16_t fadeColor = tft.color565(
      ((COLOR_ACCENT >> 11) & 0x1F) * alpha / 255 * 8,
      ((COLOR_ACCENT >> 5) & 0x3F) * alpha / 255 * 4,
      (COLOR_ACCENT & 0x1F) * alpha / 255 * 8
    );
    
    tft.drawCircle(centerX, centerY, radius, fadeColor);
  }
  
  drawLightningIcon(centerX - 15, centerY - 15, 30, COLOR_ACCENT);
  
  tft.setTextColor(COLOR_TEXT, COLOR_BG);
  tft.setTextSize(2);
  tft.setCursor(centerX - 35, centerY + 30);
  tft.print("Loading");
  
  // Points animés
  int dots = (loadFrame / 10) % 4;
  for (int i = 0; i < dots; i++) {
    tft.print(".");
  }
}

// ============= FONCTION EEPROM =============
void loadConfig() {
  EEPROM.begin(EEPROM_SIZE);
  for(int i = 0; i < 32; i++) config.ssid[i] = EEPROM.read(SSID_ADDR + i);
  for(int i = 0; i < 32; i++) config.password[i] = EEPROM.read(PASS_ADDR + i);
  for(int i = 0; i < 64; i++) config.apiKey[i] = EEPROM.read(API_KEY_ADDR + i);
  for(int i = 0; i < 128; i++) config.apiUrl[i] = EEPROM.read(API_URL_ADDR + i);
  for(int i = 0; i < 32; i++) config.walletId[i] = EEPROM.read(WALLET_ID_ADDR + i);
}

void saveConfig() {
  for(int i = 0; i < 32; i++) EEPROM.write(SSID_ADDR + i, config.ssid[i]);
  for(int i = 0; i < 32; i++) EEPROM.write(PASS_ADDR + i, config.password[i]);
  for(int i = 0; i < 64; i++) EEPROM.write(API_KEY_ADDR + i, config.apiKey[i]);
  for(int i = 0; i < 128; i++) EEPROM.write(API_URL_ADDR + i, config.apiUrl[i]);
  for(int i = 0; i < 32; i++) EEPROM.write(WALLET_ID_ADDR + i, config.walletId[i]);
  EEPROM.commit();
}

// ============= DASHBOARD WEB MODERNE =============
String getHtmlPageModern() {
  return R"(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>⚡ LNbits Dashboard</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }

    :root {
      --primary: #00d9ff;
      --primary-dark: #0099cc;
      --secondary: #ff006e;
      --success: #00ff41;
      --warning: #ffb703;
      --danger: #ff006e;
      --dark-bg: #0a0e27;
      --card-bg: #1a1f3a;
      --border-color: #00d9ff;
      --text-primary: #ffffff;
      --text-secondary: #b0b8cc;
    }

    body {
      background: linear-gradient(135deg, #0a0e27 0%, #1a1f3a 100%);
      color: var(--text-primary);
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      min-height: 100vh;
      overflow-x: hidden;
      position: relative;
    }

    body::before {
      content: '';
      position: fixed;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      background: 
        radial-gradient(circle at 20% 50%, rgba(0, 217, 255, 0.1) 0%, transparent 50%),
        radial-gradient(circle at 80% 80%, rgba(255, 0, 110, 0.1) 0%, transparent 50%);
      pointer-events: none;
      z-index: 0;
    }

    .container {
      max-width: 600px;
      margin: 0 auto;
      padding: 20px;
      position: relative;
      z-index: 1;
    }

    .header {
      text-align: center;
      margin-bottom: 40px;
      animation: slideDown 0.6s ease-out;
    }

    .header h1 {
      font-size: 2.5em;
      background: linear-gradient(135deg, var(--primary), var(--secondary));
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
      background-clip: text;
      margin-bottom: 10px;
      letter-spacing: 2px;
      font-weight: 700;
    }

    .header p {
      color: var(--text-secondary);
      font-size: 0.95em;
      letter-spacing: 1px;
    }

    .card {
      background: var(--card-bg);
      border: 2px solid var(--border-color);
      border-radius: 15px;
      padding: 25px;
      margin-bottom: 20px;
      backdrop-filter: blur(10px);
      box-shadow: 0 8px 32px rgba(0, 217, 255, 0.1);
      transition: all 0.3s ease;
      animation: fadeInUp 0.6s ease-out both;
    }

    .card:hover {
      transform: translateY(-5px);
      box-shadow: 0 12px 48px rgba(0, 217, 255, 0.2);
      border-color: var(--primary);
    }

    .card h2 {
      font-size: 1.1em;
      color: var(--primary);
      margin-bottom: 20px;
      display: flex;
      align-items: center;
      gap: 10px;
      letter-spacing: 1px;
      text-transform: uppercase;
    }

    .card h2::before {
      content: '';
      width: 4px;
      height: 20px;
      background: linear-gradient(to bottom, var(--primary), var(--secondary));
      border-radius: 2px;
    }

    .form-group {
      margin-bottom: 20px;
      animation: fadeInUp 0.6s ease-out both;
    }

    .form-group:nth-child(1) { animation-delay: 0.1s; }
    .form-group:nth-child(2) { animation-delay: 0.2s; }
    .form-group:nth-child(3) { animation-delay: 0.3s; }
    .form-group:nth-child(4) { animation-delay: 0.4s; }
    .form-group:nth-child(5) { animation-delay: 0.5s; }

    label {
      display: block;
      margin-bottom: 8px;
      color: var(--primary);
      font-weight: 600;
      font-size: 0.9em;
      letter-spacing: 0.5px;
      text-transform: uppercase;
    }

    input {
      width: 100%;
      padding: 12px 15px;
      border: 2px solid var(--border-color);
      border-radius: 8px;
      background: rgba(10, 14, 39, 0.5);
      color: var(--text-primary);
      font-size: 1em;
      font-family: 'Courier New', monospace;
      transition: all 0.3s ease;
      backdrop-filter: blur(5px);
    }

    input:focus {
      outline: none;
      border-color: var(--primary);
      box-shadow: 0 0 20px rgba(0, 217, 255, 0.3);
      background: rgba(10, 14, 39, 0.8);
    }

    input::placeholder {
      color: var(--text-secondary);
    }

    button {
      width: 100%;
      padding: 15px;
      background: linear-gradient(135deg, var(--primary), var(--primary-dark));
      color: var(--dark-bg);
      border: none;
      border-radius: 8px;
      cursor: pointer;
      font-weight: 700;
      font-size: 1em;
      letter-spacing: 1px;
      text-transform: uppercase;
      transition: all 0.3s ease;
      margin-top: 10px;
      position: relative;
      overflow: hidden;
    }

    button::before {
      content: '';
      position: absolute;
      top: 0;
      left: -100%;
      width: 100%;
      height: 100%;
      background: linear-gradient(90deg, transparent, rgba(255,255,255,0.2), transparent);
      transition: left 0.5s;
    }

    button:hover::before {
      left: 100%;
    }

    button:hover {
      transform: translateY(-2px);
      box-shadow: 0 10px 30px rgba(0, 217, 255, 0.3);
    }

    button:active {
      transform: translateY(0);
    }

    .status {
      padding: 15px;
      border-radius: 8px;
      margin-top: 20px;
      font-weight: 600;
      animation: slideInUp 0.4s ease-out;
    }

    .status.success {
      background: rgba(0, 255, 65, 0.1);
      border: 2px solid var(--success);
      color: var(--success);
    }

    .status.error {
      background: rgba(255, 0, 110, 0.1);
      border: 2px solid var(--danger);
      color: var(--danger);
    }

    .status.loading {
      background: rgba(0, 217, 255, 0.1);
      border: 2px solid var(--primary);
      color: var(--primary);
      display: flex;
      align-items: center;
      gap: 15px;
    }

    .spinner {
      width: 20px;
      height: 20px;
      border: 3px solid var(--primary);
      border-top-color: transparent;
      border-radius: 50%;
      animation: spin 1s linear infinite;
    }

    .info-box {
      background: rgba(0, 217, 255, 0.05);
      border-left: 4px solid var(--primary);
      padding: 15px;
      margin-top: 15px;
      border-radius: 4px;
      font-size: 0.9em;
      line-height: 1.6;
      color: var(--text-secondary);
    }

    .footer {
      text-align: center;
      margin-top: 40px;
      padding: 20px;
      color: var(--text-secondary);
      font-size: 0.9em;
    }

    @keyframes slideDown {
      from {
        opacity: 0;
        transform: translateY(-30px);
      }
      to {
        opacity: 1;
        transform: translateY(0);
      }
    }

    @keyframes fadeInUp {
      from {
        opacity: 0;
        transform: translateY(20px);
      }
      to {
        opacity: 1;
        transform: translateY(0);
      }
    }

    @keyframes slideInUp {
      from {
        opacity: 0;
        transform: translateY(10px);
      }
      to {
        opacity: 1;
        transform: translateY(0);
      }
    }

    @keyframes spin {
      to {
        transform: rotate(360deg);
      }
    }

    @media (max-width: 768px) {
      .container {
        padding: 10px;
      }
      
      .header h1 {
        font-size: 2em;
      }
      
      .card {
        padding: 20px;
      }
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="header">
      <h1>⚡ LNbits Dashboard</h1>
      <p>Configuration Ultra-Moderne</p>
    </div>

    <form id="config-form">
      <div class="card">
        <h2>📡 Configuration WiFi</h2>
        
        <div class="form-group">
          <label for="ssid">Nom du réseau (SSID)</label>
          <input 
            type="text" 
            id="ssid" 
            name="ssid" 
            placeholder="Votre réseau WiFi"
            autocomplete="off"
            required
          >
        </div>

        <div class="form-group">
          <label for="password">Mot de passe</label>
          <input 
            type="password" 
            id="password" 
            name="password" 
            placeholder="••••••••"
            autocomplete="off"
            required
          >
        </div>
      </div>

      <div class="card">
        <h2>💰 Configuration LNbits</h2>
        
        <div class="form-group">
          <label for="apiUrl">URL de votre instance LNbits</label>
          <input 
            type="url" 
            id="apiUrl" 
            name="apiUrl" 
            placeholder="https://your-instance.lnbits.com"
            autocomplete="off"
            required
          >
        </div>

        <div class="form-group">
          <label for="walletId">Wallet ID</label>
          <input 
            type="text" 
            id="walletId" 
            name="walletId" 
            placeholder="Identifiant de votre portefeuille"
            autocomplete="off"
            required
          >
        </div>

        <div class="form-group">
          <label for="apiKey">Admin API Key</label>
          <input 
            type="password" 
            id="apiKey" 
            name="apiKey" 
            placeholder="sk_live_xxxxxxxxxxxxx"
            autocomplete="off"
            required
          >
        </div>

        <div class="info-box">
          💡 <strong>Comment trouver ces informations ?</strong><br>
          1. Connectez-vous à votre instance LNbits<br>
          2. Sélectionnez votre wallet<br>
          3. Allez dans "API Keys"<br>
          4. Copiez l'ID et la clé admin
        </div>
      </div>

      <button type="submit">💾 Enregistrer et Redémarrer</button>
    </form>

    <div id="status"></div>

    <div class="footer">
      <p>⚡ LNbits Wallet Display Ultra v3.0</p>
      <p style="margin-top: 10px; font-size: 0.75em;">
        Powered by ESP32 + Lightning Network
      </p>
    </div>
  </div>

  <script>
    const form = document.getElementById('config-form');
    const statusDiv = document.getElementById('status');

    async function loadConfig() {
      try {
        const response = await fetch('/api/config');
        if (response.ok) {
          const data = await response.json();
          document.getElementById('ssid').value = data.ssid || '';
          document.getElementById('password').value = data.password || '';
          document.getElementById('apiUrl').value = data.apiUrl || '';
          document.getElementById('walletId').value = data.walletId || '';
          document.getElementById('apiKey').value = data.apiKey || '';
        }
      } catch (error) {
        console.error('Erreur lors du chargement:', error);
      }
    }

    form.addEventListener('submit', async (e) => {
      e.preventDefault();

      const formData = new FormData(form);
      const data = Object.fromEntries(formData);

      statusDiv.innerHTML = '<div class="status loading"><span class="spinner"></span>Enregistrement en cours...</div>';

      try {
        const response = await fetch('/api/config', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(data)
        });

        if (response.ok) {
          statusDiv.innerHTML = '<div class="status success">✓ Configuration enregistrée avec succès!</div>';
          
          setTimeout(() => {
            statusDiv.innerHTML += '<div class="status loading" style="margin-top: 15px;"><span class="spinner"></span>Redémarrage de l\'appareil...</div>';
          }, 1000);
        } else {
          statusDiv.innerHTML = '<div class="status error">✗ Erreur lors de l\'enregistrement</div>';
        }
      } catch (error) {
        statusDiv.innerHTML = '<div class="status error">✗ Erreur de connexion. Vérifiez votre réseau.</div>';
        console.error('Erreur:', error);
      }
    });

    loadConfig();
  </script>
</body>
</html>
  )";
}

void handleRoot() {
  server.send(200, "text/html", getHtmlPageModern());
}

void handleGetConfig() {
  DynamicJsonDocument doc(512);
  doc["ssid"] = config.ssid;
  doc["password"] = config.password;
  doc["apiUrl"] = config.apiUrl;
  doc["walletId"] = config.walletId;
  doc["apiKey"] = config.apiKey;

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handlePostConfig() {
  if (server.hasArg("plain")) {
    DynamicJsonDocument doc(512);
    deserializeJson(doc, server.arg("plain"));

    if (doc["ssid"]) strlcpy(config.ssid, doc["ssid"], sizeof(config.ssid));
    if (doc["password"]) strlcpy(config.password, doc["password"], sizeof(config.password));
    if (doc["apiUrl"]) strlcpy(config.apiUrl, doc["apiUrl"], sizeof(config.apiUrl));
    if (doc["walletId"]) strlcpy(config.walletId, doc["walletId"], sizeof(config.walletId));
    if (doc["apiKey"]) strlcpy(config.apiKey, doc["apiKey"], sizeof(config.apiKey));

    saveConfig();
    server.send(200, "application/json", "{\"status\":\"ok\"}");
    
    delay(1000);
    ESP.restart();
  } else {
    server.send(400, "application/json", "{\"error\":\"Invalid request\"}");
  }
}

// ============= LNBITS API AVEC STATS COMPLETES =============
bool getLNbitsWalletData() {
  if (strlen(config.apiUrl) == 0 || strlen(config.apiKey) == 0) {
    return false;
  }

  // Récupérer le solde
  String url = String(config.apiUrl) + "/api/v1/wallet";
  
  http.begin(url);
  http.addHeader("X-Api-Key", config.apiKey);

  int httpCode = http.GET();
  
  if (httpCode == 200) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, http.getString());
    
    stats.balance = (doc["balance"] | 0) / 1000.0;
    http.end();
    
    // Récupérer TOUTES les transactions pour les stats complètes
    String txUrl = String(config.apiUrl) + "/api/v1/payments?limit=100";
    http.begin(txUrl);
    http.addHeader("X-Api-Key", config.apiKey);
    
    int txCode = http.GET();
    if (txCode == 200) {
      DynamicJsonDocument txDoc(8192);
      deserializeJson(txDoc, http.getString());
      
      JsonArray payments = txDoc["payments"];
      
      stats.totalTransactions = payments.size();
      stats.receivedCount = 0;
      stats.sentCount = 0;
      stats.totalReceived = 0;
      stats.totalSent = 0;
      stats.lastTxTime = 0;
      
      // Analyser toutes les transactions
      for (JsonObject tx : payments) {
        float amount = (tx["amount"] | 0) / 1000.0;
        long txTime = tx["time"] | 0;
        bool pending = tx["pending"] | false;
        
        // Ignorer les transactions en attente
        if (pending) continue;
        
        if (amount > 0) {
          // Transaction reçue
          stats.receivedCount++;
          stats.totalReceived += amount;
        } else {
          // Transaction envoyée
          stats.sentCount++;
          stats.totalSent += abs(amount);
        }
        
        // Dernière transaction
        if (txTime > stats.lastTxTime) {
          stats.lastTxTime = txTime;
        }
      }
      
      // Calculer la moyenne
      if (stats.totalTransactions > 0) {
        stats.avgTxAmount = (stats.totalReceived + stats.totalSent) / stats.totalTransactions;
      } else {
        stats.avgTxAmount = 0;
      }
    }
    
    http.end();
    return true;
  }
  
  http.end();
  return false;
}

// ============= SETUP WIFI =============
void setupWiFi() {
  if (strlen(config.ssid) > 0) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.ssid, config.password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      displayLoadingAnimation();
      delay(500);
      attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      displayMessage("WiFi OK!");
      delay(2000);
      return;
    }
  }
  
  displayMessage("Config Mode");
  WiFi.softAP("LNbits-Config", "12345678");
  delay(2000);
}

// ============= SETUP SERVEUR WEB =============
void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/api/config", HTTP_GET, handleGetConfig);
  server.on("/api/config", HTTP_POST, handlePostConfig);
  server.begin();
}

// ============= SETUP =============
void setup() {
  Serial.begin(115200);
  displayInit();
  
  loadConfig();
  displayMessage("Init...");
  delay(1000);
  
  setupWiFi();
  setupWebServer();
  
  displayMessage("Ready!");
  delay(1000);
  
  // Initialiser les stats
  stats.balance = 0;
  stats.totalTransactions = 0;
  stats.receivedCount = 0;
  stats.sentCount = 0;
  stats.totalReceived = 0;
  stats.totalSent = 0;
  stats.avgTxAmount = 0;
  balanceAnimated = 0;
}

// ============= LOOP PRINCIPAL =============
unsigned long lastUpdate = 0;
int displayRefresh = 0;

void loop() {
  server.handleClient();
  
  // Mise à jour des données toutes les 30 secondes
  if (millis() - lastUpdate > 30000) {
    lastUpdate = millis();
    animationTime = millis();
    
    if (WiFi.status() == WL_CONNECTED) {
      if (getLNbitsWalletData()) {
        displayWalletInfoUltraModern();
      } else {
        displayMessage("API Error");
      }
    } else {
      displayMessage("No WiFi");
    }
  }
  
  // Rafraîchissement de l'affichage pour les animations (toutes les 100ms)
  if (millis() - displayRefresh > 100) {
    displayRefresh = millis();
    if (WiFi.status() == WL_CONNECTED && stats.balance > 0) {
      displayWalletInfoUltraModern();
    }
  }
  
  delay(50);
}
