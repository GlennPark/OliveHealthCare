#include "merchandisemanage.h"
#include "ui_merchandisemanage.h"

#include <QFile>
#include <QMenu>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

MerchandiseManage::MerchandiseManage(QWidget* parent)
    : QWidget(parent), ui(new Ui::MerchandiseManage)
{
    // ui 시작
    ui->setupUi(this);

    // 우클릭 후 삭제 시 ROW 에 포함된 내용들을 지우는 액션
    QAction* removeAction = new QAction(tr("&Remove"));
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeItem()));
    menu = new QMenu;
    menu->addAction(removeAction);

    // 마우스 오른쪽클릭 시 보조메뉴 등장
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(on_searchPushButton_clicked()));

    // 검색 시 보여주는 제품 항목들을 searchTreeView 의 헤더로 설정
    sModel = new QStandardItemModel(0, 8);
    sModel->setHeaderData(0, Qt::Horizontal, tr("MID"));                // 제품 Key
    sModel->setHeaderData(1, Qt::Horizontal, tr("MNAME"));              // 제품명
    sModel->setHeaderData(2, Qt::Horizontal, tr("PRICE"));              // 제품 가격
    sModel->setHeaderData(3, Qt::Horizontal, tr("QUANTITY"));           // 제품 수량
    sModel->setHeaderData(4, Qt::Horizontal, tr("MADEIN"));             // 제품 생산국가
    sModel->setHeaderData(5, Qt::Horizontal, tr("CATEGORY"));           // 제품 유형
    sModel->setHeaderData(6, Qt::Horizontal, tr("DESCRIPTION"));        // 제품 설명
    sModel->setHeaderData(7, Qt::Horizontal, tr("ENROLLDATE"));         // 제품 등록 일자

    // sModel에 담겨있는 제품 정보를 searchTreeView 로 전달
    ui->searchTreeView->setModel(sModel);
}

// 제품 관리 페이지의 데이터베이스
void MerchandiseManage::dataSave()
{
    // 제품 DB 셋업, QSQLITE 형식으로 제품 접속 및 DB 명을 분리해 준다.
    QSqlDatabase ohcDB = QSqlDatabase::addDatabase("QSQLITE","merchandiseConnection");
    ohcDB.setDatabaseName("merchandiseDatabase.db");

    // 예외처리
    if(ohcDB.open())
    {
        // 데이터베이스 ohcDB에 저장 및 호출되는 Sql Query 목록
        QSqlQuery mQuery(ohcDB);

        // 제품 정보 테이블을 생성, Key 값인 mid 를 Primary Key 로 설정 및 항목별 자료형 선정
        mQuery.exec("CREATE TABLE IF NOT EXISTS merchandise"
                    "(Mid INTEGER PRIMARY KEY, "
                    "mname VARCHAR(20), "
                    "price INTEGER, "
                    "quantity INTEGER, "
                    "madein VARCHAR(100), "
                    "category VARCHAR(100), "
                    "description VARCHAR(500), "
                    "enrollDate DATE);");

        // QSqlTableModel 형식으로 제품 정보 테이블을 저장
        mModel = new QSqlTableModel(this, ohcDB);
        mModel->setTable("merchandise");
        mModel->select();

        // 제품 테이블에서 사용하는 항목들의 머릿말 목록
        mModel->setHeaderData(0, Qt::Horizontal, tr("MID"));
        mModel->setHeaderData(1, Qt::Horizontal, tr("MNAME"));
        mModel->setHeaderData(2, Qt::Horizontal, tr("PRICE"));
        mModel->setHeaderData(3, Qt::Horizontal, tr("QUANTITY"));
        mModel->setHeaderData(4, Qt::Horizontal, tr("MADEIN"));
        mModel->setHeaderData(5, Qt::Horizontal, tr("CATEGORY"));
        mModel->setHeaderData(6, Qt::Horizontal, tr("DESCRIPTION"));
        mModel->setHeaderData(7, Qt::Horizontal, tr("ENROLLDATE"));

    }

    // tableView 에서 보여줄 mModel 설정 및 항목 크기 조정
    ui->tableView->setModel(mModel);
    ui->tableView->resizeColumnsToContents();

    // rowCount 순서 증가에 따라 항목별 자료형과 데이터를 인덱스에 저장
    for(int i = 0; i <mModel->rowCount(); i++)
    {
        int Mid = mModel->data(mModel->index(i, 0)).toInt();
        QString mname = mModel->data(mModel->index(i, 1)).toString();
        int price = mModel->data(mModel->index(i, 2)).toInt();
        int quantity = mModel->data(mModel->index(i, 3)).toInt();
        QString madein = mModel->data(mModel->index(i, 4)).toString();
        QString category = mModel->data(mModel->index(i, 5)).toString();
        QString description = mModel->data(mModel->index(i, 6)).toString();
        QString enrollDate = mModel->data(mModel->index(i, 7)).toString();

    }


}

