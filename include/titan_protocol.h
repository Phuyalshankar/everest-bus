#ifndef TITAN_PROTOCOL_H
#define TITAN_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TITAN_SIGNATURE    0x5442  // 'TB' (Titan Binary)
#define TITAN_VERSION      0x02    // Titan Protocol v2
#define TITAN_HEADER_SIZE  24      // Fixed 24-Byte Big-Endian Header
#define TITAN_DEFAULT_PORT 9092    // Core Binary TCP Relay Port

// 📋 Standardized Command Opcodes (CMD)
typedef enum {
    TITAN_CMD_REGISTER       = 0x08,
    TITAN_CMD_REGISTER_ACK   = 0x09,
    TITAN_CMD_INVITE         = 0x10,  // VoIP Call / SIP
    TITAN_CMD_ACCEPT         = 0x11,
    TITAN_CMD_REJECT         = 0x12,
    TITAN_CMD_HANGUP         = 0x13,
    TITAN_CMD_AUDIO_FRAME    = 0x14,  // Zero-Copy Realtime Audio
    TITAN_CMD_VIDEO_FRAME    = 0x15,
    TITAN_CMD_CHAT_MESSAGE   = 0x20,
    TITAN_CMD_KEYPAD_EVENT   = 0x21,  // Key press/release event
    TITAN_CMD_DISPLAY_WRITE  = 0x22,  // 16x2 LCD / Screen text write
    TITAN_CMD_RELAY_SET      = 0x23,  // Relay GPIO toggle
    TITAN_CMD_SENSOR_STREAM  = 0x24,  // Realtime Telemetry ADC
    TITAN_CMD_HEARTBEAT      = 0x30,
    TITAN_CMD_HEARTBEAT_ACK  = 0x31,
    TITAN_CMD_CUSTOM_ACTION  = 0x40,
    TITAN_CMD_ERROR          = 0xFF
} TitanCmdType;

// 🧱 Packed 24-Byte Titan Binary Header
#pragma pack(push, 1)
typedef struct {
    uint16_t signature;   // 0x5442 ('TB')
    uint8_t  version;     // 0x02
    uint8_t  cmdType;     // TitanCmdType
    int32_t  senderExt;   // Sender Extension / Node ID (Big-Endian)
    int32_t  targetExt;   // Target Extension / Node ID (Big-Endian)
    int32_t  payloadLen;  // Length of following payload in bytes
    int32_t  seqNo;       // Sequence Number
    uint16_t sessionId;   // Stream / Session ID
    uint8_t  flags;       // 0x00 = Pure Binary, 0x01 = JSON Sub-code
    uint8_t  checksum;    // CRC8 / Sum
} TitanHeader;
#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif // TITAN_PROTOCOL_H
