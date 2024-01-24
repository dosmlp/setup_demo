#include "firstwidget.h"
#include "QKmessagebox.h"
static BOOL CALLBACK enum_all_modules(PCSTR module_name, DWORD64 module_base,
                                      ULONG module_size, PVOID data) {
    qDebug() << QString::fromUtf16((const ushort*)module_name);
    return TRUE;
}

FirstWidget::FirstWidget(QWidget* parent)
    : QWidget(parent),
      m_dbg_lib(new QLibrary()) {

    setFont(QFont("微软雅黑"));
    setUi();
    initConeect();

    m_dll_list.append("QQAudioHook.dll");

}

FirstWidget::~FirstWidget() {

}

void FirstWidget::beginInstall() {
    if (!checkVersion())
        return;

    if (!checkQK())
        return;
}

bool FirstWidget::get_dbghelp_imports() {
    m_enumerate_load_modules64 = (ENUMERATELOADEDMODULES64)m_dbg_lib->resolve("EnumerateLoadedModulesW64");
    if (m_enumerate_load_modules64)
        return true;
    return false;
}

bool FirstWidget::checkdll() {
    m_dbg_lib->setFileName("dbghelp.dll");
    if (m_dbg_lib->load()) {
        get_dbghelp_imports();
    }

    ulong ids[512];
    HMODULE ms[1024];
    ulong nb_ids = 0;
    ulong nb_ms = 0;
    //获取系统中所有进程id
    if (EnumProcesses(ids, sizeof(ids), &nb_ids)) {
        for (int i = 0; i < (nb_ids / sizeof(ulong)); ++i) {
            HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, ids[i]);
            //获取指定进程中所有加载的模块
            if (EnumProcessModules(process, ms, sizeof(ms), &nb_ms)) {
                for (int n = 0; n < (nb_ms / sizeof(HMODULE)); ++n) {
                    wchar_t modulename[1024];
                    memset(modulename, 0, sizeof(modulename));
                    //获取指定进程指定模块的名称
                    ulong ns = GetModuleFileNameEx(process, ms[n], modulename, 1024);
                    if (ns > 0) {
                        QString mo = QString::fromUtf16((const ushort*)modulename, ns);
                        if (mo.contains("QKLive", Qt::CaseSensitive)) {
                            killProcess(ids[i], 1000);
                            qDebug() << "kill:" << ids[i];
                            break;
                        }
                    }

                }
            }
            CloseHandle(process);
        }
    }

    return false;
}

bool FirstWidget::checkVersion() {
    //系统版本检测
    // if (QSysInfo::windowsVersion() == QSysInfo::WV_2000 ||
    //     QSysInfo::windowsVersion() == QSysInfo::WV_XP ||
    //     QSysInfo::windowsVersion() == QSysInfo::WV_2003) {
    //     QKMessageBox::information(0, "提示",
    //                               "不支持的系统:windows xp、windows 2000、windows2003");
    //     return false;
    // }
    //助手版本检测
    QSettings qkset32("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\XXLive",
                      QSettings::NativeFormat);
    QString version = qkset32.value("DisplayVersion").toString();
    if (!version.isEmpty() && version.replace(".", "").toInt() < 2000) {
        QKMessageBox::information(0, "提示", "请先卸载旧版本，然后再安装新版助手");
        return false;
    }
    QString curver = InstallInfo::version;
    if (!version.isEmpty() && version.replace(".", "").toInt() >= curver.replace(".", "").toInt()) {
        if (QKMessageBox::information(0, "提示", "检测到已安装最新版本,是否继续安装？",
                                      QKMessageBox::Yes | QKMessageBox::No)
            == QKMessageBox::No) {
            return false;
        }
    }
    QString uninstallPath = qkset32.value("UninstallString").toString();
    qDebug() << uninstallPath;
    qDebug() << QProcess::execute(uninstallPath, QStringList("force"));
    return true;
}

