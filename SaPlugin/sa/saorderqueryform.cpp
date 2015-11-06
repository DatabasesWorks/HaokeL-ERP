#include "saorderqueryform.h"
#include "bean/CustomerBean.h"
#include "bean/CustomerTypeBean.h"
#include "dao/customerdao.h"
#include "dao/customertypedao.h"
#include "saorderwin.h"
#include "haokeapp.h"
#include "saplugin.h"

SaOrderQueryForm::SaOrderQueryForm(QWidget *parent) :
    QueryForm(parent)
{
    //qApp->setStyle("WindowsXP");
    //qApp->setStyle("Plastique");
    QString lSql1 = "select e.*,d.emp_name from (select f.*,c.dep_name from "
            " (select a.*,b.cus_name from sa_order_vouch a "
            " left join ba_customer b on a.cus_code=b.cus_code "
            " WHERE a.bus_date>=? AND a.bus_date<=? AND b.cus_type like ? AND a.cus_code like ? ) f "
            " left join ba_department c on f.dep_code=c.dep_id) e left join ba_employee d on e.person_code=d.emp_code ORDER BY e.bus_date,e.no ASC";
    QString lSql2 = "select e.*,d.emp_name from (select f.*,c.dep_name from "
            " (select a.*,b.cus_name from sa_order_vouch a "
            " left join ba_customer b on a.cus_code=b.cus_code "
            " WHERE a.no like ? ) f"
            " left join ba_department c on f.dep_code=c.dep_id) e left join ba_employee d on e.person_code=d.emp_code ";

    childItemProperties listItem[] = {
            { "no","����","����", 0,100, true, true },
            { "bus_date","��������","��������", 1,80, true, true },
            { "bus_type","��������","��������", 2,80, true, false },
            { "cus_code","����","��Ӧ��", 3,80, true, false },
            { "cus_name","����","��Ӧ��", 4, 120,true, true },
            { "dep_name","����","����", 5,80, true, false },
            { "demand_date","��������","��������", 6,80, true, false },
            { "project_date","�ƻ���������","�ƻ���������", 7,80, true, false },
            { "emp_name","����Ա","����Ա", 8,60, true, false },
            { "handler","���","���", 9,60, true, false },
            { "audit_time","���ʱ��","���ʱ��", 10,100, true, false },
            { "creater","�Ƶ�","�Ƶ�", 11,60, true, false },
            { "memo","��ע","��ע", 12,180, true, false },

    };
    QString dSql1 =
            " SELECT a.id, a.no, a.bus_date, a.bus_type, a.cus_code, b.cus_name, a.wh_code,a.dep_code,a.demand_date,c.product_code,"
                       " d.product_name, c.product_style, c.unit,  a.project_date, a.person_code, a.handler, a.audit_time, a.creater, a.created,"
                       " a.closer, a.updater, a.updated, a.status, a.memo, c.quantity,c.price,c.money "
                       " from sa_order_vouch a,sa_order_vouchs c,ba_product d,ba_customer b "
                       " WHERE a.bus_date>=? AND a.bus_date<=? AND b.cus_type like ? AND a.cus_code like ? "
                       " AND d.type_id like ? AND c.product_code like ?"
                       " AND a.no=c.order_no AND c.product_code=d.product_code AND a.cus_code= b.cus_code "
                       " ORDER BY id ASC";
    QString dSql2 = "select f.*,b.cus_name from ( "
            "SELECT a.id, a.no, a.bus_date, a.bus_type, a.cus_code,a.wh_code,a.dep_code,a.demand_date,c.product_code,"
            " d.product_name, c.product_style, c.unit,  a.project_date, a.person_code, a.handler, a.audit_time, a.creater, a.created,"
            " a.closer, a.updater, a.updated, a.status, a.memo, c.quantity,c.price,c.money "
            " from sa_order_vouch a,sa_order_vouchs c,ba_product d"
            "  WHERE a.no like ? AND a.no=c.order_no AND c.product_code=d.product_code) f "
            " left join ba_customer b on f.cus_code= b.cus_code ORDER BY id ASC";
    childItemProperties childItem[] = {
            { "no","����","����", 0,100, true, true },
            { "bus_date","��������","��������", 1,80, true, true },
            { "bus_type","��������","��������", 2,80, true, false },
            { "cus_code","����","��Ӧ��", 3,60, true, false },
            { "cus_name","����","��Ӧ��", 4, 120,true, true },
            { "demand_date","��������","��������", 5,80, true, false },
            { "project_date","�ƻ���������","�ƻ���������", 6,80, true, false },
            { "product_code","����","��Ʒ", 7,80, true, true },
            { "product_name","����","��Ʒ", 8,100, true, true },
            { "product_style","���","��Ʒ",9,80, true, false },
            { "unit","���۵�λ","���۵�λ", 10,60, true, false },
            { "quantity","����","����", 11, 60,true, true },
            { "price","���۵���","���۵���", 12,80, true, true },
            { "money","���۽��","���۽��", 13,80, true, false }

    };
    QList<struct childItemProperties > lItem;
    int size = sizeof(listItem) / sizeof(listItem[0]);
    for(int i = 0 ; i < size ; i++){
        lItem << listItem[i];
    }
    QList<struct childItemProperties > dItem;
    size = sizeof(childItem) / sizeof(childItem[0]);
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
    QString formId = HaoKeApp::saSubId() + SaPlugin::orderQueryWinId();
    QString winTitle = "���۶�����ѯ";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,winTitle,HaoKeApp::saSubId().toInt(),formId.toInt(),0,"");

    setFormId(formId);
    setTabListText("���۶����б�");
    setTabDetailText("���۶�����ϸ");
    //������ϸ�ϼ��к�
    QList<int > totalItem;
    totalItem << 11 << 13;
    setTotalItem(totalItem);
    initData(lItem,dItem,lSql1,lSql2,dSql1,dSql2);
    initTableHead();
    //�趨�Ƿ�������״̬��ѯ
    setHandleFiled("is_handle");
    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
    //setHandleVisible(false);
}

void SaOrderQueryForm::sTableClicked(QString no){
    if(no.length()>1){
        if(!HaoKeApp::getInstance()->gui()->subWinIsOpen(tr("Sales Order"))){
            SaOrderWin *pWin = new SaOrderWin(no,this);
            pWin->setWindowTitle(tr("Sales Order"));
            HaoKeApp::getInstance()->gui()->addSubWin(pWin);
            pWin->showMaximized();
        }
    }
}
