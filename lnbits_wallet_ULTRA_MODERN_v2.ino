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

// Variables globales
WebServer server(80);
TFT_eSPI tft = TFT_eSPI();

struct ConfigData {
  char ssid[32];
  char password[32];
  char apiKey[64];
  char apiUrl[128];
  char walletId[32];
};

ConfigData config;
HTTPClient http;

// ============= FONCTION AFFICHAGE =============
void displayInit() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
}

void displayMessage(const char* msg) {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 10);
  tft.println(msg);
}

void displayWalletInfo(float balance, int transactions) {
  tft.fillScreen(TFT_BLACK);
  
  // Titre
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("WALLET LNBITS");
  
  // Solde
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(10, 50);
  tft.print(balance);
  tft.println(" sat");
  
  // Nombre transactions
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 100);
  tft.print("Transactions: ");
  tft.println(transactions);
}

void displayConnectionStatus(bool connected) {
  tft.setTextColor(connected ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 220);
  tft.println(connected ? "WiFi: Connected" : "WiFi: Disconnected");
}

// ============= FONCTION EEPROM =============
void loadConfig() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.readString(SSID_ADDR, config.ssid, 32);
  EEPROM.readString(PASS_ADDR, config.password, 32);
  EEPROM.readString(API_KEY_ADDR, config.apiKey, 64);
  EEPROM.readString(API_URL_ADDR, config.apiUrl, 128);
  EEPROM.readString(WALLET_ID_ADDR, config.walletId, 32);
}

void saveConfig() {
  EEPROM.writeString(SSID_ADDR, config.ssid);
  EEPROM.writeString(PASS_ADDR, config.password);
  EEPROM.writeString(API_KEY_ADDR, config.apiKey);
  EEPROM.writeString(API_URL_ADDR, config.apiUrl);
  EEPROM.writeString(WALLET_ID_ADDR, config.walletId);
  EEPROM.commit();
}

