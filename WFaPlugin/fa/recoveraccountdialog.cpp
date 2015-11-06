#include "recoveraccountdialog.h"
#include "ui_recoveraccountdialog.h"
#include "haokeapp.h"
#include "dao/faaccntdao.h"
#include "bean/FaSubsidiaryBean.h"

RecoverAccountDialog::RecoverAccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecoverAccountDialog)
{
    ui->setupUi(this);
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/image/cancel_48.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->cancelBtn->setIcon(icon1);
    ui->cancelBtn->setIconSize(QSize(24, 24));
    ui->cancelBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/image/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->okBtn->setIcon(icon2);
    ui->okBtn->setIconSize(QSize(24, 24));
    ui->okBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QString htmlText = "<html><head/><body><p><span style=' font-weight:600; color:#ff0000;'>%1</span></p></body></html>";
    mAccntMonth = HaoKeApp::getInstance()->getOperateDate().month();
    ui->label->setText(htmlText.arg("��ǰ������:" + QString::number(mAccntMonth) + "��"));

    FaWayBean *bean = FaAccntDAO::getFayWay();
    mWayId = 0;
    if(bean && bean->wayName().length() > 1){
        mWayName = bean->wayName();
        mWayId = bean->wayId();

    }

}

RecoverAccountDialog::~RecoverAccountDialog()
{
    delete ui;
}

void RecoverAccountDialog::on_cancelBtn_clicked()
{
    this->reject();
}

