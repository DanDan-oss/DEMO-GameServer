#ifndef ROOM_H
#define ROOM_H

#include <QWidget>

namespace Ui {
class room;
}

class room : public QWidget
{
    Q_OBJECT

public:
    explicit room(QWidget *parent = nullptr);
    ~room();

private:
    Ui::room *ui;
};

#endif // ROOM_H
