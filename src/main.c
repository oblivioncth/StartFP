#include <windows.h> 

int main()
{
    // Unused crap
    STARTUPINFOW dummySInfo;
    PROCESS_INFORMATION dummyPInfo;
    
    // Prepare unused crap
    ZeroMemory(&dummySInfo, sizeof(dummySInfo));
    dummySInfo.cb = sizeof(dummySInfo);

    // Change working dir
    SetCurrentDirectoryW(L"Launcher");

    // Do the thing
    CreateProcessW(L"Flashpoint.exe", NULL, NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &dummySInfo, &dummyPInfo);

    // Cleanup more crap
    CloseHandle(dummyPInfo.hProcess);
    CloseHandle(dummyPInfo.hThread);

    // World hunger is solved
    return 0;
}
