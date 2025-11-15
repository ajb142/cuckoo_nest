#include "Backplate/Message.hpp"
#include <gtest/gtest.h>

class TestBackplateComms : public ::testing::Test {
protected:
    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
}; 

TEST_F(TestBackplateComms, BasicMessageCreation) 
{
    Message msg(MessageCommand::Reset);
    int rawLength = 0;
    const uint8_t* rawMessage = msg.GetRawMessage(rawLength);

    ASSERT_EQ(9, rawLength); // 3 bytes preamble + 2 bytes command + 2 bytes length + 2 bytes CRC
    uint8_t expectedMessage[] = {0xd5, 0x5d, 0xc3, 0xff, 0x00, 0x00, 0x00}; // dont check CRC here
    for (int i = 0; i < 7; ++i)
        EXPECT_EQ(expectedMessage[i], rawMessage[i]);
}



// Handle data length over the max size of uint16_t
// crc calculation is done correctly