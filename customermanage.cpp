#include "customermanage.h"
#include "QtCore/qdatetime.h"
#include "ui_customermanage.h"

#include <QFile>
#include <QMenu>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


CustomerManage::CustomerManage(QWidget* parent)
    : QWidget(parent), ui(new Ui::CustomerManage)
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

    // 나이 입력 시 제한
    ui->ageSpinBox->setRange(19,100);

    // 검색 시 보여주는 회원 항목들을 searchTreeView 의 헤더로 설정
    sModel = new QStandardItemModel(0, 10);
    sModel->setHeaderData(0, Qt::Horizontal, tr("CID"));                // 회원 Key
    sModel->setHeaderData(1, Qt::Horizontal, tr("NAME"));               // 회원 이름
    sModel->setHeaderData(2, Qt::Horizontal, tr("PHONE NUMBER"));       // 회원 전화 번호
    sModel->setHeaderData(3, Qt::Horizontal, tr("EMAIL"));              // 회원 이메일 주소
    sModel->setHeaderData(4, Qt::Horizontal, tr("DOMAIN"));             // 회원 도메인 주소
    sModel->setHeaderData(5, Qt::Horizontal, tr("ADDRESS"));            // 회원 거주지 주소
    sModel->setHeaderData(6, Qt::Horizontal, tr("FAVORITE"));           // 회원 선호 항목
    sModel->setHeaderData(7, Qt::Horizontal, tr("AGE"));                // 회원 연령
    sModel->setHeaderData(8, Qt::Horizontal, tr("GENDER"));             // 회원 성별
    sModel->setHeaderData(9, Qt::Horizontal, tr("JOINDATE"));           // 회원 가입 날짜

    // sModel에 담겨있는 회원 정보를 searchTreeView 로 전달
    ui->searchTreeView->setModel(sModel);

}

// 회원 관리 페이지의 데이터베이스
void CustomerManage::dataSave()
{
    // 회원 DB 셋업, QSQLITE 형식으로 회원 접속 및 DB 명을 분리해 준다.
    QSqlDatabase ohcDB = QSqlDatabase::addDatabase("QSQLITE","customerConnection");
    ohcDB.setDatabaseName("customerDatabase.db");

    // 예외처리
    if(ohcDB.open())
    {
        // 데이터베이스 ohcDB에 저장 및 호출되는 Sql Query 목록
        QSqlQuery cQuery(ohcDB);

        // 회원 정보 테이블을 생성, Key 값인 cid 를 Primary Key 로 설정 및 항목별 자료형 선정
        cQuery.exec("CREATE TABLE IF NOT EXISTS customer"
                    "(Cid INTEGER PRIMARY KEY, "
                    "name VARCHAR(20), "
                    "phoneNumber VARCHAR(100), "
                    "email VARCHAR(100), "
                    "domain VARCHAR(100), "
                    "address VARCHAR(200), "
                    "favorite VARCHAR(100), "
                    "age INTEGER, "
                    "gender VARCHAR(50), "
                    "joinDate DATE);");

        // QSqlTableModel 형식으로 회원 정보 테이블을 저장
        cModel = new QSqlTableModel(this, ohcDB);
        cModel->setTable("customer");
        cModel->select();

        // 회원 테이블에서 사용하는 항목들의 머릿말 목록
        cModel->setHeaderData(0, Qt::Horizontal, tr("CID"));
        cModel->setHeaderData(1, Qt::Horizontal, tr("NAME"));
        cModel->setHeaderData(2, Qt::Horizontal, tr("PHONENUMBER"));
        cModel->setHeaderData(3, Qt::Horizontal, tr("EMAIL"));
        cModel->setHeaderData(4, Qt::Horizontal, tr("DOMAIN"));
        cModel->setHeaderData(5, Qt::Horizontal, tr("ADDRESS"));
        cModel->setHeaderData(6, Qt::Horizontal, tr("FAVORITE"));
        cModel->setHeaderData(7, Qt::Horizontal, tr("AGE"));
        cModel->setHeaderData(8, Qt::Horizontal, tr("GENDER"));
        cModel->setHeaderData(9, Qt::Horizontal, tr("JOINDATE"));
    }

    // tableView 에서 보여줄 cModel 설정 및 항목 크기 조정
    ui->tableView->setModel(cModel);
    ui->tableView->resizeColumnsToContents();

    // 채팅 서버에 고객 이름과 ID를 전송할 때 쓰이는 리스트
    QList<int> CidList;
    QList<QString> cNameList;

    // 채팅 서버에 고객 이름과 ID를 전송할 때 쓰이는 변수

    //    int Cid;
    //    QString name;

    // rowCount 순서 증가에 따라 항목별 자료형과 데이터를 인덱스에 저장
    for(int i = 0; i < cModel->rowCount(); i++)
    {
        int Cid = cModel->data(cModel->index(i, 0)).toInt();
        QString name = cModel->data(cModel->index(i, 1)).toString();
        QString phoneNumber = cModel->data(cModel->index(i, 2)).toString();
        QString email = cModel->data(cModel->index(i, 3)).toString();
        QString domain = cModel->data(cModel->index(i, 4)).toString();
        QString address = cModel->data(cModel->index(i, 5)).toString();
        QString favorite = cModel->data(cModel->index(i, 6)).toString();
        int age = cModel->data(cModel->index(i, 7)).toInt();
        QString gender = cModel->data(cModel->index(i, 8)).toString();
        QString joinDate = cModel->data(cModel->index(i, 9)).toString();

        // 회원 키값과 이름을 record 로 담기
        Cid = cModel->record(i).value("Cid").toInt();
        name = cModel->record(i).value("name").toString();

        CidList << Cid;
        cNameList << name;
    }

    // 채팅 서버에 회원 키값과 회원 이름 리스트를 전달하기 위한 시그널
    emit cInfoSignCtoChat(CidList, cNameList);
}

