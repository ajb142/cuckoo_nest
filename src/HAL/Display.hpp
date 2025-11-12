#pragma once
#include <string>
#include <stdint.h>
#include <linux/fb.h>

#include "lvgl/lvgl.h"

enum class Font {
    FONT_H1,
    FONT_H2,
    FONT_DEFAULT
};

class Display {
public:
    Display(std::string device_path);
    ~Display();
    bool Initialize();
    void SetBackgroundColor(uint32_t color);
    void DrawText(int x, int y, const std::string &text, uint32_t color = 0xFFFFFF, Font font = Font::FONT_DEFAULT);
    void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);
    void DrawPixel(int x, int y, uint32_t color);
    void Flush();
    void TimerHandler();

private:
    std::string device_path_;
    int screen_buffer;
    char *fbp;
    char *working_buffer;
    long screensize;
    
    // Screen info
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    // lvgl display members
    lv_display_t *disp;
    lv_style_t *fontH2;
    lv_style_t *fontH1;
};