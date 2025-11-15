#pragma once

class ISerialPort {
public:
    ISerialPort() = default;
    virtual ~ISerialPort() = default;
    
    virtual bool Open(const char* portName, int baudRate) = 0;
    virtual void Close() = 0;
    virtual int Read(char* buffer, int bufferSize) = 0;
    virtual int Write(const char* data, int dataSize) = 0;
};

class BackplateComms {
public:
    BackplateComms() = default;
    virtual ~BackplateComms() = default;
    
    bool Initialize();
};
