#include "execinstall.h"
#include <sysinfo.h>
#include <QApplication>

#include "7zFile.h"
#include "7z.h"
#include "7zAlloc.h"
#include "7zBuf.h"
#include "7zCrc.h"
#include "7zFile.h"
#include "CpuArch.h"
#include "7zVersion.h"
#define kInputBufSize ((size_t)1 << 18)
#define kBufferSize (1 << 15)
#define kSignatureSearchLimit 100000000//(1 << 22)

execInstall::execInstall(QObject* parent) :
    QThread(parent)
{
}

void execInstall::run() {
    //文件释放完成
    if (releaseArchive(InstallInfo::InstallPath)) {
        emit signalNowFile("即将完成安装......");
        addVCRunTime();
        addSysInfo();
        emit signalProgress(100);
        emit signalDone();
    }
}

bool findSignature(CSzFile *stream, UInt64 *resPos)
{
    Byte buf[kBufferSize];
    size_t numPrevBytes = 0;
    *resPos = 0;
    for (;;) {
        size_t processed, pos;
        if (*resPos > kSignatureSearchLimit)
            return False;
        processed = kBufferSize - numPrevBytes;
        if (File_Read(stream, buf + numPrevBytes, &processed) != 0)
            return False;
        processed += numPrevBytes;
        if (processed < k7zStartHeaderSize ||
            (processed == k7zStartHeaderSize && numPrevBytes != 0))
            return False;
        processed -= k7zStartHeaderSize;
        for (pos = 0; pos <= processed; pos++)
        {
            for (; pos <= processed && buf[pos] != '7'; pos++);
            if (pos > processed)
                break;
            if (memcmp(buf + pos, k7zSignature, k7zSignatureSize) == 0)
                if (CrcCalc(buf + pos + 12, 20) == GetUi32(buf + pos + 8))
                {
                    *resPos += pos;
                    return True;
                }
        }
        *resPos += processed;
        numPrevBytes = k7zStartHeaderSize;
        memmove(buf, buf + processed, k7zStartHeaderSize);
    }
}

