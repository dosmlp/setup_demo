#ifndef INSTALLINFO_H
#define INSTALLINFO_H
#include <QtCore>
#include <windows.h>
class InstallInfo
{

public:
    InstallInfo();
    static QString InstallPath;
    static QString DesktopPath;
    static QString StartMenuPath;
    static QString version;
    static bool appIs64Bit();
    static bool sysIs64Bit();

};

#endif // INSTALLINFO_H
