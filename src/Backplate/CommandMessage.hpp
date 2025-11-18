#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include "CRC-CITT.hpp"
#include "MessageType.hxx"


class CommandMessage {
public:
    CommandMessage(MessageType cmd)
        : commandId(cmd), payloadLength(0)
    {
    }

    CommandMessage()
        : commandId(MessageType::Null), payloadLength(0)
    {
    }

    virtual ~CommandMessage()
    {
    }

    const uint8_t Preamble[3] = {0xd5, 0x5d, 0xc3};
    const int PreambleSize = 3;

    const std::vector<uint8_t>& GetRawMessage();
    const MessageType GetMessageCommand() const { return commandId; }

private:
    void BuildMessage();

private:
    std::vector<uint8_t> buffer;
    MessageType commandId;
    uint16_t payloadLength;

    static CRC_CITT CrcCalculator;
    
};