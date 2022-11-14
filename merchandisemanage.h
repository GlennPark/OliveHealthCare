#ifndef MERCHANDISEMANAGE_H
#define MERCHANDISEMANAGE_H

#include <QWidget>
#include <QMap>

class MerchandiseList;
class QMenu;
class QTreeWidgetItem;

class QSqlDatabase;
class QSqlTableModel;

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
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void showContextMenu(const QPoint &);

    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_searchPushButton_clicked();
    void on_tableView_clicked(const QModelIndex &index);

    /* QTreeWidget을 위한 슬롯 */
    void removeItem();              /* QAction을 위한 슬롯 */
    void acceptMerchandiseInfo(int);

signals:
//    void addMerchasdise(int);
    void addedMerchandise(int);
    void sendMerchandiseInfo(QString, QString, int, QString, QString, QString, QString);


private:
    int makeMid();
    QSqlTableModel *mModel;
    QMap<int, MerchandiseList*> merchandiseList;
    Ui::MerchandiseManage *ui;
    QMenu* menu;

};

#endif // MERCHANDISEMANAGE_H
