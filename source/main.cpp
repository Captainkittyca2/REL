/**
 * @file main.cpp
 * @author Captain Kitty Cat (you@domain.com)
 * @brief Link loses health when touching a rupee
 * @version 0.9
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
#include <tp/d_meter2.h>
#include <gc_wii/OS.h>
#include <gc_wii/OSTime.h>
#ifdef PLATFORM_WII
#include <tp/m_re_controller_pad.h>
#else
#include <tp/m_do_controller_pad.h>
#endif
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
        strcpy( sysConsolePtr->consoleLine[10].line, "Game mode:       <none>" );
        strcpy( sysConsolePtr->consoleLine[20].line, "Option Toggle: Press D-Pad Left or Right");
#ifdef PLATFORM_WII
        strcpy( sysConsolePtr->consoleLine[21].line, "Console On/Off: Hold D-Pad Down and Press Minus");
#else
        strcpy( sysConsolePtr->consoleLine[21].line, "Console On/Off: Hold D-Pad Down and Press L");
#endif
        strcpy( sysConsolePtr->consoleLine[23].line, "youtube.com/@captainkittyca2" );
        gMod = this;
        // Hook the function that runs each frame
        life_thingy =
            libtp::patch::hookFunction(libtp::tp::d_meter2_draw::setAlphaLifeAnimeMax ,[](libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr){ return gMod->UICheck(dMeterDrawPtr);});
        offUI =
            libtp::patch::hookFunction(libtp::tp::d_meter2_draw::setAlphaLifeAnimeMin ,[](libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr){ return gMod->UICheck2(dMeterDrawPtr);});
        buttonthing =
            libtp::patch::hookFunction( libtp::tp::f_ap_game::fapGm_Execute, []() { return gMod->procNewFrame(); } );
        
    }

    libtp::tp::d_com_inf_game::dComIfG_inf_c* gameinfoptr = &libtp::tp::d_com_inf_game::dComIfG_gameInfo;
    libtp::tp::d_save::dSv_player_status_a_c* playerStatusPtr = &gameinfoptr->save.save_file.player.player_status_a;
    libtp::tp::d_a_alink::daAlink* linkMapPtr = gameinfoptr->play.mPlayer;
    int32_t newHealth;
    int rupeeee = 0;
    int consooole = true;
    int toggle = 0;
    int speedToggle = 5;
    int32_t iiimer = 0;
    int64_t second = 0;
    int64_t counting = 0;
    char speeeed[23];
    char counter[20];
    bool UiChecking = false;
    //bool firstTime = true;

    void rupeePoggy() {
        if (toggle != 0) {
            if (toggle != 3) {
                if (rupeeee < playerStatusPtr->currentRupees) {
                    if (toggle == 1) {
                        newHealth = playerStatusPtr->currentHealth - 4;
                        if (playerStatusPtr->currentForm == 0) libtp::tp::d_a_alink::procDamageInit(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer, nullptr, 1);
                        else libtp::tp::d_a_alink::procWolfDamageInit(libtp::tp::d_com_inf_game::dComIfG_gameInfo.play.mPlayer, nullptr);
                        if (newHealth < 0) newHealth = 0;
                    } else if (toggle == 2) newHealth = 0;
                    playerStatusPtr->currentHealth = static_cast<uint16_t>(newHealth);
                    rupeeee = playerStatusPtr->currentRupees;
                } else if (rupeeee > playerStatusPtr->currentRupees) rupeeee = playerStatusPtr->currentRupees;
            } else {
                for (counting = iiimer; counting >= 0; counting--) {
                    if ((libtp::gc_wii::os_time::OSGetTime() - second) <= static_cast<int64_t>(( counting * (libtp::gc_wii::os::__OSBusClock/4)))) {
                        sprintf(counter, "Time Left: %lld", iiimer - counting);
                        strcpy( sysConsolePtr->consoleLine[5].line, counter);
                    }
                }
                if ((libtp::gc_wii::os_time::OSGetTime() - second) >= static_cast<int64_t>(( iiimer * (libtp::gc_wii::os::__OSBusClock/4)))) {
                    if (playerStatusPtr->currentRupees > 0) playerStatusPtr->currentRupees--;
                    else {newHealth = playerStatusPtr->currentHealth - 4; if (newHealth < 0) newHealth = 0; playerStatusPtr->currentHealth = static_cast<uint16_t>(newHealth);}
                    rupeeee = playerStatusPtr->currentRupees;
                    second = libtp::gc_wii::os_time::OSGetTime();
                }
            }
        } else if (rupeeee != playerStatusPtr->currentRupees) rupeeee = playerStatusPtr->currentRupees;
    }

    void terminalUpdateText(){
        if (toggle < 0) toggle = 3; else if (toggle > 3) toggle = 0;
        if (toggle == 0) {strcpy( sysConsolePtr->consoleLine[5].line, ""); strcpy( sysConsolePtr->consoleLine[10].line, "Game mode:       <none>" ); strcpy( sysConsolePtr->consoleLine[11].line, ""); strcpy( sysConsolePtr->consoleLine[18].line, ""); strcpy( sysConsolePtr->consoleLine[19].line, "");}
        else if (toggle == 1) {strcpy( sysConsolePtr->consoleLine[5].line, ""); strcpy( sysConsolePtr->consoleLine[10].line, "Game mode:       <rupee damage>" ); strcpy( sysConsolePtr->consoleLine[11].line, ""); strcpy( sysConsolePtr->consoleLine[19].line, "Link loses health when collecting rupees.");}
        else if (toggle == 2) {strcpy( sysConsolePtr->consoleLine[5].line, ""); strcpy( sysConsolePtr->consoleLine[10].line, "Game mode:       <rupee instakill>" ); strcpy( sysConsolePtr->consoleLine[11].line, ""); strcpy( sysConsolePtr->consoleLine[18].line, ""); strcpy( sysConsolePtr->consoleLine[19].line, "Link dies when collecting a rupee.");}
        else {
            strcpy( sysConsolePtr->consoleLine[10].line, "Game mode:       <rupee dash mode (experimental)>" );
            strcpy( sysConsolePtr->consoleLine[18].line, "Speed Toggle: Press A to change speed");
            strcpy( sysConsolePtr->consoleLine[19].line, "You keep losing rupees. Otherwise, You lose health.");
            for (int boiii = 1; boiii <= 10; boiii++) {
                if (boiii == speedToggle) sprintf(speeeed, "Speed (sec)       <%d>", boiii);
            }
            strcpy(sysConsolePtr->consoleLine[11].line, speeeed);
            iiimer = speedToggle;
            //firstTime = true;
        }
    }
    void Mod::UICheck(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr) {
        if (UiChecking == false && toggle == 3) {
            UiChecking = true;
            //if (firstTime) {
                second = libtp::gc_wii::os_time::OSGetTime();
                //firstTime = false;
            //} else second = libtp::gc_wii::os_time::OSGetTime()*(1/secondPreservation);
        }
        rupeePoggy();
        return life_thingy(dMeterDrawPtr);
        (void)dMeterDrawPtr;
    }
    void Mod::UICheck2(libtp::tp::d_meter2_draw::dMeter2Draw_c* dMeterDrawPtr) {
        (void)dMeterDrawPtr;
        if (UiChecking == true) {
            UiChecking = false;
            /*if (toggle == 3) {
                secondPreservation = 1/(second * ((iiimer * (libtp::gc_wii::os_time::__OSBusClock/4))));
            }*/
        }
        return offUI(dMeterDrawPtr);
    }

    void Mod::procNewFrame() {
#ifdef PLATFORM_WII
        if (checkForButtonInput(libtp::tp::m_re_controller_pad::ReCPadInputs::Button_DPad_Down) && checkForButtonInputSingleFrame(libtp::tp::m_re_controller_pad::ReCPadInputs::Button_Minus)) {
#else
        if (checkButtonsPressed(libtp::tp::m_do_controller_pad::Button_DPad_Down) && checkButtonsPressedThisFrame(libtp::tp::m_do_controller_pad::Button_L)) {
#endif
            if (consooole) {
                if (toggle != 3) libtp::display::setConsole(false, 0); else {libtp::display::setConsoleColor(0,0,0,0); strcpy( sysConsolePtr->consoleLine[0].line, ""); strcpy( sysConsolePtr->consoleLine[1].line, ""); sysConsolePtr->consoleLine[10].showLine = false;}
                consooole = false;
            } else {
                if (toggle != 3) libtp::display::setConsole(true, 25); else {libtp::display::setConsoleColor(0,0,0,255); strcpy( sysConsolePtr->consoleLine[0].line, "Rupee Challenges - v0.9.0 public"); strcpy( sysConsolePtr->consoleLine[1].line, "Build Date: 25 February 2024"); sysConsolePtr->consoleLine[10].showLine = true;}
                consooole = true;
            }
        } 
            else if (consooole) {
#ifdef PLATFORM_WII
            if (checkForButtonInputSingleFrame(libtp::tp::m_re_controller_pad::ReCPadInputs::Button_DPad_Left)) {toggle--; terminalUpdateText();}
            else if (checkForButtonInputSingleFrame(libtp::tp::m_re_controller_pad::ReCPadInputs::Button_DPad_Right)) {toggle++; terminalUpdateText();}
#else
            if (checkButtonsPressedThisFrame(libtp::tp::m_do_controller_pad::Button_DPad_Left)) {toggle--; terminalUpdateText();}
            else if (checkButtonsPressedThisFrame(libtp::tp::m_do_controller_pad::Button_DPad_Right)) {toggle++; terminalUpdateText();}
#endif
            if (toggle == 3) {
#ifdef PLATFORM_WII
                if (checkForButtonInputSingleFrame(libtp::tp::m_re_controller_pad::ReCPadInputs::Button_A)) {
#else
                if (checkButtonsPressedThisFrame(libtp::tp::m_do_controller_pad::Button_A)) {
#endif
                    speedToggle++;
                    if (speedToggle > 10) speedToggle = 1;
                    terminalUpdateText();
                }
            }
        }
        return buttonthing();
    }
}     // namespace mod