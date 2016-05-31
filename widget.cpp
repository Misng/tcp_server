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

        connect(server,SIGNAL(newConnection()),this,SLOT(sendMessages()));//��һ���µ�������Чʱ��newConnection���źű�������
        connect(ui->send_pushButton,SIGNAL(clicked()),this,SLOT(sendMessages()));//����ť�����ʱ������������Ϣ�Ĳۡ�
}

Widget::~Widget()
{
        delete ui;
}

void Widget::sendMessages()
{
        //��ʼ�����ݽṹ
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        //�����ݽṹ��д������
        out << (quint16)0;
        out << tr("Hello tcp");
        out.device()->seek(0);
        out << quint16(block.size() - sizeof(quint16));

        /**
         *1.��д��һquint16��16��bit�������ֽڣ���0 bit��byte
         *2.��д������
         *3.�ٶ�λ��IO�豸���ݵ�ͷ��
         *4.ʹ�������ݵĴ�С��ȥquint16�Ĵ�С�����ó����ݵĴ�С��
*/
        if(client == NULL){
                client = server->nextPendingConnection();//������һ�����ӵ�QTcpSocketĿ�ꡣ
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
        if(client->bytesAvailable() < blocksize)//bytesAvailable���صȴ���ȡ���ֽ���,������Ϊ�����������û����ȫ��ȡ
                return;
        /**
         *���ȣ�����qunint16����С
         *���ţ��ڽ���blocksize����С�����ݡ�
*/
        QString message;
        in >> message;
        ui->showMessage_label->setText(message);

}