bool FirstWidget::checkQK() {
    //卸载之前版本以及文件占用检测
    QSettings qkset32("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\XXLive",
                      QSettings::NativeFormat);
    QString uninstallPath = qkset32.value("UninstallString").toString();
    QString qkpath = QString(uninstallPath).replace("XXUninstall.exe", "");
    //checkdll();
    QProcess::execute(uninstallPath, QStringList("force"));

    InstallInfo::InstallPath = m_lePath->text().replace("\\", "/");
    QDir c;
    if (!c.mkpath(QFileInfo(InstallInfo::InstallPath).absoluteFilePath())) {
        QKMessageBox::information(0,
                                  "错误",
                                  "安装失败，无法创建安装目录");
        return false;
    }
    QDir dir(InstallInfo::InstallPath);
    if (dir.exists() && !InstallInfo::InstallPath.isEmpty()) {
        emit signalNext();

    } else {
        QKMessageBox::information(0,
                                  "提示",
                                  "所选目录不存在");
        return false;
    }
    return true;
}

void FirstWidget::setUi() {
    m_mainlayout = new QVBoxLayout(this);
    setLayout(m_mainlayout);
    m_pathlayout = new QHBoxLayout();

    //开始安装按钮
    m_btSetup = new QPushButton("立即安装", this);
    m_btSetup->setStyleSheet("QPushButton {"
                             "background-color: #FFD600;"
                             "border-radius: 4px;"
                             "padding: 6px;"
                             "font: 22px;}"
                             "QPushButton:hover{"
                             "background-color: #FFC100;}"
                            );
    m_btSetup->setFixedSize(180, 50);

    //目录选择按钮
    m_btChoose = new QPushButton("选择目录", this);
    m_btChoose->setStyleSheet("QPushButton {"
                              "background-color: #FFFFFF;"
                              "border: 1px solid #FFD600;"
                              "border-radius: 4px;"
                              "padding: 6px;"
                              "font: 18px;}"
                              "QPushButton:hover{"
                              "background-color: #FFC100;}"
                             );
    m_btChoose->setFixedSize(111, 44);

    m_lePath = new QLineEdit(this);
    m_lePath->setStyleSheet("background: #FFFFFF;"
                            "color:#BBBBBB;"
                            "border: 1px solid #D9D9D9;"
                            "border-radius: 3px;"
                            "font: 16px;");
    m_lePath->setFixedSize(399, 44);
    //m_lePath->setEnabled(false);

    if (InstallInfo::sysIs64Bit()) {
        if (InstallInfo::appIs64Bit()) {
            m_lePath->setText("C:\\Program Files\\XXLive");
        } else {
            m_lePath->setText("C:\\Program Files (x86)\\XXLive");
        }
    } else {
        if (!InstallInfo::appIs64Bit())
            m_lePath->setText("C:\\Program Files\\XXLive");
    }

    m_mainlayout->setAlignment(Qt::AlignCenter);
    m_mainlayout->addStretch(2);
    m_mainlayout->addWidget(m_btSetup, 0, Qt::AlignCenter);
    m_mainlayout->addStretch(1);
    m_mainlayout->addLayout(m_pathlayout);
    m_mainlayout->setContentsMargins(13, 0, 13, 13);
    m_mainlayout->setSpacing(67);

    m_pathlayout->addWidget(m_lePath);
    m_pathlayout->addWidget(m_btChoose);

}

void FirstWidget::initConeect() {
    connect(m_btChoose, &QPushButton::clicked, [this]() {
        m_dir = QFileDialog::getExistingDirectory();
        QString di = m_dir;
        if (!di.isEmpty()) {
            if (di.right(1) == "/") {
                m_lePath->setText(di.replace("/", "\\") + "XXLive");
            } else {
                m_lePath->setText(di.replace("/", "\\") + "\\XXLive");
            }

            InstallInfo::InstallPath = m_dir;
        }
    });

    connect(m_btSetup, &QPushButton::clicked, this, &FirstWidget::beginInstall);
}
