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
        QTcpServer * server;//��������
        QTcpSocket * client;//������ͻ���ͨ�ŵ��׽���
public slots:
        void sendMessages();//��ͻ��˷�������
        void recvMessage();//�������Կͻ��˵����ݡ�
};

#endif // WIDGET_H
