// dllmain.cpp : Defines the entry point for the DLL application.
#include "includes.hxx"

void __stdcall mainThread(HMODULE h_module) noexcept
{
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);

    csgo::interfaces::init();
    gensokyo::gui::renderer::init();
    gensokyo::gui::init();
    gensokyo::hooks::init();

    gensokyo::util::logger::log(GNSKY_INFO, "gensokyo inject OwO");
    
    while (!GetAsyncKeyState(VK_END)) Sleep(100);

    csgo::interfaces::InputSystem->EnableInput(true);
    gensokyo::hooks::uninject();

    fclose((_iobuf*)stdout);
    FreeConsole();

    FreeLibraryAndExitThread(h_module, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)mainThread, hModule, 0, nullptr);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

