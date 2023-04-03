#include "room.h"
#include "ui_room.h"

room::room(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::room)
{
    ui->setupUi(this);
}

room::~room()
{
    delete ui;
}
