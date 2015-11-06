#include "cusform.h"
#include "ui_cusform.h"
#include "haokeapp.h"
#include "widget/foottablemodel.h"
#include "widget/hierarchicalheaderview.h"
#include "cusdialog.h"
#include "dao/basedao.h"
#include "baplugin.h"

CusForm::CusForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CusForm)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,6);
    init();
    ui->treeView->expandAll();
}

CusForm::~CusForm()
{
    delete ui;
}

void CusForm::init(){
    mId="";
    mWinId = HaoKeApp::baSubId() + BaPlugin::baCusSWinId();
    mWinTitle = "����ͻ�����";
    setInputEnabled(false);
    setButtonEnabled(false);

    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::baSubId().toInt(),mWinId.toInt(),0,"");

    QList<CustomerTypeBean *> listFirstBean = CustomerTypeDAO::queryTypeByFirst();
    QList<CustomerTypeBean *> listBean = CustomerTypeDAO::queryType();
    QStandardItemModel *model;
    QIcon typeIcon;
    typeIcon.addFile(QString::fromUtf8(":/image/class_24.png"), QSize(), QIcon::Normal, QIcon::Off);

    model = new QStandardItemModel(listFirstBean.size(),1);
    model->setHeaderData(0, Qt::Horizontal, tr("Customer Type"));
    for(int i=0;i<listBean.size();i++){
        QString str = ""+listBean.at(i)->typeId() + " - " + listBean.at(i)->typeName();
        ui->typeComboBox->addItem(str);
    }
    int j=0;
    for(int i=0;i<listBean.size();i++){
        if(listBean.at(i)->typeGrade()==1){
            QString str="("+listBean.at(i)->typeId()+") "+listBean.at(i)->typeName();
            QStandardItem *item = new QStandardItem(str);
            item->setText(str);
            for(int k=0;k<listBean.size();k++){
                if(listBean.at(k)->typeGrade()==2){
                    QString str1="(" + listBean.at(k)->typeId() + ") " + listBean.at(k)->typeName();
                    if(listBean.at(i)->typeId().contains(listBean.at(k)->typeSuperior())){
                        QStandardItem *childItem = new QStandardItem(str1);
                        for(int h = 0;h < listBean.size() ; h++ ){
                            if(listBean.at(h)->typeGrade()==3){
                                QString str2="("+listBean.at(h)->typeId()+") "+listBean.at(h)->typeName();
                                if(listBean.at(k)->typeId().contains(listBean.at(h)->typeSuperior())){
                                    QStandardItem *childItem3 = new QStandardItem(str2);
                                    childItem->appendRow(childItem3);
                                }

                            }
                        }
                        item->appendRow(childItem);
                    }
                }
            }
            model->setItem(j, 0, item);
            j=j+1;
        }
    }

    ui->treeView->setModel(model);
    QString tmp_sql_f = " id,cus_code,cus_name,cus_alias,cus_type,pinyin_code,area_city,trade,address,post,cus_reg_code"
            ",cus_bank,cus_account,corporate,licence_sdate,licence_edate,dev_date,email,contacts,tel,fax,phone"
            ",cus_address,cus_person,is_credit,cus_cre_grade,cus_cre_line,cus_cre_date,is_shop,memo ";
    mSql1 = "SELECT " + tmp_sql_f + " FROM ba_customer WHERE cus_type LIKE ? ORDER BY cus_code ASC";
    mSql2 = "SELECT " + tmp_sql_f + " FROM ba_customer WHERE cus_name LIKE ? AND cus_type LIKE ? "
            " AND pinyin_code LIKE ? ORDER BY cus_code ASC";
    childItemProperties listItem[] = {
            { "id","ID","ID", 0,60, true, true },
            { "cus_code","�ͻ�����","�ͻ�����", 1,80, true, true },
            { "cus_name","�ͻ�����","�ͻ�����", 2,180, true, false },
            { "cus_alias","�ͻ�����","�ͻ�����", 3,80, true, false },
            { "cus_type","�ͻ�����","�ͻ�����", 4, 120,true, true },
            { "pinyin_code","ƴ������","ƴ������", 5,80, true, false },
            { "area_city","����","����", 6,60, true, false },
            { "trade","������ҵ","������ҵ", 7,60, true, false },
            { "address","��ַ","��ַ", 8,100, true, false },
            { "post","��������","��������", 9,60, true, false },
            { "cus_reg_code","��˰�˵ǼǺ�","��˰�˵ǼǺ�", 10,100, true, false },
            { "cus_bank","��������","��������", 11,100, true, false },
            { "cus_account","�����˺�","�����˺�", 12,100, true, false },
            { "corporate","����","����", 13,100, true, false },
            { "licence_sdate","Ӫҵִ����Ч����","Ӫҵִ����Ч����", 14,100, true, false },
            { "licence_edate","Ӫҵִ�յ�������","Ӫҵִ�յ�������", 15,100, true, false },
            { "dev_date","��չ����","��չ����", 16,100, true, false },
            { "email","Email��ַ","Email��ַ", 17,100, true, false },
            { "contacts","��ϵ��","��ϵ��", 18,100, true, false },
            { "tel","�绰","�绰", 19,100, true, false },
            { "fax","����","����", 20,100, true, false },
            { "phone","�ֻ�","�ֻ�", 21,100, true, false },
        { "cus_address","������ַ","������ַ", 22,120, true, false },
        { "cus_person","ר��ҵ��Ա����","ר��ҵ��Ա����", 23,80, true, false },
        { "is_credit","�Ƿ�������ö��","�Ƿ�������ö��", 24,80, true, false },
        { "cus_cre_grade","���õȼ�","���õȼ�", 25,80, true, false },
        { "cus_cre_line","���ö��","���ö��", 26,80, true, false },
        { "cus_cre_date","��������","��������", 27,80, true, false },
        { "is_shop","�Ƿ��ŵ�","�Ƿ��ŵ�", 28,80, true, false },

        { "memo","��ע","��ע", 29,180, true, false }
    };

    int size = sizeof(listItem) / sizeof(listItem[0]);
    for(int i = 0 ; i < size ; i++){
        mListChildItem << listItem[i];
    }
    refreshData();
    connect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT (sortByColumn(int)));

}


