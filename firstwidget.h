#ifndef FIRSTWIDGET_H
#define FIRSTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QDebug>
#include <Windows.h>
#include <sysinfo.h>
#include <Psapi.h>
#include <QLibrary>
#include <memory>
#include "installinfo.h"
typedef BOOL (CALLBACK *PENUMLOADED_MODULES_CALLBACK64)(
    PCSTR ModuleName,
    DWORD64 ModuleBase,
    ULONG ModuleSize,
    PVOID UserContext
    );
typedef BOOL (WINAPI *ENUMERATELOADEDMODULES64)(HANDLE process,
        PENUMLOADED_MODULES_CALLBACK64 enum_loaded_modules_callback,
        PVOID user_context);
typedef DWORD64 (WINAPI *SYMLOADMODULE64)(HANDLE process, HANDLE file,
        PSTR image_name, PSTR module_name, DWORD64 base_of_dll,
        DWORD size_of_dll);

class FirstWidget : public QWidget
{
    Q_OBJECT

public:
    FirstWidget(QWidget *parent = 0);
    ~FirstWidget();
    void beginInstall();
signals:
    void signalNext();
private:
    bool get_dbghelp_imports();
    bool checkdll();
    bool checkVersion();
    bool checkQK();
    void setUi();
    void initConeect();
    QString m_dir;
    QVBoxLayout* m_mainlayout;
    QHBoxLayout* m_pathlayout;

    QPushButton* m_btSetup;
    QLineEdit* m_lePath;
    QPushButton* m_btChoose;

    std::shared_ptr<QLibrary> m_dbg_lib;
    QStringList m_dll_list;
    ENUMERATELOADEDMODULES64 m_enumerate_load_modules64;

};

#endif // FIRSTWIDGET_H
