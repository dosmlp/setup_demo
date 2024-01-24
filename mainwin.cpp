#include "mainwin.h"
#include <QDebug>
#include <QPen>
MainWin::MainWin(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    setFixedSize(552,460);
    m_slayout = new QStackedLayout();
    m_vlayout = new QVBoxLayout(this);
    setLayout(m_vlayout);

    m_lbLogo = new QLabel(this);
    m_lbLogo->setFixedSize(228,173);

    m_toplayout = new QHBoxLayout();
    m_toplayout->addStretch();
    m_btClose = new QPushButton(this);
    m_btClose->setStyleSheet("QPushButton{"
                             "border-image: url(:/img/btclose);"
                             "}"
                             "QPushButton::hover{"
                             "border-image: url(:/img/btclose_hover);"
                             "}");
    m_btClose->setFixedSize(40,40);
    m_btMini = new QPushButton(this);
    m_btMini->setStyleSheet("QPushButton{"
                             "border-image: url(:/img/btMin);"
                             "}"
                             "QPushButton::hover{"
                             "border-image: url(:/img/btMin_hover);"
                             "}");
    m_btMini->setFixedSize(40,40);
    m_toplayout->addWidget(m_btMini);
    m_toplayout->addWidget(m_btClose);
    m_toplayout->setSpacing(0);
    m_toplayout->setContentsMargins(0,0,0,0);

    //图标(版本号)
    QPixmap pixlogo(":/img/img/big_logo.png");
    QPainter painter(&pixlogo);
    QPen pen(QColor("#999999"));
    painter.setPen(pen);
    painter.setFont(QFont("arial",10,QFont::Bold));
    painter.drawText(152,127,InstallInfo::version);

    m_lbLogo->setPixmap(pixlogo);
    m_vlayout->setContentsMargins(2,1,2,2);
    m_vlayout->addLayout(m_toplayout);
    m_vlayout->addWidget(m_lbLogo,0,Qt::AlignCenter);
    m_vlayout->addStretch();
    m_vlayout->addLayout(m_slayout);

    m_first = new FirstWidget(this);
    m_progress = new ProgressWidget(this);
    m_finished = new FinishedWidget(this);

    m_slayout->addWidget(m_first);
    m_slayout->addWidget(m_progress);
    m_slayout->addWidget(m_finished);

    connect(m_first,&FirstWidget::signalNext,[this](){
        m_slayout->setCurrentIndex(1);
        m_progress->startInstall();
    });
    connect(m_progress,&ProgressWidget::signalNext,[this](){
        m_slayout->setCurrentIndex(2);
    });
    connect(m_btClose,&QPushButton::clicked,this,&MainWin::close);
    connect(m_btMini,&QPushButton::clicked,this,&MainWin::showMinimized);
}

void MainWin::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_mousePressed = true;
        m_mousePressedPoint = event->pos();
    }
}

void MainWin::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_mousePressed = false;
    }
}

void MainWin::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mousePressed)
    {
        move(event->globalPos()-m_mousePressedPoint);
    }
}

void MainWin::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    QBrush brush(QColor("#FAFAFA"));
    QPen pen(QColor("#D9D9D9"));
    p.setPen(pen);
    p.setBrush(brush);

    p.drawRoundedRect(0,0,width()-1,height()-1,1,2);
}
