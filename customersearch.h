#ifndef CUSTOMERSEARCH_H
#define CUSTOMERSEARCH_H

#include <QWidget>
#include <customermanage.h>
class QStandardItemModel;

namespace Ui {
class CustomerSearch;
}

class CustomerSearch : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerSearch(QWidget *parent = nullptr);
    ~CustomerSearch();

private slots:
    void on_columnView_activated(const QModelIndex &index);

private:
    QStandardItemModel *ssModel;
    Ui::CustomerSearch *ui;
    CustomerManage *cModel;
};

#endif // CUSTOMERSEARCH_H
