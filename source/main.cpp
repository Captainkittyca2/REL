/**
 * @file main.cpp
 * @author Captain Kitty Cat (you@domain.com)
 * @brief Displays the FPS in-game
 * @version 1.0
 * @date 2024-03-05
 *
 * @copyright Copyright (c) YYYY
 *
 */
#include <display/console.h>     // Contains a very neat helper class to print to the console
#include <main.h>
#include <patch.h>     // Contains code for hooking into a function
#include <cstdio>
#include <cstring>
#include <tp/f_ap_game.h>
#include <tp/JFWSystem.h>
#include <gc_wii/OS.h>
#include <gc_wii/OSTime.h>
namespace mod
{
    Mod* gMod = nullptr;
    float prevFrameAnalogR = 0.f;
    uint8_t gameState = GAME_BOOT;

    void main()
    {
        Mod* mod = new Mod();
        mod->init();
    }

    void exit() {}

    Mod::Mod() : c( 0 )
    {
        trimer = 0;
    }
    libtp::tp::jfw_system::SystemConsole* sysConsolePtr = libtp::tp::jfw_system::systemConsole;

    void Mod::init()
    {
        /**
         * Old way of printing to the console
         * Kept for reference as its still being used by the new console class.
         *
         * libtp::display::setConsole(true, 25);
         * libtp::display::print(1, "Hello World!");
         */
        libtp::display::setConsoleColor(200, 21, 148, 0);
        gMod = this;
        // Hook the function that runs each frame
        buttonthing =
            libtp::patch::hookFunction( libtp::tp::f_ap_game::fapGm_Execute, []() { return gMod->procNewFrame(); } );
    }

    int64_t second = 1;
    char speeeed[23];
    char counter[20];
    int frameCount = 0;


    void Mod::procNewFrame() {
        frameCount++;
        if ((libtp::gc_wii::os_time::OSGetTime() - second) >= static_cast<int64_t>((libtp::gc_wii::os::__OSBusClock/4))) {
            sprintf(counter, "FPS: %d", frameCount);
            strcpy( sysConsolePtr->consoleLine[5].line, counter);
            second = libtp::gc_wii::os_time::OSGetTime();
            frameCount = 0;
        }
        return buttonthing();
    }
}     // namespace mod