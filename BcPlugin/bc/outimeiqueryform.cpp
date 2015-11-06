#include "outimeiqueryform.h"
#include "bcplugin.h"
#include "haokeapp.h"
#include "rdoutimeiwin.h"
OutImeiQueryForm::OutImeiQueryForm(QWidget *parent) :
    SingleQueryForm(parent)
{
    QString dSql1 = "SELECT a.*, c.emp_name, b.dep_name "
                " FROM rd_record a "
                " LEFT JOIN ba_department b ON a.dep_code=b.dep_id "
                " LEFT JOIN ba_employee c ON a.person_code=c.emp_code "
                " WHERE a.bus_date>=? AND a.bus_date<=? "
                " AND ( a.vouch_type='02' OR a.vouch_type='14' OR a.vouch_type='11' "
                " OR (a.vouch_type='07' AND a.bus_type<>'��������') ) "
                " AND is_handle=true "
                " ORDER BY a.bus_date,a.id ASC";
    QString dSql2 = " ";
    childItemProperties childItem[] = {
            { "rd_no","����","����", 0,100, true, true },
            { "bus_date","��������","��������", 1,80, true, true },
            { "bus_type","��������","��������", 2,80, true, false },
            { "bus_no","ҵ�񵥺�","ҵ�񵥺�", 3,100, true, false },
            { "dep_name","����","����", 4,120, true, false },
            { "emp_name","ҵ��Ա","ҵ��Ա", 5,60, true, false },
            { "is_handle","�������","�������", 6,60, true, false },
            { "handler","�����","�����", 7,60, true, false },
            { "audit_time","���ʱ��","���ʱ��", 8,140, true, false },
            { "creater","�Ƶ�","�Ƶ�", 9,60, true, false },
            { "imei_audit","�������","�������", 10,60, true, false },
            { "memo","��ע","��ע", 11,200, true, false },

    };

    QList<struct childItemProperties > dItem;
    int size = sizeof(childItem) / sizeof(childItem[0]);
    for(int i = 0 ; i < size ; i++){
        dItem << childItem[i];
    }
    idLabel->setVisible(false);
    noEdit->setVisible(false);
    dwNameQks->setVisible(false);
    dwTypeQks->setVisible(false);
    dwNameLabel->setVisible(false);
    dwTypeLabel->setVisible(false);
    proNameLabel->setVisible(false);
    proNameQks->setVisible(false);
    proTypeLabel->setVisible(false);
    proTypeQks->setVisible(false);
    Label->setText("�������:");
    rBnt2->setText("ĩ���");
    rBnt3->setText("�����");
    rBnt2->setChecked(true);
    //verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Preferred);
    //gridLayout->addItem(verticalSpacer_2, 5, 1, 1, 1);
    QString formId = HaoKeApp::bcSubId() + BcPlugin::rdInListQueryWinId();
    setFormId(formId);
    setTabDetailText("���ⵥ/�ɹ��˻����б�");

    initData(dItem,dSql1,dSql2);
    initTableHead();
    setHandleFiled("imei_audit");

    connect(this,SIGNAL(tableClicked(QString)),this,SLOT(sTableClicked(QString)));
}

void OutImeiQueryForm::sTableClicked(QString no){
    if(no.length()>1){
        if(!HaoKeApp::getInstance()->gui()->subWinIsOpen("�������")){
            RdOutImeiWin *pWin = new RdOutImeiWin(no,this);
            pWin->setWindowTitle("�������");
            HaoKeApp::getInstance()->gui()->addSubWin(pWin);
            pWin->showMaximized();
        }
    }
}

