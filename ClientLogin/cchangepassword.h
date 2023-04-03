#ifndef CCHANGEPASSWORD_H
#define CCHANGEPASSWORD_H

#include <QWidget>


class QNetworkAccessManager;
class QNetworkReply;

namespace Ui {
class CChangePassword;
}

class CChangePassword : public QWidget
{
    Q_OBJECT

public:
    explicit CChangePassword(QWidget *parent = nullptr);
    ~CChangePassword();

    void showWindows(QString&, QString&);
    void SetNetworkManager(QNetworkAccessManager* _manager);

signals:
    void signal_server_reply(QNetworkReply*);


private slots:
    void slot_reply_func(QNetworkReply*);
    void on_BTN_CHAETPASS_clicked();

private:
    Ui::CChangePassword *ui;
    QNetworkAccessManager* m_Manager = nullptr;
};

#endif // CCHANGEPASSWORD_H
