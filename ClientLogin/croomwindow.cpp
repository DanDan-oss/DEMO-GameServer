#include "croomwindow.h"
#include "ui_croomwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QMessageBox>
#include <QProcess>

CRoomWindow::CRoomWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CRoomWindow)
{
    ui->setupUi(this);
    connect(this, &CRoomWindow::signal_serverReply_createroom, this, &CRoomWindow::slot_replyfunc_createroom);
    this->setWindowTitle("服务器操作");
}

CRoomWindow::~CRoomWindow()
{
    delete ui;
}

void CRoomWindow::SetNetworkManager(QNetworkAccessManager* _manager)
{
    this->m_Manager = _manager;
}

void CRoomWindow::SetUserInfo(QString& _username, QString& _userpass)
{
    this->m_userName = _username;
    this->m_userPass=_userpass;
}

void CRoomWindow::on_BTN_ROOM_CREATE_clicked()
{
    //TODO: 构建请求(密码使用MD5进行加密)
    QJsonDocument json_doc;
    QJsonObject json;

    json["requetype"] ="room_crate";
    json["username"] = this->m_userName;
    json["password"] =  QCryptographicHash::hash( this->m_userPass.toStdString(), QCryptographicHash::Md5).toHex().data();
    json_doc.setObject(json);
    QString sendText = QString(json_doc.toJson());

    // 发送HTTP报文
    QNetworkRequest requst;
    requst.setUrl(QUrl("http://192.168.88.128:7788/room/"));
    requst.setHeader(QNetworkRequest::ContentLengthHeader, sendText.toUtf8().length());
    requst.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply=this->m_Manager->post(requst, sendText.toUtf8());

      //TODO: 设置网络回复消息的处理回调
    connect(reply, &QNetworkReply::finished, this, [=](){
        if( reply->error()!= QNetworkReply::NoError)
        {
            QMessageBox::information(this, QString ::number(reply->error()), "服务器连接失败");
            return;
        }
        emit this->signal_serverReply_createroom(reply);
     });

}

void CRoomWindow::slot_replyfunc_createroom(QNetworkReply* _reply)
{
    QJsonDocument json_doc = QJsonDocument::fromJson(_reply->readAll());
    QJsonObject json = json_doc.object();

    if("Success" != json["result"].toString())
    {
        QMessageBox::information(this, "服务器容器创建失败", json["result"].toString());
        return;
    }

    QString server_ip = json["server_address"].toString();
    QString server_port = json["server_port"].toString();

    qDebug() << "server_address: "<<server_ip;
    qDebug() << "server_port: "<<server_port;

//        QProcess proce;
//        QStringList argv;
//        argv.push_back(server_ip);
//        argv.push_back(server_port);
//        proce.startDetached("client.exe", argv, "./");
//        qApp->exit();

    return;
}

