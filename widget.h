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
        QTcpServer * server;//服务器。
        QTcpSocket * client;//用于与客户端通信的套接字
public slots:
        void sendMessages();//向客户端发送数据
        void recvMessage();//接受来自客户端的数据。
};

#endif // WIDGET_H