// 회원 등록 버튼 클릭 시 기능
void CustomerManage::on_addPushButton_clicked()
{
    // 회원 키값 정보를 저장할 리스트
    QList<int> CidAddList;
    QList<QString> cNameAddList;

    // 회원별 Key 값을 생성 및 부여, 항목별로 전달될 자료형을 선정한다
    int Cid = makeCid();
    QString name, phoneNumber, email, domain, address, favorite, gender, joinDate;
    int age;

    // idLineEdit 에 회원 정보 Key 값을 String 형식으로 반환
    ui->idLineEdit->setText(QString::number(Cid));

    // nameLineEdit ui 에 입력된 텍스트를 name 항목으로 반환
    name = ui->nameLineEdit->text();

    // phoneNumberLineEdit ui 에 입력된 텍스트를 phoneNumber 항목으로 반환
    phoneNumber = ui->phoneNumberLineEdit->text();

    // emailLineEdit ui 에 입력된 텍스트를 email 항목으로 반환
    email = ui->emailLineEdit->text();

    // domainComboBox ui 에 입력된 텍스트를 domain 항목으로 반환
    domain = ui->domainComboBox->currentText();

    // favoriteComboBox ui 에 입력된 텍스트를 favorite 항목으로 반환
    favorite = ui->favoriteComboBox->currentText();

    // addressLineEdit ui 에 입력된 텍스트를 address 항목으로 반환
    address = ui->addressLineEdit->text();

    // ageSpinBox ui 에 입력된 값를 age 항목으로 반환
    age = ui->ageSpinBox->value();

    // male, female Button ui 에서 선택한 정보를 gender 항목으로 반환
    if(ui->maleButton->isChecked())
    {
        gender = ui->maleButton->text();
    }
    else
    {
        gender = ui->femaleButton->text();
    }

    // dateEdit ui 에 입력된 값를 joinDate 항목으로 반환
    joinDate = ui->dateEdit->date().toString();

    // 회원 정보 데이터를 ohcDB 에 저장
    QSqlDatabase ohcDB = QSqlDatabase::database("customerConnection");

    // name 항목에 길이가 존재할때 ohcDB 로부터 데이터를 읽어들인다
    if(ohcDB.isOpen() && name.length())
    {
        // database 의 정보들을 cModel 로 반환하여 저장
        QSqlQuery cQuery(cModel->database());
        // 항목의 갯수와 이름을 한번에 묶어서 cQuery에 담는다
        cQuery.prepare("INSERT INTO customer VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        cQuery.bindValue(0, Cid);
        cQuery.bindValue(1, name);
        cQuery.bindValue(2, phoneNumber);
        cQuery.bindValue(3, email);
        cQuery.bindValue(4, domain);
        cQuery.bindValue(5, address);
        cQuery.bindValue(6, favorite);
        cQuery.bindValue(7, age);
        cQuery.bindValue(8, gender);
        cQuery.bindValue(9, joinDate);
        cQuery.exec();

        // tableView 에서 보여줄 cModel 설정 및 항목 크기 조정
        cModel->select();
        ui->tableView->resizeColumnsToContents();

        // 회원 정보가 등록되었을 때 구매 관리 클래스에 전달하기 위한 시그널
        emit cInfoAddSignCtoP(Cid);
        CidAddList << Cid;

        // 회원 정보가 등록되었을 때 채팅 서버에 전달하기 위한 시그널
        emit cInfoAddSignCtoChat(CidAddList, cNameAddList);
        cNameAddList << name;
    }

}

// 회원 정보 수정 버튼 클릭 시 기능
void CustomerManage::on_modifyPushButton_clicked()
{
    // 현재 tableView에 입력된 정보를 QModelIndex에 반환
    QModelIndex modelIndex = ui->tableView->currentIndex();

    // modelIndex 의 값이 일치할때 조건문
    if(modelIndex.isValid())
    {
        // 수정 시 회원 정보들의 자료형 및 자료명을 설정해 준다
        QString name, phoneNumber, email, domain, address, favorite, gender, joinDate;
        int Cid, age;

        // 항목별 ui 에 입력된 정보들을 형식에 맞춰 인덱스에 등록
        Cid = ui->idLineEdit->text().toInt();
        name = ui->nameLineEdit->text();
        phoneNumber = ui->phoneNumberLineEdit->text();
        email = ui->emailLineEdit->text();
        domain = ui->domainComboBox->currentText();
        favorite = ui->favoriteComboBox->currentText();
        address = ui->addressLineEdit->text();
        age = ui->ageSpinBox->value();

        if(ui->maleButton->isChecked())
        {
            gender = ui->maleButton->text();
        }
        else
        {
            gender = ui->femaleButton->text();
        }

        joinDate = ui->dateEdit->date().toString();

        // customer 테이블 정보 수정을 위한 QSql querry
        QSqlQuery cQuery(cModel->database());
        cQuery.prepare("UPDATE customer SET name = ?, phoneNumber = ?, email = ?, domain = ?, address = ?, favorite = ?, age = ?, gender = ?, joinDate = ? WHERE Cid = ?");

        cQuery.bindValue(0, name);
        cQuery.bindValue(1, phoneNumber);
        cQuery.bindValue(2, email);
        cQuery.bindValue(3, domain);
        cQuery.bindValue(4, address);
        cQuery.bindValue(5, favorite);
        cQuery.bindValue(6, age);
        cQuery.bindValue(7, gender);
        cQuery.bindValue(8, joinDate);
        cQuery.bindValue(9, Cid);
        cQuery.exec();

        // 컨텐츠에 맞게 컬럼 사이즈 조정
        cModel->select();
        ui->tableView->resizeColumnsToContents();

        // 수정될 회원 정보 내용을 담는 리스트
        QList<QString> cInfoList;
        cInfoList << name << phoneNumber << address << favorite;

        // 회원 정보가 수정되었을 때 구매 관리 클래스에 전달하기 위한 시그널
        emit cInfoModSignCtoP(Cid, cInfoList);

        // 회원 정보가 수정되었을 때 채팅 서버에 전달하기 위한 시그널
        emit cInfoModSignCtoChat(Cid, name);
    }

}

// 회원 검색 버튼 클릭 시 기능
void CustomerManage::on_searchPushButton_clicked()
{
    // 검색에 사용될 모델의 내용을 비워준다
    sModel->clear();
    // searchComboBox 에서 선택된 항목 (분류별 검색)의 일치 여부 판단
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    {
        // QModelIndexList 에 인덱스 정보, flag 정보와 일치하는 searchLineEdit 내용 저장
        QModelIndexList indexList = cModel->match(cModel->index(0, i),
                                                  Qt::EditRole, ui->searchLineEdit->text(), -1, Qt::MatchFlags(flag));

        // k 순서에 따라 indexList 에 저장된 cModel data 를 항목별로 검색한다
        foreach(auto k, indexList)
        {
            int Cid = cModel->data(k.siblingAtColumn(0)).toInt();
            QString name = cModel->data(k.siblingAtColumn(1)).toString();
            QString phoneNumber = cModel->data(k.siblingAtColumn(2)).toString();
            QString email = cModel->data(k.siblingAtColumn(3)).toString();
            QString domain = cModel->data(k.siblingAtColumn(4)).toString();
            QString address = cModel->data(k.siblingAtColumn(5)).toString();
            QString favorite = cModel->data(k.siblingAtColumn(6)).toString();
            int age = cModel->data(k.siblingAtColumn(7)).toInt();
            QString gender = cModel->data(k.siblingAtColumn(8)).toString();
            QString joinDate = cModel->data(k.siblingAtColumn(9)).toString();

            //QStringList 에 항목을 순서대로 넣는다
            QStringList stringList;
            stringList << QString::number(Cid) << name << phoneNumber << email << domain << address << favorite << QString::number(age) << gender << joinDate;

            // QList 에 항목별 정보를 QStandardItem 형식으로 넣는다
            QList<QStandardItem*> standardItem;
            for(int i = 0; i < 10; ++i)
            {
                standardItem.append(new QStandardItem(stringList.at(i)));
            }

            // sModel의 열 정보(Row) 를 갱신
            sModel->appendRow(standardItem);
            ui->searchTreeView->resizeColumnToContents(i);
        }
    }
}

// 회원 등록 시 회원 정보에서 Key 값을 0부터 하나씩 증가시켜 준다
int CustomerManage::makeCid( )
{
    if(cModel->rowCount() == 0)
    {
        return 0;
    }
    // 회원 rowCount 가 0이 아닐 때 rowCount 에서 -1 해 준 후 1씩 더해서 반환
    else
    {
        auto Cid = cModel->data(cModel->index(cModel->rowCount()-1, 0)).toInt();
        return ++Cid;
    }
}

// 우클릭 후 항목별 정보 삭제
void CustomerManage::removeItem()
{
    int r = ui->tableView->currentIndex().row();

    int Cid = cModel->record(r).value("Cid").toInt();
    QString name = cModel->record(r).value("name").toString();

    // 현재 tableView 에서 선택된 정보를 modelIndex 내에서 지운다
    QModelIndex modelIndex = ui->tableView->currentIndex();
    if(modelIndex.isValid())
    {
        cModel->removeRow(modelIndex.row());
        cModel->select();
        ui->tableView->resizeColumnsToContents();
    }

    // 회원 정보가 삭제되었음을 구매 관리 페이지에 전달하기 위한 시그널
    emit cInfoDelSignCtoP(Cid);

    // 회원 정보가 삭제되었음을 채팅 서버에 전달하기 위한 시그널
    emit cInfoDelSignCtoChat(name);

}

// tableView 에서 우클릭 시 해당 위치 정보를 알려준다
void CustomerManage::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    menu->exec(globalPos);
}

