#ifndef ROOMNAMEDIALOG_H
#define ROOMNAMEDIALOG_H

#include <QDialog>

namespace Ui {
class RoomNameDialog;
}

class RoomNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomNameDialog(QWidget *parent = nullptr);
    ~RoomNameDialog();

    const QString getRoomName();

private:
    Ui::RoomNameDialog *ui;


////추가
//private slots:
//    void setRoomName();
};


#endif // ROOMNAMEDIALOG_H
