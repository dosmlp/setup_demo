#include "progresswidget.h"
#include <QApplication>
ProgressWidget::ProgressWidget(QWidget* parent) : QWidget(parent) {
    m_mainlayout = new QVBoxLayout(this);
    setLayout(m_mainlayout);

    //安装信息
    m_lbProgressInfo = new QLabel(this);
//    m_lbProgressInfo->setFixedSize(600,40);
    m_lbProgressInfo->setText("正在安装......");
    m_lbProgressInfo->setAlignment(Qt::AlignCenter);
    m_lbProgressInfo->setFont(QFont("微软雅黑", 5));
    m_lbProgressInfo->setStyleSheet("font-size: 16px;"
                                    "color: #999999;");
    //进度条
    m_ProgressBar = new QSlider(Qt::Horizontal, this);
    m_ProgressBar->setEnabled(false);
    m_ProgressBar->setRange(0, 100);
    m_ProgressBar->setValue(0);
    m_ProgressBar->setFixedHeight(35);
    m_ProgressBar->setStyleSheet("\
                     QSlider::groove:horizontal {\
                     border: 1px solid #FFD600;\
                     background: #D9D9D9;\
                     height: 4px;\
                     border-radius: 1px;\
                     padding-left:-1px;\
                     padding-right:-1px;\
                     }\
                     QSlider::sub-page:horizontal {\
                     background: qlineargradient(x1:0, y1:0, x2:0, y2:1, \
                         stop:0 #D9D9D9, stop:1 #D9D9D9);\
                     background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,\
                         stop: 0 #FFD600, stop: 1 #FFD600);\
                     border: 1px solid #FFD600;\
                     height: 10px;\
                     border-radius: 2px;\
                     }\
                     \
                     QSlider::add-page:horizontal {\
                     background: #D9D9D9;\
                     border: 0px solid #777;\
                     height: 10px;\
                     border-radius: 2px;\
                     }\
                     \
                     QSlider::handle:horizontal \
                     {\
                         background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,\
                         stop:0.6 #FFFFFF, stop:0.778409 #FFD600);\
                     \
                         width: 11px;\
                         margin-top: -3px;\
                         margin-bottom: -3px;\
                         border-radius: 5px;\
                     }\
                     \
                     QSlider::handle:horizontal:hover {\
                         background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.6 #FFFFFF,\
                         stop:0.778409 #FFD600);\
                     \
                         width: 11px;\
                         margin-top: -3px;\
                         margin-bottom: -3px;\
                         border-radius: 5px;\
                     }");

    //进度数字
    m_lbProgressNum = new QLabel("0%", this);
    m_lbProgressNum->setAlignment(Qt::AlignCenter);
    m_lbProgressNum->setStyleSheet("font-size: 40px;"
                                   "color: #999999;");
    m_lbProgressNum->setFont(QFont("微软雅黑"));


    m_mainlayout->addStretch();
    m_mainlayout->addWidget(m_lbProgressInfo);
    m_mainlayout->addWidget(m_ProgressBar);
    m_mainlayout->addWidget(m_lbProgressNum);
    m_mainlayout->addStretch();
    m_mainlayout->setContentsMargins(75, 0, 75, 77);

    m_execinstall = new execInstall(this);

    connect(m_execinstall, &execInstall::signalDone, this, &ProgressWidget::OnDone, Qt::QueuedConnection);
    connect(m_execinstall, &execInstall::signalNowFile, this, &ProgressWidget::OnNowFile, Qt::QueuedConnection);
    connect(m_execinstall, &execInstall::signalProgress, this, &ProgressWidget::OnProgress, Qt::QueuedConnection);
    connect(m_execinstall, &execInstall::signalError, this, &ProgressWidget::OnError, Qt::QueuedConnection);

}

void ProgressWidget::startInstall() {
    m_execinstall->start();
}

void ProgressWidget::OnError(int code, QString msg) {
    QKMessageBox::information(0,
                              "错误",
                              "Code("+QString::number(code)+"):" + msg);
    QApplication::exit();
}

void ProgressWidget::OnProgress(int p) {
    m_ProgressBar->setValue(p);
    m_lbProgressNum->setText(QString::number(p) + "%");
}

void ProgressWidget::OnNowFile(const QString& name) {
    m_lbProgressInfo->setText(name);
}

void ProgressWidget::OnDone() {
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    IMMDeviceEnumerator *mmDeviceEnume = nullptr;
    IMMDeviceCollection *deviceCollection = nullptr;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
                                  nullptr,
                                  CLSCTX_ALL,
                                  __uuidof(IMMDeviceEnumerator),
                                  (void**)&mmDeviceEnume);
    if (SUCCEEDED(hr)) {
        hr = mmDeviceEnume->EnumAudioEndpoints(
                    eRender, DEVICE_STATE_ACTIVE, &deviceCollection);
    }
    if (SUCCEEDED(hr)) {
        uint32_t count = 0;
        hr = deviceCollection->GetCount(&count);
        for (uint32_t i = 0;i < count;++i) {
            IMMDevice *device = nullptr;
            deviceCollection->Item(i,&device);
            IPropertyStore * proStore = nullptr;
            device->OpenPropertyStore(STGM_READ,&proStore);
            PROPVARIANT name;
            PropVariantInit(&name);
            hr = proStore->GetValue(PKEY_Device_FriendlyName, &name);
            if (SUCCEEDED(hr)) {
                QString deviceName = QString::fromWCharArray(name.pwszVal);
                if (deviceName.contains("CABLE Input") ||
                        deviceName.contains("VoiceMeeter Input")) {
                    PropVariantClear(&name);
                    proStore->Release();
                    emit signalNext();
                    CoUninitialize();
                    return;
                }
            }
            PropVariantClear(&name);
            proStore->Release();
        }
    }
    CoUninitialize();
//    if (QKMessageBox::information(0,
//                                  "提示",
//                                  "即将安装虚拟声卡(Voicemeeter)，如果你已经安装或者是专业声卡用户不建议安装。\n是否安装？",
//                                  QKMessageBox::Yes | QKMessageBox::No) == QKMessageBox::Yes) {
//        addVoicemeeter();
//    }
    emit signalNext();
}
void ProgressWidget::addVoicemeeter() {
    QStringList args;
    args << "-i";
    // QProcess::execute(qt_create_commandline(InstallInfo::InstallPath + "/" + "VoicemeeterSetup.exe", args));
}
