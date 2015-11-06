#ifndef STRDWIN_H
#define STRDWIN_H

#include <QMainWindow>
#include "common/common.h"
#include "bean/RdBean.h"
#include "bean/RdsBean.h"
#include "widget/qkeditdelegate.h"
#include "common/widgetbeanutil.h"
#include "widget/hierarchicalheaderview.h"
#include "widget/foottablemodel.h"
namespace Ui {
class StRdWin;
}

class StRdWin : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit StRdWin(QWidget *parent = 0);
    StRdWin(QString no, QWidget *parent = 0);
    ~StRdWin();
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
    void on_tableWidget_itemChanged(QTableWidgetItem *item);
    void footResized(int logicalIndex, int oldSize, int newSize );
    void setValue(int &recNo, QString &paramName, QVariant &paramValue, int reportPage);
    void toolBarActTriggered(QAction *act);
private:
    Ui::StRdWin *ui;
    QToolBar *baseToolBar;
    int mEditStatus;
    QString mWinId;
    QString mWinTitle;
    bool mIsEdit;
    QString mId;
    QString mNo;
    QString mVouchType;
    QString mVouchSource;
    //�Ƿ���ֵ���
    int mRdRed;
    int mRdFlag;
    int mRecord;
    int mCountRecord;
    bool mIsChoose;
    bool mIsHathOrder; //�ɹ�ҵ���Ƿ���ж���
    bool mIsMakeRd;    //����Ƿ�������ⵥ
    QList<int > mDelChildList;
    QList<RdsBean *> mChildVouchsList;
    QList<RdsBean *> mAddChildVouchsList;
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

#endif // STRDWIN_H
