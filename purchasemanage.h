#ifndef PURCHASEMANAGE_H
#define PURCHASEMANAGE_H

#include <QWidget>

class PurchaseList;
class QMenu;
class QTreeWidgetItem;

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

private:
    int makePid();
    Ui::PurchaseManage *ui;
    QList<int> CidList;
    QList<int> MidList;
    QMap<int, PurchaseList*> purchaseList;
    QMenu* menu;

public slots:
    void addCustomer(int, QString);
    void addMerchandise(int, QString);
    void getCustomerInfo(QString, QString, QString);     // 응답
    void getMerchandiseInfo(QString, int, int);     // 응답

    /* QTreeWidget을 위한 슬롯 */
    void removeItem();              /* QAction을 위한 슬롯 */

signals:
    void getCustomerInfo(int);            // 요청
    void getMerchandiseInfo(int);           // 요청

private slots:
    void on_customerComboBox_currentIndexChanged(int index);
    void on_merchandiseComboBox_currentIndexChanged(int index);
    void on_quantitySpinBox_valueChanged(int arg1);
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_searchPushButton_clicked();
};


#endif // PURCHASEMANAGE_H