// tableView 에서 우클릭 시 해당 위치 정보에 맞는 메뉴를 출력한다
void CustomerManage::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    if(ui->tableView->indexAt(pos).isValid())
        menu->exec(globalPos);
}

// 구매 정보 등록 시 해당되는 고객 정보를 전송하는 슬롯함수
void CustomerManage::pInfoAddSlotCfromP(int Cid)
{
    // 회원 정보 중 구매 클래스에 전송할 항목 선택
    QList<QString> cInfoList;
    QSqlQuery cQuery(cModel->database());
    cQuery.prepare("SELECT name, phoneNumber, address, favorite"
                   "FROM customer WHERE Cid = ?");
    cQuery.bindValue(0, Cid);
    cQuery.exec();

    // 항목 별 인덱스를 저장한다
    QSqlRecord record = cQuery.record();
    int nameIndex = record.indexOf("name");
    int phoneNumberIndex = record.indexOf("phoneNumber");
    int addressIndex = record.indexOf("address");
    int favoriteIndex = record.indexOf("favorite");

    cQuery.next();

    // 구매 클래스로 전송할 회원 정보를 담는다
    QString name = cQuery.value(nameIndex).toString();
    QString phoneNumber = cQuery.value(phoneNumberIndex).toString();
    QString address = cQuery.value(addressIndex).toString();
    QString favorite = cQuery.value(favoriteIndex).toString();

    cInfoList << name << phoneNumber << address << favorite;

    emit pInfoAddReturnSignCtoP(cInfoList);
}
// purchaseManage 로 부터 회원 정보를 받아오기 위한 슬롯함수
//void CustomerManage::acceptCustomerInfo(int key)
//{
//     검색 시와 마찬가지로, QModelIndexList 에 모델 인덱스 및 키값이 일치하는 정보를 담는다
//    QModelIndexList indexList =
//            cModel->match(cModel->index(0, 0), Qt::EditRole, key, -1, Qt::MatchFlags(Qt::MatchCaseSensitive));

