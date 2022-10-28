#ifndef MERCHANDISEMANAGE_H
#define MERCHANDISEMANAGE_H

#include <QWidget>
#include <QMap>

class MerchandiseList;
class QMenu;
class QTreeWidgetItem;

namespace Ui {
class MerchandiseManage;
}

class MerchandiseManage : public QWidget
{
    Q_OBJECT
public:
    explicit MerchandiseManage(QWidget *parent = nullptr);
    ~MerchandiseManage();
    void dataSave();

private slots:
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_searchPushButton_clicked();
    void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);

    /* QTreeWidget을 위한 슬롯 */
    void removeItem();              /* QAction을 위한 슬롯 */
    void acceptMerchandiseInfo(int);

signals:
    void addMerchasdise(int, QString);
    void addedMerchandise(int, QString);
    void sendMerchandiseInfo(QString, int, int);

private:
    int makeid();

    QMap<int, MerchandiseList*> merchandiseList;
    Ui::MerchandiseManage *ui;
    QMenu* menu;

};

#endif // MERCHANDISEMANAGE_H
