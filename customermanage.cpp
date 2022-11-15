#include "customermanage.h"
#include "QtCore/qdatetime.h"
#include "ui_customermanage.h"
#include "customerlist.h"

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
    ui->setupUi(this);

    // 우클릭 후 삭제 시 ROW 에 포함된 내용들을 지우는 액션
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

    // 검색 시 보여주는 항목들을 seachTreeView 의 헤더로 설정
    sModel = new QStandardItemModel(0, 10);
    sModel->setHeaderData(0, Qt::Horizontal, tr("CID"));                // 회원 Key
    sModel->setHeaderData(1, Qt::Horizontal, tr("NAME"));               // 회원 이름
    sModel->setHeaderData(2, Qt::Horizontal, tr("PHONE NUMBER"));       // 전화 번호
    sModel->setHeaderData(3, Qt::Horizontal, tr("EMAIL"));              // 이메일 주소
    sModel->setHeaderData(4, Qt::Horizontal, tr("DOMAIN"));             // 도메인 주소
    sModel->setHeaderData(5, Qt::Horizontal, tr("ADDRESS"));            // 거주지 주소
    sModel->setHeaderData(6, Qt::Horizontal, tr("FAVORITE"));           // 선호 항목
    sModel->setHeaderData(7, Qt::Horizontal, tr("AGE"));                // 연령
    sModel->setHeaderData(8, Qt::Horizontal, tr("GENDER"));             // 성별
    sModel->setHeaderData(9, Qt::Horizontal, tr("JOINDATE"));           // 가입 날짜

    // sModel에 담겨있는 정보를 searchTreeView 로 전달
    ui->searchTreeView->setModel(sModel);

}

//회원 관리 페이지의 데이터베이스
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
                    "(cid INTEGER PRIMARY KEY, "
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

        // 테이블에서 사용하는 항목들의 머릿말 목록
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

    // rowCount 순서에 따라 항목별 자료형과 데이터를 인덱스에 저장
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

        // purchaseManager 에서 사용될 Cid 값을 시그널 신호로 전달
        emit addedCustomer(Cid);
    }

}

// 회원 등록 버튼 클릭시 기능
void CustomerManage::on_addPushButton_clicked()
{
    // 회원별 Key 값을 생성 및 부여, 항목별로 전달될 자료형을 선정한다
    int Cid = makeCid();
    QString name, phoneNumber, email, domain, address, favorite, gender, joinDate;
    int age;
    // idLineEdit ui 에는 부여된 키값을 반환한다
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
    // ageSpinBox ui 에 입력된 텍스트를 age 항목으로 반환
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
    // dateEdit ui 에 입력된 텍스트를 joinDate 항목으로 반환
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
        // purchaseManager 에서 사용될 Cid 값을 시그널 신호로 전달
        emit addedCustomer(Cid);
    }
}

// 수정 버튼 클릭 시 기능
void CustomerManage::on_modifyPushButton_clicked()
{
    // 현재 tableView에 입력된 정보를 QModelIndex에 반환
    QModelIndex modelIndex = ui->tableView->currentIndex();
    //
    if(modelIndex.isValid())
    {
        QString name, phoneNumber, email, domain, address, favorite, gender, joinDate;
        int age;

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
#if 1
        //        cModel->setData(modelIndex.siblingAtColumn(0), Cid);
        cModel->setData(modelIndex.siblingAtColumn(1), name);
        cModel->setData(modelIndex.siblingAtColumn(2), phoneNumber);
        cModel->setData(modelIndex.siblingAtColumn(3), email);
        cModel->setData(modelIndex.siblingAtColumn(4), domain);
        cModel->setData(modelIndex.siblingAtColumn(5), address);
        cModel->setData(modelIndex.siblingAtColumn(6), favorite);
        cModel->setData(modelIndex.siblingAtColumn(7), age);
        cModel->setData(modelIndex.siblingAtColumn(8), gender);
        cModel->setData(modelIndex.siblingAtColumn(9), joinDate);
        cModel->submit();

#else
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
#endif
        cModel->select();
        ui->tableView->resizeColumnsToContents();

    }
}

