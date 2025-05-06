#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

//Network credentials
const char* ssid = "WiFi Name";
const char* username = "WiFi Username";
const char* password = "Password";

//Server URL
const String serverName = "http://RPi-ip-address:5000/indices";

//Create an LCD object. Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

// RGB LED Pins
const int redPin = 5;
const int greenPin = 18;

void setup() {
  //Start serial to see output of program **make sure to be on the good baud**
  Serial.begin(115200);

  // Initialize LCD
  lcd.begin(20, 4);
  lcd.init();
  lcd.backlight();

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  esp_eap_client_config_t config = {
    .username = (uint8_t *)username,
    .password = (uint8_t *)password,
    .identity = (uint8_t *)username,

    .client_cert = NULL,
    .client_key = NULL,
    .client_key_pass = NULL
    .ca_cert = NULL,
  };

  esp_wifi_sta_eap_set_config(&config);

  WiFi.begin(ssid);

  //Writes connecting to WiFi to Serial + LCD as long as it's not connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting to WiFi...");
  }

  //WiFi connection OK
  Serial.println("Connected to WiFi");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
      
      //Connect to server
      HTTPClient http;
      String serverPath = serverName;
      http.begin(serverPath.c_str());
      int httpResponseCode = http.GET();

      //Connection is made
      if (httpResponseCode > 0) {
        String payload = http.getString();
        Serial.println(payload);
        
        // Allocate a JSON document
        DynamicJsonDocument doc(8192);

        // Parse JSON
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.f_str());
          return;
        }

        // Loop through each stock item
        for (JsonPair kv : doc.as<JsonObject>()) {
          const char* stockSymbol = kv.key().c_str();
          JsonArray stockInfo = kv.value().as<JsonArray>();

          const char* name = stockInfo[0];
          const char* industry = stockInfo[1];
          float price = stockInfo[2];
          float dollar_change = stockInfo[3];
          float percent_change = stockInfo[4];

          // Debugging: Print each stock's details
          Serial.print("Name: ");
          Serial.println(name);
          Serial.print("EXCH: ");
          Serial.println(exchange);
          Serial.print("CURR: ");
          Serial.println(currency);
          Serial.print("Price: ");
          Serial.println(price);
          Serial.print("Percent change: ");
          Serial.println(percent_change);

        // Prepare the lines to fit within 20 characters
          String line1 = String(name).substring(0, 20);
          String line2 = "EX: " + String(exchange).substring(0, 4) + " CUR: " + String(currency).substring(0, 3);
          String line3 = "Price: $" + String(price, 2).substring(0, 9); // Adjust price length if needed
          String line4 = "Chg: " + String(dollar_change, 2).substring(0, 7) + "$ " + String(percent_change, 2) + "%";

          // Display on LCD
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(line1);
          lcd.setCursor(0, 1);
          lcd.print(line2);
          lcd.setCursor(0, 2);
          lcd.print(line3);
          lcd.setCursor(0, 3);
          lcd.print(line4);

          // Control RGB LED
          if (percent_change > 0) {
            setLED(0, 100, 0); // Green
          } else if (percent_change < 0) {
            setLED(100, 0, 0); // Red
          } else {
            setLED(0, 0, 0); // Off
          }

          delay(5000);
      }

      //No connection to server made
      } else {
        Serial.println("Error on HTTP request");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("HTTP error");
      }
      http.end();

  //No WiFi connection
  } else {
    Serial.println("WiFi Disconnected");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Disconnected");
  } 
}

// Function to set RGB LED color
void setLED(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
}