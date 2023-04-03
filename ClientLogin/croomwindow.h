#ifndef CROOMWINDOW_H
#define CROOMWINDOW_H

#include <QWidget>


class QNetworkAccessManager;
class QNetworkReply;

namespace Ui {
class CRoomWindow;
}

class CRoomWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CRoomWindow(QWidget *parent = nullptr);
    ~CRoomWindow();

    void SetNetworkManager(QNetworkAccessManager* _manager);
    void SetUserInfo(QString& _username, QString& _userpass);

signals:
    void signal_serverReply_createroom(QNetworkReply* _reply);

private slots:
    void on_BTN_ROOM_CREATE_clicked();


    void slot_replyfunc_createroom(QNetworkReply* _reply);

private:
    Ui::CRoomWindow *ui;
    QNetworkAccessManager* m_Manager = nullptr;

    QString m_userName;
    QString m_userPass;
};

#endif // CROOMWINDOW_H
