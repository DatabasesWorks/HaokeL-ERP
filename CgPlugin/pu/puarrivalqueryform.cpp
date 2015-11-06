#include "puarrivalqueryform.h"
#include "bean/SupplieBean.h"
#include "bean/SupplieTypeBean.h"
#include "dao/suppliedao.h"
#include "dao/supplietypedao.h"
#include "puarrivalwin.h"
#include "haokeapp.h"
#include "cgplugin.h"

PuArrivalQueryForm::PuArrivalQueryForm(QWidget *parent) :
    QueryForm(parent)
{
    QString lSql1 = "select e.*,d.emp_name from (select f.*,c.dep_name from "
            " (select a.*,b.supplie_name from pu_arrival_vouch a "
            " left join ba_suppliers b on a.supplier_id=b.supplie_code "
            " WHERE a.bus_type='01' AND a.bus_date>=? AND a.bus_date<=? AND b.supplie_type like ? AND a.supplier_id like ? ) f "
            " left join ba_department c on f.dep_code=c.dep_id) e left join ba_employee d on e.person_code=d.emp_code ORDER BY e.bus_date,e.no ASC";
    QString lSql2 = "select e.*,d.emp_name from (select f.*,c.dep_name from "
            " (select a.*,b.supplie_name from pu_arrival_vouch a "
            " left join ba_suppliers b on a.supplier_id=b.supplie_code "
            " WHERE a.no like ? AND a.bus_type='01') f"
            " left join ba_department c on f.dep_code=c.dep_id) e left join ba_employee d on e.person_code=d.emp_code ";

    childItemProperties listItem[] = {
            { "no","����","����", 0,100, true, true },
            { "bus_date","��������","��������", 1,80, true, true },
            { "pu_type","��������","��������", 2,80, true, false },
            { "supplier_id","����","��Ӧ��", 3,80, true, false },
            { "supplie_name","����","��Ӧ��", 4, 120,true, true },
            { "dep_name","����","����", 5,80, true, false },
            { "emp_name","�ɹ�Ա","�ɹ�Ա", 6,60, true, false },
            { "handler","���","���", 7,60, true, false },
            { "audit_time","���ʱ��","���ʱ��", 8,100, true, false },
            { "creater","�Ƶ�","�Ƶ�", 9,60, true, false },
            { "memo","��ע","��ע", 10,180, true, false },

    };
    QString dSql1 =
            " SELECT a.id, a.no, a.bus_date, a.bus_type, a.pu_type, a.supplier_id, b.supplie_name, a.wh_code,a.dep_code,c.order_no,c.product_code,"
                       " d.product_name, c.product_style, c.unit, a.person_code, a.handler, a.audit_time, a.creater, a.created,"
                       " a.closer, a.updater, a.updated, a.status, a.memo, c.quantity,c.price,c.money "
                       " from pu_arrival_vouch a,pu_arrival_vouchs c,ba_product d,ba_suppliers b "
                       " WHERE a.bus_type='01' AND a.bus_date>=? AND a.bus_date<=? AND b.supplie_type like ? AND a.supplier_id like ? "
                       " AND d.type_id like ? AND c.product_code like ?"
                       " AND a.no=c.arrival_no AND c.product_code=d.product_code AND a.supplier_id= b.supplie_code "
                       " ORDER BY id ASC";
    QString dSql2 = "select f.*,b.supplie_name from ( "
            "SELECT a.id, a.no, a.bus_date, a.bus_type, a.pu_type, a.supplier_id,a.wh_code,a.dep_code,c.order_no,c.product_code,"
            " d.product_name, c.product_style, c.unit, a.person_code, a.handler, a.audit_time, a.creater, a.created,"
            " a.closer, a.updater, a.updated, a.status, a.memo, c.quantity,c.price,c.money "
            " from pu_arrival_vouch a,pu_arrival_vouchs c,ba_product d"
            "  WHERE a.no like ? AND a.bus_type='01' AND a.no=c.arrival_no AND c.product_code=d.product_code) f "
            " left join ba_suppliers b on f.supplier_id= b.supplie_code ORDER BY id ASC";
    childItemProperties childItem[] = {
            { "no","��������","��������", 0,100, true, true },
            { "bus_date","��������","��������", 1,80, true, true },
            { "pu_type","��������","��������", 2,80, true, false },
            { "supplier_id","����","��Ӧ��", 3,60, true, false },
            { "supplie_name","����","��Ӧ��", 4, 120,true, true },
            { "order_no","������","������", 5,100, true, true },
            { "product_code","����","��Ʒ", 6,80, true, true },
            { "product_name","����","��Ʒ", 7,100, true, true },
            { "product_style","���","��Ʒ",8,80, true, false },
            { "unit","�ɹ���λ","�ɹ���λ", 9,60, true, false },
            { "quantity","����","����", 10, 60,true, true },
            { "price","�ɹ�����","�ɹ�����", 11,80, true, true },
            { "money","�ɹ����","�ɹ����", 12,80, true, false }

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
    QString formId = HaoKeApp::puSubId() + CgPlugin::arrivalQueryWinId();
    QString winTitle = "�ɹ���������ѯ";
    HaoKeApp::writeSysLog(HaoKeApp::getInstance()->getAccountId() ,winTitle,HaoKeApp::puSubId().toInt(),formId.toInt(),0,"");

    setFormId(formId);
    setTabListText("�ɹ��������б�");
    setTabDetailText("�ɹ�������ϸ");
    //������ϸ�ϼ��к�
    QList<int > totalItem;
    totalItem << 10 << 12;
    setTotalItem(totalItem);
    initData(lItem,dItem,lSql1,lSql2,dSql1,dSql2);
    initTableHead();
    //�趨�Ƿ�������״̬��ѯ
    setHandleFiled("is_handle");
    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
    //setHandleVisible(false);
}

void PuArrivalQueryForm::sTableClicked(QString no){
    if(no.length()>1){
        if(!HaoKeApp::getInstance()->gui()->subWinIsOpen(tr("Purchase Arrival"))){
            PuArrivalWin *pWin = new PuArrivalWin(no,this);
            pWin->setWindowTitle(tr("Purchase Arrival"));
            HaoKeApp::getInstance()->gui()->addSubWin(pWin);
            pWin->showMaximized();
        }
    }
}