// 제품 등록 버튼 클릭 시 기능
void MerchandiseManage::on_addPushButton_clicked()
{
    // 제품 키값 정보를 저장할 리스트 -> 구매 클래스에서 활용
    QList<int> MidAddList;

    // 제품별 Key 값을 생성 및 부여, 항목별로 전달될 자료형을 선정한다
    int Mid = makeMid( );
    QString mname, madein, category, description, enrollDate;
    int price, quantity;

    // idLineEdit 에 제품 정보 Key 값을 String 형식으로 반환
    ui->idLineEdit->setText(QString::number(Mid));

    // nameLineEdit ui 에 입력된 텍스트를 mname 항목으로 반환
    mname = ui->nameLineEdit->text();

    // priceLineEdit ui 에 입력된 값을 price 항목으로 반환
    price = ui->priceLineEdit->text().toInt();

    // quantitySpinBox ui 에 입력된 값을 quantity 항목으로 반환
    quantity = ui->quantitySpinBox->value();

    // madeinComboBox ui 에 입력된 텍스트를 madein 항목으로 반환
    madein = ui->madeinComboBox->currentText();

    // categoryComboBox ui 에 입력된 텍스트를 category 항목으로 반환
    category = ui->categoryComboBox->currentText();

    // textEdit ui 에 입력된 텍스트를 description 에 저장
    description = ui->textEdit->toPlainText();

    // dateEdit ui 에 입력된 값를 enrollDate 항목으로 반환
    enrollDate = ui->dateEdit->date().toString();

    // 제품 정보 데이터를 ohcDB 에 저장
    QSqlDatabase ohcDB = QSqlDatabase::database("merchandiseConnection");

    // mname 항목에 길이가 존재할 때 ohcDB 로부터 데이터를 읽어들인다
    if(ohcDB.isOpen() && mname.length())
    {
        // database 의 정보들을 mModel 로 반환하여 저장
        QSqlQuery mQuery(mModel->database());
        // 항목의 갯수와 이름을 한번에 묶어서 mQuery에 담는다
        mQuery.prepare("INSERT INTO merchandise VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
        mQuery.bindValue(0, Mid);
        mQuery.bindValue(1, mname);
        mQuery.bindValue(2, price);
        mQuery.bindValue(3, quantity);
        mQuery.bindValue(4, madein);
        mQuery.bindValue(5, category);
        mQuery.bindValue(6, description);
        mQuery.bindValue(7, enrollDate);
        mQuery.exec();

        // tableView 에서 보여줄 mModel 설정 및 항목 크기 조정
        mModel->select();
        ui->tableView->resizeColumnsToContents();

        // purchaseManager 에서 사용될 제품 키 Mid 값을 시그널 신호로 전달
        emit mInfoAddSignMtoP(Mid);
    }
}

// 제품 정보 수정 버튼 클릭 시 기능
void MerchandiseManage::on_modifyPushButton_clicked()
{
    // 현재 tableView에 입력된 정보를 QModelIndex에 반환
    QModelIndex modelIndex = ui->tableView->currentIndex();

    // modelIndex 의 값이 일치할때 조건문
    if(modelIndex.isValid())
    {
        // 수정 시 제품 정보들의 자료형 및 자료명을 설정해 준다

        QString mname, madein, category, description, enrollDate;
        int Mid, price, quantity;

        // 항목별 ui 에 입력된 정보들을 형식에 맞춰 인덱스에 등록
        Mid = ui->idLineEdit->text().toInt();
        mname = ui->nameLineEdit->text();
        price = ui->priceLineEdit->text().toInt();
        quantity = ui->quantitySpinBox->value();
        madein = ui->madeinComboBox->currentText();
        category = ui->categoryComboBox->currentText();
        description = ui->textEdit->toPlainText();
        enrollDate = ui->dateEdit->date().toString();


//         항목을 순서에 따라 한 줄씩 modelIndex 에 데이터 저장
//        mModel->setData(modelIndex.siblingAtColumn(1), mname);
//        mModel->setData(modelIndex.siblingAtColumn(2), price);
//        mModel->setData(modelIndex.siblingAtColumn(3), quantity);
//        mModel->setData(modelIndex.siblingAtColumn(4), madein);
//        mModel->setData(modelIndex.siblingAtColumn(5), category);
//        mModel->setData(modelIndex.siblingAtColumn(6), description);
//        mModel->setData(modelIndex.siblingAtColumn(7), enrollDate);
//        mModel->submit();


        QSqlQuery mQuery(mModel->database());
        mQuery.prepare("UPDATE merchandise SET mname = s, price = ?, quantity = ?, domain = ?, address = ?, favorite = ?, age = ?, gender = ?, joinDate = ? WHERE Cid = ?");

        mQuery.bindValue(0, mname);
        mQuery.bindValue(1, price);
        mQuery.bindValue(2, quantity);
        mQuery.bindValue(3, madein);
        mQuery.bindValue(4, category);
        mQuery.bindValue(5, description);
        mQuery.bindValue(6, enrollDate);
        mQuery.bindValue(7, Mid);
        mQuery.exec();

        // 컨텐츠에 맞게 컬럼 사이즈 조정
        mModel->select();
        ui->tableView->resizeColumnsToContents();

        // 수정될 제품 정보 내용을 담는 리스트
        QList<QString> mInfoList;
        mInfoList << mname << QString::number(price) << QString::number(quantity) << category;

        // 제품 정보가 수정되었을 때 구매 관리 클래스에 전달하기 위한 시그널
        emit mInfoModSignMtoP(Mid, mInfoList);
    }
}

// 제품 검색 버튼 클릭 시 기능
void MerchandiseManage::on_searchPushButton_clicked()
{
    // 검색에 사용될 모델의 내용을 비워준다
    sModel->clear();
    // searchComboBox 에서 선택된 항목 (분류별 검색)의 일치 여부 판단
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    {
        // QModelIndexList 에 인덱스 정보, flag 정보와 일치하는 searchLineEdit 내용 저장
        QModelIndexList indexList = mModel->match(mModel->index(0, i),
                                                  Qt::EditRole, ui->searchLineEdit->text(), -1, Qt::MatchFlags(flag));

        // k 순서에 따라 indexList 에 저장된 mModel data 를 항목별로 검색한다
        foreach(auto k, indexList)
        {
            int Mid = mModel->data(k.siblingAtColumn(0)).toInt();
            QString mname = mModel->data(k.siblingAtColumn(1)).toString();
            int price = mModel->data(k.siblingAtColumn(2)).toInt();
            int quantity = mModel->data(k.siblingAtColumn(3)).toInt();
            QString madein = mModel->data(k.siblingAtColumn(4)).toString();
            QString category = mModel->data(k.siblingAtColumn(5)).toString();
            QString description = mModel->data(k.siblingAtColumn(6)).toString();
            QString enrollDate = mModel->data(k.siblingAtColumn(7)).toString();

            //QStringList 에 항목을 순서대로 넣는다
            QStringList stringList;
            stringList << QString::number(Mid) << mname << QString::number(price) << QString::number(quantity) << madein << category << description << enrollDate;

            // QList 에 항목별 정보를 QStandardItem 형식으로 넣는다
            QList<QStandardItem*> standardItem;
            for(int i = 0; i < 8; ++i)
            {
                standardItem.append(new QStandardItem(stringList.at(i)));
            }

            // sModel의 열 정보(Row) 를 갱신
            sModel->appendRow(standardItem);
            ui->searchTreeView->resizeColumnToContents(i);
        }
    }
}

// 제품 등록 시 제품 정보에서 Key 값을 0부터 하나씩 증가시켜 준다
int MerchandiseManage::makeMid( )
{
    if(mModel->rowCount() == 0) {
        return 0;
    }
    // 제품 rowCount 가 0이 아닐 때 rowCount 에서 -1 해 준 후 1씩 더해서 반환
    else
    {
        auto Mid = mModel->data(mModel->index(mModel->rowCount()-1, 0)).toInt();
        return ++Mid;
    }
}

// 우클릭 후 항목별 정보 삭제
void MerchandiseManage::removeItem()
{
    // 테이블에서 해당되는 열 값을 구분해준다
    int r = ui->tableView->currentIndex().row();

    // 구매 클래스에 전달할 키값과 제품명을 선언
    int Mid = mModel->record(r).value("Mid").toInt();

    // 현재 tableView 에서 선택된 정보를 modelIndex 내에서 지운다
    QModelIndex modelIndex = ui->tableView->currentIndex();
    if(modelIndex.isValid())
    {
        mModel->removeRow(modelIndex.row());
        mModel->select();
        ui->tableView->resizeColumnsToContents();
    }

    // 제품 정보가 삭제되었음을 구매 관리 페이지에 전달하기 위한 시그널
    emit mInfoDelSignMtoP(Mid);
}

// tableView 에서 우클릭 시 해당 위치 정보를 알려준다
void MerchandiseManage::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    menu->exec(globalPos);
}

