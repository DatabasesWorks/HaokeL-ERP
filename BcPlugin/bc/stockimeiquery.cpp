#include "stockimeiquery.h"
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
StockImeiQuery::StockImeiQuery(QWidget *parent) :
    SingleQueryForm(parent)
{
    QString dSql1 =
            " SELECT a.*,b.product_name,d.wh_name "
            " FROM stock_imei a "
            " INNER JOIN ba_product b ON a.product_code=b.product_code "
            " LEFT JOIN ba_ware_house d ON a.wh_code=d.wh_code "
            " WHERE bus_date>=? AND bus_date<=? "
            " AND a.wh_code LIKE ? AND b.type_id LIKE ? AND a.product_code LIKE ?";
    QString dSql2 = " SELECT a.*,b.product_name,d.wh_name "
            " FROM stock_imei a "
            " INNER JOIN ba_product b ON a.product_code=b.product_code "
            " LEFT JOIN ba_ware_house d ON a.wh_code=d.wh_code "
            " WHERE imei LIKE ? ";

    childItemProperties childItem[] = {
            { "no","����","����", 0,100, true, false },
            { "imei","IMEI","IMEI", 1,120, true, true },
            { "wh_code","����","�ֿ�", 2, 80,true, true },
            { "wh_name","����","�ֿ�", 3, 100,true, true },
            { "product_code","��Ʒ����","��Ʒ����", 4,80, true, true },
            { "product_name","��Ʒ����","��Ʒ����", 5,80, true, true },
            { "bus_date","��������","��������", 6,100, true, true },
            { "memo","��ע","��ע",7,180, true, false }
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
    dwTypeLabel->setText("��    ��:");
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
    QString formId = HaoKeApp::bcSubId() + BcPlugin::stockImeiQueryWinId();
    setFormId(formId);
    setTabDetailText("��洮��");

    initData(dItem,dSql1,dSql2);
    initTableHead();
    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
}

void StockImeiQuery::sTableClicked(QString no){
    if(no.length()>1){

    }
}
