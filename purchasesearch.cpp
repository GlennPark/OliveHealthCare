#include "purchasesearch.h"
#include "ui_purchasesearch.h"

PurchaseSearch::PurchaseSearch(QWidget *parent)
    : QWidget(parent), ui(new Ui::PurchaseSearch)
{
  ui->setupUi(this);
  ui->columnView;
}

PurchaseSearch::~PurchaseSearch()
{
    delete ui;
}
