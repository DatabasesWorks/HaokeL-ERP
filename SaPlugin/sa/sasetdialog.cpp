#include "sasetdialog.h"
#include "ui_sasetdialog.h"
#include "bean/SystemSetBean.h"
#include "dao/SystemSetdao.h"
#include "QMessageBox"
#include "haokeapp.h"
#include "saplugin.h"
SaSetDialog::SaSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaSetDialog)
{
    ui->setupUi(this);
    init();
}

SaSetDialog::~SaSetDialog()
{
    delete ui;
}

void SaSetDialog::init(){
    mWinId = HaoKeApp::saSubId() + SaPlugin::systemSetWinId();
    mWinTitle = "��������";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::saSubId().toInt(),mWinId.toInt(),0,"");

    SystemSetBean *hathOrderBean = SystemSetDAO::getBean("601");
    if(hathOrderBean && hathOrderBean->id().length() > 0){
        ui->hathOrderChk->setChecked(hathOrderBean->yesNo());
    }else{
        SystemSetBean *bean = new SystemSetBean();
        bean->setId("601");
        bean->setName("����ҵ����ж���");
        bean->setCaption(bean->name());
        bean->setYesNo(false);
        bean->setValue("");
        bean->setSubId(6);
        SystemSetDAO::createSystemSet(bean);
    }

    SystemSetBean *mrdBean = SystemSetDAO::getBean("602");
    if(mrdBean && mrdBean->id().length() > 0){
        ui->arrivalMrdChk->setChecked(mrdBean->yesNo());
    }else{
        SystemSetBean *bean = new SystemSetBean();
        bean->setId("602");
        bean->setName("��������������");
        bean->setCaption(bean->name());
        bean->setYesNo(false);
        bean->setValue("");
        bean->setSubId(6);
        SystemSetDAO::createSystemSet(bean);
    }

    SystemSetBean *overBean = SystemSetDAO::getBean("603");
    if(overBean && overBean->id().length() > 0){
        ui->overtopChk->setChecked(overBean->yesNo());
    }else{
        SystemSetBean *bean = new SystemSetBean();
        bean->setId("603");
        bean->setName("������Ʒ����������");
        bean->setCaption(bean->name());
        bean->setYesNo(false);
        bean->setValue("");
        bean->setSubId(6);
        SystemSetDAO::createSystemSet(bean);
    }
}
void SaSetDialog::on_okButton_clicked()
{
    SystemSetBean *hathOrderBean = SystemSetDAO::getBean("601");
    if(hathOrderBean && hathOrderBean->id().length() > 0){
        hathOrderBean->setYesNo(ui->hathOrderChk->isChecked());
        int result = SystemSetDAO::updateSystemSet(hathOrderBean);
        if(result < 1){
            QMessageBox::critical(this,"��ʾ","����ҵ����ж����������");
            return ;
        }
    }

    SystemSetBean *arrivalMrdBean = SystemSetDAO::getBean("602");
    if(arrivalMrdBean && arrivalMrdBean->id().length() > 0){
        arrivalMrdBean->setYesNo(ui->arrivalMrdChk->isChecked());
        int result = SystemSetDAO::updateSystemSet(arrivalMrdBean);
        if(result < 1){
            QMessageBox::critical(this,"��ʾ","�������������Ᵽ�����");
            return ;
        }
    }

    SystemSetBean *overBean = SystemSetDAO::getBean("603");
    if(overBean && overBean->id().length() > 0){
        overBean->setYesNo(ui->overtopChk->isChecked());
        int result = SystemSetDAO::updateSystemSet(overBean);
        if(result < 1){
            QMessageBox::critical(this,"��ʾ","������Ʒ����������");
            return ;
        }
    }
    this->accept();
}

void SaSetDialog::on_cancelButton_clicked()
{
    this->close();
}
