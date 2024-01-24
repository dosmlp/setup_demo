#ifndef FINISHEDWIDGET_H
#define FINISHEDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class FinishedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FinishedWidget(QWidget *parent = nullptr);

signals:

private:
    QVBoxLayout* m_mainlayout;
    QLabel* m_lbDone;
    QPushButton* m_btUseit;
};

#endif // FINISHEDWIDGET_H
