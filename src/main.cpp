#include <QDir>
#include <QProcess>

int main(int argc, char *argv[])
{
    // Unused crap
    QDir launcherDir("Launcher");
    QString launcherName("Flashpoint.exe");

    // Change working dir
    if(!launcherDir.exists()) // Could instead use 'lpCurrentDirectory' arg of CreateProcess, but CD'ing is how the shortcut does it
        return 1;
    if(QDir::setCurrent(launcherDir.path()))
        return 2;

    // Do the thing
    QProcess fpProcess;
    fpProcess.setProgram(launcherName);
    if(fpProcess.startDetached())
        return 3;

    // World hunger is solved
    return 0;
}
