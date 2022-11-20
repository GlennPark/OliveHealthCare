#include "purchasemanage.h"
#include "ui_purchasemanage.h"

#include <QFile>
#include <QMenu>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QStandardItemModel>

PurchaseManage::PurchaseManage(QWidget *parent)
    : QWidget(parent), ui(new Ui::PurchaseManage)
{
    // ui 시작
    ui->setupUi(this);

    // 우클릭 후 삭제 시 ROW 에 포함된 내용들을 지우는 액션
    QAction* removeAction = new QAction(tr("&Remove"));
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));
    menu = new QMenu;
    menu->addAction(removeAction);

    // totalTableView 에서 마우스 오른쪽클릭 시 보조메뉴 등장
    ui->totalTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->totalTableView, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(on_searchPushButton_clicked()));

    //QSQLITE 형식의 구매 정보 접속을 설정한다
    QSqlDatabase ohcDB = QSqlDatabase::addDatabase("QSQLITE","purchaseConnection");
    ohcDB.setDatabaseName("purchaseDatabase.db");

    if(ohcDB.open())
    {
        QSqlQuery pQuery(pModel->database());
        pQuery.exec("CREATE TABLE IF NOT EXISTS purchase"
                    "(Pid INTEGER PRIMARY KEY, "
                    "Cid INTEGER PRIMARY KEY, "
                    "Mid INTEGER PRIMARY KEY, "
                    "buyAmount INTEGER, "
                    "shopDate Date);");

        pModel = new QSqlTableModel(this, ohcDB);
        pModel->setTable("purchase");

        tModel = new QStandardItemModel(0, 12);
        sModel = new QStandardItemModel(0, 12);

        pModel->select();

        //pModel (purchaseModel) 의 헤더 데이터
        pModel->setHeaderData(0, Qt::Horizontal, tr("PID"));
        pModel->setHeaderData(1, Qt::Horizontal, tr("CID"));
        pModel->setHeaderData(2, Qt::Horizontal, tr("MID"));
        pModel->setHeaderData(3, Qt::Horizontal, tr("BUYAMOUNT"));
        pModel->setHeaderData(4, Qt::Horizontal, tr("SHOPDATE"));

        //tModel (totalModel) 의 헤더 데이터
        tModel->setHeaderData(0, Qt::Horizontal, tr("PID"));
        tModel->setHeaderData(1, Qt::Horizontal, tr("CID"));
        tModel->setHeaderData(2, Qt::Horizontal, tr("MID"));
        tModel->setHeaderData(3, Qt::Horizontal, tr("NAME"));
        tModel->setHeaderData(4, Qt::Horizontal, tr("MNAME"));
        tModel->setHeaderData(5, Qt::Horizontal, tr("ADDRESS"));
        tModel->setHeaderData(6, Qt::Horizontal, tr("FAVORITE"));
        tModel->setHeaderData(7, Qt::Horizontal, tr("CATEGORY"));
        tModel->setHeaderData(8, Qt::Horizontal, tr("PRICE"));
        tModel->setHeaderData(9, Qt::Horizontal, tr("QUANTITY"));
        tModel->setHeaderData(10, Qt::Horizontal, tr("BUYAMOUNT"));
        tModel->setHeaderData(11, Qt::Horizontal, tr("TOTALPRICE"));
        tModel->setHeaderData(12, Qt::Horizontal, tr("SHOPDATE"));
        tModel->setHeaderData(13, Qt::Horizontal, tr("PHONENUMBER"));

        //sModel (searchModel) 의 헤더 데이터
        sModel->setHeaderData(0, Qt::Horizontal, tr("PID"));
        sModel->setHeaderData(1, Qt::Horizontal, tr("CID"));
        sModel->setHeaderData(2, Qt::Horizontal, tr("MID"));
        sModel->setHeaderData(3, Qt::Horizontal, tr("NAME"));
        sModel->setHeaderData(4, Qt::Horizontal, tr("MNAME"));
        sModel->setHeaderData(5, Qt::Horizontal, tr("ADDRESS"));
        sModel->setHeaderData(6, Qt::Horizontal, tr("FAVORITE"));
        sModel->setHeaderData(7, Qt::Horizontal, tr("CATEGORY"));
        sModel->setHeaderData(8, Qt::Horizontal, tr("PRICE"));
        sModel->setHeaderData(9, Qt::Horizontal, tr("QUANTITY"));
        sModel->setHeaderData(10, Qt::Horizontal, tr("BUYAMOUNT"));
        sModel->setHeaderData(11, Qt::Horizontal, tr("TOTALPRICE"));
        sModel->setHeaderData(12, Qt::Horizontal, tr("SHOPDATE"));
        sModel->setHeaderData(13, Qt::Horizontal, tr("PHONENUMBER"));
    }
    ui->tableView->setModel(pModel);
    ui->tableView->resizeColumnsToContents();

    ui->totalTableView->setModel(tModel);
    ui->totalTableView->resizeColumnsToContents();

    ui->searchTableView->setModel(sModel);
    ui->searchTableView->resizeColumnsToContents();

}

