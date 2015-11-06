#ifndef RDOUTIMEIWIN_H
#define RDOUTIMEIWIN_H

#include <QMainWindow>
#include "common/common.h"
#include "bean/RdBean.h"
#include "bean/RdsBean.h"
#include "bean/RdInImeiBean.h"
#include "dao/imeidao.h"
#include "widget/qkeditdelegate.h"
#include "common/widgetbeanutil.h"
#include "widget/hierarchicalheaderview.h"
#include "widget/foottablemodel.h"
#include "log4qt/logger.h"
namespace Ui {
class RdOutImeiWin;
}

class RdOutImeiWin : public QMainWindow
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit RdOutImeiWin(QWidget *parent = 0);
    RdOutImeiWin(QString no,QWidget *parent = 0);
    RdOutImeiWin(int rdf, QString vouchType, QWidget *parent = 0);
    ~RdOutImeiWin();
    
private slots:
    void sEdit();
    void sDelete();
    void sAudit();
    void sAbAudit();
    void sPrint();
    void sExport();
    void sFirst();
    void sNext();
    void sPrevious();
    void sEnd();
    void sCancel();
    void sDelRow();
    void footResized(int logicalIndex, int oldSize, int newSize );
    void setValue(int &recNo, QString &paramName, QVariant &paramValue, int reportPage);
    void on_tableWidget_clicked(const QModelIndex &index);

    void on_saveButton_clicked();

    void on_importButton_clicked();

    void on_clearButton_clicked();

    void on_delButton_clicked();

    void on_tableWidget2_itemChanged(QTableWidgetItem *item);

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void toolBarActTriggered(QAction *act);
private:
    Ui::RdOutImeiWin *ui;
    QToolBar *baseToolBar;
    int mEditStatus;
    QString mWinId;
    QString mWinTitle;
    bool mIsEdit;
    bool mIsEditMode;
    bool mIsImport;
    QString mId;
    QString mNo;
    QString mVouchType;
    QString mVouchSource;
    QString mIdx;   //����ID
    QString mInvCode; //��Ʒ����
    int mQuantity;
    int mRow;
    int mRdFlag;
    int mRdRed;
    int mRecord;
    int mCountRecord;
    int mImeiCount;
    bool mIsChoose;
    bool mIsHathOrder; //�ɹ�ҵ���Ƿ���ж���
    bool mIsMakeRd;    //����Ƿ�������ⵥ
    QList<int > mDelChildList;

    QList<RdInImeiBean *> mChildVouchsList;
    QList<RdInImeiBean *> mAddChildVouchsList;
    QList<struct childItemProperties > mChildItem;
    QList<struct childItemProperties > mImeiItem;
    HierarchicalHeaderView *mFootView;
    FootTableModel *mFootModel;
    double mTotalQuantity ;
    double mTotalSum;
    QAction *mActDelRow;
    void readAuthority();
    void setupContextMenu();
    void closeEvent(QCloseEvent *event);
    void init();
    void createActions();
    void refreshData();
    void initTableHead2();
    void read(QString no);
    void inputClear();
    void setInputEnable(bool b);
    bool exportExcel(QString fileName, QTableWidget *tableWidget);
    void addActionsTo(actionProperties [], unsigned int);
};

#endif // RDOUTIMEIWIN_H
