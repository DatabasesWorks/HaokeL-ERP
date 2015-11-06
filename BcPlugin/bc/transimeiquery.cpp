#include "transimeiquery.h"
#include "bean/SupplieBean.h"
#include "bean/SupplieTypeBean.h"
#include "bean/WareHouseBean.h"
#include "bean/CustomerBean.h"
#include "dao/suppliedao.h"
#include "dao/supplietypedao.h"
#include "dao/customerdao.h"
#include "dao/waredao.h"
#include "bc/transimeiwin.h"
#include "haokeapp.h"
#include "bcplugin.h"
TransImeiQuery::TransImeiQuery(QWidget *parent) :
    SingleQueryForm(parent)
{
    QString dSql1 =
            " SELECT a.*,b.product_name,c.wh_name as out_wh_name,d.wh_name as in_wh_name "
            " FROM rd_trans_imei a "
            " INNER JOIN ba_product b ON a.product_code=b.product_code "
            " LEFT JOIN ba_ware_house c ON a.out_wh_code=c.wh_code "
            " LEFT JOIN ba_ware_house d ON a.in_wh_code=d.wh_code "
            " WHERE bus_date>=? AND bus_date<=? "
            " AND a.out_wh_code LIKE ? AND b.type_id LIKE ? AND a.product_code LIKE ?";
    QString dSql2 = " SELECT a.*,b.product_name,c.wh_name as out_wh_name,d.wh_name as in_wh_name "
            " FROM rd_trans_imei a "
            " INNER JOIN ba_product b ON a.product_code=b.product_code "
            " LEFT JOIN ba_ware_house c ON a.out_wh_code=c.wh_code "
            " LEFT JOIN ba_ware_house d ON a.in_wh_code=d.wh_code "
            " WHERE imei LIKE ? ";

    childItemProperties childItem[] = {
            { "no","����","����", 0,100, true, false },
            { "imei","IMEI","IMEI", 1,120, true, true },
            { "out_wh_code","����","�����ֿ�", 2, 80,true, true },
            { "out_wh_name","����","�����ֿ�", 3, 100,true, true },
            { "product_code","��Ʒ����","��Ʒ����", 4,80, true, true },
            { "product_name","��Ʒ����","��Ʒ����", 5,80, true, true },
            { "in_wh_name","����ֿ�","����ֿ�", 6,100, true, true },
            { "bus_date","��������","��������", 7,100, true, true },
            { "memo","��ע","��ע",8,180, true, false }
    };

    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }

    QList<WareHouseBean *> whBeanList = WareDAO::queryWarehouse();
    QStringList whList ;
    for(int i =0 ; i < whBeanList.size(); i++){
        whList << whBeanList.at(i)->whCode() + " - " +whBeanList.at(i)->whName();
    }
    dwTypeQks->setListModel(true);
    dwTypeQks->setItems(whList);
    dwTypeLabel->setText("�����ֿ�:");
    dwNameQks->setVisible(false);
    dwNameLabel->setVisible(false);

    Label->setVisible(false);
    //idLabel->setVisible(false);
    //noEdit->setVisible(false);
    idLabel->setText("��    ��:");
    Label->setVisible(false);
    rBnt1->setVisible(false);
    rBnt2->setVisible(false);
    rBnt3->setVisible(false);
    //verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Preferred);
    //gridLayout->addItem(verticalSpacer_2, 5, 1, 1, 1);
    QString formId = HaoKeApp::bcSubId() + BcPlugin::transImeiQueryWinId();
    setFormId(formId);
    setTabDetailText("��������");

    initData(dItem,dSql1,dSql2);
    initTableHead();
    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
}

void TransImeiQuery::sTableClicked(QString no){
    if(no.length()>1){
        if(!HaoKeApp::getInstance()->gui()->subWinIsOpen("�������")){
            TransImeiWin *pWin = new TransImeiWin(no,this);
            pWin->setWindowTitle("�������");
            HaoKeApp::getInstance()->gui()->addSubWin(pWin);
            pWin->showMaximized();
        }
    }
}