// 구매 데이터 베이스에 회원 및 제품 정보를 불러온다
void PurchaseManage::dataSave()
{
    // QSQLITE 형식의 구매 정보 접속을 설정한다
    //   QSqlDatabase ohcDB = QSqlDatabase::addDatabase("QSQLITE","purchaseConnection");
    // ohcDB.setDatabaseName("purchaseDatabase.db");

    //구매 테이블 정보를 tModel에서 표시한다
    for(int i = 0; i < pModel->rowCount(); i++)
    {
        // 구매 테이블 정보를 항목별로 저장한다
        int Pid = pModel->record(i).value("Pid").toInt();
        int Cid = pModel->record(i).value("Cid").toInt();
        int Mid = pModel->record(i).value("Mid").toInt();
        int buyAmount = pModel->record(i).value("buyAmount").toInt();
        QString shopDate = pModel->record(i).value("shopDate").toString();

        // pList 에 구매 테이블 정보를 담는다 -> totalTableView 에 활용
        QStringList pList;
        pList << QString::number(Pid) << QString::number(Cid) << QString::number(Mid) << QString::number(buyAmount) << shopDate;

        // totalTableView 의 형식이 QStandardItemModel 이므로 형변환 해준다
        QList<QStandardItem*> pItem;
        for(int i = 0; i < pList.size(); i++)
        {
            pItem << new QStandardItem(pList[i]);
        }

        // pItem 에 담긴 구매 테이블 정보를 tModel 에 저장한다
        tModel->appendRow(pItem[0]);
        tModel->setItem(tR, 1, pItem[1]);
        tModel->setItem(tR, 2, pItem[2]);
        tModel->setItem(tR, 10, pItem[3]);
        tModel->setItem(tR, 12, pItem[4]);

        // 회원 및 제품 정보를 불러올 때 필요한 시그널
        emit pInfoAddSignPtoC(Cid);
        emit pInfoAddSignPtoM(Mid);

    }
}

