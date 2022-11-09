#include "customerlist.h"
#include <QDebug>

//CustomerList::CustomerList(QObject *parent):QObject(parent)
//{

//}

//CustomerList::CustomerList(int &cid, QString &name, QString &phoneNumber, QString &email, QString &domain, QString &address,
//                           QString &favorite, QString &age, QString &gender, QString &joinDate, QObject * parent):QObject(parent),
//    m_cid(cid), m_name(name), m_phoneNumber(phoneNumber), m_email(email), m_domain(domain), m_address(address), m_favorite(favorite), m_age(age), m_gender(gender), m_joinDate(joinDate)
//{

//}

//int CustomerList::cid() const
//{
//    return m_cid;
//}

//void CustomerList::setCid(int cid)
//{
//    if(m_cid == cid)
//        return;
//    m_cid = cid;
//    emit cidConverted(m_cid);
//}

//QString CustomerList::name()
//{
//    return m_name;
//}

//void CustomerList::setName(QString name)
//{
//    if(m_name == name)
//        return;
//    m_name = name;
//    emit nameConverted(m_name);
//}

//QString CustomerList::phoneNumber()
//{
//    return m_phoneNumber;
//}

//void CustomerList::setPhoneNumber(QString phoneNumber)
//{
//    if(m_phoneNumber == phoneNumber)
//        return;
//    m_phoneNumber = phoneNumber;
//    emit phoneNumberConverted(m_phoneNumber);
//}

//QString CustomerList::email()
//{
//    return m_email;
//}

//void CustomerList::setEmail(QString email)
//{
//    if(m_email == email)
//        return;
//    m_email = email;
//    emit emailConverted(m_email);
//}

//QString CustomerList::domain()
//{
//    return m_domain;
//}

//void CustomerList::setDomain(QString domain)
//{
//    if(m_domain == domain)
//        return;
//    m_domain = domain;
//    emit domainConverted(m_domain);
//}

//QString CustomerList::address()
//{
//    return m_address;
//}

//void CustomerList::setDomain(QString domain)
//{
//    if(m_domain == domain)
//        return;
//    m_domain = domain;
//    emit domainConverted(m_domain);
//}

//QString CustomerList::favorite()
//{
//    return m_favorite;
//}

//void CustomerList::setDomain(QString domain)
//{
//    if(m_domain == domain)
//        return;
//    m_domain = domain;
//    emit domainConverted(m_domain);
//}

//QString CustomerList::age()
//{
//    return m_age;
//}

//QString CustomerList::gender()
//{
//    return m_gender;
//}

//void CustomerList::setDomain(QString domain)
//{
//    if(m_domain == domain)
//        return;
//    m_domain = domain;
//    emit domainConverted(m_domain);
//}

//QString CustomerList::joinDate()
//{
//    return m_joinDate;
//}

//void CustomerList::setDomain(QString domain)
//{
//    if(m_domain == domain)
//        return;
//    m_domain = domain;
//    emit domainConverted(m_domain);
//}

CustomerList::CustomerList(int Cid, QString name, QString phoneNumber, QString email, QString domain, QString address, QString favorite, QString age, QString gender)
{
    setText(0, QString::number(Cid));
    setText(1, name);
    setText(2, phoneNumber);
    setText(3, email);
    setText(4, domain);
    setText(5, address);
    setText(6, favorite);
    setText(7, age);
    setText(8, gender);
}

QString CustomerList::getName() const
{
    return text(1);
}

void CustomerList::setName(QString& name)
{
    setText(1, name);
}

QString CustomerList::getPhoneNumber() const
{
    return text(2);
}

void CustomerList::setPhoneNumber(QString& phoneNumber)
{
    setText(2, phoneNumber);    // c_str() --> const char*
}

QString CustomerList::getEmail() const
{
    return text(3);
}

void CustomerList::setEmail(QString& email)
{
    setText(3, email);
}

QString CustomerList::getDomain() const
{
    return text(4);
}

void CustomerList::setDomain(QString& domain)
{
    setText(4, domain);
}

QString CustomerList::getAddress() const
{
    return text(5);
}

void CustomerList::setAddress(QString& address)
{
    setText(5, address);
}

QString CustomerList::getFavorite() const
{
    return text(6);
}

void CustomerList::setFavorite(QString& favorite)
{
    setText(6, favorite);
}

QString CustomerList::getAge() const
{
   return text(7);
}

void CustomerList::setAge(QString& age)
{
    setText(7, age);
}


QString CustomerList::getGender() const
{
    return text(8);
}

void CustomerList::setGender(QString& gender)
{
    setText(8, gender);
}

int CustomerList::Cid() const
{
    return text(0).toInt();
}

// Define copy assignment operator.
bool CustomerList::operator==(const CustomerList &other) const
{
    return (this->text(1) == other.text(1));
}
