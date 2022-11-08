#ifndef CUSTOMERDB_H
#define CUSTOMERDB_H

#include <QAbstractListModel>
#include <QObject>
#include <QtSql>
#include <QList>
#include "customerlist.h"

class CustomerDB : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CustomerDB(QObject *parent = nullptr);
    ~CustomerDB() override;

    //interface

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QSqlError addConnection(const QString &driver, const QString &dbname, int &Cid);


signals:

private:
    QList<CustomerList *> customers;

};

#endif // CUSTOMERDB_H
