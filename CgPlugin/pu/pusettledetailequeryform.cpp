#include "pusettledetailequeryform.h"
#include "bean/SupplieBean.h"
#include "bean/SupplieTypeBean.h"
#include "dao/suppliedao.h"
#include "dao/supplietypedao.h"
#include "purdwin.h"
#include "haokeapp.h"
#include "cgplugin.h"

PuSettleDetaileQueryForm::PuSettleDetaileQueryForm(QWidget *parent) :
    SingleQueryForm(parent)
{
    QString dSql1 = " SELECT a.id, a.rd_no, a.bus_date, a.bus_type, a.supplier_id, b.supplie_name, a.wh_code,a.dep_code,c.product_code,"
                " d.product_name, c.product_style, c.unit, a.person_code, a.handler, a.audit_time, a.creater, a.created,"
                " a.bill_no, a.is_bill, a.status, a.memo, c.quantity,c.not_quantity, c.sum_qunatity , c.unit_price,c.money "
                " from rd_record a, "
                    " ( SELECT g.*,g.quantity-COALESCE(h.sum_qunatity,0) as not_quantity, COALESCE(h.sum_qunatity,0) as sum_qunatity "
                    " from rd_records g "
                    " LEFT JOIN ( "
                        " SELECT f.is_handle,e.rd_no,e.product_code,sum(quantity) as sum_qunatity "
                        " from pu_settle_vouchs e "
                        " INNER JOIN pu_settle_vouch f "
                        " ON e.settle_no=f.no "
                        " GROUP BY f.is_handle,e.rd_no,e.product_code HAVING f.is_handle=true ) h "
                    " ON g.rd_no=h.rd_no AND g.product_code=h.product_code "
                " ) c,ba_product d,ba_suppliers b "
                " WHERE a.bus_date>=? AND a.bus_date<=? AND b.supplie_type like ? AND a.supplier_id like ? "
                " AND d.type_id like ? AND c.product_code like ?"
                " AND a.rd_no=c.rd_no AND a.is_handle=true AND c.product_code=d.product_code AND a.supplier_id= b.supplie_code "
                " AND (a.vouch_type='01' OR a.vouch_type='14') ORDER BY id ASC";
    QString dSql2 = "";
    childItemProperties childItem[] = {
            { "rd_no","��ⵥ��","��ⵥ��", 0,100, true, true },
            { "bus_date","�������","�������", 1,80, true, true },
            { "bus_type","��������","��������", 2,80, true, false },
            { "supplier_id","��Ӧ�̱���","��Ӧ�̱���", 3,80, true, false },
            { "supplie_name","��Ӧ������","��Ӧ������", 4, 120,true, true },
            { "bill_no","���㵥��","���㵥��", 5,100, true, true },
            { "is_bill","�Ƿ����","�Ƿ����", 6,60, true, false },
            { "product_code","����","��Ʒ", 7,80, true, true },
            { "product_name","����","��Ʒ", 8,100, true, true },
            { "product_style","���","��Ʒ",9,80, true, false },
            { "quantity","�������","�������", 10, 80,true, true },
            { "sum_qunatity","�ѽ�����","�ѽ�����", 11, 80,true, true },
            { "not_quantity","δ������","δ������", 12, 80,true, true },
            { "unit_price","�ɹ�����","�ɹ�����", 13,80, true, true },
            { "money","�ɹ����","�ɹ����", 14,80, true, false }

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

    Label->setText("�Ƿ����:");
    rBnt2->setText("δ����");
    rBnt3->setText("�ѽ���");
    QString formId = HaoKeApp::puSubId() + CgPlugin::settleDetaileQueryWinId();
    QString winTitle = "�ɹ�������ϸ��";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,winTitle,HaoKeApp::puSubId().toInt(),formId.toInt(),0,"");

    setFormId(formId);
    setTabDetailText("�ɹ�������ϸ��");
    //������ϸ�ϼ��к�
    QList<int > totalItem;
    totalItem << 10 << 11 << 12 << 14;
    setTotalItem(totalItem);
    initData(dItem,dSql1,dSql2);
    initTableHead();
    setHandleFiled("a.is_bill");
    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
    //setHandleVisible(false);
}

void PuSettleDetaileQueryForm::sTableClicked(QString no){
    if(no.length()>1){
        if(!HaoKeApp::getInstance()->gui()->subWinIsOpen(tr("Purchase Receipt"))){
            PuRdWin *pWin = new PuRdWin(no,this);
            pWin->setWindowTitle(tr("Purchase Receipt"));
            HaoKeApp::getInstance()->gui()->addSubWin(pWin);
            pWin->showMaximized();
        }
    }
}
