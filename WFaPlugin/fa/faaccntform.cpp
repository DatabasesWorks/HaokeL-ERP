#include "faaccntform.h"
#include "ui_faaccntform.h"
#include "widget/foottablemodel.h"
#include "widget/hierarchicalheaderview.h"
#include "wfaplugin.h"
#include "haokeapp.h"
#include "dao/faaccntdao.h"
#include "dao/rddao.h"
#include <QTableView>
#include "recoveraccountdialog.h"

FaAccntForm::FaAccntForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaAccntForm)
{
    ui->setupUi(this);
    init();
}

FaAccntForm::~FaAccntForm()
{
    delete ui;
}

void FaAccntForm::init(){
    //����ID��
    mWinId = HaoKeApp::faSubId() + WFaPlugin::faBillACCNTWinId();
    mWinTitle = "���ݼ���";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,mWinTitle,HaoKeApp::faSubId().toInt(),mWinId.toInt(),0,"");

    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/image/cancel_48.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->cancelBtn->setIcon(icon1);
    ui->cancelBtn->setIconSize(QSize(24, 24));
    ui->cancelBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/image/search_48.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->queryBtn->setIcon(icon2);
    ui->queryBtn->setIconSize(QSize(24, 24));
    ui->queryBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/image/page_accept.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->AccntBtn->setIcon(icon3);
    ui->AccntBtn->setIconSize(QSize(24, 24));
    ui->AccntBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/image/del.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->AccntCancelBtn->setIcon(icon4);
    ui->AccntCancelBtn->setIconSize(QSize(24, 24));
    ui->AccntCancelBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QDate oneDate = QDate(HaoKeApp::getInstance()->getOperateDate().year(),HaoKeApp::getInstance()->getOperateDate().month(),1);
    ui->dateEdit->setDate(oneDate);
    ui->dateEdit1->setDate(HaoKeApp::getInstance()->getOperateDate());
    mAccntMonth = HaoKeApp::getInstance()->getOperateDate().month();
    ui->AccntMonthLabel->setText(QString::number(mAccntMonth) + "��");
    FaWayBean *bean = FaAccntDAO::getFayWay();
    mWayId = 0;
    if(bean && bean->wayName().length() > 1){
        mWayName = bean->wayName();
        mWayId = bean->wayId();
        QString htmlText = "<html><head/><body><p><span style=' font-weight:600; color:#ff0000;'>"+ bean->wayName() +"</span></p></body></html>";
        ui->ACCNTLabel->setText(htmlText);
    }
    mHNote =  "<html><head/><body><p><span style=' font-weight:600; color:#ff0000;'> %1 </span></p></body></html>";
    if(HaoKeApp::getInstance()->getUserName()=="admin"){
        ui->AccntCancelBtn->setVisible(true);
    }else{
        ui->AccntCancelBtn->setVisible(false);
    }
    intiHeadData();
}

void FaAccntForm::intiHeadData(){
    childItemProperties childItem[] = {
        { "rd_no","�շ�����","�շ�����", 0,100, true, true },
        { "bus_date","�շ�����","�շ�����", 1,80, true, true },
        { "rd_flag","�շ����","�շ����", 2,60, true, false },
        { "wh_name","�ֿ�","�ֿ�",3,80, true, true },
        { "product_code","����","��Ʒ",4,80, true, true },
        { "product_name","����","��Ʒ",5,100, true, true },
        { "product_style","���","��Ʒ",6,80, true, false },
        { "unit","��λ","��λ", 7,60, true, false },
        { "quantity","����","����", 8, 60,true, true },
        { "unit_price","�շ�����","�շ�����", 9,80, true, true },
        { "money","�շ����","�շ����", 10,80, true, false },
        { "note","˵��","˵��", 11,160, true, false }

    };
    //QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        mListChildItem << childItem[i];
    }

    mListModel = new QStandardItemModel(0, mListChildItem.size(), ui->tableView);
    int headColumn = 0;
    FootTableModel *footModel = new FootTableModel(1,mListChildItem.size(),this);
    HierarchicalHeaderView* hv = new HierarchicalHeaderView(Qt::Horizontal, ui->tableView);
    bool b = true;

    for (int i = 0; i < mListChildItem.size(); i++){
        //mListModel->setHeaderData(mListChildItem[i].id, Qt::Horizontal, mListChildItem[i].cnName);
        if(i > 0 && mListChildItem[i].title != mListChildItem[i-1].title)
            b = true;

        if(mListChildItem[i].cnName != mListChildItem[i].title && b){
            QStandardItem *rootItem = new QStandardItem(mListChildItem[i].title);
            QList<QStandardItem*> l;
            for (int j = 0; j < mListChildItem.size(); j++){
                if(mListChildItem[i].title == mListChildItem[j].title){
                    QStandardItem* rotatedTextCell = new QStandardItem(mListChildItem[j].cnName);

                    l.push_back(rotatedTextCell);
                    rootItem->appendColumn(l);

                    l.clear();
                }
            }
            footModel->setItem(headColumn,rootItem);
            headColumn = headColumn + 1;
            b = false;
        }else if(mListChildItem[i].cnName == mListChildItem[i].title) {

            footModel->setItem(headColumn,new QStandardItem(mListChildItem[i].cnName));
            headColumn = headColumn + 1;
            b = true;
        }


    }
    hv->setModel(footModel);

    ui->tableView->setModel(mListModel);
    ui->tableView->setHorizontalHeader(hv);
    for (int i = 0; i < mListChildItem.size(); i++){
        if(!mListChildItem[i].visible)ui->tableView->setColumnHidden(mListChildItem[i].id,true);
        ui->tableView->setColumnWidth(mListChildItem[i].id,mListChildItem[i].width);
    }
}
void FaAccntForm::on_queryBtn_clicked()
{
    sQuery();
}

void FaAccntForm::on_cancelBtn_clicked()
{
    HaoKeApp::getInstance()->gui()->removeSubWin(this);
    this->close();
}

