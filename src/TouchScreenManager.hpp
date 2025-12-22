#pragma once

#include <SDL.h>

#include "ZunResult.hpp"
#include "AnmVm.hpp"

#define ANM_FILE_TOUCHSCREEN                      47
#define ANM_OFFSET_TOUCHSCREEN                    0x719

#define ANM_SPRITE_TOUCHSCREEN_BUTTON             ANM_OFFSET_TOUCHSCREEN
#define ANM_SPRITE_TOUCHSCREEN_BUTTON_PRESSED     (ANM_OFFSET_TOUCHSCREEN + 1)
#define ANM_SPRITE_TOUCHSCREEN_JOYSTICK           (ANM_OFFSET_TOUCHSCREEN + 2)
#define ANM_SPRITE_TOUCHSCREEN_JOYSTICK_TOP       (ANM_OFFSET_TOUCHSCREEN + 3)

#define ANM_SCRIPT_TOUCHSCREEN_BUTTON             ANM_OFFSET_TOUCHSCREEN
#define ANM_SCRIPT_TOUCHSCREEN_JOYSTICK           (ANM_OFFSET_TOUCHSCREEN + 1)
#define ANM_SCRIPT_TOUCHSCREEN_JOYSTICK_TOP       (ANM_OFFSET_TOUCHSCREEN + 2)

#define ANM_INTERRUPT_TOUCHSCREEN_BUTTON_RELEASED 0
#define ANM_INTERRUPT_TOUCHSCREEN_BUTTON_PRESSED  1

enum
{
    TouchScreenSprite_Joystick,
    TouchScreenSprite_JoystickTop,
    TouchScreenSprite_BtnFocus,
    TouchScreenSprite_BtnShoot,
    TouchScreenSprite_BtnBomb,
    TouchScreenSprite_Last,
};

struct TouchScreenManager
{
    ZunResult Init(void);
    void HandleEvent(const SDL_Event *event);
    void OnUpdate(void);
    void OnDraw(void);

    AnmVm vms[TouchScreenSprite_Last];
};

extern TouchScreenManager g_TouchScreenManager;