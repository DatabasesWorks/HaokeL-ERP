#include "pusuppliesummary.h"
#include "bean/SupplieBean.h"
#include "bean/SupplieTypeBean.h"
#include "dao/suppliedao.h"
#include "dao/supplietypedao.h"
#include "puorderwin.h"
#include "haokeapp.h"
#include "cgplugin.h"

PuSupplieSummary::PuSupplieSummary(QWidget *parent) :
    SingleQueryForm(parent)
{
    QString dSql1 =
            " SELECT g.*, h.product_name, h.style, s.supplie_name "
            " FROM ( "
               " SELECT a.supplier_id, b.product_code,sum(b.quantity) as sumQ,sum(money) as sumMoeny "
                   " FROM rd_record a "
                   " INNER JOIN rd_records b "
                   " ON a.rd_no=b.rd_no "
                   " WHERE a.bus_date>=? AND a.bus_date<=? "
                   " AND a.supplier_id LIKE ? "
                   " AND a.rd_flag=1 AND (a.vouch_type='01' OR a.vouch_type='14') "
                   " GROUP BY a.supplier_id, b.product_code "
            ") g "
            " INNER JOIN ba_product h "
            " ON g.product_code = h.product_code "
            " INNER JOIN ba_suppliers s "
            " ON g.supplier_id=s.supplie_code"
            " WHERE h.type_id LIKE ? AND g.product_code LIKE ? "
            " ORDER BY g.supplier_id,g.product_code ";
    QString dSql2 = "";
    childItemProperties childItem[] = {
            { "supplier_id","����","��Ӧ��", 0,80, true, true },
            { "supplie_name","����","��Ӧ��", 1,180, true, true },
            { "product_code","����","��Ʒ", 2,80, true, true },
            { "product_name","����","��Ʒ", 3,100, true, true },
            { "style","���","��Ʒ",4,80, true, false },
            { "sumQ","����","����", 5,60, true, true },
            { "sumMoeny","���","���", 6,80, true, false }


    };
    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }
    QDate oneDate = QDate(QDate::currentDate().year(),QDate::currentDate().month(),1);
    sDateEdit1->setDate(oneDate);
    QList<SupplieBean *> venBeanList = SupplieDAO::querySupplie();
    QStringList venList ;
    for(int i =0 ; i < venBeanList.size(); i++){
        venList << venBeanList.at(i)->supplieCode() + " - " +venBeanList.at(i)->supplieName();
    }
    dwTypeQks->setListModel(true);
    dwTypeQks->setItems(venList);
    dwNameQks->setVisible(false);
    dwNameLabel->setVisible(false);
    noEdit->setVisible(false);
    Label->setVisible(false);
    idLabel->setVisible(false);

    Label->setVisible(false);
    rBnt1->setVisible(false);
    rBnt2->setVisible(false);
    rBnt3->setVisible(false);
    QString formId = HaoKeApp::puSubId() + CgPlugin::supplieSummaryWinId();
    QString winTitle = "��Ӧ�̹������ܱ�";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,winTitle,HaoKeApp::puSubId().toInt(),formId.toInt(),0,"");

    setFormId(formId);
    setTabDetailText("��Ӧ�̹������ܱ�");
    //������ϸ�ϼ��к�
    QList<int > totalItem;
    totalItem << 5 << 6 ;
    setTotalItem(totalItem);
    initData(dItem,dSql1,dSql2);
    initTableHead();




}