// 구매 등록 버튼 클릭 시
void PurchaseManage::on_addPushButton_clicked()
{
    // 항목 별 자료형 설정
    int Pid = makePid( );
    int Cid = 0, Mid = 0, buyAmount;
    QString shopDate;

    // 항목별 ui 에 입력된 내용을 자료형에 따라 저장한다
    ui->PidLineEdit->setText(QString::number(Pid));
    ui->CidLineEdit->text().toInt();
    ui->MidLineEdit->text().toInt();
    buyAmount = ui->buyAmountSpinBox->value();
    shopDate = ui->shopDateEdit->date().toString();

    QSqlQuery pQuery(pModel->database());
    pQuery.prepare("INSERT INTO purchase VALUES (?, ?, ?, ?, ?)");
    pQuery.bindValue(0, Pid);
    pQuery.bindValue(1, Cid);
    pQuery.bindValue(2, Mid);
    pQuery.bindValue(3, buyAmount);
    pQuery.bindValue(4, shopDate);
    pQuery.exec();
    pModel->select();

    // pList 에 구매 테이블 정보를 담는다 -> totalTableView 에 활용
    QStringList pList;
    pList << QString::number(Pid) << QString::number(Cid) << QString::number(Mid) << QString::number(buyAmount) << shopDate;

    // totalTableView 의 형식이 QStandardItemModel 이므로 형변환 해준다
    QList<QStandardItem*> pItem;
    for(int i = 0; i < pList.size(); i++)
    {
        pItem << new QStandardItem(pList[i]);
    }

    // pItem 에 담긴 구매 테이블 정보를 tModel 에 저장한다
    tModel->appendRow(pItem[0]);
    tModel->setItem(tR, 1, pItem[1]);
    tModel->setItem(tR, 2, pItem[2]);
    tModel->setItem(tR, 10, pItem[3]);
    tModel->setItem(tR, 12, pItem[4]);

    ui->tableView->resizeColumnsToContents();

    ui->buyAmountSpinBox->clear();
    ui->shopDateEdit->clear();

    // 구매 정보가 추가되었음을 회원 및 제품 클래스에 전달하는 시그널
    emit pInfoAddSignPtoC(Pid);
    emit pInfoAddSignPtoM(Mid);
}


void PurchaseManage::on_modifyPushButton_clicked()
{
    QModelIndex modelIndex = ui->tableView->currentIndex();
    if(modelIndex.isValid())
    {
        // int Pid = makePid( );

        int Cid, Mid, price, quantity, buyAmount, totalPrice;
        QString name, mname, favorite, category, shopDate;

        name = ui->nameComboBox->currentText();
        mname = ui->mnameComboBox->currentText();
        favorite = ui->favoriteComboBox->currentText();
        category = ui->categoryComboBox->currentText();
        price = ui->priceLineEdit->text().toInt();
        quantity = ui->quantitySpinBox->value();
        buyAmount = ui->buyAmountSpinBox->value();
        totalPrice = ui->totalPriceLineEdit->text().toInt();
        shopDate = ui->shopDateEdit->date().toString();


#if 1
        //        pModel->setData(modelIndex.siblingAtColumn(0), Pid);
        //        pModel->setData(modelIndex.siblingAtColumn(1), Cid);
        //        pModel->setData(modelIndex.siblingAtColumn(2), Mid);

        pModel->setData(modelIndex.siblingAtColumn(3), name);
        pModel->setData(modelIndex.siblingAtColumn(4), mname);
        pModel->setData(modelIndex.siblingAtColumn(5), favorite);
        pModel->setData(modelIndex.siblingAtColumn(6), category);
        pModel->setData(modelIndex.siblingAtColumn(7), price);
        pModel->setData(modelIndex.siblingAtColumn(8), quantity);
        pModel->setData(modelIndex.siblingAtColumn(9), buyAmount);
        pModel->setData(modelIndex.siblingAtColumn(10), totalPrice);
        pModel->setData(modelIndex.siblingAtColumn(11), shopDate);
        pModel->submit();

#else
        QSqlQuery pQuery(pModel->database());
        pQuery.prepare("UPDATE purchase SET name = ?, mname = ?, favorite = ?, category = ?, price = ?, quantity = ?, buyAmount = ?, totalPrice = ?, shopDate = ?");

        pQuery.bindValue(0, name);
        pQuery.bindValue(1, mname);
        pQuery.bindValue(2, favorite);
        pQuery.bindValue(3, category);
        pQuery.bindValue(4, price);
        pQuery.bindValue(5, quantity);
        pQuery.bindValue(6, buyAmount);
        pQuery.bindValue(7, totalPrice);
        pQuery.bindValue(8, shopDate);
        pQuery.bindValue(9, Pid);
        pQuery.bindValue(10, Cid);
        pQuery.bindValue(11, Mid);
        pQuery.exec();
#endif
        pModel->select();
        ui->tableView->resizeColumnsToContents();

    }
}


