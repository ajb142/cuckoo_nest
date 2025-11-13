#pragma once
#include <string>
#include <stdint.h>

enum class Font {
    FONT_H1,
    FONT_H2,
    FONT_DEFAULT
};

class IDisplay {
public:
    virtual ~IDisplay() = default;
    virtual bool Initialize() = 0;
    virtual void SetBackgroundColor(uint32_t color) = 0;
    virtual void DrawText(int x, int y, const std::string &text, uint32_t color = 0xFFFFFF, Font font = Font::FONT_DEFAULT) = 0;
    virtual void DrawLine(int x0, int y0, int x1, int y1, uint32_t color) = 0;
    virtual void DrawPixel(int x, int y, uint32_t color) = 0;
    virtual void Flush() = 0;
    virtual void TimerHandler() = 0;
};
