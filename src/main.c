#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    // Flashpoint Info
    LPCWSTR launcherDir = L"Launcher";
    LPCWSTR launcherExe = L"Flashpoint.exe";
    STARTUPINFOW startupInfo;

    // Spawned process info (needed for handle cleanup)
    PROCESS_INFORMATION processInfo;
    
    // Initialize unused crap to defaults
    ZeroMemory(&startupInfo, sizeof(startupInfo)); // Would be garbage otherwise
    startupInfo.cb = sizeof(startupInfo); // Needs to know its own size because it could contain extended info
    ZeroMemory(&processInfo, sizeof(processInfo)); // Would be garbage otherwise

    // Change working dir
    if(!SetCurrentDirectoryW(launcherDir)) // Could instead use 'lpCurrentDirectory' arg of CreateProcess, but CD'ing is how the shortcut does it
        return 1; // Not FP

    // Do the thing
    if(!CreateProcessW(launcherExe, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo))
        return 2; // Mission failed, we'll get 'em next time

    // Cleanup unused crap handles (required)
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    // World hunger is solved
    return 0;
}