void FaAccntForm::on_AccntBtn_clicked()
{
    if(ui->dateEdit->date().month()!=mAccntMonth
            || ui->dateEdit1->date().month()!=mAccntMonth){
        QMessageBox::information(this,"��ʾ","��ѡ��ǰ�����·����ڼ���");
        return;
    }

    if(mWayId < 1){
        QMessageBox::information(this,"��ʾ","�Ǽ۷�ʽ����ȷ,���ܼ��ʴ���!");
        return;
    }
    sQuery();
    if(ui->tableView->model()->rowCount() < 1){
        QMessageBox::information(this,"��ʾ","�޼��ʵ���");
        return;
    }
    if(QMessageBox::warning(this,"��ʾ","�Ƿ��" + QString::number(mAccntMonth)+ "�µ��ݼ��ʣ�",QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes){
        QString sql = " SELECT count(*) "
           " FROM rd_record a,rd_records b "
           " WHERE a.bus_date<'" + ui->dateEdit->date().toString("yyyy-MM-dd") + "' "
           " AND a.rd_no=b.rd_no "
           " AND b.is_account=false";
        int result = FaAccntDAO::getSqlCount(sql);
        if(result > 0){
            QMessageBox::information(this,"��ʾ",ui->dateEdit->date().toString("yyyy-MM-dd") +"֮ǰ�е���δ���ʣ����ܼ���");
            return;
        }
        sql = " SELECT count(*) "
           " FROM rd_record a,rd_records b "
           " WHERE a.bus_date>='" + ui->dateEdit->date().toString("yyyy-MM-dd") + "' "
           " AND a.bus_date<='" + ui->dateEdit1->date().toString("yyyy-MM-dd") + "' "
           " AND a.rd_no=b.rd_no "
           " AND a.is_handle=false ";
        result = FaAccntDAO::getSqlCount(sql);
        QString rdSql = " SELECT * "
                " FROM rd_record a "
                " WHERE a.bus_date>='" + ui->dateEdit->date().toString("yyyy-MM-dd") + "' "
                " AND a.bus_date<='" + ui->dateEdit1->date().toString("yyyy-MM-dd") + "' "
                " AND a.is_handle=false ";
        QList<RdBean *> listBean = FaAccntDAO::queryRdList(rdSql);
        if(result > 0 && listBean.size() > 0){

            QMessageBox::information(this,"��ʾ",listBean.at(0)->rdNo() +"����δ��ˣ����ܼ���");
            return;
        }
        rdSql = " SELECT * "
                        " FROM rd_record a "
                        " WHERE a.bus_date>='" + ui->dateEdit->date().toString("yyyy-MM-dd") + "' "
                        " AND a.bus_date<='" + ui->dateEdit1->date().toString("yyyy-MM-dd") + "' "
                        " AND a.is_bill=false AND rd_flag=1 AND vouch_type='01'";
        listBean = FaAccntDAO::queryRdList(rdSql);
        if(listBean.size() > 0){

            QMessageBox::information(this,"��ʾ",listBean.at(0)->rdNo() +"�ɹ�����δ���㣬����ȡ���ɹ�ʵ�ʳɱ������ܼ���");
            return;
        }
        if(mWayId == 1){
            FIFOCostACCOUNT();
        }else if(mWayId == 2){
            MACostACCOUNT();
        }else{

        }

    }
}

void FaAccntForm::MACostACCOUNT(){
    //-- ���ʿ�ʼ
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->AccntBtn->setEnabled(false);

    Log4Qt::PropertyConfigurator::configure(QApplication::applicationDirPath() + "/log4j.properties");
    Log4Qt::Logger *logger = Log4Qt::Logger::rootLogger();
    logger->debug(QObject::tr("���ʿ�ʼ ..."));
    QSqlQuery query;
    QVector<QVariant> v;
    v.append(ui->dateEdit->date().toString("yyyy-MM-dd"));
    v.append(ui->dateEdit1->date().toString("yyyy-MM-dd"));
    query = sqlQuery(mStrSql,v);
    int i = 0;
    while(query.next()){
        QSqlRecord sqlResult = query.record();
        QString whCode = sqlResult.value("wh_code").toString();
        QString pCode = sqlResult.value("product_code").toString();
        QString rdNo = sqlResult.value("rd_no").toString();
        int vouchsId= sqlResult.value("vouchs_id").toInt();
        RdBean *rBean = RdDAO::getRdByNo(rdNo);
        RdsBean *rdsBean = RdDAO::getRdsBy(QString::number(vouchsId));
        QDateTime time = QDateTime::currentDateTime();
        if(rBean && rBean->rdNo().length() > 1){
            rBean->setAccounter(HaoKeApp::getInstance()->getUserDesc());
            rBean->setKeepTime(time);
        }else{
            QMessageBox::information(this,"��ʾ",rdNo +"���ݲ����ڣ�������ֹ��");
            return ;
        }

        FaSubsidiaryBean *bean = new FaSubsidiaryBean();
        bean->setVouchNo(sqlResult.value("rd_no").toString());
        bean->setVouchSource(sqlResult.value("vouch_source").toString());
        bean->setVouchType(sqlResult.value("vouch_type").toString());
        bean->setRdFlag(sqlResult.value("rd_flag").toInt());
        bean->setBusNo(sqlResult.value("bus_no").toString());
        bean->setBusType(sqlResult.value("bus_type").toString());
        bean->setBusDate(sqlResult.value("bus_date").toDate());
        if(bean->rdFlag()== 1){
            bean->setPzDigest("����");
        }else if(bean->rdFlag()== 2){
            bean->setPzDigest("����");
        }
        bean->setBatchId("");
        bean->setSettleNo(sqlResult.value("bill_no").toString());
        bean->setOrderNo("");
        bean->setDvNo("");
        bean->setPzId("");
        bean->setBatchAmount(0);
        bean->setDvId(0);
        bean->setPzDate(QDate::currentDate());
        bean->setKeepDate(HaoKeApp::getInstance()->getOperateDate());
        bean->setMonth(mAccntMonth);
        bean->setWhCode(sqlResult.value("wh_code").toString());
        bean->setDepCode(sqlResult.value("dep_code").toString());
        bean->setPersonCode(sqlResult.value("person_code").toString());
        bean->setPtCode(sqlResult.value("pt_code").toString());
        bean->setStCode(sqlResult.value("st_code").toString());
        bean->setCusCode(sqlResult.value("cus_code").toString());
        bean->setSupCode(sqlResult.value("supplier_id").toString());
        bean->setProductCode(sqlResult.value("product_code").toString());
        bean->setVouchsId(sqlResult.value("vouchs_id").toInt());

        bean->setAccounter(HaoKeApp::getInstance()->getUserDesc());
        bean->setCreated(time);
        bean->setCreatedBy(HaoKeApp::getInstance()->getUserId());
        bean->setCreater(HaoKeApp::getInstance()->getUserDesc());

        //���ݲֿ�����Ʒ�����ѯ���Ľ������������
        QList<FaSubsidiaryBean *> bottomListFa = FaAccntDAO::queryFaSubsidiaryByBottom(whCode,pCode);
        double lastQuantity = 0; //�������
        double lastMoney = 0;    //�����
        double lastCost = 0;     //�ƶ�ƽ���ɱ�����
        if(bottomListFa.size() > 0){
            lastQuantity = bottomListFa.at(0)->stQuantity();
            lastMoney = bottomListFa.at(0)->stMoney();
            lastCost = bottomListFa.at(0)->stMoney()/bottomListFa.at(0)->stQuantity();
        }

        //��������ɱ�������

        //�ƶ�ƽ������ --��ʼ
        logger->debug("�ƶ�ƽ������ --��ʼ ����:" + rdNo + " ��Ʒ����:" + pCode);
        if(bean->rdFlag() == 1){
            //�ƶ�ƽ�� ���ɱ�����
            if(bean->vouchType() == "01"){
                //��ͨ�ɹ� ȡ�ɹ����㵥����
                QString settleSql = "select * from pu_settle_vouchs where rd_no='" + rdNo
                        + "' AND product_code='" + pCode + "'";
                QSqlQuery settleQuery(DBManager::getDBManager()->getGoinDBConnection());
                settleQuery.setForwardOnly(true);
                if(settleQuery.exec(settleSql)){
                    DBManager::getDBManager()->getGoinDBConnection().transaction();
                    int syResult = RdDAO::updateRd(rBean);
                    int oResult = 0 ;
                    int settle_i = 0;
                    double total_quantity = 0;
                    double total_money = 0;
                    while(settleQuery.next()){
                        QSqlRecord settleSqlResult = settleQuery.record();
                        bean->setSettleNo(settleSqlResult.value("settle_no").toString());
                        bean->setInQuantity(settleSqlResult.value("quantity").toDouble());
                        bean->setInCost(settleSqlResult.value("s_price").toDouble());
                        bean->setInMoney(bean->inQuantity()*bean->inCost());
                        total_quantity = total_quantity + bean->inQuantity();
                        total_money = total_money + bean->inMoney();
                        bean->setOutQuantity(0);
                        bean->setOutCost(0);
                        bean->setOutMoney(0);
                        bean->setOutPrice(0);
                        bean->setStQuantity(lastQuantity + total_quantity);
                        bean->setStMoney(lastMoney + total_money);
                        int result = FaAccntDAO::createSubsidiary(bean);
                        oResult = oResult + result ;
                        settle_i = settle_i + 1;
                    }
                    if(total_quantity != sqlResult.value("quantity").toDouble()){
                        DBManager::getDBManager()->getGoinDBConnection().rollback();
                        QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʳ����ɹ���������˲ɹ����δ������";
                        QString note = mHNote;
                        ui->noteLabel->setText(note.arg(err_note));
                        ui->AccntBtn->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        logger->error(err_note);
                        logger->error("total_quantity:" + QString::number(total_quantity)+ "  quantity:"+ QString::number(sqlResult.value("quantity").toDouble()));
                        QMessageBox::critical(this,"����",rdNo + "��ⵥ�ݼ��ʴ���,�ɹ��������������ֹ��");
                        return ;
                    }
                    rdsBean->setIsAccount(true);
                    int rdsResult = RdDAO::updateRds(rdsBean);
                    if(oResult == settle_i && syResult > 0 && rdsResult>0 ){
                        DBManager::getDBManager()->getGoinDBConnection().commit();
                        QAbstractItemModel *model = ui->tableView->model();
                        model->setData(model->index(i,11),rdNo + " ���ʳɹ�");
                        i = i + 1;
                    }else{
                        DBManager::getDBManager()->getGoinDBConnection().rollback();
                        QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʳ���";
                        QString note = mHNote;
                        ui->noteLabel->setText(note.arg(err_note));
                        ui->AccntBtn->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        logger->error(err_note);
                        QMessageBox::critical(this,"����",rdNo + "�ɹ���ⵥ�ݼ��ʴ��󣬼�����ֹ��");
                        return ;
                    }
                }else{
                    QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʴ���,�ɹ�����δ���� " + settleSql+ "";
                    QString note = mHNote;
                    ui->noteLabel->setText(note.arg(err_note));
                    ui->AccntBtn->setEnabled(true);
                    QApplication::restoreOverrideCursor();
                    logger->error(err_note);
                    logger->error(settleSql);
                    QMessageBox::critical(this,"����",rdNo + "���ݼ��ʴ���,�ɹ�����δ����,������ֹ��");
                    return ;
                }
            }else{
                //����ڳ� ������� �������  ��ȡ�ɹ����㵥
                if(bean->vouchType() == "05" && bean->busType() == "�������"){
                    //������� ȡԭ�����ֿ�ɱ�����, bean->busNo() Ϊ��������
                    QString allcSql = "select * from st_trans_vouch where tv_no='" + sqlResult.value("bus_no").toString().trimmed()
                            + "'";
                    QSqlQuery allcQuery(DBManager::getDBManager()->getGoinDBConnection());
                    allcQuery.setForwardOnly(true);
                    lastCost = 0;
                    bool trans_read = true;
                    if(allcQuery.exec(allcSql)){

                        if(allcQuery.next()){
                            QSqlRecord allcSqlResult = allcQuery.record();
                            whCode = allcSqlResult.value("owh_code").toString();
                            bottomListFa = FaAccntDAO::queryFaSubsidiaryByBottom(whCode,pCode);
                            if(bottomListFa.size() > 0){
                                lastCost = bottomListFa.at(0)->stMoney()/bottomListFa.at(0)->stQuantity();
                                bean->setInCost(lastCost);
                                trans_read = false;
                            }else{
                                logger->error("������� ȡԭ�����ֿ�ɱ����� û�д�:"+ whCode + " �ֿ�Ĵ��" + pCode) ;

                            }

                        }else{
                            logger->error("������� ȡԭ�����ֿ�ɱ����� �޴˵�����:"+ sqlResult.value("bus_no").toString()) ;
                            logger->error(" SQL:" + allcSql );
                        }
                    }else{
                        logger->error("err_note:" + allcSql );
                    }
                    if(lastCost == 0 && trans_read ){
                        QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʴ���,�������ɱ�δȡ��,��鿴�˵������Ƿ����";
                        QString note = mHNote;
                        ui->noteLabel->setText(note.arg(err_note));
                        ui->AccntBtn->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        logger->error(err_note);
                        QMessageBox::critical(this,"����",rdNo + "���ݼ��ʴ���,�������ɱ�δȡ��,������ֹ��");
                        return ;
                    }
                }else{
                    //����ڳ� ������� ȡԭ���ݵ���Ϊ���ɱ�����
                    bean->setInCost(sqlResult.value("unit_price").toDouble());

                }
                bean->setInQuantity(sqlResult.value("quantity").toDouble());
                bean->setInMoney(bean->inQuantity()*bean->inCost());
                bean->setOutQuantity(0);
                bean->setOutCost(0);
                bean->setOutMoney(0);
                bean->setOutPrice(0);
                bean->setStQuantity(lastQuantity + bean->inQuantity());
                bean->setStMoney(lastMoney + bean->inQuantity()*bean->inCost());
                DBManager::getDBManager()->getGoinDBConnection().transaction();
                int syResult = RdDAO::updateRd(rBean);
                int result = FaAccntDAO::createSubsidiary(bean);
                rdsBean->setIsAccount(true);
                int rdsResult = RdDAO::updateRds(rdsBean);
                if(result > 0 && syResult > 0 && rdsResult > 0){
                    DBManager::getDBManager()->getGoinDBConnection().commit();
                    QAbstractItemModel *model = ui->tableView->model();
                    model->setData(model->index(i,11),rdNo + " ���ʳɹ�");
                    i = i + 1;
                }else{
                    DBManager::getDBManager()->getGoinDBConnection().rollback();
                    QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʳ���";
                    QString note = mHNote;
                    ui->noteLabel->setText(note.arg(err_note));
                    ui->AccntBtn->setEnabled(true);
                    QApplication::restoreOverrideCursor();
                    logger->error(err_note);
                    QMessageBox::critical(this,"����",rdNo + "��ⵥ�ݼ��ʴ��󣬼�����ֹ��");
                    return ;
                }
            }

        }else{
            //�ƶ�ƽ�� ����ɱ�����
            bean->setOutQuantity(sqlResult.value("quantity").toDouble());
            bean->setOutCost(lastCost);
            bean->setOutMoney(bean->outQuantity()*bean->outCost());
            bean->setOutPrice(sqlResult.value("unit_price").toDouble());
            bean->setInQuantity(0);
            bean->setInCost(0);
            bean->setInMoney(0);
            bean->setStQuantity(lastQuantity - bean->outQuantity());
            bean->setStMoney(lastMoney - bean->outQuantity()*bean->outCost());
            DBManager::getDBManager()->getGoinDBConnection().transaction();
            int syResult = RdDAO::updateRd(rBean);
            int result = FaAccntDAO::createSubsidiary(bean);
            rdsBean->setIsAccount(true);
            int rdsResult = RdDAO::updateRds(rdsBean);
            if(result > 0 && syResult > 0 && rdsResult > 0){
                DBManager::getDBManager()->getGoinDBConnection().commit();
                QAbstractItemModel *model = ui->tableView->model();
                model->setData(model->index(i,11),rdNo + " ���ʳɹ�");
                i = i + 1;
            }else{
                DBManager::getDBManager()->getGoinDBConnection().rollback();
                QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʳ���";
                QString note = mHNote;
                ui->noteLabel->setText(note.arg(err_note));
                ui->AccntBtn->setEnabled(true);
                QApplication::restoreOverrideCursor();
                logger->error(err_note);
                QMessageBox::critical(this,"����",rdNo + "���ⵥ�ݼ��ʴ��󣬼�����ֹ��");
                return ;
            }
        }
        //�ƶ�ƽ������ --����
        logger->debug("�ƶ�ƽ������ --���� ����:" + rdNo + " ��Ʒ����:" + pCode);
    }

    //-- ���ʽ���
    logger->debug(QObject::tr("���ʽ��� "));
    QString note = mHNote;
    ui->noteLabel->setText(note.arg(QString::number(i) + " ���ݳɹ�����"));
    ui->AccntBtn->setEnabled(true);
    QApplication::restoreOverrideCursor();

}

void FaAccntForm::FIFOCostACCOUNT(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->AccntBtn->setEnabled(false);
    Log4Qt::PropertyConfigurator::configure(QApplication::applicationDirPath() + "/log4j.properties");
    Log4Qt::Logger *logger = Log4Qt::Logger::rootLogger();
    logger->debug(QObject::tr("���ʿ�ʼ ..."));
    QSqlQuery query;
    QVector<QVariant> v;
    v.append(ui->dateEdit->date().toString("yyyy-MM-dd"));
    v.append(ui->dateEdit1->date().toString("yyyy-MM-dd"));
    query = sqlQuery(mStrSql,v);
    int i = 0;
    QList<FaSubsidiaryBean *> trListFa; //�����������¼�б�
    while(query.next()){
        QSqlRecord sqlResult = query.record();
        QString whCode = sqlResult.value("wh_code").toString();
        QString pCode = sqlResult.value("product_code").toString();
        QString rdNo = sqlResult.value("rd_no").toString();
        int vouchsId= sqlResult.value("vouchs_id").toInt();
        RdBean *rBean = RdDAO::getRdByNo(rdNo);
        RdsBean *rdsBean = RdDAO::getRdsBy(QString::number(vouchsId));
        QDateTime time = QDateTime::currentDateTime();
        if(rBean && rBean->rdNo().length() > 1){
            rBean->setAccounter(HaoKeApp::getInstance()->getUserDesc());
            rBean->setKeepTime(time);
        }else{
            QMessageBox::information(this,"��ʾ",rdNo +"���ݲ����ڣ�������ֹ��");
            return ;
        }

        FaSubsidiaryBean *bean = new FaSubsidiaryBean();
        bean->setVouchNo(sqlResult.value("rd_no").toString());
        bean->setVouchSource(sqlResult.value("vouch_source").toString());
        bean->setVouchType(sqlResult.value("vouch_type").toString());
        bean->setRdFlag(sqlResult.value("rd_flag").toInt());
        bean->setBusNo(sqlResult.value("bus_no").toString());
        bean->setBusType(sqlResult.value("bus_type").toString());
        bean->setBusDate(sqlResult.value("bus_date").toDate());
        bean->setBatchId("");
        bean->setSettleNo(sqlResult.value("bill_no").toString());
        bean->setOrderNo("");
        bean->setDvNo("");
        bean->setPzId("");
        bean->setBatchAmount(0);
        bean->setDvId(0);
        bean->setPzDate(QDate::currentDate());
        if(bean->rdFlag()== 1){
            bean->setPzDigest("����");
        }else if(bean->rdFlag()== 2){
            bean->setPzDigest("����");
        }
        bean->setKeepDate(HaoKeApp::getInstance()->getOperateDate());
        bean->setMonth(mAccntMonth);
        bean->setWhCode(sqlResult.value("wh_code").toString());
        bean->setDepCode(sqlResult.value("dep_code").toString());
        bean->setPersonCode(sqlResult.value("person_code").toString());
        bean->setPtCode(sqlResult.value("pt_code").toString());
        bean->setStCode(sqlResult.value("st_code").toString());
        bean->setCusCode(sqlResult.value("cus_code").toString());
        bean->setSupCode(sqlResult.value("supplier_id").toString());
        bean->setProductCode(sqlResult.value("product_code").toString());
        bean->setVouchsId(sqlResult.value("vouchs_id").toInt());

        bean->setAccounter(HaoKeApp::getInstance()->getUserDesc());
        bean->setCreated(time);
        bean->setCreatedBy(HaoKeApp::getInstance()->getUserId());
        bean->setCreater(HaoKeApp::getInstance()->getUserDesc());

        //���ݲֿ�����Ʒ�����ѯ���Ľ������������
        QList<FaSubsidiaryBean *> bottomListFa = FaAccntDAO::queryFaSubsidiaryByBottom(whCode,pCode);
        double lastQuantity = 0; //�������
        double lastMoney = 0;    //�����
        double lastCost = 0;     //�ƶ�ƽ���ɱ�����
        if(bottomListFa.size() > 0){
            lastQuantity = bottomListFa.at(0)->stQuantity();
            lastMoney = bottomListFa.at(0)->stMoney();
            lastCost = bottomListFa.at(0)->stMoney()/bottomListFa.at(0)->stQuantity();
        }

        //��������ɱ�������
        //�Ƚ��ȳ����� --��ʼ
        logger->debug("�Ƚ��ȳ����� --��ʼ ����:" + rdNo + " ��Ʒ����:" + pCode);
        if(bean->rdFlag() == 1){
            //�Ƚ��ȳ� ���ɱ�����
            if(bean->vouchType() == "01"){
                //��ͨ�ɹ� ȡ�ɹ����㵥����
                QString settleSql = "select * from pu_settle_vouchs where rd_no='" + rdNo
                        + "' AND product_code='" + pCode + "'";
                QSqlQuery settleQuery(DBManager::getDBManager()->getGoinDBConnection());
                settleQuery.setForwardOnly(true);
                if(settleQuery.exec(settleSql)){
                    DBManager::getDBManager()->getGoinDBConnection().transaction();
                    int syResult = RdDAO::updateRd(rBean);
                    int oResult = 0 ;
                    int settle_i = 0;
                    double total_quantity = 0;
                    double total_money = 0;
                    while(settleQuery.next()){
                        QSqlRecord settleSqlResult = settleQuery.record();
                        bean->setSettleNo(settleSqlResult.value("settle_no").toString());
                        bean->setInQuantity(settleSqlResult.value("quantity").toDouble());
                        bean->setInCost(settleSqlResult.value("s_price").toDouble());
                        bean->setInMoney(bean->inQuantity()*bean->inCost());
                        bean->setBatchAmount(bean->inQuantity());
                        total_quantity = total_quantity + bean->inQuantity();
                        total_money = total_money + bean->inMoney();
                        bean->setOutQuantity(0);
                        bean->setOutCost(0);
                        bean->setOutMoney(0);
                        bean->setOutPrice(0);
                        bean->setStQuantity(lastQuantity + total_quantity);
                        bean->setStMoney(lastMoney + total_money);
                        int result = FaAccntDAO::createSubsidiary(bean);
                        oResult = oResult + result ;
                        settle_i = settle_i + 1;
                    }
                    if(total_quantity != sqlResult.value("quantity").toDouble()){
                        DBManager::getDBManager()->getGoinDBConnection().rollback();
                        QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʳ����ɹ���������˲ɹ����δ������";
                        QString note = mHNote;
                        ui->noteLabel->setText(note.arg(err_note));
                        ui->AccntBtn->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        logger->error(err_note);
                        logger->error("total_quantity:" + QString::number(total_quantity)+ "  quantity:"+ QString::number(sqlResult.value("quantity").toDouble()));
                        QMessageBox::critical(this,"����",rdNo + "��ⵥ�ݼ��ʴ���,�ɹ��������������ֹ��");
                        return ;
                    }
                    rdsBean->setIsAccount(true);
                    int rdsResult = RdDAO::updateRds(rdsBean);
                    if(oResult == settle_i && syResult > 0 && rdsResult >0 ){
                        DBManager::getDBManager()->getGoinDBConnection().commit();
                        QAbstractItemModel *model = ui->tableView->model();
                        model->setData(model->index(i,11),rdNo + " ���ʳɹ�");
                        i = i + 1;
                    }else{
                        DBManager::getDBManager()->getGoinDBConnection().rollback();
                        QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʳ���";
                        QString note = mHNote;
                        ui->noteLabel->setText(note.arg(err_note));
                        ui->AccntBtn->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        logger->error(err_note);
                        QMessageBox::critical(this,"����",rdNo + "�ɹ���ⵥ�ݼ��ʴ��󣬼�����ֹ��");
                        return ;
                    }
                }else{
                    QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʴ���,�ɹ�����δ���� " + settleSql+ "";
                    QString note = mHNote;
                    ui->noteLabel->setText(note.arg(err_note));
                    ui->AccntBtn->setEnabled(true);
                    QApplication::restoreOverrideCursor();
                    logger->error(err_note);
                    logger->error(settleSql);
                    QMessageBox::critical(this,"����",rdNo + "���ݼ��ʴ���,�ɹ�����δ����,������ֹ��");
                    return ;
                }
            }else{
                if(bean->vouchType() == "14" ){
                    //�ɹ��˻� ȡ�ɹ����㵥����
                    QString settleSql = "select * from pu_settle_vouchs where rd_no='" + rdNo
                            + "' AND product_code='" + pCode + "'";
                    QSqlQuery settleQuery(DBManager::getDBManager()->getGoinDBConnection());
                    settleQuery.setForwardOnly(true);
                    //�ɹ��˻� ��������
                    //���ݲֿ�����Ʒȡ�ɳ���ļ��ʴ����ϸ��ⵥ���б�
                    QList<FaSubsidiaryBean *> listFIFOFaSub = FaAccntDAO::queryFIFOFaSubsidiaryBy(whCode,pCode);
                    int uResult = 0;
                    int fifo_i = 0;
                    if(settleQuery.exec(settleSql)){
                        DBManager::getDBManager()->getGoinDBConnection().transaction();
                        int syResult = RdDAO::updateRd(rBean);
                        int oResult = 0 ;
                        int settle_i = 0;
                        double total_quantity = 0;
                        double total_money = 0;
                        while(settleQuery.next()){
                            QSqlRecord settleSqlResult = settleQuery.record();
                            bean->setSettleNo(settleSqlResult.value("settle_no").toString());
                            bean->setInQuantity(settleSqlResult.value("quantity").toDouble());
                            bean->setInCost(settleSqlResult.value("s_price").toDouble());
                            bean->setInMoney(bean->inQuantity()*bean->inCost());
                            bean->setBatchAmount(0);
                            total_quantity = total_quantity + bean->inQuantity();
                            total_money = total_money + bean->inMoney();
                            bean->setOutQuantity(0);
                            bean->setOutCost(0);
                            bean->setOutMoney(0);
                            bean->setOutPrice(0);
                            bean->setStQuantity(lastQuantity + total_quantity);
                            bean->setStMoney(lastMoney + total_money);
                            double batch_out = bean->inQuantity();

                            //�˻� �������ν��
                            if(listFIFOFaSub.size() > 0 || batch_out < 0){
                                while( batch_out < 0  ){
                                    if(listFIFOFaSub.size()> fifo_i ){
                                        //������� ���γ�������
                                        double amount = listFIFOFaSub.at(fifo_i)->batchAmount();
                                        double over = listFIFOFaSub.at(fifo_i)->inQuantity() - listFIFOFaSub.at(fifo_i)->batchAmount();
                                        if(amount >= qAbs(batch_out)){
                                            bean->setDvId(listFIFOFaSub.at(fifo_i)->id());
                                            bean->setDvNo(listFIFOFaSub.at(fifo_i)->vouchNo());
                                            listFIFOFaSub.at(fifo_i)->setBatchAmount(amount + batch_out);
                                            uResult = FaAccntDAO::updateSubsidiary(listFIFOFaSub.at(fifo_i));
                                            fifo_i = fifo_i + 1;
                                            batch_out = 0 ;
                                        }else{
                                            bean->setDvId(listFIFOFaSub.at(fifo_i)->id());
                                            bean->setDvNo(listFIFOFaSub.at(fifo_i)->vouchNo());
                                            batch_out = batch_out + amount;
                                            listFIFOFaSub.at(fifo_i)->setBatchAmount(0);
                                            uResult = FaAccntDAO::updateSubsidiary(listFIFOFaSub.at(fifo_i));
                                            fifo_i = fifo_i + 1;
                                        }
                                    }else{
                                        DBManager::getDBManager()->getGoinDBConnection().rollback();
                                        ui->AccntBtn->setEnabled(true);
                                        QApplication::restoreOverrideCursor();
                                        QString error_text = rdNo + "(�Ƚ��ȳ�)���ⵥ�ݼ��ʴ���,��Ʒ" + bean->productCode()
                                                + " ��" + QString::number(fifo_i) + "�βɹ��˻�,�����ν��,���ܼ���,������ֹ��";
                                        logger->error(error_text);
                                        QString note = mHNote;
                                        ui->noteLabel->setText(note.arg(error_text));
                                        QMessageBox::critical(this,"����",error_text);
                                        return ;
                                    }
                                }
                            }
                            int result = FaAccntDAO::createSubsidiary(bean);
                            oResult = oResult + result ;
                            settle_i = settle_i + 1;
                        }
                        if(total_quantity != sqlResult.value("quantity").toDouble()){
                            DBManager::getDBManager()->getGoinDBConnection().rollback();
                            QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʳ����ɹ��˻���������˲ɹ����δ������";
                            QString note = mHNote;
                            ui->noteLabel->setText(note.arg(err_note));
                            ui->AccntBtn->setEnabled(true);
                            QApplication::restoreOverrideCursor();
                            logger->error(err_note);
                            logger->error("total_quantity:" + QString::number(total_quantity)+ "  quantity:"+ QString::number(sqlResult.value("quantity").toDouble()));
                            QMessageBox::critical(this,"����",rdNo + "��ⵥ�ݼ��ʴ���,�ɹ��˻��������������ֹ��");
                            return ;
                        }

                        rdsBean->setIsAccount(true);
                        int rdsResult = RdDAO::updateRds(rdsBean);
                        if(oResult == settle_i && syResult > 0 && rdsResult >0 ){
                            DBManager::getDBManager()->getGoinDBConnection().commit();
                            QAbstractItemModel *model = ui->tableView->model();
                            model->setData(model->index(i,11),rdNo + " ���ʳɹ�");
                            i = i + 1;
                        }else{
                            DBManager::getDBManager()->getGoinDBConnection().rollback();
                            QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʳ���";
                            QString note = mHNote;
                            ui->noteLabel->setText(note.arg(err_note));
                            ui->AccntBtn->setEnabled(true);
                            QApplication::restoreOverrideCursor();
                            logger->error(err_note);
                            QMessageBox::critical(this,"����",rdNo + "�ɹ�(�˻�)��ⵥ�ݼ��ʴ��󣬼�����ֹ��");
                            return ;
                        }
                    }else{
                        QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʴ���,�ɹ��˻�����δ���� " + settleSql+ "";
                        QString note = mHNote;
                        ui->noteLabel->setText(note.arg(err_note));
                        ui->AccntBtn->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        logger->error(err_note);
                        logger->error(settleSql);
                        QMessageBox::critical(this,"����",rdNo + "���ݼ��ʴ���,�ɹ��˻�����δ����,������ֹ��");
                        return ;
                    }
                }else if(bean->vouchType() == "05" && bean->busType() == "�������"){
                    //������� ��ȡ�ɹ����㵥 ȡ��������������ݵĳɱ�
                    trListFa.append(bean);
                    bean->setId(i);
                    i = i + 1;

                }else{
                    //����ڳ� ������� ȡԭ���ݵ���Ϊ���ɱ�����
                    bean->setInCost(sqlResult.value("unit_price").toDouble());
                    //�ָ�
                    bean->setInQuantity(sqlResult.value("quantity").toDouble());
                    bean->setInMoney(bean->inQuantity()*bean->inCost());
                    bean->setBatchAmount(bean->inQuantity());
                    bean->setOutQuantity(0);
                    bean->setOutCost(0);
                    bean->setOutMoney(0);
                    bean->setOutPrice(0);
                    bean->setStQuantity(lastQuantity + bean->inQuantity());
                    bean->setStMoney(lastMoney + bean->inQuantity()*bean->inCost());
                    DBManager::getDBManager()->getGoinDBConnection().transaction();
                    int syResult = RdDAO::updateRd(rBean);
                    int result = FaAccntDAO::createSubsidiary(bean);
                    rdsBean->setIsAccount(true);
                    int rdsResult = RdDAO::updateRds(rdsBean);
                    if(result > 0 && syResult > 0 && rdsResult>0 ){
                        DBManager::getDBManager()->getGoinDBConnection().commit();
                        QAbstractItemModel *model = ui->tableView->model();
                        model->setData(model->index(i,11),rdNo + " ���ʳɹ�");
                        i = i + 1;
                    }else{
                        DBManager::getDBManager()->getGoinDBConnection().rollback();
                        QString err_note = QString::number(i) + " ���ݼ���,���ݣ�" + rdNo  + " ���ʳ���";
                        ui->AccntBtn->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        QString note = mHNote;
                        ui->noteLabel->setText(note.arg(err_note));
                        logger->error(err_note);
                        QMessageBox::critical(this,"����",rdNo + "��ⵥ�ݼ��ʴ��󣬼�����ֹ��");
                        return ;
                    }

                }

            }

        }else{
            //�Ƚ��ȳ� ����ɱ�����
            //���ݲֿ�����Ʒȡ�ɳ���ļ��ʴ����ϸ��ⵥ���б�
            QList<FaSubsidiaryBean *> listFIFOFaSub = FaAccntDAO::queryFIFOFaSubsidiaryBy(whCode,pCode);
            //���γ�������
            double batch_out = sqlResult.value("quantity").toDouble();
            if(listFIFOFaSub.size() > 0 || batch_out < 0){
                bean->setInQuantity(0);
                bean->setInCost(0);
                bean->setInMoney(0);
                int fifo_i = 0;

                int syResult = 0;
                int result = 0;
                int uResult = 0;
                int oReuslt = 0;
                //�����˻���� batch_amount �޽��� ��ȡ������һ����¼
                if(batch_out < 0 ){
                    listFIFOFaSub = FaAccntDAO::queryFIFOFaSubsidiaryReByBottom(whCode,pCode);
                }

                // ****** start ����������ɱ� �������ȳ�����
                bool isLoadTr = false; // �Ƿ��ȡ��������ⵥ
                QString iRdNo = "";
                //���ݲֿ�����Ʒ�����ѯ���Ľ������������(����)
                QList<FaSubsidiaryBean *> bottomListTrFa;
                double lastTrQuantity = 0; //�������
                double lastTrMoney = 0;    //�����
                double lastTrCost = 0;     //�ƶ�ƽ���ɱ�����
                FaSubsidiaryBean *rInBean = new FaSubsidiaryBean();
                if(bean->vouchType() == "07" && bean->busType() == "��������"){
                    QString error_text = rdNo ;
                    QString allcSql = "select * from st_trans_vouch where tv_no='" + bean->busNo().trimmed() + "'";
                    QSqlQuery allcQuery(DBManager::getDBManager()->getGoinDBConnection());
                    QString trInWhCode = "";
                    allcQuery.setForwardOnly(true);
                    if(allcQuery.exec(allcSql)){
                        if(allcQuery.next()){
                            QSqlRecord allcSqlResult = allcQuery.record();
                            iRdNo = allcSqlResult.value("ird_no").toString();
                            trInWhCode = allcSqlResult.value("iwh_code").toString();
                            if(iRdNo.length() > 1){
                                for(int h = 0 ; h < trListFa.size() ; h ++){
                                    if(trListFa.at(h)->vouchNo() == iRdNo && trListFa.at(h)->productCode() == pCode){
                                        isLoadTr = true ;
                                        rInBean = trListFa.takeAt(h);
                                        break;
                                    }
                                }
                            }else{
                                error_text = error_text + " ����������ɱ� δȡ��������ⵥ��";
                                logger->error(error_text);
                            }
                        }else{
                            error_text = error_text + "������� ȡԭ�������ⵥ�ɱ����� �޴˵�����:"+ bean->busNo();
                            logger->error(error_text);
                            logger->error(" SQL:" + allcSql );
                        }
                    }else{
                        logger->error("err_note:" + allcSql );
                        error_text = tr("database error type id:")+QString::number(allcQuery.lastError().type())+"  " + allcQuery.lastError().text();
                        logger->error(error_text );
                    }
                    if(isLoadTr){
                        bottomListTrFa = FaAccntDAO::queryFaSubsidiaryByBottom(trInWhCode,pCode);
                        if(bottomListTrFa.size() > 0){
                            lastTrQuantity = bottomListTrFa.at(0)->stQuantity();
                            lastTrMoney = bottomListTrFa.at(0)->stMoney();
                            lastTrCost = bottomListTrFa.at(0)->stMoney()/bottomListTrFa.at(0)->stQuantity();

                        }
                    }else{
                        QString note = mHNote;
                        ui->noteLabel->setText(note.arg(error_text));
                        QMessageBox::critical(this,"����",error_text);
                        return ;
                    }
                }
                int fifo_tr_i = 0;
                int trResult = 0 ;
                int oTrResult = 0 ;

                // ****** end ����������ɱ� �������ȳ�����

                DBManager::getDBManager()->getGoinDBConnection().transaction();
                if(bean->vouchType() == "15" || bean->vouchType() == "12"){
                    //�����˻� �Ƚ��ȳ�����ɱ�
                    while( batch_out < 0  ){
                        if(listFIFOFaSub.size() > fifo_i){
                            //�������
                            double amount = listFIFOFaSub.at(fifo_i)->batchAmount();
                            double over = listFIFOFaSub.at(fifo_i)->inQuantity() - listFIFOFaSub.at(fifo_i)->batchAmount();
                            if(over >= batch_out){
                                bean->setOutQuantity(batch_out);
                                bean->setOutCost(listFIFOFaSub.at(fifo_i)->inCost());
                                bean->setOutMoney(bean->outQuantity()*bean->outCost());
                                bean->setOutPrice(sqlResult.value("unit_price").toDouble());

                                bean->setStQuantity(lastQuantity - bean->outQuantity());
                                bean->setStMoney(lastMoney - bean->outQuantity()*bean->outCost());
                                bean->setDvId(listFIFOFaSub.at(fifo_i)->id());
                                bean->setDvNo(listFIFOFaSub.at(fifo_i)->vouchNo());
                                result = FaAccntDAO::createSubsidiary(bean);
                                oReuslt = oReuslt + result ;
                                listFIFOFaSub.at(fifo_i)->setBatchAmount(amount - batch_out);
                                uResult = FaAccntDAO::updateSubsidiary(listFIFOFaSub.at(fifo_i));
                                fifo_i = fifo_i + 1;
                                batch_out = 0 ;
                            }else{
                                batch_out = batch_out + over;
                                bean->setOutQuantity(amount);
                                bean->setOutCost(listFIFOFaSub.at(fifo_i)->inCost());
                                bean->setOutMoney(bean->outQuantity()*bean->outCost());
                                bean->setOutPrice(sqlResult.value("unit_price").toDouble());

                                bean->setStQuantity(lastQuantity - bean->outQuantity());
                                bean->setStMoney(lastMoney - bean->outQuantity()*bean->outCost());
                                bean->setDvId(listFIFOFaSub.at(fifo_i)->id());
                                bean->setDvNo(listFIFOFaSub.at(fifo_i)->vouchNo());
                                result = FaAccntDAO::createSubsidiary(bean);
                                oReuslt = oReuslt + result ;
                                listFIFOFaSub.at(fifo_i)->setBatchAmount(amount + over);
                                uResult = FaAccntDAO::updateSubsidiary(listFIFOFaSub.at(fifo_i));
                                lastQuantity = lastQuantity - bean->outQuantity();
                                lastMoney = lastMoney - bean->outQuantity()*bean->outCost();
                                fifo_i = fifo_i + 1;
                            }
                        }else{
                            DBManager::getDBManager()->getGoinDBConnection().rollback();
                            ui->AccntBtn->setEnabled(true);
                            QApplication::restoreOverrideCursor();
                            QString error_text = rdNo + "(�Ƚ��ȳ�)���ⵥ�ݼ��ʴ���,��Ʒ" + bean->productCode()
                                    + " ��" + QString::number(fifo_i) + "�������˻����޽��,���ܼ���,������ֹ��";
                            logger->error(error_text);
                            QString note = mHNote;
                            ui->noteLabel->setText(note.arg(error_text));
                            QMessageBox::critical(this,"����",error_text);
                            return ;
                        }
                    }
                }else{
                    //��ͨ���� ί�� ��������
                    while( qAbs(batch_out) > 0  ){
                    if(listFIFOFaSub.size() > fifo_i){
                        //�������
                        double amount = listFIFOFaSub.at(fifo_i)->batchAmount();
                        if(amount >= batch_out){

                            bean->setOutQuantity(batch_out);
                            bean->setOutCost(listFIFOFaSub.at(fifo_i)->inCost());
                            bean->setOutMoney(bean->outQuantity()*bean->outCost());
                            bean->setOutPrice(sqlResult.value("unit_price").toDouble());

                            bean->setStQuantity(lastQuantity - bean->outQuantity());
                            bean->setStMoney(lastMoney - bean->outQuantity()*bean->outCost());
                            bean->setDvId(listFIFOFaSub.at(fifo_i)->id());
                            bean->setDvNo(listFIFOFaSub.at(fifo_i)->vouchNo());
                            result = FaAccntDAO::createSubsidiary(bean);
                            oReuslt = oReuslt + result ;
                            listFIFOFaSub.at(fifo_i)->setBatchAmount(amount - batch_out);
                            uResult = FaAccntDAO::updateSubsidiary(listFIFOFaSub.at(fifo_i));
                            fifo_i = fifo_i + 1;
                            batch_out = 0 ;
                            //����������ɱ� �������ȳ�����
                            if(bean->vouchType() == "07" && bean->busType() == "��������"){
                                if(isLoadTr){
                                    rInBean->setInCost(bean->outCost());
                                    rInBean->setInQuantity(bean->outQuantity());
                                    rInBean->setBatchAmount(bean->outQuantity());
                                    rInBean->setInMoney(bean->outMoney());
                                    rInBean->setOutCost(0);
                                    rInBean->setOutPrice(0);
                                    rInBean->setOutQuantity(0);
                                    rInBean->setOutMoney(0);
                                    //logger->error("��Ʒ:" + bean->productCode() +" �ڳ�:" + QString::number(lastTrQuantity) + " ��⣺" + QString::number(rInBean->inQuantity()));
                                    double stQuantity = lastTrQuantity + rInBean->inQuantity();
                                    rInBean->setStQuantity(stQuantity);
                                    rInBean->setStMoney(lastTrMoney + rInBean->inQuantity()*rInBean->inCost());
                                    //logger->error("��Ʒ:" + bean->productCode() +" ���:" + QString::number(rInBean->stQuantity()) + " ��" + QString::number(rInBean->stMoney()));
                                    //logger->error("����:" + bean->vouchNo() + " " + rInBean->vouchNo() + " ���������Ʒ:" + rInBean->productCode());
                                    lastTrQuantity = lastTrQuantity + rInBean->inQuantity();
                                    lastTrMoney = lastTrMoney + rInBean->inQuantity()*rInBean->inCost();
                                    trResult = FaAccntDAO::createSubsidiary(rInBean);
                                    oTrResult = oTrResult + trResult;
                                    fifo_tr_i = fifo_tr_i + 1;
                                }
                            }

                        }else{
                            batch_out = batch_out - amount;
                            bean->setOutQuantity(amount);
                            bean->setOutCost(listFIFOFaSub.at(fifo_i)->inCost());
                            bean->setOutMoney(bean->outQuantity()*bean->outCost());
                            bean->setOutPrice(sqlResult.value("unit_price").toDouble());

                            bean->setStQuantity(lastQuantity - bean->outQuantity());
                            bean->setStMoney(lastMoney - bean->outQuantity()*bean->outCost());
                            bean->setDvId(listFIFOFaSub.at(fifo_i)->id());
                            bean->setDvNo(listFIFOFaSub.at(fifo_i)->vouchNo());
                            result = FaAccntDAO::createSubsidiary(bean);
                            oReuslt = oReuslt + result ;
                            listFIFOFaSub.at(fifo_i)->setBatchAmount(0);
                            uResult = FaAccntDAO::updateSubsidiary(listFIFOFaSub.at(fifo_i));
                            lastQuantity = lastQuantity - bean->outQuantity();
                            lastMoney = lastMoney - bean->outQuantity()*bean->outCost();
                            fifo_i = fifo_i + 1;
                            //����������ɱ� �������ȳ�����
                            if(bean->vouchType() == "07" && bean->busType() == "��������"){
                                if(isLoadTr){
                                    rInBean->setInCost(bean->outCost());
                                    rInBean->setInQuantity(bean->outQuantity());
                                    rInBean->setBatchAmount(bean->outQuantity());
                                    rInBean->setInMoney(bean->outMoney());
                                    rInBean->setOutCost(0);
                                    rInBean->setOutPrice(0);
                                    rInBean->setOutQuantity(0);
                                    rInBean->setOutMoney(0);
                                    logger->error("$$$��Ʒ:" + bean->productCode() +" �ڳ�:" + QString::number(lastTrQuantity) + " ��⣺" + QString::number(rInBean->inQuantity()));
                                    double stQuantity = lastTrQuantity + rInBean->inQuantity();
                                    rInBean->setStQuantity(stQuantity);

                                    rInBean->setStMoney(lastTrMoney + rInBean->inQuantity()*rInBean->inCost());
                                    lastTrQuantity = lastTrQuantity + rInBean->inQuantity();
                                    lastTrMoney = lastTrMoney + rInBean->inQuantity()*rInBean->inCost();
                                    trResult = FaAccntDAO::createSubsidiary(rInBean);
                                    oTrResult = oTrResult + trResult;
                                    fifo_tr_i = fifo_tr_i + 1;
                                }
                            }
                        }
                    }else{
                        DBManager::getDBManager()->getGoinDBConnection().rollback();
                        ui->AccntBtn->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        QString error_text = rdNo + "(�Ƚ��ȳ�)���ⵥ�ݼ��ʴ���,��Ʒ" + bean->productCode()
                                + " ��" + QString::number(fifo_i) + "�γ������޿��,���ܼ���,������ֹ��";
                        logger->error(error_text);
                        QString note = mHNote;
                        ui->noteLabel->setText(note.arg(error_text));
                        QMessageBox::critical(this,"����",error_text);
                        return ;

                    }
                }
                }
                if(bean->vouchType() == "07" && bean->busType() == "��������"){
                    RdBean *trInBean = RdDAO::getRdByNo(rInBean->vouchNo());
                    RdsBean *trRdsInBean = RdDAO::getRdsBy(QString::number(rInBean->vouchsId()));
                    syResult = RdDAO::updateRd(rBean);
                    rdsBean->setIsAccount(true);
                    int rdsResult = RdDAO::updateRds(rdsBean);

                    trInBean->setAccounter(HaoKeApp::getInstance()->getUserDesc());
                    int syTrResult = RdDAO::updateRd(trInBean);
                    trRdsInBean->setIsAccount(true);
                    int rdsTrResult = RdDAO::updateRds(trRdsInBean);
                    if(uResult > 0 && syResult > 0 && fifo_i == oReuslt && rdsResult > 0
                            && oTrResult == fifo_tr_i && syTrResult > 0 && rdsTrResult > 0){
                        DBManager::getDBManager()->getGoinDBConnection().commit();
                        QAbstractItemModel *model = ui->tableView->model();
                        model->setData(model->index(i,11),rdNo + " ���ʳɹ�");
                        model->setData(model->index(rInBean->id(),11),rInBean->vouchNo() + " ���ʳɹ�");
                        i = i + 1;

                    }else{
                        DBManager::getDBManager()->getGoinDBConnection().rollback();
                        ui->AccntBtn->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        QString error_text = rdNo + "(�Ƚ��ȳ�)�������ⵥ�ݼ��ʴ���,������ֹ��";
                        logger->error(error_text);
                        QString note = mHNote;
                        ui->noteLabel->setText(note.arg(error_text));
                        QMessageBox::critical(this,"����",error_text);
                        return ;
                    }
                }else{
                    //�ǵ�������
                    syResult = RdDAO::updateRd(rBean);
                    rdsBean->setIsAccount(true);
                    int rdsResult = RdDAO::updateRds(rdsBean);
                    if(uResult > 0 && syResult > 0 && fifo_i == oReuslt && rdsResult > 0){
                        DBManager::getDBManager()->getGoinDBConnection().commit();
                        QAbstractItemModel *model = ui->tableView->model();
                        model->setData(model->index(i,11),rdNo + " ���ʳɹ�");
                        i = i + 1;

                    }else{
                        DBManager::getDBManager()->getGoinDBConnection().rollback();
                        ui->AccntBtn->setEnabled(true);
                        QApplication::restoreOverrideCursor();
                        QString error_text = rdNo + "(�Ƚ��ȳ�)���ⵥ�ݼ��ʴ���,������ֹ��";
                        logger->error(error_text);
                        QString note = mHNote;
                        ui->noteLabel->setText(note.arg(error_text));
                        QMessageBox::critical(this,"����",error_text);
                        return ;
                    }
                }
            }else{
                ui->AccntBtn->setEnabled(true);
                QApplication::restoreOverrideCursor();
                QString error_text = rdNo + "(�Ƚ��ȳ�)���ⵥ�ݼ��ʴ���,δȡ����ⵥ���б�������ֹ��";
                logger->error(error_text);
                QString note = mHNote;
                ui->noteLabel->setText(note.arg(error_text));
                QMessageBox::critical(this,"����",error_text);
                return ;
            }
       }
       logger->debug("�Ƚ��ȳ����� --���� ����:" + rdNo  + " ��Ʒ����:" + pCode);
       //�Ƚ��ȳ����� --����
    }
    //-- ���ʽ���
    logger->debug(QObject::tr("���ʽ��� "));
    QString note = mHNote;
    ui->noteLabel->setText(note.arg(QString::number(i) + " ���ݳɹ�����"));
    ui->AccntBtn->setEnabled(true);
    QApplication::restoreOverrideCursor();

}

QSqlQuery FaAccntForm::sqlQuery(QString sql, QVector<QVariant> qvar){
    QSqlQuery query(DBManager::getDBManager()->getGoinDBConnection());
    query.setForwardOnly(true);
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
    }
    return query;
}

