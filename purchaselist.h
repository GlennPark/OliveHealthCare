#ifndef PURCHASELIST_H
#define PURCHASELIST_H

#include <QTreeWidgetItem>


class PurchaseList : public QTreeWidgetItem
{
public:
    explicit PurchaseList(int id = 0, int = 0, QString = "",
                           int = 0, QString = "",
                           int = 0, int = 0, int = 0);


    int getCid() const;
        void setCid(int&);
        int getPid() const;
        void setPid(int&);
        int getPrice() const;
        void setPrice(int&);
        int getQuantity() const;
        void setQuantity(int&);
        int getTotalPrice() const;
        void setTotalPrice(int&);
        int id() const;
        bool operator==(const PurchaseList &other) const;

    private:
        int cid, pid;

};

#endif // PURCHASELIST_H
