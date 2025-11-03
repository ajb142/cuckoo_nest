#include "ScreenManager.hpp"

ScreenManager::ScreenManager(): 
    screen_history_(),
    current_screen_(nullptr)
{
}

ScreenManager::~ScreenManager()
{
}

void ScreenManager::GoToNextScreen(ScreenBase *screen)
{
    screen_history_.push(current_screen_);
    current_screen_ = screen;
    current_screen_->Render();
}

void ScreenManager::GoToPreviousScreen()
{
    if (!screen_history_.empty()) {
        current_screen_ = screen_history_.top();
        screen_history_.pop();
        current_screen_->Render();
    }
}