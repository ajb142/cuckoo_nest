#include "DimmerScreen.hpp"
#include <cstdio>  // for snprintf
#include <cstring> // for memset

void DimmerScreen::Render()
{
    if (display_ == nullptr) {
        return;
    }

    display_->SetBackgroundColor(SCREEN_COLOR_BLACK);

    int brightnessPercent = dimmerValue / DIMMER_STEP;

    // Persistent buffer for the percentage text
    char dimmerValueBuffer[8]; // enough for "100%"
    snprintf(dimmerValueBuffer, sizeof(dimmerValueBuffer), "%d%%", brightnessPercent);

    // Draw the header text
    display_->DrawText(60, 80, "Dimmer", SCREEN_COLOR_WHITE, Font::FONT_H1);

    // Draw the brightness percentage
    display_->DrawText(100, 140, dimmerValueBuffer, SCREEN_COLOR_WHITE, Font::FONT_H2);

    // Draw a simple horizontal brightness bar
    const int barWidth = 200;
    const int barHeight = 20; // unused here, could be used in real DrawRect

    int filledWidth = (brightnessPercent * barWidth) / MAX_DIMMER_VALUE;

    // Persistent buffer for the bar
    static char barBuffer[201]; // max bar width + 1

    // Draw the background of the bar
    memset(barBuffer, '-', barWidth);
    barBuffer[barWidth] = '\0';
    display_->DrawText(60, 180, barBuffer, SCREEN_COLOR_WHITE, Font::FONT_DEFAULT);

    // Draw the filled part of the bar
    if (filledWidth > 0)
    {
        memset(barBuffer, '=', filledWidth);
        barBuffer[filledWidth] = '\0';
        display_->DrawText(60, 180, barBuffer, SCREEN_COLOR_BLUE, Font::FONT_DEFAULT);
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
