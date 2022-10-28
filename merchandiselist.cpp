#include "merchandiselist.h"

MerchandiseList::MerchandiseList(int id, QString name, int price, int quantity)
{
    setText(0, QString::number(id));
    setText(1, name);
    setText(2, QString::number(price));
    setText(3, QString::number(quantity));
}
QString MerchandiseList::getName() const
{
    return text(1);
}

void MerchandiseList::setName(QString& name)
{
    setText(1, name);
}

int MerchandiseList::getPrice() const
{
    return text(2).toInt();
}

void MerchandiseList::setPrice(int& price)
{
    setText(2, QString::number(price));
}

int MerchandiseList::getQuantity() const
{
    return text(3).toInt();
}

void MerchandiseList::setQuantity(int& quantity)
{
    setText(3, QString::number(quantity));
}

int MerchandiseList::id() const
{
    return text(0).toInt();
}

// Define copy assignment operator.
bool MerchandiseList::operator==(const MerchandiseList &other) const
{
    return (this->text(1) == other.text(1));
}
