#include "HomeScreen.hpp"
#include <string>
enum screen_color
{
   SCREEN_COLOR_BLACK = 0x000000,
   SCREEN_COLOR_WHITE = 0xFFFFFF,
   SCREEN_COLOR_RED   = 0xFF0000,
   SCREEN_COLOR_GREEN = 0x00FF00,
   SCREEN_COLOR_BLUE  = 0x0000FF
};

static enum screen_color colors[] = {
    SCREEN_COLOR_BLACK,
    SCREEN_COLOR_RED,
    SCREEN_COLOR_GREEN,
    SCREEN_COLOR_BLUE,
    SCREEN_COLOR_WHITE,
};
static int color_count = sizeof(colors) / sizeof(colors[0]);

void HomeScreen::Render()
{
    if (screen_ == nullptr) {
        return;
    }

    screen_->SetBackgroundColor(colors[currentColorIndex]);

    // screen_->DrawLine(0, 160, 320, 160, SCREEN_COLOR_WHITE);
    
    // Display text using the bitmap font
    uint32_t text_color = SCREEN_COLOR_WHITE;
    if (colors[currentColorIndex] == SCREEN_COLOR_WHITE) {
        text_color = SCREEN_COLOR_BLACK; // Use black text on white background
    }
    
    // Leaving these examples for now, others may find them useful.
    // // Display various text examples
    // screen_->DrawText(50, 60, "Hello World!", text_color, 1);
    // screen_->DrawText(50, 70, "ASCII Test: !@#$%", text_color, 1);
    // screen_->DrawText(50, 80, "Numbers: 0123456789", text_color, 1);
    // screen_->DrawText(50, 100, "Scaled Text", text_color, 2);
    // screen_->DrawText(50, 130, "Color Index: " + std::to_string(currentColorIndex), text_color, 1);

    // get current time
    time_t now = time(0);
    screen_->DrawText(40, 140, TimeToString(now), text_color, 4);
}

std::string HomeScreen::TimeToString(time_t time) {
char buffer[100];
struct tm* timeinfo = localtime(&time);
strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
return std::string(buffer);
}

void HomeScreen::handle_input_event(const InputDeviceType device_type, const struct input_event& event)
{
    if (device_type == InputDeviceType::BUTTON && event.type == EV_KEY && event.code == 't' && event.value == 1)
    {
        if (beeper_ != nullptr)
        {
            beeper_->play(100);
        }   
        
        // Move to the next color
        currentColorIndex = (currentColorIndex + 1) % color_count;
    }
}