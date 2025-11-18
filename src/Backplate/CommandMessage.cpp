#include "CommandMessage.hpp"

CRC_CITT CommandMessage::CrcCalculator;

const std::vector<uint8_t>& CommandMessage::GetRawMessage() 
{
    if (buffer.empty())
    {
        BuildMessage();
    }
    return buffer;
}

void CommandMessage::BuildMessage()
{
    for (auto &b : Preamble)
    {
        buffer.push_back(b);
    }

    buffer.push_back(static_cast<uint8_t>(static_cast<uint16_t>(commandId) & 0x00FF));
    buffer.push_back(static_cast<uint8_t>((static_cast<uint16_t>(commandId) >> 8) & 0x00FF));

    buffer.push_back(static_cast<uint8_t>(payloadLength & 0x00FF));
    buffer.push_back(static_cast<uint8_t>((payloadLength >> 8) & 0x00FF));

    // Add payload if any

    // Calculate CRC
    uint16_t crc = CrcCalculator.Calculate(
        buffer.data() + PreambleSize, 
        buffer.size() - PreambleSize
    );
    buffer.push_back(static_cast<uint8_t>(crc & 0x00FF));
    buffer.push_back(static_cast<uint8_t>((crc >> 8) & 0x00FF));
}

