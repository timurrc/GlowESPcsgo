#include "include.h"
using namespace offsets::netvars;
using namespace offsets::signatures;

BOOL WINAPI glowThread(HMODULE hModule)
{
    uintptr_t GameModule = (uintptr_t)GetModuleHandle("client.dll");

    for (; !GetAsyncKeyState(VK_END); std::this_thread::sleep_for(std::chrono::milliseconds(1)))
    {
        uintptr_t localPlayer = *(uintptr_t *)(GameModule + dwLocalPlayer);
        uintptr_t GlowObjectManager = *(uintptr_t *)(GameModule + dwGlowObjectManager);
        uintptr_t EntityList = *(uintptr_t *)(GameModule + dwEntityList);

        if (localPlayer == NULL)
            continue;

        uint32_t localTeam = *(uint32_t *)(localPlayer + m_iTeamNum);

        for (int x = 0; x < 32; x++)
        {
            uintptr_t Enemy = *(uintptr_t *)(GameModule + dwEntityList + x * 0x10);
            if (Enemy == NULL)
                continue;

            uint32_t glowIndex = *(uint32_t *)(Enemy + m_iGlowIndex);
            uint32_t enemyTeam = *(uint32_t *)(Enemy + m_iTeamNum);

            if (enemyTeam != localTeam)
            {
                *(float *)(GlowObjectManager + glowIndex * 0x38 + 0x4) = 0.f;   // R
                *(float *)(GlowObjectManager + glowIndex * 0x38 + 0x8) = 1.f;   // G
                *(float *)(GlowObjectManager + glowIndex * 0x38 + 0xC) = 1.f;   // B
                *(float *)(GlowObjectManager + glowIndex * 0x38 + 0x10) = 1.7f; // A
            }
            *(bool *)(GlowObjectManager + glowIndex * 0x38 + 0x24) = true;
            *(bool *)(GlowObjectManager + glowIndex * 0x38 + 0x25) = false;
        }
    }
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, 0, (LPTHREAD_START_ROUTINE)glowThread, 0, 0);

    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
