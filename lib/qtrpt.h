/*
Name: QtRpt
Version: 1.3.4
Programmer: Aleksey Osipov
e-mail: aliks-os@yandex.ru
2012-2014
*/

#ifndef QTRPT_H
#define QTRPT_H

class QWidget;
#include <QPainter>
#include <QDomDocument>
#include <QScriptEngine>
#include <QPrintPreviewWidget>
#include <QtScript>
#include <QPrintDialog>
#include <QStandardItemModel>
#include "common/common.h"
#include "lib_global.h"

enum BandType {
    ReportTitle,
    PageHeader,
    MasterHeader,
    MasterData,
    MasterFooter,
    ReportSummary,
    PageFooter
};

enum HiType {
    FontBold,
    FontItalic,
    FontUnderline,
    FontColor,
    BgColor
};

struct AggregateValues {
    QString paramName;
    double paramValue;
    int lnNo;
};

QScriptValue funcAggregate(QScriptContext *context, QScriptEngine *engine);
static QList<AggregateValues> listOfPair;

class LIBSHARED_EXPORT  QtRPT : public QWidget
{
    Q_OBJECT
public:
    QList<int> recordCount;
    explicit QtRPT(QWidget *parent = 0);    
    bool loadReport(QString fileName);
    bool loadReportFormat(QString format);
    void printExec(bool maximum = false, bool direct = false);
    //void setCallbackFunc(void (*func)(int &recNo, QString &paramName, QVariant &paramValue));
    void setBackgroundImage(QPixmap image);
    void setModel(QAbstractItemModel *model);
    void setChildItem(QList<struct childItemProperties > item);
private:    

    QPixmap *m_backgroundImage;
    QPainter painter;
    int m_recNo;
    int m_pageReport;
    float koefRes_h;
    float koefRes_w;
    int ph;
    int pw;
    int ml;
    int mr;
    int mt;
    int mb;
    int curPage;
    int totalPage;
    QAbstractItemModel *mModel;

    QList<struct childItemProperties > mChildItem;
    QPrintPreviewWidget *pr;
    QList<QAction *> lst ;
    QDomNode reportTitle,pageHeader,masterData,pageFooter,reportSummary,masterFooter,masterHeader;
    QDomDocument *xmlDoc;
    QDomNode getBand(BandType type, QDomElement docElem);
    void drawField(QDomNode n, int bandTop);
    void fillListOfValue(QDomNode n);
    double calcAggregateValue(QString paramName, int funcMode);
    QString sectionField(QString value, bool exp, bool firstPass = false);
    QStringList splitValue(QString value);
    QImage sectionFieldImage(QString value);
    QVariant processFunctions(QString value);
    QString sectionValue(QString paramName);
    QImage sectionValueImage(QString paramName);
    void drawGridField(QDomNode n, int bandTop);
    QString sectionGridField(QString value, bool exp, bool firstPass = false);
    QString sectionGridValue(QString paramName);
    void newPage(QPrinter *printer, int &y, bool draw);
    void processPHeader(int &y, bool draw);
    void processPFooter(bool draw);
    void processMFooter(QPrinter *printer, int &y, bool draw);
    void processRSummary(QPrinter *printer, int &y, bool draw);
    QColor colorFromString(QString value);
    //void (*callbackFunc)(int &recNo, QString &paramName, QVariant &paramValue);
    void processReport(QPrinter *printer, bool draw, int pageReport);
    void processRTitle(int &y, bool draw);
    void processMHeader(int &y, bool draw);
    void processMasterData(QPrinter *printer, int &y, bool draw, int pageReport);
    void setPageSettings(QPrinter *printer, int pageReport);
    void drawBackground(QPainter &painter);
    bool isFieldVisible(const QDomElement &e);
    QVariant processHighligthing(QDomElement e, HiType type);
    bool allowPrintPage(bool draw, int curPage_);
    bool allowNewPage(bool draw, int curPage_);
    int fromPage;
    int toPage;

public:


protected:
    bool eventFilter(QObject *obj, QEvent *e);    

signals:
    void setValue(int &recNo, QString &paramName, QVariant &paramValue, int reportPage);
    void setValueImage(int &recNo, QString &paramName, QImage &paramValue, int reportPage);

public slots:
    void printPreview(QPrinter *printer);

};

#endif // QTRPT_H
