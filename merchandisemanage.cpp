#include "merchandisemanage.h"
#include "ui_merchandisemanage.h"
#include "merchandiselist.h"

#include <QFile>
#include <QMenu>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>




MerchandiseManage::MerchandiseManage(QWidget* parent)
    : QWidget(parent), ui(new Ui::MerchandiseManage)
{
    ui->setupUi(this);

    QList<int> size;
    size << 600 << 600;
    //    ui->splitter->setSizes(size);

    QAction* removeAction = new QAction(tr("&Remove"));
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeItem()));
    menu = new QMenu;
    menu->addAction(removeAction);

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(on_searchPushButton_clicked()));

}

void MerchandiseManage::dataSave()
{
    QSqlDatabase ohcDB = QSqlDatabase::addDatabase("QSQLITE","merchandiseConnection");
    ohcDB.setDatabaseName("merchandiseDatabase.db");

    if(ohcDB.open())
    {
        QSqlQuery mQuery(ohcDB);
        mQuery.exec("CREATE TABLE IF NOT EXISTS merchandise"
                    "(pid INTEGER PRIMARY KEY, "
                    "mname VARCHAR(20), "
                    "price VARCHAR(50), "
                    "quantity INTEGER, "
                    "madein VARCHAR(100), "
                    "category VARCHAR(100), "
                    "description VARCHAR(500), "
                    "enrollDate DATE);");

        mModel = new QSqlTableModel(this, ohcDB);
        mModel->setTable("merchandise");
        mModel->select();

        mModel->setHeaderData(0, Qt::Horizontal, tr("MID"));
        mModel->setHeaderData(1, Qt::Horizontal, tr("MNAME"));
        mModel->setHeaderData(2, Qt::Horizontal, tr("PRICE"));
        mModel->setHeaderData(3, Qt::Horizontal, tr("QUANTITY"));
        mModel->setHeaderData(4, Qt::Horizontal, tr("MADEIN"));
        mModel->setHeaderData(5, Qt::Horizontal, tr("CATEGORY"));
        mModel->setHeaderData(6, Qt::Horizontal, tr("DESCRIPTION"));
        mModel->setHeaderData(7, Qt::Horizontal, tr("ENROLLDATE"));

    }
    ui->tableView->setModel(mModel);
    ui->tableView->resizeColumnsToContents();

    for(int i = 0; i <mModel->rowCount(); i++)
    {
        int Mid = mModel->data(mModel->index(i, 0)).toInt();
        QString mname = mModel->data(mModel->index(i, 1)).toString();
        QString price = mModel->data(mModel->index(i, 2)).toString();
        int quantity = mModel->data(mModel->index(i, 3)).toInt();
        QString madein = mModel->data(mModel->index(i, 4)).toString();
        QString category = mModel->data(mModel->index(i, 5)).toString();
        QString description = mModel->data(mModel->index(i, 6)).toString();
        QString enrollDate = mModel->data(mModel->index(i, 7)).toString();

        emit addedMerchandise(Mid);
    }


}

void MerchandiseManage::on_addPushButton_clicked()
{
    int Mid = makeMid( );
    QString mname, price, madein, category, description, enrollDate;
    int quantity;

    ui->idLineEdit->setText(QString::number(Mid));
    mname = ui->nameLineEdit->text();
    price = ui->priceLineEdit->text();
    quantity = ui->quantitySpinBox->value();
    madein = ui->madeinComboBox->currentText();
    category = ui->categoryComboBox->currentText();
    description = ui->textEdit->toPlainText();
    enrollDate = ui->dateEdit->dateTime().toString();

    QSqlDatabase ohcDB = QSqlDatabase::database("merchandiseConnection");

    if(ohcDB.isOpen() && mname.length())
    {
        QSqlQuery mQuery(mModel->database());

        mQuery.exec();
        mModel->select();
        ui->tableView->resizeColumnsToContents();
        emit addedMerchandise(Mid);
    }
}

void MerchandiseManage::on_modifyPushButton_clicked()
{

    QModelIndex modelIndex = ui->tableView->currentIndex();
    if(modelIndex.isValid())
    {
        //       int Cid=cModel->data(modelIndex.siblingAtColumn(0)).toInt();
        QString mname, price, madein, category, description, enrollDate;
        int quantity;

        mname = ui->nameLineEdit->text();
        price = ui->priceLineEdit->text();
        quantity = ui->quantitySpinBox->value();
        madein = ui->madeinComboBox->currentText();
        category = ui->categoryComboBox->currentText();
        description = ui->textEdit->toPlainText();
        enrollDate = ui->dateEdit->dateTime().toString();

#if 1
        //        mModel->setData(modelIndex.siblingAtColumn(0), Mid);
        mModel->setData(modelIndex.siblingAtColumn(1), mname);
        mModel->setData(modelIndex.siblingAtColumn(2), price);
        mModel->setData(modelIndex.siblingAtColumn(3), quantity);
        mModel->setData(modelIndex.siblingAtColumn(4), madein);
        mModel->setData(modelIndex.siblingAtColumn(5), category);
        mModel->setData(modelIndex.siblingAtColumn(6), description);
        mModel->setData(modelIndex.siblingAtColumn(7), enrollDate);


#else
        QSqlQuery mQuery(mModel->database());
        mQuery.prepare("UPDATE merchandise SET mname = s, price = ?, quantity = ?, domain = ?, address = ?, favorite = ?, age = ?, gender = ?, joinDate = ? WHERE Cid = ?");

        mQuery.bindValue(0, mname);
        mQuery.bindValue(1, price);
        mQuery.bindValue(2, quantity);
        mQuery.bindValue(3, madein);
        mQuery.bindValue(4, category);
        cQuery.bindValue(5, description);
        mQuery.bindValue(6, enrollDate);
        mQuery.bindValue(7, Mid);
        cQuery.exec();
#endif
        mModel->select();
        ui->tableView->resizeColumnsToContents();

    }
}

