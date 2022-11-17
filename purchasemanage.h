#ifndef PURCHASEMANAGE_H
#define PURCHASEMANAGE_H

#include <QWidget>
#include <QHash>


class QMenu;

class QSqlDatabase;
class QSqlTableModel;
class QStandardItemModel;

namespace Ui {
class PurchaseManage;
}

class PurchaseManage : public QWidget
{
    Q_OBJECT
public:
    explicit PurchaseManage(QWidget *parent = nullptr);
    ~PurchaseManage();

    void dataSave();

private slots:
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void showContextMenu(const QPoint &);


    void on_nameComboBox_currentIndexChanged(int index);
    void on_mnameComboBox_currentIndexChanged(int index);
 //   void on_merchandiseComboBox_currentIndexChanged(int index);
    void on_quantitySpinBox_valueChanged(int arg1);
 //   void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_tableView_clicked(const QModelIndex &index);
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_searchPushButton_clicked();
    void acceptPurchaseInfo(int);


public slots:
    void cInfoSlotPfromC(int);
    void addMerchandise(int);
//    void addPurchase(int);
void acceptCustomerInfo(QString, QString, QString, QString, QString, QString, QString, QString);
void addCustomer(int, QString);
    // 응답

    /* QTreeWidget을 위한 슬롯 */
    void removeItem();              /* QAction을 위한 슬롯 */

signals:
   void getCustomerInfo(int);            // 요청
    void getMerchandiseInfo(int);
    void getPurchaseInfo(int);
    void sendPurchaseInfo(QString, QString, QString, QString, int, int, int, int, QString);
    void addedPurchase(int);

private:
    int makePid();
    QStandardItemModel *sModel;
    QSqlTableModel *pModel;
    QStandardItemModel* clientInfoModel;
    Ui::PurchaseManage *ui;
    QMenu* menu;
};


#endif // PURCHASEMANAGE_H
