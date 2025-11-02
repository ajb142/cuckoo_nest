#include <stdio.h>
#include <unistd.h>
#include "linux/input.h"

#include "HAL/Beeper.hpp"
#include "HAL/Screen.hpp"
#include "HAL/Inputs.hpp"

enum screen_color
{
   SCREEN_COLOR_BLACK = 0x000000,
   SCREEN_COLOR_WHITE = 0xFFFFFF,
   SCREEN_COLOR_RED   = 0xFF0000,
   SCREEN_COLOR_GREEN = 0x00FF00,
   SCREEN_COLOR_BLUE  = 0x0000FF
};

static enum screen_color colors[] = {
    SCREEN_COLOR_RED,
    SCREEN_COLOR_GREEN,
    SCREEN_COLOR_BLUE,
    SCREEN_COLOR_WHITE,
    SCREEN_COLOR_BLACK};
static int color_count = sizeof(colors) / sizeof(colors[0]);
static int current_color_index = 0;

// Function declarations
void change_screen_color();
void handle_input_event(const std::string& device_name, const struct input_event& event);

static Beeper beeper("/dev/input/beeper");
static Screen screen("/dev/fb0");
static Inputs inputs("/dev/input/button", "/dev/input/rotary");

// Input event handler callback
void handle_input_event(const std::string& device_name, const struct input_event& event) {
    printf("%s: time_sec=%lu, time_usec=%lu, type=%hu, code=%hu, value=%d | \n",
           device_name.c_str(),
           event.time.tv_sec,
           event.time.tv_usec,
           event.type,
           event.code,
           event.value);

    // Handle button press for screen color change
    if (device_name == "rotary" && event.type == EV_KEY && event.code == 't' && event.value == 1) {
        beeper.play(100); // Play beep for 100ms
        change_screen_color();
    }
}

int main() {
   printf("Cuckoo Hello\n");

   if (!screen.initialize()) {
       fprintf(stderr, "Failed to initialize screen\n");
       return 1;
   }

   // Set up input event callback
   inputs.set_callback(handle_input_event);
   
   if (!inputs.start_polling()) {
       fprintf(stderr, "Failed to start input polling\n");
       return 1;
   }
   
   printf("Input polling started in background thread...\n");
   
   // Main thread can now do other work or just wait
   while (1) {
       sleep(1); // Sleep for 1 second - background thread handles input polling
   }

   return 0;
}

void change_screen_color() 
{
   screen.change_color(colors[current_color_index]);

   // Move to the next color
   current_color_index = (current_color_index + 1) % color_count;
}
