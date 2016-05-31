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
        connect(server,SIGNAL(newConnection()),this,SLOT(sendMessages()));//��һ���µ�������Чʱ��newConnection���źű�������
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
        out << (quint16)0;//��д��һquint16��16��bit�������ֽڣ���0 bit��byte
        out << tr("Hello tcp");//��д������
        out.device()->seek(0);//�ڶ�λ���ַ��豸�Ŀ�ͷ
        out << quint16(block.size() - sizeof(quint16));//ʹ�������ݵĴ�С��ȥquint16�Ĵ�С�����ó����ݵĴ�С��

        QTcpSocket * client = server->nextPendingConnection();//������һ�����ӵ�QTcpSocketĿ�ꡣ
//        connect(client,SIGNAL(disconnected()),client,SLOT(deleteLater()));//��һ��socketȷ���Ͽ�ʱ��disconnected�źŷ����������Ҫɾ�����Ͷ�����ʹ��deleteLater������
        client->write(block);
//        client->disconnectFromHost();//��ͼ�ر����socket��������к�����������Ҫд�룬QAbstractSocket����뵽ClosingState�����ӳ�ֱ�����е�����ȫ��д�롣��󣬽���δ����״̬�ͷ���disconnected�źš�
        qDebug("Send message successful");
}
