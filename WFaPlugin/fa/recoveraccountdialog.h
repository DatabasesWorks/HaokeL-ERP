#ifndef RECOVERACCOUNTDIALOG_H
#define RECOVERACCOUNTDIALOG_H

#include <QDialog>
#include "log4qt/logger.h"
namespace Ui {
class RecoverAccountDialog;
}

class RecoverAccountDialog : public QDialog
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit RecoverAccountDialog(QWidget *parent = 0);
    ~RecoverAccountDialog();
    
private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::RecoverAccountDialog *ui;
    int mAccntMonth;
    QString mWayName; //�Ƽ۷�ʽ����
    int mWayId;       //�Ƽ۷�ʽID
    void reAccntFIFO();
    void reAccntMACost();
};

#endif // RECOVERACCOUNTDIALOG_H
