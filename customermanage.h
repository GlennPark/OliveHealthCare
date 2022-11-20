#ifndef CUSTOMERMANAGE_H
#define CUSTOMERMANAGE_H

#include <QWidget>
#include <QHash>


class QMenu;

class QSqlDatabase;
class QSqlTableModel;
class QStandardItemModel;
class QSqlQuery;

namespace Ui {
class CustomerManage;
}

class CustomerManage : public QWidget
{
    Q_OBJECT

public:
    // 생성자와 소멸자
    explicit CustomerManage(QWidget *parent = nullptr);
    ~CustomerManage();

    // 데이터 베이스 저장
    void dataSave();

private slots:
    // 우클릭 시 보조메뉴 설정
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void showContextMenu(const QPoint &);

    // 보조 메뉴를 이용한 회원 데이터 삭제용 슬롯함수
    void removeItem();

    // 회원 정보 등록 버튼 클릭 시 작동하는 슬롯함수
    void on_addPushButton_clicked();

    // 회원 정보 수정 버튼 클릭 시 작동하는 슬롯함수
    void on_modifyPushButton_clicked();

    // 제품 정보 검색 버튼 클릭 시 작동하는 슬롯함수
    void on_searchPushButton_clicked();

    // 구매 정보 등록 시 해당되는 회원 정보를 전송하는 슬롯함수
    void pInfoAddSlotCfromP(int);

    // 구매 정보 검색 시 해당되는 회원 정보를 전송하는 슬롯함수
    void pInfoSearchSlotCfromP(int);

    // 구매 정보 수정 시 해당되는 회원 정보를 전송하는 슬롯함수
    void pInfoModSlotCfromP(int, int);

    // 현재 회원 정보를 ui 입력단에 표시해 주는 슬롯함수
    void on_tableView_clicked(const QModelIndex &index);


signals:
    // 프로그램 시작 시 회원 정보를 채팅 서버에 전달하기 위한 시그널
    void cInfoSignCtoChat(QList<int>, QList<QString>);

    // 회원 정보가 등록되었을 때 구매 관리 클래스에 전달하기 위한 시그널
    void cInfoAddSignCtoP(int);

    // 회원 정보가 등록되었을 때 채팅 서버에 전달하기 위한 시그널
    void cInfoAddSignCtoChat(QList<int>, QList<QString>);

    // 회원 정보가 수정되었을 때 구매 관리 클래스에 전달하기 위한 시그널
    void cInfoModSignCtoP(int, QList<QString>);

    // 회원 정보가 수정되었을 때 채팅 서버에 전달하기 위한 시그널
    void cInfoModSignCtoChat(int, QString);

    // 회원 정보가 삭제되었음을 구매 관리 클래스에 전달하기 위한 시그널
    void cInfoDelSignCtoP(int);

    // 회원 정보가 삭제되었음을 채팅 서버에 전달하기 위한 시그널
    void cInfoDelSignCtoChat(QString);

    // 구매 정보 등록 시 해당되는 고객 정보를 리스트로 반환하는 시그널
    void pInfoAddReturnSignCtoP(QList<QString>);

    // 구매 정보 검색 시 해당되는 고객 정보 키값을 반환하는 시그널
    void pInfoSearchReturnSignCtoP(QList<QString>);

    // 구매 정보 수정 시 해당되는 고객 정보와 열 값을 반환하는 시그널
    void pInfoModReturnSignCtoP(QList<QString>, int);

private:

    // 회원 키값 변수
    int makeCid();

    // 회원 정보 저장용 변수
    QSqlTableModel *cModel;

    // 회원 정보 검색용 변수
    QStandardItemModel *sModel;

    // ui 등록 변수
    Ui::CustomerManage *ui;

    // 메뉴 등록 변수
    QMenu *menu;

};

#endif // CUSTOMERMANAGE_H
