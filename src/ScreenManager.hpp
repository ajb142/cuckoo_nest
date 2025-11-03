#pragma once

#include <memory>
#include <vector>
#include <stack>
#include "Screens/ScreenBase.hpp"

class ScreenManager 
{
public:
    ScreenManager();
    ~ScreenManager();
    
    void GoToNextScreen(ScreenBase* screen);
    void GoToPreviousScreen();


private:
    std::stack<ScreenBase*> screen_history_;
    ScreenBase* current_screen_;
};