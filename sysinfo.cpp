#include "sysinfo.h"
#include <qt_windows.h>
#include <psapi.h>
#include <tlhelp32.h>
const int KDSYSINFO_PROCESS_QUERY_LIMITED_INFORMATION = 0x1000;

struct EnumWindowsProcParam
{
    QList<ProcessInfo> processes;
    QList<quint32> seenIDs;
};
bool CALLBACK TerminateAppEnum(HWND hwnd, LPARAM lParam)
{
   DWORD dwID;
   GetWindowThreadProcessId(hwnd, &dwID);

   if (dwID == (DWORD)lParam)
      PostMessage(hwnd, WM_CLOSE, 0, 0);
   return true;
}

bool killProcess(const ProcessInfo &process, int msecs)
{
    DWORD dwTimeout = msecs;
    if (msecs == -1)
        dwTimeout = INFINITE;

    // If we can't open the process with PROCESS_TERMINATE rights, then we give up immediately.
    HANDLE hProc = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, false, process.id);
    if (hProc == 0)
        return false;

    // TerminateAppEnum() posts WM_CLOSE to all windows whose PID matches your process's.
    EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM)process.id);

    // Wait on the handle. If it signals, great. If it times out, then kill it.
    bool returnValue = false;
    if (WaitForSingleObject(hProc, dwTimeout) != WAIT_OBJECT_0)
        returnValue = TerminateProcess(hProc, 0);

    CloseHandle(hProc);
    return returnValue;

}
typedef BOOL (WINAPI *QueryFullProcessImageNamePtr)(HANDLE, DWORD, char *, PDWORD);
typedef DWORD (WINAPI *GetProcessImageFileNamePtr)(HANDLE, char *, DWORD);

QList<ProcessInfo> runningProcesses()
{
    EnumWindowsProcParam param;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (!snapshot)
        return param.processes;

    QStringList deviceList;
    const DWORD bufferSize = 1024;
    char buffer[bufferSize + 1] = { 0 };

    // Qt6 does not support Win before 10
    bool winVerLessEqual5_2 = false;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    winVerLessEqual5_2 = QSysInfo::windowsVersion() <= QSysInfo::WV_5_2;
#endif
    if (winVerLessEqual5_2) {
        const DWORD size = GetLogicalDriveStringsA(bufferSize, buffer);
        deviceList = QString::fromLatin1(buffer, size).split(QLatin1Char(char(0)), Qt::SkipEmptyParts);
    }

    QLibrary kernel32(QLatin1String("Kernel32.dll"));
    kernel32.load();
    QueryFullProcessImageNamePtr pQueryFullProcessImageNamePtr = (QueryFullProcessImageNamePtr) kernel32
                                                                     .resolve("QueryFullProcessImageNameA");

    QLibrary psapi(QLatin1String("Psapi.dll"));
    psapi.load();
    GetProcessImageFileNamePtr pGetProcessImageFileNamePtr = (GetProcessImageFileNamePtr) psapi
                                                                 .resolve("GetProcessImageFileNameA");

    PROCESSENTRY32 processStruct;
    processStruct.dwSize = sizeof(PROCESSENTRY32);
    bool foundProcess = Process32First(snapshot, &processStruct);
    while (foundProcess) {
        HANDLE procHandle = OpenProcess(!winVerLessEqual5_2
                                            ? KDSYSINFO_PROCESS_QUERY_LIMITED_INFORMATION : PROCESS_QUERY_INFORMATION, false, processStruct
                                            .th32ProcessID);

        bool succ = false;
        QString executablePath;
        DWORD bufferSize = 1024;

        if (!winVerLessEqual5_2) {
            succ = pQueryFullProcessImageNamePtr(procHandle, 0, buffer, &bufferSize);
            executablePath = QString::fromLatin1(buffer);
        } else if (pGetProcessImageFileNamePtr) {
            succ = pGetProcessImageFileNamePtr(procHandle, buffer, bufferSize);
            executablePath = QString::fromLatin1(buffer);
            for (int i = 0; i < deviceList.count(); ++i) {
                executablePath.replace(QString::fromLatin1( "\\Device\\HarddiskVolume%1\\" ).arg(i + 1),
                                       deviceList.at(i));
            }
        }

        if (succ) {
            const quint32 pid = processStruct.th32ProcessID;
            param.seenIDs.append(pid);
            ProcessInfo info;
            info.id = pid;
            info.name = executablePath;
            param.processes.append(info);
        }

        CloseHandle(procHandle);
        foundProcess = Process32Next(snapshot, &processStruct);

    }
    if (snapshot)
        CloseHandle(snapshot);

    kernel32.unload();
    return param.processes;
}
bool sysIs64Bit()
{
    SYSTEM_INFO si;
    // Copy the hardware information to the SYSTEM_INFO structure.
    GetNativeSystemInfo(&si);
    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool killProcess(quint32 pid, int msecs)
{
    ProcessInfo info;
    info.id = pid;
    return killProcess(info,msecs);
}

