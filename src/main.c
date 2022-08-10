#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <sys/types.h>
#include <sys/stat.h>

BOOL currentDirectory(LPWSTR* cdBuffer)
{
    // Determine buffer size
    DWORD bufferSize = GetCurrentDirectoryW(0, NULL);
    if(bufferSize == 0)
        return FALSE;

    // Allocate buffer
    *cdBuffer = malloc(bufferSize * sizeof(WCHAR));

    // Get CD
    DWORD retrievedChars = GetCurrentDirectoryW(bufferSize, *cdBuffer);
    BOOL success = retrievedChars == bufferSize - 1; // Successfully invocation returns char length minus null terminator

    if(!success)
        free(*cdBuffer);

    return success;
}

void concatStrings(LPCWSTR a, LPCWSTR b, LPWSTR* new)
{
    // Get total size
    size_t aCount = wcslen(a);
    size_t aByteSize = aCount * sizeof(WCHAR);
    size_t bCount = wcslen(b);
    size_t bByteSize = bCount * sizeof(WCHAR);
    size_t bufferCount = aCount + bCount + sizeof(WCHAR); // Account for null term.
    size_t bufferByteSize = bufferCount * sizeof(WCHAR);

    // Allocate buffer
    *new = malloc(bufferByteSize);

    // Copy a contents
    memcpy_s(*new, bufferByteSize, a, aByteSize);

    // Copy b contents
    LPWSTR bPortionStart = *new + aCount;
    memcpy_s(bPortionStart, bufferByteSize - aByteSize, b, bByteSize);

    // Set null terminators
    LPWSTR nullStart = *new + bufferCount - sizeof(WCHAR);
    memcpy_s(nullStart, sizeof(WCHAR), L"\0", sizeof(WCHAR));
}

int main()
{
    // Flashpoint Info
    LPCWSTR launcherDir = L"\\Launcher\\";
    LPCWSTR launcherExe = L"Flashpoint.exe";
    STARTUPINFOW startupInfo = {0}; // Initialize members to 0
    startupInfo.cb = sizeof(startupInfo); // Required by API

    // Spawned process info (needed for handle cleanup)
    PROCESS_INFORMATION processInfo = {0}; // Initialize members to 0

    // Determine launcher directory
    LPWSTR cd = NULL;
    if(!currentDirectory(&cd))
        return 1;

    LPWSTR absoluteLauncherDir = NULL;
    LPWSTR absoluteLauncherPath = NULL;
    concatStrings(cd, launcherDir, &absoluteLauncherDir);
    concatStrings(absoluteLauncherDir, launcherExe, &absoluteLauncherPath);

    // Do the thing
    BOOL started = CreateProcessW(absoluteLauncherPath, NULL, NULL, NULL, FALSE, 0, NULL, absoluteLauncherDir, &startupInfo, &processInfo);

    // Cleanup path memory
    free(cd);
    free(absoluteLauncherDir);
    free(absoluteLauncherPath);

    if(started)
    {
        // Cleanup unused crap handles (required)
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }
    else
        return 2; // Mission failed, we'll get 'em next time

    // World hunger is solved
    return 0;
}