void FaAccntForm::sQuery(){
    //Log4Qt::Logger *logger = Log4Qt::Logger::rootLogger();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QVector<QVariant> v;
    QSqlQuery query;
    mStrSql = "SELECT f.* FROM ("
            " SELECT a.*, g.wh_name, c.id as vouchs_id, c.product_code,"
                       " d.product_name, c.product_style, c.unit, c.quantity,c.unit_price,c.money "
                       " FROM rd_record a,rd_records c,ba_product d, ba_ware_house g  "
                       " WHERE a.bus_date>=? AND a.bus_date<=? "
                       " AND a.wh_code=g.wh_code AND a.rd_no=c.rd_no "
                       " AND c.product_code=d.product_code AND c.is_account=false "
            ")f"
                       " ORDER BY f.bus_date,f.rd_flag,f.id ASC ";

    v.append(ui->dateEdit->date().toString("yyyy-MM-dd"));
    v.append(ui->dateEdit1->date().toString("yyyy-MM-dd"));

    query = sqlQuery(mStrSql,v);

    int rowNum = 0;
    mListModel->setRowCount(0);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    int i = 0;
    while(query.next()){
        QSqlRecord sqlResult = query.record();
        rowNum = i;
        mListModel->insertRow(rowNum);
        for (int j = 0; j < mListChildItem.size(); j++){
            if(sqlResult.value(mListChildItem[j].name).type() == QVariant::Bool){
                if(sqlResult.value(mListChildItem[j].name).toBool())
                    mListModel->setData(mListModel->index(rowNum, mListChildItem[j].id),"��");
                else
                    mListModel->setData(mListModel->index(rowNum, mListChildItem[j].id),"��");

            }else if(sqlResult.value(mListChildItem[j].name).type() == QVariant::Double){
                mListModel->setData(mListModel->index(rowNum, mListChildItem[j].id),QString::number(sqlResult.value(mListChildItem[j].name).toDouble(),'f',2));
            }else{
                mListModel->setData(mListModel->index(rowNum, mListChildItem[j].id),sqlResult.value(mListChildItem[j].name));

            }

        }
        i = i + 1;

    }
    ui->tableView->setModel(mListModel);
    QApplication::restoreOverrideCursor();

}

void FaAccntForm::on_AccntCancelBtn_clicked()
{
    RecoverAccountDialog *recoverAccntDlg = new RecoverAccountDialog();
    if(recoverAccntDlg->exec() == QDialog::Accepted){

    }
}
