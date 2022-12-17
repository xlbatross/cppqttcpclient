#include "roomnamedialog.h"
#include "ui_roomnamedialog.h"

RoomNameDialog::RoomNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomNameDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

RoomNameDialog::~RoomNameDialog()
{
    delete ui;
}

const QString RoomNameDialog::getRoomName()
{
    return ui->edt_roomName->text();
}


