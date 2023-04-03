#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QMessageBox>
#include <QDesktopServices>

#include "cchangepassword.h"
#include "croomwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_Manager(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("PC Game");

    this->m_Manager= new QNetworkAccessManager;
    connect(this, &MainWindow::signal_server_reply, this, &MainWindow::slot_reply_func);

    this->m_ChangeWindow = new CChangePassword();
    this->m_ChangeWindow->SetNetworkManager(this->m_Manager);

    this->m_roomWindow = new CRoomWindow();
    this->m_roomWindow->SetNetworkManager(this->m_Manager);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(this->m_ChangeWindow)
    {
        delete this->m_ChangeWindow;
        this->m_ChangeWindow = nullptr;
    }
    if(this->m_Manager)
    {
        delete this->m_Manager;
        this->m_Manager = nullptr;
    }
    if(this->m_roomWindow)
    {
        delete this->m_roomWindow;
        this->m_roomWindow=nullptr;
    }
}


void MainWindow::on_BTN_LOGIN_clicked()
{
    this->server_Login();
}

void MainWindow::on_BTN_CHAEATPASS_clicked()
{
    QString username = this->ui->EDIT_USERNAME->text();
    QString password = this->ui->EDIT_PASSWORD->text();
    this->m_ChangeWindow->showWindows(username, password);
}

void MainWindow::server_Login()
{
    QString strUserName = ui->EDIT_USERNAME->text();
    QString strPassword = ui->EDIT_PASSWORD->text();
    QString sendText;

    //TODO: 构建请求(密码使用MD5进行加密)
    QJsonDocument json_doc;
    QJsonObject json;

    json["requetype"] = "login";
    json["username"] = strUserName;
    json["password"] = QCryptographicHash::hash( strPassword.toStdString(), QCryptographicHash::Md5).toHex().data();
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


void MainWindow::slot_reply_func(QNetworkReply* reply)
{
    QJsonDocument json_doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject json = json_doc.object();

    if("Success" != json["result"].toString())
    {
        QMessageBox::information(this, "账号或者密码错误", json["result"].toString());
        return;
    }

    // TODO: 登录成功了
    QMessageBox::information(this, "登陆成功", json["result"].toString());

    QString strUserName = ui->EDIT_USERNAME->text();
    QString strPassword = ui->EDIT_PASSWORD->text();
    // TODO: 弹出服务器选择界面
    this->m_roomWindow->SetUserInfo(strUserName, strPassword);
    this->m_roomWindow->show();





}


void MainWindow::on_BTN_REG_clicked()
{
    //TODO: 构建请求(密码使用MD5进行加密)
    QJsonDocument json_doc;
    QJsonObject json;

    QDesktopServices::openUrl(QUrl("http://192.168.88.128:7788"));

}




