#ifndef TITAN_REGISTERS_H
#define TITAN_REGISTERS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ═════════════════════════════════════════════════════════════════════
// 🗺️ TITAN 2-BYTE (16-BIT: 0 - 65,535) MEMORY REGISTER MAP
// ═════════════════════════════════════════════════════════════════════

// 1. Core & Page Navigation Registers (0 - 999)
#define REG_BUS_STATUS           1
#define REG_BUS_HEARTBEAT        2
#define REG_ACTIVE_SCREEN        10   // 10 = Active Screen ID
#define REG_DRAWER_STATE         11   // 0 = Closed, 1 = Open
#define REG_THEME_MODE           12   // 0 = Light, 1 = Dark, 2 = OLED

// 2. UI & Visual Components (1,000 - 9,999)
#define REG_KEYPAD_DIAL_BUFFER   1000 // Current dial string / buffer
#define REG_KEYPAD_LAST_KEY      1001 // Last key pressed ('0'..'9', '*', '#')
#define REG_KEYPAD_DTMF_TONE     1002 // Currently playing DTMF tone
#define REG_LCD_LINE_1           1010 // 16x2 Character LCD Top Line (16 chars)
#define REG_LCD_LINE_2           1011 // 16x2 Character LCD Bottom Line (16 chars)
#define REG_SPEEDOMETER_VALUE    1020 // 0 - 240 Speedometer gauge
#define REG_VU_METER_LEFT        1021 // Audio VU Left (0 - 100)
#define REG_VU_METER_RIGHT       1022 // Audio VU Right (0 - 100)

// 3. Database & Storage Space (10,000 - 19,999)
#define REG_DB_CONTACT_BASE      10000 // Contact IDs: 10000 + Contact ID
#define REG_DB_CALLLOG_BASE      15000 // Call Log IDs: 15000 + Log ID
#define REG_DB_PROFILE_USER      19000 // Current User Profile Memory

// 4. Hardware Relays & GPIO Coils (20,000 - 29,999)
#define REG_RELAY_BASE           20000 // Relay IDs: 20000 + Relay Index (1..256)
#define REG_RELAY_1              20001
#define REG_RELAY_2              20002
#define REG_SOLENOID_LOCK        20010 // Solenoid pulse trigger
#define REG_GPIO_OUTPUT_MASK     20100 // 32-bit digital output bitmask

// 5. Sensors & Telemetry (30,000 - 39,999)
#define REG_SENSOR_CPU_LOAD      30001 // 0 - 100%
#define REG_SENSOR_BATTERY_ADC   30002 // Millivolts / Percentage
#define REG_SENSOR_TEMPERATURE   30003 // Degrees Celsius * 10
#define REG_SENSOR_WIFI_RSSI     30004 // Signal strength (dBm)

// 6. User Custom & Dynamic Extension Space (40,000 - 65,535)
#define REG_CUSTOM_BASE          40000

#ifdef __cplusplus
}
#endif

#endif // TITAN_REGISTERS_H
