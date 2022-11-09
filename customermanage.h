#ifndef CUSTOMERMANAGE_H
#define CUSTOMERMANAGE_H

#include <QWidget>
#include <QHash>
#include <QList>

//#include <QAbstractListModel>
//#include "customerlist.h"

class CustomerList;
class QMenu;
class QTreeWidgetItem;


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

    void showContextMenu(const QPoint &);
    void removeItem();              /* QAction을 위한 슬롯 */
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_searchPushButton_clicked();
    void acceptCustomerInfo(int);
    void radioCheck();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_tableView_activated(const QModelIndex &index);

signals:
    void addedCustomer(int, QString);
    void sendCustomerInfo(QString, QString, QString, QString, QString, QString, QString,  QString);

private:
    //    QList<Customer *>Customers;

    int makeCid();

    QMap<int, CustomerList*> customerList;
    Ui::CustomerManage *ui;
    QMenu* menu;

};

#endif // CUSTOMERMANAGE_H
