#include "DimmerScreen.hpp"
#include <string>


void DimmerScreen::Render()
{
    if (display_ == nullptr) return;

    display_->SetBackgroundColor(SCREEN_COLOR_BLACK);

    // Draw title
    display_->DrawText(60, 80, "Dimmer", SCREEN_COLOR_WHITE, Font::FONT_H1);

    // Numeric value
    std::string dimmerValueString = std::to_string(dimmerValue / DIMMER_STEP) + "%";
    display_->DrawText(100, 140, dimmerValueString, SCREEN_COLOR_WHITE, Font::FONT_H2);

    // Semi-circle parameters
    int centerX = 160;
    int centerY = 220;
    int radius = 80;

    // Background semi-circle (180°)
    for (float angle = 180; angle >= 0; angle -= 1.0f)
    {
        float rad = angle * M_PI / 180.0f;
        int x = centerX + int(radius * cos(rad));
        int y = centerY - int(radius * sin(rad));
        display_->DrawPixel(x, y, SCREEN_COLOR_GRAY);
    }

    // Filled portion based on dimmerValue
    float fillRatio = float(dimmerValue - MIN_DIMMER_VALUE) / ((MAX_DIMMER_VALUE * DIMMER_STEP) - MIN_DIMMER_VALUE);
    int fillAngle = int(180 * fillRatio);
    for (float angle = 180; angle >= 180 - fillAngle; angle -= 1.0f)
    {
        float rad = angle * M_PI / 180.0f;
        int x = centerX + int(radius * cos(rad));
        int y = centerY - int(radius * sin(rad));
        display_->DrawPixel(x, y, SCREEN_COLOR_GREEN);
    }
}

void DimmerScreen::handle_input_event(const InputDeviceType device_type, const struct input_event &event)
{
    if (device_type == InputDeviceType::ROTARY)
    {
        dimmerValue -= event.value;
        if (dimmerValue > MAX_DIMMER_VALUE * DIMMER_STEP)
            dimmerValue = MAX_DIMMER_VALUE * DIMMER_STEP;

        if (dimmerValue < MIN_DIMMER_VALUE)
            dimmerValue = MIN_DIMMER_VALUE;
    }

    if (device_type == InputDeviceType::BUTTON && event.type == EV_KEY && event.code == 't' && event.value == 1)
    {
        if (beeper_ != nullptr)
        {
            beeper_->play(100);
        }   
        
        auto dimmer = screenManager_->GetIntegrationContainer()->GetDimmerById(integrationId_);

        if (dimmer != nullptr)
        {
            int brightnessPercent = dimmerValue / DIMMER_STEP;
            dimmer->SetBrightness(brightnessPercent);
        }

        screenManager_->GoToPreviousScreen();
    }
}