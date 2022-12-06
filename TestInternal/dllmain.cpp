#include "pch.h"
#include "mem.h"
#include "reclass.h"
#include<iostream>

// 32c team
BYTE* damageFunctionPointerAsm;
uintptr_t jumpBackOneShot;
uintptr_t jumpBackNoDamage;
uintptr_t jumpBackALaser;

struct Offsets
{
    const uintptr_t enemyCount = 0x110D98;
    const uintptr_t damage = 0x608B0;
    const uintptr_t console = 0x6b060;
    const uintptr_t localPlayer = 0x109B74;
    const uintptr_t enemyEntity = 0x10f4f8;
    const uintptr_t crosshair = 0x19f074;
} Offset;

void __declspec(naked) OneShotAsm()
{
    __asm
    {
        _emit 0x81; // cmp [esi+0x225], 'Kaanium'
        _emit 0xbe;
        _emit 0x25;
        _emit 0x02;
        _emit 0x00;
        _emit 0x00;
        _emit 0x4b;
        _emit 0x61;
        _emit 0x61;
        _emit 0x6e;
        jne kill
            cmp dword ptr[esi + 0xF8], 0x00
            jmp[jumpBackOneShot]
            kill :
        cmp esi, esi
            jmp[jumpBackOneShot]
    }
}

void __declspec(naked) AssaultLaser()
{
    __asm
    {
        _emit 0x81; // cmp [edx+0xad], 'Kaanium'
        _emit 0xba;
        _emit 0xad;
        _emit 0x00;
        _emit 0x00;
        _emit 0x00;
        _emit 0x4b;
        _emit 0x61;
        _emit 0x61;
        _emit 0x6e;
        je pass
            mov[edx], ecx
            pass :
        mov esi, [esi + 0x14]
            jmp[jumpBackALaser]
    }
}

void __declspec(naked) NoDamageAsm()
{
    __asm
    {
        _emit 0x81;
        _emit 0xbe;
        _emit 0x25;
        _emit 0x02;
        _emit 0x00;
        _emit 0x00;
        _emit 0x4b;
        _emit 0x61;
        _emit 0x61;
        _emit 0x6e;
        jne dont
            call damageFunctionPointerAsm
            dont :
        jmp[jumpBackNoDamage]
    }
}

typedef char* (__cdecl* _Print)(char* sFormat, ...);
_Print PrintConsole;

//typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);
//
//twglSwapBuffers owglSwapBuffers;
//
//BOOL __stdcall hkwglSwapBuffers(HDC hDc)
//{
//	if (GetAsyncKeyState(VK_END) & 1)
//	{
//		mem::Patch((BYTE*)(moduleBase + 0x60B64), (BYTE*)"\xE8\x47\xFD\xFF\xFF", 5);
//		mem::Patch((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x0E", 2);
//		mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
//		mem::Patch((BYTE*)(moduleBase + 0x26bd9), (BYTE*)"\x83\xBE\xF8\x00\x00\x00\x00", 7);
//	}
//
//	if (GetAsyncKeyState(VK_NUMPAD1) & 1)
//	{
//		jmpBackAddy2 = moduleBase + 0x60B64 + 5;
//
//		bHealth = !bHealth;
//
//		if (bHealth)
//		{
//			//std::cout << "No Damage Activated\n";
//			mem::Hook((void*)(moduleBase + 0x60B64), NoDamageAsm, 5);
//		}
//		else
//		{
//			//std::cout << "No Damage Disabled\n";
//			mem::Patch((BYTE*)(moduleBase + 0x60B64), (BYTE*)"\xE8\x47\xFD\xFF\xFF", 5);
//		}
//	}
//
//	if (GetAsyncKeyState(VK_NUMPAD2) & 1)
//	{
//		bAmmo = !bAmmo;
//
//		if (bAmmo)
//		{
//			//std::cout << "Unlimited Ammo Activated\n";
//			mem::Nop((BYTE*)(moduleBase + 0x637E9), 2);
//		}
//
//		else
//		{
//			//std::cout << "Unlimited Ammo Disabled\n";
//			mem::Patch((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x0E", 2);
//		}
//	}
//
//	//no recoil NOP
//	if (GetAsyncKeyState(VK_NUMPAD3) & 1)
//	{
//		bRecoil = !bRecoil;
//
//		if (bRecoil)
//		{
//			//std::cout << "No Recoil Activated\n";
//			mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
//		}
//
//		else
//		{
//			//std::cout << "No Recoil Disabled\n";
//			mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
//		}
//	}
//
//	if (GetAsyncKeyState(VK_NUMPAD4) & 1)
//	{
//		jmpBackAddy = moduleBase + 0x26BD9 + 7;
//		bOneShot = !bOneShot;
//
//		if (bOneShot)
//		{
//			//std::cout << "One Shot Kill Activated\n";
//			mem::Hook((void*)(moduleBase + 0x26BD9), OneShotAsm, 7);
//		}
//
//		else
//		{
//			//std::cout << "One Shot Kill Disabled\n";
//			mem::Patch((BYTE*)(moduleBase + 0x26bd9), (BYTE*)"\x83\xBE\xF8\x00\x00\x00\x00", 7);
//		}
//	}
//
//	if (GetAsyncKeyState(VK_NUMPAD5) & 1)
//	{
//		bAutoAim = !bAutoAim;
//		//if (bAutoAim)
//		//	std::cout << "Auto Aim Activated\n";
//
//		//else
//		//	std::cout << "Auto Aim Disabled\n";
//	}
//
//	if (GetAsyncKeyState(VK_NUMPAD6) & 1)
//	{
//		bWallHack = !bWallHack;
//		//if (bWallHack)
//		//	std::cout << "Wall Hack Activated\n";
//		//else
//		//	std::cout << "Wall Hack Disabled\n";
//	}
//
//	if ((bAutoAim) && GetAsyncKeyState(VK_LBUTTON))
//	{
//		if (*(int*)(moduleBase + 0x110D98))
//			index = GetClosestAliveEnemy(localPlayer->headPosition, *(int*)(moduleBase + 0x110D98), enemy);
//
//		if (index != -1 && !(enemy->botPtr[index]->dead))
//			localPlayer->mousePosition = CalculateDegrees(localPlayer->bodyPosition, enemy->botPtr[index]->bodyPosition);
//	}
//
//	if (bWallHack)
//	{
//		for (int i = 0, playerCount = *(int*)(moduleBase + 0x110D98); i < playerCount; ++i)
//		{
//			if (enemy->botPtr[i]->dead) continue;
//
//			Vector3 headTarget = CalculateDegrees(localPlayer->bodyPosition, enemy->botPtr[i]->headPosition);
//
//			if (abs(headTarget.y - localPlayer->mousePosition.y) < 60)
//			{
//				Vector3 footTarget = CalculateDegrees(localPlayer->bodyPosition, enemy->botPtr[i]->bodyPosition);
//				drawRect(localPlayer->mousePosition, footTarget, headTarget);
//			}
//		}
//	}
//	return owglSwapBuffers(hDc);
//}