void CusForm::sortByColumn(int column){
    //ui->tableView->sortByColumn(column,Qt::DescendingOrder);
    //ui->tableView->model()->sort(column,Qt::DescendingOrder);

}
void CusForm::limitsInit(){
    BaPlugin::authority(ui->gridLayout,mWinId.toInt(),mWinTitle);
}
void CusForm::setButtonEnabled(bool b){

    if(b){
        ui->editButton->setEnabled(b);
        ui->delButton->setEnabled(b);

    }else{
        ui->editButton->setEnabled(false);

        ui->delButton->setEnabled(false);
    }
    limitsInit();
}

void CusForm::setInputEnabled(bool b){

}
void CusForm::refreshData(){

    QVector<QVariant> v;
    v.append(mTypeCode + "%");
    sQuery(mSql1,v);
    for (int i = 0; i < mListChildItem.size(); i++){
        if(!mListChildItem[i].visible)ui->tableView->setColumnHidden(mListChildItem[i].id,true);
        ui->tableView->setColumnWidth(mListChildItem[i].id,mListChildItem[i].width);

    }

}

QSqlQuery CusForm::sqlQuery(QString sql,QVector<QVariant> qvar){
    QSqlQuery query(DBManager::getDBManager()->getGoinDBConnection());
    //query.setForwardOnly(true);
    query.prepare(sql);

    for(int i =0 ; i < qvar.size(); i++ ){
        query.addBindValue(qvar[i]);
    }

    if(!query.exec()){
       QSqlError error = query.lastError() ;
       Log4Qt::Logger *logger = Log4Qt::Logger::rootLogger();
       logger->error(sql);
       logger->error(error.text());
       QMessageBox::critical(this,"����",error.text());
    }else{
       //model->setQuery(query);
       //model->setHeaderData(0, Qt::Horizontal, tr("id"));
    }
    return query;
}
void CusForm::on_newButton_clicked()
{
    CusDialog *cusDlg = new CusDialog(addFlag,mTypeCode,this);
    if(cusDlg->exec()== QDialog::Accepted){
        QVector<QVariant> v;
        v.append(mTypeCode + "%");
        sQuery(mSql1,v);
    }
    //д�������־
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::baSubId().toInt(),mWinId.toInt(),"newButton");

}

void CusForm::on_editButton_clicked()
{
    CusDialog *cusDlg = new CusDialog(editFlag,mId,this);
    if(cusDlg->exec()== QDialog::Accepted){
        QVector<QVariant> v;
        v.append(mTypeCode + "%");
        sQuery(mSql1,v);
    }
    //д�������־
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::baSubId().toInt(),mWinId.toInt(),"editButton");


}

