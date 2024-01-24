#ifndef SYSINFO_H
#define SYSINFO_H
#include <QtCore>
#include <QTimer>
#include <QObject>

struct ProcessInfo
{
    quint32 id;
    QString name;
};
QList<ProcessInfo> runningProcesses();
bool killProcess(const ProcessInfo &process, int msecs = 30000);
bool killProcess(quint32 pid, int msecs = 30000);
bool sysIs64Bit();
QString qt_create_commandline(const QString &program, const QStringList &arguments);

#endif // SYSINFO_H
