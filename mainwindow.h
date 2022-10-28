#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CustomerManage;
class MerchandiseManage;
class PurchaseManage;
class ChattingServer;
class ChattingClient;
class CustomerSearch;
class MerchandiseSearch;
class PurchaseSearch;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionCustomer_triggered();
    void on_actionMerchandise_triggered();
    void on_actionPurchase_triggered();
    void on_actionChatting_triggered();
   void on_customerChatAction_triggered();
    void on_customerSearchAction_triggered();



private:
    Ui::MainWindow *ui;
    CustomerManage *customerManage;
    MerchandiseManage *merchandiseManage;
    PurchaseManage* purchaseManage;
    ChattingServer* chattingServer;
    ChattingClient* chattingClient;
    CustomerSearch* customerSearch;
    MerchandiseSearch* merchandiseSearch;
    PurchaseSearch* purchaseSearch;
};
#endif // MAINWINDOW_H