void PurchaseManage::on_searchPushButton_clicked()
{
    sModel->clear();
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    QModelIndexList indexes = pModel->match(pModel->index(0, i), Qt::EditRole, ui->searchLineEdit->text(), -1, Qt::MatchFlags(flag));

    foreach(auto k, indexes)
    {
        int Pid = pModel->data(k.siblingAtColumn(0)).toInt();
        int Cid = pModel->data(k.siblingAtColumn(1)).toInt();
        int Mid = pModel->data(k.siblingAtColumn(2)).toInt();
        QString name = pModel->data(k.siblingAtColumn(3)).toString();
        QString mname = pModel->data(k.siblingAtColumn(4)).toString();
        QString favorite = pModel->data(k.siblingAtColumn(5)).toString();
        QString category = pModel->data(k.siblingAtColumn(6)).toString();
        int price = pModel->data(k.siblingAtColumn(7)).toInt();
        int quantity = pModel->data(k.siblingAtColumn(8)).toInt();
        int buyAmount = pModel->data(k.siblingAtColumn(9)).toInt();
        int totalPrice = pModel->data(k.siblingAtColumn(10)).toInt();
        QString shopDate = pModel->data(k.siblingAtColumn(11)).toString();

        QStringList stringList;

        stringList << QString::number(Pid) << QString::number(Cid) << QString::number(Mid) << name << mname << favorite << category << QString::number(price) << QString::number(quantity) << QString::number(buyAmount) << QString::number(totalPrice) << shopDate ;

        QList<QStandardItem *> items;
        for (int i = 0; i < 12; ++i) {
            items.append(new QStandardItem(stringList.at(i)));
        }

        sModel->appendRow(items);
        ui->searchTableView->resizeColumnsToContents();
    }
}

int PurchaseManage::makePid( )
{
    if(pModel->rowCount() == 0) {
        return 0;
    } else {
        auto Pid = pModel->data(pModel->index(pModel->rowCount()-1, 0)).toInt();
        return ++Pid;
    }
}

void PurchaseManage::removeItem()
{
    QModelIndex modelIndex = ui->tableView->currentIndex();
    if(modelIndex.isValid())
    {
        pModel->removeRow(modelIndex.row());
        pModel->select();
        ui->tableView->resizeColumnsToContents();
    }
}

void PurchaseManage::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    menu->exec(globalPos);
}

void PurchaseManage::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    if(ui->tableView->indexAt(pos).isValid())
        menu->exec(globalPos);
}

// 회원 정보를 받아와 totalTableView에 적용하는 슬롯함수
void PurchaseManage::pInfoAddReturnSlotPfromC(QList<QString> cInfoList)
{

    // 항목별 리스트 저장
    QString name = cInfoList[0];
    QString phoneNumber = cInfoList[1];
    QString address = cInfoList[2];
    QString favorite = cInfoList[3];

    QStringList cList;
    cList << name << phoneNumber << address << favorite;

    // 리스트 정보를 테이블에 맞는 자료형으로 변환
    QList<QStandardItem*> cItem;
    for(int i = 0; i < cItem.size(); i++)
    {
        cItem << new QStandardItem(cList[i]);
    }

    // tModel에 열에 따라 정보 저장
    tModel->setItem(tR, 3, cItem[0]);
    tModel->setItem(tR, 13, cItem[1]);
    tModel->setItem(tR, 5, cItem[2]);
    tModel->setItem(tR, 6, cItem[3]);
}

// 제품 정보를 받아와 totalTableView에 적용하는 슬롯함수
void PurchaseManage::pInfoAddReturnSlotPfromM(QList<QString> mInfoList)
{
    // 항목별 리스트 저장
    QString mname = mInfoList[0];
    QString price = mInfoList[1];
    QString quantity = mInfoList[2];
    QString category = mInfoList[3];

    QStringList mList;
    mList << mname << price << quantity << category;

    // 리스트 정보를 테이블에 맞는 자료형으로 변환
    QList<QStandardItem*> mItem;
    for(int i = 0; i < mItem.size(); i++)
    {
        mItem << new QStandardItem(mList[i]);
    }

    //
    tModel->setItem(tR, 4, mItem[0]);
    tModel->setItem(tR, 8, mItem[1]);
    tModel->setItem(tR, 9, mItem[2]);
    tModel->setItem(tR, 7, mItem[3]);
}