bool execInstall::releaseArchive(const QString& target_path)
{
    WCHAR path[MAX_PATH] = {0};

    QString error_msg = "ERROR";
    SRes res = SZ_OK;
    CFileInStream archiveStream;
    CLookToRead2 lookStream;
    ISzAlloc allocImp;
    ISzAlloc allocTempImp;
    CSzArEx db;
    QString sfx_path = QApplication::applicationFilePath();

    CrcGenerateTable();

    allocImp.Alloc = SzAlloc;
    allocImp.Free = SzFree;

    allocTempImp.Alloc = SzAllocTemp;
    allocTempImp.Free = SzFreeTemp;

    FileInStream_CreateVTable(&archiveStream);
    LookToRead2_CreateVTable(&lookStream, False);
    lookStream.buf = NULL;

    if (InFile_OpenW(&archiveStream.file,(const WCHAR*)sfx_path.utf16()) != 0) {
        error_msg = "can not open input file";
        res = SZ_ERROR_FAIL;
    } else {
        UInt64 pos = 0;
        if (!findSignature(&archiveStream.file, &pos))
            res = SZ_ERROR_FAIL;
        else if (File_Seek(&archiveStream.file, (Int64 *)&pos, SZ_SEEK_SET) != 0)
            res = SZ_ERROR_FAIL;
        if (res != 0)
            error_msg = "Can't find 7z archive";
    }
    if (res == SZ_OK) {
        lookStream.buf = (Byte *)ISzAlloc_Alloc(&allocImp, kInputBufSize);
        if (!lookStream.buf) {
            res = SZ_ERROR_MEM;
        } else {
            lookStream.bufSize = kInputBufSize;
            lookStream.realStream = &archiveStream.vt;
            LookToRead2_Init(&lookStream);
        }
    }

    SzArEx_Init(&db);

    if (res == SZ_OK) {
        res = SzArEx_Open(&db, &lookStream.vt, &allocImp, &allocTempImp);
    }

    if (res == SZ_OK) {
        UInt32 i;
        UInt32 blockIndex = 0xFFFFFFFF; /* it can have any value before first call (if outBuffer = 0) */
        Byte *outBuffer = 0; /* it must be 0 before first call for each new archive. */
        size_t outBufferSize = 0;  /* it can have any value before first call (if outBuffer = 0) */

        int last_progress = -1;
        for (i = 0; i < db.NumFiles; i++) {
            int ssss = std::ceil((i*100+100)/db.NumFiles);
            if (ssss != last_progress) {
                last_progress = ssss;
                emit signalProgress(ssss);
            }


            size_t offset = 0;
            size_t outSizeProcessed = 0;

            if (SzArEx_GetFileNameUtf16(&db, i, NULL) >= MAX_PATH) {
                res = SZ_ERROR_FAIL;
                break;
            }

            SzArEx_GetFileNameUtf16(&db, i, (UInt16 *)path);
            {
                res = SzArEx_Extract(&db, &lookStream.vt, i,
                                     &blockIndex, &outBuffer, &outBufferSize,
                                     &offset, &outSizeProcessed,
                                     &allocImp, &allocTempImp);
                if (res != SZ_OK)
                    break;
            }
            {
                QString tmp_path = target_path+"/"+QString::fromWCharArray(path);
                CSzFile outFile;
                size_t processedSize;

                if (SzArEx_IsDir(&db, i)) {
                    QDir().mkpath(tmp_path);
                    continue;
                }
                if (OutFile_OpenW(&outFile, (WCHAR*)tmp_path.utf16())) {
                    error_msg = "Can't open output file";
                    res = SZ_ERROR_FAIL;
                    break;
                }

                processedSize = outSizeProcessed;
                if (File_Write(&outFile, outBuffer + offset, &processedSize) != 0 || processedSize != outSizeProcessed) {
                    error_msg = "Can't write output file";
                    res = SZ_ERROR_FAIL;
                }

                if (SzBitWithVals_Check(&db.MTime, i)) {
                    const CNtfsFileTime *t = db.MTime.Vals + i;
                    FILETIME mTime;
                    mTime.dwLowDateTime = t->Low;
                    mTime.dwHighDateTime = t->High;
                    SetFileTime(outFile.handle, NULL, NULL, &mTime);
                }

                {
                    SRes res2 = File_Close(&outFile);
                    if (res != SZ_OK)
                        break;
                    if (res2 != SZ_OK) {
                        res = res2;
                        break;
                    }
                }
                if (SzBitWithVals_Check(&db.Attribs, i))
                    SetFileAttributesW(path, db.Attribs.Vals[i]);
            }
        }
    }
    if (res != SZ_OK) {
        if (res == SZ_ERROR_UNSUPPORTED) {
            emit signalError(res,"decoder doesn't support this archive");
        }
        else if (res == SZ_ERROR_MEM) {
            emit signalError(res,"can not allocate memory");
        }
        else if (res == SZ_ERROR_CRC) {
            emit signalError(res,"CRC error");
        }
        else {
            emit signalError(res,error_msg);
        }
        return false;
    }
    SzArEx_Free(&db, &allocImp);
    ISzAlloc_Free(&allocImp, lookStream.buf);
    File_Close(&archiveStream.file);
    return true;
}
//安装运行库
void execInstall::addVCRunTime() {
    QStringList vcrt;

    if (InstallInfo::appIs64Bit()) {
        vcrt.append("vcredist_x64_2010.exe");
    } else {
        vcrt.append("vcredist_x86_2010.exe");
    }

    for (int i = 0; i < vcrt.size(); ++i) {
        QStringList args;
        args << "/q" << "/norestart";
        QProcess::execute(InstallInfo::InstallPath + "/" + vcrt.at(i), args);
    }
    //win7安装补丁
//    if (QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS7) {
//        if (sysIs64Bit()) {
//            QStringList args;
//            args << InstallInfo::InstallPath + "/Windows6.1-KB2999226-x64.msu" << "/quiet" << "/norestart";
//            QProcess::execute(qt_create_commandline("wusa.exe", args));
//        } else {
//            QStringList args;
//            args << InstallInfo::InstallPath + "/Windows6.1-KB2999226-x86.msu" << "/quiet" << "/norestart";
//            QProcess::execute(qt_create_commandline("wusa.exe", args));
//        }
//    }
    emit signalProgress(95);
}
//添加系统信息
void execInstall::addSysInfo() {
    //添加桌面链接
    if (!QFile::link(InstallInfo::InstallPath + "/XXLive.exe",
                     InstallInfo::DesktopPath + "/XX直播助手.lnk")) {

    }
    //添加开始菜单链接
    QDir d;
    d.mkdir(InstallInfo::StartMenuPath + "/XXLive");
    if (!QFile::link(InstallInfo::InstallPath + "/XXLive.exe",
                     InstallInfo::StartMenuPath + "/XXLive/XX直播助手.lnk")) {

    }
    if (!QFile::link(InstallInfo::InstallPath + "/XXUninstall.exe",
                     InstallInfo::StartMenuPath + "/XXLive/卸载.lnk")) {

    }
    //注册表添加卸载信息
    if (InstallInfo::sysIs64Bit()) {
        QSettings qkset("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\XXLive",
                        QSettings::NativeFormat);
        QString path = InstallInfo::InstallPath;
        path.replace("/", "\\");
        qkset.setValue("DisplayName", "XX直播助手" + InstallInfo::version);
        qkset.setValue("UninstallString", path + "\\XXUninstall.exe");
        qkset.setValue("Publisher", "XX网络技术有限公司");
        qkset.setValue("DisplayIcon", path + "\\XXLive.exe");
        qkset.setValue("DisplayVersion", InstallInfo::version);
    } else {
        QSettings qkset32("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\XXLive",
                          QSettings::NativeFormat);
        QString path = InstallInfo::InstallPath;
        path.replace("/", "\\");
        qkset32.setValue("DisplayName", "XX直播助手 " + InstallInfo::version);
        qkset32.setValue("UninstallString", path + "\\XXUninstall.exe");
        qkset32.setValue("Publisher", "XX网络技术有限公司");
        qkset32.setValue("DisplayIcon", path + "\\XXLive.exe");
        qkset32.setValue("DisplayVersion", InstallInfo::version);
    }
}

// bool execInstall::load7zData() {
//     HRSRC hres = FindResource(0, (LPCWSTR)2333, (LPCWSTR)2444);
//     if (hres == 0)
//         return false;
//     HGLOBAL hg = LoadResource(0, hres);
//     if (hg == 0)
//         return false;
//     ar7z_size_ = SizeofResource(0, hres);
//     ar7z_data_ = LockResource(hg);
//     if (ar7z_data_ == nullptr)
//         return false;
//     return true;
// }