void CusForm::on_delButton_clicked()
{
    //д�������־
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::baSubId().toInt(),mWinId.toInt(),"delButton");

    if(mId.length() < 1){
        QMessageBox::information(this,"��ʾ","û��ѡ����,����ɾ��",QMessageBox::Ok);
        ui->editButton->setEnabled(false);
        ui->delButton->setEnabled(false);

        return ;
    }

    if(BaseDAO::getRdByCus(mCusCode.trimmed()) > 0){
        QMessageBox::information(this,"��ʾ","�Ѳ���ҵ������,����ɾ��",QMessageBox::Ok);
        ui->delButton->setEnabled(false);
        return ;
    }
    CustomerBean *bean = CustomerDAO::getCusById(mId);

    if(QMessageBox::warning(this, "��ʾ","�Ƿ�ɾ���˿ͻ�:" + bean->cusName() ,QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){

        bean->setId(mId.toInt());
        int result = CustomerDAO::delCus(bean);
        if(result > 0){
            QVector<QVariant> v;
            v.append(mTypeCode + "%");
            sQuery(mSql1,v);
            ui->editButton->setEnabled(false);
            ui->delButton->setEnabled(false);
            mId = "";
            ui->delButton->setEnabled(false);
            ui->editButton->setEnabled(false);

        }else{
            QMessageBox::critical(this,"����","ɾ������",QMessageBox::Ok);
        }

    }
    delete bean;
}

void CusForm::on_cancelButton_clicked()
{
    HaoKeApp::getInstance()->gui()->removeSubWin(this);
    this->close();
}

void CusForm::on_queryButton_clicked()
{
    QVector<QVariant> v;
    QString type="";
    QStringList l = ui->typeComboBox->currentText().split("-");
    if(l.length()>0){
        type=l.at(0).trimmed();
    }
    v.append("%" + ui->nameEdit->text().trimmed() + "%");
    v.append(type + "%");
    v.append("%" + ui->pinyinEdit->text() + "%");
    sQuery(mSql2,v);
}

void CusForm::sQuery(QString sql,QVector<QVariant> qvar)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QSqlQuery query(DBManager::getDBManager()->getGoinDBConnection());
    //query.setForwardOnly(true);
    query.prepare(sql);

    for(int i =0 ; i < qvar.size(); i++ ){
        query.addBindValue(qvar[i]);
    }

    if(!query.exec()){
       QSqlError error = query.lastError() ;
       Log4Qt::Logger *logger = Log4Qt::Logger::rootLogger();
       logger->error(sql);
       logger->error(error.text());
       QMessageBox::critical(this,"����",error.text());
    }{
        QSqlQueryModel *sqlModel = new QSqlQueryModel(ui->tableView);
        sqlModel->setQuery(query);
        for (int i = 0; i < mListChildItem.size(); i++){
            sqlModel->setHeaderData(i,Qt::Horizontal,mListChildItem[i].cnName);
        }
        sqlModel->sort(6,Qt::DescendingOrder);
        QSortFilterProxyModel *sortModel = new QSortFilterProxyModel(ui->tableView);
        sortModel->setSourceModel(sqlModel);
        ui->tableView->setModel(sortModel);
    }
    QApplication::restoreOverrideCursor();
}

void CusForm::on_treeView_doubleClicked(const QModelIndex &index)
{

}

void CusForm::on_treeView_clicked(const QModelIndex &index)
{
    QString str = ui->treeView->model()->data(index).toString();
    mTypeCode = str.mid(1,str.indexOf(")")-1);
    QVector<QVariant> v;
    v.append(mTypeCode + "%");
    sQuery(mSql1,v);
    ui->editButton->setEnabled(false);
    ui->delButton->setEnabled(false);
}

void CusForm::on_tableView_clicked(const QModelIndex &index)
{
    mId = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toString();
    mCusCode = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),1)).toString();
    ui->editButton->setEnabled(true);
    ui->delButton->setEnabled(true);
    limitsInit();
}

void CusForm::on_tableView_doubleClicked(const QModelIndex &index)
{
    if(LimitsCommon::getUserRoleLimits(HaoKeApp::getInstance()->getUserId(),"editButton",mWinId.toInt())){
        CusDialog *cusDlg = new CusDialog(editFlag,mId,this);
        if(cusDlg->exec()== QDialog::Accepted){

            QVector<QVariant> v;
            v.append(mTypeCode + "%");
            sQuery(mSql1,v);

        }
    }
}
