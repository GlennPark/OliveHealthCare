#ifndef CUSTOMERLIST_H
#define CUSTOMERLIST_H

#include<QTreeWidgetItem>

#include <QObject>

//class CustomerList : public QObject
//{
//    Q_OBJECT

//public:
//    explicit CustomerList(QObject *parent = nullptr);

//    CustomerList(int &cid, QString &name, QString &phoneNumber, QString &email, QString &domain, QString &address,
//                 QString &favorite, QString &age, QString &gender, QString &joinDate, QObject *parent = nullptr);

//    int cid() const;
//    void setCid(int cid);

//    QString name();
//    void setName(QString name);

//    QString phoneNumber();
//    void setPhoneNumber(QString phoneNumber);

//    QString email();
//    void setEmail(QString email);

//    QString domain();
//    void setDomain(QString domain);

//    QString address();
//    void setAddress(QString address);

//    QString favorite();
//    void setFavorite(QString favorite);

//    QString age();
//    void setAge(QString age);

//    QString gender();
//    void setGender(QString gender);

//    QString joinDate();
//    void setJoinDate(QString gender);


//signals:
//    void cidConverted(int cid);
//    void nameConverted(QString name);
//    void phoneNumberConverted(QString phoneNumber);
//    void emailConverted(QString email);
//    void domainConverted(QString domain);
//    void addressConverted(QString address);
//    void favoriteConverted(QString favorite);
//    void ageConverted(QString age);
//    void genderConverted(QString gender);
//    void joinDateConverted(QString joinDate);

//private:
//    int m_cid;
//    QString m_name;
//    QString m_phoneNumber;
//    QString m_email;
//    QString m_domain;
//    QString m_address;
//    QString m_favorite;
//    QString m_age;
//    QString m_gender;
//    QString m_joinDate;
//};




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