// tableView 에서 우클릭 시 해당 위치 정보에 맞는 메뉴를 출력한다
void MerchandiseManage::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    if(ui->tableView->indexAt(pos).isValid())
        menu->exec(globalPos);
}

// 구매 클래스에서 등록시 해당되는 제품 정보를 전송하는 슬롯함수
void MerchandiseManage::pInfoAddSlotMfromP(int Mid)
{
    // 제품 정보 중 구매 클래스에 전송할 항목 선택
    QList<QString> mInfoList;
    QSqlQuery mQuery(mModel->database());
    mQuery.prepare("SELECT mname, price, quantity, category"
                   "FROM merchandise WHERE Mid = ?");
    mQuery.bindValue(0, Mid);
    mQuery.exec();

    // 항목 별 인덱스를 저장한다
    QSqlRecord record = mQuery.record();
    int mnameIndex = record.indexOf("mname");
    int priceIndex = record.indexOf("price");
    int quantityIndex = record.indexOf("quantity");
    int categoryIndex = record.indexOf("category");

    mQuery.next();

    // 구매 클래스로 전송할 제품 정보를 담는다
    QString mname = mQuery.value(mnameIndex).toString();
    QString price = mQuery.value(priceIndex).toString();
    QString quantity = mQuery.value(quantityIndex).toString();
    QString category = mQuery.value(categoryIndex).toString();

    mInfoList << mname << price << quantity << category;

    // 구매 정보 등록 시 해당되는 제품 정보를 리스트로 반환하는 시그널
    emit pInfoAddReturnSignMtoP(mInfoList);
}

