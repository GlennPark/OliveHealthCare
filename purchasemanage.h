#ifndef PURCHASEMANAGE_H
#define PURCHASEMANAGE_H

#include <QWidget>
#include <QHash>


class QMenu;

class QSqlDatabase;
class QSqlTableModel;
class QStandardItemModel;
class QSqlQuerry;


namespace Ui {
class PurchaseManage;
}

class PurchaseManage : public QWidget
{
    Q_OBJECT

public:
    //생성자와 소멸자
    explicit PurchaseManage(QWidget *parent = nullptr);
    ~PurchaseManage();

    // 구매 데이터 베이스 저장
    void dataSave();


public slots:
    // 회원 및 제품 정보 등록 시 구매 테이블에서 전달받는 슬롯 함수
    void cInfoAddSlotPfromC(int);
    void mInfoAddSlotPfromM(int);

    // 회원 및 제품 정보 수정 시 구매 테이블에서 전달받는 슬롯 함수
    void cInfoModSlotPfromC(int, QList<QString>);
    void mInfoModSlotPfromM(int, QList<QString>);

    // 회원 및 제품 정보 삭제 시 구매 테이블에서 전달받는 슬롯 함수
    void cInfoDelSlotPfromC(int);
    void mInfoDelSlotPfromM(int);

signals:
    // 구매 정보 입력 시 회원 및 제품 테이블에 정보를 요청하는 시그널
    void pInfoAddSignPtoC(int);
    void pInfoAddSignPtoM(int);

    // 구매 정보 검색 시 회원 및 제품 테이블에 정보를 요청하는 시그널
    void pInfoSearchSignPtoC(int);
    void pInfoSearchSignPtoM(int);

    // 구매 정보 수정 시 회원 및 제품 테이블에 정보를 요청하는 시그널
    void pInfoModSignPtoC(int, int);
    void pInfoModSignPtoM(int, int);



private slots:

    void on_nameComboBox_currentIndexChanged(int index);
    void on_mnameComboBox_currentIndexChanged(int index);
 //   void on_merchandiseComboBox_currentIndexChanged(int index);
    void on_priceSpinBoy_valueChanged(int arg1);
    void on_quantitySpinBox_valueChanged(int arg2);
 //   void on_treeWidget_customContextMenuRequested(const QPoint &pos);



    // 우클릭 시 보조 메뉴 등장
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void showContextMenu(const QPoint &);

    // 보조 메뉴를 통한 데이터 삭제
    void removeItem();

    // 구매 정보 등록 버튼 클릭 시 작동하는 슬롯함수
    void on_addPushButton_clicked();

    // 구매 정보 수정 버튼 클릭 시 작동하는 슬롯함수
    void on_modifyPushButton_clicked();

    // 구매 정보 검색 버튼 클릭 시 작동하는 슬롯함수
    void on_searchPushButton_clicked();

    // 구매 정보 추가 시 회원 및 제품 정보를 반환하는 슬롯함수
    void pInfoAddReturnSlotPfromC(QList<QString>);
    void pInfoAddReturnSlotPfromM(QList<QString>);

    // 구매 정보 검색 시 회원 및 제품 정보를 반환하는 슬롯함수
    void pInfoSearchReturnSlotPfromC(QList<QString>);
    void pInfoSearchReturnSlotPfromM(QList<QString>);

    // 구매 정보 수정 시 회원 및 제품 정보를 반환하는 슬롯함수
    void pInfoModReturnSlotPfromC(QList<QString>, int);
    void pInfoModReturnSlotPfromM(QList<QString>, int);

    // 현재 구매 정보를 ui 입력단에 표시해 주는 슬롯함수
    void on_tableView_clicked(const QModelIndex &index);

private:
    // 구매 키값 변수
    int makePid();

    // tModel Row 변수
    int tR = 0;

    // 구매 정보 저장용 변수
    QSqlTableModel *pModel;

    // 종합 정보 저장용 변수
    QStandardItemModel *tModel;

    // 검색 정보 저장용 변수
    QStandardItemModel *sModel;

    // ui 등록 변수
    Ui::PurchaseManage *ui;

    // 메뉴 등록 변수
    QMenu* menu;

        QStandardItemModel* clientInfoModel;
};


#endif // PURCHASEMANAGE_H
