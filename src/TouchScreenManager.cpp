#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"
#include "TouchScreenManager.hpp"
#include "utils.hpp"
#include "ZunMath.hpp"

TouchScreenManager g_TouchScreenManager;

ZunResult TouchScreenManager::Init(void)
{
    AnmVm *vm;

    if (g_AnmManager->LoadAnm(ANM_FILE_TOUCHSCREEN, "buttons.anm", ANM_OFFSET_TOUCHSCREEN) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }
    
    g_AnmManager->SetAndExecuteScriptIdx(&this->vms[TouchScreenSprite_Joystick],
                                         ANM_SCRIPT_TOUCHSCREEN_JOYSTICK);

    g_AnmManager->SetAndExecuteScriptIdx(&this->vms[TouchScreenSprite_JoystickTop],
                                         ANM_SCRIPT_TOUCHSCREEN_JOYSTICK_TOP);
    
    for (int i = TouchScreenSprite_BtnFocus; i < TouchScreenSprite_Last; i++)
    {
        vm = &this->vms[i];
        g_AnmManager->SetAndExecuteScriptIdx(vm, ANM_SCRIPT_TOUCHSCREEN_BUTTON);
        vm->pos.z = 1.0f;
    }
    
    int controlsY = GAME_WINDOW_HEIGHT - 80.0f;

    for (int i = TouchScreenSprite_Joystick; i <= TouchScreenSprite_JoystickTop; i++)
    {
        vm = &this->vms[i];
        vm->pos.x = 40.0f;
        vm->pos.y = controlsY;
    }

    for (int i = TouchScreenSprite_BtnFocus; i < TouchScreenSprite_Last; i++)
    {
        int buttonIdx = i - TouchScreenSprite_BtnFocus;

        vm = &this->vms[i];
        vm->pos.x = (GAME_WINDOW_WIDTH - 120.0f) + (buttonIdx * (32.0f + 4.0f));
        vm->pos.y = controlsY;
    }

    return ZUN_SUCCESS;
}

void TouchScreenManager::HandleEvent(const SDL_Event *event)
{
    bool windowPress = false;
    bool isDown = false;
    
    switch (event->type)
    {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_FINGERDOWN:
            windowPress = true;
        case SDL_KEYDOWN:
            isDown = true;
            break;
        case SDL_MOUSEBUTTONUP:
        case SDL_FINGERUP:
            windowPress = true;
        case SDL_KEYUP:
            break;
        default:
            return;
    }

    SDL_Joystick *joystick = SDL_JoystickOpen(g_Supervisor.virtualJoystick);

    ZunViewport viewport;

    viewport.Get();
    
    int pressX = 0;
    int pressY = 0;


    if (windowPress && isDown)
    {
        
        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            pressX = event->button.x;
            pressY = event->button.y;
        }
        else if (event->type == SDL_FINGERDOWN)
        {
            pressX = event->tfinger.x * viewport.width;
            pressY = event->tfinger.y * viewport.height;
        }

        utils::DebugPrint2("%d %d", pressX, pressY);
    }


    switch (g_Supervisor.curState)
    {
        case SUPERVISOR_STATE_MAINMENU:
        {
            if (windowPress)
            {
                SDL_JoystickSetVirtualButton(joystick,
                                             g_Supervisor.cfg.controllerMapping.shootButton,
                                             isDown);
            }
            else
            {
                if (event->key.keysym.sym == SDLK_AC_BACK)
                {
                    SDL_JoystickSetVirtualButton(joystick,
                                                 g_Supervisor.cfg.controllerMapping.bombButton,
                                                 isDown);
                }
            }
        } break;
        case SUPERVISOR_STATE_GAMEMANAGER:
        {
            if (!windowPress && event->key.keysym.sym == SDLK_AC_BACK)
            {
                SDL_JoystickSetVirtualButton(joystick,
                                             g_Supervisor.cfg.controllerMapping.menuButton,
                                             isDown);
            }
        } break;
        default:
            break;
    }
}

void TouchScreenManager::OnUpdate(void)
{
    for (int i = 0; i < TouchScreenSprite_Last; i++)
    {
        g_AnmManager->ExecuteScript(&this->vms[i]);
    }
}

void TouchScreenManager::OnDraw(void)
{
    ZunViewport originalViewport;
    ZunViewport newViewport;

    originalViewport.Get();
    
    newViewport.x = 0;
    newViewport.y = 0;
    newViewport.width = GAME_WINDOW_WIDTH;
    newViewport.height = GAME_WINDOW_HEIGHT;
    newViewport.minZ = 0.0f;
    newViewport.maxZ = 1.0f;
    
    newViewport.Set();
    
    g_glFuncTable.glDisable(GL_DEPTH_TEST);
    
    for (int i = 0; i < TouchScreenSprite_Last; i++)
    {
        AnmVm *vm = &this->vms[i];

        g_AnmManager->Draw(vm);
    }

    g_glFuncTable.glEnable(GL_DEPTH_TEST);

    originalViewport.Set();
}