void PurchaseManage::on_tableView_clicked(const QModelIndex &index)
{
    QString Pid = pModel->data(index.siblingAtColumn(0)).toString();
    QString Cid = pModel->data(index.siblingAtColumn(1)).toString();
    QString Mid = pModel->data(index.siblingAtColumn(2)).toString();
    QString name = pModel->data(index.siblingAtColumn(3)).toString();
    QString mname = pModel->data(index.siblingAtColumn(4)).toString();
    QString favorite = pModel->data(index.siblingAtColumn(5)).toString();
    QString category = pModel->data(index.siblingAtColumn(6)).toString();
    QString price = pModel->data(index.siblingAtColumn(7)).toString();
    int quantity = pModel->data(index.siblingAtColumn(8)).toInt();
    int buyAmount = pModel->data(index.siblingAtColumn(9)).toInt();
    QString totalPrice = pModel->data(index.siblingAtColumn(10)).toString();
    QDate shopDate = pModel->data(index.siblingAtColumn(11)).toDate();

    ui->PidLineEdit->setText(Pid);
    ui->CidLineEdit->setText(Cid);
    ui->MidLineEdit->setText(Mid);
    ui->nameComboBox->setCurrentText(name);
    ui->mnameComboBox->setCurrentText(mname);
    ui->favoriteComboBox->setCurrentText(favorite);
    ui->categoryComboBox->setCurrentText(category);
    ui->priceLineEdit->setText(price);
    ui->quantitySpinBox->setValue(quantity);
    ui->buyAmountSpinBox->setValue(buyAmount);
    ui->totalPriceLineEdit->setText(totalPrice);
    ui->shopDateEdit->setDate(shopDate);


}
void PurchaseManage::on_nameComboBox_currentIndexChanged(int index)
{
    qDebug()<<"1104";
    emit getCustomerInfo(index);
}

void PurchaseManage::on_mnameComboBox_currentIndexChanged(int index)
{
    //emit getMerchandiseInfo(MidList[index]);
}

//void PurchaseManage::getCustomerInfo(QString name, QString phoneNumber, QString address)
//{
//    ui->customerTreeWidget->clear();
//    QTreeWidgetItem *item = new QTreeWidgetItem(ui->customerTreeWidget);
//    item->setText(0, name);
//    item->setText(1, phoneNumber);
//    item->setText(2, address);

//}




void PurchaseManage::on_quantitySpinBox_valueChanged(int arg1)
{
    ui->totalPriceLineEdit->setText(QString::number(arg1 * ui->priceLineEdit->text().toInt()));
}


void PurchaseManage::cInfoSlotPfromC(int Cid)
{
    //CidList << Cid;
    qDebug()<<"d";
    ui->nameComboBox->addItem(QString::number(Cid));

}

void PurchaseManage::addCustomer(int id, QString){

    ui->nameComboBox->addItem(QString::number(id));

}

void PurchaseManage::addMerchandise(int Mid)
{
    // MidList << Mid;

    ui->mnameComboBox->addItem(QString::number(Mid));
}
PurchaseManage::~PurchaseManage()
{
    delete ui;
    QSqlDatabase database = QSqlDatabase::database("purchaseConnection");

    if(database.isOpen())
    {
        pModel->submitAll();
        database.commit();
        database.close();
    }
}
void PurchaseManage::acceptCustomerInfo(QString name, QString phoneNumber, QString, QString, QString, QString, QString, QString){
    qDebug()<<"accept";
    QStandardItem* item_name = new QStandardItem;
    item_name->setText(name);

    QStandardItem* item_phone = new QStandardItem;
    item_phone->setText(phoneNumber);

    QList<QStandardItem*> ls;
    ls<<item_name<<item_phone;
    clientInfoModel->setItem(1,1,item_name);
    ui->tableView->update();

}