void RecoverAccountDialog::on_okBtn_clicked()
{
    if(QMessageBox::warning(this,"��ʾ","�Ƿ��" + QString::number(mAccntMonth)+ "�µ��ݻָ����ʣ�",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
        if(mWayId == 1){
            reAccntFIFO();
        }else if(mWayId == 2){
            reAccntMACost();
        }



    }
}

void RecoverAccountDialog::reAccntMACost(){
    QString htmlText = "<html><head/><body><p><span style=' font-weight:600; color:#ff0000;'>%1</span></p></body></html>";

    QDate loginDate = HaoKeApp::getInstance()->getOperateDate();
    int endDay = loginDate.daysInMonth();
    //QMessageBox::information(this,"",QString::number(endDay));
    QDate endDate = QDate::QDate(loginDate.year(),loginDate.month(),endDay);
    QDate startDate = QDate::QDate(loginDate.year(),loginDate.month(),1);
    QList<FaSubsidiaryBean *> faList = FaAccntDAO::queryFaSubsidiaryByEndDate(endDate);
    if(faList.size() > 0 ){
        QString noteInfo = endDate.toString("yyyy-MM-dd") + " ֮���е���û�лָ�����,���ָܻ�����";
        ui->noteLabel->setText(htmlText.arg(noteInfo));
        QMessageBox::critical(this,"����",endDate.toString("yyyy-MM-dd") + " ֮���е���û�лָ�����,���ָܻ�����");
        return ;
    }
    QList<FaSubsidiaryBean *> faAtList = FaAccntDAO::queryFaSubsidiaryByDate(startDate,endDate);
    if(faAtList.size() < 1 ){
        QString noteInfo = QString::number(mAccntMonth) + "��û�м�������,���ָܻ�����";
        ui->noteLabel->setText(htmlText.arg(noteInfo));
        QMessageBox::critical(this,"����",QString::number(mAccntMonth) + "��û�м�������,���ָܻ�����");
        return ;
    }else{
        QString info = "��ʼ�ָ�����";
        ui->noteLabel->setText(htmlText.arg(info));
        // ��ʼ�ָ�����
        for(int i = 0; i < faAtList.size(); i++ ){

            //�����ָ�����
            DBManager::getDBManager()->getGoinDBConnection().transaction();
            QString sql = "DELETE FROM fa_subsidiary WHERE id=" + QString::number(faAtList.at(i)->id());
            logger()->debug(sql);
            QueryRunner query = QueryRunner();
            int result = query.update(DBManager::getDBManager()->getGoinDBConnection(),sql);
            sql = "UPDATE rd_record SET accounter='' WHERE rd_no='" + faAtList.at(i)->vouchNo() + "'";
            int uResult = query.update(DBManager::getDBManager()->getGoinDBConnection(),sql);
            logger()->debug(sql);
            sql = "UPDATE rd_records SET is_account=false "
                    " WHERE rd_no='" + faAtList.at(i)->vouchNo() + "' AND product_code='" + faAtList.at(i)->productCode() + "'";
            int uRdsResult = query.update(DBManager::getDBManager()->getGoinDBConnection(),sql);
            logger()->debug(sql);
            if(result > 0 && uResult > 0 && uRdsResult > 0){
                DBManager::getDBManager()->getGoinDBConnection().commit();
                QString noteInfo = "��ⵥ�ݣ�" + faAtList.at(i)->vouchNo() + " ��Ʒ:" + faAtList.at(i)->productCode() + " �ָ����ʳɹ�";
                QApplication::processEvents();
                ui->noteLabel->setText(htmlText.arg(noteInfo));
            }else{
                DBManager::getDBManager()->getGoinDBConnection().rollback();
                QString noteInfo = "��ⵥ�ݣ�" + faAtList.at(i)->vouchNo() + " ��Ʒ:" + faAtList.at(i)->productCode() + " ���ݻָ����ʳ���";
                ui->noteLabel->setText(htmlText.arg(noteInfo));
                QMessageBox::critical(this,"����","��ⵥ�ݣ�" + faAtList.at(i)->vouchNo() + " ��Ʒ:" + faAtList.at(i)->productCode() + " �ָ����ʳ���");
                return ;
            }

        }
        info = "�����ָ�����";
        ui->noteLabel->setText(htmlText.arg(info));
        ui->endLabel->setText(htmlText.arg(QString::number(mAccntMonth) + "�»ָ����ʳɹ�"));
        // �����ָ�����

    }
}

void RecoverAccountDialog::reAccntFIFO(){

    QString htmlText = "<html><head/><body><p><span style=' font-weight:600; color:#ff0000;'>%1</span></p></body></html>";

    QDate loginDate = HaoKeApp::getInstance()->getOperateDate();
    int endDay = loginDate.daysInMonth();
    //QMessageBox::information(this,"",QString::number(endDay));
    QDate endDate = QDate::QDate(loginDate.year(),loginDate.month(),endDay);
    QDate startDate = QDate::QDate(loginDate.year(),loginDate.month(),1);
    QList<FaSubsidiaryBean *> faList = FaAccntDAO::queryFaSubsidiaryByEndDate(endDate);
    if(faList.size() > 0 ){
        QString noteInfo = endDate.toString("yyyy-MM-dd") + " ֮���е���û�лָ�����,���ָܻ�����";
        ui->noteLabel->setText(htmlText.arg(noteInfo));
        QMessageBox::critical(this,"����",endDate.toString("yyyy-MM-dd") + " ֮���е���û�лָ�����,���ָܻ�����");
        return ;
    }
    QList<FaSubsidiaryBean *> faAtList = FaAccntDAO::queryFaSubsidiaryByDate(startDate,endDate);
    if(faAtList.size() < 1 ){
        QString noteInfo = QString::number(mAccntMonth) + "��û�м�������,���ָܻ�����";
        ui->noteLabel->setText(htmlText.arg(noteInfo));
        QMessageBox::critical(this,"����",QString::number(mAccntMonth) + "��û�м�������,���ָܻ�����");
        return ;
    }else{
        QApplication::setOverrideCursor(Qt::WaitCursor);
        ui->okBtn->setEnabled(false);
        QString info = "��ʼ�ָ�����";
        ui->noteLabel->setText(htmlText.arg(info));
        // ��ʼ�ָ�����
        for(int i = 0; i < faAtList.size(); i++ ){
            if(faAtList.at(i)->rdFlag() == 1){
                //���ָ�����


                DBManager::getDBManager()->getGoinDBConnection().transaction();
                if(faAtList.at(i)->vouchType() == "14"){
                    //�Ƚ��ȳ� �ɹ��˻� �ָ����ν������
                    QList<FaSubsidiaryBean *> faReList = FaAccntDAO::queryFIFOFaSubsidiaryReByBottom(faAtList.at(i)->whCode(), faAtList.at(i)->productCode());

                    if(faReList.size() < 1){
                        QString noteInfo = "�ɹ��˻�������ⵥ�ݣ�" + faAtList.at(i)->vouchNo() + " ��Ʒ:" + faAtList.at(i)->productCode() + " �ָ�����ʧ�ܣ�û���ҵ�������Ʒ";
                        ui->noteLabel->setText(htmlText.arg(noteInfo));
                        ui->okBtn->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        logger()->error(noteInfo);
                        QMessageBox::critical(this,"����",noteInfo);
                        return ;
                    }
                    //�Ƚ��ȳ� �ɹ��˻� �˻�����
                    double batch_in = faAtList.at(i)->inQuantity();
                    int reResult = 0;
                    int fifo_i = 0 ;
                    while( qAbs(batch_in) > 0  ){
                        if(faReList.size() > fifo_i){
                            //���ν������
                            double amount = faReList.at(fifo_i)->inQuantity()-faReList.at(fifo_i)->batchAmount();
                            if(amount >= qAbs(batch_in)){
                                faReList.at(fifo_i)->setBatchAmount(faReList.at(fifo_i)->batchAmount() + qAbs(batch_in));
                                reResult = FaAccntDAO::updateSubsidiary(faReList.at(fifo_i));
                                fifo_i = fifo_i + 1;
                                batch_in = 0 ;
                            }else{
                                batch_in = batch_in + amount;
                                faReList.at(fifo_i)->setBatchAmount(faReList.at(fifo_i)->batchAmount() + amount);
                                reResult = FaAccntDAO::updateSubsidiary(faReList.at(fifo_i));
                                fifo_i = fifo_i + 1;
                            }
                        }else{
                            QString noteInfo = "�ɹ��˻�������ⵥ�ݣ�" + faAtList.at(i)->vouchNo() + " ��Ʒ:" + faAtList.at(i)->productCode() + " �ָ�����ʧ�ܣ�������Ʒ��β��";
                            ui->noteLabel->setText(htmlText.arg(noteInfo));
                            ui->okBtn->setEnabled(true);
                            QApplication::restoreOverrideCursor();
                            logger()->error(noteInfo);
                            QMessageBox::critical(this,"����",noteInfo);
                            return ;

                        }
                    }

                }
                QString sql = "DELETE FROM fa_subsidiary WHERE id=" + QString::number(faAtList.at(i)->id());
                logger()->debug(sql);
                QueryRunner query = QueryRunner();
                int result = query.update(DBManager::getDBManager()->getGoinDBConnection(),sql);
                sql = "UPDATE rd_record SET accounter='' WHERE rd_no='" + faAtList.at(i)->vouchNo() + "'";
                int uResult = query.update(DBManager::getDBManager()->getGoinDBConnection(),sql);
                logger()->debug(sql);
                sql = "UPDATE rd_records SET is_account=false "
                        " WHERE rd_no='" + faAtList.at(i)->vouchNo() + "' AND product_code='" + faAtList.at(i)->productCode() + "'";
                int uRdsResult = query.update(DBManager::getDBManager()->getGoinDBConnection(),sql);
                logger()->debug(sql);
                if(result > 0 && uResult > 0 && uRdsResult > 0){
                    DBManager::getDBManager()->getGoinDBConnection().commit();
                    QString noteInfo = "��ⵥ�ݣ�" + faAtList.at(i)->vouchNo() + " ��Ʒ:" + faAtList.at(i)->productCode() + " �ָ����ʳɹ�";
                    QApplication::processEvents();
                    ui->noteLabel->setText(htmlText.arg(noteInfo));
                }else{
                    DBManager::getDBManager()->getGoinDBConnection().rollback();
                    QString noteInfo = "��ⵥ�ݣ�" + faAtList.at(i)->vouchNo() + " ��Ʒ:" + faAtList.at(i)->productCode() + " ���ݻָ����ʳ���";
                    ui->noteLabel->setText(htmlText.arg(noteInfo));
                    ui->okBtn->setEnabled(true);
                    QApplication::restoreOverrideCursor();
                    QMessageBox::critical(this,"����","��ⵥ�ݣ�" + faAtList.at(i)->vouchNo() + " ��Ʒ:" + faAtList.at(i)->productCode() + " �ָ����ʳ���");
                    return ;
                }
            }else{
                //����ָ�����

                //�Ƚ��ȳ� �ָ����ν������
                QList<FaSubsidiaryBean *> faReList = FaAccntDAO::queryReFIFOFaSubsidiaryByDvId(faAtList.at(i)->dvId());

                if(faReList.size() < 1){
                    QString noteInfo = "���ⵥ�ݣ�" + faAtList.at(i)->vouchNo() + " ��Ʒ:" + faAtList.at(i)->productCode() + " �ָ�����ʧ�ܣ�û���ҵ�������Ʒ";
                    ui->noteLabel->setText(htmlText.arg(noteInfo));
                    ui->okBtn->setEnabled(true);
                    QApplication::restoreOverrideCursor();
                    QMessageBox::critical(this,"����",noteInfo);
                    return ;
                }
                DBManager::getDBManager()->getGoinDBConnection().transaction();
                int fifo_i = 0;
                //�Ƚ��ȳ� �˻�����
                double batch_out = faAtList.at(i)->outQuantity();
                int reResult = 0;
                faReList.at(fifo_i)->setBatchAmount(faReList.at(fifo_i)->batchAmount() + batch_out);
                reResult = FaAccntDAO::updateSubsidiary(faReList.at(fifo_i));

                QString sql = "DELETE FROM fa_subsidiary WHERE id=" + QString::number(faAtList.at(i)->id());
                logger()->debug(sql);
                QueryRunner query = QueryRunner();
                int result = query.update(DBManager::getDBManager()->getGoinDBConnection(),sql);
                sql = "UPDATE rd_record SET accounter='' WHERE rd_no='" + faAtList.at(i)->vouchNo() + "'";
                int uResult = query.update(DBManager::getDBManager()->getGoinDBConnection(),sql);
                logger()->debug(sql);
                sql = "UPDATE rd_records SET is_account=false "
                        " WHERE rd_no='" + faAtList.at(i)->vouchNo() + "' AND product_code='" + faAtList.at(i)->productCode() + "'";
                int uRdsResult = query.update(DBManager::getDBManager()->getGoinDBConnection(),sql);
                logger()->debug(sql);
                if(reResult>0 && result > 0 && uResult > 0 && uRdsResult > 0){
                    DBManager::getDBManager()->getGoinDBConnection().commit();
                    QString noteInfo = "���ⵥ�ݣ�" + faAtList.at(i)->vouchNo() + " ��Ʒ:" + faAtList.at(i)->productCode() + " �ָ����ʳɹ�";
                    QApplication::processEvents();
                    ui->noteLabel->setText(htmlText.arg(noteInfo));
                }else{
                    DBManager::getDBManager()->getGoinDBConnection().rollback();
                    QString noteInfo = "���ⵥ�ݣ�" + faAtList.at(i)->vouchNo() + " ��Ʒ:" + faAtList.at(i)->productCode() + " ���ݻָ����ʳ���";
                    ui->noteLabel->setText(htmlText.arg(noteInfo));
                    ui->okBtn->setEnabled(true);
                    QApplication::restoreOverrideCursor();
                    QMessageBox::critical(this,"����","���ⵥ�ݣ�" + faAtList.at(i)->vouchNo() + " ��Ʒ:" + faAtList.at(i)->productCode() + " �ָ����ʳ���");
                    return ;
                }
            }

        }
        info = "�����ָ�����";
        ui->noteLabel->setText(htmlText.arg(info));
        ui->endLabel->setText(htmlText.arg(QString::number(mAccntMonth) + "�»ָ����ʳɹ�"));
        ui->okBtn->setEnabled(true);
        QApplication::restoreOverrideCursor();
        // �����ָ�����

    }
}