// 구매 클래스에서 검색시 해당되는 제품 정보를 전송하는 슬롯함수
void MerchandiseManage::mInfoSearchSlotCfromP(int Mid)
{
    // 제품 정보 중 구매 클래스에 전송할 항목 선택
    QList<QString> mInfoList;
    QSqlQuery mQuery(mModel->database());
    mQuery.prepare("SELECT mname, price, quantity, category"
                   "FROM merchandise WHERE Mid = ?");
    mQuery.bindValue(0, Mid);
    mQuery.exec();

    // 항목 별 인덱스를 저장한다
    QSqlRecord record = mQuery.record();
    int mnameIndex = record.indexOf("mname");
    int priceIndex = record.indexOf("price");
    int quantityIndex = record.indexOf("quantity");
    int categoryIndex = record.indexOf("category");

    mQuery.next();

    // 구매 클래스로 전송할 제품 정보를 담는다
    QString mname = mQuery.value(mnameIndex).toString();
    QString price = mQuery.value(priceIndex).toString();
    QString quantity = mQuery.value(quantityIndex).toString();
    QString category = mQuery.value(categoryIndex).toString();

    mInfoList << mname << price << quantity << category;

    // 구매 정보 검색 시 해당되는 제품 정보를 리스트로 반환하는 시그널
    emit pInfoSearchReturnSignMtoP(mInfoList);
}

