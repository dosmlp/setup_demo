#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QSlider>
#include <QProgressBar>
#include <sysinfo.h>
#include "execinstall.h"
#include "QKmessagebox.h"
#include <WinBase.h>
#include <mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <Audioclient.h>

class ProgressWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressWidget(QWidget *parent = nullptr);
    void startInstall();
    void OnError(int code, QString msg);
    void OnProgress(int p);
    void OnNowFile(const QString& name);
    void OnDone();
signals:
    void signalNext();
private:
    QVBoxLayout* m_mainlayout;

    QLabel* m_lbProgressInfo;
    QSlider* m_ProgressBar;
    QLabel* m_lbProgressNum;

    execInstall* m_execinstall;
    void addVoicemeeter();
};

#endif // PROGRESSWIDGET_H
