#ifndef MERCHANDISELIST_H
#define MERCHANDISELIST_H

#include <QTreeWidgetItem>

class MerchandiseList : public QTreeWidgetItem
{
public:
    explicit MerchandiseList(int id = 0, QString = "", int = 0, int = 0);
    QString getName() const;
    void setName(QString&);
    int getPrice() const;
    void setPrice(int&);
    int getQuantity() const;
    void setQuantity(int&);
    int id() const;
    bool operator==(const MerchandiseList &other) const;
};

#endif // MERCHANDISELIST_H
