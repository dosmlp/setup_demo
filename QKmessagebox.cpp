#include "QKmessagebox.h"
#include <QTimer>
#include <QDateTime>

QKMessageBox::QKMessageBox(QWidget *parent)
    : QDialog(parent),
      m_returnButton(Ok), m_mouse_press(false)
{
    setMinimumSize(270,140);
    setWindowFlags(Qt::WindowCloseButtonHint|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowModality(Qt::ApplicationModal);
    setFont(QFont("微软雅黑"));
    m_shadow.loadFromData(shadoowimg,sizeof(shadoowimg),"PNG");
    m_title = new QLabel(this);
    m_lbtext = new QLabel(this);
    m_lbtext->setStyleSheet("background-color: #FFFFFF;"
                            "font-size: 16px;"
                            "padding-left: 20px;"
                            "padding-right: 20px;");
    m_lbtext->setMaximumHeight(500);
    m_lbtext->setMinimumHeight(80);
    m_lbtext->setAlignment(Qt::AlignCenter);
    m_mainlayout = new QVBoxLayout(this);
    m_buttonlayout = new QHBoxLayout();

    m_buttonlayout->addStretch();
    m_buttonlayout->setContentsMargins(12,12,15,15);
    m_buttonlayout->setSpacing(12);
    m_buttonlayout->addStretch();

    //m_mainlayout->setContentsMargins(5,3,5,7);
    m_mainlayout->setContentsMargins(16,15,16,16);
    m_mainlayout->addWidget(m_title);
//    m_mainlayout->addStretch(1);
//    m_mainlayout->addStretch(0);
    m_mainlayout->addWidget(m_lbtext);
    m_mainlayout->addLayout(m_buttonlayout);
    m_mainlayout->setSpacing(0);

    m_bt_style = "QPushButton {"
                 "background-color: #F4F4F4;"
                 "border: 1px solid #BBBBBB;"
                 "border-radius: 14px;"
                 "padding: 6px;"
                 "font: 14px;}"
                 "QPushButton:hover{"
                 "background-color: #FFD600;"
                 "border: 1px solid #FFC002;}";
    this->setLayout(m_mainlayout);
    this->initControl();
}

void QKMessageBox::initControl()
{
    m_title->setFixedHeight(40);
    m_title->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    m_title->setStyleSheet("color:#FFFFFF;"
                           "background-color: #353535;"
                           "border-bottom-color:#dae5ea;"
                           "font-size: 16px;"
                           "border-bottom-width: 1px;"
                           "padding-left: 5px;"
                           "border-radius: 2px;"
                           "border-bottom-style : solid;");

}

QKMessageBox::ZStandardButton QKMessageBox::information(QWidget* parent,const QString &title,const QString &text,
                             int bt)
{
    QPoint center;
    if(parent)
        center = QPoint(parent->pos().x()+parent->width()/2,parent->pos().y()+parent->height()/2);
    parent = 0;
    QKMessageBox msgBox(parent);
    msgBox.setTitle(title);
    msgBox.setText(text);

    if(bt & Ok)//
    {
        QPushButton* m_btOk = new QPushButton("确定"/*tr("Ok")*/, &msgBox);
        msgBox.addButton(m_btOk);

        connect(m_btOk,&QPushButton::clicked,[&msgBox](){
            msgBox.setClickButton(Ok);
            msgBox.close();
        });        
    }
    if(bt & Yes)//
    {
        QPushButton* m_btYes = new QPushButton("是"/*tr("Yes")*/, &msgBox);
        msgBox.addButton(m_btYes);

        connect(m_btYes,&QPushButton::clicked,[&msgBox](){
            msgBox.setClickButton(Yes);
            msgBox.close();
        });        
    }
    if(bt & No)//
    {
        QPushButton* m_btNo = new QPushButton("否"/*tr("No")*/, &msgBox);
        msgBox.addButton(m_btNo);

        connect(m_btNo,&QPushButton::clicked,[&msgBox](){
            msgBox.setClickButton(No);
            msgBox.close();
        });        
    }
    msgBox.show();
    if(center.x() != 0 && center.y() != 0)
        msgBox.move(center.x()-msgBox.width()/2,center.y()-msgBox.height()/2);
    msgBox.exec();

    return msgBox.clickButton();
}



void QKMessageBox::addButton(QPushButton *bt)
{
    if(bt == nullptr)
        return;
    bt->setFixedSize(70,28);
    bt->setStyleSheet(m_bt_style);
    m_buttonlayout->insertWidget(m_buttonlayout->count()-1,bt);
}

void QKMessageBox::setClickButton(QKMessageBox::ZStandardButton bt)
{
    m_returnButton = bt;
}

void QKMessageBox::setText(const QString &text)
{
//    QRect text_rect = fontMetrics().boundingRect(text);
//    resize(text_rect.width()+60,text_rect.height()+95);
//    m_text = text;
    m_lbtext->setText(text);
}

void QKMessageBox::setTitle(const QString &title)
{
    m_title->setText(title);
}

void QKMessageBox::setTimeout(int timeout)
{
    QTimer::singleShot( timeout, this, SLOT(onSlotTimeoutToCloseMeFunc()) );
}

QKMessageBox::ZStandardButton QKMessageBox::clickButton()
{
    return m_returnButton;
}


void QKMessageBox::paintEvent(QPaintEvent *)
{
    QPainter p(this);

//    QRect bottom(5, 136, 200, 7);
//    QRect top(5, 0, 200, 3);
//    QRect left(0, 3, 5, 133);
//    QRect right(205, 3, 5, 133);
//    QRect topRight(205, 0, 5, 3);
//    QRect topLeft(0, 0, 5, 3);
//    QRect bottomLeft(0, 136, 5, 7);
//    QRect bottomRight(205, 136, 5, 7);
    QRect bottom(16, 144, 300, 16);
    QRect top(16, 0, 300, 16);
    QRect left(0, 16, 16, 128);
    QRect right(316, 16, 16, 128);
    QRect topRight(316, 0, 16, 16);
    QRect topLeft(0, 0, 16, 16);
    QRect bottomLeft(0, 144, 16, 16);
    QRect bottomRight(316, 144, 16, 16);

//    QRect tBottom(5, this->height()-7, this->width() - 10, 7);
//    QRect tTop(5, 0, this->width() - 10, 3);
//    QRect tLeft(0, 3, 5, this->height() - 10);
//    QRect tRight(this->width() - 5, 3, 5, this->height() - 10);
//    QRect tTopLeft(0, 0, 5, 3);
//    QRect tTopRight(this->width() - 5, 0, 5, 3);
//    QRect tBottomLeft(0, this->height() - 7, 5, 7);
//    QRect tBottomRight(this->width() - 5, this->height() - 7, 5, 7);

    QRect tBottom(16, this->height()-16, this->width() - 32, 16);
    QRect tTop(16, 0, this->width() - 32, 16);
    QRect tLeft(0, 16, 16, this->height() - 32);
    QRect tRight(this->width() - 16, 16, 16, this->height() - 32);
    QRect tTopLeft(0, 0, 16, 16);
    QRect tTopRight(this->width() - 16, 0, 16, 16);
    QRect tBottomLeft(0, this->height() - 16, 16, 16);
    QRect tBottomRight(this->width() - 16, this->height() - 16, 16, 16);

    p.drawPixmap(tBottom, m_shadow, bottom);
    p.drawPixmap(tTop, m_shadow, top);
    p.drawPixmap(tLeft, m_shadow, left);
    p.drawPixmap(tRight, m_shadow, right);
    p.drawPixmap(tTopRight, m_shadow, topRight);
    p.drawPixmap(tTopLeft, m_shadow, topLeft);
    p.drawPixmap(tBottomLeft, m_shadow, bottomLeft);
    p.drawPixmap(tBottomRight, m_shadow, bottomRight);

    QPen pen;
    pen.setColor(QColor(255,255,255));

    QBrush brush;
    brush.setColor(QColor(255,255,255));
    brush.setStyle(Qt::SolidPattern);

    //top rect
//    p.setPen(pen);
//    p.setBrush(brush);
//    p.drawRect(5,3,width()-10,height() - 50);

    //bottom rect
    brush.setColor(QColor(255,255,255));
    pen.setColor(QColor(255,255,255));
    p.setPen(pen);
    p.setBrush(brush);
    p.drawRect(16,16,
               width()-33,height()-32);

    //draw text
//    pen.setColor(QColor(0,0,0));
//    p.setPen(pen);
//    QRect text_rect = fontMetrics().boundingRect(m_text);
//    p.drawText(QRectF(30,45,width()-10,height()-60),
//               Qt::AlignLeft,
//               m_text);

}

void QKMessageBox::mouseMoveEvent(QMouseEvent *e)
{
    if( m_mouse_press )
    {
        move(e->globalPos()-m_move_point);
    }
    QDialog::mouseMoveEvent(e);
}

void QKMessageBox::mousePressEvent(QMouseEvent *e)
{
    if(e->pos().y() < 56 &&
            e->button() == Qt::LeftButton)
    {
        m_mouse_press = true;
        m_move_point = e->pos();
    }
    QDialog::mousePressEvent(e);
}

void QKMessageBox::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_mouse_press = false;
    }
    QDialog::mouseReleaseEvent(e);
}


