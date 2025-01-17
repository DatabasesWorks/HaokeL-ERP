#ifndef SADELIVERYWIN_H
#define SADELIVERYWIN_H

#include <QMainWindow>
#include "common/common.h"
#include "bean/SaOrderVouchBean.h"
#include "bean/SaOrderVouchsBean.h"
#include "bean/SaDispatchVouchBean.h"
#include "bean/SaDispatchVouchsBean.h"
#include "widget/qkeditdelegate.h"
#include "common/widgetbeanutil.h"
#include "widget/hierarchicalheaderview.h"
#include "widget/foottablemodel.h"
namespace Ui {
class SaDeliveryWin;
}

class SaDeliveryWin : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SaDeliveryWin(QWidget *parent = 0);
    SaDeliveryWin(QString no,QWidget *parent = 0);
    ~SaDeliveryWin();
private slots:
    void sNew();
    void sEdit();
    void sDelete();
    void sSave();
    void sAudit();
    void sAbAudit();
    void sDelRow();
    void sPrint();
    void sChoose();
    void sFirst();
    void sNext();
    void sPrevious();
    void sEnd();
    void sCancel();
    void footResized(int logicalIndex, int oldSize, int newSize );
    void setValue(int &recNo, QString &paramName, QVariant &paramValue, int reportPage);
    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_cusSearchEditbox_editChanged(const QString &arg1);
    void sInvClick();
    void toolBarActTriggered(QAction *act);

private:
    Ui::SaDeliveryWin *ui;
    QToolBar *baseToolBar;
    int mEditStatus;
    QString mWinId;
    QString mWinTitle;
    bool mIsEdit;
    QString mId;
    QString mNo;
    int mRecord;
    int mCountRecord;
    bool mIsHathOrder; //销售业务是否必有订单
    bool mIsMakeRd;    //审核是否生成出库单
    bool mOverTop;     //是否允许超可用量发货
    //是否提取单据
    bool mIsChoose;
    QList<int > mDelChildList;
    QList<SaDispatchVouchsBean *> mChildVouchsList;
    QList<SaDispatchVouchsBean *> mAddChildVouchsList;
    QList<struct childItemProperties > mChildItem;
    HierarchicalHeaderView *mFootView;
    FootTableModel *mFootModel;
    double mTotalQuantity ;
    double mTotalSum;
    void setupContextMenu();
    void closeEvent(QCloseEvent *event);
    void init();
    void refreshData();
    void read(QString no);
    void inputClear();
    void setInputEnable(bool b);
    void addActionsTo(actionProperties [], unsigned int);
    void readAuthority();
};

#endif // SADELIVERYWIN_H
