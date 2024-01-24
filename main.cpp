#include "mainwin.h"
#include <QApplication>
#include <QThread>
#include <QTranslator>
#include <QFont>
#include <QDebug>
#include "sysinfo.h"
#include <QSettings>
#include <QProcess>
//#ifdef Q_OS_WIN
//#include <dbghelp.h>
//#include <windows.h>
//#pragma comment(lib,"Psapi.lib")
//long __stdcall callback_Exception(_EXCEPTION_POINTERS *ExceptionInfo);
//#endif
BOOL EnableDebugPrivilege(BOOL bEnableDebugPrivilege)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if(!::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        return FALSE;
    }

    if(!::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
    {
        ::CloseHandle(hToken);
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if(bEnableDebugPrivilege)
    {
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    }
    else
    {
        tp.Privileges[0].Attributes = 0;
    }

    if(!::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL))
    {
        ::CloseHandle(hToken);
        return FALSE;
    }

    ::CloseHandle(hToken);

    if(::GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    {
        return FALSE;
    }
    return TRUE;
}
int main(int argc, char *argv[])
{
//#ifdef Q_OS_WIN
//    SetUnhandledExceptionFilter(callback_Exception);
//#endif
    EnableDebugPrivilege(true);
    QApplication a(argc, argv);
    QTranslator translator;
    if(translator.load(":/lng/zh-cn"))
    {
        a.installTranslator(&translator);
    }



    MainWin w;
    w.setWindowIcon(QIcon(":/img/img/logo.ico"));
    w.setWindowTitle("XX直播助手安装程序 版本:"+InstallInfo::version);
    w.show();

    return a.exec();
}
//#ifdef Q_OS_WIN
//long __stdcall callback_Exception(_EXCEPTION_POINTERS *ExceptionInfo)
//{
//崩溃内存转储
//#if defined(_MSC_VER) && _MSC_VER >= 1400
//    QString dmpfile = QDateTime::currentDateTime().toString("yyyyMMddHHmmss")+".dmp";
//    HANDLE hFile = CreateFileA(
//            dmpfile.toLocal8Bit().data(),
//            GENERIC_WRITE,
//            0,
//            NULL,
//            CREATE_ALWAYS,
//            FILE_ATTRIBUTE_NORMAL,
//            NULL);
//    if( INVALID_HANDLE_VALUE != hFile )
//    {
//        MINIDUMP_EXCEPTION_INFORMATION einfo;
//        einfo.ThreadId          = GetCurrentThreadId();
//        einfo.ExceptionPointers = ExceptionInfo;
//        einfo.ClientPointers    = FALSE;

//        MiniDumpWriteDump(
//            GetCurrentProcess(),
//            GetCurrentProcessId(),
//            hFile,
//            MiniDumpNormal,
//            &einfo,
//            NULL,
//            NULL);
//        CloseHandle(hFile);
//     }
//#endif
//    return EXCEPTION_EXECUTE_HANDLER;//表示已经处理了异常,可以结束了
//}
//#endif
