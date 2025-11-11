#include <stdio.h>
#include <unistd.h>
#include "linux/input.h"

#include "HAL/HAL.hpp"

#include "Screens/HomeScreen.hpp"
#include "Screens/MenuScreen.hpp"
#include "Screens/DimmerScreen.hpp"
#include "Screens/SwitchScreen.hpp"

#include "Integrations/IntegrationContainer.hpp"
#include "Integrations/ActionHomeAssistantService.hpp"
#include "ConfigurationReader.hpp"

#include <iostream>

#include <ctype.h>

#include "lvgl/lvgl.h"

/**
 * @brief Sets the backlight brightness by writing to the sysfs file.
 *
 * @param brightness The brightness value to set (e.g., 115).
 */
static void set_backlight_brightness(int brightness) {
    FILE *f = fopen("/sys/class/backlight/3-0036/brightness", "w");
    if (f == nullptr) {
        perror("Failed to open backlight file");
        return;
    }
    fprintf(f, "%d\n", brightness);
    fclose(f);
}

/**
 * @brief Animation callback to set the size of an object.
 *
 * @param var The object to animate (the arc).
 * @param v The new value for width and height.
 */
static void anim_size_cb(void * var, int32_t v)
{
    lv_obj_t * obj = (lv_obj_t *)var; 
    lv_obj_set_size(obj, v, v);
    // Recenter the object as it grows/shrinks to keep it centered
    lv_obj_center(obj);
}

// Function declarations
void handle_input_event(const InputDeviceType device_type, const struct input_event &event);

static HAL hal;
static Beeper beeper("/dev/input/event0");
static Display screen("/dev/fb0");
static Inputs inputs("/dev/input/event2", "/dev/input/event1");
static IntegrationContainer integration_container;
static ScreenManager screen_manager(&hal, &integration_container);

int main()
{
    std::cout << "Cuckoo Hello\n";


    set_backlight_brightness(115);
    
    if (!screen.Initialize())
    {
        std::cerr << "Failed to initialize screen\n";
        return 1;
    }



    // /* Create the arc object which will be our expanding/contracting ring */
    // lv_obj_t * arc = lv_arc_create(lv_screen_active());

    // /* Configure the arc to be a full 360-degree ring */
    // lv_arc_set_bg_angles(arc, 0, 360);
    // // We don't need the knob or the indicator line, so remove them.
    // lv_obj_remove_style(arc, NULL, LV_PART_INDICATOR);
    // lv_obj_remove_style(arc, NULL, LV_PART_KNOB);

    // /* Create a style to define the ring's appearance (thickness and color) */
    // static lv_style_t style_ring;
    // lv_style_init(&style_ring);
    // lv_style_set_arc_width(&style_ring, 20); // Set the ring thickness to 20px
    // lv_style_set_arc_color(&style_ring, lv_palette_main(LV_PALETTE_CYAN));
    // lv_obj_add_style(arc, &style_ring, 0);

    // /* Center the arc initially (it will have 0 size) */
    // lv_obj_center(arc);

    // /*
    //  * Configure the animation
    //  */
    // lv_anim_t a;
    // lv_anim_init(&a);
    // lv_anim_set_var(&a, arc);                             // Set the object to animate
    // lv_anim_set_values(&a, 0, 320);                       // Animate from size 0 to 320
    // lv_anim_set_exec_cb(&a, anim_size_cb);                // Set the function to call on each step
    // lv_anim_set_time(&a, 2000);                           // Animation duration in milliseconds (2 seconds)
    // lv_anim_set_playback_time(&a, 2000);                  // Playback (reverse) duration (2 seconds)
    // lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE); // Repeat indefinitely
    // lv_anim_start(&a);                                    // Start the animation!


    /*Handle LVGL tasks*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;



    hal.beeper = &beeper;
    hal.display = &screen;
    hal.inputs = &inputs;

    // Load configuration
    ConfigurationReader config("config.json");
    if (config.load()) {
        std::cout << "Configuration loaded successfully\n";
        std::cout << "App name: " << config.get_string("app_name", "Unknown") << "\n";
        std::cout << "Debug mode: " << (config.get_bool("debug_mode", false) ? "enabled" : "disabled") << "\n";
        std::cout << "Max screens: " << config.get_int("max_screens", 5) << "\n";
        
        // Home Assistant configuration
        if (config.has_home_assistant_config()) {
            std::cout << "Home Assistant configured:\n";
            std::cout << "  Base URL: " << config.get_home_assistant_base_url() << "\n";
            std::cout << "  Token: " << config.get_home_assistant_token().substr(0, 10) << "...\n";
            std::cout << "  Entity ID: " << config.get_home_assistant_entity_id() << "\n";
        } else {
            std::cout << "Home Assistant not configured\n";
        }
    } else {
        std::cout << "Failed to load configuration, using defaults\n";
    }
    
    integration_container.LoadIntegrationsFromConfig("config.json");
    screen_manager.LoadScreensFromConfig("config.json");
    screen_manager.GoToNextScreen(1);

    // Set up input event callback
    inputs.set_callback(handle_input_event);

    if (!inputs.start_polling())
    {
        std::cerr << "Failed to start input polling\n";
        return 1;
    }

    std::cout << "Input polling started in background thread...\n";

    // Main thread can now do other work or just wait
    int tick = 0;
    const int ticks_per_second = 1 * 1000 * 1000 / 5000; // 1 second / input polling interval (5ms)
    while (true)
    {
        screen.TimerHandler();
        tick++;
        if (tick >= ticks_per_second)
        {
            tick = 0;
            // Do once-per-second tasks here if needed
            screen_manager.RenderCurrentScreen();
        }

        usleep(5000); // Sleep for 5ms
    }

    return 0;
}

// Input event handler callback
void handle_input_event(const InputDeviceType device_type, const struct input_event &event)
{
    if (device_type == InputDeviceType::ROTARY && event.type == 0 && event.code == 0)
    {
        return; // Ignore "end of event" markers from rotary encoder
    }

    screen_manager.ProcessInputEvent(device_type, event);
    screen_manager.RenderCurrentScreen();
}
