#include "customersearch.h"
#include "ui_customersearch.h"
#include "customermanage.h"
#include <QStandardItemModel>
#include <QSqlDatabase>


CustomerSearch::CustomerSearch(QWidget* parent)
    : QWidget(parent), ui(new Ui::CustomerSearch)
{
    ui->setupUi(this);


    QSqlDatabase ohcDB = QSqlDatabase::database("searchCustomerConnection");

    // 검색 시 보여주는 회원 항목들을 columnView 의 헤더로 설정
    ssModel = new QStandardItemModel(0, 10);
    ssModel->setHeaderData(0, Qt::Horizontal, tr("CID"));                // 회원 Key
    ssModel->setHeaderData(1, Qt::Horizontal, tr("NAME"));               // 회원 이름
    ssModel->setHeaderData(2, Qt::Horizontal, tr("PHONE NUMBER"));       // 회원 전화 번호
    ssModel->setHeaderData(3, Qt::Horizontal, tr("EMAIL"));              // 회원 이메일 주소
    ssModel->setHeaderData(4, Qt::Horizontal, tr("DOMAIN"));             // 회원 도메인 주소
    ssModel->setHeaderData(5, Qt::Horizontal, tr("ADDRESS"));            // 회원 거주지 주소
    ssModel->setHeaderData(6, Qt::Horizontal, tr("FAVORITE"));           // 회원 선호 항목
    ssModel->setHeaderData(7, Qt::Horizontal, tr("AGE"));                // 회원 연령
    ssModel->setHeaderData(8, Qt::Horizontal, tr("GENDER"));             // 회원 성별
    ssModel->setHeaderData(9, Qt::Horizontal, tr("JOINDATE"));           // 회원 가입 날짜

    // sModel에 담겨있는 회원 정보를 columnView 로 전달
    ui->columnView->setModel(ssModel);

}

CustomerSearch::~CustomerSearch()
{
    delete ui;
}

void CustomerSearch::on_columnView_activated(const QModelIndex &index)
{
    // 검색에 사용될 모델의 내용을 비워준다
//    ssModel->clear();
//    // searchComboBox 에서 선택된 항목 (분류별 검색)의 일치 여부 판단
//    int i = ui->comboBox->currentIndex();
//    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
//                   : Qt::MatchCaseSensitive;
//    {
        // QModelIndexList 에 인덱스 정보, flag 정보와 일치하는 searchLineEdit 내용 저장
//        QModelIndexList indexList = cModel->match(cModel->index(0, i),
//                                                  Qt::EditRole, ui->searchLineEdit->text(), -1, Qt::MatchFlags(flag));

        // k 순서에 따라 indexList 에 저장된 cModel data 를 항목별로 검색한다
//        foreach(auto k, indexList)
//        {
//            int Cid = cModel->data(k.siblingAtColumn(0)).toInt();
//            QString name = cModel->data(k.siblingAtColumn(1)).toString();
//            QString phoneNumber = cModel->data(k.siblingAtColumn(2)).toString();
//            QString email = cModel->data(k.siblingAtColumn(3)).toString();
//            QString domain = cModel->data(k.siblingAtColumn(4)).toString();
//            QString address = cModel->data(k.siblingAtColumn(5)).toString();
//            QString favorite = cModel->data(k.siblingAtColumn(6)).toString();
//            int age = cModel->data(k.siblingAtColumn(7)).toInt();
//            QString gender = cModel->data(k.siblingAtColumn(8)).toString();
//            QString joinDate = cModel->data(k.siblingAtColumn(9)).toString();

            //QStringList 에 항목을 순서대로 넣는다
//            QStringList stringList;
//            stringList << QString::number(Cid) << name << phoneNumber << email << domain << address << favorite << QString::number(age) << gender << joinDate;

            // QList 에 항목별 정보를 QStandardItem 형식으로 넣는다
//            QList<QStandardItem*> standardItem;
//            for(int i = 0; i < 10; ++i)
//            {
//                standardItem.append(new QStandardItem(stringList.at(i)));
//            }

//            // sModel의 열 정보(Row) 를 갱신
//            ssModel->appendRow(standardItem);
//            ui->columnView->resizeGripsVisible();
//        }
//    }
}

