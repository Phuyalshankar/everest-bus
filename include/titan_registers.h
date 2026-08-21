#ifndef TITAN_REGISTERS_H
#define TITAN_REGISTERS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ═════════════════════════════════════════════════════════════════════
// 🌐 EVEREST BUS — GLOBAL 16-BIT (0x0000 - 0xFFFF) REGISTER STANDARD
// ═════════════════════════════════════════════════════════════════════
// Domain can be extracted in 1 CPU cycle: domain = (reg >> 12)

// ---------------------------------------------------------------------
// 1. HARDWARE & IoT ENGINE (0x0000 - 0x0FFF | 0 - 4095)
// ---------------------------------------------------------------------
#define REG_HW_RELAY_BASE        0x0001 // Relay 1..256 (0x0001..0x0100)
#define REG_HW_RELAY_1           0x0001
#define REG_HW_RELAY_2           0x0002
#define REG_HW_SOLENOID_LOCK     0x0010 // Solenoid pulse trigger
#define REG_HW_MOTOR_PWM         0x0020 // Motor Speed (0 - 255)
#define REG_HW_SENSOR_TEMP       0x0030 // Temperature ADC
#define REG_HW_SENSOR_BATTERY    0x0031 // Battery Millivolts
#define REG_HW_SENSOR_LIGHT      0x0032 // Ambient Light LDR
#define REG_HW_GPIO_OUT_MASK     0x0080 // 32-bit Digital Output Bitmask

// ---------------------------------------------------------------------
// 2. UI & VECTOR GRAPHICS ENGINE (0x1000 - 0x1FFF | 4096 - 8191)
// ---------------------------------------------------------------------
#define REG_UI_ACTIVE_SCREEN     0x1000 // Active Screen ID / Route
#define REG_UI_DRAWER_STATE      0x1001 // 0 = Closed, 1 = Open
#define REG_UI_THEME_MODE        0x1002 // 0 = Light, 1 = Dark, 2 = OLED
#define REG_UI_DIAL_BUFFER       0x1010 // Keypad Dial String ("102")
#define REG_UI_LAST_KEY_DOWN     0x1011 // Last key pressed ('5')
#define REG_UI_LCD_LINE_1        0x1020 // 16x2 Character LCD Line 1
#define REG_UI_LCD_LINE_2        0x1021 // 16x2 Character LCD Line 2
#define REG_UI_GAUGE_VALUE       0x1030 // Speedometer / Gauge (0 - 100)
#define REG_UI_VU_METER          0x1031 // Audio Peak VU meter

// ---------------------------------------------------------------------
// 3. DATABASE & STORAGE ENGINE (0x2000 - 0x2FFF | 8192 - 12287)
// ---------------------------------------------------------------------
#define REG_DB_SEARCH_QUERY      0x2000 // Live search query string
#define REG_DB_SORT_MODE         0x2001 // Sort mode ('online_first', 'name_asc')
#define REG_DB_CONTACT_BASE      0x2100 // Contacts 1..2000 (0x2100 + ID)
#define REG_DB_CALLLOG_BASE      0x2800 // Call history records
#define REG_DB_PROFILE_USER      0x2F00 // Active logged in profile

// ---------------------------------------------------------------------
// 4. VIDEO & NVR STREAM ENGINE (0x3000 - 0x3FFF | 12288 - 16383)
// ---------------------------------------------------------------------
#define REG_VID_ACTIVE_FEED_ID   0x3000 // Active Camera Feed ID (1..300)
#define REG_VID_STREAM_FPS       0x3001 // Realtime Live Stream FPS
#define REG_VID_PTZ_COMMAND      0x3010 // PTZ Pan/Tilt/Zoom action
#define REG_VID_MOTION_ALARM     0x3020 // AI Motion Alert Flag (0/1)
#define REG_VID_RECORDING_STATUS 0x3030 // NVR Disk Recording State

// ---------------------------------------------------------------------
// 5. AUDIO & TELECOM PBX ENGINE (0x4000 - 0x4FFF | 16384 - 20479)
// ---------------------------------------------------------------------
#define REG_AUD_CALL_STATE       0x4000 // 0 = Idle, 1 = Ringing, 2 = Call
#define REG_AUD_CALL_PARTNER_EXT 0x4001 // Partner extension number
#define REG_AUD_VOLUME_OUTPUT    0x4010 // Speaker volume (0 - 100)
#define REG_AUD_MIC_GAIN         0x4011 // Microphone gain
#define REG_AUD_DTMF_PLAYING     0x4020 // Active playing DTMF tone

// ---------------------------------------------------------------------
// 6. AI / ML & EDGE INTELLIGENCE (0x5000 - 0x5FFF | 20480 - 24575)
// ---------------------------------------------------------------------
#define REG_AI_INFERENCE_MODEL   0x5000 // Active Model ID
#define REG_AI_CONFIDENCE_SCORE  0x5001 // Confidence (0 - 100%)
#define REG_AI_DETECTED_CLASS    0x5002 // Detected Object Class ID
#define REG_AI_FACE_RECOGNIZED   0x5010 // Recognized User ID from camera

// ---------------------------------------------------------------------
// 7. CUSTOM & COMMUNITY EXTENSIONS (0x6000 - 0xFFFF | 24576 - 65535)
// ---------------------------------------------------------------------
#define REG_CUSTOM_PLUGIN_BASE   0x6000

#ifdef __cplusplus
}
#endif

#endif // TITAN_REGISTERS_H

