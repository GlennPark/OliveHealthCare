#ifndef CUSTOMERLIST_H
#define CUSTOMERLIST_H

#include<QTreeWidgetItem>

class CustomerList : public QTreeWidgetItem
{
public:
    explicit CustomerList(int Cid = 0, QString = "", QString = "", QString = "", QString = "", QString ="", QString ="" , QString = "",QString = "");

    QString getName() const;
    void setName(QString&);
    QString getPhoneNumber() const;
    void setPhoneNumber(QString&);
    QString getEmail() const;
    void setEmail(QString&);
    QString getDomain() const;
    void setDomain(QString&);
    QString getAddress() const;
    void setAddress(QString&);
    QString getFavorite() const;
    void setFavorite(QString&);
    QString getAge() const;
    void setAge(QString&);
    QString getGender() const;
    void setGender(QString&);

    int Cid() const;
    bool operator==(const CustomerList &other) const;

};

#endif // CUSTOMERLIST_H
