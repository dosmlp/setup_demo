#ifndef EXECINSTALL_H
#define EXECINSTALL_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <qt_windows.h>
#include <QSettings>
#include <QStandardPaths>
#include "installinfo.h"

/*!
 * \brief 执行安装
 */
class execInstall : public QThread {
    Q_OBJECT
  public:
    explicit execInstall(QObject* parent = nullptr);

  signals:
    void signalNowFile(const QString& file_name);
    void signalDone();
    void signalError(int, QString);
    void signalProgress(int);
  protected:
    void run();
  private:
    bool releaseArchive(const QString &target_path);
    void addVCRunTime();
    void addSysInfo();
    void addVoicemeeter();
    // bool load7zData();
    // void* ar7z_data_ = nullptr;
    // uint ar7z_size_ = 0;
};

#endif // EXECINSTALL_H
