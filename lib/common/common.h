#ifndef COMMON_H
#define COMMON_H
#include <QMenu>
#include <QString>
#include <QToolBar>
#include <QPixmap>
#include <QIcon>

struct actionProperties {
  const char*		actionName;
  const QString     actionTitle;
  const char*		slot;
  QMenu*            menu;
  QString           priv;
  QPixmap           pixmap;
  QToolBar*         toolBar;
  bool              visible;
  const QString     toolTip;
  const QString     menuId;
};

struct childItemProperties{
    QString name;
    QString cnName;
    QString title;
    const int id;
    const int width;
    bool visible;
    bool isEdit;
};

enum gEditFlag {
    joinFlag= 0x0,
    addFlag = 0x1,
    editFlag= 0x2,
    standeFlag= 0x3
};
enum SaleFlag{
    waitFlag = 0x0,     //�ȴ�
    orderFlag = 0x01,   //�µ�
    supsendFlag = 0x02, //����
    makeFlag = 0x03,    //����
    urgeFlag = 0x04,    //�߲�
    outFlag = 0x05,     //����
    onFlag = 0x06,      //�ϲ�
    endFlag = 0x08,     //���� �ѽ���
    cancelFlag = 0x09   //ȡ��

};


enum RdFlag{
    RdInFlag = 0x01,     //���
    RdOutFlag = 0x02
};
enum VouchTypeFlag{
    VouchInFlag = 0x01,     //���
    VouchOutFlag = 0x02,
    VouchRePuFlag = 0x14
};
#endif // COMMON_H
