#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include "firstwidget.h"
#include "progresswidget.h"
#include "finishedwidget.h"
class MainWin : public QWidget
{
    Q_OBJECT
public:
    explicit MainWin(QWidget *parent = nullptr);

signals:
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent*);

    bool m_mousePressed = false;
    QPoint m_mousePressedPoint;
private:
    QLabel* m_lbLogo;
    QVBoxLayout* m_vlayout;
    QHBoxLayout* m_toplayout;
    QStackedLayout* m_slayout;
    QPushButton* m_btClose;
    QPushButton* m_btMini;

    FirstWidget* m_first;
    ProgressWidget* m_progress;
    FinishedWidget* m_finished;
};

#endif // MAINWIN_H