//    foreach(auto k, indexList)
//    {
//          k 순서대로 회원 정보 모델의 인덱스와 키값이 일치하는 데이터를 항목별 indexList에 저장한다
//               int Cid = cModel->data(k.siblingAtColumn(0)).toInt();
//        QString name = cModel->data(k.siblingAtColumn(1)).toString();
//        QString phoneNumber = cModel->data(k.siblingAtColumn(2)).toString();
//        QString email = cModel->data(k.siblingAtColumn(3)).toString();
//        QString domain = cModel->data(k.siblingAtColumn(4)).toString();
//        QString address = cModel->data(k.siblingAtColumn(5)).toString();
//        QString favorite = cModel->data(k.siblingAtColumn(6)).toString();
//        int age = cModel->data(k.siblingAtColumn(7)).toInt();
//        QString gender = cModel->data(k.siblingAtColumn(8)).toString();
//        QString joinDate = cModel->data(k.siblingAtColumn(9)).toString();

//         항목별 회원 정보를 order 로 보내주는 signal
//               emit sendCustomerInfo(name, phoneNumber, email, domain, address, favorite, gender, gender);

//    }
//}

// 현재 회원 정보를 ui 입력단에 표시해 주는 슬롯
void CustomerManage::on_tableView_clicked(const QModelIndex &index)
{


    // 항목별 데이터를 순서 및 자료형에 따라 인덱스로 보낸다
    int Cid = cModel->data(index.siblingAtColumn(0)).toInt();
    QString name = cModel->data(index.siblingAtColumn(1)).toString();
    QString phoneNumber = cModel->data(index.siblingAtColumn(2)).toString();
    QString email = cModel->data(index.siblingAtColumn(3)).toString();
    QString domain = cModel->data(index.siblingAtColumn(4)).toString();
    QString address = cModel->data(index.siblingAtColumn(5)).toString();
    QString favorite = cModel->data(index.siblingAtColumn(6)).toString();
    int age = cModel->data(index.siblingAtColumn(7)).toInt();
    QString gender = cModel->data(index.siblingAtColumn(8)).toString();
    QDate joinDate = cModel->data(index.siblingAtColumn(9)).toDate();

    // 각 ui 에 입력된 정보를 항목별로 저장한다
    ui->idLineEdit->setText(QString::number(Cid));
    ui->nameLineEdit->setText(name);
    ui->phoneNumberLineEdit->setText(phoneNumber);
    ui->emailLineEdit->setText(email);
    ui->domainComboBox->setCurrentText(domain);
    ui->addressLineEdit->setText(address);
    ui->favoriteComboBox->setCurrentText(domain);
    ui->ageSpinBox->setValue(age);
    ui->maleButton->setText(gender);
    ui->femaleButton->setText(gender);
    ui->dateEdit->setDate(joinDate);
}



// 소멸자, 회원 관리 ui 종료
CustomerManage::~CustomerManage()
{
    delete ui;

    // customerConnection 데이터베이스의 내용을 모두 저장한다
    QSqlDatabase database = QSqlDatabase::database("customerConnection");

    if(database.isOpen())
    {
        cModel->submitAll();
        database.commit();
        database.close();
    }
}


