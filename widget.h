#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
        Q_OBJECT
        
public:
        explicit Widget(QWidget *parent = 0);
        ~Widget();
        
private:
        Ui::Widget *ui;
        QTcpServer * server;
public slots:
        void sendMessages();
};

#endif // WIDGET_H
