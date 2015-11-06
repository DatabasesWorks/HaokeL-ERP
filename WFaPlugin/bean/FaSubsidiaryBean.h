#ifndef FASUBSIDIARYBEAN_H
#define FASUBSIDIARYBEAN_H
#include <QObject>
#include <QDateTime>
#include "lib_global.h"
class FaSubsidiaryBean : public QObject{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString vouchNo READ vouchNo WRITE setVouchNo)
    Q_PROPERTY(int rdFlag READ rdFlag WRITE setRdFlag)
    Q_PROPERTY(QString vouchType READ vouchType WRITE setVouchType)
    Q_PROPERTY(QString batchId READ batchId WRITE setBatchId)
    Q_PROPERTY(double batchAmount READ batchAmount WRITE setBatchAmount)
    Q_PROPERTY(QString vouchSource READ vouchSource WRITE setVouchSource)
    Q_PROPERTY(QDate busDate READ busDate WRITE setBusDate)
    Q_PROPERTY(QString busType READ busType WRITE setBusType)
    Q_PROPERTY(QString busNo READ busNo WRITE setBusNo)
    Q_PROPERTY(QString orderNo READ orderNo WRITE setOrderNo)
    Q_PROPERTY(QString dvNo READ dvNo WRITE setDvNo)
    Q_PROPERTY(int dvId READ dvId WRITE setDvId)
    Q_PROPERTY(QString settleNo READ settleNo WRITE setSettleNo)

    Q_PROPERTY(QString supCode READ supCode WRITE setSupCode)
    Q_PROPERTY(QString whCode READ whCode WRITE setWhCode)
    Q_PROPERTY(QString depCode READ depCode WRITE setDepCode)
    Q_PROPERTY(QString personCode READ personCode WRITE setPersonCode)
    Q_PROPERTY(QString ptCode READ ptCode WRITE setPtCode)
    Q_PROPERTY(QString stCode READ stCode WRITE setStCode)
    Q_PROPERTY(QString cusCode READ cusCode WRITE setCusCode)

    Q_PROPERTY(QDate keepDate READ keepDate WRITE setKeepDate)
    Q_PROPERTY(int month READ month WRITE setMonth)
    Q_PROPERTY(QString pzId READ pzId WRITE setPzId)
    Q_PROPERTY(QDate pzDate READ pzDate WRITE setPzDate)
    Q_PROPERTY(QString pzType READ pzType WRITE setPzType)
    Q_PROPERTY(QString pzDigest READ pzDigest WRITE setPzDigest)

    Q_PROPERTY(int vouchsId READ vouchsId WRITE setVouchsId)
    Q_PROPERTY(QString productCode READ productCode WRITE setProductCode)
    Q_PROPERTY(double inQuantity READ inQuantity WRITE setInQuantity)
    Q_PROPERTY(double outQuantity READ outQuantity WRITE setOutQuantity)
    Q_PROPERTY(double stQuantity READ stQuantity WRITE setStQuantity)
    Q_PROPERTY(double inCost READ inCost WRITE setInCost)
    Q_PROPERTY(double outCost READ outCost WRITE setOutCost)
    Q_PROPERTY(double outPrice READ outPrice WRITE setOutPrice)
    Q_PROPERTY(double inMoney READ inMoney WRITE setInMoney)
    Q_PROPERTY(double outMoney READ outMoney WRITE setOutMoney)
    Q_PROPERTY(double stMoney READ stMoney WRITE setStMoney)
    Q_PROPERTY(QString accounter READ accounter WRITE setAccounter)

    Q_PROPERTY(QDateTime created READ created WRITE setCreated)
    Q_PROPERTY(int createdBy READ createdBy WRITE setCreatedBy)
    Q_PROPERTY(QString creater READ creater WRITE setCreater)
    Q_PROPERTY(QString memo READ memo WRITE setMemo)
