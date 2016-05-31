#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Widget)
{
        ui->setupUi(this);
        server = new QTcpServer(this);
        if(!server->listen()){//ture on success,otherwise returns false
//        if(!server->listen(QHostAddress::LocalHost,0)){//ture on success,otherwise returns false
                qDebug()<<server->errorString();
                server->close();
        }
        qDebug() << server->serverAddress().toString() << server->serverPort();
        connect(server,SIGNAL(newConnection()),this,SLOT(sendMessages()));//当一个新的连接有效时，newConnection的信号被发出。
}

Widget::~Widget()
{
        delete ui;
}

void Widget::sendMessages()
{
        //初始化数据结构
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        //向数据结构中写入数据
        out << (quint16)0;//先写入一quint16（16个bit，两个字节）的0 bit，byte
        out << tr("Hello tcp");//再写入内容
        out.device()->seek(0);//在定位到字符设备的开头
        out << quint16(block.size() - sizeof(quint16));//使用总数据的大小减去quint16的大小，即得出内容的大小。

        QTcpSocket * client = server->nextPendingConnection();//返回下一个链接的QTcpSocket目标。
//        connect(client,SIGNAL(disconnected()),client,SLOT(deleteLater()));//当一个socket确定断开时，disconnected信号发出。如果需要删除发送对象，则使用deleteLater函数。
        client->write(block);
//        client->disconnectFromHost();//企图关闭这个socket，如果还有后续的数据需要写入，QAbstractSocket会进入到ClosingState并且延迟直到所有的数据全部写入。最后，进入未连接状态和发送disconnected信号。
        qDebug("Send message successful");
}
