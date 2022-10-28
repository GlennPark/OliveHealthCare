#include "purchaselist.h"

PurchaseList::PurchaseList(int id, int cid, QString cname,
                           int pid, QString pname,
                           int price, int quantity, int totalPrice)
    : cid(cid), pid(pid)
{
    setText(0, QString::number(id));
    setText(1, cname);
    setText(2, pname);
    setText(3, QString::number(price));
    setText(4, QString::number(quantity));
    setText(5, QString::number(totalPrice));
}

int PurchaseList::getCid() const
{
    return cid;     // text(1).toInt();
}

void PurchaseList::setCid(int& id)
{
    cid = id;
    setText(1, QString::number(id));
}

int PurchaseList::getPid() const
{
    return pid;     // text(2).toInt();
}

void PurchaseList::setPid(int& id)
{
    pid = id;
    setText(2, QString::number(id));
}

int PurchaseList::getPrice() const
{
    return text(3).toInt();
}

void PurchaseList::setPrice(int& price)
{
    setText(3, QString::number(price));
}

int PurchaseList::getQuantity() const
{
    return text(4).toInt();
}

void PurchaseList::setQuantity(int& quantity)
{
    setText(4, QString::number(quantity));
}

int PurchaseList::getTotalPrice() const
{
    return text(5).toInt();
}

void PurchaseList::setTotalPrice(int& quantity)
{
    setText(5, QString::number(quantity));
}

int PurchaseList::id() const
{
    return text(0).toInt();
}

// Define copy assignment operator.
bool PurchaseList::operator==(const PurchaseList &other) const {
    return (this->text(1) == other.text(1));
}