DWORD WINAPI HackThread(HMODULE hModule, UINT msg)
{
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);

    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);
    damageFunctionPointerAsm = (BYTE*)(moduleBase + Offset.damage);
    char* Format = (char*)"%s:\f%d %s"; // console format
    PrintConsole = (_Print)(moduleBase + Offset.console);
    int index = -1;

    bool bHealth = false, bAmmo = false, bRecoil = false,
        bOneShot = false, bAutoAim = false, bWallHack = false,
        bLaser = false, bRadar = false, bTriggetBot = false,
        bNumLock = GetKeyState(VK_NUMLOCK);

    playerEnt* localPlayer = *(playerEnt**)(moduleBase
                                            + Offset.localPlayer);

    //owglSwapBuffers = (twglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
    //owglSwapBuffers = (twglSwapBuffers)mem::TrampHook32((BYTE*)owglSwapBuffers, (BYTE*)hkwglSwapBuffers, 5);

    PrintConsole(Format, "Cheat", 1, "Enabled");

    if (bNumLock)
        PrintConsole(Format, "Numlock", 1, "Enabled");
    else
        PrintConsole(Format, "Numlock", 1, "Disabled");

    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            mem::Patch((BYTE*)(moduleBase + 0x60B64),
                       (BYTE*)"\xE8\x47\xFD\xFF\xFF", 5);
            mem::Patch((BYTE*)(moduleBase + 0x637E9),
                       (BYTE*)"\xFF\x0E", 2);
            mem::Patch((BYTE*)(moduleBase + 0x63786),
                       (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
            mem::Patch((BYTE*)(moduleBase + 0x26bd9),
                       (BYTE*)"\x83\xBE\xF8\x00\x00\x00\x00", 7);
            mem::Patch((BYTE*)(moduleBase + 0x637E4),
                       (BYTE*)"\x89\x0a\x8b\x76\x14", 5);
            mem::Patch((BYTE*)(moduleBase + 0x969b),
                       (BYTE*)"\x39\x8f\x2c\x03\x00\x00", 6);

            PrintConsole(Format, "Cheat", 3, "Closing...");
            break;
        }

        if (GetAsyncKeyState(VK_NUMLOCK) & 1)
        {
            bNumLock = !bNumLock;

            if (bNumLock)
                PrintConsole(Format, "Numlock", 1, "Enabled");
            else
                PrintConsole(Format, "Numlock", 1, "Disabled");
        }

        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            jumpBackNoDamage = moduleBase + 0x60B64 + 5;

            bHealth = !bHealth;

            if (bHealth)
            {
                PrintConsole(Format, "No Damage", 1, "Enabled");
                //std::cout << "No Damage Activated\n";
                mem::Hook((void*)(moduleBase + 0x60B64), NoDamageAsm, 5);
            }
            else
            {
                PrintConsole(Format, "No Damage", 2, "Disabled");
                //std::cout << "No Damage Disabled\n";
                mem::Patch((BYTE*)(moduleBase + 0x60B64),
                           (BYTE*)"\xE8\x47\xFD\xFF\xFF", 5);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAmmo = !bAmmo;

            if (bAmmo)
            {
                PrintConsole(Format, "Unlimited Ammo", 1, "Enabled");
                //std::cout << "Unlimited Ammo Activated\n";
                mem::Nop((BYTE*)(moduleBase + 0x637E9), 2);
            }

            else
            {
                PrintConsole(Format, "Unlimited Ammo", 2, "Disabled");
                //std::cout << "Unlimited Ammo Disabled\n";
                mem::Patch((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x0E", 2);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bRecoil = !bRecoil;

            if (bRecoil)
            {
                PrintConsole(Format, "No Recoil", 1, "Enabled");
                //std::cout << "No Recoil Activated\n";
                mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
            }

            else
            {
                PrintConsole(Format, "No Recoil", 2, "Disabled");
                //std::cout << "No Recoil Disabled\n";
                mem::Patch((BYTE*)(moduleBase + 0x63786),
                           (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        {
            jumpBackOneShot = moduleBase + 0x26BD9 + 7;
            bOneShot = !bOneShot;

            if (bOneShot)
            {
                PrintConsole(Format, "One Shot Kill", 1, "Enabled");
                //std::cout << "One Shot Kill Activated\n";
                mem::Hook((void*)(moduleBase + 0x26BD9), OneShotAsm, 7);
            }

            else
            {
                PrintConsole(Format, "One Shot Kill", 2, "Disabled");
                //std::cout << "One Shot Kill Disabled\n";
                mem::Patch((BYTE*)(moduleBase + 0x26bd9),
                           (BYTE*)"\x83\xBE\xF8\x00\x00\x00\x00", 7);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD5) & 1)
        {
            bAutoAim = !bAutoAim;
            if (bAutoAim)
                //std::cout << "Auto Aim Activated\n";
                PrintConsole(Format, "Auto Aim", 1, "Enabled");

            else
                //std::cout << "Auto Aim Disabled\n";
                PrintConsole(Format, "Auto Aim", 2, "Disabled");
        }

        if (GetAsyncKeyState(VK_NUMPAD6) & 1)
        {
            bWallHack = !bWallHack;
            if (bWallHack)
                //std::cout << "Wall Hack Activated\n";
                PrintConsole(Format, "Wall Hack", 1, "Enabled");
            else
                //std::cout << "Wall Hack Disabled\n";
                PrintConsole(Format, "Wall Hack", 2, "Disabled");
        }

        if (GetAsyncKeyState(VK_NUMPAD7) & 1)
        {
            jumpBackALaser = moduleBase + 0x637E4 + 5;
            bLaser = !bLaser;

            if (bLaser)
            {
                PrintConsole(Format, "Laser", 1, "Enabled");
                //std::cout << "Laser Activated\n";
                mem::Hook((void*)(moduleBase + 0x637E4), AssaultLaser, 5);
            }

            else
            {
                PrintConsole(Format, "Laser", 2, "Disabled");
                //std::cout << "Laser Disabled\n";
                mem::Patch((BYTE*)(moduleBase + 0x637E4),
                           (BYTE*)"\x89\x0a\x8b\x76\x14", 5);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD8) & 1)
        {
            bRadar = !bRadar;

            if (bRadar)
            {
                PrintConsole(Format, "Radar", 1, "Enabled");
                mem::Nop((BYTE*)(moduleBase + 0x969b), 6);
            }

            else
            {
                PrintConsole(Format, "Radar", 2, "Disabled");
                mem::Patch((BYTE*)(moduleBase + 0x969b),
                           (BYTE*)"\x39\x8f\x2c\x03\x00\x00", 6);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD9) & 1)
        {
            bTriggetBot = !bTriggetBot;

            if (bTriggetBot)
            {
                PrintConsole(Format, "Trigget Bot", 1, "Enabled");
            }

            else
            {
                PrintConsole(Format, "Trigget Bot", 2, "Disabled");
            }
        }

        entityList* enemy = *(entityList**)(moduleBase + Offset.enemyEntity);

        if (bTriggetBot)
        {
            int crosshair = *(int*)(Offset.crosshair);

            if (CheckCrosshair(*(int*)(moduleBase + Offset.enemyCount),
                               enemy, crosshair, (moduleBase + Offset.enemyEntity)))
            {
                localPlayer->Firing = true;
            }

            else
            {
                localPlayer->Firing = false;
            }
        }

        if ((bAutoAim) && GetAsyncKeyState(VK_LBUTTON))
        {
            if (*(int*)(moduleBase + Offset.enemyCount))
                index = GetClosestAliveEnemy(localPlayer->headPosition,
                                             *(int*)(moduleBase + Offset.enemyCount), enemy);

            if (index != -1 && !(enemy->botPtr[index]->dead))
                localPlayer->mousePosition = CalculateDegrees(localPlayer->bodyPosition,
                                                              enemy->botPtr[index]->bodyPosition);
        }

        if (bWallHack)
        {
            std::vector<Vector3> footBuffer;
            std::vector<Vector3> headBuffer;
            std::vector<Vector3> mouseBuffer;
            for (int i = 0, playerCount = *(int*)(moduleBase + Offset.enemyCount)
                 ; i < playerCount; ++i)
            {
                if (enemy->botPtr[i]->dead) continue;

                Vector3 headTarget = CalculateDegrees(localPlayer->bodyPosition,
                                                      enemy->botPtr[i]->headPosition);
                Vector3 mouse = localPlayer->mousePosition;

                if (abs(localPlayer->mousePosition.x - headTarget.x) > 45)
                {
                    if (mouse.x > headTarget.x)
                        headTarget.x += 360;
                    else
                        mouse.x += 360;
                }

                if (abs(headTarget.y - localPlayer->mousePosition.y) < 60
                    && abs(mouse.x - headTarget.x) < 45)
                {
                    footBuffer.push_back(CalculateDegrees(localPlayer->bodyPosition,
                                                          enemy->botPtr[i]->bodyPosition));
                    headBuffer.push_back(headTarget);
                    mouseBuffer.push_back(mouse);
                }
            }
            drawRect(mouseBuffer, footBuffer, headBuffer);
        }
        Sleep(1);
    }
    FreeConsole();
    fclose(fDummy);
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread,
                                 hModule, 0, nullptr));

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}