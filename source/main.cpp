/**
 * @file main.cpp
 * @author Captain Kitty Cat (you@domain.com)
 * @brief Link loses health when touching a rupee
 * @version 0.1
 * @date 2024-02-10
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
#include <tp/d_a_alink.h>
#include <tp/d_com_inf_game.h>
#include <tp/d_save.h>
#include <tp/d_item.h>
#include <tp/JFWSystem.h>
#ifdef PLATFORM_WII
#include <tp/m_re_controller_pad.h>
#else
#include <tp/m_do_controller_pad.h>
#endif
libtp::tp::d_com_inf_game::dComIfG_inf_c g_dcomIfG_gameInfo;
namespace mod
{
    Mod* gMod = nullptr;
    float prevFrameAnalogR = 0.f;

    void main()
    {
        Mod* mod = new Mod();
        mod->init();
    }

    void exit() {}

    // Create our console instance (it will automatically display some of the definitions from our Makefile like version,
    // variant, project name etc.
    // this console can be used in a similar way to cout to make printing a little easier; it also supports \n for new lines
    // (\r is implicit; UNIX-Like) and \r for just resetting the column and has overloaded constructors for all of the
    // primary cinttypes
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
        //libtp::display::setConsoleColor(200, 21, 148, 50);
        //libtp::display::setConsole(true, 25);
        // Print controls
        strcpy( sysConsolePtr->consoleLine[20].line, "Option Toggle: Press D-Pad Left or Right");
#ifdef PLATFORM_WII
        strcpy( sysConsolePtr->consoleLine[21].line, "Console On/Off: Hold D-Pad Down and Press Minus");
#else
        strcpy( sysConsolePtr->consoleLine[21].line, "Console On/Off: Hold D-Pad Down and Press L");
#endif
        strcpy( sysConsolePtr->consoleLine[23].line, "youtube.com/@captainkittyca2" );
        gMod = this;
        // Hook the function that runs each frame
        return_fapGm_Execute =
            libtp::patch::hookFunction( libtp::tp::f_ap_game::fapGm_Execute, []() { return gMod->procNewFrame(); } );
    }
#ifdef PLATFORM_WII
    bool checkForButtonInput(uint32_t buttonCombo)
    {
        return (libtp::tp::m_re_controller_pad::mReCPd::m_pad->buttonInput & buttonCombo) == buttonCombo;
    }

    bool checkForButtonInputSingleFrame(uint32_t buttonCombo)
    {
        libtp::tp::m_re_controller_pad::ReCPad* padInfo = &libtp::tp::m_re_controller_pad::mReCPd::m_pad[0];
        if ((padInfo->buttonInput & buttonCombo) == buttonCombo)
        {
            if (padInfo->buttonInputTrig & buttonCombo)
            {
                return true;
            }
        }
        return false;
    }
#else
    bool checkButtonsPressedThisFrame(uint32_t buttons)
        {
            using namespace libtp::tp::m_do_controller_pad;
            CPadInfo* padInfo = &cpadInfo[PAD_1];

            return padInfo->mPressedButtonFlags & buttons;
        }

    bool checkButtonsPressed(uint32_t buttons)
        {
            using namespace libtp::tp::m_do_controller_pad;
            CPadInfo* padInfo = &cpadInfo[PAD_1];

            return (padInfo->mButtonFlags & buttons) == buttons;
        }
#endif
    libtp::tp::d_com_inf_game::dComIfG_inf_c* gameinfoptr = &libtp::tp::d_com_inf_game::dComIfG_gameInfo;
    libtp::tp::d_a_alink::daAlink* linkMapPtr = gameinfoptr->play.mPlayer;
    libtp::tp::d_save::dSv_player_status_a_c* playerStatusPtr = &gameinfoptr->save.save_file.player.player_status_a;
    int32_t newHealth;
    int rupeeee = 0;
    int consooole = true;
    int toggle = 0;
    int speedToggle = 5;
    int iiimer = 150;
    void Mod::procNewFrame()
    {
#ifdef PLATFORM_WII
        if (checkForButtonInput(libtp::tp::m_re_controller_pad::ReCPadInputs::Button_DPad_Down) && checkForButtonInputSingleFrame(libtp::tp::m_re_controller_pad::ReCPadInputs::Button_Minus)) {
#else
        if (checkButtonsPressed(libtp::tp::m_do_controller_pad::Button_DPad_Down) && checkButtonsPressedThisFrame(libtp::tp::m_do_controller_pad::Button_L)) {
#endif
            if (consooole) {libtp::display::setConsole(false, 0); consooole = false;} else {libtp::display::setConsole(true, 25); consooole = true;}
        } 
            else if (consooole) {
#ifdef PLATFORM_WII
            if (checkForButtonInputSingleFrame(libtp::tp::m_re_controller_pad::ReCPadInputs::Button_DPad_Left)) toggle--;
            else if (checkForButtonInputSingleFrame(libtp::tp::m_re_controller_pad::ReCPadInputs::Button_DPad_Right)) toggle++;
#else
            if (checkButtonsPressedThisFrame(libtp::tp::m_do_controller_pad::Button_DPad_Left)) toggle--;
            else if (checkButtonsPressedThisFrame(libtp::tp::m_do_controller_pad::Button_DPad_Right)) toggle++;
#endif
            if (toggle < 0) toggle = 3; else if (toggle > 3) toggle = 0;
            if (toggle == 0) {strcpy( sysConsolePtr->consoleLine[10].line, "Game mode:       <none>" ); strcpy( sysConsolePtr->consoleLine[11].line, ""); strcpy( sysConsolePtr->consoleLine[18].line, "");}
            else if (toggle == 1) {strcpy( sysConsolePtr->consoleLine[10].line, "Game mode:       <rupee damage>" ); strcpy( sysConsolePtr->consoleLine[11].line, ""); strcpy( sysConsolePtr->consoleLine[18].line, ""); strcpy( sysConsolePtr->consoleLine[19].line, "Link loses health when collecting rupees.");}
            else if (toggle == 2) {strcpy( sysConsolePtr->consoleLine[10].line, "Game mode:       <rupee instakill>" ); strcpy( sysConsolePtr->consoleLine[11].line, ""); strcpy( sysConsolePtr->consoleLine[18].line, ""); strcpy( sysConsolePtr->consoleLine[19].line, "Link dies when collecting a rupee.");}
            else {
                strcpy( sysConsolePtr->consoleLine[10].line, "Game mode:       <rupee dash mode (experimental)>" );
                strcpy( sysConsolePtr->consoleLine[18].line, "Speed Toggle: Press A to change speed");
                strcpy( sysConsolePtr->consoleLine[19].line, "You keep losing rupees. Eventually, You lose health.");
#ifdef PLATFORM_WII
                if (checkForButtonInputSingleFrame(libtp::tp::m_re_controller_pad::ReCPadInputs::Button_A)) {
#else
                if (checkButtonsPressedThisFrame(libtp::tp::m_do_controller_pad::Button_A)) {
#endif
                    speedToggle++;
                    if (speedToggle > 10) speedToggle = 1;
                }
                if (speedToggle == 1) strcpy( sysConsolePtr->consoleLine[11].line, "Speed (sec)       <1>");
                else if (speedToggle == 2) strcpy( sysConsolePtr->consoleLine[11].line, "Speed (sec)       <2>");
                else if (speedToggle == 3) strcpy( sysConsolePtr->consoleLine[11].line, "Speed (sec)       <3>");
                else if (speedToggle == 4) strcpy( sysConsolePtr->consoleLine[11].line, "Speed (sec)       <4>");
                else if (speedToggle == 5) strcpy( sysConsolePtr->consoleLine[11].line, "Speed (sec)       <5>");
                else if (speedToggle == 6) strcpy( sysConsolePtr->consoleLine[11].line, "Speed (sec)       <6>");
                else if (speedToggle == 7) strcpy( sysConsolePtr->consoleLine[11].line, "Speed (sec)       <7>");
                else if (speedToggle == 8) strcpy( sysConsolePtr->consoleLine[11].line, "Speed (sec)       <8>");
                else if (speedToggle == 9) strcpy( sysConsolePtr->consoleLine[11].line, "Speed (sec)       <9>");
                else if (speedToggle == 10) strcpy( sysConsolePtr->consoleLine[11].line, "Speed (sec)       <10>");
                iiimer = 30 * speedToggle;

            }
        }
        // This runs BEFORE the original (hooked) function (fapGm_Execute)
        if (toggle != 0) {
            if (rupeeee < playerStatusPtr->currentRupees) {
                if (toggle == 1) {
                    newHealth = playerStatusPtr->currentHealth - 4;
                    if (newHealth < 0) newHealth = 0;
                } else if (toggle == 2) newHealth = 0;
                playerStatusPtr->currentHealth = static_cast<uint16_t>(newHealth);
                rupeeee = playerStatusPtr->currentRupees;
            } else if (rupeeee > playerStatusPtr->currentRupees) rupeeee = playerStatusPtr->currentRupees;
        } else if (rupeeee != playerStatusPtr->currentRupees) rupeeee = playerStatusPtr->currentRupees;
        if (toggle == 3) {
            trimer++;
            if (trimer >= iiimer) {
                trimer = 0;
                if (playerStatusPtr->currentRupees > 0) playerStatusPtr->currentRupees--;
                else {newHealth = playerStatusPtr->currentHealth - 4; if (newHealth < 0) newHealth = 0; playerStatusPtr->currentHealth = static_cast<uint16_t>(newHealth);}
                rupeeee = playerStatusPtr->currentRupees;
            }
        }
                /*if (trimer == 900) {
                    trimer = 0;
                    //libtp::tp::m_Do_Audio::mDoAud_seStartLevel(0x74, 0, 0, 0);
                    //libtp::tp::d_a_alink::setClothesChange(linkMapPtr, 0);
                    //g_dcomIfG_gameInfo.play.mSelectEquip[1] = 0x2F;
                }
            c << "\r" << "trimer: " << trimer;*/
        // return what our original function would've returned (in this case the return is obsolete since it is a void func)
        // And most importantly, since this is related to the frame output, call the original function at all because it may do
        // important stuff that would otherwise be skipped!

        return return_fapGm_Execute();     // hookFunction replaced this return_ function with a branch back to the original
                                           // function so that we can use it now
    }
}     // namespace mod
