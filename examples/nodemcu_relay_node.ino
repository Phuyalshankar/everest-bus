/**
 * ⚡ TITAN UNIFIED BUS — NodeMCU (ESP8266 / ESP32) Relay & Sensor Node
 * ═════════════════════════════════════════════════════════════════════
 * Connects directly to Titan TCP Relay on Port 9092.
 * Listens for 24-byte Titan Binary Frames:
 * - CMD 0x23 (TITAN_CMD_RELAY_SET): Toggles Relay Pin (GPIO D1 / D2)
 * - CMD 0x21 (TITAN_CMD_KEYPAD_EVENT): Physical PIN Code Access Control
 */

#include <ESP8266WiFi.h> // Or <WiFi.h> on ESP32
#include "../cpp/TitanMicroBus_Arduino.h"

const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* host     = "192.168.1.3"; // PBX / Server LAN IP
const uint16_t port  = 9092;          // Titan TCP Binary Port

#define RELAY_PIN_1 D1 // GPIO5
#define RELAY_PIN_2 D2 // GPIO4

WiFiClient client;
TitanArduinoBus titanBus;

void onRelayCommand(uint8_t cmd, uint32_t senderId, const uint8_t* payload, uint16_t len) {
    if (len >= 3) {
        uint16_t relayId = (payload[0] << 8) | payload[1];
        uint8_t state = payload[2];

        if (relayId == 1) {
            digitalWrite(RELAY_PIN_1, state ? LOW : HIGH); // Active LOW relay
            Serial.printf("⚡ [TitanBus] Relay 1 set to: %s\n", state ? "ON" : "OFF");
        } else if (relayId == 2) {
            digitalWrite(RELAY_PIN_2, state ? LOW : HIGH);
            Serial.printf("⚡ [TitanBus] Relay 2 set to: %s\n", state ? "ON" : "OFF");
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(RELAY_PIN_1, OUTPUT);
    pinMode(RELAY_PIN_2, OUTPUT);
    digitalWrite(RELAY_PIN_1, HIGH); // Default OFF
    digitalWrite(RELAY_PIN_2, HIGH);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n✅ WiFi Connected!");

    // Initialize Titan Bus with NodeMCU ID: 300
    TitanArduino_Init(&titanBus, 300);
    TitanArduino_On(&titanBus, TITAN_CMD_RELAY_SET, onRelayCommand);
}

void loop() {
    if (!client.connected()) {
        Serial.println("🔄 Connecting to Titan TCP Bus (Port 9092)...");
        if (client.connect(host, port)) {
            Serial.println("🚀 Connected to Titan TCP Bus!");
            // Send Register Announce packet
            uint8_t regFrame[32];
            uint16_t sz = TitanArduino_Pack(&titanBus, 0, TITAN_CMD_REGISTER, NULL, 0, regFrame);
            client.write(regFrame, sz);
        } else {
            delay(2000);
            return;
        }
    }

    // Read incoming bytes from TCP Bus into Titan Ring Buffer
    while (client.available()) {
        uint8_t b = client.read();
        TitanArduino_Feed(&titanBus, &b, 1);
    }
}
