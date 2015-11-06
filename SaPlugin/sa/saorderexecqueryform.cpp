#include "saorderexecqueryform.h"
#include "bean/CustomerBean.h"
#include "bean/CustomerTypeBean.h"
#include "dao/customerdao.h"
#include "dao/customertypedao.h"
#include "saorderwin.h"
#include "haokeapp.h"
#include "saplugin.h"
SaOrderExecQueryForm::SaOrderExecQueryForm(QWidget *parent) :
    SingleQueryForm(parent)
{
    QString dSql1 =
            " SELECT a.no, a.bus_date, a.bus_type, b.cus_name,c.product_code,d.product_name,c.product_style, c.unit,"
            " c.quantity, c.out_quantity, (c.quantity-c.out_quantity) as real_quantity, c.re_quantity,c.is_complete,c.price,c.money,a.demand_date,a.project_date"
            " FROM sa_order_vouch a,sa_order_vouchs c,ba_product d,ba_customer b "
                       " WHERE a.bus_date>=? AND a.bus_date<=? AND b.cus_type like ? AND a.cus_code like ? "
                       " AND d.type_id like ? AND c.product_code like ?"
                       " AND a.no=c.order_no AND c.product_code=d.product_code AND a.cus_code= b.cus_code "
                       " ORDER BY no ASC";
    QString dSql2 = "select f.*,b.cus_name from ( "
            "SELECT a.no, a.bus_date, a.bus_type, a.cus_code,a.wh_code,a.dep_code,a.demand_date,c.product_code,"
            " d.product_name, c.product_style, c.unit,  a.project_date, a.person_code, a.handler, a.audit_time, a.creater, a.created,"
            " a.closer, a.updater, a.updated, a.status, a.memo, c.quantity, c.out_quantity, (c.quantity-c.out_quantity) as real_quantity, c.re_quantity, c.is_complete, c.price,c.money "
            " from sa_order_vouch a,sa_order_vouchs c,ba_product d"
            "  WHERE a.no like ? AND a.no=c.order_no AND c.product_code=d.product_code) f "
            " left join ba_customer b on f.cus_code= b.cus_code ORDER BY id ASC";
    childItemProperties childItem[] = {
            { "no","����","����", 0,100, true, true },
            { "bus_date","��������","��������", 1,80, true, true },
            { "bus_type","��������","��������", 2,80, true, false },
            { "cus_name","�ͻ�","�ͻ�", 3, 160,true, true },
            { "product_code","����","��Ʒ", 4,80, true, true },
            { "product_name","����","��Ʒ", 5,100, true, true },
            { "product_style","���","��Ʒ",6,80, true, false },
            { "unit","���۵�λ","���۵�λ", 7,60, true, false },
            { "quantity","��������","��������", 8, 80,true, true },
            { "out_quantity","�ѷ�������","�ѷ�������", 9, 80,true, true },
            { "real_quantity","δ��������","δ��������", 10, 80,true, true },
            { "is_complete","�Ƿ�ִ��","�Ƿ�ִ��", 11,80, true, true },
            { "price","���۵���","���۵���", 12,80, true, true },
            { "money","���۽��","���۽��", 13,80, true, false },
            { "demand_date","��������","��������", 14,80, true, false },
            { "project_date","�ƻ���������","�ƻ���������", 15,80, true, false }


    };

    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }

    QList<CustomerBean *> venBeanList = CustomerDAO::queryCus();
    QStringList venList ;
    for(int i =0 ; i < venBeanList.size(); i++){
        venList << venBeanList.at(i)->cusCode() + " - " +venBeanList.at(i)->cusName();
    }
    dwNameQks->setListModel(true);
    dwNameQks->setItems(venList);

    QList<CustomerTypeBean *> typeBeanList = CustomerTypeDAO::queryType();
    QStringList typeList ;
    for(int i =0 ; i < typeBeanList.size(); i++){
        typeList << typeBeanList.at(i)->typeId() + " - " +typeBeanList.at(i)->typeName();
    }
    dwTypeQks->setListModel(true);
    dwTypeQks->setItems(typeList);
    Label->setText("�Ƿ�ִ��:");
    rBnt2->setText("ĩִ��");
    rBnt3->setText("��ִ��");
    QString formId = HaoKeApp::saSubId() + SaPlugin::orderExecQueryWinId();
    QString winTitle = "���۶���ִ��ͳ�Ʊ�";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,winTitle,HaoKeApp::saSubId().toInt(),formId.toInt(),0,"");

    setFormId(formId);
    setTabDetailText("���۶���ִ��ͳ�Ʊ�");
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

void SaOrderExecQueryForm::sTableClicked(QString no){
    if(no.length()>1){
        if(!HaoKeApp::getInstance()->gui()->subWinIsOpen(tr("Sales Order"))){
            SaOrderWin *pWin = new SaOrderWin(no,this);
            pWin->setWindowTitle(tr("Sales Order"));
            HaoKeApp::getInstance()->gui()->addSubWin(pWin);
            pWin->showMaximized();
        }
    }
}
