#include "customermanage.h"
#include "QtCore/qdatetime.h"
#include "ui_customermanage.h"
#include "customerlist.h"

#include <QFile>
#include <QMenu>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


CustomerManage::CustomerManage(QWidget* parent)
    : QWidget(parent), ui(new Ui::CustomerManage)
{
    ui->setupUi(this);


    //    QList<int> size;
    //    size << 600 << 600;
    //    ui->splitter->setSizes(size);


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

}

void CustomerManage::dataSave()
{

    // DB 셋업
    QSqlDatabase ohcDB = QSqlDatabase::addDatabase("QSQLITE","customerConnection");
    ohcDB.setDatabaseName("customerDatabase.db");

    // 예외처리
    if(ohcDB.open())
    {
        QSqlQuery cQuery(ohcDB);
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
                    "joinDate DATETIME);");

        //       cModel = new QSqlTableModel();

        cModel = new QSqlTableModel(this, ohcDB);
        cModel->setTable("customer");
        cModel->select();

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
        ui->tableView->setModel(cModel);
        ui->tableView->resizeColumnsToContents();
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
            QDateTime joinDate = cModel->data(cModel->index(i, 9)).toDateTime();

            emit addedCustomer(Cid);
        }

}

//고객 등록
void CustomerManage::on_addPushButton_clicked()
{



    int Cid = makeCid();
    QString name, phoneNumber, email, domain, address, favorite, gender, joinDate;
    int age;


    ui->idLineEdit->setText(QString::number(Cid));
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

    joinDate = ui->dateEdit->dateTime().toString();

    QSqlDatabase ohcDB = QSqlDatabase::database("customerConnection");
    if(ohcDB.isOpen() && name.length())
    {
        QSqlQuery cQuery(cModel->database());
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
        cModel->select();
        ui->tableView->resizeColumnsToContents();
        emit addedCustomer(Cid);
    }
}


void CustomerManage::on_searchPushButton_clicked()
{
    ui->searchTreeWidget->clear();
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
            stringList << QString::number(Cid) << name << phoneNumber << email << domain << address << favorite << QString::number(age) << gender<< joinDate;
            new QTreeWidgetItem(ui->searchTreeWidget, stringList);
            for(int i = 0; i < ui->searchTreeWidget->columnCount(); i++)
            ui->searchTreeWidget->resizeColumnToContents(i);
        }
    }
}


int CustomerManage::makeCid( )
{
    if(cModel->rowCount() == 0)
    {
        return 100;
    }
    else
    {
        auto Cid = cModel->data(cModel->index(cModel->rowCount()-1, 0)).toInt();
        return ++Cid;
    }
}


CustomerManage::~CustomerManage()
{
    delete ui;

    QSqlDatabase database = QSqlDatabase::database("customerConnection");

    QFile file("customer.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const auto& v : customerList) {
        CustomerList* c = v;
        out << c->Cid() << ", " << c->getName() << ", ";
        out << c->getPhoneNumber() << ", ";
        out << c->getEmail() << ", ";
        out << c->getDomain() << ", ";
        out << c->getAddress() << ", ";
        out << c->getFavorite() << ", ";
        out << c->getAge() << ", ";
        out << c->getGender() << "\n";
    }
    file.close( );
}



void CustomerManage::removeItem()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        customerList.remove(item->text(0).toInt());
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
        //        delete item;
        ui->treeWidget->update();
    }
}

void CustomerManage::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->treeWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}


void CustomerManage::on_modifyPushButton_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        int key = item->text(0).toInt();
        CustomerList* c = customerList[key];
        QString name, number, email, domain, address, favorite, age, gender;
        name = ui->nameLineEdit->text();
        number = ui->phoneNumberLineEdit->text();
        email = ui->emailLineEdit->text();
        domain = ui->domainComboBox->currentText();
        address = ui->addressLineEdit->text();
        favorite = ui->favoriteComboBox->currentText();
        age = ui->ageSpinBox->text();
        //        gender = ui->maleButton->text();


        if(ui->maleButton->isChecked())
        {
            gender = ui->maleButton->text();
        }
        else
        {
            gender = ui->femaleButton->text();
        }



        c->setName(name);
        c->setPhoneNumber(number);
        c->setEmail(email);
        c->setDomain(domain);
        c->setAddress(address);
        c->setFavorite(favorite);
        c->setAge(age);
        c->setGender(gender);

        customerList[key] = c;
    }
}


void CustomerManage::acceptCustomerInfo(int key)
{

    QModelIndexList indexList =
            cModel->match(cModel->index(0, 0), Qt::EditRole, key, -1, Qt::MatchFlags(Qt::MatchCaseSensitive));
    CustomerList* c = customerList[key];
    emit sendCustomerInfo(c->getName(), c->getPhoneNumber(), c-> getEmail(), c->getDomain(), c->getAddress(), c->getFavorite(), c->getAge(), c->getGender());
}




void CustomerManage::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    ui->idLineEdit->setText(item->text(0));
    ui->nameLineEdit->setText(item->text(1));
    ui->phoneNumberLineEdit->setText(item->text(2));
    ui->emailLineEdit->setText(item->text(3));
    ui->domainComboBox->currentText();
    ui->addressLineEdit->setText(item->text(5));
    ui->favoriteComboBox->currentText();
    ui->ageSpinBox->text();
    if(ui->maleButton->isChecked())
    {
        ui->maleButton->text();
    }
    else
    {
        ui->femaleButton->text();
    }
}



