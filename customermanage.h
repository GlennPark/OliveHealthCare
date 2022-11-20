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
    explicit CustomerManage(QWidget *parent = nullptr);
    ~CustomerManage();

    void dataSave();

private slots:
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void showContextMenu(const QPoint &);

    void removeItem();              /* QAction을 위한 슬롯 */
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_searchPushButton_clicked();
//    void acceptCustomerInfo(int);

    // 구매 정보 등록 시 해당되는 고객 정보를 전송하는 슬롯함수
    void pInfoAddSlotCfromP(int);

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



private:
    //    QList<Customer *>Customers;

//    QList<int> CidList;
//    QList<QString> cNameList;

    int makeCid();

    QStandardItemModel *sModel;
    QSqlTableModel *cModel;

    Ui::CustomerManage *ui;
    QMenu *menu;

    QSqlQuery *query;


};

#endif // CUSTOMERMANAGE_H
