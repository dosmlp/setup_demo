#include "installinfo.h"

QString InstallInfo::InstallPath = QString();
QString InstallInfo::DesktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
QString InstallInfo::StartMenuPath = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
QString InstallInfo::version = "4.0.1.0";


InstallInfo::InstallInfo() {

}

bool InstallInfo::appIs64Bit() {
    void* p = nullptr;
    if (sizeof(p) == 4)
        return false;
    else
        return true;
}

bool InstallInfo::sysIs64Bit() {
    SYSTEM_INFO si;
    // Copy the hardware information to the SYSTEM_INFO structure.
    GetNativeSystemInfo(&si);
    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
        return true;
    } else {
        return false;
    }
}
