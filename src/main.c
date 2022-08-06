#include <windows.h> 

int main()
{
    // Unused crap
    WCHAR* commandLine = L"";
    STARTUPINFO dummySInfo;
    PROCESS_INFORMATION dummyPInfo;
    
    // Prepare unused crap
    ZeroMemory(&dummySInfo, sizeof(dummySInfo));
    dummySInfo.cb = sizeof(dummySInfo);

    // Change working dir
    SetCurrentDirectory("Launcher");

    // Do the thing
    CreateProcess(L"Flashpoint.exe", commandLine, NULL, NULL, 0, NORMAL_PRIORITY_CLASS, NULL, NULL, &dummySInfo, &dummyPInfo);

    // Cleanup more crap
    CloseHandle(dummyPInfo.hProcess);
    CloseHandle(dummyPInfo.hThread);

    // World hunger is solved
    return 0;
}