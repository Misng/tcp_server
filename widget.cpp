#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Widget)
{
        ui->setupUi(this);
        client = NULL;

        server = new QTcpServer(this);
        if(!server->listen(QHostAddress::Any,55836)){
                qDebug()<<server->errorString();
                server->close();
        }

        connect(server,SIGNAL(newConnection()),this,SLOT(sendMessages()));//当一个新的连接有效时，newConnection的信号被发出。
        connect(ui->send_pushButton,SIGNAL(clicked()),this,SLOT(sendMessages()));//当按钮被点击时，触发发送信息的槽。
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
        out << (quint16)0;
        out << tr("Hello tcp");
        out.device()->seek(0);
        out << quint16(block.size() - sizeof(quint16));

        /**
         *1.先写入一quint16（16个bit，两个字节）的0 bit，byte
         *2.再写入内容
         *3.再定位到IO设备数据的头部
         *4.使用总数据的大小减去quint16的大小，即得出内容的大小。
*/
        if(client == NULL){
                client = server->nextPendingConnection();//返回下一个链接的QTcpSocket目标。
                connect(client,SIGNAL(readyRead()),this,SLOT(recvMessage()));
        }
        client->write(block);
        qDebug("Send message successful");

}

void Widget::recvMessage()
{
        QDataStream in(client);
        in.setVersion(QDataStream::Qt_4_8);
        quint16 blocksize = 0;

        if(blocksize == 0){
                if(client->bytesAvailable() < sizeof(quint16))
                        return ;
                in >> blocksize;
        }
        if(client->bytesAvailable() < blocksize)//bytesAvailable返回等待读取的字节数,如果结果为真则表明数据没有完全读取
                return;
        /**
         *首先，接收qunint16个大小
         *接着，在接收blocksize个大小的数据。
*/
        QString message;
        in >> message;
        ui->showMessage_label->setText(message);

}
