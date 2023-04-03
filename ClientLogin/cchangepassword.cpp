#include "cchangepassword.h"
#include "ui_cchangepassword.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QMessageBox>

CChangePassword::CChangePassword(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CChangePassword)
{
    ui->setupUi(this);
    connect(this, &CChangePassword::signal_server_reply, this, &CChangePassword::slot_reply_func);
    this->setWindowTitle("修改密码");
}

CChangePassword::~CChangePassword()
{
    delete ui;
}

void CChangePassword::showWindows(QString& _username, QString& _userpass)
{
    this->ui->EDIT_USERNAME->setText(_username);
    this->ui->EDIT_PASSWORD_OLD->setText(_userpass);
    this->show();
}

void CChangePassword::SetNetworkManager(QNetworkAccessManager* _manager)
{
    this->m_Manager =_manager;
}

void CChangePassword::on_BTN_CHAETPASS_clicked()
{
    QString strUserName = this->ui->EDIT_USERNAME->text();
    QString oldPassword = this->ui->EDIT_PASSWORD_OLD->text();
    QString newPassword = this->ui->EDIT_PASSWORD_NEW->text();

    QString sendText;

    //TODO: 构建请求(密码使用MD5进行加密)
    QJsonDocument json_doc;
    QJsonObject json;

    json["requetype"] ="changepass";
    json["username"] = strUserName;
    json["password"] =  QCryptographicHash::hash( oldPassword.toStdString(), QCryptographicHash::Md5).toHex().data();
    json["newpassword"] = newPassword;

    json_doc.setObject(json);
    sendText = QString(json_doc.toJson());
    //TODO:  发送请求给服务器
    QNetworkRequest requst;
    requst.setUrl(QUrl("http://192.168.88.128:7788/user/"));
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
        emit this->signal_server_reply(reply);
     });
}

void CChangePassword::slot_reply_func(QNetworkReply* _reply)
{
    QJsonDocument json_doc = QJsonDocument::fromJson(_reply->readAll());
    QJsonObject json = json_doc.object();

    if("Success" != json["result"].toString())
    {
        QMessageBox::information(this, "账号或者密码错误", json["result"].toString());
        return;
    }
    QMessageBox::information(this, json["login_result"].toString(), "修改成功");
}

