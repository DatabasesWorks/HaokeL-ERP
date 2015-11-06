#include "pusettleremainqueryform.h"
#include "bean/SupplieBean.h"
#include "bean/SupplieTypeBean.h"
#include "dao/suppliedao.h"
#include "dao/supplietypedao.h"
#include "purdwin.h"
#include "haokeapp.h"
#include "cgplugin.h"
PuSettleRemainQueryForm::PuSettleRemainQueryForm(QWidget *parent) :
    SingleQueryForm(parent)
{
    QString dSql1 = " SELECT t.* FROM pu_settle_remain(?, ?, ?, ?, ?, ?) "
            " t(supplier_id varchar, supplie_name varchar, product_code varchar, product_name varchar, "
            " sumQuantity numeric, sumMoney numeric, "
            " settleSumQuantity numeric, settleSumMoney numeric, settleSumAmount numeric,"
            " startSumQuantity numeric, startSumMoney numeric, "
            " startSettleSumQuantity numeric, startSettleSumMoney numeric, "
            " endSumQuantity numeric, endSumMoney numeric )";
    QString dSql2 = "";
    childItemProperties childItem[] = {
            { "supplier_id","��Ӧ�̱���","��Ӧ�̱���", 0,80, false, false },
            { "supplie_name","��Ӧ������","��Ӧ������", 1, 120,true, true },
            { "product_code","����","��Ʒ", 2,80, true, true },
            { "product_name","����","��Ʒ", 3,100, true, true },
            { "startSumQuantity","����","���ڽ���", 4, 80,true, true },
            { "startSumMoney","���","���ڽ���", 5, 80,true, true },
            { "sumQuantity","����","�������", 6, 80,true, true },
            { "sumMoney","���","�������", 7, 80,true, true },
            { "settleSumQuantity","����","���ڽ���", 8, 80,true, true },
            { "settleSumMoney","���","���ڽ���", 9,80, true, true },
            { "settleSumAmount","ʵ�ʽ��","���ڽ���", 10,80, true, true },
            { "endSumQuantity","����","���ڽ���", 11,80, true, false },
            { "endSumMoney","���","���ڽ���", 12,80, true, false }

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
    dwNameQks->setListModel(true);
    dwNameQks->setItems(venList);

    QList<SupplieTypeBean *> typeBeanList = SupplieTypeDAO::queryType();
    QStringList typeList ;
    for(int i =0 ; i < typeBeanList.size(); i++){
        typeList << typeBeanList.at(i)->typeId() + " - " +typeBeanList.at(i)->typeName();
    }
    dwTypeQks->setListModel(true);
    dwTypeQks->setItems(typeList);

    noEdit->setVisible(false);
    idLabel->setVisible(false);

    Label->setVisible(false);
    rBnt1->setVisible(false);
    rBnt2->setVisible(false);
    rBnt3->setVisible(false);
    QString formId = HaoKeApp::puSubId() + CgPlugin::settleRemainQueryWinId();
    QString winTitle = "�ɹ���������";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,winTitle,HaoKeApp::puSubId().toInt(),formId.toInt(),0,"");

    setFormId(formId);
    setTabDetailText("�ɹ���������");
    //������ϸ�ϼ��к�
    QList<int > totalItem;
    totalItem << 6 << 7 << 8 << 9 << 10 << 11 << 12;
    setTotalItem(totalItem);
    initData(dItem,dSql1,dSql2);
    initTableHead();
}