// ============= SERVEUR WEB DASHBOARD =============
String getHtmlPage() {
  return R"(
<!DOCTYPE html>
<html>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>LNbits Wallet Config</title>
  <style>
    body { font-family: Arial; background: #1a1a1a; color: #fff; margin: 0; padding: 20px; }
    .container { max-width: 500px; margin: 0 auto; background: #2d2d2d; padding: 20px; border-radius: 10px; }
    h1 { color: #00d4ff; text-align: center; }
    .section { margin: 20px 0; padding: 15px; background: #3a3a3a; border-radius: 5px; border-left: 4px solid #00d4ff; }
    label { display: block; margin: 10px 0 5px; color: #00d4ff; font-weight: bold; }
    input { width: 100%; padding: 8px; margin-bottom: 10px; box-sizing: border-box; background: #1a1a1a; color: #fff; border: 1px solid #00d4ff; border-radius: 3px; }
    button { width: 100%; padding: 12px; background: #00d4ff; color: #1a1a1a; border: none; border-radius: 5px; cursor: pointer; font-weight: bold; font-size: 16px; }
    .status { padding: 10px; border-radius: 5px; margin-top: 15px; text-align: center; }
    .success { background: #28a745; color: white; }
    .error { background: #dc3545; color: white; }
  </style>
</head>
<body>
  <div class='container'>
    <h1>⚡ LNbits Wallet Display</h1>
    
    <form id='config-form'>
      <div class='section'>
        <h2 style='margin-top: 0; color: #00d4ff;'>📡 WiFi Configuration</h2>
        <label for='ssid'>SSID:</label>
        <input type='text' id='ssid' name='ssid' placeholder='Votre réseau WiFi' required>
        
        <label for='password'>Mot de passe:</label>
        <input type='password' id='password' name='password' placeholder='Mot de passe WiFi' required>
      </div>

      <div class='section'>
        <h2 style='margin-top: 0; color: #00d4ff;'>💰 LNbits Configuration</h2>
        <label for='apiUrl'>URL LNbits:</label>
        <input type='url' id='apiUrl' name='apiUrl' placeholder='https://your.lnbits.com' required>
        
        <label for='walletId'>Wallet ID:</label>
        <input type='text' id='walletId' name='walletId' placeholder='ID de votre wallet' required>
        
        <label for='apiKey'>Admin Key:</label>
        <input type='text' id='apiKey' name='apiKey' placeholder='Votre clé API admin' required>
      </div>

      <button type='submit'>💾 Enregistrer la Configuration</button>
    </form>

    <div id='status'></div>
  </div>

  <script>
    document.getElementById('config-form').addEventListener('submit', async (e) => {
      e.preventDefault();
      
      const formData = new FormData(document.getElementById('config-form'));
      const data = Object.fromEntries(formData);
      
      try {
        const response = await fetch('/api/config', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(data)
        });
        
        const statusDiv = document.getElementById('status');
        if (response.ok) {
          statusDiv.className = 'status success';
          statusDiv.textContent = '✓ Configuration enregistrée avec succès!';
        } else {
          statusDiv.className = 'status error';
          statusDiv.textContent = '✗ Erreur lors de l\'enregistrement';
        }
      } catch (error) {
        document.getElementById('status').className = 'status error';
        document.getElementById('status').textContent = '✗ Erreur de connexion';
      }
    });

    fetch('/api/config').then(r => r.json()).then(data => {
      document.getElementById('ssid').value = data.ssid || '';
      document.getElementById('password').value = data.password || '';
      document.getElementById('apiUrl').value = data.apiUrl || '';
      document.getElementById('walletId').value = data.walletId || '';
      document.getElementById('apiKey').value = data.apiKey || '';
    });
  </script>
</body>
</html>
  )";
}

void handleRoot() {
  server.send(200, "text/html", getHtmlPage());
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

// ============= LNBITS API =============
bool getLNbitsWalletData(float &balance, int &txCount) {
  if (strlen(config.apiUrl) == 0 || strlen(config.apiKey) == 0) {
    return false;
  }

  String url = String(config.apiUrl) + "/api/v1/wallet";
  
  http.begin(url);
  http.addHeader("X-API-Key", config.apiKey);

  int httpCode = http.GET();
  
  if (httpCode == 200) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, http.getString());
    
    balance = doc["balance"] | 0;
    balance = balance / 1000.0; // Conversion msat à sat
    
    String txUrl = String(config.apiUrl) + "/api/v1/payments?limit=10";
    http.begin(txUrl);
    http.addHeader("X-API-Key", config.apiKey);
    
    int txCode = http.GET();
    if (txCode == 200) {
      DynamicJsonDocument txDoc(2048);
      deserializeJson(txDoc, http.getString());
      txCount = txDoc["payments"].size();
    }
    
    http.end();
    return true;
  }
  
  http.end();
  return false;
}

void displayTransactions() {
  String url = String(config.apiUrl) + "/api/v1/payments?limit=5";
  
  http.begin(url);
  http.addHeader("X-API-Key", config.apiKey);
  
  int httpCode = http.GET();
  if (httpCode == 200) {
    DynamicJsonDocument doc(4096);
    deserializeJson(doc, http.getString());
    
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("DERNIÈRES TRANSACTIONS");
    
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    
    int y = 40;
    for (size_t i = 0; i < doc["payments"].size() && i < 5; i++) {
      JsonObject payment = doc["payments"][i];
      
      float amount = payment["amount"] | 0;
      amount = amount / 1000.0;
      
      tft.setCursor(10, y);
      tft.print(amount);
      tft.print(" sat");
      
      const char* memo = payment["memo"] | "";
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.setCursor(10, y + 15);
      if (strlen(memo) > 20) {
        char shortMemo[21];
        strncpy(shortMemo, memo, 20);
        shortMemo[20] = '\0';
        tft.println(shortMemo);
      } else {
        tft.println(memo);
      }
      
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      y += 35;
    }
  }
  http.end();
}

// ============= SETUP WIFI =============
void setupWiFi() {
  if (strlen(config.ssid) > 0) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.ssid, config.password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      displayMessage("WiFi Connected!");
      delay(2000);
      return;
    }
  }
  
  displayMessage("Configuration Mode");
  WiFi.softAP("LNbits-Config", "12345678");
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
  displayMessage("Initializing...");
  
  setupWiFi();
  setupWebServer();
  
  displayMessage("Ready!");
}

// ============= LOOP PRINCIPAL =============
unsigned long lastUpdate = 0;

void loop() {
  server.handleClient();
  
  if (millis() - lastUpdate > 30000) {
    lastUpdate = millis();
    
    if (WiFi.status() == WL_CONNECTED) {
      float balance = 0;
      int txCount = 0;
      
      if (getLNbitsWalletData(balance, txCount)) {
        displayWalletInfo(balance, txCount);
      } else {
        displayMessage("API Error");
      }
    } else {
      displayMessage("No WiFi");
    }
  }
  
  displayConnectionStatus(WiFi.status() == WL_CONNECTED);
  delay(100);
}
