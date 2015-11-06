#include "puorderexecqueryform.h"
#include "bean/SupplieBean.h"
#include "bean/SupplieTypeBean.h"
#include "dao/suppliedao.h"
#include "dao/supplietypedao.h"
#include "puorderwin.h"
#include "haokeapp.h"
#include "cgplugin.h"

PuOrderExecQueryForm::PuOrderExecQueryForm(QWidget *parent) :
    SingleQueryForm(parent)
{

    QString dSql1 =
            " SELECT a.no, a.bus_date, a.bus_type, b.supplie_name,c.product_code,d.product_name,c.product_style, c.unit,"
            " c.quantity, c.in_quantity, (c.quantity-c.in_quantity) as real_quantity, c.re_quantity,c.is_complete,c.price,c.money,a.demand_date,a.project_date"
            " FROM pu_order_vouch a,pu_order_vouchs c,ba_product d,ba_suppliers b "
                       " WHERE a.bus_date>=? AND a.bus_date<=? AND b.supplie_type like ? AND a.supplier_id like ? "
                       " AND d.type_id like ? AND c.product_code like ?"
                       " AND a.no=c.order_no AND c.product_code=d.product_code AND a.supplier_id= b.supplie_code "
                       " ORDER BY no ASC";
    QString dSql2 = "select f.*,b.supplie_name from ( "
            "SELECT a.no, a.bus_date, a.bus_type, a.supplier_id,a.wh_code,a.dep_code,a.demand_date,c.product_code,"
            " d.product_name, c.product_style, c.unit,  a.project_date, a.person_code, a.handler, a.audit_time, a.creater, a.created,"
            " a.closer, a.updater, a.updated, a.status, a.memo, c.quantity, c.in_quantity, (c.quantity-c.in_quantity) as real_quantity, c.re_quantity, c.is_complete, c.price,c.money "
            " from pu_order_vouch a,pu_order_vouchs c,ba_product d"
            "  WHERE a.no like ? AND a.no=c.order_no AND c.product_code=d.product_code) f "
            " left join ba_suppliers b on f.supplier_id= b.supplie_code ORDER BY no ASC";
    childItemProperties childItem[] = {
            { "no","����","����", 0,100, true, true },
            { "bus_date","��������","��������", 1,80, true, true },
            { "bus_type","��������","��������", 2,80, true, false },
            { "supplie_name","��Ӧ��","��Ӧ��", 3, 160,true, true },
            { "product_code","����","��Ʒ", 4,80, true, true },
            { "product_name","����","��Ʒ", 5,100, true, true },
            { "product_style","���","��Ʒ",6,80, true, false },
            { "unit","�ɹ���λ","�ɹ���λ", 7,60, true, false },
            { "quantity","��������","��������", 8, 80,true, true },
            { "in_quantity","�ѵ�������","�ѵ�������", 9, 80,true, true },
            { "real_quantity","δ��������","δ��������", 10, 80,true, true },
            { "is_complete","�Ƿ�ִ��","�Ƿ�ִ��", 11,60, true, true },
            { "price","�ɹ�����","�ɹ�����", 12,60, true, true },
            { "money","�ɹ����","�ɹ����", 13,80, true, false },
            { "demand_date","��������","��������", 14,80, true, false },
            { "project_date","�ƻ���������","�ƻ���������", 15,80, true, false }


    };

    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }

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
    Label->setText("�Ƿ�ִ��:");
    rBnt2->setText("ĩִ��");
    rBnt3->setText("��ִ��");
    QString formId = HaoKeApp::puSubId() + CgPlugin::orderExecQueryWinId();
    QString winTitle = "�ɹ�����ִ��ͳ�Ʊ�";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,winTitle,HaoKeApp::puSubId().toInt(),formId.toInt(),0,"");

    setFormId(formId);
    setTabDetailText("�ɹ�����ִ��ͳ�Ʊ�");
    //������ϸ�ϼ��к�
    QList<int > totalItem;
    totalItem << 8 << 9 << 10 << 13;
    setTotalItem(totalItem);
    initData(dItem,dSql1,dSql2);
    initTableHead();
    //�趨�Ƿ�������״̬��ѯ
    setHandleFiled("c.is_complete");

    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
}

void PuOrderExecQueryForm::sTableClicked(QString no){
    if(no.length()>1){
        if(!HaoKeApp::getInstance()->gui()->subWinIsOpen(tr("Purchase Order"))){
            PuOrderWin *pWin = new PuOrderWin(no,this);
            pWin->setWindowTitle(tr("Purchase Order"));
            HaoKeApp::getInstance()->gui()->addSubWin(pWin);
            pWin->showMaximized();
        }
    }
}
