#ifndef MERCHANDISEMANAGE_H
#define MERCHANDISEMANAGE_H

#include <QWidget>
#include <QMap>


class QMenu;

class QSqlDatabase;
class QSqlTableModel;
class QStandardItemModel;

namespace Ui {
class MerchandiseManage;
}

class MerchandiseManage : public QWidget
{
    Q_OBJECT

public:
    // 생성자와 소멸자
    explicit MerchandiseManage(QWidget *parent = nullptr);
    ~MerchandiseManage();

    // 데이터 베이스 저장
    void dataSave();

private slots:
    // 우클릭 시 보조메뉴 설정
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void showContextMenu(const QPoint &);

    // 보조 메뉴를 이용한 제품 데이터 삭제용 슬롯함수
    void removeItem();

    // 제품 정보 등록 버튼 클릭 시 작동하는 슬롯함수
    void on_addPushButton_clicked();

    // 제품 정보 수정 버튼 클릭 시 작동하는 슬롯함수
    void on_modifyPushButton_clicked();

    // 제품 정보 검색 버튼 클릭 시 작동하는 슬롯함수
    void on_searchPushButton_clicked();



    // 구매 클래스에서 등록시 해당되는 제품 정보를 전송하는 슬롯함수
    void pInfoAddSlotMfromP(int);

    // 구매 클래스에서 검색시 해당되는 제품 정보를 전송하는 슬롯함수
    void mInfoSearchSlotCfromP(int);

    // 구매 클래스에서 수정시 해당되는 제품 정보를 전송하는 슬롯함수
    void mInfoModSlotMtoP(int, int);

    // 현재 제품 정보를 ui 입력단에 표시해 주는 슬롯
    void on_tableView_clicked(const QModelIndex &index);


signals:
    // 제품 정보가 등록되었을 때 구매 관리 클래스에 전달하기 위한 시그널
    void mInfoAddSignMtoP(int);

    // 제품 정보가 수정되었을 때 구매 관리 클래스에 전달하기 위한 시그널
    void mInfoModSignMtoP(int, QList<QString>);

    // 제품 정보가 삭제되었을 때 구매 관리 클래스에 전달하기 위한 시그널
    void mInfoDelSignMtoP(int);

    // 구매 정보 등록 시 해당되는 제품 정보를 리스트로 반환하는 시그널
    void pInfoAddReturnSignMtoP(QList<QString>);

    // 구매 정보 검색 시 해당되는 제품 정보를 리스트로 반환하는 시그널
    void pInfoSearchReturnSignMtoP(QList<QString>);

    // 구매 정보 수정 시 해당되는 제품 정보와 열 값을 반환하는 시그널
    void pInfoModReturnSignMtoP(QList<QString>, int);

private:

    // 제품 키값 변수
    int makeMid();

    // 제품 정보 저장용 변수
    QSqlTableModel *mModel;

    // 제품 정보 검색용 변수
    QStandardItemModel *sModel;

    // ui 등록 변수
    Ui::MerchandiseManage *ui;

    // 메뉴 등록 변수
    QMenu *menu;

};

#endif // MERCHANDISEMANAGE_H
