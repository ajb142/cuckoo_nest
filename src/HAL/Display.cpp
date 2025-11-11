#include "Display.hpp"
#include "BitmapFont.hpp"
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include <unistd.h>
#include <cstring>



Display::Display(std::string device_path) : device_path_(device_path)
{
}

Display::~Display()
{
    if (fbp != nullptr)
    {
        munmap(fbp, screensize);
    }
    if (screen_buffer >= 0)
    {
        close(screen_buffer);
    }
}

bool Display::Initialize()
{
    lv_init();

    //Create a display
    disp = lv_linux_fbdev_create();
    if (disp == NULL) {
        perror("lv_linux_fbdev_create failed");
        return false;
    }

    lv_linux_fbdev_set_file(disp, "/dev/fb0");
    lv_display_set_resolution(disp, 320, 320);

    // Setup main font
    mainFont = new lv_style_t;
    lv_style_init(mainFont);
    lv_style_set_text_font(mainFont, &lv_font_montserrat_28);

    // Draw loading screen
    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Loading...");
    lv_obj_add_style(label, mainFont, 0);
    lv_obj_center(label);

    return true;
}

void Display::SetBackgroundColor(uint32_t color)
{
    lv_obj_t * scr = lv_scr_act();
    lv_obj_clean(scr);
    lv_obj_set_style_bg_color(scr, lv_color_make((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF), 0);
}
void Display::DrawText(int x, int y, const std::string &text, uint32_t color, int scale)
{
    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, text.c_str());
    lv_obj_add_style(label, mainFont, 0);
    lv_obj_set_style_text_color(label, lv_color_make((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF), 0);
    lv_obj_set_pos(label, x, y);
}

void Display::DrawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
}

void Display::DrawPixel(int x, int y, uint32_t color)
{
}

void Display::Flush()
{
}

void Display::TimerHandler()
{
    lv_timer_handler();
}