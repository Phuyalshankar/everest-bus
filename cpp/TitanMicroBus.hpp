#ifndef TITAN_MICRO_BUS_HPP
#define TITAN_MICRO_BUS_HPP

#include "../include/titan_protocol.h"
#include "../include/titan_registers.h"
#include <string>
#include <functional>
#include <map>
#include <vector>

namespace Titan {

typedef std::function<void(uint8_t cmd, uint32_t senderId, const uint8_t* payload, uint32_t len)> NodeHandler;
typedef std::function<void(uint16_t reg, const std::string& value)> RegisterListener;

class TitanMicroBus {
public:
    static void init();

    // 1. Core Packet Transmission
    static bool emit(uint32_t targetId, uint8_t cmd, const uint8_t* payload, uint32_t len, uint8_t flags = 0);
    static void subscribe(uint8_t cmd, NodeHandler handler);

    // 2. 2-Byte (16-bit: 0 - 65535) Memory Register Read/Write
    static void write(uint16_t reg, const std::string& value);
    static void writeInt(uint16_t reg, int32_t value);
    static std::string read(uint16_t reg);
    static int32_t readInt(uint16_t reg, int32_t defaultValue = 0);

    // 3. Register Change Listeners
    static void onRegisterChange(uint16_t reg, RegisterListener listener);

    // 4. Ingest incoming raw TCP stream buffer
    static void processIncomingBytes(const uint8_t* data, size_t len);

private:
    static std::map<uint16_t, std::string> s_registers;
    static std::map<uint8_t, std::vector<NodeHandler>> s_commandHandlers;
    static std::map<uint16_t, std::vector<RegisterListener>> s_registerListeners;
    static uint8_t s_rxBuffer[512];
    static size_t s_rxHead;
    static uint32_t s_seqNo;
};

} // namespace Titan

#endif // TITAN_MICRO_BUS_HPP
