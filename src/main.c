#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int main()
{
    // Unused crap
    STARTUPINFOW startupInfo;
    PROCESS_INFORMATION processInfo;
    
    // Initialize unused crap to defaults
    ZeroMemory(&startupInfo, sizeof(startupInfo)); // Would be garbage otherwise
    startupInfo.cb = sizeof(startupInfo); // Needs to know its own size because it could contain extended info
    ZeroMemory(&processInfo, sizeof(processInfo)); // Would be garbage otherwise

    // Change working dir
    SetCurrentDirectoryW(L"Launcher"); // Could instead use 'lpCurrentDirectory' arg of CreateProcess, but CD'ing is how the shortcut does it

    // Do the thing
    if(!CreateProcessW(L"Flashpoint.exe", NULL, NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInfo))
        return 1; // Mission failed, we'll get 'em next time

    // Cleanup unused crap handles (required)
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    // World hunger is solved
    return 0;
}
