#-------------------------------------------------
#
# Project created by QtCreator 2017-10-10T18:00:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = setup
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    7z2201/C/7zAlloc.c \
    7z2201/C/7zArcIn.c \
    7z2201/C/7zBuf.c \
    7z2201/C/7zBuf2.c \
    7z2201/C/7zCrc.c \
    7z2201/C/7zCrcOpt.c \
    7z2201/C/7zDec.c \
    7z2201/C/7zFile.c \
    7z2201/C/7zStream.c \
    7z2201/C/Aes.c \
    7z2201/C/AesOpt.c \
    7z2201/C/Alloc.c \
    7z2201/C/Bcj2.c \
    7z2201/C/Bcj2Enc.c \
    7z2201/C/Blake2s.c \
    7z2201/C/Bra.c \
    7z2201/C/Bra86.c \
    7z2201/C/BraIA64.c \
    7z2201/C/BwtSort.c \
    7z2201/C/CpuArch.c \
    7z2201/C/Delta.c \
    7z2201/C/DllSecur.c \
    7z2201/C/HuffEnc.c \
    7z2201/C/LzFind.c \
    7z2201/C/LzFindMt.c \
    7z2201/C/LzFindOpt.c \
    7z2201/C/Lzma2Dec.c \
    7z2201/C/Lzma2DecMt.c \
    7z2201/C/Lzma2Enc.c \
    7z2201/C/Lzma86Dec.c \
    7z2201/C/Lzma86Enc.c \
    7z2201/C/LzmaDec.c \
    7z2201/C/LzmaEnc.c \
    7z2201/C/LzmaLib.c \
    7z2201/C/MtCoder.c \
    7z2201/C/MtDec.c \
    7z2201/C/Ppmd7.c \
    7z2201/C/Ppmd7Dec.c \
    7z2201/C/Ppmd7Enc.c \
    7z2201/C/Ppmd7aDec.c \
    7z2201/C/Ppmd8.c \
    7z2201/C/Ppmd8Dec.c \
    7z2201/C/Ppmd8Enc.c \
    7z2201/C/Sha1.c \
    7z2201/C/Sha1Opt.c \
    7z2201/C/Sha256.c \
    7z2201/C/Sha256Opt.c \
    7z2201/C/Sort.c \
    7z2201/C/Threads.c \
    7z2201/C/Xz.c \
    7z2201/C/XzCrc64.c \
    7z2201/C/XzCrc64Opt.c \
    7z2201/C/XzDec.c \
    7z2201/C/XzEnc.c \
    7z2201/C/XzIn.c \
    main.cpp \
    firstwidget.cpp \
    installinfo.cpp \
    QKmessagebox.cpp \
    mainwin.cpp \
    progresswidget.cpp \
    execinstall.cpp \
    finishedwidget.cpp \
    sysinfo.cpp

HEADERS += \
    7z2201/C/7z.h \
    7z2201/C/7zAlloc.h \
    7z2201/C/7zBuf.h \
    7z2201/C/7zCrc.h \
    7z2201/C/7zFile.h \
    7z2201/C/7zTypes.h \
    7z2201/C/7zVersion.h \
    7z2201/C/Aes.h \
    7z2201/C/Alloc.h \
    7z2201/C/Bcj2.h \
    7z2201/C/Blake2.h \
    7z2201/C/Bra.h \
    7z2201/C/BwtSort.h \
    7z2201/C/Compiler.h \
    7z2201/C/CpuArch.h \
    7z2201/C/Delta.h \
    7z2201/C/DllSecur.h \
    7z2201/C/HuffEnc.h \
    7z2201/C/LzFind.h \
    7z2201/C/LzFindMt.h \
    7z2201/C/LzHash.h \
    7z2201/C/Lzma2Dec.h \
    7z2201/C/Lzma2DecMt.h \
    7z2201/C/Lzma2Enc.h \
    7z2201/C/Lzma86.h \
    7z2201/C/LzmaDec.h \
    7z2201/C/LzmaEnc.h \
    7z2201/C/LzmaLib.h \
    7z2201/C/MtCoder.h \
    7z2201/C/MtDec.h \
    7z2201/C/Ppmd.h \
    7z2201/C/Ppmd7.h \
    7z2201/C/Ppmd8.h \
    7z2201/C/Precomp.h \
    7z2201/C/RotateDefs.h \
    7z2201/C/Sha1.h \
    7z2201/C/Sha256.h \
    7z2201/C/Sort.h \
    7z2201/C/Threads.h \
    7z2201/C/Xz.h \
    7z2201/C/XzCrc64.h \
    7z2201/C/XzEnc.h \
    firstwidget.h \
    installinfo.h \
    QKmessagebox.h \
    mainwin.h \
    progresswidget.h \
    execinstall.h \
    finishedwidget.h \
    sysinfo.h \
    resource.h


INCLUDEPATH += 7z2201/C
#    QMAKE_LFLAGS += /DEBUG /OPT:REF
#    QMAKE_CXXFLAGS += -Zi
CONFIG(release,release|debug){
   QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
}
QMAKE_CXXFLAGS += /utf-8

TRANSLATIONS += lng/zh-cn.ts

RESOURCES += \
    qkres.qrc
RC_FILE += Resource.rc
LIBS += -lUser32 -ldbghelp -lAdvapi32 -lOle32
