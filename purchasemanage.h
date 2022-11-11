#ifndef PURCHASEMANAGE_H
#define PURCHASEMANAGE_H

#include <QWidget>
#include <QHash>

class PurchaseList;
class QTreeWidgetItem;
class QMenu;

class QSqlDatabase;
class QSqlTableModel;

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

    void on_customerComboBox_currentIndexChanged(int index);
    void on_merchandiseComboBox_currentIndexChanged(int index);
    void on_quantitySpinBox_valueChanged(int arg1);
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_searchPushButton_clicked();



public slots:
    void addCustomer(int);
    void addMerchandise(int);
    void getCustomerInfo(QString, QString, QString);     // 응답
    void getMerchandiseInfo(QString, int, int);     // 응답

    /* QTreeWidget을 위한 슬롯 */
    void removeItem();              /* QAction을 위한 슬롯 */

signals:
    void getCustomerInfo(int);            // 요청
    void getMerchandiseInfo(int);           // 요청

private:
    int makePid();
    QSqlTableModel *pModel;
    Ui::PurchaseManage *ui;
    QList<int> CidList;
    QList<int> MidList;
    QMap<int, PurchaseList*> purchaseList;
    QMenu* menu;
};


#endif // PURCHASEMANAGE_H