void MerchandiseManage::on_searchPushButton_clicked()
{
    ui->searchTreeWidget->clear();
    //    for(int i = 0; i < ui->tableView->columnCount(); i++)
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    {
        QModelIndexList indexList = mModel->match(mModel->index(0, i),
                                                  Qt::EditRole, ui->searchLineEdit->text(), -1, Qt::MatchFlags(flag));

        foreach(auto k, indexList)
        {
            int Mid = mModel->data(k.siblingAtColumn(0)).toInt();
            QString mname = mModel->data(k.siblingAtColumn(1)).toString();
            QString price = mModel->data(k.siblingAtColumn(2)).toString();
            int quantity = mModel->data(k.siblingAtColumn(3)).toInt();
            QString madein = mModel->data(k.siblingAtColumn(4)).toString();
            QString category = mModel->data(k.siblingAtColumn(5)).toString();
            QString description = mModel->data(k.siblingAtColumn(6)).toString();
            QString enrollDate = mModel->data(k.siblingAtColumn(7)).toString();

            QStringList stringList;
            stringList << QString::number(Mid) << mname << price << QString::number(quantity) << madein << category << description << enrollDate;
            new QTreeWidgetItem(ui->searchTreeWidget, stringList);
            for(int i = 0; i < ui->searchTreeWidget->columnCount(); i++)
                ui->searchTreeWidget->resizeColumnToContents(i);
        }
    }
}

int MerchandiseManage::makeMid( )
{
    if(merchandiseList.size( ) == 0) {
        return 3000;
    } else {
        auto Mid = mModel->data(mModel->index(mModel->rowCount()-1, 0)).toInt();
        return ++Mid;
    }
}

void MerchandiseManage::removeItem()
{
    QModelIndex modelIndex = ui->tableView->currentIndex();
    if(modelIndex.isValid())
    {
        mModel->removeRow(modelIndex.row());
        mModel->select();
        ui->tableView->resizeColumnsToContents();
    }
}

void MerchandiseManage::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    menu->exec(globalPos);
}

void MerchandiseManage::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    if(ui->tableView->indexAt(pos).isValid())
        menu->exec(globalPos);
}

void MerchandiseManage::acceptMerchandiseInfo(int key)
{
    QModelIndexList indexList =
            mModel->match(mModel->index(0, 0), Qt::EditRole, key, -1, Qt::MatchFlags(Qt::MatchCaseSensitive));

    foreach(auto k, indexList)
    {
        //       int Mid = mModel->data(k.siblingAtColumn(0)).toInt();
        QString mname = mModel->data(k.siblingAtColumn(1)).toString();
        QString price = mModel->data(k.siblingAtColumn(2)).toString();
        int quantity = mModel->data(k.siblingAtColumn(3)).toInt();
        QString madein = mModel->data(k.siblingAtColumn(4)).toString();
        QString category = mModel->data(k.siblingAtColumn(5)).toString();
        QString description = mModel->data(k.siblingAtColumn(6)).toString();
        QString enrollDate = mModel->data(k.siblingAtColumn(7)).toString();

        emit sendMerchandiseInfo(mname, price, quantity, madein, category, description, enrollDate);

    }
}

void MerchandiseManage::on_tableView_clicked(const QModelIndex &index)
{
    QString Mid = mModel->data(index.siblingAtColumn(0)).toString();
    QString mname = mModel->data(index.siblingAtColumn(1)).toString();
    QString price = mModel->data(index.siblingAtColumn(2)).toString();
    int quantity = mModel->data(index.siblingAtColumn(3)).toInt();
    QString madein = mModel->data(index.siblingAtColumn(4)).toString();
    QString category = mModel->data(index.siblingAtColumn(5)).toString();
    QString description = mModel->data(index.siblingAtColumn(6)).toString();
    QDate enrollDate = mModel->data(index.siblingAtColumn(7)).toDate();


    ui->idLineEdit->setText(Mid);
    ui->nameLineEdit->setText(mname);
    ui->priceLineEdit->setText(price);
    ui->quantitySpinBox->setValue(quantity);
    ui->madeinComboBox->setCurrentText(madein);
    ui->categoryComboBox->setCurrentText(category);
    ui->textEdit->setText(description);
    ui->dateEdit->setDate(enrollDate);
}


MerchandiseManage::~MerchandiseManage()
{
    delete ui;

    QSqlDatabase database = QSqlDatabase::database("merchandiseConnection");

    if(database.isOpen())
    {
        mModel->submitAll();
        database.commit();
        database.close();
    }
}