public:
    int id() { return m_id;}
    void setId(int id) { m_id = id ;}
    QString vouchNo() { return m_vouch_no;}
    void setVouchNo(QString no) { m_vouch_no = no;}
    int rdFlag() { return m_rd_flag;}
    void setRdFlag(int flag){ m_rd_flag = flag;}
    QString vouchType() { return m_vouch_type;}
    void setVouchType(QString type){ m_vouch_type = type;}
    QString batchId() { return m_batch_id;}
    void setBatchId(QString id) { m_batch_id = id;}
    double batchAmount() { return m_batch_amount;}
    void setBatchAmount(double value) { m_batch_amount = value;}
    QString vouchSource() { return m_vouch_source;}
    void setVouchSource(QString s){ m_vouch_source = s;}
    QDate busDate() { return m_bus_date;}
    void setBusDate(QDate date){ m_bus_date = date;}
    QString busType() { return m_bus_type;}
    void setBusType(QString type){ m_bus_type = type;}
    QString busNo() { return m_bus_no;}
    void setBusNo(QString no){ m_bus_no = no;}
    QString orderNo() { return m_order_no;}
    void setOrderNo(QString no){ m_order_no = no;}
    QString dvNo() { return m_dv_no;}
    void setDvNo(QString no){ m_dv_no = no;}
    int dvId() { return m_dv_id;}
    void setDvId(int id){ m_dv_id = id;}
    QString settleNo() { return m_settle_no;}
    void setSettleNo(QString no){ m_settle_no = no;}

    QString supCode() { return m_sup_code;}
    void setSupCode(QString value){ m_sup_code = value;}
    QString whCode() { return m_wh_code;}
    void setWhCode(QString code){ m_wh_code = code;}
    QString depCode() { return m_dep_code;}
    void setDepCode(QString code) { m_dep_code = code;}
    QString cusCode() { return m_cus_code;}
    void setCusCode(QString code){ m_cus_code = code;}

    QString personCode() { return m_person_code;}
    void setPersonCode(QString code){ m_person_code = code;}
    QString ptCode() { return m_pt_code;}
    void setPtCode(QString code){ m_pt_code = code;}
    QString stCode() { return m_st_code;}
    void setStCode(QString code){ m_st_code = code;}

    QDate keepDate() { return m_keep_date;}
    void setKeepDate(QDate date){ m_keep_date = date;}
    int month() { return m_month;}
    void setMonth(int month){ m_month = month;}
    QString pzId() { return m_pz_id;}
    void setPzId(QString id){ m_pz_id = id;}
    QDate pzDate() { return m_pz_date;}
    void setPzDate(QDate date){ m_pz_date = date;}
    QString pzType() { return m_pz_type;}
    void setPzType(QString type){ m_pz_type = type;}
    QString pzDigest() { return m_pz_digest;}
    void setPzDigest(QString type){ m_pz_digest = type;}

    int vouchsId(){ return m_vouchs_id;}
    void setVouchsId(int id){ m_vouchs_id = id ;}
    QString productCode() { return m_product_code;}
    void setProductCode(QString code){ m_product_code = code;}
    double inQuantity(){ return m_in_quantity;}
    void setInQuantity(double value){ m_in_quantity = value; }
    double outQuantity(){ return m_out_quantity;}
    void setOutQuantity(double value){ m_out_quantity = value; }
    double stQuantity(){ return m_st_quantity;}
    void setStQuantity(double value){ m_st_quantity = value; }
    double inCost(){ return m_in_cost;}
    void setInCost(double value){ m_in_cost = value; }
    double outCost(){ return m_out_cost;}
    void setOutCost(double value){ m_out_cost = value; }
    double outPrice(){ return m_out_price;}
    void setOutPrice(double value){ m_out_price = value; }
    double inMoney(){ return m_in_money;}
    void setInMoney(double value){ m_in_money = value; }
    double outMoney(){ return m_out_money;}
    void setOutMoney(double value){ m_out_money = value; }
    double stMoney(){ return m_st_money;}
    void setStMoney(double value){ m_st_money = value; }
    QString accounter(){return m_accounter; }
    void setAccounter(QString value){ m_accounter = value;}

    QDateTime created(){ return m_created; }
    void setCreated(QDateTime time){m_created = time;}
    int createdBy(){return m_created_by; }\
    void setCreatedBy(int user){ m_created_by = user;}
    QString creater(){return m_creater; }
    void setCreater(QString user){ m_creater = user;}


    QString memo(){return m_memo;}
    void setMemo(QString memo){ m_memo = memo;}
private:
    int m_id;
    int m_rd_flag; //�շ���־
    QString m_vouch_no; //���ݱ��
    QString m_vouch_type;//�������ͱ���
    QString m_batch_id; //����
    double m_batch_amount; //���ν������ �Ƚ��ȳ��õ�
    QString m_bus_type; //ҵ������
    QString m_bus_no; //��Ӧҵ�񵥺� �ɹ�����/���۷�������
    QString m_vouch_source; //������Դ
    QString m_order_no; //�ɹ�/���۶�����
    QString m_dv_no; //����ɱ����� ȡ��ⵥ�ݱ��� �Ƚ��ȳ��õ�
    int m_dv_id; //����ɱ����� ȡ��� ID �Ƚ��ȳ��õ�
    QString m_settle_no; //���㵥��
    QDate m_bus_date; //��������

    QString m_wh_code; //�ֿ����
    QString m_dep_code; //���ű���
    QString m_person_code; //ҵ��Ա����
    QString m_pt_code; //�ɹ����ͱ���
    QString m_st_code; //�������ͱ���
    QString m_cus_code; //�ͻ�����
    QString m_sup_code; //��Ӧ�̱���

    QDate m_keep_date;
    int m_month;
    QString m_pz_id;
    QDate m_pz_date;
    QString m_pz_type;
    QString m_pz_digest;

    int m_vouchs_id;
    QString m_product_code;
    double m_in_quantity;
    double m_out_quantity;
    double m_st_quantity;
    double m_in_cost;
    double m_out_cost;
    double m_out_price;
    double m_in_money;
    double m_out_money;
    double m_st_money;
    QString m_accounter;

    QDateTime m_created;
    int m_created_by;
    QString m_creater;
    QString m_memo;

};
#endif // FASUBSIDIARYBEAN_H