void CustomerManage::on_searchPushButton_clicked()
{
    sModel->clear();
    //    for(int i = 0; i < ui->tableView->columnCount(); i++)
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    {
        QModelIndexList indexList = cModel->match(cModel->index(0, i),
                                                  Qt::EditRole, ui->searchLineEdit->text(), -1, Qt::MatchFlags(flag));

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
            QStringList stringList;
            stringList << QString::number(Cid) << name << phoneNumber << email << domain << address << favorite << QString::number(age) << gender << joinDate;
            QList<QStandardItem*> standardItem;
            for(int i = 0; i < 10; ++i)
            {
                standardItem.append(new QStandardItem(stringList.at(i)));
            }
            sModel->appendRow(standardItem);
            ui->searchTreeView->resizeColumnToContents(i);
        }
    }
}


int CustomerManage::makeCid( )
{
    if(cModel->rowCount() == 0)
    {
        return 0;
    }
    else
    {
        auto Cid = cModel->data(cModel->index(cModel->rowCount()-1, 0)).toInt();
        return ++Cid;
    }
}


void CustomerManage::removeItem()
{
    QModelIndex modelIndex = ui->tableView->currentIndex();
    if(modelIndex.isValid())
    {
        cModel->removeRow(modelIndex.row());
        cModel->select();
        ui->tableView->resizeColumnsToContents();
    }
}

void CustomerManage::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    menu->exec(globalPos);
}

void CustomerManage::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    if(ui->tableView->indexAt(pos).isValid())
        menu->exec(globalPos);
}

void CustomerManage::acceptCustomerInfo(int key)
{
    qDebug()<<"client accpet"<<key;
    QModelIndexList indexList =
            cModel->match(cModel->index(0, 0), Qt::EditRole, key, -1, Qt::MatchFlags(Qt::MatchCaseSensitive));
    qDebug()<<indexList;
    foreach(auto k, indexList)
    {
        //       int Cid = cModel->data(k.siblingAtColumn(0)).toInt();
        QString name = cModel->data(k.siblingAtColumn(1)).toString();
        QString phoneNumber = cModel->data(k.siblingAtColumn(2)).toString();
        QString email = cModel->data(k.siblingAtColumn(3)).toString();
        QString domain = cModel->data(k.siblingAtColumn(4)).toString();
        QString address = cModel->data(k.siblingAtColumn(5)).toString();
        QString favorite = cModel->data(k.siblingAtColumn(6)).toString();
        int age = cModel->data(k.siblingAtColumn(7)).toInt();
        QString gender = cModel->data(k.siblingAtColumn(8)).toString();
        QString joinDate = cModel->data(k.siblingAtColumn(9)).toString();
        qDebug()<<name;
        emit sendCustomerInfo(name, phoneNumber, email, domain, address, favorite, gender, gender);

    }
}

void CustomerManage::on_tableView_clicked(const QModelIndex &index)
{
    QString Cid = cModel->data(index.siblingAtColumn(0)).toString();
    QString name = cModel->data(index.siblingAtColumn(1)).toString();
    QString phoneNumber = cModel->data(index.siblingAtColumn(2)).toString();
    QString email = cModel->data(index.siblingAtColumn(3)).toString();
    QString domain = cModel->data(index.siblingAtColumn(4)).toString();
    QString address = cModel->data(index.siblingAtColumn(5)).toString();
    QString favorite = cModel->data(index.siblingAtColumn(6)).toString();
    int age = cModel->data(index.siblingAtColumn(7)).toInt();
    QString gender = cModel->data(index.siblingAtColumn(8)).toString();
    QDate joinDate = cModel->data(index.siblingAtColumn(9)).toDate();

    ui->idLineEdit->setText(Cid);
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
    //    if(ui->maleButton->isChecked())
    //    {
    //        ui->maleButton->setText(gender);
    //    }
    //    else
    //    {
    //        ui->femaleButton->setText(gender);
    //    }

}

CustomerManage::~CustomerManage()
{
    delete ui;

    QSqlDatabase database = QSqlDatabase::database("customerConnection");

    if(database.isOpen())
    {
        cModel->submitAll();
        database.commit();
        database.close();
    }
}