// 구매 클래스에서 수정 시 해당되는 제품 정보를 전송하는 슬롯함수
void MerchandiseManage::mInfoModSlotMtoP(int Mid, int r)
{
    // 제품 정보 중 구매 클래스에 전송할 항목 선택
    QList<QString> mInfoList;
    QSqlQuery mQuery(mModel->database());
    mQuery.prepare("SELECT mname, price, quantity, category"
                   "FROM merchandise WHERE Mid = ?");
    mQuery.bindValue(0, Mid);
    mQuery.exec();

    // 항목 별 인덱스를 저장한다
    QSqlRecord record = mQuery.record();
    int mnameIndex = record.indexOf("mname");
    int priceIndex = record.indexOf("price");
    int quantityIndex = record.indexOf("quantity");
    int categoryIndex = record.indexOf("category");

    mQuery.next();

    // 구매 클래스로 전송할 제품 정보를 담는다
    QString mname = mQuery.value(mnameIndex).toString();
    QString price = mQuery.value(priceIndex).toString();
    QString quantity = mQuery.value(quantityIndex).toString();
    QString category = mQuery.value(categoryIndex).toString();

    mInfoList << mname << price << quantity << category;

    // 구매 정보 수정 시 해당되는 제품 정보를 리스트로 반환하는 시그널
    emit pInfoModReturnSignMtoP(mInfoList, r);
}

// 현재 제품 정보를 ui 입력단에 표시해 주는 슬롯함수
void MerchandiseManage::on_tableView_clicked(const QModelIndex &index)
{
    // 항목별 데이터를 순서 및 자료형에 따라 인덱스로 보낸다
    QString Mid = mModel->data(index.siblingAtColumn(0)).toString();
    QString mname = mModel->data(index.siblingAtColumn(1)).toString();
    QString price = mModel->data(index.siblingAtColumn(2)).toString();
    int quantity = mModel->data(index.siblingAtColumn(3)).toInt();
    QString madein = mModel->data(index.siblingAtColumn(4)).toString();
    QString category = mModel->data(index.siblingAtColumn(5)).toString();
    QString description = mModel->data(index.siblingAtColumn(6)).toString();
    QDate enrollDate = mModel->data(index.siblingAtColumn(7)).toDate();

    // 각 ui 에 입력된 정보를 항목별로 저장한다
    ui->idLineEdit->setText(Mid);
    ui->nameLineEdit->setText(mname);
    ui->priceLineEdit->setText(price);
    ui->quantitySpinBox->setValue(quantity);
    ui->madeinComboBox->setCurrentText(madein);
    ui->categoryComboBox->setCurrentText(category);
    ui->textEdit->setText(description);
    ui->dateEdit->setDate(enrollDate);
}

// 소멸자, 제품 관리 ui 종료
MerchandiseManage::~MerchandiseManage()
{
    delete ui;

    // merchandiseConnection 데이터베이스의 내용을 모두 저장한다
    QSqlDatabase database = QSqlDatabase::database("merchandiseConnection");

    if(database.isOpen())
    {
        mModel->submitAll();
        database.commit();
        database.close();
    }
}
