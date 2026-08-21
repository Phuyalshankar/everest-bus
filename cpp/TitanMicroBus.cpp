#include "TitanMicroBus.hpp"
#include <cstring>
#include <cstdlib>

namespace Titan {

std::map<uint16_t, std::string> TitanMicroBus::s_registers;
std::map<uint8_t, std::vector<NodeHandler>> TitanMicroBus::s_commandHandlers;
std::map<uint16_t, std::vector<RegisterListener>> TitanMicroBus::s_registerListeners;
uint8_t TitanMicroBus::s_rxBuffer[512];
size_t TitanMicroBus::s_rxHead = 0;
uint32_t TitanMicroBus::s_seqNo = 1;

void TitanMicroBus::init() {
    s_registers.clear();
    s_commandHandlers.clear();
    s_registerListeners.clear();
    s_rxHead = 0;
    s_seqNo = 1;

    // Set default core registers
    writeInt(REG_BUS_STATUS, 1); // Online
    writeInt(REG_ACTIVE_SCREEN, 10); // Home Screen
    write(REG_KEYPAD_DIAL_BUFFER, "");
}

bool TitanMicroBus::emit(uint32_t targetId, uint8_t cmd, const uint8_t* payload, uint32_t len, uint8_t flags) {
    TitanHeader hdr;
    hdr.signature = 0x5442;
    hdr.version = 0x02;
    hdr.cmdType = cmd;
    hdr.senderExt = 101; // Default Local Node ID
    hdr.targetExt = targetId;
    hdr.payloadLen = len;
    hdr.seqNo = s_seqNo++;
    hdr.sessionId = 0;
    hdr.flags = flags;
    hdr.checksum = 0xAA;

    // Internal loopback dispatch
    auto it = s_commandHandlers.find(cmd);
    if (it != s_commandHandlers.end()) {
        for (auto& handler : it->second) {
            handler(cmd, hdr.senderExt, payload, len);
        }
    }
    return true;
}

void TitanMicroBus::subscribe(uint8_t cmd, NodeHandler handler) {
    s_commandHandlers[cmd].push_back(handler);
}

void TitanMicroBus::write(uint16_t reg, const std::string& value) {
    s_registers[reg] = value;
    auto it = s_registerListeners.find(reg);
    if (it != s_registerListeners.end()) {
        for (auto& listener : it->second) {
            listener(reg, value);
        }
    }
}

void TitanMicroBus::writeInt(uint16_t reg, int32_t value) {
    write(reg, std::to_string(value));
}

std::string TitanMicroBus::read(uint16_t reg) {
    auto it = s_registers.find(reg);
    return (it != s_registers.end()) ? it->second : "";
}

int32_t TitanMicroBus::readInt(uint16_t reg, int32_t defaultValue) {
    auto it = s_registers.find(reg);
    if (it == s_registers.end() || it->second.empty()) return defaultValue;
    return std::atoi(it->second.c_str());
}

void TitanMicroBus::onRegisterChange(uint16_t reg, RegisterListener listener) {
    s_registerListeners[reg].push_back(listener);
}

void TitanMicroBus::processIncomingBytes(const uint8_t* data, size_t len) {
    if (data == nullptr || len == 0) return;
    if (s_rxHead + len > 512) s_rxHead = 0; // Prevent overflow

    std::memcpy(&s_rxBuffer[s_rxHead], data, len);
    s_rxHead += len;

    while (s_rxHead >= TITAN_HEADER_SIZE) {
        // Check magic bytes 'TB' (0x54, 0x42)
        if (s_rxBuffer[0] != 0x54 || s_rxBuffer[1] != 0x42) {
            // Shift 1 byte and realign
            std::memmove(&s_rxBuffer[0], &s_rxBuffer[1], s_rxHead - 1);
            s_rxHead--;
            continue;
        }

        uint8_t cmd = s_rxBuffer[3];
        int32_t payloadLen = (s_rxBuffer[12] << 24) | (s_rxBuffer[13] << 16) | (s_rxBuffer[14] << 8) | s_rxBuffer[15];

        if (payloadLen < 0 || payloadLen > 4096) {
            s_rxHead = 0; // Malformed packet recovery
            break;
        }

        size_t totalPacketSize = TITAN_HEADER_SIZE + payloadLen;
        if (s_rxHead < totalPacketSize) {
            break; // Awaiting remaining payload bytes
        }

        const uint8_t* payload = &s_rxBuffer[TITAN_HEADER_SIZE];
        int32_t sender = (s_rxBuffer[4] << 24) | (s_rxBuffer[5] << 16) | (s_rxBuffer[6] << 8) | s_rxBuffer[7];

        // Dispatch command
        auto it = s_commandHandlers.find(cmd);
        if (it != s_commandHandlers.end()) {
            for (auto& handler : it->second) {
                handler(cmd, sender, payload, payloadLen);
            }
        }

        // Consume frame
        size_t remaining = s_rxHead - totalPacketSize;
        if (remaining > 0) {
            std::memmove(&s_rxBuffer[0], &s_rxBuffer[totalPacketSize], remaining);
        }
        s_rxHead = remaining;
    }
}

} // namespace Titan
