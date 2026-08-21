#ifndef TITAN_MICRO_BUS_ARDUINO_H
#define TITAN_MICRO_BUS_ARDUINO_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TITAN_SIG_HI 0x54
#define TITAN_SIG_LO 0x42
#define TITAN_VER    0x02
#define TITAN_HDR_SZ 24
#define TITAN_RING_SZ 256

typedef void (*TitanCmdCallback)(uint8_t cmd, uint32_t senderId, const uint8_t* payload, uint16_t len);

typedef struct {
    uint32_t myNodeId;
    TitanCmdCallback callbacks[32]; // Fast array jump table
    uint8_t rxBuffer[TITAN_RING_SZ];
    uint16_t rxHead;
    uint32_t seqNo;
} TitanArduinoBus;

static inline void TitanArduino_Init(TitanArduinoBus* bus, uint32_t nodeId) {
    bus->myNodeId = nodeId;
    bus->rxHead = 0;
    bus->seqNo = 1;
    memset(bus->callbacks, 0, sizeof(bus->callbacks));
}

static inline void TitanArduino_On(TitanArduinoBus* bus, uint8_t cmd, TitanCmdCallback cb) {
    if (cmd < 32) {
        bus->callbacks[cmd] = cb;
    }
}

static inline uint16_t TitanArduino_Pack(TitanArduinoBus* bus, uint32_t targetId, uint8_t cmd, const uint8_t* payload, uint16_t len, uint8_t* outBuf) {
    outBuf[0] = TITAN_SIG_HI;
    outBuf[1] = TITAN_SIG_LO;
    outBuf[2] = TITAN_VER;
    outBuf[3] = cmd;

    // Sender ID (Big Endian)
    outBuf[4] = (bus->myNodeId >> 24) & 0xFF;
    outBuf[5] = (bus->myNodeId >> 16) & 0xFF;
    outBuf[6] = (bus->myNodeId >> 8) & 0xFF;
    outBuf[7] = bus->myNodeId & 0xFF;

    // Target ID (Big Endian)
    outBuf[8] = (targetId >> 24) & 0xFF;
    outBuf[9] = (targetId >> 16) & 0xFF;
    outBuf[10] = (targetId >> 8) & 0xFF;
    outBuf[11] = targetId & 0xFF;

    // Payload Len
    outBuf[12] = (len >> 24) & 0xFF;
    outBuf[13] = (len >> 16) & 0xFF;
    outBuf[14] = (len >> 8) & 0xFF;
    outBuf[15] = len & 0xFF;

    // Seq No
    outBuf[16] = (bus->seqNo >> 24) & 0xFF;
    outBuf[17] = (bus->seqNo >> 16) & 0xFF;
    outBuf[18] = (bus->seqNo >> 8) & 0xFF;
    outBuf[19] = bus->seqNo++ & 0xFF;

    outBuf[20] = 0; // Session ID Hi
    outBuf[21] = 0; // Session ID Lo
    outBuf[22] = 0; // Flags
    outBuf[23] = 0xAA; // Checksum

    if (payload != NULL && len > 0) {
        memcpy(&outBuf[TITAN_HDR_SZ], payload, len);
    }
    return TITAN_HDR_SZ + len;
}

static inline void TitanArduino_Feed(TitanArduinoBus* bus, const uint8_t* bytes, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        if (bus->rxHead >= TITAN_RING_SZ) bus->rxHead = 0;
        bus->rxBuffer[bus->rxHead++] = bytes[i];

        if (bus->rxHead >= TITAN_HDR_SZ) {
            if (bus->rxBuffer[0] == TITAN_SIG_HI && bus->rxBuffer[1] == TITAN_SIG_LO) {
                uint8_t cmd = bus->rxBuffer[3];
                uint16_t pLen = (bus->rxBuffer[14] << 8) | bus->rxBuffer[15];
                uint16_t total = TITAN_HDR_SZ + pLen;

                if (bus->rxHead >= total) {
                    uint32_t sender = ((uint32_t)bus->rxBuffer[4] << 24) |
                                      ((uint32_t)bus->rxBuffer[5] << 16) |
                                      ((uint32_t)bus->rxBuffer[6] << 8) |
                                      (uint32_t)bus->rxBuffer[7];

                    if (cmd < 32 && bus->callbacks[cmd] != NULL) {
                        bus->callbacks[cmd](cmd, sender, &bus->rxBuffer[TITAN_HDR_SZ], pLen);
                    }
                    bus->rxHead = 0; // Reset after packet
                }
            } else {
                bus->rxHead = 0; // Resync
            }
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif // TITAN_MICRO_BUS_ARDUINO_H
