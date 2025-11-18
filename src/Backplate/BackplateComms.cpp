#include "BackplateComms.hpp"
#include "CommandMessage.hpp"
#include "ResponseMessage.hpp"
#include <unistd.h>


bool BackplateComms::Initialize() 
{
    if (!InitializeSerial())
    {
        return false;
    }

    CommandMessage resetMsg(MessageType::Reset);
    serialPort->Write(resetMsg.GetRawMessage());

    // Wait for 5s for backplate to send a bust of data
    // there will be a bunch of data points, ending with a BRK
    uint8_t readBuffer[256];
    size_t totalBytesRead = 0;
    totalBytesRead = serialPort->Read(
        reinterpret_cast<char*>(readBuffer), 
        sizeof(readBuffer)
    );
    
    if (totalBytesRead == 0)
    {
        return false;
    }

    // Parse the response to find the BRK message
    ResponseMessage responseMsg;
    responseMsg.ParseMessage(readBuffer, totalBytesRead);

    if (responseMsg.GetMessageCommand() != MessageType::ResponseAscii)
    {
        return false;
    }

    // if (responseMsg.GetPayload() != std::vector<uint8_t>{'B', 'R', 'K'})
    // {
    //     return false;
    // }

    return true;
}

bool BackplateComms::InitializeSerial()
{
    const BaudRate baudRate = BaudRate::Baud115200;

    if (!serialPort->Open(baudRate))
    {
        return false;
    }

    serialPort->Flush();
    serialPort->SendBreak(0);
    usleep(250000);
    serialPort->Flush();

    return true;
}
