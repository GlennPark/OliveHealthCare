#ifndef CUSTOMERMANAGE_H
#define CUSTOMERMANAGE_H

#include <QWidget>
#include <QHash>


//#include <QAbstractListModel>
//#include "customerlist.h"

class CustomerList;
class QTreeWidgetItem;
class QMenu;

class QSqlDatabase;
class QSqlTableModel;

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
    void acceptCustomerInfo(int);


    void on_tableView_clicked(const QModelIndex &index);


signals:
    void addedCustomer(int);
    void sendCustomerInfo(QString, QString, QString, QString, QString, QString, int, QString,  QString);

private:
    //    QList<Customer *>Customers;

    int makeCid();

    QSqlTableModel *cModel;
    QMap<int, CustomerList*> customerList;
    Ui::CustomerManage *ui;
    QMenu* menu;


};

#endif // CUSTOMERMANAGE_H
