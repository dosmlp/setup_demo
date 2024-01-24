#include "finishedwidget.h"
#include <QApplication>
#include <QProcess>
#include "installinfo.h"
FinishedWidget::FinishedWidget(QWidget *parent) : QWidget(parent)
{
    m_mainlayout = new QVBoxLayout(this);
    setLayout(m_mainlayout);
    setFont(QFont("微软雅黑"));
    //安装成功
    m_lbDone = new QLabel("安装成功",this);
    m_lbDone->setStyleSheet("font-size: 16px;"
                            "color: #999999;");
    //完成按钮
    m_btUseit = new QPushButton("开始使用",this);
    m_btUseit->setFixedSize(180,50);
    m_btUseit->setStyleSheet("QPushButton {"
                             "color: #444444;"
                             "background-color: #FFD600;"
                             "border-radius: 4px;"
                             "padding: 6px;"
                             "font: 20px;}"
                             "QPushButton:hover{"
                             "background-color: #FFC100;}"
                             );

    m_mainlayout->setSpacing(15);
    m_mainlayout->addStretch();
    m_mainlayout->addWidget(m_lbDone,0,Qt::AlignCenter);
    m_mainlayout->addWidget(m_btUseit,0,Qt::AlignCenter);
    m_mainlayout->addStretch();
    m_mainlayout->setContentsMargins(184,0,184,92);


    connect(m_btUseit,&QPushButton::clicked,[this](){
        QProcess::startDetached(InstallInfo::InstallPath+"/XXLive.exe",QStringList(),InstallInfo::InstallPath);
        QApplication::exit();
    });

}
