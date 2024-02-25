/**
 * @file main.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date YYYY-MM-DD
 *
 * @copyright Copyright (c) YYYY
 *
 */
#pragma once
#include <display/console.h>
#include <tp/f_ap_game.h>
#include <tp/d_a_alink.h>
#include <tp/d_meter2.h>

#include <cinttypes>

#define GAME_BOOT 0
#define GAME_TITLE 1
#define GAME_ACTIVE 2
namespace mod
{
    /***********************************************************************************
     * We're creating a cusutom REL file and thus the real main function already ran
     * before we even load this program.
     * That's also why we create a custom namespace to avoid confusing the compiler with the actual main function
     * whilst still having a neat starting function for you to begin your Twilight Princess mod development!
     * Note:
     * If you want to change the namespace "mod" you will have to make adjustments to rel.cpp in libtp_rel!
     *
     * This main function is going to be executed once at the beginning of the game,
     * assuming the REL got loaded in the first place.
     ***********************************************************************************/
    void main();
    class Mod
    {
       public:
        Mod();
        void init();

       public:
        // Counter
        int trimer;
        // Console
        libtp::display::Console c;
        // "trampoline/return" function to the original function that we hook in order to proc our NewFrame function
        void ( *buttonthing )() = nullptr;
        void ( *life_thingy)(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr) = nullptr;
        void ( *offUI)(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr) = nullptr;


        /**
         * @brief This function is called when there's a frame update
         */
        void procNewFrame();

        /**
         * @brief Plays every frame that lantern meter oil is present
         * 
         * @param dMeterPtr A pointer to the current dMeter2 structure.
        */
        //void rupeePoggy(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr);*/
        /**
         * @brief checks if heart UI is on for first frame
         * 
         * @param dMeterPtr A pointer to the current dMeter2 structure.
        */
        void UICheck(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr);
        /**
         * @brief checks if heart UI is off for first frame
         * 
         * @param dMeterPtr A pointer to the current dMeter2 structure.
        */
        void UICheck2(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr);
    };
}     // namespace mod