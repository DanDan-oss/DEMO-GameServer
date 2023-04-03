#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>

class CChangePassword;
class CRoomWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   void server_Login();


signals:
    void signal_server_reply(QNetworkReply*);

private slots:
    void slot_reply_func(QNetworkReply* reply);
    void on_BTN_LOGIN_clicked();
    void on_BTN_REG_clicked();
    void on_BTN_CHAEATPASS_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager* m_Manager = nullptr;
    CChangePassword* m_ChangeWindow= nullptr;
    CRoomWindow* m_roomWindow= nullptr;
    QString m_userName;
    QString m_userPass;
};
#endif // MAINWINDOW_H